#ifndef glow_browctx_h
#define glow_browctx_h

#include <iostream.h>
#include <fstream.h>

#include "glow.h"
#include "glow_ctx.h"

#define BrowCtx BrowCtxDummy

class BrowCtx : public GlowCtx {
  public:
    BrowCtx( char *ctx_name, double zoom_fact = 100) :
    	GlowCtx( ctx_name, zoom_fact), 
	indentation(0.8), frame_x_right(0), annotation_space(0.3)
	{ ctx_type = glow_eCtxType_Brow; };
    ~BrowCtx() {};
    int insert( GlowArrayElem *element, GlowArrayElem *destination, 
		glow_eDest destination_code);
    void close( GlowArrayElem *element);
    void remove( GlowArrayElem *element);
    void configure( double y_redraw);
    void change_scrollbar();
    void redraw( double y_redraw);
    void zoom( double factor);
    void unzoom() { zoom( base_zoom_factor / zoom_factor_y);};
    void print( char *filename);
    int get_first( GlowArrayElem **first) { return a.get_first( first);};
    int get_last( GlowArrayElem **last) { return a.get_last( last);};
    int get_previous( GlowArrayElem *element, GlowArrayElem **prev)
    		{ return a.get_previous( element, prev);};
    int get_next( GlowArrayElem *element, GlowArrayElem **next)
    		{ return a.get_next( element, next);};
    int get_parent( GlowArrayElem *element, GlowArrayElem **parent)
    		{ return a.brow_get_parent( element, parent);};
    int is_visible( GlowArrayElem *element);
    void center_object( GlowArrayElem *object, double factor);
    double indentation;
    double frame_x_right;
    double annotation_space;
};

// void brow_scroll_horizontal( BrowCtx *ctx, int value, int bottom);
// void brow_scroll_vertical( BrowCtx *ctx, int value, int bottom);

#endif
