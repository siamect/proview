#ifndef glow_growtrend_h
#define glow_growtrend_h

#include "glow_growrect.h"
#include "glow_growpolyline.h"
#include "glow_tracedata.h"


class GrowTrend : public GrowRect {
  public:
    GrowTrend( GlowCtx *glow_ctx, char *name, double x = 0, double y = 0, 
		double w = 0, double h = 0, 
		glow_eDrawType border_d_type = glow_eDrawType_Line, 
		int line_w = 1, 
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		int fill_rect = 0, int display_border = 1, 
		glow_eDrawType fill_d_type = glow_eDrawType_Line, int nodraw = 0);
    ~GrowTrend();
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
    glow_eObjectType type() { return glow_eObjectType_GrowTrend;};
    void set_trace_attr( GlowTraceData *attr);
    void get_trace_attr( GlowTraceData **attr);
    void set_lines( int v, int h) { vertical_lines = v; horizontal_lines = h;};
    double		max_value[TREND_MAX_CURVES];
    double		min_value[TREND_MAX_CURVES];
    int			horizontal_lines;
    int			vertical_lines;
    int			fill_curve;
    int			no_of_points;
    int			curve_width;
    glow_eDrawType	curve_drawtype[TREND_MAX_CURVES];
    glow_eDrawType	curve_fill_drawtype[TREND_MAX_CURVES];
    GlowTraceData	trace;
    GrowPolyLine	*curve[TREND_MAX_CURVES];
    int			curve_cnt;
    double		scan_time;
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node);
    void draw();
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void nav_erase( GlowTransform *t, void *node);
    void trace_scan();
    int trace_init();
    void trace_close();
    void add_value( double value, int idx);
    void configure_curves();
    void align( double x, double y, glow_eAlignDirection direction);
    void set_scan_time( double time);
    void get_scan_time( double *time) { *time = scan_time;};
    void set_range( int curve, double min, double max);
    void set_fill_curve( int fill) { fill_curve = fill;};
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);
};


#endif


