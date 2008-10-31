/* 
 * Proview   $Id: glow_polyline.h,v 1.4 2008-10-31 12:51:36 claes Exp $
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

#ifndef glow_polyline_h
#define glow_polyline_h

#include <iostream>
#include <fstream>
#include "glow.h"
#include "glow_growctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"
#include "glow_draw.h"

class GlowPolyLine : public GlowArrayElem {
  public:
    GlowPolyLine() {};
    GlowPolyLine( GrowCtx *glow_ctx, glow_sPoint *pointarray, 
		int point_cnt, glow_eDrawType d_type = glow_eDrawType_Line, 
		int line_w = 1, int fix_line_w = 0, int line_fill = 0,
		int closed = 0);
    ~GlowPolyLine();
    GlowPolyLine( const GlowPolyLine& c);

    friend ostream& operator<< ( ostream& o, const GlowPolyLine l);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    glow_eObjectType type() { return glow_eObjectType_PolyLine;};
    void set_fill( int fillval) { fill = fillval;};
    void set_drawtype( glow_eDrawType drawtype) { draw_type = drawtype;};
    void set_linewidth( int linewidth) {line_width = linewidth;};
    void add_points( void *pos, glow_sPoint *pointarray, 
		     int point_cnt, int highlight, int hot);
    void traverse( int x, int y);
    GrowCtx *ctx;    
    GlowArray a_points;
    glow_sPointX *points;
    glow_eDrawType draw_type;
    int	 line_width;
    int  fix_line_width;
    int  fill;
    int  closed_line;
};

#endif
