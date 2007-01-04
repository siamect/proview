/* 
 * Proview   $Id: flow_annotpixmap.cpp,v 1.5 2007-01-04 07:53:34 claes Exp $
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
#include <stdlib.h>
#include "flow_annotpixmap.h"
#include "flow_node.h"
#include "flow_draw.h"
#include "flow_browctx.h"

FlowAnnotPixmap::FlowAnnotPixmap( FlowCtx *flow_ctx, int num,
	 double x, double y, flow_eDrawType d_type, int size, int rel_pos) :
	ctx(flow_ctx), p(flow_ctx,x,y), number (num), pixmap_size(size), 
	draw_type(d_type), relative_pos(rel_pos)
{
}

FlowAnnotPixmap::FlowAnnotPixmap( const FlowAnnotPixmap& p)
{
  memcpy( this, &p, sizeof(p));
}

FlowAnnotPixmap::~FlowAnnotPixmap()
{
}

void FlowAnnotPixmap::zoom()
{
  p.zoom();
}

void FlowAnnotPixmap::nav_zoom()
{
  p.nav_zoom();
}

void FlowAnnotPixmap::print_zoom()
{
  p.print_zoom();
}

void FlowAnnotPixmap::traverse( int x, int y)
{
  p.traverse( x, y);
}

void FlowAnnotPixmap::print( void *pos, void *node, int highlight)
{
  if ( !((FlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->print_zoom_factor / ctx->base_zoom_factor * 
	(pixmap_size +4) - 4);
  int size = int( 8.0 + 6.0/3*idx);
  double x;

  if ( size <= 0)
    return;
  idx = MAX( idx, DRAW_TYPE_SIZE-1);

  if ( relative_pos && ((FlowNode *)node)->relative_annot_pos)
  {
    x = ((FlowPoint *)pos)->print_z_x + MAX( p.print_z_x,
           ((FlowNode *)node)->rel_annotpixmap_x[number] * ctx->print_zoom_factor);
  }
  else
    x = p.print_z_x + ((FlowPoint *)pos)->print_z_x;
  ctx->print_ps->pixmap( x, 
	p.print_z_y + ((FlowPoint *)pos)->print_z_y,
	&((FlowNode *)node)->annotpixmapv[number]->pixmap_data[pixmap_size],
	draw_type);
}

void FlowAnnotPixmap::save( ofstream& fp, flow_eSaveMode mode)
{
#if 0
  fp << int(flow_eSave_Pixmap) << endl;
  fp << int(flow_eSave_Pixmap_pixmap_size) << FSPACE << pixmap _size << endl;
  fp << int(flow_eSave_Pixmap_draw_type) << FSPACE << draw_type << endl;
  fp << int(flow_eSave_Pixmap_text) << FSPACE << text << endl;
  fp << int(flow_eSave_Pixmap_p) << endl;
  p.save( fp, mode);
  fp << int(flow_eSave_End) << endl;
#endif
}

void FlowAnnotPixmap::open( ifstream& fp)
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
      case flow_eSave_Pixmap: break;
      case flow_eSave_Pixmap_pixmap_size: fp >> pixmap_size; break;
      case flow_eSave_Pixmap_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_Pixmap_text: 
        fp.get();
        fp.getline( text, sizeof(text));
        break;
      case flow_eSave_Pixmap_p: p.open( fp); break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowAnnotPixmap:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
#endif
}

void FlowAnnotPixmap::draw( void *pos, int highlight, int hot, void *node)
{
  int x;

  if ( !((FlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * 
	(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  if ( relative_pos && ((FlowNode *)node)->relative_annot_pos)
  {
    x = int( ((FlowPoint *)pos)->z_x - ctx->offset_x + MAX( p.z_x,
           ((FlowNode *)node)->rel_annotpixmap_x[number] * ctx->zoom_factor));
  }
  else
    x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
  ctx->fdraw->pixmap( ctx, x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	&((FlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((FlowNode *) node)->annotpixmapv[number]->pixmaps,
	draw_type, idx, highlight, 0);
}

void FlowAnnotPixmap::draw_inverse( void *pos, int hot, void *node)
{
  int x;

  if ( !((FlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  if ( relative_pos && ((FlowNode *)node)->relative_annot_pos)
  {
    x = int( ((FlowPoint *)pos)->z_x - ctx->offset_x + MAX( p.z_x,
           ((FlowNode *)node)->rel_annotpixmap_x[number] * ctx->zoom_factor));
  }
  else
    x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
  ctx->fdraw->pixmap_inverse( ctx, x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	&((FlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((FlowNode *) node)->annotpixmapv[number]->pixmaps,
	draw_type, idx, 0);
}

void FlowAnnotPixmap::erase( void *pos, int hot, void *node)
{
  int x;

  if ( !((FlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  if ( relative_pos && ((FlowNode *)node)->relative_annot_pos)
  {
    x = int( ((FlowPoint *)pos)->z_x - ctx->offset_x + MAX( p.z_x,
           ((FlowNode *)node)->rel_annotpixmap_x[number] * ctx->zoom_factor));
  }
  else
    x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
  ctx->fdraw->pixmap_erase( ctx, x, 
	p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y, 
	&((FlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((FlowNode *) node)->annotpixmapv[number]->pixmaps,
	draw_type, idx, 0);
}

void FlowAnnotPixmap::nav_draw( void *pos, int highlight, void *node)
{
  if ( !((FlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * 
		(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->nav_pixmap( ctx, 
	p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	&((FlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((FlowNode *) node)->annotpixmapv[number]->pixmaps, 
	draw_type, idx, highlight, 0);
}

void FlowAnnotPixmap::nav_erase( void *pos, void *node)
{
  if ( !((FlowNode *) node)->annotpixmapv[number])
    return;
  int idx = int( ctx->nav_zoom_factor / ctx->base_zoom_factor * 
	(pixmap_size + 4) - 4);
  if ( idx < 0)
    return;
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  ctx->fdraw->nav_pixmap_erase( ctx,
	p.nav_z_x + ((FlowPoint *)pos)->nav_z_x - ctx->nav_offset_x, 
	p.nav_z_y + ((FlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	&((FlowNode *) node)->annotpixmapv[number]->pixmap_data, 
	((FlowNode *) node)->annotpixmapv[number]->pixmaps, draw_type, idx, 0);
}

int FlowAnnotPixmap::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  FlowPoint *p;

  p = (FlowPoint *) pos;
/**
  if ( p1.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x < x && 
       x < p2.z_x  + ((FlowPoint *)pos)->z_x - ctx->offset_x &&
       p1.z_y  + ((FlowPoint *)pos)->z_y - ctx->offset_y < y && 
       y < p2.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y)
  {
    cout << "Event handler: Hit in text" << endl;
    return 1;
  }  
  else
***/
    return 0;
}

void FlowAnnotPixmap::get_borders( double pos_x, double pos_y, double *x_right, 
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

void FlowAnnotPixmap::move( void *pos, double x, double y, int highlight, int hot)
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

void FlowAnnotPixmap::shift( void *pos, double delta_x, double delta_y,
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

void FlowAnnotPixmap::configure_annotations( void *pos, void *node)
{
  double width;

  if ( !((FlowNode *) node)->annotpixmapv[number])
    return;

  if ( relative_pos)
    ((FlowNode *)node)->rel_annotpixmap_x[number] = 
		((FlowNode *)node)->relative_annot_x;
  width = 1.0 * ((FlowNode *)node)->annotpixmapv[number]->
	pixmap_data[pixmap_size].width / ctx->base_zoom_factor;
  ((FlowNode *)node)->relative_annot_x = 
	MAX( p.x, ((FlowNode *)node)->rel_annotpixmap_x[number]) + 
	width + ((BrowCtx *)ctx)->annotation_space;
}


ostream& operator<< ( ostream& o, const FlowAnnotPixmap t)
{
  o << 
  '(' << t.p.x << ',' << t.p.y << ')';
  return o;
}

void flow_annot_pixmap_create( FlowCtx *ctx, flow_sPixmapData *pixmap_data,
	flow_sAnnotPixmap **pixmap)
{
  *pixmap = (flow_sAnnotPixmap *) calloc( 1, sizeof( flow_sAnnotPixmap));
  memcpy( &(*pixmap)->pixmap_data, pixmap_data, sizeof( flow_sPixmapData));
  ctx->fdraw->pixmaps_create( ctx, pixmap_data, &(*pixmap)->pixmaps);
}

void flow_annot_pixmap_free( FlowCtx *ctx, flow_sAnnotPixmap *pixmap)
{
  ctx->fdraw->pixmaps_delete( ctx, pixmap->pixmaps);
  free( pixmap);
}
