#ifndef rt_io_filter_ai_h
#define rt_io_filter_ai_h

/* rt_io_filter_ai.h -- Runtime environment - I/O filter Ai

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
