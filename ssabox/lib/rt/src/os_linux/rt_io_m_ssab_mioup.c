/* 
 * Proview   $Id$
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

/* rt_io_m_ssab_mioup.c -- io methods for ssab cards. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#include "pwr.h"
#include "co_time.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "qbus_io.h"
#include "rt_io_m_ssab_locals.h"
#include "rt_io_bfbeth.h"
#include "rt_io_card_write.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

#define IO_MAXCHAN 128
#define ADDR_DO_OFFSET 16
#define ADDR_AI_OFFSET 48
#define ADDR_AO_OFFSET 32

typedef struct {
	unsigned int	Address;
	int		Qbus_fp;
	int		ScanCount[IO_MAXCHAN];
	unsigned int	bfb_item;
	pwr_tFloat32	OldValue[IO_MAXCHAN];
	pwr_tBoolean	OldTestOn[IO_MAXCHAN];
	int		WriteFirst;
	pwr_tTime       ErrTime;
} io_sLocal;

static pwr_tStatus AoRangeToCoef( 
  io_sChannel 	*chanp)
{
  pwr_sClass_ChanAo	*cop;
  char			buf[120];
  pwr_tStatus		sts;
  pwr_tFloat32		PolyCoef1;
  pwr_tFloat32		PolyCoef0;

  cop = chanp->cop;

  if ( cop)
  {
    cop->CalculateNewCoef = 0;

    /* Coef for ActualValue to RawValue conversion */
    if ( cop->ActValRangeHigh != cop->ActValRangeLow)
    {
      cop->SigValPolyCoef1 = (cop->SensorSigValRangeHigh - cop->SensorSigValRangeLow)/
		(cop->ActValRangeHigh - cop->ActValRangeLow);
      cop->SigValPolyCoef0 = cop->SensorSigValRangeHigh - cop->ActValRangeHigh *
		cop->SigValPolyCoef1;
    }
    else
    {
      sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      errh_Error( "Invalid ActValueRange in Ao channel %s", buf);
      return IO__CHANRANGE;
    }
    /* Coef for ActualValue to SignalValue conversion */
    if ( cop->ChannelSigValRangeHigh != 0)
    {
      PolyCoef0 = 0;
      PolyCoef1 = cop->RawValRangeHigh / cop->ChannelSigValRangeHigh;
      cop->OutPolyCoef1 = cop->SigValPolyCoef1 * PolyCoef1;
      cop->OutPolyCoef0 = PolyCoef0 + PolyCoef1*
		cop->SigValPolyCoef0;
    }
    else
    {
      sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
      errh_Error( "Invalid SigValueRange in Ao channel %s", buf);
      return IO__CHANRANGE;
    }
  }
  return IO__SUCCESS;
}

void io_DoPackWord_Mio( 
  io_sCard	*cp,
  pwr_tUInt16	*data)
{
  io_sChannel 	*chanp;

    chanp = &cp->chanlist[4];

  *data = 0;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 1;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 2;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 4;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 8;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 16;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 32;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 64;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 128;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 256;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 512;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 1024;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 2048;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 4096;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 8192;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 16384;
  chanp++;
  if ( chanp->cop && chanp->sop && * (pwr_tUInt16 *) (chanp->vbp))
    *data |= 32768;
  chanp++;
}

static pwr_tStatus AiRangeToCoef( 
  io_sChannel 		*chanp)
{
  pwr_sClass_ChanAi	*cop;
  char			buf[120];
  pwr_tStatus		sts;
  pwr_tFloat32		PolyCoef1;
  pwr_tFloat32		PolyCoef0;

  cop = chanp->cop;

  if ( cop)
  {
    cop->CalculateNewCoef = 0;

    /* Coef for RawValue to SignalValue conversion */
    cop->SigValPolyCoef0 = 0;
    cop->SigValPolyCoef1 = cop->ChannelSigValRangeHigh / 30000;

    /* Coef for SignalValue to ActualValue conversion */
    if ( chanp->ChanClass != pwr_cClass_ChanAit && cop->SensorPolyType == 1)
    {
      if ( cop->SensorSigValRangeHigh != cop->SensorSigValRangeLow)
      {
        PolyCoef1 = (cop->ActValRangeHigh - cop->ActValRangeLow)/
		(cop->SensorSigValRangeHigh - cop->SensorSigValRangeLow);
        PolyCoef0 = cop->ActValRangeHigh - cop->SensorSigValRangeHigh *
		PolyCoef1;
        cop->SensorPolyCoef1 = cop->SigValPolyCoef1 * PolyCoef1;
        cop->SensorPolyCoef0 = PolyCoef0 + PolyCoef1*
		cop->SigValPolyCoef0;
      }
      else
      {
        sts = gdh_AttrrefToName( &chanp->ChanAref, buf, sizeof(buf), 
			cdh_mName_volumeStrict);
        if ( EVEN(sts)) return sts;
        errh_Error( "Invalid SigValueRange in Ai channel %s", buf);
        return IO__CHANRANGE;
      }
    }
  }
  return IO__SUCCESS;
}

static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  pwr_sClass_Ssab_BaseMCard *op;
  io_sLocal 		*local;
  int			i;
  io_sChannel		*chanp;

  op = (pwr_sClass_Ssab_BaseMCard *) cp->op;
  local = calloc( 1, sizeof(*local));
  cp->Local = local;
  local->Address = op->RegAddress;
  local->Qbus_fp = ((io_sRackLocal *)(rp->Local))->Qbus_fp;

  errh_Info( "Init of Multi IO card '%s'", cp->Name);

  /* Write the first 50 loops */
  local->WriteFirst = 50;

  /* Calculate polycoeff for Ai */
  chanp = cp->chanlist;
  for  ( i = 0; i < op->MaxNoOfAiChannels; i++)
  {
    if ( chanp->sop)
      AiRangeToCoef( chanp);
    chanp++;
  }

  /* Caluclate polycoeff for Ao */
  chanp = cp->chanlist + op->MaxNoOfAiChannels;
  for  ( i = 0; i < op->MaxNoOfAoChannels; i++)
  {
    if ( chanp->sop)
      AoRangeToCoef( chanp);
    chanp++;
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
  io_sLocal 		*local;

  errh_Info( "IO closing ai card '%s'", cp->Name);

  local = (io_sLocal *) cp->Local;
  free( (char *) local);

  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  io_sLocal 		*local;
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);
  pwr_tInt16		data = 0;
  pwr_sClass_Ssab_BaseMCard *op;
  pwr_sClass_Ssab_RemoteRack	*rrp;
  int			i;
  pwr_tFloat32		actvalue;
  io_sChannel		*chanp;
  pwr_sClass_ChanAi	*cop;
  pwr_sClass_Ai		*sop;
  int			sts;
  qbus_io_read 		rb;
  qbus_io_write		writeb;
  int			bfb_error = 0;
  pwr_tTime             now;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Ssab_BaseMCard *) cp->op;
  
  writeb.Address = local->Address;
  writeb.Data = 0;
  sts = write(local->Qbus_fp, &writeb, sizeof(writeb));
  writeb.Address = local->Address + 2;
  writeb.Data = 0x0001;
  sts = write( (int) local->Qbus_fp, &writeb, sizeof(writeb));

  chanp = &cp->chanlist[0];
  for ( i = 0; i < op->MaxNoOfAiChannels; i++)
  { 
    if ( !chanp->cop || !chanp->sop)
    {
      chanp++;
      continue;
    }
    cop = (pwr_sClass_ChanAi *) chanp->cop;
    sop = (pwr_sClass_Ai *) chanp->sop;

    if ( cop->CalculateNewCoef)
      AiRangeToCoef( chanp);

    if ( cop->ConversionOn)
    {
      if ( local->ScanCount[i] <= 1)
      {

        if (r_local->Qbus_fp != 0 && r_local->s == 0) {
          writeb.Address = local->Address;
          writeb.Data = ADDR_AI_OFFSET + 16*i;
          sts = write(local->Qbus_fp, &writeb, sizeof(writeb));
          
          if(sts != -1)
          {
              rb.Address = local->Address + 2;
              sts = read( local->Qbus_fp, &rb, sizeof(rb));
              data = (pwr_tInt16) rb.Data;
          }
          
	}
        else {
          /* Ethernet I/O, Get data from current address */
            bfbeth_set_write_req(r_local, (pwr_tUInt16) (local->Address), (pwr_tUInt16) (ADDR_AI_OFFSET + 16*i));
            
            data = bfbeth_get_data(r_local, (pwr_tUInt16) (local->Address + 2), &sts);
          /* Yes, we want to read this address the next time aswell */
          bfbeth_set_read_req(r_local, (pwr_tUInt16) (local->Address + 2));	 

          if (sts == -1) {
	    /* Error handling for ethernet Qbus-I/O */
  	    rrp = (pwr_sClass_Ssab_RemoteRack *) rp->op;
	    if (bfb_error == 0) {
              op->ErrorCount++;
	      bfb_error = 1;
              if ( op->ErrorCount == op->ErrorSoftLimit) {
                errh_Error( "IO Error soft limit reached on card '%s'", cp->Name);
		ctx->IOHandler->CardErrorSoftLimit = 1;
		ctx->IOHandler->ErrorSoftLimitObject = cdh_ObjidToAref( cp->Objid);
	      }
              if ( op->ErrorCount == op->ErrorHardLimit) {
                errh_Error( "IO Error hard limit reached on card '%s', stall action %d", cp->Name, rrp->StallAction);
		ctx->IOHandler->CardErrorHardLimit = 1;
		ctx->IOHandler->ErrorHardLimitObject = cdh_ObjidToAref( cp->Objid);
	      }
              if ( op->ErrorCount >= op->ErrorHardLimit && rrp->StallAction == pwr_eSsabStallAction_ResetInputs )
	      {
	        data = 0;
	        sts = 1;
              }
              if ( op->ErrorCount >= op->ErrorHardLimit && rrp->StallAction == pwr_eSsabStallAction_EmergencyBreak )
	      {
                ctx->Node->EmergBreakTrue = 1;
                return IO__ERRDEVICE;
              }
	    }
	    if (sts == -1) {
	      chanp++;
	      continue;
	    }
          }
          else {
	    op->ErrorCount = 0;
          }
        }
	
        if ( sts == -1)
	/* Error handling for local Qbus-I/O */
        {
          /* Increase error count and check error limits */
          time_GetTime( &now);

          if (op->ErrorCount > op->ErrorSoftLimit) {
            /* Ignore if some time has expired */
            if (now.tv_sec - local->ErrTime.tv_sec < 600)
              op->ErrorCount++;
          }
          else
            op->ErrorCount++;
          local->ErrTime = now;

          if ( op->ErrorCount == op->ErrorSoftLimit) {
            errh_Error( "IO Error soft limit reached on card '%s'", cp->Name);
	    ctx->IOHandler->CardErrorSoftLimit = 1;
	    ctx->IOHandler->ErrorSoftLimitObject = cdh_ObjidToAref( cp->Objid);
	  }
          if ( op->ErrorCount >= op->ErrorHardLimit)
          {
            errh_Error( "IO Error hard limit reached on card '%s', IO stopped", cp->Name);
            ctx->Node->EmergBreakTrue = 1;
	    ctx->IOHandler->CardErrorHardLimit = 1;
	    ctx->IOHandler->ErrorHardLimitObject = cdh_ObjidToAref( cp->Objid);
            return IO__ERRDEVICE;
          }
	  chanp++;
          continue;
        }
	
        /* Convert rawvalue to sigvalue and actualvalue */
	sop->RawValue = data;
        sop->SigValue = data * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
	switch ( chanp->ChanClass)
        {
          case pwr_cClass_ChanAi:
	    io_ConvertAi( cop, data, &actvalue);
	    break;
          case pwr_cClass_ChanAit:
	    io_ConvertAit( (pwr_sClass_ChanAit *) cop, data, &actvalue);
	    break;
	}

        /* Filter */
	if ( sop->FilterType == 1 &&
	   sop->FilterAttribute[0] > 0 &&
	   sop->FilterAttribute[0] > ctx->ScanTime)
        {
	  actvalue = *(pwr_tFloat32 *)chanp->vbp +
	  ctx->ScanTime / sop->FilterAttribute[0] *
	  (actvalue - *(pwr_tFloat32 *)chanp->vbp);
        }

	*(pwr_tFloat32 *) chanp->vbp = actvalue;
        local->ScanCount[i] = cop->ScanInterval + 1;
	  
      }
      local->ScanCount[i]--;
    }
    chanp++;
  }
  return 1;
}

static pwr_tStatus IoCardWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  io_sLocal 		*local;
  io_sRackLocal		*r_local = (io_sRackLocal *)(rp->Local);
  pwr_sClass_Ssab_BaseMCard	*op;
  int			i;
  io_sChannel		*chanp;
  pwr_sClass_ChanAo	*cop;
  pwr_sClass_Ao		*sop;
  pwr_tFloat32		value;
  int			fixout;
  pwr_tUInt16		data = 0;
  pwr_tFloat32		rawvalue;
  qbus_io_write		writeb;
  int			sts;
  pwr_tTime             now;
  pwr_tUInt16		invmask;
  pwr_tUInt16		testmask;
  pwr_tUInt16		testvalue;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Ssab_BaseMCard *) cp->op;
  
  fixout = ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT;

  chanp = &cp->chanlist[op->MaxNoOfAiChannels];
  for ( i = 0; i < op->MaxNoOfAoChannels; i++)
  { 
    if ( !chanp->cop || !chanp->sop)
    {
      chanp++;
      continue;
    }
    cop = (pwr_sClass_ChanAo *) chanp->cop;
    sop = (pwr_sClass_Ao *) chanp->sop;

    if ( *(pwr_tFloat32 *)chanp->vbp != local->OldValue[i] ||
         local->WriteFirst > 0 ||
	 cop->CalculateNewCoef ||
         fixout ||
         cop->TestOn || local->OldTestOn[i] != cop->TestOn)
    {
      if ( fixout)
        value = cop->FixedOutValue;
      else if ( cop->TestOn)
        value = cop->TestValue;
      else
	value = *(pwr_tFloat32 *) chanp->vbp;

      if ( cop->CalculateNewCoef)
        AoRangeToCoef( chanp);


      /* Convert to rawvalue */
      if ( value > cop->ActValRangeHigh)
        value = cop->ActValRangeHigh;
      else if ( value < cop->ActValRangeLow)
        value = cop->ActValRangeLow;

      rawvalue = cop->OutPolyCoef1 * value + cop->OutPolyCoef0;
      if ( rawvalue > 0)
        sop->RawValue = rawvalue + 0.5;
      else
        sop->RawValue = rawvalue - 0.5;
      data = sop->RawValue;

      if (r_local->Qbus_fp != 0 && r_local->s == 0) {
        writeb.Address = local->Address;
        writeb.Data = ADDR_AO_OFFSET + 16*i;
        sts = write( local->Qbus_fp, &writeb, sizeof(writeb));
        
        if(sts != -1)
        {
            writeb.Address = local->Address + 2;
            writeb.Data = data;
            sts = write( local->Qbus_fp, &writeb, sizeof(writeb));
        }
        
      }
      else {
      	/* Ethernet I/O, Request a write to current address */
      	bfbeth_set_write_req(r_local, (pwr_tUInt16) (local->Address), (pwr_tUInt16) (ADDR_AO_OFFSET + 16*i));
          
        bfbeth_set_write_req(r_local, (pwr_tUInt16) (local->Address + 2), data);
      	sts = 1;      
      }
      
      if ( sts == -1)
      {
        /* Exceptionhandler was called */
        /* Increase error count and check error limits */
        time_GetTime( &now);

        if (op->ErrorCount > op->ErrorSoftLimit) {
          /* Ignore if some time has expired */
          if (now.tv_sec - local->ErrTime.tv_sec < 600)
            op->ErrorCount++;
        }
        else
          op->ErrorCount++;
        local->ErrTime = now;

        if ( op->ErrorCount == op->ErrorSoftLimit) {
          errh_Error( "IO Error soft limit reached on card '%s'", cp->Name);
	  ctx->IOHandler->CardErrorSoftLimit = 1;
	  ctx->IOHandler->ErrorSoftLimitObject = cdh_ObjidToAref( cp->Objid);
	}
        if ( op->ErrorCount >= op->ErrorHardLimit)
        {
          errh_Error( "IO Error hard limit reached on card '%s', IO stopped", cp->Name);
          ctx->Node->EmergBreakTrue = 1;
	  ctx->IOHandler->CardErrorHardLimit = 1;
	  ctx->IOHandler->ErrorHardLimitObject = cdh_ObjidToAref( cp->Objid);
          return IO__ERRDEVICE;
        }
	chanp++;
        continue;
      }
      else
        local->OldValue[i] = value;
    }
    local->OldTestOn[i] = cop->TestOn;
    chanp++;
  }
  
  
  if ( local->WriteFirst)
    local->WriteFirst--;
  

  if ( ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT)
    data = op->DoFixedOutValue;
  else
    io_DoPackWord_Mio( cp, &data);

  testmask = op->DoTestMask;
  invmask = op->DoInvMask;

  /* Invert */
  data = data ^ invmask;


  /* Testvalues */
  if ( testmask) {
    testvalue = op->DoTestValue;
    data = (data & ~ testmask) | (testmask & testvalue);
  }

  if (r_local->Qbus_fp != 0 && r_local->s == 0) {
    /* Write to local Q-bus */
    writeb.Address = local->Address;
    writeb.Data = ADDR_DO_OFFSET;
    
    sts = write( local->Qbus_fp, &writeb, sizeof(writeb));
    
    writeb.Data = data;
    writeb.Address = local->Address + 2;
    sts = write( local->Qbus_fp, &writeb, sizeof(writeb));
  }
  else {
    /* Ethernet I/O, Request a write to current address */
      bfbeth_set_write_req(r_local, (pwr_tUInt16) local->Address, (pwr_tUInt16) (ADDR_DO_OFFSET));
      
      bfbeth_set_write_req(r_local, (pwr_tUInt16) local->Address + 2, data);
    sts = 1;      
  }
    
    
  if ( sts == -1) {
    /* Increase error count and check error limits */
    time_GetTime( &now);

    if (op->ErrorCount > op->ErrorSoftLimit) {
      /* Ignore if some time has expired */
      if (now.tv_sec - local->ErrTime.tv_sec < 600)
	op->ErrorCount++;
    }
    else
      op->ErrorCount++;
    local->ErrTime = now;


    if ( op->ErrorCount == op->ErrorSoftLimit) {
      errh_Error( "IO Error soft limit reached on card '%s'", cp->Name);
      ctx->IOHandler->CardErrorSoftLimit = 1;
      ctx->IOHandler->ErrorSoftLimitObject = cdh_ObjidToAref( cp->Objid);
    }
    if ( op->ErrorCount >= op->ErrorHardLimit) {
      errh_Error( "IO Error hard limit reached on card '%s', IO stopped", cp->Name);
      ctx->Node->EmergBreakTrue = 1;
      ctx->IOHandler->CardErrorHardLimit = 1;
      ctx->IOHandler->ErrorHardLimitObject = cdh_ObjidToAref( cp->Objid);
      return IO__ERRDEVICE;
    }
  }


  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Ssab_MIOuP) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
