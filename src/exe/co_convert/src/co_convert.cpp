/* 
 * Proview   $Id: co_convert.cpp,v 1.16 2008-10-31 12:51:30 claes Exp $
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
#include <vector>

extern "C" {
#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
}
#include "co_lng.h"
#include "cnv_ctx.h"
#include "cnv_readwbl.h"
#include "cnv_readsrc.h"
#include "cnv_wbltohtml.h"
#include "cnv_wbltoh.h"
#include "cnv_wbltoxtthelp.h"
#include "cnv_wbltops.h"
#include "cnv_readxtthelp.h"
#include "cnv_xtthelptohtml.h"
#include "cnv_xtthelptoxml.h"
#include "cnv_xtthelptops.h"
#include "cnv_xtthelptopdf.h"
#include "cnv_pwgtoxtthelp.h"
#include "cnv_classdep.h"
#include "cnv_changelog.h"

typedef char cnv_tName[200];

static void usage()
{
  printf( "Usage: co_convert -wxsoc -d target_directory source_files\n");
  printf( "      -w:  Create html from wb_load-files\n");
  printf( "      -x:  Create xtthelp from wb_load-filesfiles\n");
  printf( "      -c:  Create html files from c- and -h-files\n");
  printf( "      -s:  Create h files from wb_load-files\n");
  printf( "      -so: Create one common h file from wb_load-files\n");
  printf( "      -p:  Create hpp files from wb_load-files\n");
  printf( "      -po: Create one common hpp file from wb_load-files\n");
  printf( "      -t:  Create html from xtthelp-file\n");
  printf( "      -m:  Create xml from xtthelp-file\n");
  printf( "      -n:  Create postscript from xtthelp-file\n");
  printf( "      -f:  Create pdf from xtthelp-file\n");
  printf( "      -q:  Create postscript from wb_load-files\n");
  printf( "      -e:  Merge changlog files to docbook file\n");
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
  // Types before classes
  if ( (strstr( (char *)file1, "_td_") || strstr( (char *)file1, "_t_")) && 
       !(strstr( (char *)file2, "_td_") || strstr( (char *)file2, "_t_")))
    return -1;
  else if ( !(strstr( (char *)file1, "_td_") || strstr( (char *)file1, "_t_")) && 
	    (strstr( (char *)file2, "_td_") || strstr( (char *)file2, "_t_")))
    return 1;
  return ( strcmp( (char *)file1, (char *)file2));
}


int main( int argc, char *argv[])
{
  int	exit_sts = 1;
  CnvCtx *ctx;
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
  int   xtthelp_to_xml = 0;
  int   xtthelp_to_ps = 0;
  int   xtthelp_to_pdf = 0;
  int   pwg_to_xtthelp = 0;
  int   changelog = 0;
  char from[80] = "";

  if ( argc < 2 || argc > 9) {
    usage();
    exit(0);
  }

  ctx = new CnvCtx();

  for ( i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-d") == 0) {
      if ( i+1 >= argc) {
        usage();
        exit(0);
      }
      strcpy( ctx->dir, argv[i+1]);
      i++;
#if defined OS_VMS
      if ( ctx->dir[strlen(ctx->dir)-1] != ':' &&
	   (ctx->dir[strlen(ctx->dir)-1] != '>' &&
	    ctx->dir[strlen(ctx->dir)-1] != ']' ))
        strcat( ctx->dir , ":");
#else
      if ( ctx->dir[strlen(ctx->dir)-1] != '/')
        strcat( ctx->dir , "/");
#endif
    }
    if ( strcmp( argv[i], "-g") == 0) {
      if ( i+1 >= argc) {
        usage();
        exit(0);
      }
      strcpy( ctx->setup_filename, argv[i+1]);
      i++;
    }
    if ( strcmp( argv[i], "-l") == 0) {
      if ( i+1 >= argc) {
        usage();
        exit(0);
      }
      Lng::set( argv[i+1]);
      i++;
    }
    else if ( argv[i][0] == '-') {
      int next = 0;
      s = &argv[i][1];
      while( *s) {
        switch( *s) {
	case 'h':
	  help();
	  exit(0);
	case 'w':
	  ctx->generate_html = 1;
	  break;
	case 'x':
	  ctx->generate_xtthelp = 1;
	  break;
	case 'c':
	  ctx->generate_src = 1;
	  break;
	case 's':
	  ctx->generate_struct = 1;
	  break;
	case 'q':
	  ctx->generate_ps = 1;
	  ctx->common_structfile_only = 1;
	  break;
	case 'p':
	  ctx->generate_struct = 1;
	  ctx->hpp = 1;
	  break;
	case 'o':
	  ctx->common_structfile_only = 1;
	  break;
	case 'k':
	  ctx->generate_cdp = 1;
	  break;
	case 'v':
	  ctx->verbose = 1;
	  break;
	case 't':
	  xtthelp_to_html = 1;
	  break;
	case 'm':
	  xtthelp_to_xml = 1;
	  break;
	case 'n':
	  xtthelp_to_ps = 1;
            break;
	case 'f':
	  xtthelp_to_pdf = 1;
	  break;
	case 'a':
	  pwg_to_xtthelp = 1;
	  break;
	case 'e':
	  changelog = 1;
	  break;
	case '-': {
	  if ( strcmp( s, "-from") == 0 && ( i+1 < argc)) {
	    strcpy( from, argv[i+1]);
	    next = 1;
	    i++;
	  }
	  break;
	}
	default:
	  usage();
	  exit(0);
        }
	if ( next)
	  break;
        s++;
      }
    }
    else
      strcpy( files, argv[i]);
  }

  ctx->setup = new CnvSetup();
  if ( strcmp( ctx->setup_filename, "") != 0) {
    ctx->setup->setup( ctx->setup_filename);
  }

  if ( changelog) {
    CnvChangeLog *c = new CnvChangeLog( ctx, from);
    delete c;
    exit(0);
  }
  if ( xtthelp_to_html) {
    CnvXtthelpToHtml *xtthelpto = new CnvXtthelpToHtml( ctx);
    ctx->rx = new CnvReadXtthelp( files, ctx->dir, xtthelpto);
    ctx->rx->read_xtthelp();
    delete ctx->rx;
    delete xtthelpto;
    exit(0);
  }
  if ( xtthelp_to_xml) {
    CnvXtthelpToXml *xtthelpto = new CnvXtthelpToXml( ctx);
    ctx->rx = new CnvReadXtthelp( files, ctx->dir, xtthelpto);
    ctx->rx->read_xtthelp();
    delete ctx->rx;
    delete xtthelpto;
    exit(0);
  }
  if ( xtthelp_to_ps) {
    CnvXtthelpToPs *xtthelpto = new CnvXtthelpToPs( ctx);
    ctx->rx = new CnvReadXtthelp( files, ctx->dir, xtthelpto);
    ctx->rx->read_xtthelp();
    delete ctx->rx;
    delete xtthelpto;
    exit(0);
  }
  if ( xtthelp_to_pdf) {
    CnvXtthelpToPdf *xtthelpto = new CnvXtthelpToPdf( ctx);
    ctx->rx = new CnvReadXtthelp( files, ctx->dir, xtthelpto);
    ctx->rx->read_xtthelp();
    delete ctx->rx;
    delete xtthelpto;
    exit(0);
  }
  if ( ctx->generate_cdp) {
    CnvClassDep *classdep = new CnvClassDep( ctx);
    classdep->read();
    delete classdep;
    exit(0);
  }
  if ( pwg_to_xtthelp) {
    CnvPwgToXtthelp *pwgto = new CnvPwgToXtthelp( ctx);
    delete pwgto;
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

  if ( file_cnt == 0) {
    printf( "No files found\n");
    exit(0);
  }

  qsort( file_p, file_cnt, sizeof(*file_p), convert_sort_files);

  CnvReadSrc *sr = 0;
  if ( ctx->generate_src) {
    sr = new CnvReadSrc( ctx);
  }
  else {
    if ( ctx->generate_html)
      ctx->wblto = new CnvWblToHtml( ctx);
    else if ( ctx->generate_struct)
      ctx->wblto = new CnvWblToH( ctx);
    else if ( ctx->generate_xtthelp)
      ctx->wblto = new CnvWblToXtthelp( ctx);
    else if ( ctx->generate_ps)
      ctx->wblto = new CnvWblToPs( ctx);

    ctx->rw = new CnvReadWbl( ctx, ctx->wblto);
  }

  if ( ctx->wblto && ctx->wblto->confpass()) {
    ctx->wblto->set_confpass( true);
    for ( i = 0; i < file_cnt; i++) {
      if ( ctx->verbose)
	printf( "Configure file %s\n", file_p[i]);
      if ( ctx->generate_src) {
	sr->read_src( file_p[i]);
      }
      else {
	sts = ctx->rw->read_wbl( file_p[i]);
	if ( EVEN(sts)) {
	  exit_sts = sts;
	  break;
	}
      }
    }
    ctx->wblto->set_confpass( false);
    ctx->first_class = 1;
  }
  for ( i = 0; i < file_cnt; i++) {
    if ( ctx->verbose)
      printf( "Processing file %s\n", file_p[i]);
    if ( ctx->generate_src) {
      sr->read_src( file_p[i]);
    }
    else {
      sts = ctx->rw->read_wbl( file_p[i]);
      if ( EVEN(sts)) {
        exit_sts = sts;
        break;
      }
    }
  }

  if ( ctx->generate_html && ctx->wblto->index_open())
    ctx->wblto->close();
  if ( ctx->generate_xtthelp && ctx->wblto->index_open())
    ctx->wblto->close();
  if ( ctx->generate_struct)
    ctx->wblto->close();

  if ( allocated)
    free( file_p);
  if ( sr)
    delete sr;
  if ( ctx->rw)
    delete ctx->rw;
  if ( ctx->wblto)
    delete ctx->wblto;

  if ( EVEN(exit_sts))
#if defined OS_VMS
    exit(exit_sts);
#else
    exit(1);
  exit(0);
#endif
}


