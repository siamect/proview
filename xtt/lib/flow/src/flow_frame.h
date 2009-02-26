/* 
 * Proview   $Id: flow_frame.h,v 1.3 2008-10-31 12:51:33 claes Exp $
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

#ifndef flow_frame_h
#define flow_frame_h

#include <iostream>
#include <fstream>
#include "flow.h"
#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_rect.h"


class FlowFrame : public FlowRect {
  public:
    FlowFrame( FlowCtx *flow_ctx, double x = 0, double y = 0, double w = 0, 
		double h = 0, flow_eDrawType d_type = flow_eDrawType_Line, 
		int line_w = 1, int fix_line_w = 0) : 
	FlowRect( flow_ctx, x, y, w, h, d_type, line_w, fix_line_w) {};
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int hightlight, int hot, void *node);
    void erase( void *pos, int hot, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    flow_eObjectType type() { return flow_eObjectType_Frame;};
    int event_handler( void *pos, flow_eEvent event, int x, int y,
		       void *node);
};

#endif
