/* 
 * Proview   $Id: co_time_os.c,v 1.2 2005-09-01 14:57:52 claes Exp $
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

/* co_time_os.c -- OS specific time routines.
*/


#ifndef OS_LYNX
# error This file is only for LynxOS
#endif
#include <sys/times.h>
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
  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = &time;

  tics = times(&buff);
  tp->tv_sec = tics / CLK_TCK;
  tp->tv_nsec = (tics % CLK_TCK) * (1000000000 / CLK_TCK);

  pwr_Assert(tp->tv_sec >= 0 && tp->tv_nsec >= 0);

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
