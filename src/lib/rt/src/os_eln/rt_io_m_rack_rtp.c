/* rt_io_m_rack_rtp.c -- io methods for common rack objects.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

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
