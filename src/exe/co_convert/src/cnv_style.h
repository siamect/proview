/* 
 * Proview   $Id: cnv_style.h,v 1.3 2008-10-31 12:51:30 claes Exp $
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

#ifndef cnv_style_h
#define cnv_style_h

/* cnv_style.h -- Convert style */

using namespace std;

#include <iostream>
#include <vector>
#include <string>
#include "pwr.h"

using namespace std;

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


