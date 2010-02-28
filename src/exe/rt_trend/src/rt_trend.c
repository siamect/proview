/* 
 * Proview   $Id: rt_trend.c,v 1.12 2008-09-18 15:10:21 claes Exp $
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

/* rt_trend.c 

   This file contains the access routines to the datastorage 
   of data to trendplot.   */

#if defined OS_VMS
# include <stdio.h>
# include <stdlib.h>
#elif defined OS_LYNX || defined OS_LINUX || defined OS_MACOS
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <unistd.h>
# include <time.h>
# include <signal.h>
# include <string.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_ds_msg.h"
#include "rt_gdh_msg.h"
#include "rt_qcom_msg.h"
#include "rt_qcom.h"
#include "rt_ini_event.h"
#include "rt_aproc.h"
#include "rt_pwr_msg.h"

#define Log_Error(a, b) errh_CErrLog(DS__ERROR, errh_ErrArgAF(b), errh_ErrArgMsg(a), NULL)
#define Log(b) errh_CErrLog(DS__LOG, errh_ErrArgAF(b), NULL)
#define Log_Error_Exit(a, b) {Log_Error(a, b); exit(a);}
#define If_Error_Log(a, b) if ((a & 1) != 1) Log_Error(a, b)
#define If_Error_Log_Exit(a, b) if ((a & 1) != 1) Log_Error_Exit(a, b)

typedef struct s_LstNode sLstNode;   /* Node for an entry. */

struct s_LstNode
    {
    sLstNode		*Next;
    pwr_sClass_DsTrend  *DsTrend;     /* Pointer to DsTrend object */
    pwr_tSubid		DsTrendSubId; /* SubId for DsTrend object */  
    }; 

static sLstNode	*LstHead = NULL;

static pwr_tStatus InitTrendList();
static void CloseTrendList();
static pwr_tBoolean IsValidType(pwr_eTix Type);
static void StoreData();
static int IsDisabled( pwr_tAttrRef *aaref);

    
int main (int argc, char **argv)
{
  pwr_tStatus	    sts;
  pwr_tObjid	    ObjId;
  pwr_sClass_DsTrendConf *TConfP;
  pwr_tBoolean    InitOK;
  pwr_tInt32	    ScanTime = 1;
  pwr_tTime		CurrentTime, LastScan, NextScan;
  pwr_tDeltaTime	ScanDeltaTime, WaitTime;
  qcom_sQid qini;
  qcom_sQattr qAttr;
  int tmo;
  char mp[2000];
  qcom_sQid qid = qcom_cNQid;
  qcom_sGet get;
  int swap = 0;

  errh_Init("pwr_trend", errh_eAnix_trend);
  errh_SetStatus( PWR__SRVSTARTUP);

  sts = gdh_Init("ds_trend");
  If_Error_Log_Exit(sts, "gdh_Init");

  if (!qcom_Init(&sts, 0, "pwr_trend")) {
    errh_Fatal("qcom_Init, %m", sts);
    exit(sts);
  } 

  qAttr.type = qcom_eQtype_private;
  qAttr.quota = 100;
  if (!qcom_CreateQ(&sts, &qid, &qAttr, "events")) {
    errh_Fatal("qcom_CreateQ, %m", sts);
    exit(sts);
  } 

  qini = qcom_cQini;
  if (!qcom_Bind(&sts, &qid, &qini)) {
    errh_Fatal("qcom_Bind(Qini), %m", sts);
    exit(-1);
  }

  /* Wait until local nethandler has started */
  while(EVEN(gdh_NethandlerRunning()))
    sleep(1);

  /* Fetch ScanTime (seconds) and initate ScanRate (1/10 second ) */
  sts = gdh_GetClassList(pwr_cClass_DsTrendConf, &ObjId);
  if (EVEN(sts)) {
    errh_Info("Couldn't get the DsTrendConf object. Used ScanTime = 1 s");
    ScanTime = 1;
  } else {
    gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&TConfP);
    ScanTime = TConfP->ScanTime;
    if (ScanTime > 3600)
      ScanTime = 3600;
    else if (ScanTime < 1)
      ScanTime = 1;
  }

  aproc_RegisterObject( ObjId);

  sts = InitTrendList(ScanTime, &LstHead);
  if (ODD(sts)) { 
    InitOK = TRUE;
  } else {
    InitOK = FALSE;

    /* This should be removed when we can wait for init messages. */
    errh_SetStatus(0);
    errh_Info("No DsTrend objects configured");
    exit(0);
  }

  /* If even sts, just wait for init message */

  time_GetTime(&LastScan);
  ScanDeltaTime.tv_sec =  ScanTime;
  ScanDeltaTime.tv_nsec = 0;

  aproc_TimeStamp();
  errh_SetStatus( PWR__SRUN);

  for (;;) {

    time_GetTime(&CurrentTime);
    time_Aadd(&NextScan, &LastScan, &ScanDeltaTime);
    if (time_Acomp(&CurrentTime, &NextScan) < 0) { 
      time_Adiff(&WaitTime, &NextScan, &CurrentTime);
      tmo = 1000 * time_DToFloat( 0, &WaitTime);

      get.maxSize = sizeof(mp);
      get.data = mp;
      qcom_Get( &sts, &qid, &get, tmo);
      if (sts == QCOM__TMO || sts == QCOM__QEMPTY) {
	if ( !swap)
	  StoreData(LstHead);
      } 
      else {
	ini_mEvent  new_event;
	qcom_sEvent *ep = (qcom_sEvent*) get.data;

	new_event.m  = ep->mask;
	if (new_event.b.oldPlcStop && !swap) {
	  swap = 1;
	  errh_SetStatus( PWR__SRVRESTART);
	  CloseTrendList( &LstHead);
	} 
	else if (new_event.b.swapDone && swap) {
	  swap = 0;
	  sts = InitTrendList( ScanTime, &LstHead);
	  errh_SetStatus( PWR__SRUN);
	  errh_Info("Warm restart completed");
	}
	else if (new_event.b.terminate) {
	  exit(0);
	}
      }
    }
    else if ( !swap)
      StoreData(LstHead);

    LastScan = NextScan;

    aproc_TimeStamp();
  }

  return 1;
}

/* Set up subscriptions for every local DsTrend object and 
   initialize the DsTrend objects.  */

static void
CloseTrendList (
  sLstNode **LstHead
)
{
  sLstNode	    *LstNode, *TmpNode;

  /* Free old list */

  LstNode = *LstHead;
  while (LstNode != NULL) {
    TmpNode = LstNode;
    gdh_UnrefObjectInfo(LstNode->DsTrend->DataSubId);
    gdh_UnrefObjectInfo(LstNode->DsTrendSubId);
    LstNode =TmpNode->Next;
    free(TmpNode);
  }
  *LstHead = NULL;
}

static pwr_tStatus
InitTrendList (
  pwr_tInt32 ScanTime,
  sLstNode **LstHead
)
{
  sLstNode	    *LstNode;
  pwr_tStatus	    sts;
  pwr_tUInt32	    Dummy;
  pwr_tTypeId	    Type;
  int		    Tix;
  pwr_tAttrRef	    Aref;
  pwr_tAttrRef	    OAref;
  pwr_tAName   	    Name;
  pwr_sClass_DsTrend  *Trend;
  pwr_tDisableAttr  disabled;

  sts = gdh_GetClassListAttrRef(pwr_cClass_DsTrend, &Aref);
  if (EVEN(sts)) return DS__NOOBJECT;

  /* Scan through typelist and insert valid objects in list and initiate
     the DsTrend objects.  */

  for (; ODD(sts) ; sts = gdh_GetNextAttrRef( pwr_cClass_DsTrend, &Aref, &Aref) ) {
    sts = gdh_AttrrefToName( &Aref, Name, sizeof(Name), cdh_mNName);  
    if (EVEN(sts))
      continue;

    /* Check if parent object is disabled */
    sts = gdh_AttrArefToObjectAref( &Aref, &OAref);
    if ( ODD(sts)) {
      sts = gdh_ArefDisabled( &OAref, &disabled);
      if ( ODD(sts) && disabled)
	continue;
    }

    LstNode = calloc(1, sizeof(*LstNode));
    if (LstNode == NULL) {
      errh_CErrLog(DS__ERRALLOC, NULL);
      errh_SetStatus( PWR__SRVTERM);
      exit(DS__ERRALLOC);
    }

    sts = gdh_RefObjectInfo(Name, (pwr_tAddress *)&LstNode->DsTrend, 
		      &LstNode->DsTrendSubId, sizeof(*LstNode->DsTrend));
    if (EVEN(sts)) {
      errh_Error("Couldn't get subscription for '%s'\n%m", Name, sts);
      free(LstNode);
      continue;
    }
    Trend = LstNode->DsTrend;       

    /* Initiate DsTrend object, sampled attribute must be on local node */	  

    sts = gdh_DLRefObjectInfoAttrref((pwr_sAttrRef *)&Trend->DataName, 
		  (pwr_tAddress *)&Trend->DataPointer, &Trend->DataSubId);
    if (EVEN(sts)) {
      if ( sts == GDH__RTDBNULL && IsDisabled( &Trend->DataName))
	  continue;

      errh_Error("Couldn't get direct link to %s's attribute DataName\n%m", Name, sts);
      gdh_UnrefObjectInfo(LstNode->DsTrendSubId);
      free(LstNode);
      continue;
    }

    sts = gdh_GetAttributeCharAttrref((pwr_sAttrRef *)&Trend->DataName,
					&Type, &Dummy, &Dummy, &Dummy);
    if (EVEN(sts)) {
      errh_Error("Couldn't get datatype for %s's attribute DataName\n%m", Name, sts);
      gdh_UnrefObjectInfo(LstNode->DsTrendSubId);
      free(LstNode);
      continue;
    }
    Tix = cdh_TypeIdToIndex(Type);

    if (!IsValidType(Tix)) {
      errh_Error("No valid datatype for %s's attribute DataName\n%m", Name, DS__ERRTYPE);
      gdh_UnrefObjectInfo(LstNode->DsTrendSubId);
      free(LstNode);
      continue;
    }
      
    Trend->DataType = Tix;
    if ( Trend->Multiple == 0)
      Trend->Multiple = 1;
    Trend->NoOfSample = (Trend->StorageTime * ScanTime) / Trend->Multiple;

    if(Trend->NoOfSample > Trend->NoOfBufElement)
      Trend->NoOfSample = Trend->NoOfBufElement;

    Trend->ScanTime = ScanTime;

    LstNode->Next = *LstHead;
    *LstHead = LstNode;
  }

  if (*LstHead == NULL)
    return DS__NOOBJECT;
  else
    return DS__SUCCESS;
}

static pwr_tBoolean
IsValidType (
  pwr_eTix Type
)
{
  pwr_tBoolean Valid = TRUE;

  switch (Type) {
  case	pwr_eTix_Boolean:
  case	pwr_eTix_Float32:
  case	pwr_eTix_Float64:
  case	pwr_eTix_Int8:
  case	pwr_eTix_Int16:
  case	pwr_eTix_Int32:
  case	pwr_eTix_UInt8:
  case	pwr_eTix_UInt16:
  case	pwr_eTix_UInt32:
    break;
  default:
    Valid = FALSE;
    break;
  }

  return Valid;

}

/* Stor data for all DsTrend objects in list. */

static void
StoreData (
  sLstNode *LstHead
)
{
  sLstNode		*LstNode;
  pwr_tFloat32	*BuffP;
  pwr_sClass_DsTrend	*Trend;

  /* Scan the TrendList */

  for (LstNode = LstHead; LstNode != NULL; LstNode = LstNode->Next) {
    Trend = LstNode->DsTrend;

    /* If position equal first position in buffer  */

    if(Trend->NextWriteIndex[Trend->WriteBuffer] == 0) {
      Trend->BufferStatus[Trend->WriteBuffer] = 1;
      time_GetTime(&Trend->BufferTime[Trend->WriteBuffer]);
    }    

    if(Trend->NextMultiple == 0) {
      /* Store data in buffer */

      BuffP = &Trend->DataBuffer[ 
	Trend->NextWriteIndex[Trend->WriteBuffer] 
	+ (Trend->WriteBuffer * Trend->NoOfBufElement)
	];

      switch (Trend->DataType) {
      case pwr_eTix_Boolean :
	*BuffP = *(pwr_tBoolean *)Trend->DataPointer;
	break;
      case pwr_eTix_Float32 :
	*BuffP = *(pwr_tFloat32 *)Trend->DataPointer;
	break;
      case pwr_eTix_Float64 :
	*BuffP = *(pwr_tFloat64 *)Trend->DataPointer;
	break;
      case pwr_eTix_Int8 :
	*BuffP = *(pwr_tInt8 *)Trend->DataPointer;
	break;
      case pwr_eTix_Int16  :
	*BuffP = *(pwr_tInt16 *)Trend->DataPointer;
	break;
      case pwr_eTix_Int32  :
	*BuffP = *(pwr_tInt32 *)Trend->DataPointer;
	break;
      case pwr_eTix_UInt8  :
	*BuffP = *(pwr_tUInt8 *)Trend->DataPointer;
	break;
      case pwr_eTix_UInt16  :
	*BuffP = *(pwr_tUInt16 *)Trend->DataPointer;
	break;
      case pwr_eTix_UInt32  :
	*BuffP = *(pwr_tUInt32 *)Trend->DataPointer;
	break;
      default	:
	*BuffP = 0.0 /* Prevent invalid float */;
	break;
      }

      Trend->NextWriteIndex[Trend->WriteBuffer]++;
      Trend->NextMultiple = Trend->Multiple;

      /* If buffert full then mark next buffer*/

      if (Trend->NextWriteIndex[Trend->WriteBuffer] >= Trend->NoOfSample) {
	Trend->BufferStatus[Trend->WriteBuffer] = 0;	    /* sts = read */
	Trend->NextWriteIndex[Trend->WriteBuffer] = 0;    /* First position */
	Trend->WriteBuffer = 1 - Trend->WriteBuffer;	    /* Change buffer */
      }	 	     
    }	
    Trend->NextMultiple--;
  }
}
	
/* Check if signal is disabled */
/* aaref points to ActualValue */
static int IsDisabled( pwr_tAttrRef *aaref)
{
  pwr_tDisableAttr disabled;
  pwr_tAName name;
  pwr_tAttrRef oaref;
  pwr_tStatus sts;
  char *s;
  
  sts = gdh_AttrrefToName( aaref, name, sizeof(name), cdh_mNName);  
  if ( EVEN(sts)) return 0;

  if ( (s = strrchr( name, '.')))
    *s = 0;
  else
    return 0;
       
  sts = gdh_NameToAttrref( pwr_cNObjid, name, &oaref);
  if ( EVEN(sts)) return 0;

  sts = gdh_ArefDisabled( &oaref, &disabled);
  if ( ODD(sts) && disabled)
    return 1;
  return 0;
}
