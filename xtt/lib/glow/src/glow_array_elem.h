#ifndef glow_array_elem_h
#define glow_array_elem_h

#include <iostream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_transform.h"
#include "glow_tracedata.h"

/*! \file glow_array_elem.h
    \brief Contains the GrowArrayElem class. */
/*! \addtogroup Glow */
/*@{*/

//! Virtual base class for all drawing objects and components.
/*! A GlowArrayElem object can be inserted in a GlowArray vector, which contains all the
  object in a window, in a group, or in a subgraph component.
*/
class GlowArrayElem {
  public:
    virtual void zoom() {};
    virtual void nav_zoom() {};
    virtual void print_zoom() {};
    virtual void traverse( int x, int y) {};
    virtual void get_borders(
	double *x_right, double *x_left, double *y_high, double *y_low) {};
    virtual void get_borders( GlowTransform *t,
	double *x_right, double *x_left, double *y_high, double *y_low) {};
    virtual void get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node) {};
    virtual int	 event_handler( glow_eEvent event, 
		int x, int y) { return 0;};
    virtual int	 event_handler( glow_eEvent event, 
		int x, int y, double fx, double fy) { return 0;};
    virtual int	 event_handler( glow_eEvent event, double fx, double fy) 
		{ return 0;};
    virtual int	 event_handler( void *pos, 
		glow_eEvent event, int x, int y, void *node) { return 0;};
    virtual void print( double ll_x, double ll_y, double ur_x, double ur_y) {};
    virtual void save( ofstream& fp, glow_eSaveMode mode) {};
    virtual void open( ifstream& fp) {};
    virtual void print( void *pos, void *node) {};
    virtual void draw( int ll_x, int ll_y, int ur_x, int ur_y) {};
    virtual void draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) {};
    virtual void draw( void *pos, int highlight, int hot, void *node) {};
    virtual void draw( GlowTransform *t, int highlight, int hot, void *node, 
		       void *colornode) {};
    virtual void erase() {};
    virtual void erase( void *pos, int hot, void *node) {};
    virtual void erase( GlowTransform *t, int hot, void *node) {};
    virtual void draw_inverse( void *pos, int hot, void *node) {};
    virtual void move( double delta_x, double delta_y, int grid) {};
    virtual void move_noerase( int delta_x, int delta_y, int grid) {};
    virtual void shift( void *pos, double delta_x, double delta_y,
		int highlight, int hot) {};
    virtual void nav_draw( int ll_x, int ll_y, int ur_x, int ur_y) {};
    virtual void nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y) {};
    virtual void nav_draw( void *pos, int highlight, void *node) {};
    virtual void nav_draw( GlowTransform *t, int highlight, void *node,
			   void *colornode) {};
    virtual void nav_erase() {};
    virtual void nav_erase( void *pos, void *node) {};
    virtual void nav_erase( GlowTransform *t, void *node) {};
    virtual void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};
    virtual void conpoint_select( GlowTransform *t, int x, int y, 
		double *distance, void **cp, int *pix_x, int *pix_y) {};
    virtual int get_conpoint( int num, double *x, double *y,
		glow_eDirection *dir) { return 0;};
    virtual int get_conpoint( GlowTransform *t, int num, bool flip_horizontal,
			      bool flip_vertical, double *x, double *y,
		glow_eDirection *dir) { return 0;};
    virtual void redraw_node_cons( void *node) {};
    virtual int delete_node_cons( void *node) {return 0;};
    virtual void set_highlight( int on) {};
    virtual int get_highlight() {return 1;};
    virtual void set_inverse( int on) {};
    virtual void set_hot( int on) {};
    virtual void select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy) {};
    virtual glow_eObjectType type() { return glow_eObjectType_NoObject;};
    virtual void link_insert( void **start) {};
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
    virtual void set_trace_attr( GlowTraceData *data) {};
    virtual void get_trace_attr( GlowTraceData **data) {};
    virtual void *get_ctx() { return NULL;};
    virtual void configure( void *previous) {};
    virtual void move_widgets( int x, int y) {};
    virtual void get_object_name( char *name) {};
    virtual void set_object_name( char *name) {};
    virtual void set_fill( int fill) {};
    virtual void set_border( int border) {};
    virtual void set_shadow( int shadow) {};
    virtual void set_drawtype( glow_eDrawType draw_type) {};
    virtual void set_linewidth( int linewidth) {};
    virtual void set_position( double x, double y) {};
    virtual void set_scale( double scale_x, double scale_y, double x0, double y0, 
		glow_eScaleType type) {};
    virtual void set_scale_pos( double scale_x, double x, double y, 
                double scale_y, double x0, double y0, 
		glow_eScaleType type) {};
    virtual void set_rotation( double angel, double x0, double y0, 
		glow_eRotationPoint type) {};
    virtual void exec_dynamic() {};
    virtual void set_fill_color( glow_eDrawType drawtype) {};
    virtual void set_original_fill_color( glow_eDrawType drawtype) {};
    virtual void reset_fill_color() {};
    virtual void set_border_color( glow_eDrawType drawtype) {};
    virtual void set_original_border_color( glow_eDrawType drawtype) {};
    virtual void reset_border_color() {};
    virtual void set_text_color( glow_eDrawType drawtype) {};
    virtual void set_original_text_color( glow_eDrawType drawtype) {};
    virtual void reset_text_color() {};
    virtual void set_color_tone( glow_eDrawTone tone) {};
    virtual void set_original_color_tone( glow_eDrawTone tone) {};
    virtual void reset_color_tone() {};
    virtual void set_color_lightness( int lightness) {};
    virtual void incr_original_color_lightness( int lightness) {};
    virtual void set_original_color_lightness( int lightness) {};
    virtual void reset_color_lightness() {};
    virtual void set_color_intensity( int intensity) {};
    virtual void incr_original_color_intensity( int intensity) {};
    virtual void set_original_color_intensity( int intensity) {};
    virtual void reset_color_intensity() {};
    virtual void set_color_shift( int shift) {};
    virtual void incr_original_color_shift( int shift) {};
    virtual void incr_color_shift( int shift) {};
    virtual void set_original_color_shift( int shift) {};
    virtual void reset_color_shift() {};
    virtual void set_visibility( int visible) {};
    virtual void set_transform( GlowTransform *t) {};
    virtual void set_transform_from_stored(  GlowTransform *t) {};
    virtual void store_transform() {};
    virtual bool transform_is_stored() { return false;};
    virtual void get_node_borders() {};
    virtual void get_ctx( void **ctx) {};
    virtual void align( double x, double y, glow_eAlignDirection direction) {};
    virtual void measure( double *ll_x, double *ll_y, double *ur_x, 
	double *ur_y) {};
    virtual void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream& fp) {};
    virtual int find_nc( GlowArrayElem *nc) { return 0;};
    virtual int find_cc( GlowArrayElem *cc) { return 0;};
    virtual void get_nodegroups( void *a) {};
    virtual void set_last_group( char *name) {};
    virtual char *get_last_group() { return ""; };
    virtual int get_background_object_limits( GlowTransform *t, 
		    glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction)
                    { return 0;};
    virtual void call_redraw_node_cons() {};
    virtual void flip( double x0, double y0, glow_eFlipDirection dir) {};
    virtual void convert( glow_eConvert version) {};
    virtual void set_rootnode( void *node) {};
    virtual void set_linetype( glow_eLineType type) {};
    virtual void set_input_focus( int focus) {};

    virtual ~GlowArrayElem() {};
};

/*@}*/
#endif









