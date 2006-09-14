/* 
 * Proview   $Id: rt_procom.cpp,v 1.1 2006-09-14 14:16:07 claes Exp $
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

/* rt_provider.c -- Runtime provider test */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vector.h>
#include <map.h>

extern "C" {
#include "pwr.h"
#include "co_time.h"
#include "rt_qcom_msg.h"
#include "rt_qcom.h"
#include "rt_errh.h"
#include "rt_qdb.h"
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
#include "rt_pwr_msg.h"
#include "rt_ndc.h"
}

#include "co_provider.h"
#include "rt_procom.h"

class psub {
public:
  psub() : occupied(1), count(0), nid(0), dt(0), sub_by_name(0), p(0) {}

  int occupied; 
  pwr_tSubid  sid;
  pwr_tUInt32 count;
  pwr_tNodeId nid;
  pwr_tUInt32 dt;
  pwr_tBoolean sub_by_name;
  pwr_sAttrRef aref;
  void *p;
};

class subkey {
public:
  int m_nid;
  int m_rix;

  subkey() : m_nid(0), m_rix(0) {}
  subkey( pwr_tSubid sid) : m_nid(sid.nid), m_rix(sid.rix) {}
  friend bool operator<(const subkey x1, const subkey x2) {
    if ( x1.m_nid < x2.m_nid)
      return true;
    if ( x1.m_nid == x2.m_nid && x1.m_rix < x2.m_rix)
      return true;
    return false;
  }
};

typedef map<subkey, psub>::iterator sublist_iterator;
map<subkey, psub> m_sublist;

static rt_procom *pvd_procom;

rt_procom::rt_procom( co_provider *p, errh_eAnix anix, char *name, pwr_tSid sid, pwr_tVid vid,
			  char *volumename, int global) :
  co_procom(p), m_anix(anix), m_sid(sid), m_vid(vid), m_global(global), m_getmsg(0)
{
  pvd_procom = this;
  strcpy( m_name, name);
  strcpy( m_volumename, volumename);
}

// Configuration data
static qcom_tQix 	pvd_qix = 0; // 1 << 30;
static qcom_sQid	pvd_qid;

/* Declare routines used by main.  */

static void		addMyNode	(void);
static gdb_sNode*	addNode		(qcom_sNode*);
static void		bugError	(qcom_sGet*);
static void		flush		(qcom_sGet*);
static void		flushNode	(gdb_sNode*);
static void		flushAllNodes	(void);
static void		fromNet		(qcom_sGet*);
static void		id		(qcom_sGet*);
static void		idAck2		(qcom_sGet*);
static void		linkActive	(qcom_sGet*);
static void		linkConnect	(qcom_sGet*);
static void		linkDisconnect	(qcom_sGet*);
static void		linkStalled	(qcom_sGet*);
static void		netError	(qcom_sGet*);
static void		sendFlush	(gdb_sNode*);
static void		sendId		(gdb_sNode*);
static void		sendVolumes	(gdb_sNode*, pool_tRef);
static void		volumes		(qcom_sGet*);
static void		volumesR	(qcom_sGet*);
static void		volumes7	(qcom_sGet*);
#if 0
  static void		linkEvent	(pwr_tUInt32, net_eEvent);
  static void		sendIdAck	(gdb_sNode*);
  static void		sendIdAck2	(gdb_sNode*);
#endif

static void
respondError (
  qcom_sGet		*get,
  pwr_tObjid		oid,
  pwr_tStatus		rspsts
);
void
pvd_OidToObject (
  qcom_sGet		*get
);
void
pvd_NameToObject (
  qcom_sGet		*get
);
static void
sendServerConnect (
  gdb_sNode		*np
);
static void
sendServerConnectAll (
);
static void
pvd_GetObjectInfo (
  qcom_sGet		*get
);
static void
pvd_SetObjectInfo (
  qcom_sGet		*get
);
static void
pvd_SubAdd (
  qcom_sGet		*get
);
static void
pvd_SubRemove (
  qcom_sGet		*get
);
static pwr_tBoolean
pvd_SubSendBuffer ();


/* Dispatcher for 'net_cMsgClass' messages.  */

static char *cMsg[net_eMsg_end] = {
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
  "getGclassR",
  "net_eMsg_",
  "volumes7"
  "net_eMsg_",
};
static void (*fromApplication[net_eMsg_end])(qcom_sGet *) = {
  netError,
  id,
  id,
  idAck2,
  volumes,
  volumesR,
  pvd_SubAdd,			/* Add subscription */
  pvd_SubRemove,	       	/* Remove subscription */
  subcm_Data,			/* Subscription data transfer */
  sansm_Add,			/* Add subscription */
  sansm_Remove,			/* Remove subscription */
  sancm_Update,			/* Subscription data transfer */
  pvd_NameToObject,		/* Cache inquiry, fetch name */
  pvd_OidToObject,		/* Cache inquiry, fetch objid */
  bugError,			/* net_eMsg_ObjectR will never reach neth.  */
  pvd_GetObjectInfo,
  bugError,			/* net_eMsg_GetObjectInfoR will never reach neth.  */
  pvd_SetObjectInfo,
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
  bugError,                     /* net_eMsg_GetGclassR will never reach neth */
  bugError,                     /* net_eMsg_ */
  volumes7,
  bugError                      /* net_eMsg_serverConnect */
};

#if 0
int
main (
  int		argc,
  char		**argv
)
{

  init();

  /* Start receive loop which never exits (hopefully).  */

  mainLoop();

  exit(1);
}
#endif


/* Add my own node.  */

static void
addMyNode (void)
{
  pwr_tStatus	sts;
  qcom_sNode	node;
  gdb_sNode     *np;

  qcom_MyNode(&sts, &node);
  if (EVEN(sts)) errh_Bugcheck(sts, "qcom_MyNode");

  if (node.nid != gdbroot->db->nid)
    errh_Bugcheck(GDH__WEIRD, "Qcom and Gdb dont agree on node identity");

  np = addNode(&node);
  if (np == NULL) errh_Bugcheck(0, "addNode(myNode)");

  gdb_ScopeLock {
    np->netver = net_cVersion;
    np->cclassSupport = FALSE;
    np->handler = pvd_qid;
  } gdb_ScopeUnlock;

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

    np = gdb_AddNode(&sts, gdb_Snid( node->nid, pvd_procom->m_sid), gdb_mAdd__);
    if (np == NULL) break;

    sprintf(np->name, "%s_s%d", node->name, pvd_procom->m_sid);
    np->os = node->os;
    np->hw = node->hw;
    np->upcnt++;
    clock_gettime( CLOCK_REALTIME, &np->timeup);
    co_SetFormat(&np->fm, node->bo, node->ft);

    gdbroot->my_node = np;
    if (np == gdbroot->my_node)
      np->flags.m |= net_eState_up;  /* ERROR. ERROR, ERROR !!! */

  } gdb_ScopeUnlock;

  return np;
}

/* Send flush to all connected node. */

static void
flushAllNodes (void)
{
  pwr_tStatus	sts;
  pwr_tNodeId	nid;
  qcom_sNode	node;
  gdb_sNode*	np;

  gdb_AssumeUnlocked;

  for (nid = qcom_cNNid; qcom_NextNode(&sts, &node, nid); nid = node.nid) {
   gdb_ScopeLock {
      np = (gdb_sNode *) hash_Search(&sts, gdbroot->nid_ht, &node.nid);
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
    flushAllNodes();
  } else if (new_event.b.swapInit & !cur_event.b.swapInit) {
    errh_Info("Warm restart initiated.");   
  } else if (new_event.b.terminate & !cur_event.b.terminate) {
    exit(0);   
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
  net_sId	*mp = (net_sId *) get->data;
  gdb_sNode	*np;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    np = (gdb_sNode *) hash_Search(&sts, gdbroot->nid_ht, &mp->hdr.nid);
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
  net_sId	*mp = (net_sId *) get->data;
  pwr_tStatus	sts;
  gdb_sNode	*np;
  net_eEvent	event;
  pwr_tBoolean  cclassSupport = FALSE;

  gdb_AssumeUnlocked;

  if (gdbroot->db->log.b.messages) {
    errh_Info("Received '%s' from nid %s",
      cMsg[get->type.s],  cdh_NodeIdToString(NULL, mp->hdr.nid, 0, 0));
  }

  if (mp->hdr.nid == gdbroot->my_node->nid ) {
    errh_Error("New node using nid %s, attempts to appear as local node %s, '%s' ignored",
      cdh_NodeIdToString(NULL, get->sender.nid, 0, 0),
      cdh_NodeIdToString(NULL, mp->hdr.nid, 0, 0),
      cMsg[get->type.s]);
    return;
  }
  if (mp->node.netver != net_cVersion) {
    if (mp->node.netver == 7) {
      cclassSupport = FALSE;
      errh_Warning("Accepting older net protocol version, '%s' old version: %d, my version: %d, node: %s",
                   cMsg[get->type.s], mp->node.netver, net_cVersion,
                   cdh_NodeIdToString(NULL, get->sender.nid, 0, 0));
                   
    } else if (mp->node.netver > net_cVersion) {
      errh_Warning("Accepting newer net protocol version, '%s' new version: %d, my version: %d, node: %s",
                   cMsg[get->type.s], mp->node.netver, net_cVersion,
                   cdh_NodeIdToString(NULL, get->sender.nid, 0, 0));
    } else {      
      errh_Error("Proview net protocol version not supported, '%s' ignored version: %d, my version: %d, node: %s", 
                 cMsg[get->type.s], mp->node.netver, net_cVersion,
                 cdh_NodeIdToString(NULL, get->sender.nid, 0, 0));
      return;
    }
  }



  if ((net_eMsg)get->type.s == net_eMsg_id)
    event = net_eEvent_id;
  else
    event = net_eEvent_idAck;

  gdb_ScopeLock {

    // np = gdb_AddNode(NULL, get->sender.nid, gdb_mAdd__);

    np = (gdb_sNode *) hash_Search(&sts, gdbroot->nid_ht, &mp->hdr.nid);
    if (np == NULL) {
      errh_Warning("New node (%s) not connected",
		   cdh_NodeIdToString(NULL, get->sender.nid, 0, 0));
      event = net_eEvent_error;
      break;
    }

    // np->nod_oid = mp->node.nod_oid;
    // np->netver = mp->node.netver;
    // np->cclassSupport = cclassSupport;

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

void rt_procom::init()
{
  pwr_tStatus	sts;
  qcom_sQid	qid;

  errh_Init( m_name, m_anix);
  errh_SetStatus( PWR__SRVSTARTUP);

  /* NOTA BENE !
     The net handler does not call gdh_Init and thus may not use
     the externally callable interface of GDH.  */

  qid.qix = pvd_qix;
  qid.nid = qcom_cNNid;

  gdb_MapDb(&sts, &qid, m_name);
  if (EVEN (sts)) {
    errh_Error("gdb_MapDb, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  // gdbroot->db->nethandler = qid;
  pvd_qid = qid;

  if (!qcom_Bind(&sts, &gdbroot->my_qid, &qcom_cQnetEvent)) {
    errh_Error("qcom_Bind(QnetEvent), %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  if (!qcom_Bind(&sts, &gdbroot->my_qid, &qcom_cQapplEvent)) {
    errh_Error("qcom_Bind(QapplEvent), %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  if (!qcom_Bind(&sts, &gdbroot->my_qid, &qcom_cQini)) {
    errh_Error("qcom_Bind(Qini), %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  addMyNode();

  sendServerConnectAll();
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

static void linkConnect ( qcom_sGet *get)
{
  gdb_sNode *np;
  qcom_sNode	*node =(qcom_sNode *)get->data;

  if (!pvd_procom->m_global)
    return;

  np = (gdb_sNode *) hash_Search(NULL, gdbroot->nid_ht, &node->nid);
  sendServerConnect( np);
}
static void linkDisconnect ( qcom_sGet *get)
{
}

static void linkStalled ( qcom_sGet *get)
{
}


/* Receive messages.  */

void rt_procom::mainLoop()
{
  pwr_tStatus	sts;
  qcom_sGet 	get;
  void		*mp;
  pwr_tBoolean 	log_alloc_fail = TRUE;
  int		tmo = 1000;

  errh_SetStatus( PWR__SRUN);

  for (;;) {
    memset(&get, 0, sizeof(get));
    mp = net_Receive(&sts, &get, tmo);
    if ( sts == QCOM__TMO) {
      m_provider->cyclic( this);
      pvd_SubSendBuffer();
      continue;
    }
    if (mp == NULL) {
      if (log_alloc_fail) errh_Error("net_Receive %m", sts);
      log_alloc_fail = FALSE;
      continue;
    }
    
    log_alloc_fail = TRUE;

    switch (get.type.b) {
    case qcom_eBtype_qcom:
      if (gdbroot->db->log.b.messages)
	errh_Info("Received message from %x (%d) @ %s, aix: %d, type=%d, subtype=%d and length=%d",
	  get.pid, get.pid, cdh_NodeIdToString(NULL, get.sender.nid, 0, 0),
	  get.sender.aix, get.type.b, get.type.b, get.size);
      fromNet(&get);
      break;
    case net_cMsgClass:
      if (((int)get.type.s > (int)net_eMsg__  && (int)get.type.s < (int)net_eMsg_)
          || ((net_eMsg) get.type.s == net_eMsg_volumes7)
      ) {
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

    pvd_SubSendBuffer();
  }
}

static void netError ( qcom_sGet *get)
{
}

static void sendFlush ( gdb_sNode *np)
{
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

  msg.node.handler  = pvd_qid;
  msg.node.netver   = net_cVersion;
  msg.hdr.nid	    = gdbroot->my_node->nid;
  msg.node.nod_oid  = gdbroot->my_node->nod_oid;
  msg.node.vol_oid  = gdbroot->my_node->vol_oid;

  net_Put(&sts, &tgt, &msg, net_eMsg_id, 0, size, pvd_procom->m_sid);
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
  net_sVolumes7 	*m7p;
  qcom_sQid		tgt;
  pwr_tUInt32		size;
  pwr_tBoolean          cclassSupport;
  net_eMsg              msgtype;
  int			nVol = 1;
  cdh_sObjName		oname;

  if (gdbroot->db->log.b.messages) {
    errh_Info("Sending 'volumes' to %s (%s)",
      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
  }

  cclassSupport = np->cclassSupport;

  if (!cclassSupport)
    size = sizeof(*m7p) + ((nVol - 1) * sizeof(m7p->g[0]));
  else
    size = sizeof(*mp) + ((nVol - 1) * sizeof(mp->g[0]));

  mp = (net_sVolumes *) malloc(size);
  if (mp == NULL) {
    errh_Error("Failed to allocate 'volumes' to %s (%s)",
	       np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
    return;
  }


  cdh_ObjName( &oname, pvd_procom->m_volumename);
  if (!cclassSupport) {
    m7p = (net_sVolumes7 *) mp;

    m7p->g[0].vid = pvd_procom->m_vid;
    m7p->g[0].oid.oix = 0;
    m7p->g[0].oid.vid = pvd_procom->m_vid;
    m7p->g[0].name = oname;
    m7p->g[0].cid = pwr_eClass_ExternVolume;
    m7p->g[0].nid = gdbroot->my_node->nid;
    msgtype = net_eMsg_volumes7;
  } 
  else {
    mp->g[0].vid = pvd_procom->m_vid;
    mp->g[0].oid.oix = 0;
    mp->g[0].oid.vid = pvd_procom->m_vid;
    mp->g[0].name = oname;
    mp->g[0].cid = pwr_eClass_ExternVolume;
    mp->g[0].nid = gdbroot->my_node->nid;
    mp->g[0].time = pwr_cNTime;
    msgtype = net_eMsg_volumes;
  }
  
  mp->count = 1;
  mp->ctx = pool_cNRef;

  tgt.nid  = np->nid;
  tgt.qix = net_cProcHandler;

  if (!net_Put(&sts, &tgt, mp, msgtype, 0, size, 0))
    errh_Error("Sending 'volumes' to %s (%s)\n%m",
	       np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), sts);

  free(mp);
}


/* Receive a list of all volumes owned by the sending node.  */

static void
volumes (
  qcom_sGet 		*get
)
{
}

/* Receive a list of all volumes owned by the sending node.  
 * Neth version 7 of net_sVolumes
 */

static void
volumes7 (
  qcom_sGet 		*get
)
{
}


/* Receive a list of all volumes the remote node have mounted.  */

static void
volumesR (
  qcom_sGet 		*get
)
{
  net_sVolumesR		*mp = (net_sVolumesR *) get->data;
  gdb_sNode		*np;

  gdb_ScopeLock {

    np = (gdb_sNode *) hash_Search(NULL, gdbroot->nid_ht, &mp->hdr.nid);

    if (!np->flags.b.connected) {
      np = NULL;
      break;
    }

  } gdb_ScopeUnlock;

  if (np == NULL)
    return;

  if (mp->ctx != pool_cNRef)
    sendVolumes(np, mp->ctx);

}

void
pvd_OidToObject (
  qcom_sGet		*get
)
{
  net_sOidToObject	*mp = (net_sOidToObject *)get->data;

  if ( mp->oid.vid == pvd_procom->m_vid) {
    pvd_procom->m_getmsg = get;
    pvd_procom->m_provider->objectOid( pvd_procom, mp->oid.oix);
  }
  else
    respondError(get, pwr_cNObjid, GDH__NOSUCHOBJ);
}

void
pvd_NameToObject (
  qcom_sGet		*get
)
{
  net_sNameToObject	*mp = (net_sNameToObject *)get->data;

  pvd_procom->m_getmsg = get;
  pvd_procom->m_provider->objectName( pvd_procom, mp->name);
}

static void
respondError (
  qcom_sGet		*get,
  pwr_tObjid		oid,
  pwr_tStatus		rspsts
)
{
  pwr_tStatus		sts;
  net_sObjectR		*rsp;
  qcom_sPut		put;

  rsp = (net_sObjectR *) net_Alloc(&sts, &put, sizeof(*rsp), net_eMsg_objectR);
  if (rsp == NULL) errh_Bugcheck(sts, "net_Alloc");

  rsp->sts    = rspsts;
  rsp->oid    = oid;
  rsp->count  = 0;

  if (!net_Reply(&sts, get, &put, 0))
    errh_Bugcheck(sts, "net_Reply");
}

/* This routine sends serverConnect to a node.  */

static void
sendServerConnect (
  gdb_sNode		*np
)
{
  pwr_tStatus		sts;
  net_sId		msg;
  qcom_sQid		tgt;
  pwr_tUInt32		size;

  if (gdbroot->db->log.b.id) {
    errh_Info("Sending 'serverConnect' to %s (%s)",
	      np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0));
  }

  tgt.nid = np->nid;
  tgt.qix = net_cProcHandler;

  size = sizeof(msg);

  msg.node.handler  = pvd_qid;
  msg.node.netver   = net_cVersion;
  msg.hdr.nid	    = gdbroot->my_node->nid;
  msg.node.nod_oid  = gdbroot->my_node->nod_oid;
  msg.node.vol_oid  = gdbroot->my_node->vol_oid;
  
  net_Put(&sts, &tgt, &msg, net_eMsg_serverConnect, 0, size, pvd_procom->m_sid);
  if (EVEN(sts))
    errh_Error("Sending 'serverConnect' to %s (%s)\n%m",
	       np->name, cdh_NodeIdToString(NULL, np->nid, 0, 0), sts);
}

static void
sendServerConnectAll (
)
{
  pwr_tStatus		sts;
  qcom_sNode		node;
  pwr_tNodeId		nid;

  gdb_sNode		*np;

  // Send to local node
  gdb_ScopeLock {
    np = (gdb_sNode *) hash_Search(NULL, gdbroot->nid_ht, &gdbroot->db->nid);
  } gdb_ScopeUnlock;

  sendServerConnect( np);


  if ( pvd_procom->m_global) {
    // Send to all other nodes...
    for (nid = qcom_cNNid; qcom_NextNode(&sts, &node, nid); nid = node.nid) {
      gdb_ScopeLock {
	np = (gdb_sNode *) hash_Search(&sts, gdbroot->nid_ht, &node.nid);
      } gdb_ScopeUnlock;

      if (np == NULL)
	continue;

      if (node.flags.b.connected) {
	sendServerConnect( np);
      }
    }
  }
}

static void
pvd_SetObjectInfo (
  qcom_sGet		*get
)
{
  pwr_tStatus		sts;
  void			*p;
  gdb_sNode		*np;
  int                   size;
  cdh_uTypeId		cid;
  gdb_sClass		*cp;
  net_sSetObjectInfo	*mp = (net_sSetObjectInfo *)get->data;

  gdb_ScopeLock {
    
    np = (gdb_sNode *) hash_Search(NULL, gdbroot->nid_ht, &get->sender.nid);
    pwr_Assert(np != NULL);

  } gdb_ScopeUnlock;

  p = malloc( mp->aref.Size);

  if (p != NULL) {
    size = mp->aref.Size;
    cid.pwr = mp->aref.Body;
    cid.c.bix = 0;	/* To get the class id.  */
    cp = (gdb_sClass *) hash_Search(&sts, gdbroot->cid_ht, &cid.pwr);
    if (cp != NULL)    
      ndc_ConvertData(&sts, np, cp, &mp->aref, p, mp->info, (pwr_tUInt32 *)&size, 
		      ndc_eOp_decode, mp->aref.Offset, 0);
  }
  pvd_procom->m_getmsg = get;
  pvd_procom->m_provider->writeAttribute( pvd_procom, mp->aref.Objid.oix, mp->aref.Offset,
				mp->aref.Size, (char *)p);
}

static void
pvd_GetObjectInfo (
  qcom_sGet		*get
)
{
  net_sGetObjectInfo	*mp = (net_sGetObjectInfo *)get->data;

  pvd_procom->m_getmsg = get;
  pvd_procom->m_provider->readAttribute( pvd_procom, mp->aref.Objid.oix, mp->aref.Offset,
				mp->aref.Size);
}


static void
pvd_SubAdd (
  qcom_sGet		*get
)
{
  pwr_tInt32		i;
  pwr_tStatus		sts;
  net_sSubSpec		*specp;
  gdb_sNode		*np;
  net_sSubAdd		*mp = (net_sSubAdd *)get->data;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {

    np = (gdb_sNode *) hash_Search(&sts, gdbroot->nid_ht, &get->sender.nid);
    if (np == NULL) {
      errh_Error("subsm_Add, node (%s), is not known",
	cdh_NodeIdToString(NULL, get->sender.nid, 1, 0));
      break;
    }
  } gdb_ScopeUnlock;
 
  pvd_procom->m_getmsg = get;
  specp = &mp->spec[0];
  for (i=0; i < (int) mp->count; i++, specp++) {
    /* For all entries in the message.  */
    psub sub;

    sub.sid = specp->sid;
    sub.nid = np->nid;
    sub.sub_by_name = specp->sub_by_name;
    sub.aref = specp->aref;
    sub.dt = specp->dt;
    pvd_procom->m_provider->subAssociateBuffer( pvd_procom, &sub.p, sub.aref.Objid.oix, 
				      sub.aref.Offset, sub.aref.Size, sub.sid);
    subkey k(sub.sid);
    m_sublist[k] = sub;
  }

}

static void
pvd_SubRemove (
  qcom_sGet		*get
)
{
  net_sSubRemove	*mp = (net_sSubRemove *) get->data;
  pwr_tSubid 		*sp;

  pvd_procom->m_getmsg = get;
  sp = &mp->sid[0];
  for ( int i = 0; i < (int)mp->count; i++) {
    subkey k(*sp);
    sublist_iterator it = m_sublist.find( k);
    if ( it != m_sublist.end())
      m_sublist.erase( it);

    pvd_procom->m_provider->subDisassociateBuffer( pvd_procom, *sp);
    sp++;
  }
}

static pwr_tBoolean
pvd_SubSendBuffer ()
{
  pwr_tStatus		sts = 1;
  net_sSubMessage	*mp;
  net_sSubData		*dp;
  gdb_sNode		*np;
  qcom_sQid		tgt;
  pwr_tUInt32		size;
  pwr_tInt32		subdatahdrsize;
  gdb_sClass		*classp;
  cdh_uTypeId		cid;
  int 			asize;
  int 			totsize = 0;
  int			bufsubs_lc = 0;
  static int		buf_id = 1;
  pwr_tNodeId		subnid[100];
  int			subnid_cnt = 0;
  pwr_tNodeId		nid;

  /* Fill in buffer */

  // Get nodes  
  for ( sublist_iterator it = m_sublist.begin(); it != m_sublist.end(); it++) {
    int found = 0;
    for ( int j = 0; j < subnid_cnt; j++) {
      if ( subnid[j] == it->second.nid) {
	found = 1;
	break;
      }
    }
    if ( !found) 
      subnid[subnid_cnt++] = it->second.nid;
  }

  for ( int j = 0; j < subnid_cnt; j++) {
    nid = subnid[j];

    /* Build a buffer containing the update message */
    totsize = 0;
    bufsubs_lc = 0;
    for ( sublist_iterator it = m_sublist.begin(); it != m_sublist.end(); it++) {
      if ( it->second.nid == nid) {
	totsize += it->second.aref.Size;
	bufsubs_lc++;
      }
    }

    subdatahdrsize = sizeof(*dp) - sizeof(dp->data);
    size =  totsize +  
      bufsubs_lc * subdatahdrsize +		/* size of data hdrs */
      sizeof(*mp) - sizeof(mp->subdata);		/* size of msg hdr */

    size = (size + 3) & ~3;   /* Size up to nearest multiple of 4.  */

    mp = (net_sSubMessage *) calloc(1, size);

    mp->count = 0;
    dp = (net_sSubData *)&mp->subdata;

    gdb_ScopeLock {
      np = (gdb_sNode *) hash_Search(NULL, gdbroot->nid_ht, &nid);
      if ( np == NULL) continue;

      for ( sublist_iterator it = m_sublist.begin(); it != m_sublist.end(); it++) {
	if ( it->second.nid != nid)
	  continue;
	dp->sid = it->second.sid;
	dp->size = it->second.aref.Size;
	dp->sts = GDH__SUCCESS;

	if (ODD(dp->sts)) {
	  asize = it->second.aref.Size;
	  cid.pwr = it->second.aref.Body;
	  cid.c.bix = 0;	/* To get the class id.  */
	  classp = (gdb_sClass *) hash_Search(&sts, gdbroot->cid_ht, &cid.pwr);
	  if (classp && it->second.p)
	    ndc_ConvertData(&dp->sts, np, classp, &it->second.aref, dp->data, 
			    it->second.p, (pwr_tUInt32 *)&asize, ndc_eOp_encode, 
			    it->second.aref.Offset, 0);
	  it->second.count++;
	  mp->count++;
	}

	/* Advance pointer to next sdata slot */
	dp = (net_sSubData *)((pwr_tInt32)dp + it->second.aref.Size + subdatahdrsize);
      }
    } gdb_ScopeUnlock;

    if ( !mp->count)
      continue;

    tgt.qix = net_cProcHandler;
    tgt.nid = nid;

    net_Put(&sts, &tgt, mp, net_eMsg_subData, buf_id++, size, pvd_procom->m_sid);

    free(mp);
  }
  return 1;
}

void rt_procom::provideObjects( pwr_tStatus sts, vector<procom_obj>& ovect)
{
  qcom_sPut		put;
  net_sObjectR		*rsp;
  net_sGobject		*gop;
  pwr_tUInt32		count = ovect.size();
  pwr_tUInt32		pcount;
  pwr_tUInt32		size;
  int			i;
  
  net_sGobject		g;
      
  /* The parents must be first in the message, so the receiver can link
     the cache objects. They must be in top-down-order. */

  pcount = 0;
  /* Build response message.  */

  size = sizeof(net_sObjectR) + count * sizeof(net_sGobject);
  rsp = (net_sObjectR *) net_Alloc(&sts, &put, size, net_eMsg_objectR);
  if (rsp == NULL) {
    printf("NETH: could not allocate pams buffer for Cache send response, sts: %d\n", sts);
    respondError(m_getmsg, g.oid, sts);
    return;
  }
  gop = &rsp->g[0];

  for ( i = 0; i < (int)ovect.size(); i++) {
    /* Copy target object.  */
    g.oid.vid = m_vid;
    g.oid.oix = ovect[i].oix;
    g.flags.m = 0;
    g.cid = ovect[i].cid;
    cdh_ObjName( &g.f.name, ovect[i].name);
    g.f.poid.oix = ovect[i].fthoix;
    if ( ovect[i].oix)
      g.f.poid.vid = m_vid;
    else
      g.f.poid.vid = 0;
    g.sib.flink = ovect[i].fwsoix;
    g.sib.blink = ovect[i].bwsoix;
    if ( ovect[i].fchoix) {
      g.soid.vid = m_vid;
      g.soid.oix = ovect[i].fchoix;
      g.flags.b.isParent = 1;
    }
    g.size = ovect[i].body_size;

    *gop++ = g;
  }

  if ( m_getmsg->type.s == (qcom_eStype)net_eMsg_oidToObject) {
    net_sOidToObject *mp = (net_sOidToObject *)m_getmsg->data;
    rsp->oid = mp->oid;
  }
  else 
    rsp->oid    = g.oid;

  rsp->sts    = GDH__SUCCESS;
  rsp->count  = count;

  if (!net_Reply(&sts, m_getmsg, &put, 0))
    errh_Bugcheck(sts, "net_Reply");
}

void rt_procom::provideAttr( pwr_tStatus sts, pwr_tOix oix, int bsize, void *buff)
{
  net_sGetObjectInfoR	*rmp;
  qcom_sPut		put;
  net_sGetObjectInfo	*mp = (net_sGetObjectInfo *)m_getmsg->data;
  gdb_sNode		*np;
  int                   size;
  cdh_uTypeId		cid;
  gdb_sClass		*cp;

  size = mp->aref.Size + sizeof(net_sGetObjectInfoR) - sizeof(rmp->info);
  size = (size + 3) & ~3;   /* Size up to nearest multiple of 4.  */
  rmp = (net_sGetObjectInfoR *)net_Alloc(&sts, &put, size, net_eMsg_getObjectInfoR);
  if (EVEN(sts)) return;

  gdb_ScopeLock {
    
    np = (gdb_sNode *) hash_Search(NULL, gdbroot->nid_ht, &m_getmsg->sender.nid);
    pwr_Assert(np != NULL);

  } gdb_ScopeUnlock;

  if (buff != NULL) {
    size = mp->aref.Size;
    cid.pwr = mp->aref.Body;
    cid.c.bix = 0;	/* To get the class id.  */
    cp = (gdb_sClass *) hash_Search(&sts, gdbroot->cid_ht, &cid.pwr);
    if (cp != NULL)    
      ndc_ConvertData(&sts, np, cp, &mp->aref, rmp->info, buff, (pwr_tUInt32 *)&size, 
		      ndc_eOp_encode, mp->aref.Offset, 0);
  }

  rmp->aref = mp->aref;
  rmp->sts  = sts;
  rmp->size = mp->aref.Size;

  net_Reply(&sts, m_getmsg, &put, 0);
}

void rt_procom::provideStatus( pwr_tStatus rsts)
{
  pwr_tStatus sts;

  switch( m_getmsg->type.s) {
  case net_eMsg_getObjectInfo: {
    qcom_sPut		put;
    net_sGetObjectInfoR *rmp;
    net_sGetObjectInfo	*mp = (net_sGetObjectInfo *)m_getmsg->data;
    
    int size = mp->aref.Size + sizeof(net_sGetObjectInfoR) - sizeof(rmp->info);
    size = (size + 3) & ~3;   /* Size up to nearest multiple of 4.  */
    rmp = (net_sGetObjectInfoR *)net_Alloc(&sts, &put, size, 
								net_eMsg_getObjectInfoR);
    if (EVEN(sts)) return;

    rmp->aref = mp->aref;
    rmp->sts  = rsts;
    rmp->size = mp->aref.Size;
    
    net_Reply(&sts, m_getmsg, &put, 0);
    break;
  }
  case net_eMsg_setObjectInfo: {
    qcom_sPut		put;
    net_sSetObjectInfo	*mp = (net_sSetObjectInfo *)m_getmsg->data;
    net_sSetObjectInfoR *rmp = (net_sSetObjectInfoR *) net_Alloc(&sts, 
			   &put, sizeof(*rmp), net_eMsg_setObjectInfoR);
    if (EVEN(sts)) return;

    rmp->aref = mp->aref;
    rmp->sts  = sts;

    net_Reply(&sts, m_getmsg, &put, 0);
    break;
  }
  case net_eMsg_oidToObject: {
    respondError( m_getmsg, pwr_cNObjid, sts);
    break;
  }
  default: 
    ;
  }
}

void rt_procom::flushNodes()
{

  gdb_ScopeLock {

    cvolcm_ExternVolumeFlush( gdbroot->my_node);

  } gdb_ScopeUnlock;

  // cvolcm_FlushNode(NULL, gdbroot->my_node);
  // cvolsm_FlushNode(NULL, gdbroot->my_node);
  // subc_ActivateList(&gdbroot->my_node->subc_lh, pwr_cNObjid);
  // subc_ActivateList(&gdbroot->no_node->subc_lh, pwr_cNObjid);
}

