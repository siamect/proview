/* 
 * Proview   $Id: flow_draw.h,v 1.8 2008-11-28 17:13:44 claes Exp $
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

#ifndef flow_draw_h
#define flow_draw_h

#include <stdlib.h>

class FlowCtx;

class FlowDraw {
 public:
  FlowCtx *basectx;
  int     click_sensitivity;
  flow_tImData imlib;

  FlowDraw() : basectx(0), click_sensitivity(0), imlib(0) {}
  virtual ~FlowDraw() {}
  virtual void	enable_event( FlowCtx *ctx, flow_eEvent event, 
		flow_eEventType event_type, 
		int (*event_cb)(FlowCtx *ctx, flow_tEvent event)) {}
  virtual void clear( FlowCtx *ctx) {}
  virtual void nav_clear( FlowCtx *ctx) {}

  virtual void get_window_size( FlowCtx *ctx, int *width, int *height) {}
  virtual void get_nav_window_size( FlowCtx *ctx, int *width, int *height) {}
  virtual void set_nav_window_size( FlowCtx *ctx, int width, int height) {}

  virtual int rect( FlowCtx *ctx, int x, int y, int width, int height, 
	    flow_eDrawType gc_type, int idx, int highlight) {return 1;}
  virtual int rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
		  int idx) {return 1;}
  virtual int nav_rect( FlowCtx *ctx, int x, int y, int width, int height,
		flow_eDrawType gc_type, int idx, int highlight) {return 1;}
  virtual int nav_rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
		      int idx) {return 1;}
  virtual int arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	     int x3, int y3,
	     flow_eDrawType gc_type, int idx, int highlight) {return 1;}
  virtual int arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
		   int x3, int y3,
		   int idx) {return 1;}
  virtual int nav_arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
		 int x3, int y3,
		 flow_eDrawType gc_type, int idx, int highlight) {return 1;}
  virtual int nav_arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
		       int x3, int y3,
		       int idx) {return 1;}
  virtual int arc( FlowCtx *ctx, int x, int y, int width, int height, 
	   int angle1, int angle2,
	   flow_eDrawType gc_type, int idx, int highlight) {return 1;}
  virtual int arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
		 int angle1, int angle2,
		 int idx) {return 1;}
  virtual int nav_arc( FlowCtx *ctx, int x, int y, int width, int height,
	       int angle1, int angle2,
	       flow_eDrawType gc_type, int idx, int highlight) {return 1;}
  virtual int nav_arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
		     int angle1, int angle2,
		     int idx) {return 1;}
  virtual int line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	    flow_eDrawType gc_type, int idx, int highlight) {return 1;}
  virtual int line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
		  int idx) {return 1;}
  virtual int nav_line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
		flow_eDrawType gc_type, int idx, int highlight) {return 1;}
  virtual int nav_line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
		      int idx) {return 1;}
  virtual int text( FlowCtx *ctx, int x, int y, char *text, int len,
		    flow_eDrawType gc_type, int idx, int highlight, int line, double size) 
    {return 1;}
  virtual int text_inverse( FlowCtx *ctx, int x, int y, char *text, int len,
			    flow_eDrawType gc_type, int idx, int line, double size)
    {return 1;}
  virtual int text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
			  flow_eDrawType gc_type, int idx, int line, double size) {return 1;}
  virtual int nav_text( FlowCtx *ctx, int x, int y, char *text, int len,
			flow_eDrawType gc_type, int idx, int highlight, int line, double sizes) 
    {return 1;}
  virtual int nav_text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
			      flow_eDrawType gc_type, int idx, int line, double size) {return 1;}
  virtual int fill_rect( FlowCtx *ctx, int x, int y, int width, int height, 
		 flow_eDrawType gc_type) {return 1;}
  virtual int image( FlowCtx *ctx, int x, int y, int width, int height,
	     flow_tImImage image, flow_tPixmap pixmap, flow_tPixmap clip_mask) {return 1;}
  virtual int pixmaps_create( FlowCtx *ctx, flow_sPixmapData *pixmap_data,
		      void **pixmaps) {return 1;}
  virtual void pixmaps_delete( FlowCtx *ctx, void *pixmaps) {}
  virtual int pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	      void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line) {return 1;}
  virtual int pixmap_inverse( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
		      void *pixmaps, flow_eDrawType gc_type, int idx, int line) {return 1;}
  virtual int pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
		    void *pixmaps, flow_eDrawType gc_type, int idx, int line) {return 1;}
  virtual int nav_pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
		  void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line) {return 1;}
  virtual int nav_pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
			void *pixmaps, flow_eDrawType gc_type, int idx, int line) {return 1;}
  
  virtual void set_timer( FlowCtx *ctx, int time_ms,
		       void (*callback_func)( FlowCtx *ctx), void **id) {}
  virtual void cancel_timer( FlowCtx *ctx, void *id) {}
  virtual void set_cursor( FlowCtx *ctx, draw_eCursor cursor) {}
  virtual void set_nav_cursor( FlowCtx *ctx, draw_eCursor cursor) {}
  virtual int get_text_extent( FlowCtx *ctx, const char *text, int len,
			       flow_eDrawType gc_type, int idx,
			       int *width, int *height, double size) {return 1;}
  virtual void copy_area( FlowCtx *ctx, int x, int y) {}
  virtual void clear_area( FlowCtx *ctx, int ll_x, int ur_x, int ll_y, int ur_y) {}
  virtual int create_input( FlowCtx *ctx, int x, int y, char *text, int len,
		    int idx, int width, int height, void *node, int number, void **data) {return 1;}
  virtual int close_input( FlowCtx *ctx, void *data) {return 1;}
  virtual int get_input( FlowCtx *ctx, void *data, char **text) {return 1;}
  virtual void move_input( FlowCtx *ctx, void *data, int x, int y,
		   flow_ePosition pos_type) {}
  virtual void delete_secondary_ctx( FlowCtx *ctx) {}
  virtual int create_secondary_ctx( 
			   FlowCtx *flow_ctx,
			   void **secondary_flow_ctx,
			   int (*init_proc)(FlowCtx *, void *),
			   void  *client_data, 
			   flow_eCtxType type) {return 1;}
  virtual int change_ctx( 
		 FlowCtx *from_ctx,
		 FlowCtx *to_ctx) {return 1;}
  virtual void set_inputfocus( FlowCtx *ctx) {}
  virtual void set_click_sensitivity( FlowCtx *ctx, int value) {}
  virtual void set_image_clip_mask( FlowCtx *ctx, flow_tPixmap pixmap, int x, int y) {}
  virtual void reset_image_clip_mask( FlowCtx *ctx) {}
  virtual void set_white_background( FlowCtx *ctx) {}
  virtual int image_get_width( flow_tImImage image) {return 0;}
  virtual int image_get_height( flow_tImImage image) {return 0;}
  virtual void image_scale( float scale, flow_tImImage orig_im, flow_tImImage *im, 
			    flow_tPixmap *im_pixmap, flow_tPixmap *im_mask) {}
  virtual int image_load( const char *imagefile, float scale, float nav_scale,
			  flow_tImImage *orig_im, flow_tImImage *im, 
			  flow_tPixmap *im_pixmap, flow_tPixmap *im_mask,
			  flow_tPixmap *im_nav_pixmap, flow_tPixmap *im_nav_mask) {return 0;}
};


#endif
