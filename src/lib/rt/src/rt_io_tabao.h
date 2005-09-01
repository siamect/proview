/* 
 * Proview   $Id: rt_io_tabao.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_io_tabao_h
#define rt_io_tabao_h

pwr_sClass_ao_value_base *io_IniAOvalP;		/* Pointer to AO-value 	*/
pwr_sClass_Ao_7455_30	*io_IniAOcon	[512];	/* Pointer to card obj  */
pwr_tInt32			io_IniAOcsr   	[512];	/* Device adress	*/  
pwr_tInt16			io_IniAOchan  	[512];	/* Channel on device   	*/
pwr_tFloat32			io_IniAOold   	[512];	/* Old/saved value 	*/	
pwr_sClass_Ao		*io_IniAO_obj	[512];	/* Pointer to AO_OBJECT	*/	
pwr_sClass_ChanAo	*io_IniAOChObj	[512];	/* Pointer to chan_AO obj*/

pwr_tUInt16			io_IniAORackAddr[512]; /* Rack address for RTP */
pwr_tUInt16			io_IniAOCardType[512]; /* Card type for RTP */
pwr_tObjid      io_IniAOconobjid[512];

pwr_tInt32		io_actantao;	        /*  Number of ao signals in system  */

#endif
