/* 
 * Proview   $Id: rt_thread.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_thread_h
#define rt_thread_h

#include "pwr.h"

#if defined OS_VMS || defined OS_LYNX || defined OS_LINUX || defined OS_MACOS || defined OS_FREEBSD
#include <pthread.h>

typedef pthread_t	thread_s;

typedef struct {
  pthread_cond_t 	c;
  pwr_tBoolean		f;
} thread_sCond;

typedef pthread_mutex_t thread_sMutex;

#elif defined OS_ELN
# include $mutex

  typedef struct {
    PROCESS		id;
    char		name[32];
    NAME		name_id;
  } thread_s;

  typedef EVENT		thread_sCond;
  typedef MUTEX		thread_sMutex;

#else
# error Not defined for this platform !
#endif

typedef enum {
  thread_ePrio__,
  thread_ePrio_high,
  thread_ePrio_low,
  thread_ePrio_
} thread_ePrio;

pwr_tStatus	thread_CondInit		(thread_sCond*);
pwr_tStatus	thread_CondSignal	(thread_sCond*);
pwr_tStatus	thread_CondTimedWait	(thread_sCond*, thread_sMutex*, pwr_tDeltaTime*);
pwr_tStatus	thread_CondWait		(thread_sCond*, thread_sMutex*);
pwr_tStatus	thread_MutexInit	(thread_sMutex*);
pwr_tStatus	thread_MutexLock	(thread_sMutex*);
pwr_tStatus	thread_MutexUnlock	(thread_sMutex*);
pwr_tStatus	thread_Cancel		(thread_s*);
pwr_tStatus	thread_Create		(thread_s*, char*, void *(*routine)(), void*);
pwr_tStatus	thread_Wait		(pwr_tDeltaTime*);
pwr_tStatus	thread_SetPrio		(thread_s*, int);
pwr_tStatus	thread_Join		(thread_s*, pwr_tStatus*);
pwr_tStatus	thread_Signal		(thread_s*, int);
pwr_tBoolean	thread_SigTimedWait	(thread_s*, int, pwr_tDeltaTime*);
#if 0
  thread_s	thread_Self		();
#endif

#endif
