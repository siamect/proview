/* rt_io_m_node.c -- io methods for the node object.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

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


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Node) = {
  pwr_BindIoMethod(IoAgentInit),
  pwr_NullMethod
};
