/* 
 * Proview   $Id: glow_polyline.cpp,v 1.2 2005-09-01 14:57:54 claes Exp $
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
#include <stdlib.h>
#include "glow_polyline.h"
#include "glow_draw.h"

GlowPolyLine::GlowPolyLine( GlowCtx *glow_ctx, glow_sPoint *pointarray, 
		int point_cnt, glow_eDrawType d_type,
		int line_w, int fix_line_w, int line_fill, 
		int closed) : 
	ctx(glow_ctx), a_points(10,10), points(0),
	draw_type(d_type), line_width(line_w), fix_line_width(fix_line_w),
	fill(line_fill), closed_line(closed)
{
  int i;

  points = (XPoint *) calloc( point_cnt, sizeof(XPoint));
  for ( i = 0; i < point_cnt; i++)
  {
    GlowPoint *p = new GlowPoint(ctx, pointarray[i].x, pointarray[i].y);
    a_points.insert( p);
  }
}

GlowPolyLine::~GlowPolyLine()
{
  free( (char *)points);
}

GlowPolyLine::GlowPolyLine( const GlowPolyLine& c)
{
  memcpy( this, &c, sizeof(c));

  a_points.new_array( c.a_points);
  a_points.copy_from( c.a_points);
  points = (XPoint *) calloc( a_points.a_size, sizeof(XPoint));
  
}

void GlowPolyLine::zoom()
{
  a_points.zoom();
}

void GlowPolyLine::nav_zoom()
{
  a_points.nav_zoom();
}

void GlowPolyLine::print_zoom()
{
  a_points.print_zoom();
}

void GlowPolyLine::traverse( int x, int y)
{
  a_points.traverse( x, y);
}

void GlowPolyLine::print( void *pos, void *node)
{
  GlowPoint *p1;
  GlowPoint *p2;
  int i;
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
	line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  for ( i = 0; i < a_points.a_size - 1; i++)
  {
    p1 = (GlowPoint *)a_points[i];
    p2 = (GlowPoint *)a_points[i+1];
    ctx->print_ps->line( p1->print_z_x + ((GlowPoint *)pos)->print_z_x, 
	p1->print_z_y + ((GlowPoint *)pos)->print_z_y, 
	p2->print_z_x + ((GlowPoint *)pos)->print_z_x, 
	p2->print_z_y + ((GlowPoint *)pos)->print_z_y,
	draw_type, idx);
  }
}

void GlowPolyLine::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_PolyLine) << endl;
  fp << int(glow_eSave_PolyLine_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_PolyLine_line_width) << FSPACE << line_width << endl;
  fp << int(glow_eSave_PolyLine_fill) << FSPACE << fill << endl;
  fp << int(glow_eSave_PolyLine_closed_line) << FSPACE << closed_line << endl;
  fp << int(glow_eSave_PolyLine_a_points) << endl;
  a_points.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GlowPolyLine::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_PolyLine: break;
      case glow_eSave_PolyLine_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_PolyLine_line_width: fp >> line_width; break;
      case glow_eSave_PolyLine_fill: fp >> fill; break;
      case glow_eSave_PolyLine_closed_line: fp >> closed_line; break;
      case glow_eSave_PolyLine_a_points: a_points.open( ctx, fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowPolyLine:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  points = (XPoint *) calloc( a_points.a_size, sizeof(XPoint));
}

void GlowPolyLine::draw( void *pos, int highlight, int hot, void *node)
{
  int i;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  XPoint *point_p = points;
  for ( i = 0; i < a_points.a_size; i++)
  {
    point_p->x = ((GlowPoint *)a_points[i])->z_x + ((GlowPoint *)pos)->z_x - 
	ctx->offset_x;
    point_p->y = ((GlowPoint *)a_points[i])->z_y + ((GlowPoint *)pos)->z_y - 
	ctx->offset_y;
    point_p++;
  }
  if ( !fill)
    glow_draw_polyline( ctx, points, a_points.a_size, draw_type, idx, highlight);
  else
    glow_draw_fill_polyline( ctx, points, a_points.a_size, draw_type, highlight);
}

void GlowPolyLine::erase( void *pos, int hot, void *node)
{
  int i;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  XPoint *point_p = points;
  for ( i = 0; i < a_points.a_size; i++)
  {
    point_p->x = ((GlowPoint *)a_points[i])->z_x + ((GlowPoint *)pos)->z_x - 
	ctx->offset_x;
    point_p->y = ((GlowPoint *)a_points[i])->z_y + ((GlowPoint *)pos)->z_y - 
	ctx->offset_y;
    point_p++;
  }
  if ( !fill)
    glow_draw_polyline_erase( ctx, points, a_points.a_size, idx);
  else
    glow_draw_fill_polyline( ctx, points, a_points.a_size, 
		glow_eDrawType_LineErase, 0);
}

void GlowPolyLine::nav_draw( void *pos, int highlight, void *node)
{
  int i;
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  XPoint *point_p = points;
  for ( i = 0; i < a_points.a_size; i++)
  {
    point_p->x = ((GlowPoint *)a_points[i])->nav_z_x + ((GlowPoint *)pos)->nav_z_x - 
	ctx->nav_offset_x;
    point_p->y = ((GlowPoint *)a_points[i])->nav_z_y + ((GlowPoint *)pos)->nav_z_y - 
	ctx->nav_offset_y;
    point_p++;
  }
  glow_draw_nav_polyline( ctx, points, a_points.a_size, draw_type, idx, highlight);
}

void GlowPolyLine::nav_erase( void *pos, void *node)
{
  int i;
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  XPoint *point_p = points;
  for ( i = 0; i < a_points.a_size; i++)
  {
    point_p->x = ((GlowPoint *)a_points[i])->nav_z_x + ((GlowPoint *)pos)->nav_z_x - 
	ctx->nav_offset_x;
    point_p->y = ((GlowPoint *)a_points[i])->nav_z_y + ((GlowPoint *)pos)->nav_z_y - 
	ctx->nav_offset_y;
    point_p++;
  }
  glow_draw_nav_polyline_erase( ctx, points, a_points.a_size, idx);
}

int GlowPolyLine::event_handler( void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;
  int x1, x2, y1, y2;
  int i;

  p = (GlowPoint *) pos;
  for ( i = 0; i < a_points.a_size -1; i++)
  {
    x1 = ((GlowPoint *)a_points[i])->z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
    x2 = ((GlowPoint *)a_points[i+1])->z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
    y1 = ((GlowPoint *)a_points[i])->z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y;
    y2 = ((GlowPoint *)a_points[i+1])->z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y;

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
             abs(y - 1.0 * (y2-y1)/(x2-x1) * x - y1 + 1.0 * (y2-y1)/(x2-x1) * x1) < 3)
           ||
           ( !(x1 == x2 || y1 == y2) && x1 > x2 && x2 <= x && x <= x1 &&
             abs(y - 1.0 * (y2-y1)/(x2-x1) * x - y1 + 1.0 * (y2-y1)/(x2-x1) * x1) < 3))
    {
//      cout << "Event handler: Hit in line" << endl;
      return 1;
    }
  }
  return 0;
}

void GlowPolyLine::get_borders( double pos_x, double pos_y,
	double *x_right, double *x_left, double *y_high, double *y_low, 
	void *node)
{
  int i;

  for ( i = 0; i < a_points.a_size - 1; i++)
  {
    if ( pos_x + ((GlowPoint *)a_points[i])->x < *x_left)
      *x_left = pos_x + ((GlowPoint *)a_points[i])->x;
    if ( pos_x + ((GlowPoint *)a_points[i+1])->x < *x_left)
      *x_left = pos_x + ((GlowPoint *)a_points[i+1])->x;
    if ( pos_x + ((GlowPoint *)a_points[i])->x > *x_right)
      *x_right = pos_x + ((GlowPoint *)a_points[i])->x;
    if ( pos_x + ((GlowPoint *)a_points[i+1])->x > *x_right)
      *x_right = pos_x + ((GlowPoint *)a_points[i+1])->x;
    if ( pos_y + ((GlowPoint *)a_points[i])->y < *y_low)
      *y_low = pos_y + ((GlowPoint *)a_points[i])->y;
    if ( pos_y + ((GlowPoint *)a_points[i+1])->y < *y_low)
      *y_low = pos_y + ((GlowPoint *)a_points[i+1])->y;
    if ( pos_y + ((GlowPoint *)a_points[i])->y > *y_high)
      *y_high = pos_y + ((GlowPoint *)a_points[i])->y;
    if ( pos_y + ((GlowPoint *)a_points[i+1])->y > *y_high)
      *y_high = pos_y + ((GlowPoint *)a_points[i+1])->y;
  }
}

void GlowPolyLine::move( void *pos, glow_sPoint *pointarray, int point_cnt,
	int highlight, int hot)
{
  int i;

  if ( points)
  {
    erase( pos, hot, NULL);
    nav_erase( pos, NULL);
    free( (char *)points);
  }
  points = (XPoint *) calloc( point_cnt, sizeof(XPoint));
  for ( i = 0; i < point_cnt; i++)
  {
    if ( a_points.a_size < i)
    {
      ((GlowPoint *)a_points[i])->x = pointarray[i].x;
      ((GlowPoint *)a_points[i])->y = pointarray[i].y;
    }
    else
    {
      GlowPoint *p = new GlowPoint(ctx, pointarray[i].x, pointarray[i].y);
      a_points.insert( p);
    }
  }
  for ( ; i < a_points.a_size; i++)
  {
    delete a_points[i];
  }
  a_points.a_size = point_cnt;

  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void GlowPolyLine::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  int i;

  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  for ( i = 0; i < a_points.a_size; i++)
  {
    ((GlowPoint *)a_points[i])->x += delta_x;
    ((GlowPoint *)a_points[i])->y += delta_y;
  }
  zoom();
  nav_zoom();

  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

ostream& operator<< ( ostream& o, const GlowPolyLine l)
{
#if 0
  o << 
  '(' << l.p1.x << ',' << l.p1.y << ')' << 
  '(' << l.p2.x << ',' << l.p2.y << ')' << 
  '[' << l.p1.z_x << ',' << l.p1.z_y << ']' <<
  '[' << l.p2.z_x << ',' << l.p2.z_y << ']' ;
#endif
  return o;
}

void GlowPolyLine::add_points( void *pos, glow_sPoint *pointarray, 
	int point_cnt, int highlight, int hot)
{
  int i;

  if ( points)
  {
    erase( pos, hot, NULL);
    nav_erase( pos, NULL);
    free( (char *)points);
  }
  points = (XPoint *) calloc( a_points.a_size + point_cnt, sizeof(XPoint));
  for ( i = 0; i < point_cnt; i++)
  {
    GlowPoint *p = new GlowPoint(ctx, pointarray[i].x, pointarray[i].y);
    a_points.insert( p);
  }
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}
