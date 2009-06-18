/* 
 * Proview   $Id: rt_plc_bcomp.c,v 1.4 2007-02-05 09:44:21 claes Exp $
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


/*_*
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

  if ( *o->ResetP && !o->OldReset)
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

    time_GetTime( &co->ResetTime);
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
      time_GetTime( &co->StartTime);
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

  o->OldReset = *o->ResetP;
}

/*_*
  CompModePID_Fo

  @aref compmodepid_fo CompModePID_Fo
*/
void CompModePID_Fo_init( pwr_sClass_CompModePID_Fo  *o)
{
  pwr_tDlid dlid;
  pwr_tStatus sts;

  sts = gdh_DLRefObjectInfoAttrref( &o->PlcConnect, (void **)&o->PlcConnectP, &dlid);
  if ( EVEN(sts)) 
    o->PlcConnectP = 0;
}

void CompModePID_Fo_exec( plc_sThread *tp,
			  pwr_sClass_CompModePID_Fo *o)
{
  pwr_sClass_CompModePID *co = (pwr_sClass_CompModePID *) o->PlcConnectP;

  if ( !co)
    return;

  /* Get indata */
  co->ProcVal = *o->ProcValP;
  co->XSetVal = *o->XSetValP;
  if ( o->XForcValP != &o->XForcVal)
    co->XForcVal = *o->XForcValP;
  co->Forc1 = *o->Forc1P;
  co->Forc2 = *o->Forc2P;
  co->OutVal = *o->OutValP;

  /* Make appropriate actions, depending on actual mode */

  /* Manual */
  if (co->OpMod <= 1) {
    co->Force = TRUE;
    co->ManMode = TRUE;
    co->AutMode = FALSE;
    co->CascMod = FALSE;
    /* External setpoint ? */
    if ((co->AccMod & 2) == 0)
      co->SetVal = co->XSetVal;
    /* Test if Force in manual mode */
    if (co->Forc1 )
      co->ForcVal = co->XForcVal;
  }
  else {
    /* Not Manual Mode */
    if (co->OpMod == 2) {
      /* Auto */
      co->ManMode = FALSE;
      co->AutMode = TRUE;
      co->CascMod = FALSE;
    }
    else {
      /* Cascade mode */
      co->ManMode = FALSE;
      co->AutMode = FALSE;
      co->CascMod = TRUE;
      co->SetVal = o->SetVal = co->XSetVal;
    }
    /* Test if force in Auto or Cascade */
    if ( co->Forc1 || co->Forc2 ) {
      co->Force = TRUE;
      co->ForcVal = co->XForcVal;
    }
    else {
      co->Force = FALSE;
      co->ForcVal = co->OutVal;
    }
  }
  /* Transfer to outputs */
  o->SetVal = co->SetVal;
  o->ForcVal = co->ForcVal;
  o->Force = co->Force;
  o->AutMode = co->AutMode;
  o->CascMod = co->CascMod;
}

/*_*
  CompPID_Fo

  @aref comppid_fo CompPID_Fo
*/
void CompPID_Fo_init( pwr_sClass_CompPID_Fo  *o)
{
  pwr_tDlid dlid;
  pwr_tStatus sts;

  sts = gdh_DLRefObjectInfoAttrref( &o->PlcConnect, (void **)&o->PlcConnectP, &dlid);
  if ( EVEN(sts)) 
    o->PlcConnectP = 0;
}

/* Define Algoritm bitmask */
#define IALG 1		/* Integral part -> Incremental algorithm */
#define PALG 2		/* Proportional part exists */
#define PAVV 4		/* Proportional part working on control difference */
#define DALG 8		/* Derivative part exists */
#define DAVV 16		/* Derivative part working on control difference */

void CompPID_Fo_exec( plc_sThread *tp,
		      pwr_sClass_CompPID_Fo *o)
{

  float	xold;	/* Local variables */
  float	eold;
  float	bfold;
  float	ddiff;
  float	derold;
  float	ut;
  float	dut;
  pwr_sClass_CompPID *co = (pwr_sClass_CompPID *) o->PlcConnectP;

  if ( !co)
    return;

  /* Save old values */
  xold=co->ProcVal;
  eold=co->ControlDiff;
  bfold=co->Bias;
  derold=co->FiltDer;

  /* Get Input */
  co->ProcVal = *o->ProcValP;
  if ( o->SetValP != &o->SetVal)
    co->SetVal = *o->SetValP;
  if ( o->ForcValP != &o->ForcVal)
    co->ForcVal = *o->ForcValP;
  if ( o->BiasP != &o->Bias)
    co->Bias = *o->BiasP;
  if ( o->ForceP != &o->Force)
    co->Force = *o->ForceP;
  if ( o->IntOffP != &o->IntOff)
    co->IntOff = *o->IntOffP;
  else
    o->IntOff = co->IntOff;

  /* Calculate Controller Error and Filtered derivate */

  co->ControlDiff = co->ProcVal - co->SetVal;
  ddiff = ((co->PidAlg & DAVV) != 0) ?
    (co->ControlDiff - eold) / *o->ScanTime:
    (co->ProcVal - xold) / *o->ScanTime;
  if ((co->DerGain < 1.0) ||
      (co->DerGain * *o->ScanTime >= co->DerTime))
    co->FiltDer = ddiff * co->DerTime;		/* No Filter */
  else
    co->FiltDer += (ddiff - derold) *
      co->DerGain * *o->ScanTime; /* Filter */

  if ( co->Force ) {
    /* Force */
    dut = co->OutVal;
    co->OutVal = co->ForcVal;
    co->OutChange = co->OutVal - dut;
    co->EndMin = FALSE;
    co->EndMax = FALSE;
  }

  else {
    /* Auto mode */
    if ((co->PidAlg & IALG) != 0) {
      /* Incremental algorithm */
      /* Integral-part */
      if ((*o->IntOffP == FALSE) && (co->IntTime > 0))
	dut = co->ControlDiff * *o->ScanTime / co->IntTime;
      else
	dut = 0;
      if ((co->PidAlg & PALG) != 0) {
	/* Not pure I-controller */
	/* Derivative-part */
	if ((co->PidAlg & DALG) != 0)
	  dut += (co->FiltDer-derold);
	/* P-part */
	dut += ((co->PidAlg & PAVV) != 0) ?
	  co->ControlDiff - eold :
	  co->ProcVal - xold ;
	dut *= co->Gain;
      }
      if (co->Inverse != 0) dut = - dut;
      /* Bias */
      dut += co->BiasGain * (co->Bias - bfold);
      /* Limit output */
      ut = co->OutVal + dut;
      if (co->MaxOut > co->MinOut) {
	if (ut > co->MaxOut) {
	  ut = co->MaxOut;
	  co->EndMin = FALSE;
	  co->EndMax = TRUE;
	}
	else if (ut < co->MinOut) {
	  ut = co->MinOut;
	  co->EndMin = TRUE;
	  co->EndMax = FALSE;
	}
	else {
	  if (co->EndMin && (ut >= (co->MinOut + co->EndHys)))
	    co->EndMin = FALSE;
	  if (co->EndMax && (ut <= (co->MaxOut - co->EndHys)))
	    co->EndMax = FALSE;
	}
      }
    }
    
    else {
      /* Nonincremental algorithm */
      /* P-part */
      ut = co->ControlDiff;
      /* Derivative-part */
      if ((co->PidAlg & DALG) != 0)
	ut += co->FiltDer;
      /* Gain */
      ut *= co->Gain;
      if (co->Inverse != 0) ut = - ut;
      /* Bias */
      ut += co->BiasGain * co->Bias;
      /* Limit output */
      if (co->MaxOut > co->MinOut) {
	if (ut > co->MaxOut) {
	  ut = co->MaxOut;
	  co->EndMin = FALSE;
	  co->EndMax = TRUE;
	}
	else if (ut < co->MinOut) {
	  ut = co->MinOut;
	  co->EndMin = TRUE;
	  co->EndMax = FALSE;
	}
	else {
	  if (co->EndMin && (ut >= (co->MinOut + co->EndHys)))
	    co->EndMin = FALSE;
	  if (co->EndMax && (ut <= (co->MaxOut - co->EndHys)))
	    co->EndMax = FALSE;
	}
      }
      dut = ut - co->OutVal;
    }
    
    /* Output Auto */
    co->OutChange = dut;
    co->OutVal = ut;
  }

  /* Transfer outputs */
  o->OutVal = co->OutVal;
  o->OutChange = co->OutChange;
  o->ControlDiff = co->ControlDiff;
  o->EndMax = co->EndMax;
  o->EndMin = co->EndMin;
}

/*_*
  OnOffBurnerFo

  @aref onoffburnerfo OnOffBurnerFo
*/
void CompOnOffBurnerFo_init( pwr_sClass_CompOnOffBurnerFo  *o)
{
  pwr_tDlid dlid;
  pwr_tStatus sts;

  sts = gdh_DLRefObjectInfoAttrref( &o->PlcConnect, (void **)&o->PlcConnectP, &dlid);
  if ( EVEN(sts)) 
    o->PlcConnectP = 0;
}

void CompOnOffBurnerFo_exec( plc_sThread		*tp,
			     pwr_sClass_CompOnOffBurnerFo *o)
{
  pwr_sClass_CompOnOffBurner *co = (pwr_sClass_CompOnOffBurner *) o->PlcConnectP;
  pwr_sClass_CompOnOffZoneFo	*zono;
  pwr_sClass_CompOnOffZone	*zonco;
  pwr_tFloat32 Cnt;

  zono = (pwr_sClass_CompOnOffZoneFo *)
    ((char *)o->InP - (sizeof(pwr_sClass_CompOnOffZoneFo) - pwr_AlignLW(sizeof(pwr_tFloat32))));
  zonco = (pwr_sClass_CompOnOffZone *)zono->PlcConnectP;

  if ( !co || !zonco)
    return;

  co->Executing = zonco->Executing;
  if ( !co->Executing) {
    o->Status = co->Status = 0;
    co->BrTime = 0;
    co->TrendStatus = (pwr_tFloat32) co->Number;
    return;
  }
     
  if ( zonco->CycleCount < 0.5)
    co->OnDetected = 0;

  if ( (co->PulseOn && co->BrTime < zonco->BurnerTimeMinOn) ||
       (!co->PulseOn && co->BrTime < zonco->BurnerTimeMinOff)) {
    co->BrTime += *o->ScanTime;
    return;
  }

  if ( co->ManMode && co->OpMan) {
    if ( (o->Status && !co->ManStatus) || (!o->Status && co->ManStatus))
      co->BrTime = 0;
    o->Status = co->Status = co->ManStatus;
    co->TrendStatus = (pwr_tFloat32) co->Number + 0.8 * (co->Status ? 1 : 0);
    co->BrTime += *o->ScanTime;
    return;
  }
  else
    co->ManStatus = 0;

  if ( zonco->PauseMode)
    co->PulseTime = zonco->BurnerTimeMinOff;
  else
    co->PulseTime = zonco->BurnerTimeMinOn;

  Cnt = zonco->CycleCount - 100.0 * co->Number / zonco->NumberOfBurners;
  if ( Cnt < 0)
    Cnt += 100;
  if ( zonco->PauseMode)
    co->OffCnt = co->PulseTime / zonco->CycleTime * 100;
  else
    co->OffCnt = (zonco->CycleTime - co->PulseTime) / zonco->CycleTime * 100;

  if ( Cnt >= 0 && Cnt < co->OffCnt &&
       !co->Status) {
    // Turn on
    co->OnDetected = 1;
    co->Status = 1;
    co->BrTime = 0;
  }
  else if ( Cnt >= co->OffCnt &&
            co->Status) {
    // Turn off
    co->Status = 0;
    co->BrTime = 0;
  }
  co->BrTime += *o->ScanTime;
  co->TrendStatus = (pwr_tFloat32) co->Number + 0.8 * (co->Status ? 1 : 0);
  o->Status = co->Status;
}


/*_*
  CompOnOffZoneFo

  @aref componoffzonefo CompOnOffZoneFo
*/
void CompOnOffZoneFo_init( pwr_sClass_CompOnOffZoneFo  *o)
{
  pwr_tDlid dlid;
  pwr_tStatus sts;

  sts = gdh_DLRefObjectInfoAttrref( &o->PlcConnect, (void **)&o->PlcConnectP, &dlid);
  if ( EVEN(sts)) 
    o->PlcConnectP = 0;
}

void CompOnOffZoneFo_exec( plc_sThread	    *tp,
		           pwr_sClass_CompOnOffZoneFo *o)
{
  pwr_sClass_CompOnOffZone *co = (pwr_sClass_CompOnOffZone *) o->PlcConnectP;

  if ( !co)
    return;

  co->Power = o->Power = *o->PowerP;
  co->Executing = o->Execute = *o->ExecuteP;
  if ( co->Power < co->PowerMin)
    co->Power = co->PowerMin;
  if ( co->Power > co->PowerMax)
    co->Power = co->PowerMax;

  if ( !co->Executing) {
    co->CycleCount = 0;
    return;
  }

  co->PauseMode = ( co->Power / 100 > (co->BurnerTimeMinOff / (co->BurnerTimeMinOn + co->BurnerTimeMinOn))) ? 0 : 1;

  if ( co->PauseMode) {
    if ( co->Power != 100.0)
      co->CycleTime = co->BurnerTimeMinOff * 100.0 / co->Power;
  }
  else {
    if ( co->Power != 0)
      co->CycleTime = co->BurnerTimeMinOn * 100.0 / (100.0 - co->Power);
  }
  co->CycleCount += *o->ScanTime / co->CycleTime * 100;
  if ( co->CycleCount > 100)
    co->CycleCount = 0;
}











