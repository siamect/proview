/* co_time_os.c -- OS specific time routines.

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.


    */


#ifndef OS_LINUX
# error This file is only for Linux
#endif
#include <unistd.h>
#include <sys/times.h>
#include <string.h>
#include "pwr.h"
#include "co_time.h"
#include "co_time_msg.h"


/* Return delta time since system start.
   Add delta time 'add'.  */

pwr_tDeltaTime *
time_Uptime (
  pwr_tStatus *status,
  pwr_tDeltaTime *tp,
  pwr_tDeltaTime *ap
)
{
  pwr_tDeltaTime time;
  long tics;
  struct tms buff;
  static int tics_per_sec = 0;
  static pwr_tTime boot_time = {0,0};
  static pwr_tDeltaTime max_diff = {0,20000000};
  pwr_tDeltaTime uptime_tics;
  pwr_tTime current_time;
  pwr_tDeltaTime diff;
  pwr_dStatus(sts, status, TIME__SUCCESS);
 
  if ( !tics_per_sec)
    tics_per_sec = sysconf(_SC_CLK_TCK);

  if (tp == NULL)
    tp = &time;

  tics = times(&buff);
  uptime_tics.tv_sec = tics / tics_per_sec;
  uptime_tics.tv_nsec = (tics % tics_per_sec) * (1000000000 / tics_per_sec);

  // pwr_Assert(tp->tv_sec >= 0 && tp->tv_nsec >= 0);

  clock_gettime( CLOCK_REALTIME, &current_time);
  if ( !boot_time.tv_sec) {
    time_Asub( &boot_time, &current_time, &uptime_tics);
    *tp = uptime_tics;
  }
  else {
    time_Adiff( tp, &current_time, &boot_time);
    time_Dsub( &diff, tp, &uptime_tics);
    time_Dabs(NULL, &diff);
    if ( time_Dcomp(&diff, &max_diff) > 0) {
      time_Asub( &boot_time, &current_time, &uptime_tics);
      *tp = uptime_tics;
      *status = TIME__CLKCHANGE;
    }
  }

  if (ap != NULL)
    return time_Dadd(tp, tp, ap);
  else
    return tp;
}


/* Return number of clock ticks since system start.
   Add number of tics corresponding to delta time 'add'.  */

time_tClock
time_Clock (
  pwr_tStatus *status,
  pwr_tDeltaTime *ap
)
{
  long		tics;
  struct tms	buff;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  tics = times(&buff);

  if (ap != NULL) {
    tics += (ap->tv_sec * CLK_TCK) + (ap->tv_nsec / (1000000000 / CLK_TCK));
  }

  return tics;
}

time_tOs *
time_Os (
  pwr_tStatus *status,
  time_tOs *tp
)
{
  static time_tOs os_time;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = &os_time;

  clock_gettime(CLOCK_REALTIME, tp);

  return tp;
}

/* Set system time */
pwr_tStatus
time_SetTime(
  pwr_tTime *pt
)
{
  pwr_tStatus sts = TIME__SUCCESS;

  return sts;
}


time_tOs *
time_AtoOs (
  pwr_tStatus *status,
  time_tOs *tp,
  pwr_tTime *ap
)
{
  time_tOs os_time;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = &os_time;

  *tp = *ap;

  return tp;
}

/* Convert from Proview delta time format
   to native time format. */

time_tOs *
time_DtoOs (
  pwr_tStatus *status,
  time_tOs *tp,
  pwr_tDeltaTime *dp
)
{
  time_tOs os_time;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = &os_time;

  memcpy(tp, dp, sizeof(*tp));

  return tp;
}
