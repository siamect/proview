/* 
 * Proview   $Id: rt_plc_timer.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_plc_timer_h
#define rt_plc_timer_h 

#ifdef OS_ELN
# include $mutex
#endif

#if defined OS_LYNX || defined OS_LINUX || defined OS_MACOS || defined OS_FREEBSD
# include <pthread.h>
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_gdh_h
# include "rt_gdh.h"
#endif

typedef	struct {	
  pwr_tBoolean                        TimerFlag pwr_dAlignLW;
  pwr_tBoolean                        *TimerNext pwr_dAlignLW;
  pwr_tUInt32                         TimerCount pwr_dAlignLW;
  pwr_tBoolean                        *TimerDO pwr_dAlignLW;
  pwr_tFloat32                        TimerTime pwr_dAlignLW;
  pwr_tBoolean                        TimerDODum pwr_dAlignW;
  pwr_tObjid                          TimerObjDId pwr_dAlignW;
  pwr_tInt32                          TimerAcc pwr_dAlignW;
  pwr_tFloat32                        TimerMin pwr_dAlignW;
  pwr_tFloat32                        TimerMax pwr_dAlignW;
} plc_sTimer;

#define PTRREL(r, p) \
  gdh_StoreRtdbPointer((unsigned long *)r, (void *)p )

#define RELPTR(r) \
((unsigned long)r == 0) ? NULL : gdh_TranslateRtdbPointer((unsigned long)r)

/*	Insert timer into timer-queue	*/
#ifdef OS_ELN

  extern MUTEX timer_mutex;
# define PLC_LOCK_MUTEX(m)  	ELN$LOCK_MUTEX(m)
# define PLC_UNLOCK_MUTEX(m)  	ELN$UNLOCK_MUTEX(m)

#elif defined OS_LYNX  || defined OS_LINUX || defined OS_MACOS || defined OS_FREEBSD

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
