/* co_time_os.c -- OS specific time routines.

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
#include "co_time.h"
#include "co_time_msg.h"

static time_tOs		*abs_to_vms	(pwr_tStatus*, time_tOs*, pwr_tTime*);
static time_tOs		*delta_to_vms	(pwr_tStatus*, time_tOs*, pwr_tDeltaTime*);
static pwr_tTime	*vms_to_abs	(pwr_tStatus*, pwr_tTime*, time_tOs*);
static pwr_tDeltaTime	*vms_to_delta	(pwr_tStatus*, pwr_tDeltaTime*, time_tOs*);

static int timezone = 1;


/* Multiply a delta time with a factor, result is also delta.  */

pwr_tStatus
time_Dmul (
  pwr_tDeltaTime *result,
  pwr_tDeltaTime *t1,
  pwr_tInt32 fac
)
{
  pwr_tInt64 nsec1, nsec2, sum;
  int sec = t1->tv_sec * fac;
  int mul = 1000000000;
  int addend = 0;
  int sts;

  sts = lib$emul(&sec, &mul, &addend, &nsec1);
  if (EVEN(sts))
    return sts;

  sts = lib$emul(&t1->tv_nsec, &fac, &addend, &nsec2);
  if (EVEN(sts))
    return sts;

  sts = lib$addx(&nsec1, &nsec2, &sum);
  if (EVEN(sts))
    return sts;

  sts = lib$ediv(&mul, &sum, &result->tv_sec, &result->tv_nsec);
  if (EVEN(sts))
    return sts;

 return TIME__SUCCESS;

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

  *sts = sys$gettim(tp);

  return tp;
}

static time_tOs *
abs_to_vms (
  pwr_tStatus *status,
  time_tOs *result,
  pwr_tTime *pt
)
{
  time_tOs tmp;
  time_tOs ofs = {0x4beb4000, 0x007c9567};
  int multiplier = 10000000;	/* Used to convert 1 s to 100 ns, delta time.  */
  int nsec = pt->tv_nsec/100;
  int sec = pt->tv_sec;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  sec += timezone * 3600;

  *sts = lib$emul(&sec, &multiplier, &nsec, &tmp);
  if (EVEN(*sts)) return NULL;
  *sts = lib$addx(&tmp, &ofs, result);
  if (EVEN(*sts)) return NULL;

  return result;
}

static time_tOs *
delta_to_vms (
  pwr_tStatus *status,
  pwr_tVaxTime *result,
  pwr_tDeltaTime *pt
)
{
  int multiplier = -10000000;	/* Used to convert 1 s to 100 ns, delta time.  */
  int nsec = pt->tv_nsec/100;
  int sec = pt->tv_sec;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  *sts = lib$emul(&sec, &multiplier, &nsec, result);
  if (EVEN(*sts)) return NULL;

  return result;
}

static pwr_tTime *
vms_to_abs (
  pwr_tStatus *status,
  pwr_tTime *tp,
  time_tOs *vt
)
{
  time_tOs tmp;
  time_tOs time;
  time_tOs ofs = {0x4beb4000, 0x007c9567};
  int divisor = 10000000;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = (pwr_tTime *) &time;

  *sts = lib$subx(vt, &ofs, &tmp);
  if (EVEN(*sts)) return NULL;
  *sts = lib$ediv(&divisor, &tmp, &tp->tv_sec, &tp->tv_nsec);
  if (EVEN(*sts)) return NULL;
  tp->tv_nsec *= 100;

  tp->tv_sec -= timezone * 3600;

  return tp;
}

static pwr_tDeltaTime *
vms_to_delta (
  pwr_tStatus *status,
  pwr_tDeltaTime *tp,
  time_tOs *vt
)
{
  time_tOs time;
  int divisor = -10000000;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = (pwr_tDeltaTime *)&time;

  *sts = lib$ediv(&divisor, vt, &tp->tv_sec, &tp->tv_nsec);
  if (EVEN(*sts)) return NULL;
  tp->tv_nsec *= 100;

  return tp;
}

pwr_tStatus
time_PwrToVms (
  pwr_tTime *pt,
  pwr_tVaxTime *vt
)
{
  pwr_tStatus sts;

  abs_to_vms(&sts, (time_tOs *)vt, pt);

  return sts;
}

pwr_tStatus
time_PwrDeltaToVms (
  pwr_tDeltaTime *pt,
  pwr_tVaxTime *vt
)
{
  pwr_tStatus sts;

  delta_to_vms(&sts, (time_tOs *)vt, pt);

  return sts;
}

pwr_tStatus
time_VmsToPwr (
  pwr_tVaxTime *vt,
  pwr_tTime *pt
)
{
  pwr_tStatus sts;

  if (vt->high >= 0)
    vms_to_abs(&sts, pt, (time_tOs *)vt);
  else {
    /* Todo ! Give information in return code that pt contains diff time. */
    vms_to_delta(&sts, (pwr_tDeltaTime*)pt, (time_tOs *)vt);
  }

  return sts;
}

/* Set system time */
pwr_tStatus
time_SetTime(
  pwr_tTime *pt
)
{
  pwr_tStatus sts;

  pwr_tVaxTime vt;

  abs_to_vms(&sts, &vt, pt);
  if (ODD(sts))
    ker$set_time(&sts, &vt);
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

  abs_to_vms(sts, tp, ap);

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
  int multiplier = -10000000;
  int nsec = dp->tv_nsec/100;
  int sec = dp->tv_sec;

  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = &os_time;


  *sts = lib$emul(&sec, &multiplier, &nsec, tp);
  if (EVEN(*sts)) return NULL;

  return tp;
}
