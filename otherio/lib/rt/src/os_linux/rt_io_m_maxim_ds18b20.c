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

/* rt_io_m_maxim_ds18b20.c -- I/O methods for class Maxim_DS18B20. */

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "co_time.h"
#include "rt_io_base.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "rt_io_msg.h"
#include "rt_io_m_onewire.h"

static pwr_tStatus IoCardInit( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp,
			       io_sCard *cp)
{
  pwr_sClass_Maxim_DS18B20 *op = (pwr_sClass_Maxim_DS18B20 *)cp->op;
  io_sLocalDS18B20 *local;
  pwr_tStatus sts;
  char name[40];
  pwr_tFileName fname;

  if ( cp->chanlist[0].cop) {
    local = (io_sLocalDS18B20 *) calloc( 1, sizeof(io_sLocalDS18B20));
    cp->Local = local;

    sprintf( name, "%d-%012x", op->Family, op->Super.Address);
    sprintf( fname, "/sys/bus/w1/devices/w1 bus master/%s/w1_slave", name); 
    local->value_fp = fopen( fname, "r");
    if (!local->value_fp) {
      errh_Error( "Maxim_DS18B20 Unable op open %s, '%ux'", cp->Name, 
		  op->Super.Address);
      sts = IO__INITFAIL;
      op->Status = sts;
      return sts;
    }

    io_AiRangeToCoef( &cp->chanlist[0]);

    errh_Info( "Init of Maxim_DS18B20 '%s'", cp->Name);
  }
  return IO__SUCCESS;
}

static pwr_tStatus IoCardClose( io_tCtx ctx,
			        io_sAgent *ap,
			        io_sRack *rp,
			        io_sCard *cp)
{
  io_sLocalDS18B20 *local = (io_sLocalDS18B20 *)cp->Local;


  if ( cp->chanlist[0].cop) {
    fclose( local->value_fp);
  }
  free( cp->Local);
  return IO__SUCCESS;
}

static pwr_tStatus IoCardRead( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack	*rp,
			       io_sCard	*cp)
{
  io_sLocalDS18B20 *local = (io_sLocalDS18B20 *)cp->Local;
  pwr_sClass_Maxim_DS18B20 *op = (pwr_sClass_Maxim_DS18B20 *)cp->op;
  char str[80];
  char *s;
  pwr_tUInt32 error_count = op->Super.ErrorCount;

  if ( op->ScanInterval > 1) {
    if ( local->interval_cnt != 0) {
      local->interval_cnt++;
      if ( local->interval_cnt >= op->ScanInterval)
        local->interval_cnt = 0;
      return IO__SUCCESS;
    }
    local->interval_cnt++;
  }

  if ( cp->chanlist[0].cop) {
    io_sChannel *chanp = &cp->chanlist[0];
    pwr_sClass_ChanAi *cop = (pwr_sClass_ChanAi *)chanp->cop;
    pwr_sClass_Ai *sop = (pwr_sClass_Ai *)chanp->sop;
    pwr_tInt32 ivalue;
    pwr_tFloat32 actvalue;

    if ( cop->CalculateNewCoef)
      // Request to calculate new coefficients
      io_AiRangeToCoef( chanp);

    fflush( local->value_fp);
    fgets( str, sizeof(str), local->value_fp);
    fgets( str, sizeof(str), local->value_fp);
    rewind( local->value_fp);

    s = strstr( str, "t=");
    if ( s) {
      sscanf( s+2, "%d", &ivalue);
      io_ConvertAi32( cop, ivalue, &actvalue);

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
    else {
      op->Super.ErrorCount++;
    }
  }

  if ( op->Super.ErrorCount >= op->Super.ErrorSoftLimit && 
       error_count < op->Super.ErrorSoftLimit) {
    errh_Warning( "IO Card ErrorSoftLimit reached, '%s'", cp->Name);
  }
  if ( op->Super.ErrorCount >= op->Super.ErrorHardLimit) {
    errh_Error( "IO Card ErrorHardLimit reached '%s', IO stopped", cp->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }    

  return IO__SUCCESS;
}

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(Maxim_DS18B20) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
