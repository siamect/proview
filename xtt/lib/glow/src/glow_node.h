#ifndef glow_node_h
#define glow_node_h

#include <iostream.h>
#include <fstream.h>
#include "glow_ctx.h"
#include "glow_point.h"
#include "glow_array_elem.h"
#include "glow_nodeclass.h"
#include "glow_tracedata.h"

#define MAX_CONPOINTS 32

class GlowNode : public GlowArrayElem {
  public:
    GlowNode( GlowCtx *glow_ctx, char *name, GlowNodeClass *node_class,
	double x1, double y1, int nodraw = 0, int rel_annot_pos = 0);
    GlowNode() {};
    ~GlowNode();
    void copy_from( const GlowNode& n);
    friend ostream& operator<< ( ostream& o, const GlowNode n);
    void zoom() { nc->zoom(); pos.zoom();};
    void nav_zoom() { nc->nav_zoom(); pos.nav_zoom();};
    void print_zoom() { nc->print_zoom(); pos.print_zoom();};
    void traverse( int x, int y) { pos.traverse(x,y);};
    void get_borders(
	double *x1_right, double *x1_left, double *y1_high, double *y1_low)
	{ if ( x_left < *x1_left) *x1_left = x_left;
	  if ( x_right > *x1_right) *x1_right = x_right;
	  if ( y_high > *y1_high) *y1_high = y_high;
	  if ( y_low < *y1_low) *y1_low = y_low;};
//	{ nc->get_borders(pos.x, pos.y, x1_right, x1_left, y1_high, y1_low,
//	annotv);};
    void get_node_borders( )
	{ nc->get_borders(pos.x, pos.y, &x_right, &x_left, &y_high, &y_low,
	(void *)this);};
    void get_node_obstacle_borders( )
	{ nc->get_obstacle_borders(pos.x, pos.y, &obst_x_right, &obst_x_left, 
		&obst_y_high, &obst_y_low, (void *)this);};
    int	event_handler( glow_eEvent event, int x, int y);
    void print( double ll_x, double ll_y, double ur_x, double ur_y);
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void draw( int ll_x, int ll_y, int ur_x, int ur_y);
    void draw_inverse();
    void nav_draw(int ll_x, int ll_y, int ur_x, int ur_y);
    void erase();
    void nav_erase() /* { nc->nav_erase( &pos, (void *)this);} */;
    void move( int delta_x, int delta_y, int grid);
    void move_noerase( int delta_x, int delta_y, int grid);
    void store_position() { stored_pos = pos;};
    void restore_position() { pos = stored_pos;};
    int get_conpoint( int num, double *x, double *y, glow_eDirection *dir);
    void	redraw_node_cons( void *node) {};
    int		delete_node_cons( void *node) {return 0;};
    void set_highlight( int on);
    int get_highlight() {return highlight;};
    void set_inverse( int on);
    int get_inverse() {return inverse;};
    void set_hot( int on);
    void select_region_insert( double ll_x, double ll_y, double ur_x, 
		double ur_y, glow_eSelectPolicy select_policy);
    glow_eObjectType type() { return glow_eObjectType_Node;};
    void	set_annotation( int num, char *text, int size, int nodraw);
    void	get_annotation( int num, char *text, int size);
    void 	measure_annotation( int num, char *text, double *width, 
			double *height);
    void	measure( double *ll_x, double *ll_y, double *ur_x, double *ur_y)
	{ *ll_x = x_left; *ll_y = y_low; *ur_x = x_right; *ur_y = y_high;};
    void	set_annot_pixmap( int num, glow_sAnnotPixmap *pixmap, 
			int nodraw);
    void	remove_annot_pixmap( int num);
    double	x_right;
    double	x_left;
    double	y_high;
    double	y_low;
    double	obst_x_right;
    double	obst_x_left;
    double	obst_y_high;
    double	obst_y_low;
    int		hot;
    GlowCtx 		*ctx;
    GlowNodeClass 	*nc;
    GlowNodeClass 	*nc_root;
    GlowPoint		pos;
    GlowPoint		stored_pos;
    char		n_name[32];
    int			highlight;
    int			inverse;
    char		*annotv[10];
    int			annotsize[10];
    glow_sAnnotPixmap	*annotpixmapv[10];
    int			refcon_cnt[MAX_CONPOINTS];
    GlowTraceData	trace;
    GlowNode		*link;
    void		link_insert( void **start)
	{link = *(GlowNode **)start; *start = (void *)this;};
    int			in_area( double ll_x, double ll_y, double ur_x, double ur_y)
      	{return ((obst_x_left - ctx->draw_delta) < ur_x && 
	         (obst_x_right + ctx->draw_delta) > ll_x &&
	         (obst_y_low - ctx->draw_delta) < ur_y && 
	         (obst_y_high + ctx->draw_delta) > ll_y);};
    int			in_area_exact( double ll_x, double ll_y, double ur_x, double ur_y)
      	{return (obst_x_left < ur_x && obst_x_right > ll_x && 
		 obst_y_low < ur_y && obst_y_high > ll_y);};
    int			in_vert_line( double x, double l_y, double u_y)
      	{return ((obst_x_left - ctx->draw_delta) < x && 
	         (obst_x_right + ctx->draw_delta) > x && 
                 (obst_y_low - ctx->draw_delta) < u_y && 
	         (obst_y_high + ctx->draw_delta) > l_y);};
    int			in_horiz_line( double y, double l_x, double u_x)
      	{return ((obst_x_left - ctx->draw_delta) < u_x && 
	         (obst_x_right + ctx->draw_delta) > l_x && 
	         (obst_y_low - ctx->draw_delta) < y && 
	         (obst_y_high + ctx->draw_delta) > y);};
    void conpoint_refcon_reconfig( int conpoint);
    void conpoint_refcon_redraw( void *node, int conpoint) {};
    void conpoint_refcon_erase( void *node, int conpoint) {};
    void remove_notify();
    void *user_data;
    void set_user_data( void *data) { user_data = data;};
    void get_user_data( void **data) { *data = user_data;};
    void set_trace_attr( GlowTraceData *attr);
    void get_trace_attr( GlowTraceData **attr);
    void set_trace_data( void *data) { trace.p = data;};
    void trace_scan();
    int trace_init();
    void trace_close();
    void *get_ctx() { return this->ctx;};
    void configure( void *previous);
    void get_node_position( double *x, double *y) {*x = pos.x; *y = pos.y;};
    glow_eNodeGroup get_group() {return nc->group;};
    void get_object_name( char *name) { strcpy( name, n_name);};
    void set_object_name( char *name) { strcpy( n_name, name);};
    void move_widgets( int x, int y);

//  brow stuff
    void set_level( int lev) { level = lev;};
    int get_level() { return level;};
    int is_open() { return node_open;};
    void set_open( int mask) { node_open |= mask;};
    void reset_open( int mask) { node_open &= ~mask;};
    void open_annotation_input( int num);
    int annotation_input_is_open( int num) { return annotv_inputmode[num];};
    void close_annotation_input( int num);
    int get_annotation_input( int num, char **text);
    void set_radiobutton( int num, int value, int nodraw);
    void get_radiobutton( int num, int *value);
    int	level;
    int node_open;
    int relative_annot_pos;
    double relative_annot_x;
    double rel_annot_x[10];
    double rel_annotpixmap_x[10];
    int annotv_inputmode[10];
    void *annotv_input[10];
    int rbuttonv[10];
};

#endif
