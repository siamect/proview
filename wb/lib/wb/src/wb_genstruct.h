#ifndef wb_genstruct_h
#define wb_genstruct_h

/* wb_genstruct.h -- <short description>

   PROVIEW/R
   Copyright (C) 1991-1996 by Comator Process AB.

   This include file contains definitions and function prototypes
   needed to use ds_ldh_genstruct.  */

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

int genstr_process_top (
  ldh_tSesContext sesctx,
  char *volname
);
int genstr_psts (
  unsigned long int sts,
  FILE *logfile
);

#endif
