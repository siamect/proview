#ifndef glow_frame_h
#define glow_frame_h

#include <iostream.h>
#include <fstream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_rect.h"


class GlowFrame : public GlowRect {
  public:
    GlowFrame( GlowCtx *glow_ctx, double x = 0, double y = 0, double w = 0, 
		double h = 0, glow_eDrawType d_type = glow_eDrawType_Line, 
		int line_w = 1, int fix_line_w = 0) : 
	GlowRect( glow_ctx, x, y, w, h, d_type, line_w, fix_line_w) {};
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int highlight, int hot, void *node);
    void erase( void *pos, int hot, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    glow_eObjectType type() { return glow_eObjectType_Frame;};
};

#endif
