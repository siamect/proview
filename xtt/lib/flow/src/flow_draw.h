#ifndef flow_draw_h
#define flow_draw_h

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int flow_draw_init( 
	Widget toplevel, 
	void **flow_ctx,
        int (*init_proc)(Widget w, FlowCtx *ctx, void *client_data),
	void  *client_data, 
	flow_eCtxType type);
void flow_draw_delete( FlowCtx *ctx);
int flow_draw_init_nav( Widget nav_widget, void *flow_ctx);
int draw_event_handler( FlowCtx *ctx, XEvent event);
void	draw_enable_event( FlowCtx *ctx, flow_eEvent event, 
		flow_eEventType event_type, 
		int (*event_cb)(FlowCtx *ctx, flow_tEvent event));
void flow_draw_clear( FlowCtx *ctx);
void flow_draw_nav_clear( FlowCtx *ctx);

void flow_draw_get_window_size( FlowCtx *ctx, int *width, int *height);
void flow_draw_get_nav_window_size( FlowCtx *ctx, int *width, int *height);
void flow_draw_set_nav_window_size( FlowCtx *ctx, int width, int height);

int flow_draw_rect( FlowCtx *ctx, int x, int y, int width, int height, 
	flow_eDrawType gc_type, int idx, int highlight);
int flow_draw_rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int idx);
int flow_draw_nav_rect( FlowCtx *ctx, int x, int y, int width, int height,
	flow_eDrawType gc_type, int idx, int highlight);
int flow_draw_nav_rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int idx);
int flow_draw_arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	flow_eDrawType gc_type, int idx, int highlight);
int flow_draw_arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx);
int flow_draw_nav_arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	flow_eDrawType gc_type, int idx, int highlight);
int flow_draw_nav_arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx);
int flow_draw_arc( FlowCtx *ctx, int x, int y, int width, int height, 
	int angel1, int angel2,
	flow_eDrawType gc_type, int idx, int highlight);
int flow_draw_arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx);
int flow_draw_nav_arc( FlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	flow_eDrawType gc_type, int idx, int highlight);
int flow_draw_nav_arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx);
int flow_draw_line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	flow_eDrawType gc_type, int idx, int highlight);
int flow_draw_line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx);
int flow_draw_nav_line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	flow_eDrawType gc_type, int idx, int highlight);
int flow_draw_nav_line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx);
int flow_draw_text( FlowCtx *ctx, int x, int y, char *text, int len,
	flow_eDrawType gc_type, int idx, int highlight, int line);
int flow_draw_text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
	flow_eDrawType gc_type, int idx, int line);
int flow_draw_nav_text( FlowCtx *ctx, int x, int y, char *text, int len,
	flow_eDrawType gc_type, int idx, int highlight, int line);
int flow_draw_nav_text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
	flow_eDrawType gc_type, int idx, int line);
int flow_draw_fill_rect( FlowCtx *ctx, int x, int y, int width, int height, 
	flow_eDrawType gc_type);
int flow_draw_pixmaps_create( FlowCtx *ctx, flow_sPixmapData *pixmap_data,
	void **pixmaps);
void flow_draw_pixmaps_delete( FlowCtx *ctx, void *pixmaps);
int flow_draw_pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line);
int flow_draw_pixmap_inverse( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line);
int flow_draw_pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line);
int flow_draw_nav_pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line);
int flow_draw_nav_pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line);

void draw_set_timer( FlowCtx *ctx, int time_ms,
  	void (*callback_func)( FlowCtx *ctx), void **id);
void draw_cancel_timer( void *id);
void draw_set_cursor( FlowCtx *ctx, draw_eCursor cursor);
void draw_set_nav_cursor( FlowCtx *ctx, draw_eCursor cursor);
int draw_get_text_extent( FlowCtx *ctx, char *text, int len,
	flow_eDrawType gc_type, int idx,
	int *width, int *height);
void draw_copy_area( FlowCtx *ctx, int x, int y);
void draw_clear_area( FlowCtx *ctx, int ll_x, int ur_x, int ll_y, int ur_y);
int flow_draw_create_input( FlowCtx *ctx, int x, int y, char *text, int len,
	int idx, int width, int height, void *node, int number, void **data);
int flow_draw_close_input( FlowCtx *ctx, void *data);
int flow_draw_get_input( FlowCtx *ctx, void *data, char **text);
void flow_draw_move_input( FlowCtx *ctx, void *data, int x, int y,
	flow_ePosition pos_type);
void flow_draw_delete_secondary_ctx( FlowCtx *ctx);
int flow_draw_create_secondary_ctx( 
	FlowCtx *flow_ctx,
	void **secondary_flow_ctx,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void  *client_data, 
	flow_eCtxType type);
int flow_draw_change_ctx( 
	Widget w,
	FlowCtx *from_ctx,
	FlowCtx *to_ctx);
void flow_set_inputfocus( FlowCtx *ctx);
void flow_set_widget_inputfocus( Widget w);
void flow_draw_set_click_sensitivity( FlowCtx *ctx, int value);

#endif
