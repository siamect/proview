#ifndef wb_foe_api_h
#define wb_foe_api_h

/* wb_foe_api.h -- functional object editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module contains the api for the Functional Object Editor (PLC).  */

#include "pwr_class.h"
#include "wb_ldh.h"

typedef void *foe_tContext;

foe_tContext
foe_Open (
  void		    *ParentEditorContext,
  void		    *ParentWindowContext,
  ldh_tSesContext   SessCtx,
  pwr_tObjDId	    PlcPgmObject
);

void
foe_Close (
  foe_tContext FoeContext
);

#endif
