/* rt_qimp.c -- Qcom importer.

   PROVIEW/R
   Copyright (C) 1998 by Mandator AB.

   Imports buffers from other nodes.  */

#include <stdlib.h>
#include <string.h>
#include "pwr.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_qcom.h"
#include "rt_qdb.h"
#include "rt_qnet.h"
#include "rt_qcom_msg.h"
#include "rt_qdb_msg.h"

static void	down		(qdb_sNode *np, char *s);
static void	freeBuffer	(qdb_sBuffer *bp);
static void	idError		(qdb_sPort *pp, qdb_sNode *np, void *mp, char *s);
static void	impossibleEvent	(qdb_sNode *np, char *s);
static void	linkEvent	(qdb_sPort *pp, qdb_sNode *np);
static void	linkNotification(qdb_sNode *np);
static void	nullId		(qdb_sNode *np);
static void	putUser		(qdb_sPort *pp, qdb_sNode *np, qdb_sBuffer *bp);
static void	removeBcast	(qdb_sNode *np);
static void	serviceEvent	(qdb_sPort *pp, qdb_sNode *np, qdb_sBuffer *bp);
static void	setId		(qdb_sNode *np);
static void	setLinkInfo	(qdb_sNode *np, qdb_sLink *lp);
static void	stateConnecting	(qdb_sPort *pp, qdb_sNode *np, qdb_sBuffer *bp);
static void	stateDown	(qdb_sPort *pp, qdb_sNode *np, qdb_sBuffer *bp);
static void	stateStarting	(qdb_sPort *pp, qdb_sNode *np, qdb_sBuffer *bp);
static void	stateUp		(qdb_sPort *pp, qdb_sNode *np, qdb_sBuffer *bp);
static void	unhandledEvent	(qdb_sNode *np);
static void	up		(qdb_sNode *np, qdb_sBuffer *bp, char *s);


int
main (int argc, char *argv[])
{
  pwr_tStatus	sts = 1;
  qdb_sPort	port;
  qdb_sPort	*pp;
  qcom_sQid	qid = qdb_cQimport;
  qdb_sNode	*np;

  errh_Init("pwr_qimp");

  if (!qcom_Init(&sts, NULL)) exit(sts);

  if (!qcom_CreateQ(&sts, &qid, NULL)) exit(sts);

  port.sa.sin_family = AF_INET;  
  port.sa.sin_addr.s_addr = htonl(INADDR_ANY);
  port.sa.sin_port = qdb->my_node->sa.sin_port;

  pp = qnet_Open(&sts, &port);

  while (qdb->g->up) {
    np = qnet_Get(&sts, pp);
    if (np->bp == NULL) pwr_Bugcheck(QCOM__WEIRD, "(np->bp == NULL)");
    linkEvent(pp, np);
  }

  qnet_Close(&sts, pp);
  qcom_Exit(&sts);

  exit(QCOM__SUCCESS);
}

static void
down (
  qdb_sNode   *np,
  char	      *s
)
{

  if (np->state == qdb_eState_down)
    return;

  qdb_ScopeLock {
    np->state = qdb_eState_down;
    nullId(np);
    clock_gettime(CLOCK_REALTIME, &np->timedown);
    removeBcast(np);
    linkNotification(np);
  } qdb_ScopeUnlock;

  errh_Info("Down, link to node %s (%s): %s", np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), s);
}

static void
freeBuffer (
  qdb_sBuffer	*bp
)
{
  pwr_tStatus	sts = 1;

  if (bp != NULL) {
    qdb_ScopeLock {

      qdb_Free(&sts, bp);

    } qdb_ScopeUnlock;
  }

}

static void
idError (
  qdb_sPort   *pp,
  qdb_sNode   *np,
  void	      *mp,
  char	      *s
)
{
  pwr_tStatus sts;

  qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_hello);
  np->state = qdb_eState_starting;
  errh_Info("Id error, node %s (%s): %s", np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), s);
  nullId(np);
}

static void
impossibleEvent (
  qdb_sNode   *np,
  char	      *s
)
{

  errh_Info("Impossible event <%s>, node %s (%s): %s", qnet_EventString(np->event), 
    np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), s);
}

/* This is the link state machine
   The database must be locked by the caller.  */

static void
linkEvent (
  qdb_sPort	*pp,
  qdb_sNode	*np
)
{
  qdb_sBuffer	*bp;
  qdb_sLink	*lp;

  qdb_AssumeUnlocked;

  bp = np->bp;
  np->bp = NULL;

  if (np->nid == qdb->my_node->nid) {
    /* Never to ourselves... */
    freeBuffer(bp);
    return;
  }

  np->old_state = np->state;

  if (np->old_birth != 0 && np->old_birth != np->birth) {
    np->event = qdb_eEvent_idError;
  } else if (bp->b.type == qdb_eType_link) {
    lp = (qdb_sLink *)(bp + 1);
    np->event = lp->event;
  } else if (bp->b.type == qdb_eType_user) {
    np->event = qdb_eEvent_user;
  } else if (bp->b.type == qdb_eType_service) {
    serviceEvent(pp, np, bp);
    return;
  } else {
    pwr_Bugcheck(2, "");
  }

  if (qdb->g->log.b.events)
    errh_Info("Node %s (%s) event <%s>", np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), 
      qnet_EventString(np->event));

  switch (np->state) {
  case qdb_eState_down:
    stateDown(pp, np, bp);
    break;
  case qdb_eState_starting:
    stateStarting(pp, np, bp);
    break;
  case qdb_eState_connecting:
    stateConnecting(pp, np, bp);
    break;
  case qdb_eState_up:
    stateUp(pp, np, bp);
    break;
  default:
    break;
  }

  if (qdb->g->log.b.states && np->old_state != np->state)
    errh_Info("Node %s (%s): new state: %s -> %s", np->name,
      cdh_NodeIdToString(NULL, np->nid, 0, 0),
      qnet_StateString(np->old_state), qnet_StateString(np->state));

}

static void
linkNotification (
  qdb_sNode	*np
)
{
  pwr_tStatus	sts;
  
  qdb_AssumeLocked;

  switch (np->state) {
  case qdb_eState_down:
    qdb_NetEvent(&sts, np, qcom_eStype_linkDown);
    break;
  case qdb_eState_up:
    qdb_NetEvent(&sts, np, qcom_eStype_linkUp);
    break;
  default:
    break;
  }

  np->qmon_state = np->state;

}

static void
nullId (
  qdb_sNode *np
)
{

  np->old_birth = 0;
}

static void
putUser (
  qdb_sPort	*pp,
  qdb_sNode	*np,
  qdb_sBuffer	*bp
)
{
  pwr_tStatus	sts = QCOM__SUCCESS;
  pwr_tStatus	lsts = QCOM__SUCCESS;
  qdb_sQue	*qp;
  qcom_sQid	qid = bp->b.info.receiver;
  qdb_sRequest	*rp;
  pool_sQlink	*rbl;
  qdb_sBuffer	*rbp;
  qdb_sBuffer	*nbp;
  qdb_sInfo	info;
  qdb_mBuffer	flags;

  qdb_ScopeLock {
    flags.m = bp->c.flags.m;
    qp = qdb_Que(&sts, &qid, NULL);
    if (bp->c.flags.b.request) {
      if (qp == NULL) {
	sts = QCOM__NOQ;
      } else if (qp->type != qdb_eQue_private) {
	sts = QCOM__QTYPE;
      } else {
	rp = qdb_AddRequest(&sts, bp, np);
	if (rp == NULL)
	  break;
	if (sts == QDB__ALRADD) {
	  if (!pool_QisEmpty(&sts, &qdb->pool, &rp->b_lh)) {
	    rbl = pool_Qsucc(&sts, &qdb->pool, &rp->b_lh);
	    rbp = pool_Qitem(rbl, qdb_sBuffer, c.ll);
	    nbp = qdb_CopyBuffer(&sts, rbp);
	    if (nbp == NULL)
	      break;
	    qdb_Put(NULL, nbp, qdb->export);
	  }
	  qdb_Free(NULL, bp);
	} else {
	  qdb_Put(&sts, bp, qp);
	}
      }
    } else if (bp->c.flags.b.broadcast) {
      info = bp->b.info;
      if (qp == NULL || np->bc_rcv_id == bp->b.info.rid) {
	qdb_Free(NULL, bp);
      } else {
	np->bc_rcv_id = bp->b.info.rid;
	qdb_Put(NULL, bp, qp);
      }
    } else {
      if (qp == NULL) {
	qdb_Free(NULL, bp);
      } else {
	qdb_Put(NULL, bp, qp);
      }
    }
  } qdb_ScopeUnlock;

  if (flags.b.broadcast) {
#if 0
    if (1) errh_Info("Sending broadcast ack: %d", info.rid);
#endif
    qnet_SendBcastAck(&lsts, pp, np, &info);
  }

  if (EVEN(sts) && flags.b.request) {
    qnet_SendRequestError(&lsts, pp, np, bp, sts);
    qdb_ScopeLock {
      qdb_Free(NULL, bp);
    } qdb_ScopeUnlock;
  }
}

static void
removeBcast (
  qdb_sNode	*np
)
{
  pool_sQlink	*bcl;
  qdb_sBuffer	*bp;

  qdb_AssumeLocked;

  if (np == qdb->my_node || np == qdb->no_node || np->state != qdb_eState_up)
    return;

  for (
    bcl = pool_Qsucc(NULL, &qdb->pool, &np->bcb_lh);
    bcl != &np->bcb_lh;
    bcl = pool_Qsucc(NULL, &qdb->pool, &np->bcb_lh)
  ) {
    bp = pool_Qitem(bcl, qdb_sBuffer, c.ll);

    pool_Qremove(NULL, &qdb->pool, &bp->c.ll);
    pool_Free(NULL, &qdb->pool, bp);
  }

  np->bc_rcv_id = 0;

}

static void
serviceEvent (
  qdb_sPort	*pp,
  qdb_sNode	*np,
  qdb_sBuffer	*bp
)
{
  pool_sQlink	*bcbl;
  qdb_sBuffer	*bcbp;

  qdb_AssumeUnlocked;

  if (np == qdb->my_node || np == qdb->no_node)
    return;

#if 0
  errh_Info("ServiceEvent, idx: %d, nid: %d", bp->b.info.rid, bp->b.info.sender.nid);
#endif
  qdb_ScopeLock {

    bcbl = pool_Qsucc(NULL, &qdb->pool, &np->bcb_lh);
    if (bcbl == NULL)
      break;
    bcbp = pool_Qitem(bcbl, qdb_sBuffer, c.ll);
    
    if (bp->b.info.rid == bcbp->b.info.rid) {
      pool_Qremove(NULL, &qdb->pool, &bcbp->c.ll);
      pool_Free(NULL, &qdb->pool, bcbp);
    }

    qdb_Free(NULL, bp);

  } qdb_ScopeUnlock;
}

static void
setId (
  qdb_sNode *np
)
{

  np->old_birth = np->birth;
}

static void
setLinkInfo (
  qdb_sNode   *np,
  qdb_sLink  *lp
)
{

  np->birth   = lp->birth;
  strcpy(np->name, lp->name);
  np->version = lp->version;
  np->os      = lp->os;
  np->hw      = lp->hw;
  np->bo      = lp->bo;
  np->ft      = lp->ft;
}

/* State: connecting

   I know that node 'np' is up and knows about me.  */

static void
stateConnecting (
  qdb_sPort	*pp,
  qdb_sNode	*np,
  qdb_sBuffer	*bp
)
{
  pwr_tStatus	sts;

  switch (np->event) {
  case qdb_eEvent_alive:
  case qdb_eEvent_user:
  case qdb_eEvent_hello:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_hello);
    np->state = qdb_eState_starting;
    break;
  case qdb_eEvent_connect:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_up);
    up(np, bp, "");
    break;
  case qdb_eEvent_up:
    up(np, bp, "");
    break;
  case qdb_eEvent_down:
    down(np, "contact remotely terminated");
    break;
  case qdb_eEvent_idError:
    idError(pp, np, bp, "");
    break;
  default:
    unhandledEvent(np);
    break;
  }

  freeBuffer(bp);
}

/* State: down
  
   I know about node 'np' but communications where
   terminated by other node.  */

static void
stateDown (
  qdb_sPort	*pp,
  qdb_sNode	*np,
  qdb_sBuffer	*bp
)
{
  pwr_tStatus	sts;

  switch (np->event) {
  case qdb_eEvent_alive:
  case qdb_eEvent_user:
  case qdb_eEvent_connect:
  case qdb_eEvent_up:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_hello);
    np->state = qdb_eState_starting;
    break;
  case qdb_eEvent_hello:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_connect);
    setId(np);
    np->state = qdb_eState_connecting;
    break;
  case qdb_eEvent_down:
    break;
  case qdb_eEvent_idError:
    pwr_Bugcheck(2, "we have not set id yet");
    break;
  default:
    unhandledEvent(np);
    break;
  }

  freeBuffer(bp);

}

/* State: starting

   I know that node 'np' exists, but I don't know
   if it's up or if it knows about me.  */

static void
stateStarting (
  qdb_sPort	*pp,
  qdb_sNode	*np,
  qdb_sBuffer	*bp
)
{
  pwr_tStatus	sts;

  switch (np->event) {
  case qdb_eEvent_alive:
  case qdb_eEvent_user:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_hello);
    break;
  case qdb_eEvent_hello:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_connect);
    setId(np);
    np->state = qdb_eState_connecting;
    break;
  case qdb_eEvent_connect:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_up);
    up(np, bp, "");
    break;
  case qdb_eEvent_up:
    impossibleEvent(np, "");
    break;
  case qdb_eEvent_down:
    down(np, "contact with node, remotely terminated");
    break;
  case qdb_eEvent_idError:
    impossibleEvent(np, "we have not set id yet");
    break;
  default:
    unhandledEvent(np);
    break;
  }

  freeBuffer(bp);

}

/* State: up
  
   I am talking to node 'np'.  */

static void
stateUp (
  qdb_sPort	*pp,
  qdb_sNode	*np,
  qdb_sBuffer	*bp
)
{
  pwr_tStatus	sts = 1;

  switch (np->event) {
  case qdb_eEvent_alive:
    break;
  case qdb_eEvent_user:
    putUser(pp, np, bp);
    return; /* NOTA BENE, you must return, we dont want this buffer to be freed! */
    break;
  case qdb_eEvent_connect:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_up);
    break;
  case qdb_eEvent_hello:
    qnet_SendLinkEvent(&sts, pp, np, qdb_eEvent_connect);
    break;
  case qdb_eEvent_up:
    break;
  case qdb_eEvent_down:
    down(np, "contact remotely terminated");
    break;
  case qdb_eEvent_idError:
    idError(pp, np, bp, "is restarted, new contact initiated...");
    break;
  default:
    unhandledEvent(np);
    break;
  }

  freeBuffer(bp);
}

static void
unhandledEvent (
  qdb_sNode   *np
)
{

  errh_Info("Unhandled event, node %s (%s): %s", np->name,
    cdh_NodeIdToString(NULL, np->nid, 0, 0), qnet_EventString(np->event));
}

static void
up (
  qdb_sNode   *np,
  qdb_sBuffer *bp,
  char	      *s
)
{
  qdb_sLink  *lp = (qdb_sLink *) (bp + 1);

  qdb_ScopeLock {
    np->state = qdb_eState_up;
    setId(np);
    clock_gettime(CLOCK_REALTIME, &np->timeup);
    removeBcast(np);
    setLinkInfo(np, lp);
    linkNotification(np);
  } qdb_ScopeUnlock;

  errh_Info("Up, link to node %s (%s): %s", np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), s);
}
