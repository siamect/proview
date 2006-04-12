/* 
 * Proview   $Id: rt_io_m_pb_module.c,v 1.3 2006-04-12 12:16:59 claes Exp $
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
#include "pwr_profibusclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "rt_io_profiboard.h"
#include "rt_pb_msg.h"

/*----------------------------------------------------------------------------*\
  Convert ai from raw float value to signal value and actual value
\*----------------------------------------------------------------------------*/
void PbConvertAi ( io_tCtx ctx,
                 pwr_tFloat32 f_raw,
		 pwr_sClass_ChanAi *chan_ai,
		 pwr_sClass_Ai *sig_ai,
		 io_sChannel *chanp)
{
  pwr_tFloat32		sigvalue;
  pwr_tFloat32		actvalue;
  pwr_tFloat32		*polycoef_p;
  int			i;

  sigvalue = chan_ai->SigValPolyCoef0 + chan_ai->SigValPolyCoef1 * f_raw;

  switch (chan_ai->SensorPolyType)
  {
    case 0:
      actvalue = sigvalue;
      break;
    case 1:
      actvalue = chan_ai->SensorPolyCoef0 + chan_ai->SensorPolyCoef1 * f_raw;
      break;
    case 2:
      polycoef_p = &chan_ai->SensorPolyCoef2;
      actvalue = 0;
      for ( i = 0; i < 3; i++)
      {
        actvalue = sigvalue * actvalue + *polycoef_p;
        polycoef_p--;
      }
      break;
    case 3:
      actvalue = chan_ai->SensorPolyCoef0 + chan_ai->SensorPolyCoef1 * sigvalue;
      if ( actvalue >= 0)
        actvalue = chan_ai->SensorPolyCoef2 * sqrt(actvalue);
      else
        actvalue = 0;
      break;      
    case 4:
      actvalue = chan_ai->SensorPolyCoef0 + chan_ai->SensorPolyCoef1 * sigvalue;
      if ( actvalue >= 0)
        actvalue = chan_ai->SensorPolyCoef2 * sqrt(actvalue);
      else
        actvalue = -chan_ai->SensorPolyCoef2 * sqrt(-actvalue);
      break;      
  }

  if (sig_ai->FilterType == 1 && sig_ai->FilterAttribute[0] > 0 && sig_ai->FilterAttribute[0] > ctx->ScanTime) {
    actvalue = *(sig_ai->ActualValue) + ctx->ScanTime / sig_ai->FilterAttribute[0] * (actvalue - *(sig_ai->ActualValue));
  }

  sig_ai->SigValue = sigvalue;
  *(pwr_tFloat32 *) chanp->vbp = actvalue;
  
  return;
}

/*----------------------------------------------------------------------------*\
   Init method for the Pb module
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Module *op;
  int i;

  op = (pwr_sClass_Pb_Module *) cp->op;
  local = (io_sCardLocal *) cp->Local;
  
  for (i=0; i<IO_MAXCHAN; i++) {
    local->scancount[i] = 0;
  }

  op->Status = PB__NORMAL;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb module
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Module *op;
  io_sChannel *chanp;
  pwr_sClass_ChanDi *chan_di;
  pwr_sClass_Di *sig_di;
  pwr_sClass_ChanAi *chan_ai;
  pwr_sClass_Ai *sig_ai;
//  pwr_sClass_ChanAit *chan_ait;
  pwr_sClass_ChanIi *chan_ii;
  pwr_sClass_Ii *sig_ii;
  pwr_sClass_Pb_DP_Slave *slave;
  pwr_tUInt8 udata8 = 0;
  pwr_tUInt16 udata16 = 0;
  pwr_tUInt32 udata32 = 0;
  pwr_tInt8 data8 = 0;
  pwr_tInt16 data16 = 0;
  pwr_tInt32 data32 = 0;
  pwr_tFloat32 f_raw = 0.0;
  int i;

  op = (pwr_sClass_Pb_Module *) cp->op;
  local = (io_sCardLocal *) cp->Local;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;

  op->Status = slave->Status;  

  if (op->Status == PB__NORMAL) { 
  
    for (i=0; i<cp->ChanListSize; i++) {
      chanp = &cp->chanlist[i];
      switch (chanp->ChanClass) {
      
	// Channel type is Di (digital input)
	
        case pwr_cClass_ChanDi:
	  chan_di = (pwr_sClass_ChanDi *) chanp->cop;
	  sig_di = (pwr_sClass_Di *) chanp->sop;
	  if (chan_di && sig_di && chan_di->ConversionOn) {
	  
	    switch (chan_di->Representation) {

              case pwr_eDataRepEnum_Bit8:
	        memcpy(&udata8, local->input_area + cp->offset + chanp->offset, 1);
		* (pwr_tUInt16 *) (chanp->vbp) = ((udata8 & chanp->mask) != 0);
	        break;

              case pwr_eDataRepEnum_Bit16:
	        memcpy(&udata16, local->input_area + cp->offset + chanp->offset, 2);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata16 = swap16(udata16);
		* (pwr_tUInt16 *) (chanp->vbp) = ((udata16 & chanp->mask) != 0);
	        break;

              case pwr_eDataRepEnum_Bit32:
	        memcpy(&udata32, local->input_area + cp->offset + chanp->offset, 4);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata32 = swap32(udata32);
		* (pwr_tUInt16 *) (chanp->vbp) = ((udata32 & chanp->mask) != 0);
	        break;

	    }
	    
	    // Invert ?
	    if (chan_di->InvertOn) * (pwr_tUInt16 *) (chanp->vbp) ^= 1;
	    
	  }
	  break;

	// Channel type is Ai (analog input)

        case pwr_cClass_ChanAi:
	  chan_ai = (pwr_sClass_ChanAi *) chanp->cop;
	  sig_ai = (pwr_sClass_Ai *) chanp->sop;
	  if (chan_ai && sig_ai && chan_ai->ConversionOn) {

            if (chan_ai->CalculateNewCoef) io_AiRangeToCoef(chanp);

	    switch (chan_ai->Representation) {

              case pwr_eDataRepEnum_Int8:
	        memcpy(&data8, local->input_area + cp->offset + chanp->offset, 1);
		f_raw = (float) data8;
	        break;

              case pwr_eDataRepEnum_UInt8:
	        memcpy(&udata8, local->input_area + cp->offset + chanp->offset, 1);
		f_raw = (float) udata8;
	        break;
		
              case pwr_eDataRepEnum_Int16:
	        memcpy(&data16, local->input_area + cp->offset + chanp->offset, 2);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data16 = swap16(data16);
		f_raw = (float) data16;
	        break;
		
              case pwr_eDataRepEnum_UInt16:
	        memcpy(&udata16, local->input_area + cp->offset + chanp->offset, 2);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data16 = swap16(udata16);
		f_raw = (float) udata16;
	        break;

              case pwr_eDataRepEnum_Int32:
	        memcpy(&data32, local->input_area + cp->offset + chanp->offset, 4);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data32 = swap32(data32);
		f_raw = (float) data32;
	        break;
		
              case pwr_eDataRepEnum_UInt32:
	        memcpy(&udata32, local->input_area + cp->offset + chanp->offset, 4);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata32 = swap32(udata32);
		f_raw = (float) udata32;
	        break;
		
              case pwr_eDataRepEnum_Float32:
	        memcpy(&udata32, local->input_area + cp->offset + chanp->offset, 4);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian ||
		    slave->FloatRepresentation == pwr_ePbNumberRep_FloatIEEE) udata32 = swap32(udata32);
	        memcpy(&f_raw, &udata32, 4);
	        break;
		
            }
	    
	    sig_ai->RawValue = 0;
	    PbConvertAi(ctx, f_raw, chan_ai, sig_ai, chanp);

	  }
	  break;

	// Channel type is Ait (analog input with table conversion)

        case pwr_cClass_ChanAit:
	  break;
	  
	// Channel type is Ii (integer input)
	
        case pwr_cClass_ChanIi:
	  chan_ii = (pwr_sClass_ChanIi *) chanp->cop;
	  sig_ii = (pwr_sClass_Ii *) chanp->sop;
	  if (chan_ii && sig_ii /* && chan_ii->ConversionOn */) {
	    switch (chan_ii->Representation) {

              case pwr_eDataRepEnum_Int8:
	        memcpy(&data8, local->input_area + cp->offset + chanp->offset, 1);
            	*(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) data8;
	        break;
		
              case pwr_eDataRepEnum_UInt8:
	        memcpy(&udata8, local->input_area + cp->offset + chanp->offset, 1);
            	*(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) udata8;
	        break;

              case pwr_eDataRepEnum_Int16:
	        memcpy(&data16, local->input_area + cp->offset + chanp->offset, 2);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data16 = swap16(data16);
            	*(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) data16;
	        break;
		
              case pwr_eDataRepEnum_UInt16:
	        memcpy(&udata16, local->input_area + cp->offset + chanp->offset, 2);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata16 = swap16(udata16);
            	*(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) udata16;
	        break;

              case pwr_eDataRepEnum_Int32:
	        memcpy(&data32, local->input_area + cp->offset + chanp->offset, 4);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data32 = swap32(data32);
            	*(pwr_tInt32 *) chanp->vbp = data32;
	        break;
		
              case pwr_eDataRepEnum_UInt32:
	        memcpy(&udata32, local->input_area + cp->offset + chanp->offset, 4);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata32 = swap32(udata32);
            	*(pwr_tInt32 *) chanp->vbp = (pwr_tInt32) udata32;
	        break;
				
            }
	  }
	  break;
      }
    }
    
  }
//  printf("Method Pb_Module-IoCardRead\n");
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb module
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Module *op;
  io_sChannel *chanp;
  
  pwr_sClass_ChanDo *chan_do;
  pwr_sClass_Do *sig_do;
  pwr_sClass_ChanAo *chan_ao;
  pwr_sClass_Ao *sig_ao;
  pwr_sClass_ChanIo *chan_io;
  pwr_sClass_Io *sig_io;

  pwr_sClass_Pb_DP_Slave *slave;
  pwr_tUInt8 udata8 = 0;
  pwr_tUInt8 *udata8p;
  pwr_tUInt16 udata16 = 0;
  pwr_tUInt32 udata32 = 0;
  pwr_tInt8 data8 = 0;
  pwr_tInt16 data16 = 0;
  pwr_tInt32 data32 = 0;
  pwr_tFloat32 value, rawvalue;
  int fixout;
  int i;

  op = (pwr_sClass_Pb_Module *) cp->op;
  local = (io_sCardLocal *) cp->Local;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;
  
  op->Status = slave->Status;  

  if (op->Status == PB__NORMAL) { 

    fixout = ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT;

    for (i=0; i<cp->ChanListSize; i++) {
      chanp = &cp->chanlist[i];
      switch (chanp->ChanClass) {
      
        case pwr_cClass_ChanDo:
	  chan_do = (pwr_sClass_ChanDo *) chanp->cop;
	  sig_do = (pwr_sClass_Do *) chanp->sop;
	  if (chan_do && sig_do) {

	    if (fixout) {}

	    switch (chan_do->Representation) {

              case pwr_eDataRepEnum_Bit8:
	        udata8p = local->output_area + cp->offset + chanp->offset;
		if (*(pwr_tInt32 *) chanp->vbp != 0)
		  *udata8p |= chanp->mask;
		else
		  *udata8p &= ~chanp->mask;
	        break;
/*
              case pwr_eDataRepEnum_Bit16:
	        memcpy(&udata16, local->input_area + cp->offset + chanp->offset, 2);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata16 = swap16(udata16);
		* (pwr_tUInt16 *) (chanp->vbp) = ((udata16 & chanp->mask) != 0);
	        break;

              case pwr_eDataRepEnum_Bit32:
	        memcpy(&udata32, local->input_area + cp->offset + chanp->offset, 4);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata32 = swap32(udata32);
		* (pwr_tUInt16 *) (chanp->vbp) = ((udata32 & chanp->mask) != 0);
	        break;
*/
	    }
	    
	  }
	  
	  break;

	// Channel type is Ao (analog output)

        case pwr_cClass_ChanAo:
	  chan_ao = (pwr_sClass_ChanAo *) chanp->cop;
	  sig_ao = (pwr_sClass_Ao *) chanp->sop;
	  if (chan_ao && sig_ao) {

            if (fixout)
              value = chan_ao->FixedOutValue;
            else if (chan_ao->TestOn)
              value = chan_ao->TestValue;
            else
	      value = *(pwr_tFloat32 *) chanp->vbp;

            if (chan_ao->CalculateNewCoef) io_AoRangeToCoef(chanp);

            if (value > chan_ao->ActValRangeHigh)
              value = chan_ao->ActValRangeHigh;
            else if ( value < chan_ao->ActValRangeLow)
              value = chan_ao->ActValRangeLow;

            rawvalue = chan_ao->OutPolyCoef1 * value + chan_ao->OutPolyCoef0;
            if ( rawvalue > 0)
              rawvalue = rawvalue + 0.5;
            else
              rawvalue = rawvalue - 0.5;

            sig_ao->RawValue = 0;

            sig_ao->SigValue = chan_ao->SigValPolyCoef1 * value + chan_ao->SigValPolyCoef0;
	    
	    switch (chan_ao->Representation) {

              case pwr_eDataRepEnum_Int8:
	        data8 = (pwr_tInt8) rawvalue;
	        memcpy(local->output_area + cp->offset + chanp->offset, &data8, 1);
	        break;

              case pwr_eDataRepEnum_UInt8:
	        udata8 = (pwr_tUInt8) rawvalue;
	        memcpy(local->output_area + cp->offset + chanp->offset, &udata8, 1);
	        break;
		
              case pwr_eDataRepEnum_Int16:
	        data16 = (pwr_tInt16) rawvalue;
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data16 = swap16(data16);
	        memcpy(local->output_area + cp->offset + chanp->offset, &data16, 2);
	        break;
		
              case pwr_eDataRepEnum_UInt16:
	        udata16 = (pwr_tUInt16) rawvalue;
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata16 = swap16(udata16);
	        memcpy(local->output_area + cp->offset + chanp->offset, &udata16, 2);
	        break;

              case pwr_eDataRepEnum_Int32:
	        data32 = (pwr_tInt32) rawvalue;
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data32 = swap32(data32);
	        memcpy(local->output_area + cp->offset + chanp->offset, &data32, 4);
	        break;
		
              case pwr_eDataRepEnum_UInt32:
	        udata32 = (pwr_tUInt32) rawvalue;
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata32 = swap32(udata32);
	        memcpy(local->output_area + cp->offset + chanp->offset, &udata32, 4);
	        break;
		
              case pwr_eDataRepEnum_Float32:
	        memcpy(&udata32, &rawvalue, 4);
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian ||
		    slave->FloatRepresentation == pwr_ePbNumberRep_FloatIEEE) udata32 = swap32(udata32);
	        memcpy(local->output_area + cp->offset + chanp->offset, &udata32, 4);
	        break;
		
            }
	    
	  }
	  break;
	  
	// Channel type is Io (integer output)
	
        case pwr_cClass_ChanIo:
	  chan_io = (pwr_sClass_ChanIo *) chanp->cop;
	  sig_io = (pwr_sClass_Io *) chanp->sop;
	  if (chan_io && sig_io) {
	  
            if (fixout)
              data32 = (pwr_tInt32) chan_io->FixedOutValue;
            else if (chan_io->TestOn)
              data32 = (pwr_tInt32) chan_io->TestValue;
            else
	      data32 = *(pwr_tInt32 *) chanp->vbp;

	    switch (chan_io->Representation) {

              case pwr_eDataRepEnum_Int8:
	        data8 = (pwr_tInt8) data32;
	        memcpy(local->output_area + cp->offset + chanp->offset, &data8, 1);
	        break;
		
              case pwr_eDataRepEnum_UInt8:
	        udata8 = (pwr_tUInt8) data32;
	        memcpy(local->output_area + cp->offset + chanp->offset, &udata8, 1);
	        break;

              case pwr_eDataRepEnum_Int16:
	        data16 = (pwr_tInt16) data32;
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data16 = swap16(data16);
	        memcpy(local->output_area + cp->offset + chanp->offset, &data16, 2);
	        break;
		
              case pwr_eDataRepEnum_UInt16:
	        udata16 = (pwr_tUInt16) data32;
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata16 = swap16(udata16);
	        memcpy(local->output_area + cp->offset + chanp->offset, &udata16, 2);
	        break;

              case pwr_eDataRepEnum_Int32:
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) data32 = swap32(data32);
	        memcpy(local->output_area + cp->offset + chanp->offset, &data32, 4);
	        break;
		
              case pwr_eDataRepEnum_UInt32:
	        udata32 = (pwr_tUInt32) data32;
		if (slave->ByteOrdering == pwr_eByteOrdering_BigEndian) udata32 = swap32(udata32);
	        memcpy(local->output_area + cp->offset + chanp->offset, &udata32, 4);
	        break;
				
            }
	  }
	  break;
      }
    }
  }
//  printf("Method Pb_Module-IoCardWrite\n");
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Close method for the Pb module
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
  
  printf("Method Pb_Module-IoCardClose\n");
  return IO__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_Module) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardWrite),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};

