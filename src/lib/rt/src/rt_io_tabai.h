/* 
 * Proview   $Id: rt_io_tabai.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_io_tabai_h
#define rt_io_tabai_h

#ifndef rt_io_filter_ai_h
#include "rt_io_filter_ai.h"
#endif

pwr_sClass_ai_value_base *io_IniAIvalP;		/* Pointer to AI-value 	*/
pwr_sClass_Ai_7436	*io_IniAIcon	[64];	/* Pointer to Card obj  */
pwr_tInt32			io_IniAIcsr 	[64];	/* Device adress	*/  
pwr_sClass_Ai		*io_IniAI_obj	[128];	/* Pointer to AI_OBJECT	*/	
pwr_sClass_ChanAi	*io_IniAIChan	[128];	/* Pointer to chan_AI obj*/
pwr_sClass_ChanAit	*io_IniAITChan	[128];	/* Pointer to chan_AIT obj*/
pwr_tInt16			io_IniAIscanint [128];	/* Scanning interval count */

pwr_tUInt16			io_IniAIRackAddr[128];	/* Rack address for RTP */
pwr_tUInt16			io_IniAICardType[128];	/* Card type for RTP */
pwr_tObjid			io_IniAIconobjid[128];

io_sFilterCard_Ai	io_IniFilterCardAi[64];	/* Filter data */
pwr_tFloat32		io_IniAIScanTimeBus;	 /* Scan time in seconds */

pwr_tInt32			io_actantai;  /*  Number of di signals in system  */
#endif
