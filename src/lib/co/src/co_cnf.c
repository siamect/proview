/* co_dcli.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   Command line interpreter. */


#include <string.h>

#include "pwr.h"
#include "co_dcli.h"
#include "co_cnf.h"

#define cnf_cFile "/etc/proview.cnf"

char default_values[][2][200] = {
    { "mysqlSocket",  "/tmp/mysql.sock"},
    { "defaultProjectRoot", "/usr/local/pwrp"},
    { "appletCodebase", "http://java.sun.com/products/plugin/1.3/jinstall-13-win32.cab#Version=1,3,0,0"},
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
    
      if ( strcmp( name, item_str[0]) == 0) {
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




