#ifndef flow_annotpixmap_h
#define flow_annotpixmap_h

#include <iostream.h>
#include "flow.h"
#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_array_elem.h"


class FlowAnnotPixmap : public FlowArrayElem {
  public:
    FlowAnnotPixmap( FlowCtx *flow_ctx, int num,
	 double x = 0, double y = 0,
	flow_eDrawType d_type = flow_eDrawType_Line, int size = 2, int rel_pos = 0);
    FlowAnnotPixmap( const FlowAnnotPixmap& p);
    ~FlowAnnotPixmap();
    friend ostream& operator<< ( ostream& o, const FlowAnnotPixmap t);
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
    void configure_annotations( void *pos, void *node);
    flow_eObjectType type() { return flow_eObjectType_AnnotPixmap;};
    FlowCtx *ctx;
    FlowPoint p;
    int number;
    int pixmap_size;
    flow_eDrawType draw_type;
    int relative_pos;
};

void flow_annot_pixmap_create( FlowCtx *ctx, flow_sPixmapData *pixmap_data,
	flow_sAnnotPixmap **pixmap);
void flow_annot_pixmap_free( FlowCtx *ctx, flow_sAnnotPixmap *pixmap);

#endif
