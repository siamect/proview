/** 
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
 **/

/* pn_viewer_pnac.cpp -- Profinet viewer PNAC interface */

#ifdef PWRE_CONF_PNAK

#include "co_error.h"
#include "pn_viewer_pnac.h"
#include "rt_profinet.h"
#include "rt_pnak.h"
#include "sys/socket.h"
#include "sys/ioctl.h"
#include "net/if.h"
#include "netinet/in.h"
#include "arpa/inet.h"

#ifndef rt_pn_iface_h
# include "rt_pn_iface.h"
#endif



PnViewerPNAC::PnViewerPNAC( pwr_tStatus *sts)
{
  int s;
  struct ifreq ifr = {};
  PnDeviceData     *pn_dev_data;
  T_PNAK_EVENT_SET_MODE        pMode;
  T_PNAK_WAIT_OBJECT           wait_object;

  /* Init PNAC */

  local = new io_sAgentLocal;
  pnak_init();
  *sts = pnak_start_profistack(0, PNAK_CONTROLLER_MODE);

  if (*sts != PNAK_OK) {
    printf("Starting profistack returned with error code\n");
    exit(0);
  }

  /* Get configs for device */

  s = socket(AF_INET, SOCK_DGRAM, 0);
  strncpy(ifr.ifr_name, "eth1", sizeof(ifr.ifr_name));
  if (ioctl(s, SIOCGIFADDR, &ifr) >= 0) {
    strcpy(dev_data.ip_address, inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
  }
  if (ioctl(s, SIOCGIFNETMASK, &ifr) >= 0) {
    strcpy(dev_data.subnet_mask, inet_ntoa(((struct sockaddr_in *) &ifr.ifr_netmask)->sin_addr));
  }

  sscanf(dev_data.ip_address, "%hhu.%hhu.%hhu.%hhu", &local->ipaddress[3], &local->ipaddress[2], &local->ipaddress[1], &local->ipaddress[0]) ;
  sscanf(dev_data.subnet_mask, "%hhu.%hhu.%hhu.%hhu", &local->subnetmask[3], &local->subnetmask[2], &local->subnetmask[1], &local->subnetmask[0]) ;


  strcpy(dev_data.device_name, "dumle");
  dev_data.device_num = PN_DEVICE_REFERENCE_THIS_STATION;
  strcpy(dev_data.device_text, "eth1");
  dev_data.vendor_id = 279; // Softing vendor id
  dev_data.device_id = 0;
  strcpy(dev_data.version, "1.0");
  dev_data.byte_order = 0;

  pn_dev_data = new PnDeviceData;

  pn_dev_data->device_ref = PN_DEVICE_REFERENCE_THIS_STATION;
  local->device_data.push_back(pn_dev_data);

  /* Download config */

  pack_download_req(&local->service_req_res, &dev_data, pn_dev_data->device_ref);

  *sts = pnak_send_service_req_res(0, &local->service_req_res);

  if (*sts == PNAK_OK) {
    *sts = handle_service_con(local, 0);

    if (*sts != PNAK_OK) { 
	/* Loop through devices and calculate offset for io */
    }
  }

  /* Set identification */

  pack_set_identification_req(&local->service_req_res);

  *sts = pnak_send_service_req_res(0, &local->service_req_res);

  if (*sts == PNAK_OK) {
    *sts = handle_service_con(local, 0);
  }

  pMode.Mode = PNAK_MODE_ONLINE;

  *sts = pnak_set_mode(0, &pMode);

  wait_object = PNAK_WAIT_OBJECT_STATE_CHANGED;

  *sts = pnak_wait_for_multiple_objects(0, &wait_object, PNAK_INFINITE_TIMEOUT);

  if (*sts != PNAK_OK) {
    printf("Setting state failed, sts: %d\r\n", *sts);
    return;
  }

  *sts = 1;
}


PnViewerPNAC::~PnViewerPNAC()
{
  // Close PNAC
}

void PnViewerPNAC::fetch_devices( vector<PnDevice>& dev_vect)
{
  PnDevice                     device;
  unsigned int                 ii;
  int                          sts;

  while (1) {

    for (ii = 0; ii < local->dev_info.size(); ii++) {
      delete local->dev_info[ii];
    }

    local->dev_info.clear();

    pack_get_los_req(&local->service_req_res);
  
    sts = pnak_send_service_req_res(0, &local->service_req_res);
  
    if (sts == PNAK_OK) {
      sts = handle_service_con(local, 0);
      if (sts == PNAK_OK) {
	if (local->dev_info.size() > 0) {
	  for (ii = 0; ii < local->dev_info.size(); ii++) {
	    device.ipaddress[0] = local->dev_info[ii]->ipaddress[0];
	    device.ipaddress[1] = local->dev_info[ii]->ipaddress[1];
	    device.ipaddress[2] = local->dev_info[ii]->ipaddress[2];
	    device.ipaddress[3] = local->dev_info[ii]->ipaddress[3];
	    device.macaddress[0] = local->dev_info[ii]->macaddress[0];
	    device.macaddress[1] = local->dev_info[ii]->macaddress[1];
	    device.macaddress[2] = local->dev_info[ii]->macaddress[2];
	    device.macaddress[3] = local->dev_info[ii]->macaddress[3];
	    device.macaddress[4] = local->dev_info[ii]->macaddress[4];
	    device.macaddress[5] = local->dev_info[ii]->macaddress[5];
	    strncpy(device.devname, local->dev_info[ii]->devname, sizeof(device.devname));
	    device.vendorid = local->dev_info[ii]->vendorid;
	    device.deviceid = local->dev_info[ii]->deviceid;
	    dev_vect.push_back(device);
	  }
	  break;
	} else continue;
      } 
    }
  }

  /*  pMode.Mode = PNAK_MODE_OFFLINE;

  sts = pnak_set_mode(0, &pMode);
  
  wait_object = PNAK_WAIT_OBJECT_STATE_CHANGED;
  
  sts = pnak_wait_for_multiple_objects(0, &wait_object, PNAK_INFINITE_TIMEOUT);
  
  if (sts != PNAK_OK) {
  printf("Setting state failed, sts: %d\r\n", sts);
  return;
  } */
  
  // if ( ...error... ) throw co_error_str( "Somethings is wrong...");
}
 
void PnViewerPNAC::set_device_properties( unsigned char *macaddress, unsigned char *ipaddress,
					  char *devname)
{
  PnDevice                     device;
  int                          sts;
  PnDeviceInfo                 dev_info;

  /* Set name and ip-address of device */

  dev_info.ipaddress[0] = ipaddress[0];
  dev_info.ipaddress[1] = ipaddress[1];
  dev_info.ipaddress[2] = ipaddress[2];
  dev_info.ipaddress[3] = ipaddress[3];
  dev_info.subnetmask[0] = local->subnetmask[0];
  dev_info.subnetmask[1] = local->subnetmask[1];
  dev_info.subnetmask[2] = local->subnetmask[2];
  dev_info.subnetmask[3] = local->subnetmask[3];
  dev_info.macaddress[0] = macaddress[0];
  dev_info.macaddress[1] = macaddress[1];
  dev_info.macaddress[2] = macaddress[2];
  dev_info.macaddress[3] = macaddress[3];
  dev_info.macaddress[4] = macaddress[4];
  dev_info.macaddress[5] = macaddress[5];
  strncpy(dev_info.devname, devname, sizeof(dev_info.devname));


  pack_set_device_name_req(&local->service_req_res, &dev_info);
  sts = pnak_send_service_req_res(0, &local->service_req_res);

  if (sts == PNAK_OK) {
    sts = handle_service_con(local, 0);
  }

  pack_set_ip_settings_req(&local->service_req_res, &dev_info);
  sts = pnak_send_service_req_res(0, &local->service_req_res);
      
  if (sts == PNAK_OK) {
    sts = handle_service_con(local, 0);
  }

}

#endif
