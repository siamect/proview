#ifndef rt_mh_util_h
#define rt_mh_util_h

/* rt_mh_util.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef rt_mh_h
#include "rt_mh.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#if defined __cplusplus
extern "C" {
#endif

pwr_tStatus   mh_UtilCreateEvent	();			/* create the event semaphore */ 
pwr_tStatus   mh_UtilDestroyEvent	();			/* destroy the event semaphore */

pwr_tBoolean  mh_UtilIsStartedMh	();			/* is emon started */ 
pwr_tStatus   mh_UtilStartScanSup	(qcom_sQid Source);
pwr_tStatus   mh_UtilStopScanSup	(qcom_sQid Source);	/* wait for emon to start */
pwr_tStatus   mh_UtilWaitForMh		();
pwr_tStatus   mh_UtilWake		();			/* wake all waiting for emon to start */

#if defined __cplusplus
}
#endif
#endif











