/* 
 * Proview   $Id: ini.h,v 1.2 2008-10-31 12:51:30 claes Exp $
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

#ifndef ini_h
#define ini_h

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_tree.h"
#include "co_dbs.h"
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
    pwr_Bits( stop	    , 1),
    pwr_Bits( fill_0	    , 2),,,

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
    pwr_Bits( fill_3	    , 8),,,,,,
  ) b;

#define ini_mContext__			0
#define ini_mContext_ignoreWarning	pwr_Bit(0)
#define ini_mContext_ignoreError	pwr_Bit(1)
#define ini_mContext_ignoreFatal	pwr_Bit(2)
#define ini_mContext_verbose		pwr_Bit(3)
#define ini_mContext_restart		pwr_Bit(4)
#define ini_mContext_stop		pwr_Bit(5)

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
  void (*logOpenFail)(errh_sLog*, const char*, ...);
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
  int		busid;
  char		dir[256];
  char		bdir[256]; /* Base dir (pwr_load) */
  int		warnings;
  int		errors;
  int		fatals;
  qini_sNode	*me;
  tree_sTable	*nid_t;
  co_mFormat    format;
  dbs_sEnv      dbs;
  dbs_sSect	sect;
  tree_sTable	*proc_t;
  lst_sEntry	proc_lh;
  ini_sProc	*plc;
  qcom_sQid	myQ;
  qcom_sQid	yourQ;
  qcom_sQid	eventQ;
  errh_sLog	log;
} ini_sContext;

ini_sContext   *ini_CheckContext	(pwr_tStatus*, ini_sContext*);
ini_sContext   *ini_CreateContext	(pwr_tStatus*);
char *		ini_LoadDirectory	(pwr_tStatus*, ini_sContext*);
FILE           *ini_OpenFile		(pwr_tStatus*, ini_sContext*, ini_sFile*);
ini_sProc      *ini_ProcInsert		(pwr_tStatus*, ini_sContext*, char*, char*, int, int, char*, int, int, char*);
void		ini_ProcIter		(pwr_tStatus*, ini_sContext*, int,
			void	(*func)	(pwr_tStatus*, ini_sContext*, ini_sProc*));
void		ini_ProcStart		(pwr_tStatus*, ini_sContext*, ini_sProc*);
void		ini_ProcPrio		(pwr_tStatus*, ini_sContext*, ini_sProc*);
void		ini_ProcTable		(pwr_tStatus*, ini_sContext*);
void		ini_ProcWait		(pwr_tStatus*, ini_sContext*);
void		ini_ReadBootFile	(pwr_tStatus*, ini_sContext*);
pwr_tBoolean	ini_ReadNodeFile	(pwr_tStatus*, ini_sContext*);

#endif


