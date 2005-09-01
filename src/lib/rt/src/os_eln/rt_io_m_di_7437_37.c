/* 
 * Proview   $Id: rt_io_m_di_7437_37.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* io_m_rtp_di.c -- io methods for rtp cards. */

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
#include "rt_io_filter_di.h"
#if defined(OS_ELN)
#include "rt_io_rtp.h"
#endif
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

#define RTP_DI_7437_37	1

typedef struct {
	pwr_tInt16	RackAddress;
	pwr_tInt32	Address;
	pwr_tInt16	CardType;
	struct {
	  pwr_sClass_Di *sop[16];
	  void	*Data[16];
	  pwr_tBoolean Found;
	} Filter[2];
} io_sLocal;


/*** VMS Test ****/
#if defined(OS_VMS)
extern pwr_tBoolean		io_readerr;
extern pwr_tBoolean		io_fatal_error;

static unsigned short rtpdi_read(	
		pwr_tInt16	rack_address,
		pwr_tInt32	address,
		int		grupp,
		pwr_tInt16	card_type)
{
  static unsigned short di_data;

  di_data = 21845;
  return di_data;
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
  pwr_sClass_Di_7437_37	*op;
  io_sLocal 		*local;
  int			i, j;

  op = (pwr_sClass_Di_7437_37 *) cp->op;
  local = calloc( 1, sizeof(*local));
  cp->Local = local;
  local->Address = (op->CardAddress |  0xf0);   
  local->RackAddress = ((pwr_sClass_Rack_RTP *) rp->op)->RackAddress;

  /* Init filter */
  for ( i = 0; i < 2; i++)
  {
    /* The filter handles one 16-bit word */
    for ( j = 0; j < 16; j++)
      local->Filter[i].sop[j] = cp->chanlist[i*16+j].sop;
    io_InitDiFilter( local->Filter[i].sop, &local->Filter[i].Found,
		local->Filter[i].Data, ctx->ScanTime);
  }

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
  pwr_tUInt16		data = 0;
  pwr_sClass_Di_7437_37	*op;
  pwr_tUInt16		invmask;
  pwr_tUInt16		convmask;
  int			i;

  if ( cp->AgentControlled)
    return IO__SUCCESS;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Di_7437_37 *) cp->op;

  for ( i = 0; i < 2; i++)
  { 
    if ( i == 0)
    {
      convmask = op->ConvMask1;
      invmask = op->InvMask1;
    }
    else
    {
      convmask = op->ConvMask2;
      invmask = op->InvMask2;
      if ( !convmask)
        break;
      if ( op->MaxNoOfChannels == 16)
        break;
    }

    data = rtpdi_read( local->RackAddress, (local->Address & 0xf), i + 1,
			local->CardType);
    if (io_readerr || io_fatal_error)
    {
      if ( io_fatal_error)
      {
        /* Activate emergency break */
        errh_Error( "Fatal read error, card '%s', IO i stopped", cp->Name);
        ctx->Node->EmergBreakTrue = 1;
        return IO__ERRDEVICE;
      }

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

    /* Invert */
    data = data ^ invmask;

    /* Filter ... */
    if ( local->Filter[i].Found)
      io_DiFilter( local->Filter[i].sop, &data, local->Filter[i].Data);

    /* Move data to valuebase */
    io_DiUnpackWord( cp, data, convmask, i);
  }
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Di_7437_37) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
