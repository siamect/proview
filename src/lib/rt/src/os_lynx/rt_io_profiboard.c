/* 
 * Proview   $Id: rt_io_profiboard.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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
#include "pb_err.h"
#include "pbdrvr.h"

#include "pwr.h"
#include "rt_io_profiboard.h"

unsigned short swap16(unsigned short in) 
{
  unsigned short result = 0;
  result = (in << 8) & 0xFF00;
  result |= (in >> 8) & 0x00FF;
  return(result);
}

unsigned int swap32(unsigned int in) 
{
  unsigned int result = 0;;
  result  = (in << 24) & 0xFF000000;
  result |= (in <<  8) & 0x00FF0000;
  result |= (in >>  8) & 0x0000FF00;
  result |= (in >> 24) & 0x000000FF;
  return(result);
}


pwr_tUInt16 pb_cmi_init(int fp) 
{
  int sts;

  sts = ioctl(fp, PB_IOCTL_CMI_INIT, 0);
  if (sts == 0)
    return PB_OK;
  else
    return PB_DEVICE_ERROR;
}


pwr_tUInt16 pb_cmi_write(int fp,
		         T_PROFI_SERVICE_DESCR *sdb_ptr,
		         void *data_ptr,
		         pwr_tUInt16 data_len) 
{
  cmi_request_access_struct cws;
  USIGN16 retval;
  int sts;

  cws.sdb_ptr = sdb_ptr;
  cws.data_ptr = (char *) data_ptr;
  cws.data_len_ptr = &data_len;
  cws.retval_ptr = &retval;

  sts = ioctl(fp, PB_IOCTL_CMI_WRITE, (char *) &cws);

  if (sts == 0)
    return PB_OK;
  else
    return PB_DEVICE_ERROR;
}


pwr_tUInt16 pb_cmi_read(int fp,
		        T_PROFI_SERVICE_DESCR *sdb_ptr,
		        void *data_ptr,
		        pwr_tUInt16 *data_len_ptr)
{
  cmi_request_access_struct crs;

  T_PROFI_SERVICE_DESCR sdb;
  USIGN8 data[256];
  USIGN16 data_len;
  USIGN16 retval;
  int sts;
  int retry;
  struct timespec rqtp, rmtp;

  crs.sdb_ptr = &sdb;
  crs.data_ptr = (USIGN8 *) &data;
  crs.data_len_ptr = &data_len;
  crs.retval_ptr = &retval;
  rqtp.tv_sec = 0;
  rqtp.tv_nsec = 100000000;		// One tenth

  sts = ioctl(fp, PB_IOCTL_CMI_READ, (char *) &crs);

  retry = 10;
  do {
    sts = ioctl(fp, PB_IOCTL_CMI_READ, (char *) &crs);
    if (sts != 0) return PB_DEVICE_ERROR;
    if (retval != CON_IND_RECEIVED) nanosleep(&rqtp, &rmtp);
  } while (retval != CON_IND_RECEIVED && retry-- > 0);

  if (retval == CON_IND_RECEIVED) {

    if (sdb_ptr) {
      memcpy(sdb_ptr, &sdb, sizeof(T_PROFI_SERVICE_DESCR));
    }

    if (data_ptr) {
      memcpy(data_ptr, data, data_len);
    }
  
    if (data_len_ptr) {
      *data_len_ptr = data_len;
    }

    if (sdb.result == POS)
      return PB_OK;
    else
      return PB_NEG_CONF;
  }
  else
    return PB_NO_CON_IND_RECEIVED;

}


pwr_tUInt16 pb_cmi_poll(int fp,
		        T_PROFI_SERVICE_DESCR *sdb_ptr,
		        void *data_ptr,
		        pwr_tUInt16 *data_len_ptr)
{
  cmi_request_access_struct crs;

  T_PROFI_SERVICE_DESCR sdb;
  USIGN8 data[256];
  USIGN16 data_len;
  USIGN16 retval;
  int sts;

  crs.sdb_ptr = &sdb;
  crs.data_ptr = (USIGN8 *) &data;
  crs.data_len_ptr = &data_len;
  crs.retval_ptr = &retval;

  sts = ioctl(fp, PB_IOCTL_CMI_READ, (char *) &crs);

  if (retval == CON_IND_RECEIVED) {

    if (sdb_ptr) {
      memcpy(sdb_ptr, &sdb, sizeof(T_PROFI_SERVICE_DESCR));
    }

    if (data_ptr) {
      memcpy(data_ptr, data, data_len);
    }
  
    if (data_len_ptr) {
      *data_len_ptr = data_len;
    }

    if (sdb.result == POS)
      return PB_OK;
    else
      return PB_NEG_CONF;
  }
  else
    return PB_NO_CON_IND_RECEIVED;

}


pwr_tUInt16 pb_cmi_flush(int fp)
{
  cmi_request_access_struct crs;

  T_PROFI_SERVICE_DESCR sdb;
  USIGN8 data[256];
  USIGN16 data_len;
  USIGN16 retval;
  int sts;
  int retry;
  struct timespec rqtp, rmtp;

  crs.sdb_ptr = &sdb;
  crs.data_ptr = (USIGN8 *) &data;
  crs.data_len_ptr = &data_len;
  crs.retval_ptr = &retval;
  rqtp.tv_sec = 0;
  rqtp.tv_nsec = 100000000;		// One tenth

  sts = ioctl(fp, PB_IOCTL_CMI_READ, (char *) &crs);

  retry = 100;
  do {
    sts = ioctl(fp, PB_IOCTL_CMI_READ, (char *) &crs);
    if (sts != 0) return E_IF_OS_ERROR;
    if (retval == CON_IND_RECEIVED) nanosleep(&rqtp, &rmtp);
  }  while (retval == CON_IND_RECEIVED && retry-- > 0);

  return PB_OK;
}


pwr_tUInt16 pb_cmi_get_data(int fp,
		            pwr_tUInt8 data_id,
		            pwr_tUInt16 offset,
		            pwr_tUInt16 data_size,
		            void *data_ptr)
{
  USIGN16 retval;
  cmi_data_access_struct user_arg;
  int retry_counter;

  user_arg.data_id = data_id;
  user_arg.offset = offset;
  user_arg.data_ptr = (USIGN8 *) data_ptr;
  user_arg.retval_ptr = &retval;
  retry_counter = 10;
  do {
       read(fp, (char *) &user_arg, data_size);
  }
  while(retval != E_OK && retry_counter-- > 0);

  if (retval == E_OK) 
    return PB_OK;
  else 
    return PB_READ_ERROR;

}


pwr_tUInt16 pb_cmi_set_data(int fp,
		            pwr_tUInt8 data_id,
		            pwr_tUInt16 offset,
		            pwr_tUInt16 data_size,
		            void *data_ptr)
{
  USIGN16 retval;
  cmi_data_access_struct user_arg;
  int retry_counter;

  user_arg.data_id = data_id;
  user_arg.offset = offset;
  user_arg.data_ptr = (USIGN8 *) data_ptr;
  user_arg.retval_ptr = &retval;
  retry_counter = 10;
  do {
       write(fp, (char *) &user_arg, data_size);
  }
  while(retval != E_OK && retry_counter-- > 0);

  if (retval == E_OK) 
    return PB_OK;
  else 
    return PB_WRITE_ERROR;

}


pwr_tUInt16 pb_get_slave_info(int fp,
		              pwr_tUInt16 slave_address,
		              pwr_tUInt16 *offset_inputs,
		              pwr_tUInt16 *offset_outputs,
		              pwr_tUInt16 *bytes_of_input,
		              pwr_tUInt16 *bytes_of_output)
{
  pwr_tUInt16 data_len;
  pwr_tUInt16 sts;

  T_PROFI_SERVICE_DESCR sdb;
  T_DP_GET_SLAVE_PARAM_REQ gpr; 

  USIGN8 data[256];
  T_DP_SLAVE_PARAM_SLAVE_INFO *spi;

  sdb.comm_ref = 0;
  sdb.layer = DP;
  sdb.service = DP_GET_SLAVE_PARAM;
  sdb.primitive = REQ;
  sdb.invoke_id = 0;
  sdb.result = 0;

  gpr.identifier = DP_SLAVE_PARAM_SLAVE_INFO;
  gpr.rem_add = slave_address;

  sts = pb_cmi_flush(fp);

  if (sts != PB_OK) return sts;

  sts = pb_cmi_write(fp, &sdb, &gpr, sizeof(T_DP_GET_SLAVE_PARAM_REQ)); 

  if (sts != PB_OK) return sts;

  sts = pb_cmi_read(fp, &sdb, &data, &data_len);

  if (sts == E_OK) {
    if (sdb.result == POS) {
      spi = (T_DP_SLAVE_PARAM_SLAVE_INFO *) &(data[4]);
      *offset_inputs = spi->offset_inputs;
      *offset_outputs = spi->offset_outputs;
      *bytes_of_input = spi->number_inputs;
      *bytes_of_output = spi->number_outputs;
      return PB_OK;
    }
    else 
      return PB_NEG_CONF;
  }
  else
    return NO_CON_IND_RECEIVED;
}


pwr_tUInt16 pb_get_slave_cfg(int fp,
		 	     pwr_tUInt16 slave_address,
			     pwr_tUInt16 *cfg_data_len,
			     void *cfg_data)
{
  T_PROFI_SERVICE_DESCR sdb;
  T_DP_GET_CFG_REQ gcr; 
  USIGN8 data[256];
  short data_len;
  pwr_tUInt16 sts;

  sdb.comm_ref = 0;
  sdb.layer = DP;
  sdb.service = DP_GET_CFG;
  sdb.primitive = REQ;
  sdb.invoke_id = 0;
  sdb.result = 0;

  gcr.rem_add = slave_address;

  sts = pb_cmi_flush(fp);
  if (sts != PB_OK) return sts;

  sts = pb_cmi_write(fp, &sdb, &gcr, sizeof(T_DP_GET_CFG_REQ));
  if (sts != PB_OK) return sts;

  sts = pb_cmi_read(fp, NULL, &data, &data_len);
  if (sts != PB_OK) return sts;

  *cfg_data_len = data_len - 4;				// Minus header
  memcpy(cfg_data, &(data[4]), *cfg_data_len);

  return sts;
}   

pwr_tUInt16 pb_download_all(int fp,
		            pwr_tUInt16 slave_address,
		            pwr_tUInt8 wd_fact1,
		            pwr_tUInt8 wd_fact2,
		            pwr_tUInt8 min_tsdr,
		            pwr_tUInt16 ident_number,
		            pwr_tUInt8 group_ident,
		            pwr_tUInt16 prm_user_data_len,
		            void *prm_user_data,
		            pwr_tUInt16 cfg_data_len,
		            void *cfg_data)
			    

{
  T_PROFI_SERVICE_DESCR sdb;
  pwr_tUInt16 sts;
  int i;

  struct {
    T_DP_DOWNLOAD_REQ drp; 
    unsigned char param[512];
  } slave_data;

  T_DP_SLAVE_PARA_SET prm_head;
  T_DP_PRM_DATA prm_data;
  T_DP_AAT_DATA aat_data;
  T_DP_SLAVE_USER_DATA user_data;
  USIGN16 download_data_size;
  USIGN16 data_len;

  download_data_size = sizeof(prm_head) + sizeof(prm_data) + 
			prm_user_data_len + cfg_data_len +
			sizeof(aat_data) + sizeof(user_data);

  data_len = sizeof(slave_data.drp) + download_data_size;

  sdb.comm_ref = 0;
  sdb.layer = DP;
  sdb.service = DP_DOWNLOAD_LOC;
  sdb.primitive = REQ;
  sdb.invoke_id = 0;
  sdb.result = 0;

  slave_data.drp.data_len =  download_data_size;
  slave_data.drp.rem_add = 0;
  slave_data.drp.area_code = slave_address;
  slave_data.drp.add_offset = 0;

  prm_head.slave_para_len = swap16(download_data_size);
  prm_head.sl_flag = DP_SL_NEW_PRM | DP_SL_ACTIVE;
  prm_head.slave_type = DP_SLAVE_TYPE_DP;
  for (i=0; i<12; i++) prm_head.reserved[i] = 0;
  
  i = 0;
  memcpy(&slave_data.param[i], &prm_head, sizeof(prm_head));
  i += sizeof(prm_head);

  prm_data.prm_data_len = swap16(sizeof(prm_data) + prm_user_data_len);
  prm_data.station_status = DP_PRM_LOCK_REQ | DP_PRM_WD_ON;
  prm_data.wd_fact_1 = wd_fact1;
  prm_data.wd_fact_2 = wd_fact2;
  prm_data.min_tsdr = min_tsdr;
  prm_data.ident_number = swap16(ident_number);
  prm_data.group_ident = group_ident;

  memcpy(&slave_data.param[i], &prm_data, sizeof(prm_data));
  i += sizeof(prm_data);

  memcpy(&slave_data.param[i], prm_user_data, prm_user_data_len);
  i += prm_user_data_len;

  memcpy(&slave_data.param[i], cfg_data, cfg_data_len);
  i += cfg_data_len;

  aat_data.aat_data_len = swap16(4);		// AAT data not used in array mode
  aat_data.number_inputs = 0;
  aat_data.number_outputs = 0;

  memcpy(&slave_data.param[i], &aat_data, sizeof(aat_data));
  i += sizeof(aat_data);

  user_data.slave_user_data_len = swap16(2);

  memcpy(&slave_data.param[i], &user_data, sizeof(user_data));
  i += sizeof(user_data);

  sts = pb_cmi_flush(fp);
  if (sts != PB_OK) return sts;

  sts = pb_cmi_write(fp, &sdb, &slave_data, data_len); 
  if (sts != PB_OK) return sts;

  sts = pb_cmi_read(fp, NULL, NULL, NULL);
  return sts;
}
