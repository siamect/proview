/* 
 * Proview   $Id: glow_curvectx.cpp,v 1.2 2005-09-01 14:57:53 claes Exp $
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
#include <fstream.h>
#include <math.h>
#include <float.h>
#include "glow_ctx.h"
#include "glow_curvectx.h"
#include "glow_node.h"
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_line.h"
#include "glow_arc.h"
#include "glow_text.h"
#include "glow_conpoint.h"
#include "glow_node.h"
#include "glow_nodeclass.h"
#include "glow_con.h"
#include "glow_conclass.h"
#include "glow_growtrend.h"
#include "glow_growaxis.h"
#include "glow_draw.h"
#include "glow_msg.h"

void CurveCtx::configure()
{
  //  GrowTrend *t1;
  // GrowAxis *a1;

  // t1 = new GrowTrend( this, "curve", 0, 0, 200, 30, glow_eDrawType_Line,
  //    1, glow_mDisplayLevel_1, 1, 1, glow_eDrawType_LineGray, 0);
  // t1->horizontal_lines = 9;
  // t1->vertical_lines = 29;
  // for ( int i = 0; i < 100; i++)
  //   t1->add_value( 50 * sin( i * 3.14 / 20) + 50, 0);
  // a.insert( t1);
  
  // a1 = new GrowAxis( this, "curve", 0, 30, 200, 32);
  // a1->text_size = 5;
  // a1->max_value = 300;
  // a1->min_value = 0;
  // a1->lines = 151;
  // a1->longquotient = 5;
  // a1->valuequotient = 10;
  // a1->trf.rotation = 270;
  // a1->configure();
  // a.insert( a1);  
}

void CurveCtx::get_zoom( double *factor_x, double *factor_y)
{
  *factor_x = zoom_factor_x;
  *factor_y = zoom_factor_y;
}

void CurveCtx::zoom( double factor)
{ 
  if ( fabs(factor) < DBL_EPSILON)
    return;

//  cout << "Before zoom zoom factor : " << zoom_factor_x << ", offset : " <<
//     offset_x << "," << offset_y << endl;
  zoom_factor_x *= factor;
  // zoom_factor_y *= factor;
  offset_x = int( (offset_x - window_width / 2.0 * ( 1.0/factor - 1)) * factor);
  //  offset_y = int( (offset_y  - window_height / 2.0 * ( 1.0/factor - 1)) * factor);
//  cout << "After  zoom zoom factor : " << zoom_factor_x << ", offset : " <<
//     offset_x << "," << offset_y << endl;
  a.zoom();
  clear();
  draw( 0, 0, window_width, window_height);
  nav_zoom();
  change_scrollbar();
}

void CurveCtx::adjust_layout()
{
  int width, height;

  glow_draw_get_window_size( this, &width, &height);
  if ( (window_height != height && y_high != y_low) || !layout_adjusted ) {
    zoom_factor_y = height / (y_high - y_low);
    if ( !layout_adjusted && initial_position == glow_eDirection_Right) {
      offset_x = int( x_right * zoom_factor_x) - width;
    }
    draw( 0, 0, width, height);
    layout_adjusted = 1;
  }
}

void CurveCtx::nav_zoom()
{
  if ( nodraw)
    return;
  if ( a.size() > 0)
  {
    double x_nav_left, x_nav_right, y_nav_low, y_nav_high;

    get_borders();
//    cout << "Borders : <" << x_left << " > " << x_right << " ^ " << 
//		y_high << " Y " << y_low << endl;
    x_nav_left = x_left;
    x_nav_right = x_right;
    y_nav_low = y_low;
    y_nav_high = y_high;
    if ( x_nav_right - x_nav_left == 0 || y_nav_high - y_nav_low == 0)
      return;
    nav_zoom_factor_x = nav_window_width / (x_nav_right - x_nav_left);
    nav_zoom_factor_y = nav_window_height / (y_nav_high - y_nav_low);
    nav_offset_x = int( x_nav_left * nav_zoom_factor_x);
    nav_offset_y = int( y_nav_low * nav_zoom_factor_y);
    a.nav_zoom();
    nav_clear();
    nav_draw( 0, 0, nav_window_width, nav_window_height);
  }
}

void CurveCtx::get_prefered_zoom_y( int height, double *factor_y)
{
  *factor_y = height / (y_high - y_low);
}

int CurveCtx::event_handler_nav( glow_eEvent event, int x, int y)
{
  GlowCtx	*ctx;

  ctx = this;

  switch ( event)
  {
    case glow_eEvent_MB1Press:
      if ( nav_rect_ll_x < x && x < nav_rect_ur_x &&
           nav_rect_ll_y < y && y < nav_rect_ur_y)
      {
        nav_rect_movement_active = 1;
        nav_rect_move_last_x = x;
        nav_rect_move_last_y = y;
      }
      break;

    case glow_eEvent_MB2Press:
      if ( nav_rect_ll_x < x && x < nav_rect_ur_x &&
           nav_rect_ll_y < y && y < nav_rect_ur_y)
      {
        nav_rect_zoom_active = 1;
        nav_rect_move_last_x = x;
        nav_rect_move_last_y = y;
      }
      break;

    case glow_eEvent_CursorMotion:
      if ( nav_rect_ll_x < x && x < nav_rect_ur_x &&
           nav_rect_ll_y < y && y < nav_rect_ur_y)
      {
        if ( !nav_rect_hot)
        {
          draw_set_nav_cursor( ctx, glow_eDrawCursor_CrossHair);
          nav_rect_hot = 1;
        }
      }
      else
      {
        if ( nav_rect_hot)
        {
          draw_set_nav_cursor( ctx, glow_eDrawCursor_Normal);
          nav_rect_hot = 0;
        }
      }
      break;
    case glow_eEvent_Exposure:
      glow_draw_get_nav_window_size( ctx, &nav_window_width, &nav_window_height);
      nav_zoom();
      break;
    case glow_eEvent_ButtonMotion:
      if ( nav_rect_movement_active)
      {
        int delta_x, delta_y, mainwind_delta_x, mainwind_delta_y;

        glow_draw_nav_rect_erase( this, nav_rect_ll_x, nav_rect_ll_y, 
		nav_rect_ur_x - nav_rect_ll_x, nav_rect_ur_y - nav_rect_ll_y, 0);

	delta_x = x - nav_rect_move_last_x;
	delta_y = 0;
        nav_rect_ll_x += delta_x;
        nav_rect_ur_x += delta_x;
        nav_rect_ll_y += delta_y;
        nav_rect_ur_y += delta_y;
        nav_rect_move_last_x = x;
        nav_rect_move_last_y = y;
        nav_draw( nav_rect_ll_x - 10, nav_rect_ll_y - 10, 
		nav_rect_ur_x + 10, nav_rect_ur_y + 10);
//        glow_draw_nav_rect( this, nav_rect_ll_x, nav_rect_ll_y, 
//		nav_rect_ur_x - nav_rect_ll_x, nav_rect_ur_y - nav_rect_ll_y,
//		glow_eDrawType_Line, 0, 0);

        mainwind_delta_x = int( - zoom_factor_x / nav_zoom_factor_x * delta_x);
        mainwind_delta_y = int( - zoom_factor_y / nav_zoom_factor_y * delta_y);
        offset_x -= mainwind_delta_x;
        offset_y -= mainwind_delta_y;
        a.traverse( mainwind_delta_x, mainwind_delta_y);
        if ( ctx_type == glow_eCtxType_Grow)
        {
          ((GrowCtx *)this)->polyline_last_end_x += mainwind_delta_x;
          ((GrowCtx *)this)->polyline_last_end_y += mainwind_delta_y;
        }

        draw_copy_area( ctx, mainwind_delta_x, mainwind_delta_y);
//        clear();
        if ( !unobscured) 
          draw( 0, 0, window_width, window_height);
        else
        {
          if ( mainwind_delta_x >= 0 && mainwind_delta_y >= 0)
          {
            if ( mainwind_delta_x)
              draw( 0, 0, mainwind_delta_x, window_height);
            if ( mainwind_delta_y)
              draw( mainwind_delta_x, 0, window_width, mainwind_delta_y);
          }
          else if ( mainwind_delta_x <= 0 && mainwind_delta_y <= 0)
          {
            if ( mainwind_delta_x)
              draw( window_width+mainwind_delta_x, 0, window_width, window_height);
            if ( mainwind_delta_y)
              draw( 0, window_height + mainwind_delta_y, 
		window_width + mainwind_delta_x, window_height);
          }
          else if ( mainwind_delta_x <= 0 && mainwind_delta_y >= 0)
          {
            if ( mainwind_delta_x)
              draw( window_width + mainwind_delta_x, 0, window_width, 
			window_height);
            if ( mainwind_delta_y)
              draw( 0, 0, window_width + mainwind_delta_x, mainwind_delta_y);
          }
          else
          {
            if ( mainwind_delta_x)
              draw( 0, 0, mainwind_delta_x, window_height);
            if ( mainwind_delta_y)
              draw( mainwind_delta_x, window_height+ mainwind_delta_y, 
		window_width, window_height);
          }
        }
        change_scrollbar();
      }
      else if ( nav_rect_zoom_active)
      {
        int delta_x, delta_y;
	double zoom_f;
        double center_x, center_y;
        double center_dist, center_dist_last;

        glow_draw_nav_rect_erase( this, nav_rect_ll_x, nav_rect_ll_y, 
		nav_rect_ur_x - nav_rect_ll_x, nav_rect_ur_y - nav_rect_ll_y, 0);

	delta_x = x - nav_rect_move_last_x;
	delta_y = 0;

	center_x = 0.5 * (nav_rect_ur_x + nav_rect_ll_x);
	center_y = 0.5 * (nav_rect_ur_y + nav_rect_ll_y);
	center_dist_last = sqrt( (nav_rect_move_last_x - center_x) *
		       (nav_rect_move_last_x - center_x) + 
		       (nav_rect_move_last_y - center_y) *
		       (nav_rect_move_last_y - center_y));
        center_dist = sqrt((x - center_x)*(x - center_x) + 
	              (y - center_y)*(y - center_y));
        if ( center_dist < DBL_EPSILON)
          return 1;
        zoom_f =  center_dist_last / center_dist;
//        if ( fabs(zoom_f - 1) < 0.2)
//          return 1;
	zoom( zoom_f);
        nav_rect_move_last_x = x;
        nav_rect_move_last_y = y;
      }
      break;
    case glow_eEvent_ButtonRelease:
      if ( nav_rect_movement_active)
      {
        nav_rect_movement_active = 0;
	nav_zoom();
      }
      if ( nav_rect_zoom_active)
      {
        nav_rect_zoom_active = 0;
      }
      break;
    default:
      ;
  }
  return 1;
}

void curve_scroll_horizontal( CurveCtx *ctx, int value, int bottom)
{
  int x_pix;

  x_pix = int( - value * ctx->scroll_size * ctx->zoom_factor_x + 
	(ctx->offset_x - ctx->x_left * ctx->zoom_factor_x));
  ctx->scroll( x_pix, 0);
}

void curve_scroll_vertical( CurveCtx *ctx, int value, int bottom)
{
  int y_pix;

  y_pix = int( - value * ctx->scroll_size * ctx->zoom_factor_y + 
	(ctx->offset_y - ctx->y_low * ctx->zoom_factor_y));
  // Correction for the bottom position
  if ( bottom && ( y_pix >= 0 ||
	ctx->window_height + y_pix < ctx->y_high * ctx->zoom_factor_y - ctx->offset_y))
//        window_height >= (y_high - y_low) * zoom_factor_y)
    y_pix = int( ctx->window_height + ctx->offset_y - 
		ctx->y_high * ctx->zoom_factor_y);
  ctx->scroll( 0, y_pix);
}










