#ifndef flow_image_h
#define flow_image_h

#include <iostream.h>
#include <fstream.h>
#include "flow.h"
#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_array_elem.h"
#include "flow_draw.h"

class FlowImage : public FlowArrayElem {
  public:
    FlowImage( FlowCtx *flow_ctx, char *imagefile, double x = 0, double y = 0,
	       flow_mDisplayLevel display_lev = flow_mDisplayLevel_1);
    friend ostream& operator<< ( ostream& o, const FlowImage r);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    int	event_handler( void *pos, flow_eEvent event, int x, int y, void *node);
    void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};
    void print( void *pos, void *node);
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int hightlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};
    void erase( void *pos, int hot, void *node);
    void nav_erase( void *pos, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    void move( void *pos, double x, double y, int highlight, int hot);
    void shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot);
    int get_conpoint( int num, double *x, double *y, flow_eDirection *dir) 
		{ return 0;};
    flow_eObjectType type() { return flow_eObjectType_Image;};
    int insert_image( char *imagefile);

    FlowCtx *ctx;    
    double width() { return ur.x - ll.x;};
    double height() { return ur.y - ll.y;};
    FlowPoint ll;
    FlowPoint ur;
    flow_eDrawType draw_type;
    int	line_width;
    int  fix_line_width;
    flow_mDisplayLevel display_level;
    int fill;
    ImlibData   *imlib;
    ImlibImage  *image;
    ImlibImage  *original_image;
    Pixmap      pixmap;
    Pixmap      nav_pixmap;
    Pixmap      clip_mask;
    Pixmap      nav_clip_mask;
    char        image_filename[120];
    int       current_width;
    int       current_height;
    int       current_nav_width;
    int       current_nav_height;
    double	current_zoom_factor;
};

#endif





