#ifndef rt_qos_h
#define rt_qos_h

/* rt_qdb.h -- Queue communication

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   
   .  */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qdb_h
#include "rt_qdb.h"
#endif

qdb_sQlock *
qos_CreateQlock (
  pwr_tStatus	*status,
  qdb_sQue	*que
);

void
qos_DeleteQlock (
  pwr_tStatus	*status,
  qdb_sQue	*que
);

pwr_tBoolean
qos_WaitQue (
  pwr_tStatus	*status,
  qdb_sQue	*que,
  int		tmo
);

pwr_tBoolean
qos_SignalQue (
  pwr_tStatus	*sts,
  qdb_sQue	*que
);

void
qos_Platform (
  qdb_sNode	*node
);

#endif
