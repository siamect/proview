#ifndef glow_polyline_h
#define glow_polyline_h

#include <iostream.h>
#include <fstream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"
#include "glow_draw.h"

class GlowPolyLine : public GlowArrayElem {
  public:
    GlowPolyLine() {};
    GlowPolyLine( GlowCtx *glow_ctx, glow_sPoint *pointarray, 
		int point_cnt, glow_eDrawType d_type = glow_eDrawType_Line, 
		int line_w = 1, int fix_line_w = 0, int line_fill = 0,
		int closed = 0);
    ~GlowPolyLine();
    GlowPolyLine( const GlowPolyLine& c);

    friend ostream& operator<< ( ostream& o, const GlowPolyLine l);
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
    void move( void *pos, glow_sPoint *pointarray, int point_cnt,
	int highlight, int hot);
    void shift( void *pos, double delta_x, double delta_y, int highlight, int hot);
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir) 
		{ return 0;};
    glow_eObjectType type() { return glow_eObjectType_PolyLine;};
    void add_points( void *pos, glow_sPoint *pointarray, int point_cnt, 
		int highlight, int hot);
    void set_fill( int fillval) { fill = fillval;};
    void set_drawtype( glow_eDrawType drawtype) { draw_type = drawtype;};
    void set_linewidth( int linewidth) {line_width = linewidth;};
    GlowCtx *ctx;    
    GlowArray a_points;
    XPoint *points;
    glow_eDrawType draw_type;
    int	 line_width;
    int  fix_line_width;
    int  fill;
    int  closed_line;
};

#endif
