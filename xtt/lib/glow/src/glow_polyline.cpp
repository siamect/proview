/* 
 * Proview   $Id: glow_polyline.cpp,v 1.5 2008-10-31 12:51:36 claes Exp $
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


#include <iostream>
#include <stdlib.h>
#include "glow_polyline.h"
#include "glow_draw.h"

GlowPolyLine::GlowPolyLine( GrowCtx *glow_ctx, glow_sPoint *pointarray, 
		int point_cnt, glow_eDrawType d_type,
		int line_w, int fix_line_w, int line_fill, 
		int closed) : 
	ctx(glow_ctx), a_points(10,10), points(0),
	draw_type(d_type), line_width(line_w), fix_line_width(fix_line_w),
	fill(line_fill), closed_line(closed)
{
  int i;

  points = (glow_sPointX *) calloc( point_cnt, sizeof(glow_sPointX));
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
  points = (glow_sPointX *) calloc( a_points.a_size, sizeof(glow_sPointX));
  
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

  if ( a_points.size())
    a_points.delete_all();

  for (;;)
  {
    if ( !fp.good()) {
      fp.clear();
      fp.getline( dummy, sizeof(dummy));
      printf( "** Read error GlowPolyLine: \"%d %s\"\n", type, dummy);      
    }

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
  if ( points)
    free((char *)points);
  points = (glow_sPointX *) calloc( a_points.a_size, sizeof(glow_sPointX));
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

void GlowPolyLine::add_points( void *pos, glow_sPoint *pointarray, 
	int point_cnt, int highlight, int hot)
{
  int i;

  if ( points)
  {
    // erase( pos, hot, NULL);
    // nav_erase( pos, NULL);
    free( (char *)points);
  }
  points = (glow_sPointX *) calloc( a_points.a_size + point_cnt, sizeof(glow_sPointX));
  for ( i = 0; i < point_cnt; i++)
  {
    GlowPoint *p = new GlowPoint(ctx, pointarray[i].x, pointarray[i].y);
    a_points.insert( p);
  }
  zoom();
  nav_zoom();
  // draw( pos, highlight, hot, NULL);
  // nav_draw( pos, highlight, NULL);
}
