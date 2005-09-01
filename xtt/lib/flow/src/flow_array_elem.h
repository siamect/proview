/* 
 * Proview   $Id: flow_array_elem.h,v 1.3 2005-09-01 14:56:12 claes Exp $
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

#ifndef flow_array_elem_h
#define flow_array_elem_h

#include <iostream.h>
#include "flow.h"
#include "flow_ctx.h"

class FlowArrayElem {
  public:
    virtual void zoom() {};
    virtual void nav_zoom() {};
    virtual void print_zoom() {};
    virtual void traverse( int x, int y) {};
    virtual void get_borders(
	double *x_right, double *x_left, double *y_high, double *y_low) {};
    virtual void get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node) {};
    virtual void get_borders() {};
    virtual int	 event_handler( flow_eEvent event, 
		int x, int y) { return 0;};
    virtual int	 event_handler( void *pos, 
		flow_eEvent event, int x, int y, void *node) { return 0;};
    virtual void print( double ll_x, double ll_y, double ur_x, double ur_y) {};
    virtual void save( ofstream& fp, flow_eSaveMode mode) {};
    virtual void open( ifstream& fp) {};
    virtual void print( void *pos, void *node, int highlight) {};
    virtual void draw( int ll_x, int ll_y, int ur_x, int ur_y) {};
    virtual void draw( void *pos, int highlight, int hot, void *node) {};
    virtual void erase() {};
    virtual void erase( void *pos, int hot, void *node) {};
    virtual void draw_inverse( void *pos, int hot, void *node) {};
    virtual void move( int delta_x, int delta_y, int grid) {};
    virtual void move_noerase( int delta_x, int delta_y, int grid) {};
    virtual void shift( void *pos, double delta_x, double delta_y,
		int highlight, int hot) {};
    virtual void nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) {};
    virtual void nav_draw( void *pos, int highlight, void *node) {};
    virtual void nav_erase() {};
    virtual void nav_erase( void *pos, void *node) {};
    virtual void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};
    virtual int get_conpoint( int num, double *x, double *y,
		flow_eDirection *dir) { return 0;};
    virtual void redraw_node_cons( void *node) {};
    virtual int delete_node_cons( void *node) {return 0;};
    virtual void set_highlight( int on) {};
    virtual int get_highlight() {return 1;};
    virtual void set_inverse( int on) {};
    virtual void set_hot( int on) {};
    virtual void select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y) {};
    virtual flow_eObjectType type() { return flow_eObjectType_NoObject;};
    virtual void link_insert( FlowArrayElem **start) {};
    virtual int	in_area( double ll_x, double ll_y, double ur_x, double ur_y) 
		{ return 0;};
    virtual int	in_area_exact( double ll_x, double ll_y, double ur_x, double ur_y) 
		{ return 0;};
    virtual int	in_vert_line( double x, double l_y, double u_y) { return 0;};
    virtual int	in_horiz_line( double y, double l_x, double u_x) { return 0;};
    virtual void conpoint_refcon_redraw( void *node, int conpoint) {};
    virtual void conpoint_refcon_erase( void *node, int conpoint) {};
    virtual void remove_notify() {};
    virtual void set_user_data( void *data) {};
    virtual void get_user_data( void **data) {};
    virtual void trace_scan() {};
    virtual int trace_init() { return 1;};
    virtual void trace_close() {};
    virtual void set_trace_attr( char *object, char *attribute, flow_eTraceType type) {};
    virtual void get_trace_attr( char *object, char *attribute, flow_eTraceType *type) {};
    virtual void *get_ctx() { return NULL;};
    virtual void configure( void *previous) {};
    virtual void move_widgets( int x, int y) {};
    virtual void get_object_name( char *name) {};
    friend ostream& operator<< ( ostream& o, const FlowArrayElem e) { return o;};
    virtual ~FlowArrayElem() {};
};

#endif
