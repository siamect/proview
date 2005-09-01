/* 
 * Proview   $Id: glow_text.cpp,v 1.3 2005-09-01 14:57:54 claes Exp $
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
#include "glow_text.h"
#include "glow_draw.h"

void GlowText::zoom()
{
  p.zoom();
}

void GlowText::nav_zoom()
{
  p.nav_zoom();
}

void GlowText::print_zoom()
{
  p.print_zoom();
}

void GlowText::traverse( int x, int y)
{
  p.traverse( x, y);
}

void GlowText::print( void *pos, void *node)
{
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
		(text_size +4) - 4);
  int size = int( 8.0 + 6.0/3*idx);

  if ( size <= 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  ctx->print_ps->text( p.print_z_x + ((GlowPoint *)pos)->print_z_x , 
	p.print_z_y + ((GlowPoint *)pos)->print_z_y, text, strlen(text),
	draw_type, size);
}

void GlowText::save( ofstream& fp, glow_eSaveMode mode)
{
  fp << int(glow_eSave_Text) << endl;
  fp << int(glow_eSave_Text_text_size) << FSPACE << text_size << endl;
  fp << int(glow_eSave_Text_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_Text_color_drawtype) << FSPACE << int(color_drawtype) << endl;
  fp << int(glow_eSave_Text_text) << FSPACE << text << endl;
  fp << int(glow_eSave_Text_p) << endl;
  p.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GlowText::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Text: break;
      case glow_eSave_Text_text_size: fp >> text_size; break;
      case glow_eSave_Text_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_Text_color_drawtype: fp >> tmp; color_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_Text_text: 
        fp.get();
        fp.getline( text, sizeof(text));
        break;
      case glow_eSave_Text_p: p.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowText:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GlowText::draw( void *pos, int highlight, int hot, void *node)
{
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_text( ctx, p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, text, strlen(text),
	draw_type, color_drawtype, idx, highlight, 0);
}

void GlowText::erase( void *pos, int hot, void *node)
{
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_text_erase( ctx, p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, text,  strlen(text),
	draw_type, idx, 0);
}

void GlowText::nav_draw( void *pos, int highlight, void *node)
{
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * (text_size +4) 
		- 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_text( ctx, 
	p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	text, strlen(text), draw_type, idx, highlight, 0);
}

void GlowText::nav_erase( void *pos, void *node)
{
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * (text_size +4) -
		 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_text_erase( ctx,
	p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	text, strlen(text), draw_type, idx, 0);
}

int GlowText::event_handler( void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;

  p = (GlowPoint *) pos;
/**
  if ( p1.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x < x && 
       x < p2.z_x  + ((GlowPoint *)pos)->z_x - ctx->offset_x &&
       p1.z_y  + ((GlowPoint *)pos)->z_y - ctx->offset_y < y && 
       y < p2.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y)
  {
    cout << "Event handler: Hit in text" << endl;
    return 1;
  }  
  else
***/
    return 0;
}

void GlowText::get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node)
{
/*
  if ( pos_x + p1.x < *x_left)
    *x_left = pos_x + p1.x;
  if ( pos_x + p2.x < *x_left)
    *x_left = pos_x + p1.x;
  if ( pos_x + p1.x > *x_right)
    *x_right = pos_x + p2.x;
  if ( pos_x + p2.x > *x_right)
    *x_right = pos_x + p2.x;
  if ( pos_y + p1.y < *y_low)
    *y_low = pos_y + p1.y;
  if ( pos_y + p2.y < *y_low)
    *y_low = pos_y + p1.y;
  if ( pos_y + p1.y > *y_high)
    *y_high = pos_y + p2.y;
  if ( pos_y + p2.y > *y_high)
    *y_high = pos_y + p2.y;
*/
}

void GlowText::move( void *pos, double x, double y, int highlight, int hot)
{

  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p.x = x;
  p.y = y;
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void GlowText::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  p.x += delta_x;
  p.y += delta_y;
  zoom();
  nav_zoom();

  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

ostream& operator<< ( ostream& o, const GlowText t)
{
  o << 
  '(' << t.p.x << ',' << t.p.y << ')' << t.text;
  return o;
}

