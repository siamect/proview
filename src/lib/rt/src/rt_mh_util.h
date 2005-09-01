/* 
 * Proview   $Id: rt_mh_util.h,v 1.3 2005-09-01 14:57:56 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef rt_mh_util_h
#define rt_mh_util_h

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











