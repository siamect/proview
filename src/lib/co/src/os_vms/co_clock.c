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

   VMS-time is stored as the number of 100-nanosecond
   intervals since 00:00 hours, November 1858
   (the base time for the Smithsonian Institution astronomical 
    calendar).

   POSIX-time is stored as the number of seconds since the
   Epoch, 00:00:00 GMT 1 Jan 1970.

   This means that there is a difference of 3,506,716,800 seconds
   (or 35,067,168,000,000,000 100-nanoseconds)
   between the two times.

   Also, the VMS-time is stored as a quadword integer, while
   the POSIX-time is stored as two longword integers.

   To convert from VMS-time to POSIX-time we have:

    pt.tv_sec  = (vt - 35,067,168,000,000,000) / 10,000,000
    pt.tv_nsec = ((vt - 35,067,168,000,000,000) % 10,000,000) * 100

    where pt = POSIX-time and vt = VMS-time.

   or

    lib$subx(vt, 35,067,168,000,000,000, tmp_t, 2);

    lib$ediv(10,000,000, tmp_t, pt.sec, pt.nsec);

   To convert from POSIX-time to VMS-time we have:

    vt = (pt.sec * 10,000,000 + pt.nsec/100) + (3,506,716,800 * 10,000,000)

   or

    lib$emul(pt.sec, 10,000,000, pt.nsec, tmp_t);
    lib$add_times(tmp_t, 35,067,168,000,000,000, vt);

    where pt = POSIX-time and vt = VMS-time.

   35,067,168,000,000,000 expressed in hexadecimal radix is 0x007c95674beb4000. */


#ifndef OS_VMS
# error This file is only for OpenVMS
#endif

#include <lib$routines.h>
#include <descrip.h>
#include <errno.h>
#include <starlet.h>
#include <unixlib.h>
#include <times.h>

#include "pwr.h"
#include "co_time.h"
#include "co_time_msg.h"


/* Get timespec from vms.  */

int
clock_gettime (
  clockid_t		clockid,
  struct timespec	*pt
)
{
  unsigned long sts;
  pwr_tVaxTime vt;
  pwr_tVaxTime tmp;
  pwr_tVaxTime ofs = {0x4beb4000, 0x007c9567};
  int divisor = 10000000;
  struct tm	*tmpTm;
  unsigned long	t = 0;
 
  if (clockid != CLOCK_REALTIME) {
    errno = EINVAL;
    return -1;
  }

  sts = sys$gettim(&vt);

  sts = lib$subx(&vt, &ofs, &tmp);
  if (EVEN(sts)) {
    errno = sts;
    return -1;
  }
  sts = lib$ediv(&divisor, &tmp, &pt->tv_sec, &pt->tv_nsec);
  if (EVEN(sts)) {
    errno = sts;
    return -1;
  }

  pt->tv_nsec *= 100;

  tmpTm = localtime(&pt->tv_sec);
  pt->tv_sec -= tmpTm->tm_gmtoff - tmpTm->tm_isdst * 3600;

  return 0;
}

/* Get timer resolution.  */

int
clock_getres (
  clockid_t		clockid,
  struct timespec	*tp
)
{

  if (clockid != CLOCK_REALTIME) {
    errno = EINVAL;
    return -1;
  }

  tp->tv_sec  = 0;
  tp->tv_nsec = 10000000 ; /* 10 ms */
  return 0;
}
