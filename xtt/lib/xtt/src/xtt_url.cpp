/* 
 * Proview   $Id: xtt_url.cpp,v 1.6 2008-03-19 07:34:30 claes Exp $
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

/* xtt_url.cpp -- opening of URL attributes. */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "xtt_url.h"
#include "rt_xnav_msg.h"
extern "C" {
#include "rt_gdh.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
}
#include "co_lng.h"

static int find_symbol( char *name, char *value, 
			pwr_sClass_WebBrowserConfig *config);
static int replace_symbol( pwr_tURL in, pwr_tURL out,
			   pwr_sClass_WebBrowserConfig *config);


int xnav_open_URL( pwr_tURL url)
{
  char cmd[200];
  int sts;
  pwr_sClass_WebBrowserConfig *config_p;
  char browser[40] = "netscape";
  pwr_tObjid config_objid;

  sts = gdh_GetClassList( pwr_cClass_WebBrowserConfig, &config_objid);
  if ( ODD(sts))
    sts = gdh_ObjidToPointer( config_objid, (void **)&config_p);

  if ( ODD(sts)) {
    if ( strcmp( config_p->WebBrowser, "") != 0)
      strcpy( browser, config_p->WebBrowser);

    if ( strncmp( url, "$pwr_lang/", 10) == 0) {
      // If file in $pwr_lang, check if file exist, else take en_us

      if ( Lng::current() != lng_eLanguage_en_us) {
	pwr_tURL testurl;
	pwr_tTime t;

	replace_symbol( url, testurl, config_p);
	dcli_translate_filename( testurl, testurl);
	sts = dcli_file_time( testurl, &t);
	if ( EVEN(sts)) {
	  // Try en_us
	  strcpy( testurl, "$pwr_doc/en_us/");
	  strcat( testurl, &url[10]);
	  strcpy( url, testurl);
	}
      }
    }

    replace_symbol( url, url, config_p);
  }
  if ( strcmp( browser, "mozilla") == 0 ||
       strcmp( browser, "rt_mozilla") == 0) {
    // Try remote display first
    sprintf( cmd, "%s -remote \"openurl(%s,new-window)\"", browser, url);
    sts = system(cmd);
    if ( sts) {
      // Not started yet 
      sprintf( cmd, "%s %s &", browser, url);
      system( cmd);
    }
  }
  else {
    sprintf( cmd, "%s %s &", browser, url);
    system( cmd);
  }
  return XNAV__SUCCESS;
}

static int replace_symbol( pwr_tURL in, pwr_tURL out,
			  pwr_sClass_WebBrowserConfig *config)
{
  char *s, *t;
  char *sym_start;
  char sym_value[80];
  char sym_name[80];
  pwr_tURL tmp;
  int skip_sym = 0;
  pwr_tURL url;

  if ( strncmp( in, "$pwr_lang/", 10) == 0) {
    strcpy( url, "$pwr_doc/");
    strcat( url, Lng::get_language_str());
    strcat( url, &in[9]);
  }
  else
    strncpy( url, in, sizeof(pwr_tURL));
     

  sym_start = 0;
  t = tmp;
  for ( s = url; *s; s++) {
    if ( *s == '\\' && *(s+1) == '$')
      skip_sym = 1;
    if ( *s == '$' && !skip_sym) {
      if ( sym_start) {
        strncpy( sym_name, sym_start+1, s - (sym_start + 1));
        sym_name[s - sym_start - 1] = 0;
        if ( find_symbol( sym_name, sym_value, config)) {
          strcpy( t, sym_value);
          t += strlen(sym_value);
        }
        else {
          strncpy( t, sym_start, s - sym_start);
          t += s - sym_start;
        }
      }
      sym_start = s;
    }
    else if ( *s == '$' && skip_sym) {
      skip_sym = 0;
      *t++ = *s;
    }
    else if ( sym_start) {
      if ( !(isdigit(*s) || isalpha(*s) || *s == '_')) {
        // End of symbol
        strncpy( sym_name, sym_start+1, s - (sym_start + 1));
        sym_name[s - sym_start - 1] = 0;
        if ( find_symbol( sym_name, sym_value, config)) {
          strcpy( t, sym_value);
          t += strlen(sym_value);
        }
        else {
          strncpy( t, sym_start, s - sym_start);
          t += s - sym_start;
        }
        sym_start = 0;
        *t++ = *s;
      }
    }
    else
      *t++ = *s;
  }
  if ( sym_start) {
    strcpy( sym_name, sym_start+1);
    if ( find_symbol( sym_name, sym_value, config))
      strcpy( t, sym_value);
    else
      strcpy( t, sym_start);
  }
  else
    *t = 0;

  strcpy( out, tmp);
  return XNAV__SUCCESS;
}

static int find_symbol( char *name, char *value, 
			pwr_sClass_WebBrowserConfig *config)
{
  char sym_array[2][80];
  int nr;
  int i;

  for ( i = 0; i < 10; i++) {
    if ( strcmp( config->URL_Symbols[i], "") != 0) {
      nr = dcli_parse( config->URL_Symbols[i], " 	", "",
	     (char *) sym_array, sizeof( sym_array)/sizeof( sym_array[0]), 
	     sizeof( sym_array[0]), 0);
      if ( nr != 2)
        continue;
      if ( strcmp( sym_array[0], name) == 0) {
        strcpy( value, sym_array[1]);
        return 1;
      }
    }
  }
  return 0;
}















