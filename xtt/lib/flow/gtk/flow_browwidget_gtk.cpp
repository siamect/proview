/* 
 * Proview   $Id: flow_browwidget_gtk.cpp,v 1.2 2007-01-11 11:40:30 claes Exp $
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

#include <stdlib.h>
#include "glow_std.h"

#include "flow.h"
#include "flow_ctx.h"
#include "flow_browctx.h"
#include "flow_draw.h"
#include "flow_draw_gtk.h"
#include "flow_browwidget_gtk.h"

typedef struct _BrowWidgetGtk		BrowWidgetGtk;
typedef struct _BrowWidgetGtkClass	BrowWidgetGtkClass;

typedef struct {
	GtkWidget  	*brow;
	GtkWidget	*form;
	GtkWidget	*scroll_h;
	GtkWidget	*scroll_v;
	int		scroll_h_managed;
	int		scroll_v_managed;
	} browwidget_sScroll;

struct _BrowWidgetGtk {
  GtkDrawingArea parent;

  /* Private */
  void		*brow_ctx;
  void 		*draw_ctx;
  int		(*init_proc)(FlowCtx *ctx, void *clien_data);
  int		is_navigator;
  void		*client_data;
  GtkWidget	*main_brow_widget;
  GtkWidget    	*scroll_h;
  GtkWidget    	*scroll_v;
  GtkWidget    	*form;
  int		scroll_h_ignore;
  int		scroll_v_ignore;
};

struct _BrowWidgetGtkClass {
  GtkDrawingAreaClass parent_class;
};

G_DEFINE_TYPE( BrowWidgetGtk, browwidgetgtk, GTK_TYPE_DRAWING_AREA);

static void scroll_callback( flow_sScroll *data)
{
  browwidget_sScroll *scroll_data;

  scroll_data = (browwidget_sScroll *) data->scroll_data;

  if ( data->total_width <= data->window_width) {
    if ( data->offset_x == 0)
      data->total_width = data->window_width;
    if ( scroll_data->scroll_h_managed) {
      // Remove horizontal scrollbar
    }
  }
  else {
    if ( !scroll_data->scroll_h_managed) {
      // Insert horizontal scrollbar
    }
  }

  if ( data->total_height <= data->window_height) {
    if ( data->offset_y == 0)
      data->total_height = data->window_height;
    if ( scroll_data->scroll_v_managed) {
      // Remove vertical scrollbar
    }
  }
  else {
    if ( !scroll_data->scroll_v_managed) {
      // Insert vertical scrollbar
    }
  }
  if ( data->offset_x < 0) {
    data->total_width += -data->offset_x;
    data->offset_x = 0;
  }
  if ( data->offset_y < 0) {
    data->total_height += -data->offset_y;
    data->offset_y = 0;
  }
  if ( data->total_height < data->window_height + data->offset_y)
    data->total_height = data->window_height + data->offset_y;
  if ( data->total_width < data->window_width + data->offset_x)
    data->total_width = data->window_width + data->offset_x;
  if ( data->window_width < 1)
    data->window_width = 1;
  if ( data->window_height < 1)
    data->window_height = 1;

  if ( scroll_data->scroll_h_managed) {
    ((BrowWidgetGtk *)scroll_data->brow)->scroll_h_ignore = 1;
    g_object_set( ((GtkScrollbar *)scroll_data->scroll_h)->range.adjustment,
		 "upper", (gdouble)data->total_width,
		 "page-size", (gdouble)data->window_width,
		 "value", (gdouble)data->offset_x,
		 NULL);
    gtk_adjustment_changed( 
        ((GtkScrollbar *)scroll_data->scroll_h)->range.adjustment);
  }

  if ( scroll_data->scroll_v_managed) {
    ((BrowWidgetGtk *)scroll_data->brow)->scroll_v_ignore = 1;
    g_object_set( ((GtkScrollbar *)scroll_data->scroll_v)->range.adjustment,
		 "upper", (gdouble)data->total_height,
		 "page-size", (gdouble)data->window_height,
		  //  "value", (gdouble)data->offset_y,
		 NULL);
    gtk_adjustment_changed( 
        ((GtkScrollbar *)scroll_data->scroll_v)->range.adjustment);
    ((BrowWidgetGtk *)scroll_data->brow)->scroll_v_ignore = 1;
    gtk_range_set_value( GTK_RANGE(scroll_data->scroll_v), (gdouble)data->offset_y);
  }
}

static void scroll_h_action( 	GtkWidget      	*w,
				gpointer 	data)
{
  BrowWidgetGtk *broww = (BrowWidgetGtk *)data;
  if ( broww->scroll_h_ignore) {
    broww->scroll_h_ignore = 0;
    return;
  }

  BrowCtx *ctx = (BrowCtx *) broww->brow_ctx;
  gdouble value;
  g_object_get( w,
		"value", &value,
		NULL);
  flow_scroll_horizontal( ctx, int(value), 0);

}

static void scroll_v_action( 	GtkWidget 	*w,
				gpointer 	data)
{
  BrowWidgetGtk *broww = (BrowWidgetGtk *)data;

  if ( broww->scroll_v_ignore) {
    broww->scroll_v_ignore = 0;
    return;
  }
    
  BrowCtx *ctx = (BrowCtx *) broww->brow_ctx;
  gdouble value;
  g_object_get( w,
		"value", &value,
		NULL);
  flow_scroll_vertical( ctx, int(value), 0);
}

static int brow_init_proc( GtkWidget *w, FlowCtx *fctx, void *client_data)
{
  browwidget_sScroll *scroll_data;
  BrowCtx	*ctx;

  ctx = (BrowCtx *) ((BrowWidgetGtk *) w)->brow_ctx;

  if ( ((BrowWidgetGtk *) w)->scroll_h) {
    scroll_data = (browwidget_sScroll *) malloc( sizeof( browwidget_sScroll));
    scroll_data->brow = w;
    scroll_data->scroll_h = ((BrowWidgetGtk *) w)->scroll_h;
    scroll_data->scroll_v = ((BrowWidgetGtk *) w)->scroll_v;
    scroll_data->form = ((BrowWidgetGtk *) w)->form;
    scroll_data->scroll_h_managed = 1;
    scroll_data->scroll_v_managed = 1;

    ctx->register_scroll_callback( (void *) scroll_data, scroll_callback);
  }
  return (((BrowWidgetGtk *) w)->init_proc)( ctx, client_data);
}

static gboolean browwidgetgtk_expose( GtkWidget *flow, GdkEventExpose *event)
{
  ((FlowDrawGtk *)((BrowCtx *)((BrowWidgetGtk *)flow)->brow_ctx)->fdraw)->event_handler( 										(FlowCtx *)((BrowWidgetGtk *)flow)->brow_ctx, *(GdkEvent *)event);
  return TRUE;
}

static void browwidgetgtk_grab_focus( GtkWidget *flow)
{
  GTK_WIDGET_CLASS( browwidgetgtk_parent_class)->grab_focus( flow);
  gdk_window_focus( flow->window, GDK_CURRENT_TIME);
}

static gboolean browwidgetgtk_event( GtkWidget *flow, GdkEvent *event)
{

  if ( event->type == GDK_MOTION_NOTIFY) {
    gdk_display_flush( ((FlowDrawGtk *)((BrowCtx *)((BrowWidgetGtk *)flow)->brow_ctx)->fdraw)->display);
    GdkEvent *next = gdk_event_peek();
    if ( next && next->type == GDK_MOTION_NOTIFY) {
      gdk_event_free( next);
      return TRUE;
    }
    else if ( next)
      gdk_event_free( next);
  }

  ((FlowDrawGtk *)((BrowCtx *)((BrowWidgetGtk *)flow)->brow_ctx)->fdraw)->event_handler( (FlowCtx *)((BrowWidgetGtk *)flow)->brow_ctx, *event);
  return TRUE;
}

static void browwidgetgtk_realize( GtkWidget *widget)
{
  GdkWindowAttr attr;
  gint attr_mask;
  BrowWidgetGtk *brow;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (IS_BROWWIDGETGTK( widget));

  GTK_WIDGET_SET_FLAGS( widget, GTK_REALIZED);
  brow = BROWWIDGETGTK( widget);

  attr.x = widget->allocation.x;
  attr.y = widget->allocation.y;
  attr.width = widget->allocation.width;
  attr.height = widget->allocation.height;
  attr.wclass = GDK_INPUT_OUTPUT;
  attr.window_type = GDK_WINDOW_CHILD;
  attr.event_mask = gtk_widget_get_events( widget) |
    GDK_EXPOSURE_MASK | 
    GDK_BUTTON_PRESS_MASK | 
    GDK_BUTTON_RELEASE_MASK | 
    GDK_KEY_PRESS_MASK |
    GDK_POINTER_MOTION_MASK |
    GDK_BUTTON_MOTION_MASK |
    GDK_ENTER_NOTIFY_MASK |
    GDK_LEAVE_NOTIFY_MASK | 
    GDK_SCROLL_MASK;
  attr.visual = gtk_widget_get_visual( widget);
  attr.colormap = gtk_widget_get_colormap( widget);

  attr_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window = gdk_window_new( widget->parent->window, &attr, attr_mask);
  widget->style = gtk_style_attach( widget->style, widget->window);
  gdk_window_set_user_data( widget->window, widget);
  gtk_style_set_background( widget->style, widget->window, GTK_STATE_ACTIVE);

  GTK_WIDGET_SET_FLAGS( widget, GTK_CAN_FOCUS);

  if ( brow->is_navigator) {
    if ( !brow->brow_ctx) {
      BrowWidgetGtk *main_brow = (BrowWidgetGtk *) brow->main_brow_widget;

      brow->brow_ctx = main_brow->brow_ctx;
      brow->draw_ctx = main_brow->draw_ctx;
      ((FlowDrawGtk *)brow->draw_ctx)->init_nav( widget, brow->brow_ctx);
    }
  }
  else {
    if ( !brow->brow_ctx) {
      brow->draw_ctx = new FlowDrawGtk( widget, 
					&brow->brow_ctx, 
					brow_init_proc, 
					brow->client_data,
					flow_eCtxType_Brow);
    }
  }

}

static void browwidgetgtk_class_init( BrowWidgetGtkClass *klass)
{
  GtkWidgetClass *widget_class;
  widget_class = GTK_WIDGET_CLASS( klass);
  widget_class->realize = browwidgetgtk_realize;
  widget_class->expose_event = browwidgetgtk_expose;
  widget_class->event = browwidgetgtk_event;
  widget_class->grab_focus = browwidgetgtk_grab_focus;
}

static void browwidgetgtk_init( BrowWidgetGtk *flow)
{
}

GtkWidget *browwidgetgtk_new(
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data)
{
  BrowWidgetGtk *w;
  w =  (BrowWidgetGtk *) g_object_new( BROWWIDGETGTK_TYPE, NULL);
  w->init_proc = init_proc;
  w->brow_ctx = 0;
  w->is_navigator = 0;
  w->client_data = client_data;
  w->scroll_h = 0;
  w->scroll_v = 0;
  return (GtkWidget *) w;  
}

GtkWidget *scrolledbrowwidgetgtk_new(
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data, GtkWidget **browwidget)
{
  BrowWidgetGtk *w;

  GtkWidget *form = gtk_scrolled_window_new( NULL, NULL);

  w =  (BrowWidgetGtk *) g_object_new( BROWWIDGETGTK_TYPE, NULL);
  w->init_proc = init_proc;
  w->brow_ctx = 0;
  w->is_navigator = 0;
  w->client_data = client_data;
  w->scroll_h = gtk_scrolled_window_get_hscrollbar( GTK_SCROLLED_WINDOW(form));
  w->scroll_v = gtk_scrolled_window_get_vscrollbar( GTK_SCROLLED_WINDOW(form));
  w->scroll_h_ignore = 0;
  w->scroll_v_ignore = 0;
  w->form = form;
  *browwidget = GTK_WIDGET( w);

  g_signal_connect( ((GtkScrollbar *)w->scroll_h)->range.adjustment, 
		    "value-changed", G_CALLBACK(scroll_h_action), w);
  g_signal_connect( ((GtkScrollbar *)w->scroll_v)->range.adjustment, 
		    "value-changed", G_CALLBACK(scroll_v_action), w);

  GtkWidget *viewport = gtk_viewport_new( NULL, NULL);
  gtk_viewport_set_shadow_type( GTK_VIEWPORT(viewport), GTK_SHADOW_NONE);
  gtk_container_add( GTK_CONTAINER(viewport), GTK_WIDGET(w));
  gtk_container_add( GTK_CONTAINER(form), GTK_WIDGET(viewport));

  return (GtkWidget *) form;  
}

GtkWidget *brownavwidgetgtk_new( GtkWidget *main_brow)
{
  BrowWidgetGtk *w;
  w =  (BrowWidgetGtk *) g_object_new( BROWWIDGETGTK_TYPE, NULL);
  w->init_proc = 0;
  w->brow_ctx = 0;
  w->is_navigator = 1;
  w->main_brow_widget = main_brow;
  w->client_data = 0;
  w->scroll_h = 0;
  w->scroll_v = 0;
  w->scroll_h_ignore = 0;
  w->scroll_v_ignore = 0;
  return (GtkWidget *) w;  
}

void browwidgetgtk_modify_ctx( GtkWidget *w, void *ctx)
{
  ((BrowWidgetGtk *)w)->brow_ctx = ctx;
}

#if 0
GType browwidgetgtk_get_type(void)
{
  static GType browwidgetgtk_type = 0;

  if ( !browwidgetgtk_type) {
    static const GTypeInfo browwidgetgtk_info = {
      sizeof(BrowWidgetGtkClass), NULL, NULL, (GClassInitFunc)browwidgetgtk_class_init,
      NULL, NULL, sizeof(BrowWidgetGtk), 1, NULL, NULL};
    
    browwidgetgtk_type = g_type_register_static( G_TYPE_OBJECT, "BrowWidgetGtk", &browwidgetgtk_info, 
					   (GTypeFlags)0);  
  }
  return browwidgetgtk_type;
}

void browwidgetgtk_get_ctx( GtkWidget *w, void **ctx)
{
  *ctx = ((BrowWidgetGtk *)w)->brow_ctx;
}

void browwidgetgtk_modify_ctx( GtkWidget *w, void *ctx)
{
  ((BrowWidgetGtk *)w)->brow_ctx = ctx;
}

#endif
