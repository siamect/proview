/* rt_c_plcthread.c -- <short description>

   PROVIEW/R
   Copyright (C) 1997-98 by Mandator AB.

   Functions for the class PlcThread.

    */

#ifdef OS_ELN
# include $kerneldef
# include $vaxelnc
# include stdio
# include string
#else
# include <stdio.h>
# include <string.h>
#endif

#if defined OS_LYNX || defined OS_LINUX
# include <pthread.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_plc.h"
#include "rt_c_plcthread.h"

#define cNanoSec 1.0e-9


/**
  @aref plcthread PlcThread
*/
pwr_sClass_PlcThread *
pwrb_PlcThread_Init (
  pwr_tStatus	*sts,
  plc_sThread	*tp
)
{
  pwr_sClass_PlcThread  *o;

  *sts = gdh_DLRefObjectInfoAttrref(&tp->aref, (void *)&tp->PlcThread, &tp->dlid);
  if (EVEN(*sts)) return NULL;

  o = tp->PlcThread;

  tp->f_scan_time = o->ScanTime;
  tp->prio = o->Prio;

  o->Dlid = tp->dlid;

  return o;
}

void
pwrb_PlcThread_Exec (
  plc_sThread *tp
)
{
  pwr_tDeltaTime	last;
  int    	        meanCount;
  pwr_sClass_PlcThread	*o = tp->PlcThread;

  if (o == NULL) 
    return;

  o->ActualScanTime = tp->ActualScanTime;
  meanCount = o->ScanTimeMeanCount;

  if ((meanCount != 0) && (o->Count % meanCount == 0)) {
    if (o->Count != 0) {
      double diff;

      time_Dsub(&last, (pwr_tDeltaTime *) &tp->before_scan_abs, (pwr_tDeltaTime *)&o->ScanTimeStart);
      diff = last.tv_sec + last.tv_nsec * cNanoSec;
      o->ScanTimeMean = diff / meanCount; 
      
    }
    o->ScanTimeStart.tv_sec = tp->before_scan_abs.tv_sec; 
    o->ScanTimeStart.tv_nsec = tp->before_scan_abs.tv_nsec; 
  }

  o->Count++;
  
  if (o->Count != 1) {
    double scanTime;

    time_Dsub(&last, (pwr_tDeltaTime *)&tp->before_scan_abs, (pwr_tDeltaTime *)&tp->one_before_scan_abs);
    scanTime = last.tv_nsec * cNanoSec + last.tv_sec;

    if (scanTime < o->ScanTimeMin) o->ScanTimeMin = scanTime;
    if (scanTime > o->ScanTimeMax) o->ScanTimeMax = scanTime;

  }
 
  time_Dsub(&last, (pwr_tDeltaTime *)&tp->after_scan_abs, (pwr_tDeltaTime *)&tp->before_scan_abs);
  o->Last = last.tv_nsec * cNanoSec + last.tv_sec;
  
  if (o->Last < o->Min) o->Min = o->Last;
  if (o->Last > o->Max) o->Max = o->Last;

  o->Sum     += o->Last;
  o->Mean     = o->Sum / o->Count;
  o->Coverage = (o->Mean / o->ScanTime) * 100.0;

  if (o->Last < o->Limit_1_8)
    o->Count_1_8++;
  else if (o->Last < o->Limit_1_4)
    o->Count_1_4++;
  else if (o->Last < o->Limit_1_2)
    o->Count_1_2++;
  else if (o->Last < o->Limit_1_1)
    o->Count_1_1++;
  else if (o->Last < o->Limit_2_1)
    o->Count_2_1++;
  else if (o->Last < o->Limit_4_1)
    o->Count_4_1++;
  else if (o->Last < o->Limit_8_1)
    o->Count_8_1++;
  else
    o->CountHigh++;
  
  o->SlipCount = tp->sliped;


}

void
pwrb_PlcThread_Zero (
  plc_sThread	*tp
)
{
  pwr_sClass_PlcThread  *o;

  o = tp->PlcThread;

  o->Limit_1_8 = o->ScanTime / 8.0;
  o->Limit_1_4 = o->ScanTime / 4.0;
  o->Limit_1_2 = o->ScanTime / 2.0;
  o->Limit_1_1 = o->ScanTime;
  o->Limit_2_1 = o->ScanTime * 2.0;
  o->Limit_4_1 = o->ScanTime * 4.0;
  o->Limit_8_1 = o->ScanTime * 8.0;

  o->ScanTimeMin    = 1.0e30;
  o->ScanTimeMean   = 0.0;
  o->ScanTimeMax    = 0.0;
  /* Update ScanTimeMean after 100 loops or after 10 seconds */
  o->ScanTimeMeanCount = MIN(100., MAX(10./o->ScanTime, 1)) +.5; 
  o->ScanTimeStart.tv_sec = 0;
  o->ScanTimeStart.tv_nsec = 0;

  o->Count    = 0;
  o->Sum      = 0.0;
  o->Min      = 1.0e30;
  o->Mean     = 0.0;
  o->Coverage = 0.0;
  o->Max      = 0.0;

  o->Count_1_8 = 0;
  o->Count_1_4 = 0;
  o->Count_1_2 = 0;
  o->Count_1_1 = 0;
  o->Count_2_1 = 0;
  o->Count_4_1 = 0;
  o->Count_8_1 = 0;
  o->CountHigh = 0;
  o->SlipCount = 0;
}


