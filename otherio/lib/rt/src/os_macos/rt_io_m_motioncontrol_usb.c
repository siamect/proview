/* 
 * Proview   $Id: rt_io_m_motioncontrol_usb.c,v 1.1 2007-11-22 13:28:59 claes Exp $
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
#include "rt_io_base.h"
#include "rt_io_rack_init.h"
#include "rt_io_rack_close.h"
#include "rt_io_msg.h"
#include "libusbio.h"
#include "rt_io_m_motioncontrol_usb.h"

static pwr_tStatus IoRackInit( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp)
{
  io_sLocalUSB *local;
  int status;
  int i;
  unsigned int snum;
  int found;
  io_sCard *cp;
  pwr_sClass_MotionControl_USB *op = (pwr_sClass_MotionControl_USB *)rp->op;

  local = (io_sLocalUSB *) calloc( 1, sizeof(io_sLocalUSB));
  rp->Local = local;

  for ( i = 0; i < (int)sizeof(local->USB_Handle); i++) {
    status = USBIO_Open( &local->USB_Handle[i]);
    if ( status) {
      if ( i == 0)
	op->Status = status;
      break;
    }

    /* Check is this card is configured */
    status = USBIO_GetSerialNr( &local->USB_Handle[i], &snum);
    if ( !status) {
      found = 0;
      for ( cp = rp->cardlist; cp; cp = cp->next) {
	if ( ((pwr_sClass_MotionControl_USBIO *)cp->op)->Super.Address == snum) {
	  local->snum[i] = snum;
	  found = 1;
	  break;
	}
      }
      if ( !found) {
	errh_Info( "USBIO Serial number %d not configured", snum);
	op->Status = USBIO_Close( &local->USB_Handle[i]);
	i--;
	continue;
      }
    }
    else
      errh_Error( "USBIO Serial number error '%s'", rp->Name);
  }

  errh_Info( "Init of USBIO rack '%s'", rp->Name);

  return IO__SUCCESS;
}

static pwr_tStatus IoRackClose( io_tCtx ctx,
			        io_sAgent *ap,
			        io_sRack *rp)
{
  io_sLocalUSB *local = rp->Local;
  pwr_sClass_MotionControl_USB *op = (pwr_sClass_MotionControl_USB *)rp->op;
  int i;

  for ( i = 0; i < (int)sizeof(local->USB_Handle); i++) {
    if ( local->USB_Handle[i])
      USBIO_Close( &local->USB_Handle[i]);
    else
      break;
  }
  op->Status = 0;
  return IO__SUCCESS;
}

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(MotionControl_USB) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_BindIoMethod(IoRackClose),
  pwr_NullMethod
};
