#ifndef rt_io_filter_po_h
#define rt_io_filter_po_h

/* rt_io_filter_po.h -- Runtime environment - I/O filter Po

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
