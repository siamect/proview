/* 
 * Proview   $Id: rt_io_abbsr.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_io_abbsr_h
#define rt_io_abbsr_h

#define	STX		2
#define	CR		13
#define	TIMEOUT	        300		/* Time out in msec */
#define STATUSWORT	4
#define STORWORT	5
#define DREHZAHLISTWERT	6
#define WAHLISTWERT	7

typedef struct {
pwr_tUInt16		steuerw;		/* Steuerwort 			*/
pwr_tInt16		leitsw;			/* Leitsollwert			*/
pwr_tInt16		leitzusatzsw;		/* Leitzusatzsollwert		*/
} abbsr_utdata;

typedef struct {
pwr_tUInt16		statusw;		/* Statuswort			*/
pwr_tUInt16		storw;	   		/* Störwort			*/
pwr_tInt16		drehzw;			/* Drehzahlistwert		*/
pwr_tInt16		wahlistw;		/* Wahlistwert			*/
} abbsr_indata;

pwr_tInt32	io_abbsrkom(	PORT		io_tt,
			pwr_tUInt8		padnr,
		   	MESSAGE		*tty_read_messobj,
		   	char		**tty_read_message,
		   	MESSAGE		*tty_write_messobj,
		   	char		**tty_write_message,
			abbsr_indata 	*indataP,
			abbsr_utdata	*utdataP);

pwr_tUInt8	io_abbsr_bcc( 	pwr_tUInt8 		buffer[] );

#endif
