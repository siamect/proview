/* rt_qos.c -- Queue Communication

   PROVIEW/R
   Copyright (C) 1998 by Mandator AB.

   Contains functions that are heavily os-dependant.  */

#if !defined(OS_LINUX)
# error "This file is valid only for OS_LINUX"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "pwr.h"
#include "pwr_class.h"
#include "co_errno.h"
#include "co_platform.h"
#include "rt_qdb_msg.h"
#include "rt_qcom_msg.h"
#include "rt_hash_msg.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_qdb.h"
#include "rt_pool.h"
#include "rt_hash.h"


pwr_tBoolean
qos_WaitQue (
  pwr_tStatus		*status,
  qdb_sQue		*qp,
  int			tmo
)
{
  pwr_tDeltaTime	dtime;
  sigset_t		newset;
  sigset_t		oldset;
  siginfo_t		info;
  int			ok;
  pwr_tBoolean		signal = FALSE;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  if (tmo == qcom_cTmoNone)
    return FALSE;

  qp->lock.waiting = TRUE;

  sigemptyset(&newset);
  sigaddset(&newset, qdb_cSigMsg);
  sigprocmask(SIG_BLOCK, &newset, &oldset);

//  qp->lock.pid = BUILDPID(getpid(), pthread_self());
// I think that each thread has it's own pid in Linux. ML
  qp->lock.pid = getpid();

  qdb_Unlock;

    if (tmo != qcom_cTmoEternal) {
      ok = sigtimedwait(&newset, &info, (struct timespec *)time_MsToD(&dtime, tmo));  
    } else {
      for (;;) {
        ok = sigwaitinfo(&newset, &info);
        if ( ok == -1 && errno == EINTR)
          continue;
        break;
      }
    }
    
    if (ok == -1 && errno != EAGAIN) {
      errh_Error("waitQue (%d) %s", errno, strerror(errno));
    }
    else if (!(ok == -1 || ok == qdb_cSigMsg)) {
      errh_Error("qos waitQue signr %d", ok);
    }

  qdb_Lock;

  if (qp->lock.waiting) {
    *sts = QCOM__TMO;
    qp->lock.waiting = FALSE;
  } else {
    signal = TRUE;
  }

  sigprocmask(SIG_SETMASK, &oldset, NULL);
  return signal;
}

pwr_tStatus
qos_SignalQue (
  pwr_tStatus	*status,
  qdb_sQue	*qp
)
{
  union sigval	value;
  int		ok;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  if (qp->lock.waiting) {
//    value.sival_int = BUILDPID(getpid(), pthread_self());
    value.sival_int = getpid();
    qp->lock.waiting = FALSE;

    ok = sigqueue(qp->lock.pid, qdb_cSigMsg, value);

    if (ok == -1) {
      *sts = errno_Status(errno);
    }
  }

  return TRUE;
}

void
qos_Platform (
  qdb_sNode	*np
)
{
    /** @todo This function is now OS independent. Move it to??? */
#if 1
  co_sPlatform platform;

  co_GetOwnPlatform(&platform);
  np->os = platform.os;
  np->hw = platform.hw;
  np->bo = platform.fm.b.bo;
  np->ft = platform.fm.b.ft;
  

#else

  np->os = qcom_eOS_Linux;

# if defined (HW_PPC)
  np->hw = qcom_eHW_PPC;
  np->bo = qcom_eBO_big;
  np->ft = qcom_eFT_ieeeS;
# elif defined(HW_X86)
  np->hw = qcom_eHW_x86;
  np->bo = qcom_eBO_little;
  np->ft = qcom_eFT_ieeeS;
# else
#   error unknown hardware
# endif
#endif
}


qdb_sQlock *
qos_CreateQlock (
  pwr_tStatus	*sts,
  qdb_sQue	*qp
)
{

  qdb_AssumeLocked;

  return &qp->lock;
}

void
qos_DeleteQlock (
  pwr_tStatus	*sts,
  qdb_sQue	*qp
)
{

  qdb_AssumeLocked;

}
