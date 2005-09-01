/* 
 * Proview   $Id: glow_grownode.cpp,v 1.10 2005-09-01 14:57:54 claes Exp $
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
#include "glow_grownode.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_conpoint.h"
#include "glow_growgroup.h"

GrowNode::GrowNode( GlowCtx *glow_ctx, char *name, GlowNodeClass *node_class,
	double x1, double y1, int nodraw, int rel_annot_pos) :
	GlowNode(glow_ctx,name,node_class,x1,y1,nodraw,rel_annot_pos),
	dynamic(0), dynamicsize(0),
	original_border_drawtype(glow_eDrawType_No), original_text_drawtype(glow_eDrawType_No),
	original_fill_drawtype(glow_eDrawType_No), fill_drawtype(glow_eDrawType_No),
	text_drawtype(glow_eDrawType_No),
	draw_type(glow_eDrawType_No), level_fill_drawtype(glow_eDrawType_No), 
	original_color_tone( glow_eDrawTone_No),
	color_tone( glow_eDrawTone_No), level_color_tone( glow_eDrawTone_No), 
	original_color_lightness(0),
	color_lightness(0), original_color_intensity(0), color_intensity(0),
	original_color_shift(0), color_shift(0), color_inverse(0), line_width(0),
	invisible(0), dimmed(0), object_type(glow_eObjectType_GrowNode), root_node(0),
	flip_horizontal(false), flip_vertical(false), fill_level(1),
	level_direction( glow_eDirection_Right), shadow(0), input_position(0), input_selected(0)
{  
  memset( argv, 0, sizeof(argv));
  memset( argsize, 0, sizeof(argsize));
  strcpy( last_group, "");
  trf.move( pos.x, pos.y);
  if ( !nc)
    return;
  get_node_borders();
  if ( !nodraw) 
    draw(  0, 0, ctx->window_width, ctx->window_height);

//  nc = new GlowNodeClass( *nc);
}

GrowNode::~GrowNode()
{
  if ( !nc)
    return;

  erase();
  nav_erase();

  ctx->set_defered_redraw();
  ctx->delete_node_cons( this);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  ctx->redraw_defered();
  if ( hot)
    draw_set_cursor( ctx, glow_eDrawCursor_Normal);
//  delete nc;
}

void GrowNode::copy_from( const GrowNode& n) 
{
  memcpy( this, &n, sizeof(n));
  for ( int i = 0; i < 10; i++)
  {
    if ( annotsize[i])
    {
      annotv[i] = (char *) calloc( 1, annotsize[i]);
      memcpy( annotv[i], n.annotv[i], annotsize[i]);
    }
  }
  if ( n.dynamicsize)
  {
    dynamic = (char *) calloc( 1, n.dynamicsize);
    memcpy( dynamic, n.nc->dynamic, n.dynamicsize);
  }

  if ( ctx->userdata_copy_callback)
    (ctx->userdata_copy_callback)( this, user_data, &user_data, glow_eUserdataCbType_Node);
//  nc = new GlowNodeClass( *nc);
}

void GrowNode::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  char *s;
  int i;

  fp << int(glow_eSave_GrowNode) << endl;
  fp << int(glow_eSave_GrowNode_original_border_drawtype) << FSPACE 
		<< int(original_border_drawtype) << endl;
  fp << int(glow_eSave_GrowNode_original_fill_drawtype) << FSPACE 
		<< int(original_fill_drawtype) << endl;
  fp << int(glow_eSave_GrowNode_original_text_drawtype) << FSPACE 
		<< int(original_text_drawtype) << endl;
  fp << int(glow_eSave_GrowNode_fill_drawtype) << FSPACE << int(fill_drawtype) << endl;
  fp << int(glow_eSave_GrowNode_text_drawtype) << FSPACE << int(text_drawtype) << endl;
  fp << int(glow_eSave_GrowNode_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(glow_eSave_GrowNode_original_color_tone) << FSPACE << int(original_color_tone) << endl;
  fp << int(glow_eSave_GrowNode_color_tone) << FSPACE << int(color_tone) << endl;
  fp << int(glow_eSave_GrowNode_original_color_lightness) << FSPACE << original_color_lightness << endl;
  fp << int(glow_eSave_GrowNode_color_lightness) << FSPACE << color_lightness << endl;
  fp << int(glow_eSave_GrowNode_original_color_intensity) << FSPACE << original_color_intensity << endl;
  fp << int(glow_eSave_GrowNode_color_intensity) << FSPACE << color_intensity << endl;
  fp << int(glow_eSave_GrowNode_original_color_shift) << FSPACE << original_color_shift << endl;
  fp << int(glow_eSave_GrowNode_color_shift) << FSPACE << color_shift << endl;
  fp << int(glow_eSave_GrowNode_invisible) << FSPACE << invisible << endl;
  fp << int(glow_eSave_GrowNode_dynamicsize) << FSPACE << dynamicsize << endl;
  fp << int(glow_eSave_GrowNode_dynamic) << endl;
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
  fp << int(glow_eSave_GrowNode_node_part) << endl;
  GlowNode::save( fp, mode);
  fp << int(glow_eSave_GrowNode_trf) << endl;
  trf.save( fp, mode);
  fp << int(glow_eSave_GrowNode_arg_cnt) << FSPACE << nc->arg_cnt << endl;
  fp << int(glow_eSave_GrowNode_argsize) << endl;
  for ( i = 0; i < nc->arg_cnt; i++)
    fp << argsize[i] << endl;
  fp << int(glow_eSave_GrowNode_argv) << endl;
  for ( i = 0; i < nc->arg_cnt; i++)
  {
    if( argsize[i])
    {
      fp << "\"";
      for ( s  = argv[i]; *s; s++)
      {
        if ( *s == '"')
	  fp << "\\";
        fp << *s;
      }
      fp << "\"" << endl;
    }
  }
  fp << int(glow_eSave_GrowNode_line_width) << FSPACE << line_width << endl;
  fp << int(glow_eSave_GrowNode_shadow) << FSPACE << shadow << endl;

  if ( user_data && ctx->userdata_save_callback) {
    fp << int(glow_eSave_GrowNode_userdata_cb) << endl;
    (ctx->userdata_save_callback)(&fp, this, glow_eUserdataCbType_Node);
  }

  fp << int(glow_eSave_End) << endl;
}

void GrowNode::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;
  int		i, j;
  char		c;
  int		arg_cnt;
  char          *new_text;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowNode: break;
      case glow_eSave_GrowNode_original_border_drawtype: fp >> 
		tmp; original_border_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowNode_original_fill_drawtype: fp >> 
		tmp; original_fill_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowNode_fill_drawtype: fp >> 
		tmp; fill_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowNode_original_text_drawtype: fp >> 
		tmp; original_text_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowNode_text_drawtype: fp >> 
		tmp; text_drawtype = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowNode_draw_type: fp >> tmp; 
		draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_GrowNode_original_color_tone: fp >> tmp; 
		original_color_tone = (glow_eDrawTone)tmp; break;
      case glow_eSave_GrowNode_color_tone: fp >> tmp; 
		color_tone = (glow_eDrawTone)tmp; break;
      case glow_eSave_GrowNode_original_color_lightness: fp >> original_color_lightness; break;
      case glow_eSave_GrowNode_color_lightness: fp >> color_lightness; break;
      case glow_eSave_GrowNode_original_color_intensity: fp >> original_color_intensity; break;
      case glow_eSave_GrowNode_color_intensity: fp >> color_intensity; break;
      case glow_eSave_GrowNode_original_color_shift: fp >> original_color_shift; break;
      case glow_eSave_GrowNode_color_shift: fp >> color_shift; break;
      case glow_eSave_GrowNode_invisible: fp >> invisible; break;
      case glow_eSave_GrowNode_dynamicsize: fp >> dynamicsize; break;
      case glow_eSave_GrowNode_dynamic:
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
      case glow_eSave_GrowNode_node_part: 
        GlowNode::open( fp);
        break;
      case glow_eSave_GrowNode_trf: trf.open( fp); break;
      case glow_eSave_GrowNode_arg_cnt: fp >> arg_cnt; break;
      case glow_eSave_GrowNode_argsize:
        for ( i = 0; i < arg_cnt; i++)
          fp >> argsize[i];
        break;
      case glow_eSave_GrowNode_argv:
        fp.getline( dummy, sizeof(dummy));
        for ( i = 0; i < arg_cnt; i++)
        {
          if ( argsize[i])
          {
            argv[i] = (char *) calloc( 1, argsize[i]);
	    fp.get();
            for ( j = 0; j < argsize[i]; j++)
	    {
	      if ((c = fp.get()) == '"')
	      {
	        if ( argv[i][j-1] == '\\')
	          j--;
	        else
                {
	          argv[i][j] = 0;
                  break;
                }
	      }
              argv[i][j] = c;
	    }
            fp.getline( dummy, sizeof(dummy));
          }
          if ( i >= nc->arg_cnt)
          {
            // Argument obsolete
            argsize[i] = 0;
            free( argv[i]);
          }
        }
        break;
      case glow_eSave_GrowNode_line_width: fp >> line_width; break;
      case glow_eSave_GrowNode_shadow: fp >> shadow; break;
      case glow_eSave_GrowNode_userdata_cb:
	if ( ctx->userdata_open_callback)
	  (ctx->userdata_open_callback)(&fp, this, glow_eUserdataCbType_Node);
	break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowNode:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }

  // Translate the annotation to current language
  if ( ((GrowCtx *)ctx)->translate_on && 
       ctx->event_callback[glow_eEvent_Translate]) {
    for ( i = 0; i < 10; i++) {
      if ( annotsize[i] && annotv[i]) {
        if ( ((GrowCtx *)ctx)->translate_cb( this, annotv[i], &new_text))
          set_annotation( i, new_text, strlen(new_text), 1);
      }
    }
  }

}

void GrowNode::move_to( double x, double y)
{
  ctx->set_defered_redraw();
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  erase();
  nav_erase();

  trf.move( x - x_left, y - y_low);
  get_node_borders();

  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->redraw_defered();
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowNode::move( double delta_x, double delta_y, int grid)
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
    obst_x_right += dx;
    obst_x_left += dx;
    obst_y_high += dy;
    obst_y_low += dy;
  }
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->redraw_defered();
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowNode::move_noerase( int delta_x, int delta_y, int grid)
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
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowNode::set_highlight( int on)
{
  erase();
  highlight = on;
  draw();
//  if ( !inverse)
//    draw( (GlowTransform *)NULL, highlight, hot, NULL);
//  else
//    draw_inverse();
//  nav_draw( (GlowTransform *)NULL, highlight, NULL);
}

int GrowNode::event_handler( glow_eEvent event, double fx, double fy)
{
  double x, y;
  int sts;

  if ( invisible || dimmed)
    return 0;

  trf.reverse( fx, fy, &x, &y);
  sts =  nc->event_handler( event, x, y);
  if  ( ctx->trace_started && sts) {
    // Register group members with click action
    if ( is_sensitive())
      ctx->register_callback_object( glow_eObjectType_Node, this);
  }
  return sts;
}

int GrowNode::event_handler( glow_eEvent event, int x, int y, double fx,
	double fy)
{
  int sts;
  double distance;
  GlowConPoint *conpoint;
  int hot_type;

  double rx, ry;

  if ( invisible || dimmed)
    return 0;

  switch ( event) {
  case glow_eEvent_Key_Right:
  case glow_eEvent_Key_Left:
  case glow_eEvent_Key_BackSpace:
  case glow_eEvent_Key_Return:
  case glow_eEvent_Key_Ascii:
    if ( input_active) {
      annot_input_event( event, x);
      return 1;
    }
    else
      return 0;
  case glow_eEvent_Key_Up:
  case glow_eEvent_Key_Down:
    return 0;
  default:
    ;
  }

  // Convert koordinates to local koordinates
  trf.reverse( fx, fy, &rx, &ry);

  sts = 0;
  if ( event == ctx->event_move_node)
  {
    sts = nc->event_handler( event, rx, ry);
    if ( sts)
    {
      /* Register node for potential movement */
      ctx->move_insert( this);
      store_position();
    }
    return sts;
  }
  else if ( event == ctx->event_create_con)
  {
    sts = nc->event_handler( event, rx, ry);
    if ( sts)
    {
      int px, py;
      
      /* Find closest conpoint */
      distance = 1e10;
      conpoint = 0;
      nc->a.conpoint_select( &trf, x, y, &distance, 
		(void **) &conpoint, &px, &py);
      if ( conpoint)
      {
        ctx->con_create_source( this, conpoint->number, px, py);
      }
    }
    return sts;
  }
  switch ( event)
  {
    case glow_eEvent_ButtonRelease:
      if ( ctx->con_create_active)
      {

        sts = nc->event_handler( event, rx, ry);
        if ( sts)
        {
          int px, py;

          /* Find closest conpoint */
          distance = 1e10;
          conpoint = 0;
          nc->a.conpoint_select( &trf, x, y, &distance, 
		(void **) &conpoint, &px, &py);
          if ( conpoint)
          {
            ctx->con_create_dest( this, conpoint->number, event, x, y);
          }
        }
      }
      break;
    case glow_eEvent_CursorMotion:
    {
      int redraw = 0;

      if ( ctx->hot_mode == glow_eHotMode_TraceAction)
      {
        if ( ctx->hot_found)
          sts = 0;
        else
        {
          if ( !is_sensitive()) {
            if ( type() == glow_eObjectType_GrowGroup) {
              sts = nc->a.event_handler( event, x, y, rx, ry);
              sts = 0;
            }
            else
              sts = 0;
          }
          else
          {
            sts = nc->event_handler( event, rx, ry);
            if ( sts)
              ctx->hot_found = 1;
          }
        }
      }
      else
      {
        if ( ctx->hot_found)
          sts = 0;
        else
        {
          sts = nc->event_handler( event, rx, ry);
          if ( sts)
            ctx->hot_found = 1;
        }
      }
      if ( sts && !hot  &&
	   !(ctx->node_movement_active || ctx->node_movement_paste_active))
      {
        if ( (hot_type = ((GrowCtx *)ctx)->send_hot_request( this)))
        {
          if ( !ctx->trace_started) {
            draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);
            hot = 1;
            redraw = 1;
	    ctx->tiptext_event( this, x, y);
          }
          else if ( hot_type & glow_mHotType_CursorCrossHair) {
            draw_set_cursor( ctx, glow_eDrawCursor_CrossHair);
            hot = 1;
	    ctx->tiptext_event( this, x, y);
            redraw = 1;
          }
          else if ( hot_type & glow_mHotType_CursorHand) {
            draw_set_cursor( ctx, glow_eDrawCursor_Hand);
            hot = 1;
	    ctx->tiptext_event( this, x, y);
            redraw = 1;
          }
        }
      }
      if ( !sts && hot)
      {
        if ( !ctx->hot_found)
          draw_set_cursor( ctx, glow_eDrawCursor_Normal);
        if ( root_node)
          root_node->erase();
        else
          erase();
        hot = 0;
        redraw = 1;
	ctx->tiptext->remove_text( this);
      }
      if ( redraw)
      {
        if ( !inverse)
          draw();
	  //         ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	  //  y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	  //   x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	  //   y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
        else
          draw_inverse();
      }
      break;
    }
    default:
      sts = nc->event_handler( event, rx, ry);
  }
  if ( sts) {
    if ( ctx->trace_started && type() == glow_eObjectType_GrowGroup) {
      // If group, only register i sensitive (group member might be sensitive)
      if ( is_sensitive())
        ctx->register_callback_object( glow_eObjectType_Node, this);
    }
    else
      ctx->register_callback_object( glow_eObjectType_Node, this);
  }
  return sts;
}

void GrowNode::set_hot( int on)
{
  if ( hot != on)
  {
    hot = on;
    if ( !inverse)
      draw();
    else
    {
      draw_inverse();
      nc->nav_draw( &trf, highlight, (void *)this, (void *)this);
    }
  }
}

void GrowNode::set_dynamic( char *code, int size)
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

void GrowNode::exec_dynamic()
{
  // Dynamic code for the node
  if ( dynamicsize && strcmp( dynamic, "") != 0)
    ((GrowCtx *)ctx)->dynamic_cb( this, dynamic, glow_eDynamicType_Object);

  // Dynamic code for the nodeclass
  if ( nc->dynamicsize && strcmp( nc->dynamic, "") != 0)
    ((GrowCtx *)ctx)->dynamic_cb( this, nc->dynamic, glow_eDynamicType_SubGraph);
  else
    ((GrowCtx *)ctx)->dynamic_cb( this, (char *)NULL, glow_eDynamicType_SubGraph);

  nc->a.exec_dynamic();
  ((GrowCtx *)ctx)->dynamic_cb( this, (char *)NULL, glow_eDynamicType_SubGraphEnd);
}

void GrowNode::set_position( double x, double y)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  if ( trf.a13 == trf.s_a13 + x && trf.a23 == trf.s_a23 + y)
    return;
  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  erase();
  nav_erase();
  trf.move_from_stored( x, y);
  get_node_borders();
  call_redraw_node_cons();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowNode::set_scale_pos( double x, double y, 
        double scale_x, double scale_y, 
	double x0, double y0, glow_eScaleType type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;

  erase();
  nav_erase();

#if 0
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
  trf.scale_from_stored( scale_x, scale_y, x0, y0);
#endif
  ctx->set_nodraw();
  set_scale( scale_x, scale_y, x0, y0, type);
  ctx->reset_nodraw();
  trf.move( x - x_left, y - y_low);
  get_node_borders();

  ctx->set_defered_redraw();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ctx->redraw_defered();
  ((GrowCtx *)ctx)->nav_draw(  old_x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     old_y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     old_x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     old_y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);

}

void GrowNode::set_scale( double scale_x, double scale_y, 
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

/*
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
**/
  call_redraw_node_cons();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowNode::set_rotation( double angel, 
	double x0, double y0, glow_eRotationPoint type)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;
  GlowTransform t;

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
    case glow_eRotationPoint_Zero:
      x0 = trf.a13;
      y0 = trf.a23;
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
//  trf.rotate_from_stored( angel, x0, y0);
  t.rotate( angel, x0, y0);
  trf.set_from_stored( &t);
  get_node_borders();
  call_redraw_node_cons();
  ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowNode::draw()
{
  if ( root_node)
    root_node->draw();
  else
    ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowNode::draw( int ll_x, int ll_y, int ur_x, int ur_y)
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

  if ( x_right * ctx->zoom_factor_x - ctx->offset_x >= ll_x &&
      	x_left * ctx->zoom_factor_x - ctx->offset_x <= ur_x &&
       	y_high * ctx->zoom_factor_y - ctx->offset_y >= ll_y &&
       	y_low * ctx->zoom_factor_y - ctx->offset_y <= ur_y)
  {
    if ( !inverse)
      draw( (GlowTransform *)NULL, highlight, hot, (void *)this, NULL);
    else
      draw_inverse();
  }
}

void GrowNode::draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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
      draw( (GlowTransform *)NULL, highlight, hot, (void *)this, NULL);
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

void GrowNode::nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) 
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
    nav_draw( (GlowTransform *)NULL, highlight, (void *)this, NULL);

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


void GrowNode::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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
//    if ( x_right_pix - x_left_pix < 10 && y_high_pix - y_low_pix < 10)
//      glow_draw_nav_rect( ctx, x_left_pix, y_low_pix, 
//	x_right_pix - x_left_pix, y_high_pix - y_low_pix,
//	glow_eDrawType_Line, 0, highlight);
//    else
      nc->nav_draw( &trf, highlight, (void *)this, (void *)this);
  }
}

void GrowNode::draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode)
{
  if ( node && !root_node && node != (void *)this)
    root_node = (GrowNode *)node;

  if ( !node)
    node = (void *) this;
  if ( !colornode)
    colornode = (void *) this;
  else if ( ((GrowNode *)colornode)->fill_drawtype == glow_eDrawType_No && 
	    ((GrowNode *)colornode)->color_tone == glow_eDrawTone_No)
    colornode = (void *) this;

  if ( ((GrowNode *)node)->invisible)
    return;

  if ( input_focus) {
    GlowNodeClass *base_nc = nc->get_base_nc();

    switch( base_nc->input_focus_mark) {
    case glow_eInputFocusMark_Relief: {
      hot = 1;

      int x1, x2, y1, y2;
      glow_eDrawType drawtype;

      if (!t) {
	x1 = int( x_left * ctx->zoom_factor_x + 0.5) - ctx->offset_x - 2;
	y1 = int( y_low * ctx->zoom_factor_y + 0.5) - ctx->offset_y - 2;
	x2 = int( x_right * ctx->zoom_factor_x + 0.5) - ctx->offset_x + 2;
	y2 = int( y_high * ctx->zoom_factor_y + 0.5) - ctx->offset_y + 2;
      }  
      else {
	x1 = int( t->x( x_left, y_low) * ctx->zoom_factor_x + 0.5) - ctx->offset_x - 2;
	y1 = int( t->y( x_left, y_low) * ctx->zoom_factor_y + 0.5) - ctx->offset_y - 2;
	x2 = int( t->x( x_right, y_high) * ctx->zoom_factor_x + 0.5) - ctx->offset_x + 2;
	y2 = int( t->y( x_right, y_high) * ctx->zoom_factor_y + 0.5) - ctx->offset_y + 2;
      }
      
      drawtype = glow_eDrawType_DarkGray;
      glow_draw_line( ctx, x1, y1, x1, y2, drawtype, 0, 0);
      glow_draw_line( ctx, x1, y1, x2, y1, drawtype, 0, 0);
      drawtype = glow_eDrawType_LightGray;
      glow_draw_line( ctx, x2, y1, x2, y2, drawtype, 0, 0);
      glow_draw_line( ctx, x1, y2, x2, y2, drawtype, 0, 0);
      break;
    }
    default:
      ;
    }
  }

  // If group member with click action, set hot
  if ( ctx->trace_started && this->hot && root_node && 
       ((GrowNode *)root_node)->type() == glow_eObjectType_GrowGroup &&
       !((GrowGroup *)root_node)->hot && is_sensitive())
    hot = 1;

  if ( fill_level == 1) {
    if ( t) {
      GlowTransform trf_tot = *t * trf;

      if ( !trace.p)
	nc->draw( &trf_tot, highlight, hot, (void *)this, colornode);
      else
	// If this node has a trace pointer, use colors for this node
	nc->draw( &trf_tot, highlight, hot, (void *)this, (void *)this);
    }
    else
      nc->draw( &trf, highlight, hot, node, node);
  }
  else {
    int x1, x2, y1, y2, x_level, y_level;
    int clip_sts;
    glow_eDrawTone old_color_tone;
    glow_eDrawType old_fill_drawtype;

    if (!t) {
      x1 = int( x_left * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
      y1 = int( y_low * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
      x2 = int( x_right * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
      y2 = int( y_high * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
    }
    else {
      x1 = int( t->x( x_left, y_low) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
      y1 = int( t->y( x_left, y_low) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
      x2 = int( t->x( x_right, y_high) * ctx->zoom_factor_x + 0.5) - ctx->offset_x;
      y2 = int( t->y( x_right, y_high) * ctx->zoom_factor_y + 0.5) - ctx->offset_y;
    }

    switch ( level_direction) {
    case glow_eDirection_Right:
      x_level = int( fill_level * (x2 - x1) + 0.5);
      clip_sts = glow_draw_set_clip_rectangle( ctx, x1 - 1, y1 - 1, x1 + x_level, y2 + 1);
      break;
    case glow_eDirection_Left:
      x_level = int( (1 - fill_level) * ( x2 - x1) + 0.5);
      clip_sts = glow_draw_set_clip_rectangle( ctx, x1 - 1, y1 - 1, x1 + x_level, y2 + 1);
      if ( level_color_tone != glow_eDrawTone_No) {
	old_color_tone = color_tone;
	color_tone = level_color_tone;
      }
      else if ( level_fill_drawtype != glow_eDrawType_No) {
	old_fill_drawtype = fill_drawtype;
	fill_drawtype = level_fill_drawtype;
      }
      break;
    case glow_eDirection_Up:
      y_level = int( fill_level * (y2 - y1) + 0.5);
      clip_sts = glow_draw_set_clip_rectangle( ctx, x1 - 1, y1 - 1, x2 + 1, y1 + y_level);
      break;
    case glow_eDirection_Down:
      y_level = int( (1 - fill_level) * ( y2 - y1) + 0.5);
      clip_sts = glow_draw_set_clip_rectangle( ctx, x1 - 1, y1 - 1, x2 + 1, y1 + y_level);
      if ( level_color_tone != glow_eDrawTone_No) {
	old_color_tone = color_tone;
	color_tone = level_color_tone;
      }
      else if ( level_fill_drawtype != glow_eDrawType_No) {
	old_fill_drawtype = fill_drawtype;
	fill_drawtype = level_fill_drawtype;
      }
      break;
    default: ;
    }

    if ( t) {
      GlowTransform trf_tot = *t * trf;

      if ( !trace.p)
	nc->draw( &trf_tot, highlight, hot, (void *)this, colornode);
      else
	// If this node has a trace pointer, use colors for this node
	nc->draw( &trf_tot, highlight, hot, (void *)this, (void *)this);
    }
    else
      nc->draw( &trf, highlight, hot, node, node);
    if ( ODD(clip_sts))
      glow_draw_reset_clip_rectangle( ctx);

    switch ( level_direction) {
    case glow_eDirection_Right:
      if ( level_color_tone != glow_eDrawTone_No) {
	old_color_tone = color_tone;
	color_tone = level_color_tone;
      }
      else if ( level_fill_drawtype != glow_eDrawType_No) {
	old_fill_drawtype = fill_drawtype;
	fill_drawtype = level_fill_drawtype;
      }
      clip_sts = glow_draw_set_clip_rectangle( ctx, x1 + x_level, y1 - 1, x2 + 1, y2 + 1);
      break;
    case glow_eDirection_Left:
      if ( level_color_tone != glow_eDrawTone_No)
	color_tone = old_color_tone;
      else if ( level_fill_drawtype != glow_eDrawType_No)
	fill_drawtype = old_fill_drawtype;
      clip_sts = glow_draw_set_clip_rectangle( ctx, x1 + x_level, y1 - 1, x2 + 1, y2 + 1);
      break;
    case glow_eDirection_Up:
      if ( level_color_tone != glow_eDrawTone_No) {
	old_color_tone = color_tone;
	color_tone = level_color_tone;
      }
      else if ( level_fill_drawtype != glow_eDrawType_No) {
	old_fill_drawtype = fill_drawtype;
	fill_drawtype = level_fill_drawtype;
      }
      clip_sts = glow_draw_set_clip_rectangle( ctx, x1 - 1, y1 + y_level, x2 + 1, y2 + 1);
      break;
    case glow_eDirection_Down:
      if ( level_color_tone != glow_eDrawTone_No)
	color_tone = old_color_tone;
      else if ( level_fill_drawtype != glow_eDrawType_No)
	fill_drawtype = old_fill_drawtype;
      clip_sts = glow_draw_set_clip_rectangle( ctx, x1 - 1, y1 + y_level, x2 + 1, y2 + 1);
      break;
    default: ;
    }

    if ( t) {
      GlowTransform trf_tot = *t * trf;

      if ( !trace.p)
	nc->draw( &trf_tot, highlight, hot, (void *)this, colornode);
      else
	// If this node has a trace pointer, use colors for this node
	nc->draw( &trf_tot, highlight, hot, (void *)this, (void *)this);
    }
    else
      nc->draw( &trf, highlight, hot, node, node);
    if ( ODD(clip_sts))
      glow_draw_reset_clip_rectangle( ctx);

    switch ( level_direction) {
    case glow_eDirection_Right:
      if ( level_color_tone != glow_eDrawTone_No)
	color_tone = old_color_tone;
      else if ( level_fill_drawtype != glow_eDrawType_No)
	fill_drawtype = old_fill_drawtype;
      break;
    case glow_eDirection_Left:
      break;
    case glow_eDirection_Up:
      if ( level_color_tone != glow_eDrawTone_No)
	color_tone = old_color_tone;
      else if ( level_fill_drawtype != glow_eDrawType_No)
	fill_drawtype = old_fill_drawtype;
      break;
    case glow_eDirection_Down:
      break;
    default: ;
    }
  }
}

void GrowNode::erase( GlowTransform *t, int hot, void *node)
{
  ctx->set_draw_buffer_only();

  if ( input_focus) {
    GlowNodeClass *base_nc = nc->get_base_nc();

    switch( base_nc->input_focus_mark) {
    case glow_eInputFocusMark_Relief: {
      hot = 1;

      int x1, x2, y1, y2;

      if (!t) {
	x1 = int( x_left * ctx->zoom_factor_x + 0.5) - ctx->offset_x - 2;
	y1 = int( y_low * ctx->zoom_factor_y + 0.5) - ctx->offset_y - 2;
	x2 = int( x_right * ctx->zoom_factor_x + 0.5) - ctx->offset_x + 2;
	y2 = int( y_high * ctx->zoom_factor_y + 0.5) - ctx->offset_y + 2;
      }
      else {
	x1 = int( t->x( x_left, y_low) * ctx->zoom_factor_x + 0.5) - ctx->offset_x - 2;
	y1 = int( t->y( x_left, y_low) * ctx->zoom_factor_y + 0.5) - ctx->offset_y - 2;
	x2 = int( t->x( x_right, y_high) * ctx->zoom_factor_x + 0.5) - ctx->offset_x + 2;
	y2 = int( t->y( x_right, y_high) * ctx->zoom_factor_y + 0.5) - ctx->offset_y + 2;
      }
      
      glow_draw_line_erase( ctx, x1, y1, x1, y2, 0);
      glow_draw_line_erase( ctx, x1, y1, x2, y1, 0);
      glow_draw_line_erase( ctx, x2, y1, x2, y2, 0);
      glow_draw_line_erase( ctx, x1, y2, x2, y2, 0);
      break;
    }
    default:
      ;
    }
  }
  // If group member with click action, set hot
  if ( ctx->trace_started && this->hot && node && 
       ((GrowNode *)node)->type() == glow_eObjectType_GrowGroup &&
       !((GrowGroup *)node)->hot && is_sensitive())
    hot = 1;

  if ( !node)
    node = (void *) this;

  if ( t)
  {
    GlowTransform trf_tot = *t * trf;

    nc->erase( &trf_tot, hot, node);
  }
  else
    nc->erase( &trf, hot, node);

  ctx->reset_draw_buffer_only();
}

void GrowNode::nav_draw( GlowTransform *t, int highlight, void *node, void *colornode)
{
  if ( !colornode)
    colornode = (void *) this;
  else if ( ((GrowNode *)colornode)->fill_drawtype == glow_eDrawType_No && 
	    ((GrowNode *)colornode)->color_tone == glow_eDrawTone_No)
    colornode = (void *) this;

  if ( t)
  {
    GlowTransform trf_tot = *t * trf;

    nc->nav_draw( &trf_tot, highlight, (void *)this, node);
  }
  else
    nc->nav_draw( &trf, highlight, (void *)this, node);
}

void GrowNode::nav_erase( GlowTransform *t, void *node)
{
  if ( !node)
    node = (void *) this;

  if ( t)
  {
    GlowTransform trf_tot = *t * trf;

    nc->nav_erase( &trf_tot, node);
  }
  else
    nc->nav_erase( &trf, node);
}

void GrowNode::set_transform( GlowTransform *t)
{
  trf = *t * trf;
  get_node_borders();
  call_redraw_node_cons();
}


int GrowNode::get_conpoint( int num, double *x, double *y, glow_eDirection *dir)
{
  int sts;

  sts = nc->get_conpoint( &trf, num, flip_horizontal, flip_vertical, x, y, dir);
  return sts;
}


void GrowNode::set_annotation( int num, char *text, int size, int nodraw, int brief)
{
  if ( annotv_inputmode[num])
    return;

  if ( !root_node) {
    if ( !nodraw && !invisible)
    {
      if ( relative_annot_pos || !brief)
        erase();
      else
        nc->erase_annotation( &trf, highlight, hot, (void *) this, num);
    }
  }
  if ( !annotv[num])
  {
    annotv[num] = (char *) calloc( 1, size+1);
    annotsize[num] = size+1;
  }
  else if ( annotsize[num] < size+1)
  {
    free( annotv[num]);
    annotv[num] = (char *) calloc( 1, size+1);
    annotsize[num] = size+1;
  }
  strncpy( annotv[num], text, size+1);
  if ( nodraw || invisible)
    return;

  if ( !root_node) {
    if ( relative_annot_pos)
    {
    }
    else
    {
      if ( brief && !inverse)
	nc->draw_annotation( &trf, highlight, hot, (void *) this, num);
      else if ( !inverse)
	draw();
      else
        draw_inverse();
    }
    nc->nav_draw( &trf, highlight, (void *)this, (void *)this);
  }
  else {
    root_node->draw();
    root_node->nav_draw( (GlowTransform *)NULL, root_node->highlight, NULL, NULL);
  }

}

void GrowNode::get_borders( GlowTransform *t,
	double *x1_right, double *x1_left, double *y1_high, double *y1_low)
{
  if ( t) {
    GlowTransform t2 = *t * trf;
    nc->get_borders( &t2, x1_right, x1_left, y1_high, y1_low);
  }
  else
    nc->get_borders( &trf, x1_right, x1_left, y1_high, y1_low);

}

int GrowNode::get_argument( int num, char *arg, int size)
{ 

  if ( num >= nc->arg_cnt)
    return 0;

  if ( argsize[num] == 0)
    strcpy( arg, "");
  else
    strncpy( arg, argv[num], size);
  return 1;
}

int GrowNode::set_argument( int num, char *arg, int size)
{ 

  if ( num >= nc->arg_cnt)
    return 0;

  if ( !argv[num])
  {
    argv[num] = (char *) calloc( 1, size+1);
    argsize[num] = size+1;
  }
  else if ( argsize[num] < size+1)
  {
    free( argv[num]);
    argv[num] = (char *) calloc( 1, size+1);
    argsize[num] = size+1;
  }
  strncpy( argv[num], arg, size+1);
  return 1;
}

void GrowNode::set_linewidth( int linewidth)
{
  erase();
  nav_erase();
  line_width = linewidth;
  draw();
}

void GrowNode::get_node_borders()
{ 
  x_left = y_low = 1e37; 
  x_right = y_high = -1e37;
  nc->get_borders( &trf, &x_right, &x_left, &y_high, &y_low);
        
  if ( nc->no_con_obstacle)
  {
    obst_x_left = obst_y_low = 1e37; 
    obst_x_right = obst_y_high = -1e37;
  }
  else
  {
    obst_x_left = x_left; 
    obst_x_right = x_right;
    obst_y_low = y_low;
    obst_y_high = y_high;
  }
}

void GrowNode::align( double x, double y, glow_eAlignDirection direction)
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
    obst_x_right += dx;
    obst_x_left += dx;
    obst_y_high += dy;
    obst_y_low += dy;

    draw();
}

void GrowNode::export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp)
{
  double x1, y1, x2, y2, rot;
  char java_name[40];

  get_node_borders();
  if (!t)
  {
    x1 = x_left * ctx->zoom_factor_x - ctx->offset_x;
    y1 = y_low * ctx->zoom_factor_y - ctx->offset_y;
    x2 = x_right * ctx->zoom_factor_x - ctx->offset_x;
    y2 = y_high * ctx->zoom_factor_y - ctx->offset_y;
    rot = trf.rot();
  }
  else
  {
    x1 = t->x( x_left, y_low) * ctx->zoom_factor_x - ctx->offset_x;
    y1 = t->y( x_left, y_low) * ctx->zoom_factor_y - ctx->offset_y;
    x2 = t->x( x_right, y_high) * ctx->zoom_factor_x - ctx->offset_x;
    y2 = t->y( x_right, y_high) * ctx->zoom_factor_y - ctx->offset_y;
    rot = trf.rot(t);
  }

  nc->get_java_name( java_name);
  ((GrowCtx *)ctx)->export_jbean->node( x1, y1, x2, y2, java_name, 
   	draw_type, fill_drawtype, text_drawtype, color_tone, color_lightness,
	color_intensity, color_shift, line_width, rot, shadow,
    	pass, shape_cnt, node_cnt, in_nc, fp);
}

int GrowNode::set_next_nodeclass()
{
  if ( nc->next_nc)
  {
    set_nodeclass( (GlowNodeClass *)nc->next_nc);
    return 1;    
  }
  return 0;
}

void GrowNode::set_last_nodeclass()
{
  GlowArrayElem *next;
  for ( next = nc_root; 
        ((GlowNodeClass *)next)->next_nc; 
	next = ((GlowNodeClass *)next)->next_nc)
    ;
  if ( next != nc)
    set_nodeclass( (GlowNodeClass *)next);
}

int GrowNode::set_nodeclass_by_index( int idx)
{
  GlowArrayElem *next;
  int i;

  next = nc_root;
  for ( i = 0; i < idx; i++)
  {
    if ( !((GlowNodeClass *)next)->next_nc)
      return 0;
    next = ((GlowNodeClass *)next)->next_nc;
  }
  if ( next != nc)
    set_nodeclass( (GlowNodeClass *)next);
  return 1;
}

int GrowNode::set_previous_nodeclass()
{
  if ( nc->prev_nc)
  {
    set_nodeclass( (GlowNodeClass *)nc->prev_nc);
    return 1;    
  }
  return 0;
}

void GrowNode::set_root_nodeclass()
{
  if ( nc_root != nc)
    set_nodeclass( nc_root);
}

void GrowNode::set_nodeclass( GlowNodeClass *new_nc)
{
  double old_x_left, old_x_right, old_y_low, old_y_high;

  old_x_left = x_left;
  old_x_right = x_right;
  old_y_low = y_low;
  old_y_high = y_high;
  if ( root_node)
    root_node->erase();
  else
    erase();
  nav_erase();
  nc = new_nc;
  get_node_borders();
//  call_redraw_node_cons();
  old_x_left = min( old_x_left, x_left);
  old_x_right = max( old_x_right, x_right);
  old_y_low = min( old_y_low, y_low);
  old_y_high = max( old_y_high, y_high);
  if ( root_node)
    root_node->draw();
  else
    ((GrowCtx *)ctx)->draw( old_x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     old_y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     old_x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     old_y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  old_x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     old_y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     old_x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     old_y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
}

void GrowNode::trace_close()
{
  GlowNode::trace_close();
  set_root_nodeclass();
}

int GrowNode::find_nc( GlowArrayElem *nodeclass)
{
  GlowArrayElem *next_nc;

  if ( nc == nodeclass)
    return 1;

  for ( next_nc = nc->next_nc; next_nc; next_nc = ((GlowNodeClass *)next_nc)->next_nc)
    if ( nodeclass == next_nc)
      return 1;

  return nc->a.find_nc( nodeclass);
}

int GrowNode::is_sensitive()
{
  return ((GrowCtx *)ctx)->send_hot_request( this);
#if 0
  return (is_refobject_sensitive() || is_click_sensitive());
#endif
}

int GrowNode::is_click_sensitive()
{
#if 0
  int trace_type = (int) trace.attr_type;
  if ( trace_type == 0) {			// Inherit tracetype from nc
    GlowNodeClass *base_nc = nc->get_base_nc();
    trace_type = base_nc->trace_attr_type;
  }
  if ( trace_type >= 1000)
    return 1;
#endif
  return 0;
}

int GrowNode::is_refobject_sensitive()
{
  if ( strcmp( trace.ref_object, "") == 0)
    return 0;
  return 1;
}


int GrowNode::get_limits( double *min, double *max, glow_eDirection *direction)
{
  if ( nc->y0 == 0 && nc->y1 == 0)
    return 0;
  if ( !(nc->x0 == 0 && nc->x1 == 0))
    return 0;

  double x1, x2, y1, y2;
  double rotation;

  // Calculate max and min koordinates

  x1 = trf.x( 0, nc->y0);
  y1 = trf.y( 0, nc->y0);
  x2 = trf.x( 0, nc->y1);
  y2 = trf.y( 0, nc->y1);

  rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;

  if ( 45 >= rotation || rotation > 315) {
    *direction = glow_eDirection_Down;
    *min = y1;
    *max = y2;
  }
  else if ( 45 < rotation && rotation <= 135) {
    *direction = glow_eDirection_Right;
    *min = x2;
    *max = x1;
  }
  else if ( 135 < rotation && rotation <= 225) {
    *direction = glow_eDirection_Up;
    *min = y2;
    *max = y1;
  }
  else if ( 225 < rotation && rotation <= 315) {
    *direction = glow_eDirection_Left;
    *min = x1;
    *max = x2;
  }
  return 1;
}

int GrowNode::get_limits_pixel( double *pix_min, double *pix_max, glow_eDirection *direction)
{
  if ( nc->y0 == 0 && nc->y1 == 0)
    return 0;
  if ( !(nc->x0 == 0 && nc->x1 == 0))
    return 0;

  double x1, x2, y1, y2, nc_x_right, nc_x_left, nc_y_high, nc_y_low, x0, y0;
  double rotation;

  // Calculate max and min koordinates
  nc_x_left = nc_y_low = 1e37; 
  nc_x_right = nc_y_high = -1e37;
  nc->get_borders( 0, &nc_x_right, &nc_x_left, &nc_y_high, &nc_y_low);

  x1 = trf.x( 0, nc->y0);
  y1 = trf.y( 0, nc->y0);
  x2 = trf.x( 0, nc->y1);
  y2 = trf.y( 0, nc->y1);
  x0 = trf.x( 0, nc_y_low);
  y0 = trf.y( 0, nc_y_low);


  rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;
  double min, max;

  if ( 45 >= rotation || rotation > 315) {
    *direction = glow_eDirection_Down;
    min = y1 - y0;
    max = y2 - y0;
  }
  else if ( 45 < rotation && rotation <= 135) {
    *direction = glow_eDirection_Right;
    min = x0 - x1;
    max = x0 - x2;
  }
  else if ( 135 < rotation && rotation <= 225) {
    *direction = glow_eDirection_Up;
    min = y0 - y1;
    max = y0 - y2;
  }
  else if ( 225 < rotation && rotation <= 315) {
    *direction = glow_eDirection_Left;
    min = x1 - x0;
    max = x2 - x0;
  }

  switch( *direction) {
  case glow_eDirection_Up:
  case glow_eDirection_Down:
    *pix_max = max * ctx->zoom_factor_y;
    *pix_min = min * ctx->zoom_factor_y;
    break;
  case glow_eDirection_Left:
  case glow_eDirection_Right:
    *pix_max = max * ctx->zoom_factor_x;
    *pix_min = min * ctx->zoom_factor_x;
    break;
  default: ;
  }
  return 1;
}

int GrowNode::get_background_object_limits(GlowTransform *t,
		    glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction)
{
  int dyn_type;
  GlowNodeClass *base_nc = nc->get_base_nc();
  dyn_type = base_nc->dyn_type;

  if ( !( (int) type & dyn_type))
    return 0;
  if ( nc->y0 == 0 && nc->y1 == 0)
    return 0;

  double x1, x2, y1, y2;
  double rotation;
  double x1_right = 10e-37;
  double x1_left = 10e37;
  double y1_high = 10e-37;
  double y1_low = 10e37;
  get_borders( t, &x1_right, &x1_left, &y1_high, &y1_low);

  if ( x <= x1_right && x >= x1_left && y <= y1_high && y >= y1_low) {
    // Hit, calculate max and min koordinates

    if (!t)
    {
      x1 = trf.x( 0, nc->y0);
      y1 = trf.y( 0, nc->y0);
      x2 = trf.x( 0, nc->y1);
      y2 = trf.y( 0, nc->y1);
    }
    else
    {
      x1 = trf.x( t, 0, nc->y0);
      y1 = trf.y( t, 0, nc->y0);
      x2 = trf.x( t, 0, nc->y1);
      y2 = trf.y( t, 0, nc->y1);
    }

    if ( t)
      rotation = (trf.rot( t) / 360 - floor( trf.rot( t) / 360)) * 360;
    else
      rotation = (trf.rot() / 360 - floor( trf.rot() / 360)) * 360;
    if ( 45 >= rotation || rotation > 315) {
      *direction = glow_eDirection_Down;
      *min = y1;
      *max = y2;
    }
    else if ( 45 < rotation && rotation <= 135) {
      *direction = glow_eDirection_Left;
      *min = x2;
      *max = x1;
    }
    else if ( 135 < rotation && rotation <= 225) {
      *direction = glow_eDirection_Up;
      *min = y2;
      *max = y1;
    }
    else if ( 225 < rotation && rotation <= 315) {
      *direction = glow_eDirection_Right;
      *min = x1;
      *max = x2;
    }
    *background = this;
    return 1;
  }
  return 0;
}

void GrowNode::get_nodeclass_origo( GlowTransform *t, double *x, double *y)
{
  if ( t) {
    GlowTransform t2 = *t * trf;
    nc->get_origo( &t2, x, y);
  }
  else
    nc->get_origo( &trf, x, y);
}

void GrowNode::call_redraw_node_cons()
{
  ctx->redraw_node_cons( this);
}

void GrowNode::flip( double x0, double y0, glow_eFlipDirection dir)
{
  switch ( dir) {
  case glow_eFlipDirection_Horizontal:
    trf.store();
    set_scale( 1, -1, x0, y0, glow_eScaleType_FixPoint);
    flip_horizontal = !flip_horizontal;
    break;
  case glow_eFlipDirection_Vertical:
    trf.store();
    set_scale( -1, 1, x0, y0, glow_eScaleType_FixPoint);
    flip_vertical = !flip_vertical;
    break;
  }
  call_redraw_node_cons();
}

void GrowNode::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);
    original_border_drawtype = GlowColor::convert( version, original_border_drawtype);
    original_fill_drawtype = GlowColor::convert( version, original_fill_drawtype);
    fill_drawtype = GlowColor::convert( version, fill_drawtype);
    if ( trace.attr_type == 3 || trace.attr_type == 4 || trace.attr_type == 12 ||
	 trace.attr_type == 1010 || trace.attr_type == 1011 || trace.attr_type == 1012 ||
	 trace.attr_type == 1013 || trace.attr_type == 1014 || trace.attr_type == 1015 ||
	 trace.attr_type == 1016 || trace.attr_type == 1021 || trace.attr_type == 1023) {
      if ( (glow_eDrawTone) trace.color == glow_eDrawTone_YellowGreen)
	trace.color = (glow_eDrawType) glow_eDrawTone_Yellow;
      if ( (glow_eDrawTone) trace.color2 == glow_eDrawTone_YellowGreen)
	trace.color2 = (glow_eDrawType) glow_eDrawTone_Yellow;
    }
    else {
      trace.color = GlowColor::convert( version, trace.color);
      trace.color2 = GlowColor::convert( version, trace.color2);
    }
    color_lightness = 2 * color_lightness;
    original_color_lightness = 2 * original_color_lightness;
    // Correction for previous dark green colors
    if ( color_tone == glow_eDrawTone_Green && color_lightness != 0)
      color_lightness--;
    // Tone gold replaced by yellow green
    if ( color_tone == glow_eDrawTone_YellowGreen)
      color_tone = glow_eDrawTone_Yellow;
    if ( original_color_tone == glow_eDrawTone_YellowGreen)
      original_color_tone = glow_eDrawTone_Yellow;
    break;
  }
  }  
}

void GrowNode::set_input_focus( int focus)
{
  if ( focus && !input_focus) {
    input_focus = 1;
    draw();

    ctx->register_inputfocus( this, 1);
  }
  else if ( !focus && input_focus) {
    erase();
    input_focus = 0;
    draw();

    ctx->register_inputfocus( this, 0);
  }
}

void GrowNode::open_annotation_input( int num)
{
  if ( annotv_inputmode[num])
    return;

  input_active = 1;
  input_selected = 1;
  annotv_inputmode[num] = 1;
  if ( annotv[num])
    input_position = strlen(annotv[num]);
  else
    input_position = 0;
  draw();
}

void GrowNode::close_annotation_input( int num)
{
  if ( !annotv_inputmode[num])
    return;

  erase();
  input_active = 0;
  input_selected = 0;
  annotv_inputmode[num] = 0;
  draw();
}

void GrowNode::close_annotation_input()
{
  if ( input_active) {
    input_active = 0;
    for ( int i = 0; i < 10; i++)
      annotv_inputmode[i] = 0;
    draw();
  }
}

void GrowNode::set_annotation_selection( int selection)
{
  if ( !input_active)
    return;

  if ( selection && !input_selected) {
    input_selected = 1;
    draw();
  }
  else if ( !selection && input_selected) {
    input_selected = 0;
    draw();
  }
}

void GrowNode::annot_input_event( glow_eEvent event, int keycode)
{
  int i;
  int idx;
  char *s;

  for ( i = 0; i < 10; i++) {
    if ( annotv_inputmode[i]) {
      idx = i;
      break;
    }
  }

  if ( !annotv[idx])
    return;
  
  switch ( event) {
  case glow_eEvent_Key_Right:
    input_position++;
    if ( input_position > (int)strlen( annotv[idx]))
      input_position = strlen( annotv[idx]);
    input_selected = 0;
    break;
  case glow_eEvent_Key_Left:
    input_position--;
    if ( input_position < 0)
      input_position = 0;
    input_selected = 0;
    break;
  case glow_eEvent_Key_BackSpace:
    if ( input_position == 0)
      break;
    if ( input_selected) {
      annotv[idx][0] = 0;
      input_position = 0;
    }
    else {
      for ( s = &annotv[idx][input_position]; *s; s++)
	*(s - 1) = *s;
      *(s - 1) = 0;
      input_position--;
    }
    input_selected = 0;
    break;
  case glow_eEvent_Key_Ascii:
    if ( input_selected) {
      annotv[idx][0] = 0;
      input_position = 0;
    }
    if ( annotsize[idx] < (int)strlen(annotv[idx]) + 1) {
      // Increase size of annotv
      char *tmp = annotv[idx];
      annotv[idx] = (char *) calloc( 1, annotsize[idx] + 1);
      memcpy( annotv[idx], tmp, annotsize[idx]);
      free( tmp);
      annotsize[idx]++;
    }
    for ( i = strlen(annotv[idx]); i >= input_position; i--)
      annotv[idx][i+1] = annotv[idx][i];
    annotv[idx][input_position] = (char)keycode;
    input_position++;
    input_selected = 0;
    break;
  case glow_eEvent_Key_Return:
    // Send input callback
    ctx->annotation_input_cb( this, idx, annotv[idx]);
    input_selected = 0;
    break;
  default:
    ;
  }
  draw();
}

int GrowNode::get_annotation_info( int num, int *t_size, glow_eDrawType *t_drawtype, glow_eDrawType *t_color,
				 glow_eDrawType *bg_color, double *scale)
{
  int sts;

  sts = nc->get_annot_background( 0, (void *)this, bg_color);
  if ( EVEN(sts))
    *bg_color = glow_eDrawType_No;

  sts = nc->get_annotation_info( (void*)this, num, t_size, t_drawtype, t_color);
  *scale = trf.vertical_scale(0);
  return sts;
}

void GrowNode::set_visibility( glow_eVis visibility)
{
  switch( visibility) {
  case glow_eVis_Visible:
    if ( invisible == 0 && dimmed == 0)
      return;
    invisible = 0;
    dimmed = 0;
    break;
  case glow_eVis_Invisible:
    if ( invisible) 
      return;
    invisible = 1;
    erase();
    break;
  case glow_eVis_Dimmed:
    if ( dimmed && !invisible)
      return;
    dimmed = 1;
    invisible = 0;
    break;
  }
  draw();
}

