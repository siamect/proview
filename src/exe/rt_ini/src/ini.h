#ifndef ini_h
#define ini_h

/* ini.h -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   */

#include "pwr.h"
#include "pwr_class.h"
#include "pwrb_c_plcprocess.h"
#include "co_tree.h"
#include "co_dbs.h"
#include "rt_ivol.h"
#include "rt_lst.h"
#include "rt_qini.h"
#include "rt_proc.h"
#include "rt_errh.h"

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( ignoreWarning , 1),
    pwr_Bits( ignoreError   , 1),
    pwr_Bits( ignoreFatal   , 1),
    pwr_Bits( verbose	    , 1),
    pwr_Bits( restart	    , 1),
    pwr_Bits( fill_0	    , 3),,,

    pwr_Bits( interactive   , 1),
    pwr_Bits( busid	    , 1),
    pwr_Bits( hostname	    , 1),
    pwr_Bits( nodename	    , 1),
    pwr_Bits( fill_1	    , 4),,,,

    pwr_Bits( applfile	    , 1),
    pwr_Bits( aliasfile	    , 1),
    pwr_Bits( bootfile	    , 1),
    pwr_Bits( plcfile	    , 1),
    pwr_Bits( nodefile	    , 1),
    pwr_Bits( fill_2	    , 3),,,
    pwr_Bits( fill_3	    , 8),,,,,,,
  ) b;

#define ini_mContext__			0
#define ini_mContext_ignoreWarning	pwr_Bit(0)
#define ini_mContext_ignoreError	pwr_Bit(1)
#define ini_mContext_ignoreFatal	pwr_Bit(2)
#define ini_mContext_verbose		pwr_Bit(3)
#define ini_mContext_restart		pwr_Bit(4)

#define ini_mContext_interactive	pwr_Bit(8)
#define ini_mContext_busid		pwr_Bit(9)
#define ini_mContext_hostname		pwr_Bit(10)
#define ini_mContext_nodename		pwr_Bit(11)

#define ini_mContext_applfile		pwr_Bit(16)
#define ini_mContext_aliasfile		pwr_Bit(17)
#define ini_mContext_bootfile		pwr_Bit(18)
#define ini_mContext_plcfile		pwr_Bit(19)
#define ini_mContext_nodefile		pwr_Bit(20)

#define ini_mContext_			(~ini_mContext__)
} ini_mContext;
 
typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( neth	    , 1),
    pwr_Bits( qmon	    , 1),
    pwr_Bits( plc	    , 1),
    pwr_Bits( fill_0	    , 5),,,,,

    pwr_Bits( run	    , 1),
    pwr_Bits( running	    , 1),
    pwr_Bits( reap	    , 1),
    pwr_Bits( fill_1	    , 5),,,,,

    pwr_Bits( fill_2	    , 8),,,,,,,,

    pwr_Bits( inited	    , 1),
    pwr_Bits( fill_3	    , 8),,,,,,
  ) b;

#define ini_mProc__		0
#define ini_mProc_neth		pwr_Bit(0)
#define ini_mProc_qmon		pwr_Bit(1)
#define ini_mProc_plc		pwr_Bit(2)

#define ini_mProc_run		pwr_Bit(8)
#define ini_mProc_running	pwr_Bit(9)
#define ini_mProc_reap		pwr_Bit(10)

#define ini_mProc_inited	pwr_Bit(24)

#define ini_mProc_		(~ini_mProc__)
} ini_mProc;
 
typedef struct {
  char name[256];
  int *errcount;
  void (*logOpenFail)(errh_sLog*, char*, ...);
} ini_sFile;

typedef struct {
  tree_sNode	t;
  lst_sEntry	proc_ll;
  char		id[40];
  ini_mProc	flags;
  proc_sProcess	proc;
} ini_sProc;

typedef struct {
  ini_mContext	flags;
  char		alias[80];
  char		nodename[80];
  char		nodespec[80];
  char		hostname[80];
  char		hostspec[80];
  char		bootdisk[80];
  char		proj[80];
  char		group[80];
  char		console[80];
  ini_sFile	aliasfile;
  ini_sFile	applfile;
  ini_sFile	bootfile;
  ini_sFile	nodefile;
  ini_sFile	plcfile;
  int		busid;
  char		dir[256];
  int		warnings;
  int		errors;
  int		fatals;
  ivol_sNode	node;
  qini_sNode	*me;
#if 0
//  ivol_sVolume	*vp;		/* current volume */
#endif
  tree_sTable	*nid_t;
#if 0
  tree_sTable	*vname_t;
#else
  tree_sTable	*vid_t;
#endif
  lst_sEntry	vol_lh;
  co_mFormat    format;
  dbs_sEnv      dbs;
  dbs_sSect	sect;
  gdb_sInit	gdbInit;
  tree_sTable	*proc_t;
  lst_sEntry	proc_lh;
  ini_sProc	*plc;
  qcom_sQid	myQ;
  qcom_sQid	yourQ;
  qcom_sQid	eventQ;
  errh_sLog	log;
  pwr_sNode	*np;
  pwr_sClass_PlcProcess *PlcProcess;
} ini_sContext;

pwr_tBoolean	ini_BuildNode		(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_CheckNode		(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_CheckVolumeFile	(pwr_tStatus*, ini_sContext*, ivol_sVolume*);
ini_sContext   *ini_CheckContext	(pwr_tStatus*, ini_sContext*);
ini_sContext   *ini_CreateContext	(pwr_tStatus*);
pwr_tBoolean	ini_CreateDb		(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_DecodeBodies	(pwr_tStatus*, ini_sContext*, pwr_tBoolean);
pwr_tBoolean	ini_FreeBodies		(pwr_tStatus*, ini_sContext*, pwr_tBoolean);
pwr_tBoolean	ini_IterVolumes		(pwr_tStatus*, ini_sContext*,
			pwr_tBoolean (*func)(pwr_tStatus*, ini_sContext*, ivol_sVolume*));
char *		ini_LoadDirectory	(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_LoadNode		(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_LoadVolume		(pwr_tStatus*, ini_sContext*, ivol_sVolume*);
FILE           *ini_OpenFile		(pwr_tStatus*, ini_sContext*, ini_sFile*);
ini_sProc      *ini_ProcInsert		(pwr_tStatus*, ini_sContext*, char*, char*, int, int, char*, int, int, char*);
void		ini_ProcIter		(pwr_tStatus*, ini_sContext*, int,
			void	(*func)	(pwr_tStatus*, ini_sContext*, ini_sProc*));
void		ini_ProcLoad		(pwr_tStatus*, ini_sContext*, ini_sProc*);
void		ini_ProcStart		(pwr_tStatus*, ini_sContext*, ini_sProc*);
void		ini_ProcTable		(pwr_tStatus*, ini_sContext*);
void		ini_ProcWait		(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_RebuildNode		(pwr_tStatus*, ini_sContext*);
void		ini_ReadBootFile	(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_ReadNodeFile	(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_ReloadNode		(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_ReloadVolume	(pwr_tStatus*, ini_sContext*, ivol_sVolume*);
pwr_tBoolean	ini_UpdateBodies	(pwr_tStatus*, ini_sContext*, pwr_tBoolean);
pwr_tBoolean	ini_UpdateDatabase	(pwr_tStatus*, ini_sContext*);

#endif
