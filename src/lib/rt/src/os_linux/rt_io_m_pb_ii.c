/* 
 * Proview   $Id: rt_io_m_pb_ii.c,v 1.3 2005-09-01 14:57:57 claes Exp $
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
   Init method for the Pb module Ii
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Ii *op;

  op = (pwr_sClass_Pb_Ii *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  if (rp->Class != pwr_cClass_Pb_DP_Slave) {
    errh_Info( "Illegal object type %s", cp->Name );
    return IO__SUCCESS;
  }

  if (op->Status < PB_MODULE_STATE_OPERATE) errh_Info( "Error initializing Pb module Ii %s", cp->Name );

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb Ii card
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{

  io_sCardLocal *local;
  pwr_sClass_Pb_Ii *op;
  pwr_sClass_Pb_DP_Slave *slave;
  io_sChannel *chanp;
  pwr_sClass_ChanIi *cop;
  pwr_sClass_Ii *sop;
  pwr_tUInt8 udata8 = 0;
  pwr_tUInt16 udata16 = 0;
  pwr_tUInt32 udata32 = 0;
  pwr_tInt8 data8 = 0;
  pwr_tInt16 data16 = 0;
  pwr_tInt32 data32 = 0;
  int i;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Ii *) cp->op;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;
  
  if (op->Status >= PB_MODULE_STATE_OPERATE && slave->DisableSlave != 1) {
    
    for (i=0; i<cp->ChanListSize; i++) {

      chanp = &cp->chanlist[i];
      if (!chanp->cop) continue;

      cop = (pwr_sClass_ChanIi *) chanp->cop;
      sop = (pwr_sClass_Ii *) chanp->sop;

      if (cop->ConversionOn) {
      
        if (op->BytesPerChannel == 4) {
	  if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT) {
	    memcpy(&udata32, local->input_area + op->OffsetInputs + 4*i, 4);
	    if (slave->ByteOrdering == PB_BYTEORDERING_BE) udata32 = swap32(udata32);
            *(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) udata32;
	  }
	  else if (op->NumberRepresentation == PB_NUMREP_SIGNEDINT) {
	    memcpy(&data32, local->input_area + op->OffsetInputs + 4*i, 4);
	    if (slave->ByteOrdering == PB_BYTEORDERING_BE) data32 = swap32(data32);
            *(pwr_tInt32 *) chanp->vbp = data32;
	  }
        }
        else if (op->BytesPerChannel == 3) {
	  if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT) {
	    udata32 = 0;
	    memcpy(&udata32, local->input_area + op->OffsetInputs + 3*i, 3);
	    if (slave->ByteOrdering == PB_BYTEORDERING_BE) udata32 = swap32(udata32);
            *(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) udata32;
	  }
	  else if (op->NumberRepresentation == PB_NUMREP_SIGNEDINT) {
	    data32 = 0;
	    memcpy(&data32, local->input_area + op->OffsetInputs + 3*i, 3);
	    if (slave->ByteOrdering == PB_BYTEORDERING_BE) data32 = swap32(data32);
            *(pwr_tInt32 *) chanp->vbp = data32;
	  }
        }
        else if (op->BytesPerChannel == 2) {
	  if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT) {
 	    memcpy(&udata16, local->input_area + op->OffsetInputs + 2*i, 2);
	    if (slave->ByteOrdering == PB_BYTEORDERING_BE) udata16 = swap16(udata16);
            *(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) udata16;
	  }
	  else if (op->NumberRepresentation == PB_NUMREP_SIGNEDINT) {
	    memcpy(&data16, local->input_area + op->OffsetInputs + 2*i, 2);
	    if (slave->ByteOrdering == PB_BYTEORDERING_BE) data16 = swap16(data16);
            *(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) data16;
	  }
        }
        else if (op->BytesPerChannel == 1) {
	  if (op->NumberRepresentation == PB_NUMREP_UNSIGNEDINT) {
	    memcpy(&udata8, local->input_area + op->OffsetInputs + i, 1);
            *(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) udata8;
	  }
	  else if (op->NumberRepresentation == PB_NUMREP_SIGNEDINT) {
	    memcpy(&data8, local->input_area + op->OffsetInputs + i, 1);
            *(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) data8;
	  }
        }
      }
    }
  }

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Close method for the Pb Ii card
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

pwr_dExport pwr_BindIoMethods(Pb_Ii) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};
