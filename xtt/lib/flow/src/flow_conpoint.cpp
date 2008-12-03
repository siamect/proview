/* 
 * Proview   $Id: flow_conpoint.cpp,v 1.6 2008-12-03 12:01:21 claes Exp $
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
#include <math.h>

#include "flow_conpoint.h"
#include "flow_draw.h"
#include "flow_node.h"

void FlowConPoint::zoom()
{
  p.zoom();
}

void FlowConPoint::nav_zoom()
{
  p.nav_zoom();
}

void FlowConPoint::print_zoom()
{
  p.print_zoom();
}

void FlowConPoint::save( ofstream& fp, flow_eSaveMode mode)
{
  fp <<	int(flow_eSave_ConPoint) << endl;
  fp <<	int(flow_eSave_ConPoint_number) << FSPACE << number << endl;
  fp <<	int(flow_eSave_ConPoint_direction) << FSPACE << int(direction) << endl;
  fp <<	int(flow_eSave_ConPoint_p) << endl;
  p.save( fp, mode);
  fp << int(flow_eSave_ConPoint_trace_attribute) << FSPACE << trace_attribute << endl;
  fp << int(flow_eSave_ConPoint_trace_attr_type) << FSPACE << int(trace_attr_type) << endl;
  fp <<	int(flow_eSave_End) << endl;
}

void FlowConPoint::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_ConPoint: break;
      case flow_eSave_ConPoint_number: fp >> number; break;
      case flow_eSave_ConPoint_direction: fp >> tmp; direction = (flow_eDirection)tmp; break;
      case flow_eSave_ConPoint_p: p.open( fp); break;
      case flow_eSave_ConPoint_trace_attribute:
        fp.get();
        fp.getline( trace_attribute, sizeof(trace_attribute));
        break;
      case flow_eSave_ConPoint_trace_attr_type: fp >> tmp; trace_attr_type = (flow_eTraceType)tmp; break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowConPoint:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}


void FlowConPoint::traverse( int x, int y)
{
  p.traverse( x, y);
}

int FlowConPoint::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  return 0;
}

void FlowConPoint::conpoint_select( void *pos, int x, int y, 
	double *distance, void **cp)
{
  int px, py;
  double dist;

  px = ((FlowPoint *)pos)->z_x - ctx->offset_x + p.z_x;
  py = ((FlowPoint *)pos)->z_y - ctx->offset_y + p.z_y;

  dist = sqrt( 1.0*(x-px)*(x-px) + 1.0*(y-py)*(y-py));
  if ( dist < *distance)
  {
    *distance = dist;
    *cp = (void *) this;
  }
}

int FlowConPoint::get_conpoint( int num, double *x, double *y,
	flow_eDirection *dir)
{
  if ( number == num)
  {
    *x = p.x;
    *y = p.y;
    *dir = direction;
    return 1;
  }
  return 0;
}

void FlowConPoint::set_trace_attr( const char *object, const char *attribute, 
	flow_eTraceType type, int inverted)
{
  strncpy( trace_attribute, attribute, sizeof( trace_attribute));
  trace_attr_type = type;
}

void FlowConPoint::get_trace_attr( char *object, char *attribute, 
	flow_eTraceType *type, int *inverted)
{
  strncpy( attribute, trace_attribute, sizeof( trace_attribute));
  *type = trace_attr_type;
  *inverted = 0;
}

void FlowConPoint::draw( void *pos, int highlight, int hot, void *node)
{
  int line_width = 1;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1) + 2;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  int size = (int)(0.025 * ctx->zoom_factor);
  int x, y;

  if ( ((FlowNode *)node)->sel_conpoint1 == number ||
       ((FlowNode *)node)->sel_conpoint2 == number) {
    switch ( direction) {
    case flow_eDirection_Up:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x - size/2;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y - size;;
      break;
    case flow_eDirection_Down:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x - size/2;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y;
      break;
    case flow_eDirection_Right:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x - size;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y - size/2;
      break;
    case flow_eDirection_Left:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y - size/2;
      break;
    default:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x - size/2;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y - size/2;
    }
    ctx->fdraw->arc( ctx, x, y, size, size, 0, 360,
		     flow_eDrawType_LineRed, idx, 0);
  }
}

void FlowConPoint::erase( void *pos, int hot, void *node)
{
  int line_width = 1;
  int idx = int( ctx->zoom_factor / ctx->base_zoom_factor * line_width - 1) + 2;
  idx = MAX( 0, idx);
  idx = MIN( idx, DRAW_TYPE_SIZE-1);
  int size = (int)(0.025 * ctx->zoom_factor);
  int x, y;

  if ( ((FlowNode *)node)->sel_conpoint1 == number ||
       ((FlowNode *)node)->sel_conpoint2 == number) {
    switch ( direction) {
    case flow_eDirection_Up:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x - size/2;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y - size;;
      break;
    case flow_eDirection_Down:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x - size/2;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y;
      break;
    case flow_eDirection_Right:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x - size;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y - size/2;
      break;
    case flow_eDirection_Left:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y - size/2;
      break;
    default:
      x = p.z_x + ((FlowPoint *)pos)->z_x - ctx->offset_x - size/2;
      y = p.z_y + ((FlowPoint *)pos)->z_y - ctx->offset_y - size/2;
    }
    ctx->fdraw->arc_erase( ctx, x, y, size, size, 0, 360, idx);
  }
}

ostream& operator<< ( ostream& o, const FlowConPoint cp)
{
  o << '(' << cp.p.x << ',' << cp.p.y << ')'; 
  return o;
}

