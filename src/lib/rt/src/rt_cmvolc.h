#ifndef rt_cmvolc_h
#define rt_cmvolc_h

/* rt_cmvol.h -- Cached volumes

   PROVIEW/R
   Copyright (C) 1996-2002 by Cell Network AB.

   This module contains the meta cache handling routines.  */


#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_net_h
#include "rt_net.h"
#endif

#define cmvolc_AssumeLocked(ccp) pwr_Assert(ccp->flags.b.cacheLock)


gdb_sCclass *
cmvolc_GetCachedClass (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const gdb_sVolume	*vp,
  mvol_sAttribute	*ap,
  pwr_tBoolean		*equal, /**< set if classes are equal then NULL is returned */
  pwr_tBoolean		*fetched /**< true if the class has been fected from the remote node */
  );

void
cmvolc_GetNonExistingClass (
  pwr_tStatus		*sts,
  gdb_sObject		*op
  );


void
cmvolc_LockClass (
  pwr_tStatus		*sts,
  gdb_sCclass		*ccp
  );


void
cmvolc_TouchClass (
  gdb_sCclass		*ccp
  );


void
cmvolc_UnlockClass (
  pwr_tStatus		*sts,
  gdb_sCclass		*ccp
  );



#endif
