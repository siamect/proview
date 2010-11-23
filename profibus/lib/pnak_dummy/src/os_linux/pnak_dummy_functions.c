/* 
 * Copyright (C) 2010 SSAB Oxelösund AB.
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

/* pnak_dummy_functions.c -- pnak functions */

#include "rt_profinet.h"
#include "rt_pnak.h"

/*--- initialization/termination functions ----------------------------------*/

void             pnak_init                                 (void) {}
void             pnak_term                                 (void) {}

T_PNAK_RESULT    pnak_start_profistack                     (unsigned short a1, const T_PNAK_MODE_ID a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_stop_profistack                      (unsigned short a1) {return PNAK_OK;}

/*--- General interface functions -------------------------------------------*/

T_PNAK_RESULT    pnak_wait_for_multiple_objects            (unsigned short a1, T_PNAK_WAIT_OBJECT* a2, T_PNAK_TIMEOUT a3) {return PNAK_OK;}
T_PNAK_RESULT    pnak_set_multiple_objects                 (unsigned short a1, const T_PNAK_WAIT_OBJECT a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_mask_multiple_objects                (unsigned short a1, const T_PNAK_WAIT_OBJECT a2) {return PNAK_OK;}

T_PNAK_RESULT    pnak_get_exception                        (unsigned short a1, T_PNAK_EXCEPTION* a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_get_version                          (unsigned short a1, T_PNAK_VERSIONS* a2) {return PNAK_OK;}

T_PNAK_RESULT    pnak_snmp_get_data                        (unsigned short a1, T_PNAK_OID* a2, void* a3, unsigned long* a4, unsigned long* a5, unsigned char a6, PN_BOOL a7) {return PNAK_OK;}
T_PNAK_RESULT    pnak_snmp_check_data                      (unsigned short a1, T_PNAK_OID* a2, void* a3, unsigned long a4, unsigned long a5, unsigned char a6) {return PNAK_OK;}
T_PNAK_RESULT    pnak_snmp_set_data                        (unsigned short a1, T_PNAK_OID* a2, void* a3, unsigned long a4, unsigned long a5) {return PNAK_OK;}

/*--- Service interface functions -------------------------------------------*/

T_PNAK_RESULT    pnak_send_service_req_res                 (unsigned short a1, const T_PNAK_SERVICE_REQ_RES* a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_get_service_ind                      (unsigned short a1, T_PNAK_SERVICE_IND* a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_get_service_con                      (unsigned short a1, T_PNAK_SERVICE_CON* a2) {return PNAK_OK;}

/*--- Event interface functions ---------------------------------------------*/

T_PNAK_RESULT    pnak_set_mode                             (unsigned short a1, const T_PNAK_EVENT_SET_MODE* a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_set_device_state                     (unsigned short a1, const T_PNAK_EVENT_SET_DEVICE_STATE* a2) {return PNAK_OK;}

T_PNAK_RESULT    pnak_get_state                            (unsigned short a1, T_PNAK_EVENT_STATE* a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_get_device_state_ind                 (unsigned short a1, T_PNAK_EVENT_DEVICE_STATE* a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_get_alarm_ind                        (unsigned short a1, T_PNAK_EVENT_ALARM* a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_get_alarm_ack_ind                    (unsigned short a1, T_PNAK_EVENT_ALARM_ACK* a2) {return PNAK_OK;}

/*--- Data interface functions ----------------------------------------------*/

T_PNAK_RESULT    pnak_set_iocr_data                        (unsigned short a1, unsigned short a2, const unsigned char* a3, unsigned short a4) {return PNAK_OK;}

T_PNAK_RESULT    pnak_get_iocr_data                        (unsigned short a1, unsigned short a2, unsigned char* a3, unsigned short* a4, unsigned char* a5) {return PNAK_OK;}
T_PNAK_RESULT    pnak_get_consumer_data_changed_ind        (unsigned short a1, T_PNAK_DATA_CONSUMER_DATA_CHANGED* a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_get_provider_data_updated            (unsigned short a1, T_PNAK_DATA_PROVIDER_DATA_UPDATED* a2) {return PNAK_OK;}

/*=============================================================================
CALLBACK INTERFACE
=============================================================================*/

T_PNAK_RESULT    pnak_register_provider_callback           (unsigned short a1, T_PROVIDER_CALLBACK a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_unregister_provider_callback         (unsigned short a1) {return PNAK_OK;}

T_PNAK_RESULT    pnak_register_consumer_callback           (unsigned short a1, T_CONSUMER_CALLBACK a2) {return PNAK_OK;}
T_PNAK_RESULT    pnak_unregister_consumer_callback         (unsigned short a1) {return PNAK_OK;}
