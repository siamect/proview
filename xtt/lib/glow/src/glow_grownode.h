#ifndef glow_grownode_h
#define glow_grownode_h

#include "glow_node.h"

class GrowNode : public GlowNode {
  public:
    GrowNode( GlowCtx *glow_ctx, char *name, GlowNodeClass *node_class,
	double x1, double y1, int nodraw = 0, int rel_annot_pos = 0);
    GrowNode() {};
    glow_eObjectType type() { return object_type;};
    void copy_from( const GrowNode& n);
    void erase()
	{ erase( (GlowTransform *)NULL, hot, NULL);};
    void nav_erase()
	{ nav_erase( (GlowTransform *)NULL, NULL);};
    void get_node_borders();
    void get_borders( GlowTransform *t,
	double *x1_right, double *x1_left, double *y1_high, double *y1_low);
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void move( int delta_x, int delta_y, int grid);
    void move_noerase( int delta_x, int delta_y, int grid);
    void move_to( double x, double y);
    void set_highlight( int on);
    void trace_close();
    int event_handler( glow_eEvent event, double fx, double fy);
    int event_handler( glow_eEvent event, int x, int y, double fx, double fy);
    void set_hot( int on);
    char 	*dynamic;
    int 	dynamicsize;
    glow_eDrawType	original_border_drawtype;
    glow_eDrawType	original_fill_drawtype;
    glow_eDrawType	fill_drawtype;
    glow_eDrawType	draw_type;
    glow_eDrawTone	original_color_tone;
    glow_eDrawTone	color_tone;
    int		original_color_lightness;
    int		color_lightness;
    int		original_color_intensity;
    int		color_intensity;
    int		original_color_shift;
    int		color_shift;
    int		color_inverse;
    int		line_width;
    int		invisible;
    GlowTransform trf;
    char	*argv[20];
    int		argsize[20];
    glow_eObjectType object_type;
    GrowNode    *root_node;
    char        last_group[32];
    void set_dynamic( char *code, int size);
    void get_dynamic( char **code, int *size) { *code = dynamic; 
	*size = dynamicsize;};
    void exec_dynamic();
    void set_fill_color( glow_eDrawType drawtype) 
	{ if ( fill_drawtype == drawtype) return;
          fill_drawtype = drawtype; 
          color_tone = glow_eDrawTone_No;
          color_shift = 0;
          color_lightness = 0;
          color_intensity = 0;
          draw();};
    void reset_fill_color() 
	{ if ( fill_drawtype == original_fill_drawtype) return;
          fill_drawtype = original_fill_drawtype; 
          color_tone = original_color_tone;
          color_shift = original_color_shift;
          color_lightness = original_color_lightness;
          color_intensity = original_color_intensity;
          draw();};
    void set_border_color( glow_eDrawType drawtype) 
	{ if ( draw_type == drawtype) return;
          draw_type = drawtype;
          color_tone = glow_eDrawTone_No;
          color_shift = 0;
          color_lightness = 0;
          color_intensity = 0;
          draw();};
    void reset_border_color()
	{ if ( draw_type == original_border_drawtype) return;
          draw_type = original_border_drawtype; 
          draw();};
    void set_original_fill_color( glow_eDrawType drawtype) 
	{ original_color_tone = glow_eDrawTone_No;
          color_tone = glow_eDrawTone_No;
          original_color_shift = 0;
          color_shift = 0;
          original_color_lightness = 0;
          color_lightness = 0;
          original_color_intensity = 0;
          color_intensity = 0;
	  original_fill_drawtype = drawtype; 
          reset_fill_color();};
    void set_original_border_color( glow_eDrawType drawtype) 
	{ original_color_tone = glow_eDrawTone_No;
          color_tone = glow_eDrawTone_No;
          original_color_shift = 0;
          color_shift = 0;
          original_color_lightness = 0;
          color_lightness = 0;
          original_color_intensity = 0;
          color_intensity = 0;
	  original_border_drawtype = drawtype;
          reset_border_color();};
    void set_original_color_tone( glow_eDrawTone tone) 
	{ original_color_tone = tone; 
          original_color_shift = 0;
          color_shift = 0;
          original_color_lightness = 0;
          color_lightness = 0;
          original_color_intensity = 0;
          color_intensity = 0;
          reset_color_tone();};
    void set_color_tone( glow_eDrawTone tone) 
	{ if ( color_tone == tone) return;
          color_tone = tone; 
          color_shift = 0;
          draw();};
    void reset_color_tone() 
	{ if ( color_tone == original_color_tone) return;
          color_tone = original_color_tone; 
          color_shift = original_color_shift;
          draw();};
    void set_original_color_lightness( int lightness) 
	{ original_color_lightness = lightness; 
          reset_color_lightness();};
    void incr_original_color_lightness( int lightness) 
	{ original_color_lightness += lightness; 
          if ( original_color_lightness > 5) original_color_lightness = 5;
          if ( original_color_lightness < -5) original_color_lightness = -5;
          reset_color_lightness();};
    void set_color_lightness( int lightness) 
	{ if ( color_lightness == lightness) return;
          color_lightness = lightness; 
          draw();};
    void reset_color_lightness() 
	{ if ( color_lightness == original_color_lightness) return;
          color_lightness = original_color_lightness;
          draw();};
    void set_original_color_intensity( int intensity) 
	{ original_color_intensity = intensity; 
          reset_color_intensity();};
    void incr_original_color_intensity( int intensity) 
	{ original_color_intensity += intensity; 
          if ( original_color_intensity > 1) original_color_intensity = 1;
          if ( original_color_intensity < -2) original_color_intensity = -2;
          reset_color_intensity();};
    void set_color_intensity( int intensity) 
	{ if ( color_intensity == intensity) return;
          color_intensity = intensity; 
          draw();};
    void reset_color_intensity() 
	{ if ( color_intensity == original_color_intensity) return;
          color_intensity = original_color_intensity;
          draw();};
    void set_original_color_shift( int shift) 
	{ original_color_shift = shift; 
          reset_color_shift();};
    void incr_original_color_shift( int shift) 
	{ original_color_shift += shift; 
          reset_color_shift();};
    void incr_color_shift( int shift) 
	{ if ( !shift) return;
          color_shift += shift; 
          draw();};
    void set_color_shift( int shift) 
	{ if ( color_shift == shift) return;
          color_shift = shift; 
          draw();};
    void reset_color_shift() 
	{ if ( color_shift == original_color_shift) return;
          color_shift = original_color_shift;
          draw();};
    void set_color_inverse( int inverse) 
	{ color_inverse = inverse;
          draw();};
    void set_visibility( int visible) 
	{ if ( invisible == !visible) return;
          invisible = !visible;
          if ( !visible) erase();
          draw();};
    void set_position( double x, double y);
    void set_scale_pos( double x, double y, double scale_x, double scale_y, 
		double x0, double y0, glow_eScaleType type);
    void set_scale( double scale_x, double scale_y, double x0, double y0, 
		glow_eScaleType type);
    void set_rotation( double angel, 
		double x0, double y0, glow_eRotationPoint type);
    void draw();
    void draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y);
    void nav_draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void nav_draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y);
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node);
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void nav_erase( GlowTransform *t, void *node);
    void set_transform( GlowTransform *t);
    void set_transform_from_stored( GlowTransform *t) 
	{ trf.set_from_stored( t), get_node_borders(); };
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir);
    void set_annotation( int num, char *text, int size, int nodraw);
    void store_transform() { trf.store(); };
    int get_argument( int num, char *arg, int size);
    int set_argument( int num, char *arg, int size);
    void set_linewidth( int linewidth);
    void get_ctx( void **c) { *c = (void *)ctx;};
    int draw_annot_background(  GlowTransform *t, double x, double y)
	{ return nc->draw_annot_background( t, (void *)this, x, y);}; 
    void align( double x, double y, glow_eAlignDirection direction);
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);
    int set_next_nodeclass();
    int set_previous_nodeclass();
    void set_root_nodeclass();
    void set_last_nodeclass();
    int set_nodeclass_by_index( int idx);
    void set_nodeclass( GlowNodeClass *new_nc);
    void get_animation_count( int *count) { *count = nc->animation_count;}; 
    int find_nc( GlowArrayElem *nodeclass);
    void ungroup() { root_node = 0;};
    void set_last_group( char *name) { strcpy( last_group, name); };
    char *get_last_group() { return last_group; };
    int is_sensitive();
    int is_click_sensitive();
    int is_refobject_sensitive();
    int get_background_object_limits( GlowTransform *t, glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction);
    void get_nodeclass_origo( GlowTransform *t, double *x, double *y);

    ~GrowNode();
};


#endif




