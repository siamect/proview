#ifndef co_lng_h
#define co_lng_h

/* co_lng.h -- Language

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

typedef enum {
  lng_eLanguage_en_us = 0,
  lng_eLanguage_sv_se = 1
} lng_eLanguage;

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

    static void set( lng_eLanguage language) { lang = language;};
    static lng_eLanguage current() { return lang;}
    static void set( char *language) 
      { if ( strcmp(language,"en_us") == 0) lang = lng_eLanguage_en_us;
        else if ( strcmp(language,"sv_se") == 0) lang = lng_eLanguage_sv_se;};
    static char *get( int item)
      { return items[lang][item];};
    static char *get( lng_eLanguage language, int item)
      { return items[language][item];};
    static char *translate( char *str);
    static int translate( char *in, char *out);
    static char *get_language_str();
    static void get_uid( char *in, char *out);
};

#if defined __cplusplus
}
#endif
#endif


