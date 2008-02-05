/** 
 * Proview   $Id: co_lng.cpp,v 1.15 2008-02-05 13:15:48 claes Exp $
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


#include <string.h>
#include <stdio.h>
#include <locale.h>

#include "pwr.h"
 
extern "C" {
#include "co_dcli.h"
}
#include "co_lng.h"

lng_eLanguage Lng::lang = lng_eLanguage_en_us;
tree_sTable *Lng::tree = 0;

char *Lng::get_language_str()
{
  return lang_to_str( lang);
}

lng_eLanguage Lng::str_to_lang( char *str)
{
  if ( strcmp( str, "en_us") == 0)
    return lng_eLanguage_en_us;
  if ( strcmp( str, "sv_se") == 0)
    return lng_eLanguage_sv_se;
  if ( strcmp( str, "de_de") == 0)
    return lng_eLanguage_de_de;
  if ( strcmp( str, "fr_fr") == 0)
    return lng_eLanguage_fr_fr;

  return lng_eLanguage_;
}

char *Lng::lang_to_str( lng_eLanguage language)
{
  static char result[20];

  switch( language) {
    case lng_eLanguage_sv_se:
      strcpy( result, "sv_se");
      break;
    case lng_eLanguage_de_de:
      strcpy( result, "de_de");
      break;
    case lng_eLanguage_fr_fr:
      strcpy( result, "fr_fr");
      break;
    default:
      strcpy( result, "en_us");
  }
  return result;
}

char *Lng::lang_to_locale( lng_eLanguage language)
{
  static char result[20];

  switch( language) {
    case lng_eLanguage_sv_se:
      strcpy( result, "sv_SE");
      break;
    case lng_eLanguage_de_de:
      strcpy( result, "de_DE");
      break;
    case lng_eLanguage_fr_fr:
      strcpy( result, "fr_FR");
      break;
    default:
      strcpy( result, "en_US");
  }
  return result;
}

static int compKey( tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
    lang_sKey *xKey = (lang_sKey *) (tp->keyOffset + (char *) x);
    lang_sKey *yKey = (lang_sKey *) (tp->keyOffset + (char *) y);

    char type = (xKey->type == 0) ? yKey->type : xKey->type;
    switch ( type) {
    case 'E':
      return strcmp(xKey->text, yKey->text);
    case 'B': {
      char *s = strrchr( xKey->text, ',');
      if ( s)
	return strncmp(xKey->text, yKey->text, 
		       (unsigned int)s - (unsigned int)xKey->text);
      return strcmp(xKey->text, yKey->text);
    }
    }
    return 0;
}

char *Lng::translate( char *text) 
{
  static char result[200];
  lang_sKey key;
  lang_sRecord *record;
  int sts;
  char *in_p;

  if ( lang == lng_eLanguage_en_us || tree == 0) {
    // No translation is needed
    strncpy( result, text, sizeof(result));
    result[sizeof(result)-1] = 0;
    return result;
  }

  // Remove space
  for ( in_p = text; *in_p == ' ' && *in_p; in_p++)
    ;

  strncpy( key.text, text, sizeof(key.text));
  key.type = 0;
  record = (lang_sRecord *) tree_Find( &sts, tree, &key);
  if ( ODD(sts)) {
    switch ( record->key.type) {
    case 'B': {
      char *s = strrchr( record->transl, ',');
      if ( s) {
	char *t = strrchr( text, ',');
	if ( t) {
	  int len = (unsigned int)s - (unsigned int)record->transl + 1;
	  strncpy( result, record->transl, len);
	  result[len] = 0;
	  strcat( result, t+1);
	}
	else
	  strcpy( result, record->transl);
      }
      else
	strcpy( result, record->transl);
      break;
    }
    default:
      strcpy( result, record->transl);
    }
  }
  else {
    strncpy( result, text, sizeof(result));
    result[sizeof(result)-1] = 0;
  }
  return result;
}

int Lng::translate( char *text, char *out) 
{
  char result[200];
  lang_sKey key;
  lang_sRecord *record;
  int sts;
  char *in_p;

  if ( lang == lng_eLanguage_en_us || tree == 0) {
    // No translation is needed
    return 0;
  }

  // Remove space
  for ( in_p = text; *in_p == ' ' && *in_p; in_p++)
    ;

  strncpy( key.text, text, sizeof(key.text));
  key.type = 0;
  record = (lang_sRecord *) tree_Find( &sts, tree, &key);
  if ( ODD(sts)) {
    switch ( record->key.type) {
    case 'B': {
      char *s = strrchr( record->transl, ',');
      if ( s) {
	char *t = strrchr( text, ',');
	if ( t) {
	  strncpy( result, record->transl, 
		   (unsigned int)s - (unsigned int)record->transl + 1);
	  strcat( result, t+1);
	}
	else
	  strcpy( result, record->transl);
      }
      else
	strcpy( result, record->transl);
      break;
    }
    default:
      strcpy( result, record->transl);
    }
    strcpy( out, result);
    return 1;
  }
  // Not found
  return 0;
}

void Lng::unload()
{
  int sts;

  tree_DeleteTable( &sts, tree);
  tree = 0;
}

bool Lng::read()
{
  char fname1[120];
  char fname2[120];
  pwr_tFileName filename1, filename2;
  int sts;

  if ( tree)
    tree_DeleteTable( &sts, tree);

  tree = tree_CreateTable( &sts, sizeof(lang_sKey), offsetof(lang_sRecord, key),
			   sizeof(lang_sRecord), 100, compKey);

  for ( int i = 0; i < 2; i++) {
    if ( i == 0) {
      strcpy( fname1, "$pwr_exe/en_us/xtt_lng.dat");
      strcpy( fname2, "$pwr_exe/%s/xtt_lng.dat");
    }
    else {
      strcpy( fname1, "$pwrp_exe/xtt_lng_en_us.dat");
      strcpy( fname2, "$pwrp_exe/xtt_lng_%s.dat");
    }
    sprintf( filename2, fname2, get_language_str());
    dcli_translate_filename( filename1, fname1);
    dcli_translate_filename( filename2, filename2);

    ifstream fp1( filename1);
    if ( !fp1)
      return i == 0 ? false : true;

    ifstream fp2( filename2);
    if ( !fp2)
      return i == 0 ? false : true;

    Row r1( fp1, fname1);
    Row r2( fp2, fname2);
  
    bool hit = true;
    for (;;) {
      if ( hit) {
	if ( !read_line( r1))
	  break;

	if ( !read_line( r2))
	  break;
      }
      else if ( r1.lt( r2)) {
	if ( !read_line( r1))
	  break;
      }
      else {
	if ( !read_line( r2))
	  break;
      }

      hit = false;
      if ( r1.eq( r2))
	hit = true;
      if ( hit) {
	lang_sKey key;
	lang_sRecord *record;

	strcpy( key.text, r1.text);
	key.type = r1.type;
	record = (lang_sRecord *) tree_Insert( &sts, tree, &key);
	strcpy( record->transl, r2.text);
	// printf ( "%c %d.%d.%d '%s' '%s'\n", r1.type, r1.n1, r1.n2, r1.n3, r1.text,r2.text);
      }
    }    
  }
  return true;
}

bool Lng::read_line( Row& r)
{
  int nr;
  char line[200];
  char *s, *t;

  for(;;) {
    if ( !r.fp.getline( line, sizeof( line)))
      return false;

    r.row++;
    if ( line[0] == '#' || line[0] == 0)
      continue;

      
    nr = sscanf( line, "%c%d.%d.%d", &r.type, &r.n1, &r.n2, &r.n3);
    if ( nr != 4) {
      printf( "Error in line %d, file %s\n", r.row, r.fname);
      continue;
    }

    bool in_text = false;
    for ( s = line, t = r.text; *s; s++) {
      if ( in_text) {
	if ( *s == '\"') {
	  if ( *(s-1) == '\\')
	    *(t-1) = '\"';
	  else
	    break;
	}
	else
	  *t++ = *s;;	
      }
      if ( !in_text && *s == '\"')
	in_text = 1;
      if ( t > r.text + sizeof(r.text) - 1) {
	printf( "Error in line %d, file %s\n", r.row, r.fname);
	break;
      }
    }      
    *t = 0;
    break;
  }
  return true;
}


void Lng::get_uid( char *in, char *out)
{
  char result[200];

#if defined OS_VMS
  {
    char dev[80], dir[80], file[80], type[80];
    int version;
    char c;

    dcli_parse_filename( "pwr_exe:", dev, dir, file, type, &version);
    sprintf( result, "%s%s", dev, dir);
    c = result[strlen(result)-1];
    sprintf( &result[strlen(result)-1], ".%s%c%s%", get_language_str(), 
	     c, in);
  }
#else
  sprintf( result, "$pwr_exe/%s/%s", get_language_str(), in);
  dcli_translate_filename( result, result);
#endif
  strcpy( out, result);
}

void Lng::set( char *language) 
{ 
  lng_eLanguage l = str_to_lang( language);
  if ( l != lng_eLanguage_)
    set( l);

  setlocale( LC_ALL, lang_to_locale( l));
}

void Lng::set( lng_eLanguage language) 
{ 
  if ( lang == language)
    return;

  lang = language;
  if ( lang == lng_eLanguage_en_us)
    unload();
  else
    read();

  setlocale( LC_ALL, lang_to_locale( lang));
}











