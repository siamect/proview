/* rt_io_m_pb_profiboard.c -- io methods for the profibus master object
   The PbMaster object serves as agent for one Profibus DP bus
   The board we use is Profiboard from Softing

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
#include "co_cdh.h"
#include "pwr_baseclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "rt_io_agent_init.h"

#include "rt_io_profiboard.h"

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
  data.dp.max_slave_diag_entries = op->MaxNumberSlaves;
  data.dp.max_slave_diag_len = 244;
  data.dp.max_bus_para_len = 1024;
  data.dp.max_slave_para_len = 1024;

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
  Initializes one DP slave in the master card
\*----------------------------------------------------------------------------*/
static pwr_tStatus init_dp_slave (
  io_sAgent	*ap,
  pwr_tObjid	oid
)
{
  
  pwr_sClass_Pb_DP_Slave *op;
  io_sAgentLocal *local_agent;
  int i;
  char name[196];
  pwr_tUInt16 sts;
  pwr_tStatus status;
  struct timespec rqtp, rmtp;

  status = gdh_ObjidToPointer(oid, (pwr_tAddress *) &op);
  status = gdh_ObjidToName(oid, (char *) &name, sizeof(name), cdh_mNName);
  
  local_agent = (io_sAgentLocal *) (ap->Local);

  op->Status = PB_SLAVE_STATE_NOTINIT;
 
  errh_Info( "Config of Profibus DP slave %s", name );
 
  // Try to initialize slave, make three attempts before we give up

  for(i=0; i<3; i++) {
    if (op->AutoConfigure == 1) {
      sts = pb_get_slave_cfg(local_agent->Pb_fp,
		       op->SlaveAddress,
		       &op->ConfigDataLen,
		       &op->ConfigData);
    }
    else {
      sts = PB_OK;
    }
    if (sts == PB_OK) {
      sts = pb_download_all(local_agent->Pb_fp,
		      op->SlaveAddress,
		      op->WdFact1,
		      op->WdFact2,
		      0,
		      op->PNOIdent,
		      op->GroupIdent,
		      op->PrmUserDataLen,
		      &op->PrmUserData,
		      op->ConfigDataLen,
		      &op->ConfigData);
    }
    if (sts == PB_OK) {
      sts = pb_get_slave_info(local_agent->Pb_fp, 
			op->SlaveAddress, 
			&op->OffsetInputs,
			&op->OffsetOutputs,
			&op->BytesOfInput,
			&op->BytesOfOutput);
    }
    if (sts == PB_OK) break;
    rqtp.tv_sec = 1;
    rqtp.tv_nsec = 0;
    nanosleep(&rqtp, &rmtp);
  }
  if (sts != PB_OK) {
    errh_Info( "ERROR Init Profibus DP slave %s", name);
    return IO__ERRINIDEVICE;
  }

  op->Status = PB_SLAVE_STATE_STOPPED;
  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Init method for the Pb_profiboard agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus IoAgentInit (
  io_tCtx	ctx,
  io_sAgent	*ap
) 
{
  pwr_sClass_Pb_Profiboard *op;
  pwr_tUInt16 sts;
  pwr_tStatus status;
  io_sAgentLocal *local;
  unsigned char devname[25];
  
  pwr_tObjid slave_objid;
  pwr_tClassId slave_class;

  op = (pwr_sClass_Pb_Profiboard *) ap->op;

  errh_Info( "Config of Profibus DP Master %s", ap->Name );

  /* Allocate area for local data structure */
  ap->Local = calloc(1, sizeof(io_sAgentLocal));
  if (!ap->Local) {
    errh_Info( "ERROR config Profibus DP Master %s - %s", ap->Name, "calloc");
    return IO__ERRINIDEVICE;
  }
    
  local = (io_sAgentLocal *) ap->Local;
      
  if (op->DisableBus != 1) {
  
    /* Open Pb driver */
    sprintf(devname, "/dev/pbus%1d", op->BusNumber);
    local->Pb_fp = open(devname, O_RDWR);

    if (local->Pb_fp == -1)
    {
      /* Can't open driver */
      errh_Info( "ERROR config Profibus DP Master %s - %s", ap->Name, "open device");
      ctx->Node->EmergBreakTrue = 1;
      return IO__ERRDEVICE;
    }

    /* Initialize CMI */

    sts = pb_cmi_init(local->Pb_fp);
    if (sts != PB_OK) {
      errh_Info( "ERROR config Profibus DP Master %s - %s", ap->Name, "cmi init");
      return IO__ERRINIDEVICE;
    }

    /* Set FMB configuration */

    sts = fmb_set_configuration(local->Pb_fp,  op); 
    if (sts != PB_OK) {
      errh_Info( "ERROR config Profibus DP  Master %s - %s", ap->Name, "fmb set configuration");
      return IO__ERRINIDEVICE;
    }

    /* Set DP master parameters */

    sts = dp_init_master(local->Pb_fp,  op); 
    if (sts != PB_OK) {
      errh_Info( "ERROR config Profibus DP Master %s - %s", ap->Name, "dp init master");
      return IO__ERRINIDEVICE;
    }

    /* Set DP bus parameters */
    sts = dp_init_bus(local->Pb_fp,  op); 
    if (sts != PB_OK) {
      errh_Info( "ERROR config Profibus DP Master %s - %s", ap->Name, "dp init bus");
      return IO__ERRINIDEVICE;
    }

    /* Move to STOP mode */
    sts = act_param_loc(local->Pb_fp, op, DP_OP_MODE_STOP);
    if (sts != PB_OK) {
      errh_Info( "ERROR config Profibus DP Master %s - %s", ap->Name, "act param loc to STOPPED");
      return IO__ERRINIDEVICE;
    }

    op->Status = PB_MASTER_STATE_STOPPED;

    /* Loop through all slaves (traverse agent's children) and initialize them */

    op->NumberSlaves = 0;
    status = gdh_GetChild(ap->Objid, &slave_objid);
  
    while (ODD(status)) {
      status = gdh_GetObjectClass(slave_objid, &slave_class);
      if (slave_class == pwr_cClass_Pb_DP_Slave) {
        status = init_dp_slave(ap, slave_objid);
        op->NumberSlaves++;
      }
      status = gdh_GetNextSibling(slave_objid, &slave_objid);
    }

    /* Move to CLEAR and OPERATE mode */
  
    sts = act_param_loc(local->Pb_fp, op, DP_OP_MODE_CLEAR);
    if (sts == PB_OK) {
      op->Status = PB_MASTER_STATE_CLEARED;
      sts = act_param_loc(local->Pb_fp, op, DP_OP_MODE_OPERATE);
      if (sts == PB_OK) {
        op->Status = PB_MASTER_STATE_OPERATE;
        errh_Info( "Profibus DP Master %s to state OPERATE", ap->Name);
      }
      else {
        errh_Info( "ERROR config Profibus DP Master %s - %s", ap->Name, "act param loc to OPERATE");
        return IO__ERRINIDEVICE;
      }    
    }
    else {
      errh_Info( "ERROR config Profibus DP Master %s - %s", ap->Name, "act param loc to CLEAR");
      return IO__ERRINIDEVICE;
    }
    
  }    
  
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

  /* If everything is fine we should be in state OPERATE
     Make a poll to see if there are diagnostics, the answer also tell us
     if there are any hardware faults. In that case, make a reset and a new init. */

  if (op->DisableBus != 1) {
  
    switch (op->Status) {
  
      case PB_MASTER_STATE_OPERATE:
        sts = pb_cmi_poll(local->Pb_fp, NULL, NULL, NULL);
 
        /* In case of device error, move to state NOTINIT */
        if (sts == PB_DEVICE_ERROR) {
          op->Status = PB_MASTER_STATE_NOTINIT;
        }
        /* In case of diagnostic message, just mark it.
           in the future, take care of it */
        else if (sts != PB_NO_CON_IND_RECEIVED) {
          op->Diag[0]++;
        }
        break;

      default:
        op->Status = PB_MASTER_STATE_NOTINIT;
        errh_Info( "Reconfig of Profibus DP Master %s", ap->Name );      
        IoAgentClose(ctx, ap);
        IoAgentInit(ctx, ap);
        break;
    }
 
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
