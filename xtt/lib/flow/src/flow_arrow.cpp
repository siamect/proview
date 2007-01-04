/* 
 * Proview   $Id: flow_arrow.cpp,v 1.6 2007-01-04 07:53:34 claes Exp $
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


#include <iostream.h>
#include <float.h>
#include <math.h>

#include "flow.h"
#include "flow_arrow.h"
#include "flow_draw.h"

FlowArrow::FlowArrow( FlowCtx *flow_ctx, double x1, double y1, double x2, 
	double y2, double w, double l, flow_eDrawType d_type) : 
	ctx(flow_ctx), p_dest(flow_ctx,x2,y2),
	arrow_width(w), arrow_length(l), draw_type(d_type), line_width(1)
{
  double p1_x, p1_y, p2_x, p2_y;

  if ( fabs( x2 - x1) < DBL_EPSILON)
  {
    if ( y1 > y2)
    {
      p1_x = x2 + w/2;
      p1_y = y2 + l;
      p2_x = x2 - w/2;
      p2_y = y2 + l;
    }
    else
    {
      p1_x = x2 + w/2;
      p1_y = y2 - l;
      p2_x = x2 - w/2;
      p2_y = y2 - l;
    }
  }
  else if ( fabs( y2 - y1) < DBL_EPSILON)
  {
    if ( x1 > x2)
    {
      p1_x = x2 + l;
      p1_y = y2 + w/2;
      p2_x = x2 + l;
      p2_y = y2 - w/2;
    }
    else
    {
      p1_x = x2 - l;
      p1_y = y2 - w/2;
      p2_x = x2 - l;
      p2_y = y2 + w/2;
    }
  }
  else
  {
    double d = sqrt( (y1-y2)*(y1-y2) + (x1-x2)*(x1-x2));
    p1_x = x2 + (x1-x2)*l/d + (y1-y2)*w/d/2;
    p1_y = y2 + (y1-y2)*l/d - (x1-x2)*w/d/2;
    p2_x = x2 + (x1-x2)*l/d - (y1-y2)*w/d/2;
    p2_y = y2 + (y1-y2)*l/d + (x1-x2)*w/d/2;
  }
  p1 = FlowPoint(flow_ctx, p1_x, p1_y);
  p2 = FlowPoint(flow_ctx, p2_x, p2_y);
}

void FlowArrow::zoom()
{
  p_dest.zoom();
  p1.zoom();
  p2.zoom();
}

void FlowArrow::nav_zoom()
{
  p_dest.nav_zoom();
  p1.nav_zoom();
  p2.nav_zoom();
}

void FlowArrow::print_zoom()
{
  p_dest.print_zoom();
  p1.print_zoom();
  p2.print_zoom();
}

void FlowArrow::traverse( int x, int y)
{
  p_dest.traverse( x, y);
  p1.traverse( x, y);
  p2.traverse( x, y);
}

void FlowArrow::print( void *pos, void *node, int highlight)
{
  double idx = ctx->print_zoom_factor / ctx->base_zoom_factor * 
		line_width;
  idx = MAX( 0.5, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->print_ps->arrow( 
	p_dest.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	p_dest.print_z_y + ((FlowPoint *)pos)->print_z_y,
	p1.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	p1.print_z_y + ((FlowPoint *)pos)->print_z_y,
	p2.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	p2.print_z_y + ((FlowPoint *)pos)->print_z_y,
	draw_type, idx);
}

void FlowArrow::save( ofstream& fp, flow_eSaveMode mode)
{
  fp << int(flow_eSave_Arrow) << endl;
  fp << int(flow_eSave_Arrow_arrow_width) << FSPACE << arrow_width << endl;
  fp << int(flow_eSave_Arrow_arrow_length) << FSPACE << arrow_length << endl;
  fp << int(flow_eSave_Arrow_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Arrow_line_width) << FSPACE << line_width << endl;
  fp << int(flow_eSave_Arrow_p_dest) << endl;
  p_dest.save( fp, mode);
  fp << int(flow_eSave_Arrow_p1) << endl;
  p1.save( fp, mode);
  fp << int(flow_eSave_Arrow_p2) << endl;
  p2.save( fp, mode);
  fp << int(flow_eSave_End) << endl;
}

void FlowArrow::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Arrow: break;
      case flow_eSave_Arrow_arrow_width: fp >> arrow_width; break;
      case flow_eSave_Arrow_arrow_length: fp >> arrow_length; break;
      case flow_eSave_Arrow_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_Arrow_line_width: fp >> line_width; break;
      case flow_eSave_Arrow_p_dest: p_dest.open( fp); break;
      case flow_eSave_Arrow_p1: p1.open( fp); break;
      case flow_eSave_Arrow_p2: p2.open( fp); break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowArrow:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowArrow::draw( void *pos, int highlight, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->arrow( ctx,
	p_dest.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p_dest.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	p1.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p1.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	p2.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p2.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	draw_type, idx, highlight);
}

void FlowArrow::erase( void *pos, int hot, void *node)
{
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->arrow_erase( ctx,
	p_dest.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p_dest.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	p1.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p1.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	p2.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p2.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	idx);
}

void FlowArrow::nav_draw( void *pos, int highlight, void *node)
{
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->nav_arrow( ctx,
	p_dest.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p_dest.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	p1.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p1.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	p2.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p2.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	draw_type, idx, highlight);
}

void FlowArrow::nav_erase( void *pos, void *node)
{
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->nav_arrow_erase( ctx,
	p_dest.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p_dest.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	p1.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p1.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	p2.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p2.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	idx);
}

void FlowArrow::move( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);

  if ( fabs( x2 - x1) < DBL_EPSILON)
  {
    if ( y1 > y2)
    {
      p1.x = x2 + arrow_width/2;
      p1.y = y2 + arrow_length;
      p2.x = x2 - arrow_width/2;
      p2.y = y2 + arrow_length;
    }
    else
    {
      p1.x = x2 + arrow_width/2;
      p1.y = y2 - arrow_length;
      p2.x = x2 - arrow_width/2;
      p2.y = y2 - arrow_length;
    }
  }
  else if ( fabs( y2 - y1) < DBL_EPSILON)
  {
    if ( x1 > x2)
    {
      p1.x = x2 + arrow_length;
      p1.y = y2 + arrow_width/2;
      p2.x = x2 + arrow_length;
      p2.y = y2 - arrow_width/2;
    }
    else
    {
      p1.x = x2 - arrow_length;
      p1.y = y2 - arrow_width/2;
      p2.x = x2 - arrow_length;
      p2.y = y2 + arrow_width/2;
    }
  }
  else
  {
    double d = sqrt( (y1-y2)*(y1-y2) + (x1-x2)*(x1-x2));
    p1.x = x2 + (x1-x2)*arrow_length/d + (y1-y2)*arrow_width/d/2;
    p1.y = y2 + (y1-y2)*arrow_length/d - (x1-x2)*arrow_width/d/2;
    p2.x = x2 + (x1-x2)*arrow_length/d - (y1-y2)*arrow_width/d/2;
    p2.y = y2 + (y1-y2)*arrow_length/d + (x1-x2)*arrow_width/d/2;
  }
  p_dest.x = x2;
  p_dest.y = y2;
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void FlowArrow::shift( void *pos, double delta_x, double delta_y, 
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p_dest.x += delta_x;
  p_dest.y += delta_y;
  p1.x += delta_x;
  p1.y += delta_y;
  p2.x += delta_x;
  p2.y += delta_y;
  zoom();
  nav_zoom();

  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

int FlowArrow::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
    return 0;
}

