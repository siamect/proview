/* 
 * Proview   $Id: flow_ctx.h,v 1.5 2007-01-24 12:37:07 claes Exp $
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

#ifndef flow_ctx_h
#define flow_ctx_h

#include <string.h>

#include "flow.h"
#include "flow_pscript.h"
#include "flow_array.h"

class FlowDraw;

typedef struct {
	void	*scroll_data;
	int	total_width;
	int	total_height;
	int	window_width;
	int	window_height;
	int	offset_x;
	int	offset_y;
	} flow_sScroll;

class FlowCtx {
  public:
    FlowCtx( char *ctx_name, double zoom_fact = 100, int offs_x = 0, int offs_y = 0) 
	: ctx_type(flow_eCtxType_Flow), zoom_factor(zoom_fact), base_zoom_factor(zoom_fact), 
	offset_x(offs_x), offset_y(offs_y), nav_zoom_factor(zoom_fact), 
	print_zoom_factor(100), 
	x_right(0), x_left(0), y_high(0), y_low(0), 
	window_width(0), window_height(0), 
	nav_window_width(0), nav_window_height(0),
	nav_rect_ll_x(0), nav_rect_ll_y(0), nav_rect_ur_x(0), nav_rect_ur_y(0), 
	node_movement_active(0),
	node_movement_paste_active(0), node_movement_paste_pending(0),
	nav_rect_movement_active(0), nav_rect_zoom_active(0), 
	select_rect_active(0), 
	con_create_active(0), auto_scrolling_active(0), defered_redraw_active(0),
	a(50,50), a_sel(20,20), a_paste(20,20),
	a_move(20,20), a_nc(20,20), a_cc(20,20),
	event_region_select(flow_eEvent_Null),
	event_region_add_select(flow_eEvent_Null),
	event_create_con(flow_eEvent_Null), event_create_node(flow_eEvent_Null),
 	event_move_node(flow_eEvent_Null), 
	callback_object(0), callback_object_type(flow_eObjectType_NoObject),
	cursor_present(0), cursor_x(0), cursor_y(0), user_highlight(0),
        application_paste(0),
	grid_size_x(2), grid_size_y(1), grid_on(1),
	draw_delta(0.3),
	grafcet_con_delta(2), refcon_cnt(0), refcon_width(1.5), 
	refcon_height(0.8), refcon_textsize(3), refcon_linewidth(2),
        trace_connect_func(0), trace_scan_func(0), trace_started(0), 
	unobscured(1), nodraw(0), no_nav(1), widget_cnt(0),
	select_policy(flow_eSelectPolicy_Partial), 
	display_level(flow_mDisplayLevel_1), scroll_size(0), 
	scroll_callback(0), scroll_data(NULL)
	{ strcpy(name, ctx_name);
	  memset( event_callback, 0, sizeof(event_callback));};
    flow_eCtxType ctx_type;
    double 	zoom_factor;
    double 	base_zoom_factor;
    int		offset_x;
    int		offset_y;
    double 	nav_zoom_factor;
    double 	print_zoom_factor;
    int		nav_offset_x;
    int		nav_offset_y;
    double	x_right;
    double	x_left;
    double	y_high;
    double	y_low;
    int		window_width;
    int		window_height;
    int		nav_window_width;
    int		nav_window_height;
    int		nav_rect_ll_x, nav_rect_ll_y, nav_rect_ur_x, nav_rect_ur_y; 
    int		nav_rect_hot;
    FlowDraw	*fdraw;
    void	*flow_window;
    int save( char *filename, flow_eSaveMode mode);
    int open( char *filename, flow_eSaveMode mode);
    int insert( FlowArrayElem *element) 
		{ return a.insert( element);};
    void remove( FlowArrayElem *element)
		{ a.remove(element);};
    void delete_all();
    void delete_object( FlowArrayElem *element) 
		{ remove(element); select_remove(element); move_remove(element);
		delete element;} 
    int nodeclass_insert( FlowArrayElem *element) 
		{ return a_nc.insert( element);};
    int conclass_insert( FlowArrayElem *element) 
		{ return a_cc.insert( element);};
    int select_insert( FlowArrayElem *element) 
		{ return a_sel.insert( element);};
    void select_remove( FlowArrayElem *element)
		{ a_sel.remove(element);};
    void select_clear();
    int select_find( FlowArrayElem *element)
		{ return a_sel.find(element);};
    int move_insert( FlowArrayElem *element) 
		{ return a_move.insert( element);};
    void move_remove( FlowArrayElem *element)
		{ a_move.remove(element);};
    void move_clear()
		{ a_move.clear();};
    int move_find( FlowArrayElem *element)
		{ return a_move.find(element);};
    int paste_insert( FlowArrayElem *element) 
		{ return a_paste.insert( element);};
    void paste_remove( FlowArrayElem *element)
		{ a_paste.remove(element);};
    void paste_clear()
		{ a_paste.clear();};  // Delete objects also !!!
    int paste_find( FlowArrayElem *element)
		{ return a_paste.find(element);};
    void paste();
    void paste_execute();
    void cut();
    void copy();
    void set_highlight( int on)
		{ a.set_highlight( on);};
    void set_inverse( int on)
		{ a.set_inverse( on);};
    void set_select_highlight( int on)
		{ a_sel.set_highlight( on);};
    void set_select_inverse( int on)
		{ a_sel.set_inverse( on);};
    void node_movement( FlowArrayElem *node, int x, int y); 
    void con_create_source( FlowArrayElem *node, int cp_num, int cp_x, int cp_y); 
    void con_create_dest( FlowArrayElem *node, int cp_num, flow_eEvent event, int x, int y); 
    void get_selectlist( FlowArrayElem ***list, int *size)
		{ *list = a_sel.a; *size = a_sel.size();}; 
    void get_objectlist( FlowArrayElem ***list, int *size)
		{ *list = a.a; *size = a.size();}; 
    void set_gridsize( double size_x, double size_y) 
		{ grid_size_x = size_x; grid_size_y = size_y;};
    void set_grid( int on) { grid_on = on;};
    void find_grid( double x, double y, double *x_grid, double *y_grid);
    FlowArrayElem *node_moved;
    int		node_move_last_x;
    int		node_move_last_y;
    int		node_movement_active;
    int		node_movement_paste_active;
    int		node_movement_paste_pending;
    int		nav_rect_move_last_x;
    int		nav_rect_move_last_y;
    int		nav_rect_movement_active;
    int		nav_rect_zoom_active;
    int		select_rect_active;
    flow_eEvent	select_rect_event;
    int		select_rect_last_x;
    int		select_rect_last_y;
    int		select_rect_start_x;
    int		select_rect_start_y;
    int		select_rect_ll_x;
    int		select_rect_ll_y;
    int		select_rect_ur_x;
    int		select_rect_ur_y;
    double	select_area_ll_x;
    double	select_area_ll_y;
    double	select_area_ur_x;
    double	select_area_ur_y;
    FlowArrayElem *con_create_node;
    int		con_create_last_x;
    int		con_create_last_y;
    int		con_create_conpoint_num;
    int		con_create_conpoint_x;
    int		con_create_conpoint_y;
    int		con_create_active;
    int		auto_scrolling_active;
    void	*auto_scrolling_id;
    void	auto_scrolling_stop();
    void 	zoom( double factor);
    void 	zoom_absolute( double factor);
    void 	traverse( int x, int y);
    void 	get_borders();
    void	set_default_conclass( void *cc) { default_conclass=cc;};
    void	*get_default_conclass() {return default_conclass;};
    void 	print( double  ll_x, double  ll_y, double  ur_x, double  ur_y);
    void 	draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void 	clear();
    void	nav_zoom();
    void	print_zoom();
    void	nav_clear();
    void	nav_draw( int ll_x, int ll_y, int ur_x, int ur_y);
    int	 	event_handler( flow_eEvent event, int x, int y,
			int w, int h);
    int	 	event_handler_nav( flow_eEvent event, int x, int y);
    void	enable_event( flow_eEvent event, flow_eEventType event_type,
		  int (*event_cb)( FlowCtx *ctx, flow_tEvent event));
    void	disable_event( flow_eEvent event);
    void 	disable_event_all();
    void	redraw_node_cons( void *node);
    void	delete_node_cons( void *node);
    void	set_defered_redraw();
    void	redraw_defered();
    int		defered_redraw_active;
    int		defered_x_low;
    int		defered_x_high;
    int		defered_y_low;
    int		defered_y_high;
    int		defered_x_low_nav;
    int		defered_x_high_nav;
    int		defered_y_low_nav;
    int		defered_y_high_nav;
    FlowArray 	a;
    FlowArray 	a_sel;
    FlowArray 	a_paste;
    FlowArray 	a_move;
    FlowArray 	a_nc;
    FlowArray 	a_cc;
    char	name[32];
    void	*default_conclass;
    int 	(*event_callback[flow_eEvent__])(FlowCtx *ctx, flow_tEvent);
    flow_eEvent	event_region_select;
    flow_eEvent	event_region_add_select;
    flow_eEvent	event_create_con;
    flow_eEvent	event_create_node;
    flow_eEvent	event_move_node;
    FlowArrayElem *callback_object;
    flow_eObjectType callback_object_type;
    void	register_callback_object( flow_eObjectType type, FlowArrayElem *object)
	{ callback_object = object; callback_object_type = type; };
    void	select_region_insert( double ll_x, double ll_y, double ur_x, double ur_y)
        {a.select_region_insert( ll_x, ll_y, ur_x, ur_y);}
    int		cursor_present;
    int		cursor_x;
    int		cursor_y;
    int		user_highlight;
    int		application_paste;
    void	set_user_highlight( int mode) { user_highlight = mode;};
    double	grid_size_x;
    double	grid_size_y;
    int		grid_on;
    FlowPscript *print_ps;
    int		print_region( double ll_x, double ll_y, double ur_x, 
	double ur_y, char *filename);
    double	draw_delta;
    double	grafcet_con_delta;
    int		refcon_cnt;
    double	refcon_width;
    double	refcon_height;
    int		refcon_textsize;
    int		refcon_linewidth;
    void conpoint_refcon_redraw( void *node, int conpoint)
	{ a.conpoint_refcon_redraw( node, conpoint);};
    void conpoint_refcon_erase( void *node, int conpoint)
	{ a.conpoint_refcon_erase( node, conpoint);};
    FlowArrayElem *get_node_from_name( char *name);
    FlowArrayElem *get_nodeclass_from_name( char *name);
    FlowArrayElem *get_conclass_from_name( char *name);
    int (*trace_connect_func)( void *, char *, char *, flow_eTraceType, 
		void **);
    int (*trace_disconnect_func)( void *);
    int (*trace_scan_func)( void *, void *);
    int trace_started;
    void remove_trace_objects();
    int trace_init( int (*connect_func)( void *, char *, char *, flow_eTraceType, 
	void **), int (*disconnect_func)( void *),
	int (*scan_func)( void *, void *));
    void trace_close();
    void trace_scan();
    void *user_data;
    void set_user_data( void *data) { user_data = data;};
    void get_user_data( void **data) { *data = user_data;};
    void get_selected_nodes( FlowArrayElem ***nodes, int *num);
    void get_selected_cons( FlowArrayElem ***cons, int *num);
    void position_to_pixel( double x, double y, int *pix_x, int *pix_y)
	{ *pix_x = int( x * zoom_factor - offset_x);
	  *pix_y = int( y * zoom_factor - offset_y);};
    void pixel_to_position( int pix_x, int pix_y, double *x, double *y)
	{ *x = double( pix_x + offset_x) / zoom_factor;
	  *y = double( pix_y + offset_y) / zoom_factor;};
    void unzoom() { zoom( base_zoom_factor / zoom_factor);};
    void center_object( FlowArrayElem *object);
    FlowArrayElem *get_document( double x, double y);
    int unobscured;
    int nodraw;
    int no_nav;
    int widget_cnt;
    flow_eSelectPolicy select_policy;
    void set_nodraw() { nodraw++; };
    void reset_nodraw() { if ( nodraw) nodraw--; };
    void reconfigure();
    void redraw();
    void object_deleted( FlowArrayElem *object);
    void annotation_input_cb( FlowArrayElem *object, int number, 
	char *text);
    int radiobutton_cb( FlowArrayElem *object, int number, 
	int value);
    int type() { return ctx_type;};
    void move_widgets( int x, int y) { if ( widget_cnt) a.move_widgets( x, y);};
    int display_level;
    void scroll( int delta_x, int delta_y);
    void scroll( double delta_x, double delta_y);
    double scroll_size;
    void (*scroll_callback)( flow_sScroll *);
    void *scroll_data;
    void register_scroll_callback( void *data, 
		void (*callback)( flow_sScroll *))
		{ scroll_data = data; scroll_callback = callback;};
    void change_scrollbar();
    int find_by_name( char *name, FlowArrayElem **element)
		{ return a.find_by_name( name, element);};
    int find_by_name_no_case( char *name, FlowArrayElem **element)
		{ return a.find_by_name_no_case( name, element);};
    void get_zoom( double *zoom) { *zoom = zoom_factor;};
    ~FlowCtx();
};

void	auto_scrolling( FlowCtx *ctx);
void flow_scroll_horizontal( FlowCtx *ctx, int value, int bottom);
void flow_scroll_vertical( FlowCtx *ctx, int value, int bottom);


#endif

