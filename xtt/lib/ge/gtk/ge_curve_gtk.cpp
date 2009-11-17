/* 
 * Proview   $Id: ge_curve_gtk.cpp,v 1.3 2008-07-17 11:21:25 claes Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <gtk/gtk.h>
#include "rt_load.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "glow.h"
#include "glow_curvectx.h"
#include "glow_curvewidget_gtk.h"
#include "glow_growctx.h"
#include "glow_growwidget_gtk.h"
#include "glow_curveapi.h"
#include "glow_msg.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "cow_wow_gtk.h"
#include "co_lng.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "ge_curve_gtk.h"
#include "ge_msg.h"

void GeCurveGtk::activate_exit( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve->activate_exit();
}

void GeCurveGtk::activate_configure( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve->activate_configure();
}

void GeCurveGtk::activate_print( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve->activate_print();
}

void GeCurveGtk::activate_zoomin( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve_Zoom( curve->growcurve_ctx, 2.0);
}

void GeCurveGtk::activate_zoomout( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve_Zoom( curve->growcurve_ctx, 0.5);
}

void GeCurveGtk::activate_zoomreset( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve_Unzoom( curve->growcurve_ctx);
}

void GeCurveGtk::activate_page_right( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve_Scroll( curve->growcurve_ctx, -0.9);
}

void GeCurveGtk::activate_page_left( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve_Scroll( curve->growcurve_ctx, 0.9);
}

void GeCurveGtk::activate_scroll_right( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve_Scroll( curve->growcurve_ctx, -0.1);
}

void GeCurveGtk::activate_scroll_left( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve_Scroll( curve->growcurve_ctx, 0.1);
  curve->set_inputfocus();
}

void GeCurveGtk::activate_higher_res( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  if ( curve->higher_res_cb)
    (curve->higher_res_cb)( curve->parent_ctx);
}

void GeCurveGtk::activate_lower_res( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  if ( curve->lower_res_cb)
    (curve->lower_res_cb)( curve->parent_ctx);
}

void GeCurveGtk::activate_background( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve->activate_background();
}

void GeCurveGtk::activate_showname( GtkWidget *w, gpointer data)
{
}

void GeCurveGtk::activate_filledcurves( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  curve->activate_filledcurves( set);
}

void GeCurveGtk::activate_xlimits( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve->open_minmax(0);
}

void GeCurveGtk::activate_help( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  curve->activate_help();
}

void GeCurveGtk::activate_minmax_ok( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;
  char *value;
  double min_value, max_value;
  int nr;

  value = gtk_editable_get_chars( GTK_EDITABLE(((GeCurveGtk *)curve)->minmax_textmin_widget), 0, -1);
  nr = sscanf( value, "%lf", &min_value);
  g_free( value);
  if ( nr != 1)
    return;

  value = gtk_editable_get_chars( GTK_EDITABLE(((GeCurveGtk *)curve)->minmax_textmax_widget), 0, -1);
  nr = sscanf( value, "%lf", &max_value);
  g_free( value);
  if ( nr != 1)
    return;

  curve->activate_minmax_ok( min_value, max_value);
  g_object_set( ((GeCurveGtk *)curve)->minmax_widget, "visible", FALSE, NULL);
}

void GeCurveGtk::activate_minmax_cancel( GtkWidget *w, gpointer data)
{
  GeCurve *curve = (GeCurve *)data;

  g_object_set( ((GeCurveGtk *)curve)->minmax_widget, "visible", FALSE, NULL);
}

void GeCurveGtk::enable_resolution_buttons()
{
  g_object_set( tools_higher_res, "visible", TRUE, NULL);
  g_object_set( tools_lower_res, "visible", TRUE, NULL);
}

void GeCurveGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

void GeCurveGtk::write_title( char *str)
{
  gtk_window_set_title( GTK_WINDOW(toplevel), str);
}

void GeCurveGtk::resize()
{
  int main_height, main_width, height, width;
  double zoom_y;

  gdk_drawable_get_size( growaxis_main_widget->window, &main_width, &main_height);

  curve_GetPreferedZoomY( growcurve_ctx, main_height, &zoom_y);
  grow_ZoomY( growaxis_ctx, zoom_y);
  grow_ZoomX( growaxis_ctx, zoom_y);

  width = int( zoom_y * axis_window_width);
  height = main_height;
  gtk_widget_set_size_request( growaxis_main_widget, width+4, height);
}

void GeCurveGtk::axis_set_width( int width)
{
  gint w = width;

  gtk_widget_set_size_request( growaxis_main_widget, w+4, -1);
}

void GeCurveGtk::open_minmax( int idx)
{
  char value_str[80];
  gint pos = 0;

  create_minmax_dialog();

  sprintf( value_str, "%f", cd->min_value_axis[idx]);
  gtk_editable_delete_text( GTK_EDITABLE(minmax_textmin_widget), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(minmax_textmin_widget), 
			    value_str, strlen(value_str), &pos);

  sprintf( value_str, "%f", cd->max_value_axis[idx]);
  gtk_editable_delete_text( GTK_EDITABLE(minmax_textmax_widget), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(minmax_textmax_widget), 
			    value_str, strlen(value_str), &pos);

  minmax_idx = idx;
}

void GeCurveGtk::set_inputfocus()
{
  gtk_widget_grab_focus( growcurve_main_widget);
}

GeCurveGtk::~GeCurveGtk()
{
  delete wow;
  if ( minmax_widget)
    gtk_widget_destroy( minmax_widget);
  gtk_widget_destroy( toplevel);
}

static gboolean delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  GeCurveGtk::activate_exit(w, data);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

gboolean GeCurveGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  GeCurveGtk *curve = (GeCurveGtk *)data;

  if ( curve->focustimer.disabled())
    return TRUE;
  
  curve->set_inputfocus();

  curve->focustimer.disable( 400);
  return FALSE;
}

GeCurveGtk::GeCurveGtk( void *gc_parent_ctx, 
			GtkWidget *parent_widget,
			char *curve_name,
			char *filename,
			GeCurveData *curve_data,
			int pos_right) :
  GeCurve( gc_parent_ctx, curve_name, filename, curve_data, pos_right),
  minmax_widget(0)
{
  const int	window_width = 900;
  const int    	window_height = 700;
  const int    	names_height = 150;
  const int    	nav_height = 120;
  pwr_tFileName fname;

  cdh_StrncpyCutOff( title, curve_name, sizeof(title), 1);

  // Gtk
  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", CoWowGtk::convert_utf8(title),
					 NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  GtkWidget *file_refresh = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Refresh"));
  g_signal_connect( file_refresh, "activate", 
		    G_CALLBACK(activate_configure), this);
  gtk_widget_add_accelerator( file_refresh, "activate", accel_g, 'r', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *file_print = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Print"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_print), 
				 gtk_image_new_from_stock( "gtk-print", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_print, "activate", G_CALLBACK(activate_print), this);

  GtkWidget *file_close = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Close"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_close), 
				 gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_close, "activate", G_CALLBACK(activate_exit), this);
  gtk_widget_add_accelerator( file_close, "activate", accel_g,'w', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_refresh);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_File"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // View menu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _In"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_in), 
				 gtk_image_new_from_stock( "gtk-zoom-in", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(activate_zoomin), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Out"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_out), 
				 gtk_image_new_from_stock( "gtk-zoom-out", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(activate_zoomout), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Reset"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_reset), 
				 gtk_image_new_from_stock( "gtk-zoom-100", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(activate_zoomreset), this);

  GtkWidget *view_background = gtk_check_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Background Color"));
  g_signal_connect( view_background, "activate", 
		    G_CALLBACK(activate_background), this);

  GtkWidget *view_showname = gtk_check_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Show Names"));
  g_signal_connect( view_showname, "activate", 
		    G_CALLBACK(activate_showname), this);
  gtk_widget_add_accelerator( view_showname, "activate", accel_g,
			      'w', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *view_filledcurves = gtk_check_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Filled Curves"));
  g_signal_connect( view_filledcurves, "activate", 
		    G_CALLBACK(activate_filledcurves), this);

  GtkWidget *view_xlimits = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_X-Axis Limits"));
  g_signal_connect( view_xlimits, "activate", 
		    G_CALLBACK(activate_xlimits), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_background);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_showname);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_filledcurves);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_xlimits);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_View"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Menu Help
  GtkWidget *help_help = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Help"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_help), 
				 gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
  g_signal_connect(help_help, "activate", G_CALLBACK(activate_help), this);
  gtk_widget_add_accelerator( help_help, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Help"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  GtkWidget *tools_zoom_in = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(activate_zoomin), this);
  g_object_set( tools_zoom_in, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_in, CoWowGtk::translate_utf8("Zoom in"), "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(activate_zoomout), this);
  g_object_set( tools_zoom_out, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_out, CoWowGtk::translate_utf8("Zoom out"), "");

  GtkWidget *tools_zoom_reset = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_reset.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_reset), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_reset, "clicked", G_CALLBACK(activate_zoomreset), this);
  g_object_set( tools_zoom_reset, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_reset, CoWowGtk::translate_utf8("Zoom reset"), "");

  GtkWidget *tools_page_left = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_page_left.png");
  gtk_container_add( GTK_CONTAINER(tools_page_left), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_page_left, "clicked", G_CALLBACK(activate_page_left), this);
  g_object_set( tools_page_left, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_page_left, CoWowGtk::translate_utf8("Page left"), "");

  GtkWidget *tools_scroll_left = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_scroll_left.png");
  gtk_container_add( GTK_CONTAINER(tools_scroll_left), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_scroll_left, "clicked", G_CALLBACK(activate_scroll_left), this);
  g_object_set( tools_scroll_left, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_scroll_left, CoWowGtk::translate_utf8("Scroll left"), "");

  GtkWidget *tools_scroll_right = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_scroll_right.png");
  gtk_container_add( GTK_CONTAINER(tools_scroll_right), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_scroll_right, "clicked", G_CALLBACK(activate_scroll_right), this);
  g_object_set( tools_scroll_right, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_scroll_right, CoWowGtk::translate_utf8("Scroll right"), "");

  GtkWidget *tools_page_right = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/ge_page_right.png");
  gtk_container_add( GTK_CONTAINER(tools_page_right), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_page_right, "clicked", G_CALLBACK(activate_page_right), this);
  g_object_set( tools_page_right, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_page_right, CoWowGtk::translate_utf8("Page right"), "");

  tools_higher_res = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_up.png");
  gtk_container_add( GTK_CONTAINER(tools_higher_res), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_higher_res, "clicked", G_CALLBACK(activate_higher_res), this);
  g_object_set( tools_higher_res, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_higher_res, CoWowGtk::translate_utf8("Higer resolution"), "");

  tools_lower_res = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_down.png");
  gtk_container_add( GTK_CONTAINER(tools_lower_res), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_lower_res, "clicked", G_CALLBACK(activate_lower_res), this);
  g_object_set( tools_lower_res, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_lower_res, CoWowGtk::translate_utf8("Lower resolution"), "");

  GtkWidget *w;
  grownames_main_widget = scrolledgrowwidgetgtk_new( init_grownames_cb, this, &w);

  growcurve_main_widget = curvewidgetgtk_new( init_growcurve_cb, this);

  growaxis_main_widget = growwidgetgtk_new( init_growaxis_cb, this);

  nav_widget = curvenavwidgetgtk_new( growcurve_main_widget);

  GtkWidget *hbox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox), growaxis_main_widget, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox), growcurve_main_widget, TRUE, TRUE, 0);
  gtk_widget_show_all( hbox);

  GtkWidget *vpaned1 = gtk_vpaned_new();
  GtkWidget *vpaned2 = gtk_vpaned_new();

  gtk_paned_add1( GTK_PANED(vpaned1), grownames_main_widget);
  gtk_paned_add2( GTK_PANED(vpaned1), vpaned2);
  gtk_widget_show_all( vpaned1);

  gtk_paned_add1( GTK_PANED(vpaned2), hbox);
  gtk_paned_add2( GTK_PANED(vpaned2), nav_widget);
  gtk_widget_show_all( vpaned2);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(vpaned1), TRUE, TRUE, 0);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  gtk_widget_show_all( toplevel);

  gtk_paned_set_position( GTK_PANED(vpaned1), names_height);
  gtk_paned_set_position( GTK_PANED(vpaned2), window_height - names_height - nav_height - 50);
  g_object_set( tools_higher_res, "visible", FALSE, NULL);
  g_object_set( tools_lower_res, "visible", FALSE, NULL);

  wow = new CoWowGtk( toplevel);
}

static gint minmax_delete_event( GtkWidget *w, GdkEvent *event, gpointer curve)
{
  g_object_set( ((GeCurveGtk *)curve)->minmax_widget, "visible", FALSE, NULL);
  return TRUE;
}

void GeCurveGtk::create_minmax_dialog()
{
  if ( minmax_widget) {
    g_object_set( minmax_widget, "visible", TRUE, NULL);
    return;
  }

  // Create an input dialog
  minmax_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 150,
			   "default-width", 350,
			   "title", "Axis Limits",
			   NULL);
  g_signal_connect( minmax_widget, "delete_event", G_CALLBACK(minmax_delete_event), this);

  minmax_textmin_widget = gtk_entry_new();
  GtkWidget *min_label = gtk_label_new(CoWowGtk::translate_utf8("MinValue"));
  gtk_widget_set_size_request( min_label, 100, -1);
  minmax_textmax_widget = gtk_entry_new();
  g_signal_connect( minmax_textmax_widget, "activate",
  		    G_CALLBACK(activate_minmax_ok), this);
  GtkWidget *max_label = gtk_label_new(CoWowGtk::translate_utf8("MaxValue"));
  gtk_widget_set_size_request( max_label, 100, -1);

  GtkWidget *minmax_ok = gtk_button_new_with_label( CoWowGtk::translate_utf8("Ok"));
  gtk_widget_set_size_request( minmax_ok, 70, 25);
  g_signal_connect( minmax_ok, "clicked", 
  		    G_CALLBACK(activate_minmax_ok), this);
  GtkWidget *minmax_cancel = gtk_button_new_with_label( CoWowGtk::translate_utf8("Cancel"));
  gtk_widget_set_size_request( minmax_cancel, 70, 25);
  g_signal_connect( minmax_cancel, "clicked", 
  		    G_CALLBACK(activate_minmax_cancel), this);

  GtkWidget *minmax_hbox1 = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(minmax_hbox1), min_label, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(minmax_hbox1), minmax_textmin_widget, TRUE, TRUE, 30);

  GtkWidget *minmax_hbox2 = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(minmax_hbox2), max_label, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(minmax_hbox2), minmax_textmax_widget, TRUE, TRUE, 30);

  GtkWidget *minmax_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(minmax_hboxbuttons), minmax_ok, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(minmax_hboxbuttons), minmax_cancel, FALSE, FALSE, 0);

  GtkWidget *minmax_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(minmax_vbox), minmax_hbox1, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(minmax_vbox), minmax_hbox2, TRUE, TRUE, 15);
  gtk_box_pack_start( GTK_BOX(minmax_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(minmax_vbox), minmax_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(minmax_widget), minmax_vbox);

  gtk_widget_show_all( minmax_widget);
}
