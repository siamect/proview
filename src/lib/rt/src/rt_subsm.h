#ifndef rt_subsm_h
#define rt_subsm_h

/* rt_subsm.h -- Object data subscription, server side

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
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
subsm_ActivateBuffer (
  sub_sBuffer		*bp,
  pwr_tUInt32		dt
);

void
subsm_Add (
  qcom_sGet		*get
);

void
subsm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
subsm_Remove (
  qcom_sGet		*get
);

pwr_tBoolean
subsm_SendBuffer (
  sub_sBuffer		*bp
);

#endif
