#ifndef glow_curvectx_h
#define glow_curvectx_h

#include <iostream.h>
#include <fstream.h>

#include "glow.h"
#include "glow_growctx.h"

class CurveCtx : public GrowCtx {
  public:
    CurveCtx( char *ctx_name, double zoom_fact = 100) :
        GrowCtx( ctx_name, zoom_fact), layout_adjusted(0)
	{ ctx_type = glow_eCtxType_Curve;};
    int layout_adjusted;
    void configure();
    void zoom( double factor);
    void nav_zoom();
    void get_zoom( double *factor_x, double *factor_y);
    void get_prefered_zoom_y( int height, double *factor_y);
    void adjust_layout();
    int event_handler_nav( glow_eEvent event, int x, int y);
    ~CurveCtx() {};
};

void curve_scroll_horizontal( CurveCtx *ctx, int value, int bottom);
void curve_scroll_vertical( CurveCtx *ctx, int value, int bottom);

#endif

