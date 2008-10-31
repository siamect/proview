/* 
 * Proview   $Id: glow_growarc.cpp,v 1.7 2008-10-31 12:51:35 claes Exp $
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
#include "glow_growarc.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_exportflow.h"

GrowArc::GrowArc( GrowCtx *glow_ctx, const char *name, double x1, double y1, 
		  double x2, double y2, int ang1, int ang2,
		  glow_eDrawType border_d_type, int line_w, 
		  int fill_arc, int display_border, int display_shadow,
		  glow_eDrawType fill_d_type, int nodraw) : 
  GlowArc(glow_ctx,x1,y1,x2,y2,ang1,ang2,border_d_type,line_w, fill_arc), 
  hot(0), pzero(ctx), highlight(0), inverse(0), user_data(NULL),
  original_border_drawtype(border_d_type),
  original_fill_drawtype(fill_d_type), fill_drawtype(fill_d_type),
  border(display_border),
  dynamic(0), dynamicsize(0), shadow(display_shadow), shadow_width(5), 
  relief(glow_eRelief_Up), shadow_contrast(2), disable_shadow(0)
{ 
  strcpy( n_name, name);
  pzero.nav_zoom();
  strcpy( last_group, "");

  if ( ctx->grid_on)
  {
    double x_grid, y_grid;

    ctx->find_grid( ll.x, ll.y, &x_grid, &y_grid);
    ll.posit( x_grid, y_grid);
    ctx->find_grid( ur.x, ur.y, &x_grid, &y_grid);
    ur.posit( x_grid, y_grid);
  }
  if ( !nodraw)
    draw( &ctx->mw, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  get_node_borders();
}

GrowArc::~GrowArc()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase( &ctx->mw);
  erase( &ctx->navw);

  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw(  &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
  if ( hot)
    ctx->gdraw->set_cursor( &ctx->mw, glow_eDrawCursor_Normal);
}

void GrowArc::move( double delta_x, double delta_y, int grid)
{
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
    ctx->find_grid( x_left + delta_x / ctx->mw.zoom_factor_x,
	y_low + delta_y / ctx->mw.zoom_factor_y, &x_grid, &y_grid);
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

void GrowArc::move_noerase( int delta_x, int delta_y, int grid)
{
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
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);

}

int GrowArc::local_event_handler( glow_eEvent event, double x, double y)
{
  double ll_x, ur_x, ll_y, ur_y;

  ll_x = min( ll.x, ur.x);
  ur_x = max( ll.x, ur.x);
  ll_y = min( ll.y, ur.y);
  ur_y = max( ll.y, ur.y);

  if ( ll_x <= x && x <= ur_x &&
       ll_y <= y && y <= ur_y)
    return 1;
  else
    return 0;
}

int GrowArc::event_handler( GlowWind *w, glow_eEvent event, double fx, double fy)
{
  double x, y;

  trf.reverse( fx, fy, &x, &y);
  return local_event_handler( event, x, y);
}

int GrowArc::event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;

  double rx, ry;

  // Convert koordinates to local koordinates
  trf.reverse( fx, fy, &rx, &ry);

  sts = 0;
  if ( event == ctx->event_move_node) {
    sts = local_event_handler( event, rx, ry);
    if ( sts)
    {
      /* Register node for potential movement */
      ctx->move_insert( this);
    }
    return sts;
  }
  switch ( event) {
    case glow_eEvent_CursorMotion: {
      int redraw = 0;

      if ( ctx->hot_mode == glow_eHotMode_TraceAction)
        sts = 0;
      else if ( ctx->hot_found)
        sts = 0;
      else {
        sts = local_event_handler( event, rx, ry);
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
        if ( !ctx->hot_found)
          ctx->gdraw->set_cursor( w, glow_eDrawCursor_Normal);
        erase( w);
        hot = 0;
        redraw = 1;
      }
      if ( redraw) {
        ctx->draw( w, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
      }
      break;
    }
    default:
      sts = local_event_handler( event, rx, ry);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}


void GrowArc::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  char *s;

  fp << int(glow_eSave_GrowArc) << endl;
  fp << int(glow_eSave_GrowArc_n_name) << FSPACE << n_name << endl;
  fp << int(glow_eSave_GrowArc_x_right) << FSPACE << x_right << endl;
  fp << int(glow_eSave_GrowArc_x_left) << FSPACE << x_left << endl;
  fp << int(glow_eSave_GrowArc_y_high) << FSPACE << y_high << endl;
  fp << int(glow_eSave_GrowArc_y_low) << FSPACE << y_low << endl;
  fp << int(glow_eSave_GrowArc_original_border_drawtype) << FSPACE 
		<< int(original_border_drawtype) << endl;
  fp << int(glow_eSave_GrowArc_original_fill_drawtype) << FSPACE 
		<< int(original_fill_drawtype) << endl;
  fp << int(glow_eSave_GrowArc_fill_drawtype) << FSPACE << int(fill_drawtype) << endl;
  fp << int(glow_eSave_GrowArc_border) << FSPACE << border << endl;
  fp << int(glow_eSave_GrowArc_shadow) << FSPACE << shadow << endl;
  fp << int(glow_eSave_GrowArc_shadow_width) << FSPACE << shadow_width << endl;
  fp << int(glow_eSave_GrowArc_shadow_contrast) << FSPACE << shadow_contrast << endl;
  fp << int(glow_eSave_GrowArc_relief) << FSPACE << int(relief) << endl;
  fp << int(glow_eSave_GrowArc_disable_shadow) << FSPACE << disable_shadow << endl;
  fp << int(glow_eSave_GrowArc_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_GrowArc_dynamic) << endl;
  if( dynamic)
  {
    fp << "\"";
    for ( s  = dynamic; *s; s++)
    {
      if ( *s == '"')
        fp << "\\";
      fp << *s;
    }
    fp << "\"" << endl;
  }
  fp << int(glow_eSave_GrowArc_arc_part) << endl;
  GlowArc::save( fp, mode);
  fp << int(glow_eSave_GrowArc_trf) << endl;
  trf.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GrowArc::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;
  int		j;
  char		c;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowArc: break;
      case glow_eSave_GrowArc_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowArc_x_right: fp >> x_right; break;
      case glow_eSave_GrowArc_x_left: fp >> x_left; break;
      case glow_eSave_GrowArc_y_high: fp >> y_high; break;
      case glow_eSave_GrowArc_y_low: fp >> y_low; break;
      case glow_eSave_GrowArc_original_border_drawtype: fp >> 
		tmp; original_border_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowArc_original_fill_drawtype: fp >> 
		tmp; original_fill_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowArc_fill_drawtype: fp >> 
		tmp; fill_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowArc_border: fp >> border; break;
      case glow_eSave_GrowArc_shadow: fp >> shadow; break;
      case glow_eSave_GrowArc_shadow_width: fp >> shadow_width; break;
      case glow_eSave_GrowArc_shadow_contrast: fp >> shadow_contrast; break;
      case glow_eSave_GrowArc_relief: fp >> tmp; relief = (glow_eRelief)tmp; break;
      case glow_eSave_GrowArc_disable_shadow: fp >> disable_shadow; break;
      case glow_eSave_GrowArc_dynamicsize: fp >> dynamicsize; break;
      case glow_eSave_GrowArc_dynamic:
        fp.getline( dummy, sizeof(dummy));
        if ( dynamicsize)
        {
          dynamic = (char *) calloc( 1, dynamicsize);
	  fp.get();
          for ( j = 0; j < dynamicsize; j++)
	  {
	    if ((c = fp.get()) == '"')
	    {
	      if ( dynamic[j-1] == '\\')
	        j--;
	      else
              {
	        dynamic[j] = 0;
                break;
              }
	    }
            dynamic[j] = c;
	  }
          fp.getline( dummy, sizeof(dummy));
        }
        break;
      case glow_eSave_GrowArc_arc_part: 
        GlowArc::open( fp);
        break;
      case glow_eSave_GrowArc_trf: trf.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowArc:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowArc::draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y) 
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
       y_low * w->zoom_factor_y - w->offset_y <= ur_y) {
    draw( w, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  }
}

void GrowArc::draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowArc::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowArc::select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy)
{
  if ( select_policy == glow_eSelectPolicy_Surround )
  {
    if ( x_left > ll_x && x_right < ur_x && y_high < ur_y && y_low > ll_y)
      ctx->select_insert( this);
  }
  else
  {
    if ( x_right > ll_x && x_left < ur_x && y_low < ur_y && y_high > ll_y)
      ctx->select_insert( this);
  }
}

void GrowArc::set_dynamic( char *code, int size)
{
  if ( !dynamic)
  {
    dynamic = (char *) calloc( 1, size+1);
    dynamicsize = size+1;
  }
  else if ( dynamicsize < size+1)
  {
    free( dynamic);
    dynamic = (char *) calloc( 1, size+1);
    dynamicsize = size+1;
  }
  strncpy( dynamic, code, size+1);
}

void GrowArc::exec_dynamic()
{
  if ( dynamicsize && strcmp( dynamic, "") != 0)
    ctx->dynamic_cb( this, dynamic, glow_eDynamicType_Object);
}

void GrowArc::set_position( double x, double y)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.a13 == x && trf.a23 == y)
     return;
  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase( &ctx->mw);
  erase( &ctx->navw);
  trf.posit( x, y);
  get_node_borders();
  ctx->draw( &ctx->mw, old_x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     old_y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     old_x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     old_y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);

}

void GrowArc::set_scale( double scale_x, double scale_y, 
	double x0, double y0, glow_eScaleType type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.s_a11 && trf.s_a22 &&
       fabs( scale_x - trf.a11 / trf.s_a11) < FLT_EPSILON &&
       fabs( scale_y - trf.a22 / trf.s_a22) < FLT_EPSILON)
    return;

  switch( type)
  {
    case glow_eScaleType_LowerLeft:
      x0 = x_left;
      y0 = y_low;
      break;
    case glow_eScaleType_LowerRight:
      x0 = x_right;
      y0 = y_low;
      break;
    case glow_eScaleType_UpperRight:
      x0 = x_right;
      y0 = y_high;
      break;
    case glow_eScaleType_UpperLeft:
      x0 = x_left;
      y0 = y_high;
      break;
    case glow_eScaleType_FixPoint:
      break;
    case glow_eScaleType_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_low + y_high) /2;
      break;
    default:
      ;
  }

  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase( &ctx->mw);
  erase( &ctx->navw);
  trf.scale_from_stored( scale_x, scale_y, x0, y0);
  get_node_borders();

  switch( type)
  {
    case glow_eScaleType_LowerLeft:
      x_left = old_x_left;
      y_low = old_y_low;
      break;
    case glow_eScaleType_LowerRight:
      x_right = old_x_right;
      y_low = old_y_low;
      break;
    case glow_eScaleType_UpperRight:
      x_right = old_x_right;
      y_high = old_y_high;
      break;
    case glow_eScaleType_UpperLeft:
      x_left = old_x_left;
      y_high = old_y_high;
      break;
    case glow_eScaleType_FixPoint:
      break;
    case glow_eScaleType_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_low + y_high) /2;
      break;
    default:
      ;
  }
  ctx->draw( &ctx->mw, old_x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     old_y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     old_x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     old_y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowArc::set_rotation( double angle, 
	double x0, double y0, glow_eRotationPoint type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( fabs( angle - trf.rotation + trf.s_rotation) < FLT_EPSILON)
    return;

  switch( type) {
    case glow_eRotationPoint_LowerLeft:
      x0 = x_left;
      y0 = y_low;
      break;
    case glow_eRotationPoint_LowerRight:
      x0 = x_right;
      y0 = y_low;
      break;
    case glow_eRotationPoint_UpperRight:
      x0 = x_right;
      y0 = y_high;
      break;
    case glow_eRotationPoint_UpperLeft:
      x0 = x_left;
      y0 = y_high;
      break;
    case glow_eRotationPoint_Center:
      x0 = (x_left + x_right) / 2;
      y0 = (y_high + y_low) / 2;
      break;
    default:
      ;
  }

  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase( &ctx->mw);
  erase( &ctx->navw);
  trf.rotate_from_stored( angle, x0, y0);
  get_node_borders();
  ctx->draw( &ctx->mw, old_x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     old_y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     old_x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     old_y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowArc::draw( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node, void *colornode)
{
  glow_eDrawType drawtype;
  int idx;

  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }

  if ( node && ((GrowNode *)node)->line_width)
    idx = int( w->zoom_factor_y / w->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, rot;

  if (!t) {
    x1 = int( trf.x( ll.x, ll.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * w->zoom_factor_y + 0.5) - w->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * w->zoom_factor_y + 0.5) - w->offset_y;
    rot = int( trf.rot());
  }
  else {
    x1 = int( trf.x( t, ll.x, ll.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * w->zoom_factor_y + 0.5) - w->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * w->zoom_factor_y + 0.5) - w->offset_y;
    rot = int( trf.rot( t));
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  if ( fill)
  {
    int display_shadow = ((node && ((GrowNode *)node)->shadow) || shadow) && !disable_shadow;
    glow_eDrawType fillcolor = ctx->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
		 highlight, (GrowNode *)colornode, 1);
    
    if ( !display_shadow || shadow_width == 0 || angle2 != 360) {
      ctx->gdraw->fill_arc( w,  ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			  angle1 - rot, angle2, fillcolor, 0);
    }
    else {
      int ish = int( shadow_width / 100 * min(ur_x - ll_x, ur_y - ll_y) + 0.5);

      int drawtype_incr = shadow_contrast;
      if ( relief == glow_eRelief_Down)
	drawtype_incr = -shadow_contrast;

      // Draw light shadow
      drawtype = ctx->shift_drawtype( fillcolor, -drawtype_incr, (GrowNode *)colornode);
      
      ctx->gdraw->fill_arc( w,  ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			  35, 140, drawtype, 0);

      // Draw dark shadow
      drawtype = ctx->shift_drawtype( fillcolor, drawtype_incr, (GrowNode *)colornode);
      
      ctx->gdraw->fill_arc( w,  ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			  215, 140, drawtype, 0);

      // Draw medium shadow and body
      ctx->gdraw->fill_arc( w,  ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			  -5, 40, fillcolor, 0);
      ctx->gdraw->fill_arc( w,  ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
			  175, 40, fillcolor, 0);

      ctx->gdraw->fill_arc( w,  ll_x + ish, ll_y + ish, ur_x - ll_x - 2*ish, ur_y - ll_y - 2*ish,
			  angle1 - rot, angle2, fillcolor, 0);
    }
  }
  if ( border || !fill)
  {
    drawtype = ctx->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);
    ctx->gdraw->arc( w,  ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, 
	angle1 - rot, angle2, drawtype, idx, 0);
  }
}

void GrowArc::erase( GlowWind *w, GlowTransform *t, int hot, void *node)
{
  int idx;

  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }

  if ( node && ((GrowNode *)node)->line_width)
    idx = int( w->zoom_factor_y / w->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, rot;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * w->zoom_factor_y + 0.5) - w->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * w->zoom_factor_y + 0.5) - w->offset_y;
    rot = int( trf.rot());
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * w->zoom_factor_y + 0.5) - w->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * w->zoom_factor_x + 0.5) - w->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * w->zoom_factor_y + 0.5) - w->offset_y;
    rot = int( trf.rot( t));
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  w->set_draw_buffer_only();
  if ( border || !fill)
    ctx->gdraw->arc_erase( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	angle1 - rot, angle2, idx);
  if ( fill)
    ctx->gdraw->fill_arc( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	angle1 - rot, angle2, glow_eDrawType_LineErase, 0);
  w->reset_draw_buffer_only();
}

void GrowArc::draw()
{
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
}

void GrowArc::get_borders( GlowTransform *t, double *x_right, 
	double *x_left, double *y_high, double *y_low)
{
  double ll_x, ur_x, ll_y, ur_y, x1, x2, y1, y2;

  if ( t)
  {
    x1 = trf.x( t, ll.x, ll.y);
    x2 = trf.x( t, ur.x, ur.y);
    y1 = trf.y( t, ll.x, ll.y);
    y2 = trf.y( t, ur.x, ur.y);
  }
  else
  {
    x1 = trf.x( ll.x, ll.y);
    x2 = trf.x( ur.x, ur.y);
    y1 = trf.y( ll.x, ll.y);
    y2 = trf.y( ur.x, ur.y);
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  if ( ll_x < *x_left)
    *x_left = ll_x;
  if ( ur_x > *x_right)
    *x_right = ur_x;
  if ( ll_y < *y_low)
    *y_low = ll_y;
  if ( ur_y > *y_high)
    *y_high = ur_y;
}

void GrowArc::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();
}

void GrowArc::set_linewidth( int linewidth)
{
  erase( &ctx->mw);
  erase( &ctx->navw);
  GlowArc::set_linewidth( linewidth);
  draw();
}

void GrowArc::set_fill( int fillval)
{
  erase( &ctx->mw);
  erase( &ctx->navw);
  GlowArc::set_fill( fillval);
  draw();
}

void GrowArc::set_border( int borderval)
{
  erase( &ctx->mw);
  erase( &ctx->navw);
  border = borderval;
  draw();
}

void GrowArc::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy;

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

    draw();
    ctx->redraw_defered();
}

void GrowArc::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  int idx;
  if ( node && ((GrowNode *)node)->line_width)
    idx = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
  else
    idx = int( ctx->mw.zoom_factor_y / ctx->mw.base_zoom_factor * line_width - 1);
  idx += hot;
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  double x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y, rot;

  if (!t)
  {
    x1 = trf.x( ll.x, ll.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = trf.y( ll.x, ll.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = trf.x( ur.x, ur.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = trf.y( ur.x, ur.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    rot = trf.rot();
  }
  else
  {
    x1 = trf.x( t, ll.x, ll.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y1 = trf.y( t, ll.x, ll.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    x2 = trf.x( t, ur.x, ur.y) * ctx->mw.zoom_factor_x - ctx->mw.offset_x;
    y2 = trf.y( t, ur.x, ur.y) * ctx->mw.zoom_factor_y - ctx->mw.offset_y;
    rot = trf.rot( t);
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  double ish;
  if ( !disable_shadow) 
    ish = shadow_width / 100 * min(ur_x - ll_x, ur_y - ll_y);
  else
    ish = 0;

  int drawtype_incr = shadow_contrast;
  if ( relief == glow_eRelief_Down)
    drawtype_incr = -shadow_contrast;

  ctx->export_jbean->arc( ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	angle1 - rot, angle2, fill, border || !fill, fill_drawtype, draw_type,
	idx, ish, shadow, drawtype_incr, pass, shape_cnt, node_cnt, fp);
}

void GrowArc::flip( double x0, double y0, glow_eFlipDirection dir)
{
  switch ( dir) {
  case glow_eFlipDirection_Horizontal:
    trf.store();
    set_scale( 1, -1, x0, y0, glow_eScaleType_FixPoint);
    break;
  case glow_eFlipDirection_Vertical:
    trf.store();
    set_scale( -1, 1, x0, y0, glow_eScaleType_FixPoint);
    break;
  }
}

void GrowArc::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);
    original_border_drawtype = GlowColor::convert( version, original_border_drawtype);
    original_fill_drawtype = GlowColor::convert( version, original_fill_drawtype);
    fill_drawtype = GlowColor::convert( version, fill_drawtype);

    break;
  }
  }  
}

void GrowArc::export_flow( GlowExportFlow *ef) 
{
  ef->arc( this);
}
