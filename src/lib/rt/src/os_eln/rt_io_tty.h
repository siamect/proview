#ifndef rt_io_tty_h
#define rt_io_tty_h

/* rt_io_tty.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Prototype declarations of rs_io_tty routines.  */

pwr_tInt32	io_tty_ini   ( 		pwr_tInt8  	ttnr[], 
				PORT 	*ttport,
		   		MESSAGE	*io_tty_read_messobj,
		   		char	**io_tty_read_message,
				pwr_tInt32	rx_mess_size,
		   		MESSAGE	*io_tty_write_messobj,
		   		char	**io_tty_write_message,
				pwr_tInt32	tx_mess_size);

pwr_tInt32	io_tty_sdrc  ( 		PORT  	*srport,
				pwr_tUInt8 	*sbuf,
				pwr_tInt32 	sant, 
		               	pwr_tUInt8 	*mbuf,
				pwr_tInt32 	mant,  
				MESSAGE	*tty_read_messobj,
		   		char	**tty_read_message,
		   		MESSAGE	*tty_write_messobj,
		   		char	**tty_write_message,
				pwr_tUInt16 	timeout,
		               	pwr_tInt32 	nrofsnd,
				DDA$_BREAK_MASK termmask,
		               	pwr_tInt32 	*rcvant);

pwr_tInt32    io_tty_typecl( 	PORT	*clttport,	
		   		MESSAGE	*io_tty_read_messobj,
		   		char	**io_tty_read_message);

pwr_tInt32	io_tty_typere(		PORT	*rettport,
				pwr_tUInt8	*mtbuf,
				pwr_tInt32	tant,
			       	pwr_tInt32 	*rcvtant);

pwr_tInt32 	io_tty_exit  ( 		PORT 	*exttport);

pwr_tInt32 	io_tty_readfault  (	PORT	*fttport);

pwr_tInt32 	io_tty_set7bin  ( 	PORT  *sstport);

#endif
