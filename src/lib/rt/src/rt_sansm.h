#ifndef rt_sansm_h
#define rt_sansm_h

/* rt_sansm.h -- Subscribed alarm (and block) notification, server monitor calls.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_net_h
# include "rt_net.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#ifndef rt_gdb_h
# include "rt_gdb.h"
#endif

void
sansm_Add (
  qcom_sGet		*get
);

void
sansm_Check ();

void
sansm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
sansm_Remove (
  qcom_sGet		*get
);

pwr_tUInt32
sansm_Update (
  gdb_sNode		*np
);

#endif
