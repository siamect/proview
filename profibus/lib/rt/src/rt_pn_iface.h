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
 **/

/* rt_pn_iface.h -- Profinet driver interface routines  */

#ifndef rt_pn_iface_h
#define rt_pn_iface_h


#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_pn_gsdml_data_h
#include "rt_pn_gsdml_data.h"
#endif

#ifndef rt_io_pn_locals_h
# include "rt_io_pn_locals_h.h"
#endif


void pack_set_ip_settings_req(T_PNAK_SERVICE_REQ_RES *ServiceReqRes, PnDeviceInfo *dev_info);

void pack_set_device_name_req(T_PNAK_SERVICE_REQ_RES *ServiceReqRes, PnDeviceInfo *dev_info);

void pack_set_identification_req(T_PNAK_SERVICE_REQ_RES *ServiceReqRes);

void pack_get_device_state_req(T_PNAK_SERVICE_REQ_RES *ServiceReqRes, unsigned short device_ref);

void pack_get_los_req(T_PNAK_SERVICE_REQ_RES *ServiceReqRes);

void pack_get_alarm_req(T_PNAK_SERVICE_REQ_RES *ServiceReqRes, unsigned short ref);

void pack_alarm_ack_req(T_PNAK_SERVICE_REQ_RES *ServiceReqRes, unsigned short ref, unsigned short prio);

void pack_download_req(T_PNAK_SERVICE_REQ_RES *ServiceReqRes, GsdmlDeviceData *dev_data, unsigned short device_ref);

int unpack_get_los_con(T_PNAK_SERVICE_DESCRIPTION* pSdb, io_sAgentLocal *local);

int unpack_get_alarm_con(T_PNAK_SERVICE_DESCRIPTION* pSdb, io_sAgentLocal *local, io_sAgent *ap);
int unpack_get_device_state_con(T_PNAK_SERVICE_DESCRIPTION* pSdb, io_sAgentLocal *local, io_sAgent *ap);

int unpack_download_con(T_PNAK_SERVICE_DESCRIPTION* pSdb, io_sAgentLocal *local);

int handle_service_con(io_sAgentLocal *local, io_sAgent *ap);

void handle_exception (io_sAgentLocal *local);

void handle_state_changed (io_sAgentLocal *local);

void handle_device_state_changed (io_sAgentLocal *local, io_sAgent *ap);

void handle_alarm_indication (io_sAgentLocal *local, io_sAgent *ap);

void *handle_events(void *ptr);

#endif

