/* rt_io_m_rtp_dioc.c -- io method for rtp dioc objects.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_errh.h"
#include "rt_io_dioc.h"
#include "rt_io_agent_init.h"
#include "rt_io_agent_close.h"

/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus IoAgentInit (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{
  io_sRack	*rp;

  errh_Info( "Init of agent RTP DIOC %s", ap->Name );

  io_dioc_init();

  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus IoAgentClose (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{
  io_sRack	*rp;

  errh_Info( "Closing agent RTP DIOC %s", ap->Name );

  io_dioc_terminated();

  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(RTP_DIOC) = {
  pwr_BindIoMethod(IoAgentInit),
  pwr_BindIoMethod(IoAgentClose),
  pwr_NullMethod
};
