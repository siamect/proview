/* io_m_ssab_ai.c -- io methods for ssab cards.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

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
#include "co_cdh.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_filter_ai.h"
#include "rt_io_msg.h"
#if defined(OS_ELN)
#include "rt_io_rtp.h"
#endif

#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

#define RTP_DIFFR	0x00000100          /* Differential input */
#define RTP_AI_7436	20
#define IO_MAXCHAN 	32


typedef struct {
	pwr_tUInt16	CardType[IO_MAXCHAN];
	pwr_tUInt16	RackAddress;
	pwr_tInt32	Address;
	int		ScanCount[IO_MAXCHAN];
	void	 	*FilterData[IO_MAXCHAN];
} io_sLocal;



/* Look up and return the appropriate gain bits for a given range.  */

static pwr_tUInt16
io_get_rtp_ai_gain (
  pwr_tFloat32	Range
)
{
  typedef struct {
    pwr_tFloat32  UpperLim,LowerLim;
    pwr_tUInt16   GainMask;
  } GAINSTR;

  static GAINSTR GainTable[] = {
    {10.5,   9.5,    0},	/* 10V */
    { 5.5,   4.5,    1},	/* 5V  */
    { 2.8,   1.8,    2},	/* 2.5V*/
    { 1.5,   0.95,   3},	/* 1.25V*/
    { 0.8,   0.5,    4},	/* 0.64V*/
    { 0.4,   0.25,   5},	/* 0.32V*/
    { 0.2,   0.15,   6},	/* 0.16V*/
    { 0.1,   0.07,   7},	/* 0.08V (gain 7 and 8 the same !!)*/
    { 0.05,  0.032,  9},	/* 0.04V */
    { 0.03,  0.016, 10},	/* 0.02V */
    { 0.014, 0.005, 11}		/* 0.01V */
  };

  static int GainTblSize = sizeof(GainTable) / sizeof(GainTable[0]);
  int	 Index;
  pwr_tUInt16 RetVal;

  RetVal = 0;	/* Return zero if anything goes wrong */

  if ((Range < 11.0) && (Range >= 0.0))
    for (Index = 0; Index < GainTblSize; Index++)
      if ((Range >= GainTable[Index].LowerLim) && 
	  (Range <= GainTable[Index].UpperLim)) 
      {
	RetVal = GainTable[Index].GainMask;
	break;
      }

  return RetVal; 
}


static pwr_tStatus AiRangeToCoef( 
  io_sChannel 	*chanp)
{
  pwr_sClass_ChanAi	*cop;
  char			buf[120];
  pwr_tStatus		sts;
  int			i;
  pwr_tFloat32		PolyCoef1;
  pwr_tFloat32		PolyCoef0;

  cop = chanp->cop;

  if ( cop)
  {
    cop->CalculateNewCoef = 0;

    /* Coef for RawValue to SignalValue conversion */
    cop->SigValPolyCoef0 = cop->ChannelSigValRangeHigh;
    cop->SigValPolyCoef1 = cop->ChannelSigValRangeHigh / 2048;

    /* Coef for RawValue to ActualValue conversion */
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
  pwr_sClass_Ai_7436	*op;
  io_sLocal 		*local;
  int			i;
  io_sChannel		*chanp;

  op = (pwr_sClass_Ai_7436 *) cp->op;
  local = calloc( 1, sizeof(*local));
  cp->Local = local;

  local->RackAddress = ((pwr_sClass_Rack_RTP *) rp->op)->RackAddress;

  local->Address = (op->CardAddress | 0xf0); /* Keep from being 0 */   

  errh_Info( "Init of ai card '%s'", cp->Name);

  /* Caluclate polycoeff */
  chanp = cp->chanlist;
  for  ( i = 0; i < cp->ChanListSize; i++)
  {
    local->CardType[i] = RTP_AI_7436;
    if ( op->DiffInput)
      local->CardType[i] |= RTP_DIFFR;
    local->CardType[i] |= 
	(io_get_rtp_ai_gain( ((pwr_sClass_ChanAi *)chanp->cop)->ChannelSigValRangeHigh) << 12);

    AiRangeToCoef( chanp);
    chanp++;
  }

  for ( i = 0; i < cp->ChanListSize; i++)
  {
    sts = io_InitAiFilter( cp->chanlist[i].cop, cp->chanlist[i].sop,
	&local->FilterData[i], ctx->ScanTime);
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
  pwr_tStatus		sts;
  io_sLocal 		*local;
  pwr_tUInt16		data = 0;
  pwr_sClass_Ai_7436	*op;
  int			i;
  pwr_tFloat32		actvalue;
  pwr_tFloat32		*polycoef_p;
  io_sChannel		*chanp;
  pwr_sClass_ChanAi	*cop;
  pwr_sClass_Ai		*sop;

  if ( cp->AgentControlled)
    return IO__SUCCESS;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Ai_7436 *) cp->op;

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

	data = rtpai_read( local->RackAddress,
		      (local->Address & 0xf), i, /* mask out fix bits */
		      local->CardType[i]);
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
	switch ( chanp->ChanClass)
        {
          case pwr_cClass_ChanAi:
	    io_ConvertAi( cop, (pwr_tInt16) data, &actvalue);
	    break;
          case pwr_cClass_ChanAit:
	    io_ConvertAit( (pwr_sClass_ChanAit *) cop, (pwr_tInt16) data, 
		&actvalue);
	    break;
	}

        /* Filter */
	sts = io_AiFilter( (void *) op, &actvalue, local->FilterData[i]);

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

pwr_dExport pwr_BindIoMethods(Ai_7436) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
