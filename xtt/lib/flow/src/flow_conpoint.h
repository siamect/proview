/* 
 * Proview   $Id: flow_conpoint.h,v 1.5 2008-10-03 14:19:19 claes Exp $
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

#ifndef flow_conpoint_h
#define flow_conpoint_h

#include <iostream.h>
#include <fstream.h>
#include "flow.h"
#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_array_elem.h"


class FlowConPoint : public FlowArrayElem {
  public:
    FlowConPoint( FlowCtx *flow_ctx, double x = 0, double y = 0, 
	int cp_num = 0, flow_eDirection d = flow_eDirection_Center) : 
	ctx(flow_ctx), number(cp_num), p(flow_ctx,x,y), direction(d),
	trace_attr_type(flow_eTraceType_Boolean)
	{ strcpy( trace_attribute, ""); };
    friend ostream& operator<< ( ostream& o, const FlowConPoint cp);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    int	event_handler( void *pos, flow_eEvent event, int x, int y, void *node);
    void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp);
    void print( void *pos, void *node) {};
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int highlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node) {};
    void erase( void *pos, int hot, void *node);
    void nav_erase( void *pos, void *node) {};
    void get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node) {};
    int get_conpoint( int num, double *x, double *y, flow_eDirection *dir);
    flow_eObjectType type() { return flow_eObjectType_ConPoint;};
    void set_trace_attr( char *object, char *attribute, flow_eTraceType type, int inverted);
    void get_trace_attr( char *object, char *attribute, flow_eTraceType *type, int *inverted);
    FlowCtx *ctx;
    int	number;
    FlowPoint p;
    flow_eDirection direction;
    flow_tTraceAttr trace_attribute;
    flow_eTraceType trace_attr_type;
};

#endif
