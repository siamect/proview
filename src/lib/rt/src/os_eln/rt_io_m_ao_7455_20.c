/* 
 * Proview   $Id: rt_io_m_ao_7455_20.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* io_m_ssab_ao.c -- io methods for ssab cards. */

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
#include "co_cdh.h"
#include "rt_gdh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"

#if defined(OS_ELN)
#include "rt_io_rtp.h"
#endif

#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_write.h"

/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

#define RTP_AO_7455_20	30
#define RTP_AO_7455_30	32
#define RTP_PD_7435_26	34
#define IO_MAXCHAN 	4

typedef struct {
	pwr_tInt16	RackAddress;
	pwr_tInt32	Address;
	pwr_tInt16	CardType;
	pwr_tFloat32	OldValue[IO_MAXCHAN];
	int		WriteFirst;
} io_sLocal;


static pwr_tStatus AoRangeToCoef( 
  io_sChannel 	*chanp)
{
  pwr_sClass_ChanAo	*cop;
  char			buf[120];
  pwr_tStatus		sts;
  int			i;
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
      sts = gdh_ObjidToName( chanp->ChanObjid, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      errh_Error( "Invalid ActValueRange in Ao channel %s", buf);
      return IO__CHANRANGE;
    }
    /* Coef for ActualValue to SignalValue conversion */
    if ( cop->ChannelSigValRangeHigh != 0)
    {
      PolyCoef0 = 0;
      PolyCoef1 = 2047. / cop->ChannelSigValRangeHigh;
      cop->OutPolyCoef1 = cop->SigValPolyCoef1 * PolyCoef1;
      cop->OutPolyCoef0 = PolyCoef0 + PolyCoef1*
		cop->SigValPolyCoef0;
    }
    else
    {
      sts = gdh_ObjidToName( chanp->ChanObjid, buf, sizeof(buf), 
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
  pwr_tStatus		sts;
  pwr_sClass_Ao_7455_20	*op;
  io_sLocal 		*local;
  io_sChannel 		*chanp;
  int			i;

  op = (pwr_sClass_Ao_7455_20 *) cp->op;
  local = calloc( 1, sizeof(*local));

  local->Address = (op->CardAddress | 0xf0);   
  local->RackAddress = ((pwr_sClass_Rack_RTP *) rp->op)->RackAddress;

  if ( cp->Class == pwr_cClass_Ao_7455_20)
    local->CardType = RTP_AO_7455_20;
  else if ( cp->Class == pwr_cClass_Ao_7455_30)
    local->CardType = RTP_AO_7455_30;
  else if ( cp->Class == pwr_cClass_Pd_7435_26)
    local->CardType = RTP_PD_7435_26;

  errh_Info( "Init of ao card '%s'", cp->Name);

  /* Write the first 50 loops */
  local->WriteFirst = 50;
  cp->Local = local;

  /* Caluclate polycoeff */
  chanp = cp->chanlist;
  for  ( i = 0; i < cp->ChanListSize; i++)
  {
    AoRangeToCoef( chanp);
    chanp++;
  }

  return IO__SUCCESS;
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
  pwr_sClass_Ao_7455_20	*op;
  int			i;
  io_sChannel		*chanp;
  pwr_sClass_ChanAo	*cop;
  pwr_sClass_Ao		*sop;
  pwr_tFloat32		value;
  pwr_tFloat32		sigvalue;
  int			fixout;
  pwr_tUInt16		data;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Ao_7455_20 *) cp->op;

  fixout = ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT;

  chanp = &cp->chanlist[0];
  for ( i = 0; i < cp->ChanListSize; i++)
  { 
    if ( !chanp->cop)
    {
      chanp++;
      continue;
    }
    cop = (pwr_sClass_ChanAo *) chanp->cop;
    sop = (pwr_sClass_Ao *) chanp->sop;

    if ( *(pwr_tFloat32 *)chanp->vbp != local->OldValue[i] ||
         local->WriteFirst > 0 ||
         fixout)
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

      sop->RawValue = cop->OutPolyCoef1 * value + cop->OutPolyCoef0 + 0.5;
      data = sop->RawValue;

      rtpao_write( data, local->RackAddress, (local->Address & 0xf), i,
		local->CardType);
      if (io_writeerr || io_fatal_error)
      {
        if ( io_fatal_error)
        {
          /* Activate emergency break */
          errh_Error( "Fatal write error, card '%s', IO is stopped", cp->Name);
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
	chanp++;
        continue;
      }
      else
        local->OldValue[i] = value;
    }
    chanp++;
  }
  if ( local->WriteFirst)
    local->WriteFirst--;
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Ao_7455_20) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
pwr_dExport pwr_BindIoMethods(Ao_7455_30) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
pwr_dExport pwr_BindIoMethods(Pd_7435_26) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
