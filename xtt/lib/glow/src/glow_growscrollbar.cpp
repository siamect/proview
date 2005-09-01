/* 
 * Proview   $Id: glow_growscrollbar.cpp,v 1.3 2005-09-01 14:57:54 claes Exp $
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

#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growscrollbar.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowScrollBar::GrowScrollBar( GlowCtx *glow_ctx, char *name, double x, double y, 
		double w, double h, glow_eDir dir, glow_eDrawType border_d_type, int line_w, 
		glow_mDisplayLevel display_lev,
		glow_eDrawType fill_d_type, glow_eDrawType bar_d_type, int nodraw) : 
		GrowRect(glow_ctx,name,x,y,w,h,border_d_type,line_w,0,
		display_lev,1,0,0,fill_d_type,nodraw), 
		max_value(100),min_value(0),bar_value(35),bar_length(10),
		user_data(0),direction(dir), movement_active(0), start_pos(0), start_value(0),
		callback_userdata(0), value_changed_cb(0), bar_color(bar_d_type)
{
  if ( !nodraw)
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
}

GrowScrollBar::~GrowScrollBar()
{
  if ( ctx->nodraw) return;
  erase();
  nav_erase();
}

void GrowScrollBar::save( ofstream& fp, glow_eSaveMode mode) 
{ 
}

void GrowScrollBar::open( ifstream& fp)
{
}

void GrowScrollBar::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowScrollBar::draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;

  idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;
  int x0, y0, width, height;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  glow_eDrawType fdrawtype = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
							     highlight, (GrowNode *)colornode, 1);
  glow_eDrawType bdrawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);
  glow_eDrawType shift_drawtype;

  glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, fdrawtype);
  if ( shadow) {
    shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( fill_drawtype, 2, 0); // Dark
    glow_draw_line( ctx, ll_x+1, ll_y+1, ll_x+1, ur_y-1, shift_drawtype, 0, 0);
    glow_draw_line( ctx, ll_x+1, ll_y+1, ur_x-1, ll_y+1, shift_drawtype, 0, 0);
    shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( fill_drawtype, -2, 0); // Light
    glow_draw_line( ctx, ll_x+1, ur_y-1, ur_x-1, ur_y-1, shift_drawtype, 0, 0);
    glow_draw_line( ctx, ur_x-1, ll_y+1, ur_x-1, ur_y-1, shift_drawtype, 0, 0);
  }
 
  if ( max_value != min_value) {

    switch ( direction) {
    case glow_eDir_Vertical:
      height = int( bar_length / (max_value - min_value) 
		* (ur_y - ll_y));
      height = max( 0, min( height, ur_y - ll_y));
      width = ur_x - ll_x;
      x0 = ll_x;
      y0 = ll_y + int( (bar_value - min_value) / (max_value - min_value) * (ur_y - ll_y));
      break;
    case glow_eDir_Horizontal:
      width = int( bar_length / (max_value - min_value) * 
		(ur_x - ll_x));
      width = max( 0, min( width, ur_x - ll_x));
      height = ur_y - ll_y;
      x0 = ll_x + int( (bar_value - min_value) / (max_value - min_value) * (ur_x - ll_x));
      y0 = ll_y;
      break;
    }

    glow_draw_fill_rect( ctx, x0, y0, width, height, bar_color);
    if ( shadow) {
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( bar_color, -2, 0); // Light
      glow_draw_line( ctx, x0+1, y0+1, x0+1, y0+height-1, shift_drawtype, 0, 0);
      glow_draw_line( ctx, x0+1, y0+1, x0+width-1, y0+1, shift_drawtype, 0, 0);
      shift_drawtype = ((GrowCtx *)ctx)->shift_drawtype( bar_color, 2, 0); // Dark
      glow_draw_line( ctx, x0+1, y0+height-1, x0+width-1, y0+height-1, shift_drawtype, 0, 0);
      glow_draw_line( ctx, x0+width-1, y0+1, x0+width-1, y0+height-1, shift_drawtype, 0, 0);
    }
    glow_draw_rect( ctx, x0, y0, width, height, bdrawtype, idx, 0);
  }

  glow_draw_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, bdrawtype, idx, 0);
}

void GrowScrollBar::erase( GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;

  idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y) - ctx->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x) - ctx->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y) - ctx->offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  ctx->set_draw_buffer_only();
  glow_draw_rect_erase( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, idx);
  glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
  ctx->reset_draw_buffer_only();
}

void GrowScrollBar::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  glow_eDrawType drawtype;
  int idx;
  idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  drawtype = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
		 0, (GrowNode *)colornode, 1);
  glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			   drawtype);

  drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 0, (GrowNode *)colornode, 0);

  if ( max_value != min_value) {
  }
  glow_draw_nav_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	drawtype, idx, 0);
}

void GrowScrollBar::nav_erase( GlowTransform *t, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  glow_draw_nav_rect_erase( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			    idx);
  glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			   glow_eDrawType_LineErase);
}

double GrowScrollBar::set_value( double value, double length) 
{
  bar_value = value; 
  if ( length != 0)
    bar_length = length;

  if ( bar_value < min_value)
    bar_value = min_value;
  if ( bar_value > max_value - bar_length)
    bar_value = max_value - bar_length;

  if ( !fill) 
    erase(); 

  draw();
  return bar_value;
}

void GrowScrollBar::set_range( double min, double max)
{ 
  max_value = max;
  min_value = min;
  draw();
}

void GrowScrollBar::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
}

void GrowScrollBar::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    break;
  }
  }  
}

int GrowScrollBar::event_handler( glow_eEvent event, int x, int y, double fx,
	double fy)
{
  double ll_x, ur_x, ll_y, ur_y;

  ll_x = min( ll.x, ur.x);
  ur_x = max( ll.x, ur.x);
  ll_y = min( ll.y, ur.y);
  ur_y = max( ll.y, ur.y);

  if ( !movement_active &&
       !( ll_x <= fx && fx <= ur_x &&
       ll_y <= fy && fy <= ur_y))
    return 0;

  if ( event ==  glow_eEvent_ButtonMotion &&
       !movement_active)
    return 0;

  if ( min_value == max_value)
    return 1;

  switch ( event){
  case glow_eEvent_MB1Down:
    switch ( direction) {
    case glow_eDir_Vertical:
      if ( ll_y + (bar_value - min_value) / (max_value - min_value) * (ur_y - ll_y) <= fy &&
	   fy <= ll_y + (bar_value - min_value + bar_length) / (max_value - min_value) * (ur_y - ll_y)) {
	start_pos = fy;
	start_value = bar_value;
	movement_active = 1;
	glow_draw_set_click_sensitivity( ctx, glow_mSensitivity_MB1Press);
      }
      break;
    case glow_eDir_Horizontal:
      if ( ll_x + (bar_value - min_value) / (max_value - min_value) * (ur_x - ll_x) <= fx &&
	   fx <= ll_x + (bar_value - min_value + bar_length) / (max_value - min_value) * (ur_x - ll_x)) {
	start_pos = fx;
	start_value = bar_value;
	movement_active = 1;
	glow_draw_set_click_sensitivity( ctx, glow_mSensitivity_MB1Press);
      }
      break;
    }
    break;
  case glow_eEvent_MB1Up:
    movement_active = 0;
    break;
  case glow_eEvent_ButtonMotion:
    if ( movement_active) {
      if ( bar_length >= max_value - min_value)
	break;
      switch ( direction) {
      case glow_eDir_Vertical:
	bar_value = start_value + (fy - start_pos) / (ur_y - ll_y) * (max_value - min_value);
	break;
      case glow_eDir_Horizontal:
	bar_value = start_value + (fx - start_pos) / (ur_x - ll_x) * (max_value - min_value);
	break;
      }
      if ( bar_value < min_value) 
	bar_value = min_value;
      if ( bar_value > max_value - bar_length)
	bar_value = max_value - bar_length;
      (value_changed_cb)( callback_userdata, bar_value);
    }
    break;
  default: ;
  }
  return 1;
}


























