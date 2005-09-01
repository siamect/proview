/* 
 * Proview   $Id: rt_io_m_pb_npba12.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_io_m_pb_npba12.c -- io methods for the profibus slave */

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
#include "rt_gdh.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "co_cdh.h"
#include "rt_io_profiboard.h"


/*----------------------------------------------------------------------------*\
   Init method for the Pb slave NPBA12
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  io_sAgentLocal *local_agent;
  io_sRackLocal *local_rack;
  io_sCardLocal *local_card;

  pwr_sClass_Pb_NPBA12 *op;

  int i;
  pwr_tUInt16 sts;
  int fp;
  struct timespec rqtp, rmtp;
  io_sCard *cardp;
  short input_counter;
  short output_counter;
  pwr_sClass_Pb_Di *dip;
  pwr_sClass_Pb_Do *dop;
  pwr_sClass_Pb_Ai *aip;
  pwr_sClass_Pb_Ao *aop;

  local_agent = (io_sAgentLocal *) (ap->Local);
  local_rack = (io_sRackLocal *) (rp->Local);

  fp = local_agent->Pb_fp;

  op = (pwr_sClass_Pb_NPBA12 *) rp->op;

  rqtp.tv_sec = 1;
  rqtp.tv_nsec = 0;

  // Try to initialize slave.

  if (op->Status < 1) {

    // Three attempts

    for(i=0; i<3; i++) {
      if (op->AutoConfigure == 1) {
        sts = pb_get_slave_cfg(fp,
			       op->SlaveAddress,
			       &op->ConfigDataLen,
			       &op->ConfigData);
      }
      else {
        sts = PB_OK;
      }
      if (sts == PB_OK) {
        sts = pb_download_all(fp,
			      op->SlaveAddress,
			      op->WdFact1,
			      op->WdFact2,
			      0,
			      op->PNOIdent,
			      op->GroupIdent,
			      op->PrmUserDataLen,
			      &op->PrmUserData,
			      op->ConfigDataLen,
			      &op->ConfigData);
      }
      if (sts == PB_OK) {
        sts = pb_get_slave_info(fp, 
				op->SlaveAddress, 
				&op->OffsetInputs,
				&op->OffsetOutputs,
				&op->BytesOfInput,
				&op->BytesOfOutput);
      }
      if (sts == PB_OK) break;
      nanosleep(&rqtp, &rmtp);
    }
    if (sts != PB_OK) {
      errh_Info( "ERROR Init Pb slave NPBA12 %s", rp->Name );
      return IO__ERRINIDEVICE;
    }
  }

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
    local_card->byte_swap = 1;

    switch (cardp->Class) {

      case pwr_cClass_Pb_Di:
        dip = (pwr_sClass_Pb_Di *) cardp->op;
        dip->OffsetInputs = input_counter;
        dip->BytesOfInput = dip->NumberOfChannels / 8;
        input_counter += dip->BytesOfInput;
        dip->Status = 1;
        break;

      case pwr_cClass_Pb_Do:
        dop = (pwr_sClass_Pb_Do *) cardp->op;
        dop->OffsetOutputs = output_counter;
        dop->BytesOfOutput = dop->NumberOfChannels / 8;
        output_counter += dop->BytesOfOutput;
        dop->Status = 1;
        break;

      case pwr_cClass_Pb_Ai:
        aip = (pwr_sClass_Pb_Ai *) cardp->op;
        aip->OffsetInputs = input_counter;
        aip->BytesOfInput = aip->NumberOfChannels * aip->BytesPerChannel;
        input_counter += aip->BytesOfInput;
        aip->Status = 1;
        break;

      case pwr_cClass_Pb_Ao:
        aop = (pwr_sClass_Pb_Ao *) cardp->op;
        aop->OffsetOutputs = output_counter;
        aop->BytesOfOutput = aop->NumberOfChannels * aop->BytesPerChannel;
        output_counter += aop->BytesOfOutput;
        aop->Status = 1;
        break;
    }

    op->NumberModules++;
    cardp = cardp->next;
  }

  if (op->BytesOfInput != input_counter || op->BytesOfOutput != output_counter) {
    errh_Info( "Configuration mismatch in Pb slave NPBA12 %s", rp->Name);
    op->Status = 0;
    return IO__SUCCESS;
  }

  op->Status = 1;
  errh_Info( "Init Pb slave NPBA12 %s", rp->Name );
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb slave NPBA12
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  io_sRackLocal *local;
  io_sAgentLocal *agent_local;
  pwr_sClass_Pb_NPBA12 *op;
  int fp;
  unsigned char diag;
  pwr_tUInt16 sts;

  local = (io_sRackLocal *) rp->Local;
  agent_local = (io_sAgentLocal *) (ap->Local);
  fp = agent_local->Pb_fp;
  op = (pwr_sClass_Pb_NPBA12 *) rp->op;

  if (op->Status > 0) {

    sts = pb_cmi_get_data(fp, ID_DP_STATUS_IMAGE, op->SlaveAddress, 1, &diag);

    if ((sts != PB_OK) || (diag & 1)) {
      op->Status = 1;
      op->ErrorCount++;
      if (op->StallAction > 1) {
        memset(&op->Inputs, 0, op->BytesOfInput);
      }
    }
    else {
      op->Status = 2;
    }

    if ((op->Status > 1) && op->BytesOfInput > 0) {

      sts = pb_cmi_get_data(fp, 
			ID_DP_SLAVE_IO_IMAGE, 
			op->OffsetInputs, 
			op->BytesOfInput, 
			&op->Inputs);

      if (sts != PB_OK) 
        op->ErrorCount++;
      else
        op->ErrorCount = 0;
    }

    if (op->ErrorCount > op->ErrorHardLimit && op->StallAction == 2)
      ctx->Node->EmergBreakTrue = 1;
  }

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb slave NPBA12
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  io_sRackLocal *local;
  io_sAgentLocal *agent_local;
  pwr_sClass_Pb_NPBA12 *op;
  int fp;
  pwr_tUInt16 sts;

  local = (io_sRackLocal *) rp->Local;
  agent_local = (io_sAgentLocal *) (ap->Local);
  fp = agent_local->Pb_fp;
  op = (pwr_sClass_Pb_NPBA12 *) rp->op;

  // Write the whole I/O output area from local area

  if (op->Status > 0) {

    if (op->BytesOfOutput > 0) {

      sts = pb_cmi_set_data(fp, 
			ID_DP_SLAVE_IO_IMAGE, 
			op->OffsetOutputs, 
			op->BytesOfOutput, 
			&op->Outputs);

      if (sts != PB_OK) 
        op->ErrorCount++;
      else
        op->ErrorCount = 0;

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
  io_sRackLocal 	*local;

  /* Free dynamic memory  */
  local = rp->Local;

  free( (char *)local);

  return 1;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_NPBA12) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_BindIoMethod(IoRackRead),
  pwr_BindIoMethod(IoRackWrite),
  pwr_BindIoMethod(IoRackClose),
  pwr_NullMethod
};
