#ifndef glow_exportjbean_h
#define glow_exportjbean_h

#include <iostream.h>
#include <fstream.h>

#include <X11/Xlib.h>
#include "glow.h"
#include "glow_ctx.h"

class GlowExportJBean {
  public:
    GlowExportJBean( GlowCtx *glow_ctx, GlowNodeClass *nodeclass = 0) : 
      ctx(glow_ctx), nc(nodeclass), page(1), func_cnt(0)
    {
      is_nodeclass = (nc != NULL);
    };
    void growctx( glow_eExportPass pass, ofstream& fp);
    void nodeclass( GlowNodeClass *nc, glow_eExportPass pass, 
	ofstream& fp, int page, int pages);
    void polyline( glow_sPoint *points, int point_cnt, int fill, int border, 
	glow_eDrawType fill_drawtype, glow_eDrawType border_drawtype,
	int fill_eq_border, int fill_eq_light, int fill_eq_shadow, int line_width, 
        int print_shadow, int shadow, int drawtype_incr, glow_sShadowInfo *sp, int sp_num,
		   glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void line( double x1, double y1, double x2, double y2,
	glow_eDrawType border_drawtype,
	int line_width, glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void rect( double x0, double y0, double width, double height,
	int fill, int border,
	glow_eDrawType fill_drawtype, glow_eDrawType border_drawtype,
	int line_width, double shadow_width, int shadow, 
	int drawtype_incr, glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void rectrounded( double x0, double y0, double width, double height,
	int fill, int border,
	glow_eDrawType fill_drawtype, glow_eDrawType border_drawtype,
	int line_width, double roundamount, double shadow_width, int shadow, 
	int drawtype_incr, glow_eExportPass pass, int *shape_cnt, 
	int node_cnt, ofstream& fp);
    void arc( double x0, double y0, double width, double height,
	double angel1, double angel2, int fill, int border,
	glow_eDrawType fill_drawtype, glow_eDrawType border_drawtype,
	int line_width, double shadow_width, int shadow, 
	int drawtype_incr, glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void text( int x0, int y0, char *text,
	glow_eDrawType drawtype, glow_eDrawType color_drawtype, int bold,
	int idx, glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void annot( int x0, int y0, int number,
	glow_eDrawType drawtype, glow_eDrawType text_drawtype, int bold,
	int idx, glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void annot_font( int number, glow_eDrawType drawtype, 
	glow_eDrawType background, int bold,
	int idx, glow_eExportPass pass, ofstream& fp);
    void node( double x1, double y1, double x2, double y2,
	char *class_name,
    	glow_eDrawType border_drawtype,  
    	glow_eDrawType	fill_drawtype,
    	glow_eDrawType	text_drawtype,
  	glow_eDrawTone	color_tone,
    	int		color_lightness,
    	int		color_intensity,
    	int		color_shift,
    	int		line_width,
	double		rotate,
	int 		shadow,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream& fp);
    void image( double x1, double y1, double x2, double y2,
	char *filename, int transparent,
    	glow_eDrawTone	color_tone,
    	int		color_lightness,
    	int		color_intensity,
    	int		color_shift,
	double		rotate,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream& fp);
    void bar( double x1, double y1, double x2, double y2,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType	fill_drawtype,
    	glow_eDrawType	bar_drawtype,
    	glow_eDrawType	bar_bordercolor,
    	int		fill,
	int		border,
	double		min_value,
	double		max_value,
	int		bar_border_width,
    	int		line_width,
	double		rotate,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void trend( double x1, double y1, double x2, double y2,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType	fill_drawtype,
    	glow_eDrawType	curve_drawtype1,
    	glow_eDrawType	curve_drawtype2,
    	glow_eDrawType	curve_fill_drawtype1,
    	glow_eDrawType	curve_fill_drawtype2,
    	int		fill,
	int		border,
	double		min_value1,
	double		max_value1,
	double		min_value2,
	double		max_value2,
	int		curve_width,
	int		no_of_points,
	double		scan_time,
	int		horizontal_lines,
	int		vertical_lines,
    	int		line_width,
	double		rotate,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void axis( double x1, double y1, double x2, double y2,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType text_drawtype,
	double		min_value,
	double		max_value,
	int	        lines,
	int		longquotient,
	int		valuequotient,
	int		line_length,
    	int		line_width,
	double		rotate,
	int             bold,
	int             text_idx,
	char            *format,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, ofstream& fp);
    void slider( double x1, double y1, double x2, double y2,
	char *class_name,
    	glow_eDrawType border_drawtype,
    	glow_eDrawType	fill_drawtype,
    	glow_eDrawType	text_drawtype,
    	glow_eDrawTone	color_tone,
    	int		color_lightness,
    	int		color_intensity,
    	int		color_shift,
    	int		line_width,
	double		rotate,
        int		shadow,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream& fp);
    GlowCtx *ctx;
    GlowNodeClass *nc;
    int is_nodeclass;
    int page;
    int func_cnt;
};

#endif
