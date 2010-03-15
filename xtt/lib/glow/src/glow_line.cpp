/* 
 * Proview   $Id: glow_line.cpp,v 1.5 2008-10-31 12:51:36 claes Exp $
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


#include <stdlib.h>
#include <iostream>
#include "glow_line.h"
#include "glow_draw.h"
#include "glow_growctx.h"

void GlowLine::zoom()
{
  p1.zoom();
  p2.zoom();
}

void GlowLine::nav_zoom()
{
  p1.nav_zoom();
  p2.nav_zoom();
}

void GlowLine::print_zoom()
{
  p1.print_zoom();
  p2.print_zoom();
}

void GlowLine::traverse( int x, int y)
{
  p1.traverse( x, y);
  p2.traverse( x, y);
}

void GlowLine::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Line) << endl;
  fp << int(glow_eSave_Line_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_Line_line_width) << FSPACE << line_width << endl;
  fp << int(glow_eSave_Line_p1) << endl;
  p1.save( fp, mode);
  fp << int(glow_eSave_Line_p2) << endl;
  p2.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GlowLine::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    if ( !fp.good()) {
      fp.clear();
      fp.getline( dummy, sizeof(dummy));
      printf( "** Read error GlowLine: \"%d %s\"\n", type, dummy);      
    }

    fp >> type;
    switch( type) {
      case glow_eSave_Line: break;
      case glow_eSave_Line_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_Line_line_width: fp >> line_width; break;
      case glow_eSave_Line_p1: p1.open( fp); break;
      case glow_eSave_Line_p2: p2.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowLine:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GlowLine::draw( GlowWind *w, void *pos, int hightlight, int hot, void *node)
{
  int p1_x, p1_y, p2_x, p2_y;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
    p1_x = p1.nav_z_x;
    p1_y = p1.nav_z_y;
    p2_x = p2.nav_z_x;
    p2_y = p2.nav_z_y;
  }
  else {
    p1_x = p1.z_x;
    p1_y = p1.z_y;
    p2_x = p2.z_x;
    p2_y = p2.z_y;
  }
  if ( p1_x == p2_x && p1_y == p2_y)
    return;
  int idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  w->set_draw_buffer_only();
  ctx->gdraw->line( w, p1_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	p1_y + ((GlowPoint *)pos)->z_y - w->offset_y, 
	p2_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	p2_y + ((GlowPoint *)pos)->z_y - w->offset_y,
	draw_type, idx, hightlight);

  w->reset_draw_buffer_only();
}

void GlowLine::draw_shadow( GlowWind *w, int border, int shadow, int hightlight, int hot)
{
  int p1_x, p1_y, p2_x, p2_y;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
    p1_x = p1.nav_z_x;
    p1_y = p1.nav_z_y;
    p2_x = p2.nav_z_x;
    p2_y = p2.nav_z_y;
  }
  else {
    p1_x = p1.z_x;
    p1_y = p1.z_y;
    p2_x = p2.z_x;
    p2_y = p2.z_y;
  }
  if ( p1_x == p2_x && p1_y == p2_y)
    return;
  int idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int ish = 1;
  int offs = border;

  w->set_draw_buffer_only();
  if ( shadow && idx > 2) {
    if ( p1_x == p2_x) {
      // Vertical line
      ctx->gdraw->line( w, p1_x - w->offset_x + idx/2 - idx + offs, 
		    p1_y - w->offset_y, 
		    p2_x - w->offset_x + idx/2 - idx + offs, 
		    p2_y - w->offset_y,
		    ctx->shift_drawtype( draw_type, -2, 0), ish-1, hightlight);
      ctx->gdraw->line( w, p1_x - w->offset_x + idx/2 - offs, 
		    p1_y - w->offset_y, 
		    p2_x - w->offset_x + idx/2 - offs, 
		    p2_y - w->offset_y,
		    ctx->shift_drawtype( draw_type, 2, 0), ish-1, hightlight);
    }
    else {
      // Horizontal line
      ctx->gdraw->line( w, p1_x - w->offset_x, 
		    p1_y - w->offset_y + idx/2 - idx + offs, 
		    p2_x - w->offset_x, 
		    p2_y - w->offset_y + idx/2 - idx + offs,
		    ctx->shift_drawtype( draw_type, -2, 0), ish-1, hightlight);
      ctx->gdraw->line( w, p1_x - w->offset_x, 
		    p1_y - w->offset_y + idx/2 - offs,
		    p2_x - w->offset_x,
		    p2_y - w->offset_y + idx/2 - offs,
		    ctx->shift_drawtype( draw_type, 2, 0), ish-1, hightlight);
    }
  }
  if ( border) {
    if ( p1_x == p2_x) {
      // Vertical line
      ctx->gdraw->line( w, p1_x - w->offset_x + idx/2 - idx, 
		    p1_y - w->offset_y, 
		    p2_x - w->offset_x + idx/2 - idx, 
		    p2_y - w->offset_y,
		    glow_eDrawType_Line, 0, hightlight);
      if ( idx > 0)
	ctx->gdraw->line( w, p1_x - w->offset_x + idx/2, 
		    p1_y - w->offset_y, 
		    p2_x - w->offset_x + idx/2, 
		    p2_y - w->offset_y,
		    glow_eDrawType_Line, 0, hightlight);
    }
    else {
      // Horizontal line
      ctx->gdraw->line( w, p1_x - w->offset_x, 
		    p1_y - w->offset_y + idx/2 - idx, 
		    p2_x - w->offset_x, 
		    p2_y - w->offset_y + idx/2 - idx,
		    glow_eDrawType_Line, 0, hightlight);
      if ( idx > 0)
	ctx->gdraw->line( w, p1_x - w->offset_x, 
		    p1_y - w->offset_y + idx/2, 
		    p2_x - w->offset_x, 
		    p2_y - w->offset_y + idx/2,
		    glow_eDrawType_Line, 0, hightlight);
    }
  }
  w->reset_draw_buffer_only();
}

void GlowLine::erase( GlowWind *w, void *pos, int hot, void *node)
{
  int p1_x, p1_y, p2_x, p2_y;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
    p1_x = p1.nav_z_x;
    p1_y = p1.nav_z_y;
    p2_x = p2.nav_z_x;
    p2_y = p2.nav_z_y;
  }
  else {
    p1_x = p1.z_x;
    p1_y = p1.z_y;
    p2_x = p2.z_x;
    p2_y = p2.z_y;
  }
  if ( p1_x == p2_x && p1_y == p2_y)
    return;
  int idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  ctx->gdraw->line_erase( w, p1_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	p1_y + ((GlowPoint *)pos)->z_y - w->offset_y, 
	p2_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	p2_y + ((GlowPoint *)pos)->z_y - w->offset_y, idx);
}

int GlowLine::event_handler( GlowWind *w, void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;

  p = (GlowPoint *) pos;
  int x1 = p1.z_x + ((GlowPoint *)pos)->z_x - w->offset_x;
  int x2 = p2.z_x + ((GlowPoint *)pos)->z_x - w->offset_x;
  int y1 = p1.z_y + ((GlowPoint *)pos)->z_y - w->offset_y;
  int y2 = p2.z_y + ((GlowPoint *)pos)->z_y - w->offset_y;

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
//    cout << "Event handler: Hit in line" << endl;
    return 1;
  }
  else
    return 0;
}

void GlowLine::get_borders( double pos_x, double pos_y,
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

void GlowLine::move( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot)
{
  erase( &ctx->mw, pos, hot, NULL);
  erase( &ctx->navw, pos, 0, NULL);
  p1.x = x1;
  p1.y = y1;
  p2.x = x2;
  p2.y = y2;
  zoom();
  nav_zoom();
  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}

void GlowLine::move_noerase( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot)
{
  p1.x = x1;
  p1.y = y1;
  p2.x = x2;
  p2.y = y2;
  zoom();
  nav_zoom();
  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}

void GlowLine::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  erase( &ctx->mw, pos, hot, NULL);
  erase( &ctx->navw, pos, 0, NULL);
  p1.x += delta_x;
  p1.y += delta_y;
  p2.x += delta_x;
  p2.y += delta_y;
  zoom();
  nav_zoom();

  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}

void GlowLine::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  ctx->export_jbean->line( 
	p1.z_x - ctx->mw.offset_x, 
	p1.z_y - ctx->mw.offset_y, 
	p2.z_x - ctx->mw.offset_x, 
	p2.z_y - ctx->mw.offset_y,
	draw_type, idx, pass, shape_cnt, node_cnt, fp);
  (*shape_cnt)++;
}

void GlowLine::export_javabean_shadow( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp, int shadow, int border)
{
  if ( p1.z_x == p2.z_x && p1.z_y == p2.z_y)
    return;
  int idx = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * line_width - 1);
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);

  int ish = 1;
  int offs = border;

  if ( shadow) {
    if ( p1.z_x == p2.z_x) {
      // Vertical line
      ctx->export_jbean->line( p1.z_x - ctx->mw.offset_x + idx/2 - idx + offs, 
		    p1.z_y - ctx->mw.offset_y, 
		    p2.z_x - ctx->mw.offset_x + idx/2 - idx + offs, 
		    p2.z_y - ctx->mw.offset_y,
		    ctx->shift_drawtype( draw_type, -2, 0), ish-1, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line( p1.z_x - ctx->mw.offset_x + idx/2 - offs, 
		    p1.z_y - ctx->mw.offset_y, 
		    p2.z_x - ctx->mw.offset_x + idx/2 - offs, 
		    p2.z_y - ctx->mw.offset_y,
		    ctx->shift_drawtype( draw_type, 2, 0), ish-1, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
    else {
      // Horizontal line
      ctx->export_jbean->line( p1.z_x - ctx->mw.offset_x, 
		    p1.z_y - ctx->mw.offset_y + idx/2 - idx + offs, 
		    p2.z_x - ctx->mw.offset_x, 
		    p2.z_y - ctx->mw.offset_y + idx/2 - idx + offs,
		    ctx->shift_drawtype( draw_type, -2, 0), ish-1, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line( p1.z_x - ctx->mw.offset_x, 
		    p1.z_y - ctx->mw.offset_y + idx/2 - offs,
		    p2.z_x - ctx->mw.offset_x,
		    p2.z_y - ctx->mw.offset_y + idx/2 - offs,
		    ctx->shift_drawtype( draw_type, 2, 0), ish-1, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }
  if ( border) {
    if ( p1.z_x == p2.z_x) {
      // Vertical line
      ctx->export_jbean->line( p1.z_x - ctx->mw.offset_x + idx/2 - idx, 
		    p1.z_y - ctx->mw.offset_y, 
		    p2.z_x - ctx->mw.offset_x + idx/2 - idx, 
		    p2.z_y - ctx->mw.offset_y,
		    glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line( p1.z_x - ctx->mw.offset_x + idx/2, 
		    p1.z_y - ctx->mw.offset_y, 
		    p2.z_x - ctx->mw.offset_x + idx/2, 
		    p2.z_y - ctx->mw.offset_y,
		    glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
    else {
      // Horizontal line
      ctx->export_jbean->line( p1.z_x - ctx->mw.offset_x, 
		    p1.z_y - ctx->mw.offset_y + idx/2 - idx, 
		    p2.z_x - ctx->mw.offset_x, 
		    p2.z_y - ctx->mw.offset_y + idx/2 - idx,
		    glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
      ctx->export_jbean->line( p1.z_x - ctx->mw.offset_x, 
		    p1.z_y - ctx->mw.offset_y + idx/2, 
		    p2.z_x - ctx->mw.offset_x, 
		    p2.z_y - ctx->mw.offset_y + idx/2,
		    glow_eDrawType_Line, 0, pass, shape_cnt, node_cnt, fp); (*shape_cnt)++;
    }
  }

}

ostream& operator<< ( ostream& o, const GlowLine l)
{
  o << 
  '(' << l.p1.x << ',' << l.p1.y << ')' << 
  '(' << l.p2.x << ',' << l.p2.y << ')' << 
  '[' << l.p1.z_x << ',' << l.p1.z_y << ']' <<
  '[' << l.p2.z_x << ',' << l.p2.z_y << ']' ;
  return o;
}

void GlowLine::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);
    break;
  }
  }  
}
