#ifndef glow_colpalctx_h
#define glow_colpalctx_h

#include <iostream.h>
#include <fstream.h>

#include "glow.h"
#include "glow_ctx.h"

class ColPalCtx : public GlowCtx {
  public:
    ColPalCtx( char *ctx_name, double zoom_fact = 100) :
    	GlowCtx( ctx_name, zoom_fact), columns(10), 
	current_fill( glow_eDrawType_LineGray), current_border( glow_eDrawType_Line),
	entry_width(0.9), entry_height(1), display_entry_width(4.5)
	{ ctx_type = glow_eCtxType_ColPal; grid_on = 0; };
    ~ColPalCtx() {};
    void configure();
    void change_scrollbar();
    void redraw();
    void zoom( double factor);
    void unzoom() { zoom( base_zoom_factor / zoom_factor_y);};
    void print( char *filename);
    int event_handler( glow_eEvent event, int x, int y, int w, int h);
    int columns;
    glow_eDrawType current_fill;
    glow_eDrawType current_border;
    GlowArrayElem *display_fill;
    GlowArrayElem *display_border;
    double entry_width;
    double entry_height;
    double display_entry_width;
};

void colpal_scroll_horizontal( ColPalCtx *ctx, int value, int bottom);
void colpal_scroll_vertical( ColPalCtx *ctx, int value, int bottom);

#endif
