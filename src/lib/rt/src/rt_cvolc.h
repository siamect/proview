#ifndef rt_cvolc_h
#define rt_cvolc_h

/* rt_cvolc.h -- Cached volumes, client calls.

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   .  */


#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

/* Callable functions.  */

void *
cvolc_GetObjectInfo (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const pwr_sAttrRef	*arp,
  gdb_sCclass		*ccp,
  const pwr_sAttrRef	*rarp,
  pwr_tUInt32		ridx,
  mvol_sAttribute	*ap,
  void			*p,
  int			size
);

void
cvolc_LockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
);

gdb_sObject *
cvolc_NameToObject (
  pwr_tStatus		*sts,
  gdb_sObject		*p_op,
  cdh_sParseName	*pn,
  int			index,
  pwr_tBitMask		trans
);

gdb_sObject *
cvolc_OidToObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  pwr_tObjid		oid,
  pwr_tBitMask		trans,
  cvol_eHint		hint
);

pwr_tBoolean
cvolc_SetObjectInfo (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const pwr_sAttrRef	*arp,
  const gdb_sCclass	*ccp,
  const pwr_sAttrRef	*rarp,
  pwr_tUInt32		ridx,
  mvol_sAttribute	*ap,
  const void		*p,
  int			size
);

void
cvolc_TouchObject (
  gdb_sObject		*op
);

void
cvolc_UnlockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
);

#endif
