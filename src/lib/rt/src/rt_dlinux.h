/* rt_dlinux.h -- Encode VMS-data

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   Functions to convert data to loaded in file
   from Linux to VAX format */


#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#if defined(OS_LINUX)
 extern pwr_tBoolean (*dlinux[pwr_eTix_])();
#endif
