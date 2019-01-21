/*
 * Copyright (C) 2010 SSAB Oxel�sund AB.
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

/* rt_io_m_pndevice.c -- io methods for a profinet device */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

#include "co_cdh.h"
#include "co_dcli.h"
#include "pwr_profibusclasses.h"

#include "rt_io_base.h"
#include "rt_io_bus.h"
#include "rt_io_msg.h"
#include "rt_pb_msg.h"
#include "rt_pn_gsdml_data.h"
#include "rt_io_pn_locals.h"

/*----------------------------------------------------------------------------*\
   Init method for the PnDevice
\*----------------------------------------------------------------------------*/

static pwr_tStatus IoRackInit(io_tCtx ctx, io_sAgent* ap, io_sRack* rp)
{
  io_sPnCardLocal* local_card;
  io_sPnRackLocal* local;
  io_sCard* cardp;
  short input_counter;
  short output_counter;
  pwr_sClass_PnDevice* op;
  pwr_sClass_PnModule* mp;
  char name[196];
  pwr_tStatus sts;
  pwr_tCid cid;

  io_sChannel* chanp;
  int i, latent_input_count, latent_output_count;
  pwr_tInt32 chan_size;
  pwr_sClass_ChanDi* chan_di;
  pwr_sClass_ChanDo* chan_do;
  pwr_sClass_ChanAi* chan_ai;
  pwr_sClass_ChanAit* chan_ait;
  pwr_sClass_ChanIi* chan_ii;
  pwr_sClass_ChanAo* chan_ao;
  pwr_sClass_ChanIo* chan_io;

  sts = gdh_ObjidToName(rp->Objid, (char*)&name, sizeof(name), cdh_mNName);
  errh_Info("Init of Profinet Device and Modules %s", name);

  op = (pwr_sClass_PnDevice*)rp->op;
  local = (io_sPnRackLocal*)rp->Local;

  local->start_time = (int)(op->StartupTime / ctx->ScanTime);

  // Do configuration check and initialize modules.

  cardp = rp->cardlist;
  input_counter = 0;
  output_counter = 0;

  latent_input_count = 0;
  latent_output_count = 0;

  while (cardp)
  {
    local_card = (io_sPnCardLocal*)calloc(1, sizeof(*local_card));
    cardp->Local = local_card;
    local_card->input_area = ((io_sPnRackLocal*)(rp->Local))->inputs;
    local_card->output_area = ((io_sPnRackLocal*)(rp->Local))->outputs;

    /* From v4.1.3 we can have subclasses, find the super class */

    cid = cardp->Class;
    while (ODD(gdh_GetSuperClass(cid, &cid, cardp->Objid)))
      ;

    switch (cid)
    {
    /* New style configuring (from v4.1.3) with Pb_Module objects or subclass.
      Loop all channels
      in the module and set channel size and offset. */

    case pwr_cClass_PnModule:
      mp = (pwr_sClass_PnModule*)cardp->op;
      mp->Status = PB__INITFAIL;
      cardp->offset = 0;
      for (i = 0; i < cardp->ChanListSize; i++)
      {
        chanp = &cardp->chanlist[i];

        if (chanp->ChanClass != pwr_cClass_ChanDi)
        {
          input_counter += latent_input_count;
          latent_input_count = 0;
        }

        if (chanp->ChanClass != pwr_cClass_ChanDo)
        {
          output_counter += latent_output_count;
          latent_output_count = 0;
        }

        switch (chanp->ChanClass)
        {
        case pwr_cClass_ChanDi:
          chan_di = (pwr_sClass_ChanDi*)chanp->cop;
          if (chan_di->Number == 0)
          {
            input_counter += latent_input_count;
            latent_input_count = 0;
          }
          chanp->offset = input_counter;
          chanp->mask = 1 << chan_di->Number;
          // TODO Check with the PROFINET specification what to do with boolean
          // values greater than 8 bit integers.
          // The data coming from an et200sp 16 DI module represented as
          // unsigned16 and the channel value of a
          // AI modules channel represented as integer16 are sent with different
          // byte order. Maybe booleans can be
          // considered Little Endian...
          // More TODO: Check host endianess aswell and take action accordingly
          if (chan_di->Representation == pwr_eDataRepEnum_Bit16 &&
              op->ByteOrdering == pwr_eByteOrderingEnum_BigEndian)
            chanp->mask = swap16(chanp->mask);
          if (chan_di->Representation == pwr_eDataRepEnum_Bit32 &&
              op->ByteOrdering == pwr_eByteOrderingEnum_BigEndian)
            chanp->mask = swap32(chanp->mask);
          if (chan_di->Number == 0)
            latent_input_count =
                GetChanSize((pwr_eDataRepEnum)chan_di->Representation);
          // printf("Di channel found in %s, Number %d, Offset %d\n",
          // cardp->Name, chan_di->Number, chanp->offset);
          break;

        case pwr_cClass_ChanAi:
          chan_ai = (pwr_sClass_ChanAi*)chanp->cop;
          chanp->offset = input_counter;
          chan_size = GetChanSize((pwr_eDataRepEnum)chan_ai->Representation);
          chanp->size = chan_size;
          chanp->mask = 0;
          input_counter += chan_size;
          io_AiRangeToCoef(chanp);
          //	      printf("Ai channel found in %s, Number %d, Offset %d\n",
          // cardp->Name, chan_ai->Number, chanp->offset);
          break;

        case pwr_cClass_ChanAit:
          chan_ait = (pwr_sClass_ChanAit*)chanp->cop;
          chanp->offset = input_counter;
          chan_size = GetChanSize((pwr_eDataRepEnum)chan_ait->Representation);
          chanp->size = chan_size;
          chanp->mask = 0;
          input_counter += chan_size;
          io_AiRangeToCoef(chanp);
          break;

        case pwr_cClass_ChanIi:
          chan_ii = (pwr_sClass_ChanIi*)chanp->cop;
          chanp->offset = input_counter;
          chan_size = GetChanSize((pwr_eDataRepEnum)chan_ii->Representation);
          chanp->size = chan_size;
          chanp->mask = 0;
          input_counter += chan_size;
          //	      printf("Ii channel found in %s, Number %d, Offset %d\n",
          // cardp->Name, chan_ii->Number, chanp->offset);
          break;

        case pwr_cClass_ChanDo:
          chan_do = (pwr_sClass_ChanDo*)chanp->cop;
          if (chan_do->Number == 0)
          {
            output_counter += latent_output_count;
            latent_output_count = 0;
          }
          chanp->offset = output_counter;
          chan_size = GetChanSize((pwr_eDataRepEnum)chan_do->Representation);
          chanp->mask = 1 << chan_do->Number;
          if (chan_do->Representation == pwr_eDataRepEnum_Bit16 &&
              op->ByteOrdering == pwr_eByteOrderingEnum_BigEndian)
            chanp->mask = swap16(chanp->mask);
          if (chan_do->Representation == pwr_eDataRepEnum_Bit32 &&
              op->ByteOrdering == pwr_eByteOrderingEnum_BigEndian)
            chanp->mask = swap32(chanp->mask);
          if (chan_do->Number == 0)
            latent_output_count =
                GetChanSize((pwr_eDataRepEnum)chan_do->Representation);
          // printf("Do channel found in %s, Number %d, Offset %d\n",
          // cardp->Name, chan_do->Number, chanp->offset);
          break;

        case pwr_cClass_ChanAo:
          chan_ao = (pwr_sClass_ChanAo*)chanp->cop;
          chanp->offset = output_counter;
          chan_size = GetChanSize((pwr_eDataRepEnum)chan_ao->Representation);
          chanp->size = chan_size;
          chanp->mask = 0;
          output_counter += chan_size;
          io_AoRangeToCoef(chanp);
          //	      printf("Ao channel found in %s, Number %d, Offset %d\n",
          // cardp->Name, chan_ao->Number, chanp->offset);
          break;

        case pwr_cClass_ChanIo:
          chan_io = (pwr_sClass_ChanIo*)chanp->cop;
          chanp->offset = output_counter;
          chan_size = GetChanSize((pwr_eDataRepEnum)chan_io->Representation);
          chanp->size = chan_size;
          chanp->mask = 0;
          output_counter += chan_size;
          //	      printf("Io channel found in %s, Number %d, Offset %d\n",
          // cardp->Name, chan_io->Number, chanp->offset);
          break;
        }
      }

      mp->Status = PB__SUCCESS;
      break;
    }

    cardp = cardp->next;
  }

  return IO__SUCCESS;
}

/*----------------------------------------------------------------------------*\
   Read method for the Pb DP slave
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackRead(io_tCtx ctx, io_sAgent* ap, io_sRack* rp)
{
  pwr_sClass_PnDevice* sp = (pwr_sClass_PnDevice*)rp->op;
  io_sPnRackLocal* local = (io_sPnRackLocal*)rp->Local;

  if (local->start_cnt < local->start_time)
    local->start_cnt++;

  /* The reading of the process image is now performed at the agent level,
  this eliminates the need for board specific code at the rack level.  */

  if (sp->Status == PB__NORMAL)
  {
    sp->ErrorCount = 0;
  }
  else
  {
    if (local->start_cnt >= local->start_time)
      sp->ErrorCount++;
  }

  if (sp->ErrorCount == sp->ErrorHardLimit)
  {
    errh_Error("IO Error hard limit reached on card '%s', stall action %d",
               rp->Name, sp->StallAction);
    ctx->IOHandler->CardErrorHardLimit = 1;
    ctx->IOHandler->ErrorHardLimitObject = cdh_ObjidToAref(rp->Objid);
    sp->ErrorCount++;
  }
  else if (sp->ErrorCount == sp->ErrorSoftLimit)
  {
    errh_Error("IO Error soft limit reached on card '%s'", rp->Name);
    ctx->IOHandler->CardErrorSoftLimit = 1;
    ctx->IOHandler->ErrorSoftLimitObject = cdh_ObjidToAref(rp->Objid);
    sp->ErrorCount++;
  }
  if (sp->ErrorCount > sp->ErrorHardLimit)
  {
    if (sp->StallAction == pwr_ePbStallAction_ResetInputs)
    {
      if (((io_sPnRackLocal*)(rp->Local))->bytes_of_input > 0)
        memset(((io_sPnRackLocal*)(rp->Local))->inputs, 0,
               ((io_sPnRackLocal*)(rp->Local))->bytes_of_input);
    }
    else if (sp->StallAction == pwr_ePbStallAction_EmergencyBreak)
      ctx->Node->EmergBreakTrue = 1;
  }

  return IO__SUCCESS;
}

/*----------------------------------------------------------------------------*\
   Write method for the Pb DP slave
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackWrite(io_tCtx ctx, io_sAgent* ap, io_sRack* rp)
{
  pwr_sClass_PnDevice* sp = (pwr_sClass_PnDevice*)rp->op;
  io_sPnRackLocal* local = (io_sPnRackLocal*)rp->Local;

  /* The writing of the process image is now performed at the agent level,
  this eliminates the need for board specific code at the rack level.  */

  if (sp->Status == PB__NORMAL)
  {
    sp->ErrorCount = 0;
  }
  else
  {
    if (local->start_cnt >= local->start_time)
      sp->ErrorCount++;
  }

  if (sp->ErrorCount == sp->ErrorHardLimit)
  {
    errh_Error("IO Error hard limit reached on card '%s', stall action %d",
               rp->Name, sp->StallAction);
    ctx->IOHandler->CardErrorHardLimit = 1;
    ctx->IOHandler->ErrorHardLimitObject = cdh_ObjidToAref(rp->Objid);
    sp->ErrorCount++;
  }
  else if (sp->ErrorCount == sp->ErrorSoftLimit)
  {
    errh_Error("IO Error soft limit reached on card '%s'", rp->Name);
    ctx->IOHandler->CardErrorSoftLimit = 1;
    ctx->IOHandler->ErrorSoftLimitObject = cdh_ObjidToAref(rp->Objid);
    sp->ErrorCount++;
  }
  if (sp->ErrorCount > sp->ErrorHardLimit)
  {
    if (sp->StallAction == pwr_ePbStallAction_EmergencyBreak)
      ctx->Node->EmergBreakTrue = 1;
  }

  return IO__SUCCESS;
}

/*----------------------------------------------------------------------------*\

\*----------------------------------------------------------------------------*/
static pwr_tStatus IoRackClose(io_tCtx ctx, io_sAgent* ap, io_sRack* rp)
{
  return IO__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(PnDevice) = {
    pwr_BindIoMethod(IoRackInit), pwr_BindIoMethod(IoRackRead),
    pwr_BindIoMethod(IoRackWrite), pwr_BindIoMethod(IoRackClose),
    pwr_NullMethod};
