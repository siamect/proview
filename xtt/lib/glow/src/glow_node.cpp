/* 
 * Proview   $Id: glow_node.cpp,v 1.4 2007-01-04 07:57:39 claes Exp $
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
#include "glow_node.h"
#include "glow_draw.h"
#include "glow_conpoint.h"
#include "glow_browctx.h"
#include "glow_growctx.h"
#include "glow_tracedata.h"
#include "glow_grownode.h"

GlowNode::GlowNode( GrowCtx *glow_ctx, char *name, GlowNodeClass *node_class,
	double x1, double y1, int nodraw, int rel_annot_pos) : 
	x_right(x1), x_left(x1), y_high(y1), y_low(y1), 
	obst_x_right(x1), obst_x_left(x1), obst_y_high(y1), obst_y_low(y1),
	hot(0), ctx(glow_ctx), nc(node_class), nc_root(node_class), 
	pos(glow_ctx, x1,y1), stored_pos(glow_ctx, x1, y1),
	highlight(0), inverse(0), user_data(0), level(0), node_open(0),
	relative_annot_pos(rel_annot_pos), relative_annot_x(0), input_active(0),
	input_focus(0)
{
  double x_grid, y_grid;
  strncpy( n_name, name, sizeof(n_name));
  n_name[sizeof(n_name)-1] = 0;
  memset( refcon_cnt, 0, sizeof(refcon_cnt));
  memset( annotv, 0, sizeof(annotv));
  memset( annotsize, 0, sizeof(annotsize));
  memset( annotv_inputmode, 0, sizeof(annotv_inputmode));
  memset( annotv_input, 0, sizeof(annotv_input));
  memset( rbuttonv, 0, sizeof(rbuttonv));
  memset( rel_annot_x, 0, sizeof(rel_annot_x));
  if ( !nc)
    return;
  if ( ctx->grid_on) {
    ctx->find_grid( x1, y1, &x_grid, &y_grid);
    pos.posit( x_grid, y_grid);
  }
  x_left = y_low = 1e37;
  x_right = y_high = -1e37;
  get_node_borders();
  if ( nc->group == glow_eNodeGroup_Document) {
    obst_x_left = obst_y_low = 1e37;
    obst_x_right = obst_y_high = -1e37;
    get_node_obstacle_borders();
  }
  else {
    obst_x_left = x_left;
    obst_x_right = x_right;
    obst_y_low = y_low;
    obst_y_high = y_high;
  }
  zoom();
}

GlowNode::~GlowNode()
{
  if ( !nc)
    return;
  ctx->object_deleted( this);
  if ( ctx->nodraw) return;
  if ( ctx->type() == glow_eCtxType_Grow)
    return;

  erase();
  nav_erase();

  ctx->set_defered_redraw();
  ctx->delete_node_cons( this);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->nav_draw(  &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
  ctx->redraw_defered();
  if ( hot)
    ctx->gdraw->set_cursor( &ctx->mw, glow_eDrawCursor_Normal);
}

int GlowNode::get_conpoint( int num, double *x, double *y, glow_eDirection *dir)
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

void GlowNode::save( ofstream& fp, glow_eSaveMode mode) 
{ 
  int i;
  char *s;

  if ( (mode == glow_eSaveMode_Trace && nc->group != glow_eNodeGroup_Trace) ||
       (mode == glow_eSaveMode_Edit && nc->group == glow_eNodeGroup_Trace) )
    return;

  fp << int(glow_eSave_Node) << endl;
  fp << int(glow_eSave_Node_nc) << FSPACE << nc->nc_name << endl;
  fp << int(glow_eSave_Node_n_name) << FSPACE << n_name << endl;
  fp << int(glow_eSave_Node_refcon_cnt) << endl;
  for ( i = 0; i < MAX_CONPOINTS; i++)
    fp << refcon_cnt[i] << endl;
  fp << int(glow_eSave_Node_x_right) << FSPACE << x_right << endl;
  fp << int(glow_eSave_Node_x_left) << FSPACE << x_left << endl;
  fp << int(glow_eSave_Node_y_high) << FSPACE << y_high << endl;
  fp << int(glow_eSave_Node_y_low) << FSPACE << y_low << endl;
  fp << int(glow_eSave_Node_obst_x_right) << FSPACE << obst_x_right << endl;
  fp << int(glow_eSave_Node_obst_x_left) << FSPACE << obst_x_left << endl;
  fp << int(glow_eSave_Node_obst_y_high) << FSPACE << obst_y_high << endl;
  fp << int(glow_eSave_Node_obst_y_low) << FSPACE << obst_y_low << endl;
  fp << int(glow_eSave_Node_annotsize) << endl;
  for ( i = 0; i < 10; i++)
    fp << annotsize[i] << endl;
  fp << int(glow_eSave_Node_annotv) << endl;
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
  fp << int(glow_eSave_Node_pos) << endl;
  pos.save( fp, mode);
  fp << int(glow_eSave_Node_trace_data1) << FSPACE << trace.data[0] << endl;
  fp << int(glow_eSave_Node_trace_data2) << FSPACE << trace.data[1] << endl;
  fp << int(glow_eSave_Node_trace_data3) << FSPACE << trace.data[2] << endl;
  fp << int(glow_eSave_Node_trace_data4) << FSPACE << trace.data[3] << endl;
  fp << int(glow_eSave_Node_trace_data5) << FSPACE << trace.data[4] << endl;
  fp << int(glow_eSave_Node_trace_data6) << FSPACE << trace.data[5] << endl;
  fp << int(glow_eSave_Node_trace_data7) << FSPACE << trace.data[6] << endl;
  fp << int(glow_eSave_Node_trace_data8) << FSPACE << trace.data[7] << endl;
  fp << int(glow_eSave_Node_trace_data9) << FSPACE << trace.data[8] << endl;
  fp << int(glow_eSave_Node_trace_data10) << FSPACE << trace.data[9] << endl;
  fp << int(glow_eSave_Node_trace_attr_type) << FSPACE << int(trace.attr_type) << endl;
  fp << int(glow_eSave_Node_trace_color) << FSPACE << int(trace.color) << endl;
  fp << int(glow_eSave_Node_trace_color2) << FSPACE << int(trace.color2) << endl;
  fp << int(glow_eSave_Node_access) << FSPACE << (unsigned int)(trace.access) << endl;
  fp << int(glow_eSave_Node_cycle) << FSPACE << (unsigned int)(trace.cycle) << endl;
  fp << int(glow_eSave_Node_ref_object) << FSPACE << trace.ref_object << endl;
  fp << int(glow_eSave_End) << endl;  
}

void GlowNode::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  char		nc_name[80];
  int		i, j;
  char		c;
  int		tmp;
  unsigned int	utmp;
  int		sts;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_Node: break;
      case glow_eSave_Node_nc:
        fp.get();
        fp.getline( nc_name, sizeof(nc_name));
        if ( ctx->type() != glow_eCtxType_Grow ||
             ((GrowNode *)this)->type() != glow_eObjectType_GrowGroup)
        {
          nc = (GlowNodeClass *) ctx->get_nodeclass_from_name( nc_name);
          if ( !nc && ctx->type() == glow_eCtxType_Grow)
          {
            // If grow, load subgraph
            sts = ctx->open_subgraph_from_name( nc_name, glow_eSaveMode_SubGraph);
            if ( ODD(sts))
            {
              nc = (GlowNodeClass *) ctx->get_nodeclass_from_name( nc_name);
              if ( nc) 
                nc->nc_extern = 1;
            }
          }
          if ( !nc) 
            cout << "GlowNode:nodeclass not found: " << nc_name << endl;
          nc_root = nc;
        }
        break;
      case glow_eSave_Node_n_name:
        fp.get();
        fp.getline( n_name, sizeof(n_name));
        break;
      case glow_eSave_Node_refcon_cnt:
        for ( i = 0; i < MAX_CONPOINTS; i++)
          fp >> refcon_cnt[i];
        break;
      case glow_eSave_Node_x_right: fp >> x_right; break;
      case glow_eSave_Node_x_left: fp >> x_left; break;
      case glow_eSave_Node_y_high: fp >> y_high; break;
      case glow_eSave_Node_y_low: fp >> y_low; break;
      case glow_eSave_Node_obst_x_right: fp >> obst_x_right; break;
      case glow_eSave_Node_obst_x_left: fp >> obst_x_left; break;
      case glow_eSave_Node_obst_y_high: fp >> obst_y_high; break;
      case glow_eSave_Node_obst_y_low: fp >> obst_y_low; break;
      case glow_eSave_Node_annotsize:
        for ( i = 0; i < 10; i++)
          fp >> annotsize[i];
        break;
      case glow_eSave_Node_annotv:
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
	        if ( annotv[i][j-1] == '\\')
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
      case glow_eSave_Node_pos: pos.open( fp); break;
      case glow_eSave_Node_trace_data1:
        fp.get();
        fp.getline( trace.data[0], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data2:
        fp.get();
        fp.getline( trace.data[1], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data3:
        fp.get();
        fp.getline( trace.data[2], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data4:
        fp.get();
        fp.getline( trace.data[3], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data5:
        fp.get();
        fp.getline( trace.data[4], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data6:
        fp.get();
        fp.getline( trace.data[5], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data7:
        fp.get();
        fp.getline( trace.data[6], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data8:
        fp.get();
        fp.getline( trace.data[7], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data9:
        fp.get();
        fp.getline( trace.data[8], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_data10:
        fp.get();
        fp.getline( trace.data[9], sizeof(trace.data[0]));
        break;
      case glow_eSave_Node_trace_attr_type: fp >> tmp; trace.attr_type = (glow_eTraceType)tmp; break;
      case glow_eSave_Node_trace_color: fp >> tmp; trace.color = (glow_eDrawType)tmp; break;
      case glow_eSave_Node_trace_color2: fp >> tmp; trace.color2 = (glow_eDrawType)tmp; break;
      case glow_eSave_Node_access: fp >> utmp; trace.access = (glow_mAccess)utmp; break;
      case glow_eSave_Node_cycle: fp >> tmp; trace.cycle = (glow_eCycle)tmp; break;
      case glow_eSave_Node_ref_object:
        fp.get();
        fp.getline( trace.ref_object, sizeof(trace.ref_object));
        break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowNode:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
  if ( ctx->trace_started)
    trace_init();
}

void GlowNode::select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy)
{
  if ( select_policy == glow_eSelectPolicy_Surround || 
       nc->group == glow_eNodeGroup_Document) {
    if ( x_left > ll_x && x_right < ur_x && y_high < ur_y && y_low > ll_y)
      ctx->select_insert( this);
  }
  else {
    if ( x_right > ll_x && x_left < ur_x && y_low < ur_y && y_high > ll_y)
      ctx->select_insert( this);
  }
}

void GlowNode::get_annotation( int num, char *text, int size)
{
  if ( !annotv[num])
    strcpy( text, "");
  else
  {
    strncpy( text, annotv[num], size);
    text[size-1] = 0;
  }
}

void GlowNode::conpoint_refcon_reconfig( int conpoint)
{
//  ctx->conpoint_refcon_erase( this, conpoint);
  refcon_cnt[conpoint] = 0;
  ctx->conpoint_refcon_redraw( this, conpoint);
}

void GlowNode::remove_notify()
{
  ctx->delete_node_cons( this);
}

void GlowNode::set_trace_attr( GlowTraceData *attr)
{
  memcpy( &trace, attr, sizeof(trace));

  if ( ctx->trace_started /* && strcmp( trace.data[0], "") != 0 */)
    ctx->trace_connect_func( (void *) this, &trace);
}

void GlowNode::get_trace_attr( GlowTraceData **attr)
{
  *attr = &trace;
}

void GlowNode::trace_scan()
{
  if ( ctx->trace_scan_func && trace.p)
    ctx->trace_scan_func( (void *) this, trace.p);
}

int GlowNode::trace_init()
{
  int sts;

  // if ( strcmp( trace.data[0], "") == 0)
  //  return 1;

  sts = ctx->trace_connect_func( (void *) this, &trace);
  return sts;
}

void GlowNode::trace_close()
{
  // if ( strcmp( trace.data[0], "") == 0)
  //  return;

  if ( trace.p)
    ctx->trace_disconnect_func( (void *) this);
}



