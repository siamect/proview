/* 
 * Proview   $Id: glow_growsubannot.cpp,v 1.10 2008-10-31 12:51:36 claes Exp $
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
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growsubannot.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_exportflow.h"

GrowSubAnnot::GrowSubAnnot( GrowCtx *glow_ctx, const char *name, double x, double y,
	int annot_num, glow_eDrawType d_type, glow_eDrawType color_d_type,
	int t_size, glow_eAnnotType a_type,
	int rel_pos, glow_mDisplayLevel display_lev,
	int nodraw) :
	GlowAnnot(glow_ctx,x,y,annot_num,d_type,color_d_type,t_size,a_type,
	rel_pos,display_lev),
    	hot(0), pzero(ctx), highlight(0), inverse(0), 
	text( ctx, "", x, y, d_type, color_d_type, t_size),
	rect( ctx, x, y - ctx->draw_delta, ctx->draw_delta, ctx->draw_delta,
	glow_eDrawType_LineGray),
	user_data(NULL), adjustment(glow_eAdjustment_Left)
{ 
  strcpy( n_name, name);
  pzero.nav_zoom();

  sprintf( text.text, "A%d", annot_num);
  if ( ctx->grid_on) {
    double x_grid, y_grid;

    ctx->find_grid( p.x, p.y, &x_grid, &y_grid);
    p.posit( x_grid, y_grid);
    text.p.posit( p.x, p.y);
    rect.ll.posit( p.x, p.y - ctx->draw_delta);
    rect.ur.posit( p.x + ctx->draw_delta, p.y);
  }
  if ( !nodraw) {
    rect.draw( &ctx->mw, (void *)&pzero, highlight, hot, NULL);
    text.draw( &ctx->mw, (void *)&pzero, highlight, hot, NULL);
  }
  get_node_borders();
}

GrowSubAnnot::~GrowSubAnnot()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase( &ctx->mw);
  erase( &ctx->navw);

  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
  if ( hot)
    ctx->gdraw->set_cursor( &ctx->mw, glow_eDrawCursor_Normal);
}

void GrowSubAnnot::move( double delta_x, double delta_y, int grid)
{
  double x1, y1;

  ctx->set_defered_redraw();
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    erase( &ctx->mw);
    erase( &ctx->navw);
    ctx->find_grid( x_left + double( delta_x) / ctx->mw.zoom_factor_x,
	y_low + double( delta_y) / ctx->mw.zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
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
  x1 = trf.x( p.x, p.y);
  y1 = trf.y( p.x, p.y);
  rect.move( (void *)&pzero, x1, y1 - ctx->draw_delta, highlight, hot);
  text.move( (void *)&pzero, x1, y1, highlight, hot);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->redraw_defered();
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowSubAnnot::move_noerase( int delta_x, int delta_y, int grid)
{
  double x1, y1;
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    ctx->find_grid( x_left + double( delta_x) / ctx->mw.zoom_factor_x,
	y_low + double( delta_y) / ctx->mw.zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    dx = double( delta_x) / ctx->mw.zoom_factor_x;
    dy = double( delta_y) / ctx->mw.zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  x1 = trf.x( p.x, p.y);
  y1 = trf.y( p.x, p.y);
  rect.move( (void *)&pzero, x1, y1 - ctx->draw_delta, highlight, hot);
  text.move( (void *)&pzero, x1, y1, highlight, hot);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw,  x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

int GrowSubAnnot::event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;

  sts = 0;
  if ( event == ctx->event_move_node) {
    sts = rect.event_handler( w, (void *)&pzero, 
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
        sts = rect.event_handler( w, (void *)&pzero, event, x, y, NULL);
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
      sts = rect.event_handler( w, (void *)&pzero, 
		event, x, y, NULL);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}


void GrowSubAnnot::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  if ( mode == glow_eSaveMode_SubGraph) {
    // Save as GrowAnnot
    fp << int(glow_eSave_GrowAnnot) << endl;
    fp << int(glow_eSave_GrowAnnot_annot_part) << endl;
    GlowAnnot::save( fp, mode);
    fp << int(glow_eSave_GrowAnnot_trf) << endl;
    trf.save( fp, mode);
    fp << int(glow_eSave_GrowAnnot_adjustment) << FSPACE << int(adjustment) << endl;
    fp << int(glow_eSave_End) << endl;
  }
  else {
    fp << int(glow_eSave_GrowSubAnnot) << endl;
    fp << int(glow_eSave_GrowSubAnnot_n_name) << FSPACE << n_name << endl;
    fp << int(glow_eSave_GrowSubAnnot_x_right) << FSPACE << x_right << endl;
    fp << int(glow_eSave_GrowSubAnnot_x_left) << FSPACE << x_left << endl;
    fp << int(glow_eSave_GrowSubAnnot_y_high) << FSPACE << y_high << endl;
    fp << int(glow_eSave_GrowSubAnnot_y_low) << FSPACE << y_low << endl;
    fp << int(glow_eSave_GrowSubAnnot_text) << endl;
    text.save( fp, mode);
    fp << int(glow_eSave_GrowSubAnnot_rect) << endl;
    rect.save( fp, mode);
    fp << int(glow_eSave_GrowSubAnnot_annot_part) << endl;
    GlowAnnot::save( fp, mode);
    fp << int(glow_eSave_GrowSubAnnot_trf) << endl;
    trf.save( fp, mode);
    fp << int(glow_eSave_GrowSubAnnot_adjustment) << FSPACE << int(adjustment) << endl;
    fp << int(glow_eSave_End) << endl;
  }
}

void GrowSubAnnot::open( ifstream& fp)
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
      printf( "** Read error GrowSubAnnot: \"%d %s\"\n", type, dummy);      
    }

    fp >> type;
    switch( type) {
      case glow_eSave_GrowSubAnnot: break;
      case glow_eSave_GrowSubAnnot_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowSubAnnot_x_right: fp >> x_right; break;
      case glow_eSave_GrowSubAnnot_x_left: fp >> x_left; break;
      case glow_eSave_GrowSubAnnot_y_high: fp >> y_high; break;
      case glow_eSave_GrowSubAnnot_y_low: fp >> y_low; break;
      case glow_eSave_GrowSubAnnot_text: text.open( fp); break;
      case glow_eSave_GrowSubAnnot_rect: rect.open( fp); break;
      case glow_eSave_GrowSubAnnot_annot_part: 
        GlowAnnot::open( fp);
        break;
      case glow_eSave_GrowSubAnnot_trf: trf.open( fp); break;
      case glow_eSave_GrowSubAnnot_adjustment: fp >> tmp; adjustment = (glow_eAdjustment)tmp; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowSubAnnot:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowSubAnnot::draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y) 
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

void GrowSubAnnot::draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	tmp;
  int 	obj_ur_x = int( x_right * w->zoom_factor_x) - w->offset_x;
  int	obj_ll_x = int( x_left * w->zoom_factor_x) - w->offset_x;
  int	obj_ur_y = int( y_high * w->zoom_factor_y) - w->offset_y;
  int   obj_ll_y = int( y_low * w->zoom_factor_y) - w->offset_y;


  if ( *ll_x > *ur_x)
  {
    /* Shift */
    tmp = *ll_x;
    *ll_x = *ur_x;
    *ur_x = tmp;
  }
  if ( *ll_y > *ur_y)
  {
    /* Shift */
    tmp = *ll_y;
    *ll_y = *ur_y;
    *ur_y = tmp;
  }

  if (  obj_ur_x >= *ll_x &&
      	obj_ll_x <= *ur_x &&
       	obj_ur_y >= *ll_y &&
       	obj_ll_y <= *ur_y)
  {
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

void GrowSubAnnot::set_highlight( int on)
{
  highlight = on;
  rect.draw( &ctx->mw, (void *)&pzero, highlight, hot, NULL);
  text.draw( &ctx->mw, (void *)&pzero, highlight, hot, NULL);
  rect.draw( &ctx->navw, (void *)&pzero, highlight, 0, NULL);
  text.draw( &ctx->navw, (void *)&pzero, highlight, 0, NULL);
}

void GrowSubAnnot::select_region_insert( double ll_x, double ll_y, double ur_x, 
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

void GrowSubAnnot::zoom()
{
  GlowAnnot::zoom();
  rect.zoom();
  text.zoom();
}

void GrowSubAnnot::nav_zoom()
{
  GlowAnnot::nav_zoom();
  rect.nav_zoom();
  text.nav_zoom();
}

void GrowSubAnnot::draw( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node,
			 void *colornode)
{
  if ( w == &ctx->navw) {
    return;
  }
  if ( t) {
    GlowPoint p1(p);

    p1.x = trf.x( t, p.x, p.y) - trf.x( p.x, p.y);
    p1.y = trf.y( t, p.x, p.y) - trf.y( p.x, p.y);
    p1.zoom();
    rect.draw( w, (void *)&p1, highlight, hot, NULL);
    text.draw( w, (void *)&p1, highlight, hot, NULL);
  }
  else {
    rect.draw( w, (void *)&pzero, highlight, hot, NULL);
    text.draw( w, (void *)&pzero, highlight, hot, NULL);
  }
}

void GrowSubAnnot::erase( GlowWind *w, GlowTransform *t, int hot, void *node)
{
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  w->set_draw_buffer_only();
  if ( t) {
    GlowPoint p1(p);

    p1.x = trf.x( t, p.x, p.y) - trf.x( p.x, p.y);
    p1.y = trf.y( t, p.x, p.y) - trf.y( p.x, p.y);
    p1.zoom();
    rect.erase( w, (void *)&p1, hot, NULL);
    text.erase( w, (void *)&p1, hot, NULL);
  }
  else {
    rect.erase( w, (void *)&pzero, hot, NULL);
    text.erase( w, (void *)&pzero, hot, NULL);
  }
  w->reset_draw_buffer_only();
}

void GrowSubAnnot::set_transform( GlowTransform *t)
{
  double x1, y1;

  trf = *t * trf;

  x1 = trf.x( p.x, p.y);
  y1 = trf.y( p.x, p.y);
  rect.move( (void *)&pzero, x1, y1 - ctx->draw_delta, highlight, hot);
  text.move( (void *)&pzero, x1, y1, highlight, hot);

  get_node_borders();

}

void GrowSubAnnot::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy, x1, y1;

    erase( &ctx->mw);
    erase( &ctx->navw);
    ctx->set_defered_redraw();
    draw();
    switch ( direction)
    {
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

    x1 = trf.x( p.x, p.y);
    y1 = trf.y( p.x, p.y);
    rect.move( (void *)&pzero, x1, y1 - ctx->draw_delta, highlight, hot);
    text.move( (void *)&pzero, x1, y1, highlight, hot);
    draw();
    ctx->redraw_defered();
}


void GrowSubAnnot::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  int x1, y1;
  int bold;

  double trf_scale = trf.vertical_scale( t);
  int idx = int( trf_scale * ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * (text_size +4) - 4);
  if ( idx < 0)
    return;
  idx = min( idx, DRAW_TYPE_SIZE-1);

  if (!t) {
    x1 = int( trf.x( p.x, p.y) * ctx->mw.zoom_factor_x) - ctx->mw.offset_x;
    y1 = int( trf.y( p.x, p.y) * ctx->mw.zoom_factor_y) - ctx->mw.offset_y;
  }
  else {
    x1 = int( trf.x( t, p.x, p.y) * ctx->mw.zoom_factor_x) - ctx->mw.offset_x;
    y1 = int( trf.y( t, p.x, p.y) * ctx->mw.zoom_factor_y) - ctx->mw.offset_y;
  }

  bold = (draw_type == glow_eDrawType_TextHelveticaBold);

  ctx->export_jbean->annot( x1, y1, number,
	draw_type, color_drawtype, bold, adjustment, idx, pass, shape_cnt, node_cnt, fp);
  (*shape_cnt)++;
}

void GrowSubAnnot::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);
    text.draw_type = GlowColor::convert( version, text.draw_type);
    break;
  }
  }  
}

void GrowSubAnnot::draw()
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

void GrowSubAnnot::set_textbold( int bold) 
{ 
  if ( ( bold && draw_type == glow_eDrawType_TextHelveticaBold) ||
       ( !bold && draw_type == glow_eDrawType_TextHelvetica))
    return;

  erase( &ctx->mw);
  erase( &ctx->navw);
  if ( bold)
    draw_type = glow_eDrawType_TextHelveticaBold;
  else
    draw_type = glow_eDrawType_TextHelvetica;
  get_node_borders();
  draw();
}

void GrowSubAnnot::export_flow( GlowExportFlow *ef) 
{
  ef->annot( this);
}
