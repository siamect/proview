/* 
 * Proview   $Id: rt_io_ip.h,v 1.2 2005-09-01 14:57:57 claes Exp $
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

#ifndef rt_io_ip_h
#define rt_io_ip_h

/* rt_io_ip.h -- IP structures. */

#ifdef __cplusplus
extern "C" {
#endif

// Valid for Big endian only
typedef struct {
  unsigned char		filler_0;
  unsigned char		ascii0;  	// "I"
  unsigned char		filler_1;
  unsigned char		ascii1;     	// "P"
  unsigned char		filler_2;
  unsigned char		ascii2;         // "A"
  unsigned char		filler_3;
  unsigned char		frequence;	// "C" = 8 MHz, "H" = 32 MHz
  unsigned char		filler_4;
  unsigned char		manId;		// Manuacturer ID
  unsigned char		filler_5;
  unsigned char		modelNo;	// Model number
  unsigned char		filler_6;
  unsigned char		revision;	// Revision
  unsigned char		filler_7;
  unsigned char		reserved1;	// Reserved
  unsigned char		filler_8;
  unsigned char		driverIdLow;	// Driver ID, low byte
  unsigned char		filler_9;
  unsigned char		driverIdHigh;	// Driver ID, high byte
  unsigned char		filler_10;
  unsigned char		usedBytes;	// No of bytes used
  unsigned char		filler_11;
  unsigned char		crc;		// CRC
  unsigned char		userSpace[32];
} io_sIpId; 


#ifdef __cplusplus
}
#endif

#endif
