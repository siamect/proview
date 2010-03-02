/* 
 * Proview   $Id: rt_io_m_pb_module.c,v 1.12 2008-03-06 07:26:15 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <sys/file.h>
#include <sys/ioctl.h>

#include "rt_io_pb_locals.h"

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_profibusclasses.h"
#include "rt_io_base.h"
#include "rt_io_bus.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "rt_pb_msg.h"

/*----------------------------------------------------------------------------*\
   Init method for the Pb module
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Module *op;
  int i;

  op = (pwr_sClass_Pb_Module *) cp->op;
  local = (io_sCardLocal *) cp->Local;
  
  for (i=0; i<IO_MAXCHAN; i++) {
    local->scancount[i] = 0;
  }

  op->Status = PB__NORMAL;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb module
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Module *op;
  pwr_sClass_Pb_DP_Slave *slave;

  op = (pwr_sClass_Pb_Module *) cp->op;
  local = (io_sCardLocal *) cp->Local;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;

  op->Status = slave->Status;  

  /* I/O-read operations should always be made. This ensures correct values */
  /* on all inputs. Default StallAction is ResetInputs which means that */
  /* all inputs will be zeroed */

  io_card_read(ctx, rp, cp, local->input_area, slave->Diag, slave->ByteOrdering,
                 slave->FloatRepresentation);  

//  printf("Method Pb_Module-IoCardRead\n");
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb module
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sCardLocal *local;
  pwr_sClass_Pb_Module *op;
  pwr_sClass_Pb_DP_Slave *slave;

  op = (pwr_sClass_Pb_Module *) cp->op;
  local = (io_sCardLocal *) cp->Local;
  slave = (pwr_sClass_Pb_DP_Slave *) rp->op;
  
  op->Status = slave->Status;  

  if (op->Status == PB__NORMAL) { 
    io_card_write(ctx, cp, local->output_area, slave->ByteOrdering,
                 slave->FloatRepresentation);  
  }
//  printf("Method Pb_Module-IoCardWrite\n");
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Close method for the Pb module
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
  
  printf("Method Pb_Module-IoCardClose\n");
  return IO__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_Module) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardWrite),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};

