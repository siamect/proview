/* wb_c_ai.c -- work bench methods of the Ai class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <stdio.h>
#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_pwrb_msg.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "wb_wsx.h"


/*----------------------------------------------------------------------------*\
  Syntax check.
\*----------------------------------------------------------------------------*/

static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tObjid Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  pwr_tStatus sts;

  sts = wsx_CheckSigChanCon( Session, Object, ErrorCount, WarningCount);
  if (EVEN(sts)) return sts;

  return PWRB__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(Ai) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};

