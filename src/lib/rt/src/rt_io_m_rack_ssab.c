/* rt_io_m_rack_ssab.c -- io methods for ssab rack objects.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_errh.h"
#include "rt_io_rack_init.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus IoRackInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  /* This method just indicates that this is a Rack object */

  errh_Info( "Init of IO rack %s", rp->Name);
  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Rack_SSAB) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_NullMethod
};
