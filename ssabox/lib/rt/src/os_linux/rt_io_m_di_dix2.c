/* 
 * Proview   $Id: rt_io_m_di_dix2.c,v 1.6 2007-04-30 12:08:08 claes Exp $
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

/* rt_io_m_di_dix2.c -- io methods for ssab cards.
   OS Linux
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "pwr.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_filter_di.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "qbus_io.h"
#include "rt_io_m_ssab_locals.h"
#include "rt_io_bfbeth.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/


typedef struct {
	unsigned int	Address[2];
	int		Qbus_fp;
	unsigned int	bfb_item;
	struct {
	  pwr_sClass_Di *sop[16];
	  void	*Data[16];
	  pwr_tBoolean Found;
	} Filter[2];
	pwr_tTime       ErrTime;
} io_sLocal;

static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  pwr_sClass_Di_DIX2	*op;
  io_sLocal 		*local;
  int			i, j;
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);

  op = (pwr_sClass_Di_DIX2 *) cp->op;
  local = calloc( 1, sizeof(*local));
  cp->Local = local;

  errh_Info( "Init of di card '%s'", cp->Name);

  local->Address[0] = op->RegAddress;
  local->Address[1] = op->RegAddress + 2;
  local->Qbus_fp = r_local->Qbus_fp;
   
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
static pwr_tStatus IoCardClose (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  io_sLocal 		*local;
  int			i;

  local = (io_sLocal *) cp->Local;

  errh_Info( "IO closing di card '%s'", cp->Name);

  /* Free filter data */
  for ( i = 0; i < 2; i++)
  {
    if ( local->Filter[i].Found)
      io_CloseDiFilter( local->Filter[i].Data);
  }
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
  io_sLocal 		*local = (io_sLocal *) cp->Local;
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);
  pwr_tUInt16		data = 0;
  pwr_sClass_Di_DIX2	*op;
  pwr_sClass_Ssab_RemoteRack	*rrp;
  pwr_tUInt16		invmask;
  pwr_tUInt16		convmask;
  int			i;
  int			sts;
  qbus_io_read 		rb;
  int			bfb_error = 0;
  pwr_tTime             now;

  op = (pwr_sClass_Di_DIX2 *) cp->op;

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

    if (r_local->Qbus_fp != 0 && r_local->s == 0) {
      /* Read from local Q-bus */
      rb.Address = local->Address[i];
      sts = read( local->Qbus_fp, &rb, sizeof(rb));
      data = (unsigned short) rb.Data;
    }
    else {
      /* Ethernet I/O, Get data from current address */
      data = bfbeth_get_data(r_local, (pwr_tUInt16) local->Address[i], &sts);
      /* Yes, we want to read this address the next time aswell */
      bfbeth_set_read_req(r_local, (pwr_tUInt16) local->Address[i]);

      if (sts == -1) {
	/* Error handling for ethernet Qbus-I/O */
  	rrp = (pwr_sClass_Ssab_RemoteRack *) rp->op;
	if (bfb_error == 0) {
          op->ErrorCount++;
	  bfb_error = 1;
          if ( op->ErrorCount == op->ErrorSoftLimit)
            errh_Error( "IO Error soft limit reached on card '%s'", cp->Name);
          if ( op->ErrorCount == op->ErrorHardLimit)
            errh_Error( "IO Error hard limit reached on card '%s', stall action %d", cp->Name, rrp->StallAction);
          if ( op->ErrorCount >= op->ErrorHardLimit && rrp->StallAction == pwr_eSsabStallAction_ResetInputs )
	  {
	    data = 0;
	    sts = 1;
          }
          if ( op->ErrorCount >= op->ErrorHardLimit && rrp->StallAction == pwr_eSsabStallAction_EmergencyBreak )
	  {
            ctx->Node->EmergBreakTrue = 1;
            return IO__ERRDEVICE;
          }
	}
	if (sts == -1) continue;
      }
      else {
	op->ErrorCount = 0;
      }
    }
    
    if ( sts <= 0)
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

pwr_dExport pwr_BindIoMethods(Di_DIX2) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
