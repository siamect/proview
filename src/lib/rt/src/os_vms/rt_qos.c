/* rt_qos.c -- Queue Communication

   PROVIEW/R
   Copyright (C) 1998 by Comator Process AB.

   Contains functions that are heavily os-dependant.  */

#if !defined(OS_VMS)
# error "This file is valid only for OS_VMS"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <starlet.h>
#include <ssdef.h>
#include <lckdef.h>
#include <lib$routines.h>
#include <descrip.h>
#include <errno.h>
#include "pwr.h"
#include "rt_sync.h"
#include "rt_qos.h"
#include "rt_qdb_msg.h"
#include "rt_qcom_msg.h"
#include "rt_qdb.h"

typedef struct {
  qdb_sQue	*qp;
  pwr_tBoolean	signal;
  pwr_tBoolean	timeout;
} sWait;


static void
waitQueBlast (
  sWait		*wp
)
{

  wp->signal = TRUE;
  sys$enq(0, LCK$K_NLMODE, &wp->qp->lock.enq, LCK$M_CONVERT, 0, 0, 0, 0, 0, 0, 0);
  sys$setef(qdb->ap->ef);
  if (qdb->thread_lock.isThreaded)
    qdb->thread_lock.cond_signal(&qdb->thread_lock.cond);
}

static void
timerAst (
  sWait		*wp
)
{

  wp->timeout = TRUE;
  if (qdb->thread_lock.isThreaded)
    qdb->thread_lock.cond_signal(&qdb->thread_lock.cond);
}

/* Wait for a que to be signaled or timed out.
   Return true if the que was signaled. */

pwr_tBoolean
qos_WaitQue (
  pwr_tStatus	*status,
  qdb_sQue	*qp,
  int		tmo
)
{
  pwr_tStatus	lsts;
  pwr_tBoolean	signal = FALSE;
  int		dtime[2];
  int		multiplier = -10000;	      /* Used to convert 1 ms to 100 ns, delta time.  */
  int		addend = 0;
  sWait		wait = {NULL, FALSE, FALSE};
  int		mask;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  if (tmo == qcom_cTmoNone)
    return FALSE;

  if (qdb->thread_lock.isThreaded) {
    pwr_Assert(qdb->thread_lock.cond_signal != NULL);
    pwr_Assert(qdb->thread_lock.cond_wait != NULL);
  }

  qp->lock.waiting = TRUE;
  wait.qp = qp;

  if (tmo != qcom_cTmoEternal) {
    lsts = lib$emul(&multiplier, &tmo, &addend, dtime);
    if (qdb->thread_lock.isThreaded)
      lsts = sys$setimr(qdb->ap->ef, dtime, timerAst, &wait, 0);
    else
      lsts = sys$setimr(qdb->ap->ef, dtime, 0, &wait, 0);
  } else {
    lsts = sys$clref(qdb->ap->ef);
  }

  lsts = sys$enq(0, LCK$K_PWMODE, &qp->lock.enq,
    LCK$M_CONVERT|LCK$M_NODLCKWT|LCK$M_NODLCKBLK, 0, 0, 0, &wait, waitQueBlast, 0, 0);

  qdb_UnlockGlobal;

    if (qdb->thread_lock.isThreaded) {
      while ((lsts = sys$readef(qdb->ap->ef, &mask)) == SS$_WASCLR)
	qdb->thread_lock.cond_wait(&qdb->thread_lock.cond, &qdb->thread_lock.mutex);
      pwr_Assert(lsts == SS$_WASSET);
    } else {
      lsts = sys$waitfr(qdb->ap->ef);
    }

  qdb_LockGlobal;

  if (tmo != qcom_cTmoEternal && !wait.timeout)
    lsts = sys$cantim(&wait, 0);

  if (qp->lock.waiting) {
    *sts = QCOM__TMO;
    qp->lock.waiting = FALSE;
    lsts = sys$enq(0, LCK$K_NLMODE, &qp->lock.enq, LCK$M_CONVERT, 0, 0, 0, 0, 0, 0, 0);
  } else {
    signal = TRUE;
  }

  return signal;
}

pwr_tBoolean
qos_SignalQue (
  pwr_tStatus	*status,
  qdb_sQue	*qp
)
{
  qdb_sQlock	lock;
  struct dsc$descriptor_s name;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  if (qp->lock.waiting) {

    name.dsc$w_length  = qp->lock.nlen;
    name.dsc$b_dtype   = DSC$K_DTYPE_T;
    name.dsc$b_class   = DSC$K_CLASS_S;
    name.dsc$a_pointer = qp->lock.name;

    qp->lock.waiting = FALSE;

    *sts = sys$enq(0, LCK$K_PWMODE, &lock, 0, &name, 0, 0, 0, 0, 0, 0);
    *sts = sys$deq(lock.enq.id, 0, 0, 0);

  }

  return TRUE;

# if 0
  /* alternative way to signal a VMS porcess */
  if (qp->lock.waiting == 1) {/* unlockQue(qp);  */
    *sts = sys$wake(&qp->aid.pid, 0);
  }
#endif

}

void
qos_Platform (
  qdb_sNode	*np
)
{

  np->os = qcom_eOS_VMS;

#if defined(vax) || defined(__vax)
  np->hw = qcom_eHW_VAX;
  np->bo = qcom_eBO_little;
  np->ft = qcom_eFT_vaxF;
#elif defined(__Alpha_AXP) || defined(__ALPHA)
  np->hw = qcom_eHW_Alpha;
  np->bo = qcom_eBO_little;
  np->ft = qcom_eFT_vaxF;
#else
# error "Unknown OpenVMS hardware"
#endif

}

qdb_sQlock *
qos_CreateQlock (
  pwr_tStatus	*status,
  qdb_sQue	*qp
)
{
  struct dsc$descriptor_s name;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  sprintf(qp->lock.name, "qcom_q%d-%d-%d", qdb->g->bus, qdb->my_nid, qp->qix);
  qp->lock.nlen = strlen(qp->lock.name);

  name.dsc$w_length  = qp->lock.nlen;
  name.dsc$b_dtype   = DSC$K_DTYPE_T;
  name.dsc$b_class   = DSC$K_CLASS_S;
  name.dsc$a_pointer = qp->lock.name;

  *sts = sys$enqw(0, LCK$K_NLMODE, &qp->lock.enq, 0, &name, 0, 0, 0, 0, 0, 0);

  return &qp->lock;
}

void
qos_DeleteQlock (
  pwr_tStatus	*status,
  qdb_sQue	*qp
)
{
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  sys$deq(qp->lock.enq.id, 0, 0, LCK$M_CANCEL);
  sys$deq(qp->lock.enq.id, 0, 0, 0);
}

#if 0
static void
lockQue (
  qdb_sQue	*qp
);

static void
unlockQue (
  qdb_sQue	*qp
);

static void
timerAst (
  qdb_sQue	*qp
);

static void
lockQue (
  qdb_sQue	*qp
)
{

  pthread_mutex_lock(&qp->mutex);
}

static void
unlockQue (
  qdb_sQue	*qp
)
{

  pthread_mutex_unlock(&qp->mutex);
}

pwr_tBoolean
qos_WaitQue (
  pwr_tStatus	*sts,
  qdb_sQue	*qp,
  int		tmo
)
{

  qdb_AssumeLocked;

  if (tmo == qcom_cTmoNone)
    return NO;

  qp->lock.waiting = TRUE;


    pthread_cond_timedwait(&q->cond, &q->mutex, tmo);

    {
      int dtime[2];
      int multiplier = -10000;	      /* Used to convert 1 ms to 100 ns, delta time.  */
      int addend = 0;


      if (tmo != qcom_cTmoEternal) {
	lib$emul(&multiplier, &tmo, &addend, dtime);
	sys$setimr(0, dtime, timerAst, qp, 0);
      }

      qdb_Unlock;

	sys$hiber();

      qdb_Lock;

      if (qp->lock.waiting) {
	if (tmo != qcom_cTmoEternal)
	  sys$cantim(qp, 0);
	qp->lock.waiting = FALSE;
      } else {
      }

    }

  return *sts != QCOM__TMO;
}
# endif
