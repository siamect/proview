/* 
 * Proview   $Id: rt_io_m_pb_profiboard.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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
#include "rt_io_agent_init.h"

#include "rt_io_profiboard.h"


/*----------------------------------------------------------------------------*\
  Sends request to Profiboard for setting FMB parameters
\*----------------------------------------------------------------------------*/
static short fmb_set_configuration(int fp,  
				   pwr_sClass_Pb_Profiboard *op) 
{
  T_PROFI_SERVICE_DESCR sdb;
  T_FMB_SET_CONFIGURATION_REQ data;
  pwr_tUInt16 retval;

  sdb.comm_ref = 0;
  sdb.layer = FMB;
  sdb.service = FMB_SET_CONFIGURATION;
  sdb.primitive = REQ;
  sdb.invoke_id = 0;
  sdb.result = 0;

  data.fms_active = PB_FALSE;
  data.dp_active = PB_TRUE;
  data.fdlif_active = PB_FALSE;
  data.fdl_evt_receiver = DP_USR;
  data.sm7_active = PB_FALSE;
  data.data_buffer_length = 256;

  data.dp.max_number_slaves = op->MaxNumberSlaves;
  data.dp.max_slave_output_len = op->MaxSlaveOutputLen;
  data.dp.max_slave_input_len = op->MaxSlaveInputLen;
  data.dp.max_slave_diag_entries = 64;
  data.dp.max_slave_diag_len = 32;
  data.dp.max_bus_para_len = 128;
  data.dp.max_slave_para_len = 128;

  retval = pb_cmi_flush(fp);
  if (retval != PB_OK) return retval;

  retval = pb_cmi_write(fp, &sdb, &data, sizeof(T_FMB_SET_CONFIGURATION_REQ));
  if (retval != PB_OK) return retval;

  retval = pb_cmi_read(fp, NULL, NULL, NULL);
  return retval;
}


/*----------------------------------------------------------------------------*\
  Sends request to Profiboard for setting DP master parameters
\*----------------------------------------------------------------------------*/
static short dp_init_master(int fp,  
			    pwr_sClass_Pb_Profiboard *op)
{
  T_PROFI_SERVICE_DESCR sdb;
  T_DP_INIT_MASTER_REQ data;
  pwr_tUInt16 retval;

  sdb.comm_ref = 0;
  sdb.layer = DP;
  sdb.service = DP_INIT_MASTER;
  sdb.primitive = REQ;
  sdb.invoke_id = 0;
  sdb.result = 0;

  data.master_default_address = 0;
  data.master_class2 = PB_FALSE;
  data.lowest_slave_address = 2;
  data.slave_io_address_mode = DP_AAM_ARRAY;
  data.clear_outputs = PB_TRUE;
  data.auto_remote_services = DP_AUTO_REMOTE_SERVICES;
  data.cyclic_data_transfer = PB_TRUE;

  retval = pb_cmi_flush(fp);
  if (retval != PB_OK) return retval;

  retval = pb_cmi_write(fp, &sdb, &data, sizeof(T_DP_INIT_MASTER_REQ));
  if (retval != PB_OK) return retval;

  retval = pb_cmi_read(fp, NULL, NULL, NULL);
  return retval;
}


/*----------------------------------------------------------------------------*\
  Sends request to Profiboard for setting DP bus parameters
\*----------------------------------------------------------------------------*/
static short dp_init_bus(int fp,  
		         pwr_sClass_Pb_Profiboard *op)
{
  T_PROFI_SERVICE_DESCR sdb;
  struct {
    T_DP_DOWNLOAD_REQ drp; 
    T_DP_BUS_PARA_SET dbp;
  } data;
  pwr_tUInt16 retval;
  int i;

  sdb.comm_ref = 0;
  sdb.layer = DP;
  sdb.service = DP_DOWNLOAD_LOC;
  sdb.primitive = REQ;
  sdb.invoke_id = 0;
  sdb.result = 0;

  data.drp.data_len =  66;
  data.drp.rem_add = 0;
  data.drp.area_code = DP_AREA_BUS_PARAM;
  data.drp.add_offset = 0;

  data.dbp.bus_para_len = swap16(66);
  data.dbp.fdl_add = 0;

  switch (op->BaudRate) {
    case 500:
      data.dbp.baud_rate = DP_KBAUD_500;
      break;
    case 750:
      data.dbp.baud_rate = DP_KBAUD_750;
      break;
    case 1500:
      data.dbp.baud_rate = DP_KBAUD_1500;
      break;
    case 3000:
      data.dbp.baud_rate = DP_KBAUD_3000;
      break;
    case 6000:
      data.dbp.baud_rate = DP_KBAUD_6000;
      break;
    case 12000:
      data.dbp.baud_rate = DP_KBAUD_12000;
      break;
    default:
      data.dbp.baud_rate = DP_KBAUD_1500;
      break;
  }

  data.dbp.tsl = swap16(op->Tsl);
  data.dbp.min_tsdr = swap16(op->MinTsdr);
  data.dbp.max_tsdr = swap16(op->MaxTsdr);
  data.dbp.tqui = op->Tqui;
  data.dbp.tset = op->Tset;
  data.dbp.ttr = swap32(op->Ttr);
  data.dbp.g = op->G;
  data.dbp.hsa = op->Hsa;
  data.dbp.max_retry_limit = op->MaxRetryLimit;
  data.dbp.bp_flag = op->BpFlag;
  data.dbp.min_slave_interval = swap16(op->MinSlaveInterval);
  data.dbp.poll_timeout = swap16(op->PollTimeout);
  data.dbp.data_control_time = swap16(op->DataControlTime);
  for (i=0; i<6; i++) 
    data.dbp.reserved[i] = 0;
  data.dbp.master_user_data_len = swap16(DP_MASTER_USER_DATA_LEN);
  for (i=0; i<32; i++) 
    data.dbp.master_class2_name[i] = 0;

  retval = pb_cmi_flush(fp);
  if (retval != PB_OK) return retval;

  retval = pb_cmi_write(fp, &sdb, &data, 72);
  if (retval != PB_OK) return retval;

  retval = pb_cmi_read(fp, NULL, NULL, NULL);
  return retval;
}


/*----------------------------------------------------------------------------*\
  Sends request for selecting operation mode to the Profiboard
\*----------------------------------------------------------------------------*/
static short act_param_loc(int fp, 
			   pwr_sClass_Pb_Profiboard *op, 
			   short arg) {
  T_PROFI_SERVICE_DESCR sdb;
  T_DP_ACT_PARAM_REQ apr;
  pwr_tUInt16 retval;
    
  /* Fill the service description block */
  sdb.comm_ref = 0;
  sdb.layer = DP;
  sdb.service = DP_ACT_PARAM_LOC;
  sdb.primitive = REQ;
  sdb.invoke_id = 0;
  sdb.result = 0;

  apr.rem_add = 0;
  apr.area_code = DP_AREA_SET_MODE;
  apr.activate = arg;
  apr.dummy = 0;

  retval = pb_cmi_flush(fp);
  if (retval != PB_OK) return retval;

  retval = pb_cmi_write(fp, &sdb, &apr, sizeof(T_DP_ACT_PARAM_REQ));
  if (retval != PB_OK) return retval;

  retval = pb_cmi_read(fp, NULL, NULL, NULL);
  return retval;
}


/*----------------------------------------------------------------------------*\
   Init method for the Pb_profiboard agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoAgentInit (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{
  io_sAgentLocal *local_master;
  pwr_sClass_Pb_Profiboard *op;
  pwr_tUInt16 sts;
  io_sRack *slavep;
  io_sRackLocal *local_slave;
  unsigned char devname[25];

  /* Allocate area for local data structure */
  local_master = calloc(1, sizeof(*local_master));
  ap->Local = local_master;

  op = (pwr_sClass_Pb_Profiboard *) ap->op;

  /* Open Pb driver */
  sprintf(devname, "/dev/pbus%1d", op->BusNumber);
  local_master->Pb_fp = open(devname, O_RDWR);

  if ( local_master->Pb_fp == -1)
  {
    /* Can't open driver */
    errh_Error( "ERROR init Pb Master %s", ap->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }


  /* Initialize CMI */

  if (op->Status < 1) {

    sts = pb_cmi_init(local_master->Pb_fp);
    if (sts != PB_OK) return IO__ERRINIDEVICE;

    /* Set FMB configuration */

    sts = fmb_set_configuration(local_master->Pb_fp,  op); 
    if (sts != PB_OK) return IO__ERRINIDEVICE;

    /* Set DP master parameters */

    sts = dp_init_master(local_master->Pb_fp,  op); 
    if (sts != PB_OK) return IO__ERRINIDEVICE;

    /* Set DP bus parameters */
    sts = dp_init_bus(local_master->Pb_fp,  op); 
    if (sts != PB_OK) return IO__ERRINIDEVICE;

    /* Move to STOP mode */
    sts = act_param_loc(local_master->Pb_fp, op, DP_OP_MODE_STOP);
    if (sts != PB_OK) errh_Info( "ERROR Init Pb Master %s", ap->Name );
  }

  op->Status = 1;

  slavep = ap->racklist;

  op->NumberSlaves = 0;

  while (slavep) {
    /* Allocate area for local data structure */
    local_slave = calloc(1, sizeof(*local_slave));
    slavep->Local = local_slave;
    local_slave->initialized = 0;
    op->NumberSlaves++;
    slavep = slavep->next;
  }

  errh_Info( "Init Pb Master %s", ap->Name );
  return IO__SUCCESS;
}



/*----------------------------------------------------------------------------*\
   Read method for the Pb_Profiboard agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoAgentRead (
  io_tCtx	ctx,
  io_sAgent	*ap
)
{
  io_sAgentLocal *local;
  pwr_sClass_Pb_Profiboard *op;
  pwr_tUInt16 sts;

  local = (io_sAgentLocal *) ap->Local;
  op = (pwr_sClass_Pb_Profiboard *) ap->op;

// The write method for Pb agent is used to move the board to the OPERATE state. The
// reason why this isn't done in the init routine is that at least one slave has
// to be configurated before the board can move from the STOP state.

  if (op->Status < 2 || op->Status > 3) {
    /* Move to CLEAR mode */
    sts = act_param_loc(local->Pb_fp, op, DP_OP_MODE_CLEAR);
    if (sts == PB_OK) op->Status = 2;
    errh_Info( "Pb Master, to CLEAR, %d", sts );

    /* Move to OPERATE mode, this actually starts the bus communication */
    sts = act_param_loc(local->Pb_fp, op, DP_OP_MODE_OPERATE);
    if (sts == PB_OK) op->Status = 3;
    errh_Info( "Pb Master, to OPERATE, %d", sts );
  }
 
  sts = pb_cmi_poll(local->Pb_fp, NULL, NULL, NULL);
 
  if (sts != PB_NO_CON_IND_RECEIVED) op->Diag[0]++;

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
  io_sAgentLocal *local;
  pwr_sClass_Pb_Profiboard *op;

  local = (io_sAgentLocal *) ap->Local;
  op = (pwr_sClass_Pb_Profiboard *) ap->op;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoAgentClose (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{
  io_sAgentLocal 	*local;

  /* Close Pb driver */
  local = ap->Local;

  close( local->Pb_fp);
  free( (char *)local);

  return IO__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_Profiboard) = {
  pwr_BindIoMethod(IoAgentInit),
  pwr_BindIoMethod(IoAgentRead),
  pwr_BindIoMethod(IoAgentWrite),
  pwr_BindIoMethod(IoAgentClose),
  pwr_NullMethod
};
