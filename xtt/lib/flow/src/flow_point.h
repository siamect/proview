#ifndef flow_point_h
#define flow_point_h

#include <iostream.h>
#include <fstream.h>

#include "flow.h"
#include "flow_ctx.h"
#include "flow_array_elem.h"

class FlowPoint : public FlowArrayElem {
  public:
    FlowPoint() {};
    FlowPoint( FlowCtx *flow_ctx, double x1 = 0, double y1 = 0);
    const FlowPoint& operator+= (const FlowPoint p);
    FlowPoint operator+ (const FlowPoint p);
    const FlowPoint& operator-= (const FlowPoint p);
    FlowPoint operator- (const FlowPoint p);
    friend ostream& operator<< ( ostream& o, const FlowPoint p);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    void posit( double x, double y);
    void posit_z( int x, int y);
    void save( ofstream& fp, flow_eSaveMode mode);
    void open( ifstream& fp);
    FlowCtx *ctx;    
    double x;
    double y;
    int z_x;
    int z_y;
    int nav_z_x;
    int nav_z_y;
    double print_z_x;
    double print_z_y;
    double current_zoom_factor;
};

#endif
