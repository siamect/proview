/* 
 * Proview   $Id: remote.h,v 1.1 2006-01-12 06:39:33 claes Exp $
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

#define even(x) (((int)(x) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif
#define true 		1
#define false 		0
#define TRUE 		1
#define FALSE 		0
#define doomsday false

#define REMTRANS_UNDEF 		0
#define REMTRANS_IO   		1
#define REMTRANS_APPL 		2

#define REMTRANS_IN 		1
#define REMTRANS_OUT   		2

#define RT_TIMEREXP 	1
#define RT_ERROR 	2
#define RT_RECEIVE      3

#define RS_PWR_LOGG_Procno      90	/* RemTransLogg Processnumber */
#define RS_PWR_LOGG_Type        90	/* RemTransLogg message Type  */
#define RS_PWR_LOGG_Class       90	/* RemTransLogg message Class */
#define RS_PWR_LOGG_RemTransId  90	/* RemTransLogg RemTrans Identifier */

#define rs_pwr_logg_qix (1<<31 | 1000)

#define	STATUS_FELSEND	0	/* Send was not successfull */
#define	STATUS_OK	1	/* All is well */
#define	STATUS_LENGTH	2	/* Too big data receive or send */
#define	STATUS_BUFF	3	/* Receive data has been buffered */
#define	STATUS_LOST	4	/* Receive data has been lost */
#define	STATUS_BUFACK	6	/* Sent, wait for ack */

#define PSS_UPD_DV	1	/* Order new value Rem Dv */
#define PSS_UPD_DO	2	/* Order new value Rem Do */
#define PSS_UPD_AO	3	/* Order new value Rem Ao */
#define PSS_Switch_Init	5	/* PLC Switch is coming. Keep stall */
#define PSS_Switch_Done	6	/* Restart. Reinstate ordinary stall */
#define PSS_Follow_On   7       /* To much data. Next telegram is follow on */

#define MAX_ORDER_BUFFERSIZE      108	/* 124 bytes including header */
#define MAX_IO_BUFFERSIZE 	  1422	/* 1440 bytes including header */
#define MAX_ORDERS_DATASIZE	  72	/* More than enough for orders */

#define MAX_ORDER_BUFFERSIZE_VNET   116 /* 128 bytes including header */
#define MAX_RECEIVE_BUFFERSIZE_VNET 256 /* For update of IO and common */


typedef struct {
  short int	trans_code;
  short int	common_name[2];
  short int	di_offset;	/* Byte offset from here to number of di */
  short int	do_offset;	/* Byte offset from here to number of do */
  short int	dv_offset;	/* Byte offset from here to number of dv */
  short int	ai_offset;	/* Byte offset from here to number of ai */
  short int	ao_offset;	/* Byte offset from here to number of ao */
  short int	co_offset;	/* Byte offset from here to number of co */
  unsigned char	data[MAX_IO_BUFFERSIZE];	/* Data :
				short int	Number of bytes for di-area
				unsigned char	Di-area	Number of di / 8 bytes
				short int	Number of bytes for do-area
				unsigned char	Do-area	Number of do / 8 bytes
				short int	Number of bytes for dv-area
				unsigned char	Dv-area	Number of dv / 8 bytes
				short int	Number of ai
				short int	Ai-area not used
				short int	Ai-area Raw-value
				short int	Number of ao
				short int	Ao-area Raw-value
				short int	Number of co
				int		Co-area not used
				int		Co-area Raw-value */
} bsp_buffer;

typedef struct {
  unsigned char	type;		/* DV = 1, DO = 2, AO = 3 , Switch = 5*/
  unsigned char size;		/* DV,DO = 0 (puls) AO = 2 (size) */
  short int	signal;		/* Channel number */
} pssupd_order_header;

typedef struct {
  short int	transcode;
  short int	future_use;
  short int	length;		/* Number of bytes in buffer */
  short int	receive_task[2];/* Task to receive orders */
  short int	common_name[2];	/* Task to receive orders */
  short int	no_of_updates;	/* Number of orders */
  unsigned char	data[MAX_ORDER_BUFFERSIZE];
} pssupd_buffer;

typedef struct {
  short int     length;         /* Number of words in buffer */
  short int     receive_task[2];/* Task to receive orders */
  short int     common_name[2]; /* Task to receive orders */
  short int     no_of_updates;  /* Number of orders */
  unsigned char data[MAX_ORDER_BUFFERSIZE_VNET];
} pssupd_buffer_vnet;

typedef struct {
  short int     length;
  short int     common_name[2];
  short int     offset;
  unsigned char data[MAX_RECEIVE_BUFFERSIZE_VNET];
} common_buffer_vnet;

typedef struct {
  short int     length;
  short int     io_name[2];
  unsigned char data[MAX_RECEIVE_BUFFERSIZE_VNET];
} io_buffer_vnet;

typedef struct {
  pwr_sClass_RemChan_Di	*objp;		/* Pointer to RemChanDi-object */
  pwr_tBoolean		*actval;	/* Pointer to DI_VALUE_BASE */
  void			*next;		/* Pointer to next remdi_item */
} remdi_item;

typedef struct {
  pwr_sClass_RemChan_Do	*objp;		/* Pointer to RemChanDo-object */
  pwr_tBoolean		*actval;	/* Pointer to DO_VALUE_BASE */
  void			*next;		/* Pointer to next remdo_item */
} remdo_item;

typedef struct {
  pwr_sClass_RemChan_Dv	*objp;		/* Pointer to RemChanDv-object */
  pwr_tBoolean		*actval;	/* Pointer to DV_VALUE_BASE */
  void			*next;		/* Pointer to next remdv_item */
} remdv_item;

typedef struct {
  pwr_sClass_RemChan_Ai	*objp;		/* Pointer to RemChanAi-object */
  pwr_tFloat32		*actval;	/* Pointer to AI_VALUE_BASE */
  short int		*rawval;	/* Pointer to rawvalue in Ai-object */
  void			*next;		/* Pointer to next remai_item */
} remai_item;

typedef struct {
  pwr_sClass_RemChan_Ao	*objp;		/* Pointer to RemChanAo-object */
  pwr_tFloat32		*actval;	/* Pointer to AO_VALUE_BASE */
  short int		*rawval;	/* Pointer to rawvalue in Ao-object */
  void			*next;		/* Pointer to next remao_item */
} remao_item;

typedef struct {
  pwr_sClass_RemChan_Co	*objp;		/* Pointer to RemChanCo-object */
  pwr_tInt32		*actval;	/* Pointer to CA_VALUE_BASE */
  pwr_tInt32		*extval;	/* Pointer to CO_VALUE_BASE */
  void			*next;		/* Pointer to next remco_item */
} remco_item;

typedef struct {
  pwr_sClass_RemTrans	*objp;		/* Pointer to RemTrans-object */
  pwr_tObjid		objid;		/* RemTrans Objid */
  char			*datap;		/* Pointer to Data-object */
  void			*next;		/* Pointer to next remtrans_item */
  void		 	*buffp;		/* Pointer to first buffered trans */
  					/* typecast to struct rem_t_transbuff */
  pwr_tFloat32		time_since_send; /* Time since last send */
} remtrans_item;

typedef struct {
  void			*next;		/* Pointer to next buffer */
  void			*remtrans;	/* Pointer to remtrans _item */
  					/* typecast to struct remtrans_item */
  int			size;		/* Number of bytes in data below */
  int			ackbuf;		/* Buffered because of wait for ack */
  char			data;		/* Dataarea */
} rem_t_transbuff;

typedef struct {
  void 			*next;			/* Pointer to next remnode */
  void			*local;			/* Pointer to optional local data structure */
  pwr_tObjid		objid;			/* ObjId of RemNode */
  pwr_sClass_MultiCast  *multicast;             /* Pointer to (first) MultiCast-object under RemNode */
  /* Elements for remtrans use */
  remtrans_item		*remtrans;		/* Pointer to first remtrans_item */
  rem_t_transbuff	*transbuff;		/* Pointer to first buffered trans */
  pwr_tFloat32		retransmit_time;	/* Time to wait until retransmitting */
  pwr_tStatus		laststs;		/* Last send status */
  /* Elements for remio use */
  char			*remio_data;		/* Pointer to remio primary data */
  int			remio_size;		/* Size of primary data bufferobject */
  pwr_tStatus		remio_err;		/* Error message has been sent */
  remdi_item		*remdi;			/* Pointer to first remdi_item */
  remdo_item		*remdo;			/* Pointer to first remdo_item */
  remdv_item		*remdv;			/* Pointer to first remdv_item */
  remai_item		*remai;			/* Pointer to first remai_item */
  remao_item		*remao;			/* Pointer to first remao_item */
  remco_item		*remco;			/* Pointer to first remdco_item */
  int			maxremdi;		/* Number of Di in subsystem */
  int			maxremdo;		/* Number of Do in subsystem */
  int			maxremdv;		/* Number of Me in subsystem */
  int			maxremai;		/* Number of Ai in subsystem */
  int			maxremao;		/* Number of Ao in subsystem */
  int			maxremco;		/* Number of Pi in subsystem */
} remnode_item;
