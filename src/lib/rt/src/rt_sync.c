/* rt_sync.c -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */

#ifdef	OS_ELN
# include $vaxelnc
# include $kernelmsg
# include stdio
# include stdlib
# include string
#else
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#endif

#ifdef OS_VMS
# include <starlet.h>
# include <lib$routines.h>
# include <tis.h>
#endif

#include "pwr.h"
#include "co_errno.h"
#include "rt_errh.h"
#include "rt_sync.h"
#include "rt_sync_msg.h"


pwr_tStatus
sync_CondInit (
  thread_sCond	*cp
)
{

#if defined OS_ELN

  pwr_tStatus sts;

  ker$create_event(&sts, cp, EVENT$CLEARED);

  return sts;

#elif defined OS_LYNX && defined PWR_LYNX_30

  return errno_Pstatus(pthread_cond_init(&cp->c, NULL));

#elif defined OS_LYNX || defined OS_LINUX

  return errno_Status(pthread_cond_init(&cp->c, NULL));

#elif defined OS_VMS

  return errno_Status(tis_cond_init(&cp->c));

#else
# error Not defined for this platform !
#endif
}

pwr_tStatus
sync_CondSignal (
  thread_sCond	*cp
)
{

#if defined OS_ELN

  pwr_tStatus sts = SYNC__SUCCESS;

  ker$signal(&sts, *cp);

  return sts;

#elif defined OS_LYNX && defined PWR_LYNX_30
  cp->f = 1;
  return errno_Pstatus(pthread_cond_signal(&cp->c));

#elif defined OS_LYNX || defined OS_LINUX
  cp->f = 1;
  return errno_Status(pthread_cond_signal(&cp->c));

#elif defined OS_VMS

  return errno_Status(tis_cond_signal(&cp->c));

#else
# error Not defined for this platform !
#endif
}

pwr_tStatus
sync_MutexInit (
  thread_sMutex		*mp
)
{

#if defined OS_ELN

  pwr_tStatus sts = SYNC__SUCCESS;

  ELN$CREATE_MUTEX(*mp, &sts);

  return sts;

#elif defined OS_LYNX && defined PWR_LYNX_30

  return errno_Pstatus(pthread_mutex_init(mp, NULL));

#elif defined OS_LYNX || defined OS_LINUX

  return errno_Status(pthread_mutex_init(mp, NULL));

#elif defined OS_VMS

  return errno_Status(tis_mutex_init(mp));

#else
# error Not defined for this platform !
#endif
}

pwr_tStatus
sync_MutexLock (
  thread_sMutex		*mp
)
{

#if defined OS_ELN

  ELN$LOCK_MUTEX(*mp);
  return SYNC__SUCCESS;

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
sync_MutexUnlock (
  thread_sMutex		*mp
)
{

#if defined OS_ELN

  ELN$UNLOCK_MUTEX(*mp);
  return SYNC__SUCCESS;

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
