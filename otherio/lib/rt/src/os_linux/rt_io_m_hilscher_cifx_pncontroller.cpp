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

/* rt_io_m_hilscher_cifx_pncontroller.cpp -- I/O methods for class Hilscher_cifX_PnController. */


#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "pwr_profibusclasses.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "rt_io_base.h"
#include "rt_io_bus.h"
#include "rt_io_agent_init.h"
#include "rt_io_agent_close.h"
#include "rt_io_agent_read.h"
#include "rt_io_agent_write.h"
#include "rt_io_pn_locals.h"
#include "rt_io_msg.h"
#include "rt_pb_msg.h"

#if defined PWRE_CONF_CIFX


#define UUID_T TLR_UUID_T
#define __PACKED_PRE
#define __PACKED_POST __attribute__ ((packed))
#define PNM_CFG_IPFLAG_IP_ADDR_USED 1
#define PNM_CFG_IPFLAG_NETMASK_USED 2
#define PNM_CFG_IPFLAG_GATEWAY_USED 4

#include "cifxlinux.h"
#include "cifXEndianess.h"

#include "rcX_Public.h"
#include "RPCTask_Public.h"
#include "PNM_PNIOAPCTL_Public.h"
#include "PNM_PNIOAPCFG_Public.h"

#include "rt_io_m_hilscher_cifx.h"
#include "rt_pn_gsdml_data.h"

#define CIFX_DEV "cifX0"
#define PN_SRC 444
#define PN_SRCID 0

#define PNM_APPLICATION RCX_PACKET_DEST_DEFAULT_CHANNEL

union t_addr {
  unsigned int m;
  unsigned char b[4];
};

// One common handle to the driver
static CIFXHANDLE driver = 0;
static int msg_id = 1;

static void get_diag( pwr_sClass_Hilscher_cifX_Diag *diag, CIFXHANDLE chan)
{
  int32_t sts;
  uint32_t state;


  CIFXHANDLE sysdevice = NULL;
  sts = xSysdeviceOpen( driver, (char *)CIFX_DEV, &sysdevice);
  if ( sts == CIFX_NO_ERROR) {
    SYSTEM_CHANNEL_SYSTEM_STATUS_BLOCK  statusblock = {0};

    sts = xSysdeviceInfo( sysdevice, CIFX_INFO_CMD_SYSTEM_STATUS_BLOCK, sizeof(statusblock), 
			  &statusblock);
    if ( sts == CIFX_NO_ERROR) {
      diag->SystemStatus = statusblock.ulSystemStatus;
      diag->SystemError = statusblock.ulSystemError;
      diag->TimeSinceStart.tv_sec = statusblock.ulTimeSinceStart;
      diag->CpuLoad = (float) statusblock.usCpuLoad / 100.0;
    }          
  }

  NETX_COMMON_STATUS_BLOCK csb = {0};

  sts = xChannelCommonStatusBlock( chan, CIFX_CMD_READ_DATA, 0, sizeof(csb), &csb);
  if ( sts == CIFX_NO_ERROR) {
    diag->CommState = csb.ulCommunicationState;
    diag->CommError = csb.ulCommunicationError;
    diag->ErrorCount = csb.ulErrorCount;
    diag->ConfigSlaves = csb.uStackDepended.tMasterStatusBlock.ulNumOfConfigSlaves;
    diag->ActiveSlaves = csb.uStackDepended.tMasterStatusBlock.ulNumOfActiveSlaves;
    diag->SlaveState = csb.uStackDepended.tMasterStatusBlock.ulSlaveState;
  }

  sts = xChannelHostState( chan, CIFX_HOST_STATE_READ, &state, 0);
  if ( sts == CIFX_NO_ERROR) {
    diag->HostState = state;
  }

  sts = xChannelBusState( chan, CIFX_BUS_STATE_GETSTATE, &state, 0);
  if ( sts == CIFX_NO_ERROR) {
    diag->BusState = state;
  }

}

static bool status_check( pwr_sClass_Hilscher_cifX_PnController *op,
			  io_sAgent *ap, int sts, const char *text)
{
  if ( sts == CIFX_NO_ERROR)
    return true;

  xDriverGetErrorDescription( sts, op->ErrorStr, sizeof(op->ErrorStr));
  op->Status = sts;
  if ( text)
    errh_Error( "IO init %s, '%s', %s", ap->Name, op->ErrorStr, text);
  else
    errh_Error( "IO init %s, '%s'", ap->Name, op->ErrorStr);
  return false;
}

static bool cmd_check( pwr_sClass_Hilscher_cifX_PnController *op,
		       io_sAgent *ap, unsigned int cmd1, unsigned int cmd2)
{
  if ( cmd1 == cmd2)
    return true;

  strcpy( op->ErrorStr, "Unexpected message received");
  errh_Error( "IO init %s, '%s'", ap->Name, op->ErrorStr);
  return false;
}


static pwr_tStatus IoAgentInit( io_tCtx ctx,
				io_sAgent *ap)
{
  int sts;
  io_sLocalHilscher_cifX_PnController *local;
  pwr_sClass_Hilscher_cifX_PnController *op = (pwr_sClass_Hilscher_cifX_PnController *)ap->op;

  local = (io_sLocalHilscher_cifX_PnController *) calloc( 1, sizeof(io_sLocalHilscher_cifX_PnController));
  ap->Local = local;

  strcpy( op->ErrorStr, "");
  op->Status = 0;

  if ( driver == 0) {
    struct CIFX_LINUX_INIT init;

    memset( &init, 0, sizeof(init));
    init.init_options  = CIFX_DRIVER_INIT_AUTOSCAN;
    init.trace_level   = 255;

    sts = cifXDriverInit( &init);
    if ( !status_check( op, ap, sts, "cifXDriverInit"))
      return IO__INITFAIL;

    sts = xDriverOpen( &driver);
    if ( !status_check( op, ap, sts, "xDriverOpen"))
      return IO__INITFAIL;
  }
  
  // Find the board
  unsigned long board = 0;
  BOARD_INFORMATION boardinfo;
  boardinfo.lBoardError = 0;
  int found = 0;

  while ( xDriverEnumBoards( driver, board, sizeof(boardinfo), &boardinfo) 
	  == CIFX_NO_ERROR) {
    if ( cdh_NoCaseStrcmp( boardinfo.abBoardAlias, op->Alias) == 0) {
      found = 1;
      break;
    }
    board++;
  }
  if ( !found) {
    sprintf( op->ErrorStr, "Board with alias \"%s\" not found", op->Alias);
    errh_Error( "IO init %s, '%s'", ap->Name, op->ErrorStr);
    return IO__INITFAIL;
  }

  op->Diag.DeviceNumber = boardinfo.tSystemInfo.ulDeviceNumber;
  op->Diag.SerialNumber = boardinfo.tSystemInfo.ulSerialNumber;
  op->Diag.SystemError = boardinfo.ulSystemError;
  //op->Diag.VendorId = boardinfo.tSystemInfo.usManufacturer;

  local->channel = 0;

  local->chan = NULL;

  sts = xChannelOpen( NULL, (char *)CIFX_DEV, local->channel, &local->chan);
  if ( !status_check( op, ap, sts, "xChannelOpen"))
    return IO__INITFAIL;
  
  uint32_t state;
  strcpy( op->ErrorStr, "Device host state not ready");
  for (;;) {
    sts = xChannelHostState( local->chan, CIFX_HOST_STATE_READY, &state, 100);
    if ( sts != CIFX_DEV_NOT_READY)
      break;
    sleep(1);
  }
  strcpy( op->ErrorStr, "");

  sts = xChannelReset( local->chan, CIFX_SYSTEMSTART, 2000);
  if ( sts != CIFX_NO_ERROR) {
    printf( "xChannelReset: 0x%08x\n", sts);
  }

  for (;;) {
    sts = xChannelHostState( local->chan, CIFX_HOST_STATE_READY, &state, 100);
    if ( sts != CIFX_DEV_NOT_READY)
      break;
    sleep(1);
  }

  CHANNEL_INFORMATION channelinfo = {{0}};
  sts = xDriverEnumChannels( driver, board, local->channel, sizeof(channelinfo), &channelinfo);
  if ( sts == CIFX_NO_ERROR) {
    strncpy( op->Diag.FirmwareName, (char *)channelinfo.abFWName, sizeof(op->Diag.FirmwareName)); 
    snprintf( op->Diag.FirmwareVersion, sizeof(op->Diag.FirmwareVersion), 
	      "%u.%u.%u-%u (%4u-%02hu-%02hu)", channelinfo.usFWMajor, channelinfo.usFWMinor,
	      channelinfo.usFWBuild, channelinfo.usFWRevision, channelinfo.usFWYear,
	      channelinfo.bFWMonth, channelinfo.bFWDay);
  }

  // Register application
  RCX_REGISTER_APP_REQ_T regapp = {{0}};
  RCX_REGISTER_APP_CNF_T regapp_cnf = {{0}};

  regapp.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
  regapp.tHead.ulLen    = 0;
  regapp.tHead.ulCmd    = HOST_TO_LE32(RCX_REGISTER_APP_REQ);
  regapp.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
  regapp.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);

  sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&regapp, 10);
  if ( !status_check( op, ap, sts, "xChannelPutPacket"))
    return IO__INITFAIL;
    
  sts = xChannelGetPacket( local->chan, sizeof(regapp_cnf), (CIFX_PACKET *)&regapp_cnf, 1000);
  if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
       !cmd_check( op, ap, regapp_cnf.tHead.ulCmd, RCX_REGISTER_APP_CNF) ||
       !status_check( op, ap, regapp_cnf.tHead.ulSta, "Register Application"))
    return IO__INITFAIL;

  RCX_LOCK_UNLOCK_CONFIG_REQ_T unlock = {{0}};
  RCX_LOCK_UNLOCK_CONFIG_CNF_T unlock_cnf = {{0}};

  unlock.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
  unlock.tHead.ulLen    = HOST_TO_LE32(sizeof(unlock.tData));
  unlock.tHead.ulCmd    = HOST_TO_LE32(RCX_LOCK_UNLOCK_CONFIG_REQ);
  unlock.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
  unlock.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
  unlock.tData.ulParam  = 2; // Unlock

  sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&unlock, 10);
  if ( !status_check( op, ap, sts, "xChannelPutPacket"))
    return IO__INITFAIL;

  sts = xChannelGetPacket( local->chan, sizeof(unlock_cnf), (CIFX_PACKET *)&unlock_cnf, 20);
  if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
       !cmd_check( op, ap, unlock_cnf.tHead.ulCmd, RCX_LOCK_UNLOCK_CONFIG_CNF) ||
       !status_check( op, ap, unlock_cnf.tHead.ulSta, "Unlock channel"))
    return IO__INITFAIL;

  // Delete configuration, This doesn't work....
#if 0
  RCX_DELETE_CONFIG_REQ_T delete_msg;
  RCX_DELETE_CONFIG_CNF_T delete_cnf = {{0}};

  memset( &delete_msg, 0, sizeof(delete_msg));

  delete_msg.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
  delete_msg.tHead.ulLen    = 0;
  delete_msg.tHead.ulId     = HOST_TO_LE32(msg_id++);
  delete_msg.tHead.ulCmd    = HOST_TO_LE32(RCX_DELETE_CONFIG_REQ);
  delete_msg.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
  delete_msg.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);

  sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&delete_msg, 10);
  if ( !status_check( op, ap, sts, "xChannelPutPacket"))
    return IO__INITFAIL;

  sts = xChannelGetPacket( local->chan, sizeof(delete_cnf), (CIFX_PACKET *)&delete_cnf, 20);
  if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
       !cmd_check( op, ap, delete_cnf.tHead.ulCmd, RCX_DELETE_CONFIG_CNF) ||
       !status_check( op, ap, delete_cnf.tHead.ulSta, "Delete Config"))
    return IO__INITFAIL;

#endif

  // char op_ip_address[20] = "10.0.0.1";
  // char op_subnet_mask[20] = "255.255.255.0";
  // char op_device_name[40] = "controller";
  // char op_device_type[40] = "cifX RE/PNM";
  t_addr ipaddress;
  t_addr subnetmask;
  int num;

  num = sscanf( op->IP_Address, "%hhu.%hhu.%hhu.%hhu", 
	  &ipaddress.b[3], &ipaddress.b[2], &ipaddress.b[1], &ipaddress.b[0]);
  if ( num != 4) {
    sprintf( op->ErrorStr, "IP Address syntax error, %s", ap->Name);
    return IO__INITFAIL;
  }
  num = sscanf( op->SubnetMask, "%hhu.%hhu.%hhu.%hhu", 
	  &subnetmask.b[3], &subnetmask.b[2], &subnetmask.b[1], &subnetmask.b[0]);
  if ( num != 4) {
    sprintf( op->ErrorStr, "SubnetMask syntax error, %s", ap->Name);
    return IO__INITFAIL;
  }


  PNM_APCFG_CFG_PNM_REQ_T pnm_config = {{0}};
  PNM_APCFG_CFG_PNM_CNF_T pnm_config_cnf;

  memset( &pnm_config, 0, sizeof(pnm_config));
  memset( &pnm_config_cnf, 0, sizeof(pnm_config_cnf));

  pnm_config.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
  pnm_config.tHead.ulDestId = 0;
  pnm_config.tHead.ulLen    = HOST_TO_LE32(sizeof(pnm_config.tData));
  pnm_config.tHead.ulId     = HOST_TO_LE32(msg_id++);
  pnm_config.tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_PNM_REQ);
  pnm_config.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
  pnm_config.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
  pnm_config.tData.tSubHead.ulTrCntrId = 0;
  pnm_config.tData.tSubHead.ulTrDevId = 0;
  pnm_config.tData.tSubHead.ulTrApId = 0;
  pnm_config.tData.tSubHead.ulTrModId = 0;
  pnm_config.tData.tSubHead.ulTrSubModId = 0;
  pnm_config.tData.tSubHead.ulTrIdCnt = 1;
  pnm_config.tData.atData[0].ulTrId   = 1;
  pnm_config.tData.atData[0].ulSystemFlags = PNM_APCFG_STARTMODE_APPLICATION;
  pnm_config.tData.atData[0].ulWdgTime = 1000;
  strncpy( (char *)pnm_config.tData.atData[0].abTypeOfStation, op->DeviceType,
	   sizeof(pnm_config.tData.atData[0].abTypeOfStation));
  pnm_config.tData.atData[0].usTypeOfStationLen = 
    strlen((char *)pnm_config.tData.atData[0].abTypeOfStation);
  strncpy( (char *)pnm_config.tData.atData[0].abNameOfStation, op->DeviceName,
	  sizeof(pnm_config.tData.atData[0].abNameOfStation));
  pnm_config.tData.atData[0].usNameOfStationLen = 
    strlen((char *)pnm_config.tData.atData[0].abNameOfStation);
  pnm_config.tData.atData[0].usVendorID  = 0x011E; /* Hilscher Profinet VendorID */
  pnm_config.tData.atData[0].usDeviceID  = 0x0203; /* Hilscher cifX DeviceID for IO-Controller */
  pnm_config.tData.atData[0].ulIPAddr    = ipaddress.m;
  pnm_config.tData.atData[0].ulNetmask   = subnetmask.m;
  pnm_config.tData.atData[0].ulGateway   = 0;
  pnm_config.tData.atData[0].ulIPFlags   = 0;
  // These are in the 'EXT' message
  // pnm_config.tData.atData[0].usMAUTypePort0 = PNM_APCFG_PNM_MAUTYPE_AUTO;
  // pnm_config.tData.atData[0].usMAUTypePort1 = PNM_APCFG_PNM_MAUTYPE_AUTO;
  // pnm_config.tData.atData[0].ulStructVersion = PNM_APCFG_CFG_PNM_STRUCT_VERS_1;

  
  sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&pnm_config, 10);
  if ( !status_check( op, ap, sts, "xChannelPutPacket"))
    return IO__INITFAIL;

  sts = xChannelGetPacket( local->chan, sizeof(pnm_config_cnf), (CIFX_PACKET *)&pnm_config_cnf, 20);
  if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
       !cmd_check( op, ap, pnm_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_PNM_CNF) ||
       !status_check( op, ap, pnm_config_cnf.tHead.ulSta, "Controller Config"))
    return IO__INITFAIL;
    

  // Device loop

  int device = 0;

  int input_dpm_offset = 0;
  int output_dpm_offset = 0;
  int prev_input_dpm_offset = 0;
  int prev_output_dpm_offset = 0;
  io_sRack *rp;
  io_sCard *cp;

  for ( rp = ap->racklist; rp; rp = rp->next) {
    io_sPnRackLocal *rp_local;

    rp_local = (io_sPnRackLocal *) calloc( 1, sizeof(io_sPnRackLocal));
    rp->Local = rp_local;

    for ( cp = rp->cardlist; cp; cp = cp->next)
      cp->Local = calloc( 1, sizeof(io_sPnCardLocal));

    // Read device xml-file
    t_addr ipaddress;
    t_addr subnetmask;
    unsigned char macaddr[6];
  
    GsdmlDeviceData  *dev_data;
    dev_data = new GsdmlDeviceData;
    pwr_tFileName fname;
    
    sprintf( fname, "$pwrp_load/pwr_pn_%s.xml", cdh_ObjidToFnString(NULL, rp->Objid ));
    dcli_translate_filename( fname, fname);

    sts = dev_data->read( fname);
    if ( EVEN(sts)) {
      snprintf( op->ErrorStr, sizeof(op->ErrorStr), "Missing device xml file, %s", rp->Name);
      return IO__INITFAIL;
    }

    // PNM_IOD

    sscanf( dev_data->ip_address, "%hhu.%hhu.%hhu.%hhu", 
	    &ipaddress.b[3], &ipaddress.b[2], &ipaddress.b[1], &ipaddress.b[0]);
    sscanf( dev_data->subnet_mask, "%hhu.%hhu.%hhu.%hhu", 
	    &subnetmask.b[3], &subnetmask.b[2], &subnetmask.b[1], &subnetmask.b[0]);


    PNM_APCFG_CFG_IOD_REQ_T iod_config = {{0}};
    PNM_APCFG_CFG_IOD_CNF_T iod_config_cnf = {{0}};

    memset( &iod_config, 0, sizeof(iod_config));

    iod_config.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
    iod_config.tHead.ulLen    = HOST_TO_LE32(sizeof(iod_config.tData));
    iod_config.tHead.ulId     = HOST_TO_LE32(msg_id++);
    iod_config.tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_IOD_REQ);
    iod_config.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
    iod_config.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
    iod_config.tData.tSubHead.ulTrCntrId = 1;
    iod_config.tData.tSubHead.ulTrDevId = 0;
    iod_config.tData.tSubHead.ulTrApId = 0;
    iod_config.tData.tSubHead.ulTrModId = 0;
    iod_config.tData.tSubHead.ulTrSubModId = 0;
    iod_config.tData.tSubHead.ulTrIdCnt = 1;
    iod_config.tData.atData[0].ulTrId   = device + 1;
    iod_config.tData.atData[0].ulFlags = PNM_APCFG_IOD_FLAG_IOD_ACTIVE;
    strncpy( (char *)iod_config.tData.atData[0].abNameOfStation, dev_data->device_name,
	     sizeof(iod_config.tData.atData[0].abNameOfStation));
    iod_config.tData.atData[0].usNameOfStationLen = 
      strlen((char *)iod_config.tData.atData[0].abNameOfStation);
    iod_config.tData.atData[0].usVendorID  = dev_data->vendor_id;
    iod_config.tData.atData[0].usDeviceID  = dev_data->device_id;
    iod_config.tData.atData[0].ulIPAddr    = ipaddress.m;
    iod_config.tData.atData[0].ulNetworkMask = subnetmask.m;
    iod_config.tData.atData[0].ulGatewayAddr = 0;
    iod_config.tData.atData[0].ulArUuidData1 = device + 1;
    iod_config.tData.atData[0].usArUuidData2 = device + 1;
    iod_config.tData.atData[0].usArUuidData3 = device + 1;
    iod_config.tData.atData[0].usARType    = PNIO_API_AR_TYPE_SINGLE;
    iod_config.tData.atData[0].ulARProp    = PNIO_API_AR_PROP_SUPERVISOR_NONE | 
      PNIO_API_AR_PROP_STATE_PRIMARY | PNIO_API_AR_PROP_SINGLE_AR | ( 1 << 4) | (1 << 5);
    iod_config.tData.atData[0].usAlarmCRType = PNIO_API_ALCR_TYPE_ALARM;
    iod_config.tData.atData[0].ulAlarmCRProp = PNIO_API_ALCR_PROP_PRIO_DEFAULT;
    iod_config.tData.atData[0].usAlarmCRVLANID = 0;
    iod_config.tData.atData[0].ulIPFlags   = 0;
    iod_config.tData.atData[0].usRTATimeoutFact = 1;
    iod_config.tData.atData[0].usRTARetries = PNIO_API_RTA_RETRIES_MIN;
    iod_config.tData.atData[0].usObjUUIDInst = dev_data->instance;
    
    sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&iod_config, 10);
    if ( !status_check( op, ap, sts, "xChannelPutPacket"))
      return IO__INITFAIL;

    sts = xChannelGetPacket( local->chan, sizeof(iod_config_cnf), (CIFX_PACKET *)&iod_config_cnf, 20);
    if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
	 !cmd_check( op, ap, iod_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_IOD_CNF) ||
	 !status_check( op, ap, pnm_config_cnf.tHead.ulSta, "Device Config"))
      return IO__INITFAIL;
    
    // PNM_IOCR

    // Calculate data length
    unsigned int io_input_length = 0;
    unsigned int io_output_length = 0;
    unsigned int io_iocr_input_length = 0;
    unsigned int io_iocr_output_length = 0;

    for ( unsigned int i = 0; i < dev_data->slot_data.size(); i++) {
      for ( unsigned int j = 0; j < dev_data->slot_data[i]->subslot_data.size(); j++) {
	if ( dev_data->slot_data[i]->subslot_data[j]->io_input_length > 0) {
	  io_input_length += dev_data->slot_data[i]->subslot_data[j]->io_input_length + 1;
	  io_iocr_input_length += 1;
	}
	if ( dev_data->slot_data[i]->subslot_data[j]->io_output_length > 0) {
	  io_output_length += dev_data->slot_data[i]->subslot_data[j]->io_output_length + 1;
	  io_iocr_output_length += 1;
	}
	if ( dev_data->slot_data[i]->subslot_data[j]->io_input_length == 0 &&
	     dev_data->slot_data[i]->subslot_data[j]->io_output_length == 0) {
	  io_input_length += 1;
	  io_iocr_input_length += 1;
	}
      }
    }

    PNM_APCFG_CFG_IOCR_REQ_T *iocr_config = 
      (PNM_APCFG_CFG_IOCR_REQ_T *) calloc( 1, sizeof(*iocr_config) + sizeof(iocr_config->tData.atData));
    PNM_APCFG_CFG_IOCR_CNF_T iocr_config_cnf = {{0}};

    sscanf( dev_data->mac_address, "%2hhx-%2hhx-%2hhx-%2hhx-%2hhx-%2hhx", 
	    &macaddr[0], &macaddr[1], &macaddr[2], &macaddr[3], &macaddr[4], &macaddr[5]);

    iocr_config->tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
    iocr_config->tHead.ulLen    = HOST_TO_LE32(sizeof(iocr_config->tData)+sizeof(iocr_config->tData.atData));
    iocr_config->tHead.ulId     = HOST_TO_LE32(msg_id++);
    iocr_config->tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_IOD_IOCR_REQ);
    iocr_config->tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
    iocr_config->tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
    iocr_config->tData.tSubHead.ulTrCntrId = 1;
    iocr_config->tData.tSubHead.ulTrDevId = device + 1;
    iocr_config->tData.tSubHead.ulTrApId = 0;
    iocr_config->tData.tSubHead.ulTrModId = 0;
    iocr_config->tData.tSubHead.ulTrSubModId = 0;
    iocr_config->tData.tSubHead.ulTrIdCnt = 2;
    for ( int i = 0; i < 2; i++) {
      iocr_config->tData.atData[i].ulTrId   = i + 1;
      iocr_config->tData.atData[i].usType = dev_data->iocr_data[i?0:1]->type;
      iocr_config->tData.atData[i].usVLANID = 0;
      iocr_config->tData.atData[i].ulProp = PNIO_API_IOCR_PROP_RTCLASS_DATA1;
      // strncpy( (char *)iocr_config->tData.atData[i].abMcastMACAddr, (char *)macaddr,
      //	     sizeof(iocr_config->tData.atData[i].abMcastMACAddr));
      memset( (char *)iocr_config->tData.atData[i].abMcastMACAddr, 0,
	      sizeof(iocr_config->tData.atData[i].abMcastMACAddr));
      if ( iocr_config->tData.atData[i].usType == 1)
	iocr_config->tData.atData[i].usDataLen = io_input_length + io_iocr_output_length;
      else
	iocr_config->tData.atData[i].usDataLen = io_output_length + io_iocr_input_length;
      iocr_config->tData.atData[i].usSendClockFact = dev_data->iocr_data[i?0:1]->send_clock_factor;
      iocr_config->tData.atData[i].usReductRatio = dev_data->iocr_data[i?0:1]->reduction_ratio;
      iocr_config->tData.atData[i].usPhase = dev_data->iocr_data[i?0:1]->phase;
      iocr_config->tData.atData[i].usSequ = 0;
      iocr_config->tData.atData[i].ulFrameSendOffs = PNIO_API_FRAME_SEND_OFFSET_DEFAULT;
      iocr_config->tData.atData[i].usWatchdogFact = PNIO_API_CYCLIC_WATCHDOG_DEFAULT;
      iocr_config->tData.atData[i].usDataHoldFact = PNIO_API_CYCLIC_DATAHOLD_DEFAULT;
    }
  
    sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)iocr_config, 10);
    if ( !status_check( op, ap, sts, "xChannelPutPacket"))
      return IO__INITFAIL;

    sts = xChannelGetPacket( local->chan, sizeof(iocr_config_cnf), (CIFX_PACKET *)&iocr_config_cnf, 20);
    if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
	 !cmd_check( op, ap, iocr_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_IOD_IOCR_CNF) ||
	 !status_check( op, ap, pnm_config_cnf.tHead.ulSta, "IOCR Config"))
      return IO__INITFAIL;
    
    free( iocr_config);


    // PNM_IOD_AP

    PNM_APCFG_CFG_AP_REQ_T ap_config = {{0}};
    PNM_APCFG_CFG_AP_CNF_T ap_config_cnf = {{0}};

    ap_config.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
    ap_config.tHead.ulLen    = HOST_TO_LE32(sizeof(ap_config.tData));
    ap_config.tHead.ulId     = HOST_TO_LE32(msg_id++);
    ap_config.tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_IOD_AP_REQ);
    ap_config.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
    ap_config.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
    ap_config.tData.tSubHead.ulTrCntrId = 1;
    ap_config.tData.tSubHead.ulTrDevId = device + 1;
    ap_config.tData.tSubHead.ulTrApId = 0;
    ap_config.tData.tSubHead.ulTrModId = 0;
    ap_config.tData.tSubHead.ulTrSubModId = 0;
    ap_config.tData.tSubHead.ulTrIdCnt = 1;
    ap_config.tData.atData[0].ulTrId   = 1;
    ap_config.tData.atData[0].ulAPI = 0;
  
    sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&ap_config, 10);
    if ( !status_check( op, ap, sts, "xChannelPutPacket"))
      return IO__INITFAIL;

    sts = xChannelGetPacket( local->chan, sizeof(ap_config_cnf), (CIFX_PACKET *)&ap_config_cnf, 20);
    if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
	 !cmd_check( op, ap, ap_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_IOD_AP_CNF) ||
	 !status_check( op, ap, ap_config_cnf.tHead.ulSta, "Application Process"))
      return IO__INITFAIL;
    

    // PNM_MODULE

    // Check number of configured slots
    int slots = 0;
    for ( unsigned int i = 0; i < dev_data->slot_data.size(); i++) {
      if ( dev_data->slot_data[i]->module_ident_number == 0)
	break;
      slots++;
    }

    PNM_APCFG_CFG_MODULE_REQ_T *module_config = 
      (PNM_APCFG_CFG_MODULE_REQ_T *) calloc( 1, sizeof(*module_config) + (slots - 1) * sizeof(module_config->tData.atData));
    PNM_APCFG_CFG_MODULE_CNF_T module_config_cnf = {{0}};
    
    module_config->tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
    module_config->tHead.ulLen    = HOST_TO_LE32(sizeof(module_config->tData)+(slots-1)*sizeof(module_config->tData.atData));
    module_config->tHead.ulId     = HOST_TO_LE32(msg_id++);
    module_config->tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_IOD_MODULE_REQ);
    module_config->tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
    module_config->tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
    module_config->tData.tSubHead.ulTrCntrId = 1;
    module_config->tData.tSubHead.ulTrDevId = device + 1;
    module_config->tData.tSubHead.ulTrApId = 1;
    module_config->tData.tSubHead.ulTrModId = 0;
    module_config->tData.tSubHead.ulTrSubModId = 0;
    module_config->tData.tSubHead.ulTrIdCnt = slots;
    for ( int i = 0; i < slots; i++) {
      module_config->tData.atData[i].ulTrId   = i + 1;
      module_config->tData.atData[i].ulModuleID = dev_data->slot_data[i]->module_ident_number;
      module_config->tData.atData[i].usModuleProp = 0;
      module_config->tData.atData[i].usSlotNumber = dev_data->slot_data[i]->slot_number;
    }
    
    sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)module_config, 10);
    if ( !status_check( op, ap, sts, "xChannelPutPacket"))
      return IO__INITFAIL;
      
    sts = xChannelGetPacket( local->chan, sizeof(module_config_cnf), (CIFX_PACKET *)&module_config_cnf, 20);
    if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
	 !cmd_check( op, ap, module_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_IOD_MODULE_CNF) ||
	 !status_check( op, ap, module_config_cnf.tHead.ulSta, "Module Config"))
      return IO__INITFAIL;
    
    free( module_config);


    // PNM_SUBMODULE

    for ( int i = 0; i < slots; i++) {
      int subslots = dev_data->slot_data[i]->subslot_data.size();

      PNM_APCFG_CFG_SUBMODULE_REQ_T *submodule_config = 
	(PNM_APCFG_CFG_SUBMODULE_REQ_T *) calloc( 1, sizeof(*submodule_config) + (subslots - 1) * sizeof(submodule_config->tData.atData));
      PNM_APCFG_CFG_SUBMODULE_CNF_T submodule_config_cnf = {{0}};
      
      submodule_config->tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
      submodule_config->tHead.ulLen    = HOST_TO_LE32(sizeof(submodule_config->tData)+(subslots-1)*sizeof(submodule_config->tData.atData));
      submodule_config->tHead.ulId     = HOST_TO_LE32(msg_id++);
      submodule_config->tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_IOD_SUBMODULE_REQ);
      submodule_config->tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
      submodule_config->tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
      submodule_config->tData.tSubHead.ulTrCntrId = 1;
      submodule_config->tData.tSubHead.ulTrDevId = device + 1;
      submodule_config->tData.tSubHead.ulTrApId = 1;
      submodule_config->tData.tSubHead.ulTrModId = i + 1;
      submodule_config->tData.tSubHead.ulTrSubModId = 0;
      submodule_config->tData.tSubHead.ulTrIdCnt = subslots;
      for ( int j = 0; j < subslots; j++) {
	submodule_config->tData.atData[j].ulTrId   = j + 1;
	submodule_config->tData.atData[j].ulSubmoduleID = dev_data->slot_data[i]->subslot_data[j]->submodule_ident_number; 
	submodule_config->tData.atData[j].usSubmoduleProp = 0;
	if ( dev_data->slot_data[i]->subslot_data[j]->io_input_length > 0 &&
	     dev_data->slot_data[i]->subslot_data[j]->io_output_length > 0)
	  submodule_config->tData.atData[j].usSubmoduleProp = PNIO_API_SUBM_PROP_TYPE_BOTH;
	else if ( dev_data->slot_data[i]->subslot_data[j]->io_input_length > 0)
	  submodule_config->tData.atData[j].usSubmoduleProp = PNIO_API_SUBM_PROP_TYPE_INPUT;
	else if ( dev_data->slot_data[i]->subslot_data[j]->io_output_length > 0)
	  submodule_config->tData.atData[j].usSubmoduleProp = PNIO_API_SUBM_PROP_TYPE_OUTPUT;
	else
	  submodule_config->tData.atData[j].usSubmoduleProp = PNIO_API_SUBM_PROP_TYPE_NONE;
	
	submodule_config->tData.atData[j].usSubslotNumber = dev_data->slot_data[i]->subslot_data[j]->subslot_number;
	// printf( "Submodule %d %d\n", i+1, j+1);
      }
  
      sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)submodule_config, 10);
      if ( !status_check( op, ap, sts, "xChannelPutPacket"))
	return IO__INITFAIL;

      sts = xChannelGetPacket( local->chan, sizeof(submodule_config_cnf), (CIFX_PACKET *)&submodule_config_cnf, 20);
      if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
	   !cmd_check( op, ap, submodule_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_IOD_SUBMODULE_CNF) ||
	   !status_check( op, ap, submodule_config_cnf.tHead.ulSta, "Submodule Config"))
	return IO__INITFAIL;
      
      free( submodule_config);
    }

    // PNM_SUBMDESCR

    int input_frame_offset = 0;
    int output_frame_offset = 0;
    int input_iocs_frame_offset = io_output_length;
    int output_iocs_frame_offset = io_input_length;

    for ( int i = 0; i < slots; i++) {

      int subslots = dev_data->slot_data[i]->subslot_data.size();
      int prev_slot_input_dpm_offset = input_dpm_offset;
      int prev_slot_output_dpm_offset = output_dpm_offset;
    
      for ( int j = 0; j < subslots; j++) {

	int io_input_length = dev_data->slot_data[i]->subslot_data[j]->io_input_length;
	int io_output_length = dev_data->slot_data[i]->subslot_data[j]->io_output_length;

	int size = 1;
	if ( io_input_length > 0 && io_output_length > 0)
	  size = 2;

	PNM_APCFG_CFG_SUBMDESCR_REQ_T *submdescr_config = 
	  (PNM_APCFG_CFG_SUBMDESCR_REQ_T *) calloc( 1, sizeof(*submdescr_config) + (size-1) * sizeof(submdescr_config->tData.atData));
	PNM_APCFG_CFG_SUBMDESCR_CNF_T submdescr_config_cnf = {{0}};
      
	submdescr_config->tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
	submdescr_config->tHead.ulLen    = HOST_TO_LE32(sizeof(submdescr_config->tData)+(size-1)*sizeof(submdescr_config->tData.atData));
	submdescr_config->tHead.ulId     = HOST_TO_LE32(msg_id++);
	submdescr_config->tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_IOD_SUBMDESCR_REQ);
	submdescr_config->tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
	submdescr_config->tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
	submdescr_config->tData.tSubHead.ulTrCntrId = 1;
	submdescr_config->tData.tSubHead.ulTrDevId = device + 1;
	submdescr_config->tData.tSubHead.ulTrApId = 1;
	submdescr_config->tData.tSubHead.ulTrModId = i + 1;
	submdescr_config->tData.tSubHead.ulTrSubModId = j + 1;
	submdescr_config->tData.tSubHead.ulTrIdCnt = size;
	for ( int k = 0; k < size; k++) {
	  submdescr_config->tData.atData[k].ulTrId = k + 1;
	  if ( io_input_length == 0 && io_output_length == 0) {
	    submdescr_config->tData.atData[k].usDataDescr = PNIO_API_SUBMDESCR_DATA_DESCR_INPUT; 
	    submdescr_config->tData.atData[k].usSubmDataLen = 0; 
	    submdescr_config->tData.atData[k].usFrameOffs = input_frame_offset;
	    submdescr_config->tData.atData[k].usIOCSFrameOffs = input_iocs_frame_offset;
	    submdescr_config->tData.atData[k].ulIO_Block = 0;
	    submdescr_config->tData.atData[k].ulDPM_Offset = 0;
	    // printf( "%d %d None:   iocsoffs %d offs %d len %d\n", i+1, j+1, input_iocs_frame_offset, input_frame_offset, io_input_length);
	  }
	  else if ( (size == 1 && io_input_length > 0) ||
		    (size == 2 && k == 0)) {
	    submdescr_config->tData.atData[k].usDataDescr = PNIO_API_SUBMDESCR_DATA_DESCR_INPUT;
	    submdescr_config->tData.atData[k].usSubmDataLen = io_input_length; 
	    submdescr_config->tData.atData[k].usFrameOffs = input_frame_offset;
	    submdescr_config->tData.atData[k].usIOCSFrameOffs = input_iocs_frame_offset;
	    submdescr_config->tData.atData[k].ulIO_Block = 0;
	    submdescr_config->tData.atData[k].ulDPM_Offset = input_dpm_offset;
	    // printf( "%d %d Input:  iocsoffs %d offs %d len %d\n", i+1, j+1, input_iocs_frame_offset, input_frame_offset, io_input_length);
	  }
	  else {
	    submdescr_config->tData.atData[k].usDataDescr = PNIO_API_SUBMDESCR_DATA_DESCR_OUTPUT; 
	    submdescr_config->tData.atData[k].usSubmDataLen = io_output_length;
	    submdescr_config->tData.atData[k].usFrameOffs = output_frame_offset;
	    submdescr_config->tData.atData[k].usIOCSFrameOffs = output_iocs_frame_offset;
	    submdescr_config->tData.atData[k].ulIO_Block = 0;
	    submdescr_config->tData.atData[k].ulDPM_Offset = output_dpm_offset;
	    // printf( "%d %d Output: iocsoffs %d offs %d len %d\n", i+1, j+1, output_iocs_frame_offset, output_frame_offset, io_output_length);
	  }
	  submdescr_config->tData.atData[k].bIOPSLen = 1;
	  submdescr_config->tData.atData[k].bIOCSLen = 1;
	  submdescr_config->tData.atData[k].ulIOCRIdProd = 1;
	  submdescr_config->tData.atData[k].ulIOCRIdCons = 2;
	  submdescr_config->tData.atData[k].ulSignla_Attrib = 0;
	}
	
	sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)submdescr_config, 10);
	if ( !status_check( op, ap, sts, "xChannelPutPacket"))
	  return IO__INITFAIL;

	sts = xChannelGetPacket( local->chan, sizeof(submdescr_config_cnf), (CIFX_PACKET *)&submdescr_config_cnf, 20);
	if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
	     !cmd_check( op, ap, submdescr_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_IOD_SUBMDESCR_CNF) ||
	     !status_check( op, ap, submdescr_config_cnf.tHead.ulSta, "Submodule Description"))
	  return IO__INITFAIL;

    
	if ( io_input_length > 0) {
	  input_frame_offset += io_input_length + 1;
	  input_iocs_frame_offset += 1;
	  input_dpm_offset += io_input_length;
	}
	if ( io_output_length > 0) {
	  output_frame_offset += io_output_length + 1;
	  output_iocs_frame_offset += 1;
	  output_dpm_offset += io_output_length;
	}
	if ( io_input_length == 0 && io_output_length == 0) {
	  input_frame_offset += 1;
	  input_iocs_frame_offset += 1;
	}
	
	free( submdescr_config);
      }

      if ( i > 0) {
	// First slot is the device and has no card
	if ( i == 1)
	  cp = rp->cardlist;
	else
	  cp = cp->next;

	if ( !cp) {
	  snprintf( op->ErrorStr, sizeof(op->ErrorStr), "Module config mismatch in xml-file, %s", rp->Name);
	  errh_Error( "IO init %s, '%s'", ap->Name, op->ErrorStr);
	  return IO__INITFAIL;
	}

	io_sPnCardLocal *cp_local = (io_sPnCardLocal *)cp->Local;

	cp_local->input_area_size = input_dpm_offset - prev_slot_input_dpm_offset;
	cp_local->output_area_size = output_dpm_offset - prev_slot_output_dpm_offset;
      }
      prev_slot_input_dpm_offset = input_dpm_offset;
      prev_slot_output_dpm_offset = output_dpm_offset;
    }

    rp_local->bytes_of_input = input_dpm_offset - prev_input_dpm_offset;
    rp_local->bytes_of_output = output_dpm_offset - prev_output_dpm_offset;
    prev_input_dpm_offset = input_dpm_offset;
    prev_output_dpm_offset = output_dpm_offset;
    
    // PNM_RECORD

    for ( int i = 0; i < slots; i++) {

      int subslots = dev_data->slot_data[i]->subslot_data.size();
      
      for ( int j = 0; j < subslots; j++) {

	int records = dev_data->slot_data[i]->subslot_data[j]->data_record.size();

	if ( !records)
	  continue;

	int size = sizeof(PNM_APCFG_CFG_RECORD_REQ_T) + 
	  (records - 1) * (sizeof(PNM_APCFG_CFG_RECORD_DATA_T) - 1);
	for ( int k = 0; k < records; k++)
	  size += dev_data->slot_data[i]->subslot_data[j]->data_record[k]->data_length;
	
	PNM_APCFG_CFG_RECORD_REQ_T *record_config = (PNM_APCFG_CFG_RECORD_REQ_T *) calloc( 1, size);
	PNM_APCFG_CFG_RECORD_CNF_T record_config_cnf = {{0}};
      
	record_config->tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
	record_config->tHead.ulLen    = HOST_TO_LE32(size);
	record_config->tHead.ulId     = HOST_TO_LE32(msg_id++);
	record_config->tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_IOD_RECDATA_REQ);
	record_config->tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
	record_config->tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
	record_config->tData.tSubHead.ulTrCntrId = 1;
	record_config->tData.tSubHead.ulTrDevId = device + 1;
	record_config->tData.tSubHead.ulTrApId = 1;
	record_config->tData.tSubHead.ulTrModId = i + 1;
	record_config->tData.tSubHead.ulTrSubModId = j + 1;
	record_config->tData.tSubHead.ulTrIdCnt = records;

	PNM_APCFG_CFG_RECORD_DATA_T *rp = &record_config->tData.tRecord;
	for ( int k = 0; k < records; k++) {
	  rp->ulTrId = k + 1;
	  rp->usIndex = k + 1;
	  rp->ulDataLen = 
	    dev_data->slot_data[i]->subslot_data[j]->data_record[k]->data_length; 
	  memcpy( rp->abRecordData,
		  dev_data->slot_data[i]->subslot_data[j]->data_record[k]->data, rp->ulDataLen); 
	  
	  rp = (PNM_APCFG_CFG_RECORD_DATA_T *) ((char *)rp + sizeof(*rp) + rp->ulDataLen - 1);
	}
	
	sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)record_config, 10);
	if ( !status_check( op, ap, sts, "xChannelPutPacket"))
	  return IO__INITFAIL;
	  
	sts = xChannelGetPacket( local->chan, sizeof(record_config_cnf), (CIFX_PACKET *)&record_config_cnf, 20);
	if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
	     !cmd_check( op, ap, record_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_IOD_RECDATA_CNF) ||
	     !status_check( op, ap, record_config_cnf.tHead.ulSta, "Record Data"))
	  return IO__INITFAIL;
	
	free( record_config);
      }
      
    }

    device++;
  }

  // PNM_FIN

  PNM_APCFG_DWNL_FIN_REQ_T dwnl_fin_config = {{0}};
  PNM_APCFG_DWNL_FIN_CNF_T dwnl_fin_config_cnf = {{0}};

  dwnl_fin_config.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
  dwnl_fin_config.tHead.ulLen    = 0;
  dwnl_fin_config.tHead.ulId     = HOST_TO_LE32(msg_id++);
  dwnl_fin_config.tHead.ulCmd    = HOST_TO_LE32(PNM_APCTL_CMD_SET_CONFIG_DWNL_FIN_REQ);
  dwnl_fin_config.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
  dwnl_fin_config.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
  
  sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&dwnl_fin_config, 10);
  if ( !status_check( op, ap, sts, "xChannelPutPacket"))
    return IO__INITFAIL;

  sts = xChannelGetPacket( local->chan, sizeof(dwnl_fin_config_cnf), (CIFX_PACKET *)&dwnl_fin_config_cnf, 20);
  if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
       !cmd_check( op, ap, dwnl_fin_config_cnf.tHead.ulCmd, PNM_APCTL_CMD_SET_CONFIG_DWNL_FIN_CNF) ||
       !status_check( op, ap, dwnl_fin_config_cnf.tHead.ulSta, "Controller Config"))
    return IO__INITFAIL;

  // LOCK
  RCX_LOCK_UNLOCK_CONFIG_REQ_T lock = {{0}};
  RCX_LOCK_UNLOCK_CONFIG_CNF_T lock_cnf = {{0}};

  lock.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
  lock.tHead.ulLen    = HOST_TO_LE32(sizeof(lock.tData));
  lock.tHead.ulCmd    = HOST_TO_LE32(RCX_LOCK_UNLOCK_CONFIG_REQ);
  lock.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
  lock.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
  lock.tData.ulParam  = 1; // Lock

  sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&lock, 10);
  if ( !status_check( op, ap, sts, "xChannelPutPacket"))
    return IO__INITFAIL;

  sts = xChannelGetPacket( local->chan, sizeof(lock_cnf), (CIFX_PACKET *)&lock_cnf, 20);
  if ( !status_check( op, ap, sts, "xChannelGetPacket") ||
       !cmd_check( op, ap, lock_cnf.tHead.ulCmd, RCX_LOCK_UNLOCK_CONFIG_CNF) ||
       !status_check( op, ap, lock_cnf.tHead.ulSta, "Unlock channel"))
    return IO__INITFAIL;


  // Create input/output area
  local->input_area_size = input_dpm_offset;
  local->output_area_size = output_dpm_offset;

  local->input_area = calloc( 1, MAX(local->input_area_size,4));
  local->output_area = calloc( 1, MAX(local->output_area_size,4));

  int input_offset = 0;
  int output_offset = 0; 
  for ( rp = ap->racklist; rp; rp = rp->next) {
    io_sPnRackLocal *rp_local = (io_sPnRackLocal *)rp->Local;

    rp_local->inputs = (unsigned char *)local->input_area + input_offset;
    rp_local->outputs = (unsigned char *)local->output_area + output_offset;

    int slot_input_offset = input_offset;
    int slot_output_offset = output_offset;
    for ( cp = rp->cardlist; cp; cp = cp->next) {
      io_sPnCardLocal *cp_local = (io_sPnCardLocal *)cp->Local;

      cp_local->input_area = (unsigned char *)local->input_area + slot_input_offset;
      cp_local->output_area = (unsigned char *)local->output_area + slot_output_offset;
      slot_input_offset += cp_local->input_area_size;
      slot_output_offset += cp_local->output_area_size;
    }
    input_offset += rp_local->bytes_of_input;
    output_offset += rp_local->bytes_of_output;

    // Set OK status on device
    ((pwr_sClass_PnDevice *)rp->op)->Status = PB__NORMAL;
    ((pwr_sClass_PnDevice *)rp->op)->State = pwr_ePnDeviceStateEnum_Connected;
  }

  for (;;) {
    sts = xChannelHostState( local->chan, CIFX_HOST_STATE_READY, &state, 100);
    if ( sts != CIFX_DEV_NOT_RUNNING)
      break;
    sleep(1);
  }

  sts = xChannelHostState( local->chan, CIFX_HOST_STATE_READ, &state, 0);
  // printf( "Host state: %d\n", state);  
  
  sts = xChannelBusState( local->chan, CIFX_BUS_STATE_GETSTATE, &state, 0);
  // printf( "Bus state: %d\n", state);

  sts = xChannelBusState( local->chan, CIFX_BUS_STATE_ON, &state, 5000 /* 20000 */);
  // printf( "Set bus state: 0x%08x\n", sts);

  sts = xChannelBusState( local->chan, CIFX_BUS_STATE_GETSTATE, &state, 0);
  // printf( "Bus state: %d\n", state);
  
  // It takes ~20 s to get COM-flag
  local->dev_init = 1;
  local->dev_init_limit = (unsigned int) (30.0 / ctx->ScanTime + 0.5);

  errh_Info( "Init of Hilscher cifX Profinet Controller '%s'", ap->Name);
  return IO__SUCCESS;
}


static pwr_tStatus IoAgentClose( io_tCtx ctx,
				 io_sAgent *ap)
{
  io_sRack *rp;
  io_sLocalHilscher_cifX_PnController *local = (io_sLocalHilscher_cifX_PnController *)ap->Local;

  if ( driver) {
    xDriverClose( driver);
    driver = 0;
  }
      
  if ( local->input_area_size > 0)
    free( local->input_area);
  if ( local->output_area_size > 0)
    free( local->output_area);

  for ( rp = ap->racklist; rp; rp = rp->next) {
    if ( rp->Local)
      free( rp->Local);
  }

  free( local);

  return IO__SUCCESS;
}

static pwr_tStatus IoAgentRead( io_tCtx ctx, io_sAgent *ap) 
{
  io_sLocalHilscher_cifX_PnController *local = (io_sLocalHilscher_cifX_PnController *)ap->Local;
  pwr_sClass_Hilscher_cifX_PnController *op = (pwr_sClass_Hilscher_cifX_PnController *)ap->op;
  int32_t sts;

  if ( local->diag_cnt == 0)
    get_diag( &op->Diag, local->chan);
  if ( local->diag_cnt > local->diag_interval)
    local->diag_cnt = 0;
  else
    local->diag_cnt++;

  // Get Alarm or Diag
  int msg_size = 500;
  APIOC_ALARM_IND_T *msg = (APIOC_ALARM_IND_T *)calloc( 1, msg_size);

  sts = xChannelGetPacket( local->chan, msg_size, (CIFX_PACKET *)msg, 20);
  if ( sts == CIFX_NO_ERROR) {
    printf( "Diag message ?\n");

    if ( msg->tHead.ulCmd == PNIO_APCTL_CMD_APPL_ALARM_IND) {
	
      // Response, return the package
      APIOC_ALARM_RSP_T alarm_rsp = {{0}};

      alarm_rsp.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
      alarm_rsp.tHead.ulLen    = HOST_TO_LE32(sizeof(alarm_rsp.tData));
      alarm_rsp.tHead.ulCmd    = HOST_TO_LE32(PNIO_APCTL_CMD_APPL_ALARM_RSP);
      alarm_rsp.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
      alarm_rsp.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
      alarm_rsp.tData.ulHandle  = msg->tData.ulHandle;
      alarm_rsp.tData.usAlarmSpecifier = msg->tData.usAlarmSpecifier;
      
      sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&alarm_rsp, 10);
      printf( "Alarm ind\n");

      // Ack the alarm
      APIOC_ALARM_ACK_REQ_T alarm_ack = {{0}};
      APIOC_ALARM_ACK_CNF_T alarm_ack_cnf = {{0}};
      
      alarm_ack.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
      alarm_ack.tHead.ulLen    = HOST_TO_LE32(sizeof(alarm_ack.tData));
      alarm_ack.tHead.ulCmd    = HOST_TO_LE32(PNIO_APCTL_CMD_APPL_ALARM_ACK_CNF);
      alarm_ack.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
      alarm_ack.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
      alarm_ack.tData.ulHandle  = msg->tData.ulHandle;
      alarm_ack.tData.usAlarmSpecifier = msg->tData.usAlarmSpecifier;
      alarm_ack.tData.usReserved = 0;
      
      sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&alarm_ack, 10);
      if ( sts == CIFX_NO_ERROR) {

	sts = xChannelGetPacket( local->chan, sizeof(alarm_ack_cnf), (CIFX_PACKET *)&alarm_ack_cnf, 20);
	if ( sts == CIFX_NO_ERROR) {
	  printf( "ALARM_ACK\n");
	  printf( "Status 0x%08x\n", alarm_ack_cnf.tHead.ulSta);
	}
      }	
    }
    else if ( msg->tHead.ulCmd == PNIO_APCTL_CMD_APPL_DIAG_DATA_IND) {
	
      // Response, return the package
      APIOC_DIAG_DATA_RSP_T diag_data_rsp = {{0}};

      diag_data_rsp.tHead.ulDest   = HOST_TO_LE32(PNM_APPLICATION);
      diag_data_rsp.tHead.ulLen    = HOST_TO_LE32(sizeof(diag_data_rsp.tData));
      diag_data_rsp.tHead.ulCmd    = HOST_TO_LE32(PNIO_APCTL_CMD_APPL_DIAG_DATA_RSP);
      diag_data_rsp.tHead.ulSrc    = HOST_TO_LE32(PN_SRC);
      diag_data_rsp.tHead.ulSrcId  = HOST_TO_LE32(PN_SRCID);
      diag_data_rsp.tData.ulHandle  = ((APIOC_DIAG_DATA_IND_T *)msg)->tData.ulHandle;
      
      sts = xChannelPutPacket( local->chan, (CIFX_PACKET*)&diag_data_rsp, 10);
      printf( "Diag data\n");
      
    }
    else
      printf( "Unexpected cmd received: %u\n", msg->tHead.ulCmd);
  }
  else {
    // printf( "Diag msg status 0x%08x\n", sts);
  }

  free(msg);


  // Read input area
  if ( local->input_area_size) {
    sts = xChannelIORead( local->chan, 0, 0, local->input_area_size, local->input_area, 10);
    op->Status = sts;
    if ( sts == CIFX_NO_ERROR) {
      if ( local->dev_init)
	local->dev_init = 0;
    }
    else {
      if ( sts == CIFX_DEV_NO_COM_FLAG && local->dev_init && 
	   local->dev_init_cnt < local->dev_init_limit)
	local->dev_init_cnt++;
      else {
	xDriverGetErrorDescription( sts, op->ErrorStr, sizeof(op->ErrorStr));
	op->ErrorCount++;
      }
    }

    if ( op->ErrorCount == op->ErrorSoftLimit && !local->softlimit_logged) {
      errh_Error( "IO Error soft limit reached on agent '%s'", ap->Name);
      local->softlimit_logged = 1;
    }
    if ( op->ErrorCount >= op->ErrorHardLimit) {
      ctx->Node->EmergBreakTrue = 1;
      return IO__ERRDEVICE;
    }
  }
  return IO__SUCCESS;
}

static pwr_tStatus IoAgentWrite( io_tCtx ctx, io_sAgent *ap) 
{
  io_sLocalHilscher_cifX_PnController *local = (io_sLocalHilscher_cifX_PnController *)ap->Local;
  pwr_sClass_Hilscher_cifX_PnController *op = (pwr_sClass_Hilscher_cifX_PnController *)ap->op;
  int32_t sts;

  if ( local->output_area_size) {
    sts = xChannelIOWrite( local->chan, 0, 0, local->output_area_size, local->output_area, 10);
    op->Status = sts;
    if ( sts != CIFX_NO_ERROR) {

      if ( ! local->dev_init) {
	op->ErrorCount++;
	xDriverGetErrorDescription( sts, op->ErrorStr, sizeof(op->ErrorStr));
      }

      if ( op->ErrorCount == op->ErrorSoftLimit && !local->softlimit_logged) {
	errh_Error( "IO Error soft limit reached on agent '%s'", ap->Name);
	local->softlimit_logged = 1;
      }
      if ( op->ErrorCount >= op->ErrorHardLimit) {
	ctx->Node->EmergBreakTrue = 1;
	return IO__ERRDEVICE;
      }
    }
  }
  return IO__SUCCESS;
}

#else
static pwr_tStatus IoAgentInit( io_tCtx ctx, io_sAgent *ap) { return IO__RELEASEBUILD;}
static pwr_tStatus IoAgentClose( io_tCtx ctx, io_sAgent *ap) { return IO__RELEASEBUILD;}
static pwr_tStatus IoAgentRead( io_tCtx ctx, io_sAgent *ap) { return IO__RELEASEBUILD;}
static pwr_tStatus IoAgentWrite( io_tCtx ctx, io_sAgent *ap) { return IO__RELEASEBUILD;}
#endif

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(Hilscher_cifX_PnController) = {
  pwr_BindIoMethod(IoAgentInit),
  pwr_BindIoMethod(IoAgentClose),
  pwr_BindIoMethod(IoAgentRead),
  pwr_BindIoMethod(IoAgentWrite),
  pwr_NullMethod
};


