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

/* rt_io_m_gpio.c -- I/O methods for class GPIO. */

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "rt_io_base.h"
#include "rt_io_rack_init.h"
#include "rt_io_rack_close.h"
#include "rt_io_msg.h"
#include "rt_io_m_gpio.h"

static pwr_tStatus IoRackInit( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp)
{
  io_sCard *cp;
  pwr_sClass_GPIO *op = (pwr_sClass_GPIO *)rp->op;
  FILE *fp;
  pwr_tStatus sts;
  int i;

  fp = fopen( "/sys/class/gpio/export", "w");
  if (!fp) {
    errh_Error( "GPIO unable to open \"/sys/class/gpio/export\", %s", rp->Name);
    sts = IO__INITFAIL;
    op->Status = sts;
    return sts;      
  }

  for ( cp = rp->cardlist; cp; cp = cp->next) {
    for ( i = 0; i < GPIO_MAX_CHANNELS; i++) {
      if ( cp->chanlist[i].cop) {
	switch ( cp->chanlist[i].ChanClass) {
	case pwr_cClass_ChanDi:
	  fprintf( fp, "%u", ((pwr_sClass_ChanDi *)cp->chanlist[i].cop)->Number);
	  fflush( fp);
	  break;
	case pwr_cClass_ChanDo:
	  fprintf( fp, "%u", ((pwr_sClass_ChanDo *)cp->chanlist[i].cop)->Number);
	  fflush( fp);
	  break;
	default: ;
	}
      }
    }
  }
  fclose( fp);

  errh_Info( "Init of GPIO '%s'", rp->Name);
  op->Status = IO__SUCCESS;

  return IO__SUCCESS;
}

static pwr_tStatus IoRackClose( io_tCtx ctx,
			        io_sAgent *ap,
			        io_sRack *rp)
{
  pwr_sClass_GPIO *op = (pwr_sClass_GPIO *)rp->op;

  op->Status = 0;
  return IO__SUCCESS;
}

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(GPIO) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_BindIoMethod(IoRackClose),
  pwr_NullMethod
};
