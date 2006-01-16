/* 
 * Proview   $Id: rt_io_m_pb_ao.c,v 1.1 2006-01-16 10:55:42 claes Exp $
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
    return IO__SUCCESS;
  }

  if (op->Status >= PB_MODULE_STATE_OPERATE) {

    // Calculate polycoeff
    for (i=0; i<cp->ChanListSize; i++) {
      chanp = &cp->chanlist[i];
      if (!chanp->cop) continue;
      io_AoRangeToCoef(chanp);
    }
  }
  else
    errh_Info( "Error initializing Pb module Ao %s", cp->Name );

  return IO__SUCCESS;
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
  pwr_tUInt8 udata8 = 0;
  pwr_tUInt16 udata16 = 0;
  pwr_tUInt32 udata32 = 0;
  pwr_sClass_ChanAo *cop;
  pwr_sClass_Ao *sop;
  io_sChannel *chanp;
  int fixout;
  pwr_tFloat32 value;
  pwr_tFloat32 rawvalue;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Ao *) cp->op;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;
  
  if (op->Status >= PB_MODULE_STATE_OPERATE && slave->DisableSlave != 1) {

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

      if ( rawvalue > 0)
        rawvalue = rawvalue + 0.5;
      else
        rawvalue = rawvalue - 0.5;

      // We don´t use RawValue in Profibus I/O
      sop->RawValue = 0;

      // Calculate signal value
      sop->SigValue = cop->SigValPolyCoef1 * value + cop->SigValPolyCoef0;

      if (op->BytesPerChannel == 4) {

	if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT) {
          udata32 = (pwr_tUInt32) rawvalue;
          if (slave->ByteOrdering == PB_BYTEORDERING_BE) udata32 = swap32(udata32);
          memcpy(local->output_area + op->OffsetOutputs + 4*i, &udata32, 4);
	}
	else if (op->NumberRepresentation == PB_NUMREP_SIGNEDINT) {
          data32 = (pwr_tInt32) rawvalue;
          if (slave->ByteOrdering == PB_BYTEORDERING_BE) data32 = swap32(data32);
          memcpy(local->output_area + op->OffsetOutputs + 4*i, &data32, 4);
	}

      }
      else if (op->BytesPerChannel == 3) {

	if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT) {
          udata32 = (pwr_tUInt32) rawvalue;
          if (slave->ByteOrdering == PB_BYTEORDERING_BE) udata32 = swap32(udata32);
          memcpy(local->output_area + op->OffsetOutputs + 3*i, &udata32, 3);
	}
	else if (op->NumberRepresentation == PB_NUMREP_SIGNEDINT) {
          data32 = (pwr_tInt32) rawvalue;
          if (slave->ByteOrdering == PB_BYTEORDERING_BE) data32 = swap32(data32);
          memcpy(local->output_area + op->OffsetOutputs + 3*i, &data32, 3);
	}

      }
      else if (op->BytesPerChannel == 2) {

	if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT) {
          udata16 = (pwr_tUInt16) rawvalue;
          if (slave->ByteOrdering == PB_BYTEORDERING_BE) udata16 = swap16(udata16);
          memcpy(local->output_area + op->OffsetOutputs + 2*i, &udata16, 2);
        }
	else if (op->NumberRepresentation == PB_NUMREP_SIGNEDINT) {
          data16 = (pwr_tInt16) rawvalue;
          if (slave->ByteOrdering == PB_BYTEORDERING_BE) data16 = swap16(data16);
          memcpy(local->output_area + op->OffsetOutputs + 2*i, &data16, 2);
        }
	
      }
      else if (op->BytesPerChannel == 1) {

	if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT) {
          udata8 = (pwr_tUInt8) rawvalue;
          memcpy(local->output_area + op->OffsetOutputs + i, &udata8, 1);
        }	
	else if (op->NumberRepresentation == PB_NUMREP_SIGNEDINT) {
          data8 = (pwr_tInt8) rawvalue;
          memcpy(local->output_area + op->OffsetOutputs + i, &data8, 1);
        }
      }
    }
  }

  return IO__SUCCESS;
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
  local = cp->Local;

  free ((char *) local);

  return IO__SUCCESS;
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
