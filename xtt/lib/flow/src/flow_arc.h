#ifndef flow_arc_h
#define flow_arc_h

#include <iostream.h>
#include <fstream.h>
#include "flow.h"
#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_array_elem.h"


class FlowArc : public FlowArrayElem {
  public:
    FlowArc() {};
    FlowArc( FlowCtx *flow_ctx, double x1 = 0, double y1 = 0, double x2 = 0, 
		double y2 = 0, int ang1 = 0, int ang2 = 0, 
		flow_eDrawType d_type = flow_eDrawType_Line, int line_w = 1) : 
	ctx(flow_ctx), angel1(ang1), angel2(ang2),
	ll(flow_ctx,x1,y1), ur(flow_ctx,x2,y2),
	draw_type(d_type), line_width(line_w) {};
    friend ostream& operator<< ( ostream& o, const FlowArc a);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    int	event_handler( void *pos, flow_eEvent event, int x, int y, void *node);
    void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};
    void print( void *pos, void *node, int highlight);
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int highlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void erase( void *pos, int hot, void *node);
    void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};
    void nav_erase( void *pos, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    void move( void *pos, double x1, double y1, double x2, double y2,
	int ang1, int ang2, int highlight, int hot);
    void shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot);
    int get_conpoint( int num, double *x, double *y, flow_eDirection *dir) 
		{ return 0;};
    flow_eObjectType type() { return flow_eObjectType_Arc;};
    FlowCtx *ctx;    
    int	      angel1;
    int	      angel2;
    FlowPoint ll;
    FlowPoint ur;
    flow_eDrawType draw_type;
    int	 line_width;
};

#endif
