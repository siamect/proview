/* rt_qos.c -- Queue Communication

   PROVIEW/R
   Copyright (C) 1998 by Comator Process AB.

   Contains functions that are heavily os-dependant.  */

#if !defined(OS_ELN)
# error "This file is valid only for OS_ELN"
#endif

#include $vaxelnc
#include stdarg
#include stsdef
#include descrip
#include <errno.h>
#include "pwr.h"
#include "pwr_class.h"
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
  pwr_tStatus	*status,
  qdb_sQue	*qp,
  int		tmo
)
{
  pwr_tStatus	lsts;
  pwr_tBoolean	signal = FALSE;
  int		wait_result;
  int		dtime[2];
  int		*dtp;
  int		multiplier = -10000;	      /* Used to convert 1 ms to 100 ns, delta time.  */
  int		addend = 0;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  if (tmo == qcom_cTmoNone)
    return NO;

  qp->lock.waiting = TRUE;

  if (tmo != qcom_cTmoEternal) {
    lib$emul(&multiplier, &tmo, &addend, dtime);
    dtp = &dtime;
  } else {
    dtp = NULL;
  }

  ker$clear_event(&lsts, qp->lock.id);

  qdb_Unlock;

    ker$wait_any(&lsts, &wait_result, dtp, qp->lock.id);
    
  qdb_Lock;

  if (qp->lock.waiting) {
    *sts = QCOM__TMO;
    qp->lock.waiting = FALSE;
  } else {
    signal = TRUE;
  }

  return signal;
}

pwr_tStatus
qos_SignalQue (
  pwr_tStatus	*status,
  qdb_sQue	*qp
)
{
  qdb_sQlock	lock;
  void		*dummy;
  struct dsc$descriptor_s name;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  if (qp->lock.waiting) {

    name.dsc$w_length  = qp->lock.nlen;
    name.dsc$b_dtype   = DSC$K_DTYPE_T;
    name.dsc$b_class   = DSC$K_CLASS_S;
    name.dsc$a_pointer = qp->lock.name;

    qp->lock.waiting = FALSE;

    ker$create_area_event(sts, &lock.id, &dummy, 0, &name, EVENT$CLEARED, NULL);
    ker$signal(sts, lock.id);
    ker$delete(sts, lock.id);
  }

  return *sts;
}

qdb_sQlock *
qos_CreateQlock (
  pwr_tStatus	*status,
  qdb_sQue	*qp
)
{
  void		*dummy;
  struct dsc$descriptor_s name;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_AssumeLocked;

  sprintf(qp->lock.name, "qcom_q%d-%d-%d", qdb->g->bus, qdb->my_nid, qp->qix);
  qp->lock.nlen = strlen(qp->lock.name);

  name.dsc$w_length  = qp->lock.nlen;
  name.dsc$b_dtype   = DSC$K_DTYPE_T;
  name.dsc$b_class   = DSC$K_CLASS_S;
  name.dsc$a_pointer = qp->lock.name;

  ker$create_area_event(sts, &qp->lock.id, &dummy, 0, &name, EVENT$CLEARED, NULL);

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

  ker$delete(sts, qp->lock.id);
}
