/* wb_c_classdef.c -- work bench methods of the ClassDef class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"

static pwr_tStatus AnteCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  // ClassHier should be a top object
  if ( Father.oix != 0)
    return PWRS__POSCLASSHIER;

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($ClassHier) = {
  pwr_BindMethod(AnteCreate),
  pwr_NullMethod
};





