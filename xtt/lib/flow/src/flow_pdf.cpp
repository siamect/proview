/* 
 * Proview   $Id: flow_pdf.cpp,v 1.1 2007-09-25 13:11:00 claes Exp $
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
#include "flow_pdf.h"
#include "cnv_topdf.h"

FlowPdf::FlowPdf( char *filename, void *flow_ctx, int page_border,
			  int *sts) :
  border(page_border), ctx(flow_ctx), show_red(1), topdf(0)
{
  topdf = new CnvToPdf();
  topdf->set_filename( pdf_eFile_Body, filename);
  topdf->set_useoutlines( 0);

  *sts = 1;
}

FlowPdf::~FlowPdf() 
{
  if ( topdf) {
    topdf->close();
    delete topdf;
  }
}

int FlowPdf::print_page( double ll_x, double ll_y, double ur_x, double ur_y)
{
  FlowCtx *cx = (FlowCtx *) ctx;

  if ( ur_x - ll_x > ur_y - ll_y)
    cx->print_zoom_factor = 730 / (ur_x - ll_x);
  else
    cx->print_zoom_factor = 730 / (ur_y - ll_y);
  cx->print_zoom();

  offset_x = ll_x * cx->print_zoom_factor;
  offset_y = ur_y * cx->print_zoom_factor;

#if 0
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
#endif

  ((FlowCtx *)ctx)->current_print = this;

  topdf->set_confpass( true);
  topdf->open();
  ((FlowCtx *)ctx)->print( ll_x, ll_y, ur_x, ur_y);
  //topdf->print_pagebreak(0);
  topdf->set_confpass( false);

  topdf->open();
  ((FlowCtx *)ctx)->print( ll_x, ll_y, ur_x, ur_y);
  //topdf->print_pagebreak(0);

  return 1;
}

int FlowPdf::rect( double x, double y, double width, double height, flow_eDrawType type, 
		   double idx, int highlight)
{
  topdf->draw_rect( idx, x - offset_x, offset_y - y - height, width, height);
  return 1;
}

int FlowPdf::filled_rect( double x, double y, double width, double height, 
			  flow_eDrawType type, double idx)
{
  return 1;
}

int FlowPdf::arc( double x, double y, double width, double height, int angel1, int angel2,
		  flow_eDrawType type, double idx, int highlight)
{
  topdf->draw_arc( idx, x - offset_x, offset_y - y - height, width, height, angel1, angel2);
  return 1;
}

int FlowPdf::text( double x, double y, char *text, int len, flow_eDrawType type, 
		   double size)
{
  char t[500];
  int tlen = len < int(sizeof(t)-1) ? len : sizeof(t)-1;

  strncpy( t, text, tlen);
  t[tlen] = 0;
  int bold = (type == flow_eDrawType_TextHelveticaBold);

  topdf->draw_text( x - offset_x , offset_y - y, t, bold, 0.87 * size);
  return 1;
}

int FlowPdf::pixmap( double x, double y, flow_sPixmapDataElem *data,
	flow_eDrawType type)
{
  return 1;
}

int FlowPdf::line( double x1, double y1, double x2, double y2, 
		   flow_eDrawType type, double idx, int hightlight)
{
  topdf->draw_line( idx, x1 - offset_x, offset_y - y1, x2 - offset_x, offset_y - y2, 
		     type == flow_eDrawType_LineDashed, type == flow_eDrawType_LineGray);
  return 1;
}
  
int FlowPdf::arrow( double x1, double y1, double x2, double y2, 
		    double x3, double y3, flow_eDrawType type, double idx)
{
  topdf->draw_arrow( x1 - offset_x, offset_y - y1, x2 - offset_x, offset_y - y2, 
		     x3 - offset_x, offset_y - y3, type == flow_eDrawType_LineGray);
  
#if 0
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

#endif
  return 1;
}

void FlowPdf::move( double x, double y)
{
  fprintf( file, "%f %f moveto", x, y);
}


void FlowPdf::setlinewidth( double idx)
{
  fprintf( file,"%f setlinewidth\n", idx);
}

