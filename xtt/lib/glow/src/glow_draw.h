/* 
 * Proview   $Id: glow_draw.h,v 1.4 2005-09-01 14:57:53 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#ifndef glow_draw_h
#define glow_draw_h

#include <stdlib.h>

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#if defined OS_LINUX
#define IMLIB 1
#endif

#if defined IMLIB
# if defined OS_LYNX
#  define __NO_INCLUDE_WARN__ 1
# endif
# include <X11/extensions/shape.h>
# include <Imlib.h>
# if defined OS_LYNX
#  undef __NO_INCLUDE_WARN__
# endif
#else
typedef void *ImlibData;
typedef void *ImlibImage;
#endif

typedef struct {
        Widget  toplevel;
        Widget	nav_shell;
	Widget	nav_toplevel;
        XtAppContext  app_ctx;
	Display	*display;
	Window	window;
	Window	nav_window;
	Screen	*screen;
        Pixmap  buffer;
        int     buffer_width;
        int     buffer_height;
	GC	gc;
	GC	gc_erase;
	GC	gc_inverse;
	GC	gcs[glow_eDrawType__][DRAW_TYPE_SIZE];
	XFontStruct	*font_struct[glow_eDrawFont__][DRAW_FONT_SIZE];
	Font 	font[glow_eDrawFont__][DRAW_FONT_SIZE];
        int	cursors[glow_eDrawCursor__];
        int	ef;
        int	(*event_handler)(glow_eEvent event, int x, int y, int w, int h);
        int 	(*event_handler_nav)(glow_eEvent event, int x, int y);
	unsigned long background;
	unsigned long original_background;
	XtIntervalId	timer_id;
	int	clip_on;
  	int	clip_cnt;
	XRectangle clip_rectangle[2];
	int	click_sensitivity;
        ImlibData *imlib;
        Pixmap  background_pixmap;
        int     background_pixmap_width;
        int     background_pixmap_height;
} draw_sCtx, *draw_tCtx;

class DrawPs {
 public:
  DrawPs( char *filename) : fp(filename), x(0), y(0) 
    {}
  ~DrawPs() { fp.close();}
  ofstream fp;
  double x;
  double y;
};

int glow_draw_init( 
	Widget toplevel, 
	void **glow_ctx,
        int (*init_proc)(Widget w, GlowCtx *ctx, void *client_data),
	void  *client_data, 
	glow_eCtxType type);
void glow_draw_delete( GlowCtx *ctx);
int glow_draw_init_nav( Widget nav_widget, void *glow_ctx);
int draw_event_handler( GlowCtx *ctx, XEvent event);
void	glow_eDrawnable_event( GlowCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event));
void glow_draw_clear( GlowCtx *ctx);
void glow_draw_nav_clear( GlowCtx *ctx);
void glow_draw_copy_buffer( GlowCtx *ctx,
			    int ll_x, int ll_y, int ur_x, int ur_y);

void glow_draw_get_window_size( GlowCtx *ctx, int *width, int *height);
void glow_draw_get_nav_window_size( GlowCtx *ctx, int *width, int *height);
void glow_draw_set_nav_window_size( GlowCtx *ctx, int width, int height);

int glow_draw_rect( GlowCtx *ctx, int x, int y, int width, int height, 
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_rect_erase( GlowCtx *ctx, int x, int y, int width, int height,
	int idx);
int glow_draw_nav_rect( GlowCtx *ctx, int x, int y, int width, int height,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_nav_rect_erase( GlowCtx *ctx, int x, int y, int width, int height,
	int idx);
int glow_draw_arrow( GlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_arrow_erase( GlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx);
int glow_draw_nav_arrow( GlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_nav_arrow_erase( GlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx);
int glow_draw_arc( GlowCtx *ctx, int x, int y, int width, int height, 
	int angel1, int angel2,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_fill_arc( GlowCtx *ctx, int x, int y, int width, int height, 
	int angel1, int angel2, glow_eDrawType gc_type, int highlight);
int glow_draw_nav_fill_arc( GlowCtx *ctx, int x, int y, int width, int height, 
	int angel1, int angel2, glow_eDrawType gc_type);
int glow_draw_arc_erase( GlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx);
int glow_draw_nav_arc( GlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_nav_arc_erase( GlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx);
int glow_draw_line( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_line_dashed( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight, glow_eLineType line_type);
int glow_draw_line_erase( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx);
int glow_draw_nav_line( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_nav_line_erase( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx);
int glow_draw_polyline( GlowCtx *ctx, XPoint *points, int point_cnt,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_fill_polyline( GlowCtx *ctx, XPoint *points, int point_cnt,
	glow_eDrawType gc_type, int highlight);
int glow_draw_nav_fill_polyline( GlowCtx *ctx, XPoint *points, int point_cnt,
	glow_eDrawType gc_type);
int glow_draw_polyline_erase( GlowCtx *ctx, XPoint *points, int point_cnt,
	int idx);
int glow_draw_nav_polyline( GlowCtx *ctx, XPoint *points, int point_cnt,
	glow_eDrawType gc_type, int idx, int highlight);
int glow_draw_nav_polyline_erase( GlowCtx *ctx, XPoint *points, int point_cnt,
	int idx);
int glow_draw_text( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, glow_eDrawType color, int idx, int highlight, int line);
int glow_draw_text_cursor( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, glow_eDrawType color, int idx, int highlight, int pos);
int glow_draw_text_erase( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, int idx, int line);
int glow_draw_nav_text( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, int idx, int highlight, int line);
int glow_draw_nav_text_erase( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, int idx, int line);
int glow_draw_fill_rect( GlowCtx *ctx, int x, int y, int width, int height, 
	glow_eDrawType gc_type);
int glow_draw_nav_fill_rect( GlowCtx *ctx, int x, int y, int w, int h, 
	glow_eDrawType gc_type);
int glow_draw_pixmaps_create( GlowCtx *ctx, glow_sPixmapData *pixmap_data,
	void **pixmaps);
void glow_draw_pixmaps_delete( GlowCtx *ctx, void *pixmaps);
int glow_draw_pixmap( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int highlight, int line);
int glow_draw_pixmap_inverse( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line);
int glow_draw_pixmap_erase( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line);
int glow_draw_nav_pixmap( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int highlight, int line);
int glow_draw_nav_pixmap_erase( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line);
int glow_draw_image( GlowCtx *ctx, int x, int y, int width, int height,
		     Pixmap pixmap, Pixmap clip_mask);
int glow_draw_nav_image( GlowCtx *ctx, int x, int y, int width, int height,
			 Pixmap pixmap, Pixmap clip_mask);


void draw_set_timer( GlowCtx *ctx, int time_ms,
  	void (*callback_func)( GlowCtx *ctx), void **id);
void draw_remove_timer( void *id);
void draw_set_cursor( GlowCtx *ctx, glow_eDrawCursor cursor);
void draw_set_nav_cursor( GlowCtx *ctx, glow_eDrawCursor cursor);
int draw_get_text_extent( GlowCtx *ctx, char *text, int len,
	glow_eDrawType gc_type, int idx,
	int *width, int *height, int *descent);
void draw_copy_area( GlowCtx *ctx, int x, int y);
void draw_clear_area( GlowCtx *ctx, int ll_x, int ur_x, int ll_y, int ur_y);
int glow_draw_create_input( GlowCtx *ctx, int x, int y, char *text, int len,
	int idx, int width, int height, void *node, int number, void **data);
int glow_draw_close_input( GlowCtx *ctx, void *data);
int glow_draw_get_input( GlowCtx *ctx, void *data, char **text);
void glow_draw_move_input( GlowCtx *ctx, void *data, int x, int y,
	glow_ePosition pos_type);
void glow_draw_delete_secondary_ctx( GlowCtx *ctx);
int glow_draw_create_secondary_ctx( 
	GlowCtx *glow_ctx,
	void **secondary_glow_ctx,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void  *client_data, 
	glow_eCtxType type);
int glow_draw_change_ctx( 
	Widget w,
	GlowCtx *from_ctx,
	GlowCtx *to_ctx);
void glow_set_inputfocus( GlowCtx *ctx);
void glow_set_background( GlowCtx *ctx, glow_eDrawType drawtype, Pixmap pixmap,
   int pixmap_width, int pixmap_height);
void glow_reset_background( GlowCtx *ctx);
void glow_set_image_clip_mask( GlowCtx *ctx, Pixmap pixmap, int x, int y);
void glow_reset_image_clip_mask( GlowCtx *ctx);
int glow_draw_set_clip_rectangle( GlowCtx *ctx,
		int ll_x, int ll_y, int ur_x, int ur_y);
void glow_draw_reset_clip_rectangle( GlowCtx *ctx);
char *glow_color_to_name( glow_eDrawType drawtype);
char *glow_colortone_to_name( glow_eDrawType drawtype);
int glow_draw_point( GlowCtx *ctx, int x1, int y1, glow_eDrawType gc_type);
int glow_draw_points( GlowCtx *ctx, XPoint *points, int point_num, 
	glow_eDrawType gc_type);
void glow_draw_set_click_sensitivity( GlowCtx *ctx, int value);
void glow_draw_background( GlowCtx *ctx, int x, int y, int w, int h);
int glow_draw_create_buffer( GlowCtx *ctx);
void glow_draw_buffer_background( GlowCtx *ctx);
int glow_print( GlowCtx *ctx, char *filename, double x0, double x1, int end);
#endif














