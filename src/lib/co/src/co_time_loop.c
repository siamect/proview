/** 
 * Proview   $Id: co_time_loop.c,v 1.2 2005-09-01 14:57:52 claes Exp $
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

/* co_time_loop.c -- Utilities for timed loops. */

#ifdef OS_ELN
# include stdio
# include ctype
# include string
# include stdlib
# include descrip
# include errno
# include limits
# include times
#elif OS_VMS
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <stdlib.h>
# include <starlet.h>
#else
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <stdlib.h>
#endif

#if defined OS_LYNX || defined OS_LINUX || defined OS_MACOS
# include <sys/times.h>
#endif

#include "pwr.h"
#include "co_time.h"
#include "co_time_loop.h"
#include "co_time_msg.h"

#if 0
#ifdef OS_VMS
/* Help function to do a change mode to executive call.
   Ticks are in 10-millisecond units.  */

static int
vmsUpTime (
  unsigned int *sec,
  unsigned int *tick
)
{
  extern unsigned int EXE$GL_ABSTIM;
  extern unsigned int EXE$GL_ABSTIM_TICS;

  *sec  = EXE$GL_ABSTIM;
  *tick = EXE$GL_ABSTIM_TICS;

   return 1;
}
#endif

/* Get current uptime.  */

pwr_tDeltaTime *
time_GetUpTime (
  pwr_tStatus           *status,
  pwr_tDeltaTime        *tp,
  pwr_tDeltaTime        *ap
)
{
#if defined(OS_VMS)
  /* This code is valid up to 497 days after boot.  */

  void                  *argv[3];
  pwr_tStatus           sts;

  argv[0] = (void *) 2;
  argv[1] = &tp->tv_sec;
  argv[2] = &tp->tv_nsec;

  sts = sys$cmexec(&vmsUpTime, argv);

  tp->tv_nsec %= 100;
  tp->tv_nsec *= 10000000;

#elif defined(OS_ELN)

  pwr_tStatus           sts;
  int                   tmp[2];
  int                   div = -10000000;

  ker$get_uptime(&sts, &tmp);

  sts = lib$ediv(&div, &tmp, &tp->tv_sec, &tp->tv_nsec);

  tp->tv_nsec *= -100;

  pwr_Assert(tp->tv_sec >= 0 && tp->tv_nsec >= 0);
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined(OS_MACOS)
  time_GetTime( (pwr_tTime *) tp);
#endif

  return tp;
}

/* Loop init.  */

pwr_tStatus
time_LoopInit(
  time_sLoop            *ls,
  pwr_tFloat32          scantime)
{
  pwr_tStatus   sts;
  time_FloatToD( &ls->DeltaTime, scantime);
  time_GetUpTime( &sts, &ls->PreviousTime, NULL);

#if defined(OS_VMS)
  sts = lib$get_ef( &ls->TimerFlag);
  if (EVEN(sts)) return sts;
#endif

  return TIME__SUCCESS;
}

/* Wait for an uptime.  */

pwr_tBoolean
time_LoopWait (
  time_sLoop            *ls
)
{
  pwr_tDeltaTime        current_time, wait_time, t;
  int                   sts;

  time_Dadd( &t, &ls->PreviousTime, &ls->DeltaTime);

  time_GetUpTime( &sts, &current_time, NULL);

  if ( ( sts = time_Dcomp( &current_time, &t)) >= 0)
  {
    ls->PreviousTime = t;
    return !sts;
  }
  time_Dsub( &wait_time, &t, &current_time);
/*
  printf( "\nt:        tv_sec: %d  tv_nsec: %d", t.tv_sec, t.tv_nsec);
  printf( "\ncurr:     tv_sec: %d  tv_nsec: %d", current_time.tv_sec, current_time.tv_nsec);
  printf( "\nwait_time:tv_sec: %d  tv_nsec: %d", wait_time.tv_sec, wait_time.tv_nsec);
*/
#if defined(OS_VMS) || defined(OS_ELN)
  {
    pwr_tStatus         sts;
    pwr_tVaxTime        vt;

#if 0
    time_PwrDeltaToVms( &wait_time, &vt);
#endif
/*
    int                 tv_nsec;
    int                 multiplier = -10000000;
    static pwr_tDeltaTime tick = {0, 10000000};

    time_Dadd( &wait_time, &wait_time, &tick);
    tv_nsec = -wait_time.tv_nsec/100;
    sts = lib$emul(&wait_time.tv_sec, &multiplier, &tv_nsec, &vt);
*/
#if defined(OS_ELN)
    ker$wait_any( &sts, NULL, &vt);
#elif defined OS_VMS
    sts = sys$clref( ls->TimerFlag);
    sts = sys$setimr( ls->TimerFlag, &vt, 0, 0, 0);
    sts = sys$waitfr( ls->TimerFlag);
#endif
  }
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined(OS_MACOS)
  {
    struct timespec wait_time_ts;
    wait_time_ts.tv_sec = wait_time.tv_sec;
    wait_time_ts.tv_nsec = wait_time.tv_nsec;
    nanosleep( &wait_time_ts, NULL);
  }
#endif
  ls->PreviousTime = t;
  return 1;
}
#endif

/* Wait for an uptime or a signal.  */

#if defined(OS_ELN)
pwr_tBoolean
time_LoopWaitEvent (
  time_sLoop            *ls,
  int                   *WaitResult,
  unsigned long         Event
)
{
  pwr_tTime             now;
  pwr_tDeltaTime        current_time, wait_time, t;
  int                   sts;

  time_Dadd( &t, &ls->PreviousTime, &ls->DeltaTime);

  time_GetUpTime( &sts, &current_time, NULL);

  if ( ( sts = time_Dcomp( &current_time, &t)) >= 0)
  {
    ls->PreviousTime = t;
    return !sts;
  }
  time_Dsub( &wait_time, &t, &current_time);
  {
    pwr_tStatus         sts;
    int                 tv_nsec;
    pwr_tVaxTime        vt;
    int                 multiplier = -10000000;       /* Used to convert 1 s to 100 ns, delta time.  */
    static pwr_tDeltaTime tick = {0, 10000000};

    time_Dadd( &wait_time, &wait_time, &tick);
    tv_nsec = -wait_time.tv_nsec/100;   /* Convert to 100 ns.  */
    sts = lib$emul(&wait_time.tv_sec, &multiplier, &tv_nsec, &vt);
    ker$wait_any( &sts, WaitResult, &vt, Event);
  }
  ls->PreviousTime = t;
  return 1;
}
#endif
