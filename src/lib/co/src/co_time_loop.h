/** 
 * Proview   $Id: co_time_loop.h,v 1.2 2005-09-01 14:57:52 claes Exp $
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
 **/

#ifndef co_time_loop_h
#define co_time_loop_h
/* co_time_loop.h --
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
