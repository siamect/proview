/* rt_io_m_pb_di.c
   PROVIEW/R	*/

#pragma pack(1)

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/ioctl.h>

#include "pb_type.h"
#include "pb_if.h"
#include "pb_fmb.h"
#include "pb_dp.h"
#include "rt_io_pb_locals.h"

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "rt_io_profiboard.h"


/*----------------------------------------------------------------------------*\
   Init method for the Pb module Di
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Di *op;
  pwr_sClass_Pb_DP_Slave *slave;

  op = (pwr_sClass_Pb_Di *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  local->byte_swap = 0;

  if (rp->Class == pwr_cClass_Pb_DP_Slave) {
    slave = (pwr_sClass_Pb_DP_Slave *) rp->op;

    /* Little endian, no problem */
    if (slave->ByteOrdering == 0) local->byte_swap = 0;

    /* Big endian, men digitala moduler byteorienterade */
    if (slave->ByteOrdering == 1) local->byte_swap = 0;

    /* Big endian, digitala moduler wordorienterade => swappa */
    if (slave->ByteOrdering == 2) local->byte_swap = 1;
  }
  else {
    /* Old style */
    if (rp->Class == pwr_cClass_Pb_ET200M) local->byte_swap = 0;
    if (rp->Class == pwr_cClass_Pb_NPBA12) local->byte_swap = 1;
    if (rp->Class == pwr_cClass_Pb_Euro2500) local->byte_swap = 1;
  }

  if (op->Status < 1) {
    errh_Info( "Error initializing Pb module Di %s", cp->Name );
  }

  return 1;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb module Di
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Di *op;
  pwr_tUInt16 data[2] = {0, 0};
  pwr_tUInt16 invmask;
  pwr_tUInt16 convmask;
  int i;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Di *) cp->op;

  if (op->Status >= 1) {

    memcpy(&data, local->input_area + op->OffsetInputs, op->BytesOfInput);

    for (i=0; i<2; i++) {

      if (i==0) {
        convmask = op->ConvMask1;
        invmask = op->InvMask1;
      }
      else {
        convmask = op->ConvMask2;
        invmask = op->InvMask2;
        if (op->NumberOfChannels <= 16) break;
      }

      if (local->byte_swap == 1) data[i] = swap16(data[i]);      
      data[i] = data[i] ^ invmask;
      io_DiUnpackWord(cp, data[i], convmask, i);
    
    }
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
  io_sCardLocal *local;
  local = rp->Local;

  free ((char *) local);

  return 1;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_Di) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};
