/* 
 * Proview   $Id: rt_io_m_pb_ao.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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
  pwr_sClass_Pb_DP_Slave *slave;
  int i;

  op = (pwr_sClass_Pb_Ao *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  if (rp->Class == pwr_cClass_Pb_DP_Slave) {
    slave = (pwr_sClass_Pb_DP_Slave *) rp->op;

    /* Little endian, no problem */
    if (slave->ByteOrdering == 0) local->byte_swap = 0;

    /* Big endian, digitala moduler byteorienterade => swappa */
    if (slave->ByteOrdering == 1) local->byte_swap = 1;

    /* Big endian, digitala moduler wordorienterade => swappa */
    if (slave->ByteOrdering == 2) local->byte_swap = 1;
  }
  else {
    /* Old style */
    if (rp->Class == pwr_cClass_Pb_ET200M) local->byte_swap = 1;
    if (rp->Class == pwr_cClass_Pb_NPBA12) local->byte_swap = 1;
    if (rp->Class == pwr_cClass_Pb_Euro2500) local->byte_swap = 1;
  }

  if (op->Status >= 1) {
//    errh_Info( "Init of Pb module Ao %s", cp->Name );

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
  int i;
  pwr_tInt16 data = 0;
  pwr_sClass_ChanAo *cop;
  pwr_sClass_Ao *sop;
  io_sChannel *chanp;
  int fixout;
  pwr_tFloat32 value;
  pwr_tFloat32 rawvalue;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Ao *) cp->op;
  
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
      if ( rawvalue > 0)
        sop->RawValue = rawvalue + 0.5;
      else
        sop->RawValue = rawvalue - 0.5;

      data = (pwr_tInt16) sop->RawValue;
      if (local->byte_swap == 1) data = swap16(data);

      memcpy(local->output_area + op->OffsetOutputs + 2*i,
	     &data, 2);
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
