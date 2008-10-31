/* 
 * Proview   $Id: cnv_wbltops.cpp,v 1.8 2008-10-31 12:51:30 claes Exp $
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



#include <iostream>
#include <fstream>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cnv_wbltops.h"
#include "cnv_ctx.h"
extern "C" {
#include "pwr.h"
#include "co_cdh.h"
}
#include "co_lng.h"

CnvWblToPs::~CnvWblToPs()
{
  tops.close();
}

int CnvWblToPs::init( char *first)
{
  pwr_tFileName fname;
  char text[200];

  strcpy( fname, ctx->dir);
  strcat( fname, ctx->rw->volume_name);
  strcat( fname, ".ps");
  cdh_ToLower( fname, fname);

  tops.set_filename( ps_eFile_Info, fname);
  tops.set_filename( ps_eFile_Body, ps_cTmpFile);
  tops.open();

  // Print first page
  tops.set_cf( ps_eFile_Info);
  tops.set_ci( ps_eId_TitlePage);
  tops.y -= 100;
  tops.print_image("pwr_logga.gif");
  tops.y -= 100;
  tops.print_h2( Lng::translate("Object Reference Manual"));
  tops.print_h3( Lng::translate("Volume"));
  tops.print_h2( ctx->rw->volume_name);
  tops.y -= 150;
  tops.print_horizontal_line();
  tops.print_text("Version V4.1.0", tops.style[tops.ci].text);
  tops.print_horizontal_line();
  tops.print_pagebreak( 0);

  tops.set_cf( ps_eFile_Info);
  tops.set_ci( ps_eId_InfoPage);
  tops.y = ps_cPageHeight - ps_cTopMargin - 450;
  tops.print_text("Copyright 2005 SSAB Oxelösund AB", tops.style[tops.ci].text);
  tops.print_text("", tops.style[tops.ci].text);
  tops.print_text("Permission is granted to copy, distribute and/or modify this document", tops.style[tops.ci].text);
  tops.print_text("under the terms of the GNU Free Documentation License, Version 1.2", tops.style[tops.ci].text);
  tops.print_text("or any later version published by the Free Software Foundation;", tops.style[tops.ci].text);
  tops.print_text("with no Invariant Sections, no Front-Cover Texts, and no Back-Cover", tops.style[tops.ci].text);
  tops.print_text("Texts.", tops.style[tops.ci].text);

  tops.y = ps_cPageHeight - ps_cTopMargin;
  tops.set_cf( ps_eFile_Body);
  tops.set_ci( ps_eId_Class);
  sprintf( text, "Volume %s", ctx->rw->volume_name);
  tops.set_pageheader( text);
  return 1;
}

int CnvWblToPs::close()
{
  return 1;
}

int CnvWblToPs::class_exec()
{
  int i;
  char full_class_name[80];
  char link_ref[80];
  char *s;
  char txt[256];
  int lng_sts = 1;

  if ( Lng::current() != lng_eLanguage_en_us)
    lng_sts = ctx->rw->read_lng( ctx->rw->class_name, 0);

  strcpy( full_class_name, ctx->rw->volume_name);
  strcat( full_class_name, ":");
  strcat( full_class_name, ctx->rw->class_name);

  tops.set_pageheader( full_class_name);
  
  sprintf( txt, "%s %s", Lng::translate("Class"), ctx->rw->class_name);
  tops.print_h1( txt, 0, CnvCtx::low( ctx->rw->class_name));

  if ( !lng_sts) {
    sprintf( txt, "(%s)", Lng::translate( "English text not available"));
    tops.print_text( txt, tops.style[tops.ci].link);
  }

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_author, "") != 0) {
    sprintf( txt, "%s %s", Lng::translate("Author"), ctx->rw->doc_author);
    tops.print_text( txt, tops.style[tops.ci].text);
  }

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_version, "") != 0) {
    sprintf( txt, "%s %s", Lng::translate("Version"), ctx->rw->doc_version);
    tops.print_text( txt, tops.style[tops.ci].text);
  }

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_code, "") != 0) {
    sprintf( txt, "%s %s", Lng::translate("Code"), ctx->rw->doc_code);
    tops.print_text( txt, tops.style[tops.ci].text);
  }

  tops.print_h2( Lng::translate("Description"));
  if ( ctx->rw->doc_fresh) {
    for ( i = 0; i < ctx->rw->doc_cnt; i++) {
      CnvCtx::remove_spaces( ctx->rw->doc_text[i], txt);
      if ( strncmp( CnvCtx::low(txt), "@image", 6) == 0)  {
	char imagefile[80];

	CnvCtx::remove_spaces( txt + 6, imagefile);
	tops.print_image( imagefile);
      }
      else if ( strncmp( CnvCtx::low(txt), "@b", 2) == 0)  {
	tops.print_text( txt + 2, tops.style[tops.ci].boldtext);
      }
      else if ( strncmp( CnvCtx::low(txt), "@h1", 3) == 0)  {
	tops.print_h2( txt + 3);
      }
      else if ( strncmp( CnvCtx::low(txt), "@h2", 3) == 0)  {
	tops.print_h3( txt + 3);
      }
      else
  	tops.print_text( ctx->rw->doc_text[i], tops.style[tops.ci].text);
    }
  }
  for ( i = 0; i < ctx->rw->doc_clink_cnt; i++) {
    if ( !conf_pass) {
      int page;
      char str[200];
      char text[200];
      
      if ( strncmp( ctx->rw->doc_clink_ref[i], "pwrb_", 5) == 0)
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
      else if ( strncmp( ctx->rw->doc_clink_ref[i], "pwrs_", 5) == 0)
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
      else if ( strncmp( ctx->rw->doc_clink_ref[i], "nmps_", 5) == 0)
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
      else if ( strncmp( ctx->rw->doc_clink_ref[i], "ssab_", 5) == 0)
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
      else
	strcpy( link_ref, ctx->rw->doc_clink_ref[i]);
      if ( (s = strrchr( link_ref, '.')))
	*s = 0;

      int sts = tops.content.find_link( link_ref, text, &page);
      if ( ODD(sts)) {
	sprintf( str, "  (%s %s ", Lng::translate("See"), text);
	sprintf( &str[strlen(str)], "%s %d)", Lng::translate("page"), page);
	tops.print_text( ctx->rw->doc_clink_text[i], 
			 tops.style[tops.ci].text, ps_mPrintMode_Start);
	tops.print_text( str, tops.style[tops.ci].link, ps_mPrintMode_End);
      }
      else {
	tops.print_text( ctx->rw->doc_clink_text[i], 
			 tops.style[tops.ci].text, ps_mPrintMode_Start);
	sprintf( str, "  (%s %s)", Lng::translate("See"), ctx->rw->doc_clink_ref[i]);
	tops.print_text( str, tops.style[tops.ci].link, ps_mPrintMode_End);
      }
    }
    else
      tops.print_text( ctx->rw->doc_clink_text[i], 
			 tops.style[tops.ci].text);
  }
  return 1;
}

int CnvWblToPs::body_exec()
{
  char text[80];

  tops.y -= 10;
  tops.print_horizontal_line();
  sprintf( text, "%s %s", Lng::translate("Body"), ctx->rw->body_name);
  tops.print_h2( text);
  sprintf( text, "Struct pwr_sClass_%s", ctx->rw->body_structname);
  tops.print_text( text, tops.style[tops.ci].boldtext);
  if ( strcmp( ctx->rw->body_flags, "") != 0) {
    sprintf( text, "Flags %s", ctx->rw->body_flags);
    tops.print_text( text, tops.style[tops.ci].boldtext);
  }
  return 1;
}

int CnvWblToPs::class_close()
{

  return 1;
}

int CnvWblToPs::attribute_exec()
{
  int i;
  char *s;
  char txt[200];
  char text[200];
  int page;
  int lng_sts = 1;

  if ( strcmp( ctx->rw->attr_typeref, "CastId") == 0 ||
       strcmp( ctx->rw->attr_typeref, "DisableAttr") == 0)
    return 1;
    
  if ( Lng::current() != lng_eLanguage_en_us)
    lng_sts = ctx->rw->read_lng( ctx->rw->class_name, ctx->rw->attr_name);

  tops.print_h3( ctx->rw->attr_name);
  if ( ctx->rw->attr_array && ctx->rw->attr_pointer)
    sprintf( txt, "%s Array[%s] of pointers to %s", Lng::translate("Type"),
	     ctx->rw->attr_elements, ctx->rw->attr_typeref);
  else if ( ctx->rw->attr_array)
    sprintf( txt, "%s Array[%s] of %s", Lng::translate("Type"),
	     ctx->rw->attr_elements, ctx->rw->attr_typeref);
  else if ( ctx->rw->attr_pointer)
    sprintf( txt, "%s Pointer to %s", Lng::translate("Type"),
	     ctx->rw->attr_typeref);
  else
    sprintf( txt, "%s %s", Lng::translate("Type"), ctx->rw->attr_typeref);

  int sts = tops.content.find_link( CnvCtx::low(ctx->rw->attr_typeref), text, &page);
  if ( ODD(sts)) {
    tops.print_text( txt, tops.style[tops.ci].boldtext, ps_mPrintMode_Start);
    sprintf( txt, " (%s ", Lng::translate("See"));
    sprintf( &txt[strlen(txt)], "%s %d)", Lng::translate("page"), page);
    tops.print_text( txt, tops.style[tops.ci].link, ps_mPrintMode_End);
  }
  else 
    tops.print_text( txt, tops.style[tops.ci].boldtext);

  sprintf( txt, "%s %s", Lng::translate("Class"), ctx->rw->attr_type);
  tops.print_text( txt, tops.style[tops.ci].boldtext);

  if ( strcmp( ctx->rw->attr_flags, "") != 0) {
    sprintf( txt, "Flags %s", ctx->rw->attr_flags);
    tops.print_text( txt, tops.style[tops.ci].boldtext);
  }
  if ( strcmp( ctx->rw->attr_pgmname, "") != 0) {
    sprintf( txt, "PmgName %s", ctx->rw->attr_pgmname);
    tops.print_text( txt, tops.style[tops.ci].boldtext);
  }
  tops.print_text( "", tops.style[tops.ci].text);

  if ( ctx->rw->doc_fresh) {
    for ( i = 0; i < ctx->rw->doc_cnt; i++) {
      if ( (s = strstr( CnvCtx::low(ctx->rw->doc_text[i]), "@image")) != 0)  {
	char imagefile[80];

	CnvCtx::remove_spaces( s + 6, imagefile);
	tops.print_image( imagefile);
      }
      else
	tops.print_text( ctx->rw->doc_text[i], tops.style[tops.ci].text);
    }
  }
  return 1;
}


int CnvWblToPs::typedef_exec()
{
  int i;
  char full_class_name[80];
  char link_ref[80];
  char *s;
  char txt[256];
  int lng_sts = 1;

  if ( Lng::current() != lng_eLanguage_en_us)
    lng_sts = ctx->rw->read_lng( ctx->rw->typedef_name, 0);

  strcpy( full_class_name, ctx->rw->volume_name);
  strcat( full_class_name, ":");
  strcat( full_class_name, ctx->rw->typedef_name);

  tops.set_pageheader( full_class_name);
  
  sprintf( txt, "%s %s", Lng::translate("Type"), ctx->rw->typedef_name);
  tops.print_h1( txt, 0, CnvCtx::low( ctx->rw->typedef_name));

  if ( !lng_sts) {
    sprintf( txt, "(%s)", Lng::translate( "English text not available"));
    tops.print_text( txt, tops.style[tops.ci].link);
  }

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->typedef_typeref, "") != 0) {
    sprintf( txt, "TypeRef %s", ctx->rw->typedef_typeref);
    tops.print_text( txt, tops.style[tops.ci].boldtext);
  }

  if ( ctx->rw->typedef_elements > 1) {
    sprintf( txt, "Elements %d", ctx->rw->typedef_elements);
    tops.print_text( txt, tops.style[tops.ci].boldtext);
  }
  if ( strcmp( ctx->rw->typedef_pgmname, "") != 0) {
    sprintf( txt, "Pgmname %s", ctx->rw->typedef_pgmname);
    tops.print_text( txt, tops.style[tops.ci].boldtext);
  }
  tops.print_text( "", tops.style[tops.ci].text);

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_author, "") != 0) {
    sprintf( txt, "%s %s", Lng::translate("Author"), ctx->rw->doc_author); 
    tops.print_text( txt, tops.style[tops.ci].text);
  }

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_version, "") != 0) {
    sprintf( txt, "%s %s", Lng::translate("Version"), ctx->rw->doc_version);
    tops.print_text( txt, tops.style[tops.ci].text);
  }

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_code, "") != 0) {
    sprintf( txt, "%s %s", Lng::translate("Code"), ctx->rw->doc_code);
    tops.print_text( txt, tops.style[tops.ci].text);
  }

  tops.print_h2( Lng::translate("Description"));
  if ( ctx->rw->doc_fresh) {
    for ( i = 0; i < ctx->rw->doc_cnt; i++) {
      CnvCtx::remove_spaces( ctx->rw->doc_text[i], txt);
      if ( strncmp( CnvCtx::low(txt), "@image", 6) == 0)  {
	char imagefile[80];

	CnvCtx::remove_spaces( txt + 6, imagefile);
	tops.print_image( imagefile);
      }
      else if ( strncmp( CnvCtx::low(txt), "@b", 2) == 0)  {
	tops.print_text( txt + 2, tops.style[tops.ci].boldtext);
      }
      else if ( strncmp( CnvCtx::low(txt), "@h1", 3) == 0)  {
	tops.print_h2( txt + 3);
      }
      else if ( strncmp( CnvCtx::low(txt), "@h2", 3) == 0)  {
	tops.print_h3( txt + 3);
      }
      else
  	tops.print_text( ctx->rw->doc_text[i], tops.style[tops.ci].text);
    }
  }
  for ( i = 0; i < ctx->rw->doc_clink_cnt; i++) {
    if ( !conf_pass) {
      int page;
      char str[200];
      char text[200];
      
      if ( strncmp( ctx->rw->doc_clink_ref[i], "pwrb_", 5) == 0)
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
      else if ( strncmp( ctx->rw->doc_clink_ref[i], "pwrs_", 5) == 0)
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
      else if ( strncmp( ctx->rw->doc_clink_ref[i], "nmps_", 5) == 0)
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
      else if ( strncmp( ctx->rw->doc_clink_ref[i], "ssab_", 5) == 0)
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
      else
	strcpy( link_ref, ctx->rw->doc_clink_ref[i]);
      if ( (s = strrchr( link_ref, '.')))
	*s = 0;

      int sts = tops.content.find_link( link_ref, text, &page);
      if ( ODD(sts)) {
	sprintf( str, " (%s %s ", Lng::translate("See"), text);
	sprintf( &str[strlen(str)], "%s %d)", Lng::translate("page"), page);
	tops.print_text( ctx->rw->doc_clink_text[i], 
			 tops.style[tops.ci].text, ps_mPrintMode_Start);
	tops.print_text( str, tops.style[tops.ci].link, ps_mPrintMode_End);
      }
      else {
	tops.print_text( ctx->rw->doc_clink_text[i], 
			 tops.style[tops.ci].text, ps_mPrintMode_Start);
	sprintf( str, "  (%s %s)", Lng::translate("See"), ctx->rw->doc_clink_ref[i]);
	tops.print_text( str, tops.style[tops.ci].link, ps_mPrintMode_End);
      }
    }
    else
      tops.print_text( ctx->rw->doc_clink_text[i], 
			 tops.style[tops.ci].text);
  }
  return 1;
}

int CnvWblToPs::bit_exec()
{
  int i;
  char *s;
  char txt[200];
  int lng_sts = 1;

  if ( Lng::current() != lng_eLanguage_en_us)
    lng_sts = ctx->rw->read_lng( ctx->rw->typedef_name, ctx->rw->bit_name);

  tops.print_h3( ctx->rw->bit_name);
  sprintf( txt, "%s %s", Lng::translate("Type"), ctx->rw->bit_type);
  tops.print_text( txt, tops.style[tops.ci].boldtext);

  sprintf( txt, "Value %d", ctx->rw->bit_value);
  tops.print_text( txt, tops.style[tops.ci].boldtext);

  sprintf( txt, "Text %s", ctx->rw->bit_text);
  tops.print_text( txt, tops.style[tops.ci].boldtext);

  if ( strcmp( ctx->rw->bit_pgmname, "") != 0) {
    sprintf( txt, "PmgName %s", ctx->rw->bit_pgmname);
    tops.print_text( txt, tops.style[tops.ci].boldtext);
  }
  tops.print_text( "", tops.style[tops.ci].text);

  if ( ctx->rw->doc_fresh) {
    for ( i = 0; i < ctx->rw->doc_cnt; i++) {
      if ( (s = strstr( CnvCtx::low(ctx->rw->doc_text[i]), "@image")) != 0)  {
	char imagefile[80];

	CnvCtx::remove_spaces( s + 6, imagefile);
	tops.print_image( imagefile);
      }
      else
	tops.print_text( ctx->rw->doc_text[i], tops.style[tops.ci].text);
    }
  }
  return 1;
}








