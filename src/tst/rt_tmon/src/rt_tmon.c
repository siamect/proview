/* rt_tmon.c -- Timer Monitor

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   .  */

#if defined(OS_ELN)
# include $vaxelnc
# include stdio.h
# include stddef.h
# include stdlib.h
# include string.h
# include descrip
#else
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
#endif

#ifdef OS_VMS
# include <starlet.h>
# include <descrip.h>
#endif

#include "pwr.h"

#ifdef OS_VMS
  static int		timerFlag;
#endif

static pwr_tTime *
addTime (
  pwr_tTime		*t1,
  pwr_tTime		*t2
);

static pwr_tTime *
msToTime (
  pwr_tTime		*it,
  int			ms
);

static pwr_tTime *
subTime (
  pwr_tTime		*t1,
  pwr_tTime		*t2
);

static pwr_tTime *
nowTime (
  pwr_tTime		*tp
);

static void
waitTime (
  pwr_tTime		*t
);

int
main (
  int			argc,
  char			**argv
)
{
  pwr_tStatus		sts;
  pwr_tTime		time;
  pwr_tTime		now;
  int			sec,nsec;

#ifdef OS_VMS
  sts = lib$get_ef(&timerFlag);
  if (EVEN(sts)) exit(sts);
#endif

  for (sec=0,nsec=10000000;;) {
    nowTime(&now);
#if 0    
    now.tv_sec = sec;
    now.tv_nsec = nsec;
#endif
    printf("%d:%d\n", now.tv_sec, now.tv_nsec);
    waitTime(&now);
    nsec += 10000000;
    sec += nsec / 1000000000;
    nsec %= 1000000000;
  }
}

/* Add time2 to time1.  */

static pwr_tTime *
addTime (
  pwr_tTime		*t1,
  pwr_tTime		*t2
)
{
  int			tv_nsec = t1->tv_nsec + t2->tv_nsec;
  int			tv_sec  = (int)t1->tv_sec + (int)t2->tv_sec;

  t1->tv_sec  = (tv_sec + tv_nsec / 1000000000);
  t1->tv_nsec = tv_nsec % 1000000000;

  return t1;
}

/* Convert mille seconds to time format.  */

static pwr_tTime *
msToTime (
  pwr_tTime		*it,
  int			ms
)
{
  static pwr_tTime	time;
  pwr_tTime		*t = &time;

  if (it != NULL) t = it;

  t->tv_sec  = ms / 1000;
  t->tv_nsec = (ms % 1000) * 1000000;

  return t;
}

/* Subtract time2 from time1.  */

static pwr_tTime *
subTime (
  pwr_tTime		*t1,
  pwr_tTime		*t2
)
{
  int			tv_nsec = t1->tv_nsec - t2->tv_nsec;
  int			tv_sec  = (int)t1->tv_sec  - (int)t2->tv_sec;

  tv_sec  = tv_sec + tv_nsec / 1000000000;
  tv_nsec = tv_nsec % 1000000000;
  if (tv_nsec < 0 && tv_sec > 0) {
    tv_sec--;
    tv_nsec += 1000000000;
  } else if (tv_sec < 0 && tv_nsec > 0) {
    tv_sec++;
    tv_nsec -= 1000000000;
  }

  t1->tv_sec  = tv_sec;
  t1->tv_nsec = tv_nsec;
  return t1;
}

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

  *sec	= EXE$GL_ABSTIM;
  *tick	= EXE$GL_ABSTIM_TICS;

   return 1;
}
#endif

/* Get current uptime.  */

static pwr_tTime *
nowTime (
  pwr_tTime		*tp
)
{
#if defined OS_VMS
  /* This code is valid up to 497 days after boot.  */

  void			*argv[3];
  pwr_tStatus		sts;

  argv[0] = (void *) 2;
  argv[1] = &tp->tv_sec;
  argv[2] = &tp->tv_nsec;

  sts = sys$cmexec(&vmsUpTime, argv);

  tp->tv_nsec %= 100;
  tp->tv_nsec *= 10000000;

#elif defined OS_ELN

  pwr_tStatus		sts;
  int			tmp[2];
  int			div = -10000000;

  ker$get_uptime(&sts, &tmp);

  sts = lib$ediv(&div, &tmp, &tp->tv_sec, &tp->tv_nsec);

  tp->tv_nsec *= -100;

#elif defined OS_LYNX
  clock_gettime(CLOCK_REALTIME, tp);
#endif
 
  return tp;
}

/* Wait for a while.  */

static void
waitTime (
  pwr_tTime		*t
)
{
  pwr_tStatus		sts;
  pwr_tTime		now;
  pwr_tTime		then = *t;
  char tims[24];
  short len;
  struct dsc$descriptor_s tims_desc = {
    sizeof(tims)-1, DSC$K_DTYPE_T, DSC$K_CLASS_S,};

#if 0
  subTime(&then, nowTime(&now));
#endif

  if ((int)then.tv_sec > 0 || ((int)then.tv_sec == 0 && then.tv_nsec > 0)) {
#if defined OS_VMS || defined OS_ELN
    int			tv_nsec;
    int			vmstime[2];
    int			multiplier = -10000000;	      /* Used to convert 1 s to 100 ns, delta time.  */
    static pwr_tTime	tick = {0, 10000000};

    addTime(&then, &tick);
    tv_nsec = -then.tv_nsec/100;   /* Convert to 100 ns.  */
    sts = lib$emul(&then.tv_sec, &multiplier, &tv_nsec, vmstime);

#if defined OS_VMS
    tims_desc.dsc$a_pointer = tims;
    sys$asctim( &len, &tims_desc, vmstime, 0);
    tims[len] = '\0';
    printf("  %s\n", tims);
#if 0
    sts = sys$clref(timerFlag);
    sts = sys$setimr(timerFlag, vmstime, 0, 0, 0);
    sts = sys$waitfr(timerFlag);
#endif
#elif defined OS_ELN
    eln$time_string(tims, vmstime);
    tims[23] = '\0';
    printf("  %s\n", tims);
#if 0
    ker$wait_any(&sts, NULL, vmstime);
#endif
#endif

#elif defined OS_LYNX
    pwr_tTime rmt;

    nanosleep(&then, &rmt);
#endif
  }
}
