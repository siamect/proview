/* rt_que.c -- <short description>

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
# include <lib$routines.h>
#endif

#include "pwr.h"
#include "pwr_msg.h"
#include "rt_errh.h"
#include "rt_que.h"
#include "rt_que_msg.h"
#include "rt_thread.h"
#include "rt_thread_msg.h"
#include "rt_lst.h"


que_sQue *
que_Create (
  pwr_tStatus *status,
  que_sQue *qp
)
{
  que_sQue *lqp = NULL;
  pwr_dStatus(sts, status, QUE__SUCCESS);

  if (qp == NULL) {
    lqp = qp = (que_sQue *) calloc(1, sizeof(*qp));
    if (lqp == NULL)
      pwr_Return(lqp, sts, QUE__INSVIRMEM);
  }

  thread_CondInit(&qp->cond);
  thread_MutexInit(&qp->mutex);
  lst_Init(NULL, &qp->lh, NULL);

  return qp;
}

void *
que_Get (
  pwr_tStatus *status,
  que_sQue *qp,
  pwr_tDeltaTime *tp,
  void *tmo_item
)
{
  void *p = NULL;
  pwr_dStatus(sts, status, QUE__SUCCESS);

  thread_MutexLock(&qp->mutex);

    while((p = lst_RemoveSucc(NULL, &qp->lh, NULL)) == NULL && ODD(*sts) && *sts != THREAD__TIMEDOUT)
      *sts = thread_CondTimedWait(&qp->cond, &qp->mutex, tp);

  thread_MutexUnlock(&qp->mutex);

  return p == NULL ? tmo_item : p;
}

void
que_Put (
  pwr_tStatus *status,
  que_sQue *qp,
  lst_sEntry *lp,
  void *item
)
{
  pwr_dStatus(sts, status, QUE__SUCCESS);

  thread_MutexLock(&qp->mutex);

    lst_InsertPred(NULL, &qp->lh, lp, item);
    thread_CondSignal(&qp->cond);

  thread_MutexUnlock(&qp->mutex);
}
