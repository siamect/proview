/* rt_linksup.c -- Runtime environment - Link Watch

   PROVIEW/R
   Copyright (C) 1996,98 by Mandator AB.

   <Description>.  */

#if defined OS_ELN
# include $vaxelnc
#else
# include <stdlib.h>
# include <string.h>
#endif
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_lst.h"
#include "pwrb_c_nodelinksup.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_ini_event.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_qcom.h"
#include "rt_plc_timer.h"
#include "rt_plc_utl.h"
#include "rt_aproc.h"
#include "rt_pwr_msg.h"

/* In this version, only cTimerTimeScan is used.
   To save resources we only have one PAMS timer. */

#define cTimerTimeScan 1000	/* ms */
#define cTimerTimeDetect cTimerTimeScan

typedef enum {
  eListState__		= 0,
  eListState_Init	= 1,
  eListState_Scan	= 2,
  eListState_NoNodeLink	= 3,
  eListState_Wait	= 4,
  eListState_
} eListState;

typedef enum {
  eTimer_ScanMessage  = 1
} eTimer;

typedef struct s_Node sNode;

LstType(sNode);

struct s_Node {
  LstLink(sNode)	  node_l; 
  LstLink(sNode)	  timer_l; 
  pwr_tObjid		  oid;
  pwr_sClass_NodeLinkSup  node;
  pwr_sClass_NodeLinkSup  *o;
  gdh_tDlid		  dlid;
  pwr_tBoolean		  found;
  plc_sTimer		  *timer;
};

static eListState	list_state = eListState_Init;
static LstHead(sNode)	node_l;
static LstHead(sNode)	timer_l;

static void		detect (pwr_sClass_NodeLinkSup*, pwr_tBoolean, sNode*);
static void		event (qcom_sGet*);
static sNode		*init_node (pwr_tObjid, sNode*, pwr_tBoolean);
static pwr_tStatus	init_nodes ();
static sNode		*get_nodes (pwr_tObjid);
static void		reinit_nodes ();
static void		scan_nodes ();
static void		scan_timers ();
static void		update_node (sNode*);


int main (int argc, char ** argv)
{
  pwr_tStatus	sts;
  qcom_sQid	my_q = qcom_cNQid;
  qcom_sGet	get;

  errh_Init("pwr_linksup", errh_eAnix_linksup);
  errh_SetStatus( PWR__SRVSTARTUP);

  if (!qcom_Init(&sts, NULL, "pwr_linksup")) {
    errh_Error("qcom_Init, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  sts = gdh_Init("pwr_linksup");
  if (EVEN(sts)) {
    errh_Fatal("gdh_Init, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  if (!qcom_CreateQ(&sts, &my_q, NULL, "events")) {
    errh_Fatal("qcom_CreateQ, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }
  if (!qcom_Bind(&sts, &my_q, &qcom_cQini)) {
    errh_Fatal("qcom_Bind(Qini), %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(-1);
  }

  /* Wait for local nethandler to start */

  while (EVEN(gdh_NethandlerRunning()))
    sleep(1);   

  plc_UtlWaitForPlc();

  LstIni(&node_l);
  LstIni(&timer_l);

  init_nodes();
  if (!LstEmp(&node_l)) {
    list_state = eListState_Scan;
  } else {
    errh_Info("No nodes to supervise, exiting");
    errh_SetStatus( pwr_cNStatus);
    exit(0);
  }

  errh_SetStatus( PWR__SRUN);

  for (;;) {
    scan_timers();
    scan_nodes();
    get.data = NULL;
    if (qcom_Get(&sts, &my_q, &get, cTimerTimeScan) != NULL) {
      if (get.type.b == qcom_eBtype_event) {
	event(&get);
      } else {
	errh_Info("unexpected message type, type: %d", get.type.b);
      }
      qcom_Free(&sts, get.data);
    }
    aproc_TimeStamp();
  }
}

/* Detect a NodeLink object.
   NOTE: This should be the same as the macro DSup_exec
   for DSup used by PLC, in rt_plc_macro_sup.h  */

static void detect (
  pwr_sClass_NodeLinkSup *o,
  pwr_tBoolean con,
  sNode *np
)
{

  /* For DSup we compare with control position, but for NodeLink
     we only detect LinkDown  */

  if (o->LinkUp) {
    if (o->Action) o->Action = FALSE;
    if (o->ReturnCheck) {
      clock_gettime(CLOCK_REALTIME, &o->ReturnTime);
      o->ReturnCheck = FALSE;
      o->ReturnSend = TRUE;
    }
    if (o->AlarmCheck && !o->DetectCheck) {
      o->TimerFlag = FALSE;
      o->DetectCheck = TRUE;
    }
  } else if (con) {
    if (!o->Action) o->Action = TRUE;
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {
      if (o->DetectCheck) {

	o->TimerCount = (o->TimerTime * 1000) / cTimerTimeDetect;
	if (!o->TimerFlag && o->TimerCount > 0) {
	  if (!LstInl(&np->timer_l)) {
	    LstIns(&timer_l, np, timer_l);
	  }
	  o->TimerFlag = TRUE;
	}
	clock_gettime(CLOCK_REALTIME, &o->DetectTime);
	o->DetectCheck = FALSE;
      }
      if (!o->TimerFlag) {
	o->DetectSend = TRUE;
	o->ReturnCheck = TRUE;
	o->Acked = FALSE;
	o->AlarmCheck = FALSE;
      }
    }
  }
}

static void
event (
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
    reinit_nodes();
    if (!LstEmp(&node_l)) {
      list_state = eListState_Scan;
      scan_timers();
      scan_nodes();
    } else {
      list_state = eListState_NoNodeLink;
      errh_Info("No NodeLink objects.");
    }   
  } else if (new_event.b.swapInit & !cur_event.b.swapInit) {
    list_state = eListState_Wait;
    errh_Info("Warm restart initiated.");   
  } else if (new_event.b.terminate & !cur_event.b.terminate) {
    exit(0);
  }

  sav_event = ep->mask;
}

/* Initiates a sNode.  */

static sNode *
init_node (
  pwr_tObjid oid,
  sNode *np,
  pwr_tBoolean new_sub
)
{
  pwr_tStatus		 sts;
  pwr_sAttrRef		 aref;
  pwr_sClass_NodeLinkSup *o;
  gdh_tDlid		 dlid = pwr_cNDlid;
  pwr_tBoolean		 is_alias = 0;
  cdh_uTypeId		 tid;

  sts = gdh_IsAlias(oid, &is_alias);
  if (is_alias)
    return NULL;

  /* Allocate and initiate NodeLink control block */
  if (np == NULL) {
    tid.pwr = pwr_cClass_NodeLinkSup;
    tid.c.bix = 1;
    aref.Objid = oid;
    aref.Offset = 0;
    aref.Size = sizeof(pwr_sClass_NodeLinkSup);
    aref.Body = tid.pwr;
    aref.Flags.m = 0;	    
    sts = gdh_DLRefObjectInfoAttrref(&aref, (pwr_tAddress *)&o, &dlid);
    if (EVEN(sts)) {
      errh_Error("Couldn't get direct link to NodeLink object, %m", sts);
      return NULL;
    }
    np = (sNode*) calloc(1, sizeof(sNode));
    if (np == NULL) {
      if (cdh_DlidIsNotNull (dlid))
	gdh_DLUnrefObjectInfo (dlid);
      errh_Error("Error calloc, sNode");
      return NULL;
    }
    memcpy (&np->node, o, sizeof (np->node)); 
    np->dlid = dlid;
    np->o = o;
    np->oid = oid;
    np->timer = (void *)&o->TimerFlag;
  }

  /* Setup subscription to supervised Node object's attribute CurVersion. */

  if (new_sub) {
    int dt;
    int tmo;

    dt = MAX(1, (int)(o->SubscriptionInterval * 1000));
    tmo = MAX(2 * dt, 10000); /* 10 s */
    sts = gdh_SetSubscriptionDefaults (dt, tmo);

    tid.pwr = pwr_eClass_Node;
    tid.c.bix = 1;
    
    aref.Objid = o->Node;
    sts = gdh_ClassAttrToAttrref(pwr_eClass_Node, ".CurVersion", &aref);
    sts = gdh_SubRefObjectInfoAttrref(&aref, &o->SubId);
    if ( EVEN(sts))
      errh_Error("Couldn't get link to Node object, %m", sts);
  }

  return np;
}

/* This routine initiates the NodeLink list.  */

static pwr_tStatus
init_nodes ()
{
  pwr_tStatus sts;
  LstLink(sNode) *nl;
  sNode *np;
  pwr_tObjid oid;

  nl = LstEnd(&node_l);

  for (
    sts = gdh_GetClassList(pwr_cClass_NodeLinkSup, &oid);
    ODD(sts);
    sts = gdh_GetNextObject(oid, &oid)
  ) {
    np = init_node(oid, NULL, 1);
    if (np != NULL) {
      nl = LstIns(nl, np, node_l);
    }
  }

  if (LstEmp(&node_l))
    errh_Info("No NodeLink objects");

  return(sts);
}

/* This routine search nodelink list for a nodelink object.  */

static sNode *
get_nodes (
  pwr_tObjid oid
)
{
  LstLink(sNode) *nl;

  for (nl = LstFir(&node_l); nl != LstEnd(&node_l); nl = LstNex(nl)) {
    if (cdh_ObjidIsEqual(LstObj(nl)->oid, oid)) 
      return LstObj(nl);
  }

  return NULL;
}

static void
reinit_nodes ()
{
  pwr_tStatus sts;
  LstLink(sNode) *nl;
  sNode *np;
  pwr_tObjid oid;

  /* Mark all links in the NodeLink list */
  for (nl= LstFir(&node_l); nl != LstEnd(&node_l); nl = LstNex(nl))
    LstObj(nl)->found = FALSE;

  for (
    sts = gdh_GetClassList(pwr_cClass_NodeLinkSup, &oid);
    ODD(sts);
    sts = gdh_GetNextObject(oid, &oid)
  ) {
    if ((np = get_nodes(oid)) == NULL) {
      np = init_node(oid, NULL, 1);
      if (np != NULL) {
	nl = LstIns(nl, np, node_l);
	np->found = TRUE;
      }
    } else {
      update_node(np);
    }
  }

  for (nl = LstFir(&node_l); nl != LstEnd(&node_l); nl = LstNex(nl)) {
    np = LstObj(nl);
    if (!np->found) {
      nl = LstPre(&np->node_l);
      LstRem(&np->node_l);
      LstNul(&np->node_l);
      gdh_SubUnrefObjectInfo (np->o->SubId);
      gdh_DLUnrefObjectInfo(np->dlid);
      free(np);
    }
  }	    
}

static void
scan_nodes ()
{
  LstLink(sNode) *nl;
  pwr_tStatus sts;
  pwr_tBoolean Old;
  pwr_tTime LastUpdate, Timeout, CurrentTime;
  pwr_tDeltaTime Delta;
  pwr_tBoolean LinkUp;

  clock_gettime(CLOCK_REALTIME, &CurrentTime);

  for (nl = LstFir(&node_l); nl != LstEnd(&node_l); nl = LstNex(nl)) {
    sNode *np = LstObj(nl);
    pwr_sClass_NodeLinkSup  *o = np->o;
    LinkUp = 0;
    sts = gdh_GetSubscriptionOldness (o->SubId, &Old, &LastUpdate, NULL);
    if (ODD(sts)) {
      /* IF (CurrentTime < LastUpdate + TimeoutTime) THEN LinkUp */
      Delta.tv_sec = o->TimeoutTime;
      Delta.tv_nsec = 0;
      time_Aadd(&Timeout, &LastUpdate, &Delta);
      if (time_Acomp(&CurrentTime, &Timeout) < 0) 
	LinkUp = 1;
    }

    if (o->LinkUp && !LinkUp) {
      o->LinkUp = 0;
      o->DownTime = CurrentTime;
    } else if (!o->LinkUp && LinkUp) {
      o->LinkUp = 1;
      o->UpTime = CurrentTime;
      o->UpCount++;
    }

    detect(o, 1, np);
  }
}

static void
scan_timers ()
{
  LstLink(sNode) *nl, *nxtnl;
  plc_sTimer *tp;

  for (nl = LstFir(&timer_l); nl != LstEnd(&timer_l); nl = nxtnl) {
    tp = LstObj(nl)->timer;
    nxtnl = LstNex(nl);
    if (tp->TimerCount <= 1 || !tp->TimerFlag) {
      tp->TimerCount = 0;
      tp->TimerFlag = FALSE;
      LstRem(nl);
      LstNul(nl);
    } else {
      tp->TimerCount--;
    }
  }
}

static void
update_node (
  sNode *np
)
{
  pwr_tBoolean new_sub;

  /* Check that the same Node object is supervised   */
  if (cdh_ObjidIsEqual(np->node.Node, np->o->Node) &&
      np->node.SubscriptionInterval == np->o->SubscriptionInterval
  ) {   
    new_sub = FALSE;
  } else {
    gdh_SubUnrefObjectInfo (np->o->SubId);
    np->o->SubId = pwr_cNSubid;
    new_sub = TRUE;
  }

  np = init_node(np->oid, np, new_sub);
  np->found = TRUE;
}
