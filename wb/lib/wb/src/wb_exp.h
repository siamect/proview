#ifndef wb_exp_h
#define wb_exp_h

/* wb_exp.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

pwr_tStatus exp_window (
  ldh_tWBContext	ldhwb,
  ldh_tSesContext	ldhses,
  char			*window_str,
  char			*output
);
pwr_tStatus exp_plcpgm (
  ldh_tSesContext	ldhses,
  char			*plcpgm_str,
  char			*output
);

#endif
