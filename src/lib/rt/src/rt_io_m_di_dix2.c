/* rt_io_m_di_dix2.c -- io methods for ssab cards.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#if defined(OS_ELN)
# include string
# include stdlib
# include stdio
#else
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
#endif


#include "pwr.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_filter_di.h"
#include "rt_io_ssab.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

typedef struct {
	char	*Address;
	DEVICE	Device[64];
	struct {
	  pwr_sClass_Di *sop[16];
	  void	*Data[16];
	  pwr_tBoolean Found;
	} Filter[2];
} io_sLocal;

static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  pwr_tStatus		sts;
  pwr_sClass_Di_DIX2	*op;
  io_sLocal 		*local;
  int			i, j;

  op = (pwr_sClass_Di_DIX2 *) cp->op;
  local = calloc( 1, sizeof(*local));
  cp->Local = local;

  errh_Info( "Init of di card '%s'", cp->Name);

  sts = ssabd_ini( op->DevName, &local->Address, &local->Device,
		ctx->RelativVector);
  if ( EVEN(sts))
  {
    errh_Error( "IO init error of di card '%s'\n%m", cp->Name, sts);
    return sts;
  }

  /* Init filter */
  for ( i = 0; i < 2; i++)
  {
    /* The filter handles one 16-bit word */
    for ( j = 0; j < 16; j++)
      local->Filter[i].sop[j] = cp->chanlist[i*16+j].sop;
    io_InitDiFilter( local->Filter[i].sop, &local->Filter[i].Found,
		local->Filter[i].Data, ctx->ScanTime);
  }

  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardClose (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  pwr_tStatus		sts;
  io_sLocal 		*local;
  int			i;

  local = (io_sLocal *) cp->Local;

  errh_Info( "IO closing di card '%s'", cp->Name);

  sts = ssab_close( local->Device);
  if ( EVEN(sts))
    errh_Error("IO error deleting device %s\n%m", cp->Name, sts);


  /* Free filter data */
  for ( i = 0; i < 2; i++)
  {
    if ( local->Filter[i].Found)
      io_CloseDiFilter( local->Filter[i].Data);
  }
  free( (char *) local);

  return 1;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  io_sLocal 		*local;
  pwr_tUInt16		data = 0;
  pwr_sClass_Di_DIX2	*op;
  pwr_tUInt16		invmask;
  pwr_tUInt16		convmask;
  int			i;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Di_DIX2 *) cp->op;

  for ( i = 0; i < 2; i++)
  { 
    if ( i == 0)
    {
      convmask = op->ConvMask1;
      invmask = op->InvMask1;
    }
    else
    {
      convmask = op->ConvMask2;
      invmask = op->InvMask2;
      if ( !convmask)
        break;
      if ( op->MaxNoOfChannels == 16)
        break;
    }

#if defined(OS_ELN)
    vaxc$establish(machfailread);
#endif
    data = ssabdi_read( local->Address, i);
    if (io_readerr || io_fatal_error)
    {
      /* Exceptionhandler was called */
      if ( io_fatal_error)
      {
        /* Activate emergency break */
        errh_Error( "Fatal read error, card '%s', IO i stopped", cp->Name);
        ctx->Node->EmergBreakTrue = 1;
        return IO__ERRDEVICE;
      }

      /* Increase error count and check error limits */
      op->ErrorCount++;

      if ( op->ErrorCount == op->ErrorSoftLimit)
        errh_Error( "IO Error soft limit reached on card '%s'", cp->Name);
      if ( op->ErrorCount >= op->ErrorHardLimit)
      {
        errh_Error( "IO Error hard limit reached on card '%s', IO stopped", cp->Name);
        ctx->Node->EmergBreakTrue = 1;
        return IO__ERRDEVICE;
      }
      continue;
    }

    /* Invert */
    data = data ^ invmask;

    /* Filter ... */
    if ( local->Filter[i].Found)
      io_DiFilter( local->Filter[i].sop, &data, local->Filter[i].Data);

    /* Move data to valuebase */
    io_DiUnpackWord( cp, data, convmask, i);
  }
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Di_DIX2) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_NullMethod
};
