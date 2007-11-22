/* 
 * Proview   $Id: flow_con.cpp,v 1.7 2007-11-22 08:51:50 claes Exp $
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


#include <iostream.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include "flow.h"
#include "flow_con.h"
#include "flow_conpoint.h"
#include "flow_draw.h"
#include "flow_array.h"
#include "flow_arrow.h"
#include "flow_text.h"
#include "flow_utils.h"
#include "flow_msg.h"

#define LINE_TABLE_SIZE 500
#define HV_LINE_ARRAY_SIZE 500
#define CON__ROUTE_FOUND 111
#define ROUTE_LOOP_MAX 10000
#define CON_EPSILON 1e-4

typedef enum {
	eState_No,
	eState_RightToLeft,
	eState_UpToLeft,
	eState_UpToRight,
	eState_LeftToLeft,
	eState_RightToRight,
	eState_DownToLeft,
	eState_DownToRight,
	eState_LeftToRight,
	eState_RightToUp,
	eState_RightToDown,
	eState_LeftToUp,
	eState_LeftToDown,
	eState_Success,
	eState_Failure,
	eState_Exit
	} route_eState;

typedef enum {
	eCorner_RightToUp,
	eCorner_RightToDown,
	eCorner_UpToLeft,
	eCorner_UpToRight,
	eCorner_DownToLeft,
	eCorner_DownToRight,
	eCorner_LeftToUp,
	eCorner_LeftToDown,
	eCorner_Sharp
	} con_eCorner;

typedef enum {
	eLineType_Vert,
	eLineType_Horiz
	} con_eLineType;

typedef struct {
	con_eLineType	start_type;
	con_tVertLines *vert[MAX_HVLINE];
	con_tHorizLines *horiz[MAX_HVLINE];
	double vert_x[MAX_HVLINE];
        double vert_y[MAX_HVLINE];
        double horiz_x[MAX_HVLINE];
        double horiz_y[MAX_HVLINE];
	int vert_cnt;
	int horiz_cnt;
	int complete;
	} con_tRouteLine;	

static con_tVertLines vert_line[HV_LINE_ARRAY_SIZE+1];
static con_tHorizLines horiz_line[HV_LINE_ARRAY_SIZE+1];
static int vert_line_cnt;
static int horiz_line_cnt;
static con_tRouteLine line_table[LINE_TABLE_SIZE+1];
static int line_table_cnt;
static double sort_dest_x;
static double sort_dest_y;
static FlowNode *sort_source;
static FlowNode *sort_dest;

void draw_line( FlowCtx *ctx, double x1, double y1, double x2, double y2);

#if 0
FlowCon::FlowCon( FlowCtx *flow_ctx, char *name, FlowConClass *con_class,
	FlowNode *source, FlowNode *dest, int source_cp, int dest_cp) :
	ctx(flow_ctx), cc(con_class),
	source_node(source), dest_node(dest), source_conpoint(source_cp),
	dest_conpoint(dest_cp), p_num(0), l_num(0), a_num(0), line_a(10,10), 
        arc_a(10,10), hot(0), highlight(0), arrow_num(0), arrow_a(1,1),
	ref_num(0), ref_a(4,4), temporary_ref(0), trace_p(NULL)
{
  double x1, y1, x2, y2;
  FlowLine *l1;  

  if ( !cc)
    return;
  strcpy( trace_object, "");
  source->get_conpoint( source_cp, &x1, &y1, &source_direction);
  dest->get_conpoint( dest_cp, &x2, &y2, &dest_direction);
  l1 = new FlowLine( ctx, x1, y1, x2, y2, cc->draw_type, cc->line_width);
  line_a.insert( l1);
  strcpy( c_name, name);
  get_con_borders();
  line_a.draw( &cc->zero, highlight, hot, NULL);
  line_a.nav_draw( &cc->zero, highlight, NULL);
}
#endif

FlowCon::FlowCon( FlowCtx *flow_ctx, char *name, FlowConClass *con_class,
	FlowNode *source, FlowNode *dest, int source_cp, int dest_cp, 
	int *rsts, int nodraw, int point_num, double *x_vect, double *y_vect) :
	ctx(flow_ctx), cc(con_class),
	dest_node(dest), source_node(source), dest_conpoint(dest_cp), source_conpoint(source_cp),
	p_num(point_num), l_num(0), a_num(0), arrow_num(0), ref_num(0),
	line_a(10,10), arc_a(10,10), arrow_a(1,1), ref_a(4,4), temporary_ref(0), 
	hot(0), highlight(0),
	movement_type(flow_eMoveType_Route),
	trace_attr_type(flow_eTraceType_Boolean), 
	trace_p(NULL)
{
  double src_x, src_y, dest_x, dest_y;
  FlowLine *l1;  
  FlowArc *a1;
  FlowArrow *arrow;
  int i;

  if ( !cc)
    return;

  strcpy( trace_object, "");
  strcpy( trace_attribute, "");
  if ( x_vect && y_vect)
  {
    for ( i = 0; i < point_num; i++)
    {
      point_x[i] = x_vect[i];
      point_y[i] = y_vect[i];
    }
  }
  *rsts = source->get_conpoint( source_cp, &src_x, &src_y, &source_direction);
  if ( EVEN(*rsts)) return;
  *rsts = dest->get_conpoint( dest_cp, &dest_x, &dest_y, &dest_direction);
  if ( EVEN(*rsts)) return;

  switch ( cc->con_type)
  {
    case flow_eConType_Straight:
      l1 = new FlowLine( ctx, src_x, src_y, dest_x, dest_y, cc->draw_type, cc->line_width);
      line_a.insert( l1);
      l_num = 1;
      nav_zoom();
      if ( nodraw)
        break;
      l1->draw( &cc->zero, highlight, hot, NULL);
      l1->nav_draw( &cc->zero, highlight, NULL);
      break;
    case flow_eConType_StraightOneArrow:
      l1 = new FlowLine( ctx, src_x, src_y, dest_x, dest_y, cc->draw_type, cc->line_width);
      line_a.insert( l1);
      l_num = 1;
      arrow = new FlowArrow( ctx, src_x, src_y, dest_x, dest_y, 
	cc->arrow_width, cc->arrow_length, cc->draw_type);
      arrow_a.insert( arrow);
      arrow_num = 1;
      nav_zoom();
      if ( nodraw)
        break;
      l1->draw( &cc->zero, highlight, hot, NULL);
      l1->nav_draw( &cc->zero, highlight, NULL);
      arrow->draw( &cc->zero, highlight, hot, NULL);
      arrow->nav_draw( &cc->zero, highlight, NULL);
      break;
    case flow_eConType_StepDiv:
    case flow_eConType_StepConv:
    case flow_eConType_TransDiv:
    case flow_eConType_TransConv:
      for ( i = 0; i < MAX_POINT-1; i++)
      {
        l1 = new FlowLine( ctx, 0,0,0,0, cc->draw_type, cc->line_width);
        line_a.insert( l1);
      }

      if ( nodraw)
        break;
      con_route_grafcet( cc->con_type, src_x, src_y, dest_x, dest_y);
      break;
    case flow_eConType_Fixed:
      if ( !point_num)
      {
        l1 = new FlowLine( ctx, src_x, src_y, dest_x, dest_y, cc->draw_type, cc->line_width);
        line_a.insert( l1);
        i = 1;
        l_num = 1;
      }
      else
      {
        l1 = new FlowLine( ctx, src_x, src_y, point_x[0], point_y[0], cc->draw_type, cc->line_width);
        line_a.insert( l1);
        for ( i = 0; i < point_num; i++)
        {
          if ( i != point_num - 1)
            l1 = new FlowLine(ctx, point_x[i], point_y[i], point_x[i+1], 
		point_y[i+1], cc->draw_type, cc->line_width);
          else
            l1 = new FlowLine(ctx, point_x[i], point_y[i], dest_x, dest_y,
		cc->draw_type, cc->line_width);
          line_a.insert( l1);
        }
        l_num = p_num - 1;
      }
      for ( ; i < 8; i++)
      {
        l1 = new FlowLine(ctx, 0,0,0,0, cc->draw_type, cc->line_width);
        line_a.insert( l1);
      }
      if ( nodraw)
        break;
      if ( cc->corner == flow_eCorner_Rounded)
        draw_routed_roundcorner( p_num, point_x, point_y);
      else
        draw_routed( p_num, point_x, point_y);
      break;
    case flow_eConType_AllFixed:
      if ( nodraw)
        break;
      if ( cc->corner == flow_eCorner_Rounded)
        draw_routed_roundcorner( p_num, point_x, point_y);
      else
        draw_routed( p_num, point_x, point_y);
      break;
    case flow_eConType_Routed:
      for ( i = 0; i < MAX_POINT-1; i++)
      {
        l1 = new FlowLine( ctx, 0,0,0,0, cc->draw_type, cc->line_width);
        line_a.insert( l1);
      }
      if ( cc->corner == flow_eCorner_Rounded)
      {
        for ( i = 0; i < MAX_POINT-2; i++)
        {
          a1 = new FlowArc(ctx, 0,0,0,0,0,0, cc->draw_type, cc->line_width);
          arc_a.insert( a1);
        }
      }
      l_num = 0;
      a_num = 0;
      if ( nodraw)
        break;
      if ( p_num && x_vect && y_vect)
      {
        if ( cc->corner == flow_eCorner_Rounded)
          draw_routed_roundcorner( p_num, point_x, point_y);
        else
          draw_routed( p_num, point_x, point_y);
      }
      else
      {
        *rsts = con_route( src_x, src_y, source_direction, dest_x, dest_y,
			   dest_direction);
        if ( EVEN(*rsts) && *rsts != 0) return;
        if ( *rsts == 0)
          temporary_ref  = 1;
        else
          temporary_ref  = 0;
      }
      break;
    case flow_eConType_Reference:
      break;
  }

  if ( temporary_ref || cc->con_type == flow_eConType_Reference)
  {
    FlowText *t1, *t2;
    FlowRect *r1, *r2;
    double text_x, text_y, rect_x, rect_y;
    char reftext[20];

    sprintf( reftext, "R%d", ctx->refcon_cnt++);
    switch ( source_direction)
    {
      case flow_eDirection_Center:
        rect_x = src_x - ctx->refcon_width / 2;
        rect_y = src_y - (source->refcon_cnt[source_cp] - 0.5) * ctx->refcon_height;
        break;
      case flow_eDirection_Right:
        rect_x = src_x + source->refcon_cnt[source_cp] * ctx->refcon_width;
        rect_y = src_y - ctx->refcon_height / 2;
        break;
      case flow_eDirection_Left:
        rect_x = src_x - (source->refcon_cnt[source_cp] + 1) * ctx->refcon_width;
        rect_y = src_y - ctx->refcon_height / 2;
        break;
      case flow_eDirection_Up:
        rect_x = src_x - ctx->refcon_width / 2;
        rect_y = src_y + source->refcon_cnt[source_cp] * ctx->refcon_height;
        break;
      case flow_eDirection_Down:
        rect_x = src_x - ctx->refcon_width / 2;
        rect_y = src_y - (source->refcon_cnt[source_cp] + 1) * ctx->refcon_height;
        break;
    }
    text_x = rect_x + 0.2 * ctx->refcon_width;
    text_y = rect_y + 0.8 * ctx->refcon_height;
    r1 = new FlowRect( ctx, rect_x, rect_y, ctx->refcon_width, 
		ctx->refcon_height, flow_eDrawType_Line, ctx->refcon_linewidth);
    t1 = new FlowText( ctx, reftext, text_x, text_y, 
		flow_eDrawType_TextHelveticaBold, ctx->refcon_textsize);
    ref_a.insert( r1);
    ref_a.insert( t1);

    switch ( dest_direction)
    {
      case flow_eDirection_Center:
        rect_x = dest_x - ctx->refcon_width / 2;
        rect_y = dest_y - (dest->refcon_cnt[dest_cp] - 0.5) * ctx->refcon_height;
        break;
      case flow_eDirection_Right:
        rect_x = dest_x + dest->refcon_cnt[dest_cp] * ctx->refcon_width;
        rect_y = dest_y - ctx->refcon_height / 2;
        break;
      case flow_eDirection_Left:
        rect_x = dest_x - (dest->refcon_cnt[dest_cp] + 1) * ctx->refcon_width;
        rect_y = dest_y - ctx->refcon_height / 2;
        break;
      case flow_eDirection_Up:
        rect_x = dest_x - ctx->refcon_width / 2;
        rect_y = dest_y + dest->refcon_cnt[dest_cp] * ctx->refcon_height;
        break;
      case flow_eDirection_Down:
        rect_x = dest_x - ctx->refcon_width / 2;
        rect_y = dest_y - (dest->refcon_cnt[dest_cp] + 1) * ctx->refcon_height;
        break;
    }
    text_x = rect_x + 0.2 * ctx->refcon_width;
    text_y = rect_y + 0.8 * ctx->refcon_height;
    r2 = new FlowRect( ctx, rect_x, rect_y, ctx->refcon_width, 
		ctx->refcon_height, flow_eDrawType_Line, ctx->refcon_linewidth);
    t2 = new FlowText( ctx, reftext, text_x, text_y, 
		flow_eDrawType_TextHelveticaBold, ctx->refcon_textsize);
    ref_a.insert( r2);
    ref_a.insert( t2);
    ref_num = 4;
    source_ref_cnt = source->refcon_cnt[source_cp]++;
    dest_ref_cnt = dest->refcon_cnt[dest_cp]++;
    nav_zoom();
    ref_a.draw( &cc->zero, highlight, hot, NULL);
    ref_a.nav_draw( &cc->zero, highlight, NULL);
  }

  strcpy( c_name, name);
  get_con_borders();
  *rsts = FLOW__SUCCESS;
}

FlowCon::~FlowCon()
{
  int i;

  ctx->object_deleted( this);
  if ( ctx->nodraw) return;

  for ( i = 0; i < l_num; i++)
  {
    ((FlowLine *)line_a[i])->erase( &cc->zero, hot, NULL);
    ((FlowLine *)line_a[i])->nav_erase( &cc->zero, NULL);
  }
  for ( i = 0; i < a_num; i++)
  {
    ((FlowArc *)arc_a[i])->erase( &cc->zero, hot, NULL);
    ((FlowArc *)arc_a[i])->nav_erase( &cc->zero, NULL);
  }
  arrow_a.erase( &cc->zero, hot, NULL);
  arrow_a.nav_erase( &cc->zero, NULL);
  ref_a.erase( &cc->zero, hot, NULL);
  ref_a.nav_erase( &cc->zero, NULL);

  ctx->remove(this);
  ctx->select_remove(this);

  if ( temporary_ref || cc->con_type == flow_eConType_Reference)
  {
    source_node->conpoint_refcon_reconfig( source_conpoint);
    dest_node->conpoint_refcon_reconfig( dest_conpoint);
  }
  ctx->draw( int(x_left * ctx->zoom_factor - ctx->offset_x - 20),
	     int(y_low * ctx->zoom_factor - ctx->offset_y - 20),
  	     int(x_right * ctx->zoom_factor - ctx->offset_x + 20),
	     int(y_high * ctx->zoom_factor - ctx->offset_y + 20));
  ctx->nav_draw( int(x_left * ctx->nav_zoom_factor - ctx->nav_offset_x - 1),
	     int(y_low * ctx->nav_zoom_factor - ctx->nav_offset_y - 1),
  	     int(x_right * ctx->nav_zoom_factor - ctx->nav_offset_x + 1),
	     int(y_high * ctx->nav_zoom_factor - ctx->nav_offset_y + 1));
  if ( hot)
    ctx->fdraw->set_cursor( ctx, draw_eCursor_Normal);
}

FlowCon::FlowCon( const FlowCon& c, FlowNode *source, FlowNode *dest)
{
  memcpy( this, &c, sizeof(c));
  source_node = source;
  dest_node = dest;

  line_a.new_array( c.line_a);
  arc_a.new_array( c.arc_a);
  arrow_a.new_array( c.arrow_a);
  ref_a.new_array( c.ref_a);
  line_a.copy_from( c.line_a);
  arc_a.copy_from( c.arc_a);
  arrow_a.copy_from( c.arrow_a);
  ref_a.copy_from( c.ref_a);
  if ( ref_a.size() > 0)
  {
    sprintf(((FlowText *)ref_a[1])->text, "R%d", ctx->refcon_cnt);
    sprintf(((FlowText *)ref_a[3])->text, "R%d", ctx->refcon_cnt++);
  }
}

void FlowCon::set_highlight( int on)
{
  int i;

  highlight = on;
  if ( temporary_ref || cc->con_type == flow_eConType_Reference)
  {
    ref_a.draw( &cc->zero, highlight, hot, NULL);
    ref_a.nav_draw( &cc->zero, highlight, NULL);
  }
  else
  {
    for ( i = 0; i < l_num; i++)
    {
      ((FlowLine *)line_a[i])->draw( &cc->zero, highlight, hot, NULL);
      ((FlowLine *)line_a[i])->nav_draw( &cc->zero, highlight, NULL);
    }
    for ( i = 0; i < a_num; i++)
    {
      ((FlowArc *)arc_a[i])->draw( &cc->zero, highlight, hot, NULL);
      ((FlowArc *)arc_a[i])->nav_draw( &cc->zero, highlight, NULL);
    }
    arrow_a.draw( &cc->zero, highlight, hot, NULL);
    arrow_a.nav_draw( &cc->zero, highlight, NULL);
  }
}

void FlowCon::set_hot( int on)
{
  int i;

  if ( hot != on)
  {
    hot = on;
    if ( temporary_ref || cc->con_type == flow_eConType_Reference)
    {
      ref_a.draw( &cc->zero, highlight, hot, NULL);
      ref_a.nav_draw( &cc->zero, highlight, NULL);
    }
    else
    {
      for ( i = 0; i < l_num; i++)
      {
        ((FlowLine *)line_a[i])->draw( &cc->zero, highlight, hot, NULL);
        ((FlowLine *)line_a[i])->nav_draw( &cc->zero, highlight, NULL);
      }
      for ( i = 0; i < a_num; i++)
      {
        ((FlowArc *)arc_a[i])->draw( &cc->zero, highlight, hot, NULL);
        ((FlowArc *)arc_a[i])->nav_draw( &cc->zero, highlight, NULL);
      }
      arrow_a.draw( &cc->zero, highlight, hot, NULL);
      arrow_a.nav_draw( &cc->zero, highlight, NULL);
    }
  }
}

void FlowCon::select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y)
{
  if ( temporary_ref || cc->con_type == flow_eConType_Reference) {
    // Allways surround
    if ( x_left > ll_x && x_right < ur_x && y_high < ur_y && y_low > ll_y)
      ctx->select_insert( this);
  }
  else {
    if ( ctx->select_policy == flow_eSelectPolicy_Surround) {
      if ( x_left > ll_x && x_right < ur_x && y_high < ur_y && y_low > ll_y)
	ctx->select_insert( this);
    }
    else {
      if ( x_right > ll_x && x_left < ur_x && y_low < ur_y && y_high > ll_y)
	ctx->select_insert( this);
    }
  }
}

void FlowCon::zoom() 
{
  int i;

  for ( i = 0; i < l_num; i++)
    ((FlowLine *)line_a[i])->zoom();
  for ( i = 0; i < a_num; i++)
    ((FlowLine *)arc_a[i])->zoom();
  arrow_a.zoom();
  ref_a.zoom();
}

void FlowCon::nav_zoom()
{
  int i;

  for ( i = 0; i < l_num; i++)
    ((FlowLine *)line_a[i])->nav_zoom();
  for ( i = 0; i < a_num; i++)
    ((FlowLine *)arc_a[i])->nav_zoom();
   arrow_a.nav_zoom();
   ref_a.nav_zoom();
}

void FlowCon::print_zoom()
{
  int i;

  for ( i = 0; i < l_num; i++)
    ((FlowLine *)line_a[i])->print_zoom();
  for ( i = 0; i < a_num; i++)
    ((FlowLine *)arc_a[i])->print_zoom();
  arrow_a.print_zoom();
  ref_a.print_zoom();
}

void FlowCon::redraw_node_cons( void *node)
{
  if ( source_node == (FlowNode *) node ||
       dest_node == (FlowNode *) node)
    reconfigure();
}

int FlowCon::delete_node_cons( void *node)
{
  if ( source_node == (FlowNode *) node ||
       dest_node == (FlowNode *) node)
  {
    delete this;
    return 1;
  }
  return 0;
}

void FlowCon::get_con_borders( )
{ 
  int i;

  x_right = -1e10;
  x_left = 1e10;
  y_high = -1e10;
  y_low = 1e10;
  if ( temporary_ref || cc->con_type == flow_eConType_Reference)
    ref_a.get_borders( 0, 0, &x_right, &x_left, &y_high, &y_low, NULL);
  else
  {
    for ( i = 0; i < l_num; i++)
      line_a[i]->get_borders( 0, 0, &x_right, &x_left, &y_high, &y_low, NULL);
    for ( i = 0; i < a_num; i++)
      arc_a[i]->get_borders( 0, 0, &x_right, &x_left, &y_high, &y_low, NULL);
 }
}

void FlowCon::move( int delta_x, int delta_y, int grid)
{
  double x, y;
  int i;

  x = delta_x / ctx->zoom_factor;
  y = delta_y / ctx->zoom_factor;

  if ( movement_type == flow_eMoveType_Route || grid)
    reconfigure();
  else
  {
    for ( i = 0; i < p_num; i++)
    {
      point_x[i] += x;
      point_y[i] += y;
    }
    if ( temporary_ref || cc->con_type == flow_eConType_Reference)
      ref_a.shift( &cc->zero, x, y, highlight, hot);
    else
    {
      for ( i = 0; i < l_num; i++)
        ((FlowLine *)line_a[i])->shift( &cc->zero, x, y, highlight, hot);
      for ( i = 0; i < a_num; i++)
        ((FlowArc *)arc_a[i])->shift( &cc->zero, x, y, highlight, hot);
      arrow_a.shift( &cc->zero, x, y, highlight, hot);
    }
    get_con_borders();
    ctx->draw( int(x_left * ctx->zoom_factor - ctx->offset_x - 20),
	     int(y_low * ctx->zoom_factor - ctx->offset_y - 20),
  	     int(x_right * ctx->zoom_factor - ctx->offset_x + 20),
	     int(y_high * ctx->zoom_factor - ctx->offset_y + 20));
    ctx->nav_draw( int(x_left * ctx->nav_zoom_factor - ctx->nav_offset_x - 1),
	     int(y_low * ctx->nav_zoom_factor - ctx->nav_offset_y - 1),
  	     int(x_right * ctx->nav_zoom_factor - ctx->nav_offset_x + 1),
	     int(y_high * ctx->nav_zoom_factor - ctx->nav_offset_y + 1));
  }
}

void FlowCon::move_noerase( int delta_x, int delta_y, int grid)
{
  double x, y;
  int i;

  x = delta_x / ctx->zoom_factor;
  y = delta_y / ctx->zoom_factor;

  if ( cc->con_type != flow_eConType_Routed ||
       movement_type == flow_eMoveType_Route || grid || p_num == 0)
  {
    ctx->set_nodraw();
    reconfigure();
    ctx->reset_nodraw();
  }
  else
  {
    for ( i = 0; i < p_num; i++)
    {
      point_x[i] += x;
      point_y[i] += y;
    }
    l_num = a_num = 0;
    if ( cc->corner == flow_eCorner_Rounded)
      draw_routed_roundcorner( p_num, point_x, point_y);
    else
      draw_routed( p_num, point_x, point_y);
    get_con_borders();
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

void FlowCon::reconfigure()
{
  double x1, y1, x2, y2;
  FlowLine *l1;
  FlowArrow *arrow;
  flow_eDirection dir1, dir2;
  int	sts, i;

  sts = source_node->get_conpoint( source_conpoint, &x1, &y1, &dir1);
  if (EVEN(sts))
  {
    cout << "FlowCon:no such conpoint" << endl;
    return;
  }
  sts = dest_node->get_conpoint( dest_conpoint, &x2, &y2, &dir2);
  if (EVEN(sts))
  {
    cout << "FlowCon:no such conpoint" << endl;
    return;
  }

  switch ( cc->con_type)
  {
    case flow_eConType_Straight:
      l1 = (FlowLine *)line_a[0];
      l1->move( &cc->zero, x1, y1, x2, y2, highlight, hot);
      break;
    case flow_eConType_Reference:
    {
      move_ref( x1, y1, x2, y2);
      break;
    }
    case flow_eConType_StraightOneArrow:
      l1 = (FlowLine *)line_a[0];
      l1->move( &cc->zero, x1, y1, x2, y2, highlight, hot);
      arrow = (FlowArrow *)arrow_a[0];
      arrow->move(  &cc->zero, x1, y1, x2, y2, highlight, hot);
      break;
    case flow_eConType_StepDiv:
    case flow_eConType_StepConv:
    case flow_eConType_TransDiv:
    case flow_eConType_TransConv:
      con_route_grafcet( cc->con_type, x1, y1, x2, y2);
      break;
    case flow_eConType_Fixed:
      if ( !p_num)
      {
        l1 = (FlowLine *)line_a[0];
        l1->move( &cc->zero, x1, y1, x2, y2, highlight, hot);
      }
      else
      {
        l1 = (FlowLine *)line_a[0];
        l1->move( &cc->zero, x1, y1, point_x[0], point_y[0], highlight, hot);
        l1 = (FlowLine *)line_a[p_num];
        l1->move( &cc->zero, point_x[p_num-1], point_y[p_num-1], x2, y2, 
		highlight, hot);
      }
      break;
    case flow_eConType_AllFixed:
      break;
    case flow_eConType_Routed:
      sts = con_route( x1, y1, dir1, x2, y2, dir2);
      if ( sts == 0)
      {
        if ( !temporary_ref)
        {
          for ( i = 0; i < l_num; i++)
          {
            ((FlowLine *)line_a[i])->erase( &cc->zero, hot, NULL);
            ((FlowLine *)line_a[i])->nav_erase( &cc->zero, NULL);
          }
          for ( i = 0; i < a_num; i++)
          {
            ((FlowArc *)arc_a[i])->erase( &cc->zero, hot, NULL);
            ((FlowArc *)arc_a[i])->nav_erase( &cc->zero, NULL);
          }
          temporary_ref = 1;
          a_num = l_num = p_num = 0;
          source_ref_cnt = source_node->refcon_cnt[source_conpoint]++;
          dest_ref_cnt = dest_node->refcon_cnt[dest_conpoint]++;
        }
        move_ref( x1, y1, x2, y2);
      }
      else if ( temporary_ref)
      {
        temporary_ref = 0;
        ref_a.erase( &cc->zero, hot, NULL);
        ref_a.nav_erase( &cc->zero, NULL);
        source_node->conpoint_refcon_reconfig( source_conpoint);
        dest_node->conpoint_refcon_reconfig( dest_conpoint);
      }
      break;
  }
  get_con_borders();
  ctx->draw( int(x_left * ctx->zoom_factor - ctx->offset_x - 20),
	     int(y_low * ctx->zoom_factor - ctx->offset_y - 20),
  	     int(x_right * ctx->zoom_factor - ctx->offset_x + 20),
	     int(y_high * ctx->zoom_factor - ctx->offset_y + 20));
  ctx->nav_draw( int(x_left * ctx->nav_zoom_factor - ctx->nav_offset_x - 1),
	         int(y_low * ctx->nav_zoom_factor - ctx->nav_offset_y - 1),
  	         int(x_right * ctx->nav_zoom_factor - ctx->nav_offset_x + 1),
                 int(y_high * ctx->nav_zoom_factor - ctx->nav_offset_y + 1));
}

void FlowCon::print( double ll_x, double ll_y, double ur_x, double ur_y) 
{ 
  double tmp;
  int i;

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
  {
    if ( temporary_ref || cc->con_type == flow_eConType_Reference)
      ref_a.print( &cc->zero, NULL, highlight);
    else
    {
      for ( i = 0; i < l_num; i++)
        ((FlowLine *)line_a[i])->print( &cc->zero, NULL, highlight);
      for ( i = 0; i < a_num; i++)
        ((FlowArc *)arc_a[i])->print( &cc->zero, NULL, highlight);
      arrow_a.print( &cc->zero, NULL, highlight);
    }
  }
}

void FlowCon::save( ofstream& fp, flow_eSaveMode mode) 
{ 
  int i;

  if ( (mode == flow_eSaveMode_Trace && cc->group != flow_eConGroup_Trace) ||
       (mode == flow_eSaveMode_Edit && cc->group == flow_eConGroup_Trace) )
    return;

  fp << int(flow_eSave_Con) << endl;
  fp << int(flow_eSave_Con_x_right) << FSPACE << x_right << endl;
  fp << int(flow_eSave_Con_x_left) << FSPACE << x_left << endl;
  fp << int(flow_eSave_Con_y_high) << FSPACE << y_high << endl;
  fp << int(flow_eSave_Con_y_low) << FSPACE << y_low << endl;
  fp << int(flow_eSave_Con_cc) << FSPACE << cc->cc_name << endl;
  fp << int(flow_eSave_Con_dest_node) << FSPACE << dest_node->n_name << endl;
  fp << int(flow_eSave_Con_source_node) << FSPACE << source_node->n_name << endl;
  fp << int(flow_eSave_Con_dest_conpoint) << FSPACE << dest_conpoint << endl;
  fp << int(flow_eSave_Con_source_conpoint) << FSPACE << source_conpoint << endl;
  fp << int(flow_eSave_Con_dest_direction) << FSPACE << int(dest_direction) << endl;
  fp << int(flow_eSave_Con_source_direction) << FSPACE << int(source_direction) << endl;
  fp << int(flow_eSave_Con_line_a) << endl;
  line_a.save( fp, mode);
  fp << int(flow_eSave_Con_arc_a) << endl;
  arc_a.save( fp, mode);
  fp << int(flow_eSave_Con_arrow_a) << endl;
  arrow_a.save( fp, mode);
  fp << int(flow_eSave_Con_ref_a) << endl;
  ref_a.save( fp, mode);
  fp << int(flow_eSave_Con_p_num) << FSPACE << p_num << endl;
  fp << int(flow_eSave_Con_l_num) << FSPACE << l_num << endl;
  fp << int(flow_eSave_Con_a_num) << FSPACE << a_num << endl;
  fp << int(flow_eSave_Con_arrow_num) << FSPACE << arrow_num << endl;
  fp << int(flow_eSave_Con_ref_num) << FSPACE << ref_num << endl;
  fp << int(flow_eSave_Con_point_x) << endl;
  for ( i = 0; i < p_num; i++)
    fp << point_x[i] << endl;
  fp << int(flow_eSave_Con_point_y) << endl;
  for ( i = 0; i < p_num; i++)
    fp << point_y[i] << endl;
  fp << int(flow_eSave_Con_source_ref_cnt) << FSPACE << source_ref_cnt << endl;
  fp << int(flow_eSave_Con_dest_ref_cnt) << FSPACE << dest_ref_cnt << endl;
  fp << int(flow_eSave_Con_c_name) << FSPACE << c_name << endl;
  fp << int(flow_eSave_Con_trace_object) << FSPACE << trace_object << endl;
  fp << int(flow_eSave_Con_trace_attribute) << FSPACE << trace_attribute << endl;
  fp << int(flow_eSave_Con_trace_attr_type) << FSPACE << int(trace_attr_type) << endl;
  fp << int(flow_eSave_Con_temporary_ref) << FSPACE << temporary_ref << endl;
  fp << int(flow_eSave_End) << endl;  
}

void FlowCon::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  char		name[80];
  int		i;
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_Con: break;
      case flow_eSave_Con_cc:
        fp.get();
        fp.getline( name, sizeof(name));
        cc = (FlowConClass *) ctx->get_conclass_from_name( name);
        if ( !cc)
          cout << "FlowCon:conclass not found" << endl;
        break;
      case flow_eSave_Con_c_name:
        fp.get();
        fp.getline( c_name, sizeof(c_name));
        break;
      case flow_eSave_Con_x_right: fp >> x_right; break;
      case flow_eSave_Con_x_left: fp >> x_left; break;
      case flow_eSave_Con_y_high: fp >> y_high; break;
      case flow_eSave_Con_y_low: fp >> y_low; break;
      case flow_eSave_Con_dest_node:
        fp.get();
        fp.getline( name, sizeof(name));
        dest_node = (FlowNode *) ctx->get_node_from_name( name);
        if ( !dest_node)
          cout << "FlowCon:dest_node not found" << endl;
        break;
      case flow_eSave_Con_source_node:
        fp.get();
        fp.getline( name, sizeof(name));
        source_node = (FlowNode *) ctx->get_node_from_name( name);
        if ( !source_node)
          cout << "FlowCon:source_node not found" << endl;
        break;
      case flow_eSave_Con_dest_conpoint: fp >> dest_conpoint; break;
      case flow_eSave_Con_source_conpoint: fp >> source_conpoint; break;
      case flow_eSave_Con_dest_direction: fp >> tmp; dest_direction = (flow_eDirection)tmp; break;
      case flow_eSave_Con_source_direction: fp >> tmp; source_direction = (flow_eDirection)tmp; break;
      case flow_eSave_Con_line_a: line_a.open( ctx, fp); break;
      case flow_eSave_Con_arc_a: arc_a.open( ctx, fp); break;
      case flow_eSave_Con_arrow_a: arrow_a.open( ctx, fp); break;
      case flow_eSave_Con_ref_a: ref_a.open( ctx, fp); break;
      case flow_eSave_Con_p_num: fp >> p_num; break;
      case flow_eSave_Con_l_num: fp >> l_num; break;
      case flow_eSave_Con_a_num: fp >> a_num; break;
      case flow_eSave_Con_arrow_num: fp >> arrow_num; break;
      case flow_eSave_Con_ref_num: fp >> ref_num; break;
      case flow_eSave_Con_point_x:
        for ( i = 0; i < p_num; i++)
          fp >> point_x[i];
        break;
      case flow_eSave_Con_point_y:
        for ( i = 0; i < p_num; i++)
          fp >> point_y[i];
        break;
      case flow_eSave_Con_source_ref_cnt: fp >> source_ref_cnt; break;
      case flow_eSave_Con_dest_ref_cnt: fp >> dest_ref_cnt; break;
      case flow_eSave_Con_trace_object:
        fp.get();
        fp.getline( trace_object, sizeof(trace_object));
        break;
      case flow_eSave_Con_trace_attribute:
        fp.get();
        fp.getline( trace_attribute, sizeof(trace_attribute));
        break;
      case flow_eSave_Con_trace_attr_type: fp >> tmp; trace_attr_type = (flow_eTraceType)tmp; break;
      case flow_eSave_Con_temporary_ref: fp >> temporary_ref; break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowCon:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowCon::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int tmp;
  int i;

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
    if ( temporary_ref || cc->con_type == flow_eConType_Reference)
      ref_a.draw( &cc->zero, highlight, hot, NULL);
    else
    {
      for ( i = 0; i < l_num; i++)
        ((FlowLine *)line_a[i])->draw( &cc->zero, highlight, hot, NULL);
      for ( i = 0; i < a_num; i++)
        ((FlowArc *)arc_a[i])->draw( &cc->zero, highlight, hot, NULL);
      arrow_a.draw( &cc->zero, highlight, hot, NULL);
    }
  }
}

void FlowCon::nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  int tmp;
  int i;

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
 
  if ( x_right * ctx->nav_zoom_factor - ctx->nav_offset_x >= ll_x &&
       x_left * ctx->nav_zoom_factor - ctx->nav_offset_x <= ur_x &&
       y_high * ctx->nav_zoom_factor - ctx->nav_offset_y >= ll_y &&
       y_low * ctx->nav_zoom_factor - ctx->nav_offset_y <= ur_y)
  {
    if ( temporary_ref || cc->con_type == flow_eConType_Reference)
      ref_a.nav_draw( &cc->zero, highlight, NULL);
    else
    {
      for ( i = 0; i < l_num; i++)
        ((FlowLine *)line_a[i])->nav_draw( &cc->zero, highlight, NULL);
      for ( i = 0; i < a_num; i++)
        ((FlowArc *)arc_a[i])->nav_draw( &cc->zero, highlight, NULL);
      arrow_a.nav_draw( &cc->zero, highlight, NULL);
    }
  }
}

int FlowCon::con_route_noobstacle( double src_x, double src_y, flow_eDirection src_dir,
			double dest_x, double dest_y, flow_eDirection dest_dir)
{
  double x[8], y[8];
  int	point;
  route_eState	state;

  point = 0;
  x[0] = src_x;
  y[0] = src_y;
  point++;
  if ( src_dir == flow_eDirection_Right && dest_dir == flow_eDirection_Left)
    state = eState_RightToLeft;
  else if ( src_dir == flow_eDirection_Left && dest_dir == flow_eDirection_Right)
    state = eState_LeftToRight;
  else if ( src_dir == flow_eDirection_Left && dest_dir == flow_eDirection_Left)
    state = eState_LeftToLeft;
  else if ( src_dir == flow_eDirection_Right && dest_dir == flow_eDirection_Right)
    state = eState_RightToRight;

  for (;;)
  {
    switch( state)
    {
      case eState_RightToLeft:
        /* Right to Left */
        if ( x[point-1] < dest_x)
        {
          x[point] = (x[point-1] + dest_x) / 2;
          y[point] = y[point-1];
          x[point+1] = x[point];
          y[point+1] = dest_y;
	  x[point+2] = dest_x;
	  y[point+2] = dest_y;
          point += 3;
          state = eState_Success;
        }
        else
        {
          x[point] = x[point-1] + 1;
          y[point] = y[point-1];
          point++;
          if ( y[point-1] < dest_y)
            state = eState_UpToLeft;	// Up to left
          else
            state = eState_DownToLeft;	// Down to left
        }
        break;
      case eState_UpToLeft:
        /* Up to Left */
        if ( y[point-1] > dest_y)
        {
          x[point] = x[point-1];
          y[point] = y[point-1] + 1;
          point++;
          if ( x[point-1] > dest_x)
            state = eState_LeftToLeft;
          else
            state = eState_RightToLeft;
        }
        else
        {
          if ( x[point-1] < dest_x)
          {
            x[point] = x[point-1];
            y[point] = dest_y;
	    x[point+1] = dest_x;
	    y[point+1] = dest_y;
            point += 2;       
            state = eState_Success;
          }
          else
          {
            x[point] = x[point-1];
            y[point] = y[point-1] + 1;
            point++;
            state = eState_LeftToLeft;	// Left to left
          } 
        }
        break;
      case eState_UpToRight:
        /* Up to Right */
        if ( y[point-1] > dest_y)
        {
          x[point] = x[point-1];
          y[point] = y[point-1] + 1;
          if ( x[point-1] < dest_x)
            state = eState_RightToRight;
          else
            state = eState_LeftToRight;
          point++;
        }
        else
        {
          if ( x[point-1] > dest_x)
          {
            x[point] = x[point-1];
            y[point] = dest_y;
	    x[point+1] = dest_x;
	    y[point+1] = dest_y;
            point += 2;       
            state = eState_Success;
          }
          else
          {
            x[point] = x[point-1];
            y[point] = y[point-1] + 1;
            point++;
            state = eState_RightToRight;	// Right to Right
          }
        }
        break;
      case eState_LeftToLeft:
        /* Left to left */
        if ( x[point-1] > dest_x)
        {
          x[point] = dest_x - 1;
	  y[point] = y[point-1];
	  point++;
        }
        else
        {
          x[point] = x[point-1] - 1;
          y[point] = y[point-1];
	  point++;
        }
	if ( y[point-1] < dest_y)
          state = eState_UpToLeft;	// Up to left
        else
          state = eState_DownToLeft;	// Down to left
        break;
      case eState_RightToRight:
        /* Right to right */
        if ( x[point-1] < dest_x)
        {
          x[point] = dest_x + 1;
	  y[point] = y[point-1];
	  point++;
        }
        else
        {
          x[point] = x[point-1] + 1;
          y[point] = y[point-1];
	  point++;
        }
	if ( y[point-1] < dest_y)
          state = eState_UpToRight;	// Up to right
        else
          state = eState_DownToRight;	// Down to right
        break;
      case eState_DownToLeft:
        /* Down to left */
        if ( y[point-1] < dest_y)
        {
          x[point] = x[point-1];
          y[point] = y[point-1] - 1;
          point++;
          if ( x[point-1] > dest_x)
            state = eState_LeftToLeft;
          else
            state = eState_RightToLeft;
        }
        else
        {
          if ( x[point-1] < dest_x)
          {
            x[point] = x[point-1];
            y[point] = dest_y;
	    x[point+1] = dest_x;
	    y[point+1] = dest_y;
            point += 2;       
            state = eState_Success;
          }
          else
          {
            x[point] = x[point-1];
            y[point] = y[point-1] - 1;
            point++;
            state = eState_LeftToLeft;	// Left to left
          } 
        }
        break;
      case eState_DownToRight:
        /* Down to right */
        if ( y[point-1] < dest_y)
        {
          x[point] = x[point-1];
          y[point] = y[point-1] - 1;
          point++;
          if ( y[point-1] < dest_y)
            state = eState_RightToRight;
          else
            state = eState_LeftToRight;
        }
        else
        {
          if ( x[point-1] > dest_x)
          {
            x[point] = x[point-1];
            y[point] = dest_y;
	    x[point+1] = dest_x;
	    y[point+1] = dest_y;
            point += 2;       
            state = eState_Success;
          }
          else
          {
            x[point] = x[point-1];
            y[point] = y[point-1] - 1;
            point++;
            state = eState_RightToRight;	// Right to right
          } 
        } 
        break;
      case eState_LeftToRight:
        /* Left to Right */
        if ( x[point-1] > dest_x)
        {
          x[point] = (x[point-1] + dest_x) / 2;
          y[point] = y[point-1];
          x[point+1] = x[point];
          y[point+1] = dest_y;
	  x[point+2] = dest_x;
	  y[point+2] = dest_y;
          point += 3;
          state = eState_Success;
        }
        else
        {
          x[point] = x[point-1] - 1;
          y[point] = y[point-1];
          point++;
          if ( y[point-1] < dest_y)
            state = eState_UpToRight;	// Up to right
          else
            state = eState_DownToRight;	// Down to right
        }
        break;
      case eState_Success:
        /* Routing found */
        if ( cc->corner == flow_eCorner_Rounded)
          draw_routed_roundcorner( point, x, y);
        else
          draw_routed( point, x, y);

        state = eState_Exit;
        break;
      default :
        cout << "Unknown state" << endl;
        state = eState_Exit;
    }
    if ( state == eState_Exit)
      break;
    if ( point > 9)
    {
      /* Max number of points exceeded */
      point = 8;
      state = eState_Success;
    }
  }
//  printf("route : %d\n", point);
  return 1;
}

int FlowCon::con_route_grafcet( flow_eConType con_type, 
		double src_x, double src_y, double dest_x, double dest_y)
{
  double x[8], y[8];
  int point;
  int i;

  switch( con_type)
  {
    case flow_eConType_StepDiv:

      if ( src_y + ctx->grafcet_con_delta < dest_y )
      {
        point = 0;
        x[point] = src_x;
        y[point++] = src_y;
        x[point] = src_x;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y;
      }
      else if ( src_x > dest_x)
      {
        point = 0;
        x[point] = src_x;
        y[point++] = src_y;
        x[point] = src_x;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = dest_x + 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = dest_x + 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y;
      }
      else
      {
        point = 0;
        x[point] = src_x;
        y[point++] = src_y;
        x[point] = src_x;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = dest_x - 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = dest_x - 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y;
      }
      p_num = point;
      draw_routed( point, x, y);
      break;
    case flow_eConType_StepConv:

      if ( dest_y - ctx->grafcet_con_delta > src_y )
      {
        point = 0;
        x[point] = dest_x;
        y[point++] = dest_y;
        x[point] = dest_x;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y;
      }
      else if ( dest_x > src_x)
      {
        point = 0;
        x[point] = dest_x;
        y[point++] = dest_y;
        x[point] = dest_x;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = src_x + 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = src_x + 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y;
      }
      else
      {
        point = 0;
        x[point] = dest_x;
        y[point++] = dest_y;
        x[point] = dest_x;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = src_x - 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = src_x - 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y;
      }
      p_num = point;
      draw_routed( point, x, y);
      break;
    case flow_eConType_TransDiv:

      if ( src_y + ctx->grafcet_con_delta < dest_y )
      {
        point = 0;
        x[point] = src_x;
        y[point++] = src_y;
        x[point] = src_x;
        y[point++] = src_y + 0.8 * ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = src_y + 0.8 * ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y + 1.2 * ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = src_y + 1.2 * ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y;
      }
      else if ( src_x > dest_x)
      {
        point = 0;
        x[point] = src_x;
        y[point++] = src_y;
        x[point] = src_x;
        y[point++] = src_y + 0.8 * ctx->grafcet_con_delta;
        x[point] = dest_x + 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + 0.8 * ctx->grafcet_con_delta;
        x[point] = dest_x + 2 * ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y + 1.2 * ctx->grafcet_con_delta;
        x[point] = dest_x + 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + 1.2 * ctx->grafcet_con_delta;
        x[point] = dest_x + 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y;
      }
      else
      {
        point = 0;
        x[point] = src_x;
        y[point++] = src_y;
        x[point] = src_x;
        y[point++] = src_y + 0.8 * ctx->grafcet_con_delta;
        x[point] = dest_x - 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + 0.8 * ctx->grafcet_con_delta;
        x[point] = dest_x - 2 * ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y + 1.2 * ctx->grafcet_con_delta;
        x[point] = dest_x - 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + 1.2 * ctx->grafcet_con_delta;
        x[point] = dest_x - 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y - ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y;
      }
      p_num = point;
      draw_routed_trans( point, x, y);
      break;
    case flow_eConType_TransConv:

      if ( dest_y - ctx->grafcet_con_delta > src_y )
      {
        point = 0;
        x[point] = dest_x;
        y[point++] = dest_y;
        x[point] = dest_x;
        y[point++] = dest_y - 0.8 * ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = dest_y - 0.8 * ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y - 1.2 * ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = dest_y - 1.2 * ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y;
      }
      else if ( dest_x > src_x)
      {
        point = 0;
        x[point] = dest_x;
        y[point++] = dest_y;
        x[point] = dest_x;
        y[point++] = dest_y - 0.8 * ctx->grafcet_con_delta;
        x[point] = src_x + 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - 0.8 * ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y - 1.2 * ctx->grafcet_con_delta;
        x[point] = src_x + 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - 1.2 * ctx->grafcet_con_delta;
        x[point] = src_x + 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y;
      }
      else
      {
        point = 0;
        x[point] = dest_x;
        y[point++] = dest_y;
        x[point] = dest_x;
        y[point++] = dest_y - 0.8 * ctx->grafcet_con_delta;
        x[point] = src_x - 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - 0.8 * ctx->grafcet_con_delta;
        x[point] = src_x - 2 * ctx->grafcet_con_delta;
        x[point] = dest_x;
        y[point++] = dest_y - 1.2 * ctx->grafcet_con_delta;
        x[point] = src_x - 2 * ctx->grafcet_con_delta;
        y[point++] = dest_y - 1.2 * ctx->grafcet_con_delta;
        x[point] = src_x - 2 * ctx->grafcet_con_delta;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y + ctx->grafcet_con_delta;
        x[point] = src_x;
        y[point++] = src_y;
      }
      p_num = point;
      draw_routed_trans( point, x, y);
      break;
    default:
      ;
  }
  for ( i = 0; i < p_num; i++)
  {
    point_x[i] = x[i];
    point_y[i] = y[i];
  }
  return 1;
}

int FlowCon::con_route( double src_x, double src_y, flow_eDirection src_dir,
			double dest_x, double dest_y, flow_eDirection dest_dir)
{
  int sts;
  double ll_x, ur_x, ll_y, ur_y;
  double doc_ll_x, doc_ur_x, doc_ll_y, doc_ur_y;
  FlowNode *doc;

  ll_x = MIN( source_node->x_left, dest_node->x_left) - 10 * ctx->draw_delta;
  ur_x = MAX( source_node->x_right, dest_node->x_right) + 10 * ctx->draw_delta;
  ll_y = MIN( source_node->y_low, dest_node->y_low) - 10 * ctx->draw_delta;
  ur_y = MAX( source_node->y_high, dest_node->y_high) + 10 * ctx->draw_delta;

  /* Find the document node */
  doc = (FlowNode *) ctx->get_document( src_x, src_y);
  if ( doc )
  {
    doc->measure( &doc_ll_x, &doc_ll_y, &doc_ur_x, &doc_ur_y);
    ll_x = MAX( ll_x, doc_ll_x);
    ur_x = MIN( ur_x, doc_ur_x);
    ll_y = MAX( ll_y, doc_ll_y);
    ur_y = MIN( ur_y, doc_ur_y);

    if ( dest_x < doc_ll_x || dest_x > doc_ur_x ||
         dest_y < doc_ll_y || dest_y > doc_ur_y ||
         src_x < doc_ll_x || src_x > doc_ur_x ||
         src_y < doc_ll_y || src_y > doc_ur_y)
      return 0;
  }

  sts = con_route_area( ll_x, ll_y, ur_x, ur_y);
  return sts;
}

int FlowCon::con_route_area( double wind_ll_x, double wind_ll_y, 
	double wind_ur_x, double wind_ur_y)
{
  FlowNode *nodelist = 0;
  FlowCon *conlist = 0;
  FlowNode *node_p;
  int i, j, sts;
  flow_eDirection dest_dir, src_dir;
  double dest_x, dest_y;
  double src_x, src_y;
  double x[8], y[8];
  int	point;
  int	found;

  /* Get the objects in this area */
  for ( i = 0; i < ctx->a.size(); i++)
  {  
    if ( ctx->a[i]->in_area(wind_ll_x, wind_ll_y, wind_ur_x, wind_ur_y))
    {
      if ( ctx->a[i]->type() == flow_eObjectType_Node) 
        ctx->a[i]->link_insert( (FlowArrayElem **)&nodelist);
      if ( ctx->a[i]->type() == flow_eObjectType_Con &&
           ctx->a[i] != (FlowArrayElem *) this) 
        ctx->a[i]->link_insert( (FlowArrayElem **)&conlist);
    }
  }
  if ( !nodelist)
    return 0;

  sts = dest_node->get_conpoint( dest_conpoint, &dest_x, &dest_y, &dest_dir);
  if (EVEN(sts)) return sts;
  sts = source_node->get_conpoint( source_conpoint, &src_x, &src_y, &src_dir);
  if (EVEN(sts)) return sts;

  /* Find straight line between source and destination */
  if ( fabs( dest_y - src_y) < CON_EPSILON && 
       ((dest_dir == flow_eDirection_Right && src_dir == flow_eDirection_Left &&
	  dest_x <= src_x) ||
        (dest_dir == flow_eDirection_Left && src_dir == flow_eDirection_Right &&
	  dest_x >= src_x)))
  {
    found = 0;
    for ( node_p = nodelist; node_p; node_p = node_p->link)
    {
      if ( node_p->in_horiz_line( dest_y, 
		MIN( dest_x, src_x) + ctx->draw_delta + CON_EPSILON,
		MAX( dest_x, src_x) - ctx->draw_delta - CON_EPSILON))
      {
        found = 1;
        break;
      }        
    }
    if ( !found)
    {
      x[0] = dest_x;
      y[0] = dest_y;
      x[1] = src_x;
      y[1] = src_y;
      point_x[0] = x[0] = dest_x;
      point_y[0] = y[0] = dest_y;
      point_x[1] = x[1] = src_x;
      point_y[1] = y[1] = src_y;
      point = p_num = 2;
      if ( cc->corner == flow_eCorner_Rounded)
        draw_routed_roundcorner( point, x, y);
      else
        draw_routed( point, x, y);
      return 1;
    }
  }
  else if ( fabs( dest_x - src_x) < CON_EPSILON && 
       ((dest_dir == flow_eDirection_Up && src_dir == flow_eDirection_Down &&
	  dest_y <= src_y) ||
        (dest_dir == flow_eDirection_Down && src_dir == flow_eDirection_Up &&
	  dest_y >= src_y)))
  {
    found = 0;
    for ( node_p = nodelist; node_p; node_p = node_p->link)
    {
      if ( node_p->in_vert_line( dest_x, 
		MIN( dest_y, src_y) + ctx->draw_delta + CON_EPSILON,
		MAX( dest_y, src_y) - ctx->draw_delta - CON_EPSILON))
      {
        found = 1;
        break;
      }        
    }
    if ( !found)
    {
      x[0] = dest_x;
      y[0] = dest_y;
      x[1] = src_x;
      y[1] = src_y;
      point_x[0] = x[0] = dest_x;
      point_y[0] = y[0] = dest_y;
      point_x[1] = x[1] = src_x;
      point_y[1] = y[1] = src_y;
      point = p_num = 2;
      if ( cc->corner == flow_eCorner_Rounded)
        draw_routed_roundcorner( point, x, y);
      else
        draw_routed( point, x, y);
      return 1;
    }
  }

 /* Find vertical routing lines */
  vert_line_cnt = 0;
  sts = find_vert_line_right( wind_ll_x, wind_ll_y, wind_ur_y, nodelist, 
		nodelist, conlist, conlist, wind_ll_y, wind_ur_y);
  sts = find_vert_line_left( wind_ur_x, wind_ll_y, wind_ur_y, nodelist, 
		nodelist, conlist, conlist, wind_ll_y, wind_ur_y);

  for ( node_p = nodelist; node_p; node_p = node_p->link)
  {
    if ( node_p->obst_x_right < wind_ur_x)
      sts = find_vert_line_right( node_p->obst_x_right, node_p->obst_y_low, node_p->obst_y_high,
		nodelist, node_p->link, conlist, conlist, wind_ll_y, wind_ur_y);
    if ( node_p->obst_x_left > wind_ll_x)
      sts = find_vert_line_left( node_p->obst_x_left, node_p->obst_y_low, node_p->obst_y_high,
		nodelist, node_p->link, conlist, conlist, wind_ll_y, wind_ur_y);
    if ( vert_line_cnt > HV_LINE_ARRAY_SIZE - 3)
      break;
  }

  /* Find horizontal routing lines */
  horiz_line_cnt = 0;
  sts = find_horiz_line_up( wind_ll_y, wind_ll_x, wind_ur_x, nodelist, 
		nodelist, conlist, conlist, wind_ll_x, wind_ur_x);
  sts = find_horiz_line_down( wind_ur_y, wind_ll_x, wind_ur_x, nodelist, 
		nodelist, conlist, conlist, wind_ll_x, wind_ur_x);

  for ( node_p = nodelist; node_p; node_p = node_p->link)
  {
    if ( node_p->obst_y_high < wind_ur_y)
      sts = find_horiz_line_up( node_p->obst_y_high, node_p->obst_x_left, node_p->obst_x_right,
		nodelist, node_p->link, conlist, conlist, wind_ll_x, wind_ur_x);
    if ( node_p->obst_y_low > wind_ll_y)
      sts = find_horiz_line_down( node_p->obst_y_low, node_p->obst_x_left, node_p->obst_x_right,
		nodelist, node_p->link, conlist, conlist, wind_ll_x, wind_ur_x);
    if ( horiz_line_cnt > HV_LINE_ARRAY_SIZE - 3)
      break;
  }


  if ( dest_dir == flow_eDirection_Center)
  {
    switch ( src_dir)
    {
      case flow_eDirection_Center:
        if ( src_y > dest_y) 
          dest_dir = flow_eDirection_Up;
        else
          dest_dir = flow_eDirection_Down;
        if ( dest_x > src_x)
          src_dir = flow_eDirection_Right;
        else
          src_dir = flow_eDirection_Left;
        break;
      case flow_eDirection_Right:
      case flow_eDirection_Left:
        if ( src_y > dest_y) 
          dest_dir = flow_eDirection_Up;
        else
          dest_dir = flow_eDirection_Down;
        break;
      case flow_eDirection_Up:
      case flow_eDirection_Down:
        if ( src_x > dest_y)
          dest_dir = flow_eDirection_Right;
        else
          dest_dir = flow_eDirection_Left;
        break;
    }
  }
  else if ( src_dir == flow_eDirection_Center)
  {
    switch ( dest_dir)
    {
      case flow_eDirection_Center:
        if ( dest_y > src_y) 
          src_dir = flow_eDirection_Up;
        else
          src_dir = flow_eDirection_Down;
        if ( src_x > dest_x)
          dest_dir = flow_eDirection_Right;
        else
          dest_dir = flow_eDirection_Left;
        break;
      case flow_eDirection_Right:
      case flow_eDirection_Left:
        if ( dest_y > src_y) 
          src_dir = flow_eDirection_Up;
        else
          src_dir = flow_eDirection_Down;
        break;
      case flow_eDirection_Up:
      case flow_eDirection_Down:
        if ( dest_x > src_y)
          src_dir = flow_eDirection_Right;
        else
          src_dir = flow_eDirection_Left;
        break;
    }
  }

  /* Add the destination point line */
  switch( dest_dir)
  {
    case flow_eDirection_Right:
      horiz_line[horiz_line_cnt].y = dest_y;
      horiz_line[horiz_line_cnt].l_x = dest_x;
      horiz_line[horiz_line_cnt].u_x = wind_ur_x - ctx->draw_delta;
      horiz_line[horiz_line_cnt].dest = 1;
      find_horiz_line_right_border( dest_y, dest_x, dest_x,
		&horiz_line[horiz_line_cnt].u_x, nodelist, conlist);
      horiz_line_cnt++;
      break;
    case flow_eDirection_Left:
      horiz_line[horiz_line_cnt].y = dest_y;
      horiz_line[horiz_line_cnt].u_x = dest_x;
      horiz_line[horiz_line_cnt].l_x = wind_ll_x + ctx->draw_delta;
      horiz_line[horiz_line_cnt].dest = 1;
      find_horiz_line_left_border( dest_y, dest_x, dest_x,
		&horiz_line[horiz_line_cnt].l_x, nodelist, conlist);
      horiz_line_cnt++;
      break;
    case flow_eDirection_Up:
      vert_line[vert_line_cnt].x = dest_x;
      vert_line[vert_line_cnt].l_y = dest_y;
      vert_line[vert_line_cnt].u_y = wind_ur_y - ctx->draw_delta;
      vert_line[vert_line_cnt].dest = 1;
      find_vert_line_high_border( dest_x, dest_y, dest_y,
		&vert_line[vert_line_cnt].u_y, nodelist, conlist);
      vert_line_cnt++;
      break;
    case flow_eDirection_Down:
      vert_line[vert_line_cnt].x = dest_x;
      vert_line[vert_line_cnt].l_y = wind_ll_y + ctx->draw_delta;
      vert_line[vert_line_cnt].u_y = dest_y;
      vert_line[vert_line_cnt].dest = 1;
      find_vert_line_low_border( dest_x, dest_y, dest_y, 
		&vert_line[vert_line_cnt].l_y, nodelist, conlist);
      vert_line_cnt++;
      break;
    case flow_eDirection_Center:
      break;
  }

  /* Sort the lines */
  sort_lines( dest_x, dest_y, dest_dir, src_x, src_y, src_dir);
  current_line_cnt = 1000;
  loop_cnt = 0;

  /* Get the source point line */
  line_table_cnt = 0;
  switch( src_dir)
  {
    case flow_eDirection_Right:
      horiz_line[horiz_line_cnt].y = src_y;
      horiz_line[horiz_line_cnt].l_x = src_x;
      horiz_line[horiz_line_cnt].u_x = wind_ur_x - ctx->draw_delta;
      horiz_line[horiz_line_cnt].dest = 0;
      find_horiz_line_right_border( src_y, src_x, src_x,
		&horiz_line[horiz_line_cnt].u_x, nodelist, conlist);
      horiz_line_cnt++;
      line_table[0].horiz[0] = &horiz_line[horiz_line_cnt-1];
      line_table[0].horiz_x[0] = src_x;
      line_table[0].horiz_y[0] = src_y;
      line_table[0].horiz_cnt = 1;
      line_table[0].vert_cnt = 0;
      line_table[0].complete = 0;
      line_table[0].start_type = eLineType_Horiz;
      sts = find_horiz_line_next_line( &horiz_line[horiz_line_cnt-1]);
      break;
    case flow_eDirection_Left:
      horiz_line[horiz_line_cnt].y = src_y;
      horiz_line[horiz_line_cnt].u_x = src_x;
      horiz_line[horiz_line_cnt].l_x = wind_ll_x + ctx->draw_delta;
      horiz_line[horiz_line_cnt].dest = 0;
      find_horiz_line_left_border( src_y, src_x, src_x, 
		&horiz_line[horiz_line_cnt].l_x, nodelist, conlist);
      horiz_line_cnt++;
      line_table[0].horiz[0] = &horiz_line[horiz_line_cnt-1];
      line_table[0].horiz_x[0] = src_x;
      line_table[0].horiz_y[0] = src_y;
      line_table[0].horiz_cnt = 1;
      line_table[0].vert_cnt = 0;
      line_table[0].complete = 0;
      line_table[0].start_type = eLineType_Horiz;
      sts = find_horiz_line_next_line( &horiz_line[horiz_line_cnt-1]);
      break;
    case flow_eDirection_Up:
      vert_line[vert_line_cnt].x = src_x;
      vert_line[vert_line_cnt].l_y = src_y;
      vert_line[vert_line_cnt].u_y = wind_ur_y - ctx->draw_delta;
      vert_line[vert_line_cnt].dest = 0;
      find_vert_line_high_border( src_x, src_y, src_y, 
		&vert_line[vert_line_cnt].u_y, nodelist, conlist);
      vert_line_cnt++;
      line_table[0].vert[0] = &vert_line[vert_line_cnt-1];
      line_table[0].vert_x[0] = src_x;
      line_table[0].vert_y[0] = src_y;
      line_table[0].vert_cnt = 1;
      line_table[0].horiz_cnt = 0;
      line_table[0].complete = 0;
      line_table[0].start_type = eLineType_Vert;
      sts = find_vert_line_next_line( &vert_line[vert_line_cnt-1]);
      break;
    case flow_eDirection_Down:
      vert_line[vert_line_cnt].x = src_x;
      vert_line[vert_line_cnt].l_y = wind_ll_y + ctx->draw_delta;
      vert_line[vert_line_cnt].u_y = src_y;
      vert_line[vert_line_cnt].dest = 0;
      find_vert_line_low_border( src_x, src_y, src_y, 
		&vert_line[vert_line_cnt].l_y, nodelist, conlist);
      vert_line_cnt++;
      line_table[0].vert[0] = &vert_line[vert_line_cnt-1];
      line_table[0].vert_x[0] = src_x;
      line_table[0].vert_y[0] = src_y;
      line_table[0].vert_cnt = 1;
      line_table[0].horiz_cnt = 0;
      line_table[0].complete = 0;
      line_table[0].start_type = eLineType_Vert;
      sts = find_vert_line_next_line( &vert_line[vert_line_cnt-1]);
      break;
    case flow_eDirection_Center:
      break;
  }

#if 0
  for ( i = 0; i < vert_line_cnt; i++)
  {
//    printf( "vert %d  x: %7.3f  l_y: %7.3f  u_y: %7.3f\n", 
//	&vert_line[i], vert_line[i].x,
//	vert_line[i].l_y, vert_line[i].u_y);
    draw_line( ctx, vert_line[i].x, vert_line[i].l_y, vert_line[i].x, 
		vert_line[i].u_y);
  }
  for ( i = 0; i < horiz_line_cnt; i++)
  {
//    printf( "hori %d  y: %7.3f  l_x: %7.3f  u_x: %7.3f\n", 
//	&horiz_line[i], horiz_line[i].y,
//	horiz_line[i].l_x, horiz_line[i].u_x);
    draw_line( ctx, horiz_line[i].l_x, horiz_line[i].y, horiz_line[i].u_x, 
		horiz_line[i].y);
  }
  for ( i = 0; i < line_table_cnt; i++)
  {
    if ( line_table[i].complete)
    {
      if ( line_table[i].start_type == eLineType_Horiz)
      {
        for ( j = 0; j < line_table[i].horiz_cnt; j++)
        {
          printf( "p_horiz: %d  %7.3f  %7.3f\n", 
		line_table[i].horiz[j],
		line_table[i].horiz_x[j],
		line_table[i].horiz_y[j]);
          if ( j < line_table[i].vert_cnt )
            printf( "p_vert : %d  %7.3f  %7.3f\n",
		line_table[i].vert[j],
		line_table[i].vert_x[j],
		line_table[i].vert_y[j]);
        }
      }
      else
      {
        for ( j = 0; j < line_table[i].vert_cnt; j++)
        {
          printf( "p_vert : %d  %7.3f  %7.3f\n", 
		line_table[i].vert[j],
		line_table[i].vert_x[j],
		line_table[i].vert_y[j]);
          if ( j < line_table[i].horiz_cnt )
            printf( "p_horiz: %d  %7.3f  %7.3f\n",
		line_table[i].horiz[j],
		line_table[i].horiz_x[j],
		line_table[i].horiz_y[j]);
        }
      }
    }
  }
#endif
  /* Select the shortest */
  int min_idx;
  int min_cnt = 10000;
  for ( i = 0; i < line_table_cnt; i++)
  {
    if ( line_table[i].complete)
    {
      if ( line_table[i].start_type == eLineType_Horiz)
      {
        if ( min_cnt > line_table[i].horiz_cnt)
        {
          min_cnt = line_table[i].horiz_cnt;
          min_idx = i;
        }
      }
      else
      {
        if ( min_cnt > line_table[i].horiz_cnt)
        {
          min_cnt = line_table[i].vert_cnt;
          min_idx = i;
        }
      }
    }
  }
//  printf( "Shortest: %d  cnt %d (of %d)\n", min_idx, min_cnt, line_table_cnt);
  if ( !line_table_cnt)
    return 0;

  /* Draw the con */
  if ( line_table[min_idx].start_type == eLineType_Horiz)
  {
    point = 0;
    for ( j = 0; j < line_table[min_idx].horiz_cnt; j++)
    {

      point_x[point] = x[point] = line_table[min_idx].horiz_x[j];
      point_y[point] = y[point] = line_table[min_idx].horiz_y[j];
      point++;
      if ( j < line_table[min_idx].vert_cnt )
      {
        point_x[point] = x[point] = line_table[min_idx].vert_x[j];
        point_y[point] = y[point] = line_table[min_idx].vert_y[j];
        point++;
      }
    }
  }
  else
  {
    point = 0;
    for ( j = 0; j < line_table[min_idx].vert_cnt; j++)
    {

      point_x[point] = x[point] = line_table[min_idx].vert_x[j];
      point_y[point] = y[point] = line_table[min_idx].vert_y[j];
      point++;
      if ( j < line_table[min_idx].horiz_cnt )
      {
        point_x[point] = x[point] = line_table[min_idx].horiz_x[j];
        point_y[point] = y[point] = line_table[min_idx].horiz_y[j];
        point++;
      }
    }
  }
  if ( point > MAX_POINT - 1)

    return 0;

  point_x[point] = x[point] = dest_x;
  point_y[point] = y[point] = dest_y;
  point++;
  p_num = point;

  if ( cc->corner == flow_eCorner_Rounded)
    draw_routed_roundcorner( point, x, y);
  else
    draw_routed( point, x, y);

  return 1;
}

void draw_line( FlowCtx *ctx, double x1, double y1, double x2, double y2)
{
  ctx->fdraw->line( ctx, 
	int( x1*ctx->zoom_factor-ctx->offset_x), 
	int( y1*ctx->zoom_factor-ctx->offset_y), 
	int( x2*ctx->zoom_factor-ctx->offset_x), 
	int( y2*ctx->zoom_factor-ctx->offset_y),
	flow_eDrawType_Line, 0, 0);
}
void print_line()
{
  int j;
      printf( "Line nr %d, ", line_table_cnt);
      if ( line_table[line_table_cnt].start_type == eLineType_Horiz)
      {
        for ( j = 0; j < line_table[line_table_cnt].horiz_cnt; j++)
        {
          printf( "%d ", int( line_table[line_table_cnt].horiz[j]));
          if ( j < line_table[line_table_cnt].vert_cnt )
            printf( "%d ", int( line_table[line_table_cnt].vert[j]));
        }
      }
      else
      {
        for ( j = 0; j < line_table[line_table_cnt].vert_cnt; j++)
        {
          printf( "%d ", int( line_table[line_table_cnt].vert[j]));
          if ( j < line_table[line_table_cnt].horiz_cnt )
            printf( "%d ", int( line_table[line_table_cnt].horiz[j]));
        }
      }
      printf( "\n");
}

int con_cmp_v1( const void *l1, const void *l2)
{
  /* l1 is left and l2 is right of dest */
  if ( ((con_tVertLines *)l1)->x < sort_dest_x &&
       ((con_tVertLines *)l2)->x > sort_dest_x)
    return 1;

  /* l2 is left and l1 is right of dest */
  if ( ((con_tVertLines *)l2)->x < sort_dest_x &&
       ((con_tVertLines *)l1)->x > sort_dest_x)
    return -1;
  
  return ( fabs(((con_tVertLines *)l1)->x - sort_dest_x) > 
	fabs(((con_tVertLines *)l2)->x - sort_dest_x));
}

int con_cmp_v2( const void *l1, const void *l2)
{
  /* l1 is right and l2 is left of dest */
  if ( ((con_tVertLines *)l1)->x > sort_dest_x &&
       ((con_tVertLines *)l2)->x < sort_dest_x)
    return 1;

  /* l2 is right and l1 is left of dest */
  if ( ((con_tVertLines *)l2)->x > sort_dest_x &&
       ((con_tVertLines *)l1)->x < sort_dest_x)
    return -1;
  
  return ( fabs(((con_tVertLines *)l1)->x - sort_dest_x) > 
	fabs(((con_tVertLines *)l2)->x - sort_dest_x));
}

int con_cmp_h1( const void *l1, const void *l2)
{
  return ( fabs(((con_tHorizLines *)l1)->y - sort_dest_y) > 
	fabs(((con_tHorizLines *)l2)->y - sort_dest_y));
}

int con_cmp_h2( const void *l1, const void *l2)
{

  /* l1 intersects with the dest-node and not l2 */
  if ( (sort_dest->obst_y_low < ((con_tHorizLines *)l1)->y &&
       ((con_tHorizLines *)l1)->y < sort_dest->obst_y_high &&
       ((con_tHorizLines *)l1)->l_x > sort_dest_x) &&
      !(sort_dest->obst_y_low < ((con_tHorizLines *)l2)->y &&
       ((con_tHorizLines *)l2)->y < sort_dest->obst_y_high &&
       ((con_tHorizLines *)l2)->l_x > sort_dest_x) )
    return 1;

  /* l2 intersects with the dest-node and not l1 */
  if ( (sort_dest->obst_y_low < ((con_tHorizLines *)l2)->y &&
       ((con_tHorizLines *)l2)->y < sort_dest->obst_y_high &&
       ((con_tHorizLines *)l2)->l_x > sort_dest_x) &&
      !(sort_dest->obst_y_low < ((con_tHorizLines *)l1)->y &&
       ((con_tHorizLines *)l1)->y < sort_dest->obst_y_high &&
       ((con_tHorizLines *)l1)->l_x > sort_dest_x) )
    return -1;

  /* l1 is between dest and source and not l2 */
  if ( (sort_dest->obst_y_high < sort_source->obst_y_low) &&
       (sort_dest->obst_y_high < ((con_tHorizLines *)l1)->y &&
       ((con_tHorizLines *)l1)->y < sort_source->obst_y_low) &&
       ((con_tHorizLines *)l2)->y < sort_dest->obst_y_low)
    return -1;

  /* l2 is between dest and source and not l1 */
  if ( (sort_dest->obst_y_high < sort_source->obst_y_low) &&
       (sort_dest->obst_y_high < ((con_tHorizLines *)l2)->y &&
       ((con_tHorizLines *)l2)->y < sort_source->obst_y_low) &&
       ((con_tHorizLines *)l1)->y < sort_dest->obst_y_low)
    return 1;

  /* l1 is between dest and source and not l2 */
  if ( (sort_dest->obst_y_low > sort_source->obst_y_high) &&
       (sort_source->obst_y_high < ((con_tHorizLines *)l1)->y &&
       ((con_tHorizLines *)l1)->y < sort_dest->obst_y_low) &&
       (((con_tHorizLines *)l2)->y < sort_source->obst_y_low ||
        ((con_tHorizLines *)l2)->y > sort_dest->obst_y_high))
    return -1;

  /* l2 is between dest and source and not l1 */
  if ( (sort_dest->obst_y_low > sort_source->obst_y_high) &&
       (sort_source->obst_y_high < ((con_tHorizLines *)l2)->y &&
       ((con_tHorizLines *)l2)->y < sort_dest->obst_y_low) &&
       (((con_tHorizLines *)l1)->y < sort_source->obst_y_low ||
        ((con_tHorizLines *)l1)->y > sort_dest->obst_y_high))
    return 1;

  return ( fabs(((con_tHorizLines *)l1)->y - sort_dest_y) > 
	fabs(((con_tHorizLines *)l2)->y - sort_dest_y));
}

int con_cmp_h3( const void *l1, const void *l2)
{

  /* l1 intersects with the dest-node and not l2 */
  if ( (sort_dest->obst_y_low < ((con_tHorizLines *)l1)->y &&
       ((con_tHorizLines *)l1)->y < sort_dest->obst_y_high &&
       ((con_tHorizLines *)l1)->l_x < sort_dest_x) &&
      !(sort_dest->obst_y_low < ((con_tHorizLines *)l2)->y &&
       ((con_tHorizLines *)l2)->y < sort_dest->obst_y_high &&
       ((con_tHorizLines *)l2)->l_x < sort_dest_x) )
    return 1;

  /* l2 intersects with the dest-node and not l1 */
  if ( (sort_dest->obst_y_low < ((con_tHorizLines *)l2)->y &&
       ((con_tHorizLines *)l2)->y < sort_dest->obst_y_high &&
       ((con_tHorizLines *)l2)->l_x < sort_dest_x) &&
      !(sort_dest->obst_y_low < ((con_tHorizLines *)l1)->y &&
       ((con_tHorizLines *)l1)->y < sort_dest->obst_y_high &&
       ((con_tHorizLines *)l1)->l_x < sort_dest_x) )
    return -1;

  /* l1 is between dest and source and l2 is lower then dest */
  if ( (sort_dest->obst_y_high < sort_source->obst_y_low) &&
       (sort_dest->obst_y_high < ((con_tHorizLines *)l1)->y &&
       ((con_tHorizLines *)l1)->y < sort_source->obst_y_low) &&
       (((con_tHorizLines *)l2)->y < sort_source->obst_y_low ||
        ((con_tHorizLines *)l2)->y > sort_dest->obst_y_high))
    return -1;

  /* l2 is between dest and source and l1 is lower then dest or higher then src*/
  if ( (sort_dest->obst_y_high < sort_source->obst_y_low) &&
       (sort_dest->obst_y_high < ((con_tHorizLines *)l2)->y &&
       ((con_tHorizLines *)l2)->y < sort_source->obst_y_low) &&
       (((con_tHorizLines *)l1)->y < sort_source->obst_y_low ||
        ((con_tHorizLines *)l1)->y > sort_dest->obst_y_high))
    return 1;

  /* l1 is between dest and source and l2 is higher than dest */
  if ( (sort_dest->obst_y_low > sort_source->obst_y_high) &&
       (sort_source->obst_y_high < ((con_tHorizLines *)l1)->y &&
       ((con_tHorizLines *)l1)->y < sort_dest->obst_y_low) &&
       ((con_tHorizLines *)l2)->y > sort_dest->obst_y_high)
    return -1;

  /* l2 is between dest and source and l1 is higher than dest */
  if ( (sort_dest->obst_y_low > sort_source->obst_y_high) &&
       (sort_source->obst_y_high < ((con_tHorizLines *)l2)->y &&
       ((con_tHorizLines *)l2)->y < sort_dest->obst_y_low) &&
       ((con_tHorizLines *)l1)->y > sort_dest->obst_y_high)
    return 1;

  return ( fabs(((con_tHorizLines *)l1)->y - sort_dest_y) > 
	fabs(((con_tHorizLines *)l2)->y - sort_dest_y));
}

int FlowCon::sort_lines( double dest_x, double dest_y, flow_eDirection dest_dir,
	double  src_x, double src_y, flow_eDirection src_dir)
{

  sort_dest_x = dest_x;
  sort_dest_y = dest_y;
  sort_source = source_node;
  sort_dest = dest_node;
  if ( dest_dir == flow_eDirection_Right && 
       src_dir == flow_eDirection_Left &&
       src_x > dest_x)
  {
    flow_qsort( vert_line, vert_line_cnt, sizeof(vert_line[0]), con_cmp_v1);    
    flow_qsort( horiz_line, horiz_line_cnt, sizeof(horiz_line[0]), con_cmp_h1);    
    ideal_line_cnt = 3;
  }
  else if ( dest_dir == flow_eDirection_Right && 
            src_dir == flow_eDirection_Left &&
            src_x <= dest_x)
  {
    flow_qsort( vert_line, vert_line_cnt, sizeof(vert_line[0]), con_cmp_v1);    
    flow_qsort( horiz_line, horiz_line_cnt, sizeof(horiz_line[0]), con_cmp_h3);
    ideal_line_cnt = 5;
  }
  else if ( dest_dir == flow_eDirection_Left && 
	    src_dir == flow_eDirection_Right &&
	    src_x < dest_x)
  {
    flow_qsort( vert_line, vert_line_cnt, sizeof(vert_line[0]), con_cmp_v2);    
    flow_qsort( horiz_line, horiz_line_cnt, sizeof(horiz_line[0]), con_cmp_h1);    
    ideal_line_cnt = 3;
  }
  else if ( dest_dir == flow_eDirection_Left && 
	    src_dir == flow_eDirection_Right &&
	    src_x > dest_x)
  {
    flow_qsort( vert_line, vert_line_cnt, sizeof(vert_line[0]), con_cmp_v2);
    flow_qsort( horiz_line, horiz_line_cnt, sizeof(horiz_line[0]), con_cmp_h2);
    ideal_line_cnt = 5;
  }
  else if ( dest_dir == flow_eDirection_Right && 
	    src_dir == flow_eDirection_Right)
  {
    flow_qsort( vert_line, vert_line_cnt, sizeof(vert_line[0]), con_cmp_v1);
    flow_qsort( horiz_line, horiz_line_cnt, sizeof(horiz_line[0]), con_cmp_h1);
    ideal_line_cnt = 3;
  }
  else if ( dest_dir == flow_eDirection_Left && 
	    src_dir == flow_eDirection_Left)
  {
    flow_qsort( vert_line, vert_line_cnt, sizeof(vert_line[0]), con_cmp_v2);
    flow_qsort( horiz_line, horiz_line_cnt, sizeof(horiz_line[0]), con_cmp_h1);
    ideal_line_cnt = 3;
  }

  return 1;
}

int FlowCon::find_horiz_line_next_line( con_tHorizLines *h_line)
{
  int horiz_cnt;
  int vert_cnt;
  int i, j, found;
  int sts;


  loop_cnt++;
  if ( loop_cnt > ROUTE_LOOP_MAX)
    return 0;
  if ( line_table_cnt >= LINE_TABLE_SIZE)
    return 0;

  /* Find vertical lines that intercept */
  for ( i = 0; i < vert_line_cnt; i++)
  {
    if ( h_line->l_x <= vert_line[i].x && vert_line[i].x <= h_line->u_x &&
	 vert_line[i].l_y <= h_line->y && h_line->y <= vert_line[i].u_y)
    {
      if ( vert_line[i].dest)
      {
        /* The route is complete */
        if ( line_table_cnt >= LINE_TABLE_SIZE)
          return 0;
        if ( line_table[line_table_cnt].vert_cnt +
	     line_table[line_table_cnt].horiz_cnt >= current_line_cnt - 1)
          return 1;

	memcpy( &line_table[line_table_cnt+1], &line_table[line_table_cnt],
		sizeof( line_table[0]));
        line_table[line_table_cnt].vert[line_table[line_table_cnt].vert_cnt] =
		&vert_line[i];
        line_table[line_table_cnt].vert_x[line_table[line_table_cnt].vert_cnt] =
		vert_line[i].x;
        line_table[line_table_cnt].vert_y[line_table[line_table_cnt].vert_cnt] =
		h_line->y;
        line_table[line_table_cnt].vert_cnt++;
        current_line_cnt = line_table[line_table_cnt].vert_cnt +
			line_table[line_table_cnt].horiz_cnt;
        line_table[line_table_cnt].complete = 1;
        line_table_cnt++;
        if ( current_line_cnt == ideal_line_cnt)
          return CON__ROUTE_FOUND;
      }
      else
      {
        if ( (line_table[line_table_cnt].vert_cnt + 
	      line_table[line_table_cnt].horiz_cnt < current_line_cnt - 1) &&
             (line_table[line_table_cnt].vert_cnt + 
	      line_table[line_table_cnt].horiz_cnt < MAX_POINT - 1))
        {
          /* Check that the line is not already inserted */
          found = 0;
          for ( j = 0; j < line_table[line_table_cnt].vert_cnt; j++)
          {
            if ( line_table[line_table_cnt].vert[j] == &vert_line[i])
            {
              found = 1;
              break;
            }
          }
	  if ( !found)
          {
            /* Try this line  */

            /* Store the counters */
            vert_cnt = line_table[line_table_cnt].vert_cnt;
            horiz_cnt = line_table[line_table_cnt].horiz_cnt;

            line_table[line_table_cnt].vert[line_table[line_table_cnt].vert_cnt] =
		&vert_line[i];
            line_table[line_table_cnt].vert_x[line_table[line_table_cnt].vert_cnt] =
		vert_line[i].x;
            line_table[line_table_cnt].vert_y[line_table[line_table_cnt].vert_cnt] =
		h_line->y;
            line_table[line_table_cnt].vert_cnt++;
//            printf( "Trying vert: %d, vert_cnt %d, horiz_cnt %d nr %d\n",
//		&vert_line[i], line_table[line_table_cnt].vert_cnt,
//		line_table[line_table_cnt].horiz_cnt, i);
//	    print_line();
            sts = find_vert_line_next_line( &vert_line[i]);
	    if ( sts == CON__ROUTE_FOUND) return sts;
	    else if (EVEN(sts)) return sts;

	    /* Restore the counters */
            line_table[line_table_cnt].vert_cnt = vert_cnt;
            line_table[line_table_cnt].horiz_cnt = horiz_cnt;
          }
        }
      }
    }
  }
  return 1;
}

int FlowCon::find_vert_line_next_line( con_tVertLines *v_line)
{
  int horiz_cnt;
  int vert_cnt;
  int i, j, found;
  int sts;


  loop_cnt++;
  if ( loop_cnt > ROUTE_LOOP_MAX)
    return 0;
  if ( line_table_cnt >= LINE_TABLE_SIZE)
    return 0;

  /* Find horizontal lines that intercept */
  for ( i = 0; i < horiz_line_cnt; i++)
  {
    if ( v_line->l_y <= horiz_line[i].y && horiz_line[i].y <= v_line->u_y &&
	 horiz_line[i].l_x <= v_line->x && v_line->x <= horiz_line[i].u_x)
    {
      if ( horiz_line[i].dest)
      {
        /* The route is complete */
        if ( line_table_cnt >= LINE_TABLE_SIZE)
          return 0;
        if ( line_table[line_table_cnt].vert_cnt +
	     line_table[line_table_cnt].horiz_cnt >= current_line_cnt - 1)
          return 1;

	memcpy( &line_table[line_table_cnt+1], &line_table[line_table_cnt],
		sizeof( line_table[0]));
        line_table[line_table_cnt].horiz[line_table[line_table_cnt].horiz_cnt] =
		&horiz_line[i];
        line_table[line_table_cnt].horiz_y[line_table[line_table_cnt].horiz_cnt] =
		horiz_line[i].y;
        line_table[line_table_cnt].horiz_x[line_table[line_table_cnt].horiz_cnt] =
		v_line->x;
        line_table[line_table_cnt].horiz_cnt++;
        current_line_cnt = line_table[line_table_cnt].vert_cnt +
			line_table[line_table_cnt].horiz_cnt;
        line_table[line_table_cnt].complete = 1;
        line_table_cnt++;
        if ( current_line_cnt == ideal_line_cnt)
          return CON__ROUTE_FOUND;
      }
      else
      {

        /* Check that the line is not already inserted */
        if ( (line_table[line_table_cnt].vert_cnt + 
	      line_table[line_table_cnt].horiz_cnt < current_line_cnt - 1) &&
             (line_table[line_table_cnt].vert_cnt + 
	      line_table[line_table_cnt].horiz_cnt < MAX_POINT - 1))
        {
          found = 0;
          for ( j = 0; j < line_table[line_table_cnt].horiz_cnt; j++)
          {
            if ( line_table[line_table_cnt].horiz[j] == &horiz_line[i])
            {
              found = 1;
              break;
            }
          }
	  if ( !found)
          {
            /* Try this line  */

            /* Store the counters */
            vert_cnt = line_table[line_table_cnt].vert_cnt;
            horiz_cnt = line_table[line_table_cnt].horiz_cnt;

            line_table[line_table_cnt].horiz[line_table[line_table_cnt].horiz_cnt] =
		&horiz_line[i];
            line_table[line_table_cnt].horiz_y[line_table[line_table_cnt].horiz_cnt] =
		horiz_line[i].y;
            line_table[line_table_cnt].horiz_x[line_table[line_table_cnt].horiz_cnt] =
		v_line->x;
            line_table[line_table_cnt].horiz_cnt++;
//            printf( "Trying horiz: %d, vert_cnt %d, horiz_cnt %d nr %d\n",
//		&horiz_line[i], line_table[line_table_cnt].vert_cnt,
//		line_table[line_table_cnt].horiz_cnt, i);
//	    print_line();
            sts = find_horiz_line_next_line( &horiz_line[i]);
	    if ( sts == CON__ROUTE_FOUND) return sts;
	    else if (EVEN(sts)) return sts;

	    /* Restore the counters */
            line_table[line_table_cnt].vert_cnt = vert_cnt;
            line_table[line_table_cnt].horiz_cnt = horiz_cnt;
          }
        }
      }
    }
  }
  return 1;
}

int FlowCon::find_horiz_line_up( double check_y, double check_l_x,
	double check_u_x, FlowNode *nodelist, FlowNode *next_node,
	FlowCon *conlist, FlowCon *next_con,
	double wind_ll_x, double wind_ur_x)
{
  FlowNode *node_p, *node_p2;
  FlowCon *con_p;
  int found, i, j;
  double check_wind_l_x, check_wind_u_x;
  double l_x, u_x;

  for ( node_p = next_node; node_p; node_p = node_p->link)
  {
    if ( node_p->obst_y_low > check_y)
    {
      if ( node_p->obst_x_left > check_u_x)
      {
         check_wind_u_x = node_p->obst_x_left;
         check_wind_l_x = check_u_x;
      }
      else if ( node_p->obst_x_right < check_l_x)
      {
         check_wind_u_x = check_l_x;
         check_wind_l_x = node_p->obst_x_right;
      }
      else
      {
         check_wind_l_x = MAX( check_l_x, node_p->obst_x_left);
         check_wind_u_x = MIN( check_u_x, node_p->obst_x_right);
      }
      found = 0;
      for ( node_p2 = nodelist; node_p2; node_p2 = node_p2->link)
      {
        if ( node_p2 != node_p && 
             node_p2->in_area_exact(  check_wind_l_x, check_y,
	     check_wind_u_x, node_p->obst_y_low))
        {
          found = 1;
          break;
        }
      }
      if ( !found)
      {
        /* Create a line */
        double horiz_line_y = (check_y + node_p->obst_y_low) / 2;
	found = 0;
        for ( j = 0; j < horiz_line_cnt; j++)
        {
          if ( fabs( horiz_line[j].y-horiz_line_y) < ctx->draw_delta)
          {
            found = 1;
            break;
          }
        }
	if ( found)
          continue;
        horiz_line[horiz_line_cnt].y = (check_y + node_p->obst_y_low) / 2;
        horiz_line[horiz_line_cnt].l_x = wind_ll_x + ctx->draw_delta;
        horiz_line[horiz_line_cnt].u_x = wind_ur_x - ctx->draw_delta;
        horiz_line[horiz_line_cnt].dest = 0;
        find_horiz_line_left_border( horiz_line[horiz_line_cnt].y, 
		check_wind_l_x, horiz_line[horiz_line_cnt].u_x, 
		&horiz_line[horiz_line_cnt].l_x, nodelist, conlist);
        find_horiz_line_right_border( horiz_line[horiz_line_cnt].y,
		check_wind_u_x, horiz_line[horiz_line_cnt].l_x, 
		&horiz_line[horiz_line_cnt].u_x, nodelist, conlist);
        horiz_line_cnt++;
      }
    }
  }
  for ( con_p = next_con; con_p; con_p = con_p->link)
  {
    if ( con_p == this)
      continue;

    /* Check vertical lines in the con */
    if  ( cc->con_type == flow_eConType_Routed)
    {
      if( con_p->source_direction == flow_eDirection_Right ||
          con_p->source_direction == flow_eDirection_Left)
        i = 0;
      else
        i = 1;

      for ( ; i < con_p->p_num - 1; i += 2)
      {
        if ( con_p->point_y[i] < check_y)
        {
          if ( con_p->point_x[i] < con_p->point_x[i+1])
          {
            l_x = con_p->point_x[i];
            u_x = con_p->point_x[i+1];
          }
          else
          {
            l_x = con_p->point_x[i+1];
            u_x = con_p->point_x[i];
          }

          if ( l_x > check_u_x)
          {
            check_wind_u_x = l_x;
            check_wind_l_x = check_u_x;
          }
          else if ( u_x < check_l_x)
          {
            check_wind_u_x = check_l_x;
            check_wind_l_x = u_x;
          }
          else
          {
             check_wind_l_x = MAX( check_l_x, l_x);
             check_wind_u_x = MIN( check_u_x, u_x);
          }
          found = 0;
          for ( node_p2 = nodelist; node_p2; node_p2 = node_p2->link)
          {
            if (
                 node_p2->in_area_exact(  check_wind_l_x, con_p->point_y[i],
	         check_wind_u_x, check_y))
            {
              found = 1;
              break;
            }
          }
          if ( !found)
          {
            /* Create a line */
            double horiz_line_y = (check_y + con_p->point_y[i]) / 2;
	    found = 0;
            for ( j = 0; j < horiz_line_cnt; j++)
            {
              if ( fabs( horiz_line[j].y-horiz_line_y) < ctx->draw_delta)
              {
                found = 1;
                break;
              }
            }
	    if ( found)
              continue;
            horiz_line[horiz_line_cnt].y = (check_y + con_p->point_y[i]) / 2;
            horiz_line[horiz_line_cnt].l_x = wind_ll_x + ctx->draw_delta;
            horiz_line[horiz_line_cnt].u_x = wind_ur_x - ctx->draw_delta;
            horiz_line[horiz_line_cnt].dest = 0;
            find_horiz_line_left_border( horiz_line[horiz_line_cnt].y, 
		check_wind_l_x, horiz_line[horiz_line_cnt].u_x, 
		&horiz_line[horiz_line_cnt].l_x, nodelist, conlist);
            find_horiz_line_right_border( horiz_line[horiz_line_cnt].y,
		check_wind_u_x, horiz_line[horiz_line_cnt].l_x, 
		&horiz_line[horiz_line_cnt].u_x, nodelist, conlist);
            if ( horiz_line[horiz_line_cnt].u_x < horiz_line[horiz_line_cnt].l_x)
              continue;
            horiz_line_cnt++;
          }
        }
      }
    }
  }
  return 1;
}

int FlowCon::find_horiz_line_down( double check_y, double check_l_x, 
	double check_u_x, FlowNode *nodelist, FlowNode *next_node,
	FlowCon *conlist, FlowCon *next_con,
	double wind_ll_x, double wind_ur_x)
{
  FlowNode *node_p, *node_p2;
  FlowCon *con_p;
  int found, i, j;
  double check_wind_l_x, check_wind_u_x;
  double l_x, u_x;

  for ( node_p = next_node; node_p; node_p = node_p->link)
  {
    if ( node_p->obst_y_high < check_y)
    {
      if ( node_p->obst_x_left > check_u_x)
      {
         check_wind_u_x = node_p->obst_x_left;
         check_wind_l_x = check_u_x;
      }
      else if ( node_p->obst_x_right < check_l_x)
      {
         check_wind_u_x = check_l_x;
         check_wind_l_x = node_p->obst_x_right;
      }
      else
      {
         check_wind_l_x = MAX( check_l_x, node_p->obst_x_left);
         check_wind_u_x = MIN( check_u_x, node_p->obst_x_right);
      }
      found = 0;
      for ( node_p2 = nodelist; node_p2; node_p2 = node_p2->link)
      {
        if ( node_p2 != node_p && 
             node_p2->in_area_exact( check_wind_l_x, node_p->obst_y_high,
	     check_wind_u_x, check_y))
        {
          found = 1;
          break;
        }
      }
      if ( !found)
      {
        /* Create a line */
        double horiz_line_y = (check_y + node_p->obst_y_high) / 2;
        found = 0;
        for ( j = 0; j < horiz_line_cnt; j++)
        {
          if ( fabs( horiz_line[j].y-horiz_line_y) < ctx->draw_delta)
          {
            found = 1;
            break;
          }
        }
	if ( found)
          continue;
        horiz_line[horiz_line_cnt].y = (check_y + node_p->obst_y_high) / 2;
        horiz_line[horiz_line_cnt].l_x = wind_ll_x + ctx->draw_delta;
        horiz_line[horiz_line_cnt].u_x = wind_ur_x - ctx->draw_delta;
        horiz_line[horiz_line_cnt].dest = 0;
        find_horiz_line_left_border( horiz_line[horiz_line_cnt].y, 
		check_wind_l_x, horiz_line[horiz_line_cnt].u_x, 
		&horiz_line[horiz_line_cnt].l_x, nodelist, conlist);
        find_horiz_line_right_border( horiz_line[horiz_line_cnt].y, 
		check_wind_u_x, horiz_line[horiz_line_cnt].l_x, 
		&horiz_line[horiz_line_cnt].u_x, nodelist, conlist);
        horiz_line_cnt++;
      }
    }
  }
  for ( con_p = next_con; con_p; con_p = con_p->link)
  {
    if ( con_p == this)
      continue;

    /* Check vertical lines in the con */
    if  ( cc->con_type == flow_eConType_Routed)
    {
      if( con_p->source_direction == flow_eDirection_Right ||
          con_p->source_direction == flow_eDirection_Left)
        i = 0;
      else
        i = 1;

      for ( ; i < con_p->p_num - 1; i += 2)
      {
        if ( con_p->point_y[i] < check_y)
        {
          if ( con_p->point_x[i] < con_p->point_x[i+1])
          {
            l_x = con_p->point_x[i];
            u_x = con_p->point_x[i+1];
          }
          else
          {
            l_x = con_p->point_x[i+1];
            u_x = con_p->point_x[i];
          }

          if ( l_x > check_u_x)
          {
             check_wind_u_x = l_x;
             check_wind_l_x = check_u_x;
          }
          else if ( u_x < check_l_x)
          {
             check_wind_u_x = check_l_x;
             check_wind_l_x = u_x;
          }
          else
          {
             check_wind_l_x = MAX( check_l_x, l_x);
             check_wind_u_x = MIN( check_u_x, u_x);
          }
          found = 0;
          for ( node_p2 = nodelist; node_p2; node_p2 = node_p2->link)
          {
            if ( 
                 node_p2->in_area_exact( check_wind_l_x, con_p->point_y[i],
	         check_wind_u_x, check_y))
            {
              found = 1;
              break;
            }
          }
          if ( !found)
          {
            /* Create a line */
            double horiz_line_y = (check_y + con_p->point_y[i]) / 2;
	    found = 0;
            for ( j = 0; j < horiz_line_cnt; j++)
            {
              if ( fabs( horiz_line[j].y-horiz_line_y) < ctx->draw_delta)
              {
                found = 1;
                break;
              }
            }
	    if ( found)
              continue;
            horiz_line[horiz_line_cnt].y = (check_y + con_p->point_y[i]) / 2;
            horiz_line[horiz_line_cnt].l_x = wind_ll_x + ctx->draw_delta;
            horiz_line[horiz_line_cnt].u_x = wind_ur_x - ctx->draw_delta;
            horiz_line[horiz_line_cnt].dest = 0;
            find_horiz_line_left_border( horiz_line[horiz_line_cnt].y, 
		check_wind_l_x, horiz_line[horiz_line_cnt].u_x, 
		&horiz_line[horiz_line_cnt].l_x, nodelist, conlist);
            find_horiz_line_right_border( horiz_line[horiz_line_cnt].y, 
		check_wind_u_x, horiz_line[horiz_line_cnt].l_x, 
		&horiz_line[horiz_line_cnt].u_x, nodelist, conlist);
            if ( horiz_line[horiz_line_cnt].u_x < horiz_line[horiz_line_cnt].l_x)
              continue;
            horiz_line_cnt++;
          }
        }
      }
    }
  }
  return 1;
}

int FlowCon::find_vert_line_right( double check_x, double check_l_y, 
	double check_u_y, FlowNode *nodelist, FlowNode *next_node,
	FlowCon *conlist, FlowCon *next_con,
	double wind_ll_y, double wind_ur_y)
{
  FlowNode *node_p, *node_p2;
  FlowCon *con_p;
  int found, i, j;
  double check_wind_l_y, check_wind_u_y;
  double l_y, u_y;

  for ( node_p = next_node; node_p; node_p = node_p->link)
  {
    if ( node_p->obst_x_left > check_x)
    {
      if ( node_p->obst_y_low > check_u_y)
      {
         check_wind_u_y = node_p->obst_y_low;
         check_wind_l_y = check_u_y;
      }
      else if ( node_p->obst_y_high < check_l_y)
      {
         check_wind_u_y = check_l_y;
         check_wind_l_y = node_p->obst_y_high;
      }
      else
      {
         check_wind_l_y = MAX( check_l_y, node_p->obst_y_low);
         check_wind_u_y = MIN( check_u_y, node_p->obst_y_high);
      }
      found = 0;
      for ( node_p2 = nodelist; node_p2; node_p2 = node_p2->link)
      {
        if ( node_p2 != node_p && 
             node_p2->in_area_exact( check_x, check_wind_l_y,
	     node_p->obst_x_left, check_wind_u_y))
        {
          found = 1;
          break;
        }
      }
      if ( !found)
      {
        /* Create a line */
        double vert_line_x = (check_x + node_p->obst_x_left) / 2;
        found = 0;
        for ( j = 0; j < vert_line_cnt; j++)
        {
          if ( fabs( vert_line[j].x-vert_line_x) < ctx->draw_delta)
          {
            found = 1;
            break;
          }
        }
	if ( found)
          continue;
        vert_line[vert_line_cnt].x = (check_x + node_p->obst_x_left) / 2;
        vert_line[vert_line_cnt].l_y = wind_ll_y + ctx->draw_delta;
        vert_line[vert_line_cnt].u_y = wind_ur_y - ctx->draw_delta;
        vert_line[vert_line_cnt].dest = 0;
        find_vert_line_low_border( vert_line[vert_line_cnt].x, 
		check_wind_l_y, vert_line[vert_line_cnt].u_y, 
		&vert_line[vert_line_cnt].l_y, nodelist, conlist);
        find_vert_line_high_border( vert_line[vert_line_cnt].x, 
		check_wind_u_y, vert_line[vert_line_cnt].l_y, 
		&vert_line[vert_line_cnt].u_y, nodelist, conlist);
        vert_line_cnt++;
      }
    }
  }

  for ( con_p = next_con; con_p; con_p = con_p->link)
  {
    if ( con_p == this)
      continue;

    /* Check vertical lines in the con */
    if  ( con_p->cc->con_type == flow_eConType_Routed && !con_p->temporary_ref)
    {
      if( con_p->source_direction == flow_eDirection_Right ||
          con_p->source_direction == flow_eDirection_Left)
        i = 1;
      else
        i = 0;

      for ( ; i < con_p->p_num - 1; i += 2)
      {
        if ( con_p->point_x[i] > check_x)
        {
          if ( con_p->point_y[i] < con_p->point_y[i+1])
          {
            l_y = con_p->point_y[i];
            u_y = con_p->point_y[i+1];
          }
          else
          {
            l_y = con_p->point_y[i+1];
            u_y = con_p->point_y[i];
          }

          if ( l_y > check_u_y)
          {
            check_wind_u_y = l_y;
            check_wind_l_y = check_u_y;
          }
          else if ( u_y < check_l_y)
          {
            check_wind_u_y = check_l_y;
            check_wind_l_y = u_y;
          }
          else
          {
            check_wind_l_y = MAX( check_l_y, l_y);
            check_wind_u_y = MIN( check_u_y, u_y);
          }
          found = 0;
          for ( node_p2 = nodelist; node_p2; node_p2 = node_p2->link)
          {
            if ( 
                 node_p2->in_area_exact( check_x, check_wind_l_y,
	         con_p->point_x[i], check_wind_u_y))
            {
              found = 1;
              break;
            }
          }

          if ( !found)
          {
            /* Create a line */
            double vert_line_x = (check_x + con_p->point_x[i]) / 2;
	    found = 0;
            for ( j = 0; j < vert_line_cnt; j++)
            {
              if ( fabs( vert_line[j].x-vert_line_x) < ctx->draw_delta)
              {
                found = 1;
                break;
              }
            }
	    if ( found)
              continue;
            vert_line[vert_line_cnt].x = (check_x + con_p->point_x[i]) / 2;
            vert_line[vert_line_cnt].l_y = wind_ll_y + ctx->draw_delta;
            vert_line[vert_line_cnt].u_y = wind_ur_y - ctx->draw_delta;
            vert_line[vert_line_cnt].dest = 0;
            find_vert_line_low_border( vert_line[vert_line_cnt].x, 
		check_wind_l_y, vert_line[vert_line_cnt].u_y, 
		&vert_line[vert_line_cnt].l_y, nodelist, conlist);
            find_vert_line_high_border( vert_line[vert_line_cnt].x, 
		check_wind_u_y, vert_line[vert_line_cnt].l_y, 
		&vert_line[vert_line_cnt].u_y, nodelist, conlist);
            if ( vert_line[vert_line_cnt].u_y < vert_line[vert_line_cnt].l_y)
              continue;
            vert_line_cnt++;
          }
        }
      }
    }
  }
  return 1;
}

int FlowCon::find_vert_line_left( double check_x, double check_l_y, 
	double check_u_y, FlowNode *nodelist, FlowNode *next_node,
	FlowCon *conlist, FlowCon *next_con,
	double wind_ll_y, double wind_ur_y)
{
  int found, i, j;
  FlowNode *node_p, *node_p2;
  FlowCon *con_p;
  double check_wind_l_y, check_wind_u_y;
  double l_y, u_y;

  for ( node_p = next_node; node_p; node_p = node_p->link)
  {
    if ( node_p->obst_x_right < check_x)
    {
      if ( node_p->obst_y_low > check_u_y)
      {
         check_wind_u_y = node_p->obst_y_low;
         check_wind_l_y = check_u_y;
      }
      else if ( node_p->obst_y_high < check_l_y)
      {
         check_wind_u_y = check_l_y;
         check_wind_l_y = node_p->obst_y_high;
      }
      else
      {
         check_wind_l_y = MAX( check_l_y, node_p->obst_y_low);
         check_wind_u_y = MIN( check_u_y, node_p->obst_y_high);
      }
      found = 0;
      for ( node_p2 = nodelist; node_p2; node_p2 = node_p2->link)
      {
        if ( node_p2 != node_p && 
	     node_p2->in_area_exact( node_p->obst_x_right, check_wind_l_y,
	     		check_x, check_wind_u_y))
        {
          found = 1;
          break;
        }
      }
      if ( !found)
      {
        /* Create a line */
        double vert_line_x = (check_x + node_p->obst_x_right) / 2;
	found = 0;
        for ( j = 0; j < vert_line_cnt; j++)
        {
          if ( fabs( vert_line[j].x-vert_line_x) < ctx->draw_delta)
          {
            found = 1;
            break;
          }
        }
	if ( found)
          continue;
        vert_line[vert_line_cnt].x = (check_x + node_p->obst_x_right) / 2;
        vert_line[vert_line_cnt].l_y = wind_ll_y + ctx->draw_delta;
        vert_line[vert_line_cnt].u_y = wind_ur_y - ctx->draw_delta;
        vert_line[vert_line_cnt].dest = 0;
        find_vert_line_low_border( vert_line[vert_line_cnt].x, 
		check_wind_l_y, vert_line[vert_line_cnt].u_y, 
		&vert_line[vert_line_cnt].l_y, nodelist, conlist);
        find_vert_line_high_border( vert_line[vert_line_cnt].x, 
		check_wind_u_y, vert_line[vert_line_cnt].l_y, 
		&vert_line[vert_line_cnt].u_y, nodelist, conlist);
        vert_line_cnt++;
      }
    }
  }
  for ( con_p = next_con; con_p; con_p = con_p->link)
  {
    if ( con_p == this)
      continue;

    /* Check vertical lines in the con */
    if  ( con_p->cc->con_type == flow_eConType_Routed && !con_p->temporary_ref)
    {
      if( con_p->source_direction == flow_eDirection_Right ||
          con_p->source_direction == flow_eDirection_Left)
        i = 1;
      else
        i = 0;

      for ( ; i < con_p->p_num - 1; i += 2)
      {
        if ( con_p->point_x[i] < check_x)
        {
          if ( con_p->point_y[i] < con_p->point_y[i+1])
          {
            l_y = con_p->point_y[i];
            u_y = con_p->point_y[i+1];
          }
          else
          {
            l_y = con_p->point_y[i+1];
            u_y = con_p->point_y[i];
          }

          if ( l_y > check_u_y)
          {
            check_wind_u_y = l_y;
            check_wind_l_y = check_u_y;
          }
          else if ( u_y < check_l_y)
          {
            check_wind_u_y = check_l_y;
            check_wind_l_y = u_y;
          }
          else
          {
            check_wind_l_y = MAX( check_l_y, l_y);
            check_wind_u_y = MIN( check_u_y, u_y);
          }
          found = 0;
          for ( node_p2 = nodelist; node_p2; node_p2 = node_p2->link)
          {
            if (
	         node_p2->in_area_exact( con_p->point_x[i], check_wind_l_y,
	     		check_x, check_wind_u_y))
            {
              found = 1;
              break;
            }
          }
          if ( !found)
          {
            /* Create a line */
            double vert_line_x = (check_x + con_p->point_x[i]) / 2;
            found = 0;
            for ( j = 0; j < vert_line_cnt; j++)
            {
              if ( fabs( vert_line[j].x-vert_line_x) < ctx->draw_delta)
              {
                found = 1;
                break;
              }
            }
	    if ( found)
              continue;
            vert_line[vert_line_cnt].x = (check_x + con_p->point_x[i]) / 2;
            vert_line[vert_line_cnt].l_y = wind_ll_y + ctx->draw_delta;
            vert_line[vert_line_cnt].u_y = wind_ur_y - ctx->draw_delta;
            vert_line[vert_line_cnt].dest = 0;
            find_vert_line_low_border( vert_line[vert_line_cnt].x, 
		check_wind_l_y, vert_line[vert_line_cnt].u_y, 
		&vert_line[vert_line_cnt].l_y, nodelist, conlist);
            find_vert_line_high_border( vert_line[vert_line_cnt].x, 
		check_wind_u_y, vert_line[vert_line_cnt].l_y, 
		&vert_line[vert_line_cnt].u_y, nodelist, conlist);
            if ( vert_line[vert_line_cnt].u_y < vert_line[vert_line_cnt].l_y)
              continue;
            vert_line_cnt++;
          }
        }
      }
    }
  }
  return 1;
}

void FlowCon::find_horiz_line_right_border( double y, double start_x,
		double start_x_con,
		double *border_x, FlowNode *nodelist, FlowCon *conlist)
{
  FlowNode *node_p;
  FlowCon *con_p;
  double l_x;
  int i;

  for ( node_p = nodelist; node_p; node_p = node_p->link)
  {
    if ( node_p->in_horiz_line( y, start_x, *border_x))
      if ( node_p->obst_x_left < *border_x && node_p->obst_x_left >= start_x - CON_EPSILON)
        *border_x = node_p->obst_x_left;
  }
  for ( con_p = conlist; con_p; con_p = con_p->link)
  {
    if ( con_p == this)
      continue;
    if ( (con_p->source_node == source_node && 
	  con_p->source_conpoint == source_conpoint) ||
         (con_p->source_node == dest_node && 
	  con_p->source_conpoint == dest_conpoint) ||
         (con_p->dest_node == source_node && 
	  con_p->dest_conpoint == source_conpoint) ||
         (con_p->dest_node == dest_node && 
	  con_p->dest_conpoint == dest_conpoint))
      continue;

    /* Check horizontal lines in the con */
    if  ( con_p->cc->con_type == flow_eConType_Routed && !con_p->temporary_ref)
    {
      if( con_p->source_direction == flow_eDirection_Right ||
          con_p->source_direction == flow_eDirection_Left)
        i = 0;
      else
        i = 1;

      for ( ; i < con_p->p_num - 1; i += 2)
      {
        if ( con_p->point_x[i] < con_p->point_x[i+1])
          l_x = con_p->point_x[i];
        else
          l_x = con_p->point_x[i+1];
        if ( con_p->point_y[i] - ctx->draw_delta < y &&
	     y < con_p->point_y[i] + ctx->draw_delta)
        {
          if ( l_x < *border_x && l_x >= start_x_con - CON_EPSILON)
            *border_x = l_x;
        }
      }
    }
  }
}

void FlowCon::find_horiz_line_left_border( double y, double start_x,
		double start_x_con,
		double *border_x, FlowNode *nodelist, FlowCon *conlist)
{
  FlowNode *node_p;
  FlowCon *con_p;
  double u_x;
  int i;

  for ( node_p = nodelist; node_p; node_p = node_p->link)
  {
    if ( node_p->in_horiz_line( y, *border_x, start_x))
      if ( node_p->obst_x_right > *border_x && node_p->obst_x_right <= start_x + CON_EPSILON)
        *border_x = node_p->obst_x_right;
  }
  for ( con_p = conlist; con_p; con_p = con_p->link)
  {
    if ( con_p == this)
      continue;
    if ( (con_p->source_node == source_node && 
	  con_p->source_conpoint == source_conpoint) ||
         (con_p->source_node == dest_node && 
	  con_p->source_conpoint == dest_conpoint) ||
         (con_p->dest_node == source_node && 
	  con_p->dest_conpoint == source_conpoint) ||
         (con_p->dest_node == dest_node && 
	  con_p->dest_conpoint == dest_conpoint))
      continue;

    /* Check horizontal lines in the con */
    if  ( con_p->cc->con_type == flow_eConType_Routed && !con_p->temporary_ref)
    {
      if( con_p->source_direction == flow_eDirection_Right ||
          con_p->source_direction == flow_eDirection_Left)
        i = 0;
      else
        i = 1;

      for ( ; i < con_p->p_num - 1; i += 2)
      {
        if ( con_p->point_x[i] < con_p->point_x[i+1])
          u_x = con_p->point_x[i+1];
        else
          u_x = con_p->point_x[i];
        if ( con_p->point_y[i] - ctx->draw_delta < y &&
	     y < con_p->point_y[i] + ctx->draw_delta)
        {
          if ( u_x > *border_x && u_x <= start_x_con + CON_EPSILON)
            *border_x = u_x;
        }
      }
    }
  }
}

void FlowCon::find_vert_line_high_border( double x, double start_y,
		double start_y_con,
		double *border_y, FlowNode *nodelist, FlowCon *conlist)
{
  FlowNode *node_p;
  FlowCon *con_p;
  double l_y;
  int i;

  for ( node_p = nodelist; node_p; node_p = node_p->link)
  {
    if ( node_p->in_vert_line( x, start_y, *border_y))
      if ( node_p->obst_y_low < *border_y && node_p->obst_y_low >= start_y - CON_EPSILON)
        *border_y = node_p->obst_y_low;
  }
  for ( con_p = conlist; con_p; con_p = con_p->link)
  {
    if ( con_p == this)
      continue;
    if ( (con_p->source_node == source_node && 
	  con_p->source_conpoint == source_conpoint) ||
         (con_p->source_node == dest_node && 
	  con_p->source_conpoint == dest_conpoint) ||
         (con_p->dest_node == source_node && 
	  con_p->dest_conpoint == source_conpoint) ||
         (con_p->dest_node == dest_node && 
	  con_p->dest_conpoint == dest_conpoint))
      continue;

    /* Check vertical lines in the con */
    if  ( con_p->cc->con_type == flow_eConType_Routed && !con_p->temporary_ref)
    {
      if( con_p->source_direction == flow_eDirection_Right ||
          con_p->source_direction == flow_eDirection_Left)
        i = 1;
      else
        i = 0;

      for ( ; i < con_p->p_num - 1; i += 2)
      {
        if ( con_p->point_y[i] < con_p->point_y[i+1])
          l_y = con_p->point_y[i];
        else
          l_y = con_p->point_y[i+1];
        if ( con_p->point_x[i] - ctx->draw_delta < x &&
	     x < con_p->point_x[i] + ctx->draw_delta)
        {
          if ( l_y < *border_y && l_y >= start_y_con - CON_EPSILON)
            *border_y = l_y;
        }
      }
    }
  }
}

void FlowCon::find_vert_line_low_border( double x, double start_y,
		double start_y_con,
		double *border_y, FlowNode *nodelist, FlowCon *conlist)
{
  FlowNode *node_p;
  FlowCon *con_p;
  double u_y;
  int i;

  for ( node_p = nodelist; node_p; node_p = node_p->link)
  {
    if ( node_p->in_vert_line( x, *border_y, start_y))
      if ( node_p->obst_y_high > *border_y && node_p->obst_y_high <= start_y + CON_EPSILON)
        *border_y = node_p->obst_y_high;
  }
  for ( con_p = conlist; con_p; con_p = con_p->link)
  {
    if ( con_p == this)
      continue;
    if ( (con_p->source_node == source_node && 
	  con_p->source_conpoint == source_conpoint) ||
         (con_p->source_node == dest_node && 
	  con_p->source_conpoint == dest_conpoint) ||
         (con_p->dest_node == source_node && 
	  con_p->dest_conpoint == source_conpoint) ||
         (con_p->dest_node == dest_node && 
	  con_p->dest_conpoint == dest_conpoint))
      continue;

    /* Check vertical lines in the con */
    if  ( con_p->cc->con_type == flow_eConType_Routed && !con_p->temporary_ref)
    {
      if( con_p->source_direction == flow_eDirection_Right ||
          con_p->source_direction == flow_eDirection_Left)
        i = 1;
      else
        i = 0;

      for ( ; i < con_p->p_num - 1; i += 2)
      {
        if ( con_p->point_y[i] < con_p->point_y[i+1])
          u_y = con_p->point_y[i+1];
        else
          u_y = con_p->point_y[i];

        if ( con_p->point_x[i] - ctx->draw_delta < x &&
	     x < con_p->point_x[i] + ctx->draw_delta)
        {
          if ( u_y > *border_y && u_y <= start_y_con + CON_EPSILON)
            *border_y = u_y;
        }
      }
    }
  }
}

int FlowCon::event_handler( flow_eEvent event, int x, int y)
{
  int sts, i;

  sts = 0;
  switch ( event)
  {
    case flow_eEvent_CursorMotion:
      if ( temporary_ref || cc->con_type == flow_eConType_Reference)
        sts = ref_a.event_handler( &cc->zero, event, x, y, ref_a.size());
      else
        sts = line_a.event_handler( &cc->zero, event, x, y, l_num);
      if ( sts && !hot &&
	   !(ctx->node_movement_active || ctx->node_movement_paste_active))
      {
        ctx->fdraw->set_cursor( ctx, draw_eCursor_CrossHair);
        hot = 1;
        if ( temporary_ref || cc->con_type == flow_eConType_Reference)
          ref_a.draw( &cc->zero, highlight, hot, NULL);
        else
        {
          for ( i = 0; i < l_num; i++)
            ((FlowLine *)line_a[i])->draw( &cc->zero, highlight, hot, NULL);
          for ( i = 0; i < a_num; i++)
            ((FlowArc *)arc_a[i])->draw( &cc->zero, highlight, hot, NULL);
          arrow_a.draw( &cc->zero, highlight, hot, NULL);
        }
      }
      if ( !sts && hot)
      {
        ctx->fdraw->set_cursor( ctx, draw_eCursor_Normal);
        if ( temporary_ref || cc->con_type == flow_eConType_Reference)
          ref_a.erase( &cc->zero, hot, NULL);
        else
        {
          for ( i = 0; i < l_num; i++)
            ((FlowLine *)line_a[i])->erase( &cc->zero, hot, NULL);
          for ( i = 0; i < a_num; i++)
            ((FlowArc *)arc_a[i])->erase( &cc->zero, hot, NULL);
          arrow_a.erase( &cc->zero, hot, NULL);
        }
        hot = 0;
        if ( temporary_ref || cc->con_type == flow_eConType_Reference)
          ref_a.draw( &cc->zero, highlight, hot, NULL);
        else
        {
          for ( i = 0; i < l_num; i++)
            ((FlowLine *)line_a[i])->draw( &cc->zero, highlight, hot, NULL);
          for ( i = 0; i < a_num; i++)
            ((FlowArc *)arc_a[i])->draw( &cc->zero, highlight, hot, NULL);
          arrow_a.draw( &cc->zero, highlight, hot, NULL);
        }
      }
      break;
    default:
      if ( temporary_ref || cc->con_type == flow_eConType_Reference)
        sts = ref_a.event_handler( &cc->zero, event, x, y, ref_a.size());
      else
        sts = line_a.event_handler( &cc->zero, event, x, y, l_num);
  }
  if ( sts)
    ctx->register_callback_object( flow_eObjectType_Con, this);
  return sts;
}

ostream& operator<< ( ostream& o, const FlowCon c)
{
  o << "Con: " << c.c_name << " Class: " << c.cc->cc_name;
  return o;
}

void FlowCon::draw_routed( int points, double *x, double *y)
{
  int i;
  FlowLine *l;
  FlowArc *a;

  for ( i = 0; i < points - 1; i++)
  {
    l = (FlowLine *) line_a[i];
    l->move( &cc->zero, x[i], y[i],
		x[i+1], y[i+1], highlight, hot);
  }

  for ( i = points - 1; i < l_num; i++)
  {
    /* Remove lines that isn't used any longer */
    l = (FlowLine *) line_a[i];
    l->erase( &cc->zero, hot, NULL);
    l->nav_erase( &cc->zero, NULL);
  }
  for ( i = points - 2; i < a_num; i++)
  {
    /* Remove arcs that isn't used any longer */
    a = (FlowArc *) arc_a[i];
    a->erase( &cc->zero, hot, NULL);
    a->nav_erase( &cc->zero, NULL);
  }
  l_num = points - 1;
  p_num = points;
}

void FlowCon::draw_routed_trans( int points, double *x, double *y)
{
  int i, j;
  FlowLine *l;

  j = 0;
  for ( i = 0; i < points - 1; i++)
  {
    if ( i == 2)
      continue;
    l = (FlowLine *) line_a[j];
    l->move( &cc->zero, x[i], y[i],
		x[i+1], y[i+1], highlight, hot);
    j++;
  }

  for ( i = j; i < l_num; i++)
  {
    /* Remove lines that isn't used any longer */
    l = (FlowLine *) line_a[i];
    l->erase( &cc->zero, hot, NULL);
    l->nav_erase( &cc->zero, NULL);
  }
  l_num = j;
  p_num = points;
}

void FlowCon::draw_routed_roundcorner( int points, double *x, double *y)
{
  int i;
  double r = cc->round_corner_amount;
  double line_x1[8], line_y1[8], line_x2[8], line_y2[8];
  double arc_ll_x[8], arc_ll_y[8], arc_ur_x[8], arc_ur_y[8]; 
  int	 arc_angel1[8], arc_angel2[8];
  con_eCorner corner_type[8];
  FlowLine *l;
  FlowArc *a;
  double r_x[8], r_y[8];

  for ( i = 1; i < points-1; i++)
  {
    if ( i == 1 && fabs(y[0] - y[1]) < DBL_EPSILON &&
         fabs(x[0] - x[1]) < DBL_EPSILON)
    {
      /* First line is a Null line */
      corner_type[0] = eCorner_Sharp;
      if ( i != points-2)
      {
        corner_type[i] = eCorner_Sharp;
        i++;
        continue;
      }
    }

    /* Find corner type */
    if ( fabs(y[i-1] - y[i]) < DBL_EPSILON)
    {
      if ( fabs(y[i] - y[i+1]) < DBL_EPSILON)
      {
        /* Next line is a Null line */
        corner_type[i-1] = eCorner_Sharp;
        if ( i != points-2)
        {
          corner_type[i] = eCorner_Sharp;
          i++;
          continue;
        }
      }

      if ( x[i-1] < x[i] && y[i] < y[i+1])
        corner_type[i-1] = eCorner_RightToUp;
      else if ( x[i-1] < x[i] && y[i] > y[i+1])
        corner_type[i-1] = eCorner_RightToDown;
      else if ( x[i-1] > x[i] && y[i] < y[i+1])
        corner_type[i-1] = eCorner_LeftToUp;
      else if ( x[i-1] > x[i] && y[i] > y[i+1])
        corner_type[i-1] = eCorner_LeftToDown;

      /* Check if short line */
      if ( fabs(x[i] - x[i-1]) < 2 * r)
      {
        r_x[i-1] = fabs( x[i] - x[i-1]) / 2;
        if ( i > 1)
          r_x[i-2] = r_x[i-1];
      }
      else
        r_x[i-1] = r;
      r_y[i-1] = r;
    }
    else if ( fabs(x[i-1] - x[i]) < DBL_EPSILON)
    {
      if ( fabs(x[i] - x[i+1]) < DBL_EPSILON)
      {
        /* Next line is a Null line */
        corner_type[i-1] = eCorner_Sharp;
        if ( i != points-2)
        {
          corner_type[i] = eCorner_Sharp;
          i++;
          continue;
        }
      }

      if ( y[i-1] < y[i] && x[i] < x[i+1])
        corner_type[i-1] = eCorner_UpToRight;
      else if ( y[i-1] < y[i] && x[i] > x[i+1])
        corner_type[i-1] = eCorner_UpToLeft;
      else if ( y[i-1] > y[i] && x[i] < x[i+1])
        corner_type[i-1] = eCorner_DownToRight;
      else if ( y[i-1] > y[i] && x[i] > x[i+1])
        corner_type[i-1] = eCorner_DownToLeft;

      /* Check if short line */
      if ( fabs(y[i] - y[i-1]) < 2 * r)
      {
        r_y[i-1] = fabs( y[i] - y[i-1]) / 2;
        if ( i > 1)
          r_y[i-2] = r_y[i-1];
      }
      else
        r_y[i-1] = r;
      r_x[i-1] = r;
    }
    else
      corner_type[i-1] = eCorner_Sharp;
  }
  line_x1[0] = x[0];
  line_y1[0] = y[0];
  for ( i = 1; i < points-1; i++)
  {
    switch( corner_type[i-1])
    {
      case eCorner_RightToUp:
      case eCorner_RightToDown:
      {
        line_x2[i-1] = x[i] - r_x[i-1];
        line_y2[i-1] = y[i];
        switch( corner_type[i-1])
        {
          case eCorner_RightToUp:
            line_x1[i] = x[i];
            line_y1[i] = y[i] + r_y[i-1];
            arc_ll_x[i-1] = x[i] - 2*r_x[i-1];
            arc_ll_y[i-1] = y[i];
            arc_ur_x[i-1] = x[i];
            arc_ur_y[i-1] = y[i] + 2*r_y[i-1];
            arc_angel1[i-1] = 0;
            arc_angel2[i-1] = 90;
            break;
          case eCorner_RightToDown:
            line_x1[i] = x[i];
            line_y1[i] = y[i] - r_y[i-1];
            arc_ll_x[i-1] = x[i] - 2*r_x[i-1];
            arc_ll_y[i-1] = y[i] - 2*r_y[i-1];
            arc_ur_x[i-1] = x[i];
            arc_ur_y[i-1] = y[i];
            arc_angel1[i-1] = 270;
            arc_angel2[i-1] = 90;
            break;
          default:
            ;
        }
        break;
      }
      case eCorner_LeftToUp:
      case eCorner_LeftToDown:
      {
        line_x2[i-1] = x[i] + r_x[i-1];
        line_y2[i-1] = y[i];
        switch( corner_type[i-1])
        {
          case eCorner_LeftToUp:
            line_x1[i] = x[i];
            line_y1[i] = y[i] + r_y[i-1];
            arc_ll_x[i-1] = x[i];
            arc_ll_y[i-1] = y[i];
            arc_ur_x[i-1] = x[i] + 2*r_x[i-1];
            arc_ur_y[i-1] = y[i] + 2*r_y[i-1];
            arc_angel1[i-1] = 90;
            arc_angel2[i-1] = 90;
            break;
          case eCorner_LeftToDown:
            line_x1[i] = x[i];
            line_y1[i] = y[i] - r_y[i-1];
            arc_ll_x[i-1] = x[i];
            arc_ll_y[i-1] = y[i] - 2*r_y[i-1];
            arc_ur_x[i-1] = x[i] + 2*r_x[i-1];
            arc_ur_y[i-1] = y[i];
            arc_angel1[i-1] = 180;
            arc_angel2[i-1] = 90;
            break;
	  default:
            ;
        }
        break;
      }
      case eCorner_UpToRight:
      case eCorner_UpToLeft:
      {
        line_x2[i-1] = x[i];
        line_y2[i-1] = y[i] - r_y[i-1];
        switch( corner_type[i-1])
        {
          case eCorner_UpToRight:
            line_x1[i] = x[i] + r_x[i-1];
            line_y1[i] = y[i];
            arc_ll_x[i-1] = x[i];
            arc_ll_y[i-1] = y[i] - 2*r_y[i-1];
            arc_ur_x[i-1] = x[i] + 2*r_x[i-1];
            arc_ur_y[i-1] = y[i];
            arc_angel1[i-1] = 180;
            arc_angel2[i-1] = 90;
            break;
          case eCorner_UpToLeft:
            line_x1[i] = x[i] - r_x[i-1];
            line_y1[i] = y[i];
            arc_ll_x[i-1] = x[i] - 2*r_x[i-1];
            arc_ll_y[i-1] = y[i] - 2*r_y[i-1];
            arc_ur_x[i-1] = x[i];
            arc_ur_y[i-1] = y[i];
            arc_angel1[i-1] = 270;
            arc_angel2[i-1] = 90;
            break;
          default:
            ;
        }
        break;
      }
      case eCorner_DownToRight:
      case eCorner_DownToLeft:
      {
        line_x2[i-1] = x[i];
        line_y2[i-1] = y[i] + r_y[i-1];
        switch( corner_type[i-1])
        {
          case eCorner_DownToRight:
            line_x1[i] = x[i] + r_x[i-1];
            line_y1[i] = y[i];
            arc_ll_x[i-1] = x[i];
            arc_ll_y[i-1] = y[i];
            arc_ur_x[i-1] = x[i] + 2*r_x[i-1];
            arc_ur_y[i-1] = y[i] + 2*r_y[i-1];
            arc_angel1[i-1] = 90;
            arc_angel2[i-1] = 90;
            break;
          case eCorner_DownToLeft:
            line_x1[i] = x[i] - r_x[i-1];
            line_y1[i] = y[i];
            arc_ll_x[i-1] = x[i] - 2*r_x[i-1];
            arc_ll_y[i-1] = y[i];
            arc_ur_x[i-1] = x[i];
            arc_ur_y[i-1] = y[i] + 2*r_y[i-1];
            arc_angel1[i-1] = 0;
            arc_angel2[i-1] = 90;
            break;
          default:
            ;
        }
        break;
      }
      case eCorner_Sharp:
        line_x2[i-1] = x[i];
        line_y2[i-1] = y[i];
        line_x1[i] = x[i];
        line_y1[i] = y[i];
        arc_ll_x[i-1] = 0;
        arc_ll_y[i-1] = 0;
        arc_ur_x[i-1] = 0;
        arc_ur_y[i-1] = 0;
        break;
    }
  }
  line_x2[points-2] = x[points-1];
  line_y2[points-2] = y[points-1];

  for ( i = 0; i < points - 1; i++)
  {
    l = (FlowLine *) line_a[i];
    l->move( &cc->zero, line_x1[i], line_y1[i],
		line_x2[i], line_y2[i], highlight, hot);
  }
  for ( i = 0; i < points - 2; i++)
  {
    a = (FlowArc *) arc_a[i];
    a->move( &cc->zero, arc_ll_x[i], arc_ll_y[i],
		arc_ur_x[i], arc_ur_y[i], arc_angel1[i], arc_angel2[i],
		highlight, hot);
  }
  for ( i = points - 1; i < l_num; i++)
  {
    /* Remove lines that isn't used any longer */
    l = (FlowLine *) line_a[i];
    l->erase( &cc->zero, hot, NULL);
    l->nav_erase( &cc->zero, NULL);
  }
  for ( i = points - 2; i < a_num; i++)
  {
    /* Remove arcs that isn't used any longer */
    a = (FlowArc *) arc_a[i];
    a->erase( &cc->zero, hot, NULL);
    a->nav_erase( &cc->zero, NULL);
  }
  l_num = points - 1;
  p_num = points;
  a_num = points - 2;
}

void FlowCon::set_movement_type( FlowArrayElem **a, int a_size)
{
  int i;
  int source_found = 0;
  int dest_found = 0;

  for ( i = 0; i < a_size; i++)
  {
    if ( a[i] == (FlowArrayElem *) source_node)
      source_found = 1; 
    if ( a[i] == (FlowArrayElem *) dest_node)
      dest_found = 1; 
    if ( source_found && dest_found)
      break;
  }
  if ( source_found && dest_found)
    movement_type = flow_eMoveType_Frozen;
  else
    movement_type = flow_eMoveType_Route;
}

void FlowCon::move_ref( double x1, double y1, double x2, double y2)
{
  FlowText *t1, *t2;
  FlowRect *r1, *r2;
  double text_x, text_y, rect_x, rect_y;
  int	new_ref;
  char reftext[20];

  new_ref = ( ref_a.size() == 0);
    
  switch ( source_direction)
  {
    case flow_eDirection_Center:
      rect_x = x1 - ctx->refcon_width / 2;
      rect_y = y1 - (source_ref_cnt - 0.5) * ctx->refcon_height;
      break;
    case flow_eDirection_Right:
      rect_x = x1 + source_ref_cnt * ctx->refcon_width;
      rect_y = y1 - ctx->refcon_height / 2;
      break;
    case flow_eDirection_Left:
      rect_x = x1 - (source_ref_cnt + 1) * ctx->refcon_width;
      rect_y = y1 - ctx->refcon_height / 2;
      break;
    case flow_eDirection_Up:
      rect_x = x1 - ctx->refcon_width / 2;
      rect_y = y1 + source_ref_cnt * ctx->refcon_height;
      break;
    case flow_eDirection_Down:
      rect_x = x1 - ctx->refcon_width / 2;
      rect_y = y1 - (source_ref_cnt + 1) * ctx->refcon_height;
      break;
  }
  text_x = rect_x + 0.2 * ctx->refcon_width;
  text_y = rect_y + 0.8 * ctx->refcon_height;

  if ( new_ref)
  {
    sprintf( reftext, "R%d", ctx->refcon_cnt++);
    r1 = new FlowRect( ctx, rect_x, rect_y, ctx->refcon_width, 
		ctx->refcon_height, flow_eDrawType_Line, ctx->refcon_linewidth);
    t1 = new FlowText( ctx, reftext, text_x, text_y, 
		flow_eDrawType_TextHelveticaBold, ctx->refcon_textsize);
    ref_a.insert( r1);
    ref_a.insert( t1);
  }
  else
  {
    r1 = (FlowRect *)ref_a[0];
    t1 = (FlowText *)ref_a[1];
    r1->move( &cc->zero, rect_x, rect_y, highlight, hot);
    t1->move( &cc->zero, text_x, text_y, highlight, hot);
  }
  switch ( dest_direction)
  {
    case flow_eDirection_Center:
      rect_x = x2 - ctx->refcon_width / 2;
      rect_y = y2 - (dest_ref_cnt - 0.5) * ctx->refcon_height;
      break;
    case flow_eDirection_Right:
      rect_x = x2 + dest_ref_cnt * ctx->refcon_width;
      rect_y = y2 - ctx->refcon_height / 2;
      break;
    case flow_eDirection_Left:
      rect_x = x2 - (dest_ref_cnt + 1) * ctx->refcon_width;
      rect_y = y2 - ctx->refcon_height / 2;
      break;
    case flow_eDirection_Up:
      rect_x = x2 - ctx->refcon_width / 2;
      rect_y = y2 + dest_ref_cnt * ctx->refcon_height;
      break;
    case flow_eDirection_Down:
      rect_x = x2 - ctx->refcon_width / 2;
      rect_y = y2 - (dest_ref_cnt + 1) * ctx->refcon_height;
      break;
  }
  text_x = rect_x + 0.2 * ctx->refcon_width;
  text_y = rect_y + 0.8 * ctx->refcon_height;
  if ( new_ref)
  {
    r2 = new FlowRect( ctx, rect_x, rect_y, ctx->refcon_width, 
		ctx->refcon_height, flow_eDrawType_Line, ctx->refcon_linewidth);
    t2 = new FlowText( ctx, reftext, text_x, text_y, 
		flow_eDrawType_TextHelveticaBold, ctx->refcon_textsize);
    ref_a.insert( r2);
    ref_a.insert( t2);
  }
  else
  {
    r2 = (FlowRect *)ref_a[2];
    t2 = (FlowText *)ref_a[3];
    r2->move( &cc->zero, rect_x, rect_y, highlight, hot);
    t2->move( &cc->zero, text_x, text_y, highlight, hot);
  }
}

void FlowCon::conpoint_refcon_erase( void *node, int conpoint)
{
  FlowText *t1;
  FlowRect *r1;

  if ( source_node == (FlowNode *)node && conpoint == source_conpoint)
  {
    r1 = (FlowRect *)ref_a[0];
    t1 = (FlowText *)ref_a[1];
    t1->erase( &cc->zero, hot, NULL);
    t1->nav_erase( &cc->zero, NULL);
    r1->erase( &cc->zero, hot, NULL);
    r1->nav_erase( &cc->zero, NULL);
  }
  else if ( dest_node == (FlowNode *)node && conpoint == dest_conpoint)
  {
    r1 = (FlowRect *)ref_a[2];
    t1 = (FlowText *)ref_a[3];
    t1->erase( &cc->zero, hot, NULL);
    t1->nav_erase( &cc->zero, NULL);
    r1->erase( &cc->zero, hot, NULL);
    r1->nav_erase( &cc->zero, NULL);
  }
}

void FlowCon::conpoint_refcon_redraw( void *node, int conpoint)
{
  double x1, y1, x2, y2;
  flow_eDirection dir;

  if ( !( temporary_ref || cc->con_type == flow_eConType_Reference))
    return;

  if ( source_node == (FlowNode *)node && conpoint == source_conpoint)
  {
    source_node->get_conpoint( source_conpoint, &x1, &y1, &dir);
    dest_node->get_conpoint( dest_conpoint, &x2, &y2, &dir);
    source_ref_cnt = source_node->refcon_cnt[conpoint]++;
    move_ref( x1, y1, x2, y2);
  }
  else if ( dest_node == (FlowNode *)node && conpoint == dest_conpoint)
  {
    source_node->get_conpoint( source_conpoint, &x1, &y1, &dir);
    dest_node->get_conpoint( dest_conpoint, &x2, &y2, &dir);
    dest_ref_cnt = dest_node->refcon_cnt[conpoint]++;
    move_ref( x1, y1, x2, y2);
  }
}

void FlowCon::remove_notify()
{
  if ( temporary_ref || cc->con_type == flow_eConType_Reference)
  {
    source_node->conpoint_refcon_reconfig( source_conpoint);
    dest_node->conpoint_refcon_reconfig( dest_conpoint);
  }
}

void FlowCon::trace_scan()
{
  int on;

  if ( !trace_p)
    return;
  switch( trace_attr_type)
  {
    case flow_eTraceType_Boolean:
      on = *(unsigned int *) trace_p;
      if ( highlight != on)
        set_highlight( on);
      break;
    default:
      break;
  }
}

int FlowCon::trace_init()
{
  int sts;

  if ( strcmp( trace_object, "") == 0)
    return 1;

  sts = ctx->trace_connect_func( (void *) this, trace_object, trace_attribute, 
	trace_attr_type, &trace_p);
  return sts;
}

void FlowCon::trace_close()
{
  if ( strcmp( trace_object, "") == 0)
    return;

  ctx->trace_disconnect_func( (void *) this);
}

void FlowCon::get_object_name( char *name)
{
  strcpy( name, c_name);
}

