/* wb_c_rootvolume.c -- work bench methods of the RootVolume class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
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

  sts = wsx_CheckVolume( Session, Object, ErrorCount, WarningCount);
  if ( EVEN(sts)) return sts;

  return PWRS__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods($RootVolume) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
