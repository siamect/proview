#ifndef rt_sanc_h
#define rt_sanc_h

/* rt_sanc.h -- Subscribed alarm (and block) notification, client calls.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_gdb_h
# include "rt_gdb.h"
#endif

pwr_tBoolean
sanc_Subscribe (
  pwr_tStatus		*status,
  gdb_sObject		*op
);

void
sanc_SubscribeMountServers (
  pwr_tStatus		*status,
  gdb_sNode		*np
);

#endif
