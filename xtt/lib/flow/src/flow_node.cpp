/* 
 * Proview   $Id: flow_node.cpp,v 1.12 2008-10-31 12:51:33 claes Exp $
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


#include <iostream>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "flow_node.h"
#include "flow_draw.h"
#include "flow_conpoint.h"
#include "flow_browctx.h"
#include "flow_tiptext.h"
#include "flow_msg.h"

FlowNode::FlowNode( FlowCtx *flow_ctx, const char *name, FlowNodeClass *node_class,
	double x1, double y1, int nodraw, int rel_annot_pos) : 
	x_right(x1), x_left(x1), y_high(y1), y_low(y1), 
	obst_x_right(x1), obst_x_left(x1), obst_y_high(y1), obst_y_low(y1),
	hot(0), ctx(flow_ctx), nc(node_class), pos(flow_ctx, x1,y1), 
	stored_pos(flow_ctx, x1, y1),
	highlight(0), inverse(0), trace_attr_type(flow_eTraceType_Boolean), 
	trace_inverted(0), trace_p(NULL), user_data(0),
	level(0), node_open(0),
	relative_annot_pos(rel_annot_pos), relative_annot_x(0), 
	fill_color(flow_eDrawType_Inherit), sel_conpoint1(-1), sel_conpoint2(-1)
{
  double x_grid, y_grid;
  strncpy( n_name, name, sizeof(n_name));
  n_name[sizeof(n_name)-1] = 0;
  strcpy( trace_object, "");
  strcpy( trace_attribute, "");
  memset( refcon_cnt, 0, sizeof(refcon_cnt));
  memset( annotv, 0, sizeof(annotv));
  memset( annotpixmapv, 0, sizeof(annotpixmapv));
  memset( annotsize, 0, sizeof(annotsize));
  memset( annotv_inputmode, 0, sizeof(annotv_inputmode));
  memset( annotv_input, 0, sizeof(annotv_input));
  memset( rbuttonv, 0, sizeof(rbuttonv));
  memset( rel_annot_x, 0, sizeof(rel_annot_x));
  memset( rel_annotpixmap_x, 0, sizeof(rel_annotpixmap_x));
  if ( !nc)
    return;
  if ( ctx->grid_on)
  {
    ctx->find_grid( x1, y1, &x_grid, &y_grid);
    pos.posit( x_grid, y_grid);
  }
  get_borders();
  zoom();
  if ( !nodraw)
    draw(  0, 0, ctx->window_width, ctx->window_height);
}

void FlowNode::copy_from( const FlowNode& n) 
{
  memcpy( this, &n, sizeof(n));
}

FlowNode::~FlowNode()
{
  if ( ctx->trace_started)
    trace_close();

  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  erase();
  nav_erase();

  for ( int i = 0; i < 10; i++)
  {
    if ( annotv_inputmode[i])
      close_annotation_input( i);
    if ( annotsize[i] > 0)
      free( annotv[i]);
  }

  ctx->set_defered_redraw();
  ctx->delete_node_cons( this);
  ctx->conpoint_select_remove( this);
  ctx->draw( int(x_left * ctx->zoom_factor - ctx->offset_x - 20),
	     int(y_low * ctx->zoom_factor - ctx->offset_y - 20),
  	     int(x_right * ctx->zoom_factor - ctx->offset_x + 20),
	     int(y_high * ctx->zoom_factor - ctx->offset_y + 20));
  ctx->nav_draw( int(x_left * ctx->nav_zoom_factor - ctx->nav_offset_x - 1),
	     int(y_low * ctx->nav_zoom_factor - ctx->nav_offset_y - 1),
  	     int(x_right * ctx->nav_zoom_factor - ctx->nav_offset_x + 1),
	     int(y_high * ctx->nav_zoom_factor - ctx->nav_offset_y + 1));
  ctx->redraw_defered();
  if ( hot)
    ctx->fdraw->set_cursor( ctx, draw_eCursor_Normal);
}

int FlowNode::get_conpoint( int num, double *x, double *y, flow_eDirection *dir)
{
  int sts;

  sts = nc->get_conpoint( num, x, y, dir);
  if ( ODD(sts))
  {
    *x += pos.x;
    *y += pos.y;
  }
  return sts;
}

void FlowNode::move( int delta_x, int delta_y, int grid)
{
  double x, y, x_grid, y_grid;

  if ( grid)
  {
    /* Move to closest grid point */
    x = pos.x + 1.0 * delta_x / ctx->zoom_factor;
    y = pos.y + 1.0 * delta_y / ctx->zoom_factor;
    ctx->find_grid( x, y, &x_grid, &y_grid);
    if ( fabs( x_grid - pos.x) < DBL_EPSILON &&
         fabs( y_grid - pos.y) < DBL_EPSILON)
      return;
    erase();
    nav_erase();
    pos.posit( x_grid, y_grid);
    get_borders();

/*
    x_right += (x_grid - pos.x);
    x_left += (x_grid - pos.x);
    y_high += (y_grid - pos.y);
    y_low += (y_grid - pos.y);
    obst_x_right += (x_grid - pos.x);
    obst_x_left += (x_grid - pos.x);
    obst_y_high += (y_grid - pos.y);
    obst_y_low += (y_grid - pos.y);
*/
  }
  else
  {
    erase();
    nav_erase();
    x = pos.x + 1.0 * delta_x / ctx->zoom_factor;
    y = pos.y + 1.0 * delta_y / ctx->zoom_factor;
    pos.posit( x, y);
    x_right += 1.0 * delta_x / ctx->zoom_factor;
    x_left += 1.0 * delta_x / ctx->zoom_factor;
    y_high += 1.0 * delta_y / ctx->zoom_factor;
    y_low += 1.0 * delta_y / ctx->zoom_factor;
    obst_x_right += 1.0 * delta_x / ctx->zoom_factor;
    obst_x_left += 1.0 * delta_x / ctx->zoom_factor;
    obst_y_high += 1.0 * delta_y / ctx->zoom_factor;
    obst_y_low += 1.0 * delta_y / ctx->zoom_factor;
  }
//  ctx->set_defered_redraw();
//  ctx->redraw_node_cons( this);
  ctx->draw( int(x_left * ctx->zoom_factor - ctx->offset_x - 20),
	     int(y_low * ctx->zoom_factor - ctx->offset_y - 20),
  	     int(x_right * ctx->zoom_factor - ctx->offset_x + 20),
	     int(y_high * ctx->zoom_factor - ctx->offset_y + 20));
  ctx->nav_draw( int(x_left * ctx->nav_zoom_factor - ctx->nav_offset_x - 1),
	     int(y_low * ctx->nav_zoom_factor - ctx->nav_offset_y - 1),
  	     int(x_right * ctx->nav_zoom_factor - ctx->nav_offset_x + 1),
	     int(y_high * ctx->nav_zoom_factor - ctx->nav_offset_y + 1));
//  ctx->redraw_defered();

}

void FlowNode::move_noerase( int delta_x, int delta_y, int grid)
{
  double x, y, x_grid, y_grid;

  if ( grid)
  {
    /* Move to closest grid point */
    x = pos.x + 1.0 * delta_x / ctx->zoom_factor;
    y = pos.y + 1.0 * delta_y / ctx->zoom_factor;
    ctx->find_grid( x, y, &x_grid, &y_grid);
    if ( fabs( x_grid - pos.x) < DBL_EPSILON &&
         fabs( y_grid - pos.y) < DBL_EPSILON)
      return;
    pos.posit( x_grid, y_grid);
    get_borders();
  }
  else
  {
    x = pos.x + 1.0 * delta_x / ctx->zoom_factor;
    y = pos.y + 1.0 * delta_y / ctx->zoom_factor;
    pos.posit( x, y);
    get_borders();
  }
  ctx->draw( int(x_left * ctx->zoom_factor - ctx->offset_x - 20),
	     int(y_low * ctx->zoom_factor - ctx->offset_y - 20),
  	     int(x_right * ctx->zoom_factor - ctx->offset_x + 20),
	     int(y_high * ctx->zoom_factor - ctx->offset_y + 20));
  ctx->nav_draw( int(x_left * ctx->nav_zoom_factor - ctx->nav_offset_x - 1),
	     int(y_low * ctx->nav_zoom_factor - ctx->nav_offset_y - 1),
  	     int(x_right * ctx->nav_zoom_factor - ctx->nav_offset_x + 1),
	     int(y_high * ctx->nav_zoom_factor - ctx->nav_offset_y + 1));
}

void FlowNode::print( double ll_x, double ll_y, double ur_x, double ur_y) 
{ 
  double tmp;

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

  if ( x_right >= ll_x &&
       x_left <= ur_x &&
       y_high >= ll_y &&
       y_low <= ur_y)
    nc->print( &pos, (void *)this, highlight);
}

void FlowNode::save( ofstream& fp, flow_eSaveMode mode) 
{ 
  int i;
  char *s;

  if ( (mode == flow_eSaveMode_Trace && nc->group != flow_eNodeGroup_Trace) ||
       (mode == flow_eSaveMode_Edit && nc->group == flow_eNodeGroup_Trace) )
    return;

  fp << int(flow_eSave_Node) << endl;
  fp << int(flow_eSave_Node_nc) << FSPACE << nc->nc_name << endl;
  fp << int(flow_eSave_Node_n_name) << FSPACE << n_name << endl;
  fp << int(flow_eSave_Node_refcon_cnt) << endl;
  for ( i = 0; i < MAX_CONPOINTS; i++)
    fp << refcon_cnt[i] << endl;
  fp << int(flow_eSave_Node_x_right) << FSPACE << x_right << endl;
  fp << int(flow_eSave_Node_x_left) << FSPACE << x_left << endl;
  fp << int(flow_eSave_Node_y_high) << FSPACE << y_high << endl;
  fp << int(flow_eSave_Node_y_low) << FSPACE << y_low << endl;
  fp << int(flow_eSave_Node_obst_x_right) << FSPACE << obst_x_right << endl;
  fp << int(flow_eSave_Node_obst_x_left) << FSPACE << obst_x_left << endl;
  fp << int(flow_eSave_Node_obst_y_high) << FSPACE << obst_y_high << endl;
  fp << int(flow_eSave_Node_obst_y_low) << FSPACE << obst_y_low << endl;
  fp << int(flow_eSave_Node_annotsize) << endl;
  for ( i = 0; i < 10; i++)
    fp << annotsize[i] << endl;
  fp << int(flow_eSave_Node_annotv) << endl;
  for ( i = 0; i < 10; i++)
  {
    if( annotsize[i])
    {
      fp << "\"";
      for ( s  = annotv[i]; *s; s++)
      {
        if ( *s == '"')
	  fp << "\\";
        fp << *s;
      }
      fp << "\"" << endl;
    }
  }
  fp << int(flow_eSave_Node_pos) << endl;
  pos.save( fp, mode);
  fp << int(flow_eSave_Node_trace_object) << FSPACE << trace_object << endl;
  fp << int(flow_eSave_Node_trace_attribute) << FSPACE << trace_attribute << endl;
  fp << int(flow_eSave_Node_trace_attr_type) << FSPACE << int(trace_attr_type) << endl;
  fp << int(flow_eSave_Node_trace_inverted) << FSPACE << trace_inverted << endl;
  fp << int(flow_eSave_End) << endl;  
}

void FlowNode::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  char		nc_name[80];
  int		i, j;
  char		c;
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Node: break;
      case flow_eSave_Node_nc:
        fp.get();
        fp.getline( nc_name, sizeof(nc_name));
        nc = (FlowNodeClass *) ctx->get_nodeclass_from_name( nc_name);
        if ( !nc)
          cout << "FlowNode:nodeclass not found" << endl;
        break;
      case flow_eSave_Node_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case flow_eSave_Node_refcon_cnt:
        for ( i = 0; i < MAX_CONPOINTS; i++)
          fp >> refcon_cnt[i];
        break;
      case flow_eSave_Node_x_right: fp >> x_right; break;
      case flow_eSave_Node_x_left: fp >> x_left; break;
      case flow_eSave_Node_y_high: fp >> y_high; break;
      case flow_eSave_Node_y_low: fp >> y_low; break;
      case flow_eSave_Node_obst_x_right: fp >> obst_x_right; break;
      case flow_eSave_Node_obst_x_left: fp >> obst_x_left; break;
      case flow_eSave_Node_obst_y_high: fp >> obst_y_high; break;
      case flow_eSave_Node_obst_y_low: fp >> obst_y_low; break;
      case flow_eSave_Node_annotsize:
        for ( i = 0; i < 10; i++)
          fp >> annotsize[i];
        break;
      case flow_eSave_Node_annotv:
        fp.getline( dummy, sizeof(dummy));
        for ( i = 0; i < 10; i++)
        {
          if ( annotsize[i])
          {
            annotv[i] = (char *) calloc( 1, annotsize[i]);
	    fp.get();
            for ( j = 0; j < annotsize[i]; j++)
	    {
	      if ((c = fp.get()) == '"')
	      {
	        if ( j > 0 && annotv[i][j-1] == '\\')
	          j--;
	        else
                {
	          annotv[i][j] = 0;
                  break;
                }
	      }
              annotv[i][j] = c;
	    }
            fp.getline( dummy, sizeof(dummy));
          }
        }
        break;
      case flow_eSave_Node_pos: pos.open( fp); break;
      case flow_eSave_Node_trace_object:
        fp.get();
        fp.getline( trace_object, sizeof(trace_object));
        break;
      case flow_eSave_Node_trace_attribute:
        fp.get();
        fp.getline( trace_attribute, sizeof(trace_attribute));
        break;
      case flow_eSave_Node_trace_attr_type: fp >> tmp; trace_attr_type = (flow_eTraceType)tmp; break;
      case flow_eSave_Node_trace_inverted: fp >> trace_inverted; break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowNode:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  if ( ctx->trace_started)
    trace_init();
}

void FlowNode::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
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

  if ( x_right * ctx->zoom_factor - ctx->offset_x >= ll_x &&
      	x_left * ctx->zoom_factor - ctx->offset_x <= ur_x &&
       	y_high * ctx->zoom_factor - ctx->offset_y >= ll_y &&
       	y_low * ctx->zoom_factor - ctx->offset_y <= ur_y)
  {
    if ( !inverse)
      nc->draw( &pos, highlight, hot, (void *)this);
    else
      draw_inverse();
  }
}

void FlowNode::draw_inverse() 
{
  double x_r;

  if ( ctx->type() == flow_eCtxType_Brow)
    x_r = ((BrowCtx *)ctx)->frame_x_right;
  else
    x_r = x_right;


  if ( nc->group == flow_eNodeGroup_Document) {
    ctx->fdraw->fill_rect( ctx,
       		int( obst_x_left * ctx->zoom_factor - ctx->offset_x),
       		int( obst_y_low * ctx->zoom_factor - ctx->offset_y),
		int( obst_x_right * ctx->zoom_factor - obst_x_left * ctx->zoom_factor),
  	     	int( obst_y_high * ctx->zoom_factor - obst_y_low * ctx->zoom_factor),
		flow_eDrawType_Line);
    nc->draw( &pos, highlight, hot, (void *)this);
  }
  else {
    ctx->fdraw->fill_rect( ctx,
       		int( x_left * ctx->zoom_factor - ctx->offset_x),
       		int( y_low * ctx->zoom_factor - ctx->offset_y),
		int( x_r * ctx->zoom_factor - x_left * ctx->zoom_factor),
  	     	int( y_high * ctx->zoom_factor - y_low * ctx->zoom_factor),
		ctx->inverse_color);
    nc->draw_inverse( &pos, 0, (void *)this);
  }
}

void FlowNode::erase() 
{
  if ( !inverse)
    nc->erase( &pos, hot, (void *)this);
  else
  {
    double x_r;
    if ( ctx->type() == flow_eCtxType_Brow)
      x_r = ((BrowCtx *)ctx)->frame_x_right;
    else
      x_r = x_right;

    if ( nc->group == flow_eNodeGroup_Document) {
      ctx->fdraw->fill_rect( ctx,
       	int( obst_x_left * ctx->zoom_factor - ctx->offset_x),
       	int( obst_y_low * ctx->zoom_factor - ctx->offset_y),
	int( obst_x_right * ctx->zoom_factor - obst_x_left * ctx->zoom_factor),
       	int( obst_y_high * ctx->zoom_factor - obst_y_low * ctx->zoom_factor),
	flow_eDrawType_LineErase);
    }
    else {
      ctx->fdraw->fill_rect( ctx,
       	int( x_left * ctx->zoom_factor - ctx->offset_x),
       	int( y_low * ctx->zoom_factor - ctx->offset_y),
	int( x_r * ctx->zoom_factor - x_left * ctx->zoom_factor),
       	int( y_high * ctx->zoom_factor - y_low * ctx->zoom_factor),
	flow_eDrawType_LineErase);
    }
  }
}

void FlowNode::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int x_right_pix = int( x_right * ctx->nav_zoom_factor - ctx->nav_offset_x);
  int x_left_pix = int( x_left * ctx->nav_zoom_factor - ctx->nav_offset_x);
  int y_high_pix = int( y_high * ctx->nav_zoom_factor - ctx->nav_offset_y);
  int y_low_pix = int( y_low * ctx->nav_zoom_factor - ctx->nav_offset_y);

  if ( x_right_pix >= ll_x &&
       x_left_pix <= ur_x &&
       y_high_pix >= ll_y &&
       y_low_pix <= ur_y)
  {
    if ( x_right_pix - x_left_pix < 10 && y_high_pix - y_low_pix < 10)
      ctx->fdraw->nav_rect( ctx, x_left_pix, y_low_pix, 
	x_right_pix - x_left_pix, y_high_pix - y_low_pix,
	flow_eDrawType_Line, 0, highlight);
    else
      nc->nav_draw( &pos, highlight, (void *)this);
  }
}

void FlowNode::nav_erase() 
{ 
  int x_right_pix = int( x_right * ctx->nav_zoom_factor - ctx->nav_offset_x);
  int x_left_pix = int( x_left * ctx->nav_zoom_factor - ctx->nav_offset_x);
  int y_high_pix = int( y_high * ctx->nav_zoom_factor - ctx->nav_offset_y);
  int y_low_pix = int( y_low * ctx->nav_zoom_factor - ctx->nav_offset_y);

  if ( x_right_pix - x_left_pix < 10 && y_high_pix - y_low_pix < 10)
    ctx->fdraw->nav_rect_erase( ctx, x_left_pix, y_low_pix, 
	x_right_pix - x_left_pix, y_high_pix - y_low_pix,
	0);
  else
    nc->nav_erase( &pos, (void *)this);
}

void FlowNode::set_highlight( int on)
{
  highlight = on;
  if ( !inverse)
    nc->draw( &pos, highlight, hot, (void *)this);
  else
    draw_inverse();
  nc->nav_draw( &pos, highlight, (void *)this);
}

void FlowNode::set_hot( int on)
{
  if ( hot != on)
  {
    if ( hot && !inverse)
      nc->erase( &pos, hot, (void *)this);
    hot = on;
    if ( !inverse)
      nc->draw( &pos, highlight, hot, (void *)this);
    else
      draw_inverse();
    nc->nav_draw( &pos, highlight, (void *)this);
  }
}

void FlowNode::set_inverse( int on)
{
  inverse = on;

  if ( !inverse)
  {
    double x_r;
    if ( ctx->type() == flow_eCtxType_Brow)
      x_r = ((BrowCtx *)ctx)->frame_x_right;
    else
      x_r = x_right;

    if ( nc->group == flow_eNodeGroup_Document) {
      ctx->fdraw->fill_rect( ctx,
       	int( obst_x_left * ctx->zoom_factor - ctx->offset_x),
       	int( obst_y_low * ctx->zoom_factor - ctx->offset_y),
	int( obst_x_right * ctx->zoom_factor - obst_x_left * ctx->zoom_factor),
       	int( obst_y_high * ctx->zoom_factor - obst_y_low * ctx->zoom_factor),
	flow_eDrawType_LineErase);
      nc->draw( &pos, highlight, hot, (void *)this);
    }
    else {
      ctx->fdraw->fill_rect( ctx,
       	int( x_left * ctx->zoom_factor - ctx->offset_x),
       	int( y_low * ctx->zoom_factor - ctx->offset_y),
	int( x_r * ctx->zoom_factor - x_left * ctx->zoom_factor),
       	int( y_high * ctx->zoom_factor - y_low * ctx->zoom_factor + 1),
	flow_eDrawType_LineErase);
      nc->draw( &pos, highlight, hot, (void *)this);
    }
  }
  else
  {
    draw_inverse();
  }
}

void FlowNode::select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y)
{
  if ( ctx->select_policy == flow_eSelectPolicy_Surround || 
       nc->group == flow_eNodeGroup_Document)
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

void FlowNode::set_annotation( int num, const char *text, int size, int nodraw)
{

  if ( !nodraw)
  {
    if ( relative_annot_pos)
      erase();
    else
      nc->erase_annotation( &pos, highlight, hot, (void *) this, num);
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
  if ( nodraw)
    return;

  if ( relative_annot_pos)
  {
    relative_annot_x = 0;
    nc->configure_annotations( &pos, (void *)this);
    if ( !inverse)
      nc->draw( &pos, highlight, hot, (void *)this);
    else
      draw_inverse();
  }
  else
  {
    nc->draw_annotation( &pos, highlight, hot, (void *) this, num);
    if ( inverse)
      draw_inverse();
  }

  nc->nav_draw( &pos, highlight, (void *)this);

}

void FlowNode::get_annotation( int num, char *text, int size)
{
  if ( !annotv[num])
    strcpy( text, "");
  else
  {
    strncpy( text, annotv[num], size);
    text[size-1] = 0;
  }
}

void FlowNode::open_annotation_input( int num)
{
  if ( annotv_inputmode[num])
    return;

  annotv_inputmode[num] = 1;
  nc->open_annotation_input( &pos, this, num);

  if ( ctx->type() == flow_eCtxType_Brow)
    ((BrowCtx *)ctx)->configure( y_low);
  ctx->widget_cnt++;
}

void FlowNode::close_annotation_input( int num)
{
  if ( !annotv_inputmode[num])
    return;

  nc->close_annotation_input( this, num);
  annotv_inputmode[num] = 0;
  ctx->widget_cnt--;

  if ( ctx->type() == flow_eCtxType_Brow)
    ((BrowCtx *)ctx)->configure( y_low);
}

int FlowNode::get_annotation_input( int num, char **text)
{
  if ( !annotv_inputmode[num])
    return 0;

  return nc->get_annotation_input( this, num, text);
}

void FlowNode::measure_annotation( int num, char *text, double *width, 
	double *height)
{
  nc->measure_annotation( num, text, width, height);
}

void FlowNode::set_annot_pixmap( int num, flow_sAnnotPixmap *pixmap, 
	int nodraw)
{
  annotpixmapv[num] = pixmap;
  if ( relative_annot_pos)
  {
    relative_annot_x = 0;
    nc->configure_annotations( &pos, (void *)this);
  }

  if ( nodraw)
    return;

  if ( !inverse)
    nc->draw( &pos, highlight, hot, (void *)this);
  else
    draw_inverse();
  nc->nav_draw( &pos, highlight, (void *)this);

}

void FlowNode::remove_annot_pixmap( int num)
{

  nc->erase( &pos, hot, (void *)this);
  nc->nav_erase( &pos, (void *)this);

  annotpixmapv[num] = 0;

  if ( !inverse)
    nc->draw( &pos, highlight, hot, (void *)this);
  else
    draw_inverse();
  nc->nav_draw( &pos, highlight, (void *)this);

}

void FlowNode::set_radiobutton( int num, int value, int nodraw)
{
  rbuttonv[num] = value;

  if ( nodraw)
    return;

  if ( !inverse)
    nc->draw( &pos, highlight, hot, (void *)this);
  else
    draw_inverse();
  nc->nav_draw( &pos, highlight, (void *)this);
}

void FlowNode::get_radiobutton( int num, int *value)
{
  *value = rbuttonv[num];
}

void FlowNode::conpoint_refcon_reconfig( int conpoint)
{
//  ctx->conpoint_refcon_erase( this, conpoint);
  refcon_cnt[conpoint] = 0;
  ctx->conpoint_refcon_redraw( this, conpoint);
}

int FlowNode::event_handler( flow_eEvent event, int x, int y)
{
  int sts;
  double distance;
  FlowConPoint *conpoint;

  sts = 0;
  if ( event == ctx->event_move_node)
  {
    sts = nc->event_handler( &pos, event, x, y, (void *)this);
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
    sts = nc->event_handler( &pos, event, x, y, (void *)this);
    if ( sts)
    {
      /* Find closest conpoint */
      distance = 1e10;
      conpoint = 0;
      nc->a.conpoint_select( &pos, x, y, &distance, 
			     (void **) &conpoint);
      if ( conpoint) {
        ctx->con_create_source( this, conpoint->number, 
				pos.z_x - ctx->offset_x + conpoint->p.z_x, 
				pos.z_y - ctx->offset_y + conpoint->p.z_y);
      }
    }
    return sts;
  }
  else if ( event == ctx->event_select_conpoint)
  {
    sts = nc->event_handler( &pos, event, x, y, (void *)this);
    if ( sts)
    {
      /* Find closest conpoint */
      distance = 1e10;
      conpoint = 0;
      nc->a.conpoint_select( &pos, x, y, &distance, 
			     (void **) &conpoint);
      if ( conpoint) {
        ctx->conpoint_select( this, conpoint->number);
      }
    }
    return sts;
  }
  switch ( event)
  {
    case flow_eEvent_ButtonRelease:
      if ( ctx->con_create_active)
      {
        sts = nc->event_handler( &pos, event, x, y, (void *)this);
        if ( sts)
        {
          /* Find closest conpoint */
          distance = 1e10;
          conpoint = 0;
          nc->a.conpoint_select( &pos, x, y, &distance, 
		(void **) &conpoint);
          if ( conpoint)
          {
            ctx->con_create_dest( this, conpoint->number, event, x, y);
          }
        }
      }
      break;
    case flow_eEvent_CursorMotion:
    {
      int redraw = 0;

      sts = nc->event_handler( &pos, event, x, y, (void *)this);
      if ( sts && !hot  && 
	   !(ctx->node_movement_active || ctx->node_movement_paste_active))
      {
        ctx->fdraw->set_cursor( ctx, draw_eCursor_CrossHair);
        hot = 1;
        redraw = 1;
	ctx->tiptext_event( this, x, y);
      }
      if ( !sts && hot)
      {
        ctx->fdraw->set_cursor( ctx, draw_eCursor_Normal);
        erase();
        hot = 0;
        redraw = 1;
	ctx->tiptext->remove_text( this);
      }
      if ( redraw)
      {
        if ( !inverse)
          nc->draw( &pos, highlight, hot, (void *)this);
        else
          draw_inverse();
      }
      break;
    }
    default:
      sts = nc->event_handler( &pos, event, x, y, (void *)this);
      if ( sts == FLOW__DESTROYED)
        return sts;
  }
  if ( sts && sts != FLOW__NO_PROPAGATE)
    ctx->register_callback_object( flow_eObjectType_Node, this);
  return sts;
}

void FlowNode::remove_notify()
{
  ctx->delete_node_cons( this);
}

void FlowNode::set_trace_attr( const char *object, const char *attribute, 
			       flow_eTraceType type, int inverted)
{
  strncpy( trace_object, object, sizeof( trace_object)); 
  strncpy( trace_attribute, attribute, sizeof( trace_attribute));
  trace_attr_type = type;
  trace_inverted = inverted;

  if ( ctx->trace_started && strcmp( trace_object, "") != 0)
    ctx->trace_connect_func( (void *) this, trace_object, trace_attribute, 
	trace_attr_type, &trace_p);
}

void FlowNode::get_trace_attr( char *object, char *attribute, 
	flow_eTraceType *type, int *inverted)
{
  strncpy( object, trace_object, sizeof( trace_object)); 
  strncpy( attribute, trace_attribute, sizeof( trace_attribute));
  *type = trace_attr_type;
  *inverted = trace_inverted;
}

void FlowNode::trace_scan()
{
  int on;
  char txt[80];

  if ( !trace_p)
    return;

  if ( ctx->trace_scan_func)
  {
    ctx->trace_scan_func( (void *) this, trace_p);
    return;
  }    

  switch( trace_attr_type)
  {
    case flow_eTraceType_Boolean:
      on = trace_inverted ? *(unsigned int *) trace_p == 0 : *(unsigned int *) trace_p != 0;
      if ( highlight != on)
        set_highlight( on);
      if ( nc->group == flow_eNodeGroup_Trace)
      {
        sprintf( txt, "%d", *(unsigned int *) trace_p);
        set_annotation( 0, txt, strlen(txt), 0);
      }
      break;
    case flow_eTraceType_Int32:
      sprintf( txt, "%d", *(unsigned int *) trace_p);
      set_annotation( 0, txt, strlen(txt), 0);
      break;
    case flow_eTraceType_Float32:
      sprintf( txt, "%f", *(float *) trace_p);
      set_annotation( 0, txt, strlen(txt), 0);
      break;
    default:
      break;
  }
}

int FlowNode::trace_init()
{
  int sts;

  if ( strcmp( trace_object, "") == 0)
    return 1;

  sts = ctx->trace_connect_func( (void *) this, trace_object, trace_attribute, 
	trace_attr_type, &trace_p);
  return sts;
}

void FlowNode::trace_close()
{
  if ( strcmp( trace_object, "") == 0)
    return;

  ctx->trace_disconnect_func( (void *) this);
}

void FlowNode::get_object_name( char *name)
{
  strcpy( name, n_name);
}

//
// Configure the brow ctx
//
void FlowNode::configure( void *previous)
{
  double x, y;

  if ( !previous)
  {
    x = 0;
    y = 0;
  }
  else
  {
    x = ((BrowCtx *)ctx)->indentation * level;
    y = ((FlowNode *) previous)->y_high /* + 1.0 / ctx->zoom_factor */;
  }

  pos.posit( x, y);
  x_left = y_low = 1e37;
  x_right = y_high = -1e37;
  get_node_borders();

  // Fix, double size if inputmode
  int am = 0;
  for ( int i = 0; i < 10; i++)
    am +=  annotv_inputmode[i];
  if (am)
    y_high += y_high - y_low;
}

void FlowNode::move_widgets( int x, int y)
{
  int am = 0;
  for ( int i = 0; i < 10; i++)
    am +=  annotv_inputmode[i];
  if (am)
    nc->move_widgets( this, x, y);
}

void FlowNode::get_borders()
{
  x_left = y_low = 1e37;
  x_right = y_high = -1e37;
  get_node_borders();
  if ( nc->group == flow_eNodeGroup_Document || nc->no_con_obstacle)
  {
    obst_x_left = obst_y_low = 1e37;
    obst_x_right = obst_y_high = -1e37;
    get_node_obstacle_borders();
  }
  else
  {
    obst_x_left = x_left;
    obst_x_right = x_right;
    obst_y_low = y_low;
    obst_y_high = y_high;
  }
}

void FlowNode::set_fillcolor( flow_eDrawType color)
{
  fill_color = color;
  ctx->draw( int(x_left * ctx->zoom_factor - ctx->offset_x - 20),
	     int(y_low * ctx->zoom_factor - ctx->offset_y - 20),
  	     int(x_right * ctx->zoom_factor - ctx->offset_x + 20),
	     int(y_high * ctx->zoom_factor - ctx->offset_y + 20));
  ctx->nav_draw( int(x_left * ctx->nav_zoom_factor - ctx->nav_offset_x - 1),
	     int(y_low * ctx->nav_zoom_factor - ctx->nav_offset_y - 1),
  	     int(x_right * ctx->nav_zoom_factor - ctx->nav_offset_x + 1),
	     int(y_high * ctx->nav_zoom_factor - ctx->nav_offset_y + 1));
}

void FlowNode::conpoint_select( int num)
{
  if ( sel_conpoint1 == num || sel_conpoint2 == num)
    return;
  if ( sel_conpoint1 == -1)
    sel_conpoint1 = num;
  else if ( sel_conpoint2 == -1)
    sel_conpoint2 = num;
  nc->draw( &pos, highlight, hot, (void *)this);
}

void FlowNode::conpoint_select_clear( int num)
{
  if ( sel_conpoint1 == num) {
    erase();
    sel_conpoint1 = -1;
    nc->draw( &pos, highlight, hot, (void *)this);
  }
  else if ( sel_conpoint2 == num) {
    erase();
    sel_conpoint2 = -1;
    nc->draw( &pos, highlight, hot, (void *)this);
  }
}

void FlowNode::conpoint_select_clear()
{
  if ( sel_conpoint1 != -1 || sel_conpoint2 != -1) {
    erase();
    sel_conpoint1 = -1;
    sel_conpoint2 = -1;
    nc->draw( &pos, highlight, hot, (void *)this);
  }
}

int FlowNode::get_next_conpoint( int cp_num, flow_eDirection dir, double x0, double y0, int *next_cp_num)
{
  return nc->get_next_conpoint( cp_num, dir, x0 - pos.x, y0 - pos.y, next_cp_num);
}
