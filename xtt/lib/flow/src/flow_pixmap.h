#ifndef flow_pixmap_h
#define flow_pixmap_h

#include <iostream.h>
#include "flow.h"
#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_array_elem.h"


class FlowPixmap : public FlowArrayElem {
  public:
    FlowPixmap( FlowCtx *flow_ctx, flow_sPixmapData *pixmap_data,
	 double x = 0, double y = 0,
	flow_eDrawType d_type = flow_eDrawType_Line, int size = 2);
    FlowPixmap( const FlowPixmap& p);
    ~FlowPixmap();
    friend ostream& operator<< ( ostream& o, const FlowPixmap t);
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
    void draw( void *pos, int highlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void draw_inverse( void *pos, int hot, void *node);
    void erase( void *pos, int hot, void *node);
    void nav_erase( void *pos, void *node);
    void move( void *pos, double x, double y, int highlight, int hot);
    void shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    int get_conpoint( int num, double *x, double *y, flow_eDirection *dir) 
		{ return 0;};
    flow_eObjectType type() { return flow_eObjectType_Pixmap;};
    FlowCtx *ctx;
    FlowPoint p;
    void *pixmaps;
    flow_sPixmapData pixmap_data;
    int pixmap_size;
    flow_eDrawType draw_type;
};

#endif
