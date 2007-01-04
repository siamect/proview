/* 
 * Proview   $Id: flow_line.cpp,v 1.6 2007-01-04 07:53:35 claes Exp $
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
#include "flow_line.h"
#include "flow_draw.h"

void FlowLine::zoom()
{
  p1.zoom();
  p2.zoom();
}

void FlowLine::nav_zoom()
{
  p1.nav_zoom();
  p2.nav_zoom();
}

void FlowLine::print_zoom()
{
  p1.print_zoom();
  p2.print_zoom();
}

void FlowLine::traverse( int x, int y)
{
  p1.traverse( x, y);
  p2.traverse( x, y);
}

void FlowLine::print( void *pos, void *node, int highlight)
{
  if ( p1.print_z_x == p2.print_z_x && p1.print_z_y == p2.print_z_y)
    return;
  double idx = ctx->print_zoom_factor / ctx->base_zoom_factor * 
	line_width;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->print_ps->line( p1.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	p1.print_z_y + ((FlowPoint *)pos)->print_z_y, 
	p2.print_z_x + ((FlowPoint *)pos)->print_z_x, 
	p2.print_z_y + ((FlowPoint *)pos)->print_z_y,
	draw_type, idx, highlight);
}

void FlowLine::save( ofstream& fp, flow_eSaveMode mode)
{
  fp << int(flow_eSave_Line) << endl;
  fp << int(flow_eSave_Line_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Line_line_width) << FSPACE << line_width << endl;
  fp << int(flow_eSave_Line_p1) << endl;
  p1.save( fp, mode);
  fp << int(flow_eSave_Line_p2) << endl;
  p2.save( fp, mode);
  fp << int(flow_eSave_End) << endl;
}

void FlowLine::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Line: break;
      case flow_eSave_Line_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_Line_line_width: fp >> line_width; break;
      case flow_eSave_Line_p1: p1.open( fp); break;
      case flow_eSave_Line_p2: p2.open( fp); break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowLine:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowLine::draw( void *pos, int hightlight, int hot, void *node)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->line( ctx, p1.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p1.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	p2.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p2.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y,
	draw_type, idx, hightlight);
}

void FlowLine::erase( void *pos, int hot, void *node)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->line_erase( ctx, p1.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p1.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	p2.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x, 
	p2.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, idx);
}

void FlowLine::nav_draw( void *pos, int highlight, void *node)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->nav_line( ctx, 
	p1.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p1.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	p2.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p2.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y,
	draw_type, idx, highlight);
}

void FlowLine::nav_erase( void *pos, void *node)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * line_width - 1);
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->nav_line_erase( ctx,
	p1.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p1.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	p2.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p2.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y,
	idx);
}

int FlowLine::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  FlowPoint *p;

  p = (FlowPoint *) pos;
  int x1 = p1.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
  int x2 = p2.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
  int y1 = p1.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y;
  int y2 = p2.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y;

  if ((x1 == x2 && y1 < y2 && 		// Vertical
       abs( x1 - x) < 3 && 
       y1 < y && y < y2)
	||
      (x1 == x2 && y1 > y2 && 		// Vertical
       abs( x1 - x) < 3 && 
       y2 < y && y < y1)
	||
      (y1 == y2 && x1 < x2 &&		// Horizontal
       abs( y1 - y) < 3 && 
       x1 < x && x < x2)
        ||
      (y1 == y2 && x1 > x2 &&		// Horizontal
       abs( y1 - y) < 3 && 
       x2 < x && x < x1))
  {
//    cout << "Event handler: Hit in line" << endl;
    return 1;
  }  
  else if (( !(x1 == x2 || y1 == y2) && x1 < x2 && x1 <= x && x <= x2 &&
             fabs(y - 1.0 * (y2-y1)/(x2-x1) * x - y1 + 1.0 * (y2-y1)/(x2-x1) * x1) < 3)
           ||
           ( !(x1 == x2 || y1 == y2) && x1 > x2 && x2 <= x && x <= x1 &&
             fabs(y - 1.0 * (y2-y1)/(x2-x1) * x - y1 + 1.0 * (y2-y1)/(x2-x1) * x1) < 3))
  {
//    cout << "Event handler: Hit in line" << endl;
    return 1;
  }
  else
    return 0;
}

void FlowLine::get_borders( double pos_x, double pos_y,
	double *x_right, double *x_left, double *y_high, double *y_low, 
	void *node)
{
  if ( pos_x + p1.x < *x_left)
    *x_left = pos_x + p1.x;
  if ( pos_x + p2.x < *x_left)
    *x_left = pos_x + p2.x;
  if ( pos_x + p1.x > *x_right)
    *x_right = pos_x + p1.x;
  if ( pos_x + p2.x > *x_right)
    *x_right = pos_x + p2.x;
  if ( pos_y + p1.y < *y_low)
    *y_low = pos_y + p1.y;
  if ( pos_y + p2.y < *y_low)
    *y_low = pos_y + p2.y;
  if ( pos_y + p1.y > *y_high)
    *y_high = pos_y + p1.y;
  if ( pos_y + p2.y > *y_high)
    *y_high = pos_y + p2.y;

}

void FlowLine::move( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p1.x = x1;
  p1.y = y1;
  p2.x = x2;
  p2.y = y2;
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void FlowLine::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p1.x += delta_x;
  p1.y += delta_y;
  p2.x += delta_x;
  p2.y += delta_y;
  zoom();
  nav_zoom();

  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

ostream& operator<< ( ostream& o, const FlowLine l)
{
  o << 
  '(' << l.p1.x << ',' << l.p1.y << ')' << 
  '(' << l.p2.x << ',' << l.p2.y << ')' << 
  '[' << l.p1.z_x << ',' << l.p1.z_y << ']' <<
  '[' << l.p2.z_x << ',' << l.p2.z_y << ']' ;
  return o;
}

