#ifndef glow_growrect_h
#define glow_growrect_h

#include "glow_rect.h"

class GrowRect : public GlowRect {
  public:
    GrowRect( GlowCtx *glow_ctx, char *name, double x = 0, double y = 0, 
		double w = 0, double h = 0, 
		glow_eDrawType border_d_type = glow_eDrawType_Line, 
		int line_w = 1, int fix_line_w = 0, 
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		int fill_rect = 0, int display_border = 1, 
		glow_eDrawType fill_d_type = glow_eDrawType_Line, int nodraw = 0);
    int	event_handler( glow_eEvent event, int x, int y, double fx, double fy);
    int event_handler( glow_eEvent event, double fx, double fy);
    int local_event_handler( glow_eEvent event, double x, double y);
    void get_borders( GlowTransform *t,
	double *x1_right, double *x1_left, double *y1_high, double *y1_low);
    void get_borders(
	double *x1_right, double *x1_left, double *y1_high, double *y1_low)
	{ get_borders( (GlowTransform *) NULL, x1_right, x1_left, y1_high, y1_low);};
    void get_node_borders()
	{ x_left = y_low = 1e37; x_right = y_high = -1e37;
	  get_borders( (GlowTransform *) NULL, &x_right, &x_left, &y_high, &y_low);};
    void print( double ll_x, double ll_y, double ur_x, double ur_y) {};
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y);
    void draw_inverse() {};
    void nav_draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y);
    void erase()
	{ erase( (GlowTransform *)NULL, hot, NULL);};
    void nav_erase()
	{ nav_erase( (GlowTransform *)NULL, NULL);};
    void move( int delta_x, int delta_y, int grid);
    void move_noerase( int delta_x, int delta_y, int grid);
    void set_highlight( int on);
    int get_highlight() {return highlight;};
    void set_inverse( int on) {};
    int get_inverse() {return inverse;};
    void set_hot( int on) {};
    void select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy);
    glow_eObjectType type() { return glow_eObjectType_GrowRect;};
    void	measure( double *ll_x, double *ll_y, double *ur_x, double *ur_y)
	{ *ll_x = x_left; *ll_y = y_low; *ur_x = x_right; *ur_y = y_high;};
    void get_object_name( char *name) { strcpy( name, n_name);};
    void set_object_name( char *name) { strcpy( n_name, name);};
    void align( double x, double y, glow_eAlignDirection direction);
    double	x_right;
    double	x_left;
    double	y_high;
    double	y_low;
    int		hot;
    GlowPoint 	pzero;
    GlowPoint	stored_pos;
    char	n_name[32];
    int		highlight;
    int		inverse;
    void 	*user_data;
    glow_eDrawType	original_border_drawtype;
    glow_eDrawType	original_fill_drawtype;
    glow_eDrawType	fill_drawtype;
    int		border;
    char 	*dynamic;
    int 	dynamicsize;
    GlowTransform trf;
    char        last_group[32];
    void set_user_data( void *data) { user_data = data;};
    void get_user_data( void **data) { *data = user_data;};
    void set_dynamic( char *code, int size);
    void get_dynamic( char **code, int *size) { *code = dynamic; 
	*size = dynamicsize;};
    void *get_ctx() { return this->ctx;};
    void exec_dynamic();
    void set_position( double x, double y);
    void set_scale( double scale_x, double scale_y, double x0, double y0, 
		glow_eScaleType type);
    void set_rotation( double angel, 
		double x0, double y0, glow_eRotationPoint type);
    void set_fill_color( glow_eDrawType drawtype) 
	{ fill_drawtype = drawtype; draw();};
    void reset_fill_color() 
	{ fill_drawtype = original_fill_drawtype; draw();};
    void set_border_color( glow_eDrawType drawtype) 
	{ draw_type = drawtype; draw();};
    void reset_border_color()
	{ draw_type = original_border_drawtype; draw();};
    void set_original_fill_color( glow_eDrawType drawtype) 
	{ original_fill_drawtype = drawtype; reset_fill_color();};
    void set_original_border_color( glow_eDrawType drawtype) 
	{ original_border_drawtype = drawtype; reset_border_color();};
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node);
    void draw();
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void nav_erase( GlowTransform *t, void *node);
    void set_transform( GlowTransform *t);
    void set_transform_from_stored( GlowTransform *t) 
	{ trf.set_from_stored( t), get_node_borders(); };
    void store_transform() { trf.store(); };
    void set_linewidth( int linewidth);
    void set_fill( int fillval);
    void set_border( int borderval);
    void get_ctx( void **c) { *c = (void *)ctx;};
    int draw_annot_background( GlowTransform *t, void *node, double x, double y);
    int get_annot_background( GlowTransform *t, void *node,
		glow_eDrawType *background);
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);
    void set_last_group( char *name) { strcpy( last_group, name); };
    char *get_last_group() { return last_group; };
    ~GrowRect();
};


#endif







