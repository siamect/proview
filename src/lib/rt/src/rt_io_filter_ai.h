/* 
 * Proview   $Id: rt_io_filter_ai.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_io_filter_ai_h
#define rt_io_filter_ai_h

/* rt_io_filter_ai.h -- Runtime environment - I/O filter Ai */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef pwr_baseclasses_h
#include "pwr_baseclasses.h"
#endif

typedef struct ios_FilterCard_Ai io_sFilterCard_Ai;


/* The structure of card data  */

struct ios_FilterCard_Ai
    {
    void	  *FilterData[8];
    };



/* Filtration of one Ai-object.  */

extern pwr_tStatus io_AiFilter
    (
    pwr_sClass_Ai   *SignalObj,
    pwr_tFloat32    *Value,
    void	    *FilterData
    );

/* Initialize filter for one Ai-object.  */

extern pwr_tStatus io_InitAiFilter
    (
    pwr_sClass_ChanAi   *ChanObj,
    pwr_sClass_Ai   *SignalObj,
    void	    **FilterData,
    pwr_tFloat32    ScanTime
    );

#endif
