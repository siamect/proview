/*
   PROVIEW/R
   Copyright (C) 1996,98 by Mandator AB.

   This file contains the access routines to the datastorage 
   of data to fastplot.   */

#if defined OS_VMS
# include <descrip.h>
# include <starlet.h>
# include <lib$routines.h>

#elif defined OS_ELN
# include $vaxelnc
# include "rt_plc_loop.h"

#elif defined OS_LYNX || defined OS_LINUX
# include <errno.h>
# include <unistd.h>
# include <time.h>
# include <signal.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_ds_msg.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"

#define Log_Error(a, b) errh_CErrLog(DS__ERRORF, errh_ErrArgAF(b), errh_ErrArgMsg(a), NULL)
#define Log(b) errh_CErrLog(DS__LOGF, errh_ErrArgAF(b), NULL)
#define Log_Error_Exit(a, b) {Log_Error(a, b); exit(a);}
#define If_Error_Log(a, b) if ((a & 1) != 1) Log_Error(a, b)
#define If_Error_Log_Exit(a, b) if ((a & 1) != 1) Log_Error_Exit(a, b)

typedef struct s_LstNode	sLstNode;   /* Node for an entry. */

struct s_LstNode      
    {
    sLstNode		*Next;
    pwr_sClass_DsFast	*DsFast;     /* Pointer to DsFast object */
    pwr_tSubid		DsFastSubId; /* SubId for DsFast object */  
    pwr_tUInt8		LastTrigMode;
    pwr_tBoolean	LastTrigValue;
    }; 



static pwr_tUInt32 InitFastList(pwr_tInt32 BaseFrequency, sLstNode **LstHead);
static pwr_tBoolean IsTrigged(pwr_sClass_DsFast *Fast, sLstNode *Node);
static pwr_tBoolean IsValidType(int Type);
static void StoreData(sLstNode *LstHead);

int main (int argc, char **argv)
{
  pwr_tInt32	    sts;
  pwr_tObjid	    ObjId;
  pwr_tBoolean    InitOK;
  pwr_sClass_DsFastConf *FConfP;
  sLstNode	    *LstHead = NULL;
  pwr_tInt32	    CycleTime;
  pwr_tInt32	    BaseFrequency;


#ifdef OS_VMS
  extern int plc_ConvMSToLI(pwr_tUInt32 Time, pwr_tVaxTime *TimeLI);
  unsigned int    TimerFlag;
  pwr_tVaxTime    DeltaTime;

#elif defined OS_ELN
  extern int plc_ConvMSToLI(pwr_tUInt32 Time, pwr_tVaxTime *TimeLI);
  pwr_tVaxTime  NextTime;
  pwr_tVaxTime  DeltaTime;

#elif defined OS_LYNX || defined OS_LINUX
  pwr_tTime		CurrentTime, LastScan, NextScan;
  pwr_tDeltaTime	ScanTime, WaitTime;
#endif

  errh_Init("pwr_fast");
 
  sts = gdh_Init("pwr_fast");
  If_Error_Log_Exit(sts, "gdh_Init");

  /* Wait until local nethandler has started */
  while(EVEN(gdh_NethandlerRunning()))
    sleep(1);

  /* Fetch BaseFrequency (seconds) */
  sts = gdh_GetClassList(pwr_cClass_DsFastConf, &ObjId);
  if (EVEN(sts)) {
    Log("Couldn't get the DsFastConf object. Used BaseFreq = 10 Hz");
    BaseFrequency = 10;
  } else {
    gdh_ObjidToPointer(ObjId, (pwr_tAddress *)&FConfP);
    BaseFrequency = FConfP->BaseFrequency;
#ifdef OS_LYNX
    if (BaseFrequency > 100)
      BaseFrequency = 100;
#else
    if (BaseFrequency > 50)
      BaseFrequency = 50;
#endif
    else if (BaseFrequency < 1)
      BaseFrequency = 1;
  }

  /* Convert to milliseconds */	  
  CycleTime = 1000.0 / (double) BaseFrequency;

  sts = InitFastList(BaseFrequency, &LstHead);
  if (ODD(sts)) {
    InitOK = TRUE;
  } else {
    InitOK = FALSE;

    /* This should be removed when we can wait for init messages. */
    Log_Error_Exit(sts, "No valid DsFast objects");
  }

  /* If even sts, just exit. Next version wait for init message */

#if defined OS_VMS
  plc_ConvMSToLI(CycleTime, &DeltaTime);
  sts = lib$get_ef(&TimerFlag);
  if (EVEN(sts)) {
    Log_Error_Exit(sts,"Couldn't allocate event flag for timer");
  }
#elif defined OS_ELN
  plc_ConvMSToLI(CycleTime, &DeltaTime);
  sts = plc_LoopInit(&NextTime);
  if (EVEN(sts)) {
    Log_Error_Exit(sts,"Couldn't initialize timer loop");
  }
#elif defined OS_LYNX || defined OS_LINUX
  clock_gettime(CLOCK_REALTIME, &LastScan);
  ScanTime.tv_sec =  CycleTime / 1000;
  ScanTime.tv_nsec = (CycleTime % 1000) * 1000000;
#endif

  /* LOOP forever. */
  for(;;) {
#if defined OS_VMS
    /* We don't care if we slip */
    sys$clref(TimerFlag);
    sys$setimr(TimerFlag, &DeltaTime, 0, 2, 0);
    StoreData(LstHead);
    sys$waitfr(TimerFlag);
#elif defined OS_ELN
    StoreData(LstHead);
    sts = plc_LoopWait( NULL, &DeltaTime, &NextTime, NULL);
   /* If sts == 0 it is a slip, but we can't do anything */
#elif defined OS_LYNX || defined OS_LINUX
    StoreData(LstHead);
    clock_gettime(CLOCK_REALTIME, &CurrentTime);
    time_Aadd(&NextScan, &LastScan, &ScanTime);
    if (time_Acomp(&CurrentTime, &NextScan) < 0) { 
      time_Adiff(&WaitTime, &NextScan, &CurrentTime);
      nanosleep((struct timespec *)&WaitTime, NULL);
    }

    LastScan = NextScan;
#endif
  }
}

/* Set up subscriptions for every local DsFast object and 
   initiate the DsFast objects.  */

static pwr_tUInt32
InitFastList (
  pwr_tInt32 BaseFrequency,
  sLstNode **LstHead
)
{
  sLstNode	    *LstNode, *TmpNode;
  pwr_tUInt32	    sts;
  pwr_tUInt32	    Dummy;
  pwr_tTypeId	    Type;
  int		    Tix;
  pwr_tObjid	    ObjId;
  char		    Name[81];
  pwr_sClass_DsFast *Fast;

  /* Free old list */

  LstNode = *LstHead;
  while (LstNode != NULL) {
    TmpNode = LstNode;
    gdh_UnrefObjectInfo(LstNode->DsFast->DataSubId);
    if (cdh_SubidIsNotNull(LstNode->DsFast->TrigSubId))
      gdh_UnrefObjectInfo(LstNode->DsFast->TrigSubId);
    gdh_UnrefObjectInfo(LstNode->DsFastSubId);
    LstNode =TmpNode->Next;
    free(TmpNode);
  }

  *LstHead = NULL;

  sts = gdh_GetClassList(pwr_cClass_DsFast, &ObjId);
  if (EVEN(sts)) {
    return DS__NOOBJECT;
  } 

  /* Scan through classlist and insert valid objects in list and initiate
     the DsFast objects. */

  for (; ODD(sts); sts = gdh_GetNextObject(ObjId, &ObjId) ) {
    sts = gdh_ObjidToName(ObjId, Name, sizeof(Name), cdh_mNName);  
    if (EVEN(sts))
      continue;

    LstNode = calloc(1, sizeof(*LstNode));
    if (LstNode == NULL) {
      errh_CErrLog(DS__ERRALLOC, NULL);
      exit(DS__ERRALLOC);
    }

    sts = gdh_RefObjectInfo(Name, (pwr_tAddress *)&LstNode->DsFast, 
      &LstNode->DsFastSubId, sizeof(*LstNode->DsFast));
    if (EVEN(sts)) {
      errh_Error("Couldn't get subscription for %s\n%m", Name, sts);
      free(LstNode);
      continue;
    }
    Fast = LstNode->DsFast;       

    /* Initiate DsFast object. Sampled attribute must be on local node  */	  

    sts = gdh_DLRefObjectInfoAttrref(&Fast->DataName, 
      (pwr_tAddress *)&Fast->DataPointer, &Fast->DataSubId);
    if (EVEN(sts)) {
      errh_Error("Couldn't get direct link to %s's attribute DataName\n%m", Name, sts);
      gdh_UnrefObjectInfo(LstNode->DsFastSubId);
      free(LstNode);
      continue;
    }

    /* Get data type */
    sts = gdh_GetAttributeCharAttrref(&Fast->DataName, &Type, &Dummy, &Dummy, &Dummy);
    if (EVEN(sts)) {
      errh_Error("Couldn't get datatype for %s's attribute DataName\n%m", Name, sts);
      gdh_UnrefObjectInfo(LstNode->DsFastSubId);
      free(LstNode);
      continue;
    }
    Tix = cdh_TypeIdToIndex(Type);

    if (!IsValidType(Tix)) {
      errh_Error("DsFast object %s will not be used\n%m", Name, DS__ERRTYPE);
      gdh_UnrefObjectInfo(LstNode->DsFastSubId);
      free(LstNode);
      continue;
    }
      
    Fast->DataType = Tix;
      
    /* Get trigg signal, must be on local node*/

    if (cdh_ObjidIsNull(Fast->TrigName.Objid)) {
      Fast->TrigPointer = NULL;
      Fast->TrigSubId = pwr_cNSubid;
    } else {
      sts = gdh_DLRefObjectInfoAttrref((pwr_sAttrRef *) &Fast->TrigName, 
	(pwr_tAddress *)&Fast->TrigPointer, &Fast->TrigSubId);
      if (EVEN(sts)) {
	errh_Error("Couldn't get Direct Link for %s's attribute TrigName\n%m", Name, sts);
	Fast->TrigPointer = NULL;
	Fast->TrigSubId = pwr_cNSubid;	      
      }
    }

    Fast->NoOfSample = (Fast->StorageTime * BaseFrequency) / Fast->Multiple;
    if (Fast->NoOfSample > Fast->NoOfBufElement)
      Fast->NoOfSample = Fast->NoOfBufElement;
    Fast->BaseFrequency = BaseFrequency;

    LstNode->LastTrigMode = 13; /* Dummy value */
    LstNode->Next = *LstHead;
    *LstHead = LstNode;
  }

  if (*LstHead == NULL)
    return DS__NOOBJECT;
  else
    return DS__SUCCESS;

}

/* Look for trig point and return TRUE if found
   otherwise FALSE is returned.  */

static pwr_tBoolean
IsTrigged (
  pwr_sClass_DsFast *Fast,
  sLstNode *Node
)
{
  pwr_tBoolean Trigged = FALSE;

  if (Fast->TrigPointer == NULL || Fast->TrigMode == 0)
    return FALSE; 

  /* If trig mode changed then initialize new trig mode */

  if (Node->LastTrigMode != Fast->TrigMode)
    Node->LastTrigValue = *Fast->TrigPointer;

  switch (Fast->TrigMode) {
  case 1:	/* Trigmode positive flank */
    if (Node->LastTrigValue == 0 && *Fast->TrigPointer != 0)
      Trigged = TRUE;
    break;
  case 2:	/* Trigmode negative flank */
    if (Node->LastTrigValue != 0 && *Fast->TrigPointer == 0)
      Trigged = TRUE;
    break;
  case 3:	/* Trigmode one */
    if (*Fast->TrigPointer != 0)
      Trigged = TRUE;
    break;	
  case 4:	/* Trigmode zero */
    if (*Fast->TrigPointer == 0)
      Trigged = TRUE;
    break;	
  default:	/* Unknown trigmode */
    break;
  }

  Node->LastTrigMode = Fast->TrigMode;
  Node->LastTrigValue = *Fast->TrigPointer;

  return Trigged;
}

static pwr_tBoolean
IsValidType (
  int Type
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
  default :
    Valid = FALSE;
    break;
  }

  return Valid;

}

/* Store data for all DsFast objects in list. */

static void
StoreData (
  sLstNode *LstHead
)
{
  sLstNode		*LstNode;
  pwr_tFloat32		*BuffP;
  pwr_sClass_DsFast	*Fast;

  /* Scan the FastList */

  for (LstNode = LstHead; LstNode != NULL; LstNode = LstNode->Next) {
    Fast = LstNode->DsFast;
    if (!Fast->Start)
      continue;
      
    /* If not yet trigged then look for trig */

    if (!Fast->Trigged) {
      if (
	Fast->TrigManTrue 
	|| (Fast->TrigMode && Fast->TrigPointer && IsTrigged(Fast, LstNode))
      ) {
	Fast->TrigIndex = Fast->NextIndex;
	Fast->Trigged = TRUE;
      }	    
    }

    if (Fast->NextMultiple == 0) {

      /* Store data in buffer */

      BuffP = &Fast->DataBuffer[Fast->NextIndex++];
      switch (Fast->DataType) {
      case pwr_eTix_Boolean:
	*BuffP = *(pwr_tBoolean *)Fast->DataPointer;
	break;
      case pwr_eTix_Float32:
	*BuffP = *(pwr_tFloat32 *)Fast->DataPointer;
	break;
      case pwr_eTix_Float64:
	*BuffP = *(pwr_tFloat64 *)Fast->DataPointer;
	break;
      case pwr_eTix_Char:
	*BuffP = *(pwr_tChar *)Fast->DataPointer;
	break;
      case pwr_eTix_Int8:
	*BuffP = *(pwr_tInt8 *)Fast->DataPointer;
	break;
      case pwr_eTix_Int16:
	*BuffP = *(pwr_tInt16 *)Fast->DataPointer;
	break;
      case pwr_eTix_Int32:
	*BuffP = *(pwr_tInt32 *)Fast->DataPointer;
	break;
      case pwr_eTix_UInt8:
	*BuffP = *(pwr_tUInt8 *)Fast->DataPointer;
	break;
      case pwr_eTix_UInt16:
	*BuffP = *(pwr_tUInt16 *)Fast->DataPointer;
	break;
      case pwr_eTix_UInt32:
	*BuffP = *(pwr_tUInt32 *)Fast->DataPointer;
	break;
      default	:
	*BuffP = 0.0; /* Prevent invalid float */
	break;
      }
    
      if(Fast->NextIndex >= Fast->NoOfSample) {
	Fast->AllDataOK = TRUE;
	Fast->NextIndex = 0;
      }

      /* If trigged then check if stop data storage */

      if (Fast->Trigged) {   
	if (
	  (Fast->NoOfSample + Fast->TrigIndex - Fast->SampBeforeTrig) 
	    % (Fast->NoOfSample) 
	  == Fast->NextIndex
	) {
	  Fast->Start = FALSE;   
	  Fast->Trigged = FALSE;   
	  Fast->TrigManTrue = FALSE;   
	}    
      }
      Fast->NextMultiple = Fast->Multiple;
    } /* NextMultiple == 0 */

    Fast->NextMultiple--;     
  }
}
