/* 
 * Proview   $Id: rt_qos.c,v 1.8 2005-09-01 14:57:57 claes Exp $
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

/* rt_qos.c -- Queue Communication

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
#include "rt_futex.h"


pwr_tBoolean
qos_WaitQueOld (
  pwr_tStatus		*status,
  qdb_sQue		*qp,
  int			tmo
)
{
  pwr_tDeltaTime	dtime;
  sigset_t		newset;
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

  return signal;
}

pwr_tBoolean
qos_WaitQue (
  pwr_tStatus		*status,
  qdb_sQue		*qp,
  int			tmo
)
{
  pwr_tDeltaTime	dtime;
  int			ok;
  pwr_tBoolean		signal = FALSE;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  if (tmo == qcom_cTmoNone)
    return FALSE;

  qp->lock.waiting = TRUE;
  qp->lock.pid     = 0;

  qdb_Unlock;

  if (qp->lock.waiting) {
    if (tmo != qcom_cTmoEternal) {
      ok = futex_timed_wait(&(qp->lock.pid), 0, (struct timespec *) time_MsToD(&dtime, tmo));
    } else {
      for (;;) {
        ok = futex_wait(&(qp->lock.pid), 0);
        if (ok == EINTR)
          continue;
        break;
      }
    }
    
    if (ok == EWOULDBLOCK) {
      errh_Error("waitQue - Deadlock would occur");
    }
  }
  
  qdb_Lock;

  if ((qp->lock.waiting) || (ok == ETIMEDOUT)) {
    *sts = QCOM__TMO;
    qp->lock.waiting = FALSE;
  } else {
    signal = TRUE;
  }

  return signal;
}

pwr_tStatus
qos_SignalQueOld (
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

pwr_tStatus
qos_SignalQue (
  pwr_tStatus	*status,
  qdb_sQue	*qp
)
{
  int		ok;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  // if (qp->lock.waiting) {
    qp->lock.waiting = FALSE;

    ok = futex_wake(&(qp->lock.pid), INT_MAX);

  // }

  return TRUE;
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
