/* 
 * Proview   $Id: rt_io_m_pb_di.c,v 1.2 2006-01-16 13:56:52 claes Exp $
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

/* rt_io_m_pb_di.c
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
#include "pwr_profibusclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "rt_io_profiboard.h"


/*----------------------------------------------------------------------------*\
   Init method for the Pb module Di
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Di *op;

  op = (pwr_sClass_Pb_Di *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  // Check configuration 
  
  if (op->NumberOfChannels != 8 && op->NumberOfChannels != 16 && op->NumberOfChannels != 32)
    op->Status = PB_MODULE_STATE_NOTINIT;
    
  if (op->Orientation > op->NumberOfChannels)
    op->Status = PB_MODULE_STATE_NOTINIT;  

  if (op->Status < PB_MODULE_STATE_OPERATE) errh_Info( "Error initializing Pb module Di %s", cp->Name );

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb module Di
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Di *op;
  pwr_sClass_Pb_DP_Slave *slave;

  pwr_tUInt16 data[2] = {0, 0};
  pwr_tUInt32 *data32;
  
  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Di *) cp->op;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;

  if (op->Status >= PB_MODULE_STATE_OPERATE && slave->DisableSlave != 1) {

    memcpy(&data, local->input_area + op->OffsetInputs, op->BytesOfInput);

    if (slave->ByteOrdering == PB_BYTEORDERING_BE) {
      if (op->Orientation == PB_ORIENTATION_WORD) {
        data[0] = swap16(data[0]);
        data[1] = swap16(data[1]);
      }
      else if (op->Orientation == PB_ORIENTATION_DWORD) {
      	data32 = (pwr_tUInt32 *) &data;
	*data32 = swap32(*data32);
      }
    }

    // Packa upp
    
    data[0] = data[0] ^ op->InvMask1;
    io_DiUnpackWord(cp, data[0], op->ConvMask1, 0);

    if (op->NumberOfChannels > 16) {
      data[1] = data[1] ^ op->InvMask2;
      io_DiUnpackWord(cp, data[1], op->ConvMask2, 1);
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

pwr_dExport pwr_BindIoMethods(Pb_Di) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};
