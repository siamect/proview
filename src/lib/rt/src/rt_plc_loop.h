#ifndef rt_plc_loop_h
#define rt_plc_loop_h

/* rt_plc_loop.h -- PLC Loop Routines

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

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
