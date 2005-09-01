/* 
 * Proview   $Id: rt_io_m_rack_rtp.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* rt_io_m_rack_rtp.c -- io methods for common rack objects. */

#include $vaxelnc
#include $kernelmsg

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_errh.h"
#include "rt_io_rtp.h"
#include "rt_io_rack_init.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static	int init_done = 0;


static pwr_tStatus IoRackInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  errh_Info( "Init of IO rack %s", rp->Name);

  /* Init the RTP-bus */

  if ( !init_done)
  {
    init_done = 1;
    if ( ! rtp_init())
    {
      errh_Error( "IO init error, Unable to start RTP IO");
      return KER$_NO_SUCH_DEVICE;
    }
  }

  return 1;

}

/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Rack_RTP) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_NullMethod
};
