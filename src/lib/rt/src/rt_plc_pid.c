/* rt_plc_pid.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifdef OS_ELN
# include stdio
#else
# include <stdio.h>
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_plc.h"
#include "rt_plc_timer.h"

/* 		PLC RUTINER			*/

/*_*
  INC3P
  function:	accumulate output change from controller
		and convert to Open or Close orders.

  @aref inc3p Inc3p
*/
void inc3p_init(object)
pwr_sClass_inc3p 	*object;
{
	object->Acc = 0;
}

void inc3p_exec(
  plc_sThread		*tp,
  pwr_sClass_inc3p 	*object)
    {
    /*  Clear old orders */

    if (object->Open && object->Acc <= 0) object->Open = FALSE;
    if (object->Close && object->Acc >= 0) object->Close = FALSE;

    /* Get  Incremental input */

    object->OutChange = *object->OutChangeP;
    object->Acc += object->OutChange * object->Gain;

    if	/* Open */
	(
	(object->Acc >= object->MinTim) || 
	    (
	    (object->Open == TRUE) &&
	    (object->Acc > 0) &&
	    (object->TimerFlag == TRUE)
	    )
	)
	{
	*object->CloseP = FALSE;
	object->Close = FALSE;
	object->TimerDO = object->OpenP;
	object->TimerTime = object->Acc;
	timer_in( tp, object);
	if (object->TimerCount > 0)
	    {
	    object->Open = TRUE;
	    *object->OpenP = TRUE;
	    object->AccTim = 0;
	    if (object->Acc > *object->ScanTime)
		object->Acc -= *object->ScanTime;
	    else
		object->Acc = 0;
	    }
	else
	    {
	    object->Open = FALSE;
	    *object->OpenP = TRUE;
	    object->TimerCount = 0;
	    object->AccTim += *object->ScanTime;
	    }
	}
    else if /* Close */
	(
	(object->Acc <= -object->MinTim) ||
	    (
	    (object->Close == TRUE) &&
	    (object->Acc < 0) &&
	    (object->TimerFlag == TRUE)
	    )
	)
	{
	*object->OpenP = FALSE;
	object->Open = FALSE;
	object->TimerDO = object->CloseP;
	object->TimerTime = -object->Acc;
	timer_in( tp, object);
	if (object->TimerCount > 0)
	    {
	    object->Close = TRUE;
	    *object->CloseP = TRUE;
	    object->AccTim = 0;
	    if (object->Acc < - *object->ScanTime)
		object->Acc += *object->ScanTime;
	    else
		object->Acc = 0;
	    }
	else
	    {
	    object->Close = FALSE;
	    *object->CloseP = TRUE;
	    object->TimerCount = 0;
	    object->AccTim += *object->ScanTime;
	    }
  	} 
    else    /* No output */
	{
	object->Open = FALSE;
	*object->OpenP = FALSE;
	object->Close = FALSE;
	*object->CloseP = FALSE;
	object->TimerCount = 0;
	if (object->MaxTim > 0)	/* Limit integration */
	    {
	    object->AccTim += *object->ScanTime;
	    if (object->AccTim >= object->MaxTim)
		{
		object->Acc = 0;
		object->AccTim = 0;
		}
	    }
	}
    }

/*_*
  POS3P
  function:	Compare controller output to actual position
		Open or close if outside of deadzone.

  @aref pos3p Pos3p

*/
void pos3p_exec(
  plc_sThread		*tp,
  pwr_sClass_pos3p 	*object)
{
	float		error;

/* Get  Setpoint and pos */
	object->OutVal = *object->OutValP;
	object->Pos = *object->PosP;
	error = *object->OutValP - *object->PosP;

/* Open ? */
	if ((error > object->ErrSta) ||
		((object->Open == TRUE) && (error > object->ErrSto)))
	{
	  *object->CloseP = FALSE;
	  object->Close = FALSE;
	  object->Open = TRUE;
	  object->TimerDO = object->OpenP;
	  object->TimerTime = error * object->Gain;
	  if ((error <= object->ErrSta) && (*object->OpenP == FALSE))
		object->TimerTime = 0;
	  timer_in( tp, object);
	  if ( object->TimerCount > 0 )
	  {
	    *object->OpenP = TRUE;
	  }
	  else
	  {
	    *object->OpenP = FALSE;
	    object->TimerCount = 0;
	  }
  	} 
/* Close ? */
	else if ((error < -object->ErrSta) ||
		((object->Close == TRUE) && (error < -object->ErrSto)))
	{
	  *object->OpenP = FALSE;
	  object->Open = FALSE;
	  object->Close = TRUE;
	  object->TimerDO = object->CloseP;
	  object->TimerTime = -error * object->Gain;
	  if ((error >= -object->ErrSta) && (*object->CloseP == FALSE))
		object->TimerTime = 0;
	  timer_in( tp, object);
	  if ( object->TimerCount > 0 )
	  {
	    *object->CloseP = TRUE;
	  }
	  else
	  {
	    *object->CloseP = FALSE;
	    object->TimerCount = 0;
	  }
  	} 
	else
	{
/* No output */
	  object->Open = FALSE;
	  *object->OpenP = FALSE;
	  object->Close = FALSE;
	  *object->CloseP = FALSE;
	  object->TimerCount = 0;
	}
}
/*_*
  OUT2P
  function:	Konvert analog controll output to binary on / off

  @aref out2p Out2p
*/
void out2p_exec(
  plc_sThread		*tp,
  pwr_sClass_out2p 	*object)
{
	float	ontime;
	float	offtime;
/* Get  Input */
	object->OutVal = *object->OutValP;

	if (object->MaxOut > object->MinOut)
	{
	  ontime = (object->OutVal - object->MinOut) /
	     (object->MaxOut - object->MinOut) * object->Period;
	  offtime = object->Period - ontime;
	}
	else
	{
	  ontime = 0;
	  offtime = 0;
	}
/* Increase Running-time */
	object->RunTime += *object->ScanTime;

/* Should we turn off ? */
	if (object->Order == TRUE)
	{
	  if ((object->RunTime > ontime) && (object->OutVal < object->MaxOut))
	  {
	    object->Order = FALSE;
	    object->RunTime = 0;
	  }
	}
/* Should we turn on ? */
	else
	{
	  if ((object->RunTime > offtime) && (object->OutVal > object->MinOut))
	  {
	    object->Order = TRUE;
	    object->RunTime = 0;
	  }
	}
}

/*_*
   MODE
  function:	Set controller or drive in different operation mode.
		Pass analog values depending on mode.
  @aref mode Mode
*/

void mode_exec(
  plc_sThread		*tp,
  pwr_sClass_mode		*object)
{

/* Get indata */
	object->ProcVal = *object->ProcValP;
	object->XSetVal = *object->XSetValP;
	object->XForcVal = *object->XForcValP;
	object->Forc1 = *object->Forc1P;
	object->Forc2 = *object->Forc2P;
	object->OutVal = *object->OutValP;

/* Make appropriate actions, depending on actual mode */

  /* Manual */
	if (object->OpMod <= 1)
	{
	  object->Force = TRUE;
	  object->AutMode = FALSE;
	  object->CascMod = FALSE;
    /* External setpoint ? */
	  if ((object->AccMod & 2) == 0)
	      object->SetVal = object->XSetVal;
    /* Test if Force in manual mode */
	  if (object->Forc1 )
		  object->ForcVal = object->XForcVal;
	}
	else
  /* Not Manual Mode */
	{
    /* Auto */
	  if (object->OpMod == 2)
	  {
	    object->AutMode = TRUE;
	    object->CascMod = FALSE;
	  }
    /* Cascade mode */
	  else
	  {
	    object->AutMode = FALSE;
	    object->CascMod = TRUE;
	    object->SetVal = object->XSetVal;
	  }
  /* Test if force in Auto or Cascade */
	  if ( object->Forc1 || object->Forc2 )
	  {
	    object->Force = TRUE;
	    object->ForcVal = object->XForcVal;
	  }
	  else
	  {
	    object->Force = FALSE;
	    object->ForcVal = object->OutVal;
	  }
	}
}

/*_*
  PID
  function:	PID or PD Controller with basic facilities
		Possible to turn off integration and to force
		output to desired value.

  @aref pid Pid
*/

void pid_exec(
  plc_sThread		*tp,
  pwr_sClass_pid		*object)
{

/* Define Algoritm bitmask */
#define IALG 1		/* Integral part -> Incremental algorithm */
#define PALG 2		/* Proportional part exists */
#define PAVV 4		/* Proportional part working on control difference */
#define DALG 8		/* Derivative part exists */
#define DAVV 16		/* Derivative part working on control difference */

	float	xold;	/* Local variables */
	float	eold;
	float	bfold;
	float	ddiff;
	float	derold;
	float	ut;
	float	dut;

/* Save old values */
xold=object->ProcVal;
eold=object->ControlDiff;
bfold=object->Bias;
derold=object->FiltDer;

/* Get Input */
object->ProcVal = *object->ProcValP;
object->SetVal = *object->SetValP;
object->ForcVal = *object->ForcValP;
object->Bias = *object->BiasP;
object->Force = *object->ForceP;
object->IntOff = *object->IntOffP;

/* Calculate Controller Error and Filtered derivate */

object->ControlDiff = object->ProcVal - object->SetVal;
ddiff = ((object->PidAlg & DAVV) != 0) ?
  (object->ControlDiff - eold) / *object->ScanTime:
  (object->ProcVal - xold) / *object->ScanTime;
if ((object->DerGain < 1.0) ||
    (object->DerGain * *object->ScanTime >= object->DerTime))
    object->FiltDer = ddiff * object->DerTime;		/* No Filter */
else
    object->FiltDer += (ddiff - derold) *
      object->DerGain * *object->ScanTime; /* Filter */

if ( object->Force )
/* Force */
{
  dut = object->OutVal;
  object->OutVal = object->ForcVal;
  object->OutChange = object->OutVal - dut;
  object->EndMin = FALSE;
  object->EndMax = FALSE;
}

else
/* Auto mode */
{
  if ((object->PidAlg & IALG) != 0)
  /* Incremental algorithm */
  {
    /* Integral-part */
    if ((*object->IntOffP == FALSE) && (object->IntTime > 0))
      dut = object->ControlDiff * *object->ScanTime / object->IntTime;
    else
      dut = 0;
    if ((object->PidAlg & PALG) != 0)
    /* Not pure I-controller */
    {
      /* Derivative-part */
      if ((object->PidAlg & DALG) != 0)
        dut += (object->FiltDer-derold);
      /* P-part */
      dut += ((object->PidAlg & PAVV) != 0) ?
        object->ControlDiff - eold :
        object->ProcVal - xold ;
      dut *= object->Gain;
    }
    if (object->Inverse != 0) dut = - dut;
    /* Bias */
    dut += object->BiasGain * (object->Bias - bfold);
    /* Limit output */
    ut = object->OutVal + dut;
    if (object->MaxOut > object->MinOut)
    {
      if (ut > object->MaxOut)
      {
        ut = object->MaxOut;
        object->EndMin = FALSE;
        object->EndMax = TRUE;
      }
      else if (ut < object->MinOut)
      {
        ut = object->MinOut;
        object->EndMin = TRUE;
        object->EndMax = FALSE;
      }
      else
      {
        if (object->EndMin && (ut >= (object->MinOut + object->EndHys)))
          object->EndMin = FALSE;
        if (object->EndMax && (ut <= (object->MaxOut - object->EndHys)))
          object->EndMax = FALSE;
      }
    }
  }

  else
  /* Nonincremental algorithm */
  {
    /* P-part */
    ut = object->ControlDiff;
    /* Derivative-part */
    if ((object->PidAlg & DALG) != 0)
      ut += object->FiltDer;
    /* Gain */
    ut *= object->Gain;
    if (object->Inverse != 0) ut = - ut;
    /* Bias */
    ut += object->BiasGain * object->Bias;
    /* Limit output */
    if (object->MaxOut > object->MinOut)
    {
      if (ut > object->MaxOut)
      {
        ut = object->MaxOut;
        object->EndMin = FALSE;
        object->EndMax = TRUE;
      }
      else if (ut < object->MinOut)
      {
        ut = object->MinOut;
        object->EndMin = TRUE;
        object->EndMax = FALSE;
      }
      else
      {
        if (object->EndMin && (ut >= (object->MinOut + object->EndHys)))
          object->EndMin = FALSE;
        if (object->EndMax && (ut <= (object->MaxOut - object->EndHys)))
          object->EndMax = FALSE;
      }
    }
    dut = ut - object->OutVal;
  }

  /* Output Auto */
  object->OutChange = dut;
  object->OutVal = ut;
}

}
