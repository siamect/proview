/** 
 * Proview   $Id: co_cnf.c,v 1.4 2008-02-05 13:16:23 claes Exp $
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

/* co_cnf.c 
   Read global configuration file. */


#include <string.h>

#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
#include "co_cnf.h"

#define cnf_cFile "/etc/proview.cnf"

char default_values[][2][200] = {
    { "mysqlSocket",  "/var/run/mysqld/mysqld.sock"},
    { "defaultProjectRoot", "/usr/local/pwrp"},
    { "appletCodebase", "http://java.sun.com/update/1.4.2/jinstall-1_4-windows-i586.cab#Version=1,4,0,0"},
  };


char *cnf_get_value( char *name, char *value)
{
  FILE *fp;
  char line[400];
  char item_str[2][200];
  static char ret_value[200];
  int nr;  
  int i;


  fp = fopen( cnf_cFile, "r");

  if ( fp) {

    while ( dcli_read_line( line, sizeof( line), fp)) {
      if ( line[0] == '#')
        continue;

      nr = dcli_parse( line, " 	", "", (char *)item_str, 
		sizeof( item_str) / sizeof( item_str[0]), sizeof( item_str[0]), 0);
      if ( nr < 2)
        continue;
    
      if ( cdh_NoCaseStrcmp( name, item_str[0]) == 0) {
        strcpy( ret_value, item_str[1]);
        if ( value)
          strcpy( value, ret_value);
        fclose( fp);
        return ret_value;
      }
    }
    fclose( fp);
  }

  /* Find default value */
  for ( i = 0; i < sizeof(default_values)/sizeof(default_values[0]); i++) {
    if ( strcmp( name, default_values[i][0]) == 0) {
      strcpy( ret_value, default_values[i][1]);
      if ( value)
        strcpy( value, ret_value);
      return ret_value;
    }
  }
  return NULL;
}




