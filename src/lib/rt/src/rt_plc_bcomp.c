#ifdef OS_ELN
#include stdio
#else
#include <stdio.h>
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "rt_plc.h"
#include "rt_gdh.h"
#include "co_time.h"

#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif


/**
  RunTimeCounterFo

  @aref runtimecounterfo RunTimeCounterFo
*/
void RunTimeCounterFo_init( pwr_sClass_RunTimeCounterFo  *o)
{
  pwr_tDlid dlid;
  pwr_tStatus sts;

  sts = gdh_DLRefObjectInfoAttrref( &o->PlcConnect, (void **)&o->PlcConnectP, &dlid);
  if ( EVEN(sts)) 
    o->PlcConnectP = 0;
  if ( o->PlcConnectP && o->ResetP == &o->Reset)
    ((pwr_sClass_RunTimeCounter *)o->PlcConnectP)->AccTripReset = 1;
}

void RunTimeCounterFo_exec( plc_sThread		*tp,
			    pwr_sClass_RunTimeCounterFo  *o)
{
  pwr_tDeltaTime	TimeSince;
  pwr_sClass_RunTimeCounter *co = (pwr_sClass_RunTimeCounter *) o->PlcConnectP;

  if ( !co)
    return;

  if ( *o->ResetP)
    co->TripReset = 1;

  time_FloatToD( &TimeSince, *o->ScanTime);

  /* Test if New Trip */
  if (co->TripReset) {
    co->OldTripNOfStarts = co->TripNOfStarts;
    co->OldTripUsage = co->TripUsage;
    co->OldTripTime = co->TripTime;
    co->OldTripRunTime = co->TripRunTime;

    co->TripNOfStarts = 0;
    co->TripRunTime.tv_sec = co->TripRunTime.tv_nsec = 0;
    co->TripTime.tv_sec = co->TripTime.tv_nsec = 0;

    clock_gettime(CLOCK_REALTIME, &co->ResetTime);
    co->TripReset = 0;
  }
  /* Update Calendar time */
  time_Dadd( &co->TotalTime, &co->TotalTime, &TimeSince);
  time_Dadd( &co->TripTime, &co->TripTime, &TimeSince);

  /* Test if running */
  o->Start = 0;
  if (*o->RunningP) {
    /* New start ? */
    if ( !o->Running) {
      o->Start = 1;
      co->TotalNOfStarts++;
      co->TripNOfStarts++;
      clock_gettime(CLOCK_REALTIME, &co->StartTime);
    } /* End if new start */
    /* Update Running Time */
    time_Dadd( &co->TripRunTime, &co->TripRunTime, &TimeSince);
    time_Dadd( &co->TotalRunTime, &co->TotalRunTime, &TimeSince);
  } /* End if Running */

  o->Running = co->Running = *o->RunningP;

  /* Calculate usage % */
  if ( co->TotalRunTime.tv_sec)
    co->TotalUsage = ((float)co->TotalRunTime.tv_sec) / co->TotalTime.tv_sec * 100;
  if ( co->TripTime.tv_sec)
    co->TripUsage = ((float)co->TripRunTime.tv_sec) / co->TripTime.tv_sec * 100;
}













