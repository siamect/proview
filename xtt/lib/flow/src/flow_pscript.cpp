/* 
 * Proview   $Id: flow_pscript.cpp,v 1.9 2008-10-28 09:29:40 claes Exp $
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

#include "flow_std.h"


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "flow_ctx.h"

FlowPscript::FlowPscript( char *filename, void *flow_ctx, int page_border,
			  int *sts) :
  border(page_border), ctx(flow_ctx), show_red(1)
{
  strcpy( fname, filename);
  file = fopen( fname, "w");
  if ( !file)
  {
    printf( "** Unable to open file\n");
    *sts = 0;
    return;
  }
  fprintf( file, "%%!PS-Adobe-3.0\n\n");

  // Define ISO Latin fonts
  {
    char format[] = "\
/%s findfont\n\
dup length dict begin\n\
  { 1 index /FID ne\n\
	{def}\n\
	{pop pop}\n\
     ifelse\n\
  } forall\n\
  /Encoding ISOLatin1Encoding def\n\
  currentdict\n\
end\n\
/%s exch definefont pop\n";

    fprintf( file, format, "Helvetica", "Helvetica-ISOLatin1");
    fprintf( file, format, "Helvetica-Bold", "Helvetica-Bold-ISOLatin1");
  }
  fprintf( file, "%f %f scale\n\n", 1.0, 1.0);
  fprintf( file, "save\n");

  *sts = 1;
}

FlowPscript::~FlowPscript() 
{
  if ( file)
    fclose( file);
}

int FlowPscript::print_page( double ll_x, double ll_y, double ur_x, double ur_y)
{
  FlowCtx *cx = (FlowCtx *) ctx;

  if ( ur_x - ll_x > ur_y - ll_y)
    cx->print_zoom_factor = 730 / (ur_x - ll_x);
  else
    cx->print_zoom_factor = 730 / (ur_y - ll_y);
  cx->print_zoom();

  offset_x = ll_x * cx->print_zoom_factor;
  offset_y = ur_y * cx->print_zoom_factor;

  if ( ur_x - ll_x > ur_y - ll_y)
  {
    /* Landscape orientation */
    fprintf( file, "%f %f translate\n", (ur_y - ll_y) *
	cx->print_zoom_factor + 40, 20.0);
    fprintf( file, "90 rotate\n");
  }
  else
    fprintf( file, "%f %f translate\n", 40.0, 20.0);

  /* Clip the region */
  fprintf( file, "newpath\n");
  fprintf( file, "%f %f moveto\n", ll_x * cx->print_zoom_factor - offset_x, 
		offset_y - ll_y * cx->print_zoom_factor);
  fprintf( file, "%f %f lineto\n", ll_x * cx->print_zoom_factor - offset_x, 
		offset_y - ur_y * cx->print_zoom_factor);
  fprintf( file, "%f %f lineto\n", ur_x * cx->print_zoom_factor - offset_x, 
		offset_y - ur_y * cx->print_zoom_factor);
  fprintf( file, "%f %f lineto\n", ur_x * cx->print_zoom_factor - offset_x, 
		offset_y - ll_y * cx->print_zoom_factor);
  fprintf( file, "%f %f lineto\n", ll_x * cx->print_zoom_factor - offset_x, 
		offset_y - ll_y * cx->print_zoom_factor);
  fprintf( file, "closepath\n");
  fprintf( file, "clip\n");
  if ( border)
    fprintf( file, "stroke\n");
  fprintf( file, "newpath\n");

  ((FlowCtx *)ctx)->current_print = this;
  ((FlowCtx *)ctx)->print( ll_x, ll_y, ur_x, ur_y);

  fprintf( file, "showpage\n\n");
  return 1;
}

int FlowPscript::rect( double x, double y, double width, double height, flow_eDrawType type, 
	double idx, int highlight)
{
  idx = MAX( 0.5, idx);

  if ( type == flow_eDrawType_LineDashed)
  {
    fprintf( file, "gsave\n");
    fprintf( file, "[%f %f] %d setdash\n", 3 + idx, 3 + idx, 0);    
  }
  else if ( type == flow_eDrawType_LineGray)
  {
    fprintf( file, "gsave\n");
    fprintf( file, "0.5 setgray\n");    
  }
  if ( highlight && show_red) {
    fprintf( file, "gsave\n");
    fprintf( file, "0 1 1 0 setcmykcolor\n");
  }

  setlinewidth( idx);
  fprintf( file, "newpath\n");
  fprintf( file, "%f %f moveto\n", x-offset_x, offset_y-y);
  fprintf( file, "%f %f lineto\n", (x+width)-offset_x, offset_y-y);
  fprintf( file, "%f %f lineto\n", x+width-offset_x, offset_y-(y+height));
  fprintf( file, "%f %f lineto\n", x-offset_x, offset_y-(y+height));
  fprintf( file, "%f %f lineto\n", x-offset_x, offset_y-y);
  fprintf( file, "closepath\n");
  fprintf( file, "stroke\n");

  if ( type == flow_eDrawType_LineDashed || type == flow_eDrawType_LineGray)
    fprintf( file, "grestore\n");
  if ( highlight && show_red)
    fprintf( file, "grestore\n");

  return 1;
}

int FlowPscript::filled_rect( double x, double y, double width, double height, flow_eDrawType type, 
	double idx)
{
  if ( type == flow_eDrawType_LineGray)
  {
    fprintf( file, "gsave\n");
    fprintf( file, "0.5 setgray\n");    
  }

  setlinewidth( idx);
  fprintf( file, "newpath\n");
  fprintf( file, "%f %f moveto\n", x-offset_x, offset_y-y);
  fprintf( file, "%f %f lineto\n", (x+width)-offset_x, offset_y-y);
  fprintf( file, "%f %f lineto\n", x+width-offset_x, offset_y-(y+height));
  fprintf( file, "%f %f lineto\n", x-offset_x, offset_y-(y+height));
  fprintf( file, "%f %f lineto\n", x-offset_x, offset_y-y);
  fprintf( file, "closepath\n");
  fprintf( file, "fill\n");

  if ( type == flow_eDrawType_LineDashed || type == flow_eDrawType_LineGray)
    fprintf( file, "grestore\n");

  return 1;
}

int FlowPscript::arc( double x, double y, double width, double height, int angle1, int angle2,
		flow_eDrawType type, double idx, int highlight)
{
  double r = 0.5*width;
  double pi = 3.14159;

  idx = MAX( 0.5, idx);
  angle1 = angle1 % 360;
  angle2 = angle2 % 360;

  if ( type == flow_eDrawType_LineDashed)
  {
    fprintf( file, "gsave\n");
    fprintf( file, "[%f %f] %d setdash\n", 3 + idx, 3 + idx, 0);    
  }
  else if ( type == flow_eDrawType_LineGray)
  {
    fprintf( file, "gsave\n");
    fprintf( file, "0.5 setgray\n");    
  }
  if ( highlight && show_red) {
    fprintf( file, "gsave\n");
    fprintf( file, "0 1 1 0 setcmykcolor\n");
  }
  setlinewidth( idx);

  fprintf( file, "newpath\n");
  fprintf( file, "%f %f moveto\n", x+0.5*width+r*cos(pi*angle1/180)-offset_x,
		offset_y-(y+0.5*height-0.5*height*sin( pi*angle1/180)));
  if ( fabs( width - height) > 0.05 * width)
    /* Eclipse, draw a line instead */
    fprintf( file, "%f %f lineto\n", 
		x+0.5*width+r*cos(pi*(angle1+angle2)/180)-offset_x,
		offset_y-(y+0.5*height-0.5*height*sin( pi*(angle1+angle2)/180)));
  else
    fprintf( file, "%f %f %f %d %d arc\n", x+0.5*width-offset_x, 
		offset_y-(y+0.5*height),
		r, angle1, angle1+angle2);
  fprintf( file, "%f %f moveto\n", 
		x+0.5*width+r*cos(pi*(angle1+angle2)/180)-offset_x,
		offset_y-(y+0.5*height-r*sin( pi*(angle1+angle2)/180)));
  fprintf( file, "closepath\n");
  fprintf( file, "stroke\n");

  if ( type == flow_eDrawType_LineDashed || type == flow_eDrawType_LineGray)
    fprintf( file, "grestore\n");
  if ( highlight && show_red)
    fprintf( file, "grestore\n");
  return 1;
}

int FlowPscript::line( double x1, double y1, double x2, double y2, flow_eDrawType type, 
	double idx, int highlight)
{
  if ( type == flow_eDrawType_LineDashed)
  {
    fprintf( file, "gsave\n");
    fprintf( file, "[%f %f] %d setdash\n", 3 + idx, 3 + idx, 0);    
  }
  else if ( type == flow_eDrawType_LineGray)
  {
    fprintf( file, "gsave\n");
    fprintf( file, "0.5 setgray\n");    
  }
  if ( highlight && show_red) {
    fprintf( file, "gsave\n");
    fprintf( file, "0 1 1 0 setcmykcolor\n");
  }

  setlinewidth( idx);
  fprintf( file, "newpath\n");
  fprintf( file, "%f %f moveto\n", x1-offset_x, offset_y-y1);
  fprintf( file, "%f %f lineto\n", x2-offset_x, offset_y-y2);
  fprintf( file, "%f %f moveto\n", x2-offset_x, offset_y-y2);
  fprintf( file, "closepath\n");
  fprintf( file, "stroke\n");

  if ( type == flow_eDrawType_LineDashed || type == flow_eDrawType_LineGray)
    fprintf( file, "grestore\n");
  if ( highlight && show_red)
    fprintf( file, "grestore\n");

  return 1;
}

int FlowPscript::text( double x, double y, char *text, int len, flow_eDrawType type, 
	double size)
{
  char font[40];
  char c;
  char *s;

  switch( type)
  {
    case flow_eDrawType_TextHelvetica:
      strcpy( font, "Helvetica-ISOLatin1");
      break;
    case flow_eDrawType_TextHelveticaBold:
      strcpy( font, "Helvetica-Bold-ISOLatin1");
      break;
    default:
      ;
  }

  fprintf( file, "/%s findfont\n", font);
  fprintf( file, "%d scalefont\n", (int)size);
  fprintf( file, "setfont\n");
  fprintf( file, "newpath\n");
  fprintf( file, "%f %f moveto\n", x-offset_x, offset_y-y);
  c = text[len];
  text[len] = 0;
  fprintf( file, "(");
  for ( s = text; *s; s++)
  {
    if ( *s == '(' || *s == ')' || *s == '\\')
      fprintf( file, "%c", '\\');
    fprintf( file, "%c", *s);
  }
  fprintf( file, ") show\n");
  text[len] = c;
  fprintf( file, "closepath\n");
  fprintf( file, "stroke\n");

  return 1;
}

int FlowPscript::pixmap( double x, double y, flow_sPixmapDataElem *data,
	flow_eDrawType type)
{
  int i, j;
  int bit_cnt;
  char *char_p;
  double scale_factor = 0.8;

  fprintf( file, "/picstr %d string def\n", data->width);
  fprintf( file, "%f %f translate\n", x-offset_x, offset_y-y);
  fprintf( file, "%f %f scale\n", scale_factor * data->width, 
		scale_factor * data->height);
  fprintf( file, "%d %d translate\n", 0, -1);
  fprintf( file, "%d %d 8\n", data->width, data->height);
  fprintf( file, "[%d 0 0 %d 0 %d]\n", data->width, -data->height,
	data->height);
  fprintf( file, "{ currentfile picstr readhexstring pop }\n");
  fprintf( file, "image\n\n");
  char_p = data->bits;
  for ( i = 0; i < data->height; i++)
  {
    bit_cnt = 0;
    for ( j = 0; j < data->width; j++)
    {
      if ( bit_cnt == 8)
      {
        bit_cnt = 0;
        char_p++;
      }
      if ( *char_p & ( 1 << bit_cnt))
        fprintf( file , "00");
      else
        fprintf( file , "ff");
      bit_cnt++;
    }
    char_p++;
    fprintf( file, "\n");
  }
  fprintf( file, "\n");
  fprintf( file, "stroke\n");
  fprintf( file, "%d %d translate\n", 0, 1);
  fprintf( file, "%f %f scale\n", 1.0 / (scale_factor * data->width), 
		1.0 / (scale_factor * data->height));
  fprintf( file, "%f %f translate\n", -(x-offset_x), -(offset_y-y));
  return 1;
}

int FlowPscript::arrow( double x1, double y1, double x2, double y2, 
	double x3, double y3, flow_eDrawType type, double idx)
{
  idx = MAX( 0.5, idx);

  if ( type == flow_eDrawType_LineGray)
  {
    fprintf( file, "gsave\n");
    fprintf( file, "0.5 setgray\n");    
  }

  setlinewidth( idx);
  fprintf( file, "newpath\n");
  fprintf( file, "%f %f moveto\n", x1-offset_x, offset_y-y1);
  fprintf( file, "%f %f lineto\n", x2-offset_x, offset_y-y2);
  fprintf( file, "%f %f lineto\n", x3-offset_x, offset_y-y3);
  fprintf( file, "%f %f lineto\n", x1-offset_x, offset_y-y1);
  fprintf( file, "closepath\n");
  fprintf( file, "fill\n");

  if ( type == flow_eDrawType_LineGray)
    fprintf( file, "grestore\n");

  return 1;
}

void FlowPscript::move( double x, double y)
{
  fprintf( file, "%f %f moveto", x, y);
}


void FlowPscript::setlinewidth( double idx)
{
  fprintf( file,"%f setlinewidth\n", idx);
}

