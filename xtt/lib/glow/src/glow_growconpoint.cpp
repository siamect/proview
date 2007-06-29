/* 
 * Proview   $Id: glow_growconpoint.cpp,v 1.5 2007-06-29 09:31:24 claes Exp $
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
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growconpoint.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowConPoint::GrowConPoint( GrowCtx *glow_ctx, char *name, double x, double y, 
		int cp_num, glow_eDirection d, int nodraw) :
		GlowConPoint(glow_ctx,x,y,cp_num,d), 
    		hot(0), pzero(ctx), highlight(0), inverse(0), 
		arc( ctx, x - ctx->draw_delta, y - ctx->draw_delta, 
		x + ctx->draw_delta, y + ctx->draw_delta, 0, 360, 
		glow_eDrawType_Line, 2),
		user_data(NULL)
{ 
  strcpy( n_name, name);
  pzero.nav_zoom();

  if ( ctx->grid_on)
  {
    double x_grid, y_grid;

    ctx->find_grid( p.x, p.y, &x_grid, &y_grid);
    p.posit( x_grid, y_grid);
  }
  arc.ll.posit( - ctx->draw_delta, - ctx->draw_delta);
  arc.ur.posit( ctx->draw_delta, ctx->draw_delta);
  if ( !nodraw)
    draw( &ctx->mw, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  get_node_borders();
}

GrowConPoint::~GrowConPoint()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase( &ctx->mw);
  erase( &ctx->navw);

  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
  if ( hot)
    ctx->gdraw->set_cursor( &ctx->navw, glow_eDrawCursor_Normal);
}

void GrowConPoint::move( double delta_x, double delta_y, int grid)
{
  ctx->set_defered_redraw();
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  if ( grid) {
    double x, y, x_grid, y_grid;

    /* Move to closest grid point */
    erase( &ctx->mw);
    erase( &ctx->navw);
    x = (x_right + x_left) / 2;
    y = (y_high + y_low) / 2;
    ctx->find_grid( x + delta_x / ctx->mw.zoom_factor_x,
	y + delta_y / ctx->mw.zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x, y_grid - y);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    erase( &ctx->mw);
    erase( &ctx->navw);
    dx = delta_x / ctx->mw.zoom_factor_x;
    dy = delta_y / ctx->mw.zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
  ctx->redraw_defered();

}

void GrowConPoint::move_noerase( int delta_x, int delta_y, int grid)
{
  if ( grid) {
    double x, y, x_grid, y_grid;

    /* Move to closest grid point */
    x = (x_right + x_left) / 2;
    y = (y_high + y_low) / 2;
    ctx->find_grid( x + double( delta_x) / ctx->mw.zoom_factor_x,
	y + double( delta_y) / ctx->mw.zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x, y_grid - y);
    get_node_borders();
  }
  else {
    double dx, dy;

    dx = double( delta_x) / ctx->mw.zoom_factor_x;
    dy = double( delta_y) / ctx->mw.zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);

}

int GrowConPoint::event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx,
				 double fy)
{
  int sts;
  GlowPoint p1(p);
  p1.x = trf.x( p.x, p.y);
  p1.y = trf.y( p.x, p.y);
  p1.zoom();

  sts = 0;
  if ( event == ctx->event_move_node) {
    sts = arc.event_handler( w, (void *)&p1, 
			     event, x, y, NULL);
    if ( sts) {
      /* Register node for potential movement */
      ctx->move_insert( this);
    }
    return sts;
  }
  switch ( event) {
    case glow_eEvent_CursorMotion: {
      int redraw = 0;

      if ( ctx->hot_found)
        sts = 0;
      else {
        sts = arc.event_handler( w, (void *)&p1, event, x, y, NULL);
        if ( sts)
          ctx->hot_found = 1;
      }
      if ( sts && !hot  &&
	   !(ctx->node_movement_active || ctx->node_movement_paste_active)) {
        ctx->gdraw->set_cursor( w, glow_eDrawCursor_CrossHair);
        hot = 1;
        redraw = 1;
      }
      if ( !sts && hot) {
        ctx->gdraw->set_cursor( w, glow_eDrawCursor_Normal);
        erase( w);
        hot = 0;
        redraw = 1;
      }
      if ( redraw) {
	ctx->draw( w, x_left * w->zoom_factor_x - w->offset_x - DRAW_MP,
	     y_low * w->zoom_factor_y - w->offset_y - DRAW_MP,
  	     x_right * w->zoom_factor_x - w->offset_x + DRAW_MP,
	     y_high * w->zoom_factor_y - w->offset_y + DRAW_MP);
      }
      break;
    }
    default:
      sts = arc.event_handler( w, (void *)&p1, 
		event, x, y, NULL);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}


void GrowConPoint::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  if ( mode == glow_eSaveMode_SubGraph)
    GlowConPoint::save( fp, mode);
  else {
    fp << int(glow_eSave_GrowConPoint) << endl;
    fp << int(glow_eSave_GrowConPoint_n_name) << FSPACE << n_name << endl;
    fp << int(glow_eSave_GrowConPoint_x_right) << FSPACE << x_right << endl;
    fp << int(glow_eSave_GrowConPoint_x_left) << FSPACE << x_left << endl;
    fp << int(glow_eSave_GrowConPoint_y_high) << FSPACE << y_high << endl;
    fp << int(glow_eSave_GrowConPoint_y_low) << FSPACE << y_low << endl;
    fp << int(glow_eSave_GrowConPoint_arc) << endl;
    arc.save( fp, mode);
    fp << int(glow_eSave_GrowConPoint_conpoint_part) << endl;
    GlowConPoint::save( fp, mode);
    fp << int(glow_eSave_End) << endl;
  }
}

void GrowConPoint::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];

  for (;;) {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowConPoint: break;
      case glow_eSave_GrowConPoint_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowConPoint_x_right: fp >> x_right; break;
      case glow_eSave_GrowConPoint_x_left: fp >> x_left; break;
      case glow_eSave_GrowConPoint_y_high: fp >> y_high; break;
      case glow_eSave_GrowConPoint_y_low: fp >> y_low; break;
      case glow_eSave_GrowConPoint_arc: arc.open( fp); break;
      case glow_eSave_GrowConPoint_conpoint_part: 
        GlowConPoint::open( fp);
        break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowConPoint:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowConPoint::draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int tmp;

  if ( ll_x > ur_x) {
    /* Shift */
    tmp = ll_x;
    ll_x = ur_x;
    ur_x = tmp;
  }
  if ( ll_y > ur_y) {
    /* Shift */
    tmp = ll_y;
    ll_y = ur_y;
    ur_y = tmp;
  }

  if ( x_right * w->zoom_factor_x - w->offset_x >= ll_x &&
      	x_left * w->zoom_factor_x - w->offset_x <= ur_x &&
       	y_high * w->zoom_factor_y - w->offset_y >= ll_y &&
       	y_low * w->zoom_factor_y - w->offset_y <= ur_y)
  {
    draw( w, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  }
}

void GrowConPoint::draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	tmp;
  int 	obj_ur_x = int( x_right * w->zoom_factor_x) - w->offset_x;
  int	obj_ll_x = int( x_left * w->zoom_factor_x) - w->offset_x;
  int	obj_ur_y = int( y_high * w->zoom_factor_y) - w->offset_y;
  int   obj_ll_y = int( y_low * w->zoom_factor_y) - w->offset_y;


  if ( *ll_x > *ur_x) {
    /* Shift */
    tmp = *ll_x;
    *ll_x = *ur_x;
    *ur_x = tmp;
  }
  if ( *ll_y > *ur_y) {
    /* Shift */
    tmp = *ll_y;
    *ll_y = *ur_y;
    *ur_y = tmp;
  }

  if (  obj_ur_x >= *ll_x &&
      	obj_ll_x <= *ur_x &&
       	obj_ur_y >= *ll_y &&
       	obj_ll_y <= *ur_y) {
    draw( w, (GlowTransform *)NULL, highlight, hot, NULL, NULL);

    // Increase the redraw area
    if ( obj_ur_x > *ur_x)
      *ur_x = obj_ur_x;
    if ( obj_ur_y > *ur_y)
      *ur_y = obj_ur_y;
    if ( obj_ll_x < *ll_x)
      *ll_x = obj_ll_x;
    if ( obj_ll_y < *ll_y)
      *ll_y = obj_ll_y;
  }
}

void GrowConPoint::set_highlight( int on)
{
  highlight = on;
  draw( &ctx->mw, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  draw( &ctx->navw, (GlowTransform *)NULL, highlight, 0, NULL, NULL);
}

void GrowConPoint::select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy)
{
  if ( select_policy == glow_eSelectPolicy_Surround ) {
    if ( x_left > ll_x && x_right < ur_x && y_high < ur_y && y_low > ll_y)
      ctx->select_insert( this);
  }
  else {
    if ( x_right > ll_x && x_left < ur_x && y_low < ur_y && y_high > ll_y)
      ctx->select_insert( this);
  }
}

void GrowConPoint::zoom()
{
  GlowConPoint::zoom();
  arc.zoom();
}

void GrowConPoint::nav_zoom()
{
  GlowConPoint::nav_zoom();
  arc.nav_zoom();
}

void GrowConPoint::draw( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node,
			 void *colornode)
{
  if ( w == &ctx->navw) {
    return;
  }

  GlowPoint p1(p);
  if ( t) {
    p1.x = trf.x( t, p.x, p.y);
    p1.y = trf.y( t, p.x, p.y);
  }
  else {
    p1.x = trf.x( p.x, p.y);
    p1.y = trf.y( p.x, p.y);
  }
  p1.zoom();
  arc.draw( w, (void *)&p1, highlight, hot, NULL);
}

void GrowConPoint::erase( GlowWind *w, GlowTransform *t, int hot, void *node)
{
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }

  GlowPoint p1(p);
  if ( t) {
    p1.x = trf.x( t, p.x, p.y);
    p1.y = trf.y( t, p.x, p.y);
  }
  else {
    p1.x = trf.x( p.x, p.y);
    p1.y = trf.y( p.x, p.y);
  }
  p1.zoom();
  w->set_draw_buffer_only();
  arc.erase( w, (void *)&p1, hot, NULL);
  w->reset_draw_buffer_only();
}

void GrowConPoint::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();

}


void GrowConPoint::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy;

    erase( &ctx->mw);
    erase( &ctx->navw);
    ctx->set_defered_redraw();
    draw();
    switch ( direction) {
      case glow_eAlignDirection_CenterVert:
        dx = x - (x_right + x_left) / 2;
        dy = 0;
        break;        
      case glow_eAlignDirection_CenterHoriz:
        dx = 0;
        dy = y - (y_high + y_low) / 2;
        break;        
      case glow_eAlignDirection_CenterCenter:
        dx = x - (x_right + x_left) / 2;
        dy = y - (y_high + y_low) / 2;
        break;        
      case glow_eAlignDirection_Right:
        dx = x - x_right;
        dy = 0;
        break;        
      case glow_eAlignDirection_Left:
        dx = x - x_left;
        dy = 0;
        break;        
      case glow_eAlignDirection_Up:
        dx = 0;
        dy = y - y_high;
        break;        
      case glow_eAlignDirection_Down:
        dx = 0;
        dy = y - y_low;
        break;        
    }
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;

    draw();
    ctx->redraw_defered();
}

void GrowConPoint::draw()
{
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowConPoint::get_node_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  GlowPoint p1(p);
  if ( t)
  {
    p1.x = trf.x( t, p.x, p.y);
    p1.y = trf.y( t, p.x, p.y);
  }
  else
  {
    p1.x = trf.x( p.x, p.y);
    p1.y = trf.y( p.x, p.y);
  }
  arc.get_borders( p1.x, p1.y, x_right, x_left, y_high, y_low, NULL);
}

void GrowConPoint::get_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  double x1, y1;

  if ( t) {
    x1 = trf.x( t, p.x, p.y);
    y1 = trf.y( t, p.x, p.y);
  }
  else {
    x1 = trf.x( p.x, p.y);
    y1 = trf.y( p.x, p.y);
  }

  if ( x1 < *x_left)
    *x_left = x1;
  if ( x1 > *x_right)
    *x_right = x1;
  if ( y1 < *y_low)
    *y_low = y1;
  if ( y1 > *y_high)
    *y_high = y1;
}
