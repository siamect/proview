/* rt_neth_acp.c -- helps net handler.

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   The nethandler submitts work that cannot be done
   in the context of the neth to this program. Normally
   transactions that causes cache fetch operations.  */

#if defined(OS_ELN)
# include $vaxelnc
#else
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <string.h>
#endif

#include "pwr.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_gdh_msg.h"
#include "rt_errh.h"
#include "rt_gdb.h"
#include "rt_gdh.h"
#include "rt_vol.h"
#include "rt_cvolc.h"
#include "rt_sanc.h"
#include "rt_subc.h"
#include "rt_net.h"


static void init(qcom_sQid *qid);
static void lockMountServers(gdb_sNode *np);
static gdb_sNode *node(net_sMessage *mp);
static void nodeUp(net_sNodeUp *mp);
static void *receive(qcom_sQid *qid, qcom_sGet *get);


int
main (
  int			argc,
  char			**argv
)
{
  pwr_tStatus		sts;
  qcom_sGet		get;
  net_sNotify		*notify;
  net_sMessage		*mp;
  gdb_sNode		*np;
  qcom_sQid		qid = qcom_cNQid;

  errh_Init("pwr_nacp");

  init(&qid);

  for (;;) {
    mp = receive(&qid, &get);

    np = node(mp);
    if (np == NULL) {
      errh_Error("sender %u.%u, type %u.%u, size %u, reply %u.%u",
	get.sender.nid, get.sender.aix, get.type.b, get.type.s, get.size,
	get.reply.nid, get.reply.qix);
      continue;
    }

    switch (get.type.s) {
    case net_eMsg_nodeUp:
      nodeUp((net_sNodeUp *) mp);
      break;
    case net_eMsg_createObject:
      gdb_ScopeLock {
	subc_ActivateList(&gdbroot->no_node->subc_lh, pwr_cNObjid);
      } gdb_ScopeUnlock;
      break;

    case net_eMsg_deleteObject:
      notify = (net_sNotify *) mp;
      gdb_ScopeLock {
	subc_ActivateList(&np->subc_lh, notify->oid);
      } gdb_ScopeUnlock;
      break;

    case net_eMsg_moveObject:
    case net_eMsg_renameObject:
      gdb_ScopeLock {
	subc_ActivateList(&np->subc_lh, pwr_cNObjid);
	subc_ActivateList(&gdbroot->no_node->subc_lh, pwr_cNObjid);
      } gdb_ScopeUnlock;
      break;

    default:
      errh_Error("Unexpected subtype %d received from %d.%d %X",
	get.type.s, get.reply.qix, get.reply.nid, get.pid);
    }


    qcom_Free(&sts, mp);
  }
}

/* Initiate qcom, gdh and subscriptions.  */

static void
init (
  qcom_sQid *myQid
)
{
  pwr_tStatus sts;
  
  sts = gdh_Init("pwr_nacp");
  if (EVEN(sts)) {
    errh_Fatal("gdh_Init, %m", sts);
    exit(sts);
  } 

  if (!qcom_AttachQ(&sts, &qcom_cQnacp)) {
    errh_Fatal("qcom_AttachQ, %m", sts);
    exit(sts);
  } 
  *myQid = qcom_cQnacp;

  /* Activate all subscriptions that were requested before we started!  */

  gdb_ScopeLock {

    gdbroot->db->neth_acp = *myQid;
    subc_ActivateList(&gdbroot->my_node->subc_lh, pwr_cNObjid);
    subc_ActivateList(&gdbroot->no_node->subc_lh, pwr_cNObjid);

  } gdb_ScopeUnlock;
}

/* Receive a message.  */

static void *
receive (
  qcom_sQid		*qid,
  qcom_sGet		*get
)
{
  pwr_tStatus		sts;
  void			*mp;


  for (;;) {
    get->data = NULL;
    /* Loop until receiving a valid message.  */
    mp = qcom_Get(&sts, &qcom_cQnacp, get, qcom_cTmoEternal);
    if (mp == NULL) {
      errh_Error("qcom_Get, %m", sts);
      continue;
    }

    if (get->type.b != net_cMsgClass) {
      errh_Error("Received bad message, type: <%d>, subtype: <%d>", get->type.b, get->type.s);
      qcom_Free(&sts, mp);
      continue;
    }

    return mp;
  }
}

/* Check message node identity and return pointer to node.  */

static gdb_sNode *
node (
  net_sMessage		*mp
)
{
  pwr_tStatus		sts;
  gdb_sNode		*np;

  if (mp->nid == pwr_cNNodeId) {
    errh_Error("Message from pwr_cNNodeId received");
    return NULL;
  }

  gdb_ScopeLock {
    np = hash_Search(&sts, gdbroot->nid_ht, &mp->nid);
  } gdb_ScopeUnlock;

  if (np == NULL) {
    errh_Error("Message about unknown node: (%s) received\n",
      cdh_NodeIdToString(NULL, mp->nid, 0, 0));
    return NULL;
  }
  
  return np;
}

/* A node is up.
   
   Get object headers for all mount servers
   and lock them in the cache cache.
   
   Reactivate subscriptions.  */

static void
nodeUp (
  net_sNodeUp		*mp
)
{
  pwr_tStatus		sts;
  gdb_sNode		*np;
  
  np = hash_Search(&sts, gdbroot->nid_ht, &mp->nid);
  if (np == NULL) {
    errh_Error("Message about unknown node: (%s) received\n",
      cdh_NodeIdToString(NULL, mp->nid, 0, 0));
    return;
  }
  errh_Info("Up, link to node %s (%s)", 
    np->name, cdh_NodeIdToString(NULL, mp->nid, 0, 0));

  gdb_ScopeLock {
    lockMountServers(np);
    subc_ActivateList(&np->subc_lh, pwr_cNObjid);
    subc_ActivateList(&gdbroot->no_node->subc_lh, pwr_cNObjid);
    sanc_SubscribeMountServers(&sts, np);
  } gdb_ScopeUnlock;
}

/* Get the object header of all mount servers and lock them in cache.  */

static void
lockMountServers (
  gdb_sNode		*np
)
{
  pwr_tStatus		sts;
  pool_sQlink		*msl;
  pool_sQlink		*vl;
  gdb_sVolume		*vp;
  gdb_sMountServer	*msp;
  gdb_sObject		*op;

  gdb_AssumeLocked;

  for (
    vl = pool_Qsucc(NULL, gdbroot->pool, &np->own_lh);
    vl != &np->own_lh;
    vl = pool_Qsucc(NULL, gdbroot->pool, vl)
  ) {
    vp = pool_Qitem(vl, gdb_sVolume, l.own_ll);
    if(!vp->l.flags.b.isConnected) {
      /* !!! Todo !!! How do we make this known ?  */
      errh_Error("Volume not connected, %s", vp->g.name);
      continue;
    }

    for (
      msl = pool_Qsucc(NULL, gdbroot->pool, &vp->l.volms_lh);
      msl != &vp->l.volms_lh;
      msl = pool_Qsucc(NULL, gdbroot->pool, msl)
    ) {
      msp = pool_Qitem(msl, gdb_sMountServer, volms_ll);      
      op = hash_Search(&sts, gdbroot->oid_ht, &msp->oid);
      if (op == NULL) {
        op = cvolc_OidToObject(&sts, vp, msp->oid, vol_mTrans_none, cvol_eHint_none);
        if (op == NULL) {
          errh_Error("Can't fetch the mount server's object, %s", cdh_ObjidToString(NULL, msp->oid, 1));
	  /* !!! Todo !!! How do we make this error known ?  */
  	continue;
        }
      }
      msp->msor = pool_ItemReference(NULL, gdbroot->pool, op);
      op->l.flags.b.isMountServer = 1;
      pool_QinsertPred(NULL, gdbroot->pool, &msp->nodms_ll, &np->nodms_lh);
      
      if (0) errh_Info("Locking object %s", op->g.f.name.orig);
      cvolc_LockObject(&sts, op);

    }
  }
}
