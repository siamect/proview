/* 
 * Proview   $Id: cnv_topdf.cpp,v 1.6 2008-10-16 11:07:17 claes Exp $
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

/* cnv_topdf.cpp --
   Convert xtt help file to postscript. */

/*_Include files_________________________________________________________*/

#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

#include "cnv_topdf.h"
#include "co_lng.h"
#include "cnv_image.h"

#define CNV_TAB 18

#define pdf_cHead "%PDF-1.4"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

void CnvPdfObj::print_begin()
{
  offset = topdf->fp[topdf->cf].tellp();

  switch ( type) {

  case pdf_eObjType_Catalog:
    topdf->fp[topdf->cf] <<
number <<  " 0 obj" << endl <<
"  << /Type /Catalog" << endl <<
"     /Outlines 2 0 R" << endl <<
"     /Pages " << number + topdf->v_outline.size() << " 0 R" << endl;

    if ( topdf->use_outlines)
      topdf->fp[topdf->cf] <<
"     /PageMode /UseOutlines" << endl;

    topdf->fp[topdf->cf] <<
"  >>" << endl <<
"endobj" << endl << endl;
    break;

  case pdf_eObjType_Outline:
    topdf->fp[topdf->cf] <<
number <<  " 0 obj" << endl <<
"  << /Type /Outlines" << endl;
    if ( first)
      topdf->fp[topdf->cf] <<
"     /First " << first+1 << " 0 R" << endl;
    if ( last)
      topdf->fp[topdf->cf] <<
"     /Last " << last+1 << " 0 R" << endl;
    topdf->fp[topdf->cf] <<
"     /Count " << count << endl <<
"  >>" << endl <<
"endobj" << endl << endl;
    break;
  case pdf_eObjType_Pages:
    topdf->fp[topdf->cf] <<
number + topdf->v_outline.size() <<  " 0 obj" << endl <<
"  << /Type /Pages" << endl <<
"     /Kids [" << endl;
    for ( int i = 1; i < (int)topdf->v_pages.size(); i++)
      topdf->fp[topdf->cf] << 
"              " << topdf->v_outline.size() + topdf->v_pages[i].number << " 0 R" << endl;

    topdf->fp[topdf->cf] << 
"           ]" << endl <<
"     /Count " << topdf->v_pages.size() - 1 << endl <<
"  >>" << endl <<
"endobj" << endl << endl;
    break; 

  case pdf_eObjType_Page:
    topdf->fp[topdf->cf] <<
number + topdf->v_outline.size() <<  " 0 obj" << endl <<
"  << /Type /Page" << endl <<
"     /Parent " << topdf->v_outline.size() + 1 << " 0 R" << endl <<
"     /Mediabox [0 0 " << pdf_cPageWidth << " " << pdf_cPageHeight << "]" << endl <<
"     /Contents " << number - 1 + topdf->v_outline.size() + topdf->v_pages.size() << " 0 R" << endl;
    if ( resource == -1) {
      topdf->fp[topdf->cf] <<
"     /Resources << /Procset " << topdf->v_outline.size() + topdf->v_pages.size() + 
      topdf->v_content.size() + 1 << " 0 R" << endl <<
"                   /Font <<" << endl;
      for ( int i = 1; i < (int)topdf->v_font.size(); i++)
	topdf->fp[topdf->cf] << 
"                         /F" << topdf->v_font[i].number-1 << " " << 
	topdf->v_font[i].number + topdf->v_outline.size() + topdf->v_pages.size() + 
	topdf->v_content.size() << " 0 R" << endl;

      topdf->fp[topdf->cf] <<
"                          >>" << endl <<
"                >>" << endl;
    }
    else {
      topdf->fp[topdf->cf] <<
"     /Resources " << topdf->v_outline.size() + topdf->v_pages.size() + topdf->v_content.size() +
        + topdf->v_font.size() + resource + 1 << " 0 R" << endl;
    }
    topdf->fp[topdf->cf] <<
"  >>" << endl <<
"endobj" << endl << endl;
    break;

  case pdf_eObjType_Content:
    topdf->fp[topdf->cf] <<
number + topdf->v_outline.size() + topdf->v_pages.size() <<  " 0 obj" << endl <<
"  << /Length " << length << " >>" << endl <<
"stream" << endl;
    start = (int) topdf->fp[topdf->cf].tellp();
    break;
    
  case pdf_eObjType_Process:
    topdf->fp[topdf->cf] <<
number + topdf->v_outline.size() + topdf->v_pages.size() + topdf->v_content.size() <<  " 0 obj" << endl <<
"  [/PDF /Text]" << endl <<
"endobj" << endl << endl;
    break;
    
  case pdf_eObjType_Font:
    topdf->fp[topdf->cf] <<
number + topdf->v_outline.size() + topdf->v_pages.size() + topdf->v_content.size() <<  " 0 obj" << endl <<
"  << /Type /Font" << endl <<      
"     /Subtype /Type1" << endl <<
"     /Name /F" << number-1 << endl <<
"     /BaseFont /" << fontname << endl <<
"     /Encoding /WinAnsiEncoding" << endl <<
"  >>" << endl <<
"endobj" << endl << endl;
    break;
    
  case pdf_eObjType_OutlineO:
    topdf->fp[topdf->cf] << 
number <<  " 0 obj" << endl <<
"  << /Title (" << text << ")" << endl <<
"     /Parent " << parent+1 << " 0 R" << endl;
    if ( prev)
      topdf->fp[topdf->cf] <<
"     /Prev " << prev+1 << " 0 R" << endl;
    if ( next)
      topdf->fp[topdf->cf] <<
"     /Next " << next+1 << " 0 R" << endl;
    if ( first)
      topdf->fp[topdf->cf] <<
"     /First " << first+1 << " 0 R" << endl;
    if ( last)
      topdf->fp[topdf->cf] <<
"     /Last " << last+1 << " 0 R" << endl;
    if ( count)
      topdf->fp[topdf->cf] <<
"     /Count " << count << endl;
    topdf->fp[topdf->cf] <<
"     /Dest [" << dest << " 0 R /XYZ null 700 null]" << endl <<
"  >>" << endl <<
"endobj" << endl << endl;
    break;

  case pdf_eObjType_Image:
    print_image();
    break;

  case pdf_eObjType_Resource:
    topdf->fp[topdf->cf] <<
number + topdf->v_outline.size() + topdf->v_pages.size() + topdf->v_content.size() + 
      topdf->v_font.size() <<  " 0 obj" << endl <<
"  << /ProcSet [/PDF /Text /ImageB]" << endl <<
"     /XObject <<" << endl;

    for ( int i = 0; i < xobject_cnt; i++) {
      topdf->fp[topdf->cf] <<
"                  /Im" << xobject[i] + 1 << " " << xobject[i] + 1 + topdf->v_outline.size() + 
	topdf->v_pages.size() + topdf->v_content.size() + topdf->v_font.size() + 
	topdf->v_resource.size() <<  " 0 R" << endl;
    }
    topdf->fp[topdf->cf] <<
"              >>" << endl <<
"     /Font <<" << endl;
    for ( int i = 1; i < (int)topdf->v_font.size(); i++)
      topdf->fp[topdf->cf] << 
"               /F" << topdf->v_font[i].number-1 << " " << 
	topdf->v_font[i].number + topdf->v_outline.size() + topdf->v_pages.size() + 
	topdf->v_content.size() << " 0 R" << endl;

    topdf->fp[topdf->cf] <<
"           >>" << endl <<
"  >>" << endl <<
"endobj" << endl;
    break;
  }
}

void CnvPdfObj::print_end()
{
  switch ( type) {
  case pdf_eObjType_Content:
    length = (int) topdf->fp[topdf->cf].tellp() - start;
    topdf->fp[topdf->cf] <<
"endstream" << endl <<
"endobj" << endl << endl;
    break;
  default: ;
  }
}

int CnvPdfObj::print_image()
{
  cnv_tImImage image;
  cnv_tPixmap pixmap;
  pwr_tFileName fname;
  int sts;
  int width, height;
#if 0
  unsigned char *rgb;
  unsigned char transp[3] = {255,0,255};
  int i, j;
  int grey;
#endif
  char c;
	
  // Try $pwr_doc/help/
  strcpy( fname, "$pwr_doc/help/");
  strcat( fname, text);
  dcli_translate_filename( fname, fname);

  sts = cnv_get_image( fname, &image, &pixmap);
  if ( EVEN(sts)) {
    // Try $pwr_exe
    strcpy( fname, "$pwr_exe/");
    strcat( fname, text);
    dcli_translate_filename( fname, fname);

    sts = cnv_get_image( fname, &image, &pixmap);
    if ( EVEN(sts)) {
      // Try $pwrp_exe
      strcpy( fname, "$pwrp_exe/");
      strcat( fname, text);
      dcli_translate_filename( fname, fname);

      sts = cnv_get_image( fname, &image, &pixmap);
      if ( EVEN(sts)) return 0;
    }
  }

  width = cnv_image_width( image);
  height = cnv_image_height( image);

  topdf->fp[topdf->cf] <<
number + topdf->v_outline.size() + topdf->v_pages.size() + topdf->v_content.size() + 
    topdf->v_font.size() + topdf->v_resource.size() << " 0 obj" << endl <<
"  << /Type /XObject" << endl <<
"     /Subtype /Image" << endl <<
"     /Width " << width  << endl <<
"     /Height " << height << endl <<
"     /ColorSpace /DeviceRGB" << endl <<
"     /BitsPerComponent 8" << endl << 
"     /Filter /DCTDecode" << endl << 
"     /Length " << length << endl <<
"  >>" << endl <<
"stream" << endl;

  start = (int) topdf->fp[topdf->cf].tellp();

  strcpy( fname, "/tmp/pwr_cnv.jpg");
  cnv_print_image( image, fname);

  ifstream fimg( fname);
  while ( fimg.get(c))
    topdf->fp[topdf->cf].put(c);

  topdf->fp[topdf->cf] << endl;
  length = (int) topdf->fp[topdf->cf].tellp() - start;
  topdf->fp[topdf->cf] <<
"endstream" << endl << 
"endobj" << endl;


  cnv_free_image( image, pixmap);
  return 1;
}

void CnvToPdf::cnv_text( char *to, char *from)
{
  if ( !from) {
    strcpy( to, "");
    return;
  }

  char *t = to;
  char *s = from;

  for ( ; *s; s++) {
    switch ( *s) {
    case '(':
      *t++ = '\\';
      *t++ = '(';
      break;
    case ')':
      *t++ = '\\';
      *t++ = ')';
      break;
    case '\\':
      *t++ = '\\';
      *t++ = '\\';
    default:
      *t++ = *s;
    }
  }
  *t = 0;
}

void CnvToPdf::print_text( char *text, CnvStyle& style, int mode)
{
  char str[1000];

  cnv_text( str, text);

  if ( style.sidebreak && mode & pdf_mPrintMode_Pos) {
    if ( page_number[cf] == 0) {
      // First header, no pagebreak
      page_number[cf] = 1;
    }
    else {
      print_pagebreak( 0);
    }
  }
  else if ( style.pagebreak && mode & pdf_mPrintMode_Pos) {
    print_pagebreak( 0);
  }  

  if ( mode & pdf_mPrintMode_Pos || mode & pdf_mPrintMode_Start) {
    y -= style.top_offset;

    if ( y - style.bottom_offset < pdf_cBottomMargin) {
      print_pagebreak( 0);
    }
  }
  else
    y += style.bottom_offset;

  if ( !(mode & pdf_mPrintMode_FixX)) {
    if ( style.alignment == cnv_eAlignment_Center) {
      x = pdf_cLeftMargin/2 + (pdf_cPageWidth - pdf_cLeftMargin/2)/ 2 - 
	0.50 * strlen(text) * style.font_size / 2;
      if ( x < pdf_cLeftMargin/2)
	x = pdf_cLeftMargin/2;
    }
    else
      x = pdf_cLeftMargin + style.indentation;
  }
       
  int pmode = mode & 31;

  if ( strcmp( text, "") != 0) {
    switch ( pmode) {
    case pdf_mPrintMode_Pos:
    case pdf_mPrintMode_KeepY: {
      // Full path with beginning and end
      fp[cf] <<
"  BT" << endl <<
"    " << fontname(style) << " "  << style.font_size << " Tf" << endl <<
"    " << x << " " << y << " Td" << endl <<
"    (" << str << ") Tj" << endl <<
"  ET"  << endl;
      break;
    }
    case pdf_mPrintMode_Start: {
      // Start new path
      fp[cf] <<
"  BT" << endl <<
"    " << fontname(style) << " "  << style.font_size << " Tf" << endl <<
"    " << x << " " << y << " Td" << endl <<
"    (" << str << ") Tj" << endl;
      break;
    }
    case pdf_mPrintMode_Continue: {
      // Continue current path
      fp[cf] <<
"    " << fontname(style) << " "  << style.font_size << " Tf" << endl <<
"    " << x << " " << y << " Td" << endl <<
"    (" << str << ") Tj" << endl;
      break;
    }
    case pdf_mPrintMode_End: {
      // Continue and close current path
      fp[cf] <<
"    " << fontname(style) << " "  << style.font_size << " Tf" << endl <<
"    (" << str << ") Tj" << endl <<
"  ET"  << endl;
      break;
    }
    default: ;
    }
  }
  else {
    switch ( pmode) {
    case pdf_mPrintMode_Start: {
      // Start new path
      fp[cf] <<
"  BT" << endl <<
"    " << x << " " << y << " Td" << endl;
      break;
    }
    case pdf_mPrintMode_End: {
      // Continue and close current path
      fp[cf] <<
"  ET"  << endl;
      break;
    }
    default: ;
    }
  }
  y -= style.bottom_offset;
}

void CnvToPdf::draw_rect( double lw, double x, double y, double w, double h)
{
      fp[cf] <<
"  " << lw << " w" << endl <<
"  " << x << " " << y << " " << w << " " << h << " re S" << endl;
}

void CnvToPdf::draw_arc( double lw, double x, double y, double w, double h,
			  int angle1, int angle2)
{
      fp[cf] <<
"  " << lw << " w" << endl;
      if ( (angle1 == 0 && angle2 >= 90) ||
	   (angle1 == 90 && angle2 >= 360) ||
	   (angle1 == 180 && angle2 >= 270) ||
	   (angle1 == 270 && angle2 >= 180))
	fp[cf] <<
"  " << x + w << " " << y + h/2 << " m" <<
"  " << x + w << " " << y + h << " " << x + w << " " << y + h << " " << x + w/2 << " " << y + h << " " << " c" << endl;
      if ( (angle1 == 0 && angle2 >= 180) ||
	   (angle1 == 90 && angle2 >= 90) ||
	   (angle1 == 180 && angle2 >= 360) ||
	   (angle1 == 270 && angle2 >= 270))
	fp[cf] <<
"  " << x + w/2 << " " << y + h << " m" <<
"  " << x << " " << y + h << " " << x << " " << y + h << " " << x << " " << y + h/2 << " " << " c" << endl;
      if ( (angle1 == 0 && angle2 >= 270) ||
	   (angle1 == 90 && angle2 >= 180) ||
	   (angle1 == 180 && angle2 >= 90) ||
	   (angle1 == 270 && angle2 >= 360))
	fp[cf] <<
"  " << x  << " " << y + h/2 << " m" <<
"  " << x << " " << y << " " << x << " " << y << " " << x + w/2 << " " << y << " " << " c" << endl;
      if ( (angle1 == 0 && angle2 >= 360) ||
	   (angle1 == 90 && angle2 >= 270) ||
	   (angle1 == 180 && angle2 >= 180) ||
	   (angle1 == 270 && angle2 >= 90))
	fp[cf] <<
"  " << x + w/2  << " " << y << " m" <<
"  " << x + w << " " << y << " " << x + w << " " << y << " " << x + w << " " << y + h/2 << " " << " c" << endl;
      fp[cf] <<
"  S" << endl;
}

void CnvToPdf::draw_line( double lw, double x1, double y1, double x2, double y2,
 			  int dashed, int gray)
{
      fp[cf] <<
"  " << lw << " w" << endl;
      if ( dashed)
	fp[cf] <<
"  [2 3] 1 d" << endl;
      if ( gray)
	fp[cf] <<
" 0.7 G" << endl;
      fp[cf] <<
"  " << x1 << " " << y1 << " m" << endl <<
"  " << x2 << " " << y2 << " l S" << endl;
      if ( dashed)
	fp[cf] <<
"  [] 0 d" << endl;
      if ( gray)
	fp[cf] <<
"  0 G" << endl;

}
void CnvToPdf::draw_arrow( double x1, double y1, double x2, double y2, double x3, double y3,
			   int gray)
{
      if ( gray)
	fp[cf] <<
" 0.7 G" << endl <<
" 0.7 g" << endl;
      fp[cf] <<
"  " << x1 << " " << y1 << " m" << endl <<
"  " << x2 << " " << y2 << " l" << endl <<
"  " << x3 << " " << y3 << " l" << endl <<
"  " << x1 << " " << y1 << " l" << endl <<
"  B" << endl;
      if ( gray)
	fp[cf] <<
"  0 G" << endl <<
"  0 g" << endl;
}

void CnvToPdf::draw_text( double x, double y, char *text, int bold, double size)
{
  char fontname[20];

  if ( bold)
    strcpy( fontname, "/F2");
  else
    strcpy( fontname, "/F1");
    
  fp[cf] <<
"  BT" << endl <<
"    " << fontname << " "  << size << " Tf" << endl <<
"    " << x << " " << y << " Td" << endl <<
"    (" << text << ") Tj" << endl <<
"  ET"  << endl;
}

void CnvToPdf::print_pagebreak( int last)
{
  if ( page_number[cf] == 0)
    page_number[cf] = 1;
  if ( page_number[cf] != 1 || last) {
    double page_x;

    page_x = pdf_cPageNumX;

    if ( !(prev_ci == pdf_eId_TitlePage || prev_ci == pdf_eId_InfoPage)) {
      prev_ci = ci;

      fp[cf] <<
"  1 w" << endl <<
"  " << 10 << " " <<pdf_cPageHeight - 20 << " m" << endl <<
"  " << pdf_cPageWidth + 65 << " " << pdf_cPageHeight - 20 << " l" << endl <<
"  S" << endl <<
"  BT" << endl <<
"    /F1 10 Tf" << endl <<
"    " << pdf_cPageWidth / 2 - 10 * 0.5 * strlen(previous_chapter) << " " << pdf_cPageNumY << " Td" << endl <<
"    (" << previous_chapter << ") Tj" << endl <<
"  ET" << endl <<
"  BT" << endl <<
"    /F1 10 Tf" << endl <<
"    " << page_x << " " << pdf_cPageNumY << " Td" << endl <<
"    (" << page_number[cf]-1 << ") Tj" << endl <<
"  ET" << endl;
    }

    if ( page_number[cf] > 1 && v_content.size() > 0)
      v_content[page_number[cf]-2].print_end();
  }  

  if ( last)
    return;

  if ( conf_pass) {
    CnvPdfObj o1 = CnvPdfObj( this, pdf_eObjType_Page, v_pages.size()+1);
    v_pages.push_back( o1);

    CnvPdfObj o2 = CnvPdfObj( this, pdf_eObjType_Content, v_content.size()+1);
    v_content.push_back( o2);
  }
  
  if ( (int)v_content.size() > page_number[cf] - 1)
    v_content[page_number[cf]-1].print_begin();

  page_number[cf]++;

  y = pdf_cPageHeight - pdf_cTopMargin;
}

void CnvToPdf::print_content()
{
  int size = content.tab.size();
  int level = 0;
  int prev[10] = {0,0,0,0,0,0,0,0,0,0};
  int current = v_outline.size();
  int root = 1;
  int parent[4] = {0,0,0,0};
  int offset = current;

  if ( !current)
    return;

  v_outline[root].first = current;
  v_outline[root].last = current + size - 1;
  v_outline[root].count = size;
  parent[level] = root;

  for ( int i = 0; i < size; i++) {
    level = content.tab[i].header_level;
    if ( level < 0)
      level = 0;

    CnvPdfObj o1 = CnvPdfObj( this, pdf_eObjType_OutlineO, current+1);
    strcpy( o1.text, content.tab[i].text);
    o1.parent = parent[level];
    if ( parent[level] != root) {
      v_outline[parent[level]].last = current;
      v_outline[parent[level]].count++;
    }
    if ( i != size - 1) {
      if ( content.tab[i+1].header_level > content.tab[i].header_level) {
	parent[level+1] = current;
	o1.first = current + 1;
      }
    }
    if ( prev[level]) {
      o1.prev = prev[level];
      v_outline[o1.prev].next = current;
    }
    o1.dest = size + offset + content.tab[i].page_number;
    
    prev[level] = current;
    v_outline.push_back( o1);
    current++;

    if ( i != size - 1) {
      int next_level = content.tab[i+1].header_level;
      if ( next_level < 0)
	next_level = 0;
      if ( level > next_level) {
	for ( int j = next_level + 1; j <= level; j++)
	  prev[j] = 0;
      }
    }
  }
    
#if 0
  ci = pdf_eId_Content;
  x = pdf_cLeftMargin;
  y = pdf_cPageHeight - pdf_cTopMargin;

  print_pagebreak( 0);
  print_text( Lng::translate("Contents"), style[ci].h1);

  for ( int i = 0; i < (int)content.tab.size(); i++) {
    char page_str[20];
    CnvStyle *cstyle = &style[ci].boldtext;

    fp[cf] <<
"gsave" << endl <<
"[1 3] 0 setdash" << endl;

    sprintf( page_str, "%d", content.tab[i].page_number);
    print_text( content.tab[i].header_number, *cstyle);
    x = pdf_cLeftMargin + 30 + content.tab[i].header_level * 5;
    y += cstyle->top_offset + cstyle->bottom_offset;
    print_text( content.tab[i].text, *cstyle, pdf_mPrintMode_Start | pdf_mPrintMode_FixX);
    x = pdf_cLeftMargin + 340;

    fp[cf] <<
x << " " << y + cstyle->bottom_offset << " lineto" << endl <<
x << " " << y + cstyle->bottom_offset << " moveto" << endl <<
"closepath" << endl <<
"stroke" << endl <<
"grestore" << endl;

    print_text( page_str, *cstyle, pdf_mPrintMode_KeepY | pdf_mPrintMode_FixX);
  }
  print_pagebreak(0);
#endif
}

CnvToPdf::~CnvToPdf()
{
}

void CnvToPdf::close()
{
  cf = pdf_eFile_Body;
  print_pagebreak( 1);
  // print_content();

  for ( int i = 0; i < (int)v_font.size(); i++) {
    v_font[i].print_begin();
    v_font[i].print_end();
  }

  for ( int i = 0; i < (int)v_resource.size(); i++) {
    v_resource[i].print_begin();
    v_resource[i].print_end();
  }

  for ( int i = 0; i < (int)v_image.size(); i++) {
    v_image[i].print_begin();
    v_image[i].print_end();
  }

  xref_offset = fp[cf].tellp();
  fp[cf] << endl <<
"xref" << endl <<
"0 " << v_outline.size() + v_pages.size() + v_content.size() + v_font.size() + v_resource.size() +
    v_image.size() + 1 << endl <<
"0000000000 65535 f " << endl;

  for ( int i = 0; i < (int)v_outline.size(); i++) {
    fp[cf].fill('0');
    fp[cf].width(10);
    fp[cf] << v_outline[i].offset << " 00000 n " << endl;
  }
  for ( int i = 0; i < (int)v_pages.size(); i++) {
    fp[cf].fill('0');
    fp[cf].width(10);
    fp[cf] << v_pages[i].offset << " 00000 n " << endl;
  }
  for ( int i = 0; i < (int)v_content.size(); i++) {
    fp[cf].fill('0');
    fp[cf].width(10);
    fp[cf] << v_content[i].offset << " 00000 n " << endl;
  }
  for ( int i = 0; i < (int)v_font.size(); i++) {
    fp[cf].fill('0');
    fp[cf].width(10);
    fp[cf] << v_font[i].offset - start_offset << " 00000 n " << endl;
  }
  for ( int i = 0; i < (int)v_resource.size(); i++) {
    fp[cf].fill('0');
    fp[cf].width(10);
    fp[cf] << v_resource[i].offset - start_offset << " 00000 n " << endl;
  }
  for ( int i = 0; i < (int)v_image.size(); i++) {
    fp[cf].fill('0');
    fp[cf].width(10);
    fp[cf] << v_image[i].offset - start_offset << " 00000 n " << endl;
  }

  fp[cf] << endl <<
"trailer" << endl <<
"  << /Size " << v_outline.size() + v_pages.size() + v_content.size() + v_font.size() << 
    endl <<
"     /Root 1 0 R" << endl <<
"  >>" << endl <<
"startxref" << endl <<
xref_offset - start_offset<< endl <<
"%%EOF" << endl;

  fp[pdf_eFile_Body].close();
}

static void image_pixel( void *userdata, ofstream& fp, unsigned char *rgb)
{
  unsigned char transp[3] = {255,0,255};
  int grey;

  if ( *rgb == transp[0] && *(rgb+1) == transp[1] && *(rgb+2) == transp[2])
    grey = 255;
  else
    grey = (int) ((0.0 + *rgb + *(rgb+1) + *(rgb+2)) / 3 + 0.5);

  fp << (unsigned char)grey;

}

void CnvToPdf::print_horizontal_line()
{
  y -= 3;
  fp[cf] <<
"  1 w" << endl <<
"  " << pdf_cLeftMargin - 50 << " " << y << " m" << endl <<
"  " << pdf_cPageWidth << " " << y << " l" << endl <<
"  S" << endl;

  y -= 3;
}

int CnvToPdf::print_image_inline( char *filename)
{
  cnv_tImImage image;
  cnv_tPixmap pixmap;
  pwr_tFileName fname;
  int sts;
  int width, height;
  double scalex = 0.71;
  double scaley = 0.78;
	
  x = pdf_cLeftMargin;

  // Try $pwr_doc/help/
  strcpy( fname, "$pwr_doc/help/");
  strcat( fname, filename);
  dcli_translate_filename( fname, fname);

  sts = cnv_get_image( fname, &image, &pixmap);
  if ( EVEN(sts)) {
    // Try $pwr_exe
    strcpy( fname, "$pwr_exe/");
    strcat( fname, filename);
    dcli_translate_filename( fname, fname);

    sts = cnv_get_image( fname, &image, &pixmap);
    if ( EVEN(sts)) {
      // Try $pwrp_exe
      strcpy( fname, "$pwrp_exe/");
      strcat( fname, filename);
      dcli_translate_filename( fname, fname);

      sts = cnv_get_image( fname, &image, &pixmap);
      if ( EVEN(sts)) return 0;
    }
  }

  width = cnv_image_width( image);
  height = cnv_image_height( image);

  if ( width * scalex  > pdf_cPageWidth - pdf_cLeftMargin) {
    x = pdf_cPageWidth - width * scalex;
    if ( x < 50) {
      double scale_factor = (pdf_cPageWidth - 50) / (width * scalex);
      x = 50;
      scalex = scalex * scale_factor;
      scaley = scaley * scale_factor;
    }
  }

  if ( y - height * scaley + 20 < pdf_cBottomMargin)
    print_pagebreak( 0);


  fp[cf] <<
"  q" << endl <<
scalex * width << " 0 0 " << scaley * height << " " << x << " " << y - scaley * height << " cm" << endl <<
"  BI" << endl <<
"    /W " << width  << endl <<
"    /H " << height << endl <<
"    /CS /G" << endl <<
"    /BPC 8" << endl << 
#if 0
"    /F /AHx" << endl <<
#endif
"  ID" << endl;

  cnv_image_pixel_iter( image, image_pixel, 0, fp[cf]);


  fp[cf] << endl <<
"EI" << endl << 
"  Q" << endl;
  fp[cf].flags( ((fp[cf].flags() & ~ios_base::hex) & ~ios_base::uppercase) | ios_base::dec);

  cnv_free_image( image, pixmap);
  y -= height * scaley;

  return 1;
}

int CnvToPdf::print_image( char *filename)
{
  cnv_tImImage image;
  cnv_tPixmap pixmap;
  pwr_tFileName fname;
  int sts;
  int width, height;
  double scalex = 0.71;
  double scaley = 0.78;
	
  im_cnt++;
  x = pdf_cLeftMargin;

  // Try $pwr_doc/help/
  strcpy( fname, "$pwr_doc/help/");
  strcat( fname, filename);
  dcli_translate_filename( fname, fname);

  sts = cnv_get_image( fname, &image, &pixmap);
  if ( EVEN(sts)) {
    // Try $pwr_exe
    strcpy( fname, "$pwr_exe/");
    strcat( fname, filename);
    dcli_translate_filename( fname, fname);

    sts = cnv_get_image( fname, &image, &pixmap);
    if ( EVEN(sts)) {
      // Try $pwrp_exe
      strcpy( fname, "$pwrp_exe/");
      strcat( fname, filename);
      dcli_translate_filename( fname, fname);

      sts = cnv_get_image( fname, &image, &pixmap);
      if ( EVEN(sts)) return 0;
    }
  }

  width = cnv_image_width( image);
  height = cnv_image_height( image);

  if ( width * scalex  > pdf_cPageWidth - pdf_cLeftMargin) {
    x = pdf_cPageWidth - width * scalex;
    if ( x < 50) {
      double scale_factor = (pdf_cPageWidth - 50) / (width * scalex);
      x = 50;
      scalex = scalex * scale_factor;
      scaley = scaley * scale_factor;
    }
  }

  if ( y - height * scaley + 20 < pdf_cBottomMargin)
    print_pagebreak( 0);


  fp[cf] <<
"  q" << endl <<
"  " << scalex * width << " 0 0 " << scaley * height << " " << x << " " << y - scaley * height << " cm" << endl <<
"  /Im" << im_cnt << " Do" << endl <<
"  Q" << endl;

  if ( conf_pass) {
    if ( v_pages[v_pages.size()-1].resource == -1) {
      v_pages[v_pages.size()-1].resource = v_resource.size();

      CnvPdfObj o1 = CnvPdfObj( this, pdf_eObjType_Resource, v_resource.size()+1);
      v_resource.push_back( o1);
    }
    v_resource[v_resource.size()-1].xobject[v_resource[v_resource.size()-1].xobject_cnt] = 
      v_image.size();
    v_resource[v_resource.size()-1].xobject_cnt++;

    CnvPdfObj o2 = CnvPdfObj( this, pdf_eObjType_Image, v_image.size()+1);
    strcpy( o2.text, filename);
    v_image.push_back( o2);
  }

  cnv_free_image( image, pixmap);
  y -= height * scaley;

  return 1;
}


void CnvToPdf::set_pageheader( char *text)
{
  strcpy( previous_chapter, current_chapter);
  strcpy( current_chapter, text);
}

void CnvToPdf::print_h1( char *text, int hlevel, char *subject)
{
  char hnum[40];

  if ( ci == pdf_eId_Chapter) {
    set_pageheader( text);
  }

  if ( style[ci].h1.display_number) {
    if ( hlevel < 0)
      hlevel = 0;
    if ( hlevel > pdf_cMaxLevel - 1)
      hlevel = pdf_cMaxLevel - 1;
    header_number[hlevel]++;
    switch ( hlevel) {
    case 0:
      sprintf( hnum, "%d", header_number[0]);
      break;
    case 1:
      sprintf( hnum, "%d.%d", header_number[0], header_number[1]);
      break;
    case 2:
      sprintf( hnum, "%d.%d.%d", header_number[0], header_number[1], 
	       header_number[2]);
      break;
    case 3:
      sprintf( hnum, "%d.%d.%d.%d", header_number[0], header_number[1], 
	       header_number[2], header_number[3]);
      break;
    default: ;
    }
    x = pdf_cLeftMargin - 50;
    print_text( hnum, style[ci].h1, pdf_mPrintMode_Pos | pdf_mPrintMode_FixX);
    
    double x0 = x + 12.0 * ( strlen(hnum) + 1) * style[ci].h1.font_size / 24;
    x = pdf_cLeftMargin;
    if ( x0 > x)
      x = x0;

    print_text( text, style[ci].h1, pdf_mPrintMode_KeepY | pdf_mPrintMode_FixX);
  }
  else 
    print_text( text, style[ci].h1);
  
  if ( conf_pass && !(ci == pdf_eId_TitlePage || ci == pdf_eId_InfoPage)) {
    CnvContentElem cnt;
    cnt.page_number = page_number[cf];
    cnt.header_level = hlevel;
    strcpy( cnt.header_number, hnum);
    strcpy( cnt.text, text);
    strcpy( cnt.subject, subject);
    content.add( cnt);
  }
  strcpy( previous_chapter, current_chapter);
}

void CnvToPdf::print_h2( char *text)
{
  print_text( text, style[ci].h2);
}

void CnvToPdf::print_h3( char *text)
{
  print_text( text, style[ci].h3);
}

void CnvToPdf::open()
{
  y = pdf_cPageHeight - pdf_cTopMargin;
  im_cnt = 0;

  if ( conf_pass) {
    fp[pdf_eFile_Body].open( filename[pdf_eFile_Body]);
    fp[pdf_eFile_Body] << setiosflags( ios::fixed) << setprecision(6);
    start_offset = fp[pdf_eFile_Body].tellp();
  }
  else {
    print_content();

    conf_pass = true;
    page_number[cf] = v_content.size() + 1;
    print_pagebreak(1);
    conf_pass = false;
    for ( int i = 0; i < (int)v_image.size(); i++) {
      v_image[i].print_begin();
      v_image[i].print_end();
    }
    // if ( v_content.size() > 0 && v_content[v_content.size()-1].length == 0)
    //  v_content[v_content.size()-1].length = (int) fp[pdf_eFile_Body].tellp() - 
    //	v_content[v_content.size()-1].start;
    fp[pdf_eFile_Body].seekp( start_offset);
    strcpy( previous_chapter, "");
    strcpy( current_chapter, "");
  }
  cf = pdf_eFile_Body;
  page_number[cf] = 0;

  if ( conf_pass) {
    CnvPdfObj o1 = CnvPdfObj( this, pdf_eObjType_Catalog, v_outline.size()+1);
    v_outline.push_back( o1);

    CnvPdfObj o2 = CnvPdfObj( this, pdf_eObjType_Outline, v_outline.size()+1);
    v_outline.push_back( o2);

    CnvPdfObj o3 = CnvPdfObj( this, pdf_eObjType_Pages, v_pages.size()+1);
    v_pages.push_back( o3);

    CnvPdfObj o4 = CnvPdfObj( this, pdf_eObjType_Process, v_font.size()+1);
    v_font.push_back( o4);

    CnvPdfObj o5 = CnvPdfObj( this, pdf_eObjType_Font, v_font.size()+1);
    strcpy( o5.fontname, "Helvetica");
    v_font.push_back( o5);

    CnvPdfObj o6 = CnvPdfObj( this, pdf_eObjType_Font, v_font.size()+1);
    strcpy( o6.fontname, "Helvetica-Bold");
    v_font.push_back( o6);

    CnvPdfObj o7 = CnvPdfObj( this, pdf_eObjType_Font, v_font.size()+1);
    strcpy( o7.fontname, "Helvetica-Oblique");
    v_font.push_back( o7);

    CnvPdfObj o8 = CnvPdfObj( this, pdf_eObjType_Font, v_font.size()+1);
    strcpy( o8.fontname, "TimesNewRoman");
    v_font.push_back( o8);

    CnvPdfObj o9 = CnvPdfObj( this, pdf_eObjType_Font, v_font.size()+1);
    strcpy( o9.fontname, "TimesNewRoman,Bold");
    v_font.push_back( o9);

    CnvPdfObj o10 = CnvPdfObj( this, pdf_eObjType_Font, v_font.size()+1);
    strcpy( o10.fontname, "Courier");
    v_font.push_back( o10);
  }

  fp[cf] <<
    pdf_cHead << endl;

  for ( int i = 0; i < (int)v_outline.size(); i++) {
    v_outline[i].print_begin();
    v_outline[i].print_end();
  }
  for ( int i = 0; i < (int)v_pages.size(); i++) {
    v_pages[i].print_begin();
    v_pages[i].print_end();
  }

  print_pagebreak(0);

  for ( int i = 1; i < pdf_cMaxLevel; i++)
    header_number[i] = 0;
}

void CnvToPdf::incr_headerlevel()
{
  ci++;
  if ( ci > pdf_eId_TopicL3)
    ci = pdf_eId_TopicL3;
  if ( ci < pdf_eId_TopicL1)
    ci = pdf_eId_TopicL1;
      
  header_number[ci-(int)pdf_eId_Chapter] = 0;
}

void CnvToPdf::decr_headerlevel()
{
  ci--;
  if ( ci < pdf_eId_TopicL1)
    ci = pdf_eId_TopicL1;
}

void CnvToPdf::reset_headernumbers( int level)
{
      for ( int i = level; i < pdf_cMaxLevel; i++)
	header_number[i] = 0;
}

char *CnvToPdf::fontname( CnvStyle& style)
{
  static char str[80];
  char name[80];

  strcpy( name, style.font.c_str());
  if ( strncmp( name, "Helvetica-Bold", 14) == 0)
    strcpy( str, "/F2");
  else if ( strncmp( name, "Helvetica-Oblique", 17) == 0)
    strcpy( str, "/F3");
  else if ( strncmp( name, "Helvetica", 9) == 0)
    strcpy( str, "/F1");
  else if ( strncmp( name, "TimesNewRoman-Bold", 18) == 0)
    strcpy( str, "/F5");
  else if ( strncmp( name, "TimesNewRoman", 13) == 0)
    strcpy( str, "/F4");
  else if ( strncmp( name, "Courier", 7) == 0)
    strcpy( str, "/F6");
  else
    strcpy( str, "/F1");

  return str;
}


