/* 
 * Proview   $Id: rt_io_rapisys.h,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_io_rapisys.h -- System specific rapi definitions */

/*
modification history
--------------------
950108,LT	Ported to vxWorks from INTERAY demo source code.
			Removed HOST_TYPE definition uses only OS9 (VME) host.
*/

#ifndef rt_io_rapisys_h
#define rt_io_rapisys_h

#ifdef __cplusplus
extern	"C"	{
#endif

/*
DESCRIPTION
This file must include the #defines MODULE_TYPE

	The MODULE_TYPE can be:

	#define MODULE_TYPE IPCAN   for the IPCAN modules
	#define MODULE_TYPE IPMIO   for the IPMIO modules
	#define MODULE_TYPE PCCAN2  for the PCCAN2 board


*/

#define IPCAN    1
#define IPMIO    2
#define PCCAN2   3

#define C51      1
#define C166     2
#define C68000   3
#define PC       4
#define WINDOWS  5
#define OS2      6
#define OS9      7
#define PDOS     8


#define MODULE_TYPE 	IPCAN                      /* Define MODULE type here  */
#define HOST_WORD    MOTOROLA

#define MOTOROLA  0
#define INTEL     1


#if     MODULE_TYPE  == IPCAN                   /* IPCAN definitions */
#define MODULE_WORD     INTEL
#define RAPI_WORD       MOTOROLA
#define DPM_BASE        0x8800
#define DPM_SIZE        2048
#define DPM_WIDTH       1

#define NUMB_DEV        100
#define NUMB_MODULE     255
#define NUMB_IDEN       2048

#define NUMB_INP_DEV    50
#define NUMB_OUTP_DEV   50

#define NUMB_INP_LINE   20
#define NUMB_OUTP_LINE  20
#define NUMB_ADC_LINE   20
#define NUMB_SETP_REG   20
#define NUMB_LOC_VAR    20
#define NUMB_GLOB_VAR   20
#define NUMB_CHAN_IN    20
#define NUMB_CHAN_OUT   20

#define CC0INT   0x10
#define CC1INT   0x11
#define CC2INT   0x12
#define CC3INT   0x13
#define CC4INT   0x14
#define CC5INT   0x15
#define CC6INT   0x16
#define CC7INT   0x17
#define CC8INT   0x18
#define CC9INT   0x19
#define CC10INT  0x1A
#define CC11INT  0x1B
#define CC12INT  0x1C
#define CC13INT  0x1D
#define CC14INT  0x1E
#define CC15INT  0x1F

#define T0INT    0x20
#define T1INT    0x21
#define T2INT    0x22
#define T3INT    0x23
#define T4INT    0x24
#define T5INT    0x25
#define T6INT    0x26
#define CRINT    0x27

#define ADCINT   0x28
#define ADEINT   0x29

#define S0TINT   0x2A
#define S0RINT   0x2B
#define S0EINT   0x2C
#define S1TINT   0x2D
#define S1RINT   0x2E
#define S1EINT   0x2F
#endif

#if     MODULE_TYPE  == PCCAN2                  /* PCCAN2 definitions */
#define MODULE_WORD     MOTOROLA
#define RAPI_WORD       INTEL
#define DPM_BASE        0x4000
#define DPM_SIZE        4096
#define DPM_WIDTH       2

#define NUMB_DEV        50
#define NUMB_MODULE     255
#define NUMB_IDEN       2048

#define NUMB_INP_DEV    50
#define NUMB_OUTP_DEV   50

#define NUMB_INP_LINE   20
#define NUMB_OUTP_LINE  20
#define NUMB_ADC_LINE   20
#define NUMB_SETP_REG   20
#define NUMB_LOC_VAR    20
#define NUMB_GLOB_VAR   20
#define NUMB_CHAN_IN    20
#define NUMB_CHAN_OUT   20
#endif


#ifdef __cplusplus
}
#endif

#endif

