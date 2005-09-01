/* 
 * Proview   $Id: rt_rtt_os.c,v 1.2 2005-09-01 14:57:56 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* rt_rtt_os.c 
   This module contains routines for os dependent functions in rtt. */

/*_Include files_________________________________________________________*/

#if defined OS_ELN
# include $kerneldef
# include $vaxelnc
# include string
# include stdio
# include stdlib
# include descrip
# include starlet
# include chfdef
#elif defined OS_VMS
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <descrip.h>
# include <starlet.h>
# include <chfdef.h>
# include <lib$routines.h>
# include <ssdef.h>
# include <prcdef.h>
# include <jpidef.h>
# include <syidef.h>
#elif defined OS_LYNX || defined OS_LINUX
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/utsname.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "rt_gdh.h"
#include "rt_rtt.h"
#include "rt_rtt_global.h"
#include "rt_rtt_functions.h"
#include "rt_rtt_msg.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif


/*_Local function prototypes____________________________________________*/

#ifdef OS_VMS
static int 	rttvms_get_procname( int pid, char *prcnam_ptr);
static int	rttvms_get_pri( int prcnum, int *pri);
static int	rttvms_get_cputim( int prcnum, int *cputim);
static int	rttvms_get_state( int prcnum, char *state);
static int	rttvms_get_pid( char *proc_name, int *pid);
static int	rttvms_get_subproc( int owner, int *subproc, int subproc_size,
		int *subproc_count);
#elif OS_ELN
static int	rtteln_get_nodename( char *nodename, int size);
#endif	

/*************************************************************************
*
* Name:		rtt_get_nodename()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get nodename for the current system.
*
**************************************************************************/
int	rtt_get_nodename( char *nodename, int size)
{
#if defined(OS_VMS)
	return rttvms_get_nodename( nodename, size);
#elif defined(OS_ELN)
	return rtteln_get_nodename( nodename, size);
#elif defined(OS_LYNX) || defined(OS_LINUX)
	struct	utsname buf;

	if (uname( &buf) == -1)
	  return 0;
	strncpy( nodename, buf.nodename, size);
	return 1;
#endif	
}

/*************************************************************************
*
* Name:		rtt_fgetname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get filename for a filedescriptor.
*	This function is not implementet on all os, therefor a defaultname
*	should be supplied which is returned for this os.
*
**************************************************************************/
char	*rtt_fgetname( FILE *fp, char *name, char *def_name)
{
#if defined(OS_VMS) || defined(OS_ELN)
	return fgetname( fp, name);
#else
	strcpy( name, def_name);
	return name;
#endif	
}

/*************************************************************************
*
* Name:		rtt_set_prio()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Set process priority.
*
**************************************************************************/
int	rtt_set_prio( int prio)
{
#if defined(OS_VMS)
	int sts;

	if ( prio == -1)
	  prio = 4;
	sts = sys$setpri(NULL, NULL, prio, NULL, NULL, NULL);
	return sts;

#elif  defined(OS_ELN)
	int sts;

	if ( prio == -1)
	  prio = 16;
	ker$set_job_priority( &sts, prio);
	return sts;

#else
	/* TODO.... */
	return 1;
#endif	
}
/*************************************************************************
*
* Name:		rtt_set_prio()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Set process priority to default value for rtt.
*
**************************************************************************/
int	rtt_set_default_prio()
{
#if defined(OS_VMS)
	int sts;
	int prio;

	prio = 4;
	sts = sys$setpri(NULL, NULL, prio, NULL, NULL, NULL);
	return sts;

#elif  defined(OS_ELN)
	int sts;
	int prio;

	prio = 16;
	ker$set_job_priority( &sts, prio);
	return sts;

#else
	/* TODO.... */
	return 1;
#endif	
}



#ifdef OS_VMS

#define	SCH$C_COLPG	1
#define	SCH$C_MWAIT	2
#define	SCH$C_CEF	3
#define	SCH$C_PFW	4
#define	SCH$C_LEF	5
#define	SCH$C_LEFO	6
#define	SCH$C_HIB	7
#define	SCH$C_HIBO	8
#define	SCH$C_SUSP	9
#define	SCH$C_SUSPO	10
#define	SCH$C_FPG	11
#define	SCH$C_COM	12
#define	SCH$C_COMO	13
#define	SCH$C_CUR	14

typedef struct
	{
	short int	buflen;
	short int	item;
	unsigned int	bufadr;
	unsigned int	lenadr;
	} rtt_t_itemlist;

/*************************************************************************
*
* Name:		rttvms_get_nodename()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get nodename for the current system.
*
**************************************************************************/
int	rttvms_get_nodename( char *nodename, int size)
{
	int		sts;
	unsigned short	nodename_len;
	rtt_t_itemlist	item_list[2] = {{ 0,SYI$_NODENAME,0,0},
					{ 0,0,0,0}};
 	int    		pri_len;
  
	item_list[0].lenadr = (unsigned int) &nodename_len;
	item_list[0].bufadr = (unsigned int) nodename;
	item_list[0].buflen = size;

	sts = sys$getsyiw(0, NULL, NULL, &item_list, 0, NULL, 0);
	if (EVEN(sts)) return sts;
	nodename[nodename_len] = 0;
	return 1;
}
/*************************************************************************
*
* Name:		rttvms_get_uic()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get uic for the current process.
*
**************************************************************************/
int	rttvms_get_uic( int *uic)
{
	int		sts;
 	int    		uic_len = 4;
	rtt_t_itemlist	item_list[2] = {{ 4, JPI$_UIC, 0, 0},
					{0,0,0,0}};
 	int		prcnum = 0; 
  
	item_list[0].lenadr = (unsigned int) &uic_len;
	item_list[0].bufadr = (unsigned int) uic;
	sts = sys$getjpiw( 0,  /*efn  */
			&prcnum , /* pidadrt*/
			0, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
	return sts;
}

/*************************************************************************
*
* Name:		rttvms_get_identname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get name of an identifier.
*
**************************************************************************/
int rttvms_get_identname( int id, char *name)
{

  int 		sts;
  int		resid;
  static char 	identname[20];
  short		namlen;

  $DESCRIPTOR(d_identname, identname);

  sts = sys$idtoasc( id, &namlen, &d_identname, &resid, 0, 0);
  if ( EVEN(sts)) return sts; 
  identname[namlen] = '\0';
  strcpy ( name, identname); 
  return 1;
}

/*************************************************************************
*
* Name:		rttvms_get_procname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get process name for a process.
*
**************************************************************************/
static int rttvms_get_procname( int pid, char *prcnam_ptr)
{

  int 		sts, item;
  short 	len;
  char 		*ptr, *ptra;
  static char 	process[20];

  $DESCRIPTOR(d_process, process);


  ptr = process;
  item = JPI$_PRCNAM ;
  sts = lib$getjpi(&item, &pid, 0, 0, &d_process, &len);
  if ( EVEN(sts) ) 
    return (sts) ; 
  process[len] = '\0';
  if (len == 0) 
  {
    /* */
    return 0; 
  }
  strcpy ( prcnam_ptr , ptr ) ; 
  return 1;
}

/*************************************************************************
*
* Name:		rttvms_get_pri()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get priority for a process.
*
**************************************************************************/
static int	rttvms_get_pri( int prcnum, int *pri)
{
	int		sts;
 	int    		pri_len = 4;
	rtt_t_itemlist	item_list[2] = {{ 4, JPI$_PRI, 0, 0},
					{0,0,0,0}};
  
	item_list[0].lenadr = (unsigned int) &pri_len;
	item_list[0].bufadr = (unsigned int) pri;
	sts = sys$getjpiw( 0,  /*efn  */
			&prcnum , /* pidadrt*/
			0, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
	return sts;
}

/*************************************************************************
*
* Name:		rttvms_get_cputim()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get cputim for a process.
*
**************************************************************************/
static int	rttvms_get_cputim( int prcnum, int *cputim)
{
	int		sts;
 	int    		cputim_len = 4;
	rtt_t_itemlist	item_list[2] = {{4, JPI$_CPUTIM, 0,0},
					{0,0,0,0}};
  
	item_list[0].lenadr = (unsigned int) &cputim_len;
	item_list[0].bufadr = (unsigned int) cputim;
	sts = sys$getjpiw( 0,  /*efn  */
			&prcnum , /* pidadrt*/
			0, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
	return sts;
}

/*************************************************************************
*
* Name:		rttvms_get_state()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get state for a process.
*
**************************************************************************/
static int	rttvms_get_state( int prcnum, char *state)
{
	int		sts;
	unsigned long	state_flag;
 	int    		state_len = 4;
	rtt_t_itemlist	item_list[2] = {{ 4, JPI$_STATE, 0, 0},
					{0,0,0,0}};
  
	item_list[0].lenadr = (unsigned int) &state_len;
	item_list[0].bufadr = (unsigned int) &state_flag;

	sts = sys$getjpiw( 0,  /*efn  */
			&prcnum , /* pidadrt*/
			0, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
	if ( EVEN(sts)) return sts;
	switch ( state_flag)
	{	
	  case SCH$C_CEF: strcpy( state, "CEF"); break;
	  case SCH$C_COM: strcpy( state, "COM"); break;
	  case SCH$C_COMO: strcpy( state, "COMO"); break;
	  case SCH$C_CUR: strcpy( state, "CUR"); break;
	  case SCH$C_COLPG: strcpy( state, "COLPG"); break;
	  case SCH$C_FPG: strcpy( state, "FPG"); break;
	  case SCH$C_HIB: strcpy( state, "HIB"); break;
	  case SCH$C_HIBO: strcpy( state, "HIBO"); break;
	  case SCH$C_LEF: strcpy( state, "LEF"); break;
	  case SCH$C_LEFO: strcpy( state, "LEFO"); break;
	  case SCH$C_MWAIT: strcpy( state, "MWAIT"); break;
	  case SCH$C_PFW: strcpy( state, "PFW"); break;
	  case SCH$C_SUSP: strcpy( state, "SUSP"); break;
	  case SCH$C_SUSPO: strcpy( state, "SUSPO"); break;
	}
	return 1;
}

/*************************************************************************
*
* Name:		rttvms_get_pid_from_imagname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get pid for a process from image name.
*
**************************************************************************/
static int	rttvms_get_pid_from_imagname( 	char *wanted_imagname, 
						int *found_pid)
{
	int		sts;
 	int    		pid_len = 4;
	int		imagname_len;
	unsigned long	pid;
	char		imagname[80];
	rtt_t_itemlist	item_list[2] = {{ 4, JPI$_PID, 0,0},
					{0,0,0,0}};
	rtt_t_itemlist	item2_list[2] = {{80, JPI$_IMAGNAME, 0, 0},
					{0,0,0,0}};
 	int		prcnum = -1; 
	char		*s;
  
	item_list[0].lenadr = (unsigned int) &pid_len;
	item_list[0].bufadr = (unsigned int) &pid;
	item2_list[0].bufadr = (unsigned int) imagname;
	item2_list[0].lenadr = (unsigned int) &imagname_len;
	sts = 1;
	while ( ODD(sts))
	{
	  sts = sys$getjpiw( 0,  /*efn  */
			&prcnum, /* pidadrt*/
			0, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
          if ( sts == SS$_NOPRIV)
	  {
	    sts = 1;
	    continue;
	  }
	  if ( ODD(sts)) 
	  {
	    sts = sys$getjpiw( 0,  /*efn  */
			&pid, /* pidadrt*/
			0, /* prcnam*/
			&item2_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
	    if (EVEN(sts)) return sts;

	    /* Remove directory and version */
	    s = strrchr( imagname, ';');
	    if ( s)
	      *s = 0;
	    s = strrchr( imagname, ']');
	    if (s)
	      strcpy( imagname, s + 1);
	    s = strrchr( imagname, '>');
	    if (s)
	      strcpy( imagname, s + 1);
	    if ( !strcmp( wanted_imagname, imagname))
	    {
	      *found_pid = pid;
	      return 1;
	    }
	  }
	}
	return 0;
}

/*************************************************************************
*
* Name:		rttvms_get_pid()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get process identity from process name.
*
**************************************************************************/
static int	rttvms_get_pid( char *proc_name, int *pid)
{
	int		sts;
 	int    		pid_len = 4;
	rtt_t_itemlist	item_list[2] = {{ 4, JPI$_PID, 0,0},
					{0,0,0,0}};
 	int		prcnum = -1; 
	char		prcname[80];
	struct dsc$descriptor_s prcname_desc = 
			{0, DSC$K_DTYPE_T, DSC$K_CLASS_S, 0};

	strcpy( prcname, proc_name);

	prcname_desc.dsc$w_length = strlen( prcname);
	prcname_desc.dsc$a_pointer = prcname;
  
	item_list[0].bufadr = (unsigned int) pid;
	item_list[0].lenadr = (unsigned int) &pid_len;
	sts = sys$getjpiw( 0,  /*efn  */
			0, /* pidadrt*/
			&prcname_desc, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
	return sts;
}

/*************************************************************************
*
* Name:		rttvms_get_subproc()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get subprocesses to a process.
*
**************************************************************************/
static int	rttvms_get_subproc( int owner, int *subproc, int subproc_size,
		int *subproc_count)
{
	int		sts;
 	int    		pid_len = 4;
 	int    		owner_pid_len = 4;
	unsigned long	pid;
	unsigned long	owner_pid;
	rtt_t_itemlist	item_list[2] = {{ 4, JPI$_PID, 0,0},
					{0,0,0,0}};
	rtt_t_itemlist	item2_list[2] = {{ 4, JPI$_OWNER, 0,0},
					 {0,0,0,0}};
 	int		prcnum = -1; 
  
	item_list[0].lenadr = (unsigned int) &pid_len;
	item_list[0].bufadr = (unsigned int) &pid;
	item2_list[0].lenadr = (unsigned int) &owner_pid_len;
	item2_list[0].bufadr = (unsigned int) &owner_pid;
	sts = 1;
	*subproc_count = 0;
	while ( ODD(sts))
	{
	  if ( *subproc_count >= subproc_size)
	    return 3;
	  sts = sys$getjpiw( 0,  /*efn  */
			&prcnum, /* pidadrt*/
			0, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
          if ( sts == SS$_NOPRIV)
	  {
	    sts = 1;
	    continue;
	  }
	  if ( ODD(sts)) 
	  {
	    sts = sys$getjpiw( 0,  /*efn  */
			&pid, /* pidadrt*/
			0, /* prcnam*/
			&item2_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);

	    if ( owner_pid == owner)
	    {
	      *subproc = pid;
	      subproc++;
	      (*subproc_count)++;
	    }
	  }
	}
	return 1;
}

int	rttvms_get_procinfo( int prcnum, char *state, int *pri, int *cputim)
{
	int			sts;
 	int    			cputim_len = 4;
 	int    			state_len = 4;
	int			state_flag;
 	int    			pri_len = 4;
	rtt_t_itemlist	item_list[4] = {{4, JPI$_STATE, 0, 0},
					{4, JPI$_PRI, 0, 0},
					{4, JPI$_CPUTIM, 0, 0},
					{0,0,0,0}};
  
	item_list[0].bufadr = (unsigned int) &state_flag;
	item_list[0].lenadr = (unsigned int) &state_len;
	item_list[1].bufadr = (unsigned int) pri;
	item_list[1].lenadr = (unsigned int) &pri_len;
	item_list[2].bufadr = (unsigned int) cputim;
	item_list[2].lenadr = (unsigned int) &cputim_len;
	sts = sys$getjpiw( 0,  /*efn  */
			&prcnum , /* pidadrt*/
			0, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
	if ( EVEN(sts)) return sts;

	switch ( state_flag)
	{	
	  case SCH$C_CEF: strcpy( state, "CEF"); break;
	  case SCH$C_COM: strcpy( state, "COM"); break;
	  case SCH$C_COMO: strcpy( state, "COMO"); break;
	  case SCH$C_CUR: strcpy( state, "CUR"); break;
	  case SCH$C_COLPG: strcpy( state, "COLPG"); break;
	  case SCH$C_FPG: strcpy( state, "FPG"); break;
	  case SCH$C_HIB: strcpy( state, "HIB"); break;
	  case SCH$C_HIBO: strcpy( state, "HIBO"); break;
	  case SCH$C_LEF: strcpy( state, "LEF"); break;
	  case SCH$C_LEFO: strcpy( state, "LEFO"); break;
	  case SCH$C_MWAIT: strcpy( state, "MWAIT"); break;
	  case SCH$C_PFW: strcpy( state, "PFW"); break;
	  case SCH$C_SUSP: strcpy( state, "SUSP"); break;
	  case SCH$C_SUSPO: strcpy( state, "SUSPO"); break;
	}
	return sts;
}

/*************************************************************************
*
* Name:		rttvms_get_procinfo_full()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get full info for one process.
*
**************************************************************************/
int	rttvms_get_procinfo_full( 	int 		prcnum, 
					char		*state,
				 	int 		*pri,
					int		*prib,
				 	int 		*cputim,
					int		*logintim,
					int		*uic,
					int		*bufio,
					char		*imagname,
					int		*gpgcnt,
					int		*jobprccnt,
					int		*pageflts,
					int		*pagfilcnt,
					int		*pgflquota,
					char		*prcnam,
					int		*fillm,
					int		*virtpeak,
					int		*wspeak,
					int		*wsquota,
					int		*wsextent,
					int		*wssize,
					int		*ppgcnt,
					int		*dirio)
{
	int			sts;
	int			state_flag;
 	int    			cputim_len;
 	int    			state_len;
 	int    			pri_len;
	int			prib_len;
	int			logintim_len;
	int			uic_len;
	int			bufio_len;
	int			imagname_len;
	int			gpgcnt_len;
	int			jobprccnt_len;
	int			pageflts_len;
	int			pagfilcnt_len;
	int			pgflquota_len;
	int			prcnam_len;
	int			fillm_len;
	int			virtpeak_len;
	int			wspeak_len;
	int			wsquota_len;
	int			wsextent_len;
	int			wssize_len;
	int			ppgcnt_len;
	int			dirio_len;
	rtt_t_itemlist	item_list[] = {{4, JPI$_STATE, 0, 0},
					{4, JPI$_PRI, 0, 0},
					{4, JPI$_PRIB, 0, 0},
					{4, JPI$_CPUTIM, 0, 0},
					{8, JPI$_LOGINTIM, 0, 0},
					{4, JPI$_UIC, 0, 0},
					{4, JPI$_BUFIO, 0, 0},
					{80, JPI$_IMAGNAME, 0, 0},
					{4, JPI$_GPGCNT, 0, 0},
					{4, JPI$_JOBPRCCNT, 0, 0},
					{4, JPI$_PAGEFLTS, 0, 0},
					{4, JPI$_PAGFILCNT, 0, 0},
					{4, JPI$_PGFLQUOTA, 0, 0},
					{15, JPI$_PRCNAM, 0, 0},
					{4, JPI$_FILLM, 0, 0},
					{4, JPI$_VIRTPEAK, 0, 0},
					{4, JPI$_WSPEAK, 0, 0},
					{4, JPI$_WSQUOTA, 0, 0},
					{4, JPI$_WSEXTENT, 0, 0},
					{4, JPI$_WSSIZE, 0, 0},
					{4, JPI$_PPGCNT, 0, 0},
					{4, JPI$_DIRIO, 0, 0},
					{0,0,0,0}};
  
	item_list[0].bufadr = (unsigned int) &state_flag;
	item_list[0].lenadr = (unsigned int) &state_len;
	item_list[1].bufadr = (unsigned int) pri;
	item_list[1].lenadr = (unsigned int) &pri_len;
	item_list[2].bufadr = (unsigned int) prib;
	item_list[2].lenadr = (unsigned int) &prib_len;
	item_list[3].bufadr = (unsigned int) cputim;
	item_list[3].lenadr = (unsigned int) &cputim_len;
	item_list[4].bufadr = (unsigned int) logintim;
	item_list[4].lenadr = (unsigned int) &logintim_len;
	item_list[5].bufadr = (unsigned int) uic;
	item_list[5].lenadr = (unsigned int) &uic_len;
	item_list[6].bufadr = (unsigned int) bufio;
	item_list[6].lenadr = (unsigned int) &bufio_len;
	item_list[7].bufadr = (unsigned int) imagname;
	item_list[7].lenadr = (unsigned int) &imagname_len;
	item_list[8].bufadr = (unsigned int) gpgcnt;
	item_list[8].lenadr = (unsigned int) &gpgcnt_len;
	item_list[9].bufadr = (unsigned int) jobprccnt;
	item_list[9].lenadr = (unsigned int) &jobprccnt_len;
	item_list[10].bufadr = (unsigned int) pageflts;
	item_list[10].lenadr = (unsigned int) &pageflts_len;
	item_list[11].bufadr = (unsigned int) pagfilcnt;
	item_list[11].lenadr = (unsigned int) &pagfilcnt_len;
	item_list[12].bufadr = (unsigned int) pgflquota;
	item_list[12].lenadr = (unsigned int) &pgflquota_len;
	item_list[13].bufadr = (unsigned int) prcnam;
	item_list[13].lenadr = (unsigned int) &prcnam_len;
	item_list[14].bufadr = (unsigned int) fillm;
	item_list[14].lenadr = (unsigned int) &fillm_len;
	item_list[15].bufadr = (unsigned int) virtpeak;
	item_list[15].lenadr = (unsigned int) &virtpeak_len;
	item_list[16].bufadr = (unsigned int) wspeak;
	item_list[16].lenadr = (unsigned int) &wspeak_len;
	item_list[17].bufadr = (unsigned int) wsquota;
	item_list[17].lenadr = (unsigned int) &wsquota_len;
	item_list[18].bufadr = (unsigned int) wsextent;
	item_list[18].lenadr = (unsigned int) &wsextent_len;
	item_list[19].bufadr = (unsigned int) wssize;
	item_list[19].lenadr = (unsigned int) &wssize_len;
	item_list[20].bufadr = (unsigned int) ppgcnt;
	item_list[20].lenadr = (unsigned int) &ppgcnt_len;
	item_list[21].bufadr = (unsigned int) dirio;
	item_list[21].lenadr = (unsigned int) &dirio_len;
	sts = sys$getjpiw( 0,  /*efn  */
			&prcnum , /* pidadrt*/
			0, /* prcnam*/
			&item_list, /* itmlst*/
			0, /* iosb*/
			0, /* astadr*/
			0 /* astprm */
			);
	if ( EVEN(sts)) return sts;

	switch ( state_flag)
	{	
	  case SCH$C_CEF: strcpy( state, "CEF"); break;
	  case SCH$C_COM: strcpy( state, "COM"); break;
	  case SCH$C_COMO: strcpy( state, "COMO"); break;
	  case SCH$C_CUR: strcpy( state, "CUR"); break;
	  case SCH$C_COLPG: strcpy( state, "COLPG"); break;
	  case SCH$C_FPG: strcpy( state, "FPG"); break;
	  case SCH$C_HIB: strcpy( state, "HIB"); break;
	  case SCH$C_HIBO: strcpy( state, "HIBO"); break;
	  case SCH$C_LEF: strcpy( state, "LEF"); break;
	  case SCH$C_LEFO: strcpy( state, "LEFO"); break;
	  case SCH$C_MWAIT: strcpy( state, "MWAIT"); break;
	  case SCH$C_PFW: strcpy( state, "PFW"); break;
	  case SCH$C_SUSP: strcpy( state, "SUSP"); break;
	  case SCH$C_SUSPO: strcpy( state, "SUSPO"); break;
	}
	return sts;
}


/*************************************************************************
*
* Name:		rttvms_get_pwr_proc()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get the pwr processes in the system.
*
**************************************************************************/
int	rttvms_get_pwr_proc( 
		int		*proc_pid_p,
		pwr_tString32	*proc_name_p,
		pwr_tString16	*proc_pidstr_p,
		int		*proc_pri_p,
		pwr_tString16	*proc_state_p,
		float		*proc_cputim_p,
		int		max_proc,
		int		*proc_count)
{
	int		i;
	int		sts;
	short 		uic[2];
	int		cputim;
	int		pwr_pid;
	int		neth_pid;
	int		msgh_pid;
	int		pop_pid;
	int		subproc[40];
	int		subproc_num;
	int		subproc_count;

	memset( proc_pid_p, 0, max_proc*sizeof(*proc_pid_p));
	memset( proc_name_p, 0, max_proc*sizeof(*proc_name_p));
	memset( proc_pidstr_p, 0, max_proc*sizeof(*proc_pidstr_p));
	memset( proc_pri_p, 0, max_proc*sizeof(*proc_pri_p));
	memset( proc_state_p, 0, max_proc*sizeof(*proc_state_p));
	memset( proc_cputim_p, 0, max_proc*sizeof(*proc_cputim_p));

	sts = rttvms_get_uic( (int *)uic);

	*proc_count = 0;
	for ( i = 0; i < 6; i++)
	{
	  if ( *proc_count == max_proc) break;

	  if ( i == 0)
	  {
	    sts = rttvms_get_pid_from_imagname( "DMQ$COM_SERVER.EXE", 
		proc_pid_p);
	    if ( ODD(sts))
	      sts = rttvms_get_procname( *proc_pid_p, (char *) proc_name_p);
	  }
	  if ( i == 1)
	  {
	    sprintf( (char *)proc_name_p, "* PWR_%o *", uic[1]);
	    sts = rttvms_get_pid( (char *)proc_name_p, proc_pid_p);
	  }
	  if ( i == 2)
	  {
	    sprintf( (char *)proc_name_p, "* Neth_%o *", uic[1]);
	    sts = rttvms_get_pid( (char *)proc_name_p, proc_pid_p);
	  }
	  if ( i == 3)
	  {
	    sprintf( (char *)proc_name_p, "* NethACP_%o *", uic[1]);
	    sts = rttvms_get_pid( (char *)proc_name_p, proc_pid_p);
	  }
	  if ( i == 4)
	  {
	    sprintf( (char *)proc_name_p, "* Msgh_%o *", uic[1]);
	    sts = rttvms_get_pid( (char *)proc_name_p, proc_pid_p);
	  }
	  if ( i == 5)
	  {
	    sts = rttvms_get_pid_from_imagname( "OP.EXE", 
		proc_pid_p);
	    if ( ODD(sts))
	      sts = rttvms_get_procname( *proc_pid_p, (char *) proc_name_p);
	  }

	  if ( EVEN(sts))
	  {
            if ( sts == SS$_NOPRIV)
	      strcpy( (char *)proc_name_p, "No privileges");
	    else
	      strcpy( (char *)proc_name_p, "Not started");
	    *proc_pid_p = 0;
	    strcpy( (char *)proc_pidstr_p, "");
	    *proc_pri_p = 0;
	    strcpy( (char *)proc_state_p, "");
	    *proc_cputim_p = 0;
	  }
	  else
	  {
	    sprintf( (char *)proc_pidstr_p, "%lx", *proc_pid_p);
	    sts = rttvms_get_procinfo( 
			*proc_pid_p, (char *)proc_state_p, proc_pri_p, &cputim);
	    if ( EVEN(sts)) return sts;
	    *proc_cputim_p = (float) cputim / 100.;
	  }
	  if ( i == 1)
	    pwr_pid = *proc_pid_p;
	  else if ( i == 2)
	    neth_pid = *proc_pid_p;
	  else if ( i == 4)
	    msgh_pid = *proc_pid_p;
	  else if ( i == 5)
	    pop_pid = *proc_pid_p;
	  proc_pid_p++;
	  proc_name_p++;
	  proc_pidstr_p++;
	  proc_pri_p++;
	  proc_state_p++;
	  proc_cputim_p++;
	  (*proc_count)++;
	}
	
	if ( pwr_pid )
	{
	  sts = rttvms_get_subproc( pwr_pid, subproc,
		sizeof(subproc)/sizeof(subproc[0]), &subproc_num);

	  for ( i = 0; i < subproc_num; i++)
	  {
	    if ( *proc_count == max_proc) break;
	    if ( subproc[i] == neth_pid || subproc[i] == msgh_pid ||
		 subproc[i] == pop_pid)
	      continue;

	    sts = rttvms_get_procname( subproc[i], (char *)proc_name_p);
	    if ( EVEN(sts))
	    {
              if ( sts == SS$_NOPRIV)
	        strcpy( (char *)proc_name_p, "No privs");
	      else
	        strcpy( (char *)proc_name_p, "Not started");
	      *proc_pid_p = 0;
	      strcpy( (char *)proc_pidstr_p, "");
	      *proc_pri_p = 0;
	      strcpy( (char *)proc_state_p, "");
	      *proc_cputim_p = 0;
	    }
	    else
	    {
	      *proc_pid_p = subproc[i];
	      sprintf( (char *)proc_pidstr_p, "%lx", *proc_pid_p);
	      sts = rttvms_get_procinfo( 
			*proc_pid_p, (char *)proc_state_p, proc_pri_p, &cputim);
	      if ( EVEN(sts)) return sts;
	      *proc_cputim_p = (float) cputim / 100.;
	    }
	    proc_pid_p++;
	    proc_name_p++;
	    proc_pidstr_p++;
	    proc_pri_p++;
	    proc_state_p++;
	    proc_cputim_p++;
	    subproc_count++;
	    (*proc_count)++;
	  }
	}	
	return 1;
}

/*
main()
{
	int		i;
	int		sts;
	short 		uic[2];
	unsigned long	transport_pid;
	int		pri;
	char		state[10];
	int		cputim;
	int		proc_pid[40];
	pwr_tString32	proc_name[40];
	pwr_tString16	proc_pidstr[40];
	int		proc_pri[40];
	pwr_tString16	proc_state[40];
	float		proc_cputim[40];
	int		proc_count;


	sts = rttvms_get_pwr_proc( proc_pid, proc_name, proc_pidstr, 
		proc_pri, proc_state, proc_cputim, 30, &proc_count);

	for ( i = 0; i < proc_count; i++)
	  printf( "%s    %s    %d    %s    %f\n", proc_name[i], proc_pidstr[i],
			proc_pri[i], proc_state[i], proc_cputim[i]);
}
*/
#endif

#ifdef OS_ELN

#define NOJOBPTR 0x80001000

/*************************************************************************
*
* Name:		rtteln_get_nodename()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get nodename for the current system.
*
**************************************************************************/
static int	rtteln_get_nodename( char *nodename, int size)
{
	globalref VARYING_STRING (6) ker$gt_node_name;

	VARYING_TO_CSTRING (ker$gt_node_name, nodename);
	return 1;
}

/*************************************************************************
*
* Name:		rtteln_get_pwr_proc()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get the eln jobs.
*
**************************************************************************/
int	rtteln_get_jobs( 
		int		*proc_pid_p,
		pwr_tString32	*proc_name_p,
		int		*proc_pri_p,
		pwr_tString16	*proc_state_p,
		float		*proc_cputim_p,
		int		max_proc,
		int		*proc_count)
{
	int		i;
	int		sts;
	struct jcb 	*jcb_ptr;
	struct jcb 	*test_jcb_ptr;
	struct jcb 	*next_jcb_ptr;
	struct jcb 	*first_jcb_ptr;
	char		program_name[80];
	struct pcb 	*pcb_ptr;
	int		joffs;
	int		poffs;
	char		*flink;


	memset( proc_pid_p, 0, max_proc*sizeof(*proc_pid_p));
	memset( proc_name_p, 0, max_proc*sizeof(*proc_name_p));
	memset( proc_pri_p, 0, max_proc*sizeof(*proc_pri_p));
	memset( proc_state_p, 0, max_proc*sizeof(*proc_state_p));
	memset( proc_cputim_p, 0, max_proc*sizeof(*proc_cputim_p));

	ker$get_jcb(&sts, &jcb_ptr);
	if ( EVEN(sts)) return sts;

        joffs = &jcb_ptr->jcb$a_job_flink;
	joffs = joffs - (int)jcb_ptr;
	pcb_ptr = jcb_ptr->jcb$a_current_pcb;
	poffs = &pcb_ptr->pcb$a_process_flink;
	poffs = poffs - (int)pcb_ptr;

	/* Get the first jcb */
	test_jcb_ptr = (char *)jcb_ptr->jcb$a_job_flink - joffs; 
	while( test_jcb_ptr > NOJOBPTR )
	{
	  next_jcb_ptr = test_jcb_ptr;
	  test_jcb_ptr = (char *)next_jcb_ptr->jcb$a_job_flink - joffs;
	}
	first_jcb_ptr = next_jcb_ptr;

	*proc_count = 0;
	/* Get info from the jbc */
	do {
	  if ( *proc_count == max_proc) break;

	  strcpy( proc_name_p, next_jcb_ptr->jcb$a_program->prg$t_name.string_text);
	  *proc_pid_p = next_jcb_ptr->jcb$w_generation;
	  switch( next_jcb_ptr->jcb$b_state)
	  {
	    case JCB$K_WAITING: strcpy( proc_state_p, "WAIT"); break;
	    case JCB$K_READY: strcpy( proc_state_p, "READY"); break;
	    case JCB$K_RUNNING: strcpy( proc_state_p, "RUN"); break;
	    default: strcpy( proc_state_p, "");
	  }
	  *proc_pri_p = next_jcb_ptr->jcb$b_priority;

	  pcb_ptr = (char *)next_jcb_ptr->jcb$a_process_flink - poffs;
	  flink = 0;
	  *proc_cputim_p = (float)pcb_ptr->pcb$l_job_cpu_time / 100.;
	  while ( flink != next_jcb_ptr->jcb$a_process_flink)
	  {
	    *proc_cputim_p += (float)pcb_ptr->pcb$l_cpu_time / 100.;
	    pcb_ptr = (char *)pcb_ptr->pcb$a_process_flink - poffs;
	    flink = pcb_ptr->pcb$a_process_flink;
	  }
	  proc_pid_p++;
	  proc_name_p++;
	  proc_pri_p++;
	  proc_state_p++;
	  proc_cputim_p++;
	  (*proc_count)++;
	  
	  next_jcb_ptr = (char *)next_jcb_ptr->jcb$a_job_blink - joffs;
	} while ( next_jcb_ptr > NOJOBPTR);

	return 1;
}

/*************************************************************************
*
* Name:		rtteln_get_proc_info()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get info from one eln job.
*
**************************************************************************/
int	rtteln_get_job_info( 
		int		job_generation,
		pwr_tString32	*proc_name_p,
		int		*proc_pri_p,
		pwr_tString16	*proc_state_p,
		float		*proc_cputim_p,
		pwr_tString	*proc_filename_p,
		char		*pgm_mode,
		int		*user_stack,
		int		*kernel_stack,
		char		*optionflags)
{
	int		i;
	int		sts;
	struct jcb 	*jcb_ptr;
	struct jcb 	*test_jcb_ptr;
	struct jcb 	*next_jcb_ptr;
	struct jcb 	*first_jcb_ptr;
	char		program_name[80];
	struct pcb 	*pcb_ptr;
	struct prg 	*pgm_ptr;
	int		joffs;
	int		poffs;
	char		*flink;
	int		found;

	ker$get_jcb(&sts, &jcb_ptr);
	if ( EVEN(sts)) return sts;

        joffs = &jcb_ptr->jcb$a_job_flink;
	joffs = joffs - (int)jcb_ptr;
	pcb_ptr = jcb_ptr->jcb$a_current_pcb;
	poffs = &pcb_ptr->pcb$a_process_flink;
	poffs = poffs - (int)pcb_ptr;

	/* Get the first jcb */
	test_jcb_ptr = (char *)jcb_ptr->jcb$a_job_flink - joffs; 
	while( test_jcb_ptr > NOJOBPTR )
	{
	  next_jcb_ptr = test_jcb_ptr;
	  test_jcb_ptr = (char *)next_jcb_ptr->jcb$a_job_flink - joffs;
	}
	first_jcb_ptr = next_jcb_ptr;

	found = 0;
	/* Find the jbc */
	do {

	  pcb_ptr = (char *)next_jcb_ptr->jcb$a_process_flink - poffs;
	  flink = 0;
	  if ( job_generation == next_jcb_ptr->jcb$w_generation)
	  {
	    pgm_ptr = next_jcb_ptr->jcb$a_program;
	    strncpy( proc_name_p, pgm_ptr->prg$t_name.string_text, 79);
	    strncpy( proc_filename_p, 
	      pgm_ptr->prg$r_filename_overlay.prg$t_filename.string_text, 79);
	    switch( next_jcb_ptr->jcb$b_state)
	    {
	      case JCB$K_WAITING: strcpy( proc_state_p, "WAIT"); break;
	      case JCB$K_READY: strcpy( proc_state_p, "READY"); break;
	      case JCB$K_RUNNING: strcpy( proc_state_p, "RUN"); break;
	      default: strcpy( proc_state_p, "");
	    }
	    *proc_pri_p = next_jcb_ptr->jcb$b_priority;

	    pcb_ptr = (char *)next_jcb_ptr->jcb$a_process_flink - poffs;
	    flink = 0;
	    *proc_cputim_p = (float)pcb_ptr->pcb$l_job_cpu_time / 100.;
	    while ( flink != next_jcb_ptr->jcb$a_process_flink)
	    {
	      *proc_cputim_p += (float)pcb_ptr->pcb$l_cpu_time / 100.;
	      pcb_ptr = (char *)pcb_ptr->pcb$a_process_flink - poffs;
	      flink = pcb_ptr->pcb$a_process_flink; 
	    }	  
	    if ( !pgm_ptr->prg$b_mode)
	      strcpy( pgm_mode, "KERNEL");
	    else
	      strcpy( pgm_mode, "USER");
	    *kernel_stack = pgm_ptr->prg$w_kernel_stack;
	    *user_stack = pgm_ptr->prg$w_user_stack;
	    strcpy( optionflags, "");
	    if ( pgm_ptr->prg$b_option_flags.prg$v_auto_start)
	      strcat( optionflags, "AUTOSTART ");
	    if ( pgm_ptr->prg$b_option_flags.prg$v_seq_initial)
	      strcat( optionflags, "INIT ");
	    if ( pgm_ptr->prg$b_option_flags.prg$v_start_debug)
	      strcat( optionflags, "DEBUG ");
	    if ( pgm_ptr->prg$b_option_flags.prg$v_dynamic_program)
	      strcat( optionflags, "DYN ");
	    if ( pgm_ptr->prg$b_option_flags.prg$v_debug_warm)
	      strcat( optionflags, "DBGWARM ");
	    found = 1;
	    break;
	  }
	  next_jcb_ptr = (char *)next_jcb_ptr->jcb$a_job_blink - joffs;
	} while ( next_jcb_ptr > NOJOBPTR);

	if ( !found)
	  return 0;

	return 1;
}

/*************************************************************************
*
* Name:		rtteln_get_job_proc()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get the pwr processes in the system.
*
**************************************************************************/
int	rtteln_get_job_proc( 
		int		job_generation,
		int		*proc_generation_p,
		pwr_tString32	*proc_name_p,
		int		*proc_pri_p,
		pwr_tString16	*proc_state_p,
		float		*proc_cputim_p,
		int		max_proc,
		int		*proc_count)
{
	int		i;
	int		sts;
	struct jcb 	*jcb_ptr;
	struct jcb 	*test_jcb_ptr;
	struct jcb 	*next_jcb_ptr;
	struct jcb 	*first_jcb_ptr;
	char		program_name[80];
	struct pcb 	*pcb_ptr;
	int		joffs;
	int		poffs;
	char		*flink;
	int		found;


	memset( proc_generation_p, 0, max_proc*sizeof(*proc_generation_p));
	memset( proc_name_p, 0, max_proc*sizeof(*proc_name_p));
	memset( proc_pri_p, 0, max_proc*sizeof(*proc_pri_p));
	memset( proc_state_p, 0, max_proc*sizeof(*proc_state_p));
	memset( proc_cputim_p, 0, max_proc*sizeof(*proc_cputim_p));

	ker$get_jcb(&sts, &jcb_ptr);
	if ( EVEN(sts)) return sts;

        joffs = &jcb_ptr->jcb$a_job_flink;
	joffs = joffs - (int)jcb_ptr;
	pcb_ptr = jcb_ptr->jcb$a_current_pcb;
	poffs = &pcb_ptr->pcb$a_process_flink;
	poffs = poffs - (int)pcb_ptr;

	/* Get the first jcb */
	test_jcb_ptr = (char *)jcb_ptr->jcb$a_job_flink - joffs; 
	while( test_jcb_ptr > NOJOBPTR )
	{
	  next_jcb_ptr = test_jcb_ptr;
	  test_jcb_ptr = (char *)next_jcb_ptr->jcb$a_job_flink - joffs;
	}
	first_jcb_ptr = next_jcb_ptr;

	*proc_count = 0;
	found = 0;
	/* Find the jbc */
	do {

	  pcb_ptr = (char *)next_jcb_ptr->jcb$a_process_flink - poffs;
	  flink = 0;
	  if ( job_generation == next_jcb_ptr->jcb$w_generation)
	  {
	    while ( flink != next_jcb_ptr->jcb$a_process_flink)
	    {
	      if ( *proc_count == max_proc) break;

	      *proc_cputim_p = (float)pcb_ptr->pcb$l_cpu_time / 100.;
	      strcpy( proc_name_p, "");
	      *proc_generation_p = pcb_ptr->pcb$w_generation;
	      switch( pcb_ptr->pcb$b_state)
	      {
	        case PCB$K_WAITING: strcpy( proc_state_p, "WAIT"); break;
	        case PCB$K_READY: strcpy( proc_state_p, "READY"); break;
	        case PCB$K_RUNNING: strcpy( proc_state_p, "RUN"); break;
	        case PCB$K_SUSPENDED: strcpy( proc_state_p, "SUSP"); break;
	        default: strcpy( proc_state_p, "");
	      }
	      if ( pcb_ptr->pcb$b_reason.pcb$v_signal_debug)
	        strcpy( proc_state_p, "DEBUG");
	      *proc_pri_p = pcb_ptr->pcb$b_priority;

	      proc_generation_p++;
	      proc_name_p++;
	      proc_pri_p++;
	      proc_state_p++;
	      proc_cputim_p++;
	      (*proc_count)++;

	      pcb_ptr = (char *)pcb_ptr->pcb$a_process_flink - poffs;
	      flink = pcb_ptr->pcb$a_process_flink; 
	    }
	    found = 1;
	    break;
	  }
	  next_jcb_ptr = (char *)next_jcb_ptr->jcb$a_job_blink - joffs;
	} while ( next_jcb_ptr > NOJOBPTR);

	if ( !found)
	  return 0;

	return 1;
}
#endif


/*************************************************************************
*
* Name:		rtt_get_platform()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get platform for the current system.
*
**************************************************************************/
int	rtt_get_platform( char *platform)
{
#if defined(OS_VMS)
	int		sts;
	char		hw_name[20];
	unsigned short	hw_name_len;
	rtt_t_itemlist	item_list[2] = {{ 0,SYI$_HW_NAME,0,0},
					{ 0,0,0,0}};
 	int    		pri_len;
  
	item_list[0].lenadr = (unsigned int) &hw_name_len;
	item_list[0].bufadr = (unsigned int) &hw_name;
	item_list[0].buflen = sizeof( hw_name);

	sts = sys$getsyiw(0, NULL, NULL, &item_list, 0, NULL, 0);
	if (EVEN(sts)) return sts;
	hw_name[hw_name_len] = 0;
	if ( strstr(hw_name, "VAX"))
	  strcpy( platform, "VAX_VMS");
	else
	  strcpy( platform, "AXP_VMS");
	return 1;
#elif defined(OS_ELN)
	strcpy( platform, "VAX_ELN");
	return 1;
#elif defined(OS_LYNX) || defined(OS_LINUX)
	struct	utsname buf;

	if (uname( &buf) == -1)
	  return 0;
	if (strstr( buf.machine, "86") != 0)
	  strcpy( platform, "X86_LNX");
	return 1;
#endif
}
/*************************************************************************
*
* Name:		rtt_get_hw()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get hardware for the current system.
*
**************************************************************************/
int	rtt_get_hw( char *hw)
{
#if defined(OS_VMS)
	int		sts;
	char		hw_name[20];
	unsigned short	hw_name_len;
	rtt_t_itemlist	item_list[2] = {{ 0,SYI$_HW_NAME,0,0},
					{ 0,0,0,0}};
 	int    		pri_len;
  
	item_list[0].lenadr = (unsigned int) &hw_name_len;
	item_list[0].bufadr = (unsigned int) &hw_name;
	item_list[0].buflen = sizeof( hw_name);

	sts = sys$getsyiw(0, NULL, NULL, &item_list, 0, NULL, 0);
	if (EVEN(sts)) return sts;
	hw_name[hw_name_len] = 0;
	if ( strstr(hw_name, "VAX"))
	  strcpy( hw, "vax");
	else
	  strcpy( hw, "axp");
	return 1;
#elif defined(OS_ELN)
	strcpy( hw, "vax");
	return 1;
#elif defined(OS_LYNX) || defined(OS_LINUX)
	struct	utsname buf;

	if (uname( &buf) == -1)
	  return 0;
	if (strstr( buf.machine, "86") != 0)
	  strcpy( hw, "x86");
	return 1;
#endif
}

#if defined (OS_LYNX) || defined(OS_LINUX)

/*************************************************************************
*
* Name:		rtt_replace_env()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Replace env variables in UNIX filenames.
*	A string that starts with $ and ends with . or / will replaced 
*	if then string is found by getenv. Only lower case variables 
*	will be detected.
*
**************************************************************************/
int     rtt_replace_env( char *str, char *newstr)
{
        char    *s;
        char    *t;
        char    *u;
        int     symbolmode;
        int     size;
        char    *value;
        char    symbol[80];
        char    lower_symbol[80];
        char    new[160];

        symbolmode = 0;
        s = str;
        t = new;

        while ( *s != 0)
        {
          if (*s == '$')
          {
            symbolmode = 1;
            u = s + 1;
            *t = *s;
            t++;
          }
          else if (symbolmode && (*s == '/' || *s == '.'))
          {
            /* End of potential symbol */
            size = (int) s - (int) u;
            strncpy( symbol, u, size);
            symbol[size] = 0;
            cdh_ToLower( lower_symbol, symbol);
            if ( (value = getenv( lower_symbol)) == NULL)
            {
              /* It was no symbol */
              *t = *s;
              t++;
            }
            else
            {
              /* Symbol found */
              t -= strlen(symbol) + 1;
              strcpy( t, value);
              t += strlen(value);
              *t = *s;
              t++;
            }
            symbolmode = 0;
          }
          else
          {
            *t = *s;
            t++;
          }
          s++;
        }

	if ( symbolmode)
	{
	  strcpy( t, u);
	}
	else
	  *t = 0;

        strcpy( newstr, new);
        return 1;
}
#endif

