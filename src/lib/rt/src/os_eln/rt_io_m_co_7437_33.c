/* 
 * Proview   $Id: rt_io_m_co_7437_33.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* io_m_ssab_pi.c -- io methods for ssab cards. */

#if defined(OS_ELN)
# include string
# include stdlib
# include stdio
# include descrip
#else
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <descrip.h>
#endif


#include "pwr.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#if defined(OS_ELN)
#include "rt_io_rtp.h"
#endif
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"

#define IO_MAXCHAN 	4
#define RTP_CO_7435_33	40
#define RTP_CO_7437_33	41

/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

/*** VMS Test ****/
#if defined(OS_VMS)
typedef int DEVICE;
#endif
/*** Slut VMS Test ****/

typedef struct {
	pwr_tInt16	RackAddress;
	pwr_tInt32	Address;
	pwr_tInt16	CardType;
	pwr_tInt32	OldValue[IO_MAXCHAN];
	int		FirstScan[IO_MAXCHAN];
} io_sLocal;


/*** VMS Test ****/
#if defined(OS_VMS)

static void rtpco_read(	
		pwr_tUInt16	*data,
		pwr_tInt16	rack_address,
		pwr_tInt32	card_address,
		int		nr,
		pwr_tInt16	card_type)
{
  *data = 21845;
}
static void rtpco_write(
		int		chan,
		pwr_tInt16	rack_address,
		pwr_tInt32	card_address,
		pwr_tInt16	card_type)
{
}
#endif
/*** Slut VMS Test ****/

static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  pwr_tStatus		sts;
  io_sLocal 		*local;
  int			i, j;
  pwr_sClass_Co_7435_33	*op;			/* Rtp 12/16 bit single counter */
  pwr_sClass_Co_7437_33	*op_7437;		/* RTP 4 x16 bit counter crad */

  op = (pwr_sClass_Co_7435_33 *) cp->op;

  local = calloc( 1, sizeof(*local));
  local->RackAddress = ((pwr_sClass_Rack_RTP *) rp->op)->RackAddress;
  local->Address = 1;

  if ( cp->Class == pwr_cClass_Co_7435_33)
    local->CardType =  RTP_CO_7435_33;
  else if ( cp->Class == pwr_cClass_Co_7437_33)
    local->CardType =  RTP_CO_7437_33;

  for ( i = 0; i < IO_MAXCHAN; i++)
    local->FirstScan[i] = 1;
  cp->Local = local;



  if ( op->MaxNoOfCounters == 1) /* If one RTP_CO 7435_33 */
  {	
    rtpco_write( 0, local->RackAddress, op->CardAddress, 
		local->CardType);
  }
  else
  {	
    op_7437 = (pwr_sClass_Co_7437_33 *) op;
    for (i = 0; i < op_7437->MaxNoOfCounters; i++)
      rtpco_write( (i << 8) | 0x440, local->RackAddress, op_7437->CardAddress,
		local->CardType);
  }		
			
  for ( i = 0; i < op->MaxNoOfCounters; i++)
    * (pwr_tUInt32 *) cp->chanlist[i].vbp = 0;

  errh_Info( "Init of co card '%s'", cp->Name);

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
  pwr_sClass_Co_7435_33	*op;			/* Rtp 12/16 bit single counter */
  pwr_sClass_Co_7437_33	*op_7437;		/* RTP 4 x16 bit counter crad */
  pwr_tUInt16		invmask;
  pwr_tUInt16		convmask;
  int			i;
  pwr_tInt32		diff;
  pwr_tUInt16		uco16_data;

  if ( cp->AgentControlled)
    return IO__SUCCESS;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Co_7435_33 *) cp->op;

  /* RTP produces two diffrent counter cards 
     7435_33 that is a 12 or 16 bit single channel card 
     7437_33 is a 4 channel 16 bit wide card. 
     The only way to diffrentiate between these cards is to look at
     MaxNoOfChannels 
     The RTP cards can only count upwards */

  /* Check if conversion should be done for this channel, if not
     just don't do anything */


  for ( i = 0; i < op->MaxNoOfCounters; i++)
  { 
    if ( op->ConvMask & (1 << i))
    {
      /* Conversion is on */

      if ( op->MaxNoOfCounters == 1) /* Check card type */
      {
	rtpco_read( &uco16_data, local->RackAddress, op->CardAddress, 0,
		local->CardType);
      }
      else
      {
	op_7437 = (pwr_sClass_Co_7437_33 *) op;
	/* Select channel */
	rtpco_write( i << 8, local->RackAddress, op_7437->CardAddress,
			local->CardType);
	rtpco_read( &uco16_data, local->RackAddress, op_7437->CardAddress,
			0, local->CardType);
      }
      /* Save the counter value */
      *(pwr_tUInt32 *) cp->chanlist[i].vbp = uco16_data;
      if ( local->CardType == RTP_CO_7435_33 &&
	   op->NoOfBits == 12)	     /* Check for type and range*/
      {
	if ( uco16_data >= local->OldValue[i])	/* Ok we haven't counted through zero */
	  diff = uco16_data - local->OldValue[i];
        else
	  diff = uco16_data + (4095 - local->OldValue[i]);
      }
      else
      {
	if ( uco16_data >= local->OldValue[i])	/* Ok we haven't counted through zero */
	  diff = uco16_data - local->OldValue[i];
	else
	  diff = uco16_data + (0xffff - local->OldValue[i]);
      }
      *(pwr_tUInt32 *) cp->chanlist[i].abs_vbp += diff;
	local->OldValue[i] = uco16_data;
    }
    local->FirstScan[i] = 0;
  }
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Co_7435_33) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
pwr_dExport pwr_BindIoMethods(Co_7437_33) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
