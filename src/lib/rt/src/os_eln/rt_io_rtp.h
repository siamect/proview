/* 
 * Proview   $Id: rt_io_rtp.h,v 1.2 2005-09-01 14:57:57 claes Exp $
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

#ifndef rt_io_rtp_h
#define rt_io_rtp_h

/* rt_io_rtp.h 
   Contains prototypes, datastructures and defines for
   RTP io routines used in PROVIEW/R.

   This file contains defines for all the cards supported in the
   library and bitmasks for further control.  */

/* Cards supported by the RTP-library  */

/* Card codes */

#define	RTP_DI_7437_37	    1
#define RTP_DO_7437_83	    10
#define RTP_DO_7437_38	    11
#define RTP_DO_7435_40	    12
#define RTP_AI_7436	    20
#define RTP_AO_7455_20	    30
#define RTP_AO_7455_30	    32
#define RTP_PD_7435_26	    34
#define	RTP_CO_7435_33	    40
#define RTP_CO_7437_33	    41		    

/* Control codes to be OR:ed with card codes  */

/* Bits 8-15 */

#define	RTP_DIFFR	    0x00000100		/* Differential input */
#define RTP_OTD		    0x00000200		/* Open transducer detection */

#define RTP_GAIN_10V	    0x00000000		/* 10V gain */
#define RTP_GAIN_5V	    0x00001000		/* 5 V gain */
#define RTP_GAIN_1V	    0x00003000		/* 1 V gain */
#define RTP_GAIN	    0x0000F000

/*************************************************/

#define RTP_AD_ADR	    0			/* Adress of AD-Converter */
/*** Command codes ***/

#define IOBC_RESET	0x04	/* Make reset on IOBC		        */

/*  Define Command Codes  */

#define CMD_IOBC_OUT	0x01	/* IOBC command word output		*/
#define CMD_IOBC_STAT	0x02	/* Read IOBC status			*/
#define CMD_ATOMIC	0x03	/* Execute direct control list		*/
#define CMD_LOAD_DMA	0x04	/* Load DMA command list		*/
#define CMD_UNLOAD_DMA	0x05	/* Unload DMA command list		*/
#define CMD_EXEC_DMA	0x06	/* Execute selected DMA list		*/
#define CMD_RTP_LIST	0x07	/* Execute RTP I/O list			*/
#define CMD_SET_TIMES	0x0C	/* Set timeout values			*/
#define CMD_CONNECT	0x0D	/* Connect RTP interrupt to caller	*/
#define CMD_DISCONNECT	0x0E	/* Disconnect RTP interrupt		*/
#define CMD_READ_TIME	0x0F	/* Read time kept by timer interrupt	*/

/*  Define List I/O Control Codes  */

#define CTL_DI		0x11	/* Digital input			*/
#define CTL_DAO		0x12	/* Digital/analog output		*/
#define CTL_AI_P	0x13	/* Analog input -- polled		*/
#define CTL_WRAI_P	0x14	/* Slow (WR) analog input -- polled	*/
#define CTL_AI_I	0x15	/* Analog input -- interrupt		*/
#define CTL_AR_P	0x16	/* AI / auto-ranging -- polled		*/
#define CTL_AR_I	0x17	/* AI / auto-ranging -- interrupt	*/
#define CTL_HSM_DI	0x21	/* HSM digital input			*/
#define CTL_HSM_DAO	0x22	/* HSM digital/analog output		*/
#define CTL_HSM_GAINS	0x23	/* HSM load AI gains			*/
#define CTL_HSM_AI	0x24	/* HSM analog input			*/
#define CTL_HSM_RMS_P	0x25	/* HSM RMS input -- polled		*/
#define CTL_HSM_RMS_I	0x26	/* HSM RMS input -- interrupt		*/

/*  Define completion error codes  */

#define RTP$_STOPPED		3	/* Scan terminated by command	*/
#define RTP$_BAD_DPPI		5	/* DPPI address selection error	*/

#define RTP$_INIT		2	/* Driver not initialized, or	*/
					/* ... INIT called while up	*/
#define RTP$_INVALID_CMD	4	/* Invalid command code		*/
#define RTP$_INVALID_CTL	6	/* Invalid control code		*/
#define RTP$_INVALID_DEV	8	/* Invalid device number	*/
#define RTP$_NO_BUFFER		10	/* Required buffer not supplied	*/
#define RTP$_BAD_PARAM		12	/* Invalid parameter submitted	*/
#define RTP$_INUSE		14	/* Intrpt vector already in use	*/


extern pwr_tInt16 io_RTPerrno;

/*** Prototypes ***/

extern pwr_tBoolean	rtp_init();

extern pwr_tUInt16	rtpdi_read(pwr_tUInt8 Device,pwr_tUInt8 Card,pwr_tUInt8 Word,pwr_tUInt32 Type);

extern void	rtpdo_write(pwr_tUInt16 Data,pwr_tUInt8 Device
			   ,pwr_tUInt8 Card,pwr_tUInt8 Word,pwr_tUInt32 Type);

extern pwr_tUInt16	rtpai_read(pwr_tUInt8 Device,pwr_tUInt8 Card,pwr_tUInt8 Channel,pwr_tUInt32 Type);

extern void	rtpao_write(pwr_tUInt16 Data,pwr_tUInt8 Device
			   ,pwr_tUInt8 Card,pwr_tUInt8 Channel,pwr_tUInt32 Type);

extern void	rtpco_read(pwr_tUInt16 *Data,pwr_tUInt8 Device
			   ,pwr_tUInt8 Card,pwr_tUInt8 Word,pwr_tUInt32 Type);

extern void	rtpco_write(pwr_tUInt16 Data,pwr_tUInt8 Device,pwr_tUInt8 Card,pwr_tUInt32 Type);

#endif
