

#include <iostream.h>
#include <fstream.h>
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
"Class  " << full_class_name << endl <<
"<H1>" << ctx->rw->class_name << endl << endl;

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_author, "") != 0)
  {
    fp_tmp <<
"<B>Author<T>" << ctx->rw->doc_author << endl;
  }

  if ( ctx->rw->doc_fresh && strcmp( ctx->rw->doc_version, "") != 0)
  {
    fp_tmp <<
"<B>Version<T>" << ctx->rw->doc_version << endl;
  }

  fp_tmp <<
"<H1>Description" << endl;

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
  for ( i = 0; i < ctx->rw->doc_clink_cnt; i++) {
    if ( strncmp( ctx->rw->doc_clink_ref[i], "pwrb_", 5) == 0)
      strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
    else if ( strncmp( ctx->rw->doc_clink_ref[i], "pwrs_", 5) == 0)
      strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
    else if ( strncmp( ctx->rw->doc_clink_ref[i], "nmps_", 5) == 0)
      strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
    else if ( strncmp( ctx->rw->doc_clink_ref[i], "ssab_", 5) == 0)
      strcpy( link_ref, &ctx->rw->doc_clink_ref[i][5]);
    if ( (s = strrchr( link_ref, '.')))
      *s = 0;
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

  fp_tmp <<
endl <<
"<H1>" << ctx->rw->attr_name << endl;
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

