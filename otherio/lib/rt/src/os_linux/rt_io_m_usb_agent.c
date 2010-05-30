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

/* rt_io_m_usb_agent.c -- I/O methods for class USB_Agent. */

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "rt_io_base.h"
#include "rt_io_agent_init.h"
#include "rt_io_agent_close.h"
#include "rt_io_msg.h"

#if defined PWRE_CONF_LIBUSB

#include <libusb-1.0/libusb.h>
#include "rt_io_m_usb_agent.h"

static pwr_tStatus IoAgentInit( io_tCtx ctx,
				io_sAgent *ap)
{
  io_sLocalUSB_Agent *local;
  int sts;
  pwr_sClass_USB_Agent *op = (pwr_sClass_USB_Agent *)ap->op;

  local = (io_sLocalUSB_Agent *) calloc( 1, sizeof(io_sLocalUSB_Agent));
  ap->Local = local;

  sts = libusb_init( &local->libusb_ctx);
  if ( sts != 0) {
    op->Status = sts;
    local->libusb_ctx = 0;
    errh_Error( "Init of USB agent failed '%s'", ap->Name);
  }
  else {
    op->Status = IO__SUCCESS;
    errh_Info( "Init of USBIO agent '%s'", ap->Name);
  }

  return IO__SUCCESS;
}

static pwr_tStatus IoAgentClose( io_tCtx ctx,
				 io_sAgent *ap)
{
  io_sLocalUSB_Agent *local = ap->Local;
  pwr_sClass_USB_Agent *op = (pwr_sClass_USB_Agent *)ap->op;

  libusb_exit( local->libusb_ctx);
  op->Status = 0;
  free( local);

  return IO__SUCCESS;
}

#else
static pwr_tStatus IoAgentInit( io_tCtx ctx, io_sAgent *ap) { return 0;}
static pwr_tStatus IoAgentClose( io_tCtx ctx, io_sAgent *ap) { return 0;}
#endif

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(USB_Agent) = {
  pwr_BindIoMethod(IoAgentInit),
  pwr_BindIoMethod(IoAgentClose),
  pwr_NullMethod
};

