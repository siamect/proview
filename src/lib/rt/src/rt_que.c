/* 
 * Proview   $Id: rt_que.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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
