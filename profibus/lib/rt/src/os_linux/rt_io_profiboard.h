/* 
 * Proview   $Id: rt_io_profiboard.h,v 1.1 2006-01-16 13:56:52 claes Exp $
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

#ifndef __RT_IO_M_PB_UTILS__
#define __RT_IO_M_PB_UTILS__

#define PB_OK			0
#define PB_DEVICE_ERROR		1
#define PB_READ_ERROR		2
#define PB_WRITE_ERROR		3
#define PB_NO_CON_IND_RECEIVED	4
#define PB_NEG_CONF		5

extern
unsigned short swap16(unsigned short in);

extern
unsigned int swap32(unsigned int in); 

extern
pwr_tUInt16 pb_cmi_init(int fp);

extern
pwr_tUInt16 pb_cmi_write(int fp,
		         T_PROFI_SERVICE_DESCR *sdb_ptr,
		         void *data_ptr,
		         pwr_tUInt16 data_len); 

extern
pwr_tUInt16 pb_set_stalltime(int fp, unsigned short t);

extern
pwr_tUInt16 pb_cmi_read(int fp,
		        T_PROFI_SERVICE_DESCR *sdb_ptr,
		        void *data_ptr,
		        pwr_tUInt16 *data_len_ptr);

extern
pwr_tUInt16 pb_cmi_poll(int fp,
		        T_PROFI_SERVICE_DESCR *sdb_ptr,
		        void *data_ptr,
		        pwr_tUInt16 *data_len_ptr);

extern
pwr_tUInt16 pb_cmi_flush(int fp);

extern
pwr_tUInt16 pb_cmi_get_data(int fp,
		            pwr_tUInt8 data_id,
		            pwr_tUInt16 offset,
		            pwr_tUInt16 data_size,
		            void *data_ptr);

extern
pwr_tUInt16 pb_cmi_set_data(int fp,
		            pwr_tUInt8 data_id,
		            pwr_tUInt16 offset,
		            pwr_tUInt16 data_size,
		            void *data_ptr);

extern
pwr_tUInt16 pb_get_slave_info(int fp,
		              pwr_tUInt16 slave_address,
		              pwr_tUInt16 *offset_inputs,
		              pwr_tUInt16 *offset_outputs,
		              pwr_tUInt16 *bytes_of_input,
		              pwr_tUInt16 *bytes_of_output);

extern
pwr_tUInt16 pb_get_slave_cfg(int fp,
		 	     pwr_tUInt16 slave_address,
			     pwr_tUInt16 *cfg_data_len,
			     void *cfg_data);

extern
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
		            void *cfg_data);

#endif

