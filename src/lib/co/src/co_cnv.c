/* 
 * Proview   $Id: co_cnv.c,v 1.1 2007-06-01 11:26:06 claes Exp $
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

#include <float.h>
#include <string.h>
#include <iconv.h>
#include "co_cnv.h"

char *cnv_iso8859_to_utf8( char *iso, size_t iso_size)
{
  static iconv_t cd = 0;
  static char utf8[2048];
  char *utf8p = utf8;
  size_t utf8_size = sizeof(utf8);
  
  if ( !cd)
    cd = iconv_open( "UTF-8", "ISO8859-1");

  if ( iconv( cd, &iso, &iso_size, &utf8p, &utf8_size) == (size_t)(-1))
    strcpy( utf8, "");

  return utf8;
}

char *cnv_utf8_to_iso8859( char *utf8, size_t utf8_size)
{
  static iconv_t cd = 0;
  static char iso[2048];
  char *isop = iso;
  size_t iso_size = sizeof(iso);
  
  if ( !cd)
    cd = iconv_open( "ISO8859-1", "UTF-8");

  if ( iconv( cd, &utf8, &utf8_size, &isop, &iso_size) == (size_t)(-1))
    strcpy( iso, "");

  return iso;
}
