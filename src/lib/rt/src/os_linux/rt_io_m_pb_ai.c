/* rt_io_m_pb_ai.c
   PROVIEW/R	*/

#pragma pack(1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <sys/file.h>
#include <sys/ioctl.h>

#include "pb_type.h"
#include "pb_if.h"
#include "pb_fmb.h"
#include "pb_dp.h"
#include "rt_io_pb_locals.h"

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "rt_io_profiboard.h"

/*----------------------------------------------------------------------------*\
  Convert ai from rawvalue to actualvalue.
\*----------------------------------------------------------------------------*/

void ConvertAi (
  pwr_sClass_ChanAi   *cop,
  pwr_tInt16	      nobits,
  pwr_tUInt8	      rawvalue8,
  pwr_tUInt16	      rawvalue16,
  pwr_tUInt32	      rawvalue32,
  pwr_tFloat32	      *actvalue_p,
  pwr_tEnum	      representation
)

{
  pwr_tFloat32		sigvalue;
  pwr_tFloat32		actvalue;
  pwr_tFloat32		*polycoef_p;
  pwr_tFloat32		f_raw;
  int			i;

  if (nobits == 32) {
    if (representation == PB_NUMREP_SIGNEDINT)
      f_raw = (float) ((int) rawvalue32);
    else
      f_raw = (float) rawvalue32;
  }
  else if (nobits == 16){
    if (representation == PB_NUMREP_SIGNEDINT)
      f_raw = (float) ((short) rawvalue16);
    else
      f_raw = (float) rawvalue16;
  }
  else if (nobits == 8){
    if (representation == PB_NUMREP_SIGNEDINT)
      f_raw = (float) ((char) rawvalue8);
    else
      f_raw = (float) rawvalue8;
  }

  switch ( cop->SensorPolyType)
  {
    case 0:
      *actvalue_p = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * f_raw;
      break;
    case 1:
      *actvalue_p = cop->SensorPolyCoef0 + cop->SensorPolyCoef1 * f_raw;
      break;
    case 2:
      sigvalue = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * f_raw;
      polycoef_p = &cop->SensorPolyCoef2;
      actvalue = 0;
      for ( i = 0; i < 3; i++)
      {
        actvalue = sigvalue * actvalue + *polycoef_p;
        polycoef_p--;
      }
      *actvalue_p = actvalue;
      break;
    case 3:
      sigvalue = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * f_raw;
      actvalue = cop->SensorPolyCoef0 + cop->SensorPolyCoef1 * sigvalue;
      if ( actvalue >= 0)
        *actvalue_p = cop->SensorPolyCoef2 * sqrt( actvalue);
      else
        *actvalue_p = 0;
      break;      
    case 4:
      sigvalue = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * f_raw;
      actvalue = cop->SensorPolyCoef0 + cop->SensorPolyCoef1 * sigvalue;
      if ( actvalue >= 0)
        *actvalue_p = cop->SensorPolyCoef2 * sqrt( actvalue);
      else
        *actvalue_p = -cop->SensorPolyCoef2 * sqrt( -actvalue);
      break;      
  }
}

/*----------------------------------------------------------------------------*\
  Convert ait from rawvalue to actualvalue.
\*----------------------------------------------------------------------------*/

void ConvertAit (
  pwr_sClass_ChanAit  *cop,
  pwr_tInt16	      nobits,
  pwr_tUInt16	      rawvalue8,
  pwr_tUInt16	      rawvalue16,
  pwr_tUInt32	      rawvalue32,
  pwr_tFloat32	      *actvalue_p,
  pwr_tEnum	      representation
) 
{
  pwr_tFloat32	Slope;
  pwr_tFloat32  Intercept;
  pwr_tFloat32  TransTabValue;
  pwr_tUInt16	First = 0;
  pwr_tUInt16	Middle;
  pwr_tUInt16	Last;
  pwr_tFloat32  sigvalue;
  pwr_tFloat32  f_raw;

  if (nobits == 32) {
    if (representation == PB_NUMREP_SIGNEDINT)
      f_raw = (float) ((int) rawvalue32);
    else
      f_raw = (float) rawvalue32;
  }
  else if (nobits == 16) {
    if (representation == PB_NUMREP_SIGNEDINT)
      f_raw = (float) ((short) rawvalue16);
    else
      f_raw = (float) rawvalue16;
  }
  else if (nobits == 8) {
    if (representation == PB_NUMREP_SIGNEDINT)
      f_raw = (float) ((char) rawvalue8);
    else
      f_raw = (float) rawvalue8;
  }

  sigvalue = cop->SigValPolyCoef0 + cop->SigValPolyCoef1 * f_raw;

  Last = cop->NoOfCoordinates;

  while ( (Last - First) > 1) {
    Middle = (First+Last) / 2;
    TransTabValue = cop->InValue[Middle];
    if (sigvalue < TransTabValue)
      Last = Middle;
    else
      First = Middle;
  }
  
  Intercept = cop->Intercept[First];
  Slope = cop->Slope[First];
  *actvalue_p = Intercept + sigvalue * Slope; 
}


/*----------------------------------------------------------------------------*\
   Init method for the Pb module Ai
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Ai *op;
  int i;
  io_sChannel *chanp;

  op = (pwr_sClass_Pb_Ai *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  if (rp->Class != pwr_cClass_Pb_DP_Slave) {
    errh_Info( "Illegal object type %s", cp->Name );
    return 1;
  }

  if (op->Status >= 1) {

    for (i=0; i<IO_MAXCHAN; i++) {
      local->scancount[i] = 0;
    }
    // Calculate polycoeff
    for (i=0; i<cp->ChanListSize; i++) {
      chanp = &cp->chanlist[i];
      if (!chanp->cop) continue;
      io_AiRangeToCoef(chanp);
    }
  }
  else
    errh_Info( "Error initializing Pb module Ai %s", cp->Name );

  return 1;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb Ai card
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Ai *op;
  pwr_sClass_Pb_DP_Slave *slave;
  io_sChannel *chanp;
  pwr_sClass_ChanAi *cop;
  pwr_sClass_Ai *sop;
  pwr_tInt8 data8 = 0;
  pwr_tUInt8 udata8 = 0;
  pwr_tInt16 data16 = 0;
  pwr_tUInt16 udata16 = 0;
  pwr_tInt32 data32 = 0;
  pwr_tUInt32 udata32 = 0;
  pwr_tFloat32 actvalue;
  int i;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Ai *) cp->op;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;
  
  if (op->Status >= 1) {
    
    for (i=0; i<cp->ChanListSize; i++) {

      chanp = &cp->chanlist[i];
      if (!chanp->cop) continue;

      cop = (pwr_sClass_ChanAi *) chanp->cop;
      sop = (pwr_sClass_Ai *) chanp->sop;

      if (cop->CalculateNewCoef) io_AiRangeToCoef(chanp);

      if (cop->ConversionOn) {
        if (local->scancount[i] <= 1) {
	
          if (op->BytesPerChannel == 4) {
	    memcpy(&udata32, local->input_area + op->OffsetInputs + 4*i, 4);
	    if (slave->ByteOrdering == PB_BYTEORDERING_BE) udata32 = swap32(udata32);
	    data32 = (pwr_tInt32) udata32;
	    sop->RawValue = 0;		
            if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT)
     	      sop->SigValue = udata32 * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
            else
     	      sop->SigValue = data32 * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
            switch(chanp->ChanClass) {
              case pwr_cClass_ChanAi:
                ConvertAi(cop, 32, 0, 0, udata32, &actvalue, op->NumberRepresentation);
                break;
              case pwr_cClass_ChanAit:
  	        ConvertAit((pwr_sClass_ChanAit *) cop, 32, 0, 0, udata32, &actvalue, op->NumberRepresentation);
	        break;
            }
          }
          else if (op->BytesPerChannel == 2) {
	    memcpy(&udata16, local->input_area + op->OffsetInputs + 2*i, 2);
	    if (slave->ByteOrdering == PB_BYTEORDERING_BE) udata16 = swap16(udata16);
	    data16 = (pwr_tInt16) udata16;
	    sop->RawValue = udata16;
            if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT)
     	      sop->SigValue = udata16 * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
            else
     	      sop->SigValue = data16 * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
            switch(chanp->ChanClass) {
              case pwr_cClass_ChanAi:
                ConvertAi(cop, 16, 0, udata16, 0, &actvalue, op->NumberRepresentation);
                break;
              case pwr_cClass_ChanAit:
  	        ConvertAit((pwr_sClass_ChanAit *) cop, 16, 0, udata16, 0, &actvalue, op->NumberRepresentation);
	        break;
            }
          }

          else if (op->BytesPerChannel == 1) {
	    memcpy(&udata8, local->input_area + op->OffsetInputs + i, 1);
	    data8 = (pwr_tInt8) udata8;
	    sop->RawValue = udata8;
            if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT)
     	      sop->SigValue = udata8 * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
            else
     	      sop->SigValue = data8 * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
            switch(chanp->ChanClass) {
              case pwr_cClass_ChanAi:
                ConvertAi(cop, 8, udata8, 0, 0, &actvalue, op->NumberRepresentation);
                break;
              case pwr_cClass_ChanAit:
  	        ConvertAit((pwr_sClass_ChanAit *) cop, 8, udata8, 0, 0, &actvalue, op->NumberRepresentation);
	        break;
            }
          }

	  // Filter
 	  if (sop->FilterType == 1 &&
	      sop->FilterAttribute[0] > 0 &&
	      sop->FilterAttribute[0] > ctx->ScanTime) {
	    actvalue = *(pwr_tFloat32 *) chanp->vbp +
		ctx->ScanTime / sop->FilterAttribute[0] *
		(actvalue - *(pwr_tFloat32 *) chanp->vbp);
          }

          *(pwr_tFloat32 *) chanp->vbp = actvalue;
          local->scancount[i] = cop->ScanInterval + 1;
        }
        local->scancount[i]--;

      }  // if ...ConversionOn
    }  // for
  }  // if ...op->Status

  return 1;
}


/*----------------------------------------------------------------------------*\
   Close method for the Pb Ai card
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardClose (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  local = rp->Local;

  free ((char *) local);

  return 1;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_Ai) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};
