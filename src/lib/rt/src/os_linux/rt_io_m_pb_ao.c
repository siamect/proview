/* rt_io_m_pb_ao.c
   PROVIEW/R	*/

#pragma pack(1)

#include <stdio.h>
#include <string.h>
#include <errno.h>
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
   Init method for the Pb module Ao
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Ao *op;
  io_sChannel *chanp;
  int i;

  op = (pwr_sClass_Pb_Ao *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  if (rp->Class != pwr_cClass_Pb_DP_Slave) {
    errh_Info( "Illegal object type %s", cp->Name );
    return 1;
  }

  if (op->Status >= 1) {

    // Calculate polycoeff
    for (i=0; i<cp->ChanListSize; i++) {
      chanp = &cp->chanlist[i];
      if (!chanp->cop) continue;
      io_AoRangeToCoef(chanp);
    }
  }
  else
    errh_Info( "Error initializing Pb module Ao %s", cp->Name );

  return 1;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb module Ao
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Ao *op;
  pwr_sClass_Pb_DP_Slave *slave;
  int i;
  pwr_tInt8 data8 = 0;
  pwr_tInt16 data16 = 0;
  pwr_tInt32 data32 = 0;
  pwr_sClass_ChanAo *cop;
  pwr_sClass_Ao *sop;
  io_sChannel *chanp;
  int fixout;
  pwr_tFloat32 value;
  pwr_tFloat32 rawvalue;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Ao *) cp->op;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;
  
  if (op->Status >= 1) {

    fixout = ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT;

    for (i=0; i<cp->ChanListSize; i++) {

      chanp = &cp->chanlist[i];
      if (!chanp->cop) continue;

      cop = (pwr_sClass_ChanAo *) chanp->cop;
      sop = (pwr_sClass_Ao *) chanp->sop;

      // Determine what actual value we actually want!
      if (fixout)
        value = cop->FixedOutValue;
      else if (cop->TestOn)
        value = cop->TestValue;
      else
	value = *(pwr_tFloat32 *) chanp->vbp;

      // Make new coeff.. if necessary
      if (cop->CalculateNewCoef)
        io_AoRangeToCoef(chanp);

      // Convert to rawvalue
      if (value > cop->ActValRangeHigh)
        value = cop->ActValRangeHigh;
      else if ( value < cop->ActValRangeLow)
        value = cop->ActValRangeLow;

      rawvalue = cop->OutPolyCoef1 * value + cop->OutPolyCoef0;

      if (op->BytesPerChannel == 4) {

        if ( rawvalue > 0)
          rawvalue = rawvalue + 0.5;
        else
          rawvalue = rawvalue - 0.5;

	sop->RawValue = 0;

        data32 = (pwr_tInt32) rawvalue;
        if (slave->ByteOrdering == PB_BYTEORDERING_BE) data32 = swap32(data32);

        memcpy(local->output_area + op->OffsetOutputs + 4*i, &data32, 4);
      }
      else if (op->BytesPerChannel == 2) {
        if ( rawvalue > 0)
          sop->RawValue = rawvalue + 0.5;
        else
          sop->RawValue = rawvalue - 0.5;

        data16 = (pwr_tInt16) sop->RawValue;
        if (slave->ByteOrdering == PB_BYTEORDERING_BE) data16 = swap16(data16);

        memcpy(local->output_area + op->OffsetOutputs + 2*i, &data16, 2);
      }
      else if (op->BytesPerChannel == 1) {
        if ( rawvalue > 0)
          sop->RawValue = rawvalue + 0.5;
        else
          sop->RawValue = rawvalue - 0.5;

        data8 = (pwr_tInt8) sop->RawValue;

        memcpy(local->output_area + op->OffsetOutputs + i, &data8, 1);
      }
    }
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
  io_sCardLocal *local;
  local = rp->Local;

  free ((char *) local);

  return 1;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_Ao) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardWrite),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};
