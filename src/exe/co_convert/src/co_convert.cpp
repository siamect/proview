#include <stdio.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>

extern "C" {
#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
}
#include "cnv_classread.h"
#include "cnv_help.h"

typedef char cnv_tName[200];

static void usage()
{
  printf( "Usage: co_convert -wxsoc -d target_directory source_files\n");
  printf( "      -w:  Create html from wb_load-files\n");
  printf( "      -x:  Create xtthelp from wb_load-filesfiles\n");
  printf( "      -c:  Create html files from c- and -h-files\n");
  printf( "      -s:  Create h files from wb_load-files\n");
  printf( "      -so: Create one common h file from wb_load-files\n");
  printf( "      -t:  Create html from xtthelp-file\n");
  printf( "      -d:  Output directory\n");
  printf( "      -g:  Setup file\n");
  printf( "      -h:  Help\n\n");

}

static void help()
{
  usage();

  printf( "Tags in wb_loadfile:\n");
  printf( "  Class:\n");
  printf("     !/**\n");
  printf("          @Author	'author'\n");
  printf("          @Version	'version'\n");
  printf("          @Summary	'text'\n");
  printf("          @Code   	'filename'\n");
  printf("          'Description'\n");
  printf("     !*/\n");
  printf( "  Attribute:\n");
  printf("     !/**\n");
  printf("          @Summay\n");
  printf("          'Description'\n");
  printf("     !*/\n");
  printf( "\n");
  printf( "Tags in c or h file:\n");
  printf("     /**\n");
  printf("          @aref 'bookmark' 'text'\n");
  printf("     */\n");
  printf( "\n");

}

static int convert_sort_files( const void *file1, const void *file2)
{
  return ( strcmp( (char *)file1, (char *)file2));
}


int main( int argc, char *argv[])
{
  int	exit_sts = 1;
  ClassRead *cr;
  int	sts;
  char	found_file[200];
  char  files[200];
  char	*s;
  int	i;
  int file_cnt;
  int allocated, old_allocated;
  cnv_tName *file_p;
  cnv_tName *old_file_p;
  int   xtthelp_to_html = 0;


  if ( argc < 2 || argc > 8) {
    usage();
    exit(0);
  }

  cr = new ClassRead();

  for ( i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-d") == 0) {
      if ( i+1 >= argc) {
        usage();
        exit(0);
      }
      strcpy( cr->dir, argv[i+1]);
      i++;
#if defined OS_VMS
      if ( cr->dir[strlen(cr->dir)-1] != ':' &&
	   (cr->dir[strlen(cr->dir)-1] != '>' &&
	    cr->dir[strlen(cr->dir)-1] != ']' ))
        strcat( cr->dir , ":");
#else
      if ( cr->dir[strlen(cr->dir)-1] != '/')
        strcat( cr->dir , "/");
#endif
    }
    if ( strcmp( argv[i], "-g") == 0) {
      if ( i+1 >= argc) {
        usage();
        exit(0);
      }
      strcpy( cr->setup_filename, argv[i+1]);
      i++;
    }
    else if ( argv[i][0] == '-') {
      s = &argv[i][1];
      while( *s) {
        switch( *s) {
          case 'h':
            help();
            exit(0);
          case 'w':
            cr->generate_html = 1;
            break;
          case 'x':
            cr->generate_xtthelp = 1;
            break;
          case 'c':
            cr->generate_src = 1;
            break;
          case 's':
            cr->generate_struct = 1;
            break;
          case 'o':
            cr->common_structfile_only = 1;
            break;
          case 'v':
            cr->verbose = 1;
            break;
          case 't':
            xtthelp_to_html = 1;
            break;
          default:
            usage();
            exit(0);
        }
        s++;
      }
    }
    else
      strcpy( files, argv[i]);
  }

  if ( strcmp( cr->setup_filename, "") != 0)
    cr->setup();

  if ( xtthelp_to_html) {
    XhelpToHtml *xh = new XhelpToHtml( files, cr->dir);
    delete xh;
    exit(0);
  }

  // Get the files and order them
  file_cnt = 0;
  allocated = 0;
  sts = dcli_search_file( files, found_file, DCLI_DIR_SEARCH_INIT);
  while ( ODD(sts)) {
    file_cnt++;
    if ( file_cnt > allocated) {
      if ( allocated == 0) {
        allocated = 100;
        file_p = (cnv_tName *) malloc( allocated * sizeof(*file_p));
      }
      else {
        old_file_p = file_p;
        old_allocated = allocated;
        allocated += 100;
        file_p = (cnv_tName *) malloc( allocated * sizeof(*file_p));
        memcpy( file_p, old_file_p, old_allocated * sizeof(*file_p));
        free( old_file_p);
      }
    }
    strcpy( file_p[file_cnt - 1], found_file);
    sts = dcli_search_file( files, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  dcli_search_file( files, found_file, DCLI_DIR_SEARCH_END);
    
  qsort( file_p, file_cnt, sizeof(*file_p), convert_sort_files);

  for ( i = 0; i < file_cnt; i++) {
    if ( cr->verbose)
      printf( "Processing file %s\n", file_p[i]);
    if ( cr->generate_src)
      cr->src_read( file_p[i]);
    else {
      sts = cr->read( file_p[i]);
      if ( EVEN(sts)) {
        exit_sts = sts;
        break;
      }
    }
  }

  if ( cr->generate_html && cr->html_index_open)
    cr->html_close();
  if ( cr->generate_xtthelp && cr->xtthelp_index_open)
    cr->xtthelp_close();
  if ( cr->generate_struct)
    cr->struct_close();

  if ( allocated)
    free( file_p);

  if ( EVEN(exit_sts))
#if defined OS_VMS
    exit(exit_sts);
#else
    exit(1);
  exit(0);
#endif
}


