/*
 * ProviewR   Open Source Process Control.
 * Copyright (C) 2005-2019 SSAB EMEA AB.
 *
 * This file is part of ProviewR.
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
 * along with ProviewR. If not, see <http://www.gnu.org/licenses/>
 *
 * Linking ProviewR statically or dynamically with other modules is
 * making a combined work based on ProviewR. Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * In addition, as a special exception, the copyright holders of
 * ProviewR give you permission to, from the build function in the
 * ProviewR Configurator, combine ProviewR with modules generated by the
 * ProviewR PLC Editor to a PLC program, regardless of the license
 * terms of these modules. You may copy and distribute the resulting
 * combined work under the terms of your choice, provided that every
 * copy of the combined work is accompanied by a complete copy of
 * the source code of ProviewR (the version used to produce the
 * combined work), being distributed under the terms of the GNU
 * General Public License plus this exception.
 */

/* rt_io_m_do_hvdo32.c -- io methods for ssab cards.
   OS Linux
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "co_time.h"
#include "co_cdh.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_filter_po.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_write.h"
#include "qbus_io.h"
#include "rt_io_m_ssab_locals.h"
#include "rt_io_bfbeth.h"

/*----------------------------------------------------------------------------*\

\*----------------------------------------------------------------------------*/

typedef struct {
  unsigned int Address[2];
  int Qbus_fp;
  struct {
    pwr_sClass_Po* sop[16];
    void* Data[16];
    pwr_tBoolean Found;
  } Filter[2];
  unsigned int ErrReset;
  unsigned int ErrScanCnt;
} io_sLocal;

static pwr_tStatus IoCardInit(
    io_tCtx ctx, io_sAgent* ap, io_sRack* rp, io_sCard* cp)
{
  pwr_sClass_Do_HVDO32* op = (pwr_sClass_Do_HVDO32*)cp->op;
  io_sLocal* local;
  int i, j;
  io_sRackLocal* r_local = (io_sRackLocal*)(rp->Local);
  int words = op->MaxNoOfChannels <= 16 ? 1 : 2;

  local = calloc(1, sizeof(*local));
  cp->Local = local;

  errh_Info("Init of do card '%s'", cp->Name);

  local->Address[0] = op->RegAddress;
  local->Address[1] = op->RegAddress + 2;
  local->Qbus_fp = r_local->Qbus_fp;

  /* Init filter for Po signals */
  for (i = 0; i < words; i++) {
    /* The filter handles one 16-bit word */
    for (j = 0; j < 16; j++) {
      if (cp->chanlist[i * 16 + j].SigClass == pwr_cClass_Po)
        local->Filter[i].sop[j] = cp->chanlist[i * 16 + j].sop;
    }
    io_InitPoFilter(local->Filter[i].sop, &local->Filter[i].Found,
        local->Filter[i].Data, ctx->ScanTime);
  }

  local->ErrReset = 1.0 / ctx->ScanTime + 0.5;
  if (local->ErrReset < 2)
    local->ErrReset = 2;

  return 1;
}

/*----------------------------------------------------------------------------*\

\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardClose(
    io_tCtx ctx, io_sAgent* ap, io_sRack* rp, io_sCard* cp)
{
  io_sLocal* local;
  int i;
  pwr_sClass_Do_HVDO32* op = (pwr_sClass_Do_HVDO32*)cp->op;
  int words = op->MaxNoOfChannels <= 16 ? 1 : 2;

  local = (io_sLocal*)cp->Local;

  errh_Info("IO closing do card '%s'", cp->Name);

  /* Free filter data */
  for (i = 0; i < words; i++) {
    if (local->Filter[i].Found)
      io_ClosePoFilter(local->Filter[i].Data);
  }
  free((char*)local);

  return 1;
}

/*----------------------------------------------------------------------------*\

\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardWrite(
    io_tCtx ctx, io_sAgent* ap, io_sRack* rp, io_sCard* cp)
{
  io_sLocal* local = (io_sLocal*)cp->Local;
  io_sRackLocal* r_local = (io_sRackLocal*)(rp->Local);
  pwr_tUInt16 data = 0;
  pwr_tUInt16 invmask;
  pwr_tUInt16 testmask;
  pwr_tUInt16 testvalue;
  int i;
  qbus_io_write wb;
  int sts;
  pwr_sClass_Do_HVDO32* op = (pwr_sClass_Do_HVDO32*)cp->op;
  int words = op->MaxNoOfChannels <= 16 ? 1 : 2;

  for (i = 0; i < words; i++) {
    if (i == 1 && op->MaxNoOfChannels <= 16)
      break;

    if (ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT) {
      if (i == 0)
        data = op->FixedOutValue1;
      else
        data = op->FixedOutValue2;
    } else
      io_DoPackWord(cp, &data, i);

    if (i == 0) {
      testmask = op->TestMask1;
      invmask = op->InvMask1;
    } else {
      testmask = op->TestMask2;
      invmask = op->InvMask2;
      if (op->MaxNoOfChannels == 16)
        break;
    }

    /* Invert */
    data = data ^ invmask;

    /* Filter Po signals */
    if (local->Filter[i].Found)
      io_PoFilter(local->Filter[i].sop, &data, local->Filter[i].Data);

    /* Testvalues */
    if (testmask) {
      if (i == 0)
        testvalue = op->TestValue1;
      else
        testvalue = op->TestValue2;
      data = (data & ~testmask) | (testmask & testvalue);
    }

    if (r_local->Qbus_fp != 0 && r_local->s == 0) {
      /* Write to local Q-bus */
      wb.Data = data;
      wb.Address = local->Address[i];
      sts = write(local->Qbus_fp, &wb, sizeof(wb));
    } else {
      /* Ethernet I/O, Request a write to current address */
      bfbeth_set_write_req(r_local, (pwr_tUInt16)local->Address[i], data);
      sts = 1;
    }

    if (sts <= 0) {
      /* Increase error count and check error limits */
      op->ErrorCount++;

      if (op->ErrorCount == op->ErrorSoftLimit) {
        errh_Error("IO Error soft limit reached on card '%s'", cp->Name);
        ctx->IOHandler->CardErrorSoftLimit = 1;
        ctx->IOHandler->ErrorSoftLimitObject = cdh_ObjidToAref(cp->Objid);
      }
      if (op->ErrorCount >= op->ErrorHardLimit) {
        errh_Error(
            "IO Error hard limit reached on card '%s', IO stopped", cp->Name);
        ctx->Node->EmergBreakTrue = 1;
        ctx->IOHandler->CardErrorHardLimit = 1;
        ctx->IOHandler->ErrorHardLimitObject = cdh_ObjidToAref(cp->Objid);
        return IO__ERRDEVICE;
      }
      continue;
    }
  }

  /* Fix for qbus errors */
  local->ErrScanCnt++;
  if (local->ErrScanCnt >= local->ErrReset) {
    local->ErrScanCnt = 0;
    if (op->ErrorCount > op->ErrorSoftLimit)
      op->ErrorCount--;
  }

  return 1;
}

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Do_HVDO32)
    = { pwr_BindIoMethod(IoCardInit), pwr_BindIoMethod(IoCardClose),
        pwr_BindIoMethod(IoCardWrite), pwr_NullMethod };