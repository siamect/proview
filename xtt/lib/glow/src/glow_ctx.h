#ifndef glow_ctx_h
#define glow_ctx_h

#include <string.h>

#include "glow.h"
#include "glow_pscript.h"
#include "glow_array.h"
#include "glow_tracedata.h"

typedef struct {
	void	*scroll_data;
	int	total_width;
	int	total_height;
	int	window_width;
	int	window_height;
	int	offset_x;
	int	offset_y;
	} glow_sScroll;

class GlowCtx {
  public:
    GlowCtx( char *ctx_name, double zoom_fact = 100, int offs_x = 0, int offs_y = 0);
    glow_eCtxType ctx_type;
    double 	zoom_factor_x;
    double 	zoom_factor_y;
    double 	base_zoom_factor;
    int		offset_x;
    int		offset_y;
    double 	nav_zoom_factor_x;
    double 	nav_zoom_factor_y;
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
    void	*draw_ctx;
    void	*glow_window;
    int save( char *filename, glow_eSaveMode mode);
    int open( char *filename, glow_eSaveMode mode);
    int insert( GlowArrayElem *element) 
		{ return a.insert( element);};
    void remove( GlowArrayElem *element)
		{ a.remove(element);};
    void delete_all();
    void delete_object( GlowArrayElem *element) 
		{ remove(element); select_remove(element); move_remove(element);
		delete element;}
    int nodeclass_insert( GlowArrayElem *element) 
		{ return a_nc.insert( element);};
    int conclass_insert( GlowArrayElem *element) 
		{ return a_cc.insert( element);};
    int find( GlowArrayElem *element)
		{ return a.find(element);};
    int select_insert( GlowArrayElem *element) 
		{ return a_sel.insert( element);};
    void select_remove( GlowArrayElem *element)
		{ a_sel.remove(element);};
    void select_clear();
    int select_find( GlowArrayElem *element)
		{ return a_sel.find(element);};
    int move_insert( GlowArrayElem *element) 
		{ return a_move.insert( element);};
    void move_remove( GlowArrayElem *element)
		{ a_move.remove(element);};
    void move_clear()
		{ a_move.clear();};
    int move_find( GlowArrayElem *element)
		{ return a_move.find(element);};
    int paste_insert( GlowArrayElem *element) 
		{ return a_paste.insert( element);};
    void paste_remove( GlowArrayElem *element)
		{ a_paste.remove(element);};
    void paste_clear()
		{ a_paste.clear();};  // Delete objects also !!!
    int paste_find( GlowArrayElem *element)
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
    void node_movement( GlowArrayElem *node, int x, int y); 
    void con_create_source( GlowArrayElem *node, int cp_num, int cp_x, int cp_y); 
    void con_create_dest( GlowArrayElem *node, int cp_num, glow_eEvent event, int x, int y); 
    void get_selectlist( GlowArrayElem ***list, int *size)
		{ *list = a_sel.a; *size = a_sel.size();}; 
    void get_objectlist( GlowArrayElem ***list, int *size)
		{ *list = a.a; *size = a.size();}; 
    void set_gridsize( double size_x, double size_y) 
		{ grid_size_x = size_x; grid_size_y = size_y;};
    void set_grid( int on) { grid_on = on;};
    void find_grid( double x, double y, double *x_grid, double *y_grid);
    GlowArrayElem *node_moved;
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
    glow_eEvent	select_rect_event;
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
    GlowArrayElem *con_create_node;
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
    void 	zoom_x( double factor);
    void 	zoom_y( double factor);
    void 	zoom_absolute( double factor);
    void 	traverse( int x, int y);
    void 	get_borders();
    void	set_default_conclass( void *cc) { default_conclass=cc;};
    void	*get_default_conclass() {return default_conclass;};
    void 	print( double  ll_x, double  ll_y, double  ur_x, double  ur_y);
    void 	draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void        draw( double ll_x, double ll_y, double ur_x, double ur_y)
                           {draw( (int)ll_x, (int)ll_y, (int)ur_x, (int)ur_y);};
    void 	clear();
    void	nav_zoom();
    void	print_zoom();
    void	nav_clear();
    void	nav_draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void        nav_draw( double ll_x, double ll_y, double ur_x, double ur_y)
                           {nav_draw( (int)ll_x, (int)ll_y, (int)ur_x, (int)ur_y);};
    int	 	event_handler( glow_eEvent event, int x, int y,
			int w, int h);
    int	 	event_handler_nav( glow_eEvent event, int x, int y);
    void	enable_event( glow_eEvent event, glow_eEventType event_type,
		  int (*event_cb)( GlowCtx *ctx, glow_tEvent event));
    void	disable_event( glow_eEvent event);
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
    GlowArray 	a_nc;
    GlowArray 	a_cc;
    GlowArray 	a;
    GlowArray 	a_sel;
    GlowArray 	a_paste;
    GlowArray 	a_move;
    char	name[32];
    void	*default_conclass;
    int 	(*event_callback[glow_eEvent__])(GlowCtx *ctx, glow_tEvent);
    glow_eEvent	event_region_select;
    glow_eEvent	event_region_add_select;
    glow_eEvent	event_create_con;
    glow_eEvent	event_create_node;
    glow_eEvent	event_move_node;
    GlowArrayElem *callback_object;
    glow_eObjectType callback_object_type;
    void	register_callback_object( glow_eObjectType type, GlowArrayElem *object)
	{ callback_object = object; callback_object_type = type; };
    void	select_region_insert( double ll_x, double ll_y, double ur_x,
                                      double ur_y, glow_eSelectPolicy policy)
        {a.select_region_insert( ll_x, ll_y, ur_x, ur_y, policy);}
    int		cursor_present;
    int		cursor_x;
    int		cursor_y;
    int		user_highlight;
    int		application_paste;
    void	set_user_highlight( int mode) { user_highlight = mode;};
    double	grid_size_x;
    double	grid_size_y;
    int		grid_on;
    int		show_grid;
    GlowPscript *print_ps;
    void	print_region( double ll_x, double ll_y, double ur_x, 
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
    GlowArrayElem *get_node_from_name( char *name);
    GlowArrayElem *get_nodeclass_from_name( char *name);
    GlowArrayElem *get_conclass_from_name( char *name);
    int (*trace_connect_func)( void *, GlowTraceData *);
    int (*trace_disconnect_func)( void *);
    int (*trace_scan_func)( void *, void *);
    int trace_started;
    void remove_trace_objects();
    int trace_init( int (*connect_func)( void *, GlowTraceData *), 
	int (*disconnect_func)( void *),
	int (*scan_func)( void *, void *));
    void trace_close();
    void trace_scan();
    void *user_data;
    void set_user_data( void *data) { user_data = data;};
    void get_user_data( void **data) { *data = user_data;};
    void get_selected_nodes( GlowArrayElem ***nodes, int *num);
    void get_selected_cons( GlowArrayElem ***cons, int *num);
    void position_to_pixel( double x, double y, int *pix_x, int *pix_y)
	{ *pix_x = int( x * zoom_factor_x - offset_x);
	  *pix_y = int( y * zoom_factor_y - offset_y);};
    void unzoom() { zoom( base_zoom_factor / zoom_factor_x);};
    void center_object( GlowArrayElem *object);
    GlowArrayElem *get_document( double x, double y);
    int unobscured;
    int nodraw;
    int no_nav;
    int widget_cnt;
    glow_eSelectPolicy select_policy;
    void set_nodraw() { nodraw++; };
    void reset_nodraw() { if ( nodraw) nodraw--; };
    void reconfigure();
    void redraw();
    void object_deleted( GlowArrayElem *object);
    void annotation_input_cb( GlowArrayElem *object, int number, 
	char *text);
    void radiobutton_cb( GlowArrayElem *object, int number, 
	int value);
    int type() { return ctx_type;};
    void move_widgets( int x, int y) { if ( widget_cnt) a.move_widgets( x, y);};
    int display_level;
    void scroll( int delta_x, int delta_y);
    double scroll_size;
    void (*scroll_callback)( glow_sScroll *);
    void *scroll_data;
    glow_eHotMode hot_mode;
    glow_eHotMode default_hot_mode;
    int hot_found;
    int double_buffered;
    int double_buffer_on;
    int draw_buffer_only;
    void register_scroll_callback( void *data, 
		void (*callback)( glow_sScroll *))
		{ scroll_data = data; scroll_callback = callback;};
    void change_scrollbar();
    int find_by_name( char *name, GlowArrayElem **element)
		{ return a.find_by_name( name, element);};
    void set_hot_mode( glow_eHotMode mode) { hot_mode = mode;};
    void set_show_grid( int show);
    void draw_grid( int ll_x, int ll_y, int ur_x, int ur_y);
    void set_draw_buffer_only() { if (double_buffer_on) draw_buffer_only++;};
    void reset_draw_buffer_only() { if (double_buffer_on) draw_buffer_only--;};
    ~GlowCtx();
};

void	auto_scrolling( GlowCtx *ctx);
void glow_scroll_horizontal( GlowCtx *ctx, int value, int bottom);
void glow_scroll_vertical( GlowCtx *ctx, int value, int bottom);
int check_file( char *filename);


#endif

