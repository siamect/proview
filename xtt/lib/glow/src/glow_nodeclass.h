#ifndef glow_nodeclass_h
#define glow_nodeclass_h

#include <iostream.h>
#include <fstream.h>
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_array_elem.h"
#include "glow_array.h"

class GlowNodeClass : public GlowArrayElem {
  public:
    GlowNodeClass( GlowCtx *glow_ctx, char *name, 
	glow_eNodeGroup grp = glow_eNodeGroup_Common);
    GlowNodeClass( const GlowNodeClass& nc);
    void insert( GlowArrayElem *element) 
		{ a.insert( element);};
    void zoom() { a.zoom();};
    void nav_zoom() { a.nav_zoom();};
    void print_zoom() { a.print_zoom();};
    void traverse( int x, int y) { a.traverse( x, y);};
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node)
	{ a.get_borders(pos_x, pos_y, x_right, x_left, y_high, y_low, node);};
    void get_borders( GlowTransform *t, double *x_right, 
		      double *x_left, double *y_high, double *y_low);
    void get_obstacle_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    int	event_handler( void *pos, glow_eEvent event, int x, int y, void *node);
    int event_handler( glow_eEvent event, double fx, double fy);
    void print( GlowPoint *pos, void *node);
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( GlowPoint *pos, int highlight, int hot, void *node);
    void nav_draw( GlowPoint *pos, int highlight, void *node);
    void draw_inverse( GlowPoint *pos, int hot, void *node);
    void erase( GlowPoint *pos, int hot, void *node);
    void nav_erase( GlowPoint *pos, void *node);
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node);
    void nav_erase( GlowTransform *t, void *node);
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir);
    int get_conpoint( GlowTransform *t, int num, double *x, double *y, glow_eDirection *dir);
    glow_eObjectType type() { return glow_eObjectType_NodeClass;};
    void erase_annotation( void *pos, int highlight, int hot, 
	void *node, int num);
    void draw_annotation( void *pos, int highlight, int hot, 
	void *node, int num);
    void erase_annotation( GlowTransform *t, int highlight, int hot, 
	void *node, int num);
    void draw_annotation( GlowTransform *t, int highlight, int hot, 
	void *node, int num);
    void configure_annotations( void *pos, void *node);
    void measure_annotation( int num, char *text, double *width,
	double *height);
    int check_annotation( int num);
    void get_object_name( char *name);
    void open_annotation_input( void *pos, void *node, int num);
    void close_annotation_input( void *node, int num);
    int get_annotation_input( void *node, int num, char **text);
    void move_widgets( void *node, int x, int y);
    void set_fill_color( glow_eDrawType drawtype);
    void set_original_fill_color( glow_eDrawType drawtype);
    void reset_fill_color();
    void set_border_color( glow_eDrawType drawtype);
    void set_original_border_color( glow_eDrawType drawtype);
    void reset_border_color();
    void set_linewidth( int linewidth);
    int is_slider() { return slider;};
    int draw_annot_background( GlowTransform *t, void *node, double x, double y);
    int get_annot_background( GlowTransform *t, void *node,
		glow_eDrawType *background);
    void set_java_name( char *name) { strcpy( java_name, name);};
    int get_java_name( char *name);
    void get_annotation_numbers( int **numbers, int *cnt);
    void measure_javabean( double *pix_x_right, double *pix_x_left, 
	double *pix_y_high, double *pix_y_low);
    void set_saved( int value) { saved = value;};
    int is_next() { return prev_nc != 0;};
    int get_pages();
    void get_objectlist( GlowArrayElem ***list, int *size)
		{ *list = a.a; *size = a.size();}; 
    GlowNodeClass *get_base_nc();
    void set_extern( int ext) { nc_extern = ext;};
    void get_origo( GlowTransform *t, double *x, double *y);

    GlowCtx	*ctx;
    GlowArray 	a;
    char	nc_name[32];
    glow_eNodeGroup group;
    char 	*dynamic;
    int 	dynamicsize;
    int		arg_cnt;
    char	argname[20][32];
    int		argtype[20];
    int		nc_extern;
    glow_eTraceType trace_attr_type;
    glow_eDrawType trace_color;
    glow_eDrawType trace_color2;
    int		no_con_obstacle;
    int		slider;
    char	java_name[40];
    char	next_nodeclass[40];
    int		animation_count;
    double      y0;
    double      y1;
    double      x0;
    double      x1;
    GlowArrayElem *next_nc;
    GlowArrayElem *prev_nc;
    int		saved;
    glow_eCycle cycle;
};

#endif







