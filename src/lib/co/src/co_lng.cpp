/** 
 * Proview   $Id: co_lng.cpp,v 1.17 2008-10-31 12:51:30 claes Exp $
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
 
#include "co_dcli.h"
#include "co_cdh.h"
#include "co_lng.h"
#include "co_lng_msg.h"

// Note, this is matched with lng_eLanguage
static char lng_cLanguageStr[lng_eLanguage__][6] = {
  "",
  "af_ZA",
  "ar_AE",
  "ar_BH",
  "ar_DZ",
  "ar_EG",
  "ar_IN",
  "ar_IQ",
  "ar_JO",
  "ar_KW",
  "ar_LB",
  "ar_LY",
  "ar_MA",
  "ar_OM",
  "ar_QA",
  "ar_SA",
  "ar_SD",
  "ar_SY",
  "ar_TN",
  "ar_YE",
  "be_BY",
  "bg_BG",
  "br_FR",
  "bs_BA",
  "ca_ES",
  "cs_CZ",
  "cy_GB",
  "da_DK",
  "de_AT",
  "de_BE",
  "de_CH",
  "de_DE",
  "de_LU",
  "el_GR",
  "en_AU",
  "en_BW",
  "en_CA",
  "en_DK",
  "en_GB",
  "en_HK",
  "en_IE",
  "en_IN",
  "en_NZ",
  "en_PH",
  "en_SG",
  "en_US",
  "en_ZA",
  "en_ZW",
  "es_AR",
  "es_BO",
  "es_CL",
  "es_CO",
  "es_CR",
  "es_DO",
  "es_EC",
  "es_ES",
  "es_GT",
  "es_HN",
  "es_MX",
  "es_NI",
  "es_PA",
  "es_PE",
  "es_PR",
  "es_PY",
  "es_SV",
  "es_US",
  "es_UY",
  "es_VE",
  "et_EE",
  "eu_ES",
  "fa_IR",
  "fi_FI",
  "fo_FO",
  "fr_BE",
  "fr_CA",
  "fr_CH",
  "fr_FR",
  "fr_LU",
  "ga_IE",
  "gl_ES",
  "gv_GB",
  "he_IL",
  "hi_IN",
  "hr_HR",
  "hu_HU",
  "id_ID",
  "is_IS",
  "it_CH",
  "it_IT",
  "iw_IL",
  "ja_JP",
  "ka_GE",
  "kl_GL",
  "ko_KR",
  "kw_GB",
  "lt_LT",
  "lv_LV",
  "mi_NZ",
  "mk_MK",
  "mr_IN",
  "ms_MY",
  "mt_MT",
  "nl_BE",
  "nl_NL",
  "nn_NO",
  "no_NO",
  "oc_FR",
  "pl_PL",
  "pt_BR",
  "pt_PT",
  "ro_RO",
  "ru_RU",
  "ru_UA",
  "se_NO",
  "sk_SK",
  "sl_SI",
  "sq_AL",
  "sr_YU",
  "sv_FI",
  "sv_SE",
  "ta_IN",
  "te_IN",
  "tg_TJ",
  "th_TH",
  "tl_PH",
  "tr_TR",
  "uk_UA",
  "ur_PK",
  "uz_UZ",
  "vi_VN",
  "wa_BE",
  "yi_US",
  "zh_CN",
  "zh_HK",
  "zh_TW"};

lng_eCoding Lng::translfile_coding = lng_eCoding_ISO8859_1;
lng_eLanguage Lng::lang = lng_eLanguage_en_US;
tree_sTable *Lng::tree = 0;

char *Lng::get_language_str()
{
  return lang_to_str( lang);
}

lng_eLanguage Lng::str_to_lang( char *str)
{
  for ( int i = 1; i < lng_eLanguage__; i++) {
    if ( cdh_NoCaseStrcmp( lng_cLanguageStr[i], str) == 0)
      return (lng_eLanguage) i;
  }

  // Not found
  return lng_eLanguage_;
}

// Convert language enum to lower case string
char *Lng::lang_to_str( lng_eLanguage language)
{  
  static char result[20];

  if ( language >= lng_eLanguage__ || language <= 0)
    strcpy( result, "");
  else
    strcpy( result, cdh_Low( lng_cLanguageStr[language]));

  return result;
}

// Convert language enum to string
char *Lng::lang_to_locale( lng_eLanguage language)
{
  static char result[20];

  if ( language >= lng_eLanguage__ || language <= 0)
    strcpy( result, "");
  else
    strcpy( result, lng_cLanguageStr[language]);

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
		       (unsigned long)s - (unsigned long)xKey->text);
      return strcmp(xKey->text, yKey->text);
    }
    }
    return 0;
}

char *Lng::translate( const char *text) 
{
  static char result[200];
  lang_sKey key;
  lang_sRecord *record;
  int sts;
  char *in_p;

  if ( lang == lng_eLanguage_en_US || tree == 0) {
    // No translation is needed
    strncpy( result, text, sizeof(result));
    result[sizeof(result)-1] = 0;
    return result;
  }

  // Remove space
  for ( in_p = (char *)text; *in_p == ' ' && *in_p; in_p++)
    ;

  strncpy( key.text, text, sizeof(key.text));
  key.type = 0;
  record = (lang_sRecord *) tree_Find( &sts, tree, &key);
  if ( ODD(sts)) {
    switch ( record->key.type) {
    case 'B': {
      char *s = strrchr( record->transl, ',');
      if ( s) {
	const char *t = strrchr( text, ',');
	if ( t) {
	  long int len = (unsigned long)s - (unsigned long)record->transl + 1;
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

  if ( lang == lng_eLanguage_en_US || tree == 0) {
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
		   (unsigned long)s - (unsigned long)record->transl + 1);
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
  pwr_tStatus sts;

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
    if ( !fp1 && i == 0) {
      // Try $pwr_eexe
      strcpy( fname1, "$pwr_eexe/en_us/xtt_lng.dat");
      dcli_translate_filename( filename1, fname1);
      fp1.open( filename1);
      if ( !fp1) 
	return true;
    }
    else if ( !fp1)
      return i == 0 ? false : true;

    ifstream fp2( filename2);
    if ( !fp2 && i == 0) {
      // Try $pwr_eexe
      strcpy( fname2, "$pwr_eexe/%s/xtt_lng.dat");
      sprintf( filename2, fname2, get_language_str());
      dcli_translate_filename( filename2, filename2);
      fp2.open( filename2);
      if ( !fp2) 
	return true;
    }
    else if ( !fp2)
      return i == 0 ? false : true;

    Row r1( fp1, fname1);
    Row r2( fp2, fname2);

    read_metadata( fp2, (i == 0), &sts);

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

	strncpy( key.text, r1.text, sizeof(key.text));
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
  pwr_tStatus sts;

  lng_eLanguage l = str_to_lang( language);
  if ( l != lng_eLanguage_) {
    sts = set( l);
    if ( EVEN(sts)) return;
  }
  setlocale( LC_ALL, lang_to_locale( l));
  setlocale( LC_NUMERIC, "POSIX");
  setlocale( LC_TIME, "en_US");
}

pwr_tStatus Lng::set( lng_eLanguage language) 
{ 
  if ( lang == language)
    return 1;

  if ( language == lng_eLanguage_en_US) {
    lang = language;
    unload();
  }
  else {
    if ( !is_installed( language))
      return 0;
    lang = language;
    read();
  }

  setlocale( LC_ALL, lang_to_locale( lang));
  setlocale( LC_NUMERIC, "POSIX");
  setlocale( LC_TIME, "en_US");
  return 1;
}

bool Lng::is_installed( lng_eLanguage language)
{
  pwr_tFileName fname;
  pwr_tFileName found_file;
  pwr_tStatus sts;

  
  strcpy( fname, "$pwr_exe/");
  strcat( fname, lang_to_str( language));
  strcat( fname, "/xtt_lng.dat");

  dcli_translate_filename( fname, fname);

  sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
  if ( ODD(sts)) return true;

  // Try $pwr_eexe also 
  strcpy( fname, "$pwr_eexe/");
  strcat( fname, lang_to_str( language));
  strcat( fname, "/xtt_lng.dat");

  dcli_translate_filename( fname, fname);

  sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
  if ( ODD(sts)) return true;

  return false;
}


void Lng::read_metadata( ifstream& fp2, bool global, pwr_tStatus *sts) 
{
  char line[40];

  if ( !fp2.getline( line, sizeof( line))) {
    *sts = LNG__EOF;
    return;
  }
  *sts = LNG__SUCCESS;

  if ( strncmp( line, "Coding:UTF-8", 12) == 0) {
    if ( global)
      translfile_coding = lng_eCoding_UTF_8;
    else if ( translfile_coding != lng_eCoding_UTF_8)
      *sts = LNG__DIFFCODING;
  }
  else if ( strncmp( line, "Coding:ISO8859-1", 16) == 0) {
    if ( global)
      translfile_coding = lng_eCoding_ISO8859_1;
    else if ( translfile_coding != lng_eCoding_ISO8859_1)
      *sts = LNG__DIFFCODING;
  }
  else {
    fp2.seekg( 0, ios::beg);
    if ( global)
      translfile_coding = lng_eCoding_ISO8859_1;
    else if ( translfile_coding != lng_eCoding_ISO8859_1)
      *sts = LNG__DIFFCODING;
  }
}







