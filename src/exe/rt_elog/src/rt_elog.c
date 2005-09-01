/* 
 * Proview   $Id: rt_elog.c,v 1.4 2005-09-01 14:57:48 claes Exp $
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

/* rt_eventlogger.c
   Receives events from the local MH and stores them in a
   historical event log database on disk.  */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <starlet.h>
#include <limits.h>
#include <time.h>
#include <sql_literals.h>
#include <sql_sqlca.h>
#include <descrip.h>
#include "co_time.h"
#include "co_tree.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_mh_def.h"
#include "rt_mh_outunit.h"

#define Log_Error(a, b) errh_Error("%s\n%m",b, a)
#define Log(b) errh_Info(b)
#define Log_Error_Exit(a, b) {Log_Error(a, b); exit(a);}
#define Log_Error_Return(a, b) {Log_Error(a, b); return (a);}
#define If_Error_Log(a, b) if ((a & 1) != 1) Log_Error(a, b)
#define If_Error_Log_Return(a, b) if ((a & 1) != 1) Log_Error_Return(a, b)
#define If_Error_Log_Exit(a, b) if ((a & 1) != 1) Log_Error_Exit(a, b)

#define dVarChar(size) struct {unsigned short len; char data[size];}


typedef struct s_Event sEvent;
typedef struct s_Store sStore;
typedef struct s_Node  sNode;
typedef struct s_HelCB sHelCB;

LstType(sStore);

struct s_Event {
  pwr_tInt64      StoreIdx;
  pwr_tVaxTime	  BirthVaxTime;
  pwr_tNodeId     Nid;
  pwr_tUInt32	  Idx;
  pwr_tVaxTime	  EventVaxTime;
  pwr_tVaxTime	  DetectVaxTime;
  pwr_tObjid	  Object;
  pwr_tObjid	  SupObject;
  pwr_tUInt32	  EventFlags;
  dVarChar(80)	  EventName;
#if defined __Alpha_AXP
  short int	  filler1;
#endif
  pwr_tUInt32	  EventType;
  pwr_tUInt32	  TargetIdx;
  pwr_tObjid	  Outunit;
  dVarChar(80)	  EventText;
#if defined __Alpha_AXP
  short int	  filler2;
#endif
  pwr_tUInt32	  EventPrio;
  pwr_tUInt32	  Status;
  pwr_tUInt32	  SupType;
  pwr_tFloat32	  AValue;
  pwr_tUInt32	  DValue;
  pwr_tFloat32	  CtrlLimit;
  pwr_tFloat32	  Hysteres;
  pwr_tUInt32	  High;
  dVarChar(16)	  Unit;
#if defined __Alpha_AXP
  short int	  filler3;
#endif
  pwr_tUInt32	  Missing;
};

struct s_Store {
  LstLink(sStore) StoreL;
  sEvent Event; 
}; 

struct s_Node {
  tree_sNode 	TreeNode;
  pwr_tVaxTime 	BirthVaxTime;
  pwr_tNodeId 	Nid;
  pwr_tUInt32 	Idx;
};

struct s_HelCB {
  tree_sTable *Tree;
  int MaxStoreLSize;
  int StoreLSize;
  LstHead(sStore) FreeL;
  LstHead(sStore) StoreL;
  int Cardinality;
  int MaxCardinality;
  pwr_tInt32 ScanTime;
  pwr_tNodeId Nid;
  pwr_tInt64 StoreIdx;
};

static int  CompTime(
  pwr_tVaxTime,
  pwr_tVaxTime
);
static void Init(
);
static pwr_tStatus Insert(
  mh_sMsgInfo *ip
);
static void Store (
  int Count
);
static sNode *InsertNode(
  pwr_tNodeId Nid
);
static void Purge(
  int Count
);
static sStore *CopyEvent(
  mh_sMsgInfo *ip
);
sStore *MissingEvent(
  mh_sMsgInfo *ip,
  pwr_tUInt32 Idx
);
sStore *AllocEvent();
void FreeEvent(
  sStore *sp
);
static void LogSqlError(
  struct SQLCA *sqlca,
  char *msg
);

void IncStoreIdx (
  pwr_tInt64 *Idx,
  int Inc
);

sHelCB lHelCB;


extern int mh_HeldbCommit ();
extern int mh_HeldbEventCardinality ();
extern int mh_HeldbFindOldest ();
extern int mh_HeldbInsertEvent ();
extern int mh_HeldbLatestEvent ();
extern int mh_HeldbLatestIncarnation ();
extern int mh_HeldbMaxStoreIdx ();
extern int mh_HeldbMinStoreIdx ();
extern int mh_HeldbPurge ();
extern int mh_HeldbRollback ();
extern int mh_HeldbSetTxro ();
extern int mh_HeldbSetTxrw ();


int 
main ()
{
  pwr_tUInt32 sts;
  mh_sMsgInfo ip;
  pwr_tObjid oid;

  errh_Init("pwr_elog", errh_eAnix_elog);

  memset(&lHelCB, 0, sizeof(lHelCB));

  /* Declare process */



  sts = gdh_Init("pwr_elog");
  If_Error_Log_Exit(sts, "gdh_Init");

  Init();

  oid.vid = lHelCB.Nid;
  oid.oix = pwr_cNVolumeId;

  sts = mh_OutunitConnect(
    oid, 
    mh_eOutunitType_Logger, 
    mh_mOutunitFlags_ReadWait, 
    (mh_cbOutunitAck)Insert, 
    (mh_cbOutunitAlarm)Insert, 
    (mh_cbOutunitBlock)Insert, 
    (mh_cbOutunitCancel)Insert, 
    NULL, 
    NULL, 
    (mh_cbOutunitInfo)Insert,
    (mh_cbOutunitReturn)Insert
  );
  If_Error_Log_Exit(sts, "mh_OutunitConnect");

  sts = mh_OutunitSetTimeout(lHelCB.ScanTime);

  for (;;) {
    sts = mh_OutunitReceive();
    if (EVEN(sts) && sts != MH__TMO)
      Log_Error(sts, "mh_OutunitReceive");
    else if (lHelCB.StoreLSize > 0)
      Store(lHelCB.MaxStoreLSize / 10);

  }
}

static void
Init ()
{
  pwr_tStatus sts;
  sStore *sp;
  LstHead(sStore) *sl;
  sNode *np;
  int i;
  struct SQLCA	sqlca;
  char msg[80];
  pwr_tObjid	MHObjId,NodeObjId;
  pwr_tString80 Name;
  pwr_sAttrRef	AttrRef; 
  pwr_tDlid	DLId;
  pwr_tVaxTime	BirthVaxTime;
  pwr_sClass_MessageHandler *MH;
  
  sts = gdh_GetNodeIndex(&lHelCB.Nid);
  If_Error_Log_Exit(sts, "gdh_GetNodeIndex");

  sts = gdh_GetClassList(pwr_cClass_MessageHandler, &MHObjId);
  If_Error_Log_Exit(sts, "Couldn't find message handler object");

  AttrRef.Objid = MHObjId;
  AttrRef.Body = 0;
  AttrRef.Offset = 0;
  AttrRef.Size = sizeof(pwr_sClass_MessageHandler);
  AttrRef.Flags.m = 0;	    
  sts = gdh_DLRefObjectInfoAttrref(&AttrRef, (pwr_tAddress *)&MH, &DLId);
  If_Error_Log_Exit(sts,"Couldn't get direct link to message handler object");

  if (MH->EventLogSize == 0) {
    Log("EventLogSize = 0, no event logger will run on this node.");
    errh_SetStatus( pwr_cNStatus);
    exit(1);
  }

  lHelCB.MaxCardinality = MH->EventLogSize;
  lHelCB.MaxStoreLSize = 1000;
  lHelCB.ScanTime = 5000; /* 5 seconds */
  lHelCB.StoreIdx.high = 0;
  lHelCB.StoreIdx.low = 0;
  
  sp = (sStore *) calloc(lHelCB.MaxStoreLSize, sizeof(sStore));
  if (sp == NULL)
    Log_Error_Exit(MH__NOSPACE, "Couldn't allocate store list.");

  sl = &lHelCB.FreeL;
  LstIni(sl);
  for (i = 0; i < lHelCB.MaxStoreLSize; i++, sp++)
    LstIns(sl, sp, StoreL);

  LstIni(&lHelCB.StoreL);


  lHelCB.Tree = tree_CreateTable(&sts, sizeof(pwr_tNodeId), offsetof(sNode, Nid),
    sizeof(sNode), 10, tree_Comp_nid);

  if (lHelCB.Tree == NULL)
    Log_Error_Exit(MH__NOSPACE, "Couldn't allocate tree.");

  np = InsertNode(lHelCB.Nid);

  mh_HeldbSetTxro(&sqlca);
  if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

  /* Get number of events in Event table */
  mh_HeldbEventCardinality(&sqlca, &lHelCB.Cardinality);
  if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

  sprintf(msg, "Cardinality %d", lHelCB.Cardinality);
  Log(msg);

  mh_HeldbMaxStoreIdx(&sqlca, &lHelCB.StoreIdx);
  if (sqlca.SQLCODE != SQLCODE_NULLNOIND &&
    sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

  IncStoreIdx(&lHelCB.StoreIdx, 1);

  sprintf(msg, "StoreIdx %d, %u", lHelCB.StoreIdx.high,
    lHelCB.StoreIdx.low);
  Log(msg);

  mh_HeldbRollback(&sqlca);

  return;

error:
  LogSqlError(&sqlca, "Init");
  mh_HeldbRollback(&sqlca);
  exit(1);
  return;
}

/* Inserts an event into the que of its node. */

static pwr_tStatus
Insert (
  mh_sMsgInfo *ip
)
{
  sStore *sp;
  sNode *np;
  LstLink(sStore) *sl;
  int ctime;
  pwr_tVaxTime BirthVaxTime;
  pwr_tStatus sts;


  np = tree_Find(&sts, lHelCB.Tree, &ip->Id.Nix);
  if (np == NULL)
    np = InsertNode((pwr_tNodeId)ip->Id.Nix);
  
  time_PwrToVms(&ip->Id.BirthTime, &BirthVaxTime);
  if (ctime = CompTime(BirthVaxTime, np->BirthVaxTime) > 0) {
    /* Node is restarted */
    np->BirthVaxTime = BirthVaxTime;
    np->Idx = 0;
  }

  if (ip->Id.Idx > np->Idx) {
    sp = CopyEvent(ip);
    sl = LstEnd(&lHelCB.StoreL);
    LstIns(sl, sp, StoreL);
    lHelCB.StoreLSize++;
    if (ip->Id.Idx > np->Idx + 1) {
      sp = MissingEvent(ip, np->Idx + 1);
      sl = LstEnd(&lHelCB.StoreL);
      LstIns(sl, sp, StoreL);
      lHelCB.StoreLSize++;
    }
    np->Idx = ip->Id.Idx;
  }

  return MH__SUCCESS;
}

static sNode *
InsertNode (
  pwr_tNodeId Nid
) {
  pwr_tStatus sts;
  sNode *np;
  pwr_tVaxTime BirthVaxTime;
  struct SQLCA	sqlca;

  np = tree_Insert(&sts, lHelCB.Tree, &Nid);
  np->Nid = Nid;
     
  mh_HeldbSetTxro(&sqlca);
  if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

  /* Get latest event for this node */
  mh_HeldbLatestIncarnation(&sqlca, &np->Nid, &BirthVaxTime);
  if (sqlca.SQLCODE == SQLCODE_SUCCESS) {
    np->BirthVaxTime = BirthVaxTime;

    mh_HeldbLatestEvent(&sqlca, &np->Nid, &np->BirthVaxTime, &np->Idx);
    if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

    errh_Info("Node %s,  EventIdx: %d", cdh_NodeIdToString(NULL, np->Nid, 0, 0), np->Idx);
  } else if (sqlca.SQLCODE != SQLCODE_NULLNOIND) 
    goto error;

  mh_HeldbRollback(&sqlca);
  
  return np;

error:
  LogSqlError(&sqlca, "InsertNode");
  mh_HeldbRollback(&sqlca);
  exit(1);
}

static void
Store (
  int Count   /* Number of Events to store */
)
{
  int i;
  sStore *sp;
  sEvent *ep;
  sNode *np;
  LstLink(sStore) *sl;
  int Inserted = 0;
  struct SQLCA sqlca;
  char msg[80];
  
  Purge(Count);
  if (lHelCB.StoreLSize == 0)
    return;
  mh_HeldbSetTxrw(&sqlca);
  if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

  /* Write events to disk */
  for (
    i = 0, sl = LstFir(&lHelCB.StoreL);
    i < Count && sl != LstEnd(&lHelCB.StoreL);
    i++, sl = LstFir(&lHelCB.StoreL)
  ) {
    sp = LstObj(sl);

    sp->Event.StoreIdx = lHelCB.StoreIdx;

    /* Try insert */
    mh_HeldbInsertEvent(&sqlca, &sp->Event);
    switch (sqlca.SQLCODE) {
    case SQLCODE_SUCCESS:
      Inserted++;
      IncStoreIdx(&lHelCB.StoreIdx, 1);
      LstRem(sl);
      FreeEvent(sp);
      continue;
    case SQLCODE_NO_DUP:
      break;
    default:
      LogSqlError(&sqlca, "Store:mh_HeldbInsertEvent");
      return;
      break;
    }
  }

  mh_HeldbCommit(&sqlca);
  if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

  lHelCB.Cardinality += Inserted;

  return;

error:
  LogSqlError(&sqlca, "Store");
  mh_HeldbRollback(&sqlca);
  return;
}


/* If the cardinality of table Event is greater than maximum cardinality
   then delete the oldest event(s) in table Event.  */

static void
Purge (
  int Count   /* Number of Events to purge */
)
{
  struct SQLCA sqlca;
  int ToDelete = 0;
  int Deleted = 0;	/* Number of records deleted */
  char msg[80];
  pwr_tInt64 StoreIdx;
  
  StoreIdx.high = 0;
  StoreIdx.low = 0;

  if (lHelCB.Cardinality + Count > lHelCB.MaxCardinality) {
    mh_HeldbSetTxrw(&sqlca);
    if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

    mh_HeldbMinStoreIdx(&sqlca, &StoreIdx);
    if (sqlca.SQLCODE != SQLCODE_NULLNOIND &&
      sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

    ToDelete = lHelCB.Cardinality + Count - lHelCB.MaxCardinality;
    IncStoreIdx(&StoreIdx, ToDelete);

    mh_HeldbPurge(&sqlca, &StoreIdx);
    if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

    Deleted = sqlca.SQLERRD[2];

    if (Deleted != 0) {
      mh_HeldbCommit(&sqlca);
      if (sqlca.SQLCODE != SQLCODE_SUCCESS) goto error;

      lHelCB.Cardinality -= Deleted;
    } else {
      mh_HeldbRollback(&sqlca);
    }

  }

  return;

error:
  LogSqlError(&sqlca, "Purge");
  mh_HeldbRollback(&sqlca);
  return;
}

sStore *
CopyEvent (
  mh_sMsgInfo *ip
)
{
  sStore *sp;
  mh_sMessage *mp = (mh_sMessage*) ip;
  mh_sAck *ap = (mh_sAck*) ip;
  mh_sBlock *bp = (mh_sBlock*) ip;
  mh_sReturn *rp = (mh_sReturn*) ip;
  mh_uSupInfo *sip;

  sp = AllocEvent();

  sp->Event.Nid = ip->Id.Nix;
  time_PwrToVms(&ip->Id.BirthTime, &sp->Event.BirthVaxTime);
  sp->Event.Idx = ip->Id.Idx;
  sp->Event.Object = ip->Object;
  sp->Event.SupObject = ip->SupObject;
  sp->Event.EventFlags = ip->EventFlags;
  time_PwrToVms(&ip->EventTime, &sp->Event.EventVaxTime);
  strncpy(sp->Event.EventName.data, ip->EventName, sizeof(sp->Event.EventName.data) - 1);
  sp->Event.EventName.len = strlen(sp->Event.EventName.data);
  sp->Event.EventType = ip->EventType;
  sp->Event.EventPrio = ip->EventPrio;

  switch (ip->EventType) {
  case mh_eEvent_Alarm:
  case mh_eEvent_Info:
    sip = &mp->SupInfo;
    sp->Event.SupType = mp->SupInfo.SupType;
    strncpy(sp->Event.EventText.data, mp->EventText, sizeof(sp->Event.EventText.data) - 1);
    sp->Event.EventText.len = strlen(sp->Event.EventText.data);
    sp->Event.Status = mp->Status;
    break;
  case mh_eEvent_Ack:
    sip = &ap->SupInfo;
    sp->Event.SupType = ap->SupInfo.SupType;
    sp->Event.TargetIdx = ap->TargetId.Idx;
    time_PwrToVms(&ap->DetectTime, &sp->Event.DetectVaxTime);
    sp->Event.Outunit = ap->Outunit;
    break;
  case mh_eEvent_Cancel:
  case mh_eEvent_Return:
    sip = &rp->SupInfo;
    sp->Event.SupType = rp->SupInfo.SupType;
    sp->Event.TargetIdx = rp->TargetId.Idx;
    time_PwrToVms(&rp->DetectTime, &sp->Event.DetectVaxTime);
    strncpy(sp->Event.EventText.data, rp->EventText, sizeof(sp->Event.EventText.data) - 1);
    sp->Event.EventText.len = strlen(sp->Event.EventText.data);
    break;
  case mh_eEvent_Block:
  case mh_eEvent_Unblock:
  case mh_eEvent_Reblock:
  case mh_eEvent_CancelBlock:
    sp->Event.TargetIdx = bp->TargetId.Idx;
    time_PwrToVms(&bp->DetectTime, &sp->Event.DetectVaxTime);
    sp->Event.Outunit = bp->Outunit;
    sp->Event.Status = bp->Status;
    break;
    break;
  default:
    break;
  }

  switch (ip->EventType) {
  case mh_eEvent_Alarm:
  case mh_eEvent_Info:
  case mh_eEvent_Ack:
  case mh_eEvent_Return:
  case mh_eEvent_Cancel:
    switch (sp->Event.SupType) {
    case mh_eSupType_Analog:
      sp->Event.AValue = sip->mh_uSupInfo_u.A.ActualValue;
      sp->Event.CtrlLimit = sip->mh_uSupInfo_u.A.CtrlLimit;
      sp->Event.Hysteres = sip->mh_uSupInfo_u.A.Hysteres;
      sp->Event.High = sip->mh_uSupInfo_u.A.High;
      strncpy(sp->Event.Unit.data, sip->mh_uSupInfo_u.A.Unit, sizeof(sp->Event.Unit.data) - 1);
      sp->Event.Unit.len = strlen(sp->Event.Unit.data);
      break;
    case mh_eSupType_Digital:
      sp->Event.DValue = sip->mh_uSupInfo_u.D.ActualValue;
      sp->Event.High = sip->mh_uSupInfo_u.D.High;
      break;
    }
  }
  return sp;
}

sStore *
MissingEvent (
  mh_sMsgInfo *ip,
  pwr_tUInt32 Idx
)
{
  sStore *sp;

  sp = AllocEvent();

  time_PwrToVms(&ip->Id.BirthTime, &sp->Event.BirthVaxTime);
  sp->Event.Nid = ip->Id.Nix;
  sp->Event.Idx = Idx;
  sp->Event.TargetIdx = ip->Id.Idx - 1;
  sys$gettim(&sp->Event.EventVaxTime); /* It's OK to use sys$gettim */
  sp->Event.Missing = ip->Id.Idx - sp->Event.Idx;
  sprintf(
    sp->Event.EventName.data, "%d events, %d -> %d, from node %d were lost",
    sp->Event.Missing, Idx, sp->Event.TargetIdx, sp->Event.Nid
  );
  Log(sp->Event.EventName.data);
  sp->Event.EventType = mh_eEvent_Missing;

  return sp;
}

sStore *
AllocEvent ()
{
  sStore *sp;
  LstLink(sStore) *sl;

  if (LstEmp(&lHelCB.FreeL))
    Store(lHelCB.MaxStoreLSize/10);
  
  sl = LstFir(&lHelCB.FreeL);
  LstRem(sl);
  
  return LstObj(sl);
}

void
FreeEvent (
  sStore *sp
)
{

  LstRem(&sp->StoreL);
  memset(sp, 0, sizeof(*sp));
  LstIns(&lHelCB.FreeL, sp, StoreL);

}

static void
LogSqlError (
  struct SQLCA *sqlca,
  char *msg
)
{
  pwr_tStatus sts;
  struct dsc$descriptor errd;
  char err[301];
  int errlen;

  extern int SQL$GET_ERROR_TEXT ();

  errd.dsc$b_class = DSC$K_CLASS_S;
  errd.dsc$b_dtype = DSC$K_DTYPE_T;
  errd.dsc$w_length = 300;
  errd.dsc$a_pointer = err;
  
  sts = SQL$GET_ERROR_TEXT(&errd, &errlen);
  err[errlen] = '\0';
  Log(msg);
  Log(err);
}

static int
CompTime (
  pwr_tVaxTime t1,
  pwr_tVaxTime t2
)
{

  if (t1.high == t2.high)
    return(t1.low - t2.low);
  else
    return (t1.high - t2.high);
}

void
IncStoreIdx (
  pwr_tInt64 *Idx,
  int Inc
)
{
  unsigned int MaxInc = UINT_MAX - Idx->low;

  if (Inc > MaxInc) {
    Idx->high++;
    Idx->low = Inc - MaxInc - 1;
  } else {
    Idx->low += Inc;
  }
}

