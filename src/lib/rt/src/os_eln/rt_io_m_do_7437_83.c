/* io_m_rtp_dio.c -- io methods for rtp cards.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#if defined(OS_ELN)
# include string
# include stdlib
# include stdio
# include descrip
#else
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <descrip.h>
#endif


#include "pwr.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_filter_po.h"

#if defined(OS_ELN)
#include "rt_io_rtp.h"
#endif
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_write.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
#define RTP_DO_7437_83      10
#define RTP_DO_7437_38      11
#define RTP_DO_7435_40      12

typedef struct {
	pwr_tInt16	RackAddress;
	pwr_tInt32	Address;
	pwr_tInt16	CardType;
	struct {
	  pwr_sClass_Po *sop[16];
	  void	*Data[16];
	  pwr_tBoolean Found;
	} Filter[2];
} io_sLocal;


/*** VMS Test ****/
#if defined(OS_VMS)
extern pwr_tBoolean		io_writeerr;
extern pwr_tBoolean		io_fatal_error;

static void rtpdo_write(	
		short		data,
		pwr_tInt16	rack_address,
		pwr_tInt32	address,
		int		grupp,
		pwr_tInt16	card_type)
{
  io_writeerr = 0;
  io_fatal_error = 0;
}
#endif
/*** Slut VMS Test ****/

static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  pwr_tStatus		sts;
  pwr_sClass_Do_7437_83	*op;
  io_sLocal 		*local;
  int			i, j;

  op = (pwr_sClass_Do_7437_83 *) cp->op;
  local = calloc( 1, sizeof(*local));
  local->Address = (op->CardAddress |  0xf0);   
  local->RackAddress = ((pwr_sClass_Rack_RTP *) rp->op)->RackAddress;
  local->CardType = RTP_DO_7437_83;

  errh_Info( "Init of do card '%s'", cp->Name);

  /* Init filter for Po signals */
  for ( i = 0; i < 2; i++)
  {
    /* The filter handles one 16-bit word */
    for ( j = 0; j < 16; j++)
    {
      if ( cp->chanlist[i*16+j].SigClass == pwr_cClass_Po)
        local->Filter[i].sop[j] = cp->chanlist[i*16+j].sop;
    }
    io_InitPoFilter( local->Filter[i].sop, &local->Filter[i].Found,
		local->Filter[i].Data, ctx->ScanTime);
  }

  cp->Local = local;

  return 1;
}

/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp  
) 
{
  io_sLocal 		*local;
  pwr_tUInt16		data = 0;
  pwr_sClass_Do_7437_83	*op;
  pwr_tUInt16		invmask;
  pwr_tUInt16		testmask;
  pwr_tUInt16		testvalue;
  int			i;

  if ( cp->AgentControlled)
    return IO__SUCCESS;

  local = (io_sLocal *) cp->Local;
  op = (pwr_sClass_Do_7437_83 *) cp->op;

  for ( i = 0; i < 2; i++)
  { 
    if ( ctx->Node->EmergBreakTrue && ctx->Node->EmergBreakSelect == FIXOUT)
    {
      if ( i == 0)
        data = op->FixedOutValue1;
      else
        data = op->FixedOutValue2;
    }
    else
      io_DoPackWord( cp, &data, i);

    if ( i == 0)
    {
      testmask = op->TestMask1;
      invmask = op->InvMask1;
    }
    else
    {
      testmask = op->TestMask2;
      invmask = op->InvMask2;
      if ( op->MaxNoOfChannels == 16)
        break;
    }

    /* Invert */
    data = data ^ invmask;

    /* Filter */
    if ( local->Filter[i].Found)
      io_PoFilter( local->Filter[i].sop, &data, local->Filter[i].Data);

    /* Testvalues */
    if ( testmask)
    {
      if ( i == 0)
        testvalue = op->TestValue1;
      else
        testvalue = op->TestValue2;
      data = (data & ~ testmask) | (testmask & testvalue);
    }

    rtpdo_write( data, local->RackAddress, local->Address, i + 1, 
		local->CardType);
    if (io_writeerr || io_fatal_error)
    {
      if ( io_fatal_error)
      {
        /* Activate emergency break */
        errh_Error( "Fatal write error, card '%s', IO i stopped", cp->Name);
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
  }
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Do_7437_83) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
