#ifndef co_time_loop_h
#define co_time_loop_h
/* co_time_loop.h --

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This include file contains definitions and function prototypes
   needed to use timed loops functions.
*/

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_time_h
# include "co_time.h"
#endif


typedef struct {
  pwr_tDeltaTime        PreviousTime;
  pwr_tDeltaTime        DeltaTime;
  int                   TimerFlag;
} time_sLoop;


pwr_tDeltaTime *
time_GetUpTime (
  pwr_tStatus           *sts,
  pwr_tDeltaTime        *tp,
  pwr_tDeltaTime        *ap
);

pwr_tStatus
time_LoopInit(
  time_sLoop            *ls,
  pwr_tFloat32          scantime
);

pwr_tBoolean
time_LoopWait (
  time_sLoop            *ls
);

#if defined OS_ELN
pwr_tBoolean
time_LoopWaitEvent (
  time_sLoop            *ls,
  int                   *WaitResult,
  unsigned long         Event
);
#endif

#endif
