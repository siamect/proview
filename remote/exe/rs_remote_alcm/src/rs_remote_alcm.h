/* 
 * Proview   $Id: rs_remote_alcm.h,v 1.1 2006-01-12 06:39:33 claes Exp $
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

/***** Meddelandekoder i ethernetheader *****/

#define ALCM_MTYP_DATA 0
#define ALCM_MTYP_RESP 1
#define ALCM_MTYP_INIT 2

/***** Transkoder i applikationsbufferten *****/

#define ALCM_TRACO_CSPACK 1
#define ALCM_TRACO_APLACK 2
#define ALCM_TRACO_CSPNAK 3
#define ALCM_TRACO_APLNAK 4
#define ALCM_TRACO_BSPACK 5
#define ALCM_TRACO_BSPNAK 6
#define ALCM_TRACO_COFACK 7
#define ALCM_TRACO_COFNAK 8

#define NET_HEADER_SIZE 48

/*
 * Type definitions
 */

typedef unsigned short int channel_type;

typedef struct 
{
  unsigned short int completion_status, transfer_size, w3, w4;
} iosb_type;

typedef struct		
{
  char destination_address[6];			/* Destination address (us) */
  char source_address[6];			/* Source address (remnode) */
  short int protocol_type;			/* Type of protocol (60-06) */
} net_header_type;

typedef struct
{
  char msg_type;				/* Type of message */
  unsigned char msg_seqnum;			/* Sequence number */
  unsigned char filler[46];			/* Padding */
	
  unsigned short data[725];			/* Data */
} net_buffer_type;

typedef struct
{
  short int trans_code;
  short int future_use;
  short int length;
  short int receive_task[2];
  unsigned char data[1440];
} apl_buffer;

typedef struct
{
  short int trans_code;
  short int future_use;
  short int length;
  short int receive_task[2];
  unsigned short int time[7];
} clock_buffer;

typedef struct
{
  short int trans_code;
  short int common_name[2];
  short int offset;
  unsigned char data[1442];
} csp_buffer;
