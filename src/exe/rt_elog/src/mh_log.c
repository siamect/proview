/* mh_log.c -- Log utilities

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef OS_VMS
#include <starlet.h>
#include <signal.h>
#endif


#include "co_cdh.h"
#include "co_time.h"
#include "rt_mh_log.h"


FILE *mh_LogFileOpen(qcom_sQid *qid)
{
  char LogFileName[48];
  FILE *fp;
  char DECnetHost[20] = "";

#ifdef OS_ELN
  ini_GetNodeInfo(NULL, NULL, NULL, DECnetHost, NULL, NULL);
  if (strlen(DECnetHost) > 0) strcat(DECnetHost, "::");
#endif

  sprintf(LogFileName, "%spwrp_log:mh_%d_%d.log", 
    DECnetHost, qid->nid, qid->qix);
  fp = fopen (LogFileName, "w+","shr=get","ctx=rec");
  if (fp) {
    printf("Opened logfile: %s\n", LogFileName);
    fprintf(fp, "%8s %18s %19s %3s %10s %8s %8s %12s %4s %8s %10s\n",
      "QId", "Time", "MsgType", "Nix", "EventIndex", "Object",
      "EventFlg", "EventType", "Prio", "EventSts", "Status");
    fflush(fp);
  } else {
#ifdef OS_ELN
    printf("Can't open logfile %s, %d, %d\n", LogFileName, errno, vaxc$errno);
#else
    printf("Can't open logfile %s, %d\n", LogFileName, errno);
#endif
  }
  return fp;

}

void mh_LogMessage(
  FILE *fp,
  pwr_tStatus sts,
  mh_eMsg Type,
  mh_sMsgInfo *Msg,
  qcom_sQid *QId
) {
  char TimeStr[24];
  int Event;
  struct tm *TmpTm;
  pwr_tTime TmpTime;
  int nsec;
  static char *EventText[mh_eEvent_] = {
    "NoEvent",
    "Ack",
    "Block",
    "Cancel",
    "CancelBlock",
    "Missing",
    "Reblock",
    "Return",
    "Unblock",
    "***", "***", "***", "***", "***", "***", "***", "***", "***", "***",
    "***", "***", "***", "***", "***", "***", "***", "***", "***", "***",
    "***", "***", "***",
    "Info",    
    "***", "***", "***", "***", "***", "***", "***", "***", "***", "***",
    "***", "***", "***", "***", "***", "***", "***", "***", "***", "***",
    "***", "***", "***", "***", "***", "***", "***", "***", "***", "***",
    "***",
    "Alarm"
  };

  static char *PrioText[] = {
    "D", "C", "B", "A" 
  };

  static char *MsgText[mh_eMsg_] = {
    "NoMsg",
    "ApplConnect",
    "ApplDisconnect",
    "ApplGetMsgInfo",
    "ApplMessage",
    "ApplReply",
    "ApplReturn",
    "ClearLists",
    "Event",
    "HandlerConnect",
    "HandlerDisconnect",
    "HandlerHello",
    "HandlerSynchReply",
    "HandlerSynchRequest",
    "OutunitAck",
    "OutunitBlock",
    "OutunitConnect",
    "OutunitDisconnect",
    "OutunitHello",
    "OutunitUpdate",
    "OutunitSynchReply",
    "OutunitSynchRequest",
    "ProcDown",
    "StopScanSup",
    "StartScanSup"
  };

  if (fp == NULL)
    return;

  if (Event = (Type == mh_eMsg_Event))
    TmpTime = Msg->EventTime;
  else
    clock_gettime(CLOCK_REALTIME, &TmpTime);

  TmpTm = localtime(&TmpTime.tv_sec);
    
  sprintf(TimeStr, "%02d%02d%02d %02d:%02d:%02d.%02d",
            TmpTm->tm_year, TmpTm->tm_mon, TmpTm->tm_mday, TmpTm->tm_hour,
            TmpTm->tm_min, TmpTm->tm_sec, TmpTime.tv_nsec / 1e7);

  fprintf(fp, "%3d.%-4d %18s %19s %3d %10d %32s %8x %12s %4s %8x %10d\n",
    QId ? QId->nid : 0, 
    QId ? QId->qix : 0, 
    TimeStr, 
    MsgText[Type], 
    Event ? Msg->Id.Nix : 0, 
    Event ? Msg->Id.Idx : 0, 
    Event ? cdh_ObjidToString(NULL, Msg->Object, 1) : 0, 
    Event ? Msg->EventFlags : 0, 
    Event ? EventText[Msg->EventType] : "", 
    Event && (Msg->EventPrio >= mh_eEventPrio_D && Msg->EventPrio <= mh_eEventPrio_A) ?
      PrioText[Msg->EventPrio - mh_eEventPrio_D] : "0", 
    Event && (Msg->EventType == mh_eEvent_Info || Msg->EventType == mh_eEvent_Alarm) ?
      ((mh_sMessage *)Msg)->Status : 0, 
    ODD(sts) ? 1 : sts
  ); 

  fflush(fp);
}
