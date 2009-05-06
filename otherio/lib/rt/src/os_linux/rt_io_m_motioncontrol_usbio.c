/* 
 * Proview   $Id: rt_io_m_motioncontrol_usbio.c,v 1.1 2007-11-22 13:28:59 claes Exp $
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

/* rt_io_m_motioncontrol_usbio.c -- I/O methods for class MotionControl_USBIO. */

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "co_time.h"
#include "rt_io_base.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "rt_io_card_write.h"
#include "rt_io_msg.h"
#include "libusbio.h"
#include "rt_io_m_motioncontrol_usb.h"

typedef struct {
  pwr_tTime ErrTime;
  int USB_Handle;
  int portA_hasDi;
  int portA_hasDo;
  int portA_diMask;
  int portA_doMask;
  int portB_hasDi;
  int portB_hasDo;
  int portB_hasAi;
  int portB_diMask;
  int portB_doMask;
  int portB_aiMask;
  int portC_hasDi;
  int portC_hasDo;
  int portC_hasAo;
  int portC_hasIi;
  int portC_diMask;
  int portC_doMask;
  int portC_aoMask;
  int Idx;
  int Disconnected;
  pwr_tTime ConnectRetry;
} io_sLocal;


static int usbio_reconnect( io_tCtx ctx,
			    io_sAgent *ap,
			    io_sRack *rp,
			    io_sCard *cp)
{
  io_sLocal *local = cp->Local;
  io_sLocalUSB *localUSB = (io_sLocalUSB *)rp->Local;
  pwr_sClass_MotionControl_USBIO *op = (pwr_sClass_MotionControl_USBIO *)cp->op;
  pwr_tTime time;
  pwr_tDeltaTime diff;
  unsigned int snum;
  int handle;
  int i;
  int status;

  clock_gettime(CLOCK_REALTIME, &time);

  time_Adiff( &diff, &time, &local->ConnectRetry);
  if ( time_DToFloat( 0, &diff) > 1.0) {
    for ( i = 0; i < (int)sizeof(localUSB->USB_Handle); i++) {
      status = USBIO_Open( &handle);
      if ( status) break;

      status = USBIO_GetSerialNr( &handle, &snum);

      if ( snum == op->Super.Address) {
	local->USB_Handle = localUSB->USB_Handle[local->Idx] = handle;
	op->Status = status;
	op->Super.ErrorCount = 0;

	IoCardClose( ctx, ap, rp, cp);
	IoCardInit( ctx, ap, rp, cp);

	errh_Info( "USBIO card reconnected '%s'", cp->Name);
	return 1;
      }
    }
  }
  return 0;
}

static pwr_tStatus IoCardInit( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp,
			       io_sCard *cp)
{
  int found = 0;
  int i;
  unsigned char port_mask[3] = {255,255,63};
  unsigned char port[3] = {0,0,0};
  int active;
  int timeout;
  io_sLocal *local;
  io_sLocalUSB *localUSB = (io_sLocalUSB *)rp->Local;
  pwr_sClass_MotionControl_USBIO *op = (pwr_sClass_MotionControl_USBIO *)cp->op;

  local = (io_sLocal *) calloc( 1, sizeof(io_sLocal));
  cp->Local = local;

  /* Find the handle in rack local data */
  for ( i = 0; i < (int)sizeof(localUSB->USB_Handle); i++) {
    if ( localUSB->snum[i] == op->Super.Address) {
      found = 1;
      local->USB_Handle = localUSB->USB_Handle[i];
      break;
    }
  }

  if ( !found) {
    errh_Error( "Io init error, USBIO card not found '%s'", cp->Name);
    op->Status = pwr_eMotionControl_StatusEnum_FindDevice;
    return 0;
  }

  op->Status = USBIO_SoftReset( &local->USB_Handle);
  if ( op->Status)
    errh_Error( "IO Init Card '%s', Status %d", cp->Name, op->Status);

  /* Configure port A */
  local->portA_hasDi = 0;
  local->portA_hasDo = 0;
  local->portA_diMask = 0;
  local->portA_doMask = 0;
  for ( i = 0; i < 8; i++) {
    if ( cp->chanlist[i].cop && 
	 cp->chanlist[i].sop && 
	 cp->chanlist[i].ChanClass == pwr_cClass_ChanDi) {
      local->portA_hasDi = 1;
      local->portA_diMask |= (1 << i);
    }
    else if ( cp->chanlist[i].cop && 
	      cp->chanlist[i].sop && 
	      cp->chanlist[i].ChanClass == pwr_cClass_ChanDo) {
      local->portA_hasDo = 1;
      local->portA_doMask |= (1 << i);
    }
  }
  if ( local->portA_hasDi || local->portA_hasDo) {
    op->Status = USBIO_ConfigDIO( &local->USB_Handle, 1, local->portA_diMask);
    if ( op->Status)
      errh_Error( "IO Init Card '%s', Status %d", cp->Name, op->Status);
  }

  /* Configure port B */
  local->portB_hasDi = 0;
  local->portB_hasDo = 0;
  local->portB_hasAi = 0;
  for ( i = 8; i < 16; i++) {
    if ( cp->chanlist[i].cop && 
	 cp->chanlist[i].sop && 
	 cp->chanlist[i].ChanClass == pwr_cClass_ChanDi) {
      local->portB_hasDi = 1;
      local->portB_diMask |= (1 << (i - 8));
    }
    else if ( cp->chanlist[i].cop && 
	 cp->chanlist[i].sop && 
	 cp->chanlist[i].ChanClass == pwr_cClass_ChanDo) {
      local->portB_hasDo = 1;
      local->portB_doMask |= (1 << (i - 8));
    }
    if ( cp->chanlist[i].cop && 
	 cp->chanlist[i].sop && 
	 (cp->chanlist[i].ChanClass == pwr_cClass_ChanAi ||
	  cp->chanlist[i].ChanClass == pwr_cClass_ChanAit)) {
      local->portB_hasAi = 1;
      local->portB_aiMask |= (1 << (i - 8));

      // Calculate conversion coefficients
      io_AiRangeToCoef( &cp->chanlist[i]);
    }
  }

  if ( local->portB_hasDi || local->portB_hasDo) {
    op->Status = USBIO_ConfigDIO( &local->USB_Handle, 2, local->portB_diMask);
    if ( op->Status)
      errh_Error( "IO Init Card '%s', Status %d", cp->Name, op->Status);
  }

  if ( local->portB_hasAi) {
    int num_ai;
    for ( i = 0; i < 8; i++) {
      if ( local->portB_aiMask & (1 << i))
	num_ai = i + 1;
    }
    op->Status = USBIO_ConfigAI( &local->USB_Handle, num_ai);
    if ( op->Status)
      errh_Error( "IO Init Card '%s', Status %d", cp->Name, op->Status);
  }
  else
    op->Status = USBIO_ConfigAI( &local->USB_Handle, 0);

  /* Configure port C */
  local->portC_hasDi = 0;
  local->portC_hasDo = 0;
  local->portC_hasAo = 0;
  for ( i = 16; i < 21; i++) {
    if ( cp->chanlist[i].cop && 
	 cp->chanlist[i].sop && 
	 cp->chanlist[i].ChanClass == pwr_cClass_ChanDi) {
      local->portC_hasDi = 1;
      local->portC_diMask |= (1 << (i - 16));
    }
    else if ( cp->chanlist[i].cop && 
	 cp->chanlist[i].sop && 
	 cp->chanlist[i].ChanClass == pwr_cClass_ChanDo) {
      local->portC_hasDo = 1;
      local->portC_doMask |= (1 << (i - 16));
    }
    if ( cp->chanlist[i].cop && 
	 cp->chanlist[i].sop && 
	 cp->chanlist[i].ChanClass == pwr_cClass_ChanAo) {
      local->portC_hasAo = 1;
      local->portC_aoMask |= (1 << (i - 16));

      // Calculate conversion coefficients
      io_AoRangeToCoef( &cp->chanlist[i]);
    }
  }


  if ( cp->chanlist[18].cop && 
       cp->chanlist[18].sop && 
       cp->chanlist[18].ChanClass == pwr_cClass_ChanIi)
      local->portC_hasIi = 1;

  if ( local->portC_hasDi || local->portC_hasDo) {
    op->Status = USBIO_ConfigDIO( &local->USB_Handle, 3, local->portC_diMask);
    if ( op->Status)
      errh_Error( "IO Init Card '%s', Status %d", cp->Name, op->Status);
  }

  if ( local->portC_hasAo) {
    op->Status = USBIO_ConfigAO( &local->USB_Handle, local->portC_aoMask >> 3);
    if ( op->Status)
      errh_Error( "IO Init Card '%s', Status %d", cp->Name, op->Status);
  }
  else
    op->Status = USBIO_ConfigAO( &local->USB_Handle, 0);

  if ( local->portC_hasIi) {
    op->Status = USBIO_ConfigCounter( &local->USB_Handle, 0);
    if ( op->Status)
      errh_Error( "IO Init Card '%s', Status %d", cp->Name, op->Status);
  }

  // Configure Watchdog
  if ( op->WatchdogTime > 0)
    active = 1;
  else
    active = 0;

  timeout = 1000 * op->WatchdogTime;
  op->Status = USBIO_ConfigWatchdog( &local->USB_Handle, active, timeout, 1, port_mask, port, 3);

  errh_Info( "Init of USBIO card '%s'", cp->Name);

  return IO__SUCCESS;
}

static pwr_tStatus IoCardClose( io_tCtx ctx,
			        io_sAgent *ap,
			        io_sRack *rp,
			        io_sCard *cp)
{
  free( cp->Local);
  return IO__SUCCESS;
}

static pwr_tStatus IoCardRead( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack	*rp,
			       io_sCard	*cp)
{
  io_sLocal *local = cp->Local;
  pwr_sClass_MotionControl_USBIO *op = (pwr_sClass_MotionControl_USBIO *)cp->op;
  int value = 0;
  int i;
  unsigned int m;
  pwr_tUInt32 error_count = op->Super.ErrorCount;

  if ( local->Disconnected) {
    if ( !usbio_reconnect( ctx, ap, rp, cp))
      return IO__SUCCESS;
  }

  if ( local->portA_hasDi) {
    op->Status = USBIO_ReadDI( &local->USB_Handle, 1, &value);
    if ( op->Status) 
      op->Super.ErrorCount++;
    else {
      m = 1;
      for ( i = 0; i < 8; i++) {
	if ( m & local->portA_diMask)
	  *(pwr_tBoolean *)cp->chanlist[i].vbp = ((value & m) != 0);
	m = m << 1;
      }
    }
  }

  if ( local->portB_hasDi) {
    op->Status = USBIO_ReadDI( &local->USB_Handle, 2, &value);
    if ( op->Status) 
      op->Super.ErrorCount++;
    else {

      m = 1;
      for ( i = 0; i < 8; i++) {
	if ( m & local->portB_diMask)
	  *(pwr_tBoolean *)cp->chanlist[i+8].vbp = ((value & m) != 0);
	m = m << 1;
      }
    }
  }

  if ( local->portB_hasAi) {
    int ivalue;
    pwr_tFloat32 actvalue;

    // op->Status = USBIO_ReadAllAI( &local->USB_Handle, avalue);

    m = 1;
    for ( i = 0; i < 8; i++) {
      if ( m & local->portB_aiMask) {
	io_sChannel *chanp = &cp->chanlist[i + 8];
	pwr_sClass_ChanAi *cop = (pwr_sClass_ChanAi *)chanp->cop;
	pwr_sClass_Ai *sop = (pwr_sClass_Ai *)chanp->sop;

	if ( cop->CalculateNewCoef)
	  // Request to calculate new coefficients
	  io_AiRangeToCoef( chanp);

	op->Status = USBIO_ReadADVal( &local->USB_Handle, i + 1, &ivalue);
	if ( op->Status) 
	  op->Super.ErrorCount++;
	else {
	  io_ConvertAi( cop, ivalue, &actvalue);

	  // Filter
	  if ( sop->FilterType == 1 &&
	       sop->FilterAttribute[0] > 0 &&
	       sop->FilterAttribute[0] > ctx->ScanTime) {
	    actvalue = *(pwr_tFloat32 *)chanp->vbp + ctx->ScanTime / sop->FilterAttribute[0] *
	      (actvalue - *(pwr_tFloat32 *)chanp->vbp);
	  }

	  *(pwr_tFloat32 *)chanp->vbp = actvalue;
	  sop->SigValue = cop->SigValPolyCoef1 * ivalue + cop->SigValPolyCoef0;
	  sop->RawValue = ivalue;
	}
      }
      m = m << 1;
    }
  }

  if ( local->portC_hasDi) {
    op->Status = USBIO_ReadDI( &local->USB_Handle, 3, &value);
    if ( op->Status) 
      op->Super.ErrorCount++;
    else {
      m = 1;
      for ( i = 0; i < 5; i++) {
	if ( m & local->portC_diMask)
	  *(pwr_tBoolean *)cp->chanlist[i+16].vbp = ((value & m) != 0);
	m = m << 1;
      }
    }
  }

  if ( local->portC_hasIi) {
    int overflow;
    unsigned int covalue;
    
    op->Status = USBIO_ReadCounter( &local->USB_Handle, &covalue, &overflow);
    if ( op->Status)
      op->Super.ErrorCount++;
    else {
      if ( !overflow) {
	*(pwr_tUInt32 *)cp->chanlist[18].vbp = covalue;
      }
      else {
	// Reset counter
	op->Status = USBIO_ConfigCounter( &local->USB_Handle, 2);
	*(pwr_tUInt32 *)cp->chanlist[18].vbp = 0;
      }
    }
  }

  if ( op->Super.ErrorCount >= op->Super.ErrorSoftLimit && 
       error_count < op->Super.ErrorSoftLimit) {
    errh_Warning( "IO Card ErrorSoftLimit reached, '%s'", cp->Name);
    //    if ( op->Reconnect)
    //      local->Disconnected = 1;
  }
  if ( op->Super.ErrorCount >= op->Super.ErrorHardLimit) {
    errh_Error( "IO Card ErrorHardLimit reached '%s', IO stopped", cp->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }    

  return IO__SUCCESS;
}

static pwr_tStatus IoCardWrite( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack	*rp,
			       io_sCard	*cp)
{
  io_sLocal *local = cp->Local;
  pwr_sClass_MotionControl_USBIO *op = (pwr_sClass_MotionControl_USBIO *)cp->op;
  int value = 0;
  float fvalue;
  int i;
  unsigned int m;
  pwr_tUInt32 error_count = op->Super.ErrorCount;

  if ( local->Disconnected) {
    if ( !usbio_reconnect( ctx, ap, rp, cp))
      return IO__SUCCESS;
  }

  if ( local->portA_hasDo) {
    m = 1;
    value = 0;
    for ( i = 0; i < 8; i++) {
      if ( m & local->portA_doMask) {
	if ( *(pwr_tBoolean *)cp->chanlist[i].vbp)
	  value |= m;
      }
      m = m << 1;
    }
    op->Status = USBIO_WriteDO( &local->USB_Handle, 1, value);
    if ( op->Status) op->Super.ErrorCount++;
  }

  if ( local->portB_hasDo) {
    m = 1;
    value = 0;
    for ( i = 0; i < 8; i++) {
      if ( m & local->portB_doMask) {
	if (*(pwr_tBoolean *)cp->chanlist[i+8].vbp) 
	  value |= m;
      }
      m = m << 1;
    }
    op->Status = USBIO_WriteDO( &local->USB_Handle, 2, value);
    if ( op->Status) op->Super.ErrorCount++;
  }

  if ( local->portC_hasDo) {
    m = 1;
    value = 0;
    for ( i = 0; i < 5; i++) {
      if ( m & local->portC_doMask) {
	if (*(pwr_tBoolean *)cp->chanlist[i+16].vbp) 
	  value |= m;
      }
      m = m << 1;
    }
    op->Status = USBIO_WriteDO( &local->USB_Handle, 3, value);
    if ( op->Status) op->Super.ErrorCount++;
  }

  if ( local->portC_hasAo) {
    pwr_sClass_ChanAo *cop;
    if ( local->portC_aoMask & 8) {      
      cop = (pwr_sClass_ChanAo *)cp->chanlist[19].cop;
      if ( cop->CalculateNewCoef)
	// Request to calculate new coefficients
	io_AoRangeToCoef( &cp->chanlist[19]);

      fvalue = *(pwr_tFloat32 *)cp->chanlist[19].vbp * cop->OutPolyCoef1 + cop->OutPolyCoef0;
      if ( fvalue < 0)
	fvalue = 0;
      else if (fvalue > 5)
	fvalue = 5;
      op->Status = USBIO_WriteAO( &local->USB_Handle, 1, fvalue);
      if ( op->Status) op->Super.ErrorCount++;
    }
    if ( local->portC_aoMask & 16) {
      cop = (pwr_sClass_ChanAo *)cp->chanlist[20].cop;
      if ( cop->CalculateNewCoef)
	// Request to calculate new coefficients
	io_AoRangeToCoef( &cp->chanlist[20]);

      fvalue = *(pwr_tFloat32 *)cp->chanlist[20].vbp * cop->OutPolyCoef1 + cop->OutPolyCoef0;
      if ( fvalue < 0)
	fvalue = 0;
      else if (fvalue > 5)
	fvalue = 5;
      op->Status = USBIO_WriteAO( &local->USB_Handle, 2, fvalue);
      if ( op->Status) op->Super.ErrorCount++;
    }
  }

  if ( op->Super.ErrorCount >= op->Super.ErrorSoftLimit && 
       error_count < op->Super.ErrorSoftLimit) {
    errh_Warning( "IO Card ErrorSoftLimit reached, '%s'", cp->Name);
    //    if ( op->Reconnect)
    //      local->Disconnected = 1;
  }    
  if ( op->Super.ErrorCount >= op->Super.ErrorHardLimit) {
    errh_Error( "IO Card ErrorHardLimit reached '%s', IO stopped", cp->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }    

  return IO__SUCCESS;
}

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(MotionControl_USBIO) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
