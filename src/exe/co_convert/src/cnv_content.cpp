/* cnv_content.cpp --

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   Table of content ist. */

/*_Include files_________________________________________________________*/

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

#include "cnv_content.h"

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






