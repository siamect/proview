#ifndef rt_sync_h
#define rt_sync_h

/* rt_sync.h -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */

#if defined OS_VMS
# include <tis.h>
#endif
#include "pwr.h"
#include "rt_thread.h"

pwr_tStatus	sync_CondInit		(thread_sCond*);
pwr_tStatus	sync_MutexInit		(thread_sMutex*);
pwr_tStatus	sync_CondSignal		(thread_sCond*);
pwr_tStatus	sync_MutexLock		(thread_sMutex*);
pwr_tStatus	sync_MutexUnlock	(thread_sMutex*);

#endif
