#ifndef rt_cvolsm_h
#define rt_cvolsm_h

/* rt_cvolsm.h -- Cached volumes, server monitor API.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   This module contains the cache handling routines.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

gdb_sMountedOn *
cvolsm_AddMountedOn (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid,
  gdb_sNode		*np
);

void
cvolsm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
cvolsm_GetObjectInfo (
  qcom_sGet		*get
);

void
cvolsm_NameToObject (
  qcom_sGet		*get
);

void
cvolsm_OidToObject (
  qcom_sGet		*get
);

void
cvolsm_RemoveMountedOn (
  pwr_tStatus		*sts,
  gdb_sMountedOn	*mop
);

void
cvolsm_SetObjectInfo (
  qcom_sGet		*get
);

#endif
