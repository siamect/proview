/* 
 * Proview   $Id: rt_plc_loop.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_plc_loop_h
#define rt_plc_loop_h

/* rt_plc_loop.h -- PLC Loop Routines
   Link options for VMS
     AXP:   /sysexe or alpha$loadable_images:sys$base_image/share
     VAX    sys$system:sys.stb/selective_search  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifdef OS_VMS
typedef struct {
  unsigned int  LastLoop;
  unsigned int  LoopOflw;
  unsigned int  LastTick;
  unsigned int  TickOflw;
  unsigned int  TimerFlag;
} plc_sLoopWait;
#endif

#ifdef OS_ELN
/*
 * plc_LoopInit()
 *
 * Description: 
 *  Gets the Uptime which will be used as NextTime the first time
 *  plc_LoopWait is called. 
 *
 */ 
pwr_tStatus plc_LoopInit(pwr_tVaxTime *NextTime);

/*
 * plc_LoopWait()
 *
 * Description:
 *  Returns False if a slip is detected.
 */ 
pwr_tBoolean plc_LoopWait (
    int		*WaitResult,     /* Set to 1 when Event */
    pwr_tVaxTime	*DeltaTime,
    pwr_tVaxTime	*NextTime,
    unsigned long Event
);
#endif

#ifdef OS_VMS


/*
 * plc_LoopGetVmsUpTime()
 *
 * Description: 
 *  Gets the VMS uptime
 *
 */ 
pwr_tStatus plc_LoopGetVmsUpTime (
  unsigned int *Seconds,	 
  unsigned int *Ticks	/* 1 tick = 10 ms */
);

/*
 * plc_LoopInit()
 *
 * Description: 
 *  Inits the plc_sLoopWait struct
 *  This routine must be called before plc_LoopWait
 *
 */ 
pwr_tStatus plc_LoopInit (
  plc_sLoopWait *p
);

/*
 * plc_LoopWait()
 *
 * Description:
 *  Returns False if a slip is detected.
 *  The plc_sLoopWait structure must be initialized with a call to
 *  plc_LoopInit.
 */ 
pwr_tBoolean plc_LoopWait (
  plc_sLoopWait	  *p,
  unsigned int	  DeltaTime /* ms */
);

#endif
#endif
