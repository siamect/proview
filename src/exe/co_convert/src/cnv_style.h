#ifndef cnv_style_h
#define cnv_style_h

/* cnv_style.h -- Convert style

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

using namespace std;

#include <iostream.h>
#include <vector.h>
#include <string>
#include "pwr.h"

typedef enum {
  cnv_eAlignment_Left,
  cnv_eAlignment_Center
} cnv_eAlignment;

class CnvStyle {
 public:
  CnvStyle() {}
  CnvStyle( string s_font, int s_font_size, int s_indentation,
	    int s_top_offset, int s_bottom_offset, int s_alignment,
	    int s_pagebreak, int s_sidebreak, int s_display_number) :
    font(s_font), font_size(s_font_size), indentation(s_indentation),
    top_offset(s_top_offset), bottom_offset(s_bottom_offset), 
    alignment(s_alignment), pagebreak(s_pagebreak), sidebreak(s_sidebreak),
    display_number(s_display_number)
    {}
  string font;
  int font_size;
  int indentation;
  int top_offset;
  int bottom_offset;
  int alignment;
  int pagebreak;
  int sidebreak;
  int display_number;
};

class CnvIdStyle {
 public:
  CnvIdStyle() {
    h1 = CnvStyle( "Helvetica-Bold-ISOLatin1", 14, 0, 30, 25, cnv_eAlignment_Left, 0, 0, 1);
    h2 = CnvStyle( "Helvetica-Bold-ISOLatin1", 12, 0, 25, 20, cnv_eAlignment_Left, 0, 0, 1);
    h3 = CnvStyle( "Helvetica-Bold-ISOLatin1", 10, 0, 15, 10, cnv_eAlignment_Left, 0, 0, 1);
    text = CnvStyle( "Helvetica-ISOLatin1", 10, 0, 11, 2, cnv_eAlignment_Left, 0, 0, 0);
    boldtext = CnvStyle( "Helvetica-Bold-ISOLatin1", 10, 0, 11, 2, cnv_eAlignment_Left, 0, 0, 0);
    link = CnvStyle( "Helvetica-Oblique-ISOLatin1", 8, 0, 11, 2, cnv_eAlignment_Left, 0, 0, 0);
    image = CnvStyle( "Helvetica-Oblique-ISOLatin1", 10, 0, 10, 0, cnv_eAlignment_Left, 0, 0, 0);
    code = CnvStyle( "Courier-ISOLatin1", 10, 0, 11, 2, cnv_eAlignment_Left, 0, 0, 0);
  }
  CnvStyle h1;
  CnvStyle h2;
  CnvStyle h3;
  CnvStyle text;
  CnvStyle boldtext;
  CnvStyle link;
  CnvStyle image;
  CnvStyle code;
};


#endif


