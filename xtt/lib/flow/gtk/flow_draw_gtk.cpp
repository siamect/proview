/* 
 * Proview   $Id: flow_draw_gtk.cpp,v 1.14 2008-12-03 08:28:47 claes Exp $
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

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>


#include "flow.h"
#include "flow_ctx.h"
#include "flow_browctx.h"
#include "flow_draw_gtk.h"
#include "flow_msg.h"
#include "flow_widget_gtk.h"
#include "pwr.h"
#include "co_dcli.h"
#include "flow_browwidget_gtk.h"
#include <gdk/gdkkeysyms.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <gdk/gdkx.h>
typedef void *GdkImlibImage;

#define DRAW_PRESS_PIX 9

static int pango = 1;

typedef struct {
	GtkWidget      	*w;
	int		x;
	int		y;
	int		width;
	int		height;
	FlowArrayElem	*node;
	int		number;
        FlowCtx 	*ctx;
} draw_sAnnotData;

typedef struct {
	FlowCtx 	*ctx;
	void 		(*callback_func)( FlowCtx *ctx);
	guint		timer_id;
} draw_sTimerCb;

typedef struct {
	GdkPixmap *pixmap[DRAW_PIXMAP_SIZE];
	} draw_sPixmap;

static char	font_names[draw_eFont__][DRAW_FONT_SIZE][80] = { {	
	"-*-Helvetica-Bold-R-Normal--8-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--10-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--14-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--14-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--24-*-*-*-P-*-ISO8859-1"
	},{ 
	"-*-Helvetica-Medium-R-Normal--8-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--10-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--12-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--14-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--14-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--24-*-*-*-P-*-ISO8859-1"
	} };


static  GdkEvent last_event;

static GdkColor flow_allocate_color( FlowDrawGtk *draw_ctx, const char *named_color);
static void event_timer( FlowCtx *ctx, int time_ms);
static void cancel_event_timer(FlowCtx *ctx);
static gboolean event_timer_cb( void *ctx);
#if 0
static void draw_input_cb( GtkWidget *w, XtPointer client_data, XtPointer call_data);
#endif

static int flow_create_cursor( FlowDrawGtk *draw_ctx)
{
  /* Create some cursors */

  /* Cross cursor */
  draw_ctx->cursors[draw_eCursor_CrossHair] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_CROSSHAIR);

  draw_ctx->cursors[draw_eCursor_DiamondCross] = 
		gdk_cursor_new_for_display( draw_ctx->display, GDK_DIAMOND_CROSS);

  draw_ctx->cursors[draw_eCursor_Hand] = 
	        gdk_cursor_new_for_display( draw_ctx->display, GDK_HAND2);

  return 1;
}

static int draw_free_gc( FlowDrawGtk *draw_ctx)
{
  int i, j;

  for ( i = 1; i < draw_eCursor__ ; i++)
    gdk_cursor_unref( draw_ctx->cursors[i]);

  gdk_gc_unref( draw_ctx->gc_yellow);
  gdk_gc_unref( draw_ctx->gc_green);
  gdk_gc_unref( draw_ctx->gc_darkgray);
  gdk_gc_unref( draw_ctx->gc_inverse);
  for ( i = 0; i < flow_eDrawType__; i++) {
    for ( j = 0; j < DRAW_TYPE_SIZE; j++)
      gdk_gc_unref( draw_ctx->gcs[i][j]);
  }  

  for ( i = 0; i < draw_eFont__; i++) {
    for ( j = 0; j < DRAW_FONT_SIZE; j++) {
      gdk_font_unref( draw_ctx->font[i][j]);
    }
  }
  return 1;
}

static int flow_create_gc( FlowDrawGtk *draw_ctx, GdkWindow *window)
{
  GdkFont 		*font;
  GdkGCValues 		xgcv;
  int			i;

  /* Inverse gc */
  xgcv.background = draw_ctx->foreground;
  xgcv.foreground = draw_ctx->background;
  xgcv.cap_style = GDK_CAP_BUTT;
  draw_ctx->gc_inverse = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | 
	GDK_GC_CAP_STYLE));

  /* Yellow gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "yellow");
  xgcv.background = draw_ctx->background;
  draw_ctx->gc_yellow = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));

  /* Green gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "green");
  xgcv.background = draw_ctx->background;
  draw_ctx->gc_green = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));

  /* DarkGray gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "gray28");
  xgcv.background = draw_ctx->background;
  draw_ctx->gc_darkgray = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));

  /* Black line gc */
  xgcv.foreground = draw_ctx->foreground;
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[flow_eDrawType_Line][i] = gdk_gc_new_with_values( 
	window, &xgcv, 
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH));
  }

  /* Erase line gc */
  xgcv.foreground = draw_ctx->background;
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[flow_eDrawType_LineErase][i] = gdk_gc_new_with_values( 
	window, &xgcv, 
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH));
  }

  /* Red line gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "red");
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[flow_eDrawType_LineRed][i] = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH));
  }

  /* Gray line gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "gray");
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[flow_eDrawType_LineGray][i] = gdk_gc_new_with_values(
	window, &xgcv, 
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH));
  }

  /* Dashed line gc */
  xgcv.foreground = draw_ctx->foreground;
  xgcv.background = draw_ctx->background;
  xgcv.line_style = GDK_LINE_ON_OFF_DASH;
  gint dash_offset = 0;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;
    gint8 dashes = 7 + i;

    draw_ctx->gcs[flow_eDrawType_LineDashed][i] = gdk_gc_new_with_values( 
	window, &xgcv,
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH | 
        GDK_GC_LINE_STYLE));
    gdk_gc_set_dashes( draw_ctx->gcs[flow_eDrawType_LineDashed][i], 0, &dashes, 1);
  }

  /* Red dashed line gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "red");
  xgcv.background = draw_ctx->background;
  xgcv.line_style = GDK_LINE_ON_OFF_DASH;
  dash_offset = 0;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    xgcv.line_width = i + 1;
    gint8 dashes = 7 + i;

    draw_ctx->gcs[flow_eDrawType_LineDashedRed][i] = gdk_gc_new_with_values( 
	window, &xgcv,
	(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND | GDK_GC_LINE_WIDTH | 
        GDK_GC_LINE_STYLE));
    gdk_gc_set_dashes( draw_ctx->gcs[flow_eDrawType_LineDashedRed][i], dash_offset, &dashes, 1);
  }

  /* Text */
  xgcv.foreground = draw_ctx->foreground;
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    draw_ctx->gcs[flow_eDrawType_TextHelvetica][i] = gdk_gc_new_with_values( 
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }

  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    draw_ctx->gcs[flow_eDrawType_TextHelveticaBold][i] = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }

  xgcv.foreground = draw_ctx->background;
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    draw_ctx->gcs[flow_eDrawType_TextHelveticaErase][i] = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));
  }

  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[flow_eDrawType_TextHelveticaEraseBold][i] = gdk_gc_new_with_values(
	window, &xgcv, (GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_BACKGROUND));

  }

  for ( i = 0; i < DRAW_FONT_SIZE; i++) {
    font = gdk_font_load( font_names[draw_eFont_HelveticaBold][i]);
    gdk_gc_set_font( draw_ctx->gcs[flow_eDrawType_TextHelveticaBold][i], font);
    gdk_gc_set_font( draw_ctx->gcs[flow_eDrawType_TextHelveticaEraseBold][i], font);
    draw_ctx->font[draw_eFont_HelveticaBold][i] = font;
  }
  for ( i = 0; i < DRAW_FONT_SIZE; i++)
  {
    font = gdk_font_load( font_names[draw_eFont_Helvetica][i]);
    gdk_gc_set_font( draw_ctx->gcs[flow_eDrawType_TextHelvetica][i], font);
    gdk_gc_set_font( draw_ctx->gcs[flow_eDrawType_TextHelveticaErase][i], font);
    draw_ctx->font[draw_eFont_Helvetica][i] = font;
  }

  return 1;
}


static GdkColor flow_allocate_color( FlowDrawGtk *draw_ctx, const char *named_color)
{
  GdkColor color;

  if ( !gdk_color_parse( named_color, &color))
    gdk_color_parse( "black", &color);
  gdk_colormap_alloc_color( draw_ctx->colormap, &color, FALSE, TRUE);

  return color;
}

FlowDrawGtk::~FlowDrawGtk()
{
  basectx->set_nodraw();
  delete basectx;
  draw_free_gc( this);
}

int FlowDrawGtk::create_secondary_ctx( 
	FlowCtx *flow_ctx,
	void **secondary_flow_ctx,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void  *client_data, 
	flow_eCtxType type
)
{
  FlowCtx	*ctx;

  if ( type == flow_eCtxType_Brow)
    ctx = (FlowCtx *) new BrowCtx("Claes context", 20);
  else
    ctx = new FlowCtx("Claes context", 20);
  
  ctx->fdraw = this;
  ctx->set_nodraw();
  ctx->trace_connect_func = flow_ctx->trace_connect_func;
  ctx->trace_disconnect_func = flow_ctx->trace_disconnect_func;
  ctx->trace_scan_func = flow_ctx->trace_scan_func;
  ctx->trace_started = flow_ctx->trace_started;
  ctx->zoom_factor = flow_ctx->zoom_factor;
  if ( type == flow_eCtxType_Brow) {
    ((BrowCtx *)ctx)->scroll_callback = ((BrowCtx *)flow_ctx)->scroll_callback;
    ((BrowCtx *)ctx)->scroll_data = ((BrowCtx *)flow_ctx)->scroll_data;
  }

  init_proc( ctx, client_data);
  *secondary_flow_ctx = (void *) ctx;
  return 1;
}

int FlowDrawGtk::change_ctx( 
	FlowCtx *from_ctx,
	FlowCtx *to_ctx
)
{
  if ( from_ctx->type() == flow_eCtxType_Brow)
    browwidgetgtk_modify_ctx( toplevel, to_ctx);
  else
    flowwidgetgtk_modify_ctx( toplevel, to_ctx);

  to_ctx->window_width = from_ctx->window_width;
  to_ctx->window_height = from_ctx->window_height;
  from_ctx->set_nodraw();
  to_ctx->reset_nodraw();
  if ( from_ctx->type() == flow_eCtxType_Brow) {
    to_ctx->zoom_factor = from_ctx->zoom_factor;
    ((BrowCtx *)to_ctx)->configure(0);
  }
  else
    to_ctx->redraw();
  return 1;
}

void FlowDrawGtk::delete_secondary_ctx( FlowCtx *ctx)
{
  ctx->set_nodraw();
  delete ctx;
}

FlowDrawGtk::FlowDrawGtk( 
	GtkWidget *x_toplevel, 
	void **flow_ctx,
        int (*init_proc)(GtkWidget *w, FlowCtx *ctx, void *client_data),
	void  *client_data, 
	flow_eCtxType type) :
    toplevel(x_toplevel), nav_shell(0), 
    nav_toplevel(0), display(0),
    window(0), nav_window(0), screen(0), timer_id(0)
{

  memset( gcs, 0, sizeof(gcs));
  memset( font, 0, sizeof(font));
  memset( cursors, 0, sizeof(cursors));

  if ( type == flow_eCtxType_Brow)
    basectx = (FlowCtx *) new BrowCtx("Claes context", 20);
  else
    basectx = new FlowCtx("Claes context", 20);
  *flow_ctx = (void *) basectx;
  
  basectx->fdraw = this;

  display = gtk_widget_get_display( toplevel);
  window = toplevel->window;
  screen = gtk_widget_get_screen( toplevel);

  colormap = gdk_colormap_new( gdk_visual_get_system(), TRUE);

  // GtkStyle *style = gtk_widget_get_style( toplevel);
  // background = style->bg[GTK_STATE_NORMAL];
  // gdk_colormap_alloc_color( colormap, &background, FALSE, TRUE);

  foreground = flow_allocate_color( this, "black");
  flow_create_gc( this, window);

  set_white_background( basectx);

  flow_create_cursor( this);

  init_proc( toplevel, basectx, client_data);
}


int FlowDrawGtk::init_nav( GtkWidget *nav_widget, void *flow_ctx)
{
  nav_toplevel = nav_widget;
  nav_window = nav_toplevel->window;

//  flow_create_gc( this, nav_window);

  gtk_widget_modify_bg( nav_widget, GTK_STATE_NORMAL, &background);

  ((FlowCtx *)flow_ctx)->no_nav = 0;
  return 1;
}


int FlowDrawGtk::event_handler( FlowCtx *ctx, GdkEvent event)
{
  static int	button_pressed = 0;
  static int	button_clicked = 0;
  static int	button_clicked_and_pressed = 0;
  static int 	button1_pressed = 0;
  static int 	button2_pressed = 0;
  static int 	button3_pressed = 0;
  static int	last_press_x = 0;
  static int	last_press_y = 0;
  int           sts = 1;

  if ( event.any.window == window || event.type == GDK_KEY_PRESS) {
    switch ( event.type) {
    case GDK_KEY_PRESS : {
      guint keysym = event.key.keyval;

      switch ( keysym) {
      case GDK_Return:
	sts = ctx->event_handler( flow_eEvent_Key_Return, 0, 0, 0, 0);
//            printf( "-- Return key event\n");
	break;
      case GDK_Up:
	if ( event.key.state & GDK_SHIFT_MASK)
	  sts = ctx->event_handler( flow_eEvent_Key_ShiftUp, 0, 0, 0, 0);
	else
	  sts = ctx->event_handler( flow_eEvent_Key_Up, 0, 0, 0, 0);
	break;
      case GDK_Down:
	if ( event.key.state & GDK_SHIFT_MASK)
	  sts = ctx->event_handler( flow_eEvent_Key_ShiftDown, 0, 0, 0, 0);
	else
	  sts = ctx->event_handler( flow_eEvent_Key_Down, 0, 0, 0, 0);
	break;
      case GDK_Right:
	if ( event.key.state & GDK_SHIFT_MASK)
	  sts = ctx->event_handler( flow_eEvent_Key_ShiftRight, 0, 0, 0, 0);
	else
	  sts = ctx->event_handler( flow_eEvent_Key_Right, 0, 0, 0, 0);
	break;
      case GDK_Left:
	if ( event.key.state & GDK_SHIFT_MASK)
	  sts = ctx->event_handler( flow_eEvent_Key_ShiftLeft, 0, 0, 0, 0);
	else
	  sts = ctx->event_handler( flow_eEvent_Key_Left, 0, 0, 0, 0);
	break;
      case GDK_Page_Up:
      case 0xFF41:
	sts = ctx->event_handler( flow_eEvent_Key_PageUp, 0, 0, 0, 0);
	break;
      case GDK_Page_Down:
      case 0xFF42:
	sts = ctx->event_handler( flow_eEvent_Key_PageDown, 0, 0, 0, 0);
	break;
      case GDK_BackSpace:
	sts = ctx->event_handler( flow_eEvent_Key_BackSpace, 0, 0, 0, 0);
	break;
      case GDK_KP_F1:
	sts = ctx->event_handler( flow_eEvent_Key_PF1, 0, 0, 0, 0);
	break;
      case GDK_KP_F2:
	sts = ctx->event_handler( flow_eEvent_Key_PF2, 0, 0, 0, 0);
	break;
      case GDK_KP_F3:
	sts = ctx->event_handler( flow_eEvent_Key_PF3, 0, 0, 0, 0);
	break;
      case GDK_KP_F4:
	sts = ctx->event_handler( flow_eEvent_Key_PF4, 0, 0, 0, 0);
	break;
      case GDK_Tab:
	sts = ctx->event_handler( flow_eEvent_Key_Tab, 0, 0, 0, 0);
	break;
      default:
	;
      }
      break;
    }
    case GDK_BUTTON_PRESS : 

      switch ( event.button.button) {
      case 1:
	ctx->event_handler( flow_eEvent_MB1Down, (int)event.button.x, (int)event.button.y, 0, 0);
	if ( click_sensitivity & flow_mSensitivity_MB1Click && 
	     !(click_sensitivity & flow_mSensitivity_MB1DoubleClick) && 
	     !(click_sensitivity & flow_mSensitivity_MB1Press)) {
	  memcpy( &last_event, &event, sizeof(event));
	  button_pressed = 0;
	  button_clicked = 1;
	  last_press_x = (int)event.button.x;
	  last_press_y = (int)event.button.y;
	  return 1;
	}
	else if ( !(click_sensitivity & flow_mSensitivity_MB1Click) && 
		  !(click_sensitivity & flow_mSensitivity_MB1DoubleClick) && 
		  click_sensitivity & flow_mSensitivity_MB1Press) {
	  memcpy( &last_event, &event, sizeof(event));
	  button_pressed = 1;
	  button_clicked = 0;
	  last_press_x = (int)event.button.x;
	  last_press_y = (int)event.button.y;
	}
	break;
      case 2:
	ctx->event_handler( flow_eEvent_MB2Down, (int)event.button.x, (int)event.button.y, 0, 0);
	if ( click_sensitivity & flow_mSensitivity_MB2Click && 
	     !(click_sensitivity & flow_mSensitivity_MB2DoubleClick) && 
	     !(click_sensitivity & flow_mSensitivity_MB2Press)) {
	  memcpy( &last_event, &event, sizeof(event));
	  button_pressed = 0;
	  button_clicked = 1;
	  last_press_x = (int)event.button.x;
	  last_press_y = (int)event.button.y;
	  return 1;
	}
	else if ( !(click_sensitivity & flow_mSensitivity_MB2Click) && 
		  !(click_sensitivity & flow_mSensitivity_MB2DoubleClick) && 
		  click_sensitivity & flow_mSensitivity_MB2Press) {
	  memcpy( &last_event, &event, sizeof(event));
	  button_pressed = 1;
	  button_clicked = 0;
	  last_press_x = (int)event.button.x;
	  last_press_y = (int)event.button.y;
	}
	break;
      case 3: // Button 3
	ctx->event_handler( flow_eEvent_MB3Down, (int)event.button.x, (int)event.button.y, 0, 0);
	if ( click_sensitivity & flow_mSensitivity_MB3Press && 
	     !(click_sensitivity & flow_mSensitivity_MB3DoubleClick) && 
	     !(click_sensitivity & flow_mSensitivity_MB3Click)) {
	  ctx->event_handler( flow_eEvent_MB3Press, 
				  (int)event.button.x, (int)event.button.y, 0, 0);
	  click_sensitivity = 0;
	  return 1;
	}
	break;
      default:
	;
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
	/* Press event, callback from timer */
	button_pressed = 0;
	button_clicked_and_pressed = 0;
	switch ( event.button.button) {
	case 1: // Button1
	  button1_pressed = 1;
	  if ( (event.button.state & GDK_SHIFT_MASK) && 
	       !(event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB1PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB1PressCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( (event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB1PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else
	    ctx->event_handler( flow_eEvent_MB1Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  click_sensitivity = 0;
	  break;
	case 2: // Button2
	  button2_pressed = 1;
	  if ( (event.button.state & GDK_SHIFT_MASK) && 
	       !(event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB2PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB2PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( (event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB2PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else
	    sts = ctx->event_handler( flow_eEvent_MB2Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  click_sensitivity = 0;
	  break;
	case 3: // Button3
	  button3_pressed = 1;
#if 0
	  if ( (event.button.state & GDK_SHIFT_MASK) && 
	       !(event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB3PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB3PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if (  (event.button.state & GDK_SHIFT_MASK) && 
		     (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB3PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else
#endif
	    sts = ctx->event_handler( flow_eEvent_MB3Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  click_sensitivity = 0;
	  break;
	}
      }
      break;

    case GDK_BUTTON_RELEASE:
      button1_pressed = 0;
      button2_pressed = 0;
      button3_pressed = 0;

      if ( ! button_pressed ) {
	if ( button_clicked) {
	  /* Button click, callback from timer */
	  button_clicked = 0;
	  switch ( event.button.button) {
	  case 1: // Button1
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB1ClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB1ClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB1ClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else
	      sts = ctx->event_handler( flow_eEvent_MB1Click, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	    break;
	  case 2: // Button2
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB2ClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB2ClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB2ClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else
	      sts = ctx->event_handler( flow_eEvent_MB2Click, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	    break;
	  case 3: // Button3
#if 0
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB3ClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB3ClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB3ClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else
#endif
	      sts = ctx->event_handler( flow_eEvent_MB3Click, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	    break;
	  }
	}
	else {
	  /* Button release */
	  sts = ctx->event_handler( flow_eEvent_ButtonRelease, (int)event.button.x, (int)event.button.y, 0, 0);
	}        
      }
      else {
	/* Button click */
	cancel_event_timer( ctx);
	if ( ! button_clicked_and_pressed) {
	  /* wait for button double click */
	  memcpy( &last_event, &event, sizeof(event));
	  button_clicked = 1;
	  event_timer( ctx, 200);
	  button_pressed = 0;
	  return 1;
	}
	else {
	  /* Button double click */ 
	  cancel_event_timer( ctx);
	  button_clicked = 0;
	  button_pressed = 0;
	  button_clicked_and_pressed = 0;
	  switch ( event.button.button) {
	  case 1: // Button1
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB1DoubleClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB1DoubleClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB1DoubleClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else
	      sts = ctx->event_handler( flow_eEvent_MB1DoubleClick, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	    break;
	  case 2: // Button2
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB2DoubleClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB2DoubleClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if (  (event.button.state & GDK_SHIFT_MASK) && 
		       (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB2DoubleClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else
	      sts = ctx->event_handler( flow_eEvent_MB2DoubleClick, (int)event.button.x, (int)event.button.y, 0, 0);
	    click_sensitivity = 0;
	    break;
	  case 3: // Button3
#if 0
	    if ( (event.button.state & GDK_SHIFT_MASK) && 
		 !(event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB3DoubleClickShift, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB3DoubleClickCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else if ( (event.button.state & GDK_SHIFT_MASK) && 
		      (event.button.state & GDK_CONTROL_MASK))
	      sts = ctx->event_handler( flow_eEvent_MB3DoubleClickShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	    else
	      sts = ctx->event_handler( flow_eEvent_MB3DoubleClick, (int)event.button.x, (int)event.button.y, 0, 0);
#endif
	    click_sensitivity = 0;
	    break;
	  }
	}
      }
      break;
    case GDK_EXPOSE:
      sts = ctx->event_handler( flow_eEvent_Exposure, event.expose.area.x, 
		event.expose.area.y, event.expose.area.width, event.expose.area.height);
      break;
    case GDK_VISIBILITY_NOTIFY : 
      switch ( event.visibility.state) {
      case GDK_VISIBILITY_UNOBSCURED:
	sts = ctx->event_handler( flow_eEvent_VisibilityUnobscured, 0, 0, 0, 0);
	break;
      default: 
	sts = ctx->event_handler( flow_eEvent_VisibilityObscured, 0, 0, 0, 0);
	break;
      }
      break;        
    case GDK_MOTION_NOTIFY:
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
	   (abs( (int)event.button.x - last_press_x) > DRAW_PRESS_PIX ||
	    abs( (int)event.button.y - last_press_y) > DRAW_PRESS_PIX)) {

	event.button.x = last_press_x;
	event.button.y = last_press_y;

	/* Button press */
	cancel_event_timer( ctx);
	switch ( button_pressed) {
	case 1: // Button1
	  button1_pressed = 1;
	  if ( (event.button.state & GDK_SHIFT_MASK) && 
	       !(event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB1PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB1PressCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( (event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB1PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else
	    ctx->event_handler( flow_eEvent_MB1Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  break;
	case 2: // Button2
	  button2_pressed = 1;
	  if ( (event.button.state & GDK_SHIFT_MASK) && 
	       !(event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB2PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if ( !(event.button.state & GDK_SHIFT_MASK) && 
		    (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB2PressShift, (int)event.button.x, (int)event.button.y, 0, 0);
	  else if (  (event.button.state & GDK_SHIFT_MASK) && 
		     (event.button.state & GDK_CONTROL_MASK))
	    sts = ctx->event_handler( flow_eEvent_MB2PressShiftCtrl, (int)event.button.x, (int)event.button.y, 0, 0);
	  else
	    sts = ctx->event_handler( flow_eEvent_MB2Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  break;
	case 3: // Button3
	  button3_pressed = 1;
	  sts = ctx->event_handler( flow_eEvent_MB3Press, (int)event.button.x, (int)event.button.y, 0, 0);
	  break;
	}
	button_pressed = 0;
	button_clicked_and_pressed = 0;
	
      }
      if ( button1_pressed || button2_pressed || button3_pressed)
	sts = ctx->event_handler( flow_eEvent_ButtonMotion, (int)event.button.x, (int)event.button.y, 0, 0);
      else
	sts = ctx->event_handler( flow_eEvent_CursorMotion, (int)event.button.x, (int)event.button.y, 0, 0);
      break;
    case GDK_ENTER_NOTIFY:
      sts = ctx->event_handler( flow_eEvent_Enter, (int)event.crossing.x, (int)event.crossing.y, 0, 0);
      break;          
    case GDK_LEAVE_NOTIFY:
      sts = ctx->event_handler( flow_eEvent_Leave, (int)event.crossing.x, (int)event.crossing.y, 0, 0);
      break;          
    case GDK_MAP:
      sts = ctx->event_handler( flow_eEvent_Map, 0, 0, 0, 0);
      break;          
    case GDK_UNMAP:
      sts = ctx->event_handler( flow_eEvent_Unmap, 0, 0, 0, 0);
      break;          
    case GDK_FOCUS_CHANGE:
      break;          
    case GDK_SCROLL:
      if ( event.scroll.direction == GDK_SCROLL_UP)
	sts = ctx->event_handler( flow_eEvent_ScrollUp, 0, 0, 0, 0);
      else if ( event.scroll.direction == GDK_SCROLL_DOWN)
	sts = ctx->event_handler( flow_eEvent_ScrollDown, 0, 0, 0, 0);
      break;
    default:
      break;
    }
  }
  else if ( event.any.window == nav_window) {
    switch ( event.type) {
    case GDK_BUTTON_PRESS : 
      switch ( event.button.button) {
      case 1: // Button1
	button1_pressed = 1;
	sts = ctx->event_handler_nav( flow_eEvent_MB1Press, (int)event.button.x, (int)event.button.y);
	break;
      case 2: // Button2
	button2_pressed = 1;
	sts = ctx->event_handler_nav( flow_eEvent_MB2Press, (int)event.button.x, (int)event.button.y);
	break;
      case 3: // Button3
	button3_pressed = 1;
	break;
      }
      break;          
    case GDK_BUTTON_RELEASE : 
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
      sts = ctx->event_handler_nav( flow_eEvent_ButtonRelease, (int)event.button.x, (int)event.button.y);
      break;          
    case GDK_EXPOSE:
      sts = ctx->event_handler_nav( flow_eEvent_Exposure, 0, 0);
      break; 
    case GDK_MOTION_NOTIFY:
      if ( event.motion.is_hint) {
	int x, y;

	gdk_window_get_pointer( event.any.window, &x, &y, NULL);
	event.button.x = x;
	event.button.y = y;
      }

      if ( button1_pressed || button2_pressed || button3_pressed)
	sts = ctx->event_handler_nav( flow_eEvent_ButtonMotion, (int)event.button.x, (int)event.button.y);
      else
	sts = ctx->event_handler_nav( flow_eEvent_CursorMotion, (int)event.button.x, (int)event.button.y);
      break;          
    default: ;
    }
  }
  if ( sts != FLOW__DESTROYED)
    gdk_display_flush( display);
  return 1;
}

void FlowDrawGtk::enable_event( FlowCtx *ctx, flow_eEvent event, 
		flow_eEventType event_type, 
		int (*event_cb)(FlowCtx *ctx, flow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

int FlowDrawGtk::rect( FlowCtx *ctx, int x, int y, int width, int height, 
	flow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  gdk_draw_rectangle( window, 
		      gcs[gc_type+highlight][idx], 0,
		      x, y, width, height);
  return 1;
}

int FlowDrawGtk::rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int idx)
{
  if ( ctx->nodraw) return 1;

  gdk_draw_rectangle( window, 
		      gcs[flow_eDrawType_LineErase][idx], 0, 
		      x, y, width, height);
  return 1;
}

int FlowDrawGtk::nav_rect( FlowCtx *ctx, int x, int y, int width, int height,
	flow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->no_nav || ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  gdk_draw_rectangle( nav_window,
		      gcs[gc_type+highlight][idx], 0,
		      x, y, width, height);
  return 1;
}

int FlowDrawGtk::nav_rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int idx)
{
  if ( ctx->no_nav || ctx->nodraw) return 1;

  gdk_draw_rectangle( nav_window,
		      gcs[flow_eDrawType_LineErase][idx], 0,
		      x, y, width, height);
  return 1;
}

int FlowDrawGtk::arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	flow_eDrawType gc_type, int idx, int highlight)
{
  GdkPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  if ( ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  gdk_draw_polygon( window,
	gcs[gc_type+highlight][idx], 1, p, 4);
  return 1;
}

int FlowDrawGtk::arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx)
{
  GdkPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  if ( ctx->nodraw) return 1;

  gdk_draw_polygon( window,
		    gcs[flow_eDrawType_LineErase][idx], 1,
		    p, 4);
  return 1;
}

int FlowDrawGtk::nav_arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	flow_eDrawType gc_type, int idx, int highlight)
{
  GdkPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  if ( ctx->no_nav || ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  gdk_draw_polygon( nav_window,
	gcs[gc_type+highlight][idx], 1, p, 4);
  return 1;
}

int FlowDrawGtk::nav_arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx)
{
  GdkPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  if ( ctx->no_nav || ctx->nodraw) return 1;

  gdk_draw_polygon( nav_window,
		    gcs[flow_eDrawType_LineErase][idx], 1,
		    p, 4);
  return 1;
}

int FlowDrawGtk::arc( FlowCtx *ctx, int x, int y, int width, int height, 
	int angle1, int angle2,
	flow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  gdk_draw_arc( window,
		gcs[gc_type+highlight][idx], 0,
		x, y, width, height, angle1*64, angle2*64);
  return 1;
}

int FlowDrawGtk::arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int angle1, int angle2,
	int idx)
{
  if ( ctx->nodraw) return 1;

  gdk_draw_arc( window,
		gcs[flow_eDrawType_LineErase][idx], 0,
		x, y, width, height, angle1*64, angle2*64);
  return 1;
}

int FlowDrawGtk::nav_arc( FlowCtx *ctx, int x, int y, int width, int height,
	int angle1, int angle2,
	flow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->no_nav || ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  gdk_draw_arc( nav_window,
		gcs[gc_type+highlight][idx], 0,
		x, y, width, height, angle1*64, angle2*64);
  return 1;
}

int FlowDrawGtk::nav_arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int angle1, int angle2,
	int idx)
{
  if ( ctx->no_nav || ctx->nodraw) return 1;

  gdk_draw_arc( nav_window,
		gcs[flow_eDrawType_LineErase][idx], 0,
		x, y, width, height, angle1*64, angle2*64);
  return 1;
}

int FlowDrawGtk::line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	flow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  gdk_draw_line( window,
		 gcs[gc_type+highlight][idx], 
		 x1, y1, x2, y2);
  return 1;
}

int FlowDrawGtk::line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx)
{
  if ( ctx->nodraw) return 1;

  gdk_draw_line( window,
		 gcs[flow_eDrawType_LineErase][idx], 
		 x1, y1, x2, y2);
  return 1;
}

int FlowDrawGtk::nav_line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	flow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->no_nav || ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  gdk_draw_line( nav_window,
		 gcs[gc_type+highlight][idx], 
		 x1, y1, x2, y2);
  return 1;
}

int FlowDrawGtk::nav_line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx)
{
  if ( ctx->no_nav || ctx->nodraw) return 1;

  gdk_draw_line( nav_window,
		 gcs[flow_eDrawType_LineErase][idx], 
		 x1, y1, x2, y2);
  return 1;
}

#define FONTSTR "Lucida Sans"
#define FONT_SCALE 0.7

static char *font_string( flow_eDrawType gc_type, double size)
{
  static char str[80];
  switch ( gc_type) {
  case  flow_eDrawType_TextHelveticaBold:
  case flow_eDrawType_TextHelveticaEraseBold:
    sprintf( str, "%s Bold %f", FONTSTR, FONT_SCALE * size);
    break;
  default:
    sprintf( str, "%s %f", FONTSTR, FONT_SCALE * size);
  }
  return str;
}

int FlowDrawGtk::text_pango( FlowCtx *ctx, int x, int y, char *text, int len,
			     flow_eDrawType gc_type, int idx, int highlight, int line, double size)
{
  if ( ctx->nodraw) return 1;

  PangoRenderer *pr = gdk_pango_renderer_get_default( screen);
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), window);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), gcs[gc_type][0]);

  PangoContext *pctx = gdk_pango_context_get_for_screen( screen);
  PangoLayout *layout = pango_layout_new( pctx);
  char *textutf8 = g_convert( text, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  pango_layout_set_text( layout, textutf8, -1);
  g_free( textutf8);
  PangoFontDescription *desc = pango_font_description_from_string( font_string( gc_type, size));
  pango_layout_set_font_description( layout, desc);
  pango_font_description_free( desc);

  int w, h;
  pango_layout_get_size( layout, &w, &h);

  // Erase old test first
  gdk_draw_rectangle( window, 
		      gcs[flow_eDrawType_LineErase][idx], 1, 
		      x, y - 0.8 / PANGO_SCALE * h, w / PANGO_SCALE, h / PANGO_SCALE);

  pango_renderer_draw_layout( pr, layout, PANGO_SCALE * x, PANGO_SCALE * y - h * 0.8);
  
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), 0);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), 0);
  g_object_unref( layout);
  g_object_unref( pctx);
  
  return 1;
}

int FlowDrawGtk::text_inverse_pango( FlowCtx *ctx, int x, int y, char *text, int len,
				     flow_eDrawType gc_type, int idx, int line, double size)
{
  if ( ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_TextHelvetica)
    gc_type = flow_eDrawType_TextHelveticaErase;
  else if ( gc_type == flow_eDrawType_TextHelveticaBold)
    gc_type = flow_eDrawType_TextHelveticaEraseBold;

  PangoRenderer *pr = gdk_pango_renderer_get_default( screen);
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), window);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), gcs[gc_type][idx]);

  PangoContext *pctx = gdk_pango_context_get_for_screen( screen);
  PangoLayout *layout = pango_layout_new( pctx);
  char *textutf8 = g_convert( text, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  pango_layout_set_text( layout, textutf8, -1);
  g_free( textutf8);
  PangoFontDescription *desc = pango_font_description_from_string( font_string( gc_type, size));
  pango_layout_set_font_description( layout, desc);
  pango_font_description_free( desc);

  int w, h;
  pango_layout_get_size( layout, &w, &h);

  pango_renderer_draw_layout( pr, layout, PANGO_SCALE * x, PANGO_SCALE * y - h * 0.8);
  
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), 0);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), 0);
  g_object_unref( layout);
  g_object_unref( pctx);

  return 1;
}

int FlowDrawGtk::text_erase_pango( FlowCtx *ctx, int x, int y, char *text, int len,
				   flow_eDrawType gc_type, int idx, int line, double size)
{
  if ( ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_TextHelvetica)
    gc_type = flow_eDrawType_TextHelveticaErase;
  else if ( gc_type == flow_eDrawType_TextHelveticaBold)
    gc_type = flow_eDrawType_TextHelveticaEraseBold;

  PangoRenderer *pr = gdk_pango_renderer_get_default( screen);
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), window);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), gcs[gc_type][idx]);

  PangoContext *pctx = gdk_pango_context_get_for_screen( screen);
  PangoLayout *layout = pango_layout_new( pctx);
  char *textutf8 = g_convert( text, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  pango_layout_set_text( layout, textutf8, -1);
  g_free( textutf8);
  PangoFontDescription *desc = pango_font_description_from_string( font_string( gc_type, size));
  pango_layout_set_font_description( layout, desc);
  pango_font_description_free( desc);

  int w, h;
  pango_layout_get_size( layout, &w, &h);

  gdk_draw_rectangle( window, 
		      gcs[flow_eDrawType_LineErase][idx], 1, 
		      x, y - 0.8 / PANGO_SCALE * h, w / PANGO_SCALE, h / PANGO_SCALE);
  // pango_renderer_draw_layout( pr, layout, PANGO_SCALE * x, PANGO_SCALE * y - h * 0.8);
  
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), 0);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), 0);
  g_object_unref( layout);
  g_object_unref( pctx);

  return 1;
}

int FlowDrawGtk::text( FlowCtx *ctx, int x, int y, char *text, int len,
		       flow_eDrawType gc_type, int idx, int highlight, int line, double size)
{
  if ( ctx->nodraw) return 1;

  if ( pango)
    return text_pango( ctx, x, y, text, len, gc_type, idx, highlight, line, size);

  int font_idx = get_font_idx( gc_type);

  gdk_draw_text( window, font[font_idx][idx],
		 gcs[gc_type][idx], 
		 x, y, text, len);
  return 1;
}

int FlowDrawGtk::text_inverse( FlowCtx *ctx, int x, int y, char *text, int len,
			     flow_eDrawType gc_type, int idx, int line, double size)
{
  if ( ctx->nodraw) return 1;

  if ( pango)
    return text_inverse_pango( ctx, x, y, text, len, gc_type, idx, line, size);
  else
    return text_erase( ctx, x, y, text, len, gc_type, idx, line, size);
}

int FlowDrawGtk::text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
			     flow_eDrawType gc_type, int idx, int line, double size)
{
  if ( ctx->nodraw) return 1;

  if ( pango)
    return text_erase_pango( ctx, x, y, text, len, gc_type, idx, line, size);

  int font_idx = get_font_idx( gc_type);

  if ( gc_type == flow_eDrawType_TextHelvetica)
    gc_type = flow_eDrawType_TextHelveticaErase;
  else if ( gc_type == flow_eDrawType_TextHelveticaBold)
    gc_type = flow_eDrawType_TextHelveticaEraseBold;

  gdk_draw_text( window, font[font_idx][idx],
		 gcs[gc_type][idx], 
		 x, y, text, len);
  return 1;
}

int FlowDrawGtk::nav_text( FlowCtx *ctx, int x, int y, char *text, int len,
			   flow_eDrawType gc_type, int idx, int highlight, int line, double size)
{
  if ( ctx->no_nav || ctx->nodraw) return 1;
  int font_idx = get_font_idx( gc_type);

  gdk_draw_text( nav_window, font[font_idx][idx],
		 gcs[gc_type][idx], 
		 x, y, text, len);
  return 1;
}

int FlowDrawGtk::nav_text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
				 flow_eDrawType gc_type, int idx, int line, double size)
{
  if ( ctx->no_nav || ctx->nodraw) return 1;
  int font_idx = get_font_idx( gc_type);

  if ( gc_type == flow_eDrawType_TextHelvetica)
    gc_type = flow_eDrawType_TextHelveticaErase;
  else if ( gc_type == flow_eDrawType_TextHelveticaBold)
    gc_type = flow_eDrawType_TextHelveticaEraseBold;
  gdk_draw_text( nav_window, font[font_idx][idx],
		 gcs[gc_type][idx], 
		 x, y, text, len);
  return 1;
}

int FlowDrawGtk::pixmaps_create( FlowCtx *ctx, flow_sPixmapData *pixmap_data,
	void **pixmaps)
{
  draw_sPixmap *pms;
  flow_sPixmapDataElem *prev_pdata, *pdata = (flow_sPixmapDataElem *)pixmap_data;
  int	i;

  pms = (draw_sPixmap *) calloc( 1, sizeof( *pms));
  for ( i = 0; i < DRAW_PIXMAP_SIZE; i++) {
    if ( i == 0 || 
         (i > 0 && pdata->bits != prev_pdata->bits))
      pms->pixmap[i] = gdk_pixmap_create_from_data(
	window, pdata->bits, pdata->width,
	pdata->height, 1, &foreground, &background);
    else
      pms->pixmap[i] = pms->pixmap[i-1];
    prev_pdata = pdata;
    pdata++;
  }
  *pixmaps = (void *) pms;
  return 1;
}

void FlowDrawGtk::pixmaps_delete( FlowCtx *ctx, void *pixmaps)
{
  draw_sPixmap *pms;
  int i;

  pms = (draw_sPixmap *) pixmaps;
  for ( i = 0; i < DRAW_PIXMAP_SIZE; i++) {
    if ( i == 0 || 
         (i > 0 && pms->pixmap[i] != pms->pixmap[i-1]))
      g_object_unref( pms->pixmap[i]);
  }
  free ( pixmaps);
}

int FlowDrawGtk::pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_sPixmap *pms;
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->nodraw) return 1;
  
  pms = (draw_sPixmap *) pixmaps;

  //gdk_draw_drawable( window, gcs[gc_type][idx], 
  //		     pms->pixmap[idx], 
  //		     0, 0, x, y, pdata->width, pdata->height);
  gdk_draw_rectangle( window, gcs[gc_type][idx], 1, 
		      x, y, pdata->width, pdata->height);
  gdk_gc_set_clip_mask( gcs[flow_eDrawType_LineErase][idx], pms->pixmap[idx]);
  gdk_gc_set_clip_origin( gcs[flow_eDrawType_LineErase][idx], x, y);
  gdk_draw_rectangle( window, gcs[flow_eDrawType_LineErase][idx], 1, 
		      x, y, pdata->width, pdata->height);
  gdk_gc_set_clip_mask( gcs[flow_eDrawType_LineErase][idx], NULL);
  gdk_gc_set_clip_origin( gcs[flow_eDrawType_LineErase][idx], 0, 0);

  return 1;
}

int FlowDrawGtk::pixmap_inverse( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line)
{
  draw_sPixmap *pms;
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->nodraw) return 1;
  
  pms = (draw_sPixmap *) pixmaps;

  //gdk_draw_drawable( window, gc_inverse, 
  //		     pms->pixmap[idx], 
  //		     0, 0, x, y, pdata->width, pdata->height);
  gdk_draw_rectangle( window, gcs[flow_eDrawType_LineErase][idx], 1, 
		      x, y, pdata->width, pdata->height);
  gdk_gc_set_clip_mask( gcs[gc_type][idx], pms->pixmap[idx]);
  gdk_gc_set_clip_origin( gcs[gc_type][idx], x, y);
  gdk_draw_rectangle( window, gcs[gc_type][idx], 1, 
		      x, y, pdata->width, pdata->height);
  gdk_gc_set_clip_mask( gcs[gc_type][idx], NULL);
  gdk_gc_set_clip_origin( gcs[gc_type][idx], 0, 0);
  return 1;
}

int FlowDrawGtk::pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line)
{
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;
  if ( ctx->nodraw) return 1;

  gdk_window_clear_area( window, 
			 x, y, pdata->width, pdata->height);
  return 1;
}

int FlowDrawGtk::nav_pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_sPixmap *pms;
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->no_nav || ctx->nodraw) return 1;
  
  pms = (draw_sPixmap *) pixmaps;

  gdk_draw_drawable( nav_window, gcs[gc_type][idx], pms->pixmap[idx],
	0, 0, x, y, pdata->width, pdata->height);
  return 1;
}

int FlowDrawGtk::nav_pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line)
{
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  gdk_window_clear_area( nav_window, 
	x, y, pdata->width, pdata->height);
  return 1;
}

int FlowDrawGtk::fill_rect( FlowCtx *ctx, int x, int y, int w, int h, 
	flow_eDrawType gc_type)
{
  if ( ctx->nodraw) return 1;

  if ( gc_type == flow_eDrawType_Green)
    gdk_draw_rectangle( window, gc_green, 1, x, y, w, h);
  else if ( gc_type == flow_eDrawType_Yellow)
    gdk_draw_rectangle( window, gc_yellow, 1, x, y, w, h);
  else if ( gc_type == flow_eDrawType_DarkGray)
    gdk_draw_rectangle( window, gc_darkgray, 1, x, y, w, h);
  else
    gdk_draw_rectangle( window, gcs[gc_type][0], 1, x, y, w, h);
  return 1;
}

int FlowDrawGtk::image( FlowCtx *ctx, int x, int y, int width, int height,
		     flow_tImImage image, flow_tPixmap pixmap, flow_tPixmap clip_mask)
{
  if ( ctx->nodraw) return 1;

  if ( width == 0 || height == 0)
    return 1;
  
  if ( clip_mask)
    set_image_clip_mask( ctx, clip_mask, x, y);

  gdk_draw_pixbuf( window, gcs[flow_eDrawType_Line][0], (GdkPixbuf *)image, 0, 0, x, y, width, height,
		   GDK_RGB_DITHER_NONE, 0, 0);
#if 0
  gdk_draw_drawable( window,
	gcs[flow_eDrawType_Line][0], (GdkPixmap *)pixmap,
	0, 0, x, y, width, height);
#endif
  if ( clip_mask)
    reset_image_clip_mask( ctx);


  return 1;
}

void FlowDrawGtk::clear( FlowCtx *ctx)
{
  if ( ctx->nodraw) return;

  gdk_window_clear( window);
}

void FlowDrawGtk::nav_clear( FlowCtx *ctx)
{
  if ( ctx->no_nav || ctx->nodraw) return;

  gdk_window_clear( nav_window);
}


void FlowDrawGtk::get_window_size( FlowCtx *ctx, int *width, int *height)
{
  gdk_drawable_get_size( window, width, height);
}

void FlowDrawGtk::get_nav_window_size( FlowCtx *ctx, int *width, int *height)
{
  gdk_drawable_get_size( nav_window, width, height);
}

void FlowDrawGtk::set_nav_window_size( FlowCtx *ctx, int width, int height)
{
  gdk_window_resize( nav_window, width, height);
}

static gboolean draw_timer_cb( void *data)
{
  draw_sTimerCb *timer_cb = (draw_sTimerCb *)data;
  FlowDrawGtk *draw_ctx = (FlowDrawGtk *) timer_cb->ctx->fdraw;

  (timer_cb->callback_func)(timer_cb->ctx);
  gdk_display_flush( draw_ctx->display);
  free( timer_cb);
  return FALSE;
}

static gboolean event_timer_cb( void *ctx)
{
  ((FlowDrawGtk *)((FlowCtx *)ctx)->fdraw)->event_handler( (FlowCtx *)ctx, last_event);
  return FALSE;
}

static void cancel_event_timer( FlowCtx *ctx)
{
  FlowDrawGtk *draw_ctx  = (FlowDrawGtk *) ctx->fdraw;
  if ( draw_ctx->timer_id) {
    g_source_remove( draw_ctx->timer_id);
    draw_ctx->timer_id = 0;
  }
}

static void event_timer( FlowCtx *ctx, int time_ms)
{
  FlowDrawGtk *draw_ctx = (FlowDrawGtk *) ctx->fdraw;
  draw_ctx->timer_id = g_timeout_add( time_ms, event_timer_cb, ctx);
}

void FlowDrawGtk::set_timer( FlowCtx *ctx, int time_ms,
  	void (*callback_func)( FlowCtx *ctx), void **id)
{
  draw_sTimerCb *timer_cb;

  timer_cb = (draw_sTimerCb *) calloc( 1, sizeof(draw_sTimerCb));
  timer_cb->ctx = ctx;
  timer_cb->callback_func = callback_func;

  timer_cb->timer_id = g_timeout_add( time_ms,
				      draw_timer_cb, timer_cb);

  *id = (void *)timer_cb;
}

void FlowDrawGtk::cancel_timer( FlowCtx *ctx, void *id)
{
  g_source_remove( ((draw_sTimerCb *)id)->timer_id);

  free( (char *) id);
}

void FlowDrawGtk::set_cursor( FlowCtx *ctx, draw_eCursor cursor)
{
  if ( cursor == draw_eCursor_Normal)
    gdk_window_set_cursor( window, NULL);
  else
    gdk_window_set_cursor( window, cursors[cursor]);
  gdk_display_flush( display);
}

void FlowDrawGtk::set_nav_cursor( FlowCtx *ctx, draw_eCursor cursor)
{

  if ( ctx->no_nav) return;

  if ( cursor == draw_eCursor_Normal)
    gdk_window_set_cursor( nav_window, NULL);
  else
    gdk_window_set_cursor( nav_window, cursors[cursor]);
  gdk_display_flush( display);
}

int FlowDrawGtk::get_text_extent( FlowCtx *ctx, const char *text, int len,
				  flow_eDrawType gc_type, int idx,
				  int *width, int *height, double size)
{
  if ( pango)
    return get_text_extent_pango( ctx, text, len, gc_type, idx, size, width, height);

  int	text_width, text_ascent, text_descent, text_lbearing, text_rbearing;
  int font_idx = get_font_idx( gc_type);

  gdk_text_extents( font[font_idx][idx], text, len,
	&text_lbearing, &text_rbearing, &text_width, &text_ascent, 
		    &text_descent);
  *height = int(1.6 * text_ascent)  + text_descent;
  *width = text_width;
  return 1;
}

int FlowDrawGtk::get_text_extent_pango( FlowCtx *ctx, const char *text, int len,
					flow_eDrawType gc_type, int idx, double size,
					int *width, int *height)
{
  PangoRenderer *pr = gdk_pango_renderer_get_default( screen);
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), window);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), gcs[gc_type][idx]);

  PangoContext *pctx = gdk_pango_context_get_for_screen( screen);
  PangoLayout *layout = pango_layout_new( pctx);
  char *textutf8 = g_convert( text, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  pango_layout_set_text( layout, textutf8, -1);
  g_free( textutf8);
  PangoFontDescription *desc = pango_font_description_from_string( font_string( gc_type, size));
  pango_layout_set_font_description( layout, desc);
  pango_font_description_free( desc);

  int w, h;
  pango_layout_get_size( layout, &w, &h);

  *width = w / PANGO_SCALE;
  *height = h / PANGO_SCALE;
  
  gdk_pango_renderer_set_drawable( GDK_PANGO_RENDERER(pr), 0);
  gdk_pango_renderer_set_gc( GDK_PANGO_RENDERER(pr), 0);
  g_object_unref( layout);
  g_object_unref( pctx);

  return 1;
}

void FlowDrawGtk::copy_area( FlowCtx *ctx, int x, int y)
{
  GdkGC *gc;
  if ( ctx->nodraw) return;

  gc = gcs[flow_eDrawType_Line][3];
  if ( x >= 0 && y >= 0) {
    gdk_draw_drawable( window, gc, window, 
	0, 0, x, y, ctx->window_width-x, ctx->window_height-y);
    if ( x)
      gdk_window_clear_area( window, 0, 0, 
		x, ctx->window_height);
    if ( y)
      gdk_window_clear_area( window, x, 0, 
		ctx->window_width, y);
  }
  else if ( x <= 0 && y <= 0) {
    gdk_draw_drawable( window, gc, window,
	-x, -y, 0, 0, ctx->window_width+x, ctx->window_height+y);
    if ( x)
      gdk_window_clear_area( window, 
	ctx->window_width+x, 0, ctx->window_width, ctx->window_height);
    if ( y)
      gdk_window_clear_area( window, 
	0, ctx->window_height+y, ctx->window_width+x, ctx->window_height);
  }
  else if ( x <= 0 && y >= 0) {
    gdk_draw_drawable( window, gc, window,
	-x, 0, 0, y, ctx->window_width+x, ctx->window_height-y);
    if ( x)
      gdk_window_clear_area( window, 
	ctx->window_width+x, 0, ctx->window_width, ctx->window_height);
    if ( y)
      gdk_window_clear_area( window, 
	0, 0, ctx->window_width+x, y);
  }
  else {
    gdk_draw_drawable( window, gc, window,
	0, -y, x, 0, ctx->window_width-x, ctx->window_height+y);
    if ( x)
      gdk_window_clear_area( window, 
	0, 0, x, ctx->window_height);
    if ( y)
      gdk_window_clear_area( window, 
	x, ctx->window_height+y, ctx->window_width, ctx->window_height);
  }
}

void FlowDrawGtk::clear_area( FlowCtx *ctx, int ll_x, int ur_x, int ll_y, int ur_y)
{
  if ( ctx->nodraw) return;

  gdk_window_clear_area( window, ll_x, ll_y, ur_x - ll_x, 
		ur_y - ll_y);
}

#if 0
int FlowDrawGtk::create_input( FlowCtx *ctx, int x, int y, char *text, int len,
	int idx, int width, int height, void *node, int number, void **data)
{
  XmFontList fontlist;
  XmFontListEntry entry;
  draw_sAnnotData *annot_data; 
  if ( ctx->nodraw) return 1;

  annot_data = (draw_sAnnotData *) calloc(1, sizeof( draw_sAnnotData));
  entry = XmFontListEntryCreate("tag", XmFONT_IS_FONT, 
		font_struct[0][idx]);
  fontlist = XmFontListAppendEntry( NULL, entry);

  annot_data->w = XtVaCreateGtkWidget( "input", xmTextGtkWidgetClass, toplevel,
	XmNvalue, text, XmNx, x, XmNy, y - height, XmNfontList, fontlist,
	XmNwidth, width + 20, XmNheight, height + 20, NULL);
  XtAddCallback( annot_data->w, XmNactivateCallback, draw_input_cb, annot_data);
  XtManageChild( annot_data->w);
  annot_data->node = (FlowArrayElem *)node;
  annot_data->number = number;
  annot_data->x = x;
  annot_data->y = y;
  annot_data->width = width;
  annot_data->height = height;
  annot_data->ctx = ctx;
  *data = (void *) annot_data;
  XtFree( (char *)entry);
  XmFontListFree( fontlist);
  return 1;
}

int FlowDrawGtk::close_input( FlowCtx *ctx, void *data)
{
  draw_sAnnotData *annot_data = (draw_sAnnotData *)data;
  XtDestroyGtkWidget( annot_data->w);
  free( (char *) annot_data);
  return 1;
}

int FlowDrawGtk::get_input( FlowCtx *ctx, void *data, char **text)
{
  draw_sAnnotData *annot_data = (draw_sAnnotData *)data;
  char *t;

  t = XmTextGetString( annot_data->w);

  *text = (char *) malloc( strlen(t) + 1);
  strcpy( *text, t);
  XtFree( t);
  return 1;
}

static void draw_input_cb( GtkWidget *w, XtPointer client_data, XtPointer call_data)
{
  draw_sAnnotData *data = (draw_sAnnotData *)client_data;
  char *text;

  /* Get input value */
  text = XmTextGetString( w);

  /* Call backcall function */
  data->ctx->annotation_input_cb( data->node, data->number, text);
  XtFree( text);
}

void FlowDrawGtk::move_input( FlowCtx *ctx, void *data, int x, int y,
	flow_ePosition pos_type)
{
  draw_sAnnotData *annot_data = (draw_sAnnotData *)data;

  if ( pos_type == flow_ePosition_Relative) {
    if ( !x && !y)
      return;

    annot_data->x += x;
    annot_data->y += y;
  }
  else {
    if ( x == annot_data->x && y == annot_data->y)
      return;

    annot_data->x = x;
    annot_data->y = y;
  }
  XtMoveWidget( annot_data->w, annot_data->x, annot_data->y - annot_data->height);
}
#endif

void FlowDrawGtk::set_inputfocus( FlowCtx *ctx)
{
  gdk_window_focus( ((FlowDrawGtk *)ctx->fdraw)->window, GDK_CURRENT_TIME);
  // gtk_widget_grab_focus( toplevel);
}

void FlowDrawGtk::set_click_sensitivity( FlowCtx *ctx, int value)
{
  click_sensitivity = value;
}

void FlowDrawGtk::set_image_clip_mask( FlowCtx *ctx, flow_tPixmap pixmap, int x, int y)
{
  GdkPixmap *gpixmap = gdk_pixmap_foreign_new( (Pixmap) pixmap);
  gdk_gc_set_clip_mask( gcs[flow_eDrawType_Line][0], 
		gpixmap);
  gdk_gc_set_clip_origin( gcs[flow_eDrawType_Line][0], x, y);
}

void FlowDrawGtk::reset_image_clip_mask( FlowCtx *ctx)
{
  gdk_gc_set_clip_mask( gcs[flow_eDrawType_Line][0], NULL);
  gdk_gc_set_clip_origin( gcs[flow_eDrawType_Line][0], 0, 0);
}

void FlowDrawGtk::set_white_background( FlowCtx *ctx)
{
  // background.red = int( 0.9 * 65534);
  // background.green = int( 0.9 * 65534);
  // background.blue = int( 0.9 * 65534);

  // if ( !gdk_colormap_alloc_color( colormap, &background, TRUE, TRUE))
  //   return;

  background = flow_allocate_color( this, "white");

  // Change erase gcs
  for ( int i = 0; i < DRAW_TYPE_SIZE; i++) {
    gdk_gc_set_foreground( gcs[flow_eDrawType_LineErase][i], &background);
    gdk_gc_set_background( gcs[flow_eDrawType_LineErase][i], &background);
    gdk_gc_set_foreground( gcs[flow_eDrawType_TextHelveticaErase][i], &background);
    gdk_gc_set_background( gcs[flow_eDrawType_TextHelveticaErase][i], &background);
    gdk_gc_set_foreground( gcs[flow_eDrawType_TextHelveticaEraseBold][i], &background);
    gdk_gc_set_background( gcs[flow_eDrawType_TextHelveticaEraseBold][i], &background);
  }
  gtk_widget_modify_bg( toplevel, GTK_STATE_NORMAL, &background);
}

#if 0
void draw_set_widget_inputfocus( GtkWidget *w)
{
  XSetInputFocus( XtDisplay(w), XtWindow(w), 
		RevertToParent, CurrentTime);
}
#endif

int FlowDrawGtk::get_font_idx( int gc_type)
{
  int font_idx;

  switch( gc_type) {
  case flow_eDrawType_TextHelvetica:
  case flow_eDrawType_TextHelveticaErase:
    font_idx = draw_eFont_Helvetica;
    break;
  case flow_eDrawType_TextHelveticaBold:
  case flow_eDrawType_TextHelveticaEraseBold:
    font_idx = draw_eFont_HelveticaBold;
    break;
  default:
    font_idx = draw_eFont_Helvetica;
  }

  return font_idx;
}

int FlowDrawGtk::image_get_width( flow_tImImage image)
{
  return gdk_pixbuf_get_width( (GdkPixbuf *)image);
}

int FlowDrawGtk::image_get_height( flow_tImImage image)
{
  return gdk_pixbuf_get_height( (GdkPixbuf *)image);
}

void FlowDrawGtk::image_scale( float scale, flow_tImImage orig_im, flow_tImImage *im, 
			       flow_tPixmap *im_pixmap, flow_tPixmap *im_mask)
{
  int current_width = int( scale * gdk_pixbuf_get_width((GdkPixbuf *)orig_im));
  int current_height = int( scale * gdk_pixbuf_get_height((GdkPixbuf *)orig_im));

  if ( *im)
    gdk_pixbuf_unref( (GdkPixbuf *)*im);
  *im = gdk_pixbuf_scale_simple( (GdkPixbuf *)orig_im, current_width, current_height, GDK_INTERP_NEAREST);
}

int FlowDrawGtk::image_load( const char *imagefile, float scale, float nav_scale,
			     flow_tImImage *orig_im, flow_tImImage *im, 
			     flow_tPixmap *im_pixmap, flow_tPixmap *im_mask,
			     flow_tPixmap *im_nav_pixmap, flow_tPixmap *im_nav_mask)
{
  pwr_tFileName filename;
  bool found = false;

  strcpy( filename, imagefile);

  // Look for file in $pwrp_exe, $pwr_doc/en_us/orm and $pwr_exe
  for ( int i = 0; i < 3; i++) {
    switch( i) {
    case 0: strcpy( filename, "$pwrp_exe/");  break;
    case 1: strcpy( filename, "$pwr_doc/en_us/orm/");  break;
    case 2: strcpy( filename, "$pwr_exe/");  break;
    }
    strcat( filename, imagefile);
    dcli_translate_filename( filename, filename);
    FILE *fp = fopen( filename, "r");
    if ( !fp)
      continue;
    fclose( fp);
    found = true;
    break;
  }
  if ( !found)
    return 0;

  if ( *im)
    gdk_pixbuf_unref( (GdkPixbuf *)*im);
  *orig_im = (flow_tImImage *) gdk_pixbuf_new_from_file( filename, 0);

  int width = int( scale * gdk_pixbuf_get_width( (GdkPixbuf *)*orig_im));
  int height = int( scale * gdk_pixbuf_get_height( (GdkPixbuf *)*orig_im));

  *im = (flow_tImImage *) gdk_pixbuf_scale_simple( (GdkPixbuf *)*orig_im, width, height, GDK_INTERP_NEAREST);
  return 1;
}
