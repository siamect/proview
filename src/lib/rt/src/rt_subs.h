#ifndef rt_subs_h
#define rt_subs_h

/* rt_subs.h -- Object data subscription, server side

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_sub_h
#include "rt_sub.h"
#endif

void
subs_DeleteServer (
  sub_sServer		*sp
);

void
subs_UnlinkObject (
  gdb_sObject		*op
);

#endif
