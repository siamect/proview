/* 
 * Proview   $Id: flow_annot.h,v 1.4 2008-10-31 12:51:32 claes Exp $
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

#ifndef flow_annot_h
#define flow_annot_h

#include <iostream>
#include <fstream>
#include "flow.h"
#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_array_elem.h"


class FlowAnnot : public FlowArrayElem {
  public:
    FlowAnnot( FlowCtx *flow_ctx, double x = 0, double y = 0, 
	int annot_num = 0, flow_eDrawType d_type = flow_eDrawType_TextHelveticaBold,
	int t_size = 2, flow_eAnnotType a_type = flow_eAnnotType_OneLine, 
	int rel_pos = 0, flow_mDisplayLevel display_lev = flow_mDisplayLevel_1) : 
	ctx(flow_ctx), number(annot_num), p(flow_ctx,x,y), draw_type(d_type),
	text_size(t_size), annot_type(a_type), relative_pos(rel_pos),
	display_level(display_lev) {};
    friend ostream& operator<< ( ostream& o, const FlowAnnot cp);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};
    int	event_handler( void *pos, flow_eEvent event, int x, int y, void *node);
    void print( void *pos, void *node, int highlight);
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int highlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};
    void erase( void *pos, int hot, void *node);
    void nav_erase( void *pos, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    int get_conpoint( int num, double *x, double *y, flow_eDirection *dir) 
		{ return 0;};
    flow_eObjectType type() { return flow_eObjectType_Annot;};
    void measure_annot( char *text, double *width, double *height);
    void configure_annotations( void *pos, void *node);
    void open_annotation_input( void *pos, void *node);
    void close_annotation_input( void *node);
    int get_annotation_input( void *node, char **text);
    void move_widgets( void *node, int x, int y);
    FlowCtx *ctx;    
    int	number;
    FlowPoint p;
    flow_eDrawType draw_type;
    int	 text_size;
    flow_eAnnotType annot_type;
    int relative_pos;
    flow_mDisplayLevel display_level;
};

void flow_measure_annot_text( FlowCtx *ctx, char *text, flow_eDrawType draw_type, 
		int text_size, flow_eAnnotType annot_type, 
		double *width, double *height, int *rows);

#endif
