/* 
 * Proview   $Id: glow_growmenu.cpp,v 1.11 2007-01-04 07:57:38 claes Exp $
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
#include "glow_growmenu.h"
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_msg.h"

GrowMenu::GrowMenu( GrowCtx *glow_ctx, char *name, glow_sMenuInfo *menu_info, double x, double y, 
		    double min_w, glow_eDrawType border_d_type, int line_w, 
		    int fill_rect, int display_border, 
		    glow_eDrawType fill_d_type, int t_size, 
		    glow_eDrawType t_drawtype, glow_eDrawType t_color, 
		    glow_eDrawType t_color_disabled, GlowArrayElem *parent, int nodraw) : 
		GrowRect(glow_ctx,name,x,y,10,10,border_d_type,line_w,0,
		glow_mDisplayLevel_1,fill_rect,display_border,0,fill_d_type,1),
	        info(*menu_info), text_size(t_size),  text_drawtype(t_drawtype), 
		text_color(t_color), text_color_disabled(t_color_disabled), item_cnt(0),
		item_height(0), current_item(-1), new_item(0), old_item(-1), parent_menu(parent),
		min_width(min_w), input_focus(0)
{
  if ( !nodraw)
    draw( &ctx->mw, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
}

GrowMenu::~GrowMenu()
{
  // Remove this as parent from any submenu
  for ( int i = 0; i < ctx->a.a_size; i++) {
    if ( ctx->a[i]->type() == glow_eObjectType_GrowMenu) {
      if ( ((GrowMenu *)ctx->a[i])->parent_menu == this)
	((GrowMenu *)ctx->a[i])->parent_menu = 0;
    }
  }

  if ( input_focus && parent_menu)
    ((GrowMenu *)parent_menu)->set_input_focus( 1);

  if ( ctx->nodraw) return;
  erase( &ctx->mw);
  erase( &ctx->navw);
}

void GrowMenu::draw( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y) 
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

  if ( x_right * w->zoom_factor_x - w->offset_x + 1 >= ll_x &&
      	x_left * w->zoom_factor_x - w->offset_x <= ur_x &&
       	y_high * w->zoom_factor_y - w->offset_y + 1 >= ll_y &&
       	y_low * w->zoom_factor_y- w->offset_y <= ur_y) {
      draw( w, (GlowTransform *)NULL, highlight, hot, NULL, NULL);
  }
}

void GrowMenu::draw( GlowWind *w, int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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

void GrowMenu::draw( GlowWind *w, GlowTransform *t, int highlight, int hot, void *node, void *colornode)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
  int idx;
  int text_idx = int( trf.vertical_scale(t) * w->zoom_factor_y / w->base_zoom_factor * (text_size +4) - 4);
  text_idx = min( text_idx, DRAW_TYPE_SIZE-1);
  text_idx = max( 0, text_idx);

  idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
  idx += hot;

  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int ll_x, ll_y, ur_x, ur_y;

  int z_width, z_height, z_descent;
  int max_z_width = 0;
  double tot_z_height = 0;
  int i;
  int pulldown_found = 0;
  item_cnt = 0;
  for ( i = 0; i < (int) (sizeof(info.item)/sizeof(info.item[0])); i++) {
    if ( info.item[i].occupied) {
      ctx->gdraw->get_text_extent( info.item[i].text, strlen(info.item[i].text), text_drawtype, 
                max( 0, text_idx), &z_width, &z_height, &z_descent);
      if ( z_width > max_z_width)
	max_z_width = z_width;
      tot_z_height += int( 1.3 * z_height);
      if ( info.item[i].type == glow_eMenuItem_PulldownMenu)
	pulldown_found = 1;
      item_cnt++;
    }
  }
  if ( !item_cnt)
    return;

  int arrow_size = z_height * 4 / 5;
  ll_x = int( ll.x * w->zoom_factor_x) - w->offset_x;
  ll_y = int( ll.y * w->zoom_factor_y) - w->offset_y;
  ur_x = ll_x + max_z_width + 15 + int(pulldown_found * arrow_size);
  if ( ur_x - ll_x < int(min_width * w->zoom_factor_x))
    ur_x = ll_x + int(min_width * w->zoom_factor_x);
  ur_y = ll_y + int(tot_z_height);

  if ( ur_y > ctx->y_high * w->zoom_factor_y) {
    // Outside window border
    ur_y = int(ctx->y_high * w->zoom_factor_y);
    ll_y = ur_y - int(tot_z_height);
    ll.posit( ll.x, ll_y / w->zoom_factor_y);
    ur.posit_z( ur.z_x, ur_y + w->offset_y); // Might not always be correct?
  }
  else
    ur.posit_z( ur_x + w->offset_x, ur_y + w->offset_y);
  ll_y = int( ll.y * w->zoom_factor_y) - w->offset_y;
  get_node_borders();

  if ( fill) {
    ctx->gdraw->fill_rect( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, fill_drawtype);
  }
  item_height = tot_z_height / item_cnt;
  int x_text = ll_x + 3;
  int y_text;
  glow_eDrawType drawtype;

  int item_idx = 0;
  for ( i = 0; i < (int) (sizeof(info.item)/sizeof(info.item[0])); i++) {
    if ( info.item[i].occupied) {
      y_text = ll_y + int( item_height * item_idx + z_height + 1);
      if ( i == current_item) {
	if ( fill_drawtype % 10 < 7)
	  drawtype = ctx->shift_drawtype( fill_drawtype, 2, 0);
	else
	  drawtype = ctx->shift_drawtype( fill_drawtype, -2, 0);
	ctx->gdraw->fill_rect( w, ll_x, int(ll_y + item_idx * item_height), ur_x - ll_x, 
			     int(item_height), drawtype);
      }
      if ( info.item[i].type == glow_eMenuItem_ButtonDisabled)
	ctx->gdraw->text( w, x_text, y_text, info.item[i].text, strlen(info.item[i].text), text_drawtype, 
			text_color_disabled, text_idx, highlight, 0);
      else
	ctx->gdraw->text( w, x_text, y_text, info.item[i].text, strlen(info.item[i].text), text_drawtype, 
			text_color, text_idx, highlight, 0);
      if ( info.item[i].type == glow_eMenuItem_PulldownMenu) {
	// Draw arrow
	glow_sPointX p[4];
	p[0].x = ur_x - arrow_size - 2;
	p[0].y = short(ll_y + item_idx * item_height + item_height/2 - arrow_size/2);
	p[1].x = ur_x - 2;
	p[1].y = short(ll_y + item_idx * item_height + item_height/2);
	p[2].x = ur_x - arrow_size - 2;
	p[2].y = short(ll_y + item_idx * item_height + item_height/2 + arrow_size/2);
	p[3].x = ur_x - arrow_size - 2;
	p[3].y = short(ll_y + item_idx * item_height + item_height/2 - arrow_size/2);
      
	ctx->gdraw->fill_polyline( w, p, 4, glow_eDrawType_MediumGray, 0);
      }
      item_idx++;
    }
  }
  if ( border) {
    ctx->gdraw->rect( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, draw_type, idx, 0);
  }
}

void GrowMenu::erase( GlowWind *w, GlowTransform *t, int hot, void *node)
{
  if ( !(display_level & ctx->display_level))
    return;
  if ( w == &ctx->navw) {
    if ( ctx->no_nav)
      return;
    hot = 0;
  }
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
      idx = int( w->zoom_factor_y / w->base_zoom_factor * 
		((GrowNode *)node)->line_width - 1);
    else
      idx = int( w->zoom_factor_y / w->base_zoom_factor * line_width - 1);
    idx += hot;
  }
  idx = max( 0, idx);
  idx = min( idx, DRAW_TYPE_SIZE-1);
  int x1, y1, x2, y2, ll_x, ll_y, ur_x, ur_y;

  if (!t)
  {
    x1 = int( trf.x( ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
  }
  else
  {
    x1 = int( trf.x( t, ll.x, ll.y) * w->zoom_factor_x) - w->offset_x;
    y1 = int( trf.y( t, ll.x, ll.y) * w->zoom_factor_y) - w->offset_y;
    x2 = int( trf.x( t, ur.x, ur.y) * w->zoom_factor_x) - w->offset_x;
    y2 = int( trf.y( t, ur.x, ur.y) * w->zoom_factor_y) - w->offset_y;
  }
  ll_x = min( x1, x2);
  ur_x = max( x1, x2);
  ll_y = min( y1, y2);
  ur_y = max( y1, y2);

  w->set_draw_buffer_only();
  if ( border)
    ctx->gdraw->rect_erase( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, idx);
  ctx->gdraw->fill_rect( w, ll_x, ll_y, ur_x - ll_x, ur_y - ll_y, glow_eDrawType_LineErase);
  w->reset_draw_buffer_only();
}

void GrowMenu::draw()
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

int GrowMenu::local_event_handler( GlowWind *w, glow_eEvent event, double x, double y)
{
  double ll_x, ur_x, ll_y, ur_y;

  ll_x = min( ll.x, ur.x);
  ur_x = max( ll.x, ur.x);
  ll_y = min( ll.y, ur.y);
  ur_y = max( ll.y, ur.y);

  if ( ll_x <= x && x <= ur_x &&
       ll_y <= y && y <= ur_y) {
    int item;
    double vscale = trf.vertical_scale(0);
    //cout << "Event handler: Hit in menu" << endl;

    item = int((y - ll.y) / ( item_height / vscale / w->zoom_factor_y));
    if ( item > item_cnt - 1)
      item = item_cnt - 1;
    if ( item < 0)
      item = 0;
    if ( item != current_item) {
      new_item = 1;
      old_item = current_item;
      current_item = item;
      for ( int i = 0, item_idx = 0; i < 32; i++) {
	if ( !info.item[i].occupied)
	  continue;
	if ( item_idx == item) {
	  current_item = i;
	  break;
	}
	item_idx++;
      }
    }
    else {
      new_item = 0;
      old_item = current_item;
    }
    return 1;
  }
  else {
    new_item = 0;
    return 0;
  }
}

int GrowMenu::event_handler( GlowWind *w, glow_eEvent event, double fx, double fy)
{
  double x, y;

  trf.reverse( fx, fy, &x, &y);
  return local_event_handler( w, event, x, y);
}

int GrowMenu::event_handler( GlowWind *w, glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;
  int csts;

  double rx, ry;

  // Convert koordinates to local koordinates
  trf.reverse( fx, fy, &rx, &ry);

  sts = 0;
  if ( event == ctx->event_move_node) {
    return 0;
  }
  switch ( event) {
    case glow_eEvent_CursorMotion: {
      int redraw = 0;

      if ( ctx->hot_mode != glow_eHotMode_TraceAction &&
	   ctx->hot_found)
        sts = 0;
      else {
        sts = local_event_handler( w, event, rx, ry);
        if ( sts) {
          ctx->hot_found = 1;
	  redraw = 1;
	}
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
          ctx->draw( w, x_left * w->zoom_factor_x - w->offset_x - DRAW_MP,
	     y_low * w->zoom_factor_y - w->offset_y - DRAW_MP,
  	     x_right * w->zoom_factor_x - w->offset_x + DRAW_MP,
	     y_high * w->zoom_factor_y - w->offset_y + DRAW_MP);
      }
      if ( old_item != current_item && old_item != -1) {
	if ( info.item[old_item].type == glow_eMenuItem_PulldownMenu) {
	  // Delete all child menu objects.
	  ctx->delete_menu_child( this);
	}
      }
      if ( hot && new_item) {
	if ( info.item[current_item].type == glow_eMenuItem_PulldownMenu)
	  ctx->send_menu_callback( this, current_item, glow_eEvent_MenuCreate,
						ur.x, ll.y + item_height / w->zoom_factor_y * current_item);
      }
      break;
    }
    case glow_eEvent_MB1Down:
      ctx->gdraw->set_click_sensitivity( w, glow_mSensitivity_MB1Click);
      break;
    case glow_eEvent_MB1Click: {
      sts = local_event_handler( w, event, rx, ry);
      if ( sts && current_item != -1) {
	if ( info.item[current_item].type == glow_eMenuItem_Button) {
	  csts = ctx->send_menu_callback( this, current_item, glow_eEvent_MenuActivated, fx, fy);
	  if ( csts == GLOW__TERMINATED)
	    return csts;

	  // Send delete callback for the root menu of this node
	  GrowMenu *e;
	  for ( e = this; e->parent_menu; e = (GrowMenu *)e->parent_menu) 
	    ;
	  ctx->send_menu_callback( e, 0, glow_eEvent_MenuDelete, 0, 0);
	  return GLOW__NO_PROPAGATE;
	}
      }
    }
    default:
      sts = local_event_handler( w, event, rx, ry);
  }
  if ( sts)
    ctx->register_callback_object( glow_eObjectType_Node, this);
  return sts;
}

void GrowMenu::delete_menu_child( GlowArrayElem *parent) {
  if ( parent_menu == parent) {
    // Delete all children first
    ctx->delete_menu_child( this);

    // Send delete callback for this object
    ctx->send_menu_callback( this, 0, glow_eEvent_MenuDelete, 0, 0);
  }
}

void GrowMenu::get_menu_char( int *t_size, glow_eDrawType *fill_color, glow_eDrawType *t_drawtype,
		      glow_eDrawType *t_color, glow_eDrawType *t_color_disabled)
{
  *t_size = text_size;
  *fill_color = fill_drawtype;
  *t_drawtype = text_drawtype;
  *t_color = text_color;
  *t_color_disabled = text_color_disabled;
}

void GrowMenu::shift_current_item( int shift)
{
  if ( shift == 1) {
    old_item = current_item;
    if ( current_item == -1)
      current_item = 0;
    else {
      for ( int i = current_item + 1; i < 32; i++) {
	if ( info.item[i].occupied) {
	  current_item = i;
	  break;
	}
      }
    }
  }
  else if ( shift == -1) {
    if ( current_item == 0)
      return;
    old_item = current_item;
    if ( current_item == -1)
      current_item = 32;

    for ( int i = current_item - 1; i >= 0; i--) {
      if ( info.item[i].occupied) {
	current_item = i;
	break;
      }
    }
  }
  else
    return;

  if ( old_item != -1) {
    if ( info.item[old_item].type == glow_eMenuItem_PulldownMenu) {
      // Delete all child menu objects.
      ctx->delete_menu_child( this);
    }
  }
  draw();
}

int GrowMenu::get_current_item( int *item)
{
  if ( current_item == -1)
    return 0;

  *item = current_item;
  return 1;
}

void GrowMenu::get_submenu_position( int item, double *x, double *y)
{
  *x = ur.x;
  *y = ll.y + item_height / ctx->mw.zoom_factor_y * item;
}

int GrowMenu::get_parent( GlowArrayElem **parent) 
{ 
  if ( parent_menu) { 
    *parent = parent_menu; 
    return 1;
  }
  else
    return 0;
}
