/* co_time_spec.c -- OS special time routines.

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

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


#ifndef OS_ELN
# error This file is only for VAX/ELN
#endif

#include times
#include "pwr.h"
#include "co_time.h"
#include "co_time_msg.h"


/* Return delta time since system start.
   Add delta time 'add'.
   
   NOTA BENE ! This code is valid up to 497 days after boot. */

pwr_tDeltaTime *
time_Uptime (
  pwr_tStatus	  *status,
  pwr_tDeltaTime  *tp,
  pwr_tDeltaTime  *ap
)
{
  pwr_tDeltaTime	time;
  time_tOs		tmp;
  int			rest;
  int                   div = -10000000;

  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = &time;

  ker$get_uptime(sts, &tmp);

  *sts = lib$ediv(&div, &tmp, &tp->tv_sec, &tp->tv_nsec);

  tp->tv_nsec *= -100;

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
  pwr_tStatus	  *status,
  pwr_tDeltaTime  *ap
)
{
  long		  tics;
  struct tms	  buff;

  pwr_dStatus(sts, status, TIME__SUCCESS);

  tics = times(&buff);

  if (ap != NULL) {
    tics += ap->tv_sec * 100 + ap->tv_nsec / 10000000;
  }

  return tics;
}
