#ifndef rt_io_abbsr_h
#define rt_io_abbsr_h

/* rt_io_abbsr.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
