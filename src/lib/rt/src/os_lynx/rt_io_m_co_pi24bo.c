/* rt_io_m_co_pi24bo.c -- io methods for ssab cards.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#if defined(OS_ELN)
# include $vaxelnc
# include $kernelmsg
# include string
# include stdlib
# include stdio
#else
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <sys/file.h>
#endif


#include "pwr.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "qbus_io.h"

#define IO_MAXCHAN 	4
#define	MIN16          -32767
#define MIN24          -8388607
#define MAXCO16        65536
#define MAXCO24        16777216
#define MAX24          8388607
#define MAX16          32767


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

typedef struct {
	unsigned int	Address;
	int		Qbus_fp;
	pwr_tInt32	OldValue[IO_MAXCHAN];
	int		FirstScan[IO_MAXCHAN];
} io_sLocal;

typedef struct {
	int		Qbus_fp;
} io_sRackLocal;


static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  pwr_tStatus		sts;
  pwr_sClass_Co_PI24BO	*op;
  io_sLocal 		*local;
  int			i;
  pwr_tUInt16		wr_data[2];
  pwr_tUInt32		*wr_data_p;
  qbus_io_write		wb;

  op = (pwr_sClass_Co_PI24BO *) cp->op;

  local = calloc( 1, sizeof(*local));

  for ( i = 0; i < IO_MAXCHAN; i++)
    local->FirstScan[i] = 1;
  cp->Local = local;
  local->Address = op->RegAddress;
  local->Qbus_fp = ((io_sRackLocal *)(rp->Local))->Qbus_fp;

  errh_Info( "Init of co card '%s'", cp->Name);

  /* Configure card */
  for ( i = 0; i < op->MaxNoOfCounters; i++)
  {
    wr_data_p = (pwr_tUInt32 *) &wr_data;
    *wr_data_p = op->SyncRawValue[i];
    if ( op->SpeedMeasurement[i] == 1)
      wr_data[1] |= (1 << 8);
    if ( op->LengthMeasurement[i] == 1)
      wr_data[1] |= (1 << 10);
    if ( op->MulCount[i] == 2)
      wr_data[1] |= (1 << 11);
    if ( op->DivCount[i] == 16)
      wr_data[1] |= (1 << 9);
    if ( op->NoOfBits[i] == 16)
      wr_data[1] |= (1 << 12);
    if ( op->CopWrRough[i] == 1)
      wr_data[1] |= (1 << 13);
    if ( op->CopWrFine[i] == 1)
      wr_data[1] |= (1 << 14);
    if ( op->LoadWrReg[i] == 1)
      wr_data[1] |= (1 << 15);
 
    wb.Address = local->Address + 4*i;
    wb.Data = wr_data[0];
    sts = write( local->Qbus_fp, &wb, sizeof(wb));
    if ( sts != -1)
    {
      wb.Address += 2;
      wb.Data = wr_data[1];
      sts = write( local->Qbus_fp, &wb, sizeof(wb));
    }
    if ( sts == -1)
    {
      errh_Error( "IO init, Fatal write error, card '%s', IO i stopped", cp->Name);
      return IO__ERRDEVICE;
    }

    if ( op->LoadWrReg[i] == 1)
    {
      * (pwr_tUInt32 *) cp->chanlist[i].vbp = op->SyncRawValue[i];
      * (pwr_tUInt32 *) cp->chanlist[i].abs_vbp = op->SyncRawValue[i];
    }
    else
    {
      * (pwr_tUInt32 *) cp->chanlist[i].vbp = 0;
    }
    /* Data is written to device */
    op->COWrFlag[i] = 0;

  }

  return 1;
}

/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardClose (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  io_sLocal 		*local;

  local = (io_sLocal *) cp->Local;

  errh_Info( "IO closing co card '%s'", cp->Name);
  free( (char *) local);

  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  io_sLocal 		*local;
  pwr_tUInt16		wr_data[2];
  pwr_tUInt16		re_data[2];
  pwr_tUInt32		*wr_data_p;
  pwr_tUInt32		*re_data_p;
  pwr_sClass_Co_PI24BO	*op;
  pwr_tInt32		numofword;
  int			i;
  pwr_tInt16		co16_data;
  pwr_tInt32		newdiff,diff,co24_data;
  int			sts, sts1, sts2;
  qbus_io_read 		rb;
  qbus_io_write		wb;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Co_PI24BO *) cp->op;

  for ( i = 0; i < op->MaxNoOfCounters; i++)
  { 
    if ( op->ConvMask & (1 << i))
    {
      /* Conversion is on */

      if ( op->COWrFlag[i])
      {
        /* New configuration of card */

        wr_data_p = (pwr_tUInt32 *) &wr_data;
        *wr_data_p = op->SyncRawValue[i];
        if ( op->SpeedMeasurement[i] == 1)
          wr_data[1] |= (1 << 8);
        if ( op->LengthMeasurement[i] == 1)
          wr_data[1] |= (1 << 10);
        if ( op->MulCount[i] == 2)
          wr_data[1] |= (1 << 11);
        if ( op->DivCount[i] == 16)
          wr_data[1] |= (1 << 9);
        if ( op->NoOfBits[i] == 16)
	  wr_data[1] |= (1 << 12);
        if ( op->CopWrRough[i] == 1)
          wr_data[1] |= (1 << 13);
        if ( op->CopWrFine[i] == 1)
          wr_data[1] |= (1 << 14);
        if ( op->LoadWrReg[i] == 1)
          wr_data[1] |= (1 << 15);
 
        wb.Address = local->Address + 4*i;
        wb.Data = wr_data[0];
        sts = write( local->Qbus_fp, &wb, sizeof(wb));
        if ( sts != -1)
        {
          wb.Address += 2;
          wb.Data = wr_data[1];
          sts = write( local->Qbus_fp, &wb, sizeof(wb));
        }

        if ( sts == -1)
        {
          errh_Error( "IO init, Fatal write error, card '%s', IO i stopped", cp->Name);
        }
        op->COWrFlag[i] = 0;
      }

      if ( op->NoOfBits[i] == 24)
        numofword = 2;
      else
        numofword = 1;			/* 16 bitscounter */

      re_data_p = (pwr_tUInt32 *) &re_data;
#if defined(OS_ELN)
      vaxc$establish(machfailread_co);
#endif
      rb.Address = local->Address + 4*i;
      sts1 = read( local->Qbus_fp, &rb, sizeof(rb));
      re_data[0] = (unsigned short) rb.Data;
      if ( numofword == 2)
      {
        rb.Address += 2;
        sts2 = read( local->Qbus_fp, &rb, sizeof(rb));
        re_data[1] = (unsigned short) rb.Data;
      }
      else
	sts2 = 0;
      if ( sts1 == -1 || sts2 == -1)
      {
#if 0
        if ( io_fatal_error)
        {
          /* Activate emergency break */
          errh_Error( "Fatal read error, card '%s', IO i stopped", cp->Name);
          ctx->Node->EmergBreakTrue = 1;
          return IO__ERRDEVICE;
        }
#endif

        /* Increase error count and check error limits */
        op->ErrorCount++;

        if ( op->ErrorCount == op->ErrorSoftLimit)
          errh_Error( "IO Error soft limit reached on card '%s'", cp->Name);
        if ( op->ErrorCount >= op->ErrorHardLimit)
        {
          errh_Error( "IO Error hard limit reached on card '%s', IO stopped", cp->Name);
          ctx->Node->EmergBreakTrue = 1;
          return IO__ERRDEVICE;
        }
        continue;
      }
    }

    /* No faults detected calculate countervalue and store them */
    if (numofword == 1)		/* 16 bits counter 	*/
    {
      co16_data = re_data[0];

      /* If this is the first lap we dont have any correct saved
	 countvalues. Therefor we save the new value.	*/
      if ( local->FirstScan[i])
        local->OldValue[i] = co16_data;

      /* Store the counter values into RTDB */
      *(pwr_tUInt32 *) cp->chanlist[i].vbp = co16_data;
			       
      /* Test if an abs-value should be calculated and stored */
      if ( op->COAbsFlag[i] == TRUE)
      {
        /* Calculate difference between two readings*/
        diff = co16_data - local->OldValue[i];
        local->OldValue[i] = co16_data;	/* Store new value */

        /* Check if the counter is passing zero or max,
           and calculate if it is counting up or down 	*/

        if (diff < MIN16)		/* Up count over maxlimit */
          newdiff = MAXCO16 + diff;
        else if (diff > MAX16)
          newdiff = diff - MAXCO16;
        else
          newdiff = diff;

        /* Store the counter abs-values into RTDB		*/
	*(pwr_tUInt32 *) cp->chanlist[i].abs_vbp += newdiff;		
      }
    }

    else if (numofword == 2)		/* 24 bits counter 	*/
    {
      co24_data = *re_data_p;

      /* If this is the first lap we dont have any correct saved
         countvalues. Therefor we save the new value.	*/

      if ( local->FirstScan[i])
        local->OldValue[i] = co24_data;

      /* Store the counter values into RTDB 		*/
      *(pwr_tUInt32 *) cp->chanlist[i].vbp = co24_data;
			       
      /* Test if an abs-value should be calaulated and stored */
		    
      if ( op->COAbsFlag[i] == TRUE)
      {
        /* Calculate difference between two readings */
        diff = co24_data - local->OldValue[i];	
        local->OldValue[i] = co24_data; 	/* Store new value */

        /* Check if the counter is passing zero or max,
           and calculate if it is counting up or down */

        if (diff < MIN24)   	    /* Up count over maxlimit */
          newdiff = MAXCO24 + diff;
        else if (diff > MAX24 )
          newdiff = diff - MAXCO24 ;
        else
          newdiff = diff ;

        /* Store the counter abs-values into RTDB */
        *(pwr_tUInt32 *) cp->chanlist[i].abs_vbp += newdiff;
      }
    }
    local->FirstScan[i] = 0;
  }
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Co_PI24BO) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
