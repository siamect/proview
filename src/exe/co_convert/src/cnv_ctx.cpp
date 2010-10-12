/* 
 * Proview   $Id: cnv_ctx.cpp,v 1.3 2008-10-31 12:51:30 claes Exp $
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
 */


#include <stdio.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

extern "C" {
#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
}

#include "cnv_ctx.h"

using namespace std;

char *CnvCtx::low( const char *in)
{
  static char str[400];

  cdh_ToLower( str, in);
  return str;
}

int CnvCtx::remove_spaces(
			const char *in,
			char	*out)
{
  char    *s, *s1;

  for ( s = (char *)in; !((*s == 0) || ((*s != ' ') && (*s != 9))); s++);

  // strcpy( out, s);
  for ( s1 = out; *s; s++,s1++)
    *s1 = *s;
  *s1 = 0;
        
  s = out;
  if ( strlen(s) != 0) {
    for ( s += strlen(s) - 1; 
          !((s == out) || ((*s != ' ') && (*s != 9))); s--) ;
    s++;
    *s = 0;
  }

  return 1;
}

int CnvCtx::read_line(
			char	*line,
			int	maxsize,
			FILE	*file)
{ 
  char	*s;

  if (fgets( line, maxsize, file) == NULL)
    return 0;
  line[maxsize-1] = 0;
  s = strchr( line, 10);
  if ( s != 0)
    *s = 0;
  s = strchr( line, 13);
  if ( s != 0)
    *s = 0;

  return 1;
}

