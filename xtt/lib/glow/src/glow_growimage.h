#ifndef glow_growimage_h
#define glow_growimage_h

#include "glow_transform.h"
#include "glow_growctx.h"
#include "glow_point.h"
#include "glow_draw.h"

class GrowImage : public GlowArrayElem {
  public:
    GrowImage( GlowCtx *glow_ctx, char *name, double x = 0, double y = 0, 
	       char *imagefile = 0,
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1);
    GrowImage() {};
    void copy_from( const GrowImage& im);
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
    glow_eObjectType type() { return glow_eObjectType_GrowImage;};
    void	measure( double *ll_x, double *ll_y, double *ur_x, double *ur_y)
	{ *ll_x = x_left; *ll_y = y_low; *ur_x = x_right; *ur_y = y_high;};
    void get_object_name( char *name) { strcpy( name, n_name);};
    void set_object_name( char *name) { strcpy( n_name, name);};
    void align( double x, double y, glow_eAlignDirection direction);
    int update();

    GlowPoint ll;
    GlowPoint ur;
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
    char        image_filename[120];
    char 	*dynamic;
    int 	dynamicsize;
    GlowTransform trf;
    ImlibData   *imlib;
    ImlibImage  *image;
    ImlibImage  *original_image;
    Pixmap      pixmap;
    Pixmap      nav_pixmap;
    Pixmap      clip_mask;
    Pixmap      nav_clip_mask;
    GlowCtx *ctx;
    glow_mDisplayLevel display_level;
    int       current_width;
    int       current_height;
    int       current_nav_width;
    int       current_nav_height;
    glow_eDrawTone	color_tone;
    int		color_lightness;
    int		color_intensity;
    int		color_shift;
    int		color_inverse;
    glow_eDrawTone	current_color_tone;
    int		current_color_lightness;
    int		current_color_intensity;
    int		current_color_shift;
    int		current_color_inverse;
    int         current_direction;
    glow_eDrawTone	current_nav_color_tone;
    int		current_nav_color_lightness;
    int		current_nav_color_intensity;
    int		current_nav_color_shift;
    int		current_nav_color_inverse;
    int         current_nav_direction;
    char        last_group[32];
    int		date;
    char	filename[256];
 
    int insert_image( char *imagefile);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
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
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node);
    void draw();
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void nav_erase( GlowTransform *t, void *node);
    void set_original_color_tone( glow_eDrawTone tone) 
	{ set_color_tone( tone);}
    void set_color_tone( glow_eDrawTone tone) 
	{ if ( color_tone == tone) return;
          color_tone = tone; 
          color_shift = 0;
          draw();};
    void set_original_color_lightness( int lightness) 
	{ set_color_lightness( lightness);};
    void incr_original_color_lightness( int lightness) 
	{ color_lightness += lightness; 
          if ( color_lightness > 5) color_lightness = 5;
          if ( color_lightness < -5) color_lightness = -5;
          draw();};
    void set_color_lightness( int lightness) 
	{ if ( color_lightness == lightness) return;
          color_lightness = lightness; 
          draw();};
    void set_original_color_intensity( int intensity) 
	{ set_color_intensity( intensity);};
    void incr_original_color_intensity( int intensity)
	{ color_intensity += intensity;
          if ( color_intensity > 5) color_intensity = 5;
          if ( color_intensity < -5) color_intensity = -5;
          draw();};
    void set_color_intensity( int intensity) 
	{ if ( color_intensity == intensity) return;
          color_intensity = intensity; 
          draw();};
    void set_original_color_shift( int shift) 
	{ set_color_shift( shift);};
    void incr_original_color_shift( int shift) 
	{ incr_color_shift( shift);};
    void incr_color_shift( int shift) 
	{ if ( !shift) return;
          color_shift += shift; 
          draw();};
    void set_color_shift( int shift) 
	{ if ( color_shift == shift) return;
          color_shift = shift; 
          draw();};
    void set_color_inverse( int inverse) 
	{ color_inverse = inverse;
          draw();};
    int set_image_color( ImlibImage *image, void *node);
    void set_transform( GlowTransform *t);
    void set_transform_from_stored( GlowTransform *t) 
	{ trf.set_from_stored( t), get_node_borders(); };
    void store_transform() { trf.store(); };
    void get_ctx( void **c) { *c = (void *)ctx;};
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);
    void set_last_group( char *name) { strcpy( last_group, name); };
    char *get_last_group() { return last_group; };

    ~GrowImage();

};

int grow_image_to_pixmap( GlowCtx *ctx, char *imagefile, 
	    int width, int height, Pixmap *pixmap, int *w, int *h);

#endif




