#ifndef glow_growbar_h
#define glow_growbar_h

#include "glow_growrect.h"
#include "glow_tracedata.h"

class GrowBar : public GrowRect {
  public:
    GrowBar( GlowCtx *glow_ctx, char *name, double x = 0, double y = 0, 
		double w = 0, double h = 0, 
		glow_eDrawType border_d_type = glow_eDrawType_Line, 
		int line_w = 1, 
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		int fill_rect = 0, int display_border = 1, 
		glow_eDrawType fill_d_type = glow_eDrawType_Line, int nodraw = 0);
    ~GrowBar();
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void erase()
	{ erase( (GlowTransform *)NULL, hot, NULL);};
    void nav_erase()
	{ nav_erase( (GlowTransform *)NULL, NULL);};
    void draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void draw( int *ll_x, int *ll_y, int *ur_x, int *ur_y);
    void nav_draw(int ll_x, int ll_y, int ur_x, int ur_y);
    void set_highlight( int on);
    glow_eObjectType type() { return glow_eObjectType_GrowBar;};
    void set_trace_attr( GlowTraceData *attr);
    void get_trace_attr( GlowTraceData **attr);
    double		max_value;
    double		min_value;
    double		bar_value;
    glow_eDrawType	bar_drawtype;
    glow_eDrawType	bar_bordercolor;
    int			bar_borderwidth;
    GlowTraceData	trace;
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node);
    void draw();
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void nav_erase( GlowTransform *t, void *node);
    void trace_scan();
    int trace_init();
    void trace_close();
    void set_value( double value) { bar_value = value; 
	                            if ( !fill) erase(); 
	                            draw();};
    void align( double x, double y, glow_eAlignDirection direction);
    void set_range( double min, double max);
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc,  ofstream &fp);
};


#endif
