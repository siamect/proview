/* rt_io_m_pb_ai.c
   PROVIEW/R	*/

#pragma pack(1)

#include <stdio.h>
#include <stdlib.h>
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
   Init method for the Pb module Ai
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Ai *op;
  int i;
  io_sChannel *chanp;
  pwr_sClass_Pb_DP_Slave *slave;

  op = (pwr_sClass_Pb_Ai *) cp->op;
  local = (io_sCardLocal *) cp->Local;

  local->byte_swap = 0;

  if (rp->Class == pwr_cClass_Pb_DP_Slave) {
    slave = (pwr_sClass_Pb_DP_Slave *) rp->op;

    /* Little endian, no problem */
    if (slave->ByteOrdering == 0) local->byte_swap = 0;

    /* Big endian, digitala moduler byteorienterade => swappa */
    if (slave->ByteOrdering == 1) local->byte_swap = 1;

    /* Big endian, digitala moduler wordorienterade => swappa */
    if (slave->ByteOrdering == 2) local->byte_swap = 1;
  }
  else {
    /* Old style */
    if (rp->Class == pwr_cClass_Pb_ET200M) local->byte_swap = 1;
    if (rp->Class == pwr_cClass_Pb_NPBA12) local->byte_swap = 1;
    if (rp->Class == pwr_cClass_Pb_Euro2500) local->byte_swap = 1;
  }

  if (op->Status >= 1) {
//    errh_Info( "Init of Pb module Ai %s", cp->Name );
    for (i=0; i<IO_MAXCHAN; i++) {
      local->scancount[i] = 0;
    }
    // Calculate polycoeff
    for (i=0; i<cp->ChanListSize; i++) {
      chanp = &cp->chanlist[i];
      if (!chanp->cop) continue;
      io_AiRangeToCoef(chanp);
    }
  }
  else
    errh_Info( "Error initializing Pb module Ai %s", cp->Name );

  return 1;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb Ai card
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Ai *op;
  io_sChannel *chanp;
  pwr_sClass_ChanAi *cop;
  pwr_sClass_Ai *sop;
  pwr_tInt16 data = 0;
  pwr_tUInt16 udata = 0;
  pwr_tFloat32 actvalue;
  int i;

  local = (io_sCardLocal *) cp->Local;
  op = (pwr_sClass_Pb_Ai *) cp->op;

  if (op->Status >= 1) {
    
    for (i=0; i<cp->ChanListSize; i++) {

      chanp = &cp->chanlist[i];
      if (!chanp->cop) continue;

      cop = (pwr_sClass_ChanAi *) chanp->cop;
      sop = (pwr_sClass_Ai *) chanp->sop;

      if (cop->CalculateNewCoef) io_AiRangeToCoef(chanp);

      if (cop->ConversionOn) {
        if (local->scancount[i] <= 1) {
	  memcpy(&udata, local->input_area + op->OffsetInputs + 2*i, 2);
	  if (local->byte_swap == 1) udata = swap16(udata);
	  data = (pwr_tInt16) udata;
	  sop->RawValue = data;
     	  sop->SigValue = data * cop->SigValPolyCoef1 + cop->SigValPolyCoef0;
          switch(chanp->ChanClass) {
            case pwr_cClass_ChanAi:
              io_ConvertAi(cop, data, &actvalue);
              break;
            case pwr_cClass_ChanAit:
  	      io_ConvertAit((pwr_sClass_ChanAit *) cop, data, &actvalue);
	      break;
          }

	  // Filter
 	  if (sop->FilterType == 1 &&
	      sop->FilterAttribute[0] > 0 &&
	      sop->FilterAttribute[0] > ctx->ScanTime) {
	    actvalue = *(pwr_tFloat32 *) chanp->vbp +
		ctx->ScanTime / sop->FilterAttribute[0] *
		(actvalue - *(pwr_tFloat32 *) chanp->vbp);
          }

          *(pwr_tFloat32 *) chanp->vbp = actvalue;
          local->scancount[i] = cop->ScanInterval + 1;
        }
        local->scancount[i]--;

      }  // if ...ConversionOn
    }  // for
  }  // if ...op->Status

  return 1;
}


/*----------------------------------------------------------------------------*\
   Close method for the Pb Ai card
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

pwr_dExport pwr_BindIoMethods(Pb_Ai) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};
