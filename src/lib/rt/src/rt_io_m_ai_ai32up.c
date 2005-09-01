/* 
 * Proview   $Id: rt_io_m_ai_ai32up.c,v 1.2 2005-09-01 14:57:55 claes Exp $
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

/* rt_io_m_ai_ai32up.c -- io methods for ssab cards. */

#if defined(OS_ELN)
# include string
# include stdlib
# include stdio
#else
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
#endif


#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

#define IO_MAXCHAN 32

typedef struct {
	char	*Address;
	DEVICE	Device[64];
	int	ScanCount[IO_MAXCHAN];
} io_sLocal;


static pwr_tStatus AiRangeToCoef( 
  io_sChannel 		*chanp)
{
  pwr_sClass_ChanAi	*cop;
  char			buf[120];
  pwr_tStatus		sts;
  pwr_tFloat32		PolyCoef1;
  pwr_tFloat32		PolyCoef0;

  cop = chanp->cop;

  if ( cop)
  {
    cop->CalculateNewCoef = 0;

    /* Coef for RawValue to SignalValue conversion */
    cop->SigValPolyCoef0 = 0;
    cop->SigValPolyCoef1 = cop->ChannelSigValRangeHigh / 30000;

    /* Coef for SignalValue to ActualValue conversion */
    if ( chanp->ChanClass != pwr_cClass_ChanAit && cop->SensorPolyType == 1)
    {
      if ( cop->SensorSigValRangeHigh != cop->SensorSigValRangeLow)
      {
        PolyCoef1 = (cop->ActValRangeHigh - cop->ActValRangeLow)/
		(cop->SensorSigValRangeHigh - cop->SensorSigValRangeLow);
        PolyCoef0 = cop->ActValRangeHigh - cop->SensorSigValRangeHigh *
		PolyCoef1;
        cop->SensorPolyCoef1 = cop->SigValPolyCoef1 * PolyCoef1;
        cop->SensorPolyCoef0 = PolyCoef0 + PolyCoef1*
		cop->SigValPolyCoef0;
      }
      else
      {
        sts = gdh_ObjidToName( chanp->ChanObjid, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
        if ( EVEN(sts)) return sts;
        errh_Error( "Invalid SigValueRange in Ai channel %s", buf);
        return IO__CHANRANGE;
      }
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
  pwr_sClass_Ai_AI32uP	*op;
  io_sLocal 		*local;
  int			i;
  io_sChannel		*chanp;

  op = (pwr_sClass_Ai_AI32uP *) cp->op;
  local = calloc( 1, sizeof(*local));
  cp->Local = local;

  errh_Info( "Init of ai card '%s'", cp->Name);

  sts = ssabai_ini( op->DevName, &local->Address, &local->Device,
		ctx->RelativVector);
  if ( EVEN(sts))
  {
    errh_Error( "IO init error of ai card '%s'\n%m", cp->Name, sts);
    return sts;
  }

  /* Caluclate polycoeff */
  chanp = cp->chanlist;
  for  ( i = 0; i < cp->ChanListSize; i++)
  {
    AiRangeToCoef( chanp);
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
  pwr_tStatus		sts;
  io_sLocal 		*local;

  errh_Info( "IO closing ai card '%s'", cp->Name);

  local = (io_sLocal *) cp->Local;
  sts = ssab_close( local->Device);
  if ( EVEN(sts))
    errh_Error("IO error deleting device %s\n%m", cp->Name, sts);

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
  pwr_tInt16		data = 0;
  pwr_sClass_Ai_AI32uP	*op;
  int			i;
  pwr_tFloat32		actvalue;
  io_sChannel		*chanp;
  pwr_sClass_ChanAi	*cop;
  pwr_sClass_Ai		*sop;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Ai_AI32uP *) cp->op;

  chanp = &cp->chanlist[0];
  for ( i = 0; i < cp->ChanListSize; i++)
  { 
    if ( !chanp->cop)
    {
      chanp++;
      continue;
    }
    cop = (pwr_sClass_ChanAi *) chanp->cop;
    sop = (pwr_sClass_Ai *) chanp->sop;

    if ( cop->CalculateNewCoef)
      AiRangeToCoef( chanp);

    if ( cop->ConversionOn)
    {
      if ( local->ScanCount[i] <= 1)
      {

#if defined(OS_ELN)
        vaxc$establish(machfailread);
#endif
        data = ssabaiup_read( local->Address, i);
        if (io_readerr || io_fatal_error)
        {
          /* Exceptionhandler was called */
          if ( io_fatal_error)
          {
            /* Activate emergency break */
            errh_Error( "Fatal read error, card '%s', IO is stopped", cp->Name);
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
	/* Convert rawvalue to sigvalue and actualvalue */
	sop->RawValue = data;
        sop->SigValue = data * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
	switch ( chanp->ChanClass)
        {
          case pwr_cClass_ChanAi:
	    io_ConvertAi( cop, data, &actvalue);
	    break;
          case pwr_cClass_ChanAit:
	    io_ConvertAit( (pwr_sClass_ChanAit *) cop, data, &actvalue);
	    break;
	}

        /* Filter */
	if ( sop->FilterType == 1 &&
	     sop->FilterAttribute[0] > 0 &&
	     sop->FilterAttribute[0] > ctx->ScanTime)
        {
	    actvalue = *(pwr_tFloat32 *)chanp->vbp +
		ctx->ScanTime / sop->FilterAttribute[0] *
		(actvalue - *(pwr_tFloat32 *)chanp->vbp);
        }

	*(pwr_tFloat32 *) chanp->vbp = actvalue;
        local->ScanCount[i] = cop->ScanInterval + 1;
      }
      local->ScanCount[i]--;
    }
    chanp++;
  }
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Ai_AI32uP) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
