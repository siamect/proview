#ifndef flow_frame_h
#define flow_frame_h

#include <iostream.h>
#include <fstream.h>
#include "flow.h"
#include "flow_ctx.h"
#include "flow_point.h"
#include "flow_rect.h"


class FlowFrame : public FlowRect {
  public:
    FlowFrame( FlowCtx *flow_ctx, double x = 0, double y = 0, double w = 0, 
		double h = 0, flow_eDrawType d_type = flow_eDrawType_Line, 
		int line_w = 1, int fix_line_w = 0) : 
	FlowRect( flow_ctx, x, y, w, h, d_type, line_w, fix_line_w) {};
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int hightlight, int hot, void *node);
    void erase( void *pos, int hot, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    flow_eObjectType type() { return flow_eObjectType_Frame;};
};

#endif
