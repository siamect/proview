/* rt_bck.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996,98 by Mandator AB.

   This module contains the basic internal routines and
   processing done by the backup class.  */

#ifdef OS_ELN
# include $vaxelnc
# include stdio
# include stdlib
# include descrip
# include string
# include errno
#endif

#ifdef OS_VMS
# include <stdio.h>
# include <stdlib.h>
# include <descrip.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <lib$routines.h>
# include <starlet.h>
# include <unixio.h>
#endif

#if defined OS_LYNX || defined OS_LINUX
# ifdef seekbug
#   error "seekbug not ready for Lynx"
# endif
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <unistd.h>
#endif

#ifdef OS_VMS
# define _PTHREAD_USE_D4 
#endif

#include <pthread.h>

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_bckdef.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "rt_pwr_msg.h"
#include "rt_aproc.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_ini_event.h"

#define	check4a(sts,str) if((sts)==-1)perror(str)

#if defined OS_VMS || defined OS_ELN
# define A_MODE 	, "shr=get"
# define FGETNAME	fgetname(bckfile, (char *)&tmpstr)
# define LOCK 		pthread_lock_global_np ()
# define UNLOCK 	pthread_unlock_global_np ()

#elif defined OS_LYNX || defined OS_LINUX 
# define A_MODE
# define FGETNAME	backup_confp->BackupFile
# define LOCK
# define UNLOCK

#else
# define A_MODE
#endif

#ifdef OS_ELN
/* Pthreads debugger workaround from VAXELN V4.4 release notes.  */

void
pdebug ()
{
  cma_debug();
} 
#endif

#if defined (OS_VMS) && !defined(__ALPHA)
  extern void *lib$vm_malloc(size_t size);
  extern void lib$vm_free(void *ptr);
#endif

/* Interface between the collector and the file writer  */

/* An entry in the wrtblkque is a malloced piece of memory
   starting with a cycle header, followed by a number of segments.  */

typedef struct {
  BCK_CYCLEHEAD_STRUCT cyclehead;
  struct {
    BCK_DATAHEAD_STRUCT datahead;
    char data [1];
  } segment [1];
} BCK_WRTBLK_STRUCT;

pthread_mutex_t		wrtblkmtx;	/* One mutex for both queues */
BCK_WRTBLK_STRUCT	*wrtblkque [2];	/* A queue with only one slot */
pthread_cond_t		wrtblkevt;	/* There is work to do */

pthread_mutex_t		frcactmtx;	/* Forced activation mutex */
pthread_cond_t		frcactevt;	/* Forced activation cond var */
pwr_tBoolean		frcact [2];	/* Forced activation */

static FILE		*bckfile;	/* File variable */
static BCK_FILEHEAD_STRUCT
			filehead;	/* Memory copy of the file header */

static pwr_tUInt32	slowtick;
static pwr_sClass_Backup_Conf
			*backup_confp;	/* Backup_Conf object pointer */



#ifdef seekbug

# include $dap
# include $file_utility
# include $kernelmsg

# define POS_TO_VBN(pos) ((((unsigned int)(pos))+511)>>9)+1
# define VBN_TO_POS(vbn) (((vbn)-1)<<9)
# define RFA_TO_VBN(rfa) ((rfa).dap$l_rfa0)
# define NEWVBN(rfa,len) if ((curvbn = RFA_TO_VBN(rfa)+POS_TO_VBN(len)-1) > endvbn) endvbn = curvbn
# define CONSISTENCY_CHECK(rfa,vbn)		\
	if ((rfa).dap$l_rfa0 != (vbn)) {	\
	  int sts = KER$_BUGCHECK;		\
	  c$$translate (sts);			\
	  lib$signal (sts);			\
	  return sts; }

# define RFA_TO_POS(rfa)	((((rfa).dap$l_rfa0-1)<<9) + ((rfa).dap$w_rfa4 & 511))
# define POS_TO_RFA(pos,rfa)	\
	{ (rfa).dap$l_rfa0=(((unsigned int)(pos))>>9)+1;	\
	  (rfa).dap$w_rfa4=(pos) & 511; }
# define NEWPOS(rfa,len) if ((curpos = RFA_TO_POS(rfa)+(len)) > endpos) endpos = curpos

  static int dapchan;
  static FILE$ATTRIBUTES_RECORD attr;
  static int curvbn;
  static int endvbn;
  static int maxbuf = 512;

  int dapopen (
	char *filespec,
        pwr_tBoolean create)
  {
    int sts;
    VARYING_STRING (255) filestring;
    int mrs = maxbuf;
    DAP$B_RAT rat = 0;
    DAP$B_RFM rfm = DAP$K_STMLF;
    DAP$L_DEV devchar;
    DAP$Q_DEV devdep;
    static DAP$T_RHB rhb;
    FILE$ATTRIBUTES_RECORD *attrp = &attr;
    DAP$R_RFA rfa;

    CSTRING_TO_VARYING(filespec, filestring);
    sts = dap$open(&dapchan,
	create,					/* create */
	&filestring, 				/* filespec */
	DAP$M_GET|DAP$M_PUT|DAP$M_UPD|DAP$M_BRO,/* fac */
	DAP$M_UPI|DAP$M_SHRGET,			/* shr */
	0,					/* fop */
	0,					/* rop */
	&devchar,				/* device_characteristics */
	&devdep,				/* device_dependent_char */
	&mrs,					/* mrs */
	&rat,					/* rat */
	&rfm,					/* rfm */
	0,					/* deq */
	0,					/* file_size */
	maxbuf,					/* bufsize */
	0,					/* fsz */
	&rhb,					/* rhb */
	FALSE,					/* textfile */
	0,					/* owner */
	&0xffff,				/* pro */
	&attrp,					/* file_attributes */
	0,					/* message_ptr */
	0,					/* message_id */
	0);					/* default_filespec */

    if (EVEN(sts)) c$$translate(sts);	/* set errno */
    else {
      endvbn = attr.end_of_file_block + 1;
    }

    curvbn = 1;

    return sts;
  }

  int dapclose ()
  {
    int sts;

   sts = dap$close(
	&dapchan,
	0,					/* fop */
	0,					/* owner */
	&0xffff,				/* protection */
	0,					/* message_ptr */
	0,					/* message_id */
	0);					/* revision_date */
    if (EVEN(sts)) c$$translate(sts);	/* set errno */
    return sts;
  }

  char *dapgetname (
	char *namebuf)
  {
    VARYING_TO_CSTRING(attr.resultant_filename, namebuf);
    return namebuf;
  }

  int dapflush ()
  {
    int sts;

    sts = dap$flush(dapchan);
    if (EVEN(sts)) c$$translate(sts);	/* set errno */
    return sts;
  }

  int dapwrite (
	int filepos,
	char *buf,
	int buflen)
  {
    int sts = 1;
    DAP$R_RFA rfa;
    static char padbuf [512] = " ";
    char voidbuf [512];
    int voidlen;
    int thislen;
    int filevbn;

    filevbn = POS_TO_VBN(filepos);

    /* Should we extend the file ? */

    if (filevbn > endvbn) { /* extend file */
      if (padbuf[0] != '*') memset(padbuf, '*', sizeof(padbuf));
      while (filevbn > curvbn) {
        sts = dapwrite(
		VBN_TO_POS(endvbn),
		padbuf,
		sizeof(padbuf));
        if (EVEN(sts)) return sts;
      }
    } /* file extended */


    while (buflen > 0) {
      thislen = MIN(buflen, maxbuf);

      /* Do we need to reposition the file? */

      if (filevbn != curvbn) {
        if (filevbn == 1) {	/* First block needs special treatment */
          sts = dap$rewind(dapchan);
	  if (EVEN(sts)) {
	    c$$translate(sts);	/* set errno */
	    return sts;
	  }
	  curvbn = 1;
        } /* first block */
        else {	/* filevbn not first vbn in file */
          sts = dap$get( /* position file at block before requested block */
		dapchan,
		DAP$K_BLK_VBN,				/* rac */
		filevbn-1,				/* record_number */
		0,					/* rop */
		voidbuf,				/* buffer */
		sizeof(voidbuf),			/* buffer length */
		&voidlen,				/* record length */
		&rfa,					/* rfa */
		0,					/* message_ptr */
		0);					/* message_id */

	  if (EVEN(sts)) {
	    c$$translate(sts);	/* set errno */
	    return sts;
	  }
	  CONSISTENCY_CHECK(rfa, filevbn-1);
	  NEWVBN(rfa, voidlen);
        } /* Not first block */
      } /* repositioning done */

      /* The file is at the right place. Just $PUT data onto it */

      sts = dap$put(
		dapchan,
		DAP$K_BLK_VBN,				/* rac */
		filevbn,				/* record number */
		DAP$M_UIF,				/* rop */
		buf,					/* buffer */
		thislen,				/* buffer length */
		&rfa,					/* rfa, output */
		0,					/* message_ptr */
		0);					/* message_id */
      if (EVEN(sts)) {
        c$$translate(sts);	/* set errno */
        return sts;
      }
      CONSISTENCY_CHECK(rfa, filevbn);
      NEWVBN(rfa, thislen);

      buf += maxbuf;
      buflen -= maxbuf;
      filevbn += 1;
    } /* while more to write */
    return sts;
  }


  int dapread (
	int filepos,
	char *buf,
	int buflen)
  {
    int sts;
    DAP$R_RFA rfa;
    int thislen;
    int filevbn;
    int retlen;

    filevbn = POS_TO_VBN(filepos);

    while (buflen > 0) {
      thislen = MIN(buflen, maxbuf);
      sts = dap$get(
		dapchan,
		DAP$K_BLK_VBN,				/* rac */
		filevbn,				/* record_number */
		0,					/* rop */
		buf,					/* buffer */
		thislen,				/* buffer length */
		&retlen,				/* record length */
		&rfa,					/* rfa */
		0,					/* message_ptr */
		0);					/* message_id */
      if (EVEN(sts)) {
        c$$translate(sts);	/* set errno */
        return sts;
      }
      CONSISTENCY_CHECK(rfa, filevbn);
      NEWVBN(rfa, retlen);
      buf += retlen;
      buflen -= retlen;
    }
    return sts;
  }

#endif /* seekbug */


/************************************************************************
*
* Name: bck_wrtblk_insert
*
* Type:	void
*
* Type		Parameter	IOGF	Description
* void *	wrtblk		I	wrtblk to insert
* pwr_tBoolean	replace		I	Decides if existing wrtblk
*					should be replaced or not.
* Description:	
*	The routine handles insertion in the wrtblkque.
*	The wrtblk is assumed to be a dynamically allocated (malloc'ed)
*	structure which eventually is free'd after this call.
*************************************************************************/

void
bck_wrtblk_insert (
  BCK_WRTBLK_STRUCT *wrtblk,
  pwr_tBoolean replace
)
{
  int c;
  int sts4a;

  c = wrtblk->cyclehead.cycle;

#ifdef BCK_TRACE
  printf("bck_wrtblk_insert %d\n", c);
#endif

  sts4a = pthread_mutex_lock(&wrtblkmtx);
  check4a(sts4a,"pthread_mutex_lock(&wrtblkmtx)");

  if (replace) {
    if (wrtblkque [c] != NULL) {
#ifdef OS_ELN
      int sts;
      ker$free_memory(&sts, wrtblkque[c]->cyclehead.length, wrtblkque [c]);
#else
      free(wrtblkque [c]);
#endif
    }
    wrtblkque [c] = wrtblk;
  }
  else {
    if (wrtblkque [c] != NULL) {
#ifdef OS_ELN
      int sts;
      ker$free_memory(&sts, wrtblk->cyclehead.length, wrtblk);
#else
      free(wrtblk);
#endif
    }
    else wrtblkque [c] = wrtblk;
  }

  sts4a = pthread_cond_broadcast(&wrtblkevt);
  check4a(sts4a,"pthread_cond_broadcast error");

  sts4a = pthread_mutex_unlock(&wrtblkmtx);
  check4a(sts4a,"pthread_mutex_unlock(&wrtblkmtx)");
}

/************************************************************************
*
* Name: bck_wrtblk_wait
*
* Type:	BCK_WRTBLK_STRUCT *
*
* Type		Parameter	IOGF	Description
*
* Description:	
*	The routine waits for a nonempty wrtblkque and removes the entry.
*	The wrtblk is a dynamically allocated (malloc'ed)
*	structure which eventually must be free'd.
*
*************************************************************************/

BCK_WRTBLK_STRUCT *
bck_wrtblk_wait ()
{
  int c;
  BCK_WRTBLK_STRUCT *wrtblk;
  int sts4a;


#ifdef BCK_TRACE
  printf("Writebuffer wait...\n");
#endif

  do {
    sts4a = pthread_mutex_lock(&wrtblkmtx);
    check4a(sts4a,"pthread_mutex_lock(&wrtblkmtx)");

    while ((wrtblkque[0] == NULL) && (wrtblkque[1] == NULL)) {
      sts4a = pthread_cond_wait(&wrtblkevt, &wrtblkmtx);
      check4a(sts4a, "pthread_cond_wait");
    }

    c = (wrtblkque[0] != NULL) ? 0 : 1;

    wrtblk = wrtblkque [c];
    wrtblkque [c] = NULL;
  
    sts4a = pthread_mutex_unlock(&wrtblkmtx);
    check4a(sts4a,"pthread_mutex_unlock(&wrtblkmtx)");

  } while (wrtblk == NULL);

#ifdef BCK_TRACE
  printf("Writebuffer %d\n", c);
#endif

  return wrtblk;
} /* bck_wrtblk_wait */


/**************************/
/* File handling routines */
/**************************/

/************************************************************************
*
* Name: bck_file_alloc
*
* Type:	pwr_tInt32
*
* Type		Parameter	IOGF	Description
* pwr_tInt32		numbytes	I	# of bytes to reserve space for
*
* Description:	
*	The routine allocates an area in the backup file. The area
*	always starts on a block boundary. The return value is the
*	file address where things could be written.
*
*************************************************************************/

pwr_tInt32
bck_file_alloc (
  pwr_tUInt32 numbytes
)
{
  pwr_tInt32 allblock [2];
  pwr_tInt32 allsize [2];
  pwr_tInt32 freesize [2];
  pwr_tInt32 nblocks;
  pwr_tInt32 i, j, jinc;
  pwr_tInt32 firstb, b;

  /* Convert all addresses to blocks, and build the  */
  /* allblock & allsize structure describing what is */
  /* allocated in the file.                          */

  nblocks = (numbytes + 511) >> 9;
  firstb = b = (sizeof filehead + 511) >> 9;	 /* Start block */

  for (i=0; i<2; i++) {
    if (filehead.cursize [i] == 0) filehead.curdata[i] = firstb << 9;
  }

  if (filehead.curdata [0] < filehead.curdata [1])
    { j = 0; jinc = 1; }
  else
    { j = 1; jinc = -1; }

  for (i=0; i<2; i++, j+=jinc) {
    allblock [i] = (filehead.curdata [j] + 511) >> 9;
    allsize  [i] = (filehead.cursize [j] + 511) >> 9;
    freesize [i] = allblock [i] - b;
    b = allblock [i] + allsize [i];
  }

  for (i=0; i<2; i++) if (nblocks <= freesize [i]) break;

  if (i==0)
    return firstb << 9;
  else
    return (allblock[i-1] + allsize [i-1]) << 9;
  
}

/************************************************************************
*
* Name: bck_file_process
*
* Type:	void
*
* Type		Parameter	IOGF	Description
*
* Description:	
*	This is the disk process. It handles all the
*	writes to the logfile.
*
*************************************************************************/
void *
bck_file_process (
  void* arg
)
{
  BCK_WRTBLK_STRUCT *wrtblk;
  pwr_tUInt32 filepos;
  pwr_tUInt32 actpos;	/* Actual file position */
  pwr_tUInt32 c;
  pwr_tUInt32 csts;
  pwr_tUInt32 cnt;
  char fname[200];


#if defined OS_VMS || defined OS_ELN
  pwr_tUInt32 sts;
  char tmpstr [256];
#endif

#ifdef seekbug
  int dapsts;
#endif

  while (TRUE) {

    /* Open the file. If it doesn't exist, create a new one.
       Retry every 10th second.
       Make sure we have the file header in memory */
  
    for (cnt=0; ; cnt++) {
      if (cnt != 0) sleep(10);

      /* See if there is a filename... */

      if (strlen(backup_confp->BackupFile) == 0) {
        if (cnt==0) 
	  errh_Error("BACKUP has no backup file specified");
	continue;
      }

      /* First try an old file */

#ifdef seekbug
      if (backup_confp->DiskStatus == 0) {
        dapsts = dapopen(backup_confp->BackupFile, FALSE);
        if (ODD(dapsts)) {
          errh_Info("BACKUP opened existing backupfile %s", dapgetname(&tmpstr));
          dapsts = dapread(0, &filehead, sizeof filehead);
	  if (EVEN(dapsts)) {
	    perror("BACKUP cannot read header");
          }
	  else {
	    if (filehead.version == BCK_FILE_VERSION) break;	/* All well */
	    errh_Error("BACKUP file version mismatch");
	  }
	  dapclose();
        }
      }
#else

      if (backup_confp->DiskStatus == 0) {
	dcli_translate_filename( fname, backup_confp->BackupFile);
        bckfile = fopen(fname, "r+" A_MODE);
        if (bckfile != NULL) {
          errh_Info("BACKUP opened existing backupfile %s\n",  FGETNAME);
          csts = fread(&filehead, sizeof filehead, 1, bckfile);
	  if (csts == 0) {
	    perror("BACKUP cannot read header");
          }
	  else {
	    if (filehead.version == BCK_FILE_VERSION) break;	/* All well */
	    errh_Error("BACKUP file version mismatch");
	  }
	  fclose(bckfile);
        }
      }
#endif
      /* Then try creating a new one */
#ifdef seekbug
      dapsts = dapopen(backup_confp->BackupFile, TRUE);
      if (EVEN(dapsts)) perror("BACKUP cannot create backupfile");
      else {
        errh_Info("BACKUP created new backupfile %s", dapgetname(&tmpstr));

	memset(&filehead, 0, sizeof filehead);
	clock_gettime(CLOCK_REALTIME, &filehead.creationtime);
	filehead.version = BCK_FILE_VERSION;
	LOCK;
	gdh_GetNodeIndex(&filehead.nodidx);
	UNLOCK;

	/* Write a file header */

        dapsts = dapwrite(0, &filehead, sizeof filehead);
        if (ODD(dapsts)) break;				/* All well */

        perror("BACKUP cannot write header");
	dapclose();
      }
#else

      dcli_translate_filename( fname, backup_confp->BackupFile);
      bckfile = fopen(fname, "w+" A_MODE);
      if (bckfile == NULL) 
        perror("BACKUP cannot create backupfile");
      else {

        errh_Info("BACKUP created new backupfile %s", FGETNAME);
	memset(&filehead, 0, sizeof filehead);
	clock_gettime(CLOCK_REALTIME, &filehead.creationtime);
	filehead.version = BCK_FILE_VERSION;

	LOCK;
	gdh_GetNodeIndex(&filehead.nodidx);
	UNLOCK;

	/* Write a file header */

        csts = fwrite(&filehead, sizeof filehead, 1, bckfile);
        if (csts != 0) break;				/* All well */

        perror("BACKUP cannot write header");
	fclose(bckfile);
      }
#endif
    }

    backup_confp->DiskStatus = 0;

    while (TRUE) {

    /* Wait for work and fetch the writeblock information */

      wrtblk = bck_wrtblk_wait();
      c = wrtblk->cyclehead.cycle;

/*
 * See if user wants a new version of backup file
 */

      if (backup_confp->DiskStatus != 0) break;

      /* Get space in the file */

      filepos = bck_file_alloc(wrtblk->cyclehead.length);

      /* Write to file */
#ifdef seekbug
      dapsts = dapwrite(filepos, wrtblk, wrtblk->cyclehead.length);
      if (EVEN(dapsts)) break;
      dapsts = dapflush();
      if (EVEN(dapsts)) break;
#else
      actpos = ftell(bckfile);
      csts = fseek(bckfile, filepos - actpos, SEEK_CUR);
      if (csts != 0)
        break;
      csts = fwrite(wrtblk, wrtblk->cyclehead.length, 1, bckfile);
      if (csts == 0)
        break;
      csts = fflush(bckfile);
      if (csts != 0)
        break;
#endif
      /* Update the fileheader and write it to file */

      filehead.curdata [c] = filepos;
      filehead.cursize [c] = wrtblk->cyclehead.length;
      clock_gettime(CLOCK_REALTIME, &filehead.updatetime [c]);

#ifdef seekbug
      dapsts = dapwrite(0, &filehead, sizeof filehead);
      if (EVEN(dapsts)) break;
      dapsts = dapflush();
      if (EVEN(dapsts)) break;
#else
      actpos = ftell(bckfile);
      csts = fseek(bckfile, -actpos, 1);	/* To beginning of file */
      if (csts != 0) break;
      csts = fwrite(&filehead, sizeof filehead, 1, bckfile);
      if (csts == 0) break;
      csts = fflush(bckfile);
      if (csts != 0) break;
#endif
      /* Update statistics in the configuration block */

      backup_confp->LastWrite = filehead.updatetime [c];
/*
      backup_confp->DiskStatus = 1;
*/
      if (c==0) {
        backup_confp->CntFast++;
        backup_confp->BytesFast = wrtblk->cyclehead.length;
        backup_confp->SegFast = wrtblk->cyclehead.segments;
	backup_confp->ObjTimeFast = wrtblk->cyclehead.objtime;
      }
      else {
        backup_confp->CntSlow++;
        backup_confp->BytesSlow = wrtblk->cyclehead.length;
        backup_confp->SegSlow = wrtblk->cyclehead.segments;
	backup_confp->ObjTimeSlow = wrtblk->cyclehead.objtime;
      }

      aproc_TimeStamp();

      /* Signal bck_write_done */

      /* Change to Posix 1003.4 signal */
#ifdef OS_ELN
      ker$signal(NULL, bck_write_done);
#elif defined OS_VMS
      sts = sys$setef(BCK_WRITE_DONE);
      if (EVEN(sts)) lib$signal(sts);			/* BUG */
#else
	/* NYI */
#endif

      /* Throw away the writeblock data */

#ifdef OS_ELN
      ker$free_memory(&sts, wrtblk->cyclehead.length, wrtblk);
#else
      free(wrtblk);
#endif

    } /* We leave this loop when a file error occurs */

    /* Save the error code for informational purposes */

/*
#if defined OS_ELN || defined OS_VMS
    backup_confp->DiskStatus = vaxc$errno;
#else
    backup_confp->DiskStatus = errno;
#endif
    perror("BACKUP diskerror");
*/

    /* Put back the writeblock, if there isn't a new writeblock	*/

    bck_wrtblk_insert(wrtblk, FALSE);

    /* Close the old backup file */

#ifdef seekbug
    dapclose();
#else
    fclose(bckfile);
#endif
  } /* Until the end of the world comes... */

  return NULL;
} /* bck_file_process */

/*******************/
/* Data collection */
/*******************/

struct BCK_LISTENTRY_STRUCT {
  struct BCK_LISTENTRY_STRUCT *next;  /* Next object in list */
  pwr_tObjid bckobjid;		/* pwr_tObjid for this backup object */
  BCK_DATAHEAD_STRUCT datahead;	/* Data header for this entry */
};
typedef struct BCK_LISTENTRY_STRUCT BCK_LISTENTRY;

typedef struct {
  BCK_LISTENTRY *first;		/* First object in list */
  BCK_CYCLEHEAD_STRUCT cyclehead;	/* Header for this list */
} BCK_LISTHEAD;



/************************************************************************
*
* Name: bck_list_insert
*
* Type: pwr_tUInt32
*
* Type		Parameter	IOGF	Description
* char		*dataname	I	Name of data to be backed up
* pwr_tObjid		objid		I	Objid of backup object
* BCK_LISTHEAD	*blhp		I	List of objects for selected cycle
*
* Description:
*	This routine inserts an entry into the backup list.
*	If it is a dynamic object this function is called recursively to
*	insert also the backup object into the list.
*   
*************************************************************************/

pwr_tStatus
bck_list_insert (
  char			*dataname,	/* Name of data to be backed up */
  pwr_tObjid		objid,		/* Objid of backup object */
  BCK_LISTHEAD		*blhp
)
{
  pwr_tStatus		sts;
  pwr_tStatus		sts1;
  BCK_LISTENTRY		*blep;
  char			bck_objname[256];
  pwr_tBoolean		tgtdynamic = 0;
  void			*adrs;
  pwr_sAttrRef		attrref;
  pwr_tObjid		volobject;
  pwr_tClassId		volclass;

  /* Build attrref, check location.  */

  LOCK;
  sts = gdh_NameToAttrref(pwr_cNObjid, dataname, &attrref);
  UNLOCK;

  if (ODD(sts)) {
    LOCK;
    sts = gdh_ObjidToPointer(attrref.Objid, &adrs);
    UNLOCK;
    if (ODD(sts)) {
      volobject.vid = attrref.Objid.vid;
      volobject.oix = pwr_cNObjectIx;
      LOCK;
      if (ODD(gdh_GetObjectClass(volobject, &volclass)))
	tgtdynamic = volclass == pwr_eClass_DynamicVolume;
      UNLOCK;
      if (tgtdynamic & attrref.Flags.b.Indirect) 
        sts = GDH__RTDBNULL;
      else {
	/* If dynamic, save whole body.  */

        if (tgtdynamic) {
	  attrref.Offset = 0;
	  attrref.Flags.b.Indirect = 0;

	  LOCK;
	  sts = gdh_GetObjectSize(attrref.Objid, (pwr_tUInt32 *)&attrref.Size);
	  UNLOCK;
        }

	/* All info is collected, create a list entry.  */
	blep = malloc(sizeof *blep);
	memset(blep, 0, sizeof *blep);

	blep->datahead.dynamic = tgtdynamic;
	LOCK;
        gdh_GetObjectClass(attrref.Objid, &blep->datahead.class);
	UNLOCK;
	blep->datahead.valid = TRUE;
	strcpy(blep->datahead.dataname, dataname);
	blep->datahead.attrref = attrref;
	blep->bckobjid = objid;

	/* Insert list entry first.  */

	blep->next = blhp->first;
	blhp->first = blep;
	blhp->cyclehead.length += sizeof(BCK_DATAHEAD_STRUCT) + attrref.Size;
	blhp->cyclehead.segments++;

	/* Insert backup object if object to be backed up is dynamic.  */

	if (cdh_ObjidIsNotEqual(objid, pwr_cNObjid) && tgtdynamic) {
	  LOCK;
	  sts1 = gdh_ObjidToName(objid, bck_objname, sizeof bck_objname, cdh_mNName);
	  UNLOCK;
          sts1 = bck_list_insert(bck_objname, pwr_cNObjid, blhp);
	}
      } /* Found all object info */
    } /* Target object is local to this node */
  } /* Target object exists */
  return sts;
}

/************************************************************************
*
* Name: bck_list_build
*
* Type: void
*
* Type		Parameter	IOGF	Description
* pwr_tUInt32	cycle		I	Fast=0, slow=1
* BCK_LISTHEAD	**list		 O	List of objects for selected cycle
*
* Description:
*	This routine builds a backup list.
*
*************************************************************************/

void bck_list_build (
		pwr_tUInt32 cycle,
		BCK_LISTHEAD **list)
{
  pwr_tUInt32 sts;
  pwr_tObjid objid;
  BCK_LISTHEAD *blhp;
  pwr_sClass_Backup *backup;

  /* Allocate and init listhead */

  blhp = malloc(sizeof *blhp);
  memset(blhp, 0, sizeof *blhp);

  blhp->first = NULL;
  clock_gettime(CLOCK_REALTIME, &blhp->cyclehead.objtime);
  blhp->cyclehead.cycle = cycle;
  blhp->cyclehead.length = sizeof(BCK_CYCLEHEAD_STRUCT);
  *list = blhp;

  /* Find root of typelist, and walk thru it */

  LOCK;
  sts = gdh_GetClassList(pwr_cClass_Backup, &objid);
  UNLOCK;
  while (ODD(sts)) {

    /* Find body of backup object */

    LOCK;
    sts = gdh_ObjidToPointer(objid, (pwr_tAddress *)&backup);
    UNLOCK;
    if (ODD(sts) && (backup != NULL)) {

      /* Is this the correct cycle time? */

      if (backup->Fast == (cycle == 0))
        backup->Status = bck_list_insert(backup->DataName, objid, blhp);
    } /* Backup object is local */
    LOCK;
    sts = gdh_GetNextObject(objid, &objid);
    UNLOCK;
  } /* Loop thru the list */

} /* bck_list_build */

/************************************************************************
*
* Name: bck_list_free
*
* Type:	void
*
* Type		Parameter	IOGF	Description
* BCK_LISTHEAD	*list		I	List of fast cycle objects
*
* Description:	
*	This routine frees a backup list
*
*************************************************************************/

void bck_list_free (
		BCK_LISTHEAD *list)
{
  BCK_LISTENTRY *blep, *blep2;

  blep = list->first;
  while (blep != NULL) {
    blep2 = blep->next;
    free(blep);
    blep = blep2;
  }
  free(list);
} /* bck_list_free */


/************************************************************************
*
* Name: bck_coll_process
*
* Type:	void
*
* Type		Parameter	IOGF	Description
*
* Description:	
*	This is the data collection process. It is run in two
*	versions, one fast (cycle=0) and one slow (cycle=1).
*
*************************************************************************/

void *bck_coll_process (
		void* arg)
{
  BCK_WRTBLK_STRUCT *wrtblk;
  BCK_LISTHEAD *bcklist;
  BCK_LISTENTRY *blep;
  BCK_DATAHEAD_STRUCT *dhp;
  char *p;					/* data pointer */
  pwr_tUInt32 sts;
  pwr_sClass_Backup *bckp;
  pwr_tBoolean forced;
  pwr_tUInt32 cycletime;				/* in 0.1 second units */
  pwr_tUInt32 oldslowtick;
  pwr_tUInt32 cycle;
  int sts4a;
  pwr_tTime abstime;
  pwr_tDeltaTime delta;

  cycle = (pwr_tUInt32)arg;

#ifdef BCK_TRACE
  printf("coll_proc %d running...\n", cycle);
#endif

  bck_list_build(cycle, &bcklist);

  while (TRUE) {

    /* Read the timer time from the Backup conf object.
       Wait for timer, or forced activation  */

    cycletime = (cycle==0 ? backup_confp->CycleFast : backup_confp->CycleSlow);
    if (cycletime == 0) cycletime = (cycle==0 ?
					BCK_DEFAULT_FAST :
					BCK_DEFAULT_SLOW);

    delta.tv_sec = cycletime / 10;
    delta.tv_nsec = (cycletime % 10) * 100000000;

#if defined OS_VMS
    {
      unsigned long t = 0;
      struct tm	*tmpTm;
      clock_gettime(CLOCK_REALTIME, &abstime);
      time_Aadd(&abstime, &abstime, &delta);
      tmpTm = localtime(&t);
      abstime.tv_sec += tmpTm->tm_gmtoff;
    }
#elif defined OS_LYNX && defined(PWR_LYNX_30)
    abstime.tv_sec = delta.tv_sec;
    abstime.tv_nsec = delta.tv_nsec;
#elif defined OS_ELN
#undef clock_gettime
    clock_gettime(CLOCK_REALTIME, &abstime);
    time_Aadd(&abstime, &abstime, &delta);
#else
    clock_gettime(CLOCK_REALTIME, &abstime);
    time_Aadd(&abstime, &abstime, &delta);
#endif

    sts4a = pthread_mutex_lock(&frcactmtx);
    check4a(sts4a,"pthread_mutex_lock(&frcactmtx)");

    if (!frcact [cycle]) {
      sts4a = pthread_cond_timedwait(&frcactevt, &frcactmtx, &abstime);

      /* I get all kinds of funny errors from cond_timedwait, though it seems
         to work (VMS 5.5-2). The error check is just commented out... /CH
       
           if ((sts4a == -1) && (errno != EAGAIN)) {
             check4a(sts4a,"pthread_cond_timedwait error");
           }
        */

    }
    forced = frcact [cycle];
    if (forced) frcact [cycle] = FALSE;

    sts4a = pthread_mutex_unlock(&frcactmtx);
    check4a(sts4a,"pthread_mutex_unlock(&frcactmtx)");

#ifdef BCK_TRACE
    printf("coll_proc cycle %d collecting. Forced = %d\n", cycle, forced);
#endif

    /* If this is the slow cycle: increase slowtick */

    if (cycle == 1) slowtick++;

    /* If forced activation or slow cycle tick: rebuild the backup list */

    if (forced || (slowtick != oldslowtick)) {
      oldslowtick = slowtick;
      bck_list_free(bcklist);
      bck_list_build(cycle, &bcklist);
    }

    /* Allocate a wrtblk using size in cycle head */

#ifdef OS_ELN
    ker$allocate_memory(&sts, &wrtblk, bcklist->cyclehead.length, NULL, NULL);
#else
    wrtblk = malloc(bcklist->cyclehead.length);
#endif

    wrtblk->cyclehead = bcklist->cyclehead;

    /* Walk the bcklist */

    p = (char *)&wrtblk->segment [0];
    blep = bcklist->first;
    while (blep != NULL) {

      /* Copy data header */

      dhp = (BCK_DATAHEAD_STRUCT *)p;	/* dhp points to data header */
      p += sizeof *dhp;		/* p points to data part */
      *dhp = blep->datahead;

      /* Check object existence, set validity flag, move data */

      LOCK;
      sts = gdh_GetObjectInfoAttrref(&blep->datahead.attrref, p, blep->datahead.attrref.Size);
      UNLOCK;
      dhp->valid = ODD(sts);
      
      /* Record status code in backup object */

      LOCK;
      if (ODD(gdh_ObjidToPointer(blep->bckobjid, (pwr_tAddress *)&bckp))) {
	bckp->Status = sts;
      } /* Backup object is still there */
      UNLOCK;

      p += dhp->attrref.Size;
      blep = blep->next;
    }

    /* Queue the wrtblk for writing */

    bck_wrtblk_insert(wrtblk, TRUE);



  } /* until end of world */

  return NULL;

} /* bck_coll_process */

/************************************************************************
*
* Name: bck_init
*
* Type:	pwr_tUInt32, returns status code
*
* Type		Parameter	IOGF	Description
*
* Description:	
*	A call to this routine starts the backup package normal operation.
*	This means that data collation and disk writing starts. This
*	runs as subprocesses in the context of the job that
*	calls this routine.
*
*************************************************************************/

pwr_tUInt32 bck_init ()
{
  pwr_tUInt32 sts, sts4a;
  pwr_tUInt32 c;
  pwr_tObjid objid;
  pwr_tBoolean local;
  char name [256];
  pwr_tDlid dlid;
  pthread_t thr_file;
  pthread_t thr_coll [2];
  pwr_sAttrRef attrref;
#ifdef OS_VMS
  $DESCRIPTOR(efcname, BCK_EFC_NAME);
#endif
#if defined OS_LYNX || defined OS_LINUX
  pthread_attr_t attr;
#endif

  /* Create and initialize all memory structures */

  slowtick = 0;
#if defined OS_ELN || defined OS_VMS
  sts4a = pthread_mutex_init(&wrtblkmtx, pthread_mutexattr_default);
  check4a(sts4a, "pthread_mutex_init wrtblkmtx");
  sts4a = pthread_cond_init(&wrtblkevt, pthread_condattr_default);
  check4a(sts4a, "pthread_cond_init wrtblkevt");
#else
  sts4a = pthread_mutex_init(&wrtblkmtx, NULL);
  check4a(sts4a, "pthread_mutex_init wrtblkmtx");
  sts4a = pthread_cond_init(&wrtblkevt, NULL);
  check4a(sts4a, "pthread_cond_init wrtblkevt");
#endif

  for (c=0; c<2; c++) wrtblkque [c] = NULL;

#if defined OS_ELN || defined OS_VMS
  sts4a = pthread_mutex_init(&frcactmtx, pthread_mutexattr_default);
  check4a(sts4a, "pthread_mutex_init frcactmtx");
  sts4a = pthread_cond_init(&frcactevt, pthread_condattr_default);
  check4a(sts4a, "pthread_cond_init frcactevt");
#else
  sts4a = pthread_mutex_init(&frcactmtx, NULL);
  check4a(sts4a, "pthread_mutex_init frcactmtx");
  sts4a = pthread_cond_init(&frcactevt, NULL);
  check4a(sts4a, "pthread_cond_init frcactevt");
#endif
  for (c=0; c<2; c++) frcact [c] = FALSE;

  /* This should eventually use Posix 1003.4 signals */
#ifdef OS_ELN
  BCK_MAP_AREAS;
#elif defined OS_VMS
  sts = sys$ascefc(BCK_EFC, &efcname, 0, 0);
  if (EVEN(sts)) lib$signal(sts);			/* BUG */
#else


#endif

  /* Find the local Backup_Conf object */

  LOCK;
  sts = gdh_GetClassList(pwr_cClass_Backup_Conf, &objid);
  while (ODD(sts)) {
    sts = gdh_GetObjectLocation(objid, &local);
    if (ODD(sts) && local) break;
    sts = gdh_GetNextObject(objid, &objid);
  }
  UNLOCK;
  if (EVEN(sts)) return sts;		/* Something wrong, quit */

  aproc_RegisterObject( objid);

  LOCK;
  sts = gdh_ObjidToName(objid, name, sizeof name, cdh_mNName);
  UNLOCK;
  if (EVEN(sts)) return sts;

  /* Using DLRefObjectInfo means that we lock the object from being
     deleted, and the code can access the body without checking for
     object existence... */

  LOCK;
  sts = gdh_NameToAttrref(pwr_cNObjid, name, &attrref);
  if (ODD(sts)) {
    sts = gdh_DLRefObjectInfoAttrref(&attrref, (pwr_tAddress *)&backup_confp, &dlid);
  }
  UNLOCK;
  if (EVEN(sts)) return sts;

  /* Activate the disk process
     Activate the fast and slow cycle processes */

#if defined OS_ELN || (defined OS_VMS && !defined(PTHREAD_1003_1C))
  sts4a = pthread_create(&thr_file, pthread_attr_default, bck_file_process, NULL);
  check4a(sts4a, "pthread_create thr_file");

  for (c=0; c<2; c++) {
    sts4a = pthread_create(&thr_coll[c], pthread_attr_default, bck_coll_process, (void *)c);
    check4a(sts4a, "pthread_create thr_coll");
  }

#elif defined(OS_VMS) && defined(PTHREAD_1003_1C)

  sts4a = pthread_create(&thr_file, NULL, bck_file_process, NULL);
  check4a(sts4a, "pthread_create thr_file");

  for (c=0; c<2; c++) {
    sts4a = pthread_create(&thr_coll[c], NULL, bck_coll_process, (void *)c);
    check4a(sts4a, "pthread_create thr_coll");
  }

#elif defined OS_LYNX && defined(PWR_LYNX_30)

  pthread_attr_create(&attr);

  sts4a = pthread_create(&thr_file, attr, bck_file_process, NULL);
  check4a(sts4a, "pthread_create thr_file");

  for (c=0; c<2; c++) {
    sts4a = pthread_create(&thr_coll[c], attr, bck_coll_process, (void *)c);
    check4a(sts4a, "pthread_create thr_coll");
  }

#elif defined OS_LINUX || defined OS_LYNX

  pthread_attr_init(&attr);

  sts4a = pthread_create(&thr_file, &attr, bck_file_process, NULL);
  check4a(sts4a, "pthread_create thr_file");

  for (c=0; c<2; c++) {
    sts4a = pthread_create(&thr_coll[c], &attr, bck_coll_process, (void *)c);
    check4a(sts4a, "pthread_create thr_coll");
  }

#endif

  return sts;
} /* bck_init */

/* This is the main program of backup. It initializes backup
   and then waits for forced activation... */

int main ()
{
  pwr_tStatus sts;
  pwr_tInt32 sts4a;
  pwr_tUInt32 c;
  qcom_sQid	my_q = qcom_cNQid;

  errh_Init("pwr_bck", errh_eAnix_bck);
  errh_SetStatus( PWR__SRVSTARTUP);

  sts = gdh_Init("pwr_bck");
  if (EVEN(sts)) return sts;

  aproc_TimeStamp();

  sts = bck_init();
  if (EVEN(sts)) {
    errh_Fatal( "Initialization error, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }
  errh_SetStatus( PWR__SRUN);

  if (!qcom_Init(&sts, NULL, "pwr_bck")) {
    errh_Fatal("qcom_Init, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  if (!qcom_CreateQ(&sts, &my_q, NULL, "events")) {
    errh_Fatal("qcom_CreateQ, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  for (;;) {
#ifdef OS_ELN
    ker$clear_event(NULL, bck_forced_activation);
    ker$wait_any(NULL, NULL, NULL, bck_forced_activation);
#elif defined OS_VMS
    sts = sys$clref(BCK_ACTIVATE);
    if (EVEN(sts)) lib$signal(sts);			/* BUG */
    sts = sys$waitfr(BCK_ACTIVATE);
    if (EVEN(sts)) lib$signal(sts);			/* BUG */
#else
    while(1) {
      qcom_WaitAnd(&sts, &my_q, &qcom_cQini, ini_mEvent_terminate, qcom_cTmoEternal);
//      pause();
      exit(0);
    }
#endif

    /* We were activated from the outer world. Trigger */
    /* the collector threads... */


    pthread_mutex_lock(&frcactmtx);

    for (c=0; c<2; c++) frcact [c] = TRUE;

    sts4a = pthread_cond_broadcast(&frcactevt);
    check4a(sts4a,"pthread_cond_broadcast error");

    pthread_mutex_unlock(&frcactmtx);
  }

}






