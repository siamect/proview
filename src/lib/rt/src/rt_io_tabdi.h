/* 
 * Proview   $Id: rt_io_tabdi.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_io_tabdi_h
#define rt_io_tabdi_h

#ifndef rt_io_filter_di_h
#include "rt_io_filter_di.h"
#endif

pwr_sClass_di_value_base  *io_IniDIvalueP;	/* Pointer DI-value	*/	
pwr_tInt32		  io_IniDIcsr[128];	/* Device adress	*/  
pwr_sClass_Di_7437_37	  *io_IniDIcon[128];	/* Pointer card   DI*/	
pwr_tInt16		  io_IniDIKortnr[128];	/* Number of word on device */
pwr_tUInt16		  io_IniDIRackAddr[128];	/* Rack address for RTP */
pwr_tUInt16		  io_IniDICardType[128];	/* Card type for RTP */
pwr_tObjid		  io_IniDICardId[128];	/* GPd 920212 , rtp dioc */

io_sFilterCard_Di	  io_IniFilterCardDi[128]; /* Filter data */
pwr_tFloat32		  io_IniDIScanTimePLC;	 /* Scan time in seconds */

pwr_tInt32		  io_actantdi;		/*  Number of DI signals */

#endif
