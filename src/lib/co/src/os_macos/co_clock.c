/* 
 * Proview   $Id: co_clock.c,v 1.2 2005-09-01 14:57:52 claes Exp $
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

/* co_clock.c -- .

 */


#ifndef OS_MACOS
# error This file is only for Mac OS
#endif

#include <sys/time.h>
#include <errno.h>

#include "pwr.h"
#include "co_time.h"
#include "co_time_msg.h"

int
clock_gettime (
  clockid_t		clockid,
  struct timespec	*pt
)
{
  if (clockid == CLOCK_REALTIME) {
    struct timeval tv;

    gettimeofday( &tv, 0);

    pt->tv_sec = tv.tv_sec;
    pt->tv_nsec = tv.tv_usec * 1000;
  }
  else if ( clockid == CLOCK_MONOTONIC) {
    // TODO
    struct timeval tv;

    gettimeofday( &tv, 0);

    pt->tv_sec = tv.tv_sec;
    pt->tv_nsec = tv.tv_usec * 1000;
  }
  else {    
    errno = EINVAL;
    return -1;
  }

  return 0;
}
