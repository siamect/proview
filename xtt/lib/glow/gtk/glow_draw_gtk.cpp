/* 
 * Proview   $Id: glow_draw_gtk.cpp,v 1.18 2008-12-01 16:32:40 claes Exp $
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

#include "glow_std.h"

using namespace std;

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>

#include "glow.h"
#include "glow_ctx.h"
#include "glow_browctx.h"
#include "glow_growctx.h"
#include "glow_colpalctx.h"
#include "glow_curvectx.h"
#include "glow_draw_gtk.h"

#include "glow_msg.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

#define DRAW_PRESS_PIX 9


typedef struct {
	GtkWidget      	w;
	int		x;
	int		y;
	int		width;
	int		height;
	GlowArrayElem	*node;
	int		number;
        GlowCtx 	*ctx;
} draw_sAnnotData;

typedef struct {
	GlowCtx 	*ctx;
	void 		(*callback_func)( GlowCtx *ctx);
	guint		timer_id;
} draw_sTimerCb;

typedef struct {
	Pixmap pixmap[DRAW_PIXMAP_SIZE];
	} draw_sPixmap;

typedef struct {
	int	red;
	int	green;
	int	blue;
	} draw_sColor;

static char	font_names[glow_eFont__][glow_eDrawFont__][DRAW_FONT_SIZE][80] = { 
  { 
    {	
      "-*-Helvetica-Medium-R-Normal--8-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Bold-R-Normal--10-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Bold-R-Normal--14-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Bold-R-Normal--14-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Bold-R-Normal--24-*-*-*-P-*-ISO8859-1"
    },
    { 
      "-*-Helvetica-Medium-R-Normal--8-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Medium-R-Normal--10-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Medium-R-Normal--12-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Medium-R-Normal--14-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Medium-R-Normal--14-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
      "-*-Helvetica-Medium-R-Normal--24-*-*-*-P-*-ISO8859-1"
    } 
  },
  { 
    {	
      "-*-times-Medium-R-Normal--8-*-*-*-*-*-ISO8859-1", 
      "-*-times-Bold-R-Normal--10-*-*-*-*-*-ISO8859-1", 
      "-*-times-Bold-R-Normal--12-*-*-*-*-*-ISO8859-1", 
      "-*-times-Bold-R-Normal--14-*-*-*-*-*-ISO8859-1", 
      "-*-times-Bold-R-Normal--14-*-*-*-*-*-ISO8859-1", 
      "-*-times-Bold-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-times-Bold-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-times-Bold-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-times-Bold-R-Normal--24-*-*-*-*-*-ISO8859-1"
    },
    { 
      "-*-times-Medium-R-Normal--8-*-*-*-*-*-ISO8859-1", 
      "-*-times-Medium-R-Normal--10-*-*-*-*-*-ISO8859-1", 
      "-*-times-Medium-R-Normal--12-*-*-*-*-*-ISO8859-1", 
      "-*-times-Medium-R-Normal--14-*-*-*-*-*-ISO8859-1", 
      "-*-times-Medium-R-Normal--14-*-*-*-*-*-ISO8859-1", 
      "-*-times-Medium-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-times-Medium-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-times-Medium-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-times-Medium-R-Normal--24-*-*-*-*-*-ISO8859-1"
    } 
  },
  { 
    {	
      "-*-new century schoolbook-Medium-R-Normal--8-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Bold-R-Normal--10-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Bold-R-Normal--12-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Bold-R-Normal--14-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Bold-R-Normal--14-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Bold-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Bold-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Bold-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Bold-R-Normal--24-*-*-*-*-*-ISO8859-1"
    },
    { 
      "-*-new century schoolbook-Medium-R-Normal--8-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Medium-R-Normal--10-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Medium-R-Normal--12-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Medium-R-Normal--14-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Medium-R-Normal--14-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Medium-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Medium-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Medium-R-Normal--18-*-*-*-*-*-ISO8859-1", 
      "-*-new century schoolbook-Medium-R-Normal--24-*-*-*-*-*-ISO8859-1"
    } 
  },
  { 
    {	
      "-*-courier-Medium-R-Normal--8-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Bold-R-Normal--10-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Bold-R-Normal--12-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Bold-R-Normal--14-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Bold-R-Normal--14-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Bold-R-Normal--18-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Bold-R-Normal--18-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Bold-R-Normal--18-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Bold-R-Normal--24-*-*-*-m-*-ISO8859-1"
    },
    { 
      "-*-courier-Medium-R-Normal--8-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Medium-R-Normal--10-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Medium-R-Normal--12-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Medium-R-Normal--14-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Medium-R-Normal--14-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Medium-R-Normal--18-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Medium-R-Normal--18-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Medium-R-Normal--18-*-*-*-m-*-ISO8859-1", 
      "-*-courier-Medium-R-Normal--24-*-*-*-m-*-ISO8859-1"
    } 
  }
};


static  GdkEvent	last_event;

static GdkColor glow_allocate_named_color( GlowDrawGtk *draw_ctx, const char *named_color);
static GdkColor glow_allocate_color( GlowDrawGtk *draw_ctx, int rgb_red,
	int rgb_green, int rgb_blue);
static void event_timer( GlowCtx *ctx, int time_ms);
static void cancel_event_timer(GlowCtx *ctx);
static gboolean event_timer_cb( void *ctx);
static int glow_read_color_file( const char *filename, draw_sColor **color_array, 
	int *size);

static GdkGC *get_gc( GlowDrawGtk *draw_ctx, int i, int j)
{
  if ( !draw_ctx->gcs[i][j]) {
    GdkGCValues 		xgcv;
    double r, g, b;

    GlowColor::rgb_color( i, &r, &g, &b);
    xgcv.foreground = glow_allocate_color( draw_ctx, int(r * 65535), 
		int(g * 65535), int(b * 65535));
    xgcv.background = draw_ctx->background;
    xgcv.cap_style = GDK_CAP_BUTT;
    xgcv.line_width = j + 1;

    draw_ctx->gcs[i][j] = gdk_gc_new_with_values(
		draw_ctx->m_wind.window, &xgcv, 
		(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | 
		GDK_GC_LINE_WIDTH | GDK_GC_CAP_STYLE));
  }
  return draw_ctx->gcs[i][j];
}

static int glow_create_cursor( GlowDrawGtk *draw_ctx)
{
  /* Create some cursors */

  /* Cross cursor */
  draw_ctx->cursors[glow_eDrawCursor_CrossHair] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_CROSSHAIR);

  draw_ctx->cursors[glow_eDrawCursor_DiamondCross] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_DIAMOND_CROSS);

  draw_ctx->cursors[glow_eDrawCursor_Hand] = 
	        gdk_cursor_new_for_display( draw_ctx->display, GDK_HAND2);

  draw_ctx->cursors[glow_eDrawCursor_BottomLeftCorner] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_BOTTOM_LEFT_CORNER);

  draw_ctx->cursors[glow_eDrawCursor_BottomRightCorner] = 
	        gdk_cursor_new_for_display( draw_ctx->display, GDK_BOTTOM_RIGHT_CORNER);

  draw_ctx->cursors[glow_eDrawCursor_BottomSide] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_BOTTOM_SIDE);

  draw_ctx->cursors[glow_eDrawCursor_TopLeftCorner] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_TOP_LEFT_CORNER);

  draw_ctx->cursors[glow_eDrawCursor_TopRightCorner] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_TOP_RIGHT_CORNER);

  draw_ctx->cursors[glow_eDrawCursor_TopSide] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_TOP_SIDE);

  draw_ctx->cursors[glow_eDrawCursor_RightSide] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_RIGHT_SIDE);

  draw_ctx->cursors[glow_eDrawCursor_LeftSide] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_LEFT_SIDE);

  return 1;
}

static int draw_free_gc( GlowDrawGtk *draw_ctx)
{
  int i, j, k;

  for ( i = 1; i < glow_eDrawCursor__ ; i++)
    gdk_cursor_unref( draw_ctx->cursors[i]);

  gdk_gc_unref( draw_ctx->gc_inverse);
  for ( i = 0; i < glow_eDrawType__; i++) {
    for ( j = 0; j < DRAW_TYPE_SIZE; j++) {
      if ( draw_ctx->gcs[i][j])
	gdk_gc_unref( draw_ctx->gcs[i][j]);
    }
  }  

  for ( i = 0; i < glow_eFont__; i++) {
    for ( j = 0; j < glow_eDrawFont__; j++) {
      for ( k = 0; k < DRAW_FONT_SIZE; k++) {
	if ( draw_ctx->font[i][j][k])
	  gdk_font_unref( draw_ctx->font[i][j][k]);
      }
    }
  }
  return 1;
}

static int glow_create_gc( GlowDrawGtk *draw_ctx, GdkWindow *window)
{
  GdkFont 	       	*font;
  GdkGCValues 		xgcv;
  int			i;
  draw_sColor 		*color_array, *color_p;
  int 			size, sts;

  /* Inverse gc */
  xgcv.background = glow_allocate_named_color( draw_ctx, "black");
  xgcv.foreground = draw_ctx->background;
  xgcv.cap_style = GDK_CAP_BUTT;
  draw_ctx->gc_inverse = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | 
	GDK_GC_CAP_STYLE));

  /* Black line gc */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "black");
  xgcv.background = draw_ctx->background;
  xgcv.cap_style = GDK_CAP_BUTT;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[glow_eDrawType_Line][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | 
        GDK_GC_LINE_WIDTH | GDK_GC_CAP_STYLE));
  }

  /* Erase line gc */
  xgcv.foreground = draw_ctx->background;
  xgcv.background = draw_ctx->background;
  xgcv.cap_style = GDK_CAP_BUTT;
  // xgcv.fill_rule = WindingRule;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[glow_eDrawType_LineErase][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | 
        GDK_GC_LINE_WIDTH | GDK_GC_CAP_STYLE));
  }

  /* Red line gc */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "red");
  xgcv.background = draw_ctx->background;
  xgcv.cap_style = GDK_CAP_BUTT;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[glow_eDrawType_LineRed][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | 
        GDK_GC_LINE_WIDTH | GDK_GC_CAP_STYLE));
  }

  /* Gray line gc */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "gray");
  xgcv.background = draw_ctx->background;
  xgcv.cap_style = GDK_CAP_BUTT;
  // xgcv.fill_rule = WindingRule;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[glow_eDrawType_LineGray][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | 
        GDK_GC_LINE_WIDTH | GDK_GC_CAP_STYLE));
  }

  sts = glow_read_color_file( "/home/claes/test/ge_colors.dat", &color_array, &size);
  if ( ODD(sts)) {
    color_p = color_array;
    for ( int j = glow_eDrawType_Color4; j <= glow_eDrawType_Color300; j++) {
      if ( j - glow_eDrawType_Color4 >= size)
        break;

      xgcv.foreground = glow_allocate_color( draw_ctx, color_p->red, 
		color_p->green, color_p->blue);
      xgcv.background = draw_ctx->background;
      xgcv.cap_style = GDK_CAP_BUTT;
      // xgcv.fill_rule = WindingRule;
      for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
        xgcv.line_width = i + 1;

        draw_ctx->gcs[j][i] = gdk_gc_new_with_values( 
		window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | 
                GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH | GDK_GC_CAP_STYLE));
      }
      color_p++;
    }
    free( (char *) color_array);
  }

  /* Dashed line gc */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "black");
  xgcv.background = draw_ctx->background;
  xgcv.line_style = GDK_LINE_ON_OFF_DASH;
  xgcv.cap_style = GDK_CAP_BUTT;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;
    gint8 dashes = 7 + i;

    draw_ctx->gcs[glow_eDrawType_LineDashed][i] = gdk_gc_new_with_values( window, &xgcv, 
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH | 
        GDK_GC_LINE_STYLE | GDK_GC_CAP_STYLE));
    gdk_gc_set_dashes( draw_ctx->gcs[glow_eDrawType_LineDashed][i], 0, &dashes, 1);
  }

  /* Red dashed line gc */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "red");
  xgcv.background = draw_ctx->background;
  xgcv.line_style = GDK_LINE_ON_OFF_DASH;
  xgcv.cap_style = GDK_CAP_BUTT;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;
    gint8 dashes = 7 + i;

    draw_ctx->gcs[glow_eDrawType_LineDashedRed][i] = gdk_gc_new_with_values( window, &xgcv, 
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH| 
        GDK_GC_LINE_STYLE | GDK_GC_CAP_STYLE));
    gdk_gc_set_dashes( draw_ctx->gcs[glow_eDrawType_LineDashedRed][i], 0, &dashes, 1);
  }

  /* Text */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "black");
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    draw_ctx->gcs[glow_eDrawType_TextHelvetica][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }

  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    draw_ctx->gcs[glow_eDrawType_TextHelveticaBold][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }

  xgcv.foreground = draw_ctx->background;
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    draw_ctx->gcs[glow_eDrawType_TextHelveticaErase][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }

  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    draw_ctx->gcs[glow_eDrawType_TextHelveticaEraseBold][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }

  for ( i = 0; i < DRAW_FONT_SIZE; i++) {
    font = gdk_font_load( font_names[glow_eFont_Helvetica][glow_eDrawFont_HelveticaBold][i]);
    gdk_gc_set_font( draw_ctx->gcs[glow_eDrawType_TextHelveticaBold][i], font);
    gdk_gc_set_font( draw_ctx->gcs[glow_eDrawType_TextHelveticaEraseBold][i], font);
    draw_ctx->font[glow_eFont_Helvetica][glow_eDrawFont_HelveticaBold][i] = font;
  }
  for ( i = 0; i < DRAW_FONT_SIZE; i++) {
    font = gdk_font_load( font_names[glow_eFont_Helvetica][glow_eDrawFont_Helvetica][i]);
    gdk_gc_set_font( draw_ctx->gcs[glow_eDrawType_TextHelvetica][i], font);
    gdk_gc_set_font( draw_ctx->gcs[glow_eDrawType_TextHelveticaErase][i], font);
    draw_ctx->font[glow_eFont_Helvetica][glow_eDrawFont_Helvetica][i] = font;
  }

  return 1;
}


static GdkColor glow_allocate_named_color( GlowDrawGtk *draw_ctx, const char *named_color)
{
  GdkColor color;

  if ( !gdk_color_parse( named_color, &color))
    gdk_color_parse( "black", &color);
  gdk_colormap_alloc_color( draw_ctx->colormap, &color, FALSE, TRUE);

  return color;
}

static GdkColor glow_allocate_color( GlowDrawGtk *draw_ctx, int rgb_red,
	int rgb_green, int rgb_blue)
{
  GdkColor color;

  color.red = rgb_red;
  color.green = rgb_green;
  color.blue = rgb_blue;

  if ( !gdk_colormap_alloc_color( draw_ctx->colormap, &color, TRUE, TRUE)) {
    printf( "** Color not allocated !\n");
    color = glow_allocate_named_color( draw_ctx, "black");
  }
  return color;
}

GlowDrawGtk::~GlowDrawGtk()
{
  ctx->set_nodraw();
  if ( ctx->type() == glow_eCtxType_Grow)
    delete (GrowCtx *)ctx;
  else
    delete ctx;
  draw_free_gc( this);

  if ( m_wind.buffer)
    g_object_unref( m_wind.buffer);
  if ( m_wind.background_pixmap)
    g_object_unref( m_wind.background_pixmap);

  if ( nav_wind.buffer)
    g_object_unref( nav_wind.buffer);
  if ( nav_wind.background_pixmap)
    g_object_unref( nav_wind.background_pixmap);
}

int GlowDrawGtk::init_nav( GtkWidget *nav_widget)
{
  nav_wind.toplevel = nav_widget;
  nav_wind.window = nav_wind.toplevel->window;

  gtk_widget_modify_bg( nav_widget, GTK_STATE_NORMAL, &background);
//  glow_create_gc( this, nav_wind.window);

  ctx->no_nav = 0;
  return 1;
}

GlowDrawGtk::GlowDrawGtk(
	GtkWidget *toplevel, 
	void **glow_ctx,
        int (*init_proc)(GtkWidget *w, GlowCtx *ctx, void *client_data),
	void  *client_data, 
	glow_eCtxType type) 
  : ef(0), timer_id(0), click_sensitivity(0)
{
  memset( gcs, 0, sizeof(gcs));
  memset( font, 0, sizeof(font));
  memset( cursors, 0, sizeof(cursors));
  nav_wind.is_nav = 1;

  if ( type == glow_eCtxType_Brow)
    ctx = (GlowCtx *) new BrowCtx("Claes context", 20);
  else if ( type == glow_eCtxType_Grow)
    ctx = (GlowCtx *) new GrowCtx("Claes context", 20);
  else if ( type == glow_eCtxType_ColPal)
    ctx = (GlowCtx *) new ColPalCtx("Claes context", 20);
  else if ( type == glow_eCtxType_Curve)
    ctx = (GlowCtx *) new CurveCtx("Claes context", 20);
  else
    ctx = new GlowCtx("Claes context", 20);
  *glow_ctx = (void *) ctx;
  
  ctx->gdraw = this;
  m_wind.toplevel = toplevel;

  display = gtk_widget_get_display( m_wind.toplevel);
  m_wind.window = m_wind.toplevel->window;
  screen = gtk_widget_get_screen( m_wind.toplevel);

  ctx->mw.window = &m_wind;
  ctx->navw.window = &nav_wind;

  GtkStyle *style = gtk_widget_get_style( m_wind.toplevel);
  background = style->bg[GTK_STATE_NORMAL];
  original_background = background;

  colormap = gdk_colormap_new( gdk_visual_get_system(), TRUE);

  glow_create_gc( this, m_wind.window);

  glow_create_cursor( this);

  get_window_size( &ctx->mw, &ctx->mw.window_width, &ctx->mw.window_height);
  create_buffer( &ctx->mw);
  init_proc( toplevel, ctx, client_data);
}

int GlowDrawGtk::event_handler( GdkEvent event)
{
  static int	button_pressed = 0;
  static int	button_clicked = 0;
  static int	button_clicked_and_pressed = 0;
  static int 	button1_pressed = 0;
  static int 	button2_pressed = 0;
  static int 	button3_pressed = 0;
  static int	last_press_x = 0;
  static int	last_press_y = 0;

//  cout << "Event : button_pressed " << button_pressed << " clicked " << 
//	button_clicked << " c&p " << button_clicked_and_pressed << endl;

  if ( event.any.window == m_wind.window || event.type == GDK_KEY_PRESS) {
    switch ( event.type) {
    case GDK_KEY_PRESS : {
      guint keysym;

      keysym = event.key.keyval;
      if ( (keysym >= 0x020 && keysym <= 0x20ac) ||
	   (keysym >= 0xFF80 && keysym <= 0xFFB9 && keysym != GDK_KP_Enter && keysym != 0xFF44)) {
	unsigned char buff;
	//gint unival = gdk_keyval_to_unicode( keysym);
        //gchar utfval[6];
	//g_unichar_to_utf8( unival, utfval);
	// char *isoval = g_convert( utfval, 1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
	//buff = utfval[0];
	buff = event.key.string[0];

	if ( buff >= 0x020)
	  ctx->event_handler( glow_eEvent_Key_Ascii, 0, 0, (int)buff, 0);
	else
	  ctx->event_handler( glow_eEvent_Key_CtrlAscii, 0, 0, (int)buff, 0);
      }
      else {
	switch ( keysym) {
	case GDK_Return:
	case GDK_KP_Enter:
	case 0xFF44:			// XK_KP_Enter sometimes...
	  ctx->event_handler( glow_eEvent_Key_Return, 0, 0, 0, 0);
	  break;
	case GDK_Up:
	  ctx->event_handler( glow_eEvent_Key_Up, 0, 0, 0, 0);
	  break;
	case GDK_Down:
	  ctx->event_handler( glow_eEvent_Key_Down, 0, 0, 0, 0);
	  break;
	case GDK_Right:
	  ctx->event_handler( glow_eEvent_Key_Right, 0, 0, 0, 0);
	  break;
	case GDK_Left:
	  ctx->event_handler( glow_eEvent_Key_Left, 0, 0, 0, 0);
	  break;
	case GDK_Page_Up:
	  ctx->event_handler( glow_eEvent_Key_PageUp, 0, 0, 0, 0);
	  break;
	case GDK_Page_Down:
	  ctx->event_handler( glow_eEvent_Key_PageDown, 0, 0, 0, 0);
	  break;
	case GDK_Delete:
	case GDK_BackSpace:
	  ctx->event_handler( glow_eEvent_Key_BackSpace, 0, 0, 0, 0);
	  break;
	case GDK_KP_F1:
	  ctx->event_handler( glow_eEvent_Key_PF1, 0, 0, 0, 0);
	  break;
	case GDK_KP_F2:
	  ctx->event_handler( glow_eEvent_Key_PF2, 0, 0, 0, 0);
	  break;
	case GDK_KP_F3:
	  ctx->event_handler( glow_eEvent_Key_PF3, 0, 0, 0, 0);
	  break;
	case GDK_KP_F4:
	  ctx->event_handler( glow_eEvent_Key_PF4, 0, 0, 0, 0);
	  break;
	case GDK_Cancel:
	  ctx->event_handler( glow_eEvent_Key_Escape, 0, 0, 0, 0);
	  break;
	case GDK_Tab:
	  if ( event.key.state & GDK_SHIFT_MASK)
	    ctx->event_handler( glow_eEvent_Key_ShiftTab, 0, 0, 0, 0);
	  else
	    ctx->event_handler( glow_eEvent_Key_Tab, 0, 0, 0, 0);
	  break;
	default:
	  ;
	}
      }
      break;
    }
    case GDK_BUTTON_PRESS : 
// cout << "Button press event" << endl;
//        printf( "-- Button event: (%d,%d) button: %d time:%d\n", event.xbutton.x, 
//		event.xbutton.y, event.xbutton.button, event.xbutton.time);

//        XSetInputFocus( display, m_wind.window, 
//		RevertToNone, CurrentTime);

      switch ( event.button.button) {
      case 1:
	ctx->event_handler( glow_eEvent_MB1Down, (int)event.button.x, (int)event.button.y, 0, 0);
	if ( click_sensitivity & glow_mSensitivity_MB1Click && 
	     !(click_sensitivity & glow_mSensitivity_MB1DoubleClick) && 
	     !(click_sensitivity & glow_mSensitivity_MB1Press)) {
	  memcpy( &last_event, &event, sizeof(event));
	  button_pressed = 0;
	  button_clicked = 1;
	  last_press_x = (int)event.button.x;
	  last_press_y = (int)event.button.y;
	  return 1;
	}
	else if ( !(click_sensitivity & glow_mSensitivity_MB1Click) && 
		  !(click_sensitivity & glow_mSensitivity_MB1DoubleClick) && 
		  click_sensitivity & glow_mSensitivity_MB1Press) {
	  memcpy( &last_event, &event, sizeof(event));
	  button_pressed = 1;
	  button_clicked = 0;
	  last_press_x = (int)event.button.x;
	  last_press_y = (int)event.button.y;
	}
	break;
      case 3:
	ctx->event_handler( glow_eEvent_MB3Down, (int)event.button.x, (int)event.button.y, 0, 0);
	if ( click_sensitivity & glow_mSensitivity_MB3Press && 
	     !(click_sensitivity & glow_mSensitivity_MB3DoubleClick) && 
	     !(click_sensitivity & glow_mSensitivity_MB3Click)) {
	  ctx->event_handler( glow_eEvent_MB3Press, 
			      (int)event.button.x, (int)event.button.y, 0, 0);
	  click_sensitivity = 0;
	  return 1;
	}
	else if ( click_sensitivity & glow_mSensitivity_MB3Click && 
		  !(click_sensitivity & glow_mSensitivity_MB3DoubleClick) && 
		  !(click_sensitivity & glow_mSensitivity_MB3Press))
	  {
	    memcpy( &last_event, &event, sizeof(event));
	    button_pressed = 0;
	    button_clicked = 1;
	    last_press_x = (int)event.button.x;
	    last_press_y = (int)event.button.y;
	    return 1;
	  }
	break;
      }

      /* Detect press or click event */
      if ( button_clicked) {
	/* Wait for release */
	button_clicked_and_pressed = 1;
	cancel_event_timer( ctx);
	button_clicked = 0;
	memcpy( &last_event, &event, sizeof(event));
	button_pressed = event.button.button;
	last_press_x = (int)event.button.x;
	last_press_y = (int)event.button.y;
	event_timer(ctx, 200);
	return 1;
      }
      if ( !button_pressed ) {
	memcpy( &last_event, &event, sizeof(event));
	button_pressed = event.button.button;
	last_press_x = (int)event.button.x;
	last_press_y = (int)event.button.y;
	event_timer(ctx, 200);
	return 1;
      }
      else {    
// cout << "Button press detected" << endl;
	/* Press event, callback from timer */
	button_pressed = 0;
	button_clicked_and_pressed = 0;
	switch ( event.button.button) {
	case 1: // Button 1

	  button1_pressed = 1;
	  if (       (event.button.state & GDK_SHIFT_MASK) && 
		     !(event.button.state & GDK_CONTROL_MASK)) {
	    ctx->event_handler( glow_eEvent_MB1PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK)) {
	    ctx->event_handler( glow_eEvent_MB1PressCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else if (  (event.button.state & GDK_SHIFT_MASK) && 
		     (event.button.state & GDK_CONTROL_MASK)) {
	    ctx->event_handler( glow_eEvent_MB1PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else {
	    ctx->event_handler( glow_eEvent_MB1Press, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  break;
	case 2: // Button2
	  button2_pressed = 1;
	  if (       (event.button.state & GDK_SHIFT_MASK) && 
		     !(event.button.state & GDK_CONTROL_MASK)) {
	    ctx->event_handler( glow_eEvent_MB2PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK)) {
	    ctx->event_handler( glow_eEvent_MB2PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else if (  (event.button.state & GDK_SHIFT_MASK) && 
		     (event.button.state & GDK_CONTROL_MASK)) {
	    ctx->event_handler( glow_eEvent_MB2PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else {
	    ctx->event_handler( glow_eEvent_MB2Press, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  break;
	case 3: // Button3
	  button3_pressed = 1;
#if 0
	  if (       (event.button.state & GDK_SHIFT_MASK) && 
		     !(event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB3PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB3PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if (  (event.button.state & GDK_SHIFT_MASK) && 
		     (event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB3PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else
#endif
	    ctx->event_handler( glow_eEvent_MB3Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  click_sensitivity = 0;
	  break;
	}
      }
      break;

    case GDK_BUTTON_RELEASE:
      button1_pressed = 0;
      button2_pressed = 0;
      button3_pressed = 0;

// cout << "Button release event" << endl;

      switch ( event.button.button) {
      case Button1:
	ctx->event_handler( glow_eEvent_MB1Up, (int)event.button.x, (int)event.button.y, 0, 0);
	break;
      }

      if ( ! button_pressed ) {
	if ( button_clicked) {
	  /* Button click, callback from timer */
// cout << "Button click detected state " << event.button.state << endl;
            button_clicked = 0;
          switch ( event.button.button) {
	  case 1: // Button1
	    if (       (event.button.state & GDK_SHIFT_MASK) && 
		       !(event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB1ClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB1ClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB1ClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else {
	      ctx->event_handler( glow_eEvent_MB1Click, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    break;
	  case 2: // Button2
	    if (       (event.button.state & GDK_SHIFT_MASK) && 
		       !(event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB2ClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB2ClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB2ClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else {
	      ctx->event_handler( glow_eEvent_MB2Click, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    break;
	  case 3: // Button3
#if 0
	    if (       (event.button.state & GDK_SHIFT_MASK) && 
		       !(event.button.state & GDK_CONTROL_MASK))
	      ctx->event_handler( glow_eEvent_MB3ClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      ctx->event_handler( glow_eEvent_MB3ClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK))
	      ctx->event_handler( glow_eEvent_MB3ClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else
#endif
	      ctx->event_handler( glow_eEvent_MB3Click, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	    break;
	  }
	}
	else {
	  /* Button release */
// cout << "Button release detected" << endl;
	  ctx->event_handler( glow_eEvent_ButtonRelease, (int)event.button.x, (int)event.button.y, 0, 0);
	}        
      }
      else {
	/* Button click */
	cancel_event_timer( ctx);
	if ( ! button_clicked_and_pressed) {
//  cout << "Button first click detected" << endl;
	  /* wait for button double click */
	  memcpy( &last_event, &event, sizeof(event));
	  button_clicked = 1;
	  event_timer( ctx, 200);
	  button_pressed = 0;
	  return 1;
	}
	else {
	  /* Button double click */ 
// cout << "Button double click detected" << endl;
	  cancel_event_timer( ctx);
	  button_clicked = 0;
	  button_pressed = 0;
	  button_clicked_and_pressed = 0;
	  switch ( event.button.button) {
	  case 1: // Button1
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB1DoubleClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB1DoubleClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB1DoubleClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else {
	      ctx->event_handler( glow_eEvent_MB1DoubleClick, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    break;
	  case 2: // Button2
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB2DoubleClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB2DoubleClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK)) {
	      ctx->event_handler( glow_eEvent_MB2DoubleClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else {
	      ctx->event_handler( glow_eEvent_MB2DoubleClick, (int)event.button.x, (int)event.button.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    break;
	  case 3: // Button3
#if 0
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK))
	      ctx->event_handler( glow_eEvent_MB3DoubleClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      ctx->event_handler( glow_eEvent_MB3DoubleClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK))
	      ctx->event_handler( glow_eEvent_MB3DoubleClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else
	      ctx->event_handler( glow_eEvent_MB3DoubleClick, (int)event.button.x, (int)event.button.y, 0, 0);
#endif
	    break;
	  }
	}
      }
      break;
    case GDK_EXPOSE:
//        printf( "-- Expose event.. x: %d, y: %d, w: %d, h: %d\n",
//		event.expose.x, 
//		event.expose.y, event.expose.width, event.expose.height);
      ctx->event_handler( glow_eEvent_Exposure, event.expose.area.x, 
			  event.expose.area.y, event.expose.area.width, 
			  event.expose.area.height);
      break;
    case GDK_VISIBILITY_NOTIFY : 
      switch ( event.visibility.state) {
      case GDK_VISIBILITY_UNOBSCURED:
	ctx->event_handler( glow_eEvent_VisibilityUnobscured, 0, 0, 0, 0);
	break;
      default: 
	ctx->event_handler( glow_eEvent_VisibilityObscured, 0, 0, 0, 0);
	break;
      }
      break;        
    case GDK_MOTION_NOTIFY:
//            printf( "-- Button1 motion event: (%d,%d)\n", event.button.x, 
//		event.button.y);
      if ( event.motion.is_hint) {
	int x, y;

	gdk_window_get_pointer( event.any.window, &x, &y, NULL);
	event.button.x = x;
	event.button.y = y;
      }

      if ( button3_pressed) {
	button3_pressed = 0;
	button_pressed = 0;
	button_clicked_and_pressed = 0;
      }
      if ( button_pressed && 
	   (abs( event.button.x - last_press_x) > DRAW_PRESS_PIX ||
	    abs( event.button.y - last_press_y) > DRAW_PRESS_PIX)) {
//          printf( "Press: x %d last_x %d\n", event.button.x, last_press_x);
//          printf( "       y %d last_y %d\n", event.button.y, last_press_y);

	event.button.x = last_press_x;
	event.button.y = last_press_y;

	/* Button press */
	cancel_event_timer( ctx);
	switch ( button_pressed) {
	case 1: // Button1
	  button1_pressed = 1;
	  if ( (event.button.state & GDK_SHIFT_MASK) && 
	       !(event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB1PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB1PressCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if (  (event.button.state & GDK_SHIFT_MASK) && 
		     (event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB1PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else
	    ctx->event_handler( glow_eEvent_MB1Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  break;
	case 2: // Button2
	  button2_pressed = 1;
	  if ( (event.button.state & GDK_SHIFT_MASK) && 
	       !(event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB2PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB2PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if (  (event.button.state & GDK_SHIFT_MASK) && 
		     (event.button.state & GDK_CONTROL_MASK))
	    ctx->event_handler( glow_eEvent_MB2PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else
	    ctx->event_handler( glow_eEvent_MB2Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  break;
	case 3: // Button3
	  button3_pressed = 1;
	  ctx->event_handler( glow_eEvent_MB3Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  break;
	}
	button_pressed = 0;
	button_clicked_and_pressed = 0;
	
      }
      if ( button1_pressed || button2_pressed || button3_pressed)
	ctx->event_handler( glow_eEvent_ButtonMotion, (int)event.button.x, (int)event.button.y, 0, 0);
      else
	ctx->event_handler( glow_eEvent_CursorMotion, (int)event.button.x, (int)event.button.y, 0, 0);
      break;
    case GDK_ENTER_NOTIFY:
      ctx->event_handler( glow_eEvent_Enter, (int)event.crossing.x, (int)event.crossing.y, 0, 0);
      break;          
    case GDK_LEAVE_NOTIFY:

      ctx->event_handler( glow_eEvent_Leave, (int)event.crossing.x, (int)event.crossing.y, 0, 0);
      break;          
    case GDK_MAP:
      ctx->event_handler( glow_eEvent_Map, 0, 0, 0, 0);
      break;          
    case GDK_UNMAP:
      ctx->event_handler( glow_eEvent_Unmap, 0, 0, 0, 0);
      break;          
    case GDK_FOCUS_CHANGE:
      break;          
    case GDK_SCROLL:
      if ( event.scroll.direction == GDK_SCROLL_UP)
	ctx->event_handler( glow_eEvent_ScrollUp, 0, 0, 0, 0);
      else if ( event.scroll.direction == GDK_SCROLL_DOWN)
	ctx->event_handler( glow_eEvent_ScrollDown, 0, 0, 0, 0);
      break;
    default:
      break;
    }
  }
  else if ( event.any.window == nav_wind.window) {
    switch ( event.type) {
    case GDK_BUTTON_PRESS : 
//            printf( "-- Button event nav: (%d,%d) button: %d\n", event.button.x, 
//		event.button.y, event.button.button);
      switch ( event.button.button) {
      case 1: // Button1
	button1_pressed = 1;
	ctx->event_handler_nav( glow_eEvent_MB1Press, (int)event.button.x, (int)event.button.y);
	break;
      case 2: // Button2
	button2_pressed = 1;
	ctx->event_handler_nav( glow_eEvent_MB2Press, (int)event.button.x, (int)event.button.y);
	break;
      case 3: // Button3
	button3_pressed = 1;
	break;
      }
      break;          
    case GDK_BUTTON_RELEASE : 
//        printf( "-- Button release event nav: (%d,%d)\n", event.button.x, 
//		event.button.y);
      switch ( event.button.button) {
      case 1: // Button1
	button1_pressed = 0;
	break;
      case 2: // Button2
	button2_pressed = 0;
	break;
      case 3: // Button3
	button3_pressed = 0;
	break;
      }
      ctx->event_handler_nav( glow_eEvent_ButtonRelease, (int)event.button.x, (int)event.button.y);
      break;          
    case GDK_EXPOSE:
//        printf( "-- Navigator expose event..\n" );
      ctx->event_handler_nav( glow_eEvent_Exposure, 0, 0);
      break; 
    case GDK_MOTION_NOTIFY:
//            printf( "-- Button1 motion event nav: (%d,%d)\n", event.button.x, 
//		event.button.y);

      if ( event.motion.is_hint) {
	int x, y;

	gdk_window_get_pointer( event.any.window, &x, &y, NULL);
	event.button.x = x;
	event.button.y = y;
      }
      if ( button1_pressed || button2_pressed || button3_pressed)
	ctx->event_handler_nav( glow_eEvent_ButtonMotion, (int)event.button.x, (int)event.button.y);
      else
	ctx->event_handler_nav( glow_eEvent_CursorMotion, (int)event.button.x, (int)event.button.y);
      break;          
    default: ;
    }
  }
  gdk_display_flush( display);
  return 1;
}

void GlowDrawGtk::enable_event( glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

int GlowDrawGtk::rect( GlowWind *wind, int x, int y, int width, int height, 
	glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on) 
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_rectangle( w->window, 
			get_gc( this, gc_type+highlight, idx), 0,
			x, y, width, height);
  if ( w->double_buffer_on)
    gdk_draw_rectangle( w->buffer, 
		    get_gc( this, gc_type+highlight, idx), 0,
		    x, y, width, height);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));

  return 1;
}

int GlowDrawGtk::rect_erase( GlowWind *wind, int x, int y, int width, int height,
			      int idx)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_rectangle( w->window, 
			get_gc( this, glow_eDrawType_LineErase, idx), 0, 
			x, y, width, height);
  if ( w->double_buffer_on)
    gdk_draw_rectangle( w->buffer, 
			get_gc( this, glow_eDrawType_LineErase, idx), 0,
			x, y, width, height);
  
  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  return 1;
}

int GlowDrawGtk::arrow( GlowWind *wind, int x1, int y1, int x2, int y2, 
			 int x3, int y3,
			 glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;

  GdkPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_polygon( w->window,
	get_gc( this, gc_type+highlight, idx), 1, p, 4);
  if ( w->double_buffer_on)
    gdk_draw_polygon( w->buffer,
	get_gc( this, gc_type+highlight, idx), 1, p, 4);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));

  return 1;
}

int GlowDrawGtk::arrow_erase( GlowWind *wind, int x1, int y1, int x2, int y2, 
			       int x3, int y3,
			       int idx)
{
  if ( ctx->nodraw) return 1;

  GdkPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_polygon( w->window,
		      get_gc( this, glow_eDrawType_LineErase, idx), 1,
		      p, 4);
  if ( w->double_buffer_on)
    gdk_draw_polygon( w->buffer,
		      get_gc( this, glow_eDrawType_LineErase, idx), 1,
		      p, 4);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  return 1;
}

int GlowDrawGtk::arc( GlowWind *wind, int x, int y, int width, int height, 
		       int angle1, int angle2,
		       glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  // Fix for highlight for connections in grow
  if ( highlight && ctx->type() == glow_eCtxType_Grow)
    gc_type = glow_eDrawType_LineHighlight;

  // if ( width < 35 && height < 35) {width++; height++;} // This looks good in Reflexion X ...

  if ( angle1 >= 360)
    angle1 = angle1 - angle1 / 360 * 360;
  else if ( angle1 < 0)
    angle1 = angle1 + ( -angle1 / 360 + 1) * 360;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_arc( w->window,
		  get_gc( this, gc_type+highlight, idx), 0,
		  x, y, width, height, angle1*64, angle2*64);

  if ( w->double_buffer_on)
    gdk_draw_arc( w->buffer,
		  get_gc( this, gc_type+highlight, idx), 0,
		  x, y, width, height, angle1*64, angle2*64);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));

  return 1;
}

int GlowDrawGtk::fill_arc( GlowWind *wind, int x, int y, int width, int height, 
	int angle1, int angle2, glow_eDrawType gc_type, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_LineRed;

  if ( angle1 >= 360)
    angle1 = angle1 - angle1 / 360 * 360;
  else if ( angle1 < 0)
    angle1 = angle1 + ( -angle1 / 360 + 1) * 360;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, 0));

  if ( !w->draw_buffer_only)
    gdk_draw_arc( w->window,
		  get_gc( this, gc_type+highlight, 0), 1, 
		  x, y, width, height, angle1*64, angle2*64);
  if ( w->double_buffer_on)
    gdk_draw_arc( w->buffer,
		  get_gc( this, gc_type+highlight, 0), 1, 
		  x, y, width, height, angle1*64, angle2*64);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, 0));

  return 1;
}

int GlowDrawGtk::arc_erase( GlowWind *wind, int x, int y, int width, int height,
	int angle1, int angle2,
	int idx)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  // if ( width < 35 && height < 35) {width++; height++;} // This looks good in Reflexion X ...

  if ( angle1 >= 360)
    angle1 = angle1 - angle1 / 360 * 360;
  else if ( angle1 < 0)
    angle1 = angle1 + ( -angle1 / 360 + 1) * 360;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_arc( w->window,
		  get_gc( this, glow_eDrawType_LineErase, idx), 0,
		  x, y, width, height, angle1*64, angle2*64);
  if ( w->double_buffer_on)
    gdk_draw_arc( w->buffer,
		  get_gc( this, glow_eDrawType_LineErase, idx), 0,
		  x, y, width, height, angle1*64, angle2*64);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));
  return 1;
}

int GlowDrawGtk::line( GlowWind *wind, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  // Fix for highlight for connections in grow
  if ( highlight && ctx->type() == glow_eCtxType_Grow)
    gc_type = glow_eDrawType_LineHighlight;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_line( w->window,
	get_gc( this, gc_type+highlight, idx), 
	x1, y1, x2, y2);
  if ( w->double_buffer_on)
    gdk_draw_line( w->buffer,
	get_gc( this, gc_type+highlight, idx), 
	x1, y1, x2, y2);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));
  return 1;
}

int GlowDrawGtk::line_dashed( GlowWind *wind, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight, glow_eLineType line_type)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  // Fix for highlight for connections in grow
  if ( highlight && ctx->type() == glow_eCtxType_Grow)
    gc_type = glow_eDrawType_LineHighlight;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  gdk_gc_set_line_attributes( get_gc( this, gc_type+highlight, idx), idx+1,
			      GDK_LINE_ON_OFF_DASH, GDK_CAP_BUTT, GDK_JOIN_MITER);
  gint dash_offset = 0;
  gint8 dashes[4];
  switch ( line_type) {
  case glow_eLineType_Dashed1:
    dashes[0] = 1 + idx;
    gdk_gc_set_dashes( get_gc( this, gc_type+highlight, idx), dash_offset, dashes, 1);
    break;
  case glow_eLineType_Dashed2:
    dashes[0] = 1 + 2 * idx;
    gdk_gc_set_dashes( get_gc( this, gc_type+highlight, idx), dash_offset, dashes, 1);
    break;
  case glow_eLineType_Dashed3:
    dashes[0] = 1 + 3 * idx;
    gdk_gc_set_dashes( get_gc( this, gc_type+highlight, idx), dash_offset, dashes, 1);
    break;
  case glow_eLineType_Dotted: {
    dashes[0] = 1 + idx;
    dashes[1] = 1 + 4 * idx;
    gdk_gc_set_dashes( get_gc( this, gc_type+highlight, idx), dash_offset, dashes, 2);
    break;
    }
  case glow_eLineType_DotDashed1: {
    dashes[0] = 1 + 3 * idx;
    dashes[1] = 1 + 2 * idx;
    dashes[2] = 1 + idx;
    dashes[3] = 1 + 2 * idx;
    gdk_gc_set_dashes( get_gc( this, gc_type+highlight, idx), dash_offset, dashes, 4);
    break;
    }
  case glow_eLineType_DotDashed2: {
    dashes[0] = 1 + 6 * idx;
    dashes[1] = 1 + 3 * idx;
    dashes[2] = 1 + idx;
    dashes[3] = 1 + 3 * idx;
    gdk_gc_set_dashes( get_gc( this, gc_type+highlight, idx), dash_offset, dashes, 4);
    break;
    }
  default: ;
  }

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_line( w->window,
	get_gc( this, gc_type+highlight, idx), 
	x1, y1, x2, y2);
  if ( w->double_buffer_on)
    gdk_draw_line( w->buffer,
	get_gc( this, gc_type+highlight, idx), 
	x1, y1, x2, y2);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));

  gdk_gc_set_line_attributes( get_gc( this, gc_type+highlight, idx), idx+1,
			      GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);
  return 1;
}

int GlowDrawGtk::line_erase( GlowWind *wind, int x1, int y1, int x2, int y2,
	int idx)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    gdk_draw_line( w->window,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	x1, y1, x2, y2);
  if ( w->double_buffer_on)
    gdk_draw_line( w->buffer,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	x1, y1, x2, y2);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  return 1;
}

int GlowDrawGtk::polyline( GlowWind *wind, glow_sPointX *points, int point_cnt,
	glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  GdkPoint* gpoints = points_to_gdk_points( points, point_cnt);
  if ( !w->draw_buffer_only)
    gdk_draw_lines( w->window,
	get_gc( this, gc_type+highlight, idx), 
	gpoints, point_cnt);
  if ( w->double_buffer_on)
    gdk_draw_lines( w->buffer,
	get_gc( this, gc_type+highlight, idx), 
	gpoints, point_cnt);

  free( gpoints);
  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));
  return 1;
}

int GlowDrawGtk::fill_polyline( GlowWind *wind, glow_sPointX *points, int point_cnt,
	glow_eDrawType gc_type, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, 0));

  GdkPoint* gpoints = points_to_gdk_points( points, point_cnt);
  if ( !w->draw_buffer_only)
    gdk_draw_polygon( w->window,
		      get_gc( this, gc_type+highlight, 0), 
		      1, gpoints, point_cnt);
  if ( w->double_buffer_on)
    gdk_draw_polygon( w->buffer,
	get_gc( this, gc_type+highlight, 0), 1, gpoints, point_cnt);

  free( gpoints);
  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, 0));
  return 1;
}

int GlowDrawGtk::polyline_erase( GlowWind *wind, glow_sPointX *points, int point_cnt,
	int idx)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  GdkPoint* gpoints = points_to_gdk_points( points, point_cnt);
  if ( !w->draw_buffer_only)
    gdk_draw_lines( w->window,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	gpoints, point_cnt);
  if ( w->double_buffer_on)
    gdk_draw_lines( w->buffer,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	gpoints, point_cnt);

  free( gpoints);
  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));
  return 1;
}

int GlowDrawGtk::text( GlowWind *wind, int x, int y, char *text, int len,
		       glow_eDrawType gc_type, glow_eDrawType color, int idx, 
		       int highlight, int line, glow_eFont font_idx, double size)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( !((GrowCtx *)ctx)->bitmap_fonts)
    return text_pango( wind, x, y, text, len, gc_type, color, idx, highlight, line, font_idx, size);

  if ( font_idx > glow_eFont_Courier)
    font_idx = glow_eFont_Helvetica;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));

  int font_type = get_font_type( gc_type);
  load_font( font_idx, font_type, idx);

  if ( font_idx != glow_eFont_Helvetica)
    gdk_gc_set_font( get_gc( this, gc_type, idx), 
		     font[font_idx][font_type][idx]);
  
  if ( color != glow_eDrawType_Line) {
    GdkGCValues 		xgcv;

    gdk_gc_get_values( get_gc( this, color, 0), &xgcv);

    gdk_gc_set_values( get_gc( this, gc_type, idx), &xgcv,
	GDK_GC_FOREGROUND);
  }

  if ( !w->draw_buffer_only)
    gdk_draw_text( w->window,
		   font[font_idx][font_type][idx],
		   get_gc( this, gc_type, idx), 
		   x, y, text, len);
  if ( w->double_buffer_on)
    gdk_draw_text( w->buffer,
		 font[font_idx][font_type][idx],
		 get_gc( this, gc_type, idx), 
		 x, y, text, len);
  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));

  if ( color != glow_eDrawType_Line) {
    GdkGCValues 		xgcv;

    gdk_gc_get_values( get_gc( this, glow_eDrawType_Line, 0), 
	&xgcv);

    gdk_gc_set_values( get_gc( this, gc_type, idx), &xgcv,
	GDK_GC_FOREGROUND);
  }
  if ( font_idx != glow_eFont_Helvetica)
    gdk_gc_set_font( get_gc( this, gc_type, idx), 
		     font[glow_eFont_Helvetica][font_type][idx]);
  return 1;
}

int GlowDrawGtk::text_cursor( GlowWind *wind, int x, int y, char *text, int len,
			      glow_eDrawType gc_type, glow_eDrawType color, int idx, 
			      int highlight, int pos, glow_eFont font, double size)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  int theight, tdescent, width, height, descent;
  get_text_extent( "A", 1, gc_type, idx, font,
		   &width, &height, &descent, size);
  if ( pos != 0)
    get_text_extent( text, pos, gc_type, idx, font,
		     &width, &theight, &tdescent, size);
  else
    width = 0;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));

  if ( !w->draw_buffer_only) {
    gdk_draw_line( w->window,
	get_gc( this, color, 1), 
	x + width, y + descent, x + width, y - height + descent);  
  }
  if ( w->double_buffer_on) {
    gdk_draw_line( w->buffer,
	get_gc( this, color, 1), 
	x + width, y + descent, x + width, y - height + descent);
  }
  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));

  return 1;
}

int GlowDrawGtk::text_erase( GlowWind *wind, int x, int y, char *text, int len,
			     glow_eDrawType gc_type, int idx, int line, glow_eFont font_idx, 
			     double size)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( !((GrowCtx *)ctx)->bitmap_fonts)
    return text_erase_pango( wind, x, y, text, len, gc_type, idx, line, font_idx, size);

  if ( font_idx > glow_eFont_Courier)
    font_idx = glow_eFont_Helvetica;

  if ( gc_type == glow_eDrawType_TextHelvetica)
    gc_type = glow_eDrawType_TextHelveticaErase;
  else if ( gc_type == glow_eDrawType_TextHelveticaBold)
    gc_type = glow_eDrawType_TextHelveticaEraseBold;

  int font_type = get_font_type( gc_type);
  load_font( font_idx, font_type, idx);

  if ( font_idx != glow_eFont_Helvetica)
    gdk_gc_set_font( get_gc( this, gc_type, idx), 
		     font[font_idx][font_type][idx]);
  
  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));
  if ( !w->draw_buffer_only)
    gdk_draw_text( w->window,
		   font[font_idx][font_type][idx],
		   get_gc( this, gc_type, idx), 
		   x, y, text, len);
  if ( w->double_buffer_on)
    gdk_draw_text( w->buffer,
		   font[font_idx][font_type][idx],
		   get_gc( this, gc_type, idx), 
		   x, y, text, len);
  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));

  if ( font_idx != glow_eFont_Helvetica)
    gdk_gc_set_font( get_gc( this, gc_type, idx), 
		     font[glow_eFont_Helvetica][font_type][idx]);
  
  return 1;
}

int GlowDrawGtk::pixmaps_create( GlowWind *wind, glow_sPixmapData *pixmap_data,
	void **pixmaps)
{
  return 1;
}

void GlowDrawGtk::pixmaps_delete( GlowWind *wind, void *pixmaps)
{
}

int GlowDrawGtk::pixmap( GlowWind *wind, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int highlight, int line)
{
  return 1;
}

int GlowDrawGtk::pixmap_inverse( GlowWind *wind, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line)
{
  return 1;
}

int GlowDrawGtk::pixmap_erase( GlowWind *wind, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line)
{
  return 1;
}

int GlowDrawGtk::image( GlowWind *wind, int x, int y, int width, int height,
			glow_tImImage image, glow_tPixmap pixmap, glow_tPixmap clip_mask)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  if ( ctx->nodraw) return 1;

  if ( width == 0 || height == 0)
    return 1;

  if ( clip_mask)
    set_image_clip_mask( clip_mask, x, y);
  else if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_Line, 0));

  if ( !w->draw_buffer_only)
    gdk_draw_pixbuf( w->window, get_gc( this, glow_eDrawType_Line, 0), 
		     (GdkPixbuf *)image, 0, 0, x, y, width, height,
		     GDK_RGB_DITHER_NONE, 0, 0);
#if 0
    gdk_draw_drawable( w->window,
		       get_gc( this, glow_eDrawType_Line, 0), 
		       (GdkDrawable *) pixmap,
		       0, 0, x, y, width, height);
#endif
  if ( w->double_buffer_on)
    gdk_draw_pixbuf( w->buffer, get_gc( this, glow_eDrawType_Line, 0), 
		     (GdkPixbuf *)image, 0, 0, x, y, width, height,
		     GDK_RGB_DITHER_NONE, 0, 0);
#if 0
    gdk_draw_drawable( w->buffer,
		       get_gc( this, glow_eDrawType_Line, 0), 
		       (GdkDrawable *) pixmap,
		       0, 0, x, y, width, height);
#endif
  if ( clip_mask)
    reset_image_clip_mask();
  else if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_Line, 0));
  return 1;
}


int GlowDrawGtk::fill_rect( GlowWind *wind, int x, int y, int w, int h, 
	glow_eDrawType gc_type)
{
  DrawWindGtk *ww = (DrawWindGtk *) wind->window;
  if ( ctx->nodraw) return 1;

  if ( ww->clip_on)
    set_clip( ww, get_gc( this, gc_type, 0));
  if ( !ww->draw_buffer_only)
    gdk_draw_rectangle( ww->window,
	get_gc( this, gc_type, 0), 1, x, y, w, h);
  if ( ww->double_buffer_on)
    gdk_draw_rectangle( ww->buffer,
	get_gc( this, gc_type, 0), 1, x, y, w, h);
  if ( ww->clip_on)
    reset_clip( ww, get_gc( this, gc_type, 0));
  return 1;
}

void GlowDrawGtk::clear( GlowWind *wind)
{
  if ( ctx->nodraw) return;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( !w->double_buffer_on)
    gdk_window_clear( w->window);
  else
    buffer_background( w);
}

void GlowDrawGtk::copy_buffer( GlowWind *wind,
		int ll_x, int ll_y, int ur_x, int ur_y)
{
  if ( ctx->nodraw) return;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  int x0 = min( ll_x, ur_x);
  int x1 = max( ll_x, ur_x);
  int y0 = min( ll_y, ur_y);
  int y1 = max( ll_y, ur_y);

  if ( w->clip_cnt) {
    x0 = max(x0, w->clip_rectangle[w->clip_cnt-1].x);
    y0 = max(y0, w->clip_rectangle[w->clip_cnt-1].y);
    x1 = min(x1, w->clip_rectangle[w->clip_cnt-1].x + 
	     w->clip_rectangle[w->clip_cnt-1].width);
    y1 = min(y1, w->clip_rectangle[w->clip_cnt-1].y + 
	     w->clip_rectangle[w->clip_cnt-1].height);
  }

  gdk_draw_drawable( w->window,
		     get_gc( this, glow_eDrawType_Line, 0), 
		     w->buffer,
		     x0, y0, x0, y0, x1 - x0, y1 - y0);
}

void GlowDrawGtk::get_window_size( GlowWind *wind, int *width, int *height)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  gdk_drawable_get_size( w->window, width, height);
}

void GlowDrawGtk::set_window_size( GlowWind *wind, int width, int height)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( !w->window) return;

  gdk_window_resize( w->window, width, height);
}

static gboolean draw_timer_cb( void *data)
{
  draw_sTimerCb *timer_cb = (draw_sTimerCb *)data;
  GlowDrawGtk *draw_ctx = (GlowDrawGtk *) timer_cb->ctx->gdraw;

  (timer_cb->callback_func)(timer_cb->ctx);

  gdk_display_flush( draw_ctx->display);
  free( timer_cb);
  return FALSE;
}

static gboolean event_timer_cb( void *ctx)
{
//  printf( "Timer callback\n");
  GlowDrawGtk *draw_ctx = (GlowDrawGtk *) ((GlowCtx *)ctx)->gdraw;
  draw_ctx->timer_id = 0;

  draw_ctx->event_handler( last_event);
  return FALSE;
}

static void cancel_event_timer( GlowCtx *ctx)
{
  GlowDrawGtk *draw_ctx  = (GlowDrawGtk *) ctx->gdraw;
  if ( draw_ctx->timer_id) {
    g_source_remove( draw_ctx->timer_id);
    draw_ctx->timer_id = 0;
  }
//  printf( "Timer removed\n");
}

static void event_timer( GlowCtx *ctx, int time_ms)
{
  GlowDrawGtk *draw_ctx = (GlowDrawGtk *) ctx->gdraw;

//  printf( "Add timer\n");
  draw_ctx->timer_id = g_timeout_add( time_ms, event_timer_cb, ctx);

}

void GlowDrawGtk::set_timer( GlowCtx *gctx, int time_ms,
  	void (*callback_func)( GlowCtx *ctx), void **id)
{
  draw_sTimerCb *timer_cb;

  timer_cb = (draw_sTimerCb *) calloc( 1, sizeof(draw_sTimerCb));
  timer_cb->ctx = gctx;
  timer_cb->callback_func = callback_func;

  timer_cb->timer_id = g_timeout_add( time_ms,
				      draw_timer_cb, timer_cb);

  *id = (void *)timer_cb;
}

void GlowDrawGtk::remove_timer( void *id)
{
  g_source_remove( ((draw_sTimerCb *)id)->timer_id);

  free( (char *) id);
}

void GlowDrawGtk::set_cursor( GlowWind *wind, glow_eDrawCursor cursor)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  if ( !w->window) return;

  if ( cursor == glow_eDrawCursor_Normal)
    gdk_window_set_cursor( w->window, NULL);
  else
    gdk_window_set_cursor( w->window, cursors[cursor]);
  gdk_display_flush( display);
}

int GlowDrawGtk::get_text_extent( const char *text, int len,
				  glow_eDrawType gc_type, int idx, glow_eFont font_idx,
				  int *width, int *height, int *descent, double size)
{
  int	text_width, text_ascent, text_descent, text_lbearing, text_rbearing;

  if ( !((GrowCtx *)ctx)->bitmap_fonts)
    return get_text_extent_pango( text, len, gc_type, idx, font_idx, width, height, descent, size);

  if ( font_idx > glow_eFont_Courier)
    font_idx = glow_eFont_Helvetica;

  int font_type = get_font_type( gc_type);
  load_font( font_idx, font_type, idx);

  gdk_text_extents( font[font_idx][font_type][idx], text, len,
	&text_lbearing, &text_rbearing, &text_width, &text_ascent, 
		    &text_descent);
  //  *height = text_ascent + text_descent;  // text_descent is allways zero...
  //  *descent = text_descent;
  text_ascent = int(1.2 * text_ascent);
  *height = int( 1.3 * text_ascent);
  *descent = int( 0.3 * text_ascent);
  *width = text_width;
  return 1;
}

void GlowDrawGtk::copy_area( GlowWind *wind, int x, int y)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  GdkGC *gc;
  if ( ctx->nodraw) return;
  int window_width, window_height;

  if ( &ctx->mw == wind) {
    window_width = ctx->mw.window_width;
    window_height = ctx->mw.window_height;
  }
  else {
    window_width = ctx->navw.window_width;
    window_height = ctx->navw.window_height;
  }

  gc = get_gc( this, glow_eDrawType_Line, 3);
  if ( x >= 0 && y >= 0) {
    gdk_draw_drawable( w->window, gc, w->window, 
	0, 0, x, y, window_width-x, window_height-y);
    if ( !w->double_buffer_on) {
      if ( x)
        gdk_window_clear_area( w->window, 0, 0, 
		x, window_height);
      if ( y)
        gdk_window_clear_area( w->window, x, 0, 
		window_width, y);
    }
    else {
      gdk_draw_drawable( w->buffer, gc, w->buffer,
	0, 0, x, y, window_width-x, window_height-y);
      if ( x)
        gdk_draw_rectangle( w->buffer, 
			    get_gc( this, glow_eDrawType_LineErase, 0), 1,
			    0, 0, x, window_height);
      if ( y)
        gdk_draw_rectangle( w->buffer, 
			    get_gc( this, glow_eDrawType_LineErase, 0), 1,
			    x, 0, window_width, y);
    }
  }
  else if ( x <= 0 && y <= 0)
  {
    gdk_draw_drawable( w->window, gc, w->window,
	-x, -y, 0, 0, window_width+x, window_height+y);
    if ( !w->double_buffer_on) {
      if ( x)
        gdk_window_clear_area( w->window, 
	  window_width+x, 0, window_width, window_height);
      if ( y)
        gdk_window_clear_area( w->window, 
	  0, window_height+y, window_width+x, window_height);
    }
    else {
      gdk_draw_drawable( w->buffer, gc, w->buffer,
	-x, -y, 0, 0, window_width+x, window_height+y);
      if ( x)
        gdk_draw_rectangle( w->buffer, 
			    get_gc( this, glow_eDrawType_LineErase, 0), 1,
			    window_width+x, 0, -x, window_height);
      if ( y)
        gdk_draw_rectangle( w->buffer, 
			    get_gc( this, glow_eDrawType_LineErase, 0), 1,
			    0, window_height+y, window_width+x, -y);
    }
  }
  else if ( x <= 0 && y >= 0)
  {
    gdk_draw_drawable( w->window, gc, w->window,
	-x, 0, 0, y, window_width+x, window_height-y);
    if ( !w->double_buffer_on) {
      if ( x)
        gdk_window_clear_area( w->window, 
	  window_width+x, 0, window_width, window_height);
      if ( y)
        gdk_window_clear_area( w->window, 
	  0, 0, window_width+x, y);
    }
    else {
      gdk_draw_drawable( w->buffer, gc, w->buffer,
	-x, 0, 0, y, window_width+x, window_height-y);
      if ( x)
        gdk_draw_rectangle( w->buffer, 
			     get_gc( this, glow_eDrawType_LineErase, 0), 1,
			     window_width+x, 0, -x, window_height);
      if ( y)
        gdk_draw_rectangle( w->buffer, 
			     get_gc( this, glow_eDrawType_LineErase, 0), 1,
			     0, 0, window_width+x, y);
    }
  }
  else // x > 0 && y < 0
  {
    gdk_draw_drawable( w->window, gc, w->window,
	0, -y, x, 0, window_width-x, window_height+y);
    if ( !w->double_buffer_on) {
      if ( x)
        gdk_window_clear_area( w->window, 
			       0, 0, x, window_height);
      if ( y)
        gdk_window_clear_area( w->window, 
	  x, window_height+y, window_width, window_height);
    }
    else {
      gdk_draw_drawable( w->buffer, gc, w->buffer,
	0, -y, x, 0, window_width-x, window_height+y);
      if ( x)
        gdk_draw_rectangle( w->buffer, 
			    get_gc( this, glow_eDrawType_LineErase, 0), 1,
			    0, 0, x, window_height);
      if ( y)
        gdk_draw_rectangle( w->buffer, 
			    get_gc( this, glow_eDrawType_LineErase, 0), 1,
			    x, window_height+y, window_width - x, -y);
    }
  }
}

void GlowDrawGtk::clear_area( GlowWind *wind, int ll_x, int ur_x, int ll_y, int ur_y)
{

  if ( ctx->nodraw) return;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  gdk_window_clear_area( w->window, ll_x, ll_y, ur_x - ll_x, 
		ur_y - ll_y);
}

void GlowDrawGtk::set_inputfocus( GlowWind *wind)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  gdk_window_focus( w->window, GDK_CURRENT_TIME);
  // gtk_widget_grab_focus( m_wind.toplevel);
}

static int glow_read_color_file( const char *filename, draw_sColor **color_array, 
	int *size)
{
  char		line[80];
  draw_sColor	*color_p;
  ifstream	fp;
  int		nr;
  int		line_cnt;
  float		f_red, f_green, f_blue;

  if ( !check_file( filename))
  {
    return 0;
#if 0
    // No color-file exist, use default values
    double r, g, b;

    printf( "** Using default color palette\n");
    *color_array = (draw_sColor *) calloc( 300, sizeof( draw_sColor));
    *size = 0;
    color_p = *color_array;
    for ( int i = 3; i < 300; i++)
    {
      GlowColor::rgb_color( i, &r, &g, &b);
      color_p->red = int( r * 65535);
      color_p->green = int( g * 65535);
      color_p->blue = int( b * 65535);

      color_p++;
      (*size)++;
    }
#endif
  }
  else
  {
    printf( "** Opening color file %s\n", filename);

    fp.open( filename);
#ifndef OS_VMS
    if ( !fp)
      return GLOW__FILEOPEN;
#endif

    *color_array = (draw_sColor *) calloc( 300, sizeof( draw_sColor));
    *size = 0;
    line_cnt = 0;
    color_p = *color_array;
    while ( *size < 300)
    {
      fp.getline( line, sizeof( line));
      if ( line[0] == 0)
        break;
      line_cnt++;
      if ( line[0] == '!' || line[0] == '#')
        continue;
      nr = sscanf( line, "%f %f %f", &f_red, &f_green, &f_blue);
      if ( nr != 3) 
        printf( "** Syntax error in file %s, line %d", filename, line_cnt);
      else
      {
        color_p->red = int( f_red * 65535);
        color_p->green = int( f_green * 65535);
        color_p->blue = int( f_blue * 65535);
        color_p++;
        (*size)++;
      }
    }
    fp.close();
  }

  return 1;
}

void GlowDrawGtk::set_background( GlowWind *wind, glow_eDrawType drawtype, glow_tPixmap pixmap,
				  glow_tImImage image, int pixmap_width, int pixmap_height)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  GdkGCValues 		xgcv;
  int			i;
  double r, g, b;

  if ( drawtype == glow_eDrawType_LineErase)
    drawtype = glow_eDrawType_Color32;

  GlowColor::rgb_color( (int)drawtype, &r, &g, &b);
  background = glow_allocate_color( this, int(r * 65535), 
				    int(g * 65535), int(b * 65535));
  // gdk_gc_get_values( get_gc( this, drawtype, 0), &xgcv);
  // gdk_colormap_query_color( colormap, xgcv.foreground.pixel, &background);

  // Change erase gcs
  xgcv.foreground = background;
  xgcv.background = background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    gdk_gc_set_values( get_gc( this, glow_eDrawType_LineErase, i), &xgcv,
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
    gdk_gc_set_values( get_gc( this, glow_eDrawType_TextHelveticaErase, i), &xgcv,
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
    gdk_gc_set_values( get_gc( this, glow_eDrawType_TextHelveticaEraseBold, i), &xgcv,
        (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }

  if ( !image) {
    gtk_widget_modify_bg( w->toplevel, GTK_STATE_NORMAL, &background);

    if ( w->buffer)
      buffer_background( w);
  }
  else {
    GdkBitmap *mask;
    // GdkPixmap *gpixmap = gdk_pixmap_foreign_new( pixmap);

    if ( w->background_pixmap)
      g_object_unref( w->background_pixmap);

    gdk_pixbuf_render_pixmap_and_mask_for_colormap( (GdkPixbuf *)image, colormap, &w->background_pixmap,
						    &mask, 0);
    w->background_pixmap_width = pixmap_width;
    w->background_pixmap_height = pixmap_height;
    gdk_window_set_back_pixmap( w->window, w->background_pixmap, FALSE);
    if ( w->buffer)
      buffer_background( w);
  }
}

void GlowDrawGtk::reset_background( GlowWind *wind)
{
  GdkGCValues 		xgcv;

  background = original_background;
  gtk_widget_modify_bg( m_wind.toplevel, GTK_STATE_NORMAL, &background);

  // Change erase gcs
  xgcv.foreground =  background;
  xgcv.background =  background;
  for ( int i = 0; i < DRAW_TYPE_SIZE; i++) {
    gdk_gc_set_values( get_gc( this, glow_eDrawType_LineErase, i), &xgcv,
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
    gdk_gc_set_values( get_gc( this, glow_eDrawType_TextHelveticaErase, i), &xgcv,
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
    gdk_gc_set_values( get_gc( this, glow_eDrawType_TextHelveticaEraseBold, i), &xgcv,
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }
}

void GlowDrawGtk::set_clip( DrawWind *wind, GdkGC *gc)
{
  DrawWindGtk *w = (DrawWindGtk *) wind;
  gdk_gc_set_clip_rectangle( gc, &w->clip_rectangle[w->clip_cnt-1]);
}

void GlowDrawGtk::reset_clip( DrawWind *w, GdkGC *gc)
{
  gdk_gc_set_clip_rectangle( gc, NULL);
}

void GlowDrawGtk::set_image_clip_mask( glow_tPixmap pixmap, int x, int y)
{
  // GdkPixmap *gpixmap = gdk_pixmap_foreign_new( pixmap);
  gdk_gc_set_clip_mask( get_gc( this, glow_eDrawType_Line, 0), 
		(GdkPixmap *)pixmap);
  gdk_gc_set_clip_origin( get_gc( this, glow_eDrawType_Line, 0), x, y);
}

void GlowDrawGtk::reset_image_clip_mask()
{
  gdk_gc_set_clip_mask( get_gc( this, glow_eDrawType_Line, 0), 
		NULL);
  gdk_gc_set_clip_origin( get_gc( this, glow_eDrawType_Line, 0), 0, 0);
}

int GlowDrawGtk::set_clip_rectangle( GlowWind *wind,
		int ll_x, int ll_y, int ur_x, int ur_y)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  if ( w->clip_cnt >= DRAW_CLIP_SIZE) {
    printf("** Draw clip refuced\n");
    return 0;
  }
  int x0, x1, y0, y1;
  if ( w->clip_cnt == 0) {
    x0 = min( ll_x, ur_x);
    x1 = max( ll_x, ur_x);
    y0 = min( ll_y, ur_y);
    y1 = max( ll_y, ur_y);
  }
  else {
    x0 = min( ll_x, ur_x);
    x1 = max( ll_x, ur_x);
    y0 = min( ll_y, ur_y);
    y1 = max( ll_y, ur_y);

    x0 = max( x0, w->clip_rectangle[w->clip_cnt-1].x);
    x1 = min( x1, w->clip_rectangle[w->clip_cnt-1].x +
	      w->clip_rectangle[w->clip_cnt-1].width);
    y0 = max( y0, w->clip_rectangle[w->clip_cnt-1].y);
    y1 = min( y1, w->clip_rectangle[w->clip_cnt-1].y +
	      w->clip_rectangle[w->clip_cnt-1].height);
    if ( x0 > x1)
      x0 = x1;
    if ( y0 > y1)
      y0 = y1;
  }
  w->clip_rectangle[w->clip_cnt].x = x0;
  w->clip_rectangle[w->clip_cnt].y = y0;
  w->clip_rectangle[w->clip_cnt].width = x1 - x0;
  w->clip_rectangle[w->clip_cnt].height = y1 - y0;
  w->clip_cnt++;
  w->clip_on = 1;
  return 1;
}

void GlowDrawGtk::reset_clip_rectangle( GlowWind *wind)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  if ( w->clip_cnt == 0) {
    printf( "** Draw clip mismatch\n");
    return;
  }
  w->clip_cnt--;
  if ( w->clip_cnt == 0)
    w->clip_on = 0;
}

int GlowDrawGtk::clip_level( GlowWind *wind)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  return w->clip_cnt;
}

int GlowDrawGtk::draw_point( GlowWind *wind, int x1, int y1, glow_eDrawType gc_type)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  if ( ctx->nodraw) return 1;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, 0));

  gdk_draw_point( w->window,
	get_gc( this, gc_type, 0), 
	x1, y1);
  if ( w->double_buffer_on)
    gdk_draw_point( w->buffer,
	get_gc( this, gc_type, 0), 
	x1, y1);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, 0));
  return 1;
}

int GlowDrawGtk::draw_points( GlowWind *wind, glow_sPointX *points, int point_num, 
	glow_eDrawType gc_type)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, 0));

  GdkPoint* gpoints = points_to_gdk_points( points, point_num);
  if ( !w->draw_buffer_only)
    gdk_draw_points( w->window,
		     get_gc( this, gc_type, 0), 
		     gpoints, point_num);
  if ( w->double_buffer_on)
    gdk_draw_points( w->buffer,
		     get_gc( this, gc_type, 0), 
		     gpoints, point_num);
  free( gpoints);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, 0));
  return 1;
}


void GlowDrawGtk::set_click_sensitivity( GlowWind *wind, int value)
{
  click_sensitivity = value;
}

void GlowDrawGtk::draw_background( GlowWind *wind, int x, int y, int w, int h)
{
  DrawWindGtk *ww = (DrawWindGtk *) wind->window;
  gdk_window_clear_area( ww->window, x, y, w, h);
}

int GlowDrawGtk::create_buffer( GlowWind *wind)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;
  int window_width, window_height;

  if ( !w->is_nav) {
    window_width = ctx->mw.window_width;
    window_height = ctx->mw.window_height;
  }
  else {
    window_width = ctx->navw.window_width;
    window_height = ctx->navw.window_height;
  }

  if ( !w->double_buffer_on || !window_width)
    return 0;

  if ( window_width == w->buffer_width &&
       window_height == w->buffer_height)
    return 0;


  if ( w->buffer)
    g_object_unref( w->buffer);

  w->buffer = gdk_pixmap_new( w->window,
	      window_width, window_height, -1);
  w->buffer_width = window_width;
  w->buffer_height = window_height;

  buffer_background( w);
  return 1;
}

void GlowDrawGtk::delete_buffer( GlowWind *wind)
{
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( w->buffer)
    g_object_unref( w->buffer);
  w->buffer = 0;
  w->buffer_width = 0;
  w->buffer_height = 0;
}

void GlowDrawGtk::buffer_background( DrawWind *wind)
{
  DrawWindGtk *w = (DrawWindGtk *) wind;
  int window_width, window_height, subwindow_x, subwindow_y;

  if ( ctx->mw.window == wind) {
    window_width = ctx->mw.window_width;
    window_height = ctx->mw.window_height;
    subwindow_x = ctx->mw.subwindow_x;
    subwindow_y = ctx->mw.subwindow_y;
  }
  else {
    window_width = ctx->navw.window_width;
    window_height = ctx->navw.window_height;
    subwindow_x = ctx->navw.subwindow_x;
    subwindow_y = ctx->navw.subwindow_y;
  }

  if ( !w->double_buffer_on || !window_width)
    return;

  if ( w->background_pixmap) {

    if ( w->clip_on)
      set_clip( w, get_gc( this, glow_eDrawType_Line, 0));

    if ( !((GrowCtx *)ctx)->background_tiled)
      gdk_draw_drawable( w->background_pixmap,
	get_gc( this, glow_eDrawType_Line, 0), w->buffer,  
	0, 0, 0, 0, w->buffer_width, w->buffer_height);
    else {
      int i, j;
      for ( i = 0; 
            i <= w->buffer_width / w->background_pixmap_width;
            i++)
	for ( j = 0;
              j <= w->buffer_height / w->background_pixmap_height;
              j++)
          gdk_draw_drawable( w->background_pixmap,
			     get_gc( this, glow_eDrawType_Line, 0), w->buffer, 
			     0, 0, 
			     i * w->background_pixmap_width, 
			     j * w->background_pixmap_height,
			     w->background_pixmap_width, 
			     w->background_pixmap_height);
    }  
    if ( w->clip_on)
      reset_clip( w, get_gc( this, glow_eDrawType_Line, 0));
  }
  else {
    if ( w->clip_on)
      set_clip( w, get_gc( this, glow_eDrawType_LineErase, 0));

    gdk_draw_rectangle( w->buffer,
			get_gc( this, glow_eDrawType_LineErase, 0), 1,
			subwindow_x, subwindow_y, window_width, window_height);
    if ( w->clip_on)
      reset_clip( w, get_gc( this, glow_eDrawType_LineErase, 0));

  }
}

int GlowDrawGtk::print( char *filename, double x0, double x1, int end)
{

#define ps_cPageHeight 820
#define ps_cPageWidth 535
#define ps_cLeftMargin 100
#define ps_cTopMargin 100

  DrawWindGtk *w = &m_wind;
  int width, height;
  unsigned char *rgb;
  int i, j, k;
  int grey;
  int red, blue, green;
  double scalex = 0.71;
  double scaley = 0.78;
  double x, y;
  bool colorimage = true;
  static DrawPs *ps = 0;
  bool new_file = false;
  int rowstride;
  int n_channels;
  unsigned char *rgb_row;

  int window_width = ctx->mw.window_width;
  int window_height = ctx->mw.window_height;
	
  x = ps_cLeftMargin;
  y = ps_cPageHeight - ps_cTopMargin;

  GdkPixbuf *image;

  if ( w->double_buffer_on)
    image = gdk_pixbuf_get_from_drawable( NULL, w->buffer, 0,
					    0, 0, 0, 0, w->buffer_width,
					    w->buffer_height);
  else
    image = gdk_pixbuf_get_from_drawable( NULL, w->window, 0,
					    0, 0, 0, 0, window_width,
					    window_height);
  if ( !image)
    return 0;

  if ( !ps) {
    ps = new DrawPs( filename);
    new_file = true;
    ps->y = y;
  }
  else
    y = ps->y;

  width = gdk_pixbuf_get_width( image);
  height = gdk_pixbuf_get_height( image);

  if ( x0 != 0 || x1 != 0) {
    double total_width = width / (x1 - x0);

    if ( total_width * scalex  > ps_cPageWidth - ps_cLeftMargin) {
      x = ps_cPageWidth - total_width * scalex;
      if ( x < 50) {
	double scale_factor = (ps_cPageWidth - 50) / (total_width * scalex);
	x = 50;
	scalex = scalex * scale_factor;
	scaley = scaley * scale_factor;
      }
    }
    x += scalex * total_width * x0;
  }
  else if ( width * scalex  > ps_cPageWidth - ps_cLeftMargin) {
    x = ps_cPageWidth - width * scalex;
    if ( x < 50) {
      double scale_factor = (ps_cPageWidth - 50) / (width * scalex);
      x = 50;
      scalex = scalex * scale_factor;
      scaley = scaley * scale_factor;
    }
  }
  if ( (x0 == 0 && x1 == 0) || x1 == 1.0)
    ps->y -= scaley * height;

  if ( new_file) {
    ps->fp <<
"%!PS-Adobe-2.0 EPSF-1.2" << endl <<
"%%Creator: Proview   $Id: glow_draw_gtk.cpp,v 1.18 2008-12-01 16:32:40 claes Exp $ Glow" << endl <<
"%%EndComments" << endl << endl;
  }
  else
    ps->fp <<
"restore" << endl;

  ps->fp <<
"1.000000 1.000000 scale" << endl <<
"save" << endl <<
scalex * width << " " << scaley * height << " scale" << endl <<
"/oneline " << width << " string def" << endl <<
"/drawimage {" << endl <<
" " << width << " " << height << " 8 [" << width << " 0 0 -" << height << " 0 " << height << "]" << endl <<
" { currentfile oneline readhexstring pop }" << endl;
  if ( colorimage) {
    ps->fp <<
"false 3" << endl <<
"colorimage" << endl;
  }
  else
    ps->fp <<
"image" << endl;

  ps->fp <<
"} def" << endl <<
x/scalex/width << " " << (y - height*scaley)/scaley/height << " translate" << endl <<
"drawimage" << endl;

  ps->fp.flags( (ps->fp.flags() & ~ios_base::dec) | ios_base::hex | ios_base::uppercase);
  ps->fp.fill('0');
  rgb = gdk_pixbuf_get_pixels( image);
  rowstride = gdk_pixbuf_get_rowstride( image);
  n_channels = gdk_pixbuf_get_n_channels( image);
  j = 0;
  rgb_row = rgb;
  for ( k = 0; k < height; k++) {
    rgb = rgb_row;
    for ( i = 0; i < width; i++) {
      if ( !colorimage) {
	if ( n_channels >= 4 && !(rgb+3))
	  grey = 255;
	else
	  grey = (int) ((0.0 + *rgb + *(rgb+1) + *(rgb+2)) / 3 + 0.5);

	rgb += n_channels;
	ps->fp.width(2);
	ps->fp << grey;
	if ( ++j >= 40) {
	  j = 0;
	  ps->fp << endl;
	}
      }
      else {
	if ( n_channels >= 4 && !(rgb+3))
	  red = blue = green = 255;
	else {
	  red = *rgb;
	  green = *(rgb+1);
	  blue = *(rgb+2);
	}
	rgb += n_channels;
	ps->fp.width(2);
	ps->fp << red;
	ps->fp.width(2);
	ps->fp << green;
	ps->fp.width(2);
	ps->fp << blue;
	if ( ++j >= 20) {
	  j = 0;
	  ps->fp << endl;
	}
      }
    }
    rgb_row += rowstride;
  }

  if ( end) {
    ps->fp << endl <<
"restore" << endl << 
"showpage" << endl;
    delete ps;
    ps = 0;
  }
  else {
    ps->fp.flags( ((ps->fp.flags() & ~ios_base::hex) & ~ios_base::uppercase) | ios_base::dec);
  }
  gdk_pixbuf_unref( image);

  return 1;
}

GdkPoint *GlowDrawGtk::points_to_gdk_points( glow_sPointX *points, int point_cnt)
{
  GdkPoint *gpoints = (GdkPoint *)malloc( point_cnt * sizeof(GdkPoint));
  for ( int i = 0; i < point_cnt; i++) {
    gpoints[i].x = points[i].x;
    gpoints[i].y = points[i].y;
  }
  return gpoints;
}

void GlowDrawGtk::load_font( glow_eFont font_idx, int font_type, int idx)
{
  if ( !font[font_idx][font_type][idx])
    font[font_idx][font_type][idx] = 
      gdk_font_load( font_names[font_idx][font_type][idx]);
}

int GlowDrawGtk::get_font_type( int gc_type)
{
  int font_type;

  switch( gc_type) {
  case glow_eDrawType_TextHelveticaBold:
  case glow_eDrawType_TextHelveticaEraseBold:
    font_type = glow_eDrawFont_HelveticaBold;
    break;
  default:
    font_type = glow_eDrawFont_Helvetica;
    break;
  }

  return font_type;
}

// Image functions
int GlowDrawGtk::image_get_width( glow_tImImage image)
{
  return gdk_pixbuf_get_width( (GdkPixbuf *)image);
}

int GlowDrawGtk::image_get_height( glow_tImImage image)
{
  return gdk_pixbuf_get_height( (GdkPixbuf *)image);
}

int GlowDrawGtk::image_get_rowstride( glow_tImImage image)
{
  return gdk_pixbuf_get_rowstride( (GdkPixbuf *)image);
}

unsigned char *GlowDrawGtk::image_get_data( glow_tImImage image)
{
  return (unsigned char *) gdk_pixbuf_get_pixels( (GdkPixbuf *)image);
}

void GlowDrawGtk::image_rotate( glow_tImImage *image, int to_rotation, int from_rotation) 
{
#if GDK_PIXBUF_MAJOR == 2 && GDK_PIXBUF_MINOR < 8
  return;
#else
  int grot;
  int drot = to_rotation - from_rotation;
  drot = int( (float(drot) / 360 - floor( float(drot) / 360)) * 360);
  switch ( drot) {
  case 90:
    grot = GDK_PIXBUF_ROTATE_CLOCKWISE;
    break;
  case 180:
    grot = GDK_PIXBUF_ROTATE_UPSIDEDOWN;
    break;
  case 270:
    grot = GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE;
    break;
  default:
    return;
  }
  GdkPixbuf *im = gdk_pixbuf_rotate_simple( (GdkPixbuf *)*image, (GdkPixbufRotation) grot);
  gdk_pixbuf_unref( (GdkPixbuf *)*image);
  *image = (glow_tImImage) im;
#endif
}

void GlowDrawGtk::image_flip_vertical( glow_tImImage *image) 
{
#if GDK_PIXBUF_MAJOR == 2 && GDK_PIXBUF_MINOR < 8
  return;
#else
  GdkPixbuf *im = gdk_pixbuf_flip( (GdkPixbuf *)*image, TRUE);
  gdk_pixbuf_unref( (GdkPixbuf *)*image);
  *image = (glow_tImImage) im;
#endif
}

void GlowDrawGtk::image_flip_horizontal( glow_tImImage *image) 
{
#if GDK_PIXBUF_MAJOR == 2 && GDK_PIXBUF_MINOR < 8
  return;
#else
  GdkPixbuf *im = gdk_pixbuf_flip( (GdkPixbuf *)*image, FALSE);
  gdk_pixbuf_unref( (GdkPixbuf *)*image);
  *image = (glow_tImImage) im;
#endif
}

void GlowDrawGtk::image_scale( int width, int height, glow_tImImage orig_im, glow_tImImage *im, 
			       glow_tPixmap *im_pixmap, glow_tPixmap *im_mask)
{
  if ( width == gdk_pixbuf_get_width((GdkPixbuf *)*im) &&
       height == gdk_pixbuf_get_height((GdkPixbuf *)*im))
    return;

  if ( !orig_im) {
    // Scale from im
    if ( !*im)
      return;

    GdkPixbuf *im_old = (GdkPixbuf *)*im;
    *im = gdk_pixbuf_scale_simple( (GdkPixbuf *)*im, width, height, GDK_INTERP_NEAREST);
    gdk_pixbuf_unref( (GdkPixbuf *)im_old);
  }
  else {
    // Scale from orig_im
    if ( *im)
      gdk_pixbuf_unref( (GdkPixbuf *)*im);
    *im = gdk_pixbuf_scale_simple( (GdkPixbuf *)orig_im, width, height, GDK_INTERP_NEAREST);
  }
}

int GlowDrawGtk::image_load( char *imagefile, 
			     glow_tImImage *orig_im, glow_tImImage *im)
{
  if ( im && *im)
    gdk_pixbuf_unref( (GdkPixbuf *)*im);
  if ( *orig_im)
    gdk_pixbuf_unref( (GdkPixbuf *)*orig_im);
  *orig_im = (glow_tImImage *) gdk_pixbuf_new_from_file( imagefile, 0);

  if ( im)
    *im = (glow_tImImage *) gdk_pixbuf_copy( (GdkPixbuf *)*orig_im);
  return 1;
}

int GlowDrawGtk::image_render( int width, int height, 
			       glow_tImImage orig_im, glow_tImImage *im, 
			       glow_tPixmap *im_pixmap, glow_tPixmap *im_mask)
{
#if 0
  if ( width == gdk_pixbuf_get_width((GdkPixbuf *)*im) &&
       height == gdk_pixbuf_get_height((GdkPixbuf *)*im))
    return 1;

  image_scale( width, height, orig_im, im, 0, 0);
#endif
  return 1;
}

void GlowDrawGtk::image_free( glow_tImImage image) 
{
  gdk_pixbuf_unref( (GdkPixbuf *)image);
}

void GlowDrawGtk::pixmap_free( glow_tPixmap pixmap) 
{
}

void GlowDrawGtk::image_pixel_iter( glow_tImImage orig_image, glow_tImImage *image, 
				     void (* pixel_cb)(void *, unsigned char *), void *userdata)
{
  unsigned char *rgb, *rgb_row;
  int 		rgb_height;
  int 		rgb_width;
  int		rowstride;
  int		n_channels;

  
  if ( orig_image) {
    if ( image)
      gdk_pixbuf_unref( (GdkPixbuf *)*image);
    *image = (glow_tImImage *) gdk_pixbuf_copy( (GdkPixbuf *)orig_image);
  }
  else if ( !*image)
    return;
    
  rgb = gdk_pixbuf_get_pixels( (GdkPixbuf *)*image);
  rgb_height = gdk_pixbuf_get_height( (GdkPixbuf *)*image);
  rgb_width = gdk_pixbuf_get_width( (GdkPixbuf *)*image);
  rowstride = gdk_pixbuf_get_rowstride( (GdkPixbuf *)*image);
  n_channels = gdk_pixbuf_get_n_channels( (GdkPixbuf *)*image);

  rgb_row = rgb;
  for ( int j = 0; j < rgb_height; j++) {
    rgb = rgb_row;
    for ( int i = 0; i < rgb_width; i++) {
      if ( n_channels >= 4 && *(rgb+3))
	(pixel_cb) ( userdata, rgb);
      rgb += n_channels;
    }
    rgb_row += rowstride;
  }
}


void GlowDrawGtk::set_cairo_clip( DrawWind *wind, cairo_t *cr)
{
  DrawWindGtk *w = (DrawWindGtk *) wind;
  cairo_rectangle( cr, w->clip_rectangle[w->clip_cnt-1].x, w->clip_rectangle[w->clip_cnt-1].y,
		   w->clip_rectangle[w->clip_cnt-1].width, w->clip_rectangle[w->clip_cnt-1].height);
  cairo_clip( cr);
}

void GlowDrawGtk::reset_cairo_clip( DrawWind *wind, cairo_t *cr)
{
  cairo_reset_clip( cr);
}

int GlowDrawGtk::gradient_create_pattern( int x, int y, int w, int h, 
					  glow_eDrawType d0, glow_eDrawType d1, 
					  glow_eDrawType d2, glow_eGradient gradient,
					  cairo_pattern_t **pat)
{
  double r1, g1, b1, r2, g2, b2;
  int a = 15;

  GlowColor::rgb_color( d1, &r1, &g1, &b1);
  GlowColor::rgb_color( d2, &r2, &g2, &b2);

  switch ( gradient) {
  case glow_eGradient_HorizontalUp:
    *pat = cairo_pattern_create_linear( x, y, x, y+h);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    break;
  case glow_eGradient_HorizontalDown:
    *pat = cairo_pattern_create_linear( x, y, x, y+h);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r2, g2, b2);
    break;
  case glow_eGradient_HorizontalTube1:
    *pat = cairo_pattern_create_linear( x, y, x, y+h);
    cairo_pattern_add_color_stop_rgb( *pat, 0, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.5, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 1, r1, g1, b1);
    break;
  case glow_eGradient_HorizontalTube2: {
    double r0, g0, b0;

    *pat = cairo_pattern_create_linear( x, y, x, y+h);
    GlowColor::rgb_color( d0, &r0, &g0, &b0);
    cairo_pattern_add_color_stop_rgb( *pat, 0, r0, g0, b0);
    cairo_pattern_add_color_stop_rgb( *pat, 0.3, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, .95, r1, g1, b1);
    break;
  }
  case glow_eGradient_VerticalRight:
    *pat = cairo_pattern_create_linear( x, y, x+w, y);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r2, g2, b2);
    break;
  case glow_eGradient_VerticalLeft:
    *pat = cairo_pattern_create_linear( x, y, x+w, y);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    break;
  case glow_eGradient_VerticalTube1:
    *pat = cairo_pattern_create_linear( x, y, x+w, y);
    cairo_pattern_add_color_stop_rgb( *pat, 0, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.5, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 1, r1, g1, b1);
    break;
  case glow_eGradient_VerticalTube2: {
    double r0, g0, b0;

    GlowColor::rgb_color( d0, &r0, &g0, &b0);
    *pat = cairo_pattern_create_linear( x, y, x+w, y);
    cairo_pattern_add_color_stop_rgb( *pat, 0, r0, g0, b0);
    cairo_pattern_add_color_stop_rgb( *pat, 0.3, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 0.95, r1, g1, b1);
    break;
  }
  case glow_eGradient_DiagonalUpperLeft: {
    double x0,x1,y0,y1;

    if ( w > h) {
      x0 = x+ w/2-h/2;
      y0 = y - (w-h)/a;
      x1 = x+w/2+h/2;
      y1 = y + h + (w-h)/a;
    }
    else {
      y0 = y+h/2-w/2;
      x0 = x - (h-w)/a;
      y1 = y+h/2+w/2;
      x1 = x + w + (h-w)/a;
    }
    *pat = cairo_pattern_create_linear(x0, y0, x1, y1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    break;
  }
  case glow_eGradient_DiagonalLowerLeft: {
    double x0,x1,y0,y1;
    // *pat = cairo_pattern_create_linear( x, y, x+w, y+h);
    if ( w > h) {
      x0 = x+w/2-h/2;
      y0 = y + h + (w-h)/a;
      x1 = x+w/2+h/2;
      y1 = y - (w-h)/a;
    }
    else {
      y1 = y+h/2-w/2;
      x1 = x + w + (h-w)/a;
      y0 = y+h/2+w/2;
      x0 = x - (h-w)/a;
    }
    *pat = cairo_pattern_create_linear(x0, y0, x1, y1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    break;
  }
  case glow_eGradient_DiagonalUpperRight: {
    double x0,x1,y0,y1;
    // *pat = cairo_pattern_create_linear( x, y, x+w, y+h);
    if ( w > h) {
      x0 = x+w/2-h/2;
      y0 = y + h + (w-h)/a;
      x1 = x+w/2+h/2;
      y1 = y - (w-h)/a;
    }
    else {
      y1 = y+h/2-w/2;
      x1 = x + w + (h-w)/a;
      y0 = y+h/2+w/2;
      x0 = x - (h-w)/a;
    }
    *pat = cairo_pattern_create_linear(x0, y0, x1, y1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r2, g2, b2);
    break;
  }
  case glow_eGradient_DiagonalLowerRight: {
    double x0,x1,y0,y1;

    if ( w > h) {
      x0 = x+w/2-h/2;
      y0 = y - (w-h)/a;
      x1 = x+w/2+h/2;
      y1 = y + h + (w-h)/a;
    }
    else {
      y0 = y+h/2-w/2;
      x0 = x - (h-w)/a;
      y1 = y+h/2+w/2;
      x1 = x + w + (h-w)/a;
    }
    *pat = cairo_pattern_create_linear(x0, y0, x1, y1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r2, g2, b2);
    break;
  }
  case glow_eGradient_DiagonalUpTube: {
    double x0,x1,y0,y1;

    if ( w > h) {
      x0 = x+w/2-h/2;
      y0 = y - (w-h)/a;
      x1 = x+w/2+h/2;
      y1 = y + h + (w-h)/a;
    }
    else {
      y0 = y+h/2-w/2;
      x0 = x - (h-w)/a;
      y1 = y+h/2+w/2;
      x1 = x + w + (h-w)/a;
    }
    *pat = cairo_pattern_create_linear(x0, y0, x1, y1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.5, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    break;
  }
  case glow_eGradient_DiagonalDownTube: {
    double x0,x1,y0,y1;
    // *pat = cairo_pattern_create_linear( x, y, x+w, y+h);
    if ( w > h) {
      x0 = x+w/2-h/2;
      y0 = y + h + (w-h)/a;
      x1 = x+w/2+h/2;
      y1 = y - (w-h)/a;
    }
    else {
      y1 = y+h/2-w/2;
      x1 = x + w + (h-w)/a;
      y0 = y+h/2+w/2;
      x0 = x - (h-w)/a;
    }
    *pat = cairo_pattern_create_linear(x0, y0, x1, y1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.5, r2, g2, b2);
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    break;
  }
  case glow_eGradient_Globe:
    *pat = cairo_pattern_create_radial( x + w/3, y + h/3, 0, x + w/3, y + h/3, sqrt(w*w/4+h*h/4));
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    break;
  case glow_eGradient_RadialCenter:
    *pat = cairo_pattern_create_radial( x + w/2, y + h/2, 0, x + w/2, y + h/2, sqrt(w*w/4+h*h/4));
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    break;
  case glow_eGradient_RadialUpperLeft:
    *pat = cairo_pattern_create_radial( x, y, 0, x, y, sqrt(w*w+h*h));
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    break;
  case glow_eGradient_RadialLowerLeft:
    *pat = cairo_pattern_create_radial( x, y+h, 0, x, y+h, sqrt(w*w+h*h));
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    break;
  case glow_eGradient_RadialUpperRight:
    *pat = cairo_pattern_create_radial( x+w, y, 0, x+w, y, sqrt(w*w+h*h));
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    break;
  case glow_eGradient_RadialLowerRight:
    *pat = cairo_pattern_create_radial( x+w, y+h, 0, x+w, y+h, sqrt(w*w+h*h));
    cairo_pattern_add_color_stop_rgb( *pat, 0.9, r1, g1, b1);
    cairo_pattern_add_color_stop_rgb( *pat, 0.1, r2, g2, b2);
    break;
  default: 
    return 0;      
  }
  return 1;
}

glow_eGradient GlowDrawGtk::gradient_rotate( double rot, glow_eGradient gradient)
{
  glow_eGradient g;
  double rotation = (rot / 360 - floor( rot / 360)) * 360;

  if ( 45 >= rotation || rotation > 315) {
      g = gradient;
  }
  else if ( 45 < rotation && rotation <= 135) {
    switch ( gradient) {
    case glow_eGradient_HorizontalUp: 		g = glow_eGradient_VerticalRight; break;
    case glow_eGradient_HorizontalDown:		g = glow_eGradient_VerticalLeft; break;
    case glow_eGradient_HorizontalTube1:	g = glow_eGradient_VerticalTube1; break;
    case glow_eGradient_HorizontalTube2: 	g = glow_eGradient_VerticalTube2; break;
    case glow_eGradient_VerticalLeft: 		g = glow_eGradient_HorizontalUp; break;
    case glow_eGradient_VerticalRight: 		g = glow_eGradient_HorizontalDown; break;
    case glow_eGradient_VerticalTube1: 		g = glow_eGradient_HorizontalTube1; break;
    case glow_eGradient_VerticalTube2: 		g = glow_eGradient_HorizontalTube2; break;
    case glow_eGradient_DiagonalUpperLeft: 	g = glow_eGradient_DiagonalUpperRight; break;
    case glow_eGradient_DiagonalLowerLeft: 	g = glow_eGradient_DiagonalUpperLeft; break;
    case glow_eGradient_DiagonalUpperRight: 	g = glow_eGradient_DiagonalLowerRight; break;
    case glow_eGradient_DiagonalLowerRight: 	g = glow_eGradient_DiagonalLowerLeft; break;
    case glow_eGradient_DiagonalUpTube:        	g = glow_eGradient_DiagonalDownTube; break;
    case glow_eGradient_DiagonalDownTube:      	g = glow_eGradient_DiagonalUpTube; break;
    case glow_eGradient_RadialUpperLeft: 	g = glow_eGradient_RadialUpperRight; break;
    case glow_eGradient_RadialLowerLeft: 	g = glow_eGradient_RadialUpperLeft; break;
    case glow_eGradient_RadialUpperRight: 	g = glow_eGradient_RadialLowerRight; break;
    case glow_eGradient_RadialLowerRight: 	g = glow_eGradient_RadialLowerLeft; break;
    default: g = gradient;
    }
  }
  else if ( 135 < rotation && rotation <= 225) {
    switch ( gradient) {
    case glow_eGradient_HorizontalUp: 		g = glow_eGradient_HorizontalDown; break;
    case glow_eGradient_HorizontalDown:		g = glow_eGradient_HorizontalUp; break;
    case glow_eGradient_VerticalLeft: 		g = glow_eGradient_VerticalRight; break;
    case glow_eGradient_VerticalRight: 		g = glow_eGradient_VerticalLeft; break;
    case glow_eGradient_DiagonalUpperLeft: 	g = glow_eGradient_DiagonalLowerRight; break;
    case glow_eGradient_DiagonalLowerLeft: 	g = glow_eGradient_DiagonalUpperRight; break;
    case glow_eGradient_DiagonalUpperRight: 	g = glow_eGradient_DiagonalLowerLeft; break;
    case glow_eGradient_DiagonalLowerRight: 	g = glow_eGradient_DiagonalUpperLeft; break;
    case glow_eGradient_RadialUpperLeft: 	g = glow_eGradient_RadialLowerRight; break;
    case glow_eGradient_RadialLowerLeft: 	g = glow_eGradient_RadialUpperRight; break;
    case glow_eGradient_RadialUpperRight: 	g = glow_eGradient_RadialLowerLeft; break;
    case glow_eGradient_RadialLowerRight: 	g = glow_eGradient_RadialUpperLeft; break;
    default: g = gradient;
    }
  }
  else { // if ( 225 < rotation && rotation <= 315)
    switch ( gradient) {
    case glow_eGradient_HorizontalUp: 		g = glow_eGradient_VerticalLeft; break;
    case glow_eGradient_HorizontalDown:		g = glow_eGradient_VerticalRight; break;
    case glow_eGradient_HorizontalTube1:	g = glow_eGradient_VerticalTube1; break;
    case glow_eGradient_HorizontalTube2: 	g = glow_eGradient_VerticalTube2; break;
    case glow_eGradient_VerticalLeft: 		g = glow_eGradient_HorizontalDown; break;
    case glow_eGradient_VerticalRight: 		g = glow_eGradient_HorizontalUp; break;
    case glow_eGradient_VerticalTube1: 		g = glow_eGradient_HorizontalTube1; break;
    case glow_eGradient_VerticalTube2: 		g = glow_eGradient_HorizontalTube2; break;
    case glow_eGradient_DiagonalUpperLeft: 	g = glow_eGradient_DiagonalLowerLeft; break;
    case glow_eGradient_DiagonalLowerLeft: 	g = glow_eGradient_DiagonalLowerRight; break;
    case glow_eGradient_DiagonalUpperRight: 	g = glow_eGradient_DiagonalUpperLeft; break;
    case glow_eGradient_DiagonalLowerRight: 	g = glow_eGradient_DiagonalUpperRight; break;
    case glow_eGradient_DiagonalUpTube:        	g = glow_eGradient_DiagonalDownTube; break;
    case glow_eGradient_DiagonalDownTube:      	g = glow_eGradient_DiagonalUpTube; break;
    case glow_eGradient_RadialUpperLeft: 	g = glow_eGradient_RadialLowerLeft; break;
    case glow_eGradient_RadialLowerLeft: 	g = glow_eGradient_RadialLowerRight; break;
    case glow_eGradient_RadialUpperRight: 	g = glow_eGradient_RadialUpperLeft; break;
    case glow_eGradient_RadialLowerRight: 	g = glow_eGradient_RadialUpperRight; break;
    default: g = gradient;
    }
  }
  return g;
}

int GlowDrawGtk::gradient_fill_rect( GlowWind *wind, int x, int y, int w, int h, 
	glow_eDrawType d0, glow_eDrawType d1, glow_eDrawType d2, glow_eGradient gradient)
{
  DrawWindGtk *ww = (DrawWindGtk *) wind->window;
  if ( ctx->nodraw) return 1;

  cairo_t *cr;
  for ( int i = 0; i < 2; i++) {
    if ( i == 0) {
      if ( !ww->draw_buffer_only)
	cr = gdk_cairo_create( ww->window);
      else
	continue;
    }
    else if ( i == 1) {
      if ( ww->double_buffer_on)
	cr = gdk_cairo_create( ww->buffer);
      else
	continue;
    }

    if ( ww->clip_on)
      set_cairo_clip( ww, cr);

    cairo_pattern_t *pat;
    if ( !gradient_create_pattern( x, y, w, h, d0, d1, d2, gradient, &pat))
      return 0;
    cairo_rectangle( cr, x, y, w, h);
    cairo_set_source( cr, pat);
    cairo_fill(cr);

    if ( ww->clip_on)
      reset_cairo_clip( ww, cr);
    cairo_destroy(cr);
  }
  return 1;
}

int GlowDrawGtk::gradient_fill_rectrounded( GlowWind *wind, int x, int y, int w, int h, 
					    int roundamount, glow_eDrawType d0, 
					    glow_eDrawType d1, glow_eDrawType d2, 
					    glow_eGradient gradient)
{
  DrawWindGtk *ww = (DrawWindGtk *) wind->window;
  if ( ctx->nodraw) return 1;

  cairo_t *cr;
  for ( int i = 0; i < 2; i++) {
    if ( i == 0) {
      if ( !ww->draw_buffer_only)
	cr = gdk_cairo_create( ww->window);
      else
	continue;
    }
    else if ( i == 1) {
      if ( ww->double_buffer_on)
	cr = gdk_cairo_create( ww->buffer);
      else
	continue;
    }

    if ( ww->clip_on)
      set_cairo_clip( ww, cr);

    cairo_pattern_t *pat;
    if ( !gradient_create_pattern( x, y, w, h, d0, d1, d2, gradient, &pat))
      return 0;

    if ( roundamount >= 0) {
      cairo_move_to( cr, x + roundamount, y);
      cairo_line_to( cr, x + w - roundamount, y);
      cairo_arc( cr, x + w - roundamount, y + roundamount, roundamount, -M_PI/2, 0);
      cairo_line_to( cr, x + w, y + h - roundamount);
      cairo_arc( cr, x + w - roundamount, y + h - roundamount, roundamount, 0, M_PI/2);
      cairo_line_to( cr, x + roundamount, y + h);
      cairo_arc( cr, x + roundamount, y + h - roundamount, roundamount, M_PI/2, M_PI);
      cairo_line_to( cr, x, y + roundamount);
      cairo_arc( cr, x + roundamount, y + roundamount, roundamount, M_PI, M_PI * 3 / 2);
    }
    else
      cairo_rectangle( cr, x, y, w, h);

    cairo_set_source( cr, pat);
    cairo_fill(cr);

    if ( ww->clip_on)
      reset_cairo_clip( ww, cr);
    cairo_destroy(cr);
  }
  return 1;
}

int GlowDrawGtk::gradient_fill_arc( GlowWind *wind, int x, int y, int w, int h, 
	int angle1, int angle2, glow_eDrawType d0, glow_eDrawType d1, glow_eDrawType d2, 
	glow_eGradient gradient)
{
  DrawWindGtk *ww = (DrawWindGtk *) wind->window;
  if ( ctx->nodraw) return 1;

  if ( angle1 >= 360)
    angle1 = angle1 - angle1 / 360 * 360;
  else if ( angle1 < 0)
    angle1 = angle1 + ( -angle1 / 360 + 1) * 360;

  cairo_t *cr;
  for ( int i = 0; i < 2; i++) {
    if ( i == 0) {
      if ( !ww->draw_buffer_only)
	cr = gdk_cairo_create( ww->window);
      else
	continue;
    }
    else if ( i == 1) {
      if ( ww->double_buffer_on)
	cr = gdk_cairo_create( ww->buffer);
      else
	continue;
    }

    if ( ww->clip_on)
      set_cairo_clip( ww, cr);

    cairo_pattern_t *pat;
    if ( !gradient_create_pattern( x, y, w, h, d0, d1, d2, gradient, &pat))
      return 0;

      
    cairo_save(cr);
    cairo_translate( cr, double(x) + double(w)/2, double(y) + double(h)/2);      
    cairo_scale( cr, double(w)/2, double(h)/2);
    if ( !(angle2 == 360 || angle2 == 180))
      cairo_move_to( cr, 0, 0);
    cairo_arc( cr, 0, 0, 1,-double(angle1 + angle2) / 180 * M_PI,  -double(angle1) / 180 * M_PI);
    if ( !(angle2 == 360 || angle2 == 180))
      cairo_move_to( cr, 0, 0);
    cairo_restore(cr);
    cairo_set_source( cr, pat);
    cairo_fill(cr);

    if ( ww->clip_on)
      reset_cairo_clip( ww, cr);
    cairo_destroy(cr);
  }
  return 1;
}

int GlowDrawGtk::gradient_fill_polyline( GlowWind *wind, glow_sPointX *points, int point_cnt,
	glow_eDrawType d0, glow_eDrawType d1, glow_eDrawType d2, glow_eGradient gradient)
{
  DrawWindGtk *ww = (DrawWindGtk *) wind->window;
  if ( ctx->nodraw) return 1;

  cairo_t *cr;
  double x0, y0, x1, y1;
  for ( int i = 0; i < 2; i++) {
    if ( i == 0) {
      if ( !ww->draw_buffer_only)
	cr = gdk_cairo_create( ww->window);
      else
	continue;
    }
    else if ( i == 1) {
      if ( ww->double_buffer_on)
	cr = gdk_cairo_create( ww->buffer);
      else
	continue;
    }

    if ( ww->clip_on)
      set_cairo_clip( ww, cr);

    x0 = y0 = 1e37;
    x1 = y1 = 1e-37;
    for ( int j = 0; j < point_cnt; j++) {
      if ( points[j].x < x0)
	x0 = points[j].x;
      if ( points[j].y < y0)
	y0 = points[j].y;
      if ( points[j].x > x1)
	x1 = points[j].x;
      if ( points[j].y > y1)
	y1 = points[j].y;

      if ( j == 0)
	cairo_move_to( cr, points[j].x, points[j].y);
      else
	cairo_line_to( cr, points[j].x, points[j].y);
    }

    cairo_pattern_t *pat;
    if ( !gradient_create_pattern( x0, y0, x1 - x0, y1 - y0, d0, d1, d2, gradient, &pat))
      return 0;
    cairo_set_source( cr, pat);
    cairo_fill(cr);

    if ( ww->clip_on)
      reset_cairo_clip( ww, cr);
    cairo_destroy(cr);
  }

  return 1;
}

#define FONTSTR "Lucida Sans"
#define FONT_SCALE 0.7
#define FONT_DESCENT 0.2

static char *font_string( int font_idx, int font_type, double size)
{
  static char str[80];
  char bold_str[20];

  if ( font_type == glow_eDrawFont_HelveticaBold)
    strcpy( bold_str, "Bold ");
  else
    strcpy( bold_str, "");

  switch ( font_idx) {
  case glow_eFont_Helvetica:
    sprintf( str, "%s %s%f", "Helvetica", bold_str, FONT_SCALE * size);
    break;
  case glow_eFont_Times:
    sprintf( str, "%s %s%f", "DejaVu Serif", bold_str, FONT_SCALE * size);
    break;
  case glow_eFont_NewCenturySchoolbook:
    sprintf( str, "%s %s%f", "Century Schoolbook L", bold_str, FONT_SCALE * size);
    break;
  case glow_eFont_Courier:
    sprintf( str, "%s %s%f", "Courier 10 pitch", bold_str, FONT_SCALE * size);
    break;
  default:
    sprintf( str, "%s %s%f", "Lucida Sans", bold_str, FONT_SCALE * size);
  }
  return str;
}

int GlowDrawGtk::text_pango( GlowWind *wind, int x, int y, char *text, int len,
			     glow_eDrawType gc_type, glow_eDrawType color, int idx, 
			     int highlight, int line, glow_eFont font_idx, double size)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( font_idx >= glow_eFont__)
    font_idx = glow_eFont_Helvetica;

  int font_type = get_font_type( gc_type);

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));

  if ( color != glow_eDrawType_Line) {
    GdkGCValues 		xgcv;

    gdk_gc_get_values( get_gc( this, color, 0), &xgcv);

    gdk_gc_set_values( get_gc( this, gc_type, idx), &xgcv,
	GDK_GC_FOREGROUND);
  }

  PangoRenderer *pr = gdk_pango_renderer_get_default( screen);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), get_gc( this, gc_type, idx));

  for ( int i = 0; i < 2; i++) {
    if ( i == 0) {
      if ( !w->draw_buffer_only)
	gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), w->window);
      else
	continue;
    }
    else if ( i == 1) {
      if ( w->double_buffer_on)
	gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), w->buffer);
      else
	continue;
    }

    PangoContext *pctx = gdk_pango_context_get_for_screen( screen);
    PangoLayout *layout = pango_layout_new( pctx);
    char *textutf8 = g_convert( text, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
    pango_layout_set_text( layout, textutf8, -1);
    g_free( textutf8);
    PangoFontDescription *desc = pango_font_description_from_string( font_string( font_idx, font_type, size));
    pango_layout_set_font_description( layout, desc);
    pango_font_description_free( desc);

    int width, height;
    pango_layout_get_size( layout, &width, &height);
    height *= 0.9;
    pango_renderer_draw_layout( pr, layout, PANGO_SCALE * x, PANGO_SCALE * y - 0.85*height);
  
    g_object_unref( layout);
    g_object_unref( pctx);
  }
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), 0);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), 0);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));

  if ( color != glow_eDrawType_Line) {
    GdkGCValues 		xgcv;

    gdk_gc_get_values( get_gc( this, glow_eDrawType_Line, 0), 
	&xgcv);

    gdk_gc_set_values( get_gc( this, gc_type, idx), &xgcv,
	GDK_GC_FOREGROUND);
  }

  return 1;
}

int GlowDrawGtk::text_erase_pango( GlowWind *wind, int x, int y, char *text, int len,
				   glow_eDrawType gc_type, int idx, int line, glow_eFont font_idx,
				   double size)
{
  if ( ctx->nodraw) return 1;
  DrawWindGtk *w = (DrawWindGtk *) wind->window;

  if ( font_idx >= glow_eFont__)
    font_idx = glow_eFont_Helvetica;

  if ( gc_type == glow_eDrawType_TextHelvetica)
    gc_type = glow_eDrawType_TextHelveticaErase;
  else if ( gc_type == glow_eDrawType_TextHelveticaBold)
    gc_type = glow_eDrawType_TextHelveticaEraseBold;

  int font_type = get_font_type( gc_type);

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));

  PangoRenderer *pr = gdk_pango_renderer_get_default( screen);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), gcs[gc_type][0]);
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), w->window);

  PangoContext *pctx = gdk_pango_context_get_for_screen( screen);
  PangoLayout *layout = pango_layout_new( pctx);
  char *textutf8 = g_convert( text, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  pango_layout_set_text( layout, textutf8, -1);
  g_free( textutf8);
  PangoFontDescription *desc = pango_font_description_from_string( font_string( font_idx, font_type, size));
  pango_layout_set_font_description( layout, desc);
  pango_font_description_free( desc);

  int width, height;
  pango_layout_get_size( layout, &width, &height);
  height *= 0.9;
  if ( !w->draw_buffer_only)
    gdk_draw_rectangle( w->window, 
		      get_gc( this, gc_type, idx), 1, 
			x, y - (1.0 - FONT_DESCENT) * height / PANGO_SCALE, 
			width / PANGO_SCALE, height / PANGO_SCALE);
 if ( w->double_buffer_on)
    gdk_draw_rectangle( w->buffer, 
			get_gc( this, gc_type, idx), 1, 
			x, y - (1.0 - FONT_DESCENT) * height / PANGO_SCALE, 
			width / PANGO_SCALE, height / PANGO_SCALE);
  
  g_object_unref( layout);
  g_object_unref( pctx);
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), 0);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), 0);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));

  return 1;
}

int GlowDrawGtk::get_text_extent_pango( const char *text, int len,
					glow_eDrawType gc_type, int idx, glow_eFont font_idx,
					int *width, int *height, int *descent, double size)
{
  DrawWindGtk *w = &m_wind;

  if ( font_idx >= glow_eFont__)
    font_idx = glow_eFont_Helvetica;

  if ( gc_type == glow_eDrawType_TextHelvetica)
    gc_type = glow_eDrawType_TextHelveticaErase;
  else if ( gc_type == glow_eDrawType_TextHelveticaBold)
    gc_type = glow_eDrawType_TextHelveticaEraseBold;

  int font_type = get_font_type( gc_type);

  PangoRenderer *pr = gdk_pango_renderer_get_default( screen);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), gcs[gc_type][0]);
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), w->window);

  PangoContext *pctx = gdk_pango_context_get_for_screen( screen);
  PangoLayout *layout = pango_layout_new( pctx);
  char *textutf8 = g_convert( text, len, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  pango_layout_set_text( layout, textutf8, -1);
  g_free( textutf8);
  PangoFontDescription *desc = pango_font_description_from_string( font_string( font_idx, font_type, size));
  pango_layout_set_font_description( layout, desc);
  pango_font_description_free( desc);

  int lwidth, lheight;
  pango_layout_get_size( layout, &lwidth, &lheight);
  lheight *= 0.9;

  *width = lwidth / PANGO_SCALE;
  *height = lheight / PANGO_SCALE;
  *descent = FONT_DESCENT * lheight / PANGO_SCALE;

  g_object_unref( layout);
  g_object_unref( pctx);
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), 0);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), 0);

  return 1;
}
