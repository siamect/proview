#ifndef rt_dvms_h
#define rt_dvms_h

/* rt_dvms.h -- Decode VMS-data

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   Functions to convert data loaded from file
   in VAX format to big endian format.  */


#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_pdr_h
# include "co_pdr.h"
#endif

#ifndef rt_gdb_h
# include "rt_gdb.h"
#endif



typedef pwr_tBoolean (*dvmsFctn)(char *p, gdb_sAttribute *ap, co_eBO bo);

/** 
 * Returns an array of conversion functions for the passed source format
 */
dvmsFctn* dvms_GetFctns(const co_mFormat* fmp);

#endif
