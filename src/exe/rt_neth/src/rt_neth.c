/* rt_neth.c -- net handler

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   <Description>.  */

#if defined OS_ELN
# include $vaxelnc
#else
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
#endif

#include "pwr.h"
#include "co_time.h"
#include "rt_qcom_msg.h"
#include "rt_qcom.h"
#include "rt_errh.h"
#include "rt_gdh_msg.h"
#include "rt_gdb.h"
#include "rt_gdh.h"
#include "rt_dl.h"
#include "rt_ini_event.h"
#include "rt_net.h"
#include "rt_san.h"
#include "rt_sancm.h"
#include "rt_sansm.h"
#include "rt_sub.h"
#include "rt_subc.h"
#include "rt_subsm.h"
#include "rt_subcm.h"
#include "rt_cmvolsm.h"
#include "rt_cvol.h"
#include "rt_cvolsm.h"
#include "rt_cvolcm.h"

/* Declare routines used by main.  */

static void		addMyNode	(void);
static gdb_sNode*	addNode		(qcom_sNode*);
static void		addNodes	(void);
static void		bugError	(qcom_sGet*);
static void		connectNode	(gdb_sNode*);
static void		flush		(qcom_sGet*);
static void		flushNode	(gdb_sNode*);
static void		flushNodes	(void);
static void		fromNet		(qcom_sGet*);
static void		id		(qcom_sGet*);
static void		idAck2		(qcom_sGet*);
static void		init		(void);
static void		linkActive	(qcom_sGet*);
static void		linkConnect	(qcom_sGet*);
static void		linkDisconnect	(qcom_sGet*);
static void		linkStalled	(qcom_sGet*);
static void		mainLoop	(void);
static void		netError	(qcom_sGet*);
static void		nodeUp		(gdb_sNode*);
static void		processExit	(qcom_sGet*);
static void		sendFlush	(gdb_sNode*);
static void		sendId		(gdb_sNode*);
static void		sendVolumes	(gdb_sNode*, pool_tRef);
static void		sendVolumesR	(gdb_sNode*, net_sVolumes*, unsigned int);
static void		volumes		(qcom_sGet*);
static void		volumesR	(qcom_sGet*);
#if 0
  static void		linkEvent	(pwr_tUInt32, net_eEvent);
  static void		sendIdAck	(gdb_sNode*);
  static void		sendIdAck2	(gdb_sNode*);
#endif


/* Dispatcher for 'net_cMsgClass' messages.  */

static char *cMsg[net_eMsg_] = {
  "error",
  "id",
  "idAck",
  "idAck2",
  "volumes",
  "volumesR",
  "subAdd",
  "subRemove",
  "subData",
  "sanAdd",
  "sanRemove",
  "sanUpdate",
  "nameToObject",
  "oidToObject",
  "objectR",
  "getObjectInfo",
  "getObjectInfoR",
  "setObjectInfo",
  "setObjectInfoR",
  "flush",
  "createObject",
  "deleteObject",
  "moveObject",
  "renameObject",
  "nodeUp",
  "nodeDown",
  "getCclass",
  "getCclassR",
  "getGclass",
  "getGclassR"
};
static void (*fromApplication[net_eMsg_])(qcom_sGet *) = {
  netError,
  id,
  id,
  idAck2,
  volumes,
  volumesR,
  subsm_Add,			/* Add subscription */
  subsm_Remove,			/* Remove subscription */
  subcm_Data,			/* Subscription data transfer */
  sansm_Add,			/* Add subscription */
  sansm_Remove,			/* Remove subscription */
  sancm_Update,			/* Subscription data transfer */
  cvolsm_NameToObject,		/* Cache inquiry, fetch name */
  cvolsm_OidToObject,		/* Cache inquiry, fetch objid */
  bugError,			/* net_eMsg_ObjectR will never reach neth.  */
  cvolsm_GetObjectInfo,
  bugError,			/* net_eMsg_GetObjectInfoR will never reach neth.  */
  cvolsm_SetObjectInfo,
  bugError,			/* net_eMsg_SetObjectInfoR, will never reach neth.  */
  flush,			/* net_eMsg_Flush, Cache flush request */
  cvolcm_CreateObject,
  cvolcm_DeleteObject,
  cvolcm_MoveObject,
  cvolcm_RenameObject,
  bugError,			/* net_eMsg_NodeUp will never reach neth.  */
  bugError,			/* net_eMsg_NodeDown is not for neth. */
  cmvolsm_GetCclass,
  bugError,                     /* net_eMsg_GetCclassR will never reach neth */
  cmvolsm_GetGclass,
  bugError                      /* net_eMsg_GetGclassR will never reach neth */
};

int
main (
  int		argc,
  char		**argv
)
{

  init();

  gdbroot->db->log.b.messages = 0;

  /* Start receive loop which never exits (hopefully).  */

  mainLoop();

  exit(1);
}

/* Add my own node.  */

static void
addMyNode (void)
{
  pwr_tStatus	sts;
  qcom_sNode	node;

  qcom_MyNode(&sts, &node);
  if (EVEN(sts)) errh_Bugcheck(sts, "qcom_MyNode");

  if (node.nid != gdbroot->db->nid)
    errh_Bugcheck(GDH__WEIRD, "Qcom and Gdb dont agree on node identity");

  addNode(&node);
}

/* Add my own node.  */

static gdb_sNode *
addNode (
  qcom_sNode	*node
)
{
  pwr_tStatus	sts;
  gdb_sNode	*np;

  gdb_ScopeLock {

    np = gdb_AddNode(&sts, node->nid, gdb_mAdd__);
    if (np == NULL) break;

    strncpy(np->name, node->name, sizeof(np->name));
    np->os = node->os;
    np->hw = node->hw;
    co_SetFormat(&np->fm, node->bo, node->ft);

    if (np == gdbroot->my_node)
      np->flags.m |= net_eState_up;

  } gdb_ScopeUnlock;

  return np;
}

/* This comment is not valid anymore. Update when we now how
   qcom works.

   This routine receives a list_all_connections message. This
   is requested by the neth_init code, and received in the normal
   stream of messaged (here). This response is necessary to
   get things going...
   This is also requested when a node link up is reported
   since we need to know what OS is run out there...  */

static void
addNodes (void)
{
  pwr_tStatus	sts;
  pwr_tNodeId	nid;
  qcom_sNode	node;
  gdb_sNode*	np;

  gdb_AssumeUnlocked;

  for (nid = qcom_cNNid; qcom_NextNode(&sts, &node, nid); nid = node.nid) {
    np = addNode(&node);
    if (node.flags.b.connected)
      connectNode(np);
  }

}

/* Send flush to all connected node. */

static void
flushNodes (void)
{
  pwr_tStatus	sts;
  pwr_tNodeId	nid;
  qcom_sNode	node;
  gdb_sNode*	np;

  gdb_AssumeUnlocked;

  for (nid = qcom_cNNid; qcom_NextNode(&sts, &node, nid); nid = node.nid) {
    gdb_ScopeLock {
      np = hash_Search(&sts, gdbroot->nid_ht, &node.nid);
    } gdb_ScopeUnlock;

    if (np == NULL)
      continue;

    if (np == gdbroot->my_node || np == gdbroot->no_node)
      continue;

    if (node.flags.b.connected) {
      sendFlush(np);
#if 0
      sendVolumes(np, pool_cNRef); 
#endif
    }
  }
}

/* Handle messages that should not reach net handler.  */

static void
bugError (
  qcom_sGet 	*get
)
{

  errh_Warning("Unexpected message type <%d:%d> received from %x @ %s",
    get->type.b, get->type.s, get->pid,
    cdh_NodeIdToString(NULL, get->sender.nid, 0, 0));
}


static void
connectNode (
  gdb_sNode	*np
)
{

  gdb_AssumeUnlocked;

  if (!np->flags.b.connected) {
    errh_Info("Connected, link to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));

    sendId(np);

    np->flags.b.connected = 1;
    np->flags.b.active = 1;
  }

}

/* Flush all information on remote node.  */

static void
flushNode (
  gdb_sNode		*np
)
{

  gdb_ScopeLock {

    cvolcm_FlushNode(NULL, np);
    cvolsm_FlushNode(NULL, np);
    subsm_FlushNode(NULL, np);
    subcm_FlushNode(NULL, np);
    sansm_FlushNode(NULL, np);
    sancm_FlushNode(NULL, np);

  } gdb_ScopeUnlock;
}

static void
fromEvent (
  qcom_sGet	*get
)
{
  static int  sav_event = 0;
  ini_mEvent  cur_event;
  ini_mEvent  new_event;
  qcom_sEvent *ep = (qcom_sEvent*) get->data;

  if (get->type.s != qcom_cIini)
    return;

  cur_event.m   = sav_event;
  new_event.m  = ep->mask;

  if (new_event.b.swapDone & !cur_event.b.swapDone) {
    errh_Info("Warm restart completed.");   
    flushNodes();
  } else if (new_event.b.swapInit & !cur_event.b.swapInit) {
    errh_Info("Warm restart initiated.");   
  }

  sav_event = ep->mask;
}

/* Dispatcher for MSG_CLAS_PAMS messages.  */

static void
fromNet (
  qcom_sGet 	*get
)
{
  
  switch (get->type.s) {
  case qcom_eStype_linkActive:
    linkActive(get);
    break;
  case qcom_eStype_linkConnect:
    linkConnect(get);
    break;
  case qcom_eStype_linkDisconnect:
    linkDisconnect(get);
    break;
  case qcom_eStype_linkStalled:
    linkStalled(get);
    break;
  case qcom_eStype_applConnect:
    /* Nothing needs to be done */
    break;
  case qcom_eStype_applDisconnect:
    processExit(get);
    break;
  default:
    errh_Error("Unexpected message type <%d:%d> received from %x @ %s",
      get->type.b, get->type.s, get->pid,
      cdh_NodeIdToString(NULL, get->sender.nid, 0, 0));
    break;
  }
}

/* Flush a node.  */

static void
flush (
  qcom_sGet 	*get
)
{
  pwr_tStatus	sts;
  net_sId	*mp = get->data;
  gdb_sNode	*np;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    np = hash_Search(&sts, gdbroot->nid_ht, &mp->hdr.nid);
  } gdb_ScopeUnlock;

  if (np == NULL)
    return;

  if (np->flags.b.connected) {
    errh_Info("Flushing, node %s (%s)", np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
    flushNode(np);
    sendId(np);
  }

}

/* Received Id or IdAck.  */

static void
id (
  qcom_sGet 	*get
)
{
  net_sId	*mp = get->data;
  pwr_tStatus	sts;
  gdb_sNode	*np;
  gdb_sNode	*nid_np;
  net_eEvent	event;

  gdb_AssumeUnlocked;

  if (gdbroot->db->log.b.messages) {
    errh_Info("Received '%s' from nid %s",
      cMsg[get->type.s],  cdh_NodeIdToString(NULL, mp->hdr.nid, 0, 0));
  }

  if (get->sender.nid == gdbroot->my_qid.nid ) {
    errh_Error("New node using nid %s, conflicts with local node, '%s' ignored",
      cdh_NodeIdToString(NULL, mp->hdr.nid, 0, 0),
      cMsg[get->type.s]);
    return;
  }
  if (mp->hdr.nid == gdbroot->db->nid ) {
    errh_Error("New node using nid %s, attempts to appear as local node %s, '%s' ignored",
      cdh_NodeIdToString(NULL, get->sender.nid, 0, 0),
      cdh_NodeIdToString(NULL, mp->hdr.nid, 0, 0),
      cMsg[get->type.s]);
    return;
  }
  if (mp->node.netver != net_cVersion) {
    errh_Error("Proview net protocol version differs, '%s' ignored version: %d, node: %s", 
      cMsg[get->type.s], mp->node.netver,
      cdh_NodeIdToString(NULL, get->sender.nid, 0, 0));
    return;
  }

  if (get->type.s == net_eMsg_id)
    event = net_eEvent_id;
  else
    event = net_eEvent_idAck;

  gdb_ScopeLock {

    np = gdb_AddNode(NULL, get->sender.nid, gdb_mAdd__);

    nid_np = hash_Search(&sts, gdbroot->nid_ht, &mp->hdr.nid);
    if (nid_np == NULL) {
      np->nid = mp->hdr.nid;
      hash_Insert(&sts, gdbroot->nid_ht, np);
    } else if (nid_np != np ) {
      errh_Warning("New node (%s), attempts to appear as node (%s), '%s' ignored...",
	cdh_NodeIdToString(NULL, get->sender.nid, 0, 0),
	cdh_NodeIdToString(NULL, mp->hdr.nid, 0, 0),
	cMsg[get->type.s]);
      event = net_eEvent_error;
      break;
    }

    np->nod_oid = mp->node.nod_oid;

  } gdb_ScopeUnlock;

  sendVolumes(np, pool_cNRef); 
}

/* Handle neti response. Add node to nodedb.  */

static void
idAck2 (
  qcom_sGet	*get
)
{

  errh_Error("Received 'idAck2' from (%s)",
      cdh_NodeIdToString(NULL, get->sender.nid, 0, 0));

}

/* Initialize the nethandler data and communications.  */

static void
init (
  void
)
{
  pwr_tStatus	sts;
  qcom_sQid	qid;

  errh_Init("pwr_neth");

  /* NOTA BENE !
     The net handler does not call gdh_Init and thus may not use
     the externally callable interface of GDH.  */

  qid.qix = net_cProcHandler; 
  qid.nid = qcom_cNNid;

  gdb_MapDb(&sts, &qid, "pwr_neth");
  if (EVEN (sts)) {
    errh_Error("gdb_MapDb, %m", sts);
    exit(sts);
  }

  gdbroot->db->nethandler = qid;

  if (!qcom_Bind(&sts, &gdbroot->my_qid, &qcom_cQnetEvent)) {
    errh_Error("qcom_Bind(QnetEvent), %m", sts);
    exit(sts);
  }

  if (!qcom_Bind(&sts, &gdbroot->my_qid, &qcom_cQapplEvent)) {
    errh_Error("qcom_Bind(QapplEvent), %m", sts);
    exit(sts);
  }

  if (!qcom_Bind(&sts, &gdbroot->my_qid, &qcom_cQini)) {
    errh_Error("qcom_Bind(Qini), %m", sts);
    exit(sts);
  }

  addMyNode();
  addNodes();
}


static void
linkActive (
  qcom_sGet 	*get
)
{
  pwr_tStatus	sts;
  qcom_sNode	*node = (qcom_sNode *)get->data;
  gdb_sNode	*np;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    np = gdb_AddNode(&sts, node->nid, gdb_mAdd__);
  } gdb_ScopeUnlock;


  if (!np->flags.b.active) {
    errh_Info("Active, link to node %s (%s)",
      node->name, cdh_NodeIdToString(NULL, node->nid, 0, 0));

    pwr_Assert(np->flags.b.connected);
    np->flags.b.active = 1;
  }

}

/* Handle Qcom link connect event.  */

static void
linkConnect (
  qcom_sGet	*get
)
{
  qcom_sNode	*node =(qcom_sNode *)get->data;
  gdb_sNode	*np;

  gdb_AssumeUnlocked;

  np = addNode(node);
  connectNode(np);

}

/* This routine takes down a node when the link to it is lost.  */

static void
linkDisconnect (
  qcom_sGet 	*get
)
{
  pwr_tStatus	sts;
  qcom_sNode	*node = (qcom_sNode *)get->data;
  gdb_sNode	*np;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    np = gdb_AddNode(&sts, node->nid, gdb_mAdd__);
  } gdb_ScopeUnlock;


  if (np->flags.b.connected) {
    errh_Info("Disconnected, link to node %s (%s)",
      node->name, cdh_NodeIdToString(NULL, node->nid, 0, 0));

    flushNode(np);

    np->flags.b.connected = 0;
    np->flags.b.active = 0;
  }

}


static void
linkStalled (
  qcom_sGet 	*get
)
{
  pwr_tStatus	sts;
  qcom_sNode	*node = (qcom_sNode *)get->data;
  gdb_sNode	*np;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    np = gdb_AddNode(&sts, node->nid, gdb_mAdd__);
  } gdb_ScopeUnlock;


  if (np->flags.b.active) {
    errh_Info("Stalled, link to node %s (%s)",
      node->name, cdh_NodeIdToString(NULL, node->nid, 0, 0));

    np->flags.b.active = 0;
  }

}

/* Receive messages.  */

static void
mainLoop (void)
{
  pwr_tStatus	sts;
  qcom_sGet 	get;
  void		*mp;

  for (;;) {
    memset(&get, 0, sizeof(get));
    mp = net_Receive(&sts, &get, qcom_cTmoEternal);
    if (mp == NULL) {
      errh_Error("net_Receive %m", sts);
      continue;
    }

    switch (get.type.b) {
    case qcom_eBtype_qcom:
      if (gdbroot->db->log.b.messages)
	errh_Info("Received message from %x (%d) @ %s, aix: %d, type=%d, subtype=%d and length=%d",
	  get.pid, get.pid, cdh_NodeIdToString(NULL, get.sender.nid, 0, 0),
	  get.sender.aix, get.type.b, get.type.b, get.size);
      fromNet(&get);
      break;
    case net_cMsgClass:
      if ((int)get.type.s > (int)net_eMsg__ && (int)get.type.s < (int)net_eMsg_) {
	if (gdbroot->db->log.b.messages) {
	  errh_Info("Received '%s' from %x @ %s, length=%d",
	    cMsg[get.type.s], get.pid,
	    cdh_NodeIdToString(NULL, get.sender.nid, 0, 0), get.size);
	}
	fromApplication[get.type.s](&get);
      } else {
	errh_Warning("Unexpected message sub type %d", get.type.s);
      }
      break;      							    
    case qcom_eBtype_event:
      fromEvent(&get);
      break;      							    
    default:
      errh_Warning("Unexpected message type %d, subtype %d\n pid %x sender.aix %d, sender.nid 0x%x\n, reply.qix 0x%x, reply.nid 0x%x", 
		   get.type.b, get.type.s, get.pid, get.sender.aix, get.sender.nid,
		   get.reply.qix, get.reply.nid);
      break;
    }

    qcom_Free(&sts, mp);
  }
}

/* Handle 'net_eMsh_Error' message.  */

static void
netError (
  qcom_sGet 	*get
)
{
  net_sError	*mp = get->data;

  if (ODD(mp->sts)) return;

  errh_Error("Error reported by process %x @ %s when talking to process %d @ %s\n%m",
    get->pid, cdh_NodeIdToString(NULL, get->sender.nid, 0, 0),
    mp->remqid.qix, cdh_NodeIdToString(NULL, mp->remqid.nid, 0, 0), mp->sts);

#if 0 /* TODO !!!! */
  if (mp->remqid.qix == net_cProcHandler) {
    linkEvent(mp->remqid.nid, net_eEvent_error);
  }
#endif
}

static void
nodeUp (
  gdb_sNode	  *np
)
{
  pwr_tStatus	  sts;
  net_sNodeUp     nu;
  qcom_sPut	  put;
  

  np->upcnt++;
  clock_gettime(CLOCK_REALTIME, &np->timeup);
  errh_Info("Established contact with %s (%s)",
    np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));

  /* Send info to neth_acp */

  memset(&nu, 0, sizeof(nu));
  nu.hdr.nid = gdbroot->db->nid;
  nu.nid = np->nid;

  put.type.b  = net_cMsgClass;
  put.type.s  = net_eMsg_nodeUp;
  put.reply   = qcom_cNQid;
  put.data    = (char *)&nu;
  put.size    = sizeof(nu);

  qcom_Put(&sts, &qcom_cQnacp, &put);
}

/* This routine handles process rundown situations.
   Currently, all subscriptions are cancelled.  */

static void
processExit (
  qcom_sGet 	*get

)
{
  qcom_sAppl	*appl = (qcom_sAppl *)get->data;

  errh_Info("Process %d @ %s dissapeared, cleaning up", (int)appl->aid.aix,
    cdh_NodeIdToString(NULL, appl->aid.nid, 0, 0));

  gdb_ScopeLock {

    dl_CancelUser(appl->pid);
    subc_CancelUser(appl->pid);

  } gdb_ScopeUnlock;

}

/* Send a flush to a remote node.  */

static void
sendFlush (
  gdb_sNode		*np
)
{
  pwr_tStatus		sts;
  net_sId		msg;
  qcom_sQid		tgt;
  pwr_tUInt32		size;

  if (gdbroot->db->log.b.id) {
    errh_Info("Sending 'flush' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
  }

  tgt.nid = np->nid;
  tgt.qix = net_cProcHandler;

  size = sizeof(msg);

  msg.node.handler  = gdbroot->db->nethandler;
  msg.node.netver   = net_cVersion;
  msg.hdr.nid	    = gdbroot->my_node->nid;
  msg.node.nod_oid  = gdbroot->my_node->nod_oid;
  msg.node.vol_oid  = gdbroot->my_node->vol_oid;

  net_Put(&sts, &tgt, &msg, net_eMsg_flush, size);
  if (EVEN(sts))
    errh_Error("Sending 'flush' to %s (%s)\n%m",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), sts);

}

/* This routine sends an id to a remote node.  */

static void
sendId (
  gdb_sNode		*np
)
{
  pwr_tStatus		sts;
  net_sId		msg;
  qcom_sQid		tgt;
  pwr_tUInt32		size;

  if (gdbroot->db->log.b.id) {
    errh_Info("Sending 'id' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
  }

  tgt.nid = np->nid;
  tgt.qix = net_cProcHandler;

  size = sizeof(msg);

  msg.node.handler  = gdbroot->db->nethandler;
  msg.node.netver   = net_cVersion;
  msg.hdr.nid	    = gdbroot->my_node->nid;
  msg.node.nod_oid  = gdbroot->my_node->nod_oid;
  msg.node.vol_oid  = gdbroot->my_node->vol_oid;

  net_Put(&sts, &tgt, &msg, net_eMsg_id, size);
  if (EVEN(sts))
    errh_Error("Sending 'id' to %s (%s)\n%m",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), sts);

}

/* Send a list of all volumes owned by this node to the
   remote node.  */

static void
sendVolumes (
  gdb_sNode		*np,
  pool_tRef		vr
)
{
  pwr_tStatus		sts;
  net_sVolumes		*mp;
  qcom_sQid		tgt;
  pwr_tUInt32		size;
  int			i;
  int			nVol;
  pool_sQlink		*vl;
  pool_sQlink		*vlStart;
  gdb_sVolume		*vp;
  int			maxVol = ((net_cSizeLarge - sizeof(*mp)) / sizeof(mp->g[0])) + 1;

  if (gdbroot->db->log.b.messages) {
    errh_Info("Sending 'volumes' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
  }

  gdb_ScopeLock {

    if (vr == pool_cNRef) {
      vlStart = pool_Qsucc(NULL, gdbroot->pool, &gdbroot->my_node->own_lh);
    } else {
      errh_Info("sendVolumes: vr != pool_cNRef, maxVol: %d", maxVol);
      vp = pool_Address(NULL, gdbroot->pool, vr);
      vlStart = &vp->l.own_ll;
    }

    for (
      nVol = 0, vl = vlStart;
      nVol < maxVol && vl != &gdbroot->my_node->own_lh;
      nVol++, vl = pool_Qsucc(NULL, gdbroot->pool, vl)
    );
    
    size = sizeof(*mp) + ((nVol - 1) * sizeof(mp->g[0]));
    mp = (net_sVolumes *) malloc(size);
    if (mp == NULL) break;

    for (
      i = 0, vl = vlStart;
      i < nVol && vl != &gdbroot->my_node->own_lh;
      i++, vl = pool_Qsucc(NULL, gdbroot->pool, vl)
    ) {
      vp = pool_Qitem(vl, gdb_sVolume, l.own_ll);
      mp->g[i] = vp->g;
    }
       
    mp->count = i;
    if (vl == &gdbroot->my_node->own_lh)
      mp->ctx = pool_cNRef;
    else
      mp->ctx = vl->flink;

  } gdb_ScopeUnlock;

  if (mp == NULL) {
    errh_Error("Failed to allocate 'volumes' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
    return;
  }

  tgt.nid  = np->nid;
  tgt.qix = net_cProcHandler;

  if (!net_Put(&sts, &tgt, mp, net_eMsg_volumes, size))
    errh_Error("Sending 'volumes' to %s (%s)\n%m",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), sts);

  free(mp);
}

/* Send a list of the volumes we want to mount at the
   remote node.  */

static void
sendVolumesR (
  gdb_sNode		*np,
  net_sVolumes		*vmp,
  unsigned int		nConnect
)
{
  pwr_tStatus		sts;
  net_sVolumesR		*mp;
  qcom_sQid		tgt;
  pwr_tUInt32		size;
  int			i;
  int			j;

  if (gdbroot->db->log.b.messages) {
    errh_Info("Sending 'volumesR' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
  }

  size = sizeof(*mp) + sizeof(mp->mount[0]) * (nConnect - 1);
  mp = (net_sVolumesR *) malloc(size);
  if (mp == NULL) {
    errh_Error("Failed to allocate 'volumesR' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, (pwr_tNodeId)np->nid, 0, 0));
    return;
  }
  
  for (i = 0, j = 0; i < vmp->count && j < nConnect; i++) {
    if (vmp->g[i].vid != pwr_cNVolumeId)
      mp->mount[j++].vid = vmp->g[i].vid;
  }

  mp->count = j;
  mp->ctx = vmp->ctx;

  tgt.nid  = np->nid;
  tgt.qix = net_cProcHandler;

  if (!net_Put(&sts, &tgt, mp, net_eMsg_volumesR, size)) {
    errh_Error("Sending 'volumesR' to %s (%s)\n%m",
      np->name, cdh_NodeIdToString(NULL, (pwr_tNodeId)np->nid, 0, 0), sts);
  }
  free(mp);
}

/* Receive a list of all volumes owned by the sending node.  */

static void
volumes (
  qcom_sGet 		*get
)
{
  pwr_tStatus		sts;
  net_sMessage		*mp  = (net_sMessage *)get->data;
  net_sVolumes		*vmp = (net_sVolumes *) mp;
  gdb_sVolume		*vp;
  int			i;
  int			nConnect = 0;
  gdb_sNode		*np;
  pwr_tUInt32		nid = get->sender.nid;

  gdb_ScopeLock {

    np = hash_Search(&sts, gdbroot->nid_ht, &nid);

    if (!np->flags.b.connected) {
      np = NULL;
      break;
    }

    for (i = 0; i < vmp->count; i++) {

      if (vmp->g[i].cid == pwr_eClass_ClassVolume) {
        cvolcm_AddClassVolume(&sts, np, &vmp->g[i]);
        
      } else {
        
        vp = gdb_AddVolume(&sts, vmp->g[i].vid, gdb_mAdd__);
        /* if (vp == NULL) ?;  */

        if (vp->l.flags.b.isConnected) {
          cvolcm_FlushVolume(&sts, vp);
          /* !!! Todo !!! This volume is either owned by another node
             or was not flushed.  */
          errh_Error("Volume %s (%s) is connected", vp->g.name.orig, cdh_VolumeIdToString(NULL, vp->g.vid, 1, 0));
        }

        if (vp->l.flags.b.isMounted) {
          cvolcm_ConnectVolume(&sts, vp, &vmp->g[i], np);
          nConnect++;
        } else {
          vmp->g[i].vid = pwr_cNVolumeId;
        }
      }
    }
    
  } gdb_ScopeUnlock;

  if (np == NULL)
    return;

  sendVolumesR(np, vmp, nConnect);

  if (vmp->ctx == pool_cNRef)
    nodeUp(np);
}

/* Receive a list of all volumes the remote node have mounted.  */

static void
volumesR (
  qcom_sGet 		*get
)
{
  net_sVolumesR		*mp = (net_sVolumesR *) get->data;
  int			i;
  gdb_sNode		*np;

  gdb_ScopeLock {

    np = hash_Search(NULL, gdbroot->nid_ht, &mp->hdr.nid);

    if (!np->flags.b.connected) {
      np = NULL;
      break;
    }

    for (i = 0; i < mp->count; i++) {
      cvolsm_AddMountedOn(NULL, mp->mount[i].vid, np);
    }

  } gdb_ScopeUnlock;

  if (np == NULL)
    return;

  if (mp->ctx != pool_cNRef)
    sendVolumes(np, mp->ctx);

}

#if 0
/* This is the link state machine
   The database must be locked by the caller.  */

void
linkEvent (
  pwr_tNodeId		nid,
  net_eEvent		le
)
{
  pwr_tStatus		sts;
  gdb_sNode		*np;

  gdb_AssumeUnlocked;

  /* Never to ourselves... */
  if (nid == gdbroot->db->nid) return;

  gdb_ScopeLock {
    np = gdb_AddNode(&sts, nid, gdb_mAdd__);
  } gdb_ScopeUnlock;

  if (gdbroot->db->log.b.linkStates)
    errh_Info("Link event %d, nid %d, current link state %d", le, nid, np->linkstate);

  switch (np->linkstate) {
  case net_eState_down:
    switch (le) {
    case net_eEvent_nodeDown:
      break;
    case net_eEvent_nodeUp:
    default:
      sendId(np);
      np->linkstate = net_eState_istrt;
      break;
    }
    break;
  case net_eState_istrt:
    switch (le) {
    case net_eEvent_id:
      sendIdAck(np);
      np->linkstate = net_eState_astrt;
      break;
    case net_eEvent_idAck:
      sendIdAck2(np);
      np->linkstate = net_eState_up;
      np->upcnt++;
      clock_gettime(CLOCK_REALTIME, &np->timeup);
      errh_Info("Established contact with %s (%s)",
	np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
      sendVolumes(np, pool_cNRef); 
      break;
    case net_eEvent_nodeDown:
      np->linkstate = net_eState_down;
      break;
    case net_eEvent_nodeUp:
    case net_eEvent_error:
      break;
    default:
      sendId(np);
      np->linkstate = net_eState_istrt;
      break;
    }
    break;
  case net_eState_astrt:
    switch (le) {
    case net_eEvent_id:
      sendIdAck(np);
      np->linkstate = net_eState_astrt;
      break;
    case net_eEvent_idAck:
      sendIdAck2(np);
      np->linkstate = net_eState_up;
      np->upcnt++;
      clock_gettime(CLOCK_REALTIME, &np->timeup);
      errh_Info("Established contact with %s (%s)",
	np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
      sendVolumes(np, pool_cNRef);
      break;
    case net_eEvent_idAck2:
      np->linkstate = net_eState_up;
      np->upcnt++;
      clock_gettime(CLOCK_REALTIME, &np->timeup);
      errh_Info("Established contact with %s (%s)",
	np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
      sendVolumes(np, pool_cNRef);
      break;
    case net_eEvent_nodeDown:
      np->linkstate = net_eState_down;
      break;
    case net_eEvent_nodeUp:
      break;
    case net_eEvent_error:
      break;
    default:
      sendId(np);
      np->linkstate = net_eState_istrt;
      break;
    }
    break;
  case net_eState_up:
    switch (le) {
    case net_eEvent_id:
      flushNode(np);
      sendIdAck(np);
      np->linkstate = net_eState_astrt;
      errh_Info("New contact with %s (%s), remotely initiated...",
	np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
      break;
    case net_eEvent_nodeDown:
      flushNode(np);
      np->linkstate = net_eState_down;
      errh_Info("Down, link to %s (%s)",
	np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
      break;
    case net_eEvent_error:
      flushNode(np);
      sendId(np);
      np->linkstate = net_eState_istrt;
      errh_Info("New contact with %s (%s), locally initiated...",
	np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
      break;
    case net_eEvent_idAck:
      sendIdAck2(np);
      break;
    case net_eEvent_idAck2:
    case net_eEvent_nodeUp:
    default:
      break;
    }
    break;
  default:
    break;
  }
}
#endif		
#if 0

/* This routine sends an idAck to a remote node.  */

static void
sendIdAck (
  gdb_sNode		*np
)
{
  pwr_tStatus		sts;
  net_sId		msg;
  qcom_sQid		tgt;

  if (gdbroot->db->log.b.messages) {
    errh_Info("Sending 'idAck' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
  }

  tgt.nid  = np->nid;
  tgt.qix = net_cProcHandler;

  gdb_ScopeLock {

    msg.node.handler  = gdbroot->db->nethandler;
    msg.node.netver   = net_cVersion;
    msg.hdr.nid	    = gdbroot->my_node->nid;
    msg.node.nod_oid  = gdbroot->my_node->nod_oid;
    msg.node.vol_oid  = gdbroot->my_node->vol_oid;

  } gdb_ScopeUnlock;

  if (!net_Put(&sts, &tgt, &msg, net_eMsg_idAck, sizeof(msg)))
    errh_Error("Sending 'idAck' to %s (%s)\n%m",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), sts);

}

/* This routine sends an idAck2 to a remote node.  */

static void
sendIdAck2 (
  gdb_sNode		*np
)
{
  pwr_tStatus		sts;
  net_sIdAck2		msg;
  qcom_sQid		tgt;

  if (gdbroot->db->log.b.messages) {
    errh_Info("Sending 'idAck2' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
  }

  tgt.nid  = np->nid;
  tgt.qix = net_cProcHandler;

  if (!net_Put(&sts, &tgt, &msg, net_eMsg_idAck2, sizeof(msg)))
    errh_Error("Sending 'idAck2' to %s (%s)\n%m",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), sts);

}
#endif
