#ifndef rt_subcm_h
#define rt_subcm_h

/* rt_subcm.h -- Object data subscription, client monitor functions.

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.
   
   <Description.>  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_pool_h
#include "rt_pool.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_sub_h
#include "rt_sub.h"
#endif

void
subcm_CheckTimeout ();

void
subcm_Data (
  qcom_sGet		*get
);

void
subcm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

#endif
