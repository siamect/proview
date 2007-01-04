/* 
 * Proview   $Id: glow_draw_gtk.h,v 1.1 2007-01-04 08:07:43 claes Exp $
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

#ifndef glow_draw_gtk_h
#define glow_draw_gtk_h

#include <stdlib.h>

#include <libgnome.h>
#include <libgnomeui/libgnomeui.h>
#include <gtk/gtkprivate.h>

#include "glow_draw.h"

#define DRAW_CLIP_SIZE 10

class DrawWindGtk : public DrawWind {
 public:
  DrawWindGtk() : toplevel(0), shell(0), window(0), colormap(0),
    buffer(0), buffer_width(0), buffer_height(0),
    clip_on(0), clip_cnt(0), background_pixmap(0),
    background_pixmap_width(0), background_pixmap_height(0)
    { memset( clip_rectangle, 0, sizeof(clip_rectangle)); }
  GtkWidget 	*toplevel;
  GtkWidget 	*shell;
  GdkWindow 	*window;
  GdkColormap   *colormap;
  GdkPixmap  	*buffer;
  int     	buffer_width;
  int     	buffer_height;
  int		clip_on;
  int		clip_cnt;
  GdkRectangle 	clip_rectangle[DRAW_CLIP_SIZE];
  GdkPixmap  *background_pixmap;
  int     background_pixmap_width;
  int     background_pixmap_height;
};

class GlowDrawGtk : public GlowDraw {

 public:
  GlowDrawGtk(
	GtkWidget *toplevel, 
	void **glow_ctx,
        int (*init_proc)(GtkWidget *w, GlowCtx *ctx, void *client_data),
	void  *client_data, 
	glow_eCtxType type);
  ~GlowDrawGtk();

  DrawWindGtk m_wind;
  DrawWindGtk nav_wind;
  GdkDisplay	*display;
  GdkScreen	*screen;
  GdkGC	*gc;
  GdkGC	*gc_erase;
  GdkGC	*gc_inverse;
  GdkGC	*gcs[glow_eDrawType__][DRAW_TYPE_SIZE];
  // XFontStruct	*font_struct[glow_eDrawFont__][DRAW_FONT_SIZE];
  GdkFont 	*font[glow_eDrawFont__][DRAW_FONT_SIZE];
  GdkCursor	*cursors[glow_eDrawCursor__];
  int	ef;
  GdkColormap *colormap;
  GdkColor background;
  GdkColor original_background;
  guint	timer_id;
  int	click_sensitivity;
  
  int event_handler( GdkEvent event);
  virtual void enable_event( glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event));
  virtual void clear( GlowWind *w);
  virtual void copy_buffer( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y);

  virtual void get_window_size( GlowWind *w, int *width, int *height);
  virtual void set_window_size( GlowWind *w, int width, int height);

  virtual int rect( GlowWind *w, int x, int y, int width, int height, 
	    glow_eDrawType gc_type, int idx, int highlight);
  virtual int rect_erase( GlowWind *w, int x, int y, int width, int height,
		  int idx);
  virtual int arrow( GlowWind *w, int x1, int y1, int x2, int y2, 
	     int x3, int y3,
	     glow_eDrawType gc_type, int idx, int highlight);
  virtual int arrow_erase( GlowWind *w, int x1, int y1, int x2, int y2, 
		   int x3, int y3,
		   int idx);
  virtual int arc( GlowWind *w, int x, int y, int width, int height, 
	   int angel1, int angel2,
	   glow_eDrawType gc_type, int idx, int highlight);
  virtual int fill_arc( GlowWind *w, int x, int y, int width, int height, 
		int angel1, int angel2, glow_eDrawType gc_type, int highlight);
  virtual int arc_erase( GlowWind *w, int x, int y, int width, int height,
		 int angel1, int angel2,
		 int idx);
  virtual int line( GlowWind *w, int x1, int y1, int x2, int y2,
	    glow_eDrawType gc_type, int idx, int highlight);
  virtual int line_dashed( GlowWind *w, int x1, int y1, int x2, int y2,
		   glow_eDrawType gc_type, int idx, int highlight, glow_eLineType line_type);
  virtual int line_erase( GlowWind *w, int x1, int y1, int x2, int y2,
		  int idx);
  virtual int polyline( GlowWind *w, glow_sPointX *points, int point_cnt,
		glow_eDrawType gc_type, int idx, int highlight);
  virtual int fill_polyline( GlowWind *w, glow_sPointX *points, int point_cnt,
		     glow_eDrawType gc_type, int highlight);
  virtual int polyline_erase( GlowWind *w, glow_sPointX *points, int point_cnt,
		      int idx);
  virtual int text( GlowWind *w, int x, int y, char *text, int len,
	    glow_eDrawType gc_type, glow_eDrawType color, int idx, int highlight, int line);
  virtual int text_cursor( GlowWind *w, int x, int y, char *text, int len,
		   glow_eDrawType gc_type, glow_eDrawType color, int idx, int highlight, int pos);
  virtual int text_erase( GlowWind *w, int x, int y, char *text, int len,
		  glow_eDrawType gc_type, int idx, int line);
  virtual int fill_rect( GlowWind *w, int x, int y, int width, int height, 
		 glow_eDrawType gc_type);
  virtual int pixmaps_create( GlowWind *w, glow_sPixmapData *pixmap_data,
		      void **pixmaps);
  virtual void pixmaps_delete( GlowWind *w, void *pixmaps);
  virtual int pixmap( GlowWind *w, int x, int y, glow_sPixmapData *pixmap_data,
	      void *pixmaps, glow_eDrawType gc_type, int idx, int highlight, int line);
  virtual int pixmap_inverse( GlowWind *w, int x, int y, glow_sPixmapData *pixmap_data,
		      void *pixmaps, glow_eDrawType gc_type, int idx, int line);
  virtual int pixmap_erase( GlowWind *w, int x, int y, glow_sPixmapData *pixmap_data,
		    void *pixmaps, glow_eDrawType gc_type, int idx, int line);
  virtual int image( GlowWind *w, int x, int y, int width, int height,
	     glow_tImImage image, glow_tPixmap pixmap, glow_tPixmap clip_mask);
  
  virtual void set_cursor( GlowWind *w, glow_eDrawCursor cursor);
  virtual int get_text_extent( char *text, int len,
		       glow_eDrawType gc_type, int idx,
		       int *width, int *height, int *descent);
  virtual void copy_area( GlowWind *w, int x, int y);
  virtual void clear_area( GlowWind *w, int ll_x, int ur_x, int ll_y, int ur_y);
  virtual void set_inputfocus( GlowWind *w);
  virtual void set_background(  GlowWind *w, glow_eDrawType drawtype, glow_tPixmap pixmap,
		       int pixmap_width, int pixmap_height);
  virtual void reset_background( GlowWind *w);
  virtual void set_image_clip_mask( glow_tPixmap pixmap, int x, int y);
  virtual void reset_image_clip_mask();
  virtual int set_clip_rectangle( GlowWind *w, int ll_x, int ll_y, int ur_x, int ur_y);
  virtual void reset_clip_rectangle( GlowWind *w);
  virtual int clip_level( GlowWind *w);
  virtual int draw_point( GlowWind *w, int x1, int y1, glow_eDrawType gc_type);
  virtual int draw_points( GlowWind *w, glow_sPointX *points, int point_num, 
	      glow_eDrawType gc_type);
  virtual void set_click_sensitivity( GlowWind *w, int value);
  virtual void draw_background( GlowWind *w, int x, int y, int w, int h);
  virtual int create_buffer( GlowWind *w);
  virtual void buffer_background( DrawWind *w);
  virtual int print( char *filename, double x0, double x1, int end);
  void set_clip( DrawWind *w, GdkGC *gc);
  void reset_clip( DrawWind *w, GdkGC *gc);
  
  virtual void set_timer( GlowCtx *gctx, int time_ms,
		  void (*callback_func)( GlowCtx *ctx), void **id);
  virtual void remove_timer( void *id);
  int init_nav( GtkWidget *nav_widget);
  GdkPoint *points_to_gdk_points( glow_sPointX *points, int point_cnt);
  int get_font_idx( int gc_type);

  virtual void imlib_destroy_image( glow_tImData imlib, glow_tImImage image);
  virtual void imlib_kill_image( glow_tImData imlib, glow_tImImage image);
  virtual void imlib_free_pixmap( glow_tImData imlib, glow_tPixmap pixmap);
  virtual glow_tImImage imlib_load_image( glow_tImData imlib, char *filename);
  virtual glow_tImImage imlib_clone_image( glow_tImData imlib, glow_tImImage image);
  virtual int imlib_render( glow_tImData imlib, glow_tImImage image, int width, int height);
  virtual glow_tPixmap imlib_move_image( glow_tImData imlib, glow_tImImage image);
  virtual glow_tPixmap imlib_move_mask( glow_tImData imlib, glow_tImImage image);
  virtual void imlib_rotate_image( glow_tImData imlib, glow_tImImage image, int d);
  virtual void imlib_flip_image_vertical( glow_tImData imlib, glow_tImImage image);
  virtual void imlib_flip_image_horizontal( glow_tImData imlib, glow_tImImage image);
  virtual void imlib_set_image_red_curve( glow_tImData imlib, glow_tImImage image, unsigned char *mod);
  virtual void imlib_set_image_green_curve( glow_tImData imlib, glow_tImImage image, unsigned char *mod);
  virtual void imlib_set_image_blue_curve( glow_tImData imlib, glow_tImImage image, unsigned char *mod);
  virtual void imlib_changed_image( glow_tImData imlib, glow_tImImage image);
  virtual int imlib_image_rgb_width( glow_tImImage image);
  virtual int imlib_image_rgb_height( glow_tImImage image);
  virtual unsigned char *imlib_image_rgb_data( glow_tImImage image);
  int image_get_width( glow_tImImage image);
  int image_get_height( glow_tImImage image);
  int image_get_rowstride( glow_tImImage image);
  unsigned char *image_get_data( glow_tImImage image);
  void image_rotate( glow_tImImage *image, int to_rotation, int from_rotation);
  void image_flip_vertical( glow_tImImage *image);
  void image_flip_horizontal( glow_tImImage *image);
  void image_scale( int width, int height, glow_tImImage orig_im, glow_tImImage *im, 
		    glow_tPixmap *im_pixmap, glow_tPixmap *im_mask);
  int image_load( char *imagefile,
		  glow_tImImage *orig_im, glow_tImImage *im);
  int image_render( int width, int height,
		    glow_tImImage orig_im, glow_tImImage *im,
		    glow_tPixmap *im_pixmap, glow_tPixmap *im_mask);
  void image_free( glow_tImImage image);
  void pixmap_free( glow_tPixmap pixmap);
  void image_pixel_iter( glow_tImImage orig_image, glow_tImImage *image, 
			 void (* pixel_cb)(void *, unsigned char *), void *userdata);

};

class DrawPs {
 public:
  DrawPs( char *filename) : fp(filename), x(0), y(0) 
    {}
  ~DrawPs() { fp.close();}
  ofstream fp;
  double x;
  double y;
};

#endif
