/* 
 * Proview   $Id: rt_io_m_mb_master.c,v 1.1 2008-10-16 07:02:45 claes Exp $
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

/* rt_io_m_pb_profiboard.c -- io methods for the profibus master object
   The PbMaster object serves as agent for one Profibus DP bus
   The board we use is Profiboard from Softing
*/

#pragma pack(1)

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>


#include "pwr.h"
#include "co_cdh.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "rt_gdh.h"
#include "rt_io_base.h"
#include "rt_io_bus.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_mb_msg.h"

#include "rt_io_mb_locals.h"


#include "rt_io_agent_init.h"



static pwr_tStatus IoAgentInit (
  io_tCtx	ctx,
  io_sAgent	*ap
);
static pwr_tStatus IoAgentRead (
  io_tCtx	ctx,
  io_sAgent	*ap
);
static pwr_tStatus IoAgentWrite (
  io_tCtx	ctx,
  io_sAgent	*ap
);
static pwr_tStatus IoAgentClose (
  io_tCtx	ctx,
  io_sAgent	*ap
);


/*----------------------------------------------------------------------------*\
   Init method for the mb_master agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoAgentInit (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{
  io_sAgentLocal *local;

  /* Allocate area for local data structure */
  ap->Local = calloc(1, sizeof(io_sAgentLocal));
  if (!ap->Local) {
    errh_Error( "ERROR config Modbus Master %s - %s", ap->Name, "calloc");
    return IO__ERRINIDEVICE;
  }

  local = ap->Local;
        
  local->initialized = TRUE;
  
  return IO__SUCCESS;
}

/*----------------------------------------------------------------------------*\
   Read method for the Modbus Master agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoAgentRead (
  io_tCtx	ctx,
  io_sAgent	*ap
)
{
  io_sAgentLocal *local;
  io_sRackLocal *local_rack;
  pwr_tUInt16 sts;
  io_sRack *rp;
  pwr_tCid cid;
  pwr_sClass_Modbus_TCP_Slave *sp;

  local = (io_sAgentLocal *) ap->Local;

  /* Do configuration check and initialize modules. */

  rp = ap->racklist;

  while(rp) {
    
    cid = rp->Class;
    while ( ODD( gdh_GetSuperClass( cid, &cid, rp->Objid))) ;

    switch (cid) {

      case pwr_cClass_Modbus_TCP_Slave:

	sp = (pwr_sClass_Modbus_TCP_Slave *) rp->op;
        local_rack = rp->Local;
	
	/* Start receving old data so the input buffer is flushed */
	if (sp->Status == MB__NORMAL) {
	  sts = mb_recv_data(local_rack, rp, sp);
	}  

	/* Request new data */
	if (sp->Status == MB__NORMAL && sp->DisableSlave != 1) {
	  sts = mb_send_data(local_rack, rp, sp, mb_mSendMask_ReadReq);
	}

        break;
    } /* End - switch ... */

    rp = rp->next;
  }


  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb_Profiboard agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoAgentWrite (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoAgentClose (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Modbus_Master) = {
  pwr_BindIoMethod(IoAgentInit),
  pwr_BindIoMethod(IoAgentRead),
  pwr_BindIoMethod(IoAgentWrite),
  pwr_BindIoMethod(IoAgentClose),
  pwr_NullMethod
};
