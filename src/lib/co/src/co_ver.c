/** 
 * Proview   $Id: co_ver.c,v 1.2 2005-09-01 14:57:52 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

/* co_ver.c -- version information
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
  printf("   Copyright © %s by SSAB Oxelösund AB\n", pwrv_cBuildTimeStr);
  printf("\n   %s\n", program);
  printf("\n   Configuration: SMD-%s BMD-%s WB-%s LFF-%s\n\n",
    pwrv_cSmdVersionStr, pwrv_cBmdVersionStr, pwrv_cWbdbVersionStr,
    pwrv_cLffVersionStr);
}
