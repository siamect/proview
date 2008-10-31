/* 
 * Proview   $Id: glow_curvectx.h,v 1.4 2008-10-31 12:51:35 claes Exp $
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

#ifndef glow_curvectx_h
#define glow_curvectx_h

#include <iostream>
#include <fstream>

#include "glow.h"
#include "glow_growctx.h"

class CurveCtx : public GrowCtx {
  public:
    CurveCtx( const char *ctx_name, double zoom_fact = 100) :
        GrowCtx( ctx_name, zoom_fact), layout_adjusted(0)
	{ ctx_type = glow_eCtxType_Curve;};
    int layout_adjusted;
    void configure();
    void zoom( double factor);
    void unzoom() { zoom( mw.base_zoom_factor / mw.zoom_factor_x);};
    void nav_zoom();
    void get_zoom( double *factor_x, double *factor_y);
    void get_prefered_zoom_y( int height, double *factor_y);
    void adjust_layout();
    void scroll( double value);
    int event_handler_nav( glow_eEvent event, int x, int y);
    ~CurveCtx() {};
};

void curve_scroll_horizontal( CurveCtx *ctx, int value, int bottom);
void curve_scroll_vertical( CurveCtx *ctx, int value, int bottom);

#endif

