/* 
 * Proview   $Id: flow_browctx.cpp,v 1.8 2008-05-13 13:55:29 claes Exp $
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

#include <fstream.h>
#include <math.h>
#include <float.h>
#include "flow_ctx.h"
#include "flow_browctx.h"
#include "flow_node.h"
#include "flow_point.h"
#include "flow_rect.h"
#include "flow_line.h"
#include "flow_arc.h"
#include "flow_text.h"
#include "flow_conpoint.h"
#include "flow_node.h"
#include "flow_nodeclass.h"
#include "flow_con.h"
#include "flow_conclass.h"
#include "flow_pushbutton.h"
#include "flow_draw.h"

int BrowCtx::insert( FlowArrayElem *element, FlowArrayElem *destination, 
		flow_eDest destination_code)
{
  int sts;
  double y;

  sts = a.brow_insert( element, destination, destination_code);
  if ( !sts) return sts;

  if ( destination)
    y = ((FlowNode *)destination)->y_low;
  else
   y = 0;
  configure( y);
  return 1;
}

void BrowCtx::remove( FlowArrayElem *element)
{
  double pos = ((FlowNode *)element)->y_low;

  set_nodraw();
  a.brow_remove( this, element);
  reset_nodraw();
  configure( pos);
}

void BrowCtx::close( FlowArrayElem *element)
{
  set_nodraw();
  a.brow_close( this, element);
  reset_nodraw();
  configure( ((FlowNode *)element)->y_low);
}

void BrowCtx::configure( double y_redraw) 
{
  if ( nodraw) 
    return;
  
  fdraw->get_window_size( this, &window_width, &window_height);
  a.configure();
  get_borders();
  frame_x_right = MAX( x_right, 
	1.0 * (window_width + offset_x) / zoom_factor);
  a.zoom();
  redraw( y_redraw);
  change_scrollbar();
}

void BrowCtx::change_scrollbar()
{
  flow_sScroll data;

  if ( !scroll_size)
  {
    // Get scroll size from the width of the first element
    double ll_x, ur_x, ll_y, ur_y;

    if ( a.size() == 0)
      return;
    ((FlowNode *)a[0])->measure( &ll_x, &ll_y, &ur_x, &ur_y);
    scroll_size = ur_y - ll_y + 1.0 / zoom_factor;
  }

  data.scroll_data = scroll_data;
  data.total_width = int( (x_right - x_left) / scroll_size) + 1;
  data.total_height = int( (y_high - y_low) / scroll_size) + 1;
  data.window_width = int( window_width / scroll_size / zoom_factor) + 1;
  data.window_height = int( window_height / scroll_size / zoom_factor) + 1;
  data.offset_x = int( offset_x / scroll_size / zoom_factor - x_left / 
	scroll_size);
  data.offset_y = int( offset_y /scroll_size / zoom_factor - y_low / 
	scroll_size);

  (scroll_callback)( &data);
}


void BrowCtx::redraw( double y_redraw)
{

  fdraw->get_window_size( this, &window_width, &window_height);

  if ( y_redraw) {
    fdraw->clear_area( this, 0, window_width, 
		int(y_redraw * zoom_factor - offset_y), window_height);
    draw( 0, int(y_redraw * zoom_factor - offset_y), window_width, window_height);
  }
  else {
    clear();
    draw( 0, 0, window_width, window_height);
  }
  nav_zoom();
}

void BrowCtx::zoom( double factor)
{ 
  if ( fabs(factor) < DBL_EPSILON)
    return;

  zoom_factor *= factor;
  if ( offset_x != 0)
    offset_x = int( (offset_x - window_width / 2.0 * ( 1.0/factor - 1)) 
		* factor);
  if ( offset_y != 0)
    offset_y = int( (offset_y  - window_height / 2.0 * ( 1.0/factor - 1)) 
		* factor);
  offset_x = MAX( offset_x, 0);
  offset_y = MAX( offset_y, 0);
  if ( (x_right - x_left) * zoom_factor <= window_width)
    offset_x = 0;
  if ( (y_high - y_low) * zoom_factor <= window_height)
    offset_y = 0;
  a.zoom();
  clear();
  draw( 0, 0, window_width, window_height);
  nav_zoom();
  change_scrollbar();
}

int BrowCtx::print( char *filename)
{
  int		i;
  double ll_x, ll_y, ur_x, ur_y;
  double width, height;
  int sts;

  if ( a.size() == 0)
    return 0;

  ((FlowNode *)a[0])->measure( &ll_x, &ll_y, &ur_x, &ur_y);
  height = 60 * (ur_y - ll_y);
  width = 0.70 * height;
  if ( width < ur_x - ll_x) {
    // Portrait
    height = 40 * ( ur_y - ll_y);
    width = height / 0.70;
  }

  current_print = new FlowPscript( filename, this, 1, &sts);
  if ( EVEN(sts))
  {
    delete current_print;
    return 0;
  }

  for ( i = 0;; i++)
  {
    ll_y = i * height;
    ur_y = ll_y + height;
    ll_x = 0;
    ur_x = width;
    if (  ll_y > y_high)
      break;

    current_print->print_page( ll_x, ll_y, ur_x, ur_y);
  }
  delete current_print;

  return 1;
}


int BrowCtx::is_visible( FlowArrayElem *element, flow_eVisible type)
{
  double ll_x, ll_y, ur_x, ur_y;
  double window_low, window_high;

  ((FlowNode *)element)->measure( &ll_x, &ll_y, &ur_x, &ur_y);
  window_low = double(offset_y) / zoom_factor;
  window_high = double(offset_y + window_height) / zoom_factor;
  switch ( type) {
  case flow_eVisible_Full:
    if ( ll_y >= window_low && ur_y <= window_high)
      return 1;
    else
      return 0;
  case flow_eVisible_Partial:
    if ( (ll_y >= window_low && ll_y <= window_high) ||
	 (ur_y >= window_low && ur_y <= window_high) ||
	 (ll_y <= window_low && ur_y >= window_high))
      return 1;
    else
      return 0;
  case flow_eVisible_Top:
    if ( ur_y >= window_low && ur_y <= window_high)
      return 1;
    else
      return 0;
  case flow_eVisible_Bottom:
    if ( ll_y >= window_low && ll_y <= window_high)
      return 1;
    else
      return 0;
  default: ;
  }
  return 0;
}

void BrowCtx::center_object( FlowArrayElem *object, double factor)
{
  double ll_x, ll_y, ur_x, ur_y;
  int new_offset_y;
  int y_pix;

  ((FlowNode *)object)->measure( &ll_x, &ll_y, &ur_x, &ur_y);
  new_offset_y = int(ll_y * zoom_factor - factor * window_height);
  if ( new_offset_y <= 0)
    y_pix = offset_y;
  else
    y_pix = - (new_offset_y - offset_y);
  scroll( 0, y_pix);
  change_scrollbar();
}

int BrowCtx::get_first_visible( FlowArrayElem **element)
{
  double ll_x, ll_y, ur_x, ur_y;
  double window_low, window_high;
  int i;

  window_low = double(offset_y) / zoom_factor;
  window_high = double(offset_y + window_height) / zoom_factor;

  for ( i = 0; i < a.size(); i++) {
    ((FlowNode *)a[i])->measure( &ll_x, &ll_y, &ur_x, &ur_y);
    if ( ll_y >= window_low || ur_y >= window_high) {
      *element = a[i];
      return 1;
    }
  }
  return 0;
}

int BrowCtx::get_last_visible( FlowArrayElem **element)
{
  double ll_x, ll_y, ur_x, ur_y;
  double window_low, window_high;
  int i;

  window_low = double(offset_y) / zoom_factor;
  window_high = double(offset_y + window_height) / zoom_factor;

  for ( i = a.size() - 1; i >= 0; i--) {
    ((FlowNode *)a[i])->measure( &ll_x, &ll_y, &ur_x, &ur_y);
    if ( ur_y <= window_high || ll_y <= window_low) {
      *element = a[i];
      return 1;
    }
  }
  return 0;
}

int BrowCtx::page( double factor)
{
  double ll_x, ll_y, ur_x, ur_y;
  int new_offset_y;

  new_offset_y = offset_y + int( factor * window_height);

  if ( factor < 0) {
    if ( offset_y <= 0)
      return 0;

    if ( new_offset_y < 0)
      new_offset_y = 0;
  }
  if ( factor > 0) {
    FlowArrayElem *e;
    int sts;

    sts = a.get_last( &e);
    if ( EVEN(sts)) return 0;

    ((FlowNode *)e)->measure( &ll_x, &ll_y, &ur_x, &ur_y);
    if ( new_offset_y > ur_y * zoom_factor)
      return 0;
  }
  scroll( 0, offset_y - new_offset_y);
  change_scrollbar();
  return 1;
}

void brow_scroll_horizontal( BrowCtx *ctx, int value, int bottom)
{
  int x_pix;

  x_pix = int( - value * ctx->scroll_size * ctx->zoom_factor + 
	(ctx->offset_x - ctx->x_left * ctx->zoom_factor));
  ctx->scroll( x_pix, 0);
}

void brow_scroll_vertical( BrowCtx *ctx, int value, int bottom)
{
  int y_pix;

  y_pix = int( - value * ctx->scroll_size * ctx->zoom_factor + 
	(ctx->offset_y - ctx->y_low * ctx->zoom_factor));
  // Correction for the bottom position
  if ( bottom && ( y_pix >= 0 ||
	ctx->window_height + y_pix < ctx->y_high * ctx->zoom_factor - ctx->offset_y))
//        window_height >= (y_high - y_low) * zoom_factor)
    y_pix = int( ctx->window_height + ctx->offset_y - 
		ctx->y_high * ctx->zoom_factor);
  ctx->scroll( 0, y_pix);
}
