/* 
 * Proview   $Id: rt_qcom.c,v 1.10 2008-11-24 15:20:06 claes Exp $
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

#ifdef OS_ELN
# include $vaxelnc
# include $exit_utility
# include $function_descriptor
# include descrip
# include stdlib
# include string
#else
#include <stdlib.h>
#include <string.h>
#endif

#ifdef OS_VMS
# include <lib$routines.h>
#endif


#include "co_cdh.h"

#include "rt_errh.h"
#include "rt_qcom.h"
#include "rt_qdb.h"
#include "rt_qcom_msg.h"
#include "rt_qdb_msg.h"

static void
exitHandler();

static void
sigHandler();

static qdb_sBuffer *	inPool (pwr_tStatus*, void*);


static void
exitHandler()
{
  qcom_Exit(NULL);
}

#if defined OS_LINUX || defined OS_LYNX || defined OS_MACOS || defined OS_FREEBSD

static void
sigHandler()
{
  exit(0);
}
#endif


static qdb_sBuffer *
inPool (
  pwr_tStatus	*sts,
  void		*p
)
{
  pool_tRef	br;
  qdb_sBuffer	*bp;

  qdb_AssumeLocked;
  
  bp = (qdb_sBuffer *)p - 1;
  if (bp->b.cookie != qdb_cCookie) return NULL;

  br = pool_InPool(sts, &qdb->pool, bp, bp->b.size + sizeof(*bp));
  if (br == pool_cNRef) return NULL;

  return bp;
}

/**
 * @brief Allocate a buffer.
 */
void *
qcom_Alloc (
  pwr_tStatus	*status,
  unsigned int	size
)
{
  qdb_sBuffer   *bp = NULL;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_ScopeLock {

#if 0
    if (qdb->ap->alloc_count >= qdb->ap->alloc_quota) {
      *sts = QCOM__ALLOCQUOTA;
      break;
    }
#endif
    bp = qdb_Alloc(sts, qdb_eBuffer_base, size);
    if (bp == NULL) break;

    qdb->g->call_count++;
    qdb->ap->call_count++;
    qdb->ap->alloc_count++;

  } qdb_ScopeUnlock;

  if (bp == NULL)
    return NULL;

  return (void *)(bp + 1);
}

/**
 *@brief  Attach to an allready created queue.  
 */
pwr_tBoolean
qcom_AttachQ (
  pwr_tStatus		*status,
  const qcom_sQid	*qid
)
{
  qdb_sQue		*qp;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  if (qdb->ap == NULL) pwr_Return(NO, sts, QCOM__NOTINITED);

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    qp = qdb_Que(sts, qid, NULL);
    if (qp == NULL) pwr_StatusBreak(*sts, QCOM__NOQ);
                      
    if (qcom_AidIsNotEqual(&qp->aid, &qcom_cNAid)) pwr_StatusBreak(*sts, QCOM__ALLRATTACHED);
    if (qp->type != qdb_eQue_private) pwr_StatusBreak(*sts, QCOM__NOTATTACHABLE);

    qdb_AttachQue(sts, qp, qdb->ap);

  } qdb_ScopeUnlock;

  return ODD(*sts);
}

/**
 * @brief  Bind one queue to another.
 *
 * A private queue can be bound to one or many forward
 * queues.  
 */
pwr_tBoolean
qcom_Bind (
  pwr_tStatus		*status,
  const qcom_sQid	*tqid, 
  const qcom_sQid	*sqid
)
{
  qdb_sQbond		*bp = NULL;
  qdb_sQue		*sq;
  qdb_sQue		*tq;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  qdb_ScopeLock {

    if (qdb->ap == NULL) pwr_StatusBreak(*sts, QCOM__NOTINITED);
    qdb->ap->call_count++;
    qdb->g->call_count++;

    sq = qdb_Que(sts, sqid, NULL);
    if (sq == NULL) pwr_StatusBreak(*sts, QCOM__NOQ);

    if (sq->type != qdb_eQue_forward) pwr_StatusBreak(*sts, QCOM__NOTBINDABLE);

    tq = qdb_Que(sts, tqid, NULL);
    if (tq == NULL) pwr_StatusBreak(*sts, QCOM__NOQ);

    if (tq->type != qdb_eQue_private) pwr_StatusBreak(*sts, QCOM__NOTBINDABLE);

    if (qdb_GetBond(sts, sq, tq) != NULL) pwr_StatusBreak(*sts, QCOM__ALRBOUND);

    bp = qdb_AddBond(sts, sq, tq);

  } qdb_ScopeUnlock;

  return bp != NULL;
}

/**
 * @brief Create a queue and make an implicit connect to it. 
 */
pwr_tBoolean
qcom_CreateQ (
  pwr_tStatus	*status,
  qcom_sQid	*qid,
  qcom_sQattr	*attr,
  const char	*qname
)
{
  qdb_sQue	*qp;
  qdb_eQue	type;
  qdb_mQue	flags;
  qcom_sQid	lqid = qcom_cNQid;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  if (qdb->ap == NULL) pwr_Return(NO, sts, QCOM__NOTINITED);

  if (qid == NULL)
    qid = &lqid;

  flags.m = 0;

  if (attr != NULL) {
    switch (attr->type) {
    case qcom_eQtype_private:
      type = qdb_eQue_private;
      break;
    case qcom_eQtype_forward:
      type = qdb_eQue_forward;
      break;
    case qcom_eQtype_broadcast:
      type = qdb_eQue_forward;
      flags.b.broadcast = 1;
      break;
    case qcom_eQtype_event:
      type = qdb_eQue_forward;
      flags.b.event = 1;
      break;
    default:
      pwr_Return(NO, sts, QCOM__QTYPE);
      break;
    }
  } else {
    type = qdb_eQue_private;
  }

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    qp = qdb_AddQue(sts, qid->qix);
    if (qp == NULL) break;
    qid->qix = qp->qix;
    qid->nid = qdb->my_nid;
    qp->type = type;
    qp->flags.m = flags.m;
    if ( attr)
      qp->in_quota = attr->quota;
    qp = qdb_AttachQue(sts, qp, qdb->ap);
    if (qp == NULL) break;
    if (flags.b.broadcast) {
      qdb_AddBond(sts, qp, qdb->exportque);
    }

    if (qname != NULL) {
      strncpy(qp->name, qname, sizeof(qp->name) - 1);
      qp->name[sizeof(qp->name) - 1] = '\0';
    } else
      strcpy(qp->name, "\"unknown name\"");

  } qdb_ScopeUnlock;

  return qp != NULL;
}

pwr_tBoolean
qcom_DeleteQ (
  pwr_tStatus		*status,
  const qcom_sQid	*qid
)
{
  qdb_sQue		*qp;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  if (qdb->ap == NULL) pwr_Return(NO, sts, QCOM__NOTINITED);

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    qp = qdb_Que(sts, qid, NULL);
    if (qp == NULL) pwr_StatusBreak(*sts, QCOM__NOQ);

    qdb_RemoveQue(sts, qp);

  } qdb_ScopeUnlock;
  
  return ODD(*sts);
}

pwr_tBoolean
qcom_Exit (
  pwr_tStatus	*status
)
{
  qdb_sAppl	*ap = qdb->ap;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  if (ap == NULL)
    return YES;

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    qdb_RemoveAppl(sts, ap);
    qdb->ap = NULL;

  } qdb_ScopeUnlock;

  return YES;
}

pwr_tBoolean
qcom_Free (
  pwr_tStatus	*status,
  void		*p
)
{
  qdb_sBuffer	*bp;
  pool_tRef	br;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  if (p == NULL) pwr_Return(NO, sts, QCOM__NULLBUFF);
  if (qdb->ap == NULL) pwr_Return(NO, sts, QCOM__NOTINITED);
  
  bp = (qdb_sBuffer *)p - 1;
  if (bp->c.type != qdb_eBuffer_base)
    pwr_Return(NO, sts, QCOM__NOTALLOC);
  if (bp->b.cookie != qdb_cCookie)
    pwr_Return(NO, sts, QCOM__NOTALLOC);

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    br = pool_InPool(sts, &qdb->pool, bp, bp->b.size + sizeof(*bp));
    if (br == pool_cNRef) break;
    qdb_Free(sts, bp);
    qdb->ap->free_count++;

  } qdb_ScopeUnlock;

  return (br != pool_cNRef) && ODD(*sts);
}

/**
 *@brief  Get a new message.
 *
 * If mp == NULL => pass the address to the buffer in the pool.
 * If mp != NULL => copy the contents to mp and mp->data
 *	    free the pool buffer.
 * You may only read from a non bound, non forwarded, owned qeueue.
 * @return void*
 */

void *
qcom_Get (
  pwr_tStatus		*status,
  const qcom_sQid	*qid,
  qcom_sGet		*gp,
  int			tmo
)
{
  pwr_tStatus		lsts;
  qdb_sQue		*qp = NULL;
  qdb_sBuffer		*bp = NULL;
  qdb_sBuffer		*bbp = NULL;
  pool_tRef		br;
  void			*dp = NULL;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  if (qdb->ap == NULL) pwr_Return(NULL, sts, QCOM__NOTINITED);

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    /* Check that a given gp or gp->data is not in pool,
       this could cause serious problems.  */

    if (gp != NULL && gp->data != NULL) {
      br = pool_InPool(&lsts, &qdb->pool, gp->data, gp->size);
      if (br != pool_cNRef) return NULL;
    }

    qp = qdb_Que(sts, qid, NULL);
    if (qp == NULL) break;

    if (qp->aid.aix != qdb->my_aix) pwr_StatusBreak(*sts, QCOM__NOTOWNED); 
    if (qp->type != qdb_eQue_private) pwr_StatusBreak(*sts, QCOM__NOTOWNED);

    bp = qdb_Get(sts, qp, tmo, gp, 0);

    if (bp != NULL) {
      qdb->ap->get_count++;
      qp->get_count++;
    }
  } qdb_ScopeUnlock;

  if (bp == NULL) return NULL;

  dp = bp + 1;

  if (gp == NULL) {
    ; /* do nothing! */
  } else if (gp->data == NULL) {
    gp->data = dp;
  } else {
    if (bp->c.type == qdb_eBuffer_reference)
      bbp = pool_Address(sts, &qdb->pool, bp->r.src);
    else
      bbp = bp;

    dp = qdb_CopyBufferData(sts, bbp, gp);

    qdb_ScopeLock {
      if (bp->c.type == qdb_eBuffer_reference)
	qdb_Free(&lsts, bp);
      qdb_Free(&lsts, bbp);
      qdb->ap->free_count++;
    } qdb_ScopeUnlock;
  }

  return dp;
}

qcom_tBus
qcom_MyBus (
  pwr_tStatus	*status
)
{
  qcom_tBus bus;

  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  qdb_ScopeLock {

    bus = qdb->g->bus;

  } qdb_ScopeUnlock;

  return bus;
}

pwr_tBoolean
qcom_MyNode (
  pwr_tStatus	*status,
  qcom_sNode	*node
)
{
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  if (node == NULL)
    pwr_Return(NO, sts, QCOM__BADARG);

  qdb_ScopeLock {

    qdb_NodeInfo(NULL, node, qdb->my_node);

  } qdb_ScopeUnlock;

  return YES;
}

pwr_tBoolean
qcom_NextNode (
  pwr_tStatus	*status,
  qcom_sNode	*node,
  pwr_tNodeId	nid
)
{
  pool_sQlink	*nl = NULL;
  qdb_sNode	*np = NULL;
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  if (node == NULL)
    pwr_Return(NO, sts, QCOM__BADARG);

  qdb_ScopeLock {

    if (nid == pwr_cNNodeId) {
      nl = pool_Qsucc(NULL, &qdb->pool, &qdb->g->node_lh);
    } else {
      np = hash_Search(sts, &qdb->nid_ht, &nid);
      if (np == NULL) {
	*sts = QCOM__NOSUCHNODE;
      } else {
	nl = &np->node_ll;
      }
    }

    if (nl == NULL) break;

    while (TRUE) {
      /* loop over no node */
      np = NULL;
      nl = pool_Qsucc(NULL, &qdb->pool, nl);
      if (nl == &qdb->g->node_lh) {
	*sts = QCOM__NO_NODE;
	break;
      } else {
	np = pool_Qitem(nl, qdb_sNode, node_ll);
	if (np->nid == pwr_cNNodeId) {
	  continue;
	} else {
	  break;
	}
      }    
    }

    if (np == NULL) break;

    qdb_NodeInfo(NULL, node, np);

  } qdb_ScopeUnlock;

  return (np != NULL);
}

pwr_tBoolean
qcom_Init (
  pwr_tStatus	*status,
  qcom_sAid	*aid,
  const char	*aname
)
{
  qdb_sAppl	*ap = NULL;
  pwr_tStatus	lsts;
  qcom_sAid	laid;
  pwr_tBoolean	added = 0;
#ifdef OS_ELN
  FUNCTION_DESCRIPTOR   f_dsc;
#elif defined OS_LINUX || defined OS_LYNX || defined OS_MACOS || defined OS_FREEBSD
  struct sigaction sa;
  sigset_t         ss;
#endif
  pwr_dStatus	(sts, status, QCOM__SUCCESS);

  if (aid == NULL) {
    memset(&laid, 0, sizeof(laid));
    aid = &laid;
  }

  qdb = qdb_MapDb(sts);
  if (qdb == NULL || (EVEN(*sts) && *sts != QDB__ALRMAP)) return NO;

  qdb_ScopeLock {

    qdb->g->call_count++;

    if (qdb->ap == NULL) {
      ap = hash_Search(sts, &qdb->pid_ht, &qdb->my_pid);
      if (ap != NULL) {
	qdb->ap = ap;
	qdb_RemoveAppl(&lsts, ap);
	qdb->ap = NULL;
      }

      ap = qdb_AddAppl(sts, NO);
      if (ap == NULL) break;

      added = 1;

      ap->call_count = 1;
      if (aname != NULL) {
        strncpy(ap->name, aname, sizeof(ap->name) - 1);
        ap->name[sizeof(ap->name) - 1] = '\0';
      } else {
        strcpy(ap->name, "\"unknown name\"");
      }
    } else {
      ap = qdb->ap;
    }

  } qdb_ScopeUnlock;

  if (ap == NULL) return NO;

  qdb->ap = ap;

  if (!added)
    return YES;

  *aid = ap->aid;

#if defined OS_ELN
  if (ODD(*sts))
    errh_Info("Adding application. aix: %d, pid: %d", ap->aid.aix, ap->pid);
#else
    errh_Info("Adding application. aix: %d", ap->aid.aix);
#endif
#if defined OS_ELN
  f_dsc.function = exitHandler;
  f_dsc.display  = 0;

  eln$declare_exit_handler(&f_dsc, NULL);

#elif defined OS_LINUX || defined OS_LYNX || defined OS_MACOS || defined OS_FREEBSD
  atexit(exitHandler);
  sa.sa_handler = sigHandler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGHUP, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigemptyset(&ss);
  sigaddset(&ss, qdb_cSigMsg);
  sigprocmask(SIG_BLOCK, &ss, NULL);

#elif defined OS_VMS
  atexit(exitHandler);
  *sts = lib$get_ef(&qdb->ap->ef);
  if (EVEN(*sts)) errh_Bugcheck(sts, "lib$get_ef()");
#endif

  qdb_ScopeLock {
    qdb_ApplEvent(NULL, ap, qcom_eStype_applConnect);
  } qdb_ScopeUnlock;

  return YES;
}

pwr_tBoolean
qcom_Put (
  pwr_tStatus		*status,
  const qcom_sQid	*qidp,
  qcom_sPut		*pp
)
{
  qdb_sBuffer		*bp;
  qdb_sNode		*np = NULL;
  qdb_sQue		*qp = NULL;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  pwr_Assert(pp != NULL);

  if (qdb->ap == NULL) pwr_Return(NO, sts, QCOM__NOTINITED);

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    qp = qdb_Que(sts, qidp, &np);
    if (qp == NULL) break;

    bp = inPool(sts, pp->data);
    if (bp == NULL) {
      bp = qdb_Alloc(sts, qdb_eBuffer_base, pp->size);
      memcpy((char *)(bp + 1), pp->data, pp->size);
    } else {
      /* check that this buffer is really owned by this process */
    }

    qdb_PutInfo(bp, pp, qidp, 0);

    if (qp->qix == qdb_cIexport) {
      bp->c.flags.b.remote = 1;
      bp->b.noderef = pool_Reference(sts, &qdb->pool, np);
      bp->b.msg_id = pp->msg_id;
    }

    qdb_Put(sts, bp, qp);
    if (ODD(*sts)) qdb->ap->put_count++;

  } qdb_ScopeUnlock;

  return ODD(*sts);
}
 
/**
 * @brief Disconnect a target queue from a bound source queue.
 *
 * Buffers allready queued in the bound queue from the
 * forwarding queue will not be unqueued.
 */
pwr_tBoolean
qcom_Unbind (
  pwr_tStatus		*status,
  const qcom_sQid	*sqid,
  const qcom_sQid	*tqid
)
{
  qdb_sQue		*sq;
  qdb_sQue		*tq;
  qdb_sQbond		*bp;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  if (qdb->ap == NULL) pwr_Return(NO, sts, QCOM__NOTINITED);

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    /* Make sure queues exists.  */

    sq = qdb_Que(sts, sqid, NULL);
    if (sq == NULL) pwr_StatusBreak(*sts, QCOM__NOQ);

    tq = qdb_Que(sts, tqid, NULL);
    if (tq == NULL) pwr_StatusBreak(*sts, QCOM__NOQ);

    if ((bp = qdb_GetBond(sts, sq, tq)) == NULL)
      pwr_StatusBreak(*sts, QCOM__NOTBOUND);

    pool_Qremove(sts, &qdb->pool, &bp->tgt_ll);
    pool_Qremove(sts, &qdb->pool, &bp->src_ll);
    pool_Free(sts, &qdb->pool, bp);
    
  } qdb_ScopeUnlock;

  return ODD(*sts);
}

/**
 * @brief Compare two queue identities;
 *
 * return <0 if qid_1 <  qid_2
 * return  0 if qid_1 == qid_2
 * return >0 if qid_1 <  qid_2.  
 * @return int
 */

int
qcom_QidCompare (
  const qcom_sQid  *q1,
  const qcom_sQid  *q2
)
{

  if (q1->nid == q2->nid) {
    if (q1->qix == q2->qix)
      return 0;
    else if (q1->qix < q2->qix)
      return -1;
    else
      return 1;
  } else if (q1->nid < q2->nid)
    return -1;
  else
    return 1;

}


/** 
 * @brief Test if two queue identities are equal.
 *
 * return true  if qid_1 == qid_2
 * return false if qid_1 != qid_2.  
 * @return pwr_tBoolean
 */
pwr_tBoolean
qcom_QidIsEqual (
  const qcom_sQid  *q1,
  const qcom_sQid  *q2
)
{

  return (q1->nid == q2->nid) && (q1->qix == q2->qix);
}

/** 
 * @brief Test if two queue identities are different.
 *
 * return true  if qid_1 != qid_2
 * return false if qid_1 == qid_2.
 * @return pwr_tBoolean
 */
pwr_tBoolean
qcom_QidIsNotEqual (
  const qcom_sQid  *q1,
  const qcom_sQid  *q2
)
{

  return (q1->nid != q2->nid) || (q1->qix != q2->qix);
}


/**
 * @brief Test if queue identity is null.
 *
 * return true  if qid_1 == qcom_cNQid
 * return false if qid_1 != qcom_cNQid.  
 * @return pwr_tBoolean
 */

pwr_tBoolean
qcom_QidIsNull (
  const qcom_sQid  *q
)
{
  return (q->nid == qcom_cNQid.nid) && (q->qix == qcom_cNQid.qix);
}


/**
 * @brief Test if queue identity is not null.
 *
 * return true  if qid_1 != qcom_cNQid
 * return false if qid_1 == qcom_cNQid.  
 * @return pwr_tBoolean
 */

pwr_tBoolean
qcom_QidIsNotNull (
  const qcom_sQid  *q
)
{
  return (q->nid != qcom_cNQid.nid) || (q->qix != qcom_cNQid.qix);
}



/**
 * @brief Compare two application identities;
 *
 * return <0 if aid_1 <  aid_2
 * return  0 if aid_1 == aid_2
 * return >0 if aid_1 >  aid_2.  
 * @return int
 */

int
qcom_AidCompare (
  const qcom_sAid  *a1,
  const qcom_sAid  *a2
)
{

  if (a1->nid == a2->nid) {
    if (a1->aix == a2->aix)
      return 0;
    else if (a1->aix < a2->aix)
      return -1;
    else
      return 1;
  } else if (a1->nid < a2->nid)
    return -1;
  else
    return 1;

}


/**
 *@brief Test if two application identities are equal.
 *
 * return true  if aid_1 == aid_2
 * return false if aid_1 != aid_2.  
 * @return pwr_tBoolean
 */
 
pwr_tBoolean
qcom_AidIsEqual (
  const qcom_sAid  *a1,
  const qcom_sAid  *a2
)
{

  return (a1->nid == a2->nid) && (a1->aix == a2->aix);
}

/**
 * @brief Test if two application identities are different.
 *
 * return true  if aid_1 != aid_2
 * return false if aid_1 == aid_2.  
 * @return pwr_tBoolean
 */

pwr_tBoolean
qcom_AidIsNotEqual (
  const qcom_sAid  *a1,
  const qcom_sAid  *a2
)
{

  return (a1->nid != a2->nid) || (a1->aix != a2->aix);
}


/** 
 * @brief Test if application identity is null.
 *
 * return true  if aid == qcom_cNAid
 * return false if aid != qcom_cNAid.  
 * @return pwr_tBoolean
 */

pwr_tBoolean
qcom_AidIsNull (
  const qcom_sAid  *a
)
{
  return (a->nid == qcom_cNAid.nid) && (a->aix == qcom_cNAid.aix);
}


/**
 * @brief Test if application identity is not null.
 *
 * return true  if aid != qcom_cNAid
 * return false if aid == qcom_cNAid.  
 * @return pwr_tBoolean
 */ 
pwr_tBoolean
qcom_AidIsNotNull (
  const qcom_sAid  *a
)
{
  return (a->nid != qcom_cNAid.nid) || (a->aix != qcom_cNAid.aix);
}

/*
    pwr_tBoolean		qcom_Put (
      pwr_tStatus		*sts,
      const qcom_sQid	*receiver,
      qcom_sPut		*put
    );
    void*			qcom_Get (
      pwr_tStatus		*sts,
      const qcom_sQid	*myQ,
      qcom_sGet		*get,
      int			tmo_ms
    );
*/

void*
qcom_Request (
  pwr_tStatus		*status,
  const qcom_sQid	*pqid,
  qcom_sPut		*pp,
  const qcom_sQid	*gqid,
  qcom_sGet		*gp,
  int			tmo,
  pwr_tBitMask          flags
)
{
  qdb_sBuffer		*pbp;
  qdb_sNode		*np = NULL;
  qdb_sQue		*pqp = NULL;
  pwr_tStatus		lsts;
  qdb_sQue		*gqp = NULL;
  qdb_sBuffer		*gbp = NULL;
  qdb_sBuffer		*gbbp = NULL;
  pool_tRef		gbr;
  void			*dp = NULL;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  pwr_Assert(pp != NULL);

  if (tmo < 0) {
    pwr_Return(NULL, sts, QCOM__HIGHTMO);
  }

  if (qdb->ap == NULL) pwr_Return(NULL, sts, QCOM__NOTINITED);

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    pqp = qdb_Que(sts, pqid, &np);
    if (pqp == NULL) break;

    /* Check that a given gp or gp->data is not in pool,
       this could cause serious problems.  */

    if (gp != NULL && gp->data != NULL) {
      gbr = pool_InPool(&lsts, &qdb->pool, gp->data, gp->size);
      if (gbr != pool_cNRef) return NULL;
    }

    gqp = qdb_Que(sts, gqid, NULL);
    if (gqp == NULL) break;

    if (gqp->aid.aix != qdb->my_aix) pwr_StatusBreak(*sts, QCOM__NOTOWNED); 
    if (gqp->type != qdb_eQue_private) pwr_StatusBreak(*sts, QCOM__NOTOWNED);

    pbp = inPool(sts, pp->data);
    if (pbp == NULL) {
      pbp = qdb_Alloc(sts, qdb_eBuffer_base, pp->size);
      memcpy((char *)(pbp + 1), pp->data, pp->size);
    } else {
      /* check that this buffer is really owned by this process */
    }

    pbp->c.flags.b.request = 1;
    qdb_PutInfo(pbp, pp, pqid, 0);

    if (pqp->qix == qdb_cIexport) {
      pbp->c.flags.b.remote = 1;
      pbp->b.noderef = pool_Reference(sts, &qdb->pool, np);
    }

    gbp = qdb_Request(sts, pbp, pqp, gqp, tmo, gp, flags);
    if (ODD(*sts)) qdb->ap->request_count++;

  } qdb_ScopeUnlock;

  if (gbp == NULL) return NULL;

  dp = gbp + 1;

  if (gp == NULL) {
    ; /* do nothing! */
  } else if (gp->data == NULL) {
    gp->data = dp;
  } else {
    if (gbp->c.type == qdb_eBuffer_reference)
      gbbp = pool_Address(sts, &qdb->pool, gbp->r.src);
    else
      gbbp = gbp;

    dp = qdb_CopyBufferData(sts, gbbp, gp);

    qdb_ScopeLock {
      if (gbp->c.type == qdb_eBuffer_reference)
	qdb_Free(&lsts, gbp);
      qdb_Free(&lsts, gbbp);
      qdb->ap->free_count++;
    } qdb_ScopeUnlock;
  }

  return dp;
}

pwr_tBoolean
qcom_Reply (
  pwr_tStatus		*status,
  qcom_sGet		*gp,
  qcom_sPut		*pp
)
{
  qdb_sBuffer		*bp;
  qdb_sNode		*np = NULL;
  qdb_sQue		*qp = NULL;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  pwr_Assert(gp != NULL);
  pwr_Assert(pp != NULL);

  if (qdb->ap == NULL) pwr_Return(NO, sts, QCOM__NOTINITED);

  qdb_ScopeLock {

    qdb->ap->call_count++;
    qdb->g->call_count++;

    qp = qdb_Que(sts, &gp->reply, &np);
    if (qp == NULL) break;

    bp = inPool(sts, pp->data);
    if (bp == NULL) {
      bp = qdb_Alloc(sts, qdb_eBuffer_base, pp->size);
      memcpy((char *)(bp + 1), pp->data, pp->size);
    } else {
      /* check that this buffer is really owned by this process */
    }

    qdb_PutInfo(bp, pp, &gp->reply, 0);

    bp->c.flags.b.reply = 1;
    bp->c.flags.b.request = 0;
    bp->b.info.rid = gp->rid;
    bp->b.info.flags.b.reply = 1;
    bp->b.info.flags.b.request = 0;
    bp->b.info.status = QCOM__SUCCESS;

    if (qp->qix == qdb_cIexport) {
      bp->c.flags.b.remote = 1;
      bp->b.noderef = pool_Reference(sts, &qdb->pool, np);
    }

    qdb_Put(sts, bp, qp);

    qdb->ap->reply_count++;

  } qdb_ScopeUnlock;

  return ODD(*sts);
}

/**
 * @brief  Converts a queue identifier, 'qid' to a string.
 *   
 * The output string will be in the format:
 *
 *  0.1.2.3:1234
 *
 *  If 's' is non null the resultant string will be catenated
 *  to 's', otherwise the resultant string will be returned.
 *
 *  If 'prefix' is not zero, a '_Q' prefix will be included in
 *  the resultant string.  
 * @return char*
 */

char *
qcom_QidToString (
  char			*s,
  qcom_sQid		*qid,
  int			prefix
)
{
  cdh_uObjid		loid;
  static char		ls[sizeof("_Q255.255.255.255:4294967295")];

  loid.pwr.vid = (pwr_tVolumeId)qid->nid;
  loid.pwr.oix = (pwr_tObjectIx)qid->qix;

  sprintf(ls, "%s%u.%u.%u.%u:%u", (prefix ? "_Q" : ""), loid.o.vid_3,
    loid.o.vid_2, loid.o.vid_1, loid.o.vid_0, loid.o.oix);

  if (s != NULL)
    return strcat(s, ls);
  else
    return ls;
  
}

pwr_tBoolean
qcom_WaitOr (
  pwr_tStatus		*status,
  const qcom_sQid	*qid,
  const qcom_sQid	*eid,
  int			mask,
  int			tmo,
  int		       	*event
)
{
  int			result = 0;
  qdb_sQue		*qp;
  qdb_sQue		*ep;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  qdb_ScopeLock {

    qp = qdb_Que(sts, qid, NULL);
    if (qp == NULL) break;
    ep = qdb_Que(sts, eid, NULL);
    if (ep == NULL) break;

    if (qp->aid.aix != qdb->my_aix) pwr_StatusBreak(*sts, QCOM__NOTOWNED); 
    if (qp->type != qdb_eQue_private) pwr_StatusBreak(*sts, QCOM__NOTOWNED);

    result = ep->mask & mask;
    if (result) break;
      
    qp->or_event = 1;
    qp->mask = mask;
    result = qdb_Wait(sts, qp, ep, tmo);
    *event = ep->mask & mask;
  } qdb_ScopeUnlock;

  return result != 0;
}

pwr_tBoolean
qcom_WaitAnd (
  pwr_tStatus		*status,
  const qcom_sQid	*qid,
  const qcom_sQid	*eid,
  int			mask,
  int			tmo
)
{
  int			result = 0;
  qdb_sQue		*qp;
  qdb_sQue		*ep;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  qdb_ScopeLock {

    qp = qdb_Que(sts, qid, NULL);
    if (qp == NULL) break;
    ep = qdb_Que(sts, eid, NULL);
    if (ep == NULL) break;

    if (qp->aid.aix != qdb->my_aix) pwr_StatusBreak(*sts, QCOM__NOTOWNED); 
    if (qp->type != qdb_eQue_private) pwr_StatusBreak(*sts, QCOM__NOTOWNED);

    result = (ep->mask & mask) == mask;
    if (result) break;
      
    qp->or_event = 0;
    qp->mask = mask;
    result = qdb_Wait(sts, qp, ep, tmo);

  } qdb_ScopeUnlock;

  return result != 0;
}

pwr_tBoolean
qcom_SignalAnd (
  pwr_tStatus		*status,
  const qcom_sQid	*eid,
  int			mask
)
{
  qdb_sQue		*ep;

  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  qdb_ScopeLock {

    ep = qdb_Que(sts, eid, NULL);
    if (ep == NULL) break;

    ep->mask &= mask;

    qdb_Eput(sts, ep);
    qdb_Signal(sts, ep);

  } qdb_ScopeUnlock;

  return ODD(*sts);
}

pwr_tBoolean
qcom_SignalOr (
  pwr_tStatus		*status,
  const qcom_sQid	*eid,
  int			mask
)
{
  qdb_sQue		*ep;

  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  qdb_ScopeLock {

    ep = qdb_Que(sts, eid, NULL);
    if (ep == NULL) break;

    ep->mask |= mask;

    qdb_Eput(sts, ep);
    qdb_Signal(sts, ep);

  } qdb_ScopeUnlock;

  return ODD(*sts);
}

pwr_tBitMask
qcom_EventMask (
  pwr_tStatus		*status,
  const qcom_sQid	*eid
)
{
  int			result = 0;
  qdb_sQue		*ep;
  pwr_dStatus		(sts, status, QCOM__SUCCESS);

  qdb_ScopeLock {

    ep = qdb_Que(sts, eid, NULL);
    if (ep == NULL) break;

    result = ep->mask;
      
  } qdb_ScopeUnlock;

  return result;
}



/*
* XDR Routines
*/

bool_t 
xdr_qcom_sAid(XDR *xdrs, qcom_sAid *objp)
{
  if (!xdr_int(xdrs, &objp->aix)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->nid)) return FALSE;

  return TRUE;
}

bool_t 
xdr_qcom_sQid(XDR *xdrs, qcom_sQid *objp)
{
  if (!xdr_int(xdrs, &objp->qix)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->nid)) return FALSE;

  return TRUE;
}


