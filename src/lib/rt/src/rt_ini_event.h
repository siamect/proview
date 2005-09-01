/* 
 * Proview   $Id: rt_ini_event.h,v 1.3 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_ini_event_h
#define rt_ini_event_h

#include "pwr.h"

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( swapInit		, 1),
    pwr_Bits( rebuildInit	, 1),
    pwr_Bits( newPlcInit	, 1),
    pwr_Bits( newPlcInitDone	, 1),
    pwr_Bits( newPlcStart	, 1),
    pwr_Bits( newPlcStartDone	, 1),
    pwr_Bits( oldPlcStop	, 1),
    pwr_Bits( oldPlcStopDone	, 1),

    pwr_Bits( swapDone		, 1),
    pwr_Bits( terminate		, 1),
    pwr_Bits( fill_1	    , 6),,,,,,

    pwr_Bits( fill_2	    , 8),,,,,,,,
    pwr_Bits( fill_3	    , 8),,,,,,,
  ) b;

#define ini_mEvent__			0
#define ini_mEvent_swapInit		pwr_Bit(0)
#define ini_mEvent_rebuildInit		pwr_Bit(1)
#define ini_mEvent_newPlcInit		pwr_Bit(2)
#define ini_mEvent_newPlcInitDone	pwr_Bit(3)
#define ini_mEvent_newPlcStart		pwr_Bit(4)
#define ini_mEvent_newPlcStartDone	pwr_Bit(5)
#define ini_mEvent_oldPlcStop		pwr_Bit(6)
#define ini_mEvent_oldPlcStopDone	pwr_Bit(7)

#define ini_mEvent_swapDone		pwr_Bit(8)
#define ini_mEvent_terminate		pwr_Bit(9)
#define ini_mEvent_			(~ini_mEvent__)
} ini_mEvent;
 
#endif
