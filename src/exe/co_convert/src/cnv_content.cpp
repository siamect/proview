/* 
 * Proview   $Id: cnv_content.cpp,v 1.3 2008-10-31 12:51:30 claes Exp $
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

/* cnv_content.cpp --
   Table of content list. */

/*_Include files_________________________________________________________*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

#include "cnv_content.h"

using namespace std;

int CnvContent::find_link( char *subject, char *text, int *page)
{
  char	key_part[4][40];
  char	subject_part[4][40];
  int 	i, j;
  int   subject_nr;
  int   key_nr;
  int   hit = 0;

  subject_nr = dcli_parse( subject, " 	", "", (char *)subject_part,
			   sizeof( subject_part) / sizeof( subject_part[0]),
			   sizeof( subject_part[0]), 0);

  for ( j = 0; j < (int)tab.size(); j++) {
    key_nr = dcli_parse( tab[j].subject, " 	", "", (char *)key_part,
			 sizeof( key_part) / sizeof( key_part[0]), 
			 sizeof( key_part[0]), 0);
    if ( key_nr == subject_nr ) {
      for ( i = 0; i < key_nr; i++) {
	if ( cdh_NoCaseStrcmp( subject_part[i], key_part[i]) == 0) {
	  if ( i == key_nr - 1) {
	    hit = 1;
	    break;
	  }
	}
	else
	  break;
      }
    }
    if ( hit)
      break;
  }
  if ( !hit)
    return 0;
  strcpy( text, tab[j].text);
  *page = tab[j].page_number;
  return 1;
}

void CnvContent::add( CnvContentElem& elem)
{
  tab.push_back( elem);
}






