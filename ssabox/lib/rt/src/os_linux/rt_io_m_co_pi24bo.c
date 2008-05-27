/* 
 * Proview   $Id: rt_io_m_co_pi24bo.c,v 1.8 2008-05-27 12:06:57 claes Exp $
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

/* rt_io_m_co_pi24bo.c -- io methods for ssab cards.
   OS Linux
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "pwr.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "qbus_io.h"
#include "rt_io_m_ssab_locals.h"
#include "rt_io_bfbeth.h"

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
	unsigned int	bfb_item;
	pwr_tInt32	OldValue[IO_MAXCHAN];
	int		FirstScan[IO_MAXCHAN];
	pwr_tTime       ErrTime;
} io_sLocal;

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
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);

  op = (pwr_sClass_Co_PI24BO *) cp->op;

  local = calloc( 1, sizeof(*local));

  for ( i = 0; i < IO_MAXCHAN; i++)
    local->FirstScan[i] = 1;
  cp->Local = local;
  local->Address = op->RegAddress;
  local->Qbus_fp = r_local->Qbus_fp;

  errh_Info( "Init of co card '%s'", cp->Name);

  /* Configure card */
  for ( i = 0; i < op->MaxNoOfCounters; i++)
  {
    if ( !cp->chanlist[i].cop)
      continue;

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

    if (r_local->Qbus_fp != 0 && r_local->s == 0) {
      /* Write to local Q-bus */
      wb.Address = local->Address + 4*i;
      wb.Data = wr_data[0];
      sts = write( local->Qbus_fp, &wb, sizeof(wb));
      if ( sts != -1) {
        wb.Address += 2;
        wb.Data = wr_data[1];
        sts = write( local->Qbus_fp, &wb, sizeof(wb));
      }
    }
    else {
      /* Ethernet I/O, Request a write to current address */
      bfbeth_set_write_req(r_local, (pwr_tUInt16) (local->Address + 4*i), wr_data[0]);
      bfbeth_set_write_req(r_local, (pwr_tUInt16) (local->Address + 4*i + 2), wr_data[1]);
      sts = 1;      
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
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);
  pwr_tTime             now;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Co_PI24BO *) cp->op;

  for ( i = 0; i < op->MaxNoOfCounters; i++)
  { 
    if ( !cp->chanlist[i].cop)
      continue;

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
 
        if (r_local->Qbus_fp != 0 && r_local->s == 0) {
          /* Write to local Q-bus */
          wb.Address = local->Address + 4*i;
          wb.Data = wr_data[0];
          sts = write( local->Qbus_fp, &wb, sizeof(wb));
          if ( sts != -1)
          {
            wb.Address += 2;
            wb.Data = wr_data[1];
            sts = write( local->Qbus_fp, &wb, sizeof(wb));
          }
        }
        else {
      	  /* Ethernet I/O, Request a write to current address */
      	  bfbeth_set_write_req(r_local, (pwr_tUInt16) (local->Address + 4*i), wr_data[0]);
      	  bfbeth_set_write_req(r_local, (pwr_tUInt16) (local->Address + 4*i + 2), wr_data[1]);
      	  sts = 1;      
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
      
      if (r_local->Qbus_fp != 0 && r_local->s == 0) {
	/* Read from local Q-bus */
	if ( numofword == 1) {
	  rb.Address = local->Address + 4*i;
	  sts1 = read( local->Qbus_fp, &rb, sizeof(rb));
	  re_data[0] = (unsigned short) rb.Data;
	  sts2 = 0;
	}
	else if ( numofword == 2) {
	  /* Read second word, then first word, and the second word again */
	  int			j;
	  pwr_tUInt16		val;

	  rb.Address = local->Address + 4*i + 2;
	  sts2 = read( local->Qbus_fp, &rb, sizeof(rb));
	  val = (unsigned short) rb.Data;

	  for ( j = 0; j < 3; j++) {
	    rb.Address -= 2;
	    sts1 = read( local->Qbus_fp, &rb, sizeof(rb));
	    re_data[0] = (unsigned short) rb.Data;

	    rb.Address += 2;
	    sts2 = read( local->Qbus_fp, &rb, sizeof(rb));
	    re_data[1] = (unsigned short) rb.Data;

	    if ( val == re_data[1])
	      break;

	    /* First and second didn't match, try again */
	    val = re_data[1];
	  }
	  if ( val != re_data[1])
	    continue;
	}	
      }
      else {
        /* Ethernet I/O, Get data from current address */

        re_data[0] = bfbeth_get_data(r_local, (pwr_tUInt16) (local->Address + 4*i), &sts1);
        /* Yes, we want to read this address the next time aswell */
        bfbeth_set_read_req(r_local, (pwr_tUInt16) (local->Address + 4*i));	 

	if ( numofword == 2) {
          re_data[1] = bfbeth_get_data(r_local, (pwr_tUInt16) (local->Address + 4*i + 2), &sts2);
          /* Yes, we want to read this address the next time aswell */
          bfbeth_set_read_req(r_local, (pwr_tUInt16) (local->Address + 4*i + 2));	 
	}	
      }
      
      if ( sts1 == -1 || sts2 == -1)
      {
        /* Increase error count and check error limits */
        clock_gettime(CLOCK_REALTIME, &now);

        if (op->ErrorCount > op->ErrorSoftLimit) {
          /* Ignore if some time has expired */
          if (now.tv_sec - local->ErrTime.tv_sec < 600)
            op->ErrorCount++;
        }
        else
          op->ErrorCount++;
        local->ErrTime = now;

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

pwr_dExport pwr_BindIoMethods(Co_CO4uP) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};

