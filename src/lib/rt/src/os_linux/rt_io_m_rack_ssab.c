/* rt_io_m_rack_ssab.c -- io methods for ssab rack objects.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_errh.h"
#include "rt_io_rack_init.h"
#include "rt_io_msg.h"


typedef struct {
	int		Qbus_fp;
} io_sRackLocal;


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus IoRackInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  io_sRackLocal 	*local;

  /* Open Qbus driver */
  local = calloc( 1, sizeof(*local));
  rp->Local = local;

  local->Qbus_fp = open("/dev/qbus", O_RDWR);
  if ( local->Qbus_fp == -1)
  {
    errh_Error( "Qbus initialization error, IO rack %s", rp->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }
  
  errh_Info( "Init of IO rack %s", rp->Name);
  return 1;
}

static pwr_tStatus IoRackClose (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  io_sRackLocal 	*local;

  /* Close Qbus driver */
  local = rp->Local;

  close( local->Qbus_fp);
  free( (char *)local);

  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Rack_SSAB) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_BindIoMethod(IoRackClose),
  pwr_NullMethod
};
