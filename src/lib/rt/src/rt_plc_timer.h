#ifndef rt_plc_timer_h
#define rt_plc_timer_h 

/* rt_plc_timer.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996,99 by Mandator AB.

   <Description>.  */

#ifdef OS_ELN
# include $mutex
#endif

#if defined OS_LYNX || defined OS_LINUX
# include <pthread.h>
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_gdh_h
# include "rt_gdh.h"
#endif

typedef	struct plc_sTimertag {	
  pwr_tBoolean                        TimerFlag;
/*  struct plc_sTimertag	*TimerNext; */
  pwr_tBoolean                        *TimerNext;
  pwr_tUInt32                         TimerCount;
  pwr_tBoolean                        *TimerDO;
  pwr_tFloat32                        TimerTime;
  pwr_tBoolean                        TimerDODum;
  pwr_tObjid                          TimerObjDId;
  pwr_tInt32                          TimerAcc;
  pwr_tFloat32                        TimerMin;
  pwr_tFloat32                        TimerMax;
} plc_sTimer;

#define PTRREL(r, p) \
  gdh_StoreRtdbPointer((pwr_tUInt32 *)r, (void *)p )

#define RELPTR(r) \
((unsigned int)r == 0) ? NULL : gdh_TranslateRtdbPointer((pwr_tUInt32)r)

/*	Insert timer into timer-queue	*/
#ifdef OS_ELN

  extern MUTEX timer_mutex;
# define PLC_LOCK_MUTEX(m)  	ELN$LOCK_MUTEX(m)
# define PLC_UNLOCK_MUTEX(m)  	ELN$UNLOCK_MUTEX(m)

#elif defined OS_LYNX  || defined OS_LINUX

  extern pthread_mutex_t timer_mutex;
# define PLC_LOCK_MUTEX(m)  pthread_mutex_lock(&m)
# define PLC_UNLOCK_MUTEX(m)  pthread_mutex_unlock(&m)

#elif defined OS_VMS

# define timer_mutex 0
# define PLC_LOCK_MUTEX(m) 
# define PLC_UNLOCK_MUTEX(m)

#endif

#define	timer_in(tp, o)	{				\
  o->TimerCount = o->TimerTime / tp->f_scan_time;	\
  if ( !o->TimerFlag && (o->TimerCount > 0)) {		\
    o->TimerFlag = TRUE;				\
    /*PLC_LOCK_MUTEX(tp->timer_mutex);*/		\
    (o->TimerNext) = tp->PlcThread->TimerStart;		\
    PTRREL(&tp->PlcThread->TimerStart, &o->TimerFlag );	\
    /*PLC_UNLOCK_MUTEX(tp->timer_mutex);*/		\
  }							\
}

#ifndef rt_plc_h
# include "rt_plc.h"
#endif

void plc_inittimer (plc_sThread *tp);
void plc_timerhandler(plc_sThread *tp);

#endif
