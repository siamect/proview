
#ifndef glow_radiobutton_h
#define glow_radiobutton_h

#include <iostream.h>
#include <fstream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"


class GlowRadiobutton : public GlowArrayElem {
  public:
    GlowRadiobutton( GlowCtx *glow_ctx, double x = 0, double y = 0, 
		double w = 0, double h = 0, int annot_num = 0, 
		glow_eDrawType d_type = glow_eDrawType_Line, int line_w = 1) : 
	ctx(glow_ctx), ll(glow_ctx,x,y), ur(glow_ctx,x+w,y+h), 
	draw_type(d_type), line_width(line_w), number(annot_num) {};
    friend ostream& operator<< ( ostream& o, const GlowRadiobutton r);
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
    void draw( void *pos, int hightlight, int hot, void *node);
    void nav_draw( void *pos, int highlight, void *node);
    void draw_inverse( void *pos, int hot, void *node)
	{ erase( pos, hot, node);};
    void erase( void *pos, int hot, void *node);
    void nav_erase( void *pos, void *node);
    void get_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node);
    void move( void *pos, double x, double y, int highlight, int hot);
    void shift( void *pos, double delta_x, double delta_y,
	int highlight, int hot);
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir) 
		{ return 0;};
    glow_eObjectType type() { return glow_eObjectType_Radiobutton;};
    GlowCtx *ctx;    
    double width() { return ur.x - ll.x;};
    double height() { return ur.y - ll.y;};
    GlowPoint ll;
    GlowPoint ur;
    glow_eDrawType draw_type;
    int	line_width;
    int number;
};

#endif
