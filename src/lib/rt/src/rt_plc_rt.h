/* rt_plc_rt.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifdef OS_ELN
#  include stdlib
#  include math
#else
#  include <stdlib.h>
#  include <math.h>
#endif

#ifndef rt_plc_h
# include "rt_plc.h"
#endif

#ifndef rt_plc_proc_h
# include "rt_plc_proc.h"
#endif

/* Direct link table */
struct plc_rtdbref
{
  void		**Pointer;
  pwr_tObjid	Objid;
  pwr_tClassId	ObjType;
  pwr_tUInt32	Size;
  pwr_tUInt32	UseCode;
};

#define UC_NORMAL	0
#define UC_READ		1
#define UC_WRITE	2
#define UC_READ2	3
#define UC_WRITE2	4
