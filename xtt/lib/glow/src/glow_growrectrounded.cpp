/* 
 * Proview   $Id: glow_growrectrounded.cpp,v 1.2 2005-09-01 14:57:54 claes Exp $
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
#include "glow_growrectrounded.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"

GrowRectRounded::GrowRectRounded( GlowCtx *glow_ctx, char *name, double x, double y, 
		double w, double h, glow_eDrawType border_d_type, int line_w, 
		int fix_line_w, glow_mDisplayLevel display_lev,
		int fill_rect, int display_border, int display_shadow,
		glow_eDrawType fill_d_type, int nodraw) : 
		GlowRect(glow_ctx,x,y,w,h,border_d_type,line_w,fix_line_w,display_lev,
		fill_rect), 
    		hot(0), pzero(ctx), highlight(0), inverse(0), user_data(NULL),
		original_border_drawtype(border_d_type),
		original_fill_drawtype(fill_d_type), fill_drawtype(fill_d_type),
		border(display_border),
		dynamic(0), dynamicsize(0), round_amount(15), shadow(display_shadow), 
		shadow_width(5), relief(glow_eRelief_Up), shadow_contrast(2), disable_shadow(0)
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
    draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  get_node_borders();
}

GrowRectRounded::~GrowRectRounded()
{
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase();
  nav_erase();

  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  if ( hot)
    draw_set_cursor( ctx, glow_eDrawCursor_Normal);
}

void GrowRectRounded::move( double delta_x, double delta_y, int grid)
{
  ctx->set_defered_redraw();
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    erase();
    nav_erase();
    ctx->find_grid( x_left + delta_x / ctx->zoom_factor_x,
	y_low + delta_y / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    erase();
    nav_erase();
    dx = delta_x / ctx->zoom_factor_x;
    dy = delta_y / ctx->zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->redraw_defered();
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowRectRounded::move_noerase( int delta_x, int delta_y, int grid)
{
  if ( grid)
  {
    double x_grid, y_grid;

    /* Move to closest grid point */
    ctx->find_grid( x_left + double( delta_x) / ctx->zoom_factor_x,
	y_low + double( delta_y) / ctx->zoom_factor_y, &x_grid, &y_grid);
    trf.move( x_grid - x_left, y_grid - y_low);
    get_node_borders();
  }
  else
  {
    double dx, dy;

    dx = double( delta_x) / ctx->zoom_factor_x;
    dy = double( delta_y) / ctx->zoom_factor_y;
    trf.move( dx, dy);
    x_right += dx;
    x_left += dx;
    y_high += dy;
    y_low += dy;
  }
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

int GrowRectRounded::local_event_handler( glow_eEvent event, double x, double y)
{
  double ll_x, ur_x, ll_y, ur_y;

  ll_x = min( ll.x, ur.x);
  ur_x = max( ll.x, ur.x);
  ll_y = min( ll.y, ur.y);
  ur_y = max( ll.y, ur.y);

  if ( ll_x <= x && x <= ur_x &&
       ll_y <= y && y <= ur_y)
  {
//    cout << "Event handler: Hit in rect" << endl;
    return 1;
  }  
  else
    return 0;
}

int GrowRectRounded::event_handler( glow_eEvent event, double fx, double fy)
{
  double x, y;

  trf.reverse( fx, fy, &x, &y);
  return local_event_handler( event, x, y);
}

int GrowRectRounded::event_handler( glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;

  double rx, ry;

  // Convert koordinates to local koordinates
  trf.reverse( fx, fy, &rx, &ry);

  sts = 0;
  if ( event == ctx->event_move_node)
  {
    sts = local_event_handler( event, rx, ry);
    if ( sts)
    {
      /* Register node for potential movement */
      ctx->move_insert( this);
    }
    return sts;
  }
  switch ( event)
  {
    case glow_eEvent_CursorMotion:
    {
      int redraw = 0;

      if ( ctx->hot_mode == glow_eHotMode_TraceAction)
        sts = 0;
      else if ( ctx->hot_found)
        sts = 0;
      else
      {
        sts = local_event_handler( event, rx, ry);
        if ( sts)
          ctx->hot_found = 1;
      }
      if ( sts && !hot  &&
	   !(ctx->node_movement_active || ctx->node_movement_paste_active))
      {
        draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);
        hot = 1;
        redraw = 1;
      }
      if ( !sts && hot)
      {
        if ( !ctx->hot_found)
          draw_set_cursor( ctx, glow_eDrawCursor_Normal);
        erase();
        hot = 0;
        redraw = 1;
      }
      if ( redraw)
      {
        if ( !inverse)
          ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
//          ((GlowRect *)this)->draw( (void *)&pzero, highlight, hot, NULL);
        else
          draw_inverse();
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


void GrowRectRounded::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  char *s;

  fp << int(glow_eSave_GrowRectRounded) << endl;
  fp << int(glow_eSave_GrowRectRounded_n_name) << FSPACE << n_name << endl;
  fp << int(glow_eSave_GrowRectRounded_x_right) << FSPACE << x_right << endl;
  fp << int(glow_eSave_GrowRectRounded_x_left) << FSPACE << x_left << endl;
  fp << int(glow_eSave_GrowRectRounded_y_high) << FSPACE << y_high << endl;
  fp << int(glow_eSave_GrowRectRounded_y_low) << FSPACE << y_low << endl;
  fp << int(glow_eSave_GrowRectRounded_original_border_drawtype) << FSPACE 
		<< int(original_border_drawtype) << endl;
  fp << int(glow_eSave_GrowRectRounded_original_fill_drawtype) << FSPACE 
		<< int(original_fill_drawtype) << endl;
  fp << int(glow_eSave_GrowRectRounded_fill_drawtype) << FSPACE << int(fill_drawtype) << endl;
  fp << int(glow_eSave_GrowRectRounded_border) << FSPACE << border << endl;
  fp << int(glow_eSave_GrowRectRounded_round_amount) << FSPACE << round_amount << endl;
  fp << int(glow_eSave_GrowRectRounded_shadow_width) << FSPACE << shadow_width << endl;
  fp << int(glow_eSave_GrowRectRounded_shadow) << FSPACE << shadow << endl;
  fp << int(glow_eSave_GrowRectRounded_shadow_contrast) << FSPACE << shadow_contrast << endl;
  fp << int(glow_eSave_GrowRectRounded_relief) << FSPACE << int(relief) << endl;
  fp << int(glow_eSave_GrowRectRounded_disable_shadow) << FSPACE << disable_shadow << endl;
  fp << int(glow_eSave_GrowRectRounded_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_GrowRectRounded_dynamic) << endl;
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
  fp << int(glow_eSave_GrowRectRounded_rect_part) << endl;
  GlowRect::save( fp, mode);
  fp << int(glow_eSave_GrowRectRounded_trf) << endl;
  trf.save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GrowRectRounded::open( ifstream& fp)
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
      case glow_eSave_GrowRectRounded: break;
      case glow_eSave_GrowRectRounded_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_GrowRectRounded_x_right: fp >> x_right; break;
      case glow_eSave_GrowRectRounded_x_left: fp >> x_left; break;
      case glow_eSave_GrowRectRounded_y_high: fp >> y_high; break;
      case glow_eSave_GrowRectRounded_y_low: fp >> y_low; break;
      case glow_eSave_GrowRectRounded_original_border_drawtype: fp >> 
		tmp; original_border_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowRectRounded_original_fill_drawtype: fp >> 
		tmp; original_fill_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowRectRounded_fill_drawtype: fp >> 
		tmp; fill_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowRectRounded_border: fp >> border; break;
      case glow_eSave_GrowRectRounded_round_amount: fp >> round_amount; break;
      case glow_eSave_GrowRectRounded_shadow: fp >> shadow; break;
      case glow_eSave_GrowRectRounded_shadow_width: fp >> shadow_width; break;
      case glow_eSave_GrowRectRounded_shadow_contrast: fp >> shadow_contrast; break;
      case glow_eSave_GrowRectRounded_relief: fp >> tmp; relief = (glow_eRelief)tmp; break;
      case glow_eSave_GrowRectRounded_disable_shadow: fp >> disable_shadow; break;
      case glow_eSave_GrowRectRounded_dynamicsize: fp >> dynamicsize; break;
      case glow_eSave_GrowRectRounded_dynamic:
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
      case glow_eSave_GrowRectRounded_rect_part: 
        GlowRect::open( fp);
        break;
      case glow_eSave_GrowRectRounded_trf: trf.open( fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowRectRounded:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GrowRectRounded::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int tmp;

  if ( ll_x > ur_x)
  {
    /* Shift */
    tmp = ll_x;
    ll_x = ur_x;
    ur_x = tmp;
  }
  if ( ll_y > ur_y)
  {
    /* Shift */
    tmp = ll_y;
    ll_y = ur_y;
    ur_y = tmp;
  }

  if ( x_right * ctx->zoom_factor_x - ctx->offset_x + 1 >= ll_x &&
      	x_left * ctx->zoom_factor_x - ctx->offset_x <= ur_x &&
       	y_high * ctx->zoom_factor_y - ctx->offset_y + 1 >= ll_y &&
       	y_low * ctx->zoom_factor_y - ctx->offset_y <= ur_y)
  {
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    else
      draw_inverse();
  }
}

void GrowRectRounded::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	tmp;
  int 	obj_ur_x = int( x_right * ctx->zoom_factor_x) - ctx->offset_x;
  int	obj_ll_x = int( x_left * ctx->zoom_factor_x) - ctx->offset_x;
  int	obj_ur_y = int( y_high * ctx->zoom_factor_y) - ctx->offset_y;
  int   obj_ll_y = int( y_low * ctx->zoom_factor_y) - ctx->offset_y;


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
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, NULL, NULL);
    else
      draw_inverse();

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

void GrowRectRounded::nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
{ 
  int 	obj_ur_x = int( x_right * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
  int	obj_ll_x = int( x_left * ctx->nav_zoom_factor_x) - ctx->nav_offset_x;
  int	obj_ur_y = int( y_high * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;
  int   obj_ll_y = int( y_low * ctx->nav_zoom_factor_y) - ctx->nav_offset_y;

  if (  obj_ur_x >= *ll_x &&
      	obj_ll_x <= *ur_x &&
       	obj_ur_y >= *ll_y &&
       	obj_ll_y <= *ur_y)
  {
    nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);

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

void GrowRectRounded::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int x_right_pix = int( x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int x_left_pix = int( x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x);
  int y_high_pix = int( y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y);
  int y_low_pix = int( y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y);

  if ( x_right_pix >= ll_x &&
       x_left_pix <= ur_x &&
       y_high_pix >= ll_y &&
       y_low_pix <= ur_y)
  {
    nav_draw( (GlowTransform *)NULL, highlight, NULL, NULL);
  }
}

void GrowRectRounded::set_highlight( int on)
{
  highlight = on;
  draw();
}

void GrowRectRounded::select_region_insert( double ll_x, double ll_y, double ur_x, 
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

void GrowRectRounded::set_dynamic( char *code, int size)
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

void GrowRectRounded::exec_dynamic()
{
  if ( dynamicsize && strcmp( dynamic, "") != 0)
    ((GrowCtx *)ctx)->dynamic_cb( this, dynamic, glow_eDynamicType_Object);
}

void GrowRectRounded::set_position( double x, double y)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.a13 == x && trf.a23 == y)
     return;
  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase();
  nav_erase();
  trf.posit( x, y);
  get_node_borders();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowRectRounded::set_scale( double scale_x, double scale_y, 
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
  erase();
  nav_erase();
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
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowRectRounded::set_rotation( double angel, 
	double x0, double y0, glow_eRotationPoint type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( fabs( angel - trf.rotation + trf.s_rotation) < FLT_EPSILON)
    return;

  switch( type)
  {
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
  erase();
  nav_erase();
  trf.rotate_from_stored( angel, x0, y0);
  get_node_borders();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}


void GrowRectRounded::draw()
{
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}
void GrowRectRounded::draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  glow_eDrawType drawtype;

  if ( fix_line_width)
  {
    idx = line_width;
    idx += hot;
    if ( idx < 0)
    {
      erase( t, hot, node);
      return;
    }
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);
  int amount = int( round_amount/100 * min(ur_x - ll_x, ur_y - ll_y) + 0.5);
  if ( fill)
  {
    int ish = int( shadow_width / 100 * min(ur_x - ll_x, ur_y - ll_y) + 0.5);
    int display_shadow = ((node && ((GrowNode *)node)->shadow) || shadow) && !disable_shadow;
    glow_eDrawType fillcolor = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
		 highlight, (GrowNode *)colornode, 1);

    if ( !display_shadow || ish == 0) {
      glow_draw_fill_rect( ctx, ll_x, ll_y + amount, ur_x - ll_x, ur_y - ll_y - 2*amount, fillcolor);
      glow_draw_fill_rect( ctx, ll_x + amount, ll_y, ur_x - ll_x - 2*amount, amount, fillcolor);
      glow_draw_fill_rect( ctx, ll_x + amount, ur_y - amount, ur_x - ll_x - 2*amount, amount, fillcolor);
      glow_draw_fill_arc( ctx,  ll_x, ll_y, 2*amount, 2*amount, 90, 90, 
			  fillcolor, 0);
      glow_draw_fill_arc( ctx,  ll_x, ur_y - 2*amount, 2*amount, 2*amount, 180, 90, 
			  fillcolor, 0);
      glow_draw_fill_arc( ctx,  ur_x - 2*amount, ur_y - 2*amount, 2*amount, 2*amount, 270, 90, 
			  fillcolor, 0);
      glow_draw_fill_arc( ctx,  ur_x - 2*amount, ll_y, 2*amount, 2*amount, 0, 90, 
			  fillcolor, 0);
    }
    else {

      int drawtype_incr = shadow_contrast;
      if ( relief == glow_eRelief_Down)
	drawtype_incr = -shadow_contrast;

      // Draw light shadow
      drawtype = ((GrowCtx *)ctx)->shift_drawtype( fillcolor, -drawtype_incr, (GrowNode *)colornode);
      glow_draw_fill_rect( ctx, ll_x + amount, ll_y, ur_x - ll_x - 2*amount, ish, drawtype);
      glow_draw_fill_rect( ctx, ll_x, ll_y + amount, ish, ur_y - ll_y - 2*amount, drawtype);
      glow_draw_fill_arc( ctx,  ll_x, ll_y, 2*amount, 2*amount, 90, 90, 
			  drawtype, 0);
      glow_draw_fill_arc( ctx,  ll_x, ur_y - 2*amount, 2*amount, 2*amount, 180, 45, 
			  drawtype, 0);
      glow_draw_fill_arc( ctx,  ur_x - 2*amount, ll_y, 2*amount, 2*amount, 45, 45, 
			  drawtype, 0);
      // Draw dark shadow
      drawtype = ((GrowCtx *)ctx)->shift_drawtype( fillcolor, drawtype_incr, (GrowNode *)colornode);
      glow_draw_fill_rect( ctx, ll_x + amount, ur_y - ish, ur_x - ll_x - 2*amount, ish, drawtype);
      glow_draw_fill_rect( ctx, ur_x - ish, ll_y + amount, ish, ur_y - ll_y - 2*amount, drawtype);
      glow_draw_fill_arc( ctx,  ll_x, ur_y - 2*amount, 2*amount, 2*amount, 225, 45, 
			  drawtype, 0);
      glow_draw_fill_arc( ctx,  ur_x - 2*amount, ll_y, 2*amount, 2*amount, 0, 45, 
			  drawtype, 0);
      glow_draw_fill_arc( ctx,  ur_x - 2*amount, ur_y - 2*amount, 2*amount, 2*amount, 270, 90, 
			  drawtype, 0);


      if ( amount > ish) {
	glow_draw_fill_rect( ctx, ll_x+ish, ll_y + amount, ur_x - ll_x-2*ish, ur_y - ll_y - 2*amount, fillcolor);
	glow_draw_fill_rect( ctx, ll_x + amount, ll_y+ish, ur_x - ll_x - 2*amount, amount-ish, fillcolor);
	glow_draw_fill_rect( ctx, ll_x + amount, ur_y - amount, ur_x - ll_x - 2*amount, amount-ish, fillcolor);
	glow_draw_fill_arc( ctx,  ll_x+ish, ll_y+ish, 2*amount-2*ish, 2*amount-2*ish, 90, 90, 
			  fillcolor, 0);
	glow_draw_fill_arc( ctx,  ll_x+ish, ur_y - 2*amount+ish, 2*amount-2*ish, 2*amount-2*ish, 180, 90, 
			  fillcolor, 0);
	glow_draw_fill_arc( ctx,  ur_x - 2*amount+ish, ur_y - 2*amount+ish, 2*amount-2*ish, 2*amount-2*ish, 270, 90, 
			  fillcolor, 0);
	glow_draw_fill_arc( ctx,  ur_x - 2*amount+ish, ll_y+ish, 2*amount-2*ish, 2*amount-2*ish, 0, 90, 
			  fillcolor, 0);
      }
      else {
	glow_draw_fill_rect( ctx, ll_x+amount, ll_y + amount, ur_x - ll_x-2*amount, ur_y - ll_y - 2*amount, fillcolor);
      }
    }
  }
  if ( border || !fill)
  {
    drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);
    int c = 0;
    glow_draw_line( ctx, ll_x + amount, ll_y, ur_x - amount, ll_y, drawtype, idx, 0);
    glow_draw_line( ctx, ll_x, ll_y-c + amount, ll_x, ur_y-c - amount, drawtype, idx, 0);
    glow_draw_line( ctx, ll_x + amount, ur_y, ur_x - amount, ur_y, drawtype, idx, 0);
    glow_draw_line( ctx, ur_x-c, ll_y + amount, ur_x-c, ur_y - amount, drawtype, idx, 0);
    glow_draw_arc( ctx,  ll_x, ll_y, 2*amount, 2*amount, 90, 90, 
		   drawtype, idx, 0);
    glow_draw_arc( ctx,  ll_x, ur_y - 2*amount, 2*amount, 2*amount, 180, 90, 
		   drawtype, idx, 0);
    glow_draw_arc( ctx,  ur_x - 2*amount, ur_y - 2*amount, 2*amount, 2*amount, 270, 90, 
		   drawtype, idx, 0);
    glow_draw_arc( ctx,  ur_x - 2*amount, ll_y, 2*amount, 2*amount, 0, 90, 
		   drawtype, idx, 0);
  }
}

void GrowRectRounded::erase( GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    idx += hot;
    if ( idx < 0) return;
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  int amount = int( round_amount/100 * min(ur_x - ll_x, ur_y - ll_y) + 0.5);
  ctx->set_draw_buffer_only();
  if ( border || !fill) {
    glow_draw_line_erase( ctx, ll_x + amount, ll_y, ur_x - amount, ll_y, idx);
    glow_draw_line_erase( ctx, ll_x, ll_y + amount, ll_x, ur_y - amount, idx);
    glow_draw_line_erase( ctx, ll_x + amount, ur_y, ur_x - amount, ur_y, idx);
    glow_draw_line_erase( ctx, ur_x, ll_y + amount, ur_x, ur_y - amount, idx);
    glow_draw_arc_erase( ctx,  ll_x, ll_y, 2*amount, 2*amount, 90, 90, idx);
    glow_draw_arc_erase( ctx,  ll_x, ur_y - 2*amount + 1, 2*amount, 2*amount, 180, 90, idx);
    glow_draw_arc_erase( ctx,  ur_x - 2*amount + 1, ur_y - 2*amount + 1, 2*amount, 2*amount, 270, 90, idx);
    glow_draw_arc_erase( ctx,  ur_x - 2*amount + 1, ll_y, 2*amount, 2*amount, 0, 90, idx);
  }
  if ( fill)
    glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
  ctx->reset_draw_buffer_only();
}


void GrowRectRounded::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  glow_eDrawType drawtype;
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    if ( idx < 0) return;
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  int amount = int( round_amount/100 * min(ur_x - ll_x, ur_y - ll_y) + 0.5);
  if ( fill)
  {
    int ish = int( shadow_width / 100 * min(ur_x - ll_x, ur_y - ll_y) + 0.5);
    int display_shadow = ((node && ((GrowNode *)node)->shadow) || shadow) && !disable_shadow;
    glow_eDrawType fillcolor = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
		 highlight, (GrowNode *)colornode, 1);

    if ( !display_shadow || ish == 0) {
      drawtype = fillcolor;
      glow_draw_nav_fill_rect( ctx, ll_x, ll_y + amount, ur_x - ll_x, ur_y - ll_y - 2*amount, drawtype);
      glow_draw_nav_fill_rect( ctx, ll_x + amount, ll_y, ur_x - ll_x - 2*amount, amount, drawtype);
      glow_draw_nav_fill_rect( ctx, ll_x + amount, ur_y - amount, ur_x - ll_x - 2*amount, amount, drawtype);
      glow_draw_nav_fill_arc( ctx,  ll_x, ll_y, 2*amount, 2*amount, 90, 90, 
		   drawtype);
      glow_draw_nav_fill_arc( ctx,  ll_x, ur_y - 2*amount, 2*amount, 2*amount, 180, 90, 
			      drawtype);
      glow_draw_nav_fill_arc( ctx,  ur_x - 2*amount, ur_y - 2*amount, 2*amount, 2*amount, 270, 90, 
			      drawtype);
      glow_draw_nav_fill_arc( ctx,  ur_x - 2*amount, ll_y, 2*amount, 2*amount, 0, 90, 
			      drawtype);
    }
    else {

      // Draw light shadow
      int drawtype_incr = shadow_contrast;
      if ( relief == glow_eRelief_Down)
	drawtype_incr = -shadow_contrast;

      drawtype = ((GrowCtx *)ctx)->shift_drawtype( fillcolor, -drawtype_incr, (GrowNode *)colornode);
      glow_draw_nav_fill_rect( ctx, ll_x + amount, ll_y, ur_x - ll_x - 2*amount, ish, drawtype);
      glow_draw_nav_fill_rect( ctx, ll_x, ll_y + amount, ish, ur_y - ll_y - 2*amount, drawtype);
      glow_draw_nav_fill_arc( ctx,  ll_x, ll_y, 2*amount, 2*amount, 90, 90, 
			  drawtype);
      glow_draw_nav_fill_arc( ctx,  ll_x, ur_y - 2*amount, 2*amount, 2*amount, 180, 45, 
			  drawtype);
      glow_draw_nav_fill_arc( ctx,  ur_x - 2*amount, ll_y, 2*amount, 2*amount, 45, 45, 
			  drawtype);
      // Draw dark shadow
      drawtype = ((GrowCtx *)ctx)->shift_drawtype( fillcolor, drawtype_incr, (GrowNode *)colornode);

      glow_draw_nav_fill_rect( ctx, ll_x + amount, ur_y - ish, ur_x - ll_x - 2*amount, ish, drawtype);
      glow_draw_nav_fill_rect( ctx, ur_x - ish, ll_y + amount, ish, ur_y - ll_y - 2*amount, drawtype);
      glow_draw_nav_fill_arc( ctx,  ll_x, ur_y - 2*amount, 2*amount, 2*amount, 225, 45, 
			  drawtype);
      glow_draw_nav_fill_arc( ctx,  ur_x - 2*amount, ll_y, 2*amount, 2*amount, 0, 45, 
			  drawtype);
      glow_draw_nav_fill_arc( ctx,  ur_x - 2*amount, ur_y - 2*amount, 2*amount, 2*amount, 270, 90, 
			  drawtype);


      drawtype = fillcolor;

      if ( amount > ish) {
	glow_draw_nav_fill_rect( ctx, ll_x+ish, ll_y + amount, ur_x - ll_x-2*ish, ur_y - ll_y - 2*amount, drawtype);
	glow_draw_nav_fill_rect( ctx, ll_x + amount, ll_y+ish, ur_x - ll_x - 2*amount, amount-ish, drawtype);
	glow_draw_nav_fill_rect( ctx, ll_x + amount, ur_y - amount, ur_x - ll_x - 2*amount, amount-ish, drawtype);
	glow_draw_nav_fill_arc( ctx,  ll_x+ish, ll_y+ish, 2*amount-2*ish, 2*amount-2*ish, 90, 90, 
			  drawtype);
	glow_draw_nav_fill_arc( ctx,  ll_x+ish, ur_y - 2*amount+ish, 2*amount-2*ish, 2*amount-2*ish, 180, 90, 
			  drawtype);
	glow_draw_nav_fill_arc( ctx,  ur_x - 2*amount+ish, ur_y - 2*amount+ish, 2*amount-2*ish, 2*amount-2*ish, 270, 90, 
			  drawtype);
	glow_draw_nav_fill_arc( ctx,  ur_x - 2*amount+ish, ll_y+ish, 2*amount-2*ish, 2*amount-2*ish, 0, 90, 
			  drawtype);
      }
      else {
	glow_draw_nav_fill_rect( ctx, ll_x+amount, ll_y + amount, ur_x - ll_x-2*amount, ur_y - ll_y - 2*amount, drawtype);
      }
    }
  }
  if ( border || !fill)
  {
    drawtype = ((GrowCtx *)ctx)->get_drawtype( draw_type, glow_eDrawType_LineHighlight,
		 highlight, (GrowNode *)colornode, 0);
    glow_draw_nav_line( ctx, ll_x + amount, ll_y, ur_x - amount, ll_y, drawtype, idx, 0);
    glow_draw_nav_line( ctx, ll_x, ll_y + amount, ll_x, ur_y - amount, drawtype, idx, 0);
    glow_draw_nav_line( ctx, ll_x + amount, ur_y, ur_x - amount, ur_y, drawtype, idx, 0);
    glow_draw_nav_line( ctx, ur_x, ll_y + amount, ur_x, ur_y - amount, drawtype, idx, 0);
    glow_draw_nav_arc( ctx,  ll_x, ll_y, 2*amount, 2*amount, 90, 90, 
		   drawtype, idx, 0);
    glow_draw_nav_arc( ctx,  ll_x, ur_y - 2*amount + 1, 2*amount, 2*amount, 180, 90, 
		   drawtype, idx, 0);
    glow_draw_nav_arc( ctx,  ur_x - 2*amount + 1, ur_y - 2*amount + 1, 2*amount, 2*amount, 270, 90, 
		   drawtype, idx, 0);
    glow_draw_nav_arc( ctx,  ur_x - 2*amount + 1, ll_y, 2*amount, 2*amount, 0, 90, 
		   drawtype, idx, 0);
  }
}

void GrowRectRounded::nav_erase( GlowTransform *t, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    if ( idx < 0) return;
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y1 = int( trf.y( ll.x, ll.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
    x2 = int( trf.x( ur.x, ur.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y2 = int( trf.y( ur.x, ur.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * ctx->nav_zoom_factor_x + 0.5) - ctx->nav_offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * ctx->nav_zoom_factor_y + 0.5) - ctx->nav_offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  int amount = int( round_amount/100 * min(ur_x - ll_x, ur_y - ll_y) + 0.5);
  if ( border || !fill) {
    glow_draw_nav_line_erase( ctx, ll_x + amount, ll_y, ur_x - amount, ll_y, idx);
    glow_draw_nav_line_erase( ctx, ll_x, ll_y + amount, ll_x, ur_y - amount, idx);
    glow_draw_nav_line_erase( ctx, ll_x + amount, ur_y, ur_x - amount, ur_y, idx);
    glow_draw_nav_line_erase( ctx, ur_x, ll_y + amount, ur_x, ur_y - amount, idx);
    glow_draw_nav_arc_erase( ctx,  ll_x, ll_y, 2*amount, 2*amount, 90, 90, idx);
    glow_draw_nav_arc_erase( ctx,  ll_x, ur_y - 2*amount + 1, 2*amount, 2*amount, 180, 90, idx);
    glow_draw_nav_arc_erase( ctx,  ur_x - 2*amount + 1, ur_y - 2*amount + 1, 2*amount, 2*amount, 270, 90, idx);
    glow_draw_nav_arc_erase( ctx,  ur_x - 2*amount + 1, ll_y, 2*amount, 2*amount, 0, 90, idx);

  }
  if ( fill)
    glow_draw_nav_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	glow_eDrawType_LineErase);
}

void GrowRectRounded::get_borders( GlowTransform *t, double *x_right, 
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

  if ( display_level != glow_mDisplayLevel_1)
    return;
  if ( ll_x < *x_left)
    *x_left = ll_x;
  if ( ur_x > *x_right)
    *x_right = ur_x;
  if ( ll_y < *y_low)
    *y_low = ll_y;
  if ( ur_y > *y_high)
    *y_high = ur_y;
}

void GrowRectRounded::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();
}

void GrowRectRounded::set_linewidth( int linewidth)
{
  erase();
  nav_erase();
  GlowRect::set_linewidth( linewidth);
  draw();
}

void GrowRectRounded::set_fill( int fillval)
{
  erase();
  nav_erase();
  GlowRect::set_fill( fillval);
  draw();
}

void GrowRectRounded::set_border( int borderval)
{
  erase();
  nav_erase();
  border = borderval;
  draw();
}

int GrowRectRounded::draw_annot_background( GlowTransform *t, void *node, 
		double x, double y)
{
  if ( fill)
  {
    draw( t, 0, 0, node, node);
    return 1;
  }
  return 0;
}

int GrowRectRounded::get_annot_background( GlowTransform *t, void *node, 
		glow_eDrawType *background)
{
  if ( fill)
  {
    *background = ((GrowCtx *)ctx)->get_drawtype( fill_drawtype, glow_eDrawType_FillHighlight,
		 0, (GrowNode *)node, 1);
    return 1;
  }
  return 0;
}

void GrowRectRounded::align( double x, double y, glow_eAlignDirection direction)
{
    double dx, dy;

    erase();
    nav_erase();
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
}


void GrowRectRounded::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  if ( !(display_level & ctx->display_level))
    return;
  int idx;

  if ( fix_line_width)
  {
    idx = line_width;
    idx += hot;
    if ( idx < 0)
    {
      erase( t, hot, node);
      return;
    }
  }
  else
  {
    if ( node && ((GrowNode *)node)->line_width)
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( ctx->zoom_factor_y / ctx->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  double x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = trf.x( ll.x, ll.y) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = trf.y( ll.x, ll.y) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = trf.x( ur.x, ur.y) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = trf.y( ur.x, ur.y) * ctx->zoom_factor_y - ctx->offset_y;
  }
  else
  {
    x1 = trf.x( t, ll.x, ll.y) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = trf.y( t, ll.x, ll.y) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = trf.x( t, ur.x, ur.y) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = trf.y( t, ur.x, ur.y) * ctx->zoom_factor_y - ctx->offset_y;
  }

  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  double amount = round_amount/100 * min(ur_x - ll_x, ur_y - ll_y);
  double ish;
  if ( !disable_shadow)
    ish = shadow_width / 100 * min(ur_x - ll_x, ur_y - ll_y);
  else
    ish = 0;

  int drawtype_incr = shadow_contrast;
  if ( relief == glow_eRelief_Down)
    drawtype_incr = -shadow_contrast;

  ((GrowCtx *)ctx)->export_jbean->rectrounded( ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	fill, border || !fill, fill_drawtype, draw_type, idx, amount, ish, shadow,
	drawtype_incr, pass, shape_cnt, node_cnt, fp);

}

void GrowRectRounded::flip( double x0, double y0, glow_eFlipDirection dir)
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

void GrowRectRounded::convert( glow_eConvert version) 
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
