#ifndef rt_io_filter_di_h
#define rt_io_filter_di_h

/* rt_io_filter_di.h -- Runtime environment - I/O filter Di

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#define RS_IO_FILTER_DI_VERSION "V2.1-1"

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef pwr_baseclasses_h
#include "pwr_baseclasses.h"
#endif

typedef struct ios_FilterCard_Di io_sFilterCard_Di;

/* The structure of card data  */

struct ios_FilterCard_Di
    {
    pwr_tBoolean  Filter;
    pwr_sClass_Di *SignalObj[32];
    void	  *FilterData[32];
    };

/* Filtration of Di.  */

pwr_tStatus io_DiFilter
    (
    pwr_sClass_Di   *SignalObj[],
    pwr_tUInt16	    *Data,
    void	    *FilterData[]
    );

/* Initialize filter for max 16 Di ( one Di-card ).  */

pwr_tStatus io_InitDiFilter
    (
    pwr_sClass_Di   *SignalObj[],
    pwr_tBoolean    *Filter,
    void	    *FilterData[],
    pwr_tFloat32    ScanTime
    );

void io_CloseDiFilter
    (
    void	    *FilterData[]
    );

#endif
