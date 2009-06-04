/* 
 * Proview   $Id: rt_io_m_do_hvdo32.c,v 1.7 2008-10-03 14:29:32 claes Exp $
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

/* rt_io_m_do_hvdo32.c -- io methods for ssab cards.
   OS Linux
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "pwr.h"
#include "co_time.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_filter_po.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_write.h"
#include "qbus_io.h"
#include "rt_io_m_ssab_locals.h"
#include "rt_io_bfbeth.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

typedef struct {
	unsigned int	Address[2];
	int		Qbus_fp;
	struct {
	  pwr_sClass_Po *sop[16];
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
  pwr_sClass_Do_HVDO32	*op;
  io_sLocal 		*local;
  int			i, j;
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);

  op = (pwr_sClass_Do_HVDO32 *) cp->op;
  local = calloc( 1, sizeof(*local));
  cp->Local = local;

  errh_Info( "Init of do card '%s'", cp->Name);

  local->Address[0] = op->RegAddress;
  local->Address[1] = op->RegAddress + 2;
  local->Qbus_fp = r_local->Qbus_fp;

  /* Init filter for Po signals */
  for ( i = 0; i < 2; i++)
  {
    /* The filter handles one 16-bit word */
    for ( j = 0; j < 16; j++)
    {
      if ( cp->chanlist[i*16+j].SigClass == pwr_cClass_Po)
        local->Filter[i].sop[j] = cp->chanlist[i*16+j].sop;
    }
    io_InitPoFilter( local->Filter[i].sop, &local->Filter[i].Found,
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

  errh_Info( "IO closing do card '%s'", cp->Name);

  /* Free filter data */
  for ( i = 0; i < 2; i++)
  {
    if ( local->Filter[i].Found)
      io_ClosePoFilter( local->Filter[i].Data);
  }
  free( (char *) local);

  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  io_sLocal 		*local = (io_sLocal *) cp->Local;
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);
  pwr_tUInt16		data = 0;
  pwr_sClass_Do_HVDO32	*op;
  pwr_tUInt16		invmask;
  pwr_tUInt16		testmask;
  pwr_tUInt16		testvalue;
  int			i;
  qbus_io_write		wb;
  int			sts;
  pwr_tTime             now;

  op = (pwr_sClass_Do_HVDO32 *) cp->op;

#if defined(OS_ELN)
  vaxc$establish(machfailwrite);
#endif

  for ( i = 0; i < 2; i++)
  { 
    if ( i == 1 && op->MaxNoOfChannels <= 16)
      break;

    if ( ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT)
    {
      if ( i == 0)
        data = op->FixedOutValue1;
      else
        data = op->FixedOutValue2;
    }
    else
      io_DoPackWord( cp, &data, i);

    if ( i == 0)
    {
      testmask = op->TestMask1;
      invmask = op->InvMask1;
    }
    else
    {
      testmask = op->TestMask2;
      invmask = op->InvMask2;
      if ( op->MaxNoOfChannels == 16)
        break;
    }

    /* Invert */
    data = data ^ invmask;

    /* Filter Po signals */
    if ( local->Filter[i].Found)
      io_PoFilter( local->Filter[i].sop, &data, local->Filter[i].Data);

    /* Testvalues */
    if ( testmask)
    {
      if ( i == 0)
        testvalue = op->TestValue1;
      else
        testvalue = op->TestValue2;
      data = (data & ~ testmask) | (testmask & testvalue);
    }

    if (r_local->Qbus_fp != 0 && r_local->s == 0) {
      /* Write to local Q-bus */
      wb.Data = data;
      wb.Address = local->Address[i];
      sts = write( local->Qbus_fp, &wb, sizeof(wb));
    }
    else {
      /* Ethernet I/O, Request a write to current address */
      bfbeth_set_write_req(r_local, (pwr_tUInt16) local->Address[i], data);
      sts = 1;      
    }

    if ( sts <= 0)
    {
#if 0
      /* Exceptionhandler was called */
      if ( io_fatal_error)
      {
        /* Activate emergency break */
        errh_Error( "Fatal write error, card '%s', IO is stopped", cp->Name);
        ctx->Node->EmergBreakTrue = 1;
        return IO__ERRDEVICE;
      }
#endif
      /* Increase error count and check error limits */
      time_GetTime(&now);

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
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Do_HVDO32) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
