#ifndef glow_pushbutton_h
#define glow_pushbutton_h

#include <iostream.h>
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_text.h"
#include "glow_array_elem.h"
#include "glow_pushbutton.h"

class GlowPushButton : public GlowArrayElem {
  public:
    GlowPushButton( GlowCtx *glow_ctx, char *p_name,
	double x1, double y1, double width, double height);
    friend ostream& operator<< ( ostream& o, const GlowPushButton p);
    int	event_handler( glow_eEvent event, int x, int y);
    void zoom() {};
    void nav_zoom() {};
    void print_zoom() {};
    void traverse( int x, int y) {};
    void print( double ll_x, double ll_y, double ur_x, double ur_y) {};
    void draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void get_borders(
	double *x1_right, double *x1_left, double *y1_high, double *y1_low) {};
    void get_borders( double pos_x, double pos_y, 
	double *x_right, double *x_left, double *y_high, double *y_low) {};
    double		zoom_factor;
    GlowCtx 		*ctx;
    GlowPoint		pos;
    char		name[32];
    GlowRect		r;
    GlowText		t;
    void conpoint_refcon_redraw( void *node, int conpoint) {};
    void conpoint_refcon_erase( void *node, int conpoint) {};
    void remove_notify() {};
};

#endif
