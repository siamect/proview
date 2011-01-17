/* 
 * Proview   $Id: co_merge.c,v 1.7 2007-01-04 08:47:26 claes Exp $
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
#include <stdlib.h>
#include <string.h>
#include "pwr.h"
#include "co_cdh.h"
#include "co_dcli.h"

typedef enum {
  merge_eMtype_IoBase,
  merge_eMtype_WbBase,
  merge_eMtype_XttBase
} merge_eMtype;

void usage()
{
  printf("\
  co_merge_methods\n\
\n\
  Options:\n\
      -k   Keep c file.\n\
      -v   Verbose.\n\
\n\
  Arguments : \n\
      1. Method utility (io_base, wb_base, xtt_base) \n\
      2. Methods files, e.g. $pwr_inc/io_base_*.meth\n\
      3. output file\n\
");
}

static int check_os( char *str, char *os)
{
  char os_vect[10][20];
  int nr;
  int i;
  char os_element[20];

  nr = dcli_parse( str, "||", "", (char *)os_vect, 
                       sizeof( os_vect) / sizeof( os_vect[0]), sizeof( os_vect[0]), 0);

  for ( i = 0; i < nr; i++) {
    dcli_trim( os_element, os_vect[i]);
    if ( cdh_NoCaseStrcmp( os, os_element) == 0)
      return 1;    
  }
	
  return 0;
}

int main(  int argc, char *argv[])
{
  pwr_tFileName filespec;
  pwr_tFileName outfile;
  pwr_tFileName found_file;
  merge_eMtype mtype;
  FILE *fp;
  FILE *outfp;
  char line[200];
  int sts;
  char mtab[1000][32];
  int mtabcnt = 0;
  int i;
  int idx;
  pwr_tCmd cmd;
  pwr_tFileName incdir;
  pwr_tFileName cfile;
  pwr_tFileName ofile;
  char pwre_cc[80];
  char pwre_cxx[80];
  char pwre_ar[80];
  char *s;
  int arg_keep = 0;
  int arg_verbose = 0;
  int in_if = 0;
#if defined OS_LINUX
  char dos[] = "OS_LINUX";
#elif defined OS_MACOS
  char dos[] = "OS_MACOS";
#elif defined OS_FREEBSD
  char dos[] = "OS_FREEBSD";
#endif
  if ( argc < 4) {
    usage();
    exit(1);
  }

  if ( (s = getenv( "pwre_cc")))
    strncpy( pwre_cc, s, sizeof(pwre_cc));
  else
    strcpy( pwre_cc, "gcc");
  if ( (s = getenv( "pwre_cxx")))
    strncpy( pwre_cxx, s, sizeof(pwre_cxx));
  else
    strcpy( pwre_cxx, "g++");
  if ( (s = getenv( "pwre_ar")))
    strncpy( pwre_ar, s, sizeof(pwre_ar));
  else
    strcpy( pwre_ar, "ar");


  idx = 0;
  for ( i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-k") == 0)
      arg_keep = 1;
    else if ( strcmp( argv[i], "-v") == 0)
      arg_verbose = 1;
    else {
      switch ( idx) {
	case 0:
	  if ( strcmp( argv[i], "io_base") == 0)
   	    mtype = merge_eMtype_IoBase;
  	  else if ( strcmp( argv[i], "wb_base") == 0)
    	    mtype = merge_eMtype_WbBase;
  	  else if ( strcmp( argv[i], "xtt_base") == 0)
    	    mtype = merge_eMtype_XttBase;
  	  else {
    	   usage();
    	   exit(1);
  	  }
	  idx++;
	  break;
	case 1:
	  strcpy( filespec, argv[i]);
	  idx++;
	  break;
	case 2:
	  strcpy( outfile, argv[i]);
	  idx++;
	  break;
      }
    }
  }
  sts = dcli_search_file( filespec, found_file, DCLI_DIR_SEARCH_INIT);
  while ( ODD(sts)) {
    fp = fopen( found_file, "r");
    if ( !fp) {
      printf( "Unable to open file %s\n", found_file);
      exit(1);
    }

    while( dcli_read_line( line, sizeof(line), fp)) {
      dcli_trim( line, line);
      if ( strncmp( line, "#if" , 3) == 0) {
        in_if = ! check_os( &line[4], dos);
        continue;
      }
      else if ( in_if) {
        if ( strncmp( line, "#endif", 6) == 0) {
          if ( in_if)
	    in_if = 0;
	  continue;
        }
        continue;
      }
      else if ( line[0] == 0 || line[0] == '#')
	continue;
      strncpy( mtab[mtabcnt++], line, sizeof(mtab[0]));
      if ( mtabcnt > (int) sizeof(mtab)/sizeof(mtab[0]) - 1)
	break;
    }    

    fclose( fp);
    sts = dcli_search_file( filespec, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  dcli_search_file( filespec, found_file, DCLI_DIR_SEARCH_END);

  switch ( mtype) {
  case merge_eMtype_IoBase: {
    strcpy( cfile, "/tmp/rt_io_base_methods.c");
    strcpy( ofile, "/tmp/rt_io_base_methods.o");

    outfp = fopen( cfile, "w");
    if ( !outfp) {
      printf( "Unable to open file %s\n", outfile);
      exit(1);
    }

    fprintf( outfp, "#include \"rt_io_base.h\"\n");
    for ( i = 0; i < mtabcnt; i++) {
      fprintf( outfp, "pwr_dImport pwr_BindIoMethods(%s);\n", mtab[i]);
    }
  
    fprintf( outfp, "pwr_BindIoClasses(Base) = {\n");
    for ( i = 0; i < mtabcnt; i++) {
      fprintf( outfp, "  pwr_BindIoClass(%s),\n", mtab[i]);    
    }
    fprintf( outfp, "  pwr_NullClass};\n");    
    fclose( outfp);
    break;
  }
  case merge_eMtype_WbBase: {
    strcpy( cfile, "/tmp/wb_i_base_methods.cpp");
    strcpy( ofile, "/tmp/wb_i_base_methods.o");

    outfp = fopen( cfile, "w");
    if ( !outfp) {
      printf( "Unable to open file %s\n", outfile);
      exit(1);
    }

    fprintf( outfp, "#include \"wb_pwrs.h\"\n");
    for ( i = 0; i < mtabcnt; i++) {
      fprintf( outfp, "pwr_dImport pwr_BindMethods(%s);\n", mtab[i]);
    }
  
    fprintf( outfp, "pwr_BindClasses(Base) = {\n");
    for ( i = 0; i < mtabcnt; i++) {
      fprintf( outfp, "  pwr_BindClass(%s),\n", mtab[i]);    
    }
    fprintf( outfp, "  pwr_NullClass};\n");    
    fclose( outfp);
    break;
  }
  case merge_eMtype_XttBase: {
    strcpy( cfile, "/tmp/xtt_i_methods.c");
    strcpy( ofile, "/tmp/xtt_i_methods.o");

    outfp = fopen( cfile, "w");
    if ( !outfp) {
      printf( "Unable to open file %s\n", outfile);
      exit(1);
    }

    fprintf( outfp, "#include \"xtt_menu.h\"\n");
    for ( i = 0; i < mtabcnt; i++) {
      fprintf( outfp, "pwr_dImport pwr_BindXttMethods(%s);\n", mtab[i]);
    }
  
    fprintf( outfp, "pwr_BindXttClasses(Base) = {\n");
    for ( i = 0; i < mtabcnt; i++) {
      fprintf( outfp, "  pwr_BindXttClass(%s),\n", mtab[i]);    
    }
    fprintf( outfp, "  pwr_NullClass};\n");    
    fclose( outfp);
    break;
  }
  }

  dcli_translate_filename( incdir, "$pwr_einc");
  switch ( mtype) {
  case merge_eMtype_WbBase:
    sprintf( cmd, "%s -c -I%s -D%s -o %s %s", pwre_cxx, incdir, dos, ofile, cfile);
    break;
  default:
    sprintf( cmd, "%s -c -I%s -D%s -o %s %s", pwre_cc, incdir, dos, ofile, cfile);
  }
  if ( arg_verbose)
    printf( "co_merge: %s\n", cmd);
  system( cmd);
  sprintf( cmd, "%s r %s %s", pwre_ar, outfile, ofile);
  if ( arg_verbose)
    printf( "co_merge: %s\n", cmd);
  system(cmd);
  sprintf( cmd, "rm %s", ofile);
  system(cmd);
  if ( !arg_keep) {
    sprintf( cmd, "rm %s", cfile);
    system(cmd);
  }
  return 1;
}
