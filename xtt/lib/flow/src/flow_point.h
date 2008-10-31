/* 
 * Proview   $Id: flow_point.h,v 1.3 2008-10-31 12:51:33 claes Exp $
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

#ifndef flow_point_h
#define flow_point_h

#include <iostream>
#include <fstream>

#include "flow.h"
#include "flow_ctx.h"
#include "flow_array_elem.h"

class FlowPoint : public FlowArrayElem {
  public:
    FlowPoint() {};
    FlowPoint( FlowCtx *flow_ctx, double x1 = 0, double y1 = 0);
    const FlowPoint& operator+= (const FlowPoint p);
    FlowPoint operator+ (const FlowPoint p);
    const FlowPoint& operator-= (const FlowPoint p);
    FlowPoint operator- (const FlowPoint p);
    friend ostream& operator<< ( ostream& o, const FlowPoint p);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    void posit( double x, double y);
    void posit_z( int x, int y);
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    FlowCtx *ctx;    
    double x;
    double y;
    int z_x;
    int z_y;
    int nav_z_x;
    int nav_z_y;
    double print_z_x;
    double print_z_y;
    double current_zoom_factor;
};

#endif
