/* 
 * Proview   $Id: glow_annotpixmap.cpp,v 1.2 2005-09-01 14:57:53 claes Exp $
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
#include "glow_annotpixmap.h"
#include "glow_node.h"
#include "glow_draw.h"
#include "glow_browctx.h"

GlowAnnotPixmap::GlowAnnotPixmap( GlowCtx *glow_ctx, int num,
	 double x, double y, glow_eDrawType d_type, int size, int rel_pos) :
	ctx(glow_ctx), p(glow_ctx,x,y), number (num), pixmap_size(size), 
	draw_type(d_type), relative_pos(rel_pos)
{
}

GlowAnnotPixmap::GlowAnnotPixmap( const GlowAnnotPixmap& p)
{
  memcpy( this, &p, sizeof(p));
}

GlowAnnotPixmap::~GlowAnnotPixmap()
{
}

void GlowAnnotPixmap::zoom()
{
  p.zoom();
}

void GlowAnnotPixmap::nav_zoom()
{
  p.nav_zoom();
}

void GlowAnnotPixmap::print_zoom()
{
  p.print_zoom();
}

void GlowAnnotPixmap::traverse( int x, int y)
{
  p.traverse( x, y);
}

void GlowAnnotPixmap::print( void *pos, void *node)
{
  if ( !((GlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
	(pixmap_size +4) - 4);
  int size = int( 8.0 + 6.0/3*idx);
  double x;

  if ( size <= 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if ( relative_pos && ((GlowNode *)node)->relative_annot_pos)
  {
    x = ((GlowPoint *)pos)->print_z_x + max( p.print_z_x,
           ((GlowNode *)node)->rel_annotpixmap_x[number] * ctx->print_zoom_factor);
  }
  else
    x = p.print_z_x + ((GlowPoint *)pos)->print_z_x;
  ctx->print_ps->pixmap( x, 
	p.print_z_y + ((GlowPoint *)pos)->print_z_y,
	&((GlowNode *)node)->annotpixmapv[number]->pixmap_data[pixmap_size],
	draw_type);
}

void GlowAnnotPixmap::save( ofstream& fp, glow_eSaveMode mode)
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

void GlowAnnotPixmap::open( ifstream& fp)
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
        cout << "GlowAnnotPixmap:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
#endif
}

void GlowAnnotPixmap::draw( void *pos, int highlight, int hot, void *node)
{
  int x;

  if ( !((GlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
	(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  if ( relative_pos && ((GlowNode *)node)->relative_annot_pos)
  {
    x = int( ((GlowPoint *)pos)->z_x - ctx->offset_x + max( p.z_x,
           ((GlowNode *)node)->rel_annotpixmap_x[number] * ctx->zoom_factor_x));
  }
  else
    x = p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
  glow_draw_pixmap( ctx, x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, 
	&((GlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((GlowNode *) node)->annotpixmapv[number]->pixmaps,
	draw_type, idx, highlight, 0);
}

void GlowAnnotPixmap::draw_inverse( void *pos, int hot, void *node)
{
  int x;

  if ( !((GlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  if ( relative_pos && ((GlowNode *)node)->relative_annot_pos)
  {
    x = int( ((GlowPoint *)pos)->z_x - ctx->offset_x + max( p.z_x,
           ((GlowNode *)node)->rel_annotpixmap_x[number] * ctx->zoom_factor_x));
  }
  else
    x = p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
  glow_draw_pixmap_inverse( ctx, x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, 
	&((GlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((GlowNode *) node)->annotpixmapv[number]->pixmaps,
	draw_type, idx, 0);
}

void GlowAnnotPixmap::erase( void *pos, int hot, void *node)
{
  int x;

  if ( !((GlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  if ( relative_pos && ((GlowNode *)node)->relative_annot_pos)
  {
    x = int( ((GlowPoint *)pos)->z_x - ctx->offset_x + max( p.z_x,
           ((GlowNode *)node)->rel_annotpixmap_x[number] * ctx->zoom_factor_x));
  }
  else
    x = p.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
  glow_draw_pixmap_erase( ctx, x, 
	p.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y, 
	&((GlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((GlowNode *) node)->annotpixmapv[number]->pixmaps,
	draw_type, idx, 0);
}

void GlowAnnotPixmap::nav_draw( void *pos, int highlight, void *node)
{
  if ( !((GlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_pixmap( ctx, 
	p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	&((GlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((GlowNode *) node)->annotpixmapv[number]->pixmaps, 
	draw_type, idx, highlight, 0);
}

void GlowAnnotPixmap::nav_erase( void *pos, void *node)
{
  if ( !((GlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
	(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_pixmap_erase( ctx,
	p.nav_z_x + ((GlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	&((GlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((GlowNode *) node)->annotpixmapv[number]->pixmaps, draw_type, idx, 0);
}

int GlowAnnotPixmap::event_handler( void *pos, glow_eEvent event, int x, int y,
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

void GlowAnnotPixmap::get_borders( double pos_x, double pos_y, double *x_right, 
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

void GlowAnnotPixmap::move( void *pos, double x, double y, int highlight, int hot)
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

void GlowAnnotPixmap::shift( void *pos, double delta_x, double delta_y,
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

void GlowAnnotPixmap::configure_annotations( void *pos, void *node)
{
  double width;

  if ( !((GlowNode *) node)->annotpixmapv[number])
    return;

  if ( relative_pos)
    ((GlowNode *)node)->rel_annotpixmap_x[number] = 
		((GlowNode *)node)->relative_annot_x;
  width = 1.0 * ((GlowNode *)node)->annotpixmapv[number]->
	pixmap_data[pixmap_size].width / ctx->base_zoom_factor;
  ((GlowNode *)node)->relative_annot_x = 
	max( p.x, ((GlowNode *)node)->rel_annotpixmap_x[number]) + 
	width + ((BrowCtx *)ctx)->annotation_space;
}


ostream& operator<< ( ostream& o, const GlowAnnotPixmap t)
{
  o << 
  '(' << t.p.x << ',' << t.p.y << ')';
  return o;
}

void glow_annot_pixmap_create( GlowCtx *ctx, glow_sPixmapData *pixmap_data,
	glow_sAnnotPixmap **pixmap)
{
  *pixmap = (glow_sAnnotPixmap *) calloc( 1, sizeof( glow_sAnnotPixmap));
  memcpy( &(*pixmap)->pixmap_data, pixmap_data, sizeof( glow_sPixmapData));
  glow_draw_pixmaps_create( ctx, pixmap_data, &(*pixmap)->pixmaps);
}

void glow_annot_pixmap_free( GlowCtx *ctx, glow_sAnnotPixmap *pixmap)
{
  glow_draw_pixmaps_delete( ctx, pixmap->pixmaps);
  free( pixmap);
}
