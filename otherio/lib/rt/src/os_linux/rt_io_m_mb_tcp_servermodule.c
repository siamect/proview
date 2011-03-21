/* 
 * Proview   $Id$
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>



#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "rt_io_bus.h"
#include "rt_mb_msg.h"

#include "rt_io_mb_locals.h"
#include "co_time.h"

/*----------------------------------------------------------------------------*\
   Init method for the Modbus module
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sServerModuleLocal *local;
  pwr_sClass_Modbus_TCP_ServerModule *op;
  int i;

  op = (pwr_sClass_Modbus_TCP_ServerModule *) cp->op;
  local = (io_sServerModuleLocal *) cp->Local;
  
  for (i = 0; i < IO_MAXCHAN; i++) {
    local->scancount[i] = 0;
  }

  op->Status = pwr_eModbusModule_StatusEnum_StatusUnknown;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Read method for the Modbus TCP module
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoCardRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp,
  io_sCard	*cp
) 
{
  io_sServerModuleLocal *local;
  io_sServerLocal *local_server;
  pwr_sClass_Modbus_TCP_ServerModule *op;
  pwr_sClass_Modbus_TCP_Server *server;

  op = (pwr_sClass_Modbus_TCP_ServerModule *) cp->op;
  local = (io_sServerModuleLocal *) cp->Local;
  server = (pwr_sClass_Modbus_TCP_Server *) rp->op;
  local_server = (io_sServerLocal *) rp->Local;

  if ( server->DisableServer || !local)
    return IO__SUCCESS;

  if (server->Status == MB__NORMAL) { 
    thread_MutexLock( &local_server->mutex);

    io_bus_card_read(ctx, rp, cp, local->input_area, NULL,  pwr_eByteOrderingEnum_BigEndian, pwr_eFloatRepEnum_FloatIntel);  

    thread_MutexUnlock( &local_server->mutex);
  }
//  printf("Method Modbus_Module-IoCardRead\n");
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
  io_sServerModuleLocal *local;
  io_sServerLocal *local_server;
  pwr_sClass_Modbus_TCP_ServerModule *op;  
  pwr_sClass_Modbus_TCP_Server *server;

  op = (pwr_sClass_Modbus_TCP_ServerModule *) cp->op;
  local = (io_sServerModuleLocal *) cp->Local;
  server = (pwr_sClass_Modbus_TCP_Server *) rp->op;
  local_server = (io_sServerLocal *) rp->Local;
  
  if ( server->DisableServer || !local)
    return IO__SUCCESS;

  if (server->Status == MB__NORMAL) { 
    thread_MutexLock( &local_server->mutex);

    io_bus_card_write(ctx, cp, local->output_area, pwr_eByteOrderingEnum_BigEndian, pwr_eFloatRepEnum_FloatIntel);

    thread_MutexUnlock( &local_server->mutex);
  }
//  printf("Method Modbus_Module-IoCardWrite\n");
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Modbus_TCP_ServerModule) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardRead),
   pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};

