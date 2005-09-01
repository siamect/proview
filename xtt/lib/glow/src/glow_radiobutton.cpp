/* 
 * Proview   $Id: glow_radiobutton.cpp,v 1.2 2005-09-01 14:57:54 claes Exp $
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
#include "glow_radiobutton.h"
#include "glow_node.h"
#include "glow_draw.h"
#include "glow_msg.h"


void GlowRadiobutton::zoom()
{
  ll.zoom();
  ur.zoom();
}

void GlowRadiobutton::nav_zoom()
{
  ll.nav_zoom();
  ur.nav_zoom();
}

void GlowRadiobutton::print_zoom()
{
  ll.print_zoom();
  ur.print_zoom();
}

void GlowRadiobutton::traverse( int x, int y)
{
  ll.traverse( x, y);
  ur.traverse( x, y);
}

void GlowRadiobutton::print( void *pos, void *node)
{
  int ll_x, ll_y, ur_x, ur_y;
  int idx = 0;
  double sc = ctx->print_zoom_factor / ctx->zoom_factor_y;

  ll_x = int( ll.print_z_x + ((GlowPoint *)pos)->print_z_x);
  ll_y = int( ll.print_z_y + ((GlowPoint *)pos)->print_z_y);
  ur_x = int( ll_x + ur.print_z_x - ll.print_z_x);
  ur_y = int( ll_y + ur.print_z_y - ll.print_z_y);
  ctx->print_ps->rect( ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	draw_type, idx);
  if ( ((GlowNode *)node)->rbuttonv[number])
  {
    ctx->print_ps->line( ll_x + sc*1, ll_y + sc*1, ur_x - sc*1, ll_y + sc*1,
	glow_eDrawType_LineGray, idx);
    ctx->print_ps->line( ll_x + sc*2, ll_y + sc*2, ur_x - sc*2, ll_y + sc*2,
	glow_eDrawType_LineGray, idx);
    ctx->print_ps->line( ll_x + sc*1, ll_y + sc*1, ll_x + sc*1, ur_y - sc*1,
	glow_eDrawType_LineGray, idx);
    ctx->print_ps->line( ll_x + sc*2, ll_y + sc*2, ll_x + sc*2, ur_y - sc*2,
	glow_eDrawType_LineGray, idx);
    ctx->print_ps->filled_rect( ll_x + sc*3, ll_y + sc*3, ur_x - ll_x - sc*5, 
	ur_y - ll_y - sc*5, glow_eDrawType_Line, idx);
  }
  else
  {
    ctx->print_ps->line( ur_x - sc*1, ur_y - sc*1, ll_x + sc*1, ur_y - sc*1,
	glow_eDrawType_LineGray, idx);
    ctx->print_ps->line( ur_x - sc*2, ur_y - sc*2, ll_x + sc*2, ur_y - sc*2,
	glow_eDrawType_LineGray, idx);
    ctx->print_ps->line( ur_x - sc*1, ur_y - sc*1, ur_x - sc*1, ll_y + sc*1,
	glow_eDrawType_LineGray, idx);
    ctx->print_ps->line( ur_x - sc*2, ur_y - sc*2, ur_x - sc*2, ll_y + sc*2,
	glow_eDrawType_LineGray, idx);
  }
}

void GlowRadiobutton::save( ofstream& fp, glow_eSaveMode mode)
{
  // Not yet implemented
}

void GlowRadiobutton::open( ifstream& fp)
{
  // Not yet implemented
}

void GlowRadiobutton::draw( void *pos, int highlight, int hot, void *node)
{
  int ll_x, ll_y, ur_x, ur_y;
  int idx = 0;

  ll_x = ll.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
  ll_y = ll.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y;
  ur_x = ll_x + ur.z_x - ll.z_x;
  ur_y = ll_y + ur.z_y - ll.z_y;
  glow_draw_rect( ctx, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y,
	draw_type, idx, 0);
  if ( ((GlowNode *)node)->rbuttonv[number])
  {
    glow_draw_line( ctx, ll_x + 1, ll_y + 1, ur_x - 1, ll_y + 1,
	glow_eDrawType_LineGray, idx, 0);
    glow_draw_line( ctx, ll_x + 2, ll_y + 2, ur_x - 2, ll_y + 2,
	glow_eDrawType_LineGray, idx, 0);
    glow_draw_line( ctx, ll_x + 1, ll_y + 1, ll_x + 1, ur_y - 1,
	glow_eDrawType_LineGray, idx, 0);
    glow_draw_line( ctx, ll_x + 2, ll_y + 2, ll_x + 2, ur_y - 2,
	glow_eDrawType_LineGray, idx, 0);
    glow_draw_line_erase( ctx, ur_x - 1, ur_y - 1, ll_x + 1, ur_y - 1,
	idx);
    glow_draw_line_erase( ctx, ur_x - 2, ur_y - 2, ll_x + 2, ur_y - 2,
	idx);
    glow_draw_line_erase( ctx, ur_x - 1, ur_y - 1, ur_x - 1, ll_y + 1,
	idx);
    glow_draw_line_erase( ctx, ur_x - 2, ur_y - 2, ur_x - 2, ll_y + 2,
	idx);
    glow_draw_fill_rect( ctx, ll_x + 3, ll_y + 3, ur_x - ll_x - 5, 
	ur_y - ll_y - 5, glow_eDrawType_Line);
  }
  else
  {
    glow_draw_line_erase( ctx, ll_x + 1, ll_y + 1, ur_x - 1, ll_y + 1,
	idx);
    glow_draw_line_erase( ctx, ll_x + 2, ll_y + 2, ur_x - 2, ll_y + 2,
	idx);
    glow_draw_line_erase( ctx, ll_x + 1, ll_y + 1, ll_x + 1, ur_y - 1,
	idx);
    glow_draw_line_erase( ctx, ll_x + 2, ll_y + 2, ll_x + 2, ur_y - 2,
	idx);
    glow_draw_line( ctx, ur_x - 1, ur_y - 1, ll_x + 1, ur_y - 1,
	glow_eDrawType_LineGray, idx, 0);
    glow_draw_line( ctx, ur_x - 2, ur_y - 2, ll_x + 2, ur_y - 2,
	glow_eDrawType_LineGray, idx, 0);
    glow_draw_line( ctx, ur_x - 1, ur_y - 1, ur_x - 1, ll_y + 1,
	glow_eDrawType_LineGray, idx, 0);
    glow_draw_line( ctx, ur_x - 2, ur_y - 2, ur_x - 2, ll_y + 2,
	glow_eDrawType_LineGray, idx, 0);
    glow_draw_fill_rect( ctx, ll_x + 3, ll_y + 3, ur_x - ll_x - 5, 
	ur_y - ll_y - 5, glow_eDrawType_LineErase);
  }
}

void GlowRadiobutton::erase( void *pos, int hot, void *node)
{
  int ll_x, ll_y, ur_x, ur_y;

  ll_x = ll.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x;
  ll_y = ll.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y;
  ur_x = ll_x + ur.z_x - ll.z_x;
  ur_y = ll_y + ur.z_y - ll.z_y;
  glow_draw_fill_rect( ctx, ll_x, ll_y, ur_x - ll_x, 
	ur_y - ll_y, glow_eDrawType_LineErase);
}

void GlowRadiobutton::nav_draw( void *pos, int highlight, void *node)
{
#if 0
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    if ( idx < 0) return;
  }
  else
  {
    idx = ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_rect( ctx, ll.nav_z_x + ((GlowPoint *)pos)->nav_z_x - 
	ctx->nav_offset_x, ll.nav_z_y + 
	((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	ur.nav_z_x - ll.nav_z_x, ur.nav_z_y - ll.nav_z_y,
	draw_type, idx, highlight);
#endif
}

void GlowRadiobutton::nav_erase( void *pos, void *node)
{
#if 0
  int idx;
  if ( fix_line_width)
  {
    idx = line_width;
    if ( idx < 0) return;
  }
  else
  {
    idx = ctx->nav_zoom_factor_y / ctx->base_zoom_factor * line_width - 1;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  glow_draw_nav_rect_erase( ctx, ll.nav_z_x + ((GlowPoint *)pos)->nav_z_x - 
	ctx->nav_offset_x, ll.nav_z_y + 
	((GlowPoint *)pos)->nav_z_y - ctx->nav_offset_y, 
	ur.nav_z_x - ll.nav_z_x, ur.nav_z_y - ll.nav_z_y,
	idx);
#endif
}

int GlowRadiobutton::event_handler( void *pos, glow_eEvent event, int x, int y,
	void *node)
{
  GlowPoint *p;

  p = (GlowPoint *) pos;
  if ( ll.z_x + ((GlowPoint *)pos)->z_x - ctx->offset_x <= x && 
       x <= ur.z_x  + ((GlowPoint *)pos)->z_x - ctx->offset_x &&
       ll.z_y  + ((GlowPoint *)pos)->z_y - ctx->offset_y <= y && 
       y <= ur.z_y + ((GlowPoint *)pos)->z_y - ctx->offset_y)
  {
    switch ( event)
    {
      case glow_eEvent_MB1Click:
        cout << "Event handler: Hit in radiobutton" << endl;
        /* Call backcall function */
        ctx->radiobutton_cb( (GlowArrayElem *)node, number,
		((GlowNode *)node)->rbuttonv[number]);
        return GLOW__NO_PROPAGATE;
        break;
      default:
        ;
    }
    return 1;
  }  
  else
    return 0;
}

void GlowRadiobutton::get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node)
{
  if ( pos_x + ll.x < *x_left)
    *x_left = pos_x + ll.x;
  if ( pos_x + ur.x > *x_right)
    *x_right = pos_x + ur.x;
  if ( pos_y + ll.y < *y_low)
    *y_low = pos_y + ll.y;
  if ( pos_y + ur.y > *y_high)
    *y_high = pos_y + ur.y;
}

void GlowRadiobutton::move( void *pos, double x, double y, int highlight, int hot)
{
  double width, height;

  width = ur.x - ll.x;
  height = ur.y - ll.y;
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  ll.x = x;
  ll.y = y;
  ur.x = x + width;
  ur.y = y + height;
  zoom();
  nav_zoom();
  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

void GlowRadiobutton::shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot)
{
  erase( pos, hot, NULL);
  nav_erase( pos, NULL);
  ll.x += delta_x;
  ll.y += delta_y;
  ur.x += delta_x;
  ur.y += delta_y;
  zoom();
  nav_zoom();

  draw( pos, highlight, hot, NULL);
  nav_draw( pos, highlight, NULL);
}

ostream& operator<< ( ostream& o, const GlowRadiobutton r)
{
  o << 
  '(' << r.ll.x << ',' << r.ll.y << ')' << 
  '(' << r.ur.x << ',' << r.ur.y << ')' << 
  '[' << r.ll.z_x << ',' << r.ll.z_y << ']' <<
  '[' << r.ur.z_x << ',' << r.ur.z_y << ']' ;
  return o;
}

