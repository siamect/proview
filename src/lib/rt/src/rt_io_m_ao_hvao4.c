/* rt_io_m_ao_hvao4.c -- io methods for ssab cards.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

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
#include "rt_errh.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_write.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

#define IO_MAXCHAN 8

typedef struct {
	char		*Address;
	DEVICE		Device[64];
	pwr_tFloat32	OldValue[IO_MAXCHAN];
	pwr_tBoolean	OldTestOn[IO_MAXCHAN];
	int		WriteFirst;
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
      PolyCoef1 = cop->RawValRangeHigh / cop->ChannelSigValRangeHigh;
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
  pwr_sClass_Ao_HVAO4	*op;
  io_sChannel		*chanp;
  int			i;
  io_sLocal 		*local;

  op = (pwr_sClass_Ao_HVAO4 *) cp->op;
  local = calloc( 1, sizeof(*local));

  errh_Info( "Init of ao card '%s'", cp->Name);

  sts = ssabao_ini( op->DevName, &local->Address, &local->Device,
		ctx->RelativVector);
  if ( EVEN(sts))
  {
    errh_Error( "IO init error of ao card '%s'\n%m", cp->Name, sts);
    return sts;
  }
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

  local = (io_sLocal *) cp->Local;

  errh_Info( "IO closing ao card '%s'", cp->Name);

  sts = ssab_close( local->Device);
  if ( EVEN(sts))
    errh_Error("IO error deleting device %s\n%m", cp->Name, sts);

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
  pwr_sClass_Ao_HVAO4	*op;
  int			i;
  io_sChannel		*chanp;
  pwr_sClass_ChanAo	*cop;
  pwr_sClass_Ao		*sop;
  pwr_tFloat32		value;
  int			fixout;
  pwr_tUInt16		data;
  pwr_tFloat32		rawvalue;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Ao_HVAO4 *) cp->op;
  
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
      ssabao_write( data, local->Address, i);
      if (io_writeerr || io_fatal_error)
      {
        /* Exceptionhandler was called */
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

pwr_dExport pwr_BindIoMethods(Ao_HVAO4) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
