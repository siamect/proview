#ifndef glow_line_h
#define glow_line_h

#include <iostream.h>
#include <fstream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"


class GlowLine : public GlowArrayElem {
  public:
    GlowLine() {};
    GlowLine( GlowCtx *glow_ctx, double x1 = 0, double y1 = 0, double x2 = 0, 
		double y2 = 0, glow_eDrawType d_type = glow_eDrawType_Line, 
		int line_w = 1, int fix_line_w = 0) : 
	ctx(glow_ctx), p1(glow_ctx,x1,y1), p2(glow_ctx,x2,y2),
	draw_type(d_type), line_width(line_w), fix_line_width(fix_line_w) {};
    friend ostream& operator<< ( ostream& o, const GlowLine l);
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
    void draw( void *pos, int highlight, int hot, void *node);
    void nav_draw( void *pos, int hightlight, void *node);
    void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};
    void erase( void *pos, int hot, void *node);
    void nav_erase( void *pos, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    void move( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot);
    void move_noerase( void *pos, double x1, double y1, double x2, double y2,
	int highlight, int hot);
    void shift( void *pos, double delta_x, double delta_y, int highlight, int hot);
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir) 
		{ return 0;};
    glow_eObjectType type() { return glow_eObjectType_Line;};
    void set_drawtype( glow_eDrawType drawtype) { draw_type = drawtype;};
    void set_linewidth( int linewidth) {line_width = linewidth;};
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);
    GlowCtx *ctx;    
    GlowPoint p1;
    GlowPoint p2;
    glow_eDrawType draw_type;
    int	 line_width;
    int  fix_line_width;
};

#endif
