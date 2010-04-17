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
#include "rt_io_m_gpio.h"

static pwr_tStatus IoCardInit( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp,
			       io_sCard *cp)
{
  int i;
  pwr_sClass_GPIO_Module *op = (pwr_sClass_GPIO_Module *)cp->op;
  FILE *fp;
  char str[80];
  io_sLocalGPIO_Module *local;
  char direction[20];
  char access[20];
  pwr_tStatus sts;

  local = (io_sLocalGPIO_Module *) calloc( 1, sizeof(io_sLocalGPIO_Module));
  cp->Local = local;

  for ( i = 0; i < GPIO_MAX_CHANNELS; i++) {
    if ( cp->chanlist[i].cop) {
      switch( cp->chanlist[i].ChanClass) {
      case pwr_cClass_ChanDi:
	local->number[i] = ((pwr_sClass_ChanDi *)cp->chanlist[i].cop)->Number;
	strcpy( direction, "in");
	strcpy( access, "r+");
	break;
      case pwr_cClass_ChanDo:
	local->number[i] = ((pwr_sClass_ChanDo *)cp->chanlist[i].cop)->Number;
	strcpy( direction, "out");
	strcpy( access, "w");
	break;
      default: ;
      }

      switch( cp->chanlist[i].ChanClass) {
      case pwr_cClass_ChanDi:
      case pwr_cClass_ChanDo:
	sprintf( str, "/sys/class/gpio/gpio%u/direction", local->number[i]);
	fp = fopen( str, "w");
	if (!fp) {
	  errh_Error( "GPIO uable to open %s, %s, Id: %d", str, cp->Name,
		      local->number[i]);
	  sts = IO__INITFAIL;
	  op->Status = sts;
	  return sts;
	}
	fprintf( fp, "%s", direction);      
	fclose( fp);
	
	sprintf( str, "/sys/class/gpio/gpio%u/value", local->number[i]);
	local->value_fp[i] = fopen( str, access);
	if (!local->value_fp[i]) {
	  errh_Error( "GPIO Unable op open %s, '%s' Id: %d", str, cp->Name, 
		      local->number[i]);
	  sts = IO__INITFAIL;
	  op->Status = sts;
	  return sts;
	}
	break;
      default: ;
      }
    }
  }
  errh_Info( "Init of GPIO Module '%s'", cp->Name);

  return IO__SUCCESS;
}

static pwr_tStatus IoCardClose( io_tCtx ctx,
			        io_sAgent *ap,
			        io_sRack *rp,
			        io_sCard *cp)
{
  int i;
  io_sLocalGPIO_Module *local = (io_sLocalGPIO_Module *)cp->Local;


  for ( i = 0; i < GPIO_MAX_CHANNELS; i++) {
    if ( cp->chanlist[i].cop) {
      switch( cp->chanlist[i].ChanClass){
      case pwr_cClass_ChanDi:
      case pwr_cClass_ChanDo:
	if ( local->value_fp[i])
	  fclose( local->value_fp[i]);
	break;
      default: ;
      }
    }
  }
  free( cp->Local);
  return IO__SUCCESS;
}

static pwr_tStatus IoCardRead( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack	*rp,
			       io_sCard	*cp)
{
  io_sLocalGPIO_Module *local = (io_sLocalGPIO_Module *)cp->Local;
  int i;
  char str[20];

  for ( i = 0; i < GPIO_MAX_CHANNELS; i++) {
    if ( cp->chanlist[i].cop) {
      switch( cp->chanlist[i].ChanClass) {
      case pwr_cClass_ChanDi:
	fflush( local->value_fp[i]);
	fgets( str, sizeof(str), local->value_fp[i]);
	rewind( local->value_fp[i]);
	if ( str[0] == '0')
	  *(pwr_tBoolean *)cp->chanlist[i].vbp = 0;
	else
	  *(pwr_tBoolean *)cp->chanlist[i].vbp = 1;
	break;
      default: ;
      }
    }
  }
  return IO__SUCCESS;
}

static pwr_tStatus IoCardWrite( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack	*rp,
			       io_sCard	*cp)
{
  io_sLocalGPIO_Module *local = (io_sLocalGPIO_Module *)cp->Local;
  int i;

  for ( i = 0; i < GPIO_MAX_CHANNELS; i++) {
    if ( cp->chanlist[i].cop) {
      switch( cp->chanlist[i].ChanClass) {
      case pwr_cClass_ChanDo:
	if ( *(pwr_tBoolean *)cp->chanlist[i].vbp)
	  fprintf( local->value_fp[i], "1");
	else
	  fprintf( local->value_fp[i], "0");
	fflush( local->value_fp[i]);
	// printf( "Write %d %d\n", local->number[i], *(pwr_tBoolean *)cp->chanlist[i].vbp); 
	break;
      default: ;
      }
    }
  }
  return IO__SUCCESS;
}

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(GPIO_Module) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
