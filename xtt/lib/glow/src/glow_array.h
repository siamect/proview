#ifndef glow_array_h
#define glow_array_h

#include <iostream.h>
#include <fstream.h>

#include "glow_array_elem.h"
#include "glow_transform.h"

class GlowArray {
  public:
    GlowArray( int allocate, int incr);
    GlowArray() {};
    void new_array( const GlowArray& array);
    GlowArrayElem *operator[] ( int idx);
    void copy_from( const GlowArray& array);
    void copy_from_common_objects( GlowArray& array);
    void move_from( GlowArray& array);
    int size() { return a_size;};
    int insert( GlowArrayElem *element);
    void remove( GlowArrayElem *element);
    int find( GlowArrayElem *element);
    int find_by_name( char *name, GlowArrayElem **element);
    void clear() { a_size = 0;}
    void zoom();
    void nav_zoom();
    void print_zoom();
    void print( void *pos, void *node);
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( void *ctx, ifstream& fp);
    void draw( void *pos, int highlight, int hot, void *node);
    void erase( void *pos, int hot, void *node); 
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node); 
    void draw_inverse( void *pos, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void nav_erase( void *pos, void *node);
    void nav_erase( GlowTransform *t, void *node);
    void traverse( int x, int y);
    void get_borders(
	double *x_right, double *x_left, double *y_high, double *y_low);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    void get_borders( GlowTransform *t, double *x_right, 
		double *x_left, double *y_high, double *y_low);
    int	event_handler( glow_eEvent event, int x, int y);
    int	event_handler( glow_eEvent event, int x, int y, double fx, double fy);
    int	event_handler( glow_eEvent event, double fx, double fy);
    int	event_handler( void *pos, glow_eEvent event, int x, int y, void *node);
    int event_handler( void *pos, glow_eEvent event, int x, int y, int num);
    void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp);
    void conpoint_select( GlowTransform *t, int x, int y, double *distance, 
		void **cp, int *pix_x, int *pix_y);
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir);
    void set_highlight( int on);
    void set_hot( int on);
    void select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy);
    void shift( void *pos, double delta_x, double delta_y, int highlight, 
		int hot);
    void move( int delta_x, int delta_y, int grid);
    void move_noerase( int delta_x, int delta_y, int grid);
    void conpoint_refcon_redraw( void *node, int conpoint);
    void conpoint_refcon_erase( void *node, int conpoint);
    void set_inverse( int on);
    void configure();
    int brow_insert( GlowArrayElem *element, GlowArrayElem *destination, 
	glow_eDest code);
    void brow_remove( void *ctx, GlowArrayElem *element);
    void brow_close( void *ctx, GlowArrayElem *element);
    int brow_get_parent( GlowArrayElem *element, GlowArrayElem **parent);
    void move_widgets( int x, int y);
    int get_first( GlowArrayElem **first);
    int get_last( GlowArrayElem **last);
    int get_previous( GlowArrayElem *element, GlowArrayElem **prev);
    int get_next( GlowArrayElem *element, GlowArrayElem **next);
    void exec_dynamic();
    void set_fill_color( glow_eDrawType drawtype);
    void reset_fill_color();
    void set_border_color( glow_eDrawType drawtype);
    void reset_border_color();
    void set_original_fill_color( glow_eDrawType drawtype);
    void set_original_border_color( glow_eDrawType drawtype);
    void set_color_tone( glow_eDrawTone tone);
    void set_original_color_tone( glow_eDrawTone tone);
    void reset_color_tone();
    void set_color_lightness( int lightness);
    void incr_original_color_lightness( int lightness);
    void set_original_color_lightness( int lightness);
    void reset_color_lightness();
    void set_color_intensity( int intensity);
    void incr_original_color_intensity( int intensity);
    void set_original_color_intensity( int intensity);
    void reset_color_intensity();
    void set_color_shift( int shift);
    void incr_original_color_shift( int shift);
    void incr_color_shift( int shift);
    void set_original_color_shift( int shift);
    void reset_color_shift();
    void set_linewidth( int linewidth);
    void set_fill( int fill);
    void set_border( int border);
    void set_drawtype( glow_eDrawType drawtype);
    void set_transform( GlowTransform *t);
    void set_transform_from_stored(  GlowTransform *t);
    void store_transform();
    void push( GlowArrayElem *element);
    void pop( GlowArrayElem *element);
    void get_node_borders();
    void align( double x, double y, glow_eAlignDirection direction);
    int find_nc( GlowArrayElem *nc);
    void trace_scan();
    int trace_init();
    void trace_close();
    void get_nodegroups( void *a);
    void set_last_group( char *name);
    char *get_last_group();
    int get_background_object_limits( GlowTransform *t, glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction);


    friend class GlowNodeClass;
    friend class GlowCtx;

    int	allocated;
    int alloc_incr;
    int a_size;
    GlowArrayElem **a;
    ~GlowArray();
};

#endif
