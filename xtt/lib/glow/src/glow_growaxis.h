#ifndef glow_growaxis_h
#define glow_growaxis_h

#include "glow_growrect.h"

class GrowAxis : public GrowRect {
  public:
    GrowAxis( GlowCtx *glow_ctx, char *name, double x1 = 0, double y1 = 0, 
		double x2 = 0, double y2 = 0, 
		glow_eDrawType border_d_type = glow_eDrawType_Line, 
	        int line_w = 1, int t_size = 2, 
                glow_eDrawType t_drawtype = glow_eDrawType_TextHelvetica,
		int nodraw = 0);
    ~GrowAxis();
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
    glow_eObjectType type() { return glow_eObjectType_GrowAxis;};
    int                 text_size;
    glow_eDrawType      text_drawtype;
    double		max_value;
    double		min_value;
    int			lines;
    int                 linelength;
    int                 longquotient;
    int                 valuequotient;
    double              increment;
    char                format[20];
    void erase( GlowTransform *t, int hot, void *node);
    void nav_erase( GlowTransform *t, void *node);
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void draw();
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void configure();
    void align( double x, double y, glow_eAlignDirection direction);
    void set_range( double min, double max);
    void set_textsize( int size);
    void set_textbold( int bold);
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);
    void set_conf( double max_val, double min_val, int no_of_lines, 
                   int long_quot, int value_quot, double rot, char *format);
};


#endif















