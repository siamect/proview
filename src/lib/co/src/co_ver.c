/** 
 * Proview   $Id: co_ver.c,v 1.4 2008-06-25 07:48:12 claes Exp $
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

  printf("\n   Proview/R Version %s for %s on %s\n", pwrv_cPwrVersionStr,
    pwrv_cOpSys, pwrv_cHardware);
  printf("   Copyright © %s by SSAB Oxelösund AB\n\n", pwrv_cBuildTimeStr);
  printf("   Proview/R is free software; you can redistribute it and/or\n\
   modify it under the terms of the GNU General Public License.\n\n\
   This program is distributed in the hope that it will be useful\n\
   but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n\
   GNU General Public License for more details.\n\n");
  printf("   %s\n\n", program);
}




