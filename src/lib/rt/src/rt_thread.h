#ifndef rt_thread_h
#define rt_thread_h

/* rt_thread.h -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */

#include "pwr.h"

#if defined OS_VMS || defined OS_LYNX || defined OS_LINUX
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
