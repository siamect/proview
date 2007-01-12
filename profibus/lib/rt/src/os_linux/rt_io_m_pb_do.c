/* 
 * Proview   $Id: rt_io_m_pb_do.c,v 1.5 2007-01-12 13:28:31 claes Exp $
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

/* rt_io_m_pb_do.c
   PROVIEW/R	*/

#pragma pack(1)

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/ioctl.h>

#include "keywords.h"

#include "pb_type.h"
#include "pb_conf.h"
#include "pb_if.h"
#include "pb_err.h"
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
   Init method for the Pb module Do
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Do *op;

  op = (pwr_sClass_Pb_Do *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  if (op->NumberOfChannels != 8 && op->NumberOfChannels != 16 && op->NumberOfChannels != 32)
    op->Status = PB_MODULE_STATE_NOTINIT;
    
  if (op->Orientation > op->NumberOfChannels)
    op->Status = PB_MODULE_STATE_NOTINIT;  

  if (op->Status < PB_MODULE_STATE_OPERATE) errh_Info( "Error initializing Pb module Do %s", cp->Name );

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb module Do
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Do *op;
  pwr_sClass_Pb_DP_Slave *slave;
  int i;
  io_sChannel *chanp;
  pwr_sClass_ChanDo *chan_do;
  pwr_sClass_Do *sig_do;
  pwr_tUInt32 mask = 0;
  pwr_tInt32 do_actval;

  pwr_tUInt16 data[2] = {0, 0};
  pwr_tUInt32 *data32;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Do *) cp->op;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;

  if (op->Status >= PB_MODULE_STATE_OPERATE && slave->DisableSlave != 1) {

    data32 = (pwr_tUInt32 *) &data;

    // Packa ner
    for (i=0; i<cp->ChanListSize; i++) {
      chanp = &cp->chanlist[i];
      chan_do = (pwr_sClass_ChanDo *) chanp->cop;
      sig_do = (pwr_sClass_Do *) chanp->sop;
      if (chan_do && sig_do) {
        mask = 1<<chan_do->Number;
        do_actval = *(pwr_tInt32 *) chanp->vbp;
	  if (do_actval != 0)
	    *data32 |= mask;
	  else
	    *data32 &= ~mask;
      }
    }
/*
    io_DoPackWord(cp, &data[0], 0);
    if (op->NumberOfChannels > 16) io_DoPackWord(cp, &data[1], 1);
*/
    if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) {
      if (op->Orientation == PB_ORIENTATION_WORD) {
        data[0] = swap16(data[0]);
        data[1] = swap16(data[1]);
      }
      else if (op->Orientation == PB_ORIENTATION_DWORD) {
	*data32 = swap32(*data32);
      }
    }    

    memcpy(local->output_area + op->OffsetOutputs, &data, op->BytesOfOutput);
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

pwr_dExport pwr_BindIoMethods(Pb_Do) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardWrite),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};
