/* rt_io_m_ai_ai32up.c -- io methods for ssab cards.

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
# include <errno.h>
# include <sys/file.h>
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
#include "qbus_io.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

#define IO_MAXCHAN 32

typedef struct {
	unsigned int	Address;
	int		Qbus_fp;
	int	ScanCount[IO_MAXCHAN];
} io_sLocal;

typedef struct {
	int		Qbus_fp;
} io_sRackLocal;


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
  pwr_sClass_Ai_AI32uP	*op;
  io_sLocal 		*local;
  int			i;
  io_sChannel		*chanp;

  op = (pwr_sClass_Ai_AI32uP *) cp->op;
  local = calloc( 1, sizeof(*local));
  cp->Local = local;
  local->Address = op->RegAddress;
  local->Qbus_fp = ((io_sRackLocal *)(rp->Local))->Qbus_fp;

  errh_Info( "Init of ai card '%s'", cp->Name);

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
  io_sLocal 		*local;

  errh_Info( "IO closing ai card '%s'", cp->Name);

  local = (io_sLocal *) cp->Local;
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
  int			sts;
  qbus_io_read 		rb;

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
        rb.Address = local->Address + 2*i;
        sts = read( local->Qbus_fp, &rb, sizeof(rb));
        data = (unsigned short) rb.Data;
        if ( sts == -1)
        {
#if 0
          /* Exceptionhandler was called */
          if ( io_fatal_error)
          {
            /* Activate emergency break */
            errh_Error( "Fatal read error, card '%s', IO is stopped", cp->Name);
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
