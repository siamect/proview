/* rt_io_vipc616.h -- I/O VIPC616, Greenspring.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

*/
#ifndef rt_io_vipc616_h
#define rt_io_vipc616_h

#ifdef __cplusplus
extern "C" {
#endif 

#ifndef pwr_baseclasses_h
# include "pwr_baseclasses.h"
#endif

pwr_tStatus vipc616_GetAddress(
  pwr_tObjid		ipObjid, /* Objid of IP-module */
  pwr_tUInt32		slot,
  char			**ioAddress,
  char			**idAddress,
  char			**memoryAddress
);


#ifdef __cplusplus
}
#endif 

#endif
