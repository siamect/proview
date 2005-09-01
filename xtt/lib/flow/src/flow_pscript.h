/* 
 * Proview   $Id: flow_pscript.h,v 1.4 2005-09-01 14:56:12 claes Exp $
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

#ifndef flow_pscript_h
#define flow_pscript_h

#include <stdio.h>
#include "flow.h"

class FlowPscript {
  public:
    FlowPscript( char *filename, void *flow_ctx, int page_border, int *sts);
    ~FlowPscript();
     int print_page( double ll_x, double ll_y, double ur_x, double ur_y);
     int rect( double x, double y, double width, double height, flow_eDrawType type, double idx, int highlight);
     int filled_rect( double x, double y, double width, double height, flow_eDrawType type, double idx);
     int arc( double x, double y, double width, double height, int angel1, int angel2,
		flow_eDrawType type, double idx, int highlight);
     int line( double x1, double y1, double x2, double y2, flow_eDrawType type, double idx, int highlight);
     int text( double x, double y, char *text, int len,  flow_eDrawType type, int size);
     int pixmap( double x, double y, flow_sPixmapDataElem *data, flow_eDrawType type);
     int arrow( double x1, double y1, double x2, double y2, 
	double x3, double y3, flow_eDrawType type, double idx);
     void move( double x, double y);
     void setlinewidth( double idx);
     void rowbreak();
     void set_showred( int show) { show_red = show;}
     double offset_x;
     double offset_y;
     FILE *file;
     char fname[80];
     int border;
     void *ctx;
     int show_red;
};

#endif
