/** 
 * Proview   $Id: co_lng.h,v 1.7 2007-04-17 13:55:58 claes Exp $
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

#ifndef co_lng_h
#define co_lng_h

/* co_lng.h -- Language */

#include <iostream.h>
#include <fstream.h>

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_tree_h
# include "co_tree.h"
#endif

typedef enum {
  lng_eLanguage_,
  lng_eLanguage_en_us,
  lng_eLanguage_sv_se,
  lng_eLanguage_de_de
} lng_eLanguage;

typedef struct {
  char		text[80];
  char		type;
} lang_sKey;

typedef struct {
  tree_sNode 	n;
  lang_sKey	key;
  char		transl[80];
} lang_sRecord;

class Row {
 public:
  char type;
  int n1;
  int n2;
  int n3;
  char text[200];
  int row;
  ifstream& fp;
  char fname[200];

  Row( ifstream& f, char *filename) : row(0), fp(f) { 
    strcpy( text, ""); strcpy( fname, filename);}
  bool eq( Row& r) { return n1 == r.n1 && n2 == r.n2 && n3 == r.n3;}
  bool lt( Row& r) {
    if ( n1 < r.n1)
      return true;
    else if ( n1 > r.n1)
      return false;
    if ( n2 < r.n2)
      return true;
    else if ( n2 > r.n2)
      return false;
    if ( n3 < r.n3)
      return true;
    return false;
  }
  bool gt( Row& r) {
    if ( n1 > r.n1)
      return true;
    else if ( n1 < r.n1)
      return false;
    if ( n2 > r.n2)
      return true;
    else if ( n2 < r.n2)
      return false;
    if ( n3 > r.n3)
      return true;
    return false;
  }
};

class Lng {
  public:
    Lng() {};
    static lng_eLanguage lang;
    static const int Help        = 0;
    static const int Help_Class  = 1;
    static const int Graph       = 2;
    static const int Trend       = 3;
    static const int Open_Object = 4;
    static const int Open_Object___ = 5;
    static const int RtNavigator = 6;
    static const int Class_Graph = 7;
    static const int Crossreferences = 8;
    static const int DataSheet   = 9;
    static const int Collect     = 10;
    static const int Type_Graph___ = 11;
    static const int Open_URL___ = 12;
    static const int Open_Plc___ = 13;
    static char items[][2][40];

    static tree_sTable *tree;

    static bool read();
    static bool read_line( Row& r);
    static char *translate( char *text);
    static char *get_language_str();
    static int translate( char *in, char *out);
    static void unload();
    static void set( lng_eLanguage language);
    static lng_eLanguage current() { return lang;}
    static void set( char *language);
    static char *get( int item)
      { return items[lang][item];}
    static char *get( lng_eLanguage language, int item)
      { return items[language][item];}
    static void get_uid( char *in, char *out);
    static lng_eLanguage str_to_lang( char *str);
    static char *lang_to_str( lng_eLanguage language);
    static char *lang_to_locale( lng_eLanguage language);
};

#if defined __cplusplus
}
#endif
#endif


