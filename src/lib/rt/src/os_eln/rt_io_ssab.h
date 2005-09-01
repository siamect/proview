/* 
 * Proview   $Id: rt_io_ssab.h,v 1.2 2005-09-01 14:57:57 claes Exp $
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

#ifndef rt_io_ssab_h
#define rt_io_ssab_h

/* rt_io_ssab.h 

   Contains prototypes, datastructures and defines for
   SSAB io routines used in PROVIEW/R.

   This file contains defines for all the cards supported in the
   library and bitmasks for further control.  */

#include "pwr.h"

#define	MASKAI	 	0xFFF		/*  No polling bit in output data */
#define	READYAI	  	0x8000		/*  AI-read polling ready	  */
#define	AI_FAULT 	0xFFFF		/*  Convesion fault AI		  */
#define	RMAX	 	100		/*  Number of AI-read polling     */
#define	MAXMACHERR	10		/*  Number of machine exeption 
					    before exit                   */
typedef struct 	
       	{
	    unsigned short	csr[32];
	}register_def;  

typedef struct 	
       	{
	    unsigned short	aicsr0;
	    unsigned short	aicsr1;
	}ai_register_def;  

typedef	struct 	
       	{
	    unsigned short	cocsr1[4];
	    unsigned short	cocsr2[4];
	}co_register_def;  

extern	pwr_tBoolean	io_writeerr;
extern	pwr_tBoolean	io_readerr;
extern	unsigned int io_readerr_co_nb;
extern	unsigned int io_readerr_co_sts;
extern	pwr_tBoolean io_fatal_error;

int 		ssab_close( int *dev);

int		ssabd_ini(  char *inidev,	
			    int	adressini,
			    int	hv_device,
			    int	relvec);

int		ssabai_ini( char *inidevai,
			    int adressiniai,
			    int hv_deviceai,
			    int relvec );


int		ssabao_ini( char *inidevao,
			    int adressiniao,
			    int hv_deviceao,
			    int relvec );

unsigned short	ssabdi_read(register_def    *reg_diP,
			    int		    grupp);

void		ssabdo_write(unsigned short bit16_dodata,
			    register_def    *reg_doP,
			    int		    grupp);

unsigned short	ssabai_read(ai_register_def *reg_aiP,
			    int		    ai_channel);

short		ssabaiup_read(register_def *reg_aiP,
			    int		    ai_channel);

void		ssabao_write(unsigned short	bit16_aodata,
			     register_def	*reg_aoP,
			     int		ao_channel);

void		ssabco_read(co_register_def	*reg_coP,
			    int			numofco,
                            int			*co_id_p , 
			    unsigned short	*co_outvalueP,
			    int			channel);

void		ssabco_read2(co_register_def	*reg_coP,
			    int			numofco,
                            int			*co_id_p , 
			    unsigned short	*co_outvalueP,
			    int			channel);


void		ssabco_write(unsigned short	*codata,
			     co_register_def	*reg_coP,
			     int		channel);

void		SSABAIINT();
void		SSABAOINT();

pwr_tBoolean	 machfailread_co();
pwr_tBoolean	 machfailread();
pwr_tBoolean	 machfailwrite();

#endif
