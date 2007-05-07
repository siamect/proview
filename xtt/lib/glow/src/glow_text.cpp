/* 
 * Proview   $Id: glow_text.cpp,v 1.6 2007-05-07 14:35:03 claes Exp $
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
#include <fstream.h>
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

void GlowText::draw( GlowWind *w, void *pos, int highlight, int hot, void *node)
{
  int idx = int( w->zoom_factor_y / w->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  ctx->gdraw->text( w, p.z_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	p.z_y + ((GlowPoint *)pos)->z_y - w->offset_y, text, strlen(text),
	draw_type, color_drawtype, idx, highlight, 0, glow_eFont_Helvetica);
}

void GlowText::erase( GlowWind *w, void *pos, int hot, void *node)
{
  int idx = int( w->zoom_factor_y / w->base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  ctx->gdraw->text_erase( w, p.z_x + ((GlowPoint *)pos)->z_x - w->offset_x, 
	p.z_y + ((GlowPoint *)pos)->z_y - w->offset_y, text,  strlen(text),
	draw_type, idx, 0, glow_eFont_Helvetica);
}

int GlowText::event_handler( GlowWind *w, void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;

  p = (GlowPoint *) pos;
    return 0;
}

void GlowText::get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node)
{
}

void GlowText::move( void *pos, double x, double y, int highlight, int hot)
{

  erase( &ctx->mw, pos, hot, NULL);
  erase( &ctx->navw, pos, 0, NULL);
  p.x = x;
  p.y = y;
  zoom();
  nav_zoom();
  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}

void GlowText::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  erase( &ctx->mw, pos, hot, NULL);
  erase( &ctx->navw, pos, 0, NULL);
  p.x += delta_x;
  p.y += delta_y;
  zoom();
  nav_zoom();

  draw( &ctx->mw, pos, highlight, hot, NULL);
  draw( &ctx->navw, pos, highlight, 0, NULL);
}


