

#include <iostream.h>
#include <fstream.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cnv_classread.h"
extern "C" {
#include "pwr.h"
#include "co_cdh.h"
}


int ClassRead::xtthelp_init()
{
  char fname[200];

  strcpy( fname, dir);
  strcat( fname, volume_name);
  strcat( fname, "_xtthelp.dat");
  cdh_ToLower( fname, fname);

  fp_xtthelp_index.open( fname);

  fp_tmp.open( cread_cTmpFile2);

  xtthelp_index_open = 1;
  xtthelp_in_topic= 1;

  fp_xtthelp_index <<
"<TOPIC> " << volume_name << endl <<
"ClassVolume " << volume_name << endl << endl;

  return 1;
}

int ClassRead::xtthelp_close()
{

  fp_xtthelp_index <<
"</TOPIC>" << endl << endl;

  fp_tmp.close();

  copy_tmp_file( cread_cTmpFile2, fp_xtthelp_index);
  
  fp_xtthelp_index.close();
  return 1;
}

int ClassRead::xtthelp_class()
{
  int i;
  char full_class_name[80];
  char link_ref[80];
  char *s;

  strcpy( full_class_name, volume_name);
  strcat( full_class_name, ":");
  strcat( full_class_name, class_name);
  
  // Add into index file
  fp_xtthelp_index <<
"<B>" << class_name << "<T><T> " << doc_summary << " <LINK> " << class_name << endl;

  // Add to class file
  if ( xtthelp_in_topic)
    fp_tmp <<
"</TOPIC>" << endl;

  xtthelp_in_topic = 1;
  fp_tmp <<
"<TOPIC> " << class_name << endl <<
"Class  " << full_class_name << endl <<
"<H1>" << class_name << endl << endl;

  if ( doc_fresh && strcmp( doc_author, "") != 0)
  {
    fp_tmp <<
"<B>Author<T>" << doc_author << endl;
  }

  if ( doc_fresh && strcmp( doc_version, "") != 0)
  {
    fp_tmp <<
"<B>Version<T>" << doc_version << endl;
  }

  fp_tmp <<
"<H1>Description" << endl;

  if ( doc_fresh)
    for ( i = 0; i < doc_cnt; i++)
      fp_tmp << doc_text[i] << endl;

  for ( i = 0; i < doc_clink_cnt; i++) {
    if ( strncmp( doc_clink_ref[i], "pwrb_", 5) == 0)
      strcpy( link_ref, &doc_clink_ref[i][5]);
    else if ( strncmp( doc_clink_ref[i], "pwrs_", 5) == 0)
      strcpy( link_ref, &doc_clink_ref[i][5]);
    else if ( strncmp( doc_clink_ref[i], "nmps_", 5) == 0)
      strcpy( link_ref, &doc_clink_ref[i][5]);
    else if ( strncmp( doc_clink_ref[i], "ssab_", 5) == 0)
      strcpy( link_ref, &doc_clink_ref[i][5]);
    if ( (s = strrchr( link_ref, '.')))
      *s = 0;
    fp_tmp << "      " << doc_clink_text[i] << " <LINK>" << link_ref << endl;
  }
  return 1;
}

int ClassRead::xtthelp_body()
{
  fp_tmp <<
"<H2>" << endl <<
"<H1>" << body_name << endl;

  return 1;
}

int ClassRead::xtthelp_class_close()
{

  xtthelp_in_topic = 0;
  fp_tmp <<
"</TOPIC>" << endl << endl;

  return 1;
}

int ClassRead::xtthelp_attribute()
{
  int i;

  fp_tmp <<
endl <<
"<H1>" << attr_name << endl;
  if ( attr_array && attr_pointer)
    fp_tmp <<
"<B>Array[" << attr_elements << "] of pointers to " << attr_typeref << "   ";
  else if ( attr_array)
    fp_tmp <<
"<B>Array[" << attr_elements << "] of "<< attr_typeref << "   ";
  else if ( attr_pointer)
    fp_tmp <<
"<B>Pointer to " << attr_typeref << "   ";
  else
    fp_tmp <<
"<B>" << attr_typeref << "   ";

  fp_tmp <<
"$" << attr_type << "   ";

  if ( strcmp( attr_flags, "") != 0)
    fp_tmp <<
"Flags[" << attr_flags << "]";

  fp_tmp << endl;

  if ( doc_fresh)
    for ( i = 0; i < doc_cnt; i++)
      fp_tmp << doc_text[i] << endl;

  return 1;
}

