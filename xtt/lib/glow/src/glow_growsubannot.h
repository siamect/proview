#ifndef glow_growsubannot_h
#define glow_growsubannot_h

#include "glow_annot.h"
#include "glow_text.h"
#include "glow_rect.h"

class GrowSubAnnot : public GlowAnnot {
  public:
    GrowSubAnnot( GlowCtx *glow_ctx, char *name, double x = 0, double y = 0,
	int annot_num = 0, glow_eDrawType d_type = glow_eDrawType_TextHelveticaBold,
	glow_eDrawType color_d_type = glow_eDrawType_Line,
	int t_size = 2, glow_eAnnotType a_type = glow_eAnnotType_OneLine,
	int rel_pos = 0, glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
	int nodraw =0);
    ~GrowSubAnnot();
    int	event_handler( glow_eEvent event, int x, int y, double fx, double fy);
    void get_borders(
	double *x1_right, double *x1_left, double *y1_high, double *y1_low)
        { rect.get_borders( 0, 0, x1_right, x1_left, y1_high, y1_low, NULL);};
    void get_node_borders( )
	{ x_left = y_low = 1e37; x_right = y_high = -1e37;
	  rect.get_borders( 0, 0, &x_right, &x_left, 
		&y_high, &y_low, NULL);};
    void print( double ll_x, double ll_y, double ur_x, double ur_y) {};
    void zoom();
    void nav_zoom();
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y);
    void draw_inverse() {};
    void nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y);
    void nav_draw(int ll_x, int ll_y, int ur_x, int ur_y);
    void erase()
	{ text.erase( (void *)&pzero, hot, NULL);
          rect.erase( (void *)&pzero, hot, NULL);};
    void nav_erase()
	{ text.nav_erase( (void *)&pzero, NULL);
          rect.nav_erase( (void *)&pzero, NULL);};
    void move( double delta_x, double delta_y, int grid);
    void move_noerase( int delta_x, int delta_y, int grid);
    void set_highlight( int on);
    int get_highlight() {return highlight;};
    void set_inverse( int on) {};
    int get_inverse() {return inverse;};
    void set_hot( int on) {};
    void select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy);
    glow_eObjectType type() { return glow_eObjectType_GrowSubAnnot;};
    void	measure( double *ll_x, double *ll_y, double *ur_x, double *ur_y)
	{ *ll_x = x_left; *ll_y = y_low; *ur_x = x_right; *ur_y = y_high;};
    void get_object_name( char *name) { strcpy( name, n_name);};
    void set_object_name( char *name) { strcpy( n_name, name);};
    double	x_right;
    double	x_left;
    double	y_high;
    double	y_low;
    int		hot;
    GlowPoint	pzero;
    GlowPoint	stored_pos;
    char	n_name[32];
    int		highlight;
    int		inverse;
    GlowText	text;
    GlowRect	rect;
    void *user_data;
    GlowTransform trf;
    void set_user_data( void *data) { user_data = data;};
    void get_user_data( void **data) { *data = user_data;};
    void *get_ctx() { return this->ctx;};
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node);
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void nav_erase( GlowTransform *t, void *node);
    void set_transform( GlowTransform *t);
    void set_transform_from_stored( GlowTransform *t) 
	{ trf.set_from_stored( t), get_node_borders(); };
    void store_transform() { trf.store(); };
    void get_ctx( void **c) { *c = (void *)ctx;};
    void align( double x, double y, glow_eAlignDirection direction);
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);
    void convert( glow_eConvert version);
    void set_original_text_color( glow_eDrawType drawtype) 
	{ color_drawtype = drawtype; text.color_drawtype = drawtype; draw();};
    void draw();
};

#endif
