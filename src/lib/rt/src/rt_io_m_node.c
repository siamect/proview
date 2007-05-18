/* 
 * Proview   $Id: rt_io_m_node.c,v 1.3 2007-05-18 12:05:12 claes Exp $
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

/* rt_io_m_node.c -- io methods for the node object. */

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_errh.h"
#include "rt_io_agent_init.h"

/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus IoAgentInit (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{

  errh_Info( "Init of IO agent %s", ap->Name );

  return 1;
}

static pwr_tStatus IoAgentSwap (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{

  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Node) = {
  pwr_BindIoMethod(IoAgentInit),
  pwr_BindIoMethod(IoAgentSwap),
  pwr_NullMethod
};
