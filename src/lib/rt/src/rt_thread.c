/* rt_thread.c -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */

#ifdef	OS_ELN
# include $vaxelnc
# include $kernelmsg
# include stdio
# include stdlib
# include string
# include descrip
#else
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
#endif

#ifdef OS_VMS
# include <starlet.h>
# include <lib$routines.h>
# include <tis.h>
#endif

#ifdef OS_LYNX
# include <signal.h>
#endif


#include "pwr.h"
#include "co_errno.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_thread.h"
#include "rt_thread_msg.h"

#if defined OS_VMS
typedef struct {
  pthread_cond_t *cp;
  pwr_tStatus sts;
} sTimerAst;
#endif

pwr_tStatus
thread_CondInit (
  thread_sCond	*cp
)
{

#if defined OS_ELN

  pwr_tStatus sts;

  ker$create_event(&sts, cp, EVENT$CLEARED);

  return sts;

#elif defined OS_VMS

  return errno_Status(tis_cond_init(&cp->c));

#elif defined OS_LYNX && defined PWR_LYNX_30

  return errno_Pstatus(pthread_cond_init(&cp->c, 0));

#elif defined OS_LYNX || defined OS_LINUX

  return errno_Status(pthread_cond_init(&cp->c, 0));

#else
# error Not defined for this platform !
#endif
}

pwr_tStatus
thread_MutexInit (
  thread_sMutex		*mp
)
{

#if defined OS_ELN

  pwr_tStatus sts = THREAD__SUCCESS;

  ELN$CREATE_MUTEX(*mp, &sts);

  return sts;

#elif defined OS_VMS

  return errno_Status(tis_mutex_init(mp));

#elif defined OS_LYNX && defined PWR_LYNX_30

  return errno_Pstatus(pthread_mutex_init(mp, NULL));

#elif defined OS_LYNX || defined OS_LINUX

  return errno_Status(pthread_mutex_init(mp, NULL));

#else
# error Not defined for this platform !
#endif
}

pwr_tStatus
thread_MutexLock (
  thread_sMutex		*mp
)
{

#if defined OS_ELN

  ELN$LOCK_MUTEX(*mp);
  return THREAD__SUCCESS;

#elif defined OS_LYNX && defined PWR_LYNX_30

  return errno_Pstatus(pthread_mutex_lock(mp));

#elif defined OS_LYNX || defined OS_LINUX

  return errno_Status(pthread_mutex_lock(mp));

#elif defined OS_VMS

  return errno_Status(tis_mutex_lock(mp));

#else
# error Not defined for this platform !
#endif

}

pwr_tStatus
thread_MutexUnlock (
  thread_sMutex		*mp
)
{

#if defined OS_ELN

  ELN$UNLOCK_MUTEX(*mp);
  return THREAD__SUCCESS;

#elif defined OS_LYNX && defined PWR_LYNX_30

  return errno_Pstatus(pthread_mutex_unlock(mp));

#elif defined OS_LYNX || defined OS_LINUX

  return errno_Status(pthread_mutex_unlock(mp));

#elif defined OS_VMS

  return errno_Status(tis_mutex_unlock(mp));

#else
# error Not defined for this platform !
#endif

}

pwr_tStatus
thread_Cancel (
  thread_s	*tp
)
{
  
#if defined OS_ELN

  pwr_tStatus sts = THREAD__SUCCESS;

  ker$delete(&sts, *tp);

  return sts;

#elif defined OS_LYNX && defined PWR_LYNX_30

    return errno_Pstatus(pthread_cancel(*tp));

#elif defined OS_LYNX || defined OS_LINUX

    return errno_Status(pthread_cancel(*tp));

#elif defined OS_VMS

    return errno_Status(pthread_cancel(*tp));

#else
# error Not defined for this platform !
#endif
}

pwr_tStatus
thread_Create (
  thread_s	*tp,
  char		*name,
  void		*(*routine)(),
  void		*arg
)
{

#if defined OS_ELN

  pwr_tStatus	sts = THREAD__SUCCESS;
  struct dsc$descriptor dsc;
  
  ker$create_process(&sts, &tp->id, routine, NULL, arg);

  strncpy(tp->name, name, MIN(strlen(name), sizeof(tp->name) - 1));
  tp->name[sizeof(tp->name) - 1] = '\0';

  dsc.dsc$a_pointer  = tp->name;
  dsc.dsc$w_length   = MIN(31, strlen(tp->name));	
  dsc.dsc$b_dtype    = DSC$K_DTYPE_T;
  dsc.dsc$b_class    = DSC$K_CLASS_S;

  ker$create_name(NULL, &tp->name_id, &dsc, tp->id, NULL);

  return sts;

#elif defined OS_LYNX && defined PWR_LYNX_30

  return errno_Pstatus(pthread_create(tp, pthread_attr_default, routine, arg));

#elif defined OS_VMS || defined OS_LINUX || defined OS_LYNX

  return errno_Status(pthread_create(tp, NULL, routine, arg));

#else
# error Not defined for this platform !
#endif
}

pwr_tStatus
thread_CondWait (
  thread_sCond		*cp,
  thread_sMutex		*mp
)
{

#if defined OS_ELN

  pwr_tStatus sts = THREAD__SUCCESS;

  ker$clear_event(&sts, *cp);
  if (EVEN(sts)) return sts;

  ELN$UNLOCK_MUTEX(*mp);

  ker$wait_any(&sts, NULL, NULL, *cp);

  ELN$LOCK_MUTEX(*mp);

  return sts;

#elif defined OS_LYNX || defined OS_LINUX

  pwr_tStatus sts = THREAD__SUCCESS;

  cp->f = 0;

  while (!cp->f) {
# if defined OS_LYNX && defined PWR_LYNX_30
    sts = errno_Pstatus(pthread_cond_wait(&cp->c, mp));
# else
    sts = errno_Status(pthread_cond_wait(&cp->c, mp));
# endif
    if (sts != ERRNO__INTR)
      break; 
  }
  return cp->f ? THREAD__SUCCESS : sts;
  

#elif defined OS_VMS

  return errno_Status(pthread_cond_wait(&cp->c, mp));

#else
# error Not defined for this platform !
#endif

}

#if defined OS_VMS
static void
timerAst (
  sTimerAst	*ap
)
{

  ap->sts = THREAD__TIMEDOUT;
  pthread_cond_signal_int_np(ap->cp);
}
#endif

pwr_tStatus
thread_CondTimedWait (
  thread_sCond		*cp,
  thread_sMutex		*mp,
  pwr_tDeltaTime	*time
)
{

#if defined OS_ELN
  {
    pwr_tStatus sts = THREAD__SUCCESS;
    int tv_nsec;
    int vmstime[2];
    int multiplier = -10000000;	      /* Used to convert 1 s to 100 ns, delta time.  */
    int result;

    if (time == NULL) {
      ;
    } else if ((int)time->tv_sec < 0 || time->tv_nsec < 0 || (time->tv_sec == 0 && time->tv_nsec == 0)) {
      return THREAD__TIMEDOUT;
    } else {
      tv_nsec = -time->tv_nsec/100;   /* Convert to 100 ns.  */
      sts = lib$emul(&time->tv_sec, &multiplier, &tv_nsec, vmstime);
      if (EVEN(sts)) return sts;
    }

    ker$clear_event(&sts, *cp);
    if (EVEN(sts)) return sts;

    ELN$UNLOCK_MUTEX(*mp);

    if (time != NULL) {
      ker$wait_any(&sts, &result, vmstime, *cp);
      if (ODD(sts) && result == 0)
	sts = THREAD__TIMEDOUT;
    } else {
      ker$wait_any(&sts, NULL, NULL, *cp);
    }

    ELN$LOCK_MUTEX(*mp);

    return sts;
  }
#elif defined OS_LYNX && defined PWR_LYNX_30
  {
    pwr_tStatus sts;

    cp->f = 0;

    if (time == NULL)
      return thread_CondWait(cp, mp);

    sts = errno_Pstatus(pthread_cond_timedwait(&cp->c, mp, (pwr_tTime *)time));
    if (sts == ERRNO__INTR)
      return THREAD__TIMEDOUT;
    else 
      return sts;

  }
#elif defined OS_LINUX || OS_LYNX
  {
    pwr_tTime now;
    pwr_tTime then;

    cp->f = 0;

    if (time == NULL)
      return thread_CondWait(cp, mp);

    clock_gettime(CLOCK_REALTIME, &now);
    time_Aadd(&then, &now, time);

    return errno_Status(pthread_cond_timedwait(&cp->c, mp, &then));

  }
#elif defined OS_VMS
  {
    pwr_tStatus sts = THREAD__SUCCESS;
    int		dtime[2];
    int		multiplier = -10000000;	      /* Used to convert 1 s to 100 ns, delta time.  */
    int		tv_nsec;
    sTimerAst	ast;
    int		ret;

    ast.sts = THREAD__SUCCESS;
    ast.cp = NULL;

    if (time == NULL) {
      ;
    } else if ((int)time->tv_sec < 0 || time->tv_nsec < 0 || (time->tv_sec == 0 && time->tv_nsec == 0)) {
      return THREAD__TIMEDOUT;
    } else {
      ast.cp = &cp->c;
      tv_nsec = -time->tv_nsec/100;   /* Convert to 100 ns.  */
      sts = lib$emul(&time->tv_sec, &multiplier, &tv_nsec, dtime);
      if (EVEN(sts)) return sts;
      sts = sys$setimr(0, dtime, timerAst, &ast, 0);
      if (EVEN(sts)) return sts;
    }

    ret = pthread_cond_wait(&cp->c, mp);
    if (ret != 0)
      sts = errno_Status(ret);
    else {
      sts = ast.sts;
    }

    if (time != NULL)
      sys$cantim(&ast, 0);

    return sts;
  }
#else
# error Not defined for this platform !
#endif

}

pwr_tStatus
thread_CondSignal (
  thread_sCond	*cp
)
{

#if defined OS_ELN

  pwr_tStatus sts = THREAD__SUCCESS;

  ker$signal(&sts, *cp);

  return sts;

#elif defined OS_LYNX || defined OS_LINUX

  cp->f = 1;

# if defined OS_LYNX && defined PWR_LYNX_30
  return errno_Pstatus(pthread_cond_signal(&cp->c));
# else
  return errno_Status(pthread_cond_signal(&cp->c));
# endif


#elif defined OS_VMS

  return errno_Status(pthread_cond_signal(&cp->c));

#else
# error Not defined for this platform !
#endif
}

pwr_tStatus
thread_Wait (
  pwr_tDeltaTime *tp
)
{
  pwr_tStatus sts = THREAD__SUCCESS;
  pwr_tDeltaTime time = {999999999, 0};

  if (tp == NULL)
    tp = &time;

  if ((int)tp->tv_sec > 0 || ((int)tp->tv_sec == 0 && tp->tv_nsec > 0)) {
#if defined OS_VMS

    sts = errno_Status(pthread_delay_np((struct timespec *)tp));

#elif defined OS_ELN

    int tv_nsec;
    int vmstime[2];
    int multiplier = -10000000;	      /* Used to convert 1 s to 100 ns, delta time.  */

    tv_nsec = -tp->tv_nsec/100;   /* Convert to 100 ns.  */
    sts = lib$emul(&tp->tv_sec, &multiplier, &tv_nsec, vmstime);

    ker$wait_any(&sts, NULL, vmstime);

#elif defined OS_LYNX || defined OS_LINUX

    pwr_tTime rmt;
    pwr_tDeltaTime ttime = {9999999, 0};

    if (tp == &time) {
      int i;
      for ( i = 0; i < 100; i++)
        sts = errno_Pstatus(nanosleep((pwr_tTime *) &ttime, &rmt));
    }
    else
      sts = errno_Pstatus(nanosleep((pwr_tTime *)tp, &rmt));

#else
# error Not defined for this platform
#endif
  }

  return sts;
}

#if 0
thread_s
thread_Self ()
{

#if defined OS_ELN

  return NULL;

#elif defined OS_LYNX || defined OS_VMS || defined OS_LINUX

  return pthread_self();

#else
# error Not defined for this platform
#endif
}
#endif

pwr_tStatus
thread_SetPrio (
  thread_s	*tp,
  int		prio
)
{

#if defined OS_ELN

  pwr_tStatus sts = THREAD__SUCCESS;
  PROCESS id;

  prio = 15 - MIN(MAX(prio, 0), 15);
  if (tp == NULL)
    ker$current_process(&sts, &id);
  else
    id = tp->id;

  if (ODD(sts))
    ker$set_process_priority(&sts, id, prio);

  return sts;

#elif defined OS_VMS
  {
    struct sched_param par;
    pthread_t id;

    par.sched_priority = MIN(PRI_FIFO_MAX, PRI_FIFO_MIN + prio);

    if (tp == NULL)
      id = pthread_self();
    else
      id = *tp;

    return errno_Status(pthread_setschedparam(id, SCHED_FIFO, &par));

  }
#elif defined OS_LYNX && defined PWR_LYNX_30
  {
    pthread_t id;
    int sts;

    prio = MIN(PRIO_FIFO_MAX, PRIO_FIFO_MIN + prio);

    if (tp == NULL)
      id = pthread_self();
    else
      id = *tp;
	

    sts = pthread_setscheduler(id, SCHED_FIFO, prio);
    if ( sts == -1 )
      return errno_Pstatus(sts);
    else
      return ERRNO__SUCCESS;

  }
#elif defined OS_LYNX
  {
    struct sched_param par;
    pthread_t id;

    par.sched_priority = MIN(sched_get_priority_max(SCHED_FIFO), 
	       sched_get_priority_min(SCHED_FIFO) + prio);

    if (tp == NULL)
      id = pthread_self();
    else
      id = *tp;

    return errno_Status(pthread_setschedparam(id, SCHED_FIFO, &par));
  }
#elif defined OS_LINUX
  {
    pthread_t tid;
    struct sched_param par;
    pwr_tStatus sts = THREAD__SUCCESS;

    /* Set priority and scheduling mechanism for thread. */
    tid = pthread_self();
    prio = MIN(sched_get_priority_max(SCHED_FIFO), 
	       sched_get_priority_min(SCHED_FIFO) + prio);
    par.sched_priority = prio;
    pthread_setschedparam(tid, SCHED_FIFO, &par);
    return sts;
  }
#else
# error Not defined for this platform !
#endif

}


#if defined OS_LYNX

pwr_tStatus	
thread_Signal (
  thread_s	*tp, 
  int		signo
)
{
  union sigval	value;
  int		ok;
  pwr_tStatus	sts = THREAD__SUCCESS;
  pthread_t 	tid;
  pid_t		pid;

  value.sival_int = BUILDPID(getpid(), pthread_self());

  if (tp == NULL)
    tid = pthread_self();
  else
    tid = *tp;
  pid = BUILDPID(getpid(), tid );

  ok = sigqueue(pid, signo, value);

  if (ok == -1) {
    sts = errno_Status(errno);
  }

  return sts;
}



pwr_tBoolean
thread_SigTimedWait (
  thread_s*	tp,
  int		signo, 
  pwr_tDeltaTime* tmo
)
{
  sigset_t		newset;
  sigset_t		oldset;
  siginfo_t		info;
  int			ok;
  pwr_tBoolean		signal = FALSE;



  sigemptyset(&newset);
  sigaddset(&newset, signo);
  sigprocmask(SIG_BLOCK, &newset, &oldset);

  if (tmo != NULL) {
    ok = sigtimedwait(&newset, &info, (pwr_tTime *)tmo);  
  } else {
    ok = sigwaitinfo(&newset, &info);
  }
    
  if (ok == -1) {
    if ( errno != EAGAIN) {
      errh_Error("thread_SigTimedWait (%d) %s", errno, strerror(errno));
    }
  } else
   signal = TRUE;


  sigprocmask(SIG_SETMASK, &oldset, NULL);

  return signal;
}
#endif
