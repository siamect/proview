/* rt_plc_arithm.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifdef OS_ELN
#  include stdio
#else
#  include <stdio.h>
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_plc.h"


/* 		PLC RUTINER			*/

/**
  SUM
  funktion:       Add  up to 8 analogue values.
                  Each value is multiplied with  a factor.

  @aref sum Sum
*/
void sum_exec(
  plc_sThread		*tp,
  pwr_sClass_sum	*object)
{
#define sumsize 8
	char    *charp;
        float	**ptr;          /* Pointer to ptr to input */
	static int offset = sizeof( object->In1) + sizeof( object->In1P); 
				/* Offset to next input */
        int     i;              /* Loopindex */
        float   sum;            /* Result */
/* Initialize */
        sum = object->Const;
        ptr = &object->In1P;
/* SUM loop */
        for ( i = 0 ; i < sumsize ; i++ )
        {
          if ( *ptr != NULL )
            sum += **ptr * object->FVect[i];

          charp = (char *) ptr;
          charp += offset;
          ptr = (float **)charp;
        }
/* Result */
        object->ActVal = sum;
}

/**
  MAXMIN
  funktion:       Select largest and smallest value of up to
                  8 input values.

  @aref maxmin MaxMin
*/

void maxmin_exec(
  plc_sThread		*tp,
  pwr_sClass_maxmin       *object)
{
#define maxminsize      8
#define maxmin_maxfloat        1.E37
#define maxmin_minfloat        -1.E37

        float   minval;            /* Lowest value */
        float   maxval;            /* Highest value */
        int     i;              /* Loopcounter */
	static int offset = sizeof( object->In1) + sizeof( object->In1P); /* Offset to next input */
        char    *charp;
        float   **ptr;          /* Pointer to ptr to digin */

/* Initialize */
        ptr = &object->In1P;
        minval = maxmin_maxfloat;
        maxval = maxmin_minfloat;
/* MaxMin loop */
        for ( i = 0 ; i < maxminsize ; i++)
        {
          if ( *ptr != NULL )
          {
	    if ( **ptr > maxval) maxval = **ptr;
	    if ( **ptr < minval) minval = **ptr;
          }
          charp = (char *) ptr;
          charp += offset;
          ptr = (float **)charp;
        }
/* Set Output */
        object->MaxVal = maxval;
        object->MinVal = minval;
}

/**
  LIMIT
  funktion:	Upper and lower limit of analog value

  @aref limin Limit
*/
void limit_exec(
  plc_sThread		*tp,
  pwr_sClass_limit 	*object)
{
	object->Max = *object->MaxP;
	object->Min = *object->MinP;
	object->In = *object->InP;

  	if (object->In > object->Max)
	{
	  object->ActVal = object->Max;
	  object->High = TRUE;
	  object->Low = FALSE;
	}
  	else if (object->In < object->Min)
	{
	  object->Low = TRUE;
	  if (object->Min <= object->Max)
	  {
	    object->ActVal = object->Min;
	    object->High = FALSE;
	  }
	  else
	  {
	    object->ActVal = object->Max;
	    object->High = TRUE;
	  }
	}
  	else
	{
	  object->ActVal = object->In;
	  object->High = FALSE;
	  object->Low = FALSE;
	}
}

/**
  COMPH
  funktion:	Compare Analog Value with high limit

  @aref comph Comph
*/
void comph_exec(
  plc_sThread		*tp,
  pwr_sClass_comph 	*object)
{
	object->In = *object->InP;
	object->Lim = *object->LimP;

  	if (object->High)
	{
	  if ( object->In <= (object->Lim - object->Hysteres))
  	    object->High = FALSE;
	}
	else
	  if (object->In > object->Lim ) object->High = TRUE;
}

/**
  COMPL
  funktion:	Compare Analog Value with low limit

  @aref compl Compl
*/
void compl_exec(
  plc_sThread		*tp,
  pwr_sClass_compl 	*object)
{
	object->In = *object->InP;
	object->Lim = *object->LimP;

  	if (object->Low)
	{
	  if ( object->In >= (object->Lim + object->Hysteres))
  	    object->Low = FALSE;
	}
	else
	  if (object->In < object->Lim ) object->Low = TRUE;
}

/**
  SELECT
  funktion:	Select between two analog values depending on
		one logical control value

  @aref select Select
*/
void select_exec(
  plc_sThread		*tp,
  pwr_sClass_select 	*object)
{
	object->Control = *object->ControlP;

	if (object->Control)
	{
  	  object->ActVal = *object->HighP;
  	  object->NotActVal = *object->LowP;
  	}
  	else
	{
  	  object->ActVal = *object->LowP;
  	  object->NotActVal = *object->HighP;
  	}
}

/**
  RAMP
  funktion:	Limit output change.
 		If external feedback is used we limit change
		from this signal.

  @aref ramp Ramp
*/

void ramp_init(
  pwr_sClass_ramp 	*object)
{

	object->In = *object->InP;
	object->ActVal = object->In;
}

/* New ramp_exec with RampUp used when absolute value is increasing */
/*  and RampDown used when absolute value is decreasing */
/* New functionality will be used when new Boolean parameter 'RampUpAbs' */
/*  is on. For Proview 3.0 we will use 'AccUp' not zero for new functionality */
/* If this parameter is zero the routine will workin the old way with */
/*  RampUp for increasing and RampDown for decreasing */
/* 2001-03-13 Hans Werner */

void ramp_exec(
  plc_sThread		*tp,
  pwr_sClass_ramp 	*object)
{
	float limit1;		/* First limit */
	float limit2;		/* Second limit at sign change */
	float old;		/* Actual start value */
	float out;		/* New output */
	float scan;		/* scantime in seconds */

	/* Assume new output as unlimited as a start */
	out = object->In = *object->InP;	/* Get aimed value */
	old = *object->FeedBP;			/* Startvalue */
	scan = *object->ScanTime;		/* Get scantime */

	if (out > old)			/* Increase */
	{
	  if (old >= 0.0)		/* Positive rising even more */
	  {
	    limit1 = scan * object->RampUp;
	    if ((limit1 > 0.0) && (out > old + limit1)) out = old + limit1;
	  }
	  else if (out <= 0.0)		/* Negative rising towards zero */
	  {
	    if (object->RampUpAbs) limit1 = scan * object->RampDown;
	    else limit1 = scan * object->RampUp;
	    if ((limit1 > 0.0) && (out > old + limit1)) out = old + limit1;
	  }
	  else				/* From Neg to Pos */
	  {
	    if (object->RampUpAbs)
	    {
	      limit1 = scan * object->RampDown;
	      limit2 = scan * object->RampUp;
	    }
	    else limit1 = limit2 = scan * object->RampUp;

	    if (limit1 <= 0.0)	/* No limit up to zero */
	    {
	      if ((limit2 > 0.0) && (out > limit2)) out = limit2;
	    }
	    else if (old <= -limit1)	/* Will still not reach zero */
	    {
	      if (out > old + limit1) out = old + limit1;
	    }
	    else if (limit2 > 0)	/* Use second limitation above zero */
	    {
	      if (old < -limit2) out = 0.0;
	      else if (out > old + limit2) out = old + limit2;
	    }
	  } /* End Neg to Pos */
	} /* End Increasing */
	else if (out < old)			/* Decrease */
	{
	  if (old <= 0.0)		/* Negative falling even more */
	  {
	    if (object->RampUpAbs) limit1 = scan * object->RampUp;
	    else limit1 = scan * object->RampDown;
	    if ((limit1 > 0.0) && (out < old - limit1)) out = old - limit1;
	  }
	  else if (out >= 0.0)		/* Positive falling towards zero */
	  {
	    limit1 = scan * object->RampDown;
	    if ((limit1 > 0.0) && (out < old - limit1)) out = old - limit1;
	  }
	  else				/* From Pos to Neg */
	  {
	    if (object->RampUpAbs)
	    {
	      limit1 = scan * object->RampDown;
	      limit2 = scan * object->RampUp;
	    }
	    else limit1 = limit2 = scan * object->RampDown;

	    if (limit1 <= 0.0)	/* No limit down to zero */
	    {
	      if ((limit2 > 0.0) && (out < -limit2)) out = -limit2;
	    }
	    else if (old >= limit1)	/* Will still not reach zero */
	    {
	      if (out < old - limit1) out = old - limit1;
	    }
	    else if (limit2 > 0)	/* Use second limitation below zero */
	    {
	      if (old > limit2) out = 0.0;
	      else if (out < old - limit2) out = old - limit2;
	    }
	  } /* End Neg to Pos */
	} /* End Decreasing */

	object->ActVal = out;	/* New output */
}

/**
  FILTER
  funktion:	Exponential filter.
		If external feedback is used we filter
		from this signal.
	out = old + (in-old)* Ts / Tf

  @aref filter Filter
*/
void filter_init(object)
pwr_sClass_filter 	*object;
{
	object->In = *object->InP;
	object->ActVal = object->In;
}
void filter_exec(
  plc_sThread		*tp,
  pwr_sClass_filter 	*object)
{
	object->In = *object->InP;
	if (object->FiltCon > *object->ScanTime)
	  object->ActVal = *object->FeedBP +
	    (object->In - *object->FeedBP) * *object->ScanTime / object->FiltCon;
	else
	  object->ActVal = object->In;
}

/**
  SPEED
  Function:	Calculate difference divided to scantime

  @aref speed Speed
*/
void speed_exec(
  plc_sThread		*tp,
  pwr_sClass_speed 	*object)
{
	float	old;

	old = object->In;
	object->In = *object->InP;
	object->ActVal = (object->In - old) / *object->ScanTime * object->TimFact;
}

/**
  TIMINT
  Function:       Integration in time

  @aref timint Timint
*/
void timint_exec(
  plc_sThread		*tp,
  pwr_sClass_timint       *object)
{
/* Clear ? */
        if (*object->ClearP && !object->Clear)
        {
          object->OldAcc = object->ActVal;
          object->ActVal = 0;
        }
        object->Clear = *object->ClearP;
/* Ackumulate new value */
        object->ActVal += *object->InP * *object->ScanTime / object->TimFact;
}

/**
  CURVE
  Funktion:	Interpollation in a table

  @aref curve Curve
*/
void curve_exec(
  plc_sThread		*tp,
  pwr_sClass_curve		*object)
{
	float	x0;
	float	x1;
	float	y0;
	float	y1;
	float	number;
	float	*tabpointer;
/* Get input */
	object->In = *object->InP;
	if (object->TabP == NULL) object->ActVal = object->In;
	else
/* Get pointer to table, and number of pairs in table */
	{
	  tabpointer = object->TabP;
	  number = *tabpointer++;
	  if (number <= 0) object->ActVal = object->In;
	  else
/* Search in table */
	  {
	    x1 = *tabpointer++;
	    y1 = *tabpointer++;
	    if ( object->In <= x1 ) object->ActVal = y1;
	    else
	    {
	      for ( ; (number > 1) && ( object->In > x1) ; number--)
	      {
	        x0 = x1;
	        x1 = *tabpointer++;
	        y0 = y1;
	        y1 = *tabpointer++;
	      }
	      if ( object->In > x1) object->ActVal = y1; /* End of table */
	      else  object->ActVal = y0 + (y1 - y0) *
		  (object->In - x0) / (x1 - x0); /* Interpollation */
	    }
	  }
	}
}

#if 0

	TABLE
	Funktion:	Table for interpollation
#define table_exec(tableobject)  /* Table. No code for execution */
#endif

/**
	ADELAY

	Funktion:	
	This routine performes the function of delay for an analog or any
	float value. Values are stored in the structure for the object.
	There is  a small difference in operation depending on the maximum
	delay time (MaxTime). If MaxTime <= vektorsize*scantime, 
	all numbers are stored in the vektor.
	Otherwise if MaxTime is greater then vektorsize*scantime 
	MaxTime/vektorsize*scantime=Count numbers are via a filter algorithm
	used to give the next value that is stored in the vektor.
	The value for MaxTime is set by means of the editor. The vektorsize is
	set in the stucture.
	
	Structure data:
     	In		Analog input data
     	Tim		Actual delay time
     	MaxCount	Number of cycles per step
     	StoredNumbers	Number of stored values, set to 0 at init
     	StoInd		Index to last stored value
     	Count		Actual number of loop since last shift
     	Values		Vektor with saved values
     	ActualValue	Result from the routine 

  @aref adelay Adelay
*/

void adelay_init(object)
pwr_sClass_adelay		*object;
{
	object->StoredNumbers = 0;
	object->StoInd = -1;
	object->Count = object->MaxCount;
	object->ActVal = *object->InP;
}
void adelay_exec(
  plc_sThread		*tp,
  pwr_sClass_adelay		*object)
{

/* Local variables */
	int	maxindex;
	int	actindex;
	int	actoff;

	maxindex = sizeof(object->TimVect)/4;

/*		Get input
*/
	object->In = *object->InP;
	object->Tim = *object->TimP;

/*		Calculate average in each storeplace.
		MaxCount is number of cycles before each shift
*/
	object->Count++;
	if (object->Count >= object->MaxCount )
	{
	  object->StoInd++;
	  if ((object->StoInd >=  maxindex) || (object->StoInd < 0))
	 	object->StoInd = 0;
	  if (object->StoredNumbers <= maxindex) 
		object->StoredNumbers++;
	  object->Count = 0;
	  object->TimVect[object->StoInd] = object->In;
	}
	else
	  object->TimVect[object->StoInd] = (object->TimVect[object->StoInd] *
		object->Count + object->In) / (object->Count + 1);

/*		Calculate position for output
*/
	actoff = (object->Tim / *object->ScanTime - object->Count)
		/ object->MaxCount;
	if (actoff >= object->StoredNumbers)
		actoff = object->StoredNumbers - 1;
	actindex = object->StoInd - actoff;
	if (actindex < 0) actindex += maxindex;

	object->ActVal=object->TimVect[actindex];
}	

/**
  PISPEED
  function:       Measure flow with pulse input

  @aref pispeed PiSpeed
*/

void pispeed_init(object)
pwr_sClass_pispeed              *object;
{
/* Read input */
        object->PulsIn = *object->PulsInP;
}

void pispeed_exec(
  plc_sThread		*tp,
  pwr_sClass_pispeed              *object)
{
  int   piold;          /* Old input */

/* Read input */
        piold = object->PulsIn;
        object->PulsIn = *object->PulsInP;
/* Calculate flow */
        object->ActVal = (object->PulsIn - piold) * object->Gain *
                object->TimFact / *object->ScanTime;
}
