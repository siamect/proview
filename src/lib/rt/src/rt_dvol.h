#ifndef rt_dvol_h
#define rt_dvol_h

/* rt_dvol.h -- Dynamic volumes

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   .  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_vol_h
#include "rt_vol.h"
#endif

#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

gdb_sObject *
dvol_AdoptObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sObject		*p_op,
  net_sNotify		*nmp
);

gdb_sObject *
dvol_CreateObject (
  pwr_tStatus		*sts,
  cdh_sParseName	*pn,
  pwr_tClassId		cid,
  pwr_tUInt32		size,
  pwr_tObjid		oid		/* Requested objid, */
);

pwr_tBoolean
dvol_DeleteObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid
);

gdb_sObject *
dvol_MoveObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid,
  pwr_tObjid		poid
);

gdb_sObject *
dvol_RenameObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid,
  cdh_sParseName	*pn
);

gdb_sObject *
dvol_UnadoptObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sObject		*pop,
  net_sNotify		*nmp
);

#endif
