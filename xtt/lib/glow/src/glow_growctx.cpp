#include "glow_std.h"

#include <fstream.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include "glow_ctx.h"
#include "glow_growctx.h"
#include "glow_node.h"
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_growrect.h"
#include "glow_growtext.h"
#include "glow_growslider.h"
#include "glow_growpolyline.h"
#include "glow_line.h"
#include "glow_arc.h"
#include "glow_text.h"
#include "glow_conpoint.h"
#include "glow_node.h"
#include "glow_nodeclass.h"
#include "glow_con.h"
#include "glow_conclass.h"
#include "glow_pushbutton.h"
#include "glow_grownode.h"
#include "glow_growannot.h"
#include "glow_growsubannot.h"
#include "glow_growimage.h"
#include "glow_growgroup.h"
#include "glow_growaxis.h"
#include "glow_draw.h"

#include "glow_msg.h"


extern "C" {
#include "co_dcli.h"
}

GrowCtx::~GrowCtx()
{
  clear_all( 0);
  if ( dynamic)
    free( dynamic);
}

void GrowCtx::set_mode( grow_eMode grow_mode) 
{
  if ( edit_mode == grow_eMode_Scale)
  {
    // Erase scale rectangle
    select_rect_active = 0;
    glow_draw_rect_erase( this, select_rect_ll_x, select_rect_ll_y,
	  	select_rect_ur_x - select_rect_ll_x,
	  	select_rect_ur_y - select_rect_ll_y, 0);
    draw( select_rect_ll_x, select_rect_ll_y, select_rect_ur_x, 
		select_rect_ur_y);
  }


  edit_mode = grow_mode;
  if ( edit_mode == grow_eMode_Edit || edit_mode == grow_eMode_EditPolyLine)
  {
    draw_set_cursor( this, glow_eDrawCursor_Normal);
    hot_mode = default_hot_mode;
  }
  else if ( edit_mode == grow_eMode_Scale)
  {
    double ll_x, ll_y, ur_x, ur_y;

    hot_mode = glow_eHotMode_Disabled;

    // Draw a scale rectangle
    ur_x = -1e10;
    ll_x = 1e10;
    ur_y = -1e10;
    ll_y = 1e10;
    a_sel.get_borders( &ur_x, &ll_x, &ur_y, &ll_y);
    select_rect_ll_x = int( ll_x * zoom_factor_x) - offset_x;
    select_rect_ll_y = int( ll_y * zoom_factor_y) - offset_y;
    select_rect_ur_x = int( ur_x * zoom_factor_x) - offset_x;
    select_rect_ur_y = int( ur_y * zoom_factor_y) - offset_y;
    scale_x = 1;
    scale_y = 1;

    glow_draw_rect( this, select_rect_ll_x, select_rect_ll_y, 
	  select_rect_ur_x - select_rect_ll_x,
	  select_rect_ur_y - select_rect_ll_y, glow_eDrawType_Line, 0, 0);

  }
  else
  {
    draw_set_cursor( this, glow_eDrawCursor_DiamondCross);
    hot_mode = glow_eHotMode_Disabled;
  }
}

int GrowCtx::event_handler( glow_eEvent event, int x, int y, int w, int h)
{
  int sts;
  int i;
  GlowCtx	*ctx;
  int node_move_event = 0;
  double fx, fy;

  ctx = this;
//  cout << "Event: " << event << endl;

  fx = double( x + ctx->offset_x) / ctx->zoom_factor_x;
  fy = double( y + ctx->offset_y) / ctx->zoom_factor_y;

  callback_object_type = glow_eObjectType_NoObject;
  callback_object = 0;

  if ( event == event_create_con && edit_mode == grow_eMode_Edit)
  {
    sts = 0;
    for ( i = 0; i < a.a_size; i++)
    {
      sts = a.a[a.a_size-i-1]->event_handler( event, x, y, fx, fy);
      if ( sts)
        break;
    }
  }
  else if ( event == event_create_node)
  {
  }
  else if ( event == event_move_node && edit_mode == grow_eMode_Edit)
  {
    move_clear();

    sts = 0;
    for ( i = 0; i < a.a_size; i++)
    {
      sts = a.a[a.a_size-i-1]->event_handler( event, x, y, fx, fy);
      if ( sts)
        break;
    }
    if ( sts)
    {
      int j, node_cnt;

      modified = 1;
      if ( a_sel.size() > 1 && select_find( a_move[0]))
      {

        /* Move all selected nodes */
        move_clear();

	/* Insert nodes first and then all connections connected to the nodes */
        for ( i = 0; i < a_sel.size(); i++)
        {
	  if ( a_sel[i]->type() != glow_eObjectType_Con)
            move_insert( a_sel[i]);
	}
        node_movement_active = 1;
        node_move_last_x = x;
        node_move_last_y = y;
      }

      node_movement_active = 1;
      node_move_last_x = x;
      node_move_last_y = y;

      /* Insert all connnected cons for movement */
      node_cnt = a_move.size();
      for ( i = 0; i < node_cnt; i++)
      {
        for ( j = 0; j < a.size(); j++)
        {
	  if ( a[j]->type() == glow_eObjectType_Con &&
	       ((GlowCon *)a[j])->is_connected_to( (GlowNode *)a_move[i]))
	  {
            if ( move_insert( a[j]))
              ((GlowCon *)a[j])->set_movement_type( a_move.a, node_cnt);
	  }
	}
      }

      draw_set_cursor( ctx, glow_eDrawCursor_DiamondCross);
      if ( event == event_region_select) 
        // Move and region select is defined as the same event
        node_move_event = 1;
    }
    if ( event_callback[glow_eEvent_SliderMoveStart] && 
	 a_move.size() == 1 && 
	 a_move[0]->type() == glow_eObjectType_GrowSlider)
    {
      static glow_sEvent e;
	
      e.event = glow_eEvent_SliderMoveStart;
      e.any.type = glow_eEventType_Object;
      e.any.x_pixel = x;
      e.any.y_pixel = y;
      e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
      e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
      e.object.object = a_move[0];
      e.object.object_type = a_move[0]->type();
      event_callback[glow_eEvent_SliderMoveStart]( this, &e);

      if ( restriction_object && 
	   (move_restriction == glow_eMoveRestriction_VerticalSlider ||
	    move_restriction == glow_eMoveRestriction_HorizontalSlider))
      {
        double ll_x, ll_y, ur_x, ur_y;

        ((GrowSlider *)restriction_object)->measure( &ll_x, &ll_y,
			&ur_x, &ur_y);
        if (move_restriction == glow_eMoveRestriction_VerticalSlider)
          slider_cursor_offset = ll_y - e.any.y;
        else
          slider_cursor_offset = ll_x - e.any.x;
      }
    }
    if (move_restriction == glow_eMoveRestriction_Disable)
    {
      move_clear();
      node_movement_active = 0;
      draw_set_cursor( ctx, glow_eDrawCursor_Normal);
    }
  }
  else if ( event == event_move_node && edit_mode == grow_eMode_EditPolyLine)
  {
    move_clear();

    sts = 0;
    for ( i = 0; i < a.a_size; i++)
    {
      sts = a.a[a.a_size-i-1]->event_handler( event, x, y, fx, fy);
      if ( sts)
        break;
    }
    if ( sts)
    {
      if ( a_sel.size() == 1 && select_find( a_move[0]) &&
           a_move[0]->type() == glow_eObjectType_GrowPolyLine)
      {
        modified = 1;
        node_movement_active = 1;
        node_move_last_x = x;
        node_move_last_y = y;
        draw_set_cursor( ctx, glow_eDrawCursor_DiamondCross);
        if ( event == event_region_select) 
          // Move and region select is defined as the same event
          node_move_event = 1;
      }
    }
  }
  if ( (event == event_region_select && !node_move_event) || 
       event == event_region_add_select) 
  {
    if ( edit_mode == grow_eMode_Line)
    {
      con_create_conpoint_x = x;
      con_create_conpoint_y = y;
      con_create_last_x = x;
      con_create_last_y = y;
      con_create_active = 1;
    }
    else if ( edit_mode == grow_eMode_PolyLine)
    {
      if ( !polyline_not_first)
      {
        con_create_conpoint_x = x;
        con_create_conpoint_y = y;
        con_create_last_x = x;
        con_create_last_y = y;
        con_create_active = 1;
      }
      else
      {
        con_create_conpoint_x = polyline_last_end_x;
        con_create_conpoint_y = polyline_last_end_y;
        con_create_last_x = x;
        con_create_last_y = y;
        con_create_active = 1;
      }
    }
    else if ( edit_mode == grow_eMode_Edit || edit_mode == grow_eMode_Rect
	|| edit_mode == grow_eMode_Circle || edit_mode == grow_eMode_Bar
	|| edit_mode == grow_eMode_Trend || edit_mode == grow_eMode_Axis)
    {
      select_rect_active = 1;
      select_rect_event = event;
      select_rect_start_x = x;
      select_rect_start_y = y;
      select_rect_last_x = x;
      select_rect_last_y = y;
      select_rect_ll_x = x;
      select_rect_ll_y = y; 
      select_rect_ur_x = x;
      select_rect_ur_y = y; 
    }
    else if ( edit_mode == grow_eMode_Scale)
    {
      double ll_x, ll_y, ur_x, ur_y;
      int x0 = select_rect_ll_x;
      int delta_x = (select_rect_ur_x - select_rect_ll_x) / 3 + 1;
      int y0 = select_rect_ll_y;
      int delta_y = (select_rect_ur_y - select_rect_ll_y) / 3 + 1;

      // Store last scale
      a_sel.store_transform();

      ur_x = -1e10;
      ll_x = 1e10;
      ur_y = -1e10;
      ll_y = 1e10;
      a_sel.get_borders( &ur_x, &ll_x, &ur_y, &ll_y);

      if ( (x0 <= x && x < x0 + delta_x) && 
	   (y0 <= y && y < y0 + delta_y))
      {
	scale_type = glow_eScaleType_UpperRight; 
        scale_center_x = ur_x;
        scale_center_y = ur_y;
      }
      else if ( (x0 + delta_x <= x && x < x0 + 2 * delta_x) && 
	        (y0 <= y && y < y0 + delta_y))
      {
        scale_type = glow_eScaleType_Up;
        scale_center_x = ur_x;
        scale_center_y = ur_y;
      }
      else if ( (x0 + 2 * delta_x <= x && x < x0 + 3 * delta_x) && 
	        (y0 <= y && y < y0 + delta_y))
      {
	scale_type = glow_eScaleType_UpperLeft;
        scale_center_x = ll_x;
        scale_center_y = ur_y;
      }
      else if ( (x0 <= x && x < x0 + delta_x) && 
                (y0 + delta_y <= y && y < y0 + 2 * delta_y))
      {
	scale_type = glow_eScaleType_Right; 
        scale_center_x = ur_x;
        scale_center_y = ur_y;
      }
      else if ( (x0 + 2 * delta_x <= x && x < x0 + 3 * delta_x) && 
	        (y0 + delta_y <= y && y < y0 + 2 * delta_y))
      {
        scale_type = glow_eScaleType_Left;
        scale_center_x = ll_x;
        scale_center_y = ll_y;
      }
      else if ( (x0 <= x && x < x0 + delta_x) && 
	        (y0 + 2 * delta_y <= y && y < y0 + 3 * delta_y))
      {
	scale_type = glow_eScaleType_LowerRight; 
        scale_center_x = ur_x;
        scale_center_y = ll_y;
      }
      else if ( (x0 + delta_x <= x && x < x0 + 2 * delta_x) && 
	        (y0 + 2 * delta_y <= y && y < y0 + 3 * delta_y))
      {
        scale_type = glow_eScaleType_Down;
        scale_center_x = ll_x;
        scale_center_y = ll_y;
      }
      else if ( (x0 + 2 * delta_x <= x && x < x0 + 3 * delta_x) && 
	        (y0 + 2 * delta_y <= y && y < y0 + 3 * delta_y))
      {
	scale_type = glow_eScaleType_LowerLeft;
        scale_center_x = ll_x;
        scale_center_y = ll_y;
      }
      else
	scale_type = glow_eScaleType_No;


      select_rect_stored_ll_x = select_rect_ll_x;
      select_rect_stored_ll_y = select_rect_ll_y;
      select_rect_stored_ur_x = select_rect_ur_x;
      select_rect_stored_ur_y = select_rect_ur_y;
      select_rect_start_x = x;
      select_rect_start_y = y;
      select_rect_last_x = x;
      select_rect_last_y = y;
      select_rect_active = 1;
    }
    return 1;
  }

  switch ( event)
  {
    case glow_eEvent_MB1Click:
      if ( node_movement_paste_active)
      {
        modified = 1;
        if ( auto_scrolling_active)
          auto_scrolling_stop();
        set_defered_redraw();
        switch( move_restriction)
        {
          case glow_eMoveRestriction_Vertical:
            a_move.move( 0, y - node_move_last_y, grid_on);
            break;
          case glow_eMoveRestriction_Horizontal:
            a_move.move( x - node_move_last_x, 0, grid_on);
            break;
          default:
            a_move.move( x - node_move_last_x, y - node_move_last_y, grid_on);
            break;
        }
        node_move_last_x = x;
        node_move_last_y = y;
        redraw_defered();
        node_movement_paste_active = 0;
	nav_zoom();
        draw_set_cursor( ctx, glow_eDrawCursor_Normal);

	/* Send callback for all move objects */
	if ( event_callback[glow_eEvent_ObjectMoved] )
	{
	  static glow_sEvent e;
	
          e.event = glow_eEvent_ObjectMoved;
	  e.any.type = glow_eEventType_Object;
	  e.any.x_pixel = x;
	  e.any.y_pixel = y;
	  e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
	  e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
          for ( i = 0; i < a_move.size(); i++)
          {
	    e.object.object = a_move[i];
	    e.object.object_type = a_move[i]->type();
	    event_callback[event_move_node]( this, &e);
          }
	}
        return 1;
      }
    case glow_eEvent_MB1DoubleClick:
    case glow_eEvent_MB1ClickShift:
    case glow_eEvent_MB1DoubleClickShift:
    case glow_eEvent_MB1ClickCtrl:
    case glow_eEvent_MB1DoubleClickCtrl:
    case glow_eEvent_MB1ClickShiftCtrl:
    case glow_eEvent_MB1DoubleClickShiftCtrl:
    case glow_eEvent_MB2Click:
    case glow_eEvent_MB2DoubleClick:
    case glow_eEvent_MB2ClickShift:
    case glow_eEvent_MB2DoubleClickShift:
    case glow_eEvent_MB2ClickCtrl:
    case glow_eEvent_MB2DoubleClickCtrl:
    case glow_eEvent_MB2ClickShiftCtrl:
    case glow_eEvent_MB2DoubleClickShiftCtrl:
    case glow_eEvent_MB3Click:
    case glow_eEvent_MB3Press:
    case glow_eEvent_MB1Down:
    case glow_eEvent_MB1Up:
      sts = 0;
      for ( i = 0; i < a.a_size; i++)
      {
        sts = a.a[i]->event_handler( event, x, y, fx, fy);
        if ( sts == GLOW__NO_PROPAGATE)
          break;
      }
      break;

    case glow_eEvent_Exposure:
    {
      int width, height;

      glow_draw_get_window_size( ctx, &width, &height);
      if ( window_width != width || window_height != height) {
        window_width = width;
        window_height = height;
	
        if ( event_callback[glow_eEvent_Resized]) {
	  static glow_sEvent e;

          e.event = glow_eEvent_Resized;
	  e.any.type = glow_eEventType_CallBack;
	  event_callback[glow_eEvent_Resized]( this, &e);
        }
        change_scrollbar();
      }

      if ( glow_draw_create_buffer( ctx))
        draw( 0, 0, window_width, window_height);
      else
        draw( x, y, x + w, y + h);
      nav_zoom();
      }
      break;
    case glow_eEvent_CursorMotion:
      cursor_present = 1;
      cursor_x = x;
      cursor_y = y;
      if ( node_movement_paste_active)
      {
        set_defered_redraw();
        switch( move_restriction)
        {
          case glow_eMoveRestriction_Vertical:
            a_move.move( 0, y - node_move_last_y, 0);
            break;
          case glow_eMoveRestriction_Horizontal:
            a_move.move( x - node_move_last_x, 0, 0);
            break;
          default:
            a_move.move( x - node_move_last_x, y - node_move_last_y, 0);
            break;
        }
        node_move_last_x = x;
        node_move_last_y = y;
        redraw_defered();
      }
      sts = 0;
      hot_found = ( hot_mode == glow_eHotMode_Disabled);
      for ( i = 0; i < a.a_size; i++)
      {
        sts = a.a[a.a_size-i-1]->event_handler( event, x, y, fx, fy);
      }
      break;
    case glow_eEvent_ButtonMotion:
      if ( node_movement_active && edit_mode != grow_eMode_EditPolyLine)
      {
        int move_x, move_y;
        double cursor_y, cursor_x;

        modified = 1;
        switch( move_restriction)
        {
          case glow_eMoveRestriction_No:
            set_defered_redraw();
            a_move.move( x - node_move_last_x, y - node_move_last_y, 0);
            node_move_last_x = x;
            node_move_last_y = y;
            redraw_defered();
            break;
          case glow_eMoveRestriction_Vertical:
            set_defered_redraw();
            a_move.move( 0, y - node_move_last_y, 0);
            node_move_last_x = x;
            node_move_last_y = y;
            redraw_defered();
            break;
          case glow_eMoveRestriction_Horizontal:
            set_defered_redraw();
            a_move.move( x - node_move_last_x, 0, 0);
            node_move_last_x = x;
            node_move_last_y = y;
            redraw_defered();
            break;
          case glow_eMoveRestriction_VerticalSlider:
            cursor_y = double(y + offset_y) / zoom_factor_y;
            if ( cursor_y + slider_cursor_offset > restriction_max_limit) 
            {
              if ( double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset > restriction_max_limit)
                break;
              else
                move_y = int( (restriction_max_limit - 
			(double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset)) * zoom_factor_y);
            }
            else if ( cursor_y + slider_cursor_offset < restriction_min_limit) 
            {
              if ( double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset < restriction_min_limit)
                break;
              else
                move_y = int( (restriction_min_limit - 
			(double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset)) * zoom_factor_y);
            }
            else
            {
              if ( double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset > restriction_max_limit)
                move_y = int( (cursor_y + slider_cursor_offset - 
 			restriction_max_limit) * zoom_factor_y);
              else if ( double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset < restriction_min_limit)
                move_y = int( (cursor_y + slider_cursor_offset - 
			restriction_min_limit) * zoom_factor_y);
              else
                move_y = y - node_move_last_y;
            }
            if ( !move_y)
              break;
            set_defered_redraw();
            a_move.move( 0, move_y, 0);
            redraw_defered();
	    if ( restriction_object && event_callback[glow_eEvent_SliderMoved] &&
	         a_move.size() == 1 &&
	         a_move[0]->type() == glow_eObjectType_GrowSlider)
	    {
	      static glow_sEvent e;
	
              e.event = glow_eEvent_SliderMoved;
	      e.any.type = glow_eEventType_Object;
	      e.any.x_pixel = x;
	      e.any.y_pixel = node_move_last_y + move_y;
	      e.any.x = double (x + offset_x) / zoom_factor_x;
	      e.any.y = double (node_move_last_y + move_y + offset_y) / zoom_factor_y;
	      e.object.object = restriction_object;
	      e.object.object_type = restriction_object->type();
	      event_callback[glow_eEvent_SliderMoved]( this, &e);
	    }
            node_move_last_x = x;
            node_move_last_y = y;
            break;
          case glow_eMoveRestriction_HorizontalSlider:
            cursor_x = double(x + offset_x) / zoom_factor_x;
            if ( cursor_x + slider_cursor_offset > restriction_max_limit) 
            {
              if ( double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset > restriction_max_limit)
                break;
              else
                move_x = int((restriction_max_limit - 
			(double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset)) * zoom_factor_x);
            }
            else if ( cursor_x + slider_cursor_offset < restriction_min_limit) 
            {
              if ( double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset < restriction_min_limit)
                break;
              else
                move_x = int( (restriction_min_limit - 
			(double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset)) * zoom_factor_x);
            }
            else
            {
              if ( double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset > restriction_max_limit)
                move_x = int( (cursor_x + slider_cursor_offset - 
 			restriction_max_limit) * zoom_factor_x);
              else if ( double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset < restriction_min_limit)
                move_x = int( (cursor_x + slider_cursor_offset - 
			restriction_min_limit) * zoom_factor_x);
              else
                move_x = x - node_move_last_x;
            }
            if ( !move_x)
              break;
            set_defered_redraw();
            a_move.move( move_x, 0, 0);
            redraw_defered();
	    if ( restriction_object && event_callback[glow_eEvent_SliderMoved] &&
	         a_move.size() == 1 &&
	         a_move[0]->type() == glow_eObjectType_GrowSlider)
	    {
	      static glow_sEvent e;
	
              e.event = glow_eEvent_SliderMoved;
	      e.any.type = glow_eEventType_Object;
	      e.any.x_pixel = node_move_last_x + move_x;
	      e.any.y_pixel = y;
	      e.any.x = double (node_move_last_x + move_x+ offset_x) / zoom_factor_x;
	      e.any.y = double (y + offset_y) / zoom_factor_y;
	      e.object.object = restriction_object;
	      e.object.object_type = restriction_object->type();
	      event_callback[glow_eEvent_SliderMoved]( this, &e);
	    }
            node_move_last_x = x;
            node_move_last_y = y;
            break;
          case glow_eMoveRestriction_Disable:
            break;
        }
      }
      else if ( con_create_active)
      {
        switch( move_restriction) {
          case glow_eMoveRestriction_Vertical:
            x = con_create_conpoint_x;
	    break;
          case glow_eMoveRestriction_Horizontal:
            y = con_create_conpoint_y;
	    break;
	  default:
	    ;
        }
        glow_draw_line_erase( this, con_create_conpoint_x, 
		con_create_conpoint_y, con_create_last_x, con_create_last_y ,0);
        draw( con_create_conpoint_x, con_create_conpoint_y, 
		con_create_last_x, con_create_last_y);
        glow_draw_line( this, con_create_conpoint_x, con_create_conpoint_y,
		x, y, glow_eDrawType_Line, 0, 0);
        con_create_last_x = x;
        con_create_last_y = y;
        hot_found = ( hot_mode == glow_eHotMode_Disabled);
        for ( i = 0; i < a.a_size; i++)
        {
          sts = a.a[a.a_size-i-1]->event_handler( glow_eEvent_CursorMotion, x, y, fx, fy);
        }
      }
      else if ( select_rect_active && edit_mode != grow_eMode_Scale)
      {
        int draw_ll_x;
        int draw_ll_y;
        int draw_ur_x;
        int draw_ur_y;

        if ( edit_mode == grow_eMode_Circle)
          glow_draw_arc_erase( this, select_rect_ll_x, select_rect_ll_y,
	  	select_rect_ur_x - select_rect_ll_x,
	  	select_rect_ur_y - select_rect_ll_y, 0, 360, 0);
        else
          glow_draw_rect_erase( this, select_rect_ll_x, select_rect_ll_y,
	  	select_rect_ur_x - select_rect_ll_x,
	  	select_rect_ur_y - select_rect_ll_y, 0);

        if ( scale_equal &&
             (edit_mode == grow_eMode_Circle ||
              edit_mode == grow_eMode_Rect))

        {
          int delta_x, delta_y;
          delta_y = delta_x = max( abs(x - select_rect_start_x),
		       abs(y - select_rect_start_y));
          if ( x < select_rect_start_x)
            delta_x = - delta_x;
          if ( y < select_rect_start_y)
            delta_y = - delta_y;

          draw_ll_x = min( select_rect_ll_x, min( select_rect_start_x, select_rect_start_x + delta_x));
          draw_ll_y = min( select_rect_ll_y, min( select_rect_start_y, select_rect_start_y + delta_y));
          draw_ur_x = max( select_rect_ur_x, max( select_rect_start_x, select_rect_start_x + delta_x));
          draw_ur_y = max( select_rect_ur_y, max( select_rect_start_y, select_rect_start_y + delta_y));

          select_rect_ll_x = min( select_rect_start_x,
				  select_rect_start_x + delta_x);
          select_rect_ll_y = min( select_rect_start_y, 
				  select_rect_start_y + delta_y);
          select_rect_ur_x = max( select_rect_start_x, 
				  select_rect_start_x + delta_x);
          select_rect_ur_y = max( select_rect_start_y, 
				  select_rect_start_y + delta_y);
        }
        else {
          draw_ll_x = min( min( x, select_rect_start_x), select_rect_ll_x);
          draw_ll_y = min( min( y, select_rect_start_y), select_rect_ll_y);
          draw_ur_x = max( max( x, select_rect_start_x), select_rect_ur_x);
          draw_ur_y = max( max( y, select_rect_start_y), select_rect_ur_y);

          select_rect_ll_x = min( x, select_rect_start_x);
          select_rect_ll_y = min( y, select_rect_start_y); 
          select_rect_ur_x = max( x, select_rect_start_x);
          select_rect_ur_y = max( y, select_rect_start_y); 
        }
        draw( draw_ll_x - 1, draw_ll_y - 1, draw_ur_x + 1, draw_ur_y + 1);

        select_rect_last_x = x;
        select_rect_last_y = y;
      }
      else if ( select_rect_active && edit_mode == grow_eMode_Scale)
      {
	double x1, y1, x2, y2;
        double scale;

        if ( scale_type == glow_eScaleType_No)
          return 1;

        scale_active = 1;

        switch ( scale_type)
        {
          case glow_eScaleType_LowerLeft:
	    if ( select_rect_start_x == select_rect_stored_ll_x)
              return 1;
	    if ( select_rect_start_y == select_rect_stored_ll_y)
              return 1;
	    scale_x = double(x - select_rect_stored_ll_x) /
		(select_rect_start_x - select_rect_stored_ll_x);
	    scale_y = double(y - select_rect_stored_ll_y) /
		(select_rect_start_y - select_rect_stored_ll_y);
            if ( scale_equal) {
              scale = max( fabs(scale_x), fabs(scale_y));
              if ( scale_x >= 0)
                scale_x = scale;
              else
                scale_x = -scale;
              if ( scale_y >= 0)
                scale_y = scale;
              else
                scale_y = -scale; 
            }
	    x1 = select_rect_stored_ll_x;
	    y1 = select_rect_stored_ll_y;
            x2 = select_rect_stored_ll_x + 
		scale_x * (select_rect_stored_ur_x - select_rect_stored_ll_x);
            y2 = select_rect_stored_ll_y + 
		scale_y * (select_rect_stored_ur_y - select_rect_stored_ll_y);
            break;
          case glow_eScaleType_Down:
	    if ( select_rect_start_y == select_rect_stored_ll_y)
              return 1;
            if ( scale_equal) 
              return 1;
	    scale_y = double(y - select_rect_stored_ll_y) /
		(select_rect_start_y - select_rect_stored_ll_y);
	    x1 = select_rect_stored_ll_x;
	    y1 = select_rect_stored_ll_y;
            x2 = select_rect_stored_ur_x;
            y2 = select_rect_stored_ll_y + 
		scale_y * (select_rect_stored_ur_y - select_rect_stored_ll_y);
	    break;
          case glow_eScaleType_UpperLeft:
	    if ( select_rect_start_x == select_rect_stored_ll_x)
              return 1;
	    if ( select_rect_start_y == select_rect_stored_ur_y)
              return 1;
	    scale_x = double(x - select_rect_stored_ll_x) /
		(select_rect_start_x - select_rect_stored_ll_x);
	    scale_y = double(select_rect_stored_ur_y - y) /
		(select_rect_stored_ur_y - select_rect_start_y);
            if ( scale_equal) {
              scale = max( fabs(scale_x), fabs(scale_y));
              if ( scale_x >= 0)
                scale_x = scale;
              else
                scale_x = -scale;
              if ( scale_y >= 0)
                scale_y = scale;
              else
                scale_y = -scale; 
            }
	    x1 = select_rect_stored_ll_x;
	    y1 = select_rect_stored_ur_y;
            x2 = select_rect_stored_ll_x +
		scale_x * (select_rect_stored_ur_x - select_rect_stored_ll_x);
            y2 = select_rect_stored_ur_y - 
		scale_y * (select_rect_stored_ur_y - select_rect_stored_ll_y);
            break;
          case glow_eScaleType_Left:
	    if ( select_rect_start_x == select_rect_stored_ll_x)
              return 1;
            if ( scale_equal) 
              return 1;
	    scale_x = double(x - select_rect_stored_ll_x) /
		(select_rect_start_x - select_rect_stored_ll_x);
	    x1 = select_rect_stored_ll_x;
	    y1 = select_rect_stored_ll_y;
            x2 = select_rect_stored_ll_x +
		scale_x * (select_rect_stored_ur_x - select_rect_stored_ll_x);
            y2 = select_rect_stored_ur_y;
            break;
          case glow_eScaleType_Right:
	    if ( select_rect_start_x == select_rect_stored_ur_x)
              return 1;
            if ( scale_equal)
              return 1;
	    scale_x = double(select_rect_stored_ur_x - x) /
		(select_rect_stored_ur_x - select_rect_start_x);
	    x1 = select_rect_stored_ur_x;
	    y1 = select_rect_stored_ll_y;
            x2 = select_rect_stored_ur_x -
		scale_x * (select_rect_stored_ur_x - select_rect_stored_ll_x);
            y2 = select_rect_stored_ur_y;
            break;
          case glow_eScaleType_LowerRight:
	    if ( select_rect_start_x == select_rect_stored_ur_x)
              return 1;
	    if ( select_rect_start_y == select_rect_stored_ll_y)
              return 1;
	    scale_x = double(select_rect_stored_ur_x - x) /
		(select_rect_stored_ur_x - select_rect_start_x);
	    scale_y = double(y - select_rect_stored_ll_y) /
		( select_rect_start_y - select_rect_stored_ll_y);
            if ( scale_equal) {
              scale = max( fabs(scale_x), fabs(scale_y));
              if ( scale_x >= 0)
                scale_x = scale;
              else
                scale_x = -scale;
              if ( scale_y >= 0)
                scale_y = scale;
              else
                scale_y = -scale; 
            }
	    x1 = select_rect_stored_ur_x;
	    y1 = select_rect_stored_ll_y;
            x2 = select_rect_stored_ur_x -
		scale_x * (select_rect_stored_ur_x - select_rect_stored_ll_x);
            y2 = select_rect_stored_ll_y + 
		scale_y * (select_rect_stored_ur_y - select_rect_stored_ll_y);
            break;
          case glow_eScaleType_Up:
	    if ( select_rect_start_y == select_rect_stored_ur_y)
              return 1;
            if ( scale_equal)
              return 1;
	    scale_y = double(select_rect_stored_ur_y - y) /
		(select_rect_stored_ur_y - select_rect_start_y);
	    x1 = select_rect_stored_ll_x;
	    y1 = select_rect_stored_ur_y;
            x2 = select_rect_stored_ur_x;
            y2 = select_rect_stored_ur_y -
		scale_y * (select_rect_stored_ur_y - select_rect_stored_ll_y);
            break;
          case glow_eScaleType_UpperRight:
	    if ( select_rect_start_x == select_rect_stored_ur_x)
              return 1;
	    if ( select_rect_start_y == select_rect_stored_ur_y)
              return 1;
	    scale_x = double(select_rect_stored_ur_x - x) /
		(select_rect_stored_ur_x - select_rect_start_x);
	    scale_y = double(select_rect_stored_ur_y - y) /
		(select_rect_stored_ur_y - select_rect_start_y);
            if ( scale_equal) {
              scale = max( fabs(scale_x), fabs(scale_y));
              if ( scale_x >= 0)
                scale_x = scale;
              else
                scale_x = -scale;
              if ( scale_y >= 0)
                scale_y = scale;
              else
                scale_y = -scale; 
            }
	    x1 = select_rect_stored_ur_x;
	    y1 = select_rect_stored_ur_y;
            x2 = select_rect_stored_ur_x -
		scale_x * (select_rect_stored_ur_x - select_rect_stored_ll_x);
            y2 = select_rect_stored_ur_y - 
		scale_y * (select_rect_stored_ur_y - select_rect_stored_ll_y);
            break;
          default:
            ;
        }

        glow_draw_rect_erase( this, select_rect_ll_x, select_rect_ll_y,
	  	select_rect_ur_x - select_rect_ll_x,
	  	select_rect_ur_y - select_rect_ll_y, 0);

        modified = 1;

        select_rect_ll_x = int( min( x2, x1));
        select_rect_ll_y = int( min( y2, y1)); 
        select_rect_ur_x = int( max( x2, x1));
        select_rect_ur_y = int( max( y2, y1)); 

        draw( select_rect_ll_x, select_rect_ll_y,
	  select_rect_ur_x, select_rect_ur_y);

        scale_select( scale_x, scale_y, scale_type);

        select_rect_last_x = x;
        select_rect_last_y = y;
      }
      else if ( edit_mode == grow_eMode_EditPolyLine && node_movement_active)
      {
        ((GrowPolyLine *)a_move[0])->move_current_point(
		x - node_move_last_x, y - node_move_last_y, 0);
        node_move_last_x = x;
        node_move_last_y = y;
      }
      break;
    case glow_eEvent_ButtonRelease:
      if ( auto_scrolling_active)
        auto_scrolling_stop();
      if ( scale_active)
        scale_active = 0;
      if ( node_movement_active && edit_mode == grow_eMode_EditPolyLine)
      {
        ((GrowPolyLine *)a_move[0])->move_current_point(
		x - node_move_last_x, y - node_move_last_y, grid_on);
        node_move_last_x = x;
        node_move_last_y = y;
        node_movement_active = 0;
	nav_zoom();
        draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);
      }
      else if ( node_movement_active)
      {
        int move_x, move_y;
        double cursor_y, cursor_x;

        switch( move_restriction)
        {
          case glow_eMoveRestriction_No:
          case glow_eMoveRestriction_Vertical:
          case glow_eMoveRestriction_Horizontal:
            set_defered_redraw();
            if ( !grid_on || a_move.a_size == 1)
            {
              if ( move_restriction == glow_eMoveRestriction_No)
                a_move.move( x - node_move_last_x, y - node_move_last_y, grid_on);
              else if ( move_restriction == glow_eMoveRestriction_Vertical)
                a_move.move( 0, y - node_move_last_y, grid_on);
              else
                a_move.move( x - node_move_last_x, 0, grid_on);
            }
            else
            {
              // Keep the internal distances between the nodes
              double ll_x, ll_y, ur_x, ur_y, x_grid, y_grid;
              ur_x = -1e10;
              ll_x = 1e10;
              ur_y = -1e10;
              ll_y = 1e10;
              a_move.get_borders( &ur_x, &ll_x, &ur_y, &ll_y);
              find_grid( ll_x, ll_y, &x_grid, &y_grid);
              if ( move_restriction == glow_eMoveRestriction_No)
                a_move.move( 
			(int)(x - node_move_last_x + (x_grid - ll_x) * zoom_factor_x), 
			(int)(y - node_move_last_y + (y_grid - ll_y) * zoom_factor_y), 
			0);
              else if ( move_restriction == glow_eMoveRestriction_Vertical)
                a_move.move( 0, 
			(int)(y - node_move_last_y + (y_grid - ll_y) * zoom_factor_y), 
			0);
              else
                a_move.move( 
			(int)(x - node_move_last_x + (x_grid - ll_x) * zoom_factor_x), 
			0, 0);
            }
            node_move_last_x = x;
            node_move_last_y = y;
            redraw_defered();
            node_movement_active = 0;
	    nav_zoom();
            draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);

	    /* Send callback for all move objects */
	    if ( event_callback[glow_eEvent_ObjectMoved] )
	    {
	      static glow_sEvent e;
	
              e.event = glow_eEvent_ObjectMoved;
	      e.any.type = glow_eEventType_Object;
	      e.any.x_pixel = x;
	      e.any.y_pixel = y;
	      e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
	      e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
              for ( i = 0; i < a_move.size(); i++)
              {
	        e.object.object = a_move[i];
	        e.object.object_type = a_move[i]->type();
	        event_callback[event_move_node]( this, &e);
              }
	    }
            break;
          case glow_eMoveRestriction_VerticalSlider:
            cursor_y = double(y + offset_y) / zoom_factor_y;
            if ( cursor_y + slider_cursor_offset > restriction_max_limit) 
            {
              if ( double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset > restriction_max_limit)
                move_y = 0;
              else
                move_y = int( (restriction_max_limit - 
			(double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset)) * zoom_factor_y);
            }
            else if ( cursor_y + slider_cursor_offset < restriction_min_limit) 
            {
              if ( double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset < restriction_min_limit)
                move_y = 0;
              else
                move_y = int( (restriction_min_limit - 
			(double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset)) * zoom_factor_y);
            }
            else
            {
              if ( double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset > restriction_max_limit)
                move_y = int( (cursor_y + slider_cursor_offset - 
 			restriction_max_limit) * zoom_factor_y);
              else if ( double(node_move_last_y + offset_y) / zoom_factor_y +
			slider_cursor_offset < restriction_min_limit)
                move_y = int( (cursor_y + slider_cursor_offset - 
			restriction_min_limit) * zoom_factor_y);
              else
                move_y = y - node_move_last_y;
            }
            set_defered_redraw();
            if ( move_y)
              a_move.move( 0, move_y, 0);
            redraw_defered();
            node_movement_active = 0;
	    nav_zoom();
            draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);
	    if ( restriction_object && event_callback[glow_eEvent_SliderMoved] &&
	         a_move.size() == 1 &&
	         a_move[0]->type() == glow_eObjectType_GrowSlider)
	    {
	      static glow_sEvent e;
	
              e.event = glow_eEvent_SliderMoved;
	      e.any.type = glow_eEventType_Object;
	      e.any.x_pixel = x;
	      e.any.y_pixel = node_move_last_y + move_y;
	      e.any.x = double (x + offset_x) / zoom_factor_x;
	      e.any.y = double (node_move_last_y + move_y + offset_y) / zoom_factor_y;
	      e.object.object = restriction_object;
	      e.object.object_type = restriction_object->type();
	      event_callback[glow_eEvent_SliderMoved]( this, &e);

	      // Send slider movement end event
              e.event = glow_eEvent_SliderMoveStart;
	      e.any.type = glow_eEventType_Object;
	      e.any.x_pixel = x;
	      e.any.y_pixel = y;
	      e.any.x = double (x + offset_x) / zoom_factor_x;
	      e.any.y = double (y + offset_y) / zoom_factor_y;
	      e.object.object = NULL;
	      e.object.object_type = glow_eObjectType_NoObject;
	      event_callback[glow_eEvent_SliderMoveStart]( this, &e);
	    }
            node_move_last_x = x;
            node_move_last_y = y;
            break;
          case glow_eMoveRestriction_HorizontalSlider:
            cursor_x = double(x + offset_x) / zoom_factor_x;
            if ( cursor_x + slider_cursor_offset > restriction_max_limit) 
            {
              if ( double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset > restriction_max_limit)
                move_x = 0;
              else
                move_x = int( (restriction_max_limit - 
			(double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset)) * zoom_factor_x);
            }
            else if ( cursor_x + slider_cursor_offset < restriction_min_limit) 
            {
              if ( double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset < restriction_min_limit)
                move_x = 0;
              else
                move_x = int( (restriction_min_limit - 
			(double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset)) * zoom_factor_x);
            }
            else
            {
              if ( double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset > restriction_max_limit)
                move_x = int( (cursor_x + slider_cursor_offset - 
 			restriction_max_limit) * zoom_factor_x);
              else if ( double(node_move_last_x + offset_x) / zoom_factor_x +
			slider_cursor_offset < restriction_min_limit)
                move_x = int( (cursor_x + slider_cursor_offset - 
			restriction_min_limit) * zoom_factor_x);
              else
                move_x = x - node_move_last_x;
            }
            set_defered_redraw();
            if ( move_x)
              a_move.move( move_x, 0, 0);
            redraw_defered();
            node_movement_active = 0;
	    nav_zoom();
            draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);
	    if ( restriction_object && event_callback[glow_eEvent_SliderMoved] &&
	         a_move.size() == 1 &&
	         a_move[0]->type() == glow_eObjectType_GrowSlider)
	    {
	      static glow_sEvent e;
	
              e.event = glow_eEvent_SliderMoved;
	      e.any.type = glow_eEventType_Object;
	      e.any.x_pixel = node_move_last_x + move_x;
	      e.any.y_pixel = y;
	      e.any.x = double (node_move_last_x + move_x+ offset_x) / zoom_factor_x;
	      e.any.y = double (y + offset_y) / zoom_factor_y;
	      e.object.object = restriction_object;
	      e.object.object_type = restriction_object->type();
	      event_callback[glow_eEvent_SliderMoved]( this, &e);

	      // Send slider movement end event
              e.event = glow_eEvent_SliderMoveStart;
	      e.any.type = glow_eEventType_Object;
	      e.any.x_pixel = x;
	      e.any.y_pixel = y;
	      e.any.x = double (x + offset_x) / zoom_factor_x;
	      e.any.y = double (y + offset_y) / zoom_factor_y;
	      e.object.object = NULL;
	      e.object.object_type = glow_eObjectType_NoObject;
	      event_callback[glow_eEvent_SliderMoveStart]( this, &e);
	    }
            node_move_last_x = x;
            node_move_last_y = y;
            break;
          case glow_eMoveRestriction_Disable:
            break;
	}
      }
      else if ( select_rect_active && edit_mode != grow_eMode_Scale)
      {
        glow_eSelectPolicy policy;

        if ( ctx->select_policy == glow_eSelectPolicy_Both) {
          if ( x < select_rect_start_x)
            policy = glow_eSelectPolicy_Partial;
          else
            policy = glow_eSelectPolicy_Surround;
        }
        else
          policy = ctx->select_policy;

        select_rect_active = 0;
        select_rect_last_x = x;
        select_rect_last_y = y;

        if ( edit_mode == grow_eMode_Circle)
          glow_draw_arc_erase( this, select_rect_ll_x, select_rect_ll_y,
	  	select_rect_ur_x - select_rect_ll_x,
	  	select_rect_ur_y - select_rect_ll_y, 0, 360, 0);
	else
          glow_draw_rect_erase( this, select_rect_ll_x, select_rect_ll_y,
	  	select_rect_ur_x - select_rect_ll_x,
	  	select_rect_ur_y - select_rect_ll_y, 0);

        if ( scale_equal &&
             (edit_mode == grow_eMode_Rect ||
              edit_mode == grow_eMode_Circle))
        {
          int delta_x, delta_y;
          delta_y = delta_x = max( abs(x - select_rect_start_x),
		       abs(y - select_rect_start_y));
          if ( x < select_rect_start_x)
            delta_x = - delta_x;
          if ( y < select_rect_start_y)
            delta_y = - delta_y;

          select_rect_ll_x = 
	    min( select_rect_start_x, select_rect_start_x + delta_x);
          select_rect_ll_y = 
	    min( select_rect_start_y, select_rect_start_y + delta_y);
          select_rect_ur_x = 
	    max( select_rect_start_x, select_rect_start_x + delta_x);
          select_rect_ur_y = 
	    max( select_rect_start_y, select_rect_start_y + delta_y);
        }
        else {
          select_rect_ll_x = min( x, select_rect_start_x);
          select_rect_ll_y = min( y, select_rect_start_y); 
          select_rect_ur_x = max( x, select_rect_start_x);
          select_rect_ur_y = max( y, select_rect_start_y);
        }

        draw( select_rect_ll_x, select_rect_ll_y,
	  select_rect_ur_x + 1, select_rect_ur_y + 1);

        /* Save the final select area */
        select_area_ll_x = (select_rect_ll_x + offset_x) / zoom_factor_x;
        select_area_ll_y = (select_rect_ll_y + offset_y) / zoom_factor_y;
        select_area_ur_x = (select_rect_ur_x + offset_x) / zoom_factor_x;
        select_area_ur_y = (select_rect_ur_y + offset_y) / zoom_factor_y;

        if ( edit_mode == grow_eMode_Rect ||
             edit_mode == grow_eMode_Circle ||
             edit_mode == grow_eMode_Bar ||
             edit_mode == grow_eMode_Trend ||
             edit_mode == grow_eMode_Axis)
        {
          if ( event_callback[glow_eEvent_CreateGrowObject] )
          {
            static glow_sEvent e;

            e.event = glow_eEvent_CreateGrowObject;
            e.any.type = glow_eEventType_CreateGrowObject;
            e.any.x_pixel = x;
            e.any.y_pixel = y;
            e.any.x = select_area_ll_x;
            e.any.y = select_area_ll_y;
            e.create_grow_object.mode = edit_mode;
            e.create_grow_object.x2 = select_area_ur_x;
            e.create_grow_object.y2 = select_area_ur_y;
            event_callback[glow_eEvent_CreateGrowObject]( this, &e);
          }
        }
        else
        {
          if ( select_rect_event == event_region_select)
          {
	    /* Insert selected objects to selectlist */

            select_clear();
            select_region_insert( select_area_ll_x, select_area_ll_y,
		select_area_ur_x, select_area_ur_y, policy);
          }
          if ( select_rect_event == event_region_add_select)
          {
	    /* Add selected objects to selectlist */
            select_region_insert( select_area_ll_x, select_area_ll_y,
		select_area_ur_x, select_area_ur_y, policy);
          }
          /* Send event backcall */
          if ( event_callback[select_rect_event] )
          {
            static glow_sEvent e;

            e.event = select_rect_event;
            e.any.type = glow_eEventType_Object;
            e.any.x_pixel = x;
            e.any.y_pixel = y;
            e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
            e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
            e.object.object_type = glow_eObjectType_NoObject;
            e.object.object = 0;
            event_callback[select_rect_event]( this, &e);
          }
        }
      }
      else if ( select_rect_active && edit_mode != grow_eMode_Scale)
      {

      }
      else if ( con_create_active)
      {
        glow_draw_line_erase( this, con_create_conpoint_x,
		con_create_conpoint_y, con_create_last_x, con_create_last_y, 0);
        draw( con_create_conpoint_x, con_create_conpoint_y, 
		con_create_last_x, con_create_last_y);

        if ( edit_mode == grow_eMode_Line || 
             edit_mode == grow_eMode_PolyLine)
        {
          switch( move_restriction) {
            case glow_eMoveRestriction_Vertical:
              x = con_create_conpoint_x;
	      break;
            case glow_eMoveRestriction_Horizontal:
              y = con_create_conpoint_y;
	      break;
	    default:
	      ;
          }
          if ( edit_mode == grow_eMode_PolyLine)
          {
            // If move restrictinos, switch direction for next point
            switch( move_restriction) {
              case glow_eMoveRestriction_Vertical:
                move_restriction = glow_eMoveRestriction_Horizontal;
	        break;
              case glow_eMoveRestriction_Horizontal:
                move_restriction = glow_eMoveRestriction_Vertical;
	        break;
	      default:
	        ;
            }
	    if ( !grid_on)
            {
              polyline_last_end_x = x;
              polyline_last_end_y = y;
	    }
            else
            {
	      double grid_x, grid_y;

              find_grid( double(x + offset_x) / zoom_factor_x, 
		double( y + offset_y) / zoom_factor_y, &grid_x, &grid_y);
              polyline_last_end_x = int( grid_x * zoom_factor_x) - offset_x;
              polyline_last_end_y = int( grid_y * zoom_factor_y) - offset_y;
            }
          }
          if ( event_callback[glow_eEvent_CreateGrowObject] )
          {
            static glow_sEvent e;

            e.event = glow_eEvent_CreateGrowObject;
            e.any.type = glow_eEventType_CreateGrowObject;
            e.any.x_pixel = x;
            e.any.y_pixel = y;
            e.any.x = (x + offset_x) / zoom_factor_x;
            e.any.y = (y + offset_y) / zoom_factor_y;
            e.create_grow_object.mode = edit_mode;
            e.create_grow_object.x2 = (con_create_conpoint_x + offset_x) / zoom_factor_x;
            e.create_grow_object.y2 = (con_create_conpoint_y + offset_y) / zoom_factor_y;
            e.create_grow_object.first_line = !polyline_not_first;
            event_callback[glow_eEvent_CreateGrowObject]( this, &e);
          }
          if ( edit_mode == grow_eMode_PolyLine && !polyline_not_first)
            polyline_not_first = 1;
        }
        else
        {

	  /* Find the destination node */
          for ( i = 0; i < a.a_size; i++)
          {
            sts = a.a[a.a_size-i-1]->event_handler( event, x, y, fx, fy);
            if ( sts)
              break;
          }
	  if ( !sts)
	  {
	    /* No hit */
	    if ( event_callback[event_create_con] )
	    {
	      static glow_sEvent e;
	
	      e.event = event;
	      e.any.type = glow_eEventType_CreateCon;
	      e.any.x_pixel = x;
	      e.any.y_pixel = y;
	      e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
	      e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
	      e.con_create.source_object = con_create_node;
	      e.con_create.source_conpoint = con_create_conpoint_num;
	      e.con_create.dest_object = 0;
	      e.con_create.dest_conpoint = 0;
	      event_callback[event_create_con]( this, &e);
	    }
	  }
	}
        con_create_active = 0;
      }
      break;
    case glow_eEvent_Enter:
      cursor_present = 1;
      cursor_x = x;
      cursor_y = y;
      if ( node_movement_paste_pending)
      {
        node_movement_paste_pending = 0;
        paste_execute();
      }
      if ( auto_scrolling_active)
        auto_scrolling_stop();
      break;
    case glow_eEvent_Leave:
      cursor_present = 0;
      if ( node_movement_active || con_create_active || select_rect_active ||
	   node_movement_paste_active)
      {
        if ( x < 0 || x > ctx->window_width || y < 0 || y > window_height)
        {
          /* Start auto scrolling */
          auto_scrolling( this);
        }
      }
      else if ( x < 0 || x > ctx->window_width || y < 0 || y > window_height)
        a.set_hot( 0);
      break;
    case glow_eEvent_VisibilityUnobscured:
      unobscured = 1;
      break;
    case glow_eEvent_VisibilityObscured:
      unobscured = 0;
      break;
    default:
      ;
  }

  if ( event_callback[event] &&
	sts != GLOW__NO_PROPAGATE && event != event_move_node)
  {
    static glow_sEvent e;

    e.event = event;
    e.any.type = glow_eEventType_Object;
    e.any.x_pixel = x;
    e.any.y_pixel = y;
    e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
    e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
    e.object.object_type = callback_object_type;
    if ( callback_object_type != glow_eObjectType_NoObject)
      e.object.object = callback_object;
    event_callback[event]( this, &e);
  }
  return 1;
}

int GrowCtx::save( char *filename, glow_eSaveMode mode)
{
  // Remove unused nodeclasses
  int found, i, j;
  GlowArrayElem	*element;

  for ( i = 0; i < a_nc.a_size; i++)
  {
    found = a.find_nc( a_nc[i]);
    if ( !found)
      found = a_paste.find_nc( a_nc[i]);
    if ( !found)
    {
      for ( j = 0; j < a_nc.a_size; j++)
      {
        if ( ((GlowNodeClass *)a_nc[j])->next_nc == a_nc[i])
        {
          found = 1;
          break;
        }
      }
    }
    if ( !found)
    {
      element = a_nc.a[i];
      a_nc.remove(element);
      delete element;
      i--;
    }
  }

  return GlowCtx::save( filename, mode);
}

void GrowCtx::save_grow( ofstream& fp, glow_eSaveMode mode)
{
  int i;
  char *s;

  fp << int(glow_eSave_GrowCtx) << endl;
  fp << int(glow_eSave_GrowCtx_conpoint_num_cnt) << FSPACE << conpoint_num_cnt << endl;
  fp << int(glow_eSave_GrowCtx_objectname_cnt) << FSPACE << objectname_cnt << endl;
  fp << int(glow_eSave_GrowCtx_name) << FSPACE << name << endl;
  fp << int(glow_eSave_GrowCtx_background_color) << FSPACE << int(background_color) << endl;
  fp << int(glow_eSave_GrowCtx_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_GrowCtx_dynamic) << endl;
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
  fp << int(glow_eSave_GrowCtx_arg_cnt) << FSPACE << arg_cnt << endl;
  fp << int(glow_eSave_GrowCtx_argname) << endl;  
  for ( i = 0; i < arg_cnt; i++)
    fp << argname[i] << endl;
  fp << int(glow_eSave_GrowCtx_argtype) << endl;  
  for ( i = 0; i < arg_cnt; i++)
    fp << argtype[i] << endl;
  fp << int(glow_eSave_GrowCtx_x0) << FSPACE << x0 << endl;  
  fp << int(glow_eSave_GrowCtx_y0) << FSPACE << y0 << endl;  
  fp << int(glow_eSave_GrowCtx_x1) << FSPACE << x1 << endl;  
  fp << int(glow_eSave_GrowCtx_y1) << FSPACE << y1 << endl;  
  fp << int(glow_eSave_GrowCtx_path_cnt) << FSPACE << path_cnt << endl;
  fp << int(glow_eSave_GrowCtx_path) << endl;  
  for ( i = 0; i < path_cnt; i++)
    fp << path[i] << endl;
  fp << int(glow_eSave_GrowCtx_trace_attr_type) << FSPACE << int(trace_attr_type) << endl;
  fp << int(glow_eSave_GrowCtx_trace_color) << FSPACE << int(trace_color) << endl;
  fp << int(glow_eSave_GrowCtx_trace_color2) << FSPACE << int(trace_color2) << endl;
  fp << int(glow_eSave_GrowCtx_no_con_obstacle) << FSPACE << no_con_obstacle << endl;
  fp << int(glow_eSave_GrowCtx_slider) << FSPACE << slider << endl;
  fp << int(glow_eSave_GrowCtx_subgraph) << FSPACE << subgraph << endl;
  fp << int(glow_eSave_GrowCtx_java_name) << FSPACE << java_name << endl;
  fp << int(glow_eSave_GrowCtx_is_javaapplet) << FSPACE << is_javaapplet << endl;
  fp << int(glow_eSave_GrowCtx_is_javaapplication) << FSPACE << is_javaapplication << endl;
  fp << int(glow_eSave_GrowCtx_next_subgraph) << FSPACE << next_subgraph << endl;
  fp << int(glow_eSave_GrowCtx_animation_count) << FSPACE << animation_count << endl;
  fp << int(glow_eSave_GrowCtx_scantime) << FSPACE << scantime << endl;
  fp << int(glow_eSave_GrowCtx_fast_scantime) << FSPACE << fast_scantime << endl;
  fp << int(glow_eSave_GrowCtx_animation_scantime) << FSPACE << animation_scantime << endl;
  fp << int(glow_eSave_GrowCtx_java_width) << FSPACE << java_width << endl;
  fp << int(glow_eSave_GrowCtx_background_image) << FSPACE << background_image << endl;
  fp << int(glow_eSave_GrowCtx_background_tiled) << FSPACE << background_tiled << endl;
  fp << int(glow_eSave_GrowCtx_double_buffered) << FSPACE << double_buffered << endl;
  fp << int(glow_eSave_GrowCtx_cycle) << FSPACE << int(cycle) << endl;
  fp << int(glow_eSave_GrowCtx_mb3_action) << FSPACE << int(mb3_action) << endl;
  fp << int(glow_eSave_GrowCtx_translate_on) << FSPACE << translate_on << endl;
  fp << int(glow_eSave_End) << endl;
}

void GrowCtx::open_grow( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;
  int		i, j;
  char		c;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowCtx: break;
      case glow_eSave_GrowCtx_conpoint_num_cnt: fp >> conpoint_num_cnt; break;
      case glow_eSave_GrowCtx_objectname_cnt: fp >> objectname_cnt; break;
      case glow_eSave_GrowCtx_name:
        fp.get();
        fp.getline( name, sizeof(name));
        break;
      case glow_eSave_GrowCtx_background_color: 
	fp >> tmp;
	background_color = (glow_eDrawType)tmp;
	break;
      case glow_eSave_GrowCtx_dynamicsize: fp >> dynamicsize; break;
      case glow_eSave_GrowCtx_dynamic:
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
      case glow_eSave_GrowCtx_arg_cnt: fp >> arg_cnt; break;
      case glow_eSave_GrowCtx_argname:
        fp.get();
        for ( i = 0; i < arg_cnt; i++)
        {
          fp.getline( argname[i], sizeof(argname[0]));
        }
        break;
      case glow_eSave_GrowCtx_argtype:
        for ( i = 0; i < arg_cnt; i++)
          fp >> argtype[i];
        break;
      case glow_eSave_GrowCtx_x0: fp >> x0; break;
      case glow_eSave_GrowCtx_y0: fp >> y0; break;
      case glow_eSave_GrowCtx_x1: fp >> x1; break;
      case glow_eSave_GrowCtx_y1: fp >> y1; break;
      case glow_eSave_End: end_found = 1; break;
      case glow_eSave_GrowCtx_path_cnt: fp >> path_cnt; break;
      case glow_eSave_GrowCtx_path:
        fp.get();
        for ( i = 0; i < path_cnt; i++)
        {
          fp.getline( path[i], sizeof(path[0]));
        }
        break;
      case glow_eSave_GrowCtx_trace_attr_type: 
	fp >> tmp;
	trace_attr_type = (glow_eTraceType)tmp;
	break;
      case glow_eSave_GrowCtx_trace_color: 
	fp >> tmp;
	trace_color = (glow_eDrawType)tmp;
	break;
      case glow_eSave_GrowCtx_trace_color2: 
	fp >> tmp;
	trace_color2 = (glow_eDrawType)tmp;
	break;
      case glow_eSave_GrowCtx_no_con_obstacle: fp >> no_con_obstacle; break;
      case glow_eSave_GrowCtx_slider: fp >> slider; break;
      case glow_eSave_GrowCtx_subgraph: fp >> subgraph; break;
      case glow_eSave_GrowCtx_java_name:
        fp.get();
        fp.getline( java_name, sizeof(java_name));
        break;
      case glow_eSave_GrowCtx_is_javaapplet: fp >> is_javaapplet; break;
      case glow_eSave_GrowCtx_is_javaapplication: fp >> is_javaapplication; break;
      case glow_eSave_GrowCtx_next_subgraph:
        fp.get();
        fp.getline( next_subgraph, sizeof(next_subgraph));
        break;
      case glow_eSave_GrowCtx_animation_count: fp >> animation_count; break;
      case glow_eSave_GrowCtx_scantime: fp >> scantime; break;
      case glow_eSave_GrowCtx_fast_scantime: fp >> fast_scantime; break;
      case glow_eSave_GrowCtx_animation_scantime: fp >> animation_scantime; break;
      case glow_eSave_GrowCtx_java_width: fp >> java_width; break;
      case glow_eSave_GrowCtx_background_image:
        fp.get();
        fp.getline( background_image, sizeof(background_image));
        break;
      case glow_eSave_GrowCtx_background_tiled: fp >> background_tiled; break;
      case glow_eSave_GrowCtx_double_buffered: fp >> double_buffered; break;
      case glow_eSave_GrowCtx_cycle: 
	fp >> tmp;
	cycle = (glow_eCycle)tmp;
	break;
      case glow_eSave_GrowCtx_mb3_action: 
	fp >> tmp;
	mb3_action = (glow_eMB3Action)tmp;
	break;
      case glow_eSave_GrowCtx_translate_on: fp >> translate_on; break;
      default:
        cout << "GrowCtx:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  if ( double_buffered && ! double_buffer_on) {
    double_buffer_on = 1;
    glow_draw_create_buffer( this);
  }
  set_background( background_color);
}

int GrowCtx::save_subgraph( char *filename, glow_eSaveMode mode)
{
  ofstream	fp;
  char		nc_name[80];
  char		*s;
  int		i;

  if ( (s = strrchr( filename, ':')))
    strcpy( nc_name, s+1);
  else if ( (s = strrchr( filename, '/')))
    strcpy( nc_name, s+1);
  else
    strcpy( nc_name, filename);
  if ( (s = strrchr( nc_name, '.')))
    *s = 0;
  fp.open( filename);
#ifndef OS_VMS
  if ( !fp)
    return GLOW__FILEOPEN;
#endif


  // Save as nc_array
  fp <<	int(glow_eSave_Ctx_a_nc) << endl;
  fp <<	int(glow_eSave_Array) << endl;

  // Save as NodeClass
  fp <<	int(glow_eSave_NodeClass) << endl;
  fp <<	int(glow_eSave_NodeClass_nc_name) << FSPACE << nc_name << endl;
  fp <<	int(glow_eSave_NodeClass_a) << endl;
  a.save( fp, mode);
  fp <<	int(glow_eSave_NodeClass_group) << FSPACE << int(glow_eNodeGroup_Common) << endl;
  fp << int(glow_eSave_NodeClass_arg_cnt) << FSPACE << arg_cnt << endl;
  fp << int(glow_eSave_NodeClass_argname) << endl;  
  for ( i = 0; i < arg_cnt; i++)
    fp << argname[i] << endl;
  fp << int(glow_eSave_NodeClass_argtype) << endl;
  for ( i = 0; i < arg_cnt; i++)
    fp << argtype[i] << endl;
  fp << int(glow_eSave_NodeClass_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_NodeClass_dynamic) << endl;
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
  fp << int(glow_eSave_NodeClass_trace_attr_type) << FSPACE << int(trace_attr_type) << endl;
  fp << int(glow_eSave_NodeClass_trace_color) << FSPACE << int(trace_color) << endl;
  fp << int(glow_eSave_NodeClass_trace_color2) << FSPACE << int(trace_color2) << endl;
  fp << int(glow_eSave_NodeClass_no_con_obstacle) << FSPACE << no_con_obstacle << endl;
  fp << int(glow_eSave_NodeClass_slider) << FSPACE << slider << endl;
  fp << int(glow_eSave_NodeClass_java_name) << FSPACE << java_name << endl;
  fp << int(glow_eSave_NodeClass_next_nodeclass) << FSPACE << next_subgraph << endl;
  fp << int(glow_eSave_NodeClass_animation_count) << FSPACE << animation_count << endl;
  fp << int(glow_eSave_NodeClass_cycle) << FSPACE << int(cycle) << endl;
  fp << int(glow_eSave_NodeClass_y0) << FSPACE << y0 << endl;  
  fp << int(glow_eSave_NodeClass_y1) << FSPACE << y1 << endl;  
  fp << int(glow_eSave_NodeClass_x0) << FSPACE << x0 << endl;  
  fp << int(glow_eSave_NodeClass_x1) << FSPACE << x1 << endl;  
  fp <<	int(glow_eSave_End) << endl;

  // End Array
  fp << int(glow_eSave_End) << endl;

  fp << int(glow_eSave_End) << endl;

  fp.close();
  return 1;
}

int GrowCtx::open_subgraph_from_name( char *name, glow_eSaveMode mode)
{
  char filename[120];
  char path_name[120];

  strcpy( filename, name);
  strcat( filename, ".pwsg");

  if ( check_file( filename))
    return open_subgraph( filename, glow_eSaveMode_SubGraph);

  // Add some search path
  for ( int i = 0; i < path_cnt; i++)
  {
    strcpy( path_name, path[i]);
    strcat( path_name, filename);
    dcli_translate_filename( path_name, path_name);
    if ( check_file( path_name))
      return open_subgraph( path_name, glow_eSaveMode_SubGraph);
  }

  return GLOW__FILEOPEN;
}

int GrowCtx::open_subgraph( char *filename, glow_eSaveMode mode)
{
  ifstream	fp;
  int		type;
  int		end_found = 0;
  char		dummy[40];
  char		fname[120];

  dcli_translate_filename( fname, filename);

  if ( !check_file( fname))
    return GLOW__FILEOPEN;

  fp.open( fname);
#ifndef OS_VMS
  if ( !fp)
    return GLOW__FILEOPEN;
#endif

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Ctx_a_nc: a_nc.open( this, fp); break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowCtx:open_subgraph syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
   }
   if ( end_found)
     break;
  }
  fp.close();
  return 1;
}

void GrowCtx::clear_all( int keep_paste)
{ 
  int i;
  GlowArrayElem	*element;

  delete_all();
  conpoint_num_cnt = 0;
  objectname_cnt = 0;
  subgraph = 0;
  scantime = 0.5;
  fast_scantime = 0.5;
  animation_scantime = 0.5;
  java_width = 0;
  is_javaapplet = 0;
  is_javaapplication = 0;
  strcpy( name, "");
  strcpy( java_name, "");
  strcpy( background_image, "");
  background_tiled = 0;
  double_buffered = 0;
  is_javaapplet = 0;
  is_javaapplication = 0;
  cycle = glow_eCycle_Slow;
  mb3_action = glow_eMB3Action_Close;
  translate_on = 0;

  nav_clear();
  set_nodraw();
  zoom_absolute( base_zoom_factor);
  offset_x = 0;
  offset_y = 0;

  if ( !keep_paste)
  {
    for ( i = a_nc.a_size - 1; i >= 0; i--)
    {
      element = a_nc.a[i];
      a_nc.remove(element);
      delete element;
    }
    a_paste.a_size = 0;  
  }
  else
  {
    // Remove all nodeclasses except for nodes in pastebuffer
    int found, i, j;

    for ( i = a_nc.a_size - 1; i >= 0; i--)
    {
      found = 0;
      for ( j = 0; j < a_paste.a_size; j++)
      {
        if ( a_paste[j]->type() == glow_eObjectType_GrowNode ||
             a_paste[j]->type() == glow_eObjectType_GrowSlider)
	{
          found = ((GrowNode *) a_paste[j])->find_nc( a_nc[i]);
          if ( found)
            break;
        }
      }
      if ( !found)
      {
        element = a_nc.a[i];
        a_nc.remove(element);
        delete element;
      }
    }
  }
  reset_nodraw();
  // if ( show_grid)
  //  draw_grid( 0, 0, window_width, window_height);
  draw( 0, 0, window_width, window_height);
}

void GrowCtx::redraw_defered()
{
  defered_redraw_active--;
  if ( !defered_redraw_active )
  {
    draw( defered_x_low, defered_y_low, defered_x_high, defered_y_high);
    nav_draw( defered_x_low_nav, defered_y_low_nav, defered_x_high_nav, 
	defered_y_high_nav);
  }
}

void GrowCtx::draw( int ll_x, int ll_y, int ur_x, int ur_y)
{
  int		i;

  if ( defered_redraw_active)
  {
    if ( ll_x < defered_x_low)
      defered_x_low = ll_x;
    if ( ll_y < defered_y_low)
      defered_y_low = ll_y;
    if ( ur_x > defered_x_high)
      defered_x_high = ur_x;
    if ( ur_y > defered_y_high)
      defered_y_high = ur_y;
     return;
  }
#if 0
  int		loc_ll_x = ll_x;
  int		loc_ll_y = ll_y;
  int		loc_ur_x = ur_x;
  int		loc_ur_y = ur_y;

  for ( i = 0; i < a.a_size; i++)
  {
    a.a[i]->draw( &loc_ll_x, &loc_ll_y, &loc_ur_x, &loc_ur_y);
  }
#endif
  glow_draw_set_clip_rectangle( this, ll_x, ll_y, ur_x, ur_y);
  set_draw_buffer_only();
  if ( double_buffer_on)
    glow_draw_buffer_background( this);
  for ( i = 0; i < a.a_size; i++)
  {
    a.a[i]->draw( ll_x, ll_y, ur_x, ur_y);
  }
  if ( show_grid)
    draw_grid( ll_x, ll_y, ur_x, ur_y);
  if ( double_buffer_on)
    glow_draw_copy_buffer( this, ll_x, ll_y, ur_x, ur_y);
  reset_draw_buffer_only();
  glow_draw_reset_clip_rectangle( this);

  if ( select_rect_active)
  {
    if ( edit_mode == grow_eMode_Circle)
      glow_draw_arc( this, select_rect_ll_x, select_rect_ll_y, 
	  select_rect_ur_x - select_rect_ll_x,
	  select_rect_ur_y - select_rect_ll_y, 0, 360, glow_eDrawType_Line, 0, 0);
    else
      glow_draw_rect( this, select_rect_ll_x, select_rect_ll_y, 
	  select_rect_ur_x - select_rect_ll_x,
	  select_rect_ur_y - select_rect_ll_y, glow_eDrawType_Line, 0, 0);
  }
}

void GrowCtx::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y)
{
  int		i;
  int		loc_ll_x = ll_x;
  int		loc_ll_y = ll_y;
  int		loc_ur_x = ur_x;
  int		loc_ur_y = ur_y;

  if ( defered_redraw_active)
  {
    if ( ll_x < defered_x_low_nav)
      defered_x_low_nav = ll_x;
    if ( ll_y < defered_y_low_nav)
      defered_y_low_nav = ll_y;
    if ( ur_x > defered_x_high_nav)
      defered_x_high_nav = ur_x;
    if ( ur_y > defered_y_high_nav)
      defered_y_high_nav = ur_y;
    return;
  }
  for ( i = 0; i < a.a_size; i++)
  {
    a.a[i]->nav_draw( &loc_ll_x, &loc_ll_y, &loc_ur_x, &loc_ur_y);
  }

  nav_rect_ll_x = int( nav_zoom_factor_x * offset_x / zoom_factor_x - nav_offset_x);
  nav_rect_ur_x = int( nav_zoom_factor_x * (offset_x + window_width) / 
		zoom_factor_x - nav_offset_x);
  nav_rect_ll_y = int( nav_zoom_factor_y * offset_y / zoom_factor_y - nav_offset_y);
  nav_rect_ur_y = int( nav_zoom_factor_y * (offset_y + window_height) / 
		zoom_factor_y - nav_offset_y);

//  cout << "Nav rect: ll : " << nav_rect_ll_x << "," << nav_rect_ll_y << 
//	" ur: " << nav_rect_ur_x << "," << nav_rect_ur_y << endl;
  glow_draw_nav_rect( this, nav_rect_ll_x, nav_rect_ll_y, 
	nav_rect_ur_x - nav_rect_ll_x, nav_rect_ur_y - nav_rect_ll_y,
	glow_eDrawType_Line, 0, 0);
}

void GrowCtx::dynamic_cb( GlowArrayElem *object, char *code,
	glow_eDynamicType type)
{
  if ( event_callback[glow_eEvent_GrowDynamics] )
  {
    /* Send a grow dynamics callback */
    static glow_sEvent e;

    e.event = glow_eEvent_GrowDynamics;
    e.any.type = glow_eEventType_Object;
    e.any.x_pixel = 0;
    e.any.y_pixel = 0;
    e.any.x = 0;
    e.any.y = 0;
    e.dynamics.object_type = object->type();
    e.dynamics.object = object;
    e.dynamics.code = code;
    e.dynamics.dynamic_type = type;
    event_callback[glow_eEvent_GrowDynamics]( this, &e);
  }
}

void GrowCtx::scale_select( double scale_x, double scale_y, 
	glow_eScaleType type)
{
  double sx, sy, x0, y0;
  double ll_x, ll_y, ur_x, ur_y;
  GlowTransform t;

  switch( type)
  {
    case glow_eScaleType_LowerLeft:
    case glow_eScaleType_LowerRight:
    case glow_eScaleType_UpperRight:
    case glow_eScaleType_UpperLeft:
      x0 = scale_center_x;
      y0 = scale_center_y;
      sx = scale_x;
      sy = scale_y;
      break;
    case glow_eScaleType_Left:
    case glow_eScaleType_Right:
      x0 = scale_center_x;
      y0 = 0;
      sx = scale_x;
      sy = 1;
      break;
    case glow_eScaleType_Up:
    case glow_eScaleType_Down:
      x0 = 0;
      y0 = scale_center_y;
      sx = 1;
      sy = scale_y;
      break;
    default:
      ;
  }

  t.scale( sx, sy, x0, y0);
  a_sel.erase( (GlowTransform *)NULL, 0, NULL);
  // Some objects might be hot, erase as hot also
  a_sel.erase( (GlowTransform *)NULL, 1, NULL);
  a_sel.nav_erase( (GlowTransform *)NULL, NULL);
  set_defered_redraw();
  ur_x = ur_y = -1e10;
  ll_x = ll_y = 1e10;
  a_sel.get_borders( &ur_x, &ll_x, &ur_y, &ll_y);
  draw( int(ll_x * zoom_factor_x) - offset_x - DRAW_MP, 
        int(ll_y * zoom_factor_y) - offset_y - DRAW_MP,
        int(ur_x * zoom_factor_x) - offset_x + DRAW_MP,
        int(ur_y * zoom_factor_y) - offset_y + DRAW_MP);
  a_sel.set_transform_from_stored( &t);
  //  a_sel.draw( (GlowTransform *)NULL, 1, 0, NULL, NULL);
  ur_x = ur_y = -1e10;
  ll_x = ll_y = 1e10;
  a_sel.get_borders( &ur_x, &ll_x, &ur_y, &ll_y);
  draw( int(ll_x * zoom_factor_x) - offset_x - DRAW_MP,
        int(ll_y * zoom_factor_y) - offset_y - DRAW_MP,
        int(ur_x * zoom_factor_x) - offset_x + DRAW_MP,
        int(ur_y * zoom_factor_y) - offset_y + DRAW_MP);
  a_sel.nav_draw( (GlowTransform *)NULL, 1, NULL, NULL);
  for ( int i = 0; i < a_sel.size(); i++)
    redraw_node_cons( a_sel[i]);
  redraw_defered();
}

void GrowCtx::rotate_select( double angel, glow_eRotationPoint type)
{
  double ll_x, ll_y, ur_x, ur_y;
  double x0, y0;
  GlowTransform t;

  ur_x = ur_y = -1e10;
  ll_x = ll_y = 1e10;
  a_sel.get_borders( &ur_x, &ll_x, &ur_y, &ll_y);
  
  switch( type)
  {
    case glow_eRotationPoint_LowerLeft:
      x0 = ll_x;
      y0 = ll_y;
      break;
    case glow_eRotationPoint_LowerRight:
      x0 = ur_x;
      y0 = ll_y;
      break;
    case glow_eRotationPoint_UpperRight:
      x0 = ur_x;
      y0 = ur_y;
      break;
    case glow_eRotationPoint_UpperLeft:
      x0 = ll_x;
      y0 = ur_y;
      break;
    case glow_eRotationPoint_Center:
      x0 = (ur_x + ll_x) / 2;
      y0 = (ur_y + ll_y) / 2;
      break;
    default:
      ;
  }
  t.rotate( angel, x0, y0);
  a_sel.erase( (GlowTransform *)NULL, 0, NULL);
  a_sel.nav_erase( (GlowTransform *)NULL, NULL);
  set_defered_redraw();
  a_sel.set_transform( &t);
  // a_sel.draw( (GlowTransform *)NULL, 1, 0, NULL, NULL);
  draw( ll_x * zoom_factor_x - offset_x - DRAW_MP,
        ll_y * zoom_factor_y - offset_y - DRAW_MP,
        ur_x * zoom_factor_x - offset_x + DRAW_MP,
        ur_y * zoom_factor_y - offset_y + DRAW_MP);
  ur_x = ur_y = -1e10;
  ll_x = ll_y = 1e10;
  a_sel.get_borders( &ur_x, &ll_x, &ur_y, &ll_y);
  draw( ll_x * zoom_factor_x - offset_x - DRAW_MP,
        ll_y * zoom_factor_y - offset_y - DRAW_MP,
        ur_x * zoom_factor_x - offset_x + DRAW_MP,
        ur_y * zoom_factor_y - offset_y + DRAW_MP);
  a_sel.nav_draw( (GlowTransform *)NULL, 1, NULL, NULL);
  redraw_defered();
}

void GrowCtx::set_background( glow_eDrawType color)
{

  if ( enable_bg_pixmap && strcmp( background_image, "") != 0)
  {
    Pixmap pixmap;
    int sts;
    int width, height;

    if ( !background_tiled && !(window_width == 0 || window_height == 0)) {
      sts = grow_image_to_pixmap( this, background_image,
				  window_width, window_height, &pixmap,
				  &width, &height);
      if ( EVEN(sts))
        glow_set_background( this, color, 0, 0, 0);
      else
        glow_set_background( this, color, pixmap, width, height);
    }
    else if ( background_tiled ) {
      sts = grow_image_to_pixmap( this, background_image, 0, 0,
			    &pixmap, &width, &height);
      if ( EVEN(sts))
        glow_set_background( this, color, 0, 0, 0);
      else
        glow_set_background( this, color, pixmap, width, height);
    }
    else
      glow_set_background( this, color, 0, 0, 0);

#if 0
    // Create a pixmap
    GrowImage *i1 = new GrowImage( this, "", 0, 0);
    i1->insert_image( background_image);
    if ( i1->pixmap) {
      glow_set_background( this, color, 0, 0, 0);
      return;
    }

    if ( !background_tiled && !(window_width == 0 || window_height == 0)) {

      double ur_x = -1e10;
      double ll_x = 1e10;
      double ur_y = -1e10;
      double ll_y = 1e10;
      double width, height, scale;

      i1->get_borders( &ur_x, &ll_x, &ur_y, &ll_y);

      width = (ur_x - ll_x) * zoom_factor_x;
      height = (ur_y - ll_y) * zoom_factor_y;
      if ( width == 0 || height == 0) {
        glow_set_background( this, color, 0);
        return;
      }
      scale = min( double(window_width) / width, double(window_height)/height);
      i1->set_scale( scale, scale, 0, 0, glow_eScaleType_LowerLeft);     
    }
    glow_set_background( this, color, i1->pixmap);
    delete i1;
#endif
  }
  else
    glow_set_background( this, color, 0, 0, 0);

  background_color = color;
}

int GrowCtx::get_background_image_size( int *width, int *height)
{
  if ( strcmp( background_image, "") == 0)
    return 0;

  // Create a pixmap
  GrowImage *i1 = new GrowImage( this, "", 0, 0);
  double ur_x = -1e10;
  double ll_x = 1e10;
  double ur_y = -1e10;
  double ll_y = 1e10;

  i1->insert_image( background_image);
  if ( !i1->pixmap)
    return 0;

  i1->get_borders( &ur_x, &ll_x, &ur_y, &ll_y);
  delete i1;

  *width = (int)((ur_x - ll_x) * base_zoom_factor);
  *height = (int)((ur_y - ll_y) * base_zoom_factor);

  return 1;
}

void GrowCtx::reset_background()
{
  glow_reset_background( this);
  background_color = glow_eDrawType_LineErase;
}

glow_eDrawType GrowCtx::get_drawtype( glow_eDrawType local_drawtype,
		glow_eDrawType highlight_drawtype, int highlight, void *node, 
		int fill)
{
  glow_eDrawType drawtype;
  int	base_drawtype;
  int	incr;

  if ( highlight && !highlight_disabled)
  {
    drawtype = highlight_drawtype;
  }
  else if ( highlight && highlight_disabled && !fill && local_drawtype == 
	glow_eDrawType_Line)
  {
    drawtype = highlight_drawtype;
  }
  else
  {
    if ( node && ((GrowNode *)node)->color_tone != glow_eDrawTone_No)
    {
      if ( local_drawtype > 10)
      {
        if ( ((GrowNode *)node)->color_tone == glow_eDrawTone_DarkGray)
        {
          switch ( local_drawtype % 5)
          {
            case 0: drawtype = glow_eDrawType_Color14; break;
            case 1: drawtype = glow_eDrawType_Color19; break;
            case 2: drawtype = glow_eDrawType_Color15; break;
            case 3: drawtype = glow_eDrawType_Color20; break;
            case 4: drawtype = glow_eDrawType_Color9; break;
          }
        }
        else
          drawtype = (glow_eDrawType) (local_drawtype - local_drawtype / 10 * 10 + 
		10 * ((GrowNode *)node)->color_tone);
      }
      else
        drawtype = local_drawtype;
    }
    else if ( node && fill && ((GrowNode *)node)->fill_drawtype != glow_eDrawType_No)
    {
      drawtype = ((GrowNode *)node)->fill_drawtype;
    }
    else if ( node && !fill && ((GrowNode *)node)->draw_type != glow_eDrawType_No)
    {
      drawtype = ((GrowNode *)node)->draw_type;
    }
    else
      drawtype = local_drawtype;

    if ( node && ((GrowNode *)node)->color_lightness)
    {
      if ( local_drawtype >= 10)
      {
        base_drawtype = drawtype / 5 * 5;
        incr = -((GrowNode *)node)->color_lightness + drawtype - base_drawtype;
        if ( incr < 0)
          drawtype = glow_eDrawType_Color4; // White
        else if ( incr >= 5)
          drawtype = glow_eDrawType_Color10; // DarkGrey
        else
          drawtype = (glow_eDrawType)(base_drawtype + incr);
      }
    }
    if ( node && ((GrowNode *)node)->color_intensity)
    {
      if ( drawtype >= 20)
      {
        base_drawtype = drawtype / 10 * 10;
        incr = drawtype - base_drawtype;
        if ( 0 <= incr && incr < 5)
        {
          if ( ((GrowNode *)node)->color_intensity > 0)
            drawtype = (glow_eDrawType)( drawtype + 5);
          else
            drawtype = (glow_eDrawType)( glow_eDrawType_Color16 + incr);
        }
        else
        {
          if ( ((GrowNode *)node)->color_intensity == -1)
            drawtype = (glow_eDrawType)( drawtype - 5);
          else if ( ((GrowNode *)node)->color_intensity < 0)
            drawtype = (glow_eDrawType)( glow_eDrawType_Color16 + incr - 5);
        }
      }
    }
    if ( node && ((GrowNode *)node)->color_shift)
    {
      if ( drawtype >= 20)
      {
        incr = ((GrowNode *)node)->color_shift - 
		((GrowNode *)node)->color_shift / 8 * 8;
        if ( incr < 0)
          incr += 8;

        incr = drawtype + incr * 10;
        if ( incr >= 100)
          incr -= 80;
        drawtype = (glow_eDrawType)incr;
      }
    }
    if ( node && ((GrowNode *)node)->color_inverse)
    {
      if ( drawtype >= 10)
        drawtype = (glow_eDrawType)(drawtype + 5 - 2 * (drawtype % 5) - 1);
    }
  }
  if ( drawtype < 0 || drawtype >= 100)
  {
    printf("** Invalid drawtype : %d\n", drawtype);    
    drawtype = glow_eDrawType_Line;
  }
  return drawtype;
}

void GrowCtx::set_dynamic( char *code, int size)
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

  // Get argument list...
}

void GrowCtx::pop_select()
{
  for ( int i = 0; i < a_sel.size(); i++)
  {
    a.pop( a_sel[i]);
  }
  redraw();
}

void GrowCtx::push_select()
{
  for ( int i = 0; i < a_sel.size(); i++)
  {
    a.push( a_sel[i]);
  }
  redraw();
}

int GrowCtx::get_default_window_size( int *width, int *height)
{
  if ( x0 >= x1 || y0 >= y1)
    return 0;

  *width = int( (x1 - x0) * zoom_factor_x);
  *height = int( (y1 - y0) * zoom_factor_y);
  return 1;
}

void GrowCtx::set_default_layout()
{
  if ( x0 >= x1 || y0 >= y1)
    return;

  glow_draw_get_window_size( this, &window_width, &window_height);
  zoom_factor_x = min( window_width / (x1 - x0),
	window_height / (y1 - y0));
  zoom_factor_y = zoom_factor_x;
  offset_x = int( x0 * zoom_factor_x);
  offset_y = int( y0 * zoom_factor_y);
  change_scrollbar();
  a.zoom();
  if ( enable_bg_pixmap && strcmp( background_image, "") != 0 && 
       !background_tiled)
    set_background( background_color);
  redraw();
}

int GrowCtx::any_select_is_con()
{
  for ( int i = 0; i < a_sel.size(); i++)
  {
    if ( a_sel[i]->type() == glow_eObjectType_Con)
      return 1;
  }
  return 0;
}

void GrowCtx::change_select_conclass( GlowArrayElem *conclass)
{
  for ( int i = 0; i < a_sel.size(); i++)
  {
    if ( a_sel[i]->type() == glow_eObjectType_Con)
      ((GlowCon *)a_sel[i])->change_conclass((GlowConClass *)conclass); 
  }
}


void grow_auto_scrolling( GrowCtx *ctx)
{
  int delta_x, delta_y;

  if ( ctx->edit_mode == grow_eMode_Scale && !ctx->scale_active)
    return;

  ctx->auto_scrolling_active = 1;

  if ( ctx->node_movement_active && ctx->edit_mode == grow_eMode_EditPolyLine)
  {
    delta_x = - (ctx->node_move_last_x - ctx->window_width / 2) / 6;
    delta_y = - (ctx->node_move_last_y - ctx->window_height / 2) / 6;

    ctx->set_defered_redraw();
    ((GrowPolyLine *)ctx->a_move[0])->move_current_point(
		-delta_x, -delta_y, 0);
    ctx->redraw_defered();
  }
  else if ( ctx->node_movement_active || ctx->node_movement_paste_active)
  {
    if (ctx->move_restriction == glow_eMoveRestriction_VerticalSlider ||
        ctx->move_restriction == glow_eMoveRestriction_HorizontalSlider)
    {
      ctx->auto_scrolling_active = 0;
      return;
    }
    delta_x = - (ctx->node_move_last_x - ctx->window_width / 2) / 6;
    delta_y = - (ctx->node_move_last_y - ctx->window_height / 2) / 6;

    ctx->set_defered_redraw();
    switch( ctx->move_restriction)
    {
      case glow_eMoveRestriction_Vertical:
        ctx->a_move.move( 0, -delta_y, 0);
        break;
      case glow_eMoveRestriction_Horizontal:
        ctx->a_move.move( -delta_x, 0, 0);
        break;
      default:
        ctx->a_move.move( -delta_x, -delta_y, 0);
    }
    ctx->redraw_defered();
  }
  else if ( ctx->select_rect_active)
  {
    delta_x = - (ctx->select_rect_last_x - ctx->window_width / 2) / 12;
    delta_y = - (ctx->select_rect_last_y - ctx->window_height / 2) / 12;

    ctx->select_rect_stored_ll_x += delta_x;
    ctx->select_rect_stored_ll_y += delta_y;
    ctx->select_rect_stored_ur_x += delta_x;
    ctx->select_rect_stored_ur_y += delta_y;
  }
  else if ( ctx->con_create_active)
  {
    delta_x = - (ctx->con_create_last_x - ctx->window_width / 2) / 6;
    delta_y = - (ctx->con_create_last_y - ctx->window_height / 2) / 6;

    ctx->polyline_start_x += delta_x;
    ctx->polyline_start_y += delta_y;
    ctx->polyline_last_end_x += delta_x;
    ctx->polyline_last_end_y += delta_y;
  }
 
  ctx->traverse( delta_x,delta_y);
  ctx->nav_draw( 0, 0, ctx->nav_window_width, ctx->nav_window_height);
  ctx->change_scrollbar();
  draw_set_timer( ctx, 300, auto_scrolling, &ctx->auto_scrolling_id);
}

void GrowCtx::set_select_textsize( int size)
{
  for ( int i = 0; i < a_sel.size(); i++) {
    if ( a_sel[i]->type() == glow_eObjectType_GrowText)
      ((GrowText *)a_sel[i])->set_textsize( size);
    else if ( a_sel[i]->type() == glow_eObjectType_GrowAxis)
      ((GrowAxis *)a_sel[i])->set_textsize( size);
  }
}

void GrowCtx::set_select_textbold( int bold)
{
  for ( int i = 0; i < a_sel.size(); i++) {
    if ( a_sel[i]->type() == glow_eObjectType_GrowText)
      ((GrowText *)a_sel[i])->set_textbold( bold);
    else if ( a_sel[i]->type() == glow_eObjectType_GrowAxis)
      ((GrowAxis *)a_sel[i])->set_textbold( bold);
  }
}

void GrowCtx::set_move_restrictions( glow_eMoveRestriction restriction,
	double max_limit, double min_limit, GlowArrayElem *object)
{
  move_restriction = restriction;
  restriction_max_limit = max_limit;
  restriction_min_limit = min_limit;
  restriction_object = object;
}

void GrowCtx::align_select( glow_eAlignDirection direction)
{
  int i;
  double object_x, object_y;
  double ll_x, ll_y, ur_x, ur_y;

  if ( a_sel.size() < 2)
    return;

  switch ( direction)
  {
    case glow_eAlignDirection_Left:
      // Find the object with the lowest y-koord
      object_y = 1e37;
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->measure( &ll_x, &ll_y, &ur_x, &ur_y);
        if ( ll_y < object_y)
        {
          object_y = ll_y;
          object_x = ll_x;
        }
      }
      set_defered_redraw();
      a_sel.align( object_x, 0, direction);
      a_sel.zoom();
      redraw_defered();
      break;
    case glow_eAlignDirection_Right:
      // Find the object with the lowest y-koord
      object_y = 1e37;
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->measure( &ll_x, &ll_y, &ur_x, &ur_y);
        if ( ll_y < object_y)
        {
          object_y = ll_y;
          object_x = ur_x;
        }
      }
      set_defered_redraw();
      a_sel.align( object_x, 0, direction);
      a_sel.zoom();
      redraw_defered();
      break;
    case glow_eAlignDirection_Up:
      // Find the object with the lowest x-koord
      object_x = 1e37;
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->measure( &ll_x, &ll_y, &ur_x, &ur_y);
        if ( ll_x < object_x)
        {
          object_y = ur_y;
          object_x = ll_x;
        }
      }
      set_defered_redraw();
      a_sel.align( 0, object_y, direction);
      a_sel.zoom();
      redraw_defered();
      break;
    case glow_eAlignDirection_Down:
      // Find the object with the lowest x-koord
      object_x = 1e37;
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->measure( &ll_x, &ll_y, &ur_x, &ur_y);
        if ( ll_x < object_x)
        {
          object_y = ll_y;
          object_x = ll_x;
        }
      }
      set_defered_redraw();
      a_sel.align( 0, object_y, direction);
      a_sel.zoom();
      redraw_defered();
      break;
    case glow_eAlignDirection_CenterHoriz:
      // Find the object with the lowest x-koord
      object_x = 1e37;
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->measure( &ll_x, &ll_y, &ur_x, &ur_y);
        if ( ll_x < object_x)
        {
          object_y = (ll_y + ur_y) / 2;
          object_x = ll_x;
        }
      }
      set_defered_redraw();
      a_sel.align( 0, object_y, direction);
      a_sel.zoom();
      redraw_defered();
      break;
    case glow_eAlignDirection_CenterVert:
      // Find the object with the lowest x-koord
      object_y = 1e37;
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->measure( &ll_x, &ll_y, &ur_x, &ur_y);
        if ( ll_y < object_y)
        {
          object_y = ll_y;
          object_x = (ll_x + ur_x) / 2;
        }
      }
      set_defered_redraw();
      a_sel.align( object_x, 0, direction);
      a_sel.zoom();
      redraw_defered();
      break;
    case glow_eAlignDirection_CenterCenter:
      // Align with the last object
      a_sel[a_sel.size() - 1]->measure( &ll_x, &ll_y, &ur_x, &ur_y);
      object_y = (ll_y + ur_y) / 2;
      object_x = (ll_x + ur_y) / 2;
      set_defered_redraw();
      a_sel.align( 0, object_y, direction);
      redraw_defered();
      break;
  }
}

void GrowCtx::equidistance_select( glow_eAlignDirection direction)
{
  int i ,j;
  double dx, dy;
  double ll_x1, ll_y1, ur_x1, ur_y1;
  double ll_x2, ll_y2, ur_x2, ur_y2;
  GlowArrayElem *tmp;

  if ( a_sel.size() < 3)
    return;

  switch ( direction)
  {
    case glow_eAlignDirection_Down:
      // Order in y-koord
      for ( i = a_sel.size() - 1; i > 0; i--)
      {
        for ( j = 0; j < i; j++)
        {
          a_sel[j + 1]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
          a_sel[j]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
	  if ( ll_y1 < ll_y2)
	  {
            tmp = a_sel.a[j + 1];
            a_sel.a[j + 1] = a_sel.a[j];
            a_sel.a[j] = tmp;
	  }
	}
      }
      a_sel[0]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
      a_sel[a_sel.size() - 1]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
      dy = (ll_y2 - ll_y1) / (a_sel.size() - 1);
      set_defered_redraw();
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->align( 0, ll_y1 + i * dy, direction);
      }
      redraw_defered();
      break;
    case glow_eAlignDirection_Up:
      // Order in y-koord
      for ( i = a_sel.size() - 1; i > 0; i--)
      {
        for ( j = 0; j < i; j++)
        {
          a_sel[j + 1]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
          a_sel[j]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
	  if ( ur_y1 < ur_y2)
	  {
            tmp = a_sel.a[j + 1];
            a_sel.a[j + 1] = a_sel.a[j];
            a_sel.a[j] = tmp;
	  }
	}
      }
      a_sel[0]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
      a_sel[a_sel.size() - 1]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
      dy = (ur_y2 - ur_y1) / (a_sel.size() - 1);
      set_defered_redraw();
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->align( 0, ur_y1 + i * dy, direction);
      }
      redraw_defered();
      break;
    case glow_eAlignDirection_CenterVert:
      // Order in y-koord
      for ( i = a_sel.size() - 1; i > 0; i--)
      {
        for ( j = 0; j < i; j++)
        {
          a_sel[j + 1]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
          a_sel[j]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
	  if ( (ur_y1 + ll_y1) / 2 < (ur_y2 + ll_y2) / 2)
	  {
            tmp = a_sel.a[j + 1];
            a_sel.a[j + 1] = a_sel.a[j];
            a_sel.a[j] = tmp;
	  }
	}
      }
      a_sel[0]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
      a_sel[a_sel.size() - 1]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
      dy = ((ur_y2 + ll_y2) / 2 - (ur_y1 + ll_y1) / 2) / (a_sel.size() - 1);
      set_defered_redraw();
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->align( 0, (ur_y1 + ll_y1) / 2 + i * dy, 
		glow_eAlignDirection_CenterHoriz);
      }
      redraw_defered();
      break;
    case glow_eAlignDirection_Left:
      // Order in x-koord
      for ( i = a_sel.size() - 1; i > 0; i--)
      {
        for ( j = 0; j < i; j++)
        {
          a_sel[j + 1]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
          a_sel[j]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
	  if ( ll_x1 < ll_x2)
	  {
            tmp = a_sel.a[j + 1];
            a_sel.a[j + 1] = a_sel.a[j];
            a_sel.a[j] = tmp;
	  }
	}
      }
      a_sel[0]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
      a_sel[a_sel.size() - 1]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
      dx = (ll_x2 - ll_x1) / (a_sel.size() - 1);
      set_defered_redraw();
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->align( ll_x1 + i * dx, 0, direction);
      }
      redraw_defered();
      break;
    case glow_eAlignDirection_Right:
      // Order in x-koord
      for ( i = a_sel.size() - 1; i > 0; i--)
      {
        for ( j = 0; j < i; j++)
        {
          a_sel[j + 1]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
          a_sel[j]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
	  if ( ur_x1 < ur_x2)
	  {
            tmp = a_sel.a[j + 1];
            a_sel.a[j + 1] = a_sel.a[j];
            a_sel.a[j] = tmp;
	  }
	}
      }
      a_sel[0]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
      a_sel[a_sel.size() - 1]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
      dx = (ur_x2 - ur_x1) / (a_sel.size() - 1);
      set_defered_redraw();
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->align( ur_x1 + i * dx, 0, direction);
      }
      redraw_defered();
      break;
    case glow_eAlignDirection_CenterHoriz:
      // Order in x-koord
      for ( i = a_sel.size() - 1; i > 0; i--)
      {
        for ( j = 0; j < i; j++)
        {
          a_sel[j + 1]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
          a_sel[j]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
	  if ( (ur_x1 + ll_x1) / 2 < (ur_x2 + ll_x2) / 2)
	  {
            tmp = a_sel.a[j + 1];
            a_sel.a[j + 1] = a_sel.a[j];
            a_sel.a[j] = tmp;
	  }
	}
      }
      a_sel[0]->measure( &ll_x1, &ll_y1, &ur_x1, &ur_y1);
      a_sel[a_sel.size() - 1]->measure( &ll_x2, &ll_y2, &ur_x2, &ur_y2);
      dx = ((ur_x2 + ll_x2) / 2 - (ur_x1 + ll_x1) / 2) / (a_sel.size() - 1);
      set_defered_redraw();
      for ( i = 0; i < a_sel.size(); i++)
      {
        a_sel[i]->align( (ur_x1 + ll_x1) / 2 + i * dx, 0, 
		glow_eAlignDirection_CenterVert);
      }
      redraw_defered();
      break;
    default : 
      ;
  }
}

void GrowCtx::measure_javabean( double *pix_x_right, double *pix_x_left, 
	double *pix_y_high, double *pix_y_low)
{ 
  if ( java_width == 0 || ( x0 == 0 && x1 == 0)) {
    double jb_x_right = -1e10;
    double jb_x_left = 1e10;
    double jb_y_high = -1e10;
    double jb_y_low = 1e10;

    for ( int i = 0; i < a.size(); i++)
    {
      switch ( a[i]->type())
      {
        case glow_eObjectType_GrowConPoint:
          break;
        default:
          a[i]->get_borders( &jb_x_right, &jb_x_left, &jb_y_high, &jb_y_low);
      }
    }
    *pix_x_right = jb_x_right * zoom_factor_x - double(offset_x);
    *pix_x_left = jb_x_left * zoom_factor_x - double(offset_x);
    *pix_y_high = jb_y_high * zoom_factor_y - double(offset_y);
    *pix_y_low = jb_y_low * zoom_factor_y - double(offset_y);
  }
  else {
    *pix_x_left = x0 * zoom_factor_x - double(offset_x);
    *pix_x_right = x1 * zoom_factor_x - double(offset_x);
    *pix_y_low = y0 * zoom_factor_y - double(offset_y);
    *pix_y_high = y1 * zoom_factor_y - double(offset_y);
  }


}

void GrowCtx::to_pixel( double x, double y, double *pix_x, double *pix_y)
{
    *pix_x = x * zoom_factor_x - double(offset_x);
    *pix_y = y * zoom_factor_y - double(offset_y);
}

void GrowCtx::set_javaframe( double *pix_x_right, double *pix_x_left, 
	double *pix_y_high, double *pix_y_low)
{ 
  if ( java_width == 0 || ( x0 == 0 && x1 == 0))
    measure_javabean( pix_x_right, pix_x_left, pix_y_high, pix_y_low);
  else
  {
    // Zoom to desired zoom factor
    zoom_absolute( double( java_width) / (x1 - x0));

    measure_javabean( pix_x_right, pix_x_left, pix_y_high, pix_y_low);
  }
}

void GrowCtx::export_javabean( ofstream& fp, int components)
{
  int i;
  int shape_cnt;

  export_jbean = new GlowExportJBean( this);

  if ( components == 1)
  {
    // Declare components and set compoinent attirbutes
    shape_cnt = 0;
    for ( i = 0; i < a.size(); i++)
    {
      a[i]->export_javabean( (GlowTransform *) NULL, NULL, glow_eExportPass_Declare, &shape_cnt, 
	i, 0, fp);
    }
    export_jbean->growctx( glow_eExportPass_Declare, fp);
  }
  else if ( components == 2)
  {
    // Attribute pass
    shape_cnt = 0;
    for ( i = 0; i < a.size(); i++)
    {
      a[i]->export_javabean( (GlowTransform *) NULL, NULL, glow_eExportPass_Attributes, &shape_cnt,
	i, 0, fp);
    }
    export_jbean->growctx( glow_eExportPass_Attributes, fp);
  }
  else
  {
    // Paint 

    // Init pass
    shape_cnt = 0;
    for ( i = 0; i < a.size(); i++)
    {
      a[i]->export_javabean( (GlowTransform *) NULL, NULL, glow_eExportPass_Init, &shape_cnt,
		i, 0, fp);
    }
    export_jbean->growctx( glow_eExportPass_Init, fp);

    // Shape pass
    shape_cnt = 0;
    for ( i = 0; i < a.size(); i++)
    {
      a[i]->export_javabean( (GlowTransform *) NULL, NULL, glow_eExportPass_Shape, &shape_cnt, 
		i, 0, fp);
    }
    export_jbean->growctx( glow_eExportPass_Shape, fp);

    // Draw pass
    export_jbean->growctx( glow_eExportPass_Draw, fp);
    shape_cnt = 0;
    for ( i = 0; i < a.size(); i++)
    {
      a[i]->export_javabean( (GlowTransform *) NULL, NULL, glow_eExportPass_Draw, &shape_cnt, 
		i, 0, fp);
    }

    // End pass
    export_jbean->growctx( glow_eExportPass_End, fp);
  }

  delete export_jbean;
}

void GrowCtx::export_nodeclass_javabean( GlowArrayElem *nc, ofstream& fp, int components)
{
  int i;
  int shape_cnt;
  int page;
  int pages;
  GlowNodeClass *next_nc;

  // See if there are any next_nodeclasses
  page = 1;
  for ( next_nc = (GlowNodeClass*)((GlowNodeClass *)nc)->next_nc; 
        next_nc; 
        next_nc = (GlowNodeClass*)next_nc->next_nc)
    page++;
  pages = page;

  export_jbean = new GlowExportJBean( this, (GlowNodeClass *) nc);

  if ( components == 1)
  {
    // Declare components and set component attributes
    shape_cnt = 0;
    for ( i = 0; i < ((GlowNodeClass *)nc)->a.size(); i++)
    {
      ((GlowNodeClass *)nc)->a[i]->export_javabean( (GlowTransform *) NULL, NULL, glow_eExportPass_Declare, &shape_cnt,
	i, 1, fp);
    }

    if ( pages > 1)
    {
      page = 1;
      for ( next_nc = (GlowNodeClass*)((GlowNodeClass *)nc)->next_nc; 
  	    next_nc; 
	    next_nc = (GlowNodeClass*)next_nc->next_nc)
      {
        page++;
        export_jbean->nc = next_nc;
        export_jbean->page = page;

        for ( i = 0; i < ((GlowNodeClass *)next_nc)->a.size(); i++)
        {
          next_nc->a[i]->export_javabean( (GlowTransform *) NULL, NULL, 
		glow_eExportPass_Declare, &shape_cnt, i, 1, fp);
        }
      }
    }
    export_jbean->nc = (GlowNodeClass *)nc;
    export_jbean->page = 1;
  }
  else if ( components == 2)
  {
    // Attribute pass
    shape_cnt = 0;
    for ( i = 0; i < ((GlowNodeClass *)nc)->a.size(); i++)
    {
      ((GlowNodeClass *)nc)->a[i]->export_javabean( (GlowTransform *) NULL, NULL, glow_eExportPass_Attributes, &shape_cnt,
	i, 1, fp);
    }

    if ( pages > 1)
    {
      page = 1;
      for ( next_nc = (GlowNodeClass*)((GlowNodeClass *)nc)->next_nc; 
  	    next_nc; 
	    next_nc = (GlowNodeClass*)next_nc->next_nc)
      {
        page++;
        export_jbean->nc = next_nc;
        export_jbean->page = page;

        for ( i = 0; i < ((GlowNodeClass *)next_nc)->a.size(); i++)
        {
          next_nc->a[i]->export_javabean( (GlowTransform *) NULL, NULL, 
		glow_eExportPass_Attributes, &shape_cnt, i, 1, fp);
        }
      }
    }
    export_jbean->nc = (GlowNodeClass *)nc;
    export_jbean->page = 1;

    export_jbean->nodeclass( (GlowNodeClass*) nc, glow_eExportPass_Attributes,
	 fp, 1, 1);
  }
  else
  {

    // Paint 

    // Init pass
    shape_cnt = 0;
    for ( i = 0; i < ((GlowNodeClass *)nc)->a.size(); i++)
    {
      ((GlowNodeClass *)nc)->a[i]->export_javabean( (GlowTransform *) NULL, 
		NULL, glow_eExportPass_Init, &shape_cnt, i, 1, fp);
    }
    export_jbean->nodeclass( (GlowNodeClass*) nc, glow_eExportPass_Init, fp, 
		1, page);

    // Shape pass
    shape_cnt = 0;
    for ( i = 0; i < ((GlowNodeClass *)nc)->a.size(); i++)
    {
      ((GlowNodeClass *)nc)->a[i]->export_javabean( (GlowTransform *) NULL, 
		NULL, glow_eExportPass_Shape, &shape_cnt, i, 1, fp);
    }
    export_jbean->nodeclass( (GlowNodeClass*) nc, glow_eExportPass_Shape, fp,
	1, pages);

    // Shapes for next_nodeclasses
    if ( pages > 1)
    {
      page = 1;
      for ( next_nc = (GlowNodeClass*)((GlowNodeClass *)nc)->next_nc; 
  	    next_nc; 
	    next_nc = (GlowNodeClass*)next_nc->next_nc)
      {
        page++;
        export_jbean->nc = next_nc;
        export_jbean->page = page;
        shape_cnt = 0;
        for ( i = 0; i < ((GlowNodeClass *)next_nc)->a.size(); i++)
        {
          next_nc->a[i]->export_javabean( (GlowTransform *) NULL, NULL, 
		glow_eExportPass_Init, &shape_cnt, i, 1, fp);
        }

        export_jbean->nodeclass( next_nc, glow_eExportPass_Init, fp,
		page, pages);
        shape_cnt = 0;
        for ( i = 0; i < ((GlowNodeClass *)next_nc)->a.size(); i++)
        {
          next_nc->a[i]->export_javabean( (GlowTransform *) NULL, NULL, 
		glow_eExportPass_Shape, &shape_cnt, i, 1, fp);
        }
        export_jbean->nodeclass( next_nc, glow_eExportPass_Shape, fp,
		page, pages);
      }
    }
    export_jbean->nc = (GlowNodeClass *)nc;
    export_jbean->page = 1;

    // Draw pass
    export_jbean->nodeclass( (GlowNodeClass *) nc, glow_eExportPass_Draw, fp,
	1, pages);
    shape_cnt = 0;
    for ( i = 0; i < ((GlowNodeClass *)nc)->a.size(); i++)
    {
      ((GlowNodeClass *)nc)->a[i]->export_javabean( (GlowTransform *) NULL, NULL, glow_eExportPass_Draw, &shape_cnt, 
		i, 1, fp);
    }

    // End pass
    export_jbean->nodeclass( (GlowNodeClass*) nc, glow_eExportPass_End, fp,
	1, pages);

    // Draw functions for next_nodeclasses
    if ( pages > 1)
    {
      page = 1;
      for ( next_nc = (GlowNodeClass*)((GlowNodeClass *)nc)->next_nc; 
	    next_nc; 
	    next_nc = (GlowNodeClass*)next_nc->next_nc)
      {
        page++;
        export_jbean->nc = next_nc;
        export_jbean->page = page;

        // Draw pass
        export_jbean->nodeclass( next_nc, glow_eExportPass_Draw, fp,
		page, pages);

        shape_cnt = 0;
        for ( i = 0; i < ((GlowNodeClass *)next_nc)->a.size(); i++)
        {
          next_nc->a[i]->export_javabean( (GlowTransform *) NULL, NULL, 
		glow_eExportPass_Draw, &shape_cnt, i, 1, fp);
        }

        // End pass
        export_jbean->nodeclass( next_nc, glow_eExportPass_End, fp,
		page, pages);
      }
    }
    export_jbean->nc = (GlowNodeClass *)nc;
    export_jbean->page = 1;

  }

  delete export_jbean;
}

void GrowCtx::export_nc_javabean_font( GlowArrayElem *nc, ofstream& fp, int components)
{
  int i;

  export_jbean = new GlowExportJBean( this, (GlowNodeClass *) nc);

  if ( components == 1)
  {
    // Declare components and set component attributes
  }
  else if ( components == 2)
  {
    // Attribute pass
  }
  else
  {
    // Paint 

    // Init pass
    for ( i = 0; i < ((GlowNodeClass *)nc)->a.size(); i++)
    {
      if ( ((GlowNodeClass *)nc)->a[i]->type() == glow_eObjectType_GrowAnnot)
      {
        ((GrowAnnot *)((GlowNodeClass *)nc)->a[i])->export_javabean_font( 
		(GlowTransform *) NULL, NULL, glow_eExportPass_Init, fp);
        break;
      }
    }
  }

  delete export_jbean;
}

int GrowCtx::get_java_name( char *jname)
{ 
  if ( strcmp( java_name, "") != 0)
  {
    strcpy( jname, java_name);
    return 1;
  }
  strcpy( jname, name);
  return 0;
}

void GrowCtx::get_annotation_numbers( int **numbers, int *cnt)
{
  int		i;
  int		*p;

  *cnt = 0;  
  p = (int *) calloc( 10, sizeof(int));
  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == glow_eObjectType_GrowSubAnnot)
    {
      if ( *cnt >= 10)
        break;
      p[*cnt] = ((GrowSubAnnot *)a.a[i])->number;
      (*cnt)++;
    }
  }
  *numbers = p;
}

int GrowCtx::send_hot_request( GlowArrayElem *object)
{

  if ( event_callback[glow_eEvent_HotRequest] )
  {
    /* Send a host request callback */
    static glow_sEvent e;

    e.event = glow_eEvent_HotRequest;
    e.any.type = glow_eEventType_Object;
    e.any.x_pixel = 0;
    e.any.y_pixel = 0;
    e.any.x = 0;
    e.any.y = 0;
    e.object.object_type = object->type();
    e.object.object = object;
    return event_callback[glow_eEvent_HotRequest]( this, &e);
  }
  else
    return 1;
}

void GrowCtx::store_geometry()
{
  stored_offset_x = offset_x;
  stored_offset_y = offset_y;
  stored_zoom_factor_x = zoom_factor_x;
  stored_zoom_factor_y = zoom_factor_y;
  stored_grid_size_x = grid_size_x;
  stored_grid_size_y = grid_size_y;
  stored_grid_on = grid_on;
  stored_show_grid = show_grid;
}

void GrowCtx::restore_geometry()
{
  if ( stored_zoom_factor_x == 0)
    return;
  offset_x = stored_offset_x;
  offset_y = stored_offset_y;
  zoom_factor_x = stored_zoom_factor_x;
  zoom_factor_y = stored_zoom_factor_y;
  a.zoom();
  grid_size_x = stored_grid_size_x;
  grid_size_y = stored_grid_size_y;
  grid_on = stored_grid_on;
  show_grid = stored_show_grid;
  clear();
  draw( 0, 0, window_width, window_height);
  nav_zoom();
}

void GrowCtx::get_text_extent( char *text, int len, glow_eDrawType draw_type,
	int text_size, double *width, double *height, double *descent)
{
  int z_width, z_height, z_descent;

  draw_get_text_extent( this, text, len, draw_type, text_size,
	&z_width, &z_height, &z_descent);

  *width = double( z_width) / zoom_factor_y;
  *height = double( z_height) / zoom_factor_y;
  *descent = double( z_descent) / zoom_factor_y;
}

int GrowCtx::group_select( GlowArrayElem **group, char *last_group)
{
  char name[20];
  int i, j;
  GlowArray a_group(20,20);

  if ( a_sel.size() == 0)
    return GLOW__NOSELECT;

  // Connections in a group are not allowed
  for ( i = 0; i < a_sel.size(); i++) {
    if ( a_sel[i]->type() == glow_eObjectType_Con)
      return GLOW__CONSELECTED;
  }

  strcpy( last_group, a_sel.get_last_group());

  a_sel.set_highlight(0);
  sprintf( name, "Grp%d_", objectname_cnt++); 

  // The objects i the group has to be in the same order as a
  for ( i = 0; i < a.size(); i++) {
    for ( j = 0; j < a_sel.size(); j++) {
      if ( a[i] == a_sel[j]) {
	a_group.insert( a[i]);
        break;
      }
    }
    if ( a_sel.size() == a_group.size())
      break;
  }
  GrowGroup *g1 = new GrowGroup( this, name, a_group);
  a_group.clear();

  // Remove objects
  for ( i = 0; i < a_sel.size(); i++)
    a.remove( a_sel.a[i]);
  a_sel.clear();

  a.insert( g1);
  *group = g1;
  return 1;
}

int GrowCtx::ungroup_select()
{
  int i;
  GrowGroup *group;

  a_sel.set_highlight(0);
  for ( i = 0; i < a_sel.size(); i++) {
    if ( a_sel[i]->type() == glow_eObjectType_GrowGroup) {
      group = (GrowGroup *)a_sel[i];
      group->ungroup();
      a.remove( group);
      a_sel.remove( group);
      a_paste.remove( group);
      delete group;
      i--;
    }
  }
  return 1;
}

void GrowCtx::get_nodegrouplist( GlowArrayElem ***list, int *size)
{
  GlowArray *a_ng = new GlowArray(20, 20);

  a.get_nodegroups( a_ng);
  if ( a_ng->size() == 0) {
    *size = 0;
    return;
  }
  *list = (GlowArrayElem **) malloc( a_ng->size() * sizeof(a_ng->a[0]));
  memcpy( *list, a_ng->a, a_ng->size() * sizeof(a_ng->a[0]));
  *size = a_ng->size();
  a_ng->clear();
  delete a_ng;
}

int GrowCtx::get_object_group( GlowArrayElem *object, GlowArrayElem **group)
{
  int sts;

  for ( int i = 0; i < a.size(); i++) {
    if ( a[i]->type() == glow_eObjectType_GrowGroup) {
      sts = ((GrowGroup *)a[i])->get_object_group( object, group);
      if (ODD(sts))
        return sts;
    }
  }
  return 0;
}

int GrowCtx::get_background_object_limits( glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction)
{
  return a.get_background_object_limits( NULL, type, x, y, background, 
					 min, max, direction);
}

int GrowCtx::translate_cb( GlowArrayElem *object, char *text, char **new_text)
{
  
  if ( event_callback[glow_eEvent_Translate] )
  {
    /* Send an translate callback */
    static glow_sEvent e;

    e.event = glow_eEvent_Translate;
    e.any.type = glow_eEventType_Translate;
    e.any.x_pixel = 0;
    e.any.y_pixel = 0;
    e.any.x = 0;
    e.any.y = 0;
    e.translate.object_type = object->type();
    e.translate.object = object;
    e.translate.text = text;
    if ( event_callback[glow_eEvent_Translate]( this, &e)) {
      *new_text = e.translate.new_text;
      return 1;
    }
  }
  return 0;
}






