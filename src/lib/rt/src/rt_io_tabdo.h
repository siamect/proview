/* 
 * Proview   $Id: rt_io_tabdo.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_io_tabdo_h
#define rt_io_tabdo_h

#ifndef rt_io_filter_po_h
#include "rt_io_filter_po.h"
#endif

pwr_sClass_do_value_base  *io_IniDOvalueP;	/* Pointer to DO-value */	
pwr_tUInt16		  io_IniDOold   [128];	/* Old/saved value bitdata */	
pwr_sClass_Do_7437_83	  *io_IniDOcon  [128];	/* Pointer to config object */	
pwr_tInt32		  io_IniDOcsr   [128];	/* Device adress 		*/  
pwr_tInt16		  io_IniDoKortnr[128];	/* Word number on device */
pwr_tUInt16		  io_IniDORackAddr[128];	/* Rack address for RTP */
pwr_tUInt16		  io_IniDOCardType[128];	/* Card type for RTP */
pwr_tObjid		  io_IniDoCardId[128];	/* Objid for rtp dioc 929214 GPd */

io_sFilterCard_Po	  io_IniFilterCardPo[128]; /* Filter data */
pwr_tFloat32		  io_IniDOScanTimePLC;	 /* Scan time in seconds */

pwr_tInt32		  io_actantdo;  	/*  Number of do signals in system */
#endif
