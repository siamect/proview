#ifndef rt_ini_event_h
#define rt_ini_event_h

/* rt_ini_event.h -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   */

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
    pwr_Bits( fill_1	    , 7),,,,,,,

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
#define ini_mEvent_			(~ini_mEvent__)
} ini_mEvent;
 
#endif
