/* 
 * Proview   $Id: rt_io_m_ssab_aoup.c,v 1.5 2007-04-30 12:08:08 claes Exp $
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

/* rt_io_m_ssab_aoup.c -- io methods for ssab cards. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#include "pwr.h"
#include "rt_errh.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_write.h"
#include "qbus_io.h"
#include "rt_io_m_ssab_locals.h"
#include "rt_io_bfbeth.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

#define IO_MAXCHAN 8

typedef struct {
	unsigned int	Address;
	int		Qbus_fp;
	unsigned int	bfb_item;
	pwr_tFloat32	OldValue[IO_MAXCHAN];
	pwr_tBoolean	OldTestOn[IO_MAXCHAN];
	int		WriteFirst;
	pwr_tTime       ErrTime;
} io_sLocal;

static pwr_tStatus AoRangeToCoef( 
  io_sChannel 	*chanp)
{
  pwr_sClass_ChanAo	*cop;
  char			buf[120];
  pwr_tStatus		sts;
  pwr_tFloat32		PolyCoef1;
  pwr_tFloat32		PolyCoef0;

  cop = chanp->cop;

  if ( cop)
  {
    cop->CalculateNewCoef = 0;

    /* Coef for ActualValue to RawValue conversion */
    if ( cop->ActValRangeHigh != cop->ActValRangeLow)
    {
      cop->SigValPolyCoef1 = (cop->SensorSigValRangeHigh - cop->SensorSigValRangeLow)/
		(cop->ActValRangeHigh - cop->ActValRangeLow);
      cop->SigValPolyCoef0 = cop->SensorSigValRangeHigh - cop->ActValRangeHigh *
		cop->SigValPolyCoef1;
    }
    else
    {
      sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      errh_Error( "Invalid ActValueRange in Ao channel %s", buf);
      return IO__CHANRANGE;
    }
    /* Coef for ActualValue to SignalValue conversion */
    if ( cop->ChannelSigValRangeHigh != 0)
    {
      PolyCoef0 = 0;
      PolyCoef1 = cop->RawValRangeHigh / cop->ChannelSigValRangeHigh;
      cop->OutPolyCoef1 = cop->SigValPolyCoef1 * PolyCoef1;
      cop->OutPolyCoef0 = PolyCoef0 + PolyCoef1*
		cop->SigValPolyCoef0;
    }
    else
    {
      sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      errh_Error( "Invalid SigValueRange in Ao channel %s", buf);
      return IO__CHANRANGE;
    }
  }
  return IO__SUCCESS;
}

static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  pwr_sClass_Ssab_BaseACard	*op;
  io_sChannel		*chanp;
  int			i;
  io_sLocal 		*local;
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);

  op = (pwr_sClass_Ssab_BaseACard *) cp->op;
  local = calloc( 1, sizeof(*local));
  local->Address = op->RegAddress;
  local->Qbus_fp = r_local->Qbus_fp;

  errh_Info( "Init of ao card '%s'", cp->Name);
  
  /* Write the first 50 loops */
  local->WriteFirst = 50;
  cp->Local = local;

  /* Caluclate polycoeff */
  chanp = cp->chanlist;
  for  ( i = 0; i < cp->ChanListSize; i++)
  {
    if ( chanp->sop)
      AoRangeToCoef( chanp);
    chanp++;
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

  errh_Info( "IO closing ao card '%s'", cp->Name);

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
  io_sLocal 		*local;
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);
  pwr_sClass_Ssab_BaseACard	*op;
  int			i;
  io_sChannel		*chanp;
  pwr_sClass_ChanAo	*cop;
  pwr_sClass_Ao		*sop;
  pwr_tFloat32		value;
  int			fixout;
  pwr_tUInt16		data;
  pwr_tFloat32		rawvalue;
  qbus_io_write		wb;
  int			sts;
  pwr_tTime             now;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Ssab_BaseACard *) cp->op;
  
  fixout = ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT;

  chanp = &cp->chanlist[0];
  for ( i = 0; i < cp->ChanListSize; i++)
  { 
    if ( !chanp->cop || !chanp->sop)
    {
      chanp++;
      continue;
    }
    cop = (pwr_sClass_ChanAo *) chanp->cop;
    sop = (pwr_sClass_Ao *) chanp->sop;

    if ( *(pwr_tFloat32 *)chanp->vbp != local->OldValue[i] ||
         local->WriteFirst > 0 ||
	 cop->CalculateNewCoef ||
         fixout ||
         cop->TestOn || local->OldTestOn[i] != cop->TestOn)
    {
      if ( fixout)
        value = cop->FixedOutValue;
      else if ( cop->TestOn)
        value = cop->TestValue;
      else
	value = *(pwr_tFloat32 *) chanp->vbp;

      if ( cop->CalculateNewCoef)
        AoRangeToCoef( chanp);


      /* Convert to rawvalue */
      if ( value > cop->ActValRangeHigh)
        value = cop->ActValRangeHigh;
      else if ( value < cop->ActValRangeLow)
        value = cop->ActValRangeLow;

      rawvalue = cop->OutPolyCoef1 * value + cop->OutPolyCoef0;
      if ( rawvalue > 0)
        sop->RawValue = rawvalue + 0.5;
      else
        sop->RawValue = rawvalue - 0.5;
      data = sop->RawValue;

#if defined(OS_ELN)
      vaxc$establish(machfailwrite);
#endif
      if (r_local->Qbus_fp != 0 && r_local->s == 0) {
        wb.Data = data;
        wb.Address = local->Address + 2*i;
        sts = write( local->Qbus_fp, &wb, sizeof(wb));
      }
      else {
      	/* Ethernet I/O, Request a write to current address */
      	bfbeth_set_write_req(r_local, (pwr_tUInt16) (local->Address + 2*i), data);
      	sts = 1;      
      }
      
      if ( sts == -1)
      {
        /* Exceptionhandler was called */
#if 0
        if ( io_fatal_error)
        {
          /* Activate emergency break */
          errh_Error( "Fatal write error, card '%s', IO is stopped", cp->Name);
          ctx->Node->EmergBreakTrue = 1;
          return IO__ERRDEVICE;
        }
#endif
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
	chanp++;
        continue;
      }
      else
        local->OldValue[i] = value;
    }
    local->OldTestOn[i] = cop->TestOn;
    chanp++;
  }
  if ( local->WriteFirst)
    local->WriteFirst--;
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Ssab_AouP) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};

