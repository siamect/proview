/* 
 * Proview   $Id: ge_util.cpp,v 1.2 2005-09-01 14:57:53 claes Exp $
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

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"
extern "C" {
#include "co_dcli.h"
#include "rt_load.h"
}


int ge_get_systemname(
  char		*systemname
)
{
  FILE 	*file;
  int	nr;
  char	fname[120];

  dcli_translate_filename( fname, load_cNameSysObject);
  file = fopen( fname, "r");
  if ( file == 0) {
    printf("** Warning, systemname not found\n");
    return 0;
  }

  nr = fscanf( file, "%s", systemname);
  fclose(file);
  if ( nr != 1) return 0;
  return 1;
}

