/* rt_io_m_pb_do.c
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
   Init method for the Pb module Do
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Do *op;

  op = (pwr_sClass_Pb_Do *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  if (op->NumberOfChannels != 8 && op->NumberOfChannels != 16 && op->NumberOfChannels != 32)
    op->Status = PB_MODULE_STATE_NOTINIT;
    
  if (op->Orientation > op->NumberOfChannels)
    op->Status = PB_MODULE_STATE_NOTINIT;  

  if (op->Status < PB_MODULE_STATE_OPERATE) errh_Info( "Error initializing Pb module Do %s", cp->Name );

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb module Do
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Do *op;
  pwr_sClass_Pb_DP_Slave *slave;

  pwr_tUInt16 data[2] = {0, 0};
  pwr_tUInt32 *data32;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Do *) cp->op;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;

  if (op->Status >= PB_MODULE_STATE_OPERATE && slave->DisableSlave != 1) {

    io_DoPackWord(cp, &data[0], 0);
    if (op->NumberOfChannels > 16) io_DoPackWord(cp, &data[1], 1);

    if (slave->ByteOrdering == PB_BYTEORDERING_BE) {
      if (op->Orientation == PB_ORIENTATION_WORD) {
        data[0] = swap16(data[0]);
        data[1] = swap16(data[1]);
      }
      else if (op->Orientation == PB_ORIENTATION_DWORD) {
      	data32 = (pwr_tUInt32 *) &data;
	*data32 = swap32(*data32);
      }
    }    

    memcpy(local->output_area + op->OffsetOutputs, &data, op->BytesOfOutput);
  }
  return IO__SUCCESS;
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
  local = cp->Local;

  free ((char *) local);

  return IO__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_Do) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardWrite),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};
