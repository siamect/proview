/* 
 * Proview   $Id: rt_io_mb_locals.h,v 1.3 2008-09-30 14:29:56 claes Exp $
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

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#define IO_MAXCHAN 96
#define MAX_MSGS_LOST 5

// These constants are obsolete from V4.1, except for the old style
// (Pb_Di, Pb_Do etc)

#define PB_MODULE_STATE_NOTINIT 0
#define PB_MODULE_STATE_OPERATE 1

//#define PB_SLAVE_STATE_NOTINIT 0
//#define PB_SLAVE_STATE_STOPPED 1
//#define PB_SLAVE_STATE_OPERATE 2

//#define PB_MASTER_STATE_NOTINIT 0
//#define PB_MASTER_STATE_STOPPED 1
//#define PB_MASTER_STATE_CLEARED 2
//#define PB_MASTER_STATE_OPERATE 3

//#define PB_STALLACTION_NONE	0
//#define PB_STALLACTION_RESET 	1
//#define PB_STALLACTION_BREAK 	2

#define PB_NUMREP_UNSIGNEDINT	0
#define PB_NUMREP_SIGNEDINT	1
#define PB_NUMREP_FLOATIEEE	2
#define PB_NUMREP_FLOATVAX	3
#define PB_NUMREP_FLOATINTEL	4

//#define PB_BYTEORDERING_LE	0
//#define PB_BYTEORDERING_BE	1

#define PB_ORIENTATION_BYTE	8
#define PB_ORIENTATION_WORD	16
#define PB_ORIENTATION_DWORD	32

#define PB_UDATA_DIAG 		1

typedef pwr_tMask mb_tSendMask;

typedef enum {
  mb_mSendMask_ReadReq               = 1,
  mb_mSendMask_WriteReq              = 2,
} mb_mSendMask;

typedef struct {
  int initialized;
} io_sAgentLocal;

typedef struct {
  int initialized;
  int s;
  short int trans_id;
  struct sockaddr_in rem_addr;		/* Remote socket description */
  struct sockaddr_in loc_addr;		/* Remote socket description */
  int input_size;
  int output_size;
  int expected_msgs;
  int msgs_lost;
  pwr_tTime last_try_connect_time;
} io_sRackLocal;

typedef struct {
  void *input_area;
  void *output_area;
  int scancount[IO_MAXCHAN];
  int trans_id;
  int input_size;
  int output_size;
  short int no_di;
  short int no_do;
} io_sCardLocal;

#pragma pack(1)
typedef struct
{
  unsigned char protocol_id[2];
  short int msg_size;
  short int msg_id[2];
} remote_tcp_header;

typedef struct _mbap_header {
  short int trans_id;
  short int proto_id;
  short int length;
  unsigned char      unit_id;
} mbap_header;

typedef struct _read_req {
  mbap_header  head;
  unsigned char         fc;
  short int    addr;
  short int    quant;
} read_req;

typedef struct _rec_buf {
  mbap_header  head;
  short int    buf[1000];
} rec_buf;

typedef struct _write_single_req {
  mbap_header  head;
  unsigned char         fc;
  short int    addr;
  short int    value;
} write_single_req;

typedef struct _write_reg_req {
  mbap_header  head;
  unsigned char         fc;
  short int    addr;
  short int    quant;
  unsigned char         bc;
  short int    reg[125];
} write_reg_req;

typedef struct _write_coils_req {
  mbap_header  head;
  unsigned char         fc;
  short int    addr;
  short int    quant;
  unsigned char         bc;
  unsigned  char    reg[250];
} write_coils_req;

typedef struct _res_write {
  unsigned char         fc;
  short int    addr;
  short int    quant;
  short int    buf[250];
} res_write;

typedef struct _res_read {
  unsigned char         fc;
  unsigned char         bc;
  short int    buf[250];
} res_read;

typedef struct _res_fault {
  unsigned char         fc;
  unsigned char         ec;
} res_fault;

#pragma pack(0)

pwr_tStatus mb_recv_data(io_sRackLocal *local, 
                             io_sRack      *rp,
			     pwr_sClass_Modbus_TCP_Slave *sp);

pwr_tStatus mb_send_data(io_sRackLocal *local, 
                             io_sRack      *rp,
			     pwr_sClass_Modbus_TCP_Slave *sp, 
			     mb_tSendMask   mask);


