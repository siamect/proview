#ifndef glow_point_h
#define glow_point_h

#include <iostream.h>
#include <fstream.h>

#include "glow.h"
#include "glow_ctx.h"
#include "glow_array_elem.h"

class GlowPoint : public GlowArrayElem {
  public:
    GlowPoint() {};
    GlowPoint( GlowCtx *glow_ctx, double x1 = 0, double y1 = 0);
    const GlowPoint& operator+= (const GlowPoint p);
    GlowPoint operator+ (const GlowPoint p);
    const GlowPoint& operator-= (const GlowPoint p);
    GlowPoint operator- (const GlowPoint p);
    friend ostream& operator<< ( ostream& o, const GlowPoint p);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    void posit( double x, double y);
    void posit_z( int x, int y);
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    glow_eObjectType type() { return glow_eObjectType_Point;};
    GlowCtx *ctx;    
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
