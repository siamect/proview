/* 
 * Proview   $Id: glow_pixmap.cpp,v 1.3 2005-12-13 15:14:16 claes Exp $
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
#include "glow_pixmap.h"
#include "glow_draw.h"

GlowPixmap::GlowPixmap( GlowCtx *glow_ctx, glow_sPixmapData *pix_data,
	 double x, double y, glow_eDrawType d_type, int size) :
	ctx(glow_ctx), p(glow_ctx,x,y), pixmap_size(size), draw_type(d_type)
{
  if ( pix_data == NULL)
    return;
  memcpy( pixmap_data, pix_data, sizeof( pixmap_data));
  glow_draw_pixmaps_create( ctx, pix_data, &pixmaps);
}

GlowPixmap::GlowPixmap( const GlowPixmap& p)
{
  memcpy( this, &p, sizeof(p));
  glow_draw_pixmaps_create( ctx, (glow_sPixmapData *) pixmap_data, &pixmaps);
}

GlowPixmap::~GlowPixmap()
{
  glow_draw_pixmaps_delete( ctx, pixmaps);
}

void GlowPixmap::zoom()
{
  p.zoom();
}

void GlowPixmap::nav_zoom()
{
  p.nav_zoom();
}

void GlowPixmap::print_zoom()
{
  p.print_zoom();
}

void GlowPixmap::traverse( int x, int y)
{
  p.traverse( x, y);
}

void GlowPixmap::print( void *pos, void *node)
{
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
		(pixmap_size +4) - 4);
  int size = int( 8.0 + 6.0/3*idx);

  if ( size <= 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);

  ctx->print_ps->pixmap( p.print_z_x + ((GlowPoint *)pos)->print_z_x, 
	p.print_z_y + ((GlowPoint *)pos)->print_z_y,
	&pixmap_data[pixmap_size],
	draw_type);
}

void GlowPixmap::save( ofstream& fp, glow_eSaveMode mode)
{
#if 0
  fp << int(glow_eSave_Pixmap) << endl;
  fp << int(glow_eSave_Pixmap_pixmap_size) << FSPACE << pixmap _size << endl;
  fp << int(glow_eSave_Pixmap_draw_type) << FSPACE << draw_type << endl;
  fp << int(glow_eSave_Pixmap_text) << FSPACE << text << endl;
  fp << int(glow_eSave_Pixmap_p) << endl;
  p.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
#endif
}

void GlowPixmap::open( ifstream& fp)
{
#if 0
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Pixmap: break;
      case glow_eSave_Pixmap_pixmap_size: fp >> pixmap_size; break;
      case glow_eSave_Pixmap_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_Pixmap_text: 
        fp.get();
        fp.getline( text, sizeof(text));
        break;
      case glow_eSave_Pixmap_p: p.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowPixmap:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
#endif
}

void GlowPixmap::draw( void *pos, int highlight, int hot, void *node)
{
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (pixmap_size + 4) - 
		4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_pixmap( ctx, p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, &pixmap_data, pixmaps,
	draw_type, idx, highlight, 0);
}

void GlowPixmap::draw_inverse( void *pos, int hot, void *node)
{
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (pixmap_size + 4) -
	 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_pixmap_inverse( ctx, p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, &pixmap_data, pixmaps,
	draw_type, idx, 0);
}

void GlowPixmap::erase( void *pos, int hot, void *node)
{
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * (pixmap_size + 4) -
	 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_pixmap_erase( ctx, p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, &pixmap_data, pixmaps,
	draw_type, idx, 0);
}

void GlowPixmap::nav_draw( void *pos, int highlight, void *node)
{
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_pixmap( ctx, 
	p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	&pixmap_data, pixmaps, draw_type, idx, highlight, 0);
}

void GlowPixmap::nav_erase( void *pos, void *node)
{
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_pixmap_erase( ctx,
	p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	&pixmap_data, pixmaps, draw_type, idx, 0);
}

int GlowPixmap::event_handler( void *pos, glow_eEvent event, int x, int y,
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

void GlowPixmap::get_borders( double pos_x, double pos_y, double *x_right, 
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

void GlowPixmap::move( void *pos, double x, double y, int highlight, int hot)
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

void GlowPixmap::shift( void *pos, double delta_x, double delta_y,
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

ostream& operator<< ( ostream& o, const GlowPixmap t)
{
  o << 
  '(' << t.p.x << ',' << t.p.y << ')';
  return o;
}
