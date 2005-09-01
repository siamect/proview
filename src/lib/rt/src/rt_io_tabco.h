/* 
 * Proview   $Id: rt_io_tabco.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_io_tabco_h
#define rt_io_tabco_h

pwr_sClass_co_value_base  *io_IniCOvalueP;    /* Pointer CO-value	*/	
pwr_sClass_ca_value_base  *io_IniCAvalueP;    /* Pointer CA-value	*/	
pwr_tInt32		  io_IniCOcsr[64];    /* Device adress	*/  
pwr_sClass_Co_PI24BO	  *io_IniCOcon[64];   /* Pointer cardobj  */	
pwr_sClass_ChanCo	  *io_IniCOChan[64];  /* Pointer to chan_CO obj*/
pwr_tInt32		  io_IniCAold[256];   /* Saved CA values	*/  

pwr_tUInt16		  io_IniCORackAddr[64]; /* Rack address for RTP */
pwr_tUInt16		  io_IniCOCardType[64]; /* Card type for RTP */
pwr_tObjid		  io_IniCoCardId[64];	/* GPd 920212 , rtp dioc */


pwr_tInt32		  io_actantco;	    /*  Number of CO signals */
pwr_tBoolean		  io_co_firstlap;

#endif
