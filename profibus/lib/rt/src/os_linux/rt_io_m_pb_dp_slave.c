/* 
 * Proview   $Id: rt_io_m_pb_dp_slave.c,v 1.1 2006-01-16 10:55:42 claes Exp $
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

/* rt_io_m_pb_dp_slave.c -- io methods for a profibus DP slave */

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
#include "co_cdh.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "co_cdh.h"
#include "rt_io_profiboard.h"
#include "rt_pb_msg.h"


/*----------------------------------------------------------------------------*\
   Init method for the Pb DP slave 
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  io_sCardLocal *local_card;
  io_sCard *cardp;
  short input_counter;
  short output_counter;
  pwr_sClass_Pb_DP_Slave *op;
  pwr_sClass_Pb_Di *dip;
  pwr_sClass_Pb_Do *dop;
  pwr_sClass_Pb_Ai *aip;
  pwr_sClass_Pb_Ao *aop;
  pwr_sClass_Pb_Ii *iip;
  pwr_sClass_Pb_Io *iop;
  pwr_sClass_Pb_Module *mp;
  char name[196];
  pwr_tStatus sts;
  pwr_tCid cid;

  sts = gdh_ObjidToName(rp->Objid, (char *) &name, sizeof(name), cdh_mNName);
  errh_Info( "Init of Profibus DP Slave and modules %s", name);

  op = (pwr_sClass_Pb_DP_Slave *) rp->op;
  
  // Do configuration check and initialize modules.

  cardp = rp->cardlist;
  input_counter = 0;
  output_counter = 0;
  op->NumberModules = 0;

  while(cardp) {
    local_card = calloc(1, sizeof(*local_card));
    cardp->Local = local_card;
    local_card->input_area = (void *) &(op->Inputs);
    local_card->output_area = (void *) &(op->Outputs);

    cid = cardp->Class;
    while ( ODD( gdh_GetSuperClass( cid, &cid, cardp->Objid))) ;

    switch (cid) {

      case pwr_cClass_Pb_Di:
        dip = (pwr_sClass_Pb_Di *) cardp->op;
        dip->OffsetInputs = input_counter;
        dip->BytesOfInput = dip->NumberOfChannels / 8;
        input_counter += dip->BytesOfInput;
        dip->Status = PB_MODULE_STATE_OPERATE;
        break;

      case pwr_cClass_Pb_Do:
        dop = (pwr_sClass_Pb_Do *) cardp->op;
        dop->OffsetOutputs = output_counter;
        dop->BytesOfOutput = dop->NumberOfChannels / 8;
        output_counter += dop->BytesOfOutput;
        dop->Status = PB_MODULE_STATE_OPERATE;
        break;

      case pwr_cClass_Pb_Ai:
        aip = (pwr_sClass_Pb_Ai *) cardp->op;
        aip->OffsetInputs = input_counter;
        aip->BytesOfInput = aip->NumberOfChannels * aip->BytesPerChannel;
        input_counter += aip->BytesOfInput;
        aip->Status = PB_MODULE_STATE_OPERATE;
        break;

      case pwr_cClass_Pb_Ao:
        aop = (pwr_sClass_Pb_Ao *) cardp->op;
        aop->OffsetOutputs = output_counter;
        aop->BytesOfOutput = aop->NumberOfChannels * aop->BytesPerChannel;
        output_counter += aop->BytesOfOutput;
        aop->Status = PB_MODULE_STATE_OPERATE;
        break;

      case pwr_cClass_Pb_Ii:
        iip = (pwr_sClass_Pb_Ii *) cardp->op;
        iip->OffsetInputs = input_counter;
        iip->BytesOfInput = iip->NumberOfChannels * iip->BytesPerChannel;
        input_counter += iip->BytesOfInput;
        iip->Status = PB_MODULE_STATE_OPERATE;
        break;

      case pwr_cClass_Pb_Io:
        iop = (pwr_sClass_Pb_Io *) cardp->op;
        iop->OffsetOutputs = output_counter;
        iop->BytesOfOutput = iop->NumberOfChannels * iop->BytesPerChannel;
        output_counter += iop->BytesOfOutput;
        iop->Status = PB_MODULE_STATE_OPERATE;
        break;

      case pwr_cClass_Pb_Module:
        mp = (pwr_sClass_Pb_Module *) cardp->op;
//        iop->OffsetOutputs = output_counter;
//        iop->BytesOfOutput = iop->NumberOfChannels * iop->BytesPerChannel;
//        output_counter += iop->BytesOfOutput;
        mp->Status = PB__SUCCESS;
        break;
    }

    op->NumberModules++;
    cardp = cardp->next;
  }

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb DP slave 
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  pwr_sClass_Pb_Profiboard *mp;
  pwr_sClass_Pb_DP_Slave *sp;
  int fp;
  unsigned char diag;
  pwr_tUInt16 sts;
  
  fp = ((io_sAgentLocal *) (ap->Local))->Pb_fp;
  
  sp = (pwr_sClass_Pb_DP_Slave *) rp->op;
  mp = (pwr_sClass_Pb_Profiboard *) ap->op;

  if (sp->Status > PB_SLAVE_STATE_NOTINIT && mp->Status == PB_MASTER_STATE_OPERATE && sp->DisableSlave != 1 && mp->DisableBus != 1) {

    sts = pb_cmi_get_data(fp, ID_DP_STATUS_IMAGE, sp->SlaveAddress, 1, &diag);

    if ((sts != PB_OK) || (diag & 1)) {
      sp->Status = PB_SLAVE_STATE_STOPPED;
      sp->ErrorCount++;
      if (sp->ErrorCount > sp->ErrorSoftLimit && sp->StallAction >= PB_STALLACTION_RESET) {
        memset(&sp->Inputs, 0, sp->BytesOfInput);
      }
    }
    else {
      sp->Status = PB_SLAVE_STATE_OPERATE;
    }

    if ((sp->Status > PB_SLAVE_STATE_STOPPED) && sp->BytesOfInput > 0) {

      sts = pb_cmi_get_data(fp, 
			ID_DP_SLAVE_IO_IMAGE, 
			sp->OffsetInputs, 
			sp->BytesOfInput, 
			&sp->Inputs);

      if (sts != PB_OK) 
        sp->ErrorCount++;
      else
        sp->ErrorCount = 0;
    }

    if (sp->ErrorCount > sp->ErrorHardLimit && sp->StallAction >= PB_STALLACTION_BREAK)
      ctx->Node->EmergBreakTrue = 1;
  }

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb DP slave
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  pwr_sClass_Pb_Profiboard *mp;
  pwr_sClass_Pb_DP_Slave *sp;
  int fp;
  pwr_tUInt16 sts;

  fp = ((io_sAgentLocal *) (ap->Local))->Pb_fp;
  
  sp = (pwr_sClass_Pb_DP_Slave *) rp->op;
  mp = (pwr_sClass_Pb_Profiboard *) ap->op;

  // Write the whole I/O output area from local area

  if (sp->Status > PB_SLAVE_STATE_NOTINIT && mp->Status == PB_MASTER_STATE_OPERATE && sp->DisableSlave != 1 && mp->DisableBus != 1) {

    if (sp->BytesOfOutput > 0) {

      sts = pb_cmi_set_data(fp, 
			ID_DP_SLAVE_IO_IMAGE, 
			sp->OffsetOutputs, 
			sp->BytesOfOutput, 
			&sp->Outputs);

      if (sts != PB_OK) sp->ErrorCount++;
    }
  }

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackClose (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_DP_Slave) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_BindIoMethod(IoRackRead),
  pwr_BindIoMethod(IoRackWrite),
  pwr_BindIoMethod(IoRackClose),
  pwr_NullMethod
};
