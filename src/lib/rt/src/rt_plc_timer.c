/* rt_plc_timer.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   Count down all timers that are linked to 'timerstart'. If Count expires
   or Flag has been reset, timer is reset, and digital value is reset
   if there is a pointer to DO.  */

#include <stdio.h>
#include "pwr.h"
#include "pwr_class.h"
#include "rt_plc_timer.h"

#if defined OS_LYNX || defined OS_LINUX
# include <errno.h>
# include "rt_errh.h"
#endif

#ifdef OS_ELN
  MUTEX timer_mutex;

#elif defined OS_LYNX  || defined OS_LINUX
  pthread_mutex_t timer_mutex;
#endif


void plc_inittimer (
  plc_sThread *tp
)
{

#if 0
  /* Create the timer mutex */
#ifdef OS_ELN
  {
    int	sts;
    ELN$CREATE_MUTEX(tp->timer_mutex, &sts);
    if (EVEN(sts))
      pwr_Bugcheck(sts, "Cannot create timer_mutex");
  }
#elif defined OS_LYNX || defined OS_LINUX
  {
    pthread_mutexattr_t attr;

    pthread_mutexattr_create(&attr);
    if (pthread_mutex_init(&tp->timer_mutex, attr) == -1) 
      pwr_Bugcheck(errno, "Cannot create timer_mutex");
    pthread_mutexattr_delete(&attr);
  }
#endif
#endif
}

void
plc_timerhandler (
  plc_sThread	*tp
)
{
  plc_sTimer	*cp;
  plc_sTimer	*prev;

  prev = NULL;

#if 0
  PLC_LOCK_MUTEX(tp->timer_mutex);
#endif
  cp = RELPTR(tp->PlcThread->TimerStart);

  while ( cp != NULL) {
    if ((cp->TimerCount <= 1) || !cp->TimerFlag) {
      cp->TimerCount = 0;
      cp->TimerFlag = 0;
      *cp->TimerDO = FALSE;
      if ( prev != NULL )
        prev->TimerNext = cp->TimerNext;
      else
        tp->PlcThread->TimerStart = cp->TimerNext;
    } else {
      cp->TimerCount--;
      prev = cp;
    }
    cp = RELPTR(cp->TimerNext);
  }
#if 0
  PLC_UNLOCK_MUTEX(tp->timer_mutex);
#endif
}

#if 0
void plc_timer_in (
  plc_sThread	*tp,
  pwr_sClass_DSup    *o
)
{
  o->TimerCount = o->TimerTime / tp->f_scan_time;
  if (!o->TimerFlag && (o->TimerCount > 0)) {
    o->TimerFlag = TRUE;
    /*PLC_LOCK_MUTEX(tp->timer_mutex);*/
    (o->TimerNext) = tp->PlcThread->TimerStart;
    PTRREL(&tp->PlcThread->TimerStart, &o->TimerFlag );
    /*PLC_UNLOCK_MUTEX(tp->timer_mutex);*/
  }
}
#endif
