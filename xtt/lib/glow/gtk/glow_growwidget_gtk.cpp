/* 
 * Proview   $Id: glow_growwidget_gtk.cpp,v 1.3 2007-02-01 07:10:33 claes Exp $
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
#include <math.h>
#include "glow_std.h"

#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>
#include "glow_growwidget_gtk.h"
#include "glow.h"
#include "glow_ctx.h"
#include "glow_growctx.h"
#include "glow_draw.h"
#include "glow_draw_gtk.h"

typedef struct _GrowWidgetGtk		GrowWidgetGtk;
typedef struct _GrowWidgetGtkClass	GrowWidgetGtkClass;

typedef struct {
	GtkWidget  	*grow;
	GtkWidget	*form;
	GtkWidget	*scroll_h;
	GtkWidget	*scroll_v;
	int		scroll_h_managed;
	int		scroll_v_managed;
	} growwidget_sScroll;

struct _GrowWidgetGtk {
  GtkDrawingArea parent;

  /* Private */
  void		*grow_ctx;
  void 		*draw_ctx;
  int		(*init_proc)(GlowCtx *ctx, void *clien_data);
  int		is_navigator;
  void		*client_data;
  GtkWidget	*main_grow_widget;
  GtkWidget    	*scroll_h;
  GtkWidget    	*scroll_v;
  GtkWidget    	*form;
  int		scroll_h_ignore;
  int		scroll_v_ignore;
  gdouble       scroll_h_value;
  gdouble       scroll_v_value;
};

struct _GrowWidgetGtkClass {
  GtkDrawingAreaClass parent_class;
};

G_DEFINE_TYPE( GrowWidgetGtk, growwidgetgtk, GTK_TYPE_DRAWING_AREA);

static void scroll_callback( glow_sScroll *data)
{
  growwidget_sScroll *scroll_data;

  scroll_data = (growwidget_sScroll *) data->scroll_data;

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
    ((GrowWidgetGtk *)scroll_data->grow)->scroll_h_ignore = 1;
    g_object_set( ((GtkScrollbar *)scroll_data->scroll_h)->range.adjustment,
		 "upper", (gdouble)data->total_width,
		 "page-size", (gdouble)data->window_width,
		 "value", (gdouble)data->offset_x,
		 NULL);
    gtk_adjustment_changed( 
        ((GtkScrollbar *)scroll_data->scroll_h)->range.adjustment);
    ((GrowWidgetGtk *)scroll_data->grow)->scroll_h_value = (gdouble)data->offset_x;
  }

  if ( scroll_data->scroll_v_managed) {
    ((GrowWidgetGtk *)scroll_data->grow)->scroll_v_ignore = 1;
    g_object_set( ((GtkScrollbar *)scroll_data->scroll_v)->range.adjustment,
		 "upper", (gdouble)data->total_height,
		 "page-size", (gdouble)data->window_height,
		 "value", (gdouble)data->offset_y,
		 NULL);
    gtk_adjustment_changed( 
        ((GtkScrollbar *)scroll_data->scroll_v)->range.adjustment);
    ((GrowWidgetGtk *)scroll_data->grow)->scroll_v_value = (gdouble)data->offset_y;
  }
}

static void scroll_h_action( 	GtkWidget      	*w,
				gpointer 	data)
{
  GrowWidgetGtk *groww = (GrowWidgetGtk *)data;
  if ( groww->scroll_h_ignore) {
    groww->scroll_h_ignore = 0;
    return;
  }

  GrowCtx *ctx = (GrowCtx *) groww->grow_ctx;
  gdouble value;
  value = gtk_range_get_value( GTK_RANGE(groww->scroll_h));

  if ( value == 0 && fabs(groww->scroll_h_value) > 2) {
    // Probably a resize that seems to set value to zero, set old value
    ctx->change_scrollbar();
    return;
  }
  groww->scroll_h_value = value;

  glow_scroll_horizontal( ctx, int(value), 0);

}

static void scroll_v_action( 	GtkWidget 	*w,
				gpointer 	data)
{
  GrowWidgetGtk *groww = (GrowWidgetGtk *)data;

  if ( groww->scroll_v_ignore) {
    groww->scroll_v_ignore = 0;
    return;
  }
    
  GrowCtx *ctx = (GrowCtx *) groww->grow_ctx;
  gdouble value;
  value = gtk_range_get_value( GTK_RANGE(groww->scroll_v));

  if ( value == 0 && fabs(groww->scroll_v_value) > 2) {
    // Probably a resize that seems to set value to zero, set old value
    ctx->change_scrollbar();
    return;
  }
  groww->scroll_v_value = value;

  glow_scroll_vertical( ctx, int(value), 0);
}

static int grow_init_proc( GtkWidget *w, GlowCtx *fctx, void *client_data)
{
  growwidget_sScroll *scroll_data;
  GrowCtx	*ctx;

  ctx = (GrowCtx *) ((GrowWidgetGtk *) w)->grow_ctx;

  if ( ((GrowWidgetGtk *) w)->scroll_h) {
    scroll_data = (growwidget_sScroll *) malloc( sizeof( growwidget_sScroll));
    scroll_data->grow = w;
    scroll_data->scroll_h = ((GrowWidgetGtk *) w)->scroll_h;
    scroll_data->scroll_v = ((GrowWidgetGtk *) w)->scroll_v;
    scroll_data->form = ((GrowWidgetGtk *) w)->form;
    scroll_data->scroll_h_managed = 1;
    scroll_data->scroll_v_managed = 1;

    ctx->register_scroll_callback( (void *) scroll_data, scroll_callback);
  }
  return (((GrowWidgetGtk *) w)->init_proc)( ctx, client_data);
}

static gboolean growwidgetgtk_expose( GtkWidget *glow, GdkEventExpose *event)
{
  ((GlowDrawGtk *)((GrowCtx *)((GrowWidgetGtk *)glow)->grow_ctx)->gdraw)->event_handler( 
					*(GdkEvent *)event);
  return TRUE;
}

static void growwidgetgtk_grab_focus( GtkWidget *glow)
{
  GTK_WIDGET_CLASS( growwidgetgtk_parent_class)->grab_focus( glow);
  gdk_window_focus( glow->window, GDK_CURRENT_TIME);
}

static void growwidgetgtk_destroy( GtkObject *object)
{
  GrowWidgetGtk *grow = (GrowWidgetGtk *)object;

  if ( grow->is_navigator) {
    ((GrowCtx *)grow->grow_ctx)->no_nav = 1;
  }
  GTK_OBJECT_CLASS( growwidgetgtk_parent_class)->destroy( object);
}


static gboolean growwidgetgtk_event( GtkWidget *glow, GdkEvent *event)
{
  if ( event->type == GDK_MOTION_NOTIFY) {
    gdk_display_flush( ((GlowDrawGtk *)((GrowCtx *)((GrowWidgetGtk *)glow)->grow_ctx)->gdraw)->display);
    GdkEvent *next = gdk_event_peek();
    if ( next && next->type == GDK_MOTION_NOTIFY) {
      gdk_event_free( next);
      return TRUE;
    }
    else if ( next)
      gdk_event_free( next);
  }

  ((GlowDrawGtk *)((GrowCtx *)((GrowWidgetGtk *)glow)->grow_ctx)->gdraw)->event_handler( *event);
  return TRUE;
}

static void growwidgetgtk_realize( GtkWidget *widget)
{
  GdkWindowAttr attr;
  gint attr_mask;
  GrowWidgetGtk *grow;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (IS_GROWWIDGETGTK( widget));

  GTK_WIDGET_SET_FLAGS( widget, GTK_REALIZED);
  grow = GROWWIDGETGTK( widget);

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
    GDK_LEAVE_NOTIFY_MASK;
  attr.visual = gtk_widget_get_visual( widget);
  attr.colormap = gtk_widget_get_colormap( widget);

  attr_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window = gdk_window_new( widget->parent->window, &attr, attr_mask);
  widget->style = gtk_style_attach( widget->style, widget->window);
  gdk_window_set_user_data( widget->window, widget);
  gtk_style_set_background( widget->style, widget->window, GTK_STATE_ACTIVE);

  GTK_WIDGET_SET_FLAGS( widget, GTK_CAN_FOCUS);

  if ( grow->is_navigator) {
    if ( !grow->grow_ctx) {
      GrowWidgetGtk *main_grow = (GrowWidgetGtk *) grow->main_grow_widget;

      grow->grow_ctx = main_grow->grow_ctx;
      grow->draw_ctx = main_grow->draw_ctx;
      ((GlowDrawGtk *)grow->draw_ctx)->init_nav( widget);
    }
  }
  else {
    if ( !grow->grow_ctx) {
      grow->draw_ctx = new GlowDrawGtk( widget, 
					&grow->grow_ctx, 
					grow_init_proc, 
					grow->client_data,
					glow_eCtxType_Grow);
    }
  }

}

static void growwidgetgtk_class_init( GrowWidgetGtkClass *klass)
{
  GtkWidgetClass *widget_class;
  GtkObjectClass *object_class;

  widget_class = GTK_WIDGET_CLASS( klass);
  object_class = GTK_OBJECT_CLASS(klass);
  widget_class->realize = growwidgetgtk_realize;
  widget_class->expose_event = growwidgetgtk_expose;
  widget_class->event = growwidgetgtk_event;
  widget_class->grab_focus = growwidgetgtk_grab_focus;
  object_class->destroy = growwidgetgtk_destroy;
}

static void growwidgetgtk_init( GrowWidgetGtk *glow)
{
}

GtkWidget *growwidgetgtk_new(
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data)
{
  GrowWidgetGtk *w;
  w =  (GrowWidgetGtk *) g_object_new( GROWWIDGETGTK_TYPE, NULL);
  w->init_proc = init_proc;
  w->grow_ctx = 0;
  w->is_navigator = 0;
  w->client_data = client_data;
  w->scroll_h = 0;
  w->scroll_v = 0;
  return (GtkWidget *) w;  
}

GtkWidget *scrolledgrowwidgetgtk_new(
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data, GtkWidget **growwidget)
{
  GrowWidgetGtk *w;

  GtkWidget *form = gtk_scrolled_window_new( NULL, NULL);

  w =  (GrowWidgetGtk *) g_object_new( GROWWIDGETGTK_TYPE, NULL);
  w->init_proc = init_proc;
  w->grow_ctx = 0;
  w->is_navigator = 0;
  w->client_data = client_data;
  w->scroll_h = gtk_scrolled_window_get_hscrollbar( GTK_SCROLLED_WINDOW(form));
  w->scroll_v = gtk_scrolled_window_get_vscrollbar( GTK_SCROLLED_WINDOW(form));
  w->scroll_h_ignore = 0;
  w->scroll_v_ignore = 0;
  w->form = form;
  *growwidget = GTK_WIDGET( w);

  g_signal_connect( ((GtkScrollbar *)w->scroll_h)->range.adjustment, 
		    "value-changed", G_CALLBACK(scroll_h_action), w);
  g_signal_connect( ((GtkScrollbar *)w->scroll_v)->range.adjustment, 
		    "value-changed", G_CALLBACK(scroll_v_action), w);

  GtkWidget *viewport = gtk_viewport_new( NULL, NULL);
  gtk_container_add( GTK_CONTAINER(viewport), GTK_WIDGET(w));
  gtk_container_add( GTK_CONTAINER(form), GTK_WIDGET(viewport));

  return (GtkWidget *) form;  
}

GtkWidget *grownavwidgetgtk_new( GtkWidget *main_grow)
{
  GrowWidgetGtk *w;
  w =  (GrowWidgetGtk *) g_object_new( GROWWIDGETGTK_TYPE, NULL);
  w->init_proc = 0;
  w->grow_ctx = 0;
  w->is_navigator = 1;
  w->main_grow_widget = main_grow;
  w->client_data = 0;
  w->scroll_h = 0;
  w->scroll_v = 0;
  w->scroll_h_ignore = 0;
  w->scroll_v_ignore = 0;
  return (GtkWidget *) w;  
}

#if 0
GType growwidgetgtk_get_type(void)
{
  static GType growwidgetgtk_type = 0;

  if ( !growwidgetgtk_type) {
    static const GTypeInfo growwidgetgtk_info = {
      sizeof(GrowWidgetGtkClass), NULL, NULL, (GClassInitFunc)growwidgetgtk_class_init,
      NULL, NULL, sizeof(GrowWidgetGtk), 1, NULL, NULL};
    
    growwidgetgtk_type = g_type_register_static( G_TYPE_OBJECT, "GrowWidgetGtk", &growwidgetgtk_info, 
					   (GTypeFlags)0);  
  }
  return growwidgetgtk_type;
}
#endif
