/* 
 * Proview   $Id: glow_rect.cpp,v 1.3 2007-01-04 07:57:39 claes Exp $
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
#include <iostream.h>
#include "glow_rect.h"
#include "glow_draw.h"

void GlowRect::zoom()
{
  ll.zoom();
  ur.zoom();
}

void GlowRect::nav_zoom()
{
  ll.nav_zoom();
  ur.nav_zoom();
}

void GlowRect::print_zoom()
{
  ll.print_zoom();
  ur.print_zoom();
}

void GlowRect::traverse( int x, int y)
{
  ll.traverse( x, y);
  ur.traverse( x, y);
}

void GlowRect::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Rect) << endl;
  fp << int(glow_eSave_Rect_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_Rect_line_width) << FSPACE << line_width << endl;
  fp << int(glow_eSave_Rect_display_level) << FSPACE << int(display_level) << endl;
  fp << int(glow_eSave_Rect_fill) << FSPACE << fill << endl;
  fp << int(glow_eSave_Rect_ll) << endl;
  ll.save( fp, mode);
  fp << int(glow_eSave_Rect_ur) << endl;
  ur.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GlowRect::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Rect: break;
      case glow_eSave_Rect_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_Rect_line_width: fp >> line_width; break;
      case glow_eSave_Rect_display_level: fp >> tmp; display_level = (glow_mDisplayLevel)tmp; break;
      case glow_eSave_Rect_fill: fp >> fill; break;
      case glow_eSave_Rect_ll: ll.open( fp); break;
      case glow_eSave_Rect_ur: ur.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowRect:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GlowRect::draw( GlowWind *w, void *pos, int highlight, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int ll_x, ll_y, ur_x, ur_y;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
    ll_x = ll.nav_z_x;
    ll_y = ll.nav_z_y;
    ur_x = ur.nav_z_x;
    ur_y = ur.nav_z_y;
  }
  else {
    ll_x = ll.z_x;
    ll_y = ll.z_y;
    ur_x = ur.z_x;
    ur_y = ur.z_y;
  }
  int idx;
  if ( fix_line_width) {
    idx = line_width;
    idx += hot;
    if ( idx < 0) {
      erase( w, pos, hot, node);
      return;
    }
  }
  else {
    idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  if ( !fill)
    ctx->gdraw->rect( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x, ll_y + 
	((GlowPoint *)pos)->z_y - w->offset_y, 
	ur_x - ll_x, ur_y - ll_y, draw_type, idx, highlight);
  else
    ctx->gdraw->fill_rect( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x, ll_y + 
	((GlowPoint *)pos)->z_y - w->offset_y, 
	ur_x - ll_x, ur_y - ll_y, draw_type);
}

void GlowRect::erase( GlowWind *w, void *pos, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int ll_x, ll_y, ur_x, ur_y;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
    ll_x = ll.nav_z_x;
    ll_y = ll.nav_z_y;
    ur_x = ur.nav_z_x;
    ur_y = ur.nav_z_y;
  }
  else {
    ll_x = ll.z_x;
    ll_y = ll.z_y;
    ur_x = ur.z_x;
    ur_y = ur.z_y;
  }
  int idx;
  if ( fix_line_width) {
    idx = line_width;
    idx += hot;
    if ( idx < 0) return;
  }
  else {
    idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  if ( !fill)
    ctx->gdraw->rect_erase( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x, ll_y + 
	((GlowPoint *)pos)->z_y - w->offset_y, 
	ur_x - ll_x, ur_y - ll_y, idx);
  else
    ctx->gdraw->fill_rect( w, ll_x + ((GlowPoint *)pos)->z_x - w->offset_x, ll_y + 
	((GlowPoint *)pos)->z_y - w->offset_y, 
	ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
}

int GlowRect::event_handler( GlowWind *w, void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;

  p = (GlowPoint *) pos;
  if ( ll.z_x + ((GlowPoint *)pos)->z_x - w->offset_x <= x && 
       x <= ur.z_x  + ((GlowPoint *)pos)->z_x - w->offset_x &&
       ll.z_y  + ((GlowPoint *)pos)->z_y - w->offset_y <= y && 
       y <= ur.z_y + ((GlowPoint *)pos)->z_y - w->offset_y)
  {
//    cout << "Event handler: Hit in rect" << endl;
    return 1;
  }  
  else
    return 0;
}

void GlowRect::get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node)
{
  if ( display_level != glow_mDisplayLevel_1)
    return;
  if ( pos_x + ll.x < *x_left)
    *x_left = pos_x + ll.x;
  if ( pos_x + ur.x > *x_right)
    *x_right = pos_x + ur.x;
  if ( pos_y + ll.y < *y_low)
    *y_low = pos_y + ll.y;
  if ( pos_y + ur.y > *y_high)
    *y_high = pos_y + ur.y;
}

void GlowRect::move( void *pos, double x, double y, int highlight, int hot)
{
  double width, height;

  width = ur.x - ll.x;
  height = ur.y - ll.y;
  erase( &ctx->mw, pos, hot, NULL);
  erase( &ctx->navw, pos, 0, NULL);
  ll.x = x;
  ll.y = y;
  ur.x = x + width;
  ur.y = y + height;
  zoom();
  nav_zoom();
  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}

void GlowRect::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  erase( &ctx->mw, pos, hot, NULL);
  erase( &ctx->navw, pos, 0, NULL);
  ll.x += delta_x;
  ll.y += delta_y;
  ur.x += delta_x;
  ur.y += delta_y;
  zoom();
  nav_zoom();

  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}


