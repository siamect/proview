/* co_ver.c -- version information

   PROVIEW/R
   Copyright (C) 1995-1996 by Comator Process AB.

   This module contains functions for versioning.  */

#include <stdio.h>
#include <time.h>
#include "pwr_version.h"
#include "co_ver.h"

void
ver_WriteVersionInfo (
  char	*program
) {

  printf("\n   PROVIEW/R Version %s for %s on %s\n", pwrv_cPwrVersionStr,
    pwrv_cOpSys, pwrv_cHardware);
  printf("   Copyright © %s by Mandator AB\n", pwrv_cBuildTimeStr);
  printf("\n   %s\n", program);
  printf("\n   Configuration: SMD-%s BMD-%s WB-%s LFF-%s\n\n",
    pwrv_cSmdVersionStr, pwrv_cBmdVersionStr, pwrv_cWbdbVersionStr,
    pwrv_cLffVersionStr);
}
