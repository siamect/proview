#ifndef glow_conclass_h
#define glow_conclass_h

#include <iostream.h>
#include <fstream.h>
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_array_elem.h"
#include "glow_array.h"

class GlowConClass : public GlowArrayElem {
  public:
    GlowConClass( GlowCtx *glow_ctx, char *name, glow_eConType contype,
	glow_eCorner cornertype, glow_eDrawType d_type, int line_w,
	double arrow_w = 0, double arrow_l = 0, double round_corner_amnt = 0.5,
	glow_eConGroup grp = glow_eConGroup_Common);
    friend ostream& operator<< ( ostream& o, const GlowConClass cc);
    void zoom() {};
    void nav_zoom() {};
    void print_zoom() {};
    void traverse( int x, int y) {};
    void get_borders( double pos_x, double pos_y, double *x_right, 
	double *x_left, double *y_high, double *y_low, void *node)
	{};
    int	event_handler( void *pos, glow_eEvent event, int x, int y, void *node)
		 { return 0;};
    void print( GlowPoint *pos, void *node) {};
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( GlowPoint *pos, int highlight, int hot, void *node) {};
    void nav_draw( GlowPoint *pos, int highlight, void *node) {};
    void erase( GlowPoint *pos, int hot, void *node) {};
    void nav_erase( GlowPoint *pos, void *node) {};
    glow_eObjectType type() { return glow_eObjectType_ConClass;};
    void get_object_name( char *name);
    GlowCtx	*ctx;
    GlowPoint	zero;
    char	cc_name[32];
    glow_eConType con_type;
    glow_eCorner corner;
    glow_eDrawType draw_type;
    int	 line_width;
    double arrow_width;
    double arrow_length;
    double round_corner_amount;
    glow_eConGroup group;
};

#endif

