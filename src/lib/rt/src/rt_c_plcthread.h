/* rt_c_plcthread.h -- <short description>

   PROVIEW/R
   Copyright (C) 1997,99 by Mandator AB.

   Functions for the class PlcThread.

    */

#include "pwr.h"
#include "pwr_class.h"
#include "rt_plc.h"

#ifndef pwr_cClass_PlcThread
# include "pwrb_c_plcthread.h"
#endif

pwr_sClass_PlcThread *
pwrb_PlcThread_Init (
  pwr_tStatus	*sts,
  plc_sThread	*tp
);

void
pwrb_PlcThread_Zero (
  plc_sThread	*tp
);

void
pwrb_PlcThread_Exec (
  plc_sThread *tp
);
