/* 
 * Proview   $Id: rt_io_m_ssab_remoterack.c,v 1.1 2006-02-10 14:32:36 claes Exp $
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

/* rt_io_m_ssab_remoterack.c -- io methods for ssab remote rack objects. */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_errh.h"
#include "rt_io_rack_init.h"
#include "rt_io_msg.h"


typedef struct {
	int		xxx;
} io_sRackLocal;


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus IoRackInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  errh_Info( "Init of IO remote rack %s", rp->Name);
  return 1;
}

static pwr_tStatus IoRackClose (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  return 1;
}

static pwr_tStatus IoRackRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  return 1;
}

static pwr_tStatus IoRackWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Ssab_RemoteRack) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_BindIoMethod(IoRackClose),
  pwr_BindIoMethod(IoRackRead),
  pwr_BindIoMethod(IoRackWrite),
  pwr_NullMethod
};


