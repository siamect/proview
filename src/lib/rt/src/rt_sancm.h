#ifndef rt_sancm_h
#define rt_sancm_h

/* rt_sanc.h -- Subscribed alarm (and block) notification, client monitor calls.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#ifndef rt_net_h
# include "rt_net.h"
#endif

#ifndef rt_gdb_h
# include "rt_gdb.h"
#endif

void
sancm_Add (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
sancm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);
			        
void
sancm_MoveExpired(
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
sancm_Remove (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
sancm_Update (
  qcom_sGet		*get
);

#endif
