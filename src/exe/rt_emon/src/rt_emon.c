/* 
 * Proview   $Id: rt_emon.c,v 1.12 2007-05-24 14:50:13 claes Exp $
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

/* rt_eventmonitor.c -- Runtime environment - Message Handler */

#ifndef OS_ELN
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <math.h>
#endif

#if defined OS_VMS
# include <starlet.h>
# include <lib$routines.h>
# include <signal.h>
# include <processes.h>
# include <descrip.h>
#elif defined OS_ELN
# include ctype
# include stdio
# include stdlib
# include string
# include $vaxelnc
# include $kernelmsg
# include $dda_utility
# include $elnmsg
# include math
#endif

#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_tree.h"
#include "rt_ini_event.h"
#include "rt_mh_def.h"
#include "rt_mh_net.h"
#include "rt_mh_util.h"
#include "rt_mh_outunit.h"
#include "rt_mh_blockdb.h"
#include "rt_mh_log.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "co_syi.h"
#include "rt_aproc.h"
#include "rt_pwr_msg.h"
#include "rt_c_node.h"

/* Local defines */

#define cTimerOpenBlock	  600.
#define cTimerOutunitSync 5.
#define cNodes		  256
#define cTimers		  4

#define cBlockDbIdx  (eTimer_BlockDbOpen - 1)
#define cDetectIdx   (eTimer_ScanDetect - 1)
#define cMessageIdx  (eTimer_ScanMessage - 1)
#define cOutunitIdx  (eTimer_OutunitSync - 1)

/* Local types */

typedef struct s_Active sActive;
typedef struct s_Appl sAppl;
typedef struct s_ApplActive sApplActive;
typedef struct s_Block sBlock;
typedef struct s_Event sEvent;
typedef struct s_EventTab sEventTab;
typedef struct s_SupActive sSupActive;
typedef struct s_EventList sEventList;
typedef struct s_NodeInfo sNodeInfo;
typedef struct s_Outunit sOutunit;
typedef struct s_ProcLink sProcLink;
typedef struct s_SaveBlock sSaveBlock;
typedef struct s_Sup sSup;
typedef struct s_Timer sTimer;	
typedef struct s_ASup sASup;
typedef struct s_DSup sDSup;
typedef union u_Event uEvent;

LstType(sActive);
LstType(sApplActive);
LstType(sAppl);
LstType(sBlock);
LstType(sOutunit);
LstType(sProcLink);
LstType(sSupActive);

struct s_ProcLink {
  LstLink(sProcLink)	proc_l;
  mh_eSource		source;
  pwr_tNodeIndex	nix;
  qcom_sQid		qid;
  qcom_sAid		aid;
  co_sPlatform		platform;
};

struct s_Appl {
  sProcLink		link;		/* Link in process list */
  LstLink(sAppl)	appl_l;		/* Link in application list */
  LstHead(sApplActive)	active_l;	/* Head of application alarm list */
  pwr_tUInt32		activeMessages;	/* Active messages */
  mh_eApplState		state;
  mh_eApplState		oldState;
  pwr_tObjid		aid;
  pwr_tTime		birthTime;
};

struct s_Active {
  LstLink(sActive)	active_l;	/* Link in active list */
  LstLink(sActive)	activeIdx_l;	/* Link in active index list */
  pwr_tUInt32		idx;		/* Event index of alarm */
  pwr_tUInt32		returnIdx;	/* Event index of return message */
  pwr_tUInt32		ackIdx;		/* Event index of ack message */
  mh_eSource		source;		/* Originator of alarm */
  pwr_tObjid		object;		/* Object associated with alarm */
  pwr_tObjid		supObject;	/* Supervisor object */
  pwr_tObjid		outunit;	/* Optional outunit destination */
  pwr_tOName		objName;
  pwr_tOName		eventName;
  mh_mEventFlags	eventFlags;
  mh_uEventInfo		status;
  mh_eEvent		event;
  pwr_tAttrRef		eventSound;
  pwr_tBoolean		local;
  sEventTab		*detect_etp;
  sEventTab		*return_etp;
  sEventTab		*ack_etp;
};


struct s_NodeInfo {
  sAppl		    appl;
  sApplActive	    *hp;
  gdh_sNodeInfo	    gdh;
  gdh_eLinkState    oldLinkState; 
  gdh_eLinkState    newLinkState; 
  pwr_tString40	    name;
  pwr_tString80	    fullname;
  pwr_tBoolean	    initiated;
  pwr_tBoolean	    check;
  pwr_tBoolean	    checkInit;
};

struct s_Block {
  sActive		link;
  LstLink(sBlock)	block_l;
  mh_sOutunitBlock	outunitBlock;
  mh_sOutunitBlock	outunitUnblock;
  mh_sEventId		targetId;
};

union u_Event {
  mh_sMsgInfo	  info;
  mh_sMessage	  message;
  mh_sAck	  ack;
  mh_sBlock	  block;
  mh_sReturn	  ret;
};

struct s_Event {
  pwr_tUInt32	  idx;    
  LstHead(sActive)	activeIdx_l;
  pwr_tObjid	  outunit;
  pwr_tObjid	  object;
  pwr_tOName	  objName;
  mh_eEvent	  event;
  pwr_tUInt32	  msgSize;
  pwr_tBoolean	  local;
  sEventTab	  *etp;
  uEvent	  msg;
};

struct s_EventTab {
  tree_sNode	  node;
  pwr_tUInt32	  idx;    
  mh_eEvent	  event;
  sActive	  *ap;
  sEvent	  *ep;
};

struct s_EventList {
  pwr_tUInt32	idx;
  pwr_tUInt32	oldIdx;
  pwr_tUInt32	size;
  sEvent	list[1];
  /* Here comes the event list */
};

struct s_SaveBlock {
  mh_sOutunitBlock outunitBlock;
  mh_sEventId targetId;
};

struct s_Timer {	
  pwr_tBoolean          TimerFlag;
/*  struct sTimer	*TimerNext; */
  pwr_tBoolean          *TimerNext;
  pwr_tUInt32           TimerCount;
  pwr_tBoolean          *TimerDO;
  pwr_tFloat32          TimerTime;
  pwr_tBoolean          TimerDODum;
  pwr_tObjid            TimerObjDId;
  pwr_tInt32            TimerAcc;
  pwr_tFloat32          TimerMin;
  pwr_tFloat32          TimerMax;
};

struct s_Sup {
  pwr_tBoolean    *ControlP;
  pwr_tBoolean    Control;
  pwr_tBoolean    Action;
  pwr_tBoolean    Acked;
  pwr_tBoolean    Blocked;
  pwr_tString80	  Description;
  pwr_tBoolean    DetectOn;
  pwr_tString80   DetectText;
  pwr_tString80   ReturnText; 
  mh_eEvent	  EventType;
  mh_eEventPrio   EventPriority;
  mh_mEventFlags  EventFlags;
  pwr_tAttrRef	  Sound;
  /* Internal attributes */
  pwr_sAttrRef    Attribute;
  mh_uEventInfo   AlarmStatus;
  pwr_tBoolean    AlarmCheck;
  pwr_tBoolean    DetectCheck;
  pwr_tBoolean    DetectSend;
  pwr_tTime	  DetectTime;
  pwr_tBoolean    ReturnCheck;
  pwr_tBoolean    ReturnSend;
  pwr_tTime	  ReturnTime;
  pwr_tTime	  AckTime;
  pwr_tObjid	  AckOutunit;
};

struct s_SupActive {
  sActive	  link;
  LstLink(sSupActive) sup_l;
  LstLink(sSupActive) detect_l;
  LstLink(sSupActive) timer_l;
  mh_eAgent	  agent;
  mh_uEventInfo	  alarmVisibility;
  mh_eSupType	  supType;
  gdh_tDlid	  supDlid;
  pwr_sAttrRef    attribute;
  gdh_tDlid	  attrDlid;
  void		  *actualValue;
  pwr_tBoolean    found;
  sSup		  *sup;
  pwr_tUInt32	  supInfoSize;
  void		  *supInfoP;
  void		  *op;
  sTimer	  *timer;
  void		  (*detect_exec)(sSupActive *sp);
};

struct s_ASup {
  pwr_tFloat32    *InP;
  pwr_tFloat32    In;
  sSup	  Sup;
  mh_sASupInfo    Info;
  sTimer	  Timer;
};

struct s_DSup {
  pwr_tBoolean    *InP;
  pwr_tBoolean    In;
  sSup	  Sup;
  mh_sDSupInfo    Info;
  sTimer	  Timer;
};

struct s_ApplActive {
  sActive		    link;
  LstLink(sApplActive)	    active_l;
  mh_sApplMessage	    message;
  pwr_tTime		    ackTime;
  pwr_tObjid		    ackOutunit;
  pwr_tTime		    returnTime;
  pwr_tString80		    returnText;
  mh_eEvent		    returnType;	    /* Return or Cancel */
  sAppl			    *ap;	    /* Pointer to owner appl */
};

struct s_Outunit {
  sProcLink		link;		/* Link in process list */
  LstLink(sOutunit)	outunit_l;	/* Link in outunit list */
  pwr_tTime		birthTime;	/* Time when outunit was started */
  pwr_tObjid		outunit;	/* Object id of outunit object */
  mh_eOutunitType	type;		/* Type of outunit */
  pwr_tUInt32		ackGen;		/* Generation of last ack received from outunit */
  pwr_tUInt32		blockGen;	/* Generation of last block received from outunit */
  pwr_tUInt32		eventIdx;	/* Index of last sent event to outunit */
  pwr_tUInt32		eventGen;	/* Generation of eventlist when last sent to outunit */
  pwr_tUInt32		maxIdx;		/*  */
  pwr_tUInt32		syncedIdx;	/*  */
  pwr_tBoolean		check;
  pwr_tBoolean		linkUp;
  pwr_tUInt32		selGen;		/* Index of last received select list from outunit */
  pwr_tUInt32		selSize;	/* Size of last received select list from outunit */
  mh_sSelL		sel_l[mh_cSelLSize];
};


typedef enum {
  eSupListState__	  = 0,
  eSupListState_Init      = 1,
  eSupListState_Scan      = 2,
  eSupListState_NoSup     = 3,
  eSupListState_Wait      = 4,
  eSupListState_Restart   = 5,
  eSupListState_
} eSupListState;

typedef enum {
  eTimer_BlockDbOpen  = 1,
  eTimer_ScanDetect   = 2,
  eTimer_ScanMessage  = 3,
  eTimer_OutunitSync  = 4
} eTimer;

typedef enum {
  eHEvent_None       = 0,
  eHEvent_LinkStart,
  eHEvent_LinkUp,
  eHEvent_LinkDown,
  eHEvent_Booted,
  eHEvent_RestartInit,
  eHEvent_RestartComplete
} eHEvent;

typedef struct {
  eTimer	type;
  int		multiple;
  int		count;
  pwr_tBoolean	active;
} sTimerData;

struct sLocal {
  pwr_sClass_MessageHandler	*emon;
  pwr_tString80			emonName;
  pwr_tObjid			emonObject;
  LstHead(sOutunit)		outunit_l;
  mh_sHead			head;
  LstHead(sActive)		active_l;
  sEventList			*event_l;
  LstHead(sProcLink)		proc_l;
  LstHead(sAppl)		appl_l;
  LstHead(sSupActive)		sup_l;
  LstHead(sBlock)		block_l;
  LstHead(sBlock)		blockFree_l;
  LstHead(sSupActive)		detect_l;
  LstHead(sSupActive)		timer_l;
  LstHead(sApplActive)		handlerFree_l;
  eSupListState			supListState; 
  tree_sTable			*eventTab;
  pwr_tBoolean			newBlock;
  pwr_tFloat32			detectTimerTime;
  sTimerData			timers[cTimers];
  double			timerTime;
  pwr_tDeltaTime		timerTimeDelta;
  pwr_tBoolean			timerActive;
  sSaveBlock			*blockSave;
  void				*blockDb;
  pwr_tUInt32			blockSaveSize;
  char				blockDbName[256];
  sNodeInfo			nodeDb[cNodes];
  pwr_tBoolean			outunitServer;
  pwr_sClass_IOHandler		*iohp;
}; 

/* Global variables */

/* Local variables */

static char		cText_Restart[] = "Canceled due to restart";
static char		cText_Block[] = "Canceled due to blocking";
static struct sLocal	l;


/* Local function definitions */

static sActive		*activeListGet(pwr_tUInt32);
static void		activeListInsert(sActive*, sEvent*, mh_eSource);
static void		activeListRemove(sActive*);
static void		applConnect(mh_sHead*, sAppl*, mh_uApplReply*);
static void		applDisconnect(mh_sHead*, sAppl*, mh_uApplReply*);
static void		applGetMsgInfo(mh_sHead*, sAppl*, mh_uApplReply*);
static void		applLogState(sAppl*);
static void		applMessage(mh_sHead*, sAppl*, mh_uApplReply*);
static void		applReturn(mh_sHead*, sAppl*, mh_uApplReply*);
static void		aSup_exec(sSupActive*);
static sBlock		*blockListAlloc();
static void		blockListFree(sBlock*);
static void		cancelAlarm(sActive*, char*);
static void		checkOutunits();
static void		cSup_exec(sSupActive*);
static void		dSup_exec(sSupActive*);
static void		enableQcomEvents(qcom_sQid);
static void		enableQcomAllHandlers(qcom_sQid);
static void		eventListInit();
static sEvent		*eventListInsert(mh_eEvent, char*, sActive*);
static void		eventToOutunits(sEvent*);
static void		formatApplEvent(mh_eEvent, char*, sApplActive*, uEvent*, pwr_tUInt32*);
static void		formatOutunitEvent(mh_eEvent, char*, sBlock*, uEvent*, pwr_tUInt32*);
static void		formatSupEvent(mh_eEvent, char*, sSupActive*, uEvent*, pwr_tUInt32*);
static void		fromApplication(qcom_sGet*);
static void		fromHandler(qcom_sGet*);
static void		fromMessageUnit(qcom_sGet*);
static void		fromOutunit(qcom_sGet*);
static void		fromQcom(qcom_sGet*);
static void		fromEvent(qcom_sGet*);
static mh_eAgent	getAgent(sSupActive*);
static void		handleAlarm(sSupActive*);
static void		handleInfo(sSupActive*);
static void		handleMessage(qcom_sGet*);
static void		handleReturn(sSupActive*);
static void		handlerEvent(eHEvent, pwr_tNodeIndex);
static sApplActive	*handlerListAlloc();
static void		handlerListFree(sApplActive*);
static void		getHandlerObject();
static void		initBlockList();
static void		initNodeDb();
static pwr_tStatus	initSupActiveCB(pwr_tObjid, pwr_tClassId, sSupActive**, pwr_tBoolean, pwr_tBoolean);
static pwr_tStatus	initSupList();
static pwr_tStatus	initSupListClass(pwr_tClassId);
static pwr_tBoolean	isForOutunit(sOutunit*, pwr_tObjid, pwr_tObjid, pwr_tString80, mh_mEventFlags, pwr_tBoolean);
static pwr_tBoolean	isValidApplication(mh_sHead*, qcom_sAid*, sAppl**, mh_uApplReply*);
static pwr_tBoolean	isValidOutunit(mh_sHead*, qcom_sAid*, sOutunit**);
static void		linkActive(qcom_sGet*);
static void		linkConnect(qcom_sGet*);
static void		linkDisconnect(qcom_sGet*);
static void		linkStalled(qcom_sGet*);
static void		outunitAborted(sOutunit*);
static void		outunitAck(mh_sHead*, sOutunit*);
static void		outunitBlock(mh_sHead*, sOutunit*);
static void		outunitInfo(mh_sHead*, sOutunit*);
static void		outunitDisconnect(mh_sHead*, sOutunit*);
static void		outunitLog(sOutunit*, char*);
static void		outunitSync(mh_sHead *, sOutunit*);
static void		procDown(qcom_sAid*);
static void		receive(qcom_sQid);
static void		reInitSupList();
static pwr_tStatus	reInitSupListClass(pwr_tClassId);
static pwr_tBoolean	reSendEventToOutunit(sOutunit*, sEventTab*);
static void		saveBlockList();
static void		scanDetectList();
static void		scanSupList();
static void		scanTimerList();
static void		sendEventListToOutunit(sOutunit*);
static pwr_tBoolean	sendEventToOutunit(sOutunit*, sEventTab*);
static pwr_tStatus	sendMessage(mh_eMsg, qcom_sQid*, co_sPlatform*, void*, pwr_tUInt32);
static pwr_tStatus	sendToOutunit(sOutunit*, mh_eMsg, int, void*, int);
static void		setTimerActive(int, pwr_tBoolean);
static sSupActive	*supListGet(pwr_tObjid);
static void		timeOut();
static void		timerIn(sSupActive*, sTimer*);
static void		updateAlarm(sActive*, sEvent*);
static void		updateAlarmInfo(sActive*);
static void		updateSupActive(sSupActive*);



int main ()
{
  pwr_tStatus sts;
  qcom_sQid myQid = {mh_cProcHandler, 0};
  qcom_sNode myNode;
  qcom_sQattr qAttr;

  errh_Init("pwr_emon", errh_eAnix_emon);
  errh_SetStatus( PWR__SRVSTARTUP);

  if (!qcom_Init(&sts, 0, "pwr_emon")) {
    errh_Fatal("qcom_Init, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  } 

  qAttr.type = qcom_eQtype_private;
  qAttr.quota = 100;
  if (!qcom_CreateQ(&sts, &myQid, &qAttr, "pwr_emon")) {
    if (sts == QCOM__QALLREXIST) {
      if (!qcom_AttachQ(&sts, &myQid)) {
        if (!qcom_DeleteQ(&sts, &myQid)) {
          errh_Fatal("qcom_DeleteQ, %m", sts);
          exit(sts);
        }
        if (!qcom_CreateQ(&sts, &myQid, &qAttr, "pwr_emon")) {
          errh_Fatal("qcom_CreateQ, %m", sts);
          exit(sts);
	}
      }
    }
    else {
      errh_Fatal("qcom_CreateQ, %m", sts);
      errh_SetStatus( PWR__SRVTERM);
      exit(sts);
    }
  } 
  l.head.qid = myQid;

  if (!qcom_MyNode(&sts, &myNode)) {
    errh_Fatal("qcom_MyNode, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  } 
  l.head.qid = myQid;

  l.head.platform.os = myNode.os;
  l.head.platform.hw = myNode.hw;

  sts = gdh_Init("pwr_emon");
  if (EVEN(sts)) {
    errh_Error("gdh_Init, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }


  /* Wait for local nethandler to start */

  while(EVEN(gdh_NethandlerRunning()))
    sleep(1);   

  l.newBlock = TRUE;
  l.detectTimerTime = 1.0;
  l.timerActive = FALSE;

  l.blockSave = NULL;
  l.blockDb = NULL;
  l.outunitServer = FALSE;

  l.eventTab = tree_CreateTable(&sts, sizeof(pwr_tUInt32), offsetof(sEventTab, idx),
    sizeof(sEventTab), 200, tree_Comp_uint32);

  /* Init Handler head */

  l.head.ver = mh_cVersion;
  clock_gettime(CLOCK_REALTIME, &l.head.birthTime);
  l.head.source = mh_eSource_Handler;

  l.head.nix = l.head.qid.nid;
  enableQcomEvents(myQid);
  enableQcomAllHandlers(myQid);

  getHandlerObject();

  eventListInit();

  LstIni(&l.outunit_l);
  LstIni(&l.active_l);
  LstIni(&l.proc_l);
  LstIni(&l.appl_l);
  LstIni(&l.sup_l);
  LstIni(&l.block_l);
  LstIni(&l.blockFree_l);
  LstIni(&l.detect_l);
  LstIni(&l.timer_l);
  LstIni(&l.handlerFree_l);

  l.supListState = eSupListState_Init; 

  if (l.emon->BlockFile) {
    initBlockList();
    setTimerActive(cBlockDbIdx, TRUE);
  }

  initNodeDb();

  sts = sendMessage(mh_eMsg_HandlerHello, NULL, NULL, NULL, 0);

  setTimerActive(cMessageIdx, TRUE);
  setTimerActive(cOutunitIdx, TRUE);

  mh_UtilWake();

  handlerEvent(eHEvent_Booted, 0);

  errh_SetStatus( PWR__SRUN);

  for(;;) {
    receive(myQid);
  }
}

static sActive *
activeListGet (
  pwr_tUInt32 idx
)
{
  LstLink(sActive) *al;
  LstHead(sActive) *ll;

  ll = (LstHead(sActive) *)&l.event_l->list[idx % l.event_l->size].activeIdx_l;

  for (al = (LstLink(sActive) *)LstFir(ll); al != LstEnd(ll); al = LstNex(al))
    if (idx == LstObj(al)->idx)
      return LstObj(al);

  return NULL;
}

/* Puts an active link block at the end of the active list and at the end
   of the event index list pointed to by ep.  */

static void
activeListInsert (
  sActive *ap,
  sEvent *ep,
  mh_eSource source
)
{
  LstLink(sActive) *al;
  LstLink(sBlock) *bl;

  ap->idx = ep->idx;
  ap->source = source;

  al = LstEnd(&l.active_l);
  LstIns(al, ap, active_l);

  al = LstEnd(&ep->activeIdx_l);
  LstIns(al, ap, activeIdx_l);

  switch (ap->event) {
  case  mh_eEvent_Alarm:
    ++l.emon->AlarmCount;
    break;
  case mh_eEvent_Block:
  case mh_eEvent_Reblock:
    bl = LstEnd(&l.block_l);
    LstIns(bl, (sBlock *)ap, block_l);
    ++l.emon->BlockCount;
    break;
  case  mh_eEvent_Info:
    break;
  default:
    errh_Error("activeListInsert, program error, event: %d", ap->event);
    break;
  }

}

/* Removes a active link block from the active list and event index list. */

static void
activeListRemove (
  sActive *ap
)
{
  pwr_tStatus sts;
  sApplActive *aap;
  sBlock *bp;

  LstRem(&ap->active_l);
  LstNul(&ap->active_l);
  LstRem(&ap->activeIdx_l);
  LstNul(&ap->activeIdx_l);
  ap->idx = 0;
  ap->ackIdx = 0;
  ap->returnIdx = 0;

  switch (ap->event) {
  case  mh_eEvent_Alarm:
    --l.emon->AlarmCount;
    break;
  case mh_eEvent_Block:
  case mh_eEvent_Unblock:
  case mh_eEvent_Reblock:
  case mh_eEvent_CancelBlock:
    bp = (sBlock *) ap;
    LstRem(&bp->block_l);
    LstNul(&bp->block_l);
    --l.emon->BlockCount;
    break;
  case  mh_eEvent_Info:
    break;
  default:
    errh_Error("activeListRemove, program error, event: %d", ap->event);
    break;
  }

  /* remove from event table */

  if (ap->detect_etp != NULL) {
    ap->detect_etp->ap = NULL;
    if (ap->detect_etp->ep == NULL)
      tree_Remove(&sts, l.eventTab, &ap->detect_etp->idx);
    ap->detect_etp = NULL;
  }
  if (ap->return_etp != NULL) {
    ap->return_etp->ap = NULL;
    if (ap->return_etp->ep == NULL)
      tree_Remove(&sts, l.eventTab, &ap->return_etp->idx);
    ap->return_etp = NULL;
  }
  if (ap->ack_etp != NULL) {
    ap->ack_etp->ap = NULL;
    if (ap->ack_etp->ep == NULL)
      tree_Remove(&sts, l.eventTab, &ap->ack_etp->idx);
    ap->ack_etp = NULL;
  }

  if (ap->source == mh_eSource_Application
    || ap->source == mh_eSource_Handler
  ) {
    aap = (sApplActive*) ap;

    LstRem(&aap->active_l); /* Remove from application alarm list */
    LstNul(&aap->active_l);
    --aap->ap->activeMessages;
    if (ap->source == mh_eSource_Application)
      free(ap);
    else
      handlerListFree((sApplActive*)ap);
  }

}

static void
applConnect (
  mh_sHead *hp,
  sAppl *ap,
  mh_uApplReply *reply
)
{
  switch (ap->state) {
  case mh_eApplState_New:
  case mh_eApplState_Restarted:
  case mh_eApplState_Aborted:
    reply->Connect.Sts = MH__SUCCESS;
    ap->state = mh_eApplState_Connected;
    break;
  case mh_eApplState_Connected:
    errh_Info("ApplConnect, this should not happen");
    reply->Connect.Sts = MH__EVENTERR;
    break;
  default:
    errh_Info("applConnect, program error, state: %d", ap->state);
    reply->Connect.Sts = MH__PROGERR;
    break;
  }

  applLogState(ap);
  reply->Connect.NoOfActMessages = ap->activeMessages;
}

static void
applDisconnect (
  mh_sHead *hp,
  sAppl *ap,
  mh_uApplReply *Reply
)
{

  ap->state = mh_eApplState_Disconnected;
  applLogState(ap);

  /* Cancel all active messages */

  LstRem(&ap->appl_l);      /* Remove from application list */
  LstNul(&ap->appl_l);
  LstRem(&ap->link.proc_l);    /* Remove from process link */
  LstNul(&ap->link.proc_l);
  free(ap);       /* Free control block */
  Reply->Sts = MH__SUCCESS;
}

static void
applLogState (
  sAppl *ap
)
{
  static char *StateText[mh_eApplState_] = {
    "NoState",
    "Aborted",
    "Connected",
    "Disconnected",
    "New",
    "Restarted",
  };
  char mess[256];

  if (ap->state == ap->oldState)
    return;

  sprintf(
    mess,
    "Link to application qid: %s, oid: %s, state: %s",
    qcom_QidToString(NULL, &ap->link.qid, 1),
    cdh_ObjidToString(NULL, ap->aid, 1),
    (ap->state < mh_eApplState_ ? StateText[ap->state] : "Progerror")
  );
  errh_Info(mess);

  ap->oldState = ap->state;
}

static void
applGetMsgInfo (
  mh_sHead *hp,
  sAppl *ap,
  mh_uApplReply *Reply
)
{
  pwr_tUInt32 *TargetIdxP = (pwr_tUInt32*) (hp + 1);
  sApplActive *aap;
  LstLink(sApplActive) *al;

  switch (ap->state) {
  case mh_eApplState_Connected:
    break;
  case mh_eApplState_New:
  case mh_eApplState_Restarted:
  case mh_eApplState_Aborted:
    Reply->Info.Sts = MH__EVENTERR;
    errh_Info("applGetMsgInfo, this should not happen");
    return;
  default:
    Reply->Info.Sts = MH__PROGERR;
    errh_Info("applGetMsgInfo, program error, state: %d", ap->state);
    return;
  }

  for (al = LstFir(&ap->active_l); al != LstEnd(&ap->active_l); al = LstNex(al))
    if (LstObj(al)->link.idx >= *TargetIdxP)
      break;

  if (al != LstEnd(&ap->active_l)) {
    aap = LstObj(al);
    Reply->Info.Sts = MH__SUCCESS;
    Reply->Info.Message = aap->message;
  } else
    Reply->Info.Sts = MH__NOMOREMSG;
}

static void
applMessage (
  mh_sHead *hp,
  sAppl *ap,
  mh_uApplReply *reply
)
{
  pwr_tStatus sts;
  mh_sApplMessage *ip = (mh_sApplMessage*) (hp + 1);
  sEvent *ep;
  sApplActive *aap;
  pwr_tOName ObjName;

  switch (ap->state) {
  case mh_eApplState_Connected:
    break;
  case mh_eApplState_New:
  case mh_eApplState_Restarted:
  case mh_eApplState_Aborted:
    reply->Message.Sts = MH__EVENTERR;
    errh_Info("applMessage, this should not happen");
    return;
  default:
    reply->Message.Sts = MH__PROGERR;
    errh_Info("applMessage, program error, state: %d", ap->state);
    return;
  }

  /* get object name from gdh */

  if (cdh_ObjidIsNotEqual(ip->Object, pwr_cNObjid)) {
    sts = gdh_ObjidToName(ip->Object, ObjName, sizeof(ObjName), cdh_mNName);
    if (EVEN(sts)) {
      reply->Message.Sts = MH__NOSUCHOBJ;
      return;
    }
  } else {
    ObjName[0] = '\0';
  }

  aap = (sApplActive *) calloc(1, sizeof(sApplActive));
  if (aap == NULL) {
    reply->Message.Sts = MH__NOSPACE;
    return;
  }

  aap->link.source = mh_eSource_Application;
  aap->link.object = ip->Object;
  aap->link.supObject = ip->SupObject;
  aap->link.outunit = ip->Outunit;
  strncpy(aap->link.objName, ObjName, sizeof(aap->link.objName));
  cdh_ToUpper(aap->link.objName, NULL);
  strncpy(aap->link.eventName, ip->EventName, sizeof(aap->link.eventName));
  aap->link.eventFlags = ip->EventFlags;
  aap->link.eventSound = ip->EventSound;
  aap->link.event = ip->EventType;

  aap->message = *ip;

  if (ip->EventType == mh_eEvent_Info &&
    !(ip->EventFlags & mh_mEventFlags_InfoWindow)
  ) {
    ep = eventListInsert(ip->EventType, NULL, (sActive*) aap);
    free(aap);
  } else {
    if (ap->activeMessages > l.emon->MaxApplAlarms) {
      reply->Message.Sts = MH__APPLQUOTA;
      free(aap);
      return;
    }

   /* Insert in application alarm list */
    (void)LstIns(&ap->active_l, aap, active_l); 
    aap->ap = ap;

    ++ap->activeMessages;

    ep = eventListInsert(ip->EventType, NULL, (sActive*) aap);
    activeListInsert((sActive*) aap, ep, mh_eSource_Application);
    updateAlarm((sActive *) aap, ep);
  }

  reply->Message.Sts = MH__SUCCESS;
  reply->Message.Idx = ep->idx;
  eventToOutunits(ep);
}

static pwr_tStatus
applReply (
  qcom_sGet	*get,
  void		*msg,
  pwr_tUInt32	msgSize
)
{
  pwr_tStatus	sts;
  mh_sHead	*hp;
  unsigned int  size = sizeof(l.head) + msgSize;
  qcom_sPut	put;

  if ((put.data = (char*)qcom_Alloc(&sts, size)) == NULL) {
    errh_Error("applReply: qcom_alloc, %m", sts);
    return sts;
  }

  hp = (mh_sHead*) put.data;
  *hp = l.head;
  hp->type     = mh_eMsg_ApplReply;

  memcpy(hp + 1, msg, msgSize);

  put.type.b = mh_cMsgClass;
  put.type.s = mh_eMsg_ApplReply;
  put.reply  = l.head.qid;
  put.size   = size;

  qcom_Reply(&sts, get, &put);
  if (EVEN(sts)) {
    errh_Error("applReply, qcom_Reply\n%m", sts);
    qcom_Free(NULL, put.data);
  }

  return sts;
}

static void
applReturn (
  mh_sHead *hp,
  sAppl *ApplP,
  mh_uApplReply *reply
)
{
  sActive *ap;
  mh_sApplReturn *ip = (mh_sApplReturn*) (hp + 1);
  sEvent *ep;
  sApplActive *aap;

  switch (ApplP->state) {
  case mh_eApplState_Connected:
    break;
  case mh_eApplState_New:
  case mh_eApplState_Restarted:
  case mh_eApplState_Aborted:
    errh_Info("applReturn, this should not happen");
    reply->Connect.Sts = MH__EVENTERR;
    return;
  default:
    errh_Info("applReturn, program error, state: %d", ApplP->state);
    reply->Connect.Sts = MH__PROGERR;
    return;
  }

  if ((ap = activeListGet(ip->TargetIdx)) == NULL) {
    /* Alarm does not exist */
    errh_Info("applReturn, Got a return on a nonexisting alarm!");
    reply->Sts = MH__NONEXALARM;
    return;
  }

  if ((ap->status.Event.Status & mh_mEventStatus_NotRet) == 0) {
    /* Alarm already returned */
    reply->Sts = MH__ALLRET;
    return;
  }

  aap = (sApplActive*) ap;

  if (aap->ap != ApplP) {   /* Alarm not owned by this application */
    errh_Info("applReturn, Got a return on an alarm not owned by application!");
    reply->Sts = MH__NOTOWNED;
    return;
  }

  clock_gettime(CLOCK_REALTIME, &aap->returnTime);
  strncpy(aap->returnText, ip->ReturnText, sizeof(pwr_tString80));
  aap->returnType = ip->ReturnType;
  ep = eventListInsert(aap->returnType, NULL, ap);

  switch (aap->link.event) {
  case mh_eEvent_Alarm:
    updateAlarm(ap, ep);
    break;
  case mh_eEvent_Info:
    if ((aap->link.eventFlags & mh_mEventFlags_InfoWindow) != 0)
      updateAlarm(ap, ep);
    break;
  default:
    errh_Info("applReturn, Return on unknown event type: %d", aap->link.event);
    reply->Sts = MH__EVENTERR;
    return;
  }

  reply->Sts = MH__SUCCESS;
  eventToOutunits(ep);
}

static sBlock *
blockListAlloc ()
{
  LstLink(sBlock) *bl;
  LstHead(sBlock) *ll;
  sBlock *bp;
  const int alloc = 100;
  int i;

  ll = &l.blockFree_l;

  if (LstEmp(ll)) {
    bp = (sBlock *) calloc(alloc, sizeof(sBlock));
    if (bp != NULL) {
      for (i = 0; i < alloc; i++, bp++)
        LstIns(ll, bp, link.active_l);
      l.emon->FreeCount += alloc;
    } else {
      return NULL;
    }
  }

  bl = LstFir(ll);
  LstRem(bl);
  --l.emon->FreeCount;
  return LstObj(bl);
}

static void
blockListFree (
 sBlock *bp
)
{

  memset(bp, 0, sizeof(*bp));
  LstIns(&l.blockFree_l, bp, link.active_l);
  ++l.emon->FreeCount;
  return;
}

/* Detect an ASup object.

   !!! NOTE: This should be the same as the macro used by PLC,
	     in rt_plc_macro_sup.h  */

static void
aSup_exec (
  sSupActive *sp
)
{
  pwr_sClass_ASup *o = (pwr_sClass_ASup *) sp->op;
  pwr_tFloat32 In = *(pwr_tFloat32 *) sp->actualValue;

  if ((o->High && In <= o->CtrlLimit - o->Hysteres)
    || (!o->High && In >= o->CtrlLimit + o->Hysteres)
  ) {
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
  } else if ((o->High && In > o->CtrlLimit) || (!o->High && In < o->CtrlLimit)) {
    if (!o->Action) {
      o->Action = TRUE;
    }
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {
      if (o->DetectCheck) {
        o->ActualValue = In;
        timerIn(sp, (sTimer *)&o->TimerFlag);
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

/* Cancel an alarm.  */

static void
cancelAlarm (
  sActive *ap,
  char *text
)
{
  sEvent *ep;
  sSupActive *sp;

  ep = eventListInsert(mh_eEvent_Cancel, text, ap);
  ap->status.All = 0;
  if (ap->source == mh_eSource_Scanner) {
    sp = (sSupActive *) ap;
    sp->sup->AlarmCheck = FALSE;
    sp->sup->ReturnSend = FALSE;
    sp->sup->ReturnCheck = FALSE;
    sp->sup->DetectSend = FALSE;
    sp->sup->Acked = TRUE;
    sp->sup->DetectCheck = TRUE;
    sp->sup->AlarmCheck = TRUE;
    sp->sup->AlarmStatus.All = 0;
    sp->timer->TimerCount = 0;
    if (sp->supType == mh_eSupType_Cycle) {
      pwr_sClass_CycleSup *cp = (pwr_sClass_CycleSup *) sp->op;
      cp->Delayed = FALSE;
      cp->DelayNoted = FALSE;
    }
  }
  updateAlarmInfo(ap);
  activeListRemove(ap);
  eventToOutunits(ep);
}

static void
checkOutunits ()
{
  sOutunit *op;
  LstLink(sOutunit) *ol;

  for (ol = LstFir(&l.outunit_l); ol != LstEnd(&l.outunit_l); ol = LstNex(ol)) {
    op = LstObj(ol);
    if (op->linkUp && op->syncedIdx != op->eventIdx) {
      if (op->check) {
#if 0
	errh_Info("Sending sync request to outunit qid: %s, oid: %s\n %d =! %d",
	  qcom_QidToString(NULL, &op->link.qid, 1),
	  cdh_ObjidToString(NULL, op->outunit, 1),
	  op->syncedIdx, op->eventIdx);
#endif
        sendToOutunit(op, mh_eMsg_HandlerSync, 0, NULL, 0);
      } else {
	op->check = 1;
      }
    }
  }
}

static void
cSup_exec (
  sSupActive *sp
)
{
  pwr_sClass_CycleSup *o = (pwr_sClass_CycleSup *) sp->op;
  pwr_tDeltaTime dnow;
  pwr_tDeltaTime nextLimit;
  int diff;

  time_Uptime(NULL, &dnow, NULL);
  nextLimit.tv_nsec = o->NextLimit.tv_nsec;
  nextLimit.tv_sec = o->NextLimit.tv_sec;
  o->DetectCount++;

  diff = time_Dcomp(&dnow, &nextLimit);

  if (diff > 0 || (o->Delayed && !o->DelayNoted)) {
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {
      if (o->DetectCheck) {
	timerIn(sp, (sTimer *)&o->TimerFlag);
	if (o->Delayed) {
	  o->DetectTime = o->DelayedTime;
	} else {
	  clock_gettime(CLOCK_REALTIME, &o->DetectTime);
	}
	o->DetectCheck = FALSE;
      }
      if (!o->TimerFlag) {
	o->DetectSend = TRUE;
	o->ReturnCheck = TRUE;
	o->Acked = FALSE;
	o->AlarmCheck = FALSE;
	o->DelayNoted = TRUE;
	if (o->DelayAction == 2) {
	  l.iohp->IOReadWriteFlag = FALSE;
	  errh_SetStatus( MH__IOSTALLED);
	}
      }
    }
  } else if (diff <= 0 || (o->Delayed && o->Timely)) {
    if (o->ReturnCheck) {
      if (o->Timely) {
	o->ReturnTime = o->TimelyTime;
      } else {
	clock_gettime(CLOCK_REALTIME, &o->ReturnTime);
      }
      o->ReturnCheck = FALSE;
      o->ReturnSend = TRUE;
    }
    if (o->AlarmCheck && !o->DetectCheck) {
      o->DelayNoted = FALSE;
      o->TimerCount = 0;
      o->DetectCheck = TRUE;
      o->Delayed = FALSE;
    }
  }
}

static void
dSup_exec (
  sSupActive *sp
)
{
  pwr_sClass_DSup *o = (pwr_sClass_DSup *) sp->op;
  pwr_tBoolean In = *(pwr_tBoolean *) sp->actualValue;

  if (In != o->CtrlPosition) {
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
  } else {
    if (!o->Action) {
      o->Action = TRUE;
    }
    if (o->AlarmCheck && o->DetectOn && !o->Blocked) {
      if (o->DetectCheck) {
        o->ActualValue = In;
        timerIn(sp, (sTimer *)&o->TimerFlag);
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
enableQcomEvents (
  qcom_sQid myQ
)
{
  int sts;
  qcom_sQid otherQue;

  otherQue = qcom_cQnetEvent;
  if (!qcom_Bind(&sts, &myQ, &otherQue)) {
    errh_Fatal("qcom_Bind(QnetEvent), %m", sts);
    exit(-1);
  }

  otherQue = qcom_cQapplEvent;
  if (!qcom_Bind(&sts, &myQ, &otherQue)) {
    errh_Fatal("qcom_Bind(QapplEvent), %m", sts);
    exit(-1);
  }

  otherQue = qcom_cQini;
  if (!qcom_Bind(&sts, &myQ, &otherQue)) {
    errh_Fatal("qcom_Bind(Qini), %m", sts);
    exit(-1);
  }

}

static void
enableQcomAllHandlers (
  qcom_sQid myQ
)
{
  int sts;
  qcom_sQid allHandQid = mh_cProcAllHandlers;

  if (!qcom_Bind(&sts, &myQ, &allHandQid)) {
    errh_Fatal("qcom_Bind(ProcAllHandlers), %m", sts);
    exit(-1);
  }
}

static void
eventListInit ()
{
  int i;

  l.event_l = (sEventList*) calloc(1, sizeof(sEventList) +
    sizeof(sEvent) * l.emon->EventListSize);
  if (l.event_l == NULL) {
    errh_Error("Allocating event list");
    exit(0);
  }
  l.event_l->idx = 0;      /* Index 0 not used for events */
  l.event_l->oldIdx = 0;
  l.event_l->size = l.emon->EventListSize;

  /* Initialize all alarm index lists */

  for (i = 0; i  <= l.event_l->size; i++)
    LstIni(&l.event_l->list[i].activeIdx_l);
}

static sEvent *
eventListInsert (
  mh_eEvent	event,
  char		*text,
  sActive	*ap
)
{
  pwr_tStatus   sts;
  sEvent	*ep;
  pwr_tUInt32	idx;
  sEventTab	*etp;

  idx = ++l.event_l->idx;
  l.emon->EventCount = idx;

  if (l.event_l->idx == 1) {
    l.event_l->oldIdx = 1;
    l.emon->EventCount = 1;
  } else if (l.event_l->idx > l.event_l->size) {
    l.event_l->oldIdx += 1;
  }

  ep = &l.event_l->list[idx % l.event_l->size];
  ep->idx = idx;
  ep->outunit = ap->outunit;
  ep->object = ap->object;
  ep->local = ap->local;
  memcpy(ep->objName, ap->objName, sizeof(ap->objName));
  ep->event = event;
  memset(&ep->msg, 0, sizeof(ep->msg));

  if (ep->etp != NULL) {
    ep->etp->ep = NULL;
    if (ep->etp->ap == NULL)
      tree_Remove(&sts, l.eventTab, &ep->etp->idx);
  }
  ep->etp = etp = tree_Insert(&sts, l.eventTab, &idx);
  
  if (ap->event != mh_eEvent_Info || (ap->eventFlags & mh_mEventFlags_InfoWindow) != 0)
    etp->ap = ap;
  else
    etp->ap = NULL;
  etp->event = event;
  etp->ep = ep;

  switch (event) {
  case mh_eEvent_Info:
  case mh_eEvent_Alarm:
  case mh_eEvent_Block:
  case mh_eEvent_Reblock:
    ap->idx = idx;
    ap->detect_etp = etp;
    break;
  case mh_eEvent_Unblock:
    ap->returnIdx = idx;
    ap->return_etp = etp;
    break;
  case mh_eEvent_Ack:
    ap->ackIdx = idx;
    ap->ack_etp = etp;
    break;
  case mh_eEvent_Return:
  case mh_eEvent_Cancel:
    ap->returnIdx = idx;
    ap->return_etp = etp;
    break;
  case mh_eEvent_CancelBlock: /* Is this correct ??*/
  default:
   errh_Error("eventListInsert, program error, event: %d", event);
   break;
  }

  switch (ap->source) {
  case mh_eSource_Scanner:
    formatSupEvent(event, text, (sSupActive*) ap, &ep->msg, &ep->msgSize);
    break;
  case mh_eSource_Application:
  case mh_eSource_Handler:
    formatApplEvent(event, text, (sApplActive*) ap, &ep->msg, &ep->msgSize);
    break;
  case mh_eSource_Outunit:
    formatOutunitEvent(event, text, (sBlock*) ap, &ep->msg, &ep->msgSize);
    break;
  default:
    errh_Error("eventListInsert, programming error, source: %d", ap->source);
    break;
  }

  return ep;
}

static void
eventToOutunits (
  sEvent *ep
)
{
  LstLink(sOutunit) *ol;
  sOutunit *op;

  for (ol = LstFir(&l.outunit_l); ol != LstEnd(&l.outunit_l); ol = LstNex(ol)) {
    op = LstObj(ol);
    if (op->syncedIdx == op->eventIdx)
      sendEventListToOutunit(op);
  }
}

static void
formatApplEvent (
  mh_eEvent event,
  char *text,
  sApplActive *aap,
  uEvent *up,
  pwr_tUInt32 *size
)
{
  mh_sAck *acp;
  mh_sReturn *rp;
  mh_sMessage *mp;
  mh_sMsgInfo *ip = &up->message.Info;

  ip->Id.Nix = l.head.nix;
  ip->Id.BirthTime = l.head.birthTime;
  ip->Object = aap->link.object;
  ip->SupObject = aap->link.supObject;
  ip->EventFlags = aap->link.eventFlags;
  strncpy(ip->EventName, aap->link.eventName, sizeof(ip->EventName));
  ip->EventType = event;

  switch (event) {
  case mh_eEvent_Alarm:
  case mh_eEvent_Info:
    mp = &up->message;
    ip->Id.Idx = aap->link.idx;
    ip->EventTime = aap->message.EventTime;
    if (aap->link.event == mh_eEvent_Alarm)
      ip->EventPrio = aap->message.EventPrio;
    strncpy(mp->EventText, aap->message.EventText, sizeof(mp->EventText));
    mp->SupInfo.SupType = aap->message.SupInfo.SupType;
    memcpy(&mp->SupInfo, &aap->message.SupInfo, sizeof(mh_uSupInfo));
    mp->EventSound = aap->link.eventSound;
    *size = sizeof(mh_sMessage);
    break;
  case mh_eEvent_Ack:
    acp = &up->ack;
    ip->Id.Idx = aap->link.ackIdx;
    ip->EventTime = aap->ackTime;
    if (aap->link.event == mh_eEvent_Alarm)
      ip->EventPrio = aap->message.EventPrio;
    acp->TargetId.Nix = l.head.nix;
    acp->TargetId.BirthTime = l.head.birthTime;
    acp->TargetId.Idx = aap->link.idx;
    acp->DetectTime = aap->message.EventTime;
    acp->Outunit = aap->ackOutunit;
    acp->SupInfo.SupType = aap->message.SupInfo.SupType;
    memcpy(&acp->SupInfo, &aap->message.SupInfo, sizeof(mh_uSupInfo));
    *size = sizeof(mh_sAck);
    break;
  case mh_eEvent_Cancel:
    rp = &up->ret;
    ip->Id.Idx = aap->link.returnIdx;
    clock_gettime(CLOCK_REALTIME, &ip->EventTime);
    if (aap->link.event == mh_eEvent_Alarm)
      ip->EventPrio = aap->message.EventPrio;
    strncpy(rp->EventText, text, sizeof(rp->EventText));
    rp->TargetId.Nix = l.head.nix;
    rp->TargetId.BirthTime = l.head.birthTime;
    rp->TargetId.Idx = aap->link.idx;
    rp->DetectTime = aap->message.EventTime;
    rp->SupInfo.SupType = aap->message.SupInfo.SupType;
    memcpy(&rp->SupInfo, &aap->message.SupInfo, sizeof(mh_uSupInfo));
    *size = sizeof(mh_sReturn);
    break;
  case mh_eEvent_Return:
    rp = &up->ret;
    ip->Id.Idx = aap->link.returnIdx;
    ip->EventTime = aap->returnTime;
    if (aap->link.event == mh_eEvent_Alarm)
      ip->EventPrio = aap->message.EventPrio;
    strncpy(rp->EventText, aap->returnText, sizeof(rp->EventText));
    rp->TargetId.Nix = l.head.nix;
    rp->TargetId.BirthTime = l.head.birthTime;
    rp->TargetId.Idx = aap->link.idx;
    rp->DetectTime = aap->message.EventTime;
    rp->SupInfo.SupType = aap->message.SupInfo.SupType;
    memcpy(&rp->SupInfo, &aap->message.SupInfo, sizeof(mh_uSupInfo));
    *size = sizeof(mh_sReturn);
    break;
  default:
    errh_Error("formatApplEvent, program error, event: %d", event);
    break;
  }
}

static void
formatSupEvent (
  mh_eEvent event,
  char *text,
  sSupActive *sp,
  uEvent *up,
  pwr_tUInt32 *size
)
{
  sSup *sup;
  mh_sAck *acp;
  mh_sReturn *rp;
  mh_sMessage *mp;
  mh_sMsgInfo *ip = &up->message.Info;

  ip->Id.Nix = l.head.nix;
  ip->Id.BirthTime = l.head.birthTime;
  ip->Object = sp->link.object;
  ip->SupObject = sp->link.supObject;
  ip->EventFlags = sp->link.eventFlags;
  strncpy(ip->EventName, sp->link.eventName, sizeof(ip->EventName));
  ip->EventType = event;
  sup = sp->sup;

  switch (event) {
  case mh_eEvent_Alarm:
  case mh_eEvent_Info:
    mp = &up->message;
    ip->Id.Idx = sp->link.idx;
    ip->EventTime = sup->DetectTime;
    strncpy(mp->EventText, sup->DetectText, sizeof(mp->EventText));
    if (sp->link.event == mh_eEvent_Alarm)
      ip->EventPrio = sup->EventPriority;
    mp->SupInfo.SupType = sp->supType;
#if 1
    if (sp->supInfoSize > sizeof(mp->SupInfo.mh_uSupInfo_u)) {
      errh_Error("formatSupEvent, program error, size: %d", sp->supInfoSize);
      sp->supInfoSize = sizeof(mp->SupInfo.mh_uSupInfo_u);
    }
#endif
    mp->EventSound = sp->link.eventSound;
    memcpy(&mp->SupInfo.mh_uSupInfo_u, sp->supInfoP, sp->supInfoSize);
    *size = sizeof(mh_sMessage);
    break;
  case mh_eEvent_Ack:
    acp = &up->ack;
    ip->Id.Idx = sp->link.ackIdx;
    ip->EventTime = sup->AckTime;
    if (sp->link.event == mh_eEvent_Alarm)
      ip->EventPrio = sup->EventPriority;
    acp->TargetId.Idx = sp->link.idx;
    acp->TargetId.Nix = l.head.nix;
    acp->TargetId.BirthTime = l.head.birthTime;
    acp->DetectTime = sup->DetectTime;
    acp->Outunit = sup->AckOutunit;
    *size = sizeof(mh_sAck);
    break;
  case mh_eEvent_Return:
    rp = &up->ret;
    ip->Id.Idx = sp->link.returnIdx;
    ip->EventTime = sup->ReturnTime;
    if (sp->link.event == mh_eEvent_Alarm)
      ip->EventPrio = sup->EventPriority;
    strncpy(rp->EventText, sup->ReturnText, sizeof(rp->EventText));
    rp->TargetId.Idx = sp->link.idx;
    rp->TargetId.Nix = l.head.nix;
    rp->TargetId.BirthTime = l.head.birthTime;
    rp->DetectTime = sup->DetectTime;
    rp->SupInfo.SupType = sp->supType;
#if 1
    if (sp->supInfoSize > sizeof(rp->SupInfo.mh_uSupInfo_u)) {
      errh_Error("formatSupEvent, program error, size: %d", sp->supInfoSize);
      sp->supInfoSize = sizeof(rp->SupInfo.mh_uSupInfo_u);
    }
#endif
    memcpy(&rp->SupInfo.mh_uSupInfo_u, sp->supInfoP, sp->supInfoSize);
    *size = sizeof(mh_sReturn);
    break;
  case mh_eEvent_Cancel:
    rp = &up->ret;
    ip->Id.Idx = sp->link.returnIdx;
    if (sp->link.event == mh_eEvent_Alarm)
      ip->EventPrio = sup->EventPriority;
    clock_gettime(CLOCK_REALTIME, &ip->EventTime);
    strncpy(rp->EventText, text, sizeof(rp->EventText));
    rp->TargetId.Idx = sp->link.idx;
    rp->TargetId.Nix = l.head.nix;
    rp->TargetId.BirthTime = l.head.birthTime;
    rp->DetectTime = sup->DetectTime;
    rp->SupInfo.SupType = sp->supType;
#if 1
    if (sp->supInfoSize > sizeof(rp->SupInfo.mh_uSupInfo_u)) {
      errh_Error("formatSupEvent, program error, size: %d", sp->supInfoSize);
      sp->supInfoSize = sizeof(rp->SupInfo.mh_uSupInfo_u);
    }
#endif
    memcpy(&rp->SupInfo.mh_uSupInfo_u, sp->supInfoP, sp->supInfoSize);
    *size = sizeof(mh_sReturn);
    break;
  default:
    errh_Error("formatSupEvent, program error, event: %d", event);
    break;
  }
}

static void
formatOutunitEvent (
  mh_eEvent event,
  char *text,
  sBlock *bp,
  uEvent *up,
  pwr_tUInt32 *size
)
{
  mh_sBlock *bmp = &up->block;
  mh_sMsgInfo *ip = &up->message.Info;

  ip->Id.Nix = l.head.nix;
  ip->Id.BirthTime = l.head.birthTime;
  ip->Object = bp->link.object;
  ip->EventFlags = bp->link.eventFlags;
  strncpy(ip->EventName, bp->link.eventName, sizeof(ip->EventName));
  ip->EventType = event;
  bmp->Status = bp->link.status.Event.Status;

  switch (event) {
  case mh_eEvent_Block:
    ip->EventTime = bp->outunitBlock.time;
    ip->EventPrio = bp->outunitBlock.prio;
    ip->Id.Idx = bp->link.idx;
    bmp->Outunit = bp->outunitBlock.outunit;
    *size = sizeof(mh_sBlock);
    break;
  case mh_eEvent_Unblock:
    ip->EventTime = bp->outunitUnblock.time;
    ip->Id.Idx = bp->link.returnIdx;
    ip->EventPrio = bp->outunitBlock.prio;
    bmp->TargetId.Idx = bp->link.idx;
    bmp->TargetId.Nix = l.head.nix;
    bmp->TargetId.BirthTime = l.head.birthTime;
    bmp->Outunit = bp->outunitUnblock.outunit;
    bmp->DetectTime = bp->outunitBlock.time;
    *size = sizeof(mh_sBlock);
    break;
  case mh_eEvent_Reblock:
    ip->EventTime = bp->outunitBlock.time;
    ip->Id.Idx = bp->link.idx;
    ip->EventPrio = bp->outunitBlock.prio;
    bmp->TargetId.Idx = bp->link.returnIdx;
    bmp->TargetId.Nix = l.head.nix;
    bmp->TargetId.BirthTime = l.head.birthTime;
    bmp->Outunit = bp->outunitBlock.outunit;
    bmp->DetectTime = bp->outunitUnblock.time;
    *size = sizeof(mh_sBlock);
    break;
  default:
    errh_Error("formatOutUnitEvent, program error, event: %d", event);
    break;
  }
}

static void
fromApplication (
  qcom_sGet	*get
)
{
  sAppl		*ap;
  mh_uApplReply reply;
  mh_sHead	*hp = (mh_sHead*) get->data;
  qcom_sAid	*aid = &get->sender;
  struct timespec  hold = {0, 10000};

  get->reply = hp->qid; 

  if (!isValidApplication(hp, aid, &ap, &reply)) {
    applReply(get, &reply, sizeof(reply));
    return;
  }

  switch (hp->type) {
  case mh_eMsg_ApplMessage:
    applMessage(hp, ap, &reply);
    break;
  case mh_eMsg_ApplReturn:
    applReturn(hp, ap, &reply);
    break;
  case mh_eMsg_ApplConnect:
    applConnect(hp, ap, &reply);
    break;
  case mh_eMsg_ApplDisconnect:
    applDisconnect(hp, ap, &reply);
    break;
  case mh_eMsg_ApplGetMsgInfo:
    applGetMsgInfo(hp, ap, &reply);
    break;
  default:
    errh_Info("fromApplication, program error, type: %d", hp->type);
    reply.Sts = MH__PROGERR;
    break;
  }

#if defined(OS_LINUX) 
  nanosleep(&hold, NULL);
#endif

  applLogState(ap);
  applReply(get, &reply, sizeof(reply));
}

static void
fromHandler (
  qcom_sGet	*get
)
{
  mh_sHead	*hp = (mh_sHead*) get->data;

  switch (hp->type) {
  case mh_eMsg_ProcDown:
    if (hp->ver != mh_cVersion) {
      /* Different versions, not yet implemented */
      errh_Info("fromHandler, Received a Message with different version");
      break;
    }

    if (qcom_QidIsEqual(&hp->qid, &l.head.qid))
      break;   /* Message is from this handler */
    procDown((qcom_sAid*) (hp + 1));
    break;
  default:
    break;
  }
}

static void
fromMessageUnit (
  qcom_sGet	*get
)
{
  mh_sHead	*hp = (mh_sHead*) get->data;

  switch (hp->source) {
  case mh_eSource_Outunit:
    fromOutunit(get);
    break;
  case mh_eSource_Application:
    fromApplication(get);
    break;
  case mh_eSource_Handler:
    fromHandler(get);
    break;
  default:
    errh_Error("fromMessageUnit, programming error, source: %d", hp->source);
    break;
  }
}

static void
fromOutunit (
  qcom_sGet	*get
)
{
  sOutunit	*op;
  mh_sHead	*hp = (mh_sHead*) get->data;
  qcom_sAid	*aid = &get->sender;

  if (hp->type == mh_eMsg_OutunitHello) {
    sendMessage(mh_eMsg_HandlerHello, &hp->qid, &hp->platform, NULL, 0);
    return;
  }

  if (!isValidOutunit(hp, aid, &op))
    return;

  switch (hp->type) {
  case mh_eMsg_OutunitAck:
    outunitAck(hp, op);
    break;
  case mh_eMsg_OutunitBlock:
    outunitBlock(hp, op);
    break;
  case mh_eMsg_OutunitDisconnect:
    outunitDisconnect(hp, op);
    break;
  case mh_eMsg_OutunitInfo:
    outunitInfo(hp, op);
    break;
  case mh_eMsg_Sync:
    outunitSync(hp, op);
    break;
  case mh_eMsg_OutunitSync:
    sendToOutunit(op, mh_eMsg_Sync, 0, NULL, 0);
    break;
  default:
    errh_Error("fromOutunit, program error, type: %d", hp->type);
    break;
  }

}

static void
fromQcom (
  qcom_sGet *gp
)
{
  qcom_sQid qid;
  qcom_sAid *aid;

  switch (gp->type.s) {
  case qcom_eStype_applConnect:
    break;
  case qcom_eStype_applDisconnect:
    aid = &((qcom_sAppl*) gp->data)->aid;
    procDown(aid);
    /* Inform other handlers */
    qid = mh_cProcAllHandlers;
    sendMessage(mh_eMsg_ProcDown, &qid, NULL, aid, sizeof(*aid));
    break;
  case qcom_eStype_linkActive: 
    linkActive(gp);
    break;
  case qcom_eStype_linkConnect:
    linkConnect(gp);
    break;
  case qcom_eStype_linkDisconnect: 
    linkDisconnect(gp);
    break;
  case qcom_eStype_linkStalled:
    linkStalled(gp);
    break;
  default:
    errh_Error("fromQcom, program error, type: %d", gp->type.s);
    break;
  }
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
    handlerEvent(eHEvent_RestartComplete, l.head.nix);
    reInitSupList();

    if (!LstEmp(&l.sup_l)) {
      l.supListState = eSupListState_Scan;
      setTimerActive(cMessageIdx, TRUE);
      if (!LstEmp(&l.detect_l)) {
        scanTimerList();
        scanDetectList();
        setTimerActive(cDetectIdx, TRUE);
      }
      scanSupList();
    } else {
      l.supListState = eSupListState_NoSup;
      setTimerActive(cMessageIdx, FALSE);
      errh_Info("No supervise objects.");
    }
  } else if (new_event.b.swapInit & !cur_event.b.swapInit) {
    l.supListState = eSupListState_Wait;
    errh_Info("Warm restart initiated.");   
    handlerEvent(eHEvent_RestartInit, l.head.nix);
  } else if (new_event.b.terminate & !cur_event.b.terminate) {
    exit(0);
  }

  sav_event = ep->mask;
}

static void
getHandlerObject ()
{
  pwr_tStatus  sts;
  pwr_tObjid   nodeOid;
  pwr_tObjid   oid;
  pwr_sAttrRef aref; 
  pwr_tDlid dlid;
  pwr_tBoolean created = FALSE;

  sts = gdh_GetClassList(pwr_eClass_Node, &nodeOid);            
  if (EVEN(sts)) {
    errh_Fatal("Couldn't get node object\n%m", sts);
    exit(sts);
  }
  sts = gdh_ObjidToName(nodeOid, l.emonName, sizeof(l.emonName), cdh_mNName);
  if (EVEN(sts)) {
    errh_Fatal("Couldn't get node name\n%m", sts);
    exit(sts);
  }

  strcat(l.emonName,"-MessageHandler");

  sts = gdh_GetClassList(pwr_cClass_MessageHandler, &l.emonObject);
  if (EVEN(sts)) {
    strcpy(l.emonName,"pwrNode-MessageHandler");
    sts = gdh_CreateObject(l.emonName, pwr_cClass_MessageHandler, 
      sizeof(pwr_sClass_MessageHandler), &l.emonObject,
      pwr_cNObjid, 0, pwr_cNObjid);
    if (EVEN(sts)) {
      errh_Fatal("Couldn't create message handler object\n%m", sts);
      exit(sts);
    }
    created = TRUE;
  }
  aref.Objid = l.emonObject;
  aref.Offset = 0;
  aref.Size = sizeof(pwr_sClass_MessageHandler);
  aref.Flags.m = 0;      
  sts = gdh_DLRefObjectInfoAttrref(&aref, (pwr_tAddress *)&l.emon, &dlid);
  if (EVEN(sts)) {
    errh_Fatal("Couldn't get direct link to message handler object\n%m", sts);
    exit(sts);
  }

  if (created) {
    l.emon->EventListSize = mh_cMaxEvents;
    l.emon->MaxApplAlarms = mh_cMaxApplAlarm;
    l.emon->DetectTimer = mh_cDetectTimer;
    l.emon->MessageTimer = mh_cMessageTimer;
    l.emon->OutunitServer = FALSE;
  }

  l.outunitServer = l.emon->OutunitServer;
  l.emon->AlarmCount = 0;
  l.emon->EventCount = 0;

  l.timerTime = MIN(l.emon->DetectTimer, l.emon->MessageTimer);
  l.timerTime = MIN(l.timerTime, cTimerOpenBlock);
  l.timerTime = MAX(l.timerTime, 0.1); /* Not faster than 10 Hz */
  l.timerTimeDelta.tv_sec = floor(l.timerTime);
  l.timerTimeDelta.tv_nsec = (l.timerTime - l.timerTimeDelta.tv_sec) * 1e9;


  l.timers[cBlockDbIdx].type = eTimer_BlockDbOpen;
  l.timers[cBlockDbIdx].multiple = MAX(1, cTimerOpenBlock / l.timerTime);
  l.timers[cBlockDbIdx].count = 0;
  l.timers[cBlockDbIdx].active = FALSE;

  l.timers[cDetectIdx].type = eTimer_ScanDetect;
  l.timers[cDetectIdx].multiple = MAX(1, l.emon->DetectTimer / l.timerTime);
  l.timers[cDetectIdx].count = 0;
  l.timers[cDetectIdx].active = FALSE;
  l.detectTimerTime = l.timerTime * l.timers[cDetectIdx].multiple;

  l.timers[cMessageIdx].type = eTimer_ScanMessage;
  l.timers[cMessageIdx].multiple = MAX(1, l.emon->MessageTimer / l.timerTime);
  l.timers[cMessageIdx].count = 0;
  l.timers[cMessageIdx].active = FALSE;

  l.timers[cOutunitIdx].type = eTimer_OutunitSync;
  l.timers[cOutunitIdx].multiple = MAX(1, cTimerOutunitSync / l.timerTime);
  l.timers[cOutunitIdx].count = 0;
  l.timers[cOutunitIdx].active = FALSE;

  aproc_RegisterObject( l.emonObject);

#if defined OS_VMS

  if (l.emon->EventLogSize > 0) {
    $DESCRIPTOR(CmdDsc,"mc pwr_exe:rt_elog.exe");
    unsigned int Flags = 1; /* Nowait */

    sts = lib$spawn(&CmdDsc, NULL, NULL, &Flags);
    if (EVEN(sts))
      errh_Info("Couldn't start Historical Event Logger.");

  } else {
    errh_Info("No Historical Event Logger will be started on this node.");
  }

#endif

  /* Get IOHandler object */
  sts = gdh_GetClassList(pwr_cClass_IOHandler, &oid);
  if (EVEN(sts)) {
    errh_Info("Could not find IO handler object, %m", sts);
    return;
  }

  l.iohp = NULL;
  gdh_ObjidToPointer(oid, (void *) &l.iohp);

}

static mh_eAgent
getAgent (
  sSupActive *sap
)
{
  pwr_tStatus sts;
  pwr_tClassId cid;
  pwr_tObjid Parent = sap->link.supObject;

  while (TRUE) {
    sts = gdh_GetParent(Parent, &Parent);
    if (sts == GDH__NO_PARENT) {
      return mh_eAgent_MH;
    } else if (EVEN(sts)) {
      errh_Error("%s\n%m", "Error getting parent of SupObject.", sts);
      return  mh_eAgent_None;
    } else if (cdh_ObjidIsEqual(Parent, pwr_cNObjid)) {
      return mh_eAgent_MH;
    }
    sts = gdh_GetObjectClass(Parent, &cid);
    if (EVEN(sts)) {
      errh_Error("%s\n%m", "Error getting class of SupObject parent.", sts);
      return mh_eAgent_None;
    }
    if (
      cid == pwr_cClass_Av
      || cid == pwr_cClass_Dv
    ) {
      return mh_eAgent_MH;
    } else if (
      cid == pwr_cClass_Ai
      || cid == pwr_cClass_Ao
      || cid == pwr_cClass_Co
      || cid == pwr_cClass_Di
      || cid == pwr_cClass_Do
      || cid == pwr_cClass_Po
    ) {
      return mh_eAgent_IO;
    } else if (cid == pwr_cClass_plc) {
      return mh_eAgent_Plc;
    }
  }
}

static void
handleAlarm (
  sSupActive *sp
)
{
  sEvent *ep;

  ep = eventListInsert(mh_eEvent_Alarm, NULL, (sActive*) sp);
  activeListInsert((sActive *) sp, ep, mh_eSource_Scanner);
  updateAlarm((sActive *) sp, ep);

  eventToOutunits(ep);
}

static void
handleInfo (
  sSupActive *sp
)
{
  sEvent *ep;

  ep = eventListInsert(mh_eEvent_Info, NULL, (sActive*) sp);
  if ((sp->link.eventFlags & mh_mEventFlags_InfoWindow) != 0) {
    activeListInsert((sActive *) sp, ep, mh_eSource_Scanner);
    updateAlarm((sActive *) sp, ep);
  }

  eventToOutunits(ep);
}

static void
handleMessage (
  qcom_sGet *get
) 
{
  pwr_tStatus sts;
  mh_sHead *hp;
  XDR xdrs;

  switch (get->type.b) {
  case mh_cMsgClass:
    hp = (mh_sHead*) get->data;
    if (hp->xdr) {
      int type = ntohl(hp->type);
      int source = ntohl(hp->source);

      xdrmem_create(&xdrs, (char *)hp, get->size, XDR_DECODE);
      sts = mh_NetXdrMessage(&xdrs, get->type.s, hp);
      if (EVEN(sts)) {
        errh_Error("XDR_DECODE. Type = %d,%d, Size = %d, Source = %d, Nix = %x, pid = %x", 
          type, get->type.s, get->size, source, get->sender.nid, (int)get->sender.aix);
      }
    }
    fromMessageUnit(get);
    break;
  case qcom_eBtype_qcom:
    fromQcom(get);
    break;
  case qcom_eBtype_event:
    fromEvent(get);
    break;
  default:
    errh_Info("handleMessage, unexpected message type, type: %d", get->type.b);
    break;
  }
}

static void
handleReturn (
  sSupActive *sp
)
{
  sEvent *ep;

  ep = eventListInsert(mh_eEvent_Return, NULL, (sActive*) sp);

  switch (sp->link.event) {
  case mh_eEvent_Alarm:
    updateAlarm((sActive *) sp, ep);
    break;
  case mh_eEvent_Info:
    if ((sp->link.eventFlags & mh_mEventFlags_InfoWindow) != 0) {
      updateAlarm((sActive *) sp, ep);
    } else {
      sp->sup->DetectCheck = TRUE;  /* Activate detection */
      sp->sup->AlarmCheck = TRUE;   /* Activate alarm supervision */
    }
    break;
  default:
    errh_Info("return: Return on unknown event type, event: %d", sp->link.event);
    break;
  }

  eventToOutunits(ep);
}

static void
handlerEvent (
  eHEvent event,
  pwr_tNodeIndex nix
)
{
  mh_sApplMessage *ip;
  char eventText[256];
  sApplActive *hp;
  sEvent *ep;
  mh_eEvent eventType = 0;
  mh_eEventPrio eventPrio = 0;
  mh_mEventFlags eventFlags = 0;
  pwr_tBoolean local = FALSE;

  return; /* todo */

  switch (event) {
  case eHEvent_LinkStart:
    if (!l.outunitServer)
      return;
    eventType = mh_eEvent_Alarm;
    eventPrio = mh_eEventPrio_A;
    eventFlags = mh_mEventFlags_Bell | mh_mEventFlags_Force
      | mh_mEventFlags_Ack | mh_mEventFlags_Return;
    sprintf(eventText, "No contact with node (%s)", l.nodeDb[nix].gdh.nodename);
    local = TRUE;
    break;
  case eHEvent_LinkUp:
    if (!l.outunitServer)
      return;
    eventType = mh_eEvent_Return;
    sprintf(eventText, "Established contact with node (%s): %s", 
      l.nodeDb[nix].gdh.nodename, l.nodeDb[nix].fullname);
    local = TRUE;
    break;
  case eHEvent_LinkDown:
    if (!l.outunitServer)
      return;
    eventType = mh_eEvent_Alarm;
    eventPrio = mh_eEventPrio_A;
    eventFlags = mh_mEventFlags_Bell | mh_mEventFlags_Force
      | mh_mEventFlags_Ack | mh_mEventFlags_Return;
    sprintf(eventText, "Lost contact with node (%s): %s",
      l.nodeDb[nix].gdh.nodename, l.nodeDb[nix].fullname);
    local = TRUE;
    break;
  case eHEvent_Booted:
    eventFlags = mh_mEventFlags_Bell | mh_mEventFlags_Force
      | mh_mEventFlags_InfoWindow | mh_mEventFlags_Returned;
    eventType = mh_eEvent_Info;
    sprintf(eventText, "Node up and running (%s): %s",
      l.nodeDb[0].gdh.nodename, l.nodeDb[0].fullname);
    break;
  case eHEvent_RestartInit:
    eventType = mh_eEvent_Alarm;
    eventPrio = mh_eEventPrio_A;
    eventFlags = mh_mEventFlags_Bell | mh_mEventFlags_Force
      | mh_mEventFlags_Ack | mh_mEventFlags_Return;
    sprintf(eventText, "Restart initiated at node (%s): %s",
      l.nodeDb[0].gdh.nodename, l.nodeDb[0].fullname);
    break;
  case eHEvent_RestartComplete:
    eventType = mh_eEvent_Return;
    sprintf(eventText, "Restart completed at node (%s): %s",
      l.nodeDb[0].gdh.nodename, l.nodeDb[0].fullname);
    break;
  default:
    return;
    break;
  }

  if (event == eHEvent_Booted) {
    hp = handlerListAlloc();
    if (hp == NULL) {
      errh_Error("%s\n%m", "HandlerEvent", MH__NOSPACE);
      return;
    }
  } else if (l.nodeDb[nix].hp == NULL) {
    hp = handlerListAlloc();
    if (hp == NULL) {
      errh_Error("%s\n%m", "HandlerEvent", MH__NOSPACE);
      return;
    }
    l.nodeDb[nix].hp = hp;
  } else {
    hp = l.nodeDb[nix].hp;
  }

  switch (eventType) {
  case mh_eEvent_Alarm:
  case mh_eEvent_Info:

    if (LstInl(&hp->link.active_l)) /* already active */
      return;

    ip = &hp->message;
    ip->Object = l.emonObject;
    ip->EventFlags = eventFlags;
    ip->EventPrio = eventPrio;
    clock_gettime(CLOCK_REALTIME, &ip->EventTime);
    strncpy(ip->EventName, l.emonName, sizeof(ip->EventName) - 1);
    ip->EventType = eventType;
    strncpy(ip->EventText, eventText, sizeof(ip->EventText) - 1);

    hp->link.local = local;
    hp->link.source = mh_eSource_Handler;
    hp->link.object = ip->Object;
    strncpy(hp->link.objName, ip->EventName, sizeof(hp->link.objName));
    cdh_ToUpper(hp->link.objName, NULL);
    strncpy(hp->link.eventName, ip->EventName, sizeof(hp->link.eventName));
    hp->link.eventFlags = ip->EventFlags;
    hp->link.event = ip->EventType;

    /* Insert in application alarm list */
    (void)LstIns(&l.nodeDb[nix].appl.active_l, hp, active_l);    
    hp->ap = (sAppl *)&l.nodeDb[nix];
    ++hp->ap->activeMessages;
    ep = eventListInsert(eventType, NULL, (sActive*) hp);
    activeListInsert((sActive*) hp, ep, mh_eSource_Handler);
    updateAlarm((sActive *) hp, ep);
    break;
  case mh_eEvent_Return:

    if (!LstInl(&hp->link.active_l)) /* not active */
      return;

    if ((hp->link.status.Event.Status & mh_mEventStatus_NotRet) == 0)
      return; /* already returned */

    clock_gettime(CLOCK_REALTIME, &hp->returnTime);
    hp->returnType = mh_eEvent_Return;
    strncpy(hp->returnText, eventText, sizeof(ip->EventText) - 1);
    ep = eventListInsert(eventType, NULL, (sActive*) hp);
    updateAlarm((sActive *) hp, ep);
  default:
    errh_Info("handlerEvent, unexpected event type, event: %d", eventType);
    break;
  }

  eventToOutunits(ep);
}

static sApplActive *
handlerListAlloc (
  pwr_tNodeIndex Nix
)
{
  LstLink(sApplActive) *hl;
  LstHead(sApplActive) *ll;
  sApplActive *hp;
  const int Alloc = 50;
  int i;

  return NULL;

  ll = &l.handlerFree_l;

  if (LstEmp(ll)) {
    hp = (sApplActive *) calloc(Alloc, sizeof(sApplActive));
    if (hp != NULL) {
      for (i = 0; i < Alloc; i++, hp++)
        LstIns(ll, hp, link.active_l);
      l.emon->AlarmMaxCount += Alloc;
    } else {
      return NULL;
    }
  }

  hl = LstFir(ll);
  LstRem(hl);
  LstNul(hl);
  --l.emon->AlarmMaxCount;
  return LstObj(hl);
}

static void
handlerListFree (
 sApplActive *hp
)
{

  return;
  memset(hp, 0, sizeof(*hp));
  LstIns(&l.handlerFree_l, hp, link.active_l);
  ++l.emon->AlarmMaxCount;
  return;
}

static void
initBlockList ()
{
  pwr_tStatus sts;
  pwr_tUInt32 blocks;
  char msg[80];
  sSaveBlock *sp;
  sBlock *bp;
  sEvent *ep;
  pwr_tUInt32 size;
  char hostspec[12];
  pwr_tNodeIndex nix;
  int i;

#if defined OS_LYNX || defined OS_LINUX
  char *env = getenv("pwrp_load"); 
  hostspec[0] = '\0'; /* Prevent warnings from gcc */

#elif defined OS_ELN

  syi_HostSpec(NULL, hostspec, sizeof(hostspec));

#elif defined OS_VMS

  hostspec[0] = '\0';

#else
# error "Block Db not implemented for this platform."
#endif


#if defined OS_ELN || defined OS_VMS
  sprintf(l.blockDbName, "%spwrp_load:ld_bdb_%06x.dat", hostspec, l.head.nix);
#else
  sprintf(l.blockDbName, "%s/ld_bdb_%06x.dat", env != NULL ? env : "", l.head.nix);
#endif

  l.blockDb = mh_BlockDbOpen(l.blockDbName, &size);
  if (l.blockDb == NULL) return;

  if (size == 0) return;

  l.blockSaveSize = MAX(size, 100 * sizeof(sSaveBlock));
  l.blockSave = calloc(1, l.blockSaveSize);
  if (l.blockSave == NULL) {
    errh_Error("InitBlockList, %m", MH__NOSPACE);
    exit(MH__NOSPACE);
  }
  l.blockDb = mh_BlockDbGet(l.blockDb, &size, (char *)l.blockSave);  
  if (l.blockDb == NULL) return;
  blocks = size / sizeof(sSaveBlock);
  sprintf(msg, "found %d blocks in blockdb", blocks);
  errh_Info(msg);

  for (i = 0, sp = l.blockSave; i < blocks; i++, sp++) {

    sts = gdh_GetObjectNodeIndex(sp->outunitBlock.object, &nix);
    if (EVEN(sts) || nix != l.head.nix) {
      errh_Error("%s\n%m", "InitBlockList, object not owned or known", sts);
      continue;
    }

    bp = blockListAlloc();
    if (bp == NULL) {
      errh_Error("InitBlockList, %m", MH__NOSPACE);
      exit(MH__NOSPACE);
    }
    bp->link.event = mh_eEvent_Block;
    bp->link.eventFlags = mh_mEventFlags_Force;
    bp->link.source = mh_eSource_Outunit;
    bp->link.object = sp->outunitBlock.object;
    sts = gdh_ObjidToName(sp->outunitBlock.object, bp->link.objName,
      sizeof(bp->link.objName), cdh_mNName);
    if (EVEN(sts)) {
      errh_Error("%s\n%m", "InitBlockList, couldn't block object", sts);
      blockListFree(bp);
      continue;
    }
    strncpy(bp->link.eventName, bp->link.objName, sizeof(bp->link.eventName));
    cdh_ToUpper(bp->link.objName, NULL);
    bp->outunitBlock = sp->outunitBlock;
    bp->targetId = sp->targetId;
    bp->link.status.Event.Prio = (pwr_tUInt8) sp->outunitBlock.prio;
    bp->link.status.Event.Status = mh_mEventStatus_Block;
    sts = gdh_SetAlarmBlockLevel(sp->outunitBlock.object, bp->link.status.All);
    ep = eventListInsert(bp->link.event, NULL, (sActive *) bp);
    activeListInsert((sActive *) bp, ep, mh_eSource_Outunit);
  } 
}

/* Initialize the node database.  */

static void
initNodeDb ()
{
  int i;

  pwr_tStatus sts;
  sNodeInfo *np;
  pwr_tNodeId nid;

  return;

  memset(l.nodeDb, 0, sizeof(l.nodeDb));

  np = &l.nodeDb[0];

  sts = gdh_GetNodeIndex(&nid);
  sts = gdh_GetNodeObject(nid, &np->gdh.objid);
  sts = gdh_ObjidToName(np->gdh.objid, np->fullname, sizeof(np->fullname), cdh_mNName);

  for (i = 0; i < cNodes; i++) {
    LstIni(&l.nodeDb[i].appl.active_l);
  }
}

/* Initialize a SupActiveCB record.  */

static pwr_tStatus
initSupActiveCB (
  pwr_tObjid SupObject,
  pwr_tClassId cid,
  sSupActive **spp,
  pwr_tBoolean Allocate,
  pwr_tBoolean NewAttribute
)
{
  pwr_tStatus sts;
  sASup *asp;
  sDSup *dsp;
  pwr_sClass_NodeLinkSup *nlsp;
  pwr_sClass_CycleSup *csp;
  pwr_tObjid Object;
  sSupActive *sp;
  pwr_sAttrRef aref; 
  gdh_tDlid Dlid;
  pwr_tBoolean IsAlias = 0;

  sts = gdh_IsAlias(SupObject, &IsAlias);
  if (IsAlias)
    return 2;

  /* Get pointer to supervisory object */
  if (cid == pwr_cClass_ASup) {
    aref.Objid = SupObject;
    aref.Offset = 0;
    aref.Size = sizeof(pwr_sClass_ASup);
    aref.Flags.m = 0;       
    sts = gdh_DLRefObjectInfoAttrref(&aref, (pwr_tAddress *)&asp, &Dlid);
  } else if (cid == pwr_cClass_DSup) {
    aref.Objid = SupObject;
    aref.Offset = 0;
    aref.Size = sizeof(pwr_sClass_DSup);
    aref.Flags.m = 0;       
    sts = gdh_DLRefObjectInfoAttrref(&aref, (pwr_tAddress *)&dsp, &Dlid);
  } else if (cid == pwr_cClass_NodeLinkSup) {
    aref.Objid = SupObject;
    aref.Offset = 0;
    aref.Size = sizeof(pwr_sClass_NodeLinkSup);
    aref.Flags.m = 0;
    sts = gdh_DLRefObjectInfoAttrref(&aref, (pwr_tAddress *)&nlsp, &Dlid);
  } else if (cid == pwr_cClass_CycleSup) {
    aref.Objid = SupObject;
    aref.Offset = 0;
    aref.Size = sizeof(pwr_sClass_CycleSup);
    aref.Flags.m = 0;
    sts = gdh_DLRefObjectInfoAttrref(&aref, (pwr_tAddress *)&csp, &Dlid);
  }

  if (EVEN(sts)) {
    errh_Fatal("Couldn't get direct link to sup object\n%m", sts);
    return sts;
  }

  /* Get objid of supervised object */
  if (cid == pwr_cClass_ASup)
    Object = asp->Sup.Attribute.Objid;  
  else if (cid == pwr_cClass_DSup)
    Object = dsp->Sup.Attribute.Objid;
  else if (cid == pwr_cClass_NodeLinkSup)
    Object = SupObject;
  else if (cid == pwr_cClass_CycleSup)
    Object = SupObject;

  /* Allocate and initiate supervisor control block */
  if (Allocate) {
    sp = (sSupActive*) calloc(1, sizeof(sSupActive));
    if (sp == NULL) {
      errh_Error("Error calloc, InitSupActiveCB");
      return 1;
    }
  } else
    sp = *spp;

  sp->link.source = mh_eSource_Scanner;
  sp->link.object = Object;
  sp->link.supObject = SupObject;
  sp->link.objName[0] = '\0';
  sts = gdh_ObjidToName(Object, sp->link.objName, sizeof(sp->link.objName), cdh_mNName);
  if (EVEN(sts))
    errh_Error("Couldn't get name for supervised object\n%m", sts);
  strncpy(sp->link.eventName, sp->link.objName, sizeof(sp->link.eventName));
  cdh_ToUpper(sp->link.objName, NULL);

  switch (cid) {
  case pwr_cClass_ASup:
    sp->supType = mh_eSupType_Analog;
    sp->supDlid = Dlid;
    sp->sup = &asp->Sup;
    sp->supInfoSize = sizeof(mh_sASupInfo);
    sp->supInfoP = &asp->Info; 
    sp->attribute = asp->Sup.Attribute; 
    sp->link.eventSound = asp->Sup.Sound; 
    sp->timer = &asp->Timer;
    sp->op = (void *) asp;
    sp->agent = getAgent(sp);
    if (sp->agent == mh_eAgent_MH)
      sp->detect_exec = aSup_exec;
    break;
  case pwr_cClass_DSup:
    sp->supType = mh_eSupType_Digital;
    sp->supDlid = Dlid;
    sp->sup = &dsp->Sup;
    sp->supInfoSize = sizeof(mh_sDSupInfo);
    sp->supInfoP = &dsp->Info; 
    sp->attribute = dsp->Sup.Attribute;
    sp->link.eventSound = dsp->Sup.Sound;
    sp->timer = &dsp->Timer;
    sp->op = (void *) dsp;
    sp->agent = getAgent(sp);
    if (sp->agent == mh_eAgent_MH)
      sp->detect_exec = dSup_exec;
    break;
  case pwr_cClass_NodeLinkSup:
    sp->supType = mh_eSupType_Link;
    sp->supDlid = Dlid;
    sp->sup = (sSup *)&nlsp->ControlP;
    sp->supInfoSize = 0;
    sp->supInfoP = NULL;
    sp->link.eventSound = nlsp->Sound; 
    sp->attribute.Objid = Object;
    sp->attribute.Offset = pwr_Offset(nlsp, LinkUp);
    sp->attribute.Size = sizeof(nlsp->LinkUp);
    sp->attribute.Flags.m = 0;
    nlsp->Attribute = sp->attribute;
    sp->timer = (sTimer *)&nlsp->TimerFlag;
    sp->op = (void *) nlsp;
    sp->agent = mh_eAgent_None;
    break;
  case pwr_cClass_CycleSup:
    sp->supType = mh_eSupType_Cycle;
    sp->supDlid = Dlid;
    sp->sup = (sSup *)&csp->ControlP;
    sp->supInfoSize = 0;
    sp->supInfoP = NULL;
    sp->link.eventSound = csp->Sound; 
    sp->attribute.Objid = Object;
    sp->attribute.Offset = pwr_Offset(csp, DelayLimit);
    sp->attribute.Size = sizeof(csp->DelayLimit);
    sp->attribute.Flags.m = 0;
    csp->Attribute = sp->attribute;
    sp->timer = (sTimer *)&csp->TimerFlag;
    sp->op = (void *) csp;
    sp->agent = mh_eAgent_MH;
    sp->detect_exec = cSup_exec;
    break;
  default:
    errh_Error("initSupActiveCB, program error, cid: %s", cdh_ClassIdToString(NULL, cid, 0));
    break;
  }

  sp->link.eventFlags = sp->sup->EventFlags;
  sp->link.event = sp->sup->EventType;

  if (sp->agent == mh_eAgent_MH) {
    sts = gdh_DLRefObjectInfoAttrref(
      &sp->attribute, (pwr_tAddress *)&sp->actualValue, &sp->attrDlid
    );
    if (EVEN(sts)) {
      errh_Error("%s\n%m", "Couldn't link to supervised attribute", sts);
      sp->agent = mh_eAgent_None;    
    }
  }

  /* Initiate flags to make detection possible by PLC */

  if (NewAttribute) {
    sp->sup->AlarmCheck = FALSE;
    sp->sup->ReturnSend = FALSE;
    sp->sup->ReturnCheck = FALSE;
    sp->sup->DetectSend = FALSE;
    sp->sup->Acked = TRUE;
    sp->sup->DetectCheck = TRUE;
    sp->sup->AlarmCheck = TRUE;
    sp->sup->AlarmStatus.All = 0;
  }

  *spp = sp;
  return sts;
}

/* Initialize the Supervisor list.  */

static pwr_tStatus
initSupList ()
{
  pwr_tStatus sts;


/* Get class list and loop trough Sup objects */

  sts = initSupListClass(pwr_cClass_ASup);
  if (EVEN(sts))
    errh_Error("Initiate list of ASup's\n%m", sts);

  sts = initSupListClass(pwr_cClass_DSup);
  if (EVEN(sts))
    errh_Error("Initialize list of DSup's\n%m", sts);

  sts = initSupListClass(pwr_cClass_NodeLinkSup);
  if (EVEN(sts))
    errh_Error("Initialize list of LinkSup's\n%m", sts);

  sts = initSupListClass(pwr_cClass_CycleSup);
  if (EVEN(sts))
    errh_Error("Initialize list of CycleSup's\n%m", sts);

  if (LstEmp(&l.sup_l))
    errh_Info("No sup objects");

  return(sts);
}

static pwr_tStatus
initSupListClass(
  pwr_tClassId cid
)
{
  pwr_tStatus sts;
  LstLink(sSupActive) *sl;
  LstLink(sSupActive) *dl;
  sSupActive *sp;
  pwr_tObjid oid;

  sl = LstEnd(&l.sup_l);
  dl = LstEnd(&l.detect_l);

  /* Loop trough objects in class list.  */

  for (
    sts = gdh_GetClassList(cid, &oid);
    ODD(sts);
    sts = gdh_GetNextObject(oid, &oid)
  ) {
    if ( oid.vid < cdh_cUserVolMin)
      continue;

    sts = initSupActiveCB(oid, cid, &sp, 1, 1);
    if (ODD(sts)) {
      sl = LstIns(sl, sp, sup_l);
      l.emon->BlockMaxCount++;
      if (sp->agent == mh_eAgent_MH) {
        dl = LstIns(dl, sp, detect_l);
        l.emon->EventFirstIdx++;
      }
    }
  }

  if (sts == GDH__NO_TYPE || sts == GDH__NOSUCHOBJ) sts = MH__SUCCESS;

  return(sts);
}

static pwr_tBoolean
isForOutunit (
  sOutunit *op,
  pwr_tObjid outunit,
  pwr_tObjid object,
  pwr_tString80 objName,
  mh_mEventFlags eventFlags,
  pwr_tBoolean local
)
{
  int i;

  if (op->selGen == 0)
    return FALSE;

  if (op->type == mh_eOutunitType_Logger)
    return TRUE;  /* Logger gets all events */

  if (local)
    if (op->link.nix != l.head.nix)
      return FALSE;

  if (cdh_ObjidIsEqual(outunit,op->outunit))
    return TRUE;  /* Send message dedicated for this Outunit */
  else if (cdh_ObjidIsNotEqual(outunit,pwr_cNObjid))
    return FALSE; /* This message is dedicated to other Outunit */

  if ((eventFlags & mh_mEventFlags_Force) != 0)
    return TRUE;   /* Send forced message */

  if (cdh_ObjidIsEqual(object,pwr_cNObjid))
    return TRUE;   /* Send message not associated with object */

  /* Check select list */

  for (i=0; i < op->selSize; i++)
    if (strncmp(op->sel_l[i].objName, objName, op->sel_l[i].len) == 0)
      return TRUE;

  return FALSE;
}

static pwr_tBoolean
isValidApplication (
  mh_sHead *hp,
  qcom_sAid *aid,
  sAppl **appl,
  mh_uApplReply *Reply
)
{
  sAppl *ap;
  LstLink(sAppl) *al;

  if (hp->ver != mh_cVersion) {
    /* Different versions, not yet implemented */
    errh_Info("isValidApplication: Received a Message with different version");
    Reply->Sts = MH__VERSION;
    return FALSE;
  }

  /* Find outunit in outunit list */

  for (al = LstFir(&l.appl_l); al != LstEnd(&l.appl_l); al = LstNex(al))
    if (cdh_ObjidIsEqual(LstObj(al)->aid, hp->aid))
      break;

  if (al == LstEnd(&l.appl_l)) {
    /* Application not known, make it known */
    ap = (sAppl*) calloc(1, sizeof(*ap));
    if (ap == NULL) {
      errh_Error("applConnect, error calloc\n%m", MH__NOSPACE);
      Reply->Sts = MH__NOSPACE;
      return FALSE;
    }
    ap->link.source = mh_eSource_Application;
    ap->link.qid = hp->qid;
    ap->link.aid = *aid; /* QCOM ApplId */
    ap->link.platform = hp->platform;
    ap->birthTime = hp->birthTime;
    ap->aid = hp->aid;
    ap->state = mh_eApplState_New;
    /* Insert in application list */
    LstIns(&l.appl_l, ap, appl_l);
    /* Insert in process list */
    LstIns(&l.proc_l, ap, link.proc_l); 
    LstIni(&ap->active_l);      /* Init application alarm list */
  } else {
    ap = LstObj(al);

    if (memcmp(&ap->birthTime, &hp->birthTime, sizeof(ap->birthTime)) != 0) {
      /* Different times, i.e. the application is restarted */
      ap->link.source = mh_eSource_Application;
      ap->link.qid = hp->qid;
      ap->link.aid = *aid; /* QCOM ApplId */
      ap->link.platform = hp->platform;
      ap->birthTime = hp->birthTime;
      ap->aid = hp->aid;
      ap->state = mh_eApplState_Restarted;
    }
  }

  applLogState(ap);
  *appl = ap;
  Reply->Sts = MH__SUCCESS;
  return TRUE;
}

static pwr_tBoolean
isValidOutunit (
  mh_sHead *hp,
  qcom_sAid *aid,
  sOutunit **outunit
)
{
  sOutunit *op;
  LstLink(sOutunit) *ol;

  if (hp->ver != mh_cVersion) {
    /* Different versions, not yet implemented */
    errh_Info("isValidOutunit: Received a Message with different version");
    return FALSE;
  }

  /* Find outunit in outunit list */

  for (ol = LstFir(&l.outunit_l); ol != LstEnd(&l.outunit_l); ol = LstNex(ol))
    if (cdh_ObjidIsEqual(LstObj(ol)->outunit, hp->outunit))
      break;

  if (ol == LstEnd(&l.outunit_l)) {
    /* Outunit not known, make it known */
    op = (sOutunit*) calloc(1, sizeof(*op));
    if (op == NULL) {
      errh_Error("Error calloc, OutUnitHello");
      return 1;
    }
    op->link.source = mh_eSource_Outunit;
    op->link.qid = hp->qid;
    op->link.aid = *aid; /* QCOM ApplId */
    op->link.platform = hp->platform;
    op->link.nix = hp->nix;
    op->birthTime = hp->birthTime;
    op->outunit = hp->outunit;
    /* Insert in outunit list */
    LstIns(&l.outunit_l, op, outunit_l);   
    /* Insert in process list */
    LstIns(&l.proc_l, op, link.proc_l);    
    op->linkUp = TRUE;
    outunitLog(op, "New outunit");
  } else {
    op = LstObj(ol);

    if (memcmp(&op->birthTime, &hp->birthTime, sizeof(op->birthTime)) != 0) {
      /* Different times, i.e. the outunit is restarted */
      op->link.source = mh_eSource_Outunit;
      op->link.qid = hp->qid;
      op->link.aid = *aid; /* QCOM ApplId */
      op->link.platform = hp->platform;
      op->link.nix = hp->nix;
      op->birthTime = hp->birthTime;
      op->outunit = hp->outunit;
      op->selGen = 0;
      op->eventGen = 0;
      op->linkUp = TRUE;
      outunitLog(op, "Outunit restarted");
    }
  }

  *outunit = op;
  return TRUE;
}

static void
linkActive (
  qcom_sGet *msg
)
{
  qcom_sNode *node = (qcom_sNode *)msg->data;

  errh_Info("Active, link to node %s (%s)",
    node->name, cdh_NodeIdToString(NULL, node->nid, 0, 0));

}

static void
linkConnect (
  qcom_sGet *msg
)
{
  pwr_tStatus sts;
  LstLink(sOutunit) *ol;
  qcom_sNode *node = (qcom_sNode *)msg->data;
  int nix = node->nid;

  errh_Info("Connected, link to node %s (%s)",
    node->name, cdh_NodeIdToString(NULL, node->nid, 0, 0));

#if 0
  int i;
  for (i = 1; i < cNodes; i++)
    if (l.nodeDb[i].gdh.nix == nix) {
      l.nodeDb[i].newLinkState = gdh_eLinkState_Up;
      l.nodeDb[i].check = TRUE;
      break;
    }
#endif

  for (ol = LstFir(&l.outunit_l); ol != LstEnd(&l.outunit_l); ol = LstNex(ol))
    if (nix == LstObj(ol)->link.qid.nid) {
      LstObj(ol)->linkUp = TRUE;
    }

  sts = sendMessage(mh_eMsg_HandlerHello, NULL, NULL, NULL, 0); /* Left to do !!! Send to specific node */
}

static void
linkDisconnect (
  qcom_sGet *msg
)
{
  LstLink(sOutunit) *ol;
  qcom_sNode	*node = (qcom_sNode *)msg->data;
  int		nix = node->nid;


  errh_Info("Disconnected, link to node %s (%s)",
    node->name, cdh_NodeIdToString(NULL, node->nid, 0, 0));

#if 0
  int i;
  for (i = 1; i < cNodes; i++)
    if (l.nodeDb[i].gdh.nix == node->nid) {
      l.nodeDb[i].newLinkState = gdh_eLinkState_Down;
      l.nodeDb[i].check = TRUE;
      break;
    }
#endif

  for (ol = LstFir(&l.outunit_l); ol != LstEnd(&l.outunit_l); ol = LstNex(ol))
    if (nix == LstObj(ol)->link.qid.nid) {
      LstObj(ol)->linkUp = FALSE;
    }
}

static void
linkStalled (
  qcom_sGet *msg
)
{
  qcom_sNode *node = (qcom_sNode *)msg->data;

  errh_Info("Stalled, link to node %s (%s)",
    node->name, cdh_NodeIdToString(NULL, node->nid, 0, 0));

}

static void
outunitAborted (
  sOutunit *op
)
{

  outunitLog(op, "Outunit aborted");

  LstRem(&op->outunit_l);   /* Remove from outunit list */
  LstNul(&op->outunit_l);
  LstRem(&op->link.proc_l); /* Remove from process link */
  LstNul(&op->link.proc_l);
  free(op);       /* Free control block */
}

static void
outunitAck (
  mh_sHead *hp,
  sOutunit *op
)
{
  sActive *ap;
  mh_sOutunitAck *ip = (mh_sOutunitAck*) (hp + 1);
  sEvent *ep;
  sSupActive *sp;
  sApplActive *aap;

  op->ackGen = ip->ackGen;
  sendToOutunit(op, mh_eMsg_Sync, 0, NULL, 0);

  if ((ap = activeListGet(ip->targetIdx)) == NULL) {
    /* Alarm does not exist */
    return;
  } else if ((ap->status.Event.Status & mh_mEventStatus_NotAck) == 0) {
    /* Alarm already acked */
    return;
  }

  switch (ap->source) {
  case mh_eSource_Scanner:
    sp = (sSupActive*) ap;
    sp->sup->AckOutunit = op->outunit;  /* Update ack outunit in sup object */
    clock_gettime(CLOCK_REALTIME, &sp->sup->AckTime);       /* Update ack time in sup object */
    break;
  case mh_eSource_Application:
  case mh_eSource_Handler:
    aap = (sApplActive*) ap;
    aap->ackOutunit = op->outunit;
    clock_gettime(CLOCK_REALTIME, &aap->ackTime);
    break;
  default:
    errh_Error("outunitAck, programming error, source: %d", ap->source);
    return;
  }

  ep = eventListInsert(mh_eEvent_Ack, NULL, ap);

  switch (ap->event) {
  case mh_eEvent_Alarm:
    updateAlarm(ap, ep);
    break;
  case mh_eEvent_Info:
    if ((ap->eventFlags & mh_mEventFlags_InfoWindow) != 0)
      updateAlarm(ap, ep);
    break;
  default:
    errh_Info("outunitAck, Ack on unknown event type, event: %d", ap->event);
    break;
  }

  eventToOutunits(ep);

}

static void
outunitBlock (
  mh_sHead *hp,
  sOutunit *op
)
{
  pwr_tStatus sts;
  sBlock *bp = NULL;
  mh_sOutunitBlock *ip = (mh_sOutunitBlock*) (hp + 1);
  sEvent *ep;
  LstLink(sBlock) *bl;
  pwr_tNodeIndex nix;

  op->blockGen = ip->blockGen;
  sendToOutunit(op, mh_eMsg_Sync, 0, NULL, 0);

  for (bl = LstFir(&l.block_l); bl != LstEnd(&l.block_l); bl = LstNex(bl))
    if (cdh_ObjidIsEqual(ip->object, LstObj(bl)->link.object))
      bp = LstObj(bl);

  if (bp == NULL) {
    sts = gdh_GetObjectNodeIndex(ip->object, &nix);
    if (EVEN(sts) || nix != l.head.nix) {
      errh_Error("%s\n%m", "OutunitBlock: object not owned by this node", sts);
      return;
    }
    if (ip->prio == 0) { /* object already unblocked */
      sts = gdh_SetAlarmBlockLevel(ip->object, 0);
      return; /* object already unblocked */
    }

    bp = blockListAlloc();
    if (bp == NULL) {
      errh_Error("%s\n%m", "OutunitBlock", MH__NOSPACE);
      return;
    }

    bp->link.event = mh_eEvent_Block;
    bp->link.eventFlags = mh_mEventFlags_Force;
    bp->link.source = mh_eSource_Outunit;
    bp->link.object = ip->object;
    sts = gdh_ObjidToName(ip->object, bp->link.objName,
      sizeof(bp->link.objName), cdh_mNName);
    if (EVEN(sts)) {
      errh_Error("%s\n%m", "OutunitBlock: couldn't get name of blocked object", sts);
      blockListFree(bp);
      return;
    }
    strncpy(bp->link.eventName, bp->link.objName, sizeof(bp->link.eventName));
    cdh_ToUpper(bp->link.objName, NULL);
  } else if (bp->outunitBlock.prio != ip->prio) { /* new block level */
    if (bp->link.idx >= l.event_l->oldIdx) {
      /* Block message still in event list, update status */
      l.event_l->list[bp->link.idx % l.event_l->size].msg.block.Status = 0;
    }
    if (ip->prio == 0)
      bp->link.event = mh_eEvent_Unblock;
    else
      bp->link.event = mh_eEvent_Reblock;
  } else { /* object already have this block level */
    return;
  }

  l.newBlock = TRUE;

  switch (bp->link.event) {
  case mh_eEvent_Block:
    bp->outunitBlock = *ip;
    clock_gettime(CLOCK_REALTIME, &bp->outunitBlock.time);      /* Update block time */
    bp->link.status.Event.Prio = (pwr_tUInt8) ip->prio;
    bp->link.status.Event.Status = mh_mEventStatus_Block;
    bp->targetId.Nix = l.head.nix;
    bp->targetId.BirthTime = l.head.birthTime;
    break;
  case mh_eEvent_Reblock:
    bp->outunitUnblock = bp->outunitBlock;
    bp->outunitBlock = *ip;
    clock_gettime(CLOCK_REALTIME, &bp->outunitBlock.time);      /* Update block time */
    bp->link.status.Event.Prio = (pwr_tUInt8) ip->prio;
    bp->link.status.Event.Status = mh_mEventStatus_Block;
    break;
  case mh_eEvent_Unblock:
    bp->outunitUnblock = *ip;
    clock_gettime(CLOCK_REALTIME, &bp->outunitUnblock.time);   /* Update unblock time */
    bp->link.status.All = 0;
    break;
  default:
    errh_Error("outunitBlock, program error (1), event: %d", bp->link.event);
    break;
  }

  sts = gdh_SetAlarmBlockLevel(ip->object, bp->link.status.All);
  ep =  eventListInsert(bp->link.event, NULL, (sActive *) bp);
  switch (bp->link.event) {
  case mh_eEvent_Unblock:
    activeListRemove((sActive *) bp);
    blockListFree(bp);
    break;
  case mh_eEvent_Block:
    bp->targetId.Idx = bp->link.idx;
    activeListInsert((sActive *) bp, ep, mh_eSource_Outunit);
    break;
  case mh_eEvent_Reblock:
    bp->targetId.Idx = bp->link.idx;
    activeListRemove((sActive *) bp);
    activeListInsert((sActive *) bp, ep, mh_eSource_Outunit);
    break;
  default:
    errh_Error("outunitBlock, program error (2), event: %d", bp->link.event);
    break;
  }

  eventToOutunits(ep);
  saveBlockList();
}

static void
outunitInfo (
  mh_sHead *hp,
  sOutunit *op
)
{
  mh_sOutunitInfo *ip = (mh_sOutunitInfo*) (hp + 1);

  op->outunit = hp->outunit;
  op->type = ip->type;
  if (op->selGen != ip->selGen) {
    op->selSize = MIN(ip->selSize, mh_cSelLSize);
    if (ip->selSize > 0)
      memcpy(&op->sel_l[0], ip + 1, op->selSize * sizeof(mh_sSelL));
    op->selGen = ip->selGen;
    op->eventGen++;
#if 0
    errh_Info("outunitInfo op->eventGen++ <%d>\n(qid: %s, oid: %s)",
      op->eventGen, qcom_QidToString(NULL, &op->link.qid, 1),
      cdh_ObjidToString(NULL, op->outunit, 1)
    );
#endif
    op->maxIdx = op->syncedIdx = op->eventIdx = 0;
    op->check = 0;
  }

  sendToOutunit(op, mh_eMsg_Sync, 0, NULL, 0);
  sendEventListToOutunit(op);

}

static void
outunitDisconnect (
  mh_sHead *hp,
  sOutunit *op
)
{

  outunitLog(op, "Outunit disconnected");

  LstRem(&op->outunit_l);   /* Remove from outunit list */
  LstNul(&op->outunit_l);
  LstRem(&op->link.proc_l); /* Remove from process link */
  LstNul(&op->link.proc_l);
  free(op);       /* Free control block */
}

static void
outunitLog (
  sOutunit *op,
  char *text
)
{

  errh_Info("%s (qid: %s, oid: %s)",
    text, qcom_QidToString(NULL, &op->link.qid, 1),
    cdh_ObjidToString(NULL, op->outunit, 1)
  );
}

static void
outunitSync (
  mh_sHead *hp,
  sOutunit *op
)
{

  if (hp->eventGen != op->eventGen)
    return;

  if (op->eventIdx == op->syncedIdx)
    return;

  if (op->eventIdx == hp->eventIdx) {
    if (op->check) {
#if 0
  char s1[256] = "";
  char s2[256] = "";
      errh_Info("Now synced: op->eventIdx: %d != hp->eventIdx: %d, op->syncedIdx: %d\nOutunit %s, %s", 
	op->eventIdx, hp->eventIdx, op->syncedIdx,
	qcom_QidToString(s1, &op->link.qid, 1),
	cdh_ObjidToString(s2, op->outunit, 1));
#endif
      op->check = 0;
    }
    op->syncedIdx = hp->eventIdx;
  }
  sendEventListToOutunit(op);
}

static void
procDown ( 
  qcom_sAid *aid
)
{
  LstLink(sProcLink) *pl;

  for (pl = LstFir(&l.proc_l); pl != LstEnd(&l.proc_l); pl = LstNex(pl))
    if (LstObj(pl)->aid.nid == aid->nid && LstObj(pl)->aid.aix == aid->aix) {
      switch (LstObj(pl)->source) {
      case mh_eSource_Outunit:
        outunitAborted((sOutunit *) LstObj(pl));
        break;
      case mh_eSource_Application:
#if 0
        ApplAborted((sAppl *) LstObj(pl));
#endif
        break;
      default:
	errh_Error("procDown, programming error, source: %d", LstObj(pl)->source);
        break;
      }
      return;
    }
}

#define SET_TIMEOUT              \
  if (l.timerActive) {               \
    tmo = l.timerTime * 1000.; /* Milli seconds */ \
    clock_gettime(CLOCK_REALTIME, &curTime);    \
    time_Aadd(&next_tmo, &curTime, &l.timerTimeDelta); \
  } else                \
    tmo = qcom_cTmoEternal;


static void
receive (
 qcom_sQid myQ
)
{
  pwr_tStatus sts;
  static char mp[2000];
  qcom_sGet get;
  int tmo;
  pwr_tTime curTime, next_tmo;
  pwr_tDeltaTime diff;

  SET_TIMEOUT

  while (1) {
    get.maxSize = sizeof(mp);
    get.data = mp;
    qcom_Get(&sts,&myQ, &get, tmo);
    if (sts == QCOM__TMO || sts == QCOM__QEMPTY) {
      timeOut();
      SET_TIMEOUT
    } else if (EVEN(sts))
      errh_Error("Receive: qcom_Get, timout: %d\n%m", tmo, sts);
    else {
      handleMessage(&get);

      if (!l.timerActive) {
        tmo = qcom_cTmoEternal;
        continue;
      }

      clock_gettime(CLOCK_REALTIME, &curTime);
      if (time_Acomp(&curTime, &next_tmo) > 0) {
        timeOut();
        SET_TIMEOUT
      } else {
        time_Asub((pwr_tTime *)&diff, &next_tmo, (pwr_tDeltaTime *)&curTime);
        tmo = diff.tv_sec * 1000 + diff.tv_nsec / 1000000;
        /* Never wait more than l.timerTime */
        tmo = MIN(l.timerTime * 1000, tmo); 
      }
    }
    aproc_TimeStamp();
  }    
}

static void
reInitSupList ()
{
  pwr_tStatus sts;
  LstLink(sSupActive) *sl;
  sSupActive *sp;
  sActive *ap;


  for (sl= LstFir(&l.sup_l); sl != LstEnd(&l.sup_l); sl = LstNex(sl))
    LstObj(sl)->found = FALSE;

  sts = reInitSupListClass(pwr_cClass_ASup);
  if (EVEN(sts))
    errh_Error("Reinitialize list of ASup's\n%m", sts);

  sts = reInitSupListClass(pwr_cClass_DSup);
  if (EVEN(sts))
    errh_Error("Reinitialize list of DSup's\n%m", sts);

  sts = reInitSupListClass(pwr_cClass_NodeLinkSup);
  if (EVEN(sts))
    errh_Error("Reinitialize list of LinkSup's\n%m", sts);

  sts = reInitSupListClass(pwr_cClass_CycleSup);
  if (EVEN(sts))
    errh_Error("Reinitialize list of CycleSup's\n%m", sts);

  for (sl= LstFir(&l.sup_l); sl != LstEnd(&l.sup_l); sl = LstNex(sl)) {
    sp = LstObj(sl);
    if(!sp->found) {
      if ((ap = activeListGet(sp->link.idx)) != NULL)
        cancelAlarm(ap, cText_Restart);

      sl = LstPre(&sp->sup_l);
      LstRem(&sp->sup_l);
      LstNul(&sp->sup_l);
      l.emon->BlockMaxCount--;
      gdh_DLUnrefObjectInfo(sp->supDlid);
      if (sp->agent == mh_eAgent_MH) {
        gdh_DLUnrefObjectInfo(sp->attrDlid);
        if(LstInl(&sp->timer_l)) {
          LstRem(&sp->timer_l);
          LstNul(&sp->timer_l);
          l.emon->EventLastIdx--;
        }
        LstRem(&sp->detect_l);
        LstNul(&sp->detect_l);
        l.emon->EventFirstIdx--;
      }
      free(sp);
    }
  }
}

static pwr_tStatus
reInitSupListClass(
  pwr_tClassId cid
)
{
  pwr_tStatus sts;
  LstLink(sSupActive) *sl;
  LstLink(sSupActive) *dl;
  sSupActive *sp;
  pwr_tObjid oid;

  dl = LstEnd(&l.detect_l);
  sl = LstEnd(&l.sup_l);

  /* Loop through objects in class list.  */

  for (
    sts = gdh_GetClassList(cid, &oid);
    ODD(sts);
    sts = gdh_GetNextObject(oid, &oid)
  ) {
    if ( oid.vid < cdh_cUserVolMin)
      continue;

    if ((sp = supListGet(oid)) == NULL) {
      sts = initSupActiveCB(oid, cid, &sp, 1, 1);
      if (ODD(sts)) {
        sl = LstIns(sl, sp, sup_l);
        l.emon->BlockMaxCount++;
        sp->found = TRUE;
        if (sp->agent == mh_eAgent_MH) {
          dl = LstIns(dl, sp, detect_l);
          l.emon->EventFirstIdx++;
        }
      }
    } else {
      updateSupActive(sp);
    }
  }

  if (sts == GDH__NO_TYPE || sts == GDH__NOSUCHOBJ) sts = MH__SUCCESS;

  return(sts);

}

static pwr_tBoolean
reSendEventToOutunit (
  sOutunit *op,
  sEventTab *etp
)
{
  pwr_tStatus sts;
  pwr_tUInt32 size;
  uEvent event;
  sSupActive *sp;
  sActive *ap = etp->ap;
  mh_mEventStatus Status;

  Status = 0;

  if (etp->ap == NULL) {
    errh_Error("reSendEventToOutunit: etp->ap == NULL");
    errh_Error("idx: %d, event: %d, ep: %d",
      etp->idx, etp->event, etp->ep);
    exit(2);
  }

  switch (ap->source) {
  case mh_eSource_Scanner:
    sp = (sSupActive*) ap;
    formatSupEvent(etp->event, NULL, (sSupActive*) ap, &event, &size);
    Status = sp->sup->AlarmStatus.All;
    break;
  case mh_eSource_Application: 
  case mh_eSource_Handler: 
    formatApplEvent(etp->event, NULL, (sApplActive*) ap, &event, &size);
    Status = ap->status.All;
    break;
  case mh_eSource_Outunit: 
    formatOutunitEvent(etp->event, NULL, (sBlock*) ap, &event, &size);
    Status = ap->status.All;
    break;
  default:
    errh_Error("reSendEventToOutunit, programming error");
    errh_Error("ap: %d", ap);
    errh_Error("source %d", ap->source);
    errh_Error("idx: %d, event: %d, ep: %d",
      etp->idx, etp->event, etp->ep);
    return FALSE;
    break;
  }

  switch (event.message.Info.EventType) {
  case mh_eEvent_Alarm:
  case mh_eEvent_Info:
    event.message.Status = Status;
    break;
  default:
    break;
  }

  sts = sendToOutunit(op, mh_eMsg_Event, etp->event, &event.message, size);

  return ODD(sts);
}

static void
saveBlockList ()
{
  unsigned long size;
  sSaveBlock *sp;
  LstLink(sBlock) *bl;
  sBlock *bp;

  if (l.blockDb == NULL)
    return;

  if (l.emon->BlockCount > l.blockSaveSize / sizeof(sSaveBlock)) {
    free(l.blockSave);
    l.blockSaveSize = 2 * l.emon->BlockCount;
    l.blockSave = calloc(l.blockSaveSize, sizeof(sSaveBlock));
    if (l.blockSave == NULL) {
      errh_Error("%s\n%m", "SaveBlockList", MH__NOSPACE);
      return;
    }
  }

  for (
    bl = LstFir(&l.block_l), sp = l.blockSave, size = 0;
    bl != LstEnd(&l.block_l);
    bl = LstNex(bl), sp++, size += sizeof(sSaveBlock)
  ) {
    bp = LstObj(bl);
    sp->outunitBlock = bp->outunitBlock;
    sp->targetId = bp->targetId;
  }

  l.blockDb = mh_BlockDbPut(l.blockDb, size, (char *)l.blockSave);  

}

static void
scanDetectList ()
{
  LstLink(sSupActive) *dl;

  for (dl = LstFir(&l.detect_l); dl != LstEnd(&l.detect_l); dl = LstNex(dl)) {
    sSupActive *sp = LstObj(dl);
    if (sp->detect_exec != NULL)
      sp->detect_exec(sp);
  }
}

static void
scanSupList ()
{
  pwr_tStatus sts;
  mh_uEventInfo AlarmVisibility;
  sSupActive *sp;
  LstLink(sSupActive) *sl;

  for (sl = LstFir(&l.sup_l); sl != LstEnd(&l.sup_l); sl = LstNex(sl)) {
    sp = LstObj(sl);
    if (l.newBlock) {
      sts = gdh_GetAlarmInfo(sp->link.object, NULL, NULL, NULL, NULL,
        &AlarmVisibility.All);
      if (sp->alarmVisibility.All != AlarmVisibility.All) {
        if (sp->sup->EventPriority <= AlarmVisibility.Event.Prio) {
          sp->sup->Blocked = TRUE;
          if (sp->sup->AlarmStatus.Event.Status != 0) {
            cancelAlarm((sActive *)sp, cText_Block);
          }
        } else {
          sp->sup->Blocked = FALSE;
        }
        sp->alarmVisibility.All = AlarmVisibility.All;
      }
    }
    if (!sp->sup->Blocked) {
      if (sp->sup->DetectSend) {
        sp->sup->DetectSend = FALSE;
        sp->link.eventFlags = sp->sup->EventFlags;

        switch (sp->link.event) {
        case mh_eEvent_Info:
          handleInfo(sp);
          break;
        case mh_eEvent_Alarm:
        default:
          handleAlarm(sp);
          break;
        }
      } else if (sp->sup->ReturnSend) {
        sp->sup->ReturnSend = FALSE;
        handleReturn(sp);
      }
    }
  }
  l.newBlock = FALSE;
}

static void
scanTimerList ()
{
  LstLink(sSupActive) *sl, *nsl;
  sTimer *tp;

  for (sl = LstFir(&l.timer_l); sl != LstEnd(&l.timer_l); sl = nsl) {
    tp = LstObj(sl)->timer;
    nsl = LstNex(sl);
    if (tp->TimerCount <= 1 || !tp->TimerFlag) {
      tp->TimerCount = 0;
      tp->TimerFlag = FALSE;
      LstRem(sl);
      LstNul(sl);
      l.emon->EventLastIdx--;
    } else {
      tp->TimerCount--;
    }
  }
}

static void
sendEventListToOutunit (
  sOutunit		*op
)
{
  pwr_tStatus           sts;
  sEvent		*ep;
  sActive		*ap;
  sEventTab		*etp;
  pwr_tBoolean		ok;

  if (op->maxIdx < l.event_l->oldIdx) {
    op->eventGen++;
#if 0
    errh_Info("sendEventListToOutunit op->eventGen++ <%d>\n(qid: %s, oid: %s)",
      op->eventGen, qcom_QidToString(NULL, &op->link.qid, 1),
      cdh_ObjidToString(NULL, op->outunit, 1)
    );
#endif
    op->maxIdx = l.event_l->idx;
    op->syncedIdx = op->eventIdx = 0;
    op->check = 0;
  }

  /* !!! To do !!! If there is a big gap between last synced and
     next event, then we have to call a number of unnecessary
     isForOutunit.  */

  for (
    etp = tree_FindSuccessor(&sts, l.eventTab, &op->syncedIdx);
    etp != NULL;
    etp = tree_Successor(&sts, l.eventTab, etp)
  ) {
    if ((ep = etp->ep) != NULL) {
      if (isForOutunit(op, ep->outunit, ep->object, ep->objName, ep->msg.info.EventFlags, ep->local))
	break;
    } else if ((ap = etp->ap) != NULL) {
      if (isForOutunit(op, ap->outunit, ap->object, ap->objName, ap->eventFlags, ap->local))
	break;
    } else {
      errh_Error("ap == NULL && ep == NULL");
    }
  }

  if (etp == NULL)
    return;
    
  if (etp->ep != NULL) {
    ok = sendEventToOutunit(op, etp);
  } else {
    ok = reSendEventToOutunit(op, etp);
  }

  if (ok) {
    op->eventIdx = etp->idx;	/* Message was sent, update last sent index */
    op->maxIdx = l.event_l->idx;
    op->check = 0;
  }
}

static pwr_tBoolean
sendEventToOutunit (
  sOutunit	*op,
  sEventTab	*etp
)
{
  pwr_tStatus	sts;
  sEvent	*ep = etp->ep;

  sts = sendToOutunit(op, mh_eMsg_Event, ep->msg.info.EventType, &ep->msg, ep->msgSize);

  if (EVEN(sts)) {
    errh_Error("%s\n%m", "sendEventToOutunit", sts);
    return NO;
  }

  return YES;
}

static pwr_tStatus
sendMessage (
  mh_eMsg	type,
  qcom_sQid	*qid,
  co_sPlatform	*platform,
  void		*msg,
  pwr_tUInt32	msgSize
)
{
  pwr_tStatus	sts;
  char		*mp;
  mh_sHead	*hp;
  unsigned int  size = sizeof(l.head) + msgSize;
  qcom_sQid	target;
  int		prio = 0;

  mp = calloc(1, size);
  if (mp == NULL) {
    errh_Error("SendMessage: Error malloc");
    return MH__QCOMALLOCMSG;
  }

  memcpy(mp, &l.head, sizeof(l.head));
  hp = (mh_sHead*) mp;
  hp->type     = type;
  hp->eventGen = 0;    
  hp->eventIdx = 0;    
  hp->ackGen   = 0;    
  hp->selGen   = 0;    

  memcpy(mp + sizeof(l.head), msg, msgSize);

  if (qid == NULL) {
    target = mh_cProcAllOutunits;
  } else {
    target = *qid;
  }

  sts = mh_NetSendMessage(&target, platform, prio, 0, (mh_sHead *)mp, size);
  if (EVEN(sts) && sts != QCOM__LINKDOWN) {
    errh_Error("%s\n%m", "SendMessage: mh_NetSendMessage", sts);
  }

  free(mp);

  return sts;
}

static pwr_tStatus
sendToOutunit (
  sOutunit	*op,
  mh_eMsg	type,
  int		subType,
  void		*msg,
  int		msgSize
)
{
  pwr_tStatus	sts;
  char		*mp;
  mh_sHead	*hp;
  unsigned int  size = sizeof(l.head) + msgSize;
  int		prio = 0;

  if (!op->linkUp)
    return QCOM__LINKDOWN;

  mp = malloc(size);
  if (mp == NULL) {
    errh_Error("SendMessage: Error malloc");
    return MH__QCOMALLOCMSG;
  }

  memcpy(mp, &l.head, sizeof(l.head));
  hp = (mh_sHead*) mp;

  memcpy(mp + sizeof(l.head), msg, msgSize);

  hp->type     = type;
  hp->eventGen = op->eventGen;    
  hp->eventIdx = op->eventIdx;    
  hp->ackGen   = op->ackGen;    
  hp->blockGen = op->blockGen;    
  hp->selGen   = op->selGen;    

  sts = mh_NetSendMessage(&op->link.qid, &op->link.platform, prio, subType, (mh_sHead *)mp, size);
  if (EVEN(sts) && sts != QCOM__LINKDOWN) {
    errh_Error("%s\n%m", "SendMessage: mh_NetSendMessage", sts);
  }

  free(mp);

  return sts;
}

static void
setTimerActive(
  int timerIdx,
  pwr_tBoolean active
)
{
  int i;

  l.timers[timerIdx].active = active;
  l.timers[timerIdx].count = 0;

  for (i = 0, l.timerActive = FALSE; i < cTimers; i++)
    l.timerActive |= l.timers[i].active;

}

/* This routine search suplist for a sup object.  */

static sSupActive *
supListGet (
  pwr_tObjid oid
)
{
  LstLink(sSupActive) *sl;

  for (sl = LstFir(&l.sup_l); sl != LstEnd(&l.sup_l); sl = LstNex(sl)) {
    sSupActive *sp = LstObj(sl);

    if (cdh_ObjidIsEqual(sp->link.supObject, oid)) 
      return sp;
  }

  return NULL;
}

static void
timeOut()
{
  if (l.timers[cDetectIdx].active 
      && ++l.timers[cDetectIdx].count >= l.timers[cDetectIdx].multiple
  ) {
    l.timers[cDetectIdx].count = 0;
    if (l.supListState == eSupListState_Scan) {
      scanTimerList();
      scanDetectList();
    } else
      setTimerActive(cDetectIdx, FALSE);
  }


  if (l.timers[cMessageIdx].active 
      && ++l.timers[cMessageIdx].count >= l.timers[cMessageIdx].multiple
  ) {
    l.timers[cMessageIdx].count = 0;

    if (l.supListState == eSupListState_Scan) {
      scanSupList();
    } else if (l.supListState == eSupListState_Init) {
      if (qcom_EventMask(NULL, &qcom_cQini) & ini_mEvent_newPlcStartDone) {
        initSupList();
        if (!LstEmp(&l.sup_l)) {
          l.supListState = eSupListState_Scan;
          if (!LstEmp(&l.detect_l)) {
            scanTimerList();
            scanDetectList();
            setTimerActive(cDetectIdx, TRUE);
          }
          scanSupList();
        } else {
          l.supListState = eSupListState_NoSup;
          setTimerActive(cMessageIdx, FALSE);
        }
      } 
    } else
      setTimerActive(cMessageIdx, FALSE);      
  }


  if (l.timers[cBlockDbIdx].active 
      && ++l.timers[cBlockDbIdx].count >= l.timers[cBlockDbIdx].multiple
  ) {
    l.timers[cBlockDbIdx].count = 0;
     if (l.blockDb == NULL) {
      l.blockDb = mh_BlockDbOpen(l.blockDbName, NULL);
      if (l.blockDb != NULL)
        saveBlockList();
    }
  }

  if (l.timers[cOutunitIdx].active 
      && ++l.timers[cOutunitIdx].count >= l.timers[cOutunitIdx].multiple
  ) {
    l.timers[cOutunitIdx].count = 0;
    checkOutunits();
  }

  pwrs_Node_Exec();
}

static void
timerIn (
  sSupActive *s,
  sTimer *t
)
{

  t->TimerCount = t->TimerTime / l.detectTimerTime;
  if (!t->TimerFlag && t->TimerCount > 0) {
    if (!LstInl(&s->timer_l)) {
      LstIns(&l.timer_l, s, timer_l);
      l.emon->EventLastIdx++;
    }
    t->TimerFlag = TRUE;
  }
}

static void
updateAlarm (
  sActive *ap,
  sEvent *ep
)
{
  sSupActive *sp;
  sApplActive *aap;

  sp = (sSupActive*) ap;
  aap = (sApplActive*) ap;

  switch (ep->event) {
  case mh_eEvent_Alarm:
    ap->status.Event.Status = mh_mEventStatus_NotRet |
      mh_mEventStatus_NotAck;
    if (ap->source == mh_eSource_Scanner) {
      ap->status.Event.Prio = sp->sup->EventPriority;
      if (sp->sup->EventFlags & mh_mEventFlags_Returned)
        ap->status.Event.Status &= ~mh_mEventStatus_NotRet;
    } else {
      ap->status.Event.Prio = aap->message.EventPrio;
      if (aap->message.EventFlags & mh_mEventFlags_Returned)
        ap->status.Event.Status &= ~mh_mEventStatus_NotRet;
    }

    ep->msg.message.Status = ap->status.All;
    break;
  case mh_eEvent_Info:
    ap->status.Event.Status = mh_mEventStatus_NotRet |
      mh_mEventStatus_NotAck;
    if (ap->source == mh_eSource_Scanner) {
      ap->status.Event.Prio = 0;
      if (sp->sup->EventFlags & mh_mEventFlags_Returned)
        ap->status.Event.Status &= ~mh_mEventStatus_NotRet;
    } else {
      ap->status.Event.Prio = 0;
      if (aap->message.EventFlags & mh_mEventFlags_Returned)
        ap->status.Event.Status &= ~mh_mEventStatus_NotRet;
    }
    ep->msg.message.Status = ap->status.All;
    break;
  case mh_eEvent_Ack:
    ap->status.Event.Status &= ~mh_mEventStatus_NotAck;
    if (ap->source == mh_eSource_Scanner)
      sp->sup->Acked = TRUE; /* Set Acked flag in sup object TRUE */
    if (ap->idx >= l.event_l->oldIdx) {
      /* Alarm message still in event list, update status */
      l.event_l->list[ap->idx % l.event_l->size].msg.message.Status =
        ap->status.All;
    }
    break;
  case mh_eEvent_Return:
    ap->status.Event.Status &= ~mh_mEventStatus_NotRet;
    if (ap->idx >= l.event_l->oldIdx) {
      /* Alarm message still in event list, update status */
      l.event_l->list[ap->idx % l.event_l->size].msg.message.Status =
        ap->status.All;
    }
    break;
  default:
    break;
  }

  if ((ap->status.Event.Status &
    (mh_mEventStatus_NotRet | mh_mEventStatus_NotAck)) == 0
  ) { /* The alarm is acked and unactive, remove it from alarm list */
    ap->status.All = 0;
    updateAlarmInfo(ap);
    if (ap->source == mh_eSource_Scanner) {
      sp->sup->DetectCheck = TRUE;   /* Activate detection */
      sp->sup->AlarmCheck = TRUE;    /* Activate alarm supervision */
      sp->sup->AlarmStatus.All = ap->status.All;
      if (sp->supType == mh_eSupType_Cycle) {
	pwr_sClass_CycleSup *cp = (pwr_sClass_CycleSup *) sp->op;
	cp->DelayNoted = FALSE;
	cp->Delayed = FALSE;
      }
    }
    activeListRemove(ap);
  } else {
    if (ap->source == mh_eSource_Scanner)
      sp->sup->AlarmStatus.All = ap->status.All;
    else
      aap->message.EventStatus = (mh_mEventStatus) ap->status.Event.Status;
    updateAlarmInfo(ap);
  }
}

static void
updateAlarmInfo (
  sActive *iap
)
{
  sActive *ap;
  LstLink(sActive) *al;
  mh_uEventInfo maxAlarm;

  if (cdh_ObjidIsEqual(iap->object,pwr_cNObjid)) /* No object associated with this alarm */
    return;

  /* Search alarm list for ocurrence of object */
  maxAlarm.All = 0;
  for (al = LstFir(&l.active_l); al != LstEnd(&l.active_l) ; al = LstNex(al)) {
    ap = LstObj(al);
    if (cdh_ObjidIsEqual(iap->object, ap->object))
      if (ap->event == mh_eEvent_Alarm)
        maxAlarm.All = MAX(maxAlarm.All, ap->status.All);
  }

  gdh_SetAlarmLevel(iap->object, maxAlarm.All);
}

static void
updateSupActive (
  sSupActive	*sp
)
{
  pwr_tStatus	sts;
  sActive	*ap;
  sASup		*asp;
  sDSup		*dsp;
  sSup		*sup;
  mh_eAgent	agent;
  pwr_tClassId	cid;
  pwr_tBoolean	newAttribute;
  LstLink(sSupActive) *dl = LstEnd(&l.detect_l);

  /* Get pointer to supervisory object */

  if (sp->supType == mh_eSupType_Analog) {
    sts = gdh_ObjidToPointer(sp->link.supObject, (pwr_tAddress *)&asp);
    sup =  &asp->Sup;
    cid = pwr_cClass_ASup;
  } else if (sp->supType == mh_eSupType_Digital) {
    sts = gdh_ObjidToPointer(sp->link.supObject, (pwr_tAddress *)&dsp);
    sup = &dsp->Sup;
    cid = pwr_cClass_DSup;
  } else if (sp->supType == mh_eSupType_Link) {
    return; /* Supervised attribute can't change. */
  } else if (sp->supType == mh_eSupType_Cycle) {
    return; /**to do**/
  }

  if (EVEN(sts)) {
    errh_Error("%s\n%m", "Couldn't get pointer to sup object", sts);
    return;
  }

  /* Compare attribute  */
  if (memcmp(&sup->Attribute, &sp->attribute, sizeof(sp->attribute)) == 0) {   
    newAttribute = FALSE;
  } else {
    newAttribute = TRUE;
    if ((ap = activeListGet(sp->link.idx)) != NULL) 
      cancelAlarm(ap, cText_Restart);
  }

  gdh_DLUnrefObjectInfo(sp->supDlid);
  if (sp->agent == mh_eAgent_MH)
    gdh_DLUnrefObjectInfo(sp->attrDlid);
  agent = sp->agent;
  initSupActiveCB(sp->link.supObject, cid, &sp, 0, newAttribute);
  sp->found = TRUE;
  if (sp->agent != agent) {
    if (sp->agent == mh_eAgent_MH) {
      LstIns(dl, sp, detect_l);
      l.emon->EventFirstIdx++;
    } else {
      LstRem(&sp->detect_l);
      LstNul(&sp->detect_l);
      l.emon->EventFirstIdx--;
      if (LstInl(&sp->timer_l)) {
        LstRem(&sp->timer_l);
        LstNul(&sp->timer_l);
        l.emon->EventLastIdx--;
      }
    }
  }
}
