/* cnv_tops.cpp --

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   Convert xtt help file to postscript. */

/*_Include files_________________________________________________________*/

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

#include "cnv_tops.h"
#include "co_lng.h"
#include "cnv_image.h"

#define CNV_TAB 18

#define ps_cHead "%!PS-Adobe-2.0 EPSF-1.2\n\
%%Creator: Proview co_convert\n\
%%EndComments\n\
\n\
/Helvetica findfont\n\
dup length dict begin\n\
  { 1 index /FID ne\n\
	{def}\n\
	{pop pop}\n\
     ifelse\n\
  } forall\n\
  /Encoding ISOLatin1Encoding def\n\
  currentdict\n\
end\n\
/Helvetica-ISOLatin1 exch definefont pop\n\
/Helvetica-Bold findfont\n\
dup length dict begin\n\
  { 1 index /FID ne\n\
	{def}\n\
	{pop pop}\n\
     ifelse\n\
  } forall\n\
  /Encoding ISOLatin1Encoding def\n\
  currentdict\n\
end\n\
/Helvetica-Bold-ISOLatin1 exch definefont pop\n\
/Helvetica-Oblique findfont\n\
dup length dict begin\n\
  { 1 index /FID ne\n\
	{def}\n\
	{pop pop}\n\
     ifelse\n\
  } forall\n\
  /Encoding ISOLatin1Encoding def\n\
  currentdict\n\
end\n\
/Helvetica-Oblique-ISOLatin1 exch definefont pop\n\
/TimesNewRoman findfont\n\
dup length dict begin\n\
  { 1 index /FID ne\n\
	{def}\n\
	{pop pop}\n\
     ifelse\n\
  } forall\n\
  /Encoding ISOLatin1Encoding def\n\
  currentdict\n\
end\n\
/TimesNewRoman-ISOLatin1 exch definefont pop\n\
/TimesNewRoman-Bold findfont\n\
dup length dict begin\n\
  { 1 index /FID ne\n\
	{def}\n\
	{pop pop}\n\
     ifelse\n\
  } forall\n\
  /Encoding ISOLatin1Encoding def\n\
  currentdict\n\
end\n\
/TimesNewRoman-Bold-ISOLatin1 exch definefont pop\n\
/Courier findfont\n\
dup length dict begin\n\
  { 1 index /FID ne\n\
	{def}\n\
	{pop pop}\n\
     ifelse\n\
  } forall\n\
  /Encoding ISOLatin1Encoding def\n\
  currentdict\n\
end\n\
/Courier-ISOLatin1 exch definefont pop\n\
1.000000 1.000000 scale\n\
save\n"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

void CnvToPs::cnv_text( char *to, char *from)
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

void CnvToPs::print_text( char *text, CnvStyle& style, int mode)
{
  char str[1000];

  cnv_text( str, text);

  if ( style.sidebreak && mode & ps_mPrintMode_Pos) {
    if ( page_number[cf] == 0) {
      // First header, no pagebreak
      page_number[cf] = 1;
    }
    else {
      print_pagebreak( 1);
      if ( EVEN(page_number[cf]))
	print_pagebreak( 1);
    }
  }
  else if ( style.pagebreak && mode & ps_mPrintMode_Pos)
    print_pagebreak( 1);
  

  if ( mode & ps_mPrintMode_Pos || mode & ps_mPrintMode_Start) {
    y -= style.top_offset;

    if ( y - style.bottom_offset < ps_cBottomMargin) {
      print_pagebreak( 1);
    }
  }
  else
    y += style.bottom_offset;

  if ( !(mode & ps_mPrintMode_FixX)) {
    if ( style.alignment == cnv_eAlignment_Center) {
      x = ps_cLeftMargin/2 + (ps_cPageWidth - ps_cLeftMargin/2)/ 2 - 
	0.50 * strlen(text) * style.font_size / 2;
      if ( x < ps_cLeftMargin/2)
	x = ps_cLeftMargin/2;
    }
    else
      x = ps_cLeftMargin + style.indentation;
  }
       
  int pmode = mode & 31;
  if ( !conf_pass) {
    if ( strcmp( text, "") != 0) {
      switch ( pmode) {
      case ps_mPrintMode_Pos:
      case ps_mPrintMode_KeepY: {
	// Full path with beginning and end
	fp[cf] <<
"/" <<  style.font << " findfont" << endl <<
style.font_size << " scalefont" << endl <<
"setfont" << endl <<
"newpath" << endl <<
x << " " << y << " moveto" << endl <<
"("<< str << ") show" << endl <<
"closepath" << endl <<
"stroke" << endl;
	break;
      }
      case ps_mPrintMode_Start: {
	// Start new path
	fp[cf] <<
"/" <<  style.font << " findfont" << endl <<
style.font_size << " scalefont" << endl <<
"setfont" << endl <<
"newpath" << endl <<
x << " " << y << " moveto" << endl <<
"("<< str << ") show" << endl;
	break;
      }
      case ps_mPrintMode_Continue: {
	// Continue current path
	fp[cf] <<
"/" <<  style.font << " findfont" << endl <<
style.font_size << " scalefont" << endl <<
"setfont" << endl <<
"("<< str << ") show" << endl;
	break;
      }
      case ps_mPrintMode_End: {
	// Continue and close current path
	fp[cf] <<
"/" <<  style.font << " findfont" << endl <<
style.font_size << " scalefont" << endl <<
"setfont" << endl <<
"("<< str << ") show" << endl <<
"closepath" << endl <<
"stroke" << endl;
	break;
      }
      default: ;
      }
    }
    else {
      switch ( pmode) {
      case ps_mPrintMode_Start: {
	// Start new path
	fp[cf] <<
"newpath" << endl <<
x << " " << y << " moveto" << endl;
	break;
      }
      case ps_mPrintMode_End: {
	// Continue and close current path
	fp[cf] <<
"closepath" << endl <<
"stroke" << endl;
	break;
      }
      default: ;
      }
    }
  }
  y -= style.bottom_offset;
}

void CnvToPs::print_pagebreak( int printnum)
{
  if ( printnum && !conf_pass && cf != ps_eFile_Info) {
    double page_x;
    if ( page_number[cf] == 0) {
      page_number[cf]++;
      return;
    }

    if ( ODD(page_number[cf]))
      page_x = ps_cPageNumX;
    else
      page_x = 40;

    fp[cf] <<
"newpath" << endl <<
10 << " " <<ps_cPageHeight - 20 << " moveto" << endl <<
ps_cPageWidth + 90 << " " << ps_cPageHeight - 20 << " lineto" << endl <<
ps_cPageWidth + 90 << " " << ps_cPageHeight - 20 << " moveto" << endl <<
"closepath" << endl <<
"stroke" << endl <<
"/Helvetica-ISOLatin1 findfont" << endl <<
"10 scalefont" << endl <<
"setfont" << endl <<
"newpath" << endl <<
ps_cPageWidth / 2 - 10 * 0.5 * strlen(previous_chapter) << " " << ps_cPageNumY << " moveto" << endl <<
"(" << previous_chapter << ") show" << endl <<
"closepath" << endl <<
"stroke" << endl <<
"newpath" << endl <<
page_x << " " << ps_cPageNumY << " moveto" << endl <<
"(" << page_number[cf] << ") show" << endl <<
"closepath" << endl <<
"stroke" << endl;
  }
  page_number[cf]++;
  
  if ( !conf_pass) {
    fp[cf] << 
"save" << endl <<
"showpage" << endl <<
"restore" << endl;
  }
  y = ps_cPageHeight - ps_cTopMargin;
}

void CnvToPs::print_content()
{

  cf = ps_eFile_Info;
  ci = ps_eId_Content;
  x = ps_cLeftMargin;
  y = ps_cPageHeight - ps_cTopMargin;

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
    x = ps_cLeftMargin + 30 + content.tab[i].header_level * 5;
    y += cstyle->top_offset + cstyle->bottom_offset;
    print_text( content.tab[i].text, *cstyle, ps_mPrintMode_Start | ps_mPrintMode_FixX);
    x = ps_cLeftMargin + 340;

    fp[cf] <<
x << " " << y + cstyle->bottom_offset << " lineto" << endl <<
x << " " << y + cstyle->bottom_offset << " moveto" << endl <<
"closepath" << endl <<
"stroke" << endl <<
"grestore" << endl;

    print_text( page_str, *cstyle, ps_mPrintMode_KeepY | ps_mPrintMode_FixX);
  }
  print_pagebreak(0);
  if ( ODD(page_number[ps_eFile_Info]))
    print_pagebreak(0);
}

CnvToPs::~CnvToPs()
{
}

void CnvToPs::close()
{
  cf = ps_eFile_Body;
  print_pagebreak( 1);
  cf = ps_eFile_Info;
  print_content();
  if ( !conf_pass) {
    fp[ps_eFile_Info].close();
    fp[ps_eFile_Body].close();
  }

  char cmd[256];
  // Concatenate files
  sprintf( cmd, "cat %s >> %s", filename[ps_eFile_Body], 
	   filename[ps_eFile_Info]);
  system( cmd);
}


void CnvToPs::print_horizontal_line()
{
  y -= 3;
  if ( !conf_pass) {
    fp[cf] <<
"newpath" << endl <<
ps_cLeftMargin - 50 << " " << y << " moveto" << endl <<
ps_cPageWidth << " " << y << " lineto" << endl <<
ps_cPageWidth << " " << y << " moveto" << endl <<
"closepath" << endl <<
"stroke" << endl;
  }
  y -= 3;
}

int CnvToPs::print_image( char *filename)
{
  ImlibImage *image;
  Pixmap pixmap;
  pwr_tFileName fname;
  int sts;
  int width, height;
  unsigned char *rgb;
  unsigned char transp[3] = {255,0,255};
  int i, j;
  int grey;
  double scalex = 0.71;
  double scaley = 0.78;
	
  x = ps_cLeftMargin;

  strcpy( fname, "$pwr_doc/help/");
  strcat( fname, filename);
  dcli_translate_filename( fname, fname);

  sts = cnv_get_image( fname, &image, &pixmap);
  if ( EVEN(sts)) return 0;

  width = image->rgb_width;
  height = image->rgb_height;

  if ( width * scalex  > ps_cPageWidth - ps_cLeftMargin) {
    x = ps_cPageWidth - width * scalex;
    if ( x < 50) {
      double scale_factor = (ps_cPageWidth - 50) / (width * scalex);
      x = 50;
      scalex = scalex * scale_factor;
      scaley = scaley * scale_factor;
    }
  }

  if ( y - height * scaley + 20 < ps_cBottomMargin)
    print_pagebreak( 1);

  if ( !conf_pass) {
    fp[cf] <<
"save" << endl <<
scalex * width << " " << scaley * height << " scale" << endl <<
"/oneline " << width << " string def" << endl <<
"/drawimage {" << endl <<
" " << width << " " << height << " 8 [" << width << " 0 0 -" << height << " 0 " << height << "]" << endl <<
" { currentfile oneline readhexstring pop } image" << endl <<
"} def" << endl <<
x/scalex/width << " " << (y - height*scaley)/scaley/height << " translate" << endl <<
"drawimage" << endl;

    fp[cf].flags( (fp[cf].flags() & ~ios_base::dec) | ios_base::hex | ios_base::uppercase);
    fp[cf].fill('0');
    rgb = image->rgb_data;
    j = 0;
    for ( i = 0; i < image->rgb_height * image->rgb_width * 3; i+=3) {
      if ( *rgb == transp[0] && *(rgb+1) == transp[1] && *(rgb+2) == transp[2]) {
	grey = 255;
      }
      else {
	grey = (int) ((0.0 + *rgb + *(rgb+1) + *(rgb+2)) / 3 + 0.5);
      }
      rgb += 3;
      fp[cf].width(2);
      fp[cf] << grey;
      if ( ++j >= 40) {
	j = 0;
	fp[cf] << endl;
      }
    }

    fp[cf] << endl <<
"restore" << endl;
    fp[cf].flags( ((fp[cf].flags() & ~ios_base::hex) & ~ios_base::uppercase) | ios_base::dec);

    cnv_free_image( image, pixmap);
  }
  y -= height * scaley;

  return 1;
}

void CnvToPs::set_pageheader( char *text)
{
  strcpy( previous_chapter, current_chapter);
  strcpy( current_chapter, text);
}

void CnvToPs::print_h1( char *text, int hlevel, char *subject)
{
  char hnum[40];

  if ( cf == ps_eFile_Info)
    return;

  if ( ci == ps_eId_Chapter) {
    set_pageheader( text);
  }

  if ( style[ci].h1.display_number) {
    if ( hlevel < 0)
      hlevel = 0;
    if ( hlevel > ps_cMaxLevel - 1)
      hlevel = ps_cMaxLevel - 1;
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
    x = ps_cLeftMargin - 50;
    print_text( hnum, style[ci].h1, ps_mPrintMode_Pos | ps_mPrintMode_FixX);
    x = ps_cLeftMargin;
    print_text( text, style[ci].h1, ps_mPrintMode_KeepY | ps_mPrintMode_FixX);
  }
  else 
    print_text( text, style[ci].h1);
  
  if ( conf_pass) {
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

void CnvToPs::print_h2( char *text)
{
  print_text( text, style[ci].h2);
}

void CnvToPs::print_h3( char *text)
{
  print_text( text, style[ci].h3);
}

void CnvToPs::open()
{
  y = ps_cPageHeight - ps_cTopMargin;
  if ( !conf_pass) {
    fp[ps_eFile_Info].open( filename[ps_eFile_Info]);
    fp[ps_eFile_Body].open( filename[ps_eFile_Body]);
  }
  if ( !conf_pass)
    fp[cf] << 
      ps_cHead << endl;
  for ( int i = 1; i < ps_cMaxLevel; i++)
    header_number[i] = 0;
}

void CnvToPs::incr_headerlevel()
{
  ci++;
  if ( ci > ps_eId_TopicL3)
    ci = ps_eId_TopicL3;
  if ( ci < ps_eId_TopicL1)
    ci = ps_eId_TopicL1;
      
  header_number[ci-(int)ps_eId_Chapter] = 0;
}

void CnvToPs::decr_headerlevel()
{
  ci--;
  if ( ci < ps_eId_TopicL1)
    ci = ps_eId_TopicL1;
}

void CnvToPs::reset_headernumbers( int level)
{
      for ( int i = level; i < ps_cMaxLevel; i++)
	header_number[i] = 0;
}
