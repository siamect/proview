/* rt_plc_io.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifdef OS_ELN
#include stdio
#else
#include <stdio.h>
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_plc.h"
#include "rt_plc_timer.h"

/**
  PiPos
  Function:
    Handles input from pulse-interface-card
    and converts it to position and speed.
    Handles Calibration in one or two points.

  @aref pipos PiPos
*/

void pipos_exec (
  plc_sThread	    *tp,
  pwr_sClass_pipos  *object
) {
  float		pdiff;	/* Position difference from Kal1 */
  int		idiff;	/* Raw value difference from Kal1 */

  /* Read Input */
  object->PulsIn = *object->PulsInP;
  object->CalPos1 = *object->CalPos1P;
  object->CalOrder1 = *object->CalOrder1P;
  object->CalPos2 = *object->CalPos2P;
  object->CalOrder2 = *object->CalOrder2P;

  /* Calibration point 1 */
  if (object->CalOrder1 && !object->CalOrder1Old) {
    object->PosCal1 = object->ActVal = object->CalPos1;
    object->PICal1 = object->PulsIn;

    /* Clear order if manual */
    if (object->CalOrder1P == &object->CalOrder1)
      object->CalOrder1 = FALSE;
  } else if (object->CalOrder2 && !object->CalOrder2Old) {
    /*	Calibration point 2 */
    pdiff = object->CalPos2 - object->PosCal1;
    idiff = object->PulsIn - object->PICal1;

    if (pdiff != 0 && idiff != 0) {
      object->Gain = pdiff/idiff;
      object->PosCal2 = object->ActVal = object->CalPos2;
    }

    /* Clear order if manual */
    if (object->CalOrder2P == &object->CalOrder2)
      object->CalOrder2 = FALSE;
  } else {
    /* Calculate position when no calibration */
    idiff = object->PulsIn - object->PICal1;
    object->ActVal = object->PosCal1 + object->Gain * idiff;
  }

  /* Save old values */
  object->CalOrder1Old = object->CalOrder1;
  object->CalOrder2Old = object->CalOrder2;
}

/**
  Count
  Function:
    Handles all functions for flank counter

  @aref count Count
*/

void count_exec (
  plc_sThread	    *tp,
  pwr_sClass_count  *object
) {
  float		old;	/* For flank detect */

  /* Count up */
  old = object->CountUp;
  object->CountUp = *object->CountUpP;
  if (object->CountUp && !old)
    object->Accum++;

  /* Count down	*/
  old = object->CountDown;
  object->CountDown = *object->CountDownP;
  if (object->CountDown && !old)
    object->Accum--;

  /* Initialize	*/
  old = object->Init;
  object->Init = *object->InitP;
  if (object->Init && !old)
    object->Accum = object->Preset;

  /* Clear */
  old = object->Clear;
  object->Clear = *object->ClearP;
  if (object->Clear && !old)
    object->Accum = 0;

  /* Set Output status */
  if (object->Accum > 0) {
    object->Pos = TRUE;
    object->Neg = FALSE;
    object->Zero = FALSE;
  } else if ( object->Accum == 0 ) {
    object->Pos = FALSE;
    object->Neg = FALSE;
    object->Zero = TRUE;
  } else {
    object->Pos = FALSE;
    object->Neg = TRUE;
    object->Zero = FALSE;
  }

  object->Equal = object->Accum == object->Preset;
}

/**
  BCDDO
  Function:
    Converts float to 4-digit BCD value.
    Overflow is output to next BCDDO-step

  @aref bcddo BCDDO
*/

void bcddo_exec	(
  plc_sThread	    *tp,
  pwr_sClass_bcddo  *object
) {
  int		val;	/* One digit */
  int		i;
  int		j;
  int		tal;
  int		oldtal;
  float		rest;	/* Overflow */
  pwr_tBoolean	*dopoint; /* Points to output */

  rest = object->In = *object->InP;		/* Get Input */
  dopoint = &object->BCD0;	/* Initialize pointer */

  if (rest < 0)
    rest = 0;
  tal = rest / 10000;
  tal = rest - tal * 10000;
  rest = (rest - tal) / 10000;
  for (i=0; i < 4; i++ ) {	/* Loop 4 digits */
    oldtal = tal;
    tal = tal / 10;
    val = oldtal - 10*tal;	/* Integer 0 - 9 */
    for (j=0; j < 4; j++ ) {
      *dopoint++ = (pwr_tBoolean) (val & 1);
      val = val / 2;
    }
  }
  object->Rest = rest;
}

/**
  DIBCD
  function:       Converts 4-digit BCD value to float.

  @aref dibcd DIBCD
*/

#define DIBCDSIZE       16

void dibcd_exec (
  plc_sThread	    *tp,
  pwr_sClass_dibcd  *object
) {
  int           val;    /* One digit */
  int           i;      /* Loop index*/
  int           j;      /* Loop index*/
  int           res;    /* Result */
  int           offset; /* Offset to next pointer */
  char          *ptr;   /* Pointer to pointer */
  pwr_tBoolean  *p2;    /* Pointer to digin */
  pwr_tBoolean  err;    /* Error flag */

  /* Initialize */
  res = 0;
  err = FALSE;
  offset = (char *)&object->BCD1P - (char *)&object->BCD0P;
  ptr = (char *)&object->BCD0P;
  ptr += (DIBCDSIZE -1) * offset;

  /* Double loop for convert */
  for ( i = 0; i < (DIBCDSIZE / 4); i++) {
    val = 0;
    for ( j = 0; j < 4; j++) {
      val += val;                         /* Mult 2 */
      p2 = *(pwr_tBoolean **)ptr;                 /* Pointer to input */
      if (*p2 != object->Inv) val++;    /* Signal till ? */
      ptr -= offset;                      /* Pointer to next pointer */
    }
    if ( val > 9 ) err = TRUE;
    res = 10 * res + val;
  }
  if (!err)
    object->ActVal = res;
  object->Error = err;
}

/**
  Gray
  Function:       
    Gray converts up to 16 digital Gray coded input signals
    to a analog output signal.
    Nota bene !
      Digital parameter för samtliga ingångar inverterade.
      Detta i väntan på generell inverteringshantering.

    Om ingångarna är inverterade måste ej använda ingångar ettställas!
    Minst signifikant bit på första pinnen. Ej använda signaler pekar på
    sig själva.
    Blockets signaler blir alltså:
                16 st digitala in :     di0 ... dif i grafiken
                                        Din0, Din0P ... DinF, DinFP i struct
                                        Insignalerna lagras ej i objektet.
                1 analog ut:            OUT i grafik, ActVal i struct
                1 digital parameter:    Inv anger samtliga in är inverterade
        Metoder bör likna DIBCD-blocket

        Gray-koden vid ej inverterade insignaler:
Signal  0 1 2 3  4 5 6 7  8 9 A B  C D E F      ActVal
        ----------------------------------------------
        0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0           0
        1 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0           1
        1 1 0 0  0 0 0 0  0 0 0 0  0 0 0 0           2
        0 1 0 0  0 0 0 0  0 0 0 0  0 0 0 0           3
        0 1 1 0  0 0 0 0  0 0 0 0  0 0 0 0           4
        1 1 1 0  0 0 0 0  0 0 0 0  0 0 0 0           5
        1 0 1 0  0 0 0 0  0 0 0 0  0 0 0 0           6
        0 0 1 0  0 0 0 0  0 0 0 0  0 0 0 0           7
        0 0 1 1  0 0 0 0  0 0 0 0  0 0 0 0           8
        1 0 1 1  0 0 0 0  0 0 0 0  0 0 0 0           9
        .
        1 0 0 0  1 0 0 0  0 0 0 0  0 0 0 0           30
        0 0 0 0  1 0 0 0  0 0 0 0  0 0 0 0           31
        0 0 0 0  1 1 0 0  0 0 0 0  0 0 0 0           32
        1 0 0 0  1 1 0 0  0 0 0 0  0 0 0 0           33
        .
        1 1 0 0  0 0 0 0  0 0 0 0  0 0 0 1       65533
        1 0 0 0  0 0 0 0  0 0 0 0  0 0 0 1       65534
        0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 1       65535

  @aref gray Gray
*/

#define GRAYSIZE        16

void gray_exec (
  plc_sThread	    *tp,
  pwr_sClass_gray *object
) {
  int		i;      /* Loopcounter */
  pwr_tBoolean  in;     /* Digital in after invert */
  pwr_tBoolean  odd;    /* Convert flag */
  int		sum;    /* Convert sum */
  int		offset; /* Offset to next pointer */
  char		*ptr;   /* Pointer to ptr to digin */
  pwr_tBoolean  *p2;    /* Pointer to digin */

  /* Init */
  odd = 0;
  sum = 0;
  offset = (char *)&object->Din1P - (char *)&object->Din0P;
  ptr = (char *)&object->Din0P;
  ptr += (GRAYSIZE - 1) * offset;

  /* Graycode convert loop */
  for (i = 0; i < GRAYSIZE; i++) {
    sum += sum;                   /* Mult 2 */
    p2 = *(pwr_tBoolean **)ptr;           /* Pointer to next dig in */
    in = (*p2 != object->Inv);    /* Invert ? */
    odd = in ? !odd : odd;        /* Odd up to now ? */
    sum += odd;                   /* Inc if odd input */
    ptr -= offset;
  }

  /* Result */
  object->ActVal = sum;
}
