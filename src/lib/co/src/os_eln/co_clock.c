/* co_clock.c -- .

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

#include stdio
#include ctype
#include string
#include stdlib
#include descrip
#include errno
#include limits

#include "pwr.h"
#include "co_time.h"
#include "co_time_msg.h"

static int timezone = 1;

struct  p1_ctx {
  int p1$errno;		/* UNIX-style error code */
  int p1$vaxc$errno;	/* VAX equivalent of errno above */
  int p1$randx;		/* random number seed */
			/* More members follows in the real struct */
};


#undef clock_gettime

int
eln_clock_gettime (
  clockid_t             clockid,
  struct timespec       *pt
)
{
  int sts;
  sts = clock_gettime(clockid, pt);
  pt->tv_sec -= timezone * 3600;

  return sts;
}

#undef localtime

struct tm 
*eln_localtime (
  const time_t *bintim
)
{
  struct timespec pt;

  memcpy( &pt, bintim, sizeof(pt));
  /* Compensation for 2000 leap day bugg */
  pt.tv_sec += (timezone+5-24) * 3600;
  return localtime( &pt);
}

#undef mktime

time_t 
eln_mktime (
  tm_t *timeptr
)
{
  time_t sec;

  sec = mktime( timeptr);
  sec -= (timezone+5)* 3600;
  return sec;
}

/* Get timer resolution.  */

#undef clock_getres

int
eln_clock_getres (
  clockid_t		clockid,
  struct timespec	*tp
)
{

  if (clockid != CLOCK_REALTIME) {
    struct p1_ctx *ctx_pointer = eln$locate_crtl_ctx();
    ctx_pointer->p1$errno = EINVAL;
    return -1;
  }

  tp->tv_sec  = 0;
  tp->tv_nsec = 10000000 ; /* 10 ms */
  return 0;
}
