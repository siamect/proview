#ifndef wb_foe_dataarithm_h
#define wb_foe_dataarithm_h

/* wb_foe_dataarithm.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

pwr_tStatus dataarithm_convert (
  char *str,
  char *newstr,
  char *object, 
  int  bufsize,
  char *error_line,
  int  *error_line_size,
  int  *error_line_num,
  int  *outsize
);

#endif
