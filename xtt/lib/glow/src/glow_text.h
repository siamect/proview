#ifndef glow_text_h
#define glow_text_h

#include <iostream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"


class GlowText : public GlowArrayElem {
  public:
    GlowText( GlowCtx *glow_ctx, char *text1, double x = 0, double y = 0,
	glow_eDrawType d_type = glow_eDrawType_TextHelveticaBold, 
	int t_size = 2, glow_mDisplayLevel display_lev = glow_mDisplayLevel_1) : 
	ctx(glow_ctx), p(glow_ctx,x,y), draw_type(d_type), text_size(t_size),
	display_level(display_lev)
	{ strncpy( text, text1, sizeof(text));};
    friend ostream& operator<< ( ostream& o, const GlowText t);
    void zoom();
    void nav_zoom();
    void print_zoom();
    void traverse( int x, int y);
    int	event_handler( void *pos, glow_eEvent event, int x, int y, void *node);
    void conpoint_select( void *pos, int x, int y, double *distance, 
		void **cp) {};
    void print( void *pos, void *node);
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( void *pos, int highlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};
    void erase( void *pos, int hot, void *node);
    void nav_erase( void *pos, void *node);
    void move( void *pos, double x, double y, int highlight, int hot);
    void shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir) 
		{ return 0;};
    glow_eObjectType type() { return glow_eObjectType_Text;};
    GlowCtx *ctx;    
    GlowPoint p;
    char text[80];
    glow_eDrawType draw_type;
    int	 text_size;
    glow_mDisplayLevel display_level;
};

#endif
