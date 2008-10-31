/* 
 * Proview   $Id: flow_conclass.h,v 1.3 2008-10-31 12:51:33 claes Exp $
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

#ifndef flow_conclass_h
#define flow_conclass_h

#include <iostream>
#include <fstream>
#include "flow_point.h"
#include "flow_rect.h"
#include "flow_array_elem.h"
#include "flow_array.h"

class FlowConClass : public FlowArrayElem {
  public:
    FlowConClass( FlowCtx *flow_ctx, const char *name, flow_eConType contype,
	flow_eCorner cornertype, flow_eDrawType d_type, int line_w,
	double arrow_w = 0, double arrow_l = 0, double round_corner_amnt = 0.5,
	flow_eConGroup grp = flow_eConGroup_Common);
    friend ostream& operator<< ( ostream& o, const FlowConClass cc);
    void zoom() {};
    void nav_zoom() {};
    void print_zoom() {};
    void traverse( int x, int y) {};
    void get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node)
	{};
    int	event_handler( void *pos, flow_eEvent event, int x, int y, void *node)
		 { return 0;};
    void print( FlowPoint *pos, void *node) {};
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( FlowPoint *pos, int highlight, int hot, void *node) {};
    void nav_draw( FlowPoint *pos, int highlight, void *node) {};
    void erase( FlowPoint *pos, int hot, void *node) {};
    void nav_erase( FlowPoint *pos, void *node) {};
    flow_eObjectType type() { return flow_eObjectType_ConClass;};
    void get_object_name( char *name);
    FlowCtx	*ctx;
    FlowPoint	zero;
    char	cc_name[32];
    flow_eConType con_type;
    flow_eCorner corner;
    flow_eDrawType draw_type;
    int	 line_width;
    double arrow_width;
    double arrow_length;
    double round_corner_amount;
    flow_eConGroup group;
};

#endif

