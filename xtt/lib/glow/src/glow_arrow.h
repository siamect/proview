#ifndef glow_arrow_h
#define glow_arrow_h

#include <stdlib.h>
#include <iostream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"


class GlowArrow : public GlowArrayElem {
  public:
    GlowArrow( GlowCtx *glow_ctx, double x1, double y1, double x2, 
		double y2, double w, double l,
	   	glow_eDrawType d_type = glow_eDrawType_Line);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    int	event_handler( void *pos, glow_eEvent event, int x, int y, void *node);
    void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};
    void print( void *pos, void *node);
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int hightlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void erase( void *pos, int hot, void *node);
    void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};
    void nav_erase( void *pos, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node) 
		{};
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir) 
		{ return 0;};
    glow_eObjectType type() { return glow_eObjectType_Arrow;};
    void move( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot);
    void shift( void *pos, double delta_x, double delta_y, 
	int highlight, int hot);
    void set_drawtype( glow_eDrawType drawtype) { draw_type = drawtype;};
    GlowCtx *ctx;    
    GlowPoint p_dest;
    GlowPoint p1;
    GlowPoint p2;
    double arrow_width;
    double arrow_length;
    glow_eDrawType draw_type;
    int	line_width;
};

#endif
