#ifndef rt_cvolcm_h
#define rt_cvolcm_h

/* rt_cvolcm.h -- Cached volumes, server monitor API.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   This module contains the cache handling routines.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

void 
cvolcm_AddClassVolume(
  pwr_tStatus		*sts,
  gdb_sNode		*np,
  const net_sGvolume	*vp
);

gdb_sVolume *
cvolcm_ConnectVolume (
  pwr_tStatus	*sts,
  gdb_sVolume	*vp,
  net_sGvolume	*gvp,
  gdb_sNode	*np
);

void
cvolcm_CreateObject (
  qcom_sGet	*get
);

void
cvolcm_DeleteObject (
  qcom_sGet	*get
);

void
cvolcm_FlushNode (
  pwr_tStatus	*sts,
  gdb_sNode	*np 
);

void
cvolcm_FlushVolume (
  pwr_tStatus	*sts,
  gdb_sVolume	*vp 
);

void
cvolcm_MoveObject (
  qcom_sGet	*get
);

void
cvolcm_RenameObject (
  qcom_sGet	*get
);

void
cvolcm_TrimOld (
);

#endif
