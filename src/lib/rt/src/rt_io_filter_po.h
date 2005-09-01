/* 
 * Proview   $Id: rt_io_filter_po.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_io_filter_po_h
#define rt_io_filter_po_h

/* rt_io_filter_po.h -- Runtime environment - I/O filter Po */

#define RS_IO_FILTER_PO_VERSION "V1.1-1"
#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef pwr_baseclasses_h
#include "pwr_baseclasses.h"
#endif

typedef struct ios_FilterCard_Po io_sFilterCard_Po;

/* The structure of card data   */

struct ios_FilterCard_Po
    {
    pwr_tBoolean  Filter;
    pwr_sClass_Po *SignalObj[32];
    void	  *FilterData[32];
    };

/* Filtration of Po.  */

pwr_tStatus io_PoFilter
    (
    pwr_sClass_Po   *SignalObj[],
    pwr_tUInt16	    *Data,
    void	    *FilterData[]
    );

/* Initialize filter for max 16 Po ( one Do-card ).  */

pwr_tStatus io_InitPoFilter
    (
    pwr_sClass_Po   *SignalObj[],
    pwr_tBoolean    *Filter,
    void	    *FilterData[],
    pwr_tFloat32    ScanTime
    );

void io_ClosePoFilter
    (
    void	    *FilterData[]
    );

#endif
