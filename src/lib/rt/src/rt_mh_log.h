#ifndef rt_mh_log_h
#define rt_mh_log_h

/* rt_mh_log.h -- Log utilities

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_mh_def_h
#include "rt_mh_def.h"
#endif

#ifndef rt_mh_outunit_h
#include "rt_mh_outunit.h"
#endif

#define mhLog 0


FILE *mh_LogFileOpen (
  qcom_sQid *Address
);


void mh_LogMessage (
  FILE *fp,
  pwr_tStatus sts,
  mh_eMsg Type,
  mh_sMsgInfo *Msg,
  qcom_sQid *Address
);

#endif
