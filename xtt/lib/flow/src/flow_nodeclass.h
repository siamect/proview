#ifndef flow_nodeclass_h
#define flow_nodeclass_h

#include <iostream.h>
#include <fstream.h>
#include "flow_point.h"
#include "flow_rect.h"
#include "flow_array_elem.h"
#include "flow_array.h"

class FlowNodeClass : public FlowArrayElem {
  public:
    FlowNodeClass( FlowCtx *flow_ctx, char *name, 
	flow_eNodeGroup grp = flow_eNodeGroup_Common);
    void insert( FlowArrayElem *element) 
		{ a.insert( element);};
    void zoom() { a.zoom();};
    void nav_zoom() { a.nav_zoom();};
    void print_zoom() { a.print_zoom();};
    void traverse( int x, int y) { a.traverse( x, y);};
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node)
	{ a.get_borders(pos_x, pos_y, x_right, x_left, y_high, y_low, node);};
    void get_obstacle_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    int	event_handler( void *pos, flow_eEvent event, int x, int y, void *node);
    void print( FlowPoint *pos, void *node, int highlight);
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( FlowPoint *pos, int highlight, int hot, void *node);
    void nav_draw( FlowPoint *pos, int highlight, void *node);
    void draw_inverse( FlowPoint *pos, int hot, void *node);
    void erase( FlowPoint *pos, int hot, void *node);
    void nav_erase( FlowPoint *pos, void *node);
    int get_conpoint( int num, double *x, double *y, flow_eDirection *dir);
    flow_eObjectType type() { return flow_eObjectType_NodeClass;};
    void erase_annotation( void *pos, int highlight, int hot, 
	void *node, int num);
    void draw_annotation( void *pos, int highlight, int hot, 
	void *node, int num);
    void configure_annotations( void *pos, void *node);
    void measure_annotation( int num, char *text, double *width,
	double *height);
    void get_object_name( char *name);
    void open_annotation_input( void *pos, void *node, int num);
    void close_annotation_input( void *node, int num);
    int get_annotation_input( void *node, int num, char **text);
    void move_widgets( void *node, int x, int y);
    int get_conpoint_trace_attr( int num, char *trace_attr, 
	flow_eTraceType *type);
    FlowCtx	*ctx;
    FlowArray 	a;
    char	nc_name[80];
    flow_eNodeGroup group;
};

#endif
