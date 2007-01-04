/* 
 * Proview   $Id: flow_draw_xlib.h,v 1.1 2007-01-04 07:57:00 claes Exp $
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

#ifndef flow_draw_xlib_h
#define flow_draw_xlib_h

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

#include "flow_draw.h"

class FlowCtx;

class FlowDrawXLib : public FlowDraw {
 public:
  Widget  	toplevel;
  Widget	nav_shell;
  Widget	nav_toplevel;
  XtAppContext  app_ctx;
  Display	*display;
  Window	window;
  Window	nav_window;
  Screen	*screen;
  GC	gc;
  GC	gc_erase;
  GC	gc_inverse;
  GC	gc_yellow;
  GC	gc_green;
  GC	gcs[flow_eDrawType__][DRAW_TYPE_SIZE];
  XFontStruct	*font_struct[draw_eFont__][DRAW_FONT_SIZE];
  Font 	font[draw_eFont__][DRAW_FONT_SIZE];
  int	cursors[draw_eCursor__];
  unsigned long background;
  XtIntervalId	timer_id;

  FlowDrawXLib( Widget toplevel, 
		void **flow_ctx,
		int (*init_proc)(Widget w, FlowCtx *ctx, void *client_data),
		void  *client_data, 
		flow_eCtxType type);
  ~FlowDrawXLib();
  int init_nav( Widget nav_widget, void *flow_ctx);
  int event_handler( FlowCtx *ctx, XEvent event);
  void	enable_event( FlowCtx *ctx, flow_eEvent event, 
		flow_eEventType event_type, 
		int (*event_cb)(FlowCtx *ctx, flow_tEvent event));
  void clear( FlowCtx *ctx);
  void nav_clear( FlowCtx *ctx);

  void get_window_size( FlowCtx *ctx, int *width, int *height);
  void get_nav_window_size( FlowCtx *ctx, int *width, int *height);
  void set_nav_window_size( FlowCtx *ctx, int width, int height);

  int rect( FlowCtx *ctx, int x, int y, int width, int height, 
	    flow_eDrawType gc_type, int idx, int highlight);
  int rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
		  int idx);
  int nav_rect( FlowCtx *ctx, int x, int y, int width, int height,
		flow_eDrawType gc_type, int idx, int highlight);
  int nav_rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
		      int idx);
  int arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	     int x3, int y3,
	     flow_eDrawType gc_type, int idx, int highlight);
  int arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
		   int x3, int y3,
		   int idx);
  int nav_arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
		 int x3, int y3,
		 flow_eDrawType gc_type, int idx, int highlight);
  int nav_arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
		       int x3, int y3,
		       int idx);
  int arc( FlowCtx *ctx, int x, int y, int width, int height, 
	   int angel1, int angel2,
	   flow_eDrawType gc_type, int idx, int highlight);
  int arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
		 int angel1, int angel2,
		 int idx);
  int nav_arc( FlowCtx *ctx, int x, int y, int width, int height,
	       int angel1, int angel2,
	       flow_eDrawType gc_type, int idx, int highlight);
  int nav_arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
		     int angel1, int angel2,
		     int idx);
  int line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	    flow_eDrawType gc_type, int idx, int highlight);
  int line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
		  int idx);
  int nav_line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
		flow_eDrawType gc_type, int idx, int highlight);
  int nav_line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
		      int idx);
  int text( FlowCtx *ctx, int x, int y, char *text, int len,
	    flow_eDrawType gc_type, int idx, int highlight, int line);
  int text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
		  flow_eDrawType gc_type, int idx, int line);
  int nav_text( FlowCtx *ctx, int x, int y, char *text, int len,
		flow_eDrawType gc_type, int idx, int highlight, int line);
  int nav_text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
		      flow_eDrawType gc_type, int idx, int line);
  int fill_rect( FlowCtx *ctx, int x, int y, int width, int height, 
		 flow_eDrawType gc_type);
  int image( FlowCtx *ctx, int x, int y, int width, int height,
	     flow_tImImage image, flow_tPixmap pixmap, flow_tPixmap clip_mask);
  int pixmaps_create( FlowCtx *ctx, flow_sPixmapData *pixmap_data,
		      void **pixmaps);
  void pixmaps_delete( FlowCtx *ctx, void *pixmaps);
  int pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	      void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line);
  int pixmap_inverse( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
		      void *pixmaps, flow_eDrawType gc_type, int idx, int line);
  int pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
		    void *pixmaps, flow_eDrawType gc_type, int idx, int line);
  int nav_pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
		  void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line);
  int nav_pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
			void *pixmaps, flow_eDrawType gc_type, int idx, int line);
  
  void set_timer( FlowCtx *ctx, int time_ms,
		       void (*callback_func)( FlowCtx *ctx), void **id);
  void cancel_timer( FlowCtx *ctx, void *id);
  void set_cursor( FlowCtx *ctx, draw_eCursor cursor);
  void set_nav_cursor( FlowCtx *ctx, draw_eCursor cursor);
  int get_text_extent( FlowCtx *ctx, char *text, int len,
			    flow_eDrawType gc_type, int idx,
			    int *width, int *height);
  void copy_area( FlowCtx *ctx, int x, int y);
  void clear_area( FlowCtx *ctx, int ll_x, int ur_x, int ll_y, int ur_y);
  int create_input( FlowCtx *ctx, int x, int y, char *text, int len,
		    int idx, int width, int height, void *node, int number, void **data);
  int close_input( FlowCtx *ctx, void *data);
  int get_input( FlowCtx *ctx, void *data, char **text);
  void move_input( FlowCtx *ctx, void *data, int x, int y,
		   flow_ePosition pos_type);
  void delete_secondary_ctx( FlowCtx *ctx);
  int create_secondary_ctx( 
			   FlowCtx *flow_ctx,
			   void **secondary_flow_ctx,
			   int (*init_proc)(FlowCtx *, void *),
			   void  *client_data, 
			   flow_eCtxType type);
  int change_ctx( 
		 FlowCtx *from_ctx,
		 FlowCtx *to_ctx);
  void set_inputfocus( FlowCtx *ctx);
  void set_click_sensitivity( FlowCtx *ctx, int value);
  void set_image_clip_mask( FlowCtx *ctx, flow_tPixmap pixmap, int x, int y);
  void reset_image_clip_mask( FlowCtx *ctx);
  void set_white_background( FlowCtx *ctx);
  unsigned char *imlib_image_rgb_data( flow_tImImage image);
  int image_get_width( flow_tImImage image);
  int image_get_height( flow_tImImage image);
  void image_scale( float scale, flow_tImImage *im, flow_tPixmap *im_pixmap,
		    flow_tPixmap *im_mask);
  int image_load( char *imagefile, float scale, float nav_scale,
		  flow_tImImage *orig_im, flow_tImImage *im, 
		  flow_tPixmap *im_pixmap, flow_tPixmap *im_mask,
		  flow_tPixmap *im_nav_pixmap, flow_tPixmap *im_nav_mask);
    
};

#endif
