#ifndef rt_cvols_h
#define rt_cvols_h

/* rt_cvols.h -- Cached volumes, server calls.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   .  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_net_h
#include "rt_net.h"
#endif

#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

cvol_sNotify *
cvols_InitNotify (
  gdb_sObject		*op,
  cvol_sNotify		*nmp,
  net_eMsg		type
);

void
cvols_Notify (
  cvol_sNotify		*nmp
);

#endif
