/* 
 * Proview   $Id: cnv_wbltoxtthelp.cpp,v 1.11 2008-10-31 12:51:30 claes Exp $
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
#include "cnv_wbltoxtthelp.h"
#include "cnv_ctx.h"
extern "C" {
#include "pwr.h"
#include "co_cdh.h"
}
#include "co_lng.h"

int CnvWblToXtthelp::init( char *first)
{
  pwr_tFileName fname;

  strcpy( fname, ctx->dir);
  strcat( fname, ctx->rw->volume_name);
  strcat( fname, "_xtthelp.dat");
  cdh_ToLower( fname, fname);

  fp_xtthelp_index.open( fname);

  fp_tmp.open( cread_cTmpFile2);

  xtthelp_index_open = 1;
  xtthelp_in_topic= 1;

  fp_xtthelp_index <<
"<TOPIC> " << ctx->rw->volume_name << endl <<
"ClassVolume " << ctx->rw->volume_name << endl << endl;

  return 1;
}

int CnvWblToXtthelp::close()
{

  fp_xtthelp_index <<
"</TOPIC>" << endl << endl;

  fp_tmp.close();

  ctx->rw->copy_tmp_file( cread_cTmpFile2, fp_xtthelp_index);
  
  fp_xtthelp_index.close();
  return 1;
}

int CnvWblToXtthelp::class_exec()
{
  int i;
  char full_class_name[80];
  char link_ref[80];
  char *s;
  char txt[256];
  char prefix[80];
  int lng_sts = 1;

  if ( Lng::current() != lng_eLanguage_en_us)
    lng_sts = ctx->rw->read_lng( ctx->rw->class_name, 0);

  strcpy( full_class_name, ctx->rw->volume_name);
  strcat( full_class_name, ":");
  strcat( full_class_name, ctx->rw->class_name);
  
  // Add into index file
  fp_xtthelp_index <<
"<B>" << ctx->rw->class_name << "<T><T> " << ctx->rw->doc_summary << " <LINK> " << ctx->rw->class_name << endl;

  // Add to class file
  if ( xtthelp_in_topic)
    fp_tmp <<
"</TOPIC>" << endl;

  xtthelp_in_topic = 1;
  fp_tmp <<
"<TOPIC> " << ctx->rw->class_name << endl <<
Lng::translate("Class") << " " << full_class_name << endl <<
"<H1>" << ctx->rw->class_name << endl << endl;

  if ( !lng_sts)
    fp_tmp <<
"(" << Lng::translate( "English text not available") << ")" << endl << endl;

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_author, "") != 0)
  {
    fp_tmp <<
"<B>" << Lng::translate("Author") << "<T>" << ctx->rw->doc_author << endl;
  }

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_version, "") != 0)
  {
    fp_tmp <<
"<B>" << Lng::translate("Version") << "<T>" << ctx->rw->doc_version << endl;
  }

  fp_tmp <<
    "<H1>"<< Lng::translate("Description") << endl;

  if ( ctx->rw->doc_fresh) {

    for ( i = 0; i < ctx->rw->doc_cnt; i++) {
      CnvCtx::remove_spaces( ctx->rw->doc_text[i], txt);
      if ( strncmp( CnvCtx::low(txt), "@image", 6) == 0)  {
	char imagefile[80];

	CnvCtx::remove_spaces( txt + 6, imagefile);
	fp_tmp << "<IMAGE> " << imagefile << endl;
      }
      else if ( strncmp( CnvCtx::low(txt), "@b", 2) == 0)  {
	fp_tmp << "<B> " << txt + 2 << endl;
      }
      else if ( strncmp( CnvCtx::low(txt), "@h1", 3) == 0)  {
	fp_tmp << "<H1> " << txt + 3 << endl;
      }
      else if ( strncmp( CnvCtx::low(txt), "@h2", 3) == 0)  {
	fp_tmp << "<H2> " << txt + 3 << endl;
      }
      else
	fp_tmp << ctx->rw->doc_text[i] << endl;
    }
  }
  for ( i = 0; i < ctx->rw->doc_xlink_cnt; i++) {
    if ( strncmp( ctx->rw->doc_xlink_ref[i], "../../en_us/man_exlib_", 
		  strlen("../../en_us/man_exlib_")) == 0)
      strncpy( link_ref, &ctx->rw->doc_xlink_ref[i][0] + strlen("../../en_us/man_exlib_"),
	       sizeof(link_ref));
    else
      strncpy( link_ref, ctx->rw->doc_xlink_ref[i], sizeof(link_ref));
    
    if ( (s = strrchr( link_ref, '.')))
      *s = 0;
    strcat( link_ref, ", ,$pwr_exe/en_us/man_exlib.dat");
    fp_tmp << "      " << ctx->rw->doc_xlink_text[i] << " <LINK>" << link_ref << endl;
  }
  for ( i = 0; i < ctx->rw->doc_clink_cnt; i++) {
    strcpy( prefix, CnvCtx::low(ctx->rw->volume_name));
    strcat( prefix, "_");
    if ( strncmp( ctx->rw->doc_clink_ref[i], prefix, strlen(prefix)) == 0) {
      strcpy( link_ref, &ctx->rw->doc_clink_ref[i][strlen(prefix)]);
      if ( (s = strrchr( link_ref, '.')))
	*s = 0;
    }
    else {
      if ( (s = strchr( ctx->rw->doc_clink_ref[i], '_'))) {
	int len = s - ctx->rw->doc_clink_ref[i];
	memset( link_ref, 0, sizeof(link_ref));
	strcpy( link_ref, &ctx->rw->doc_clink_ref[i][len+1]);
	if ( (s = strrchr( link_ref, '.')))
	  *s = 0;
	strcat( link_ref, ", ,$pwr_lang/");
	strncat( link_ref, ctx->rw->doc_clink_ref[i], len);
	strcat( link_ref, "_xtthelp.dat");
      }
      else {
	strcpy( link_ref, ctx->rw->doc_clink_ref[i]);
	if ( (s = strrchr( link_ref, '.')))
	  *s = 0;
      }
    }
    fp_tmp << "      " << ctx->rw->doc_clink_text[i] << " <LINK>" << link_ref << endl;
  }
  return 1;
}

int CnvWblToXtthelp::body_exec()
{
  fp_tmp <<
"<HR>" << endl <<
"<H1>" << ctx->rw->body_name << endl;

  return 1;
}

int CnvWblToXtthelp::class_close()
{

  xtthelp_in_topic = 0;
  fp_tmp <<
"</TOPIC>" << endl << endl;

  return 1;
}

int CnvWblToXtthelp::bit_exec()
{
  return 1;
}

int CnvWblToXtthelp::attribute_exec()
{
  int i;
  char *s;
  int lng_sts = 1;

  if ( strcmp( ctx->rw->attr_typeref, "CastId") == 0 ||
       strcmp( ctx->rw->attr_typeref, "DisableAttr") == 0)
    return 1;
    
  if ( Lng::current() != lng_eLanguage_en_us)
    lng_sts = ctx->rw->read_lng( ctx->rw->class_name, ctx->rw->attr_name);

  fp_tmp <<
endl <<
"<H2>" << ctx->rw->attr_name << "<BOOKMARK>" << ctx->rw->attr_name << endl;
  if ( ctx->rw->attr_array && ctx->rw->attr_pointer)
    fp_tmp <<
"<B>Array[" << ctx->rw->attr_elements << "] of pointers to " << ctx->rw->attr_typeref << "   ";
  else if ( ctx->rw->attr_array)
    fp_tmp <<
"<B>Array[" << ctx->rw->attr_elements << "] of "<< ctx->rw->attr_typeref << "   ";
  else if ( ctx->rw->attr_pointer)
    fp_tmp <<
"<B>Pointer to " << ctx->rw->attr_typeref << "   ";
  else
    fp_tmp <<
"<B>" << ctx->rw->attr_typeref << "   ";

  fp_tmp <<
"$" << ctx->rw->attr_type << "   ";

  if ( strcmp( ctx->rw->attr_flags, "") != 0)
    fp_tmp <<
"Flags[" << ctx->rw->attr_flags << "]";

  fp_tmp << endl;

  if ( ctx->rw->doc_fresh) {
    for ( i = 0; i < ctx->rw->doc_cnt; i++) {
      if ( (s = strstr( CnvCtx::low(ctx->rw->doc_text[i]), "@image")) != 0)  {
	char imagefile[80];

	CnvCtx::remove_spaces( s + 6, imagefile);
	fp_tmp << "<IMAGE> " << imagefile << endl;
      }
      else
	fp_tmp << ctx->rw->doc_text[i] << endl;
    }
  }
  return 1;
}

