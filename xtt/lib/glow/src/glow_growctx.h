#ifndef glow_growctx_h
#define glow_growctx_h

#include <iostream.h>
#include <fstream.h>

#include "glow.h"
#include "glow_ctx.h"
#include "glow_transform.h"
#include "glow_exportjbean.h"

typedef enum {
	grow_eMode_Edit,
	grow_eMode_Rect,
	grow_eMode_Line,
	grow_eMode_PolyLine,
	grow_eMode_Text,
	grow_eMode_Circle,
	grow_eMode_Annot,
	grow_eMode_ConPoint,
	grow_eMode_Scale,
	grow_eMode_Bar,
	grow_eMode_Trend,
	grow_eMode_EditPolyLine,
        grow_eMode_Axis
	} grow_eMode;

class GrowCtx : public GlowCtx {
  public:
    GrowCtx( char *ctx_name, double zoom_fact = 100) :
    	GlowCtx( ctx_name, zoom_fact), edit_mode(grow_eMode_Edit),
	conpoint_num_cnt(0), objectname_cnt(0), polyline_not_first(0),
	background_color(glow_eDrawType_LineErase), highlight_disabled(0),
	dynamic(0), dynamicsize(0), arg_cnt(0), x0(0), y0(0), x1(0), y1(0),
	path_cnt(0),
	trace_attr_type(glow_eTraceType_Boolean), 
	trace_color(glow_eDrawType_Line), trace_color2(glow_eDrawType_Line),
	no_con_obstacle(0), slider(0), slider_cursor_offset(0), 
	move_restriction(glow_eMoveRestriction_No), restriction_max_limit(0),
	restriction_min_limit(0), restriction_object(0), subgraph(0),
        modified(0), export_jbean(0), animation_count(1), scantime(0.5),
        fast_scantime(0.5), animation_scantime(0.5), stored_zoom_factor_x(0), 
        stored_zoom_factor_y(0), java_width(0),
        enable_bg_pixmap(0), background_tiled(0), scale_active(0),
        initial_position(glow_eDirection_Up), is_javaapplet(0),
        is_javaapplication(0), cycle(glow_eCycle_Slow),
        mb3_action(glow_eMB3Action_Close), scale_equal(0), translate_on(0)
	{ ctx_type = glow_eCtxType_Grow;
	  strcpy( name, "");
	  strcpy( java_name, ""); 
	  strcpy( next_subgraph, "");
          strcpy( background_image, ""); 
	  memset( argname, 0, sizeof(argname));
	  memset( argtype, 0, sizeof(argtype));
	};
    ~GrowCtx();

    int event_handler( glow_eEvent event, int x, int y, int w, int h);
    void draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void draw( double ll_x, double ll_y, double ur_x, double ur_y)
                {draw( (int)ll_x, (int)ll_y, (int)ur_x, (int)ur_y);};
    void nav_draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void nav_draw( double ll_x, double ll_y, double ur_x, double ur_y)
                {nav_draw( (int)ll_x, (int)ll_y, (int)ur_x, (int)ur_y);};
    void redraw_defered();
    void set_mode( grow_eMode grow_mode); 
    grow_eMode mode() { return edit_mode; };
    int open_subgraph_from_name( char *name, glow_eSaveMode mode);
    int save_subgraph( char *filename, glow_eSaveMode mode);
    int open_subgraph( char *filename, glow_eSaveMode mode);
    int save( char *filename, glow_eSaveMode mode);
    void save_grow( ofstream& fp, glow_eSaveMode mode);
    void open_grow( ifstream& fp);
    int find_by_name( char *name, GlowArrayElem **element)
		{ return a.find_by_name( name, element);};
    int find_nc_by_name( char *name, GlowArrayElem **element)
		{ return a_nc.find_by_name( name, element);};
    int find_cc_by_name( char *name, GlowArrayElem **element)
		{ return a_cc.find_by_name( name, element);};
    int get_next_conpoint_num() { return conpoint_num_cnt++;};
    int get_next_objectname_num() { return objectname_cnt++;};
    void clear_all( int keep_paste);
    void set_name( char *grow_name) { strcpy( name, grow_name);};
    void get_name( char *grow_name) { strcpy( grow_name, name);};
    void get_movelist( GlowArrayElem ***list, int *size)
		{ *list = a_move.a; *size = a_move.size();}; 
    void polyline_end() { polyline_not_first = 0;};
    void exec_dynamic() { a.exec_dynamic();};
    void dynamic_cb( GlowArrayElem *object, char *code, glow_eDynamicType type);
    void set_select_original_border_color( glow_eDrawType drawtype)
	{ a_sel.set_original_border_color( drawtype);};
    void set_select_original_fill_color( glow_eDrawType drawtype)
	{ a_sel.set_original_fill_color( drawtype);};
    void set_select_original_color_tone( glow_eDrawTone tone)
	{ a_sel.set_original_color_tone( tone);};
    void set_select_original_color_lightness( int lightness)
	{ a_sel.set_original_color_lightness( lightness);};
    void incr_select_original_color_lightness( int lightness)
	{ a_sel.incr_original_color_lightness( lightness);};
    void set_select_original_color_intensity( int intensity)
	{ a_sel.set_original_color_intensity( intensity);};
    void incr_select_original_color_intensity( int intensity)
	{ a_sel.incr_original_color_intensity( intensity);};
    void set_select_original_color_shift( int shift)
	{ a_sel.set_original_color_shift( shift);};
    void incr_select_original_color_shift( int shift)
	{ a_sel.incr_original_color_shift( shift);};
    void set_select_linewidth( int linewidth)
	{ a_sel.set_linewidth( linewidth);};
    void set_select_fill( int fill)
	{ a_sel.set_fill( fill);};
    void set_select_border( int border)
	{ a_sel.set_border( border);};
    void scale_select( double scale_x, double scale_y, glow_eScaleType type);
    void rotate_select( double angel, glow_eRotationPoint type);
    void set_select_textsize( int size);
    void set_select_textbold( int bold);
    void set_background( glow_eDrawType color);
    void get_background( glow_eDrawType *color) { *color = background_color;};
    void set_background_image( char *image) { strcpy( background_image, image);};
    void get_background_image( char *image) { strcpy( image, background_image);};
    int get_background_image_size( int *width, int *height);
    void reset_background();
    glow_eDrawType get_drawtype( glow_eDrawType local_drawtype,
		glow_eDrawType highlight_drawtype, int highlight, void *node, 
		int fill);
    void disable_highlight() { highlight_disabled = 1;};
    void enable_highlight() { highlight_disabled = 0;};
    void set_dynamic( char *code, int size);
    void get_dynamic( char **code, int *size) { *code = dynamic; 
	*size = dynamicsize;};
    void get_nodeclasslist( GlowArrayElem ***list, int *size)
		{ *list = a_nc.a; *size = a_nc.size();}; 
    void get_nodegrouplist( GlowArrayElem ***list, int *size);
    void pop_select();
    void push_select();
    void set_default_layout();
    void set_path( int path_count, char *path_vect)
		{ path_cnt = path_count; memcpy( path, path_vect, sizeof(path)); };
    int any_select_is_con();
    void change_select_conclass( GlowArrayElem *conclass);
    void set_move_restrictions( glow_eMoveRestriction restriction,
	double max_limit, double min_limit, GlowArrayElem *object);
    void align_select( glow_eAlignDirection direction);
    void equidistance_select( glow_eAlignDirection direction);
    int is_subgraph() { return subgraph;};
    int is_slider() { return slider;};
    void set_modified( int mod) { modified = mod;};
    int get_modified() { return modified;};
    void measure_javabean( double *pix_x_right, double *pix_x_left, 
	double *pix_y_high, double *pix_y_low);
    void to_pixel( double x, double y, double *pix_x, double *pix_y);
    void set_javaframe( double *pix_x_right, double *pix_x_left, 
	double *pix_y_high, double *pix_y_low);
    void export_javabean( ofstream& fp, int components);
    void export_nodeclass_javabean( GlowArrayElem *nc, ofstream& fp, int components);
    void export_nc_javabean_font( GlowArrayElem *nc, ofstream& fp, int components);
    void set_java_name( char *name) { strcpy( java_name, name);};
    int get_java_name( char *name);
    void get_annotation_numbers( int **numbers, int *cnt);
    int send_hot_request( GlowArrayElem *object);
    void get_scantime( double *time, double *fast_time, 
		       double *animation_time)
 	{ *time = scantime; *fast_time = fast_scantime; 
	  *animation_time = animation_scantime;};
    void store_geometry();
    void restore_geometry();
    void get_text_extent( char *text, int len, glow_eDrawType draw_type,
	int text_size, double *width, double *height, double *descent);
    int get_default_window_size( int *width, int *height);
    void set_layout( double nx0, double ny0, double nx1, double ny1)
      { x0 = nx0; y0 = ny0; x1 = nx1; y1= ny1; }; 
    int group_select( GlowArrayElem **group, char *last_group);
    int ungroup_select();
    int get_object_group( GlowArrayElem *object, GlowArrayElem **group);
    int get_background_object_limits( glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction);
    void set_scale_equal( int equal) { scale_equal = equal;};
    int translate_cb( GlowArrayElem *object, char *text, char **new_text);

    char	name[40];
    grow_eMode	edit_mode;
    int		conpoint_num_cnt;
    int		objectname_cnt;
    int		polyline_not_first;
    int		polyline_start_x;
    int		polyline_start_y;
    int		polyline_last_end_x;
    int		polyline_last_end_y;
    int		select_rect_stored_ll_x;
    int		select_rect_stored_ll_y;
    int		select_rect_stored_ur_x;
    int		select_rect_stored_ur_y;
    double	scale_x;
    double 	scale_y;
    double	scale_center_x;
    double	scale_center_y;
    glow_eScaleType scale_type;
    glow_eDrawType	background_color;
    int		highlight_disabled;
    char 	*dynamic;
    int 	dynamicsize;
    int		arg_cnt;
    char	argname[20][32];
    int		argtype[20];
    double	x0;
    double 	y0;
    double	x1;
    double	y1;
    int		path_cnt;
    char	path[10][80];
    glow_eTraceType trace_attr_type;
    glow_eDrawType trace_color;
    glow_eDrawType trace_color2;
    int		no_con_obstacle;
    int		slider;
    double	slider_cursor_offset;
    glow_eMoveRestriction move_restriction;
    double	restriction_max_limit;
    double	restriction_min_limit;
    GlowArrayElem *restriction_object;
    int		subgraph;
    int		modified;
    GlowExportJBean *export_jbean;
    char	java_name[40];
    char	next_subgraph[40];
    int		animation_count;
    double	scantime;
    double	fast_scantime;
    double	animation_scantime;
    int		stored_offset_x;
    int		stored_offset_y;
    double	stored_zoom_factor_x;
    double	stored_zoom_factor_y;
    double	stored_grid_size_x;
    double	stored_grid_size_y;
    int		stored_grid_on;
    int		stored_show_grid;
    int		java_width;
    int         enable_bg_pixmap;
    char        background_image[80];
    int         background_tiled;
    int         scale_active;
    int         initial_position;
    int		is_javaapplet;
    int		is_javaapplication;
    glow_eCycle cycle;
    glow_eMB3Action mb3_action;
    int         scale_equal;
    int         translate_on;
};

void grow_auto_scrolling( GrowCtx *ctx);

#endif



