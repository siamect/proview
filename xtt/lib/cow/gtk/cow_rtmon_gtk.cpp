/* 
 * Proview   $Id: co_rtmon_gtk.cpp,v 1.6 2008-10-31 12:51:30 claes Exp $
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
 */

/* co_rtmon_gtk.cpp -- Console log in statusmon */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_syi.h"
#include "rt_gdh.h"

#include "co_lng.h"
#include "cow_wow_gtk.h"
#include "cow_msgwindow_gtk.h"
#include "cow_rtmon.h"
#include "co_dcli.h"
#include "rt_pwr_msg.h"
#include "cow_rtmon_gtk.h"
#include "cow_statusmon_nodelistnav_gtk.h"

static gboolean rtmon_scan( void *data);

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  RtMonGtk::activate_exit( w, data);
  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

RtMonGtk::RtMonGtk( void *rtmon_parent_ctx,
		    GtkWidget *rtmon_parent_wid,
		    const char *rtmon_name,
		    const char *rtmon_display,
		    pwr_tStatus *status) :
  RtMon( rtmon_parent_ctx, rtmon_name, rtmon_display, status), parent_wid(rtmon_parent_wid),
  clock_cursor(0), timerid(0), old_status(9999)
{
  char nodename[80];
  char title[200];
  pwr_tStatus sts;
 
  syi_NodeName( &sts, nodename, sizeof(nodename));
  strcpy( title, nodename);
  strcat( title, " ");
  strcat( title, rtmon_name);

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 340,
			   "default-width", 480,
			   "title", title,
			   NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);

  // Menu
  // Accelerators
  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File entry
  GtkWidget *file_close = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Close"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_close), 
				 gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_close, "activate", G_CALLBACK(activate_exit), this);
  gtk_widget_add_accelerator( file_close, "activate", accel_g,
			      'w', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  // GtkWidget *file_reset = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Reset Runtime"));
  // g_signal_connect(file_reset, "activate", G_CALLBACK(activate_reset), this);

  file_xtt = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Start Runtime Navigator"));
  g_signal_connect(file_xtt, "activate", G_CALLBACK(activate_xtt), this);

  file_op = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Start Operator Environment"));
  g_signal_connect(file_op, "activate", G_CALLBACK(activate_op), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  // gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_reset);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_xtt);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_op);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_File"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // View menu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _In"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_in), 
				 gtk_image_new_from_stock( "gtk-zoom-in", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Out"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_out), 
				 gtk_image_new_from_stock( "gtk-zoom-out", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Reset"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_reset), 
				 gtk_image_new_from_stock( "gtk-zoom-100", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(activate_zoom_reset), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);

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

  nodelistnav = new NodelistNavGtk( this, vbox, 0, nodename, nodelist_eMode_SystemStatus,
				    0, 0, &nodelistnav_widget);

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  pwr_tFileName fname;
  GtkWidget *tools_zoom_in = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
		     gtk_image_new_from_file( fname));

  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(activate_zoom_in), this);
  g_object_set( tools_zoom_in, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_in,CoWowGtk::translate_utf8("Zoom in"), "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(activate_zoom_out), this);
  g_object_set( tools_zoom_out, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_out,CoWowGtk::translate_utf8("Zoom out"), "");

  GtkWidget *tools_zoom_reset = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_reset.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_reset), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_reset, "clicked", G_CALLBACK(activate_zoom_reset), this);
  g_object_set( tools_zoom_reset, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_reset,CoWowGtk::translate_utf8("Zoom reset"), "");

  tools_xtt = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_navigator.png");
  gtk_container_add( GTK_CONTAINER(tools_xtt), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_xtt, "clicked", G_CALLBACK(activate_xtt), this);
  g_object_set( tools_xtt, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_xtt,CoWowGtk::translate_utf8("Start Runtime Navigator"), "");

  tools_op = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_op.png");
  gtk_container_add( GTK_CONTAINER(tools_op), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_op, "clicked", G_CALLBACK(activate_op), this);
  g_object_set( tools_op, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_op,CoWowGtk::translate_utf8("Start Operator Environment"), "");

  // Button box 
  dcli_translate_filename( fname, "$pwr_exe/proview_icon2.png");
  bbox_image = gtk_image_new_from_file( fname);
  g_object_set( bbox_image,
		"xalign", 0.5,
		"yalign", 0.3,
		NULL);

  dcli_translate_filename( fname, "$pwr_exe/proview_icon2_gray.png");
  bbox_image_gray = gtk_image_new_from_file( fname);
  g_object_set( bbox_image_gray,
		"xalign", 0.5,
		"yalign", 0.3,
		NULL);

  bbox_start = gtk_button_new_with_label( "Start Runtime");
  gtk_widget_set_size_request( bbox_start, 170, 25);
  g_object_set( bbox_start, "can-focus", FALSE, NULL);
  g_signal_connect( bbox_start, "clicked", 
  		    G_CALLBACK(activate_start), this);

  bbox_restart = gtk_button_new_with_label( "Restart Runtime");
  gtk_widget_set_size_request( bbox_restart, 170, 25);
  g_object_set( bbox_restart, "can-focus", FALSE, NULL);
  g_signal_connect( bbox_restart, "clicked", 
  		    G_CALLBACK(activate_restart), this);

  bbox_stop = gtk_button_new_with_label( "Stop Runtime");
  gtk_widget_set_size_request( bbox_stop, 170, 25);
  g_object_set( bbox_stop, "can-focus", FALSE, NULL);
  g_signal_connect( bbox_stop, "clicked", 
  		    G_CALLBACK(activate_stop), this);

  lbox_reset = gtk_button_new_with_label( "Reset Runtime");
  gtk_widget_set_size_request( lbox_reset, 170, 25);
  g_object_set( lbox_reset, "can-focus", FALSE, NULL);
  g_signal_connect( lbox_reset, "clicked", 
  		    G_CALLBACK(activate_reset), this);

  bbox_label = gtk_label_new( "Down");
  bbox_label_eb = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(bbox_label_eb), bbox_label);
  gtk_widget_set_size_request( bbox_label_eb, 170, 25);

  GtkWidget *bbox_label_fr = gtk_frame_new( 0);
  gtk_container_add( GTK_CONTAINER(bbox_label_fr), bbox_label_eb);

  GtkWidget *bbox = gtk_vbox_new( TRUE, 0);
  gtk_box_pack_start( GTK_BOX(bbox), bbox_start, FALSE, FALSE, 5);
  gtk_box_pack_start( GTK_BOX(bbox), bbox_restart, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(bbox), bbox_stop, FALSE, FALSE, 5);

  GtkWidget *lbox = gtk_vbox_new( FALSE, 0);
  g_object_set( lbox, "spacing", 33, NULL);
  gtk_box_pack_start( GTK_BOX(lbox), bbox_label_fr, FALSE, FALSE, 5);
  gtk_box_pack_end( GTK_BOX(lbox), lbox_reset, FALSE, FALSE, 5);

  GtkWidget *bbox_vbox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(bbox_vbox), bbox_image, FALSE, FALSE, 30);
  gtk_box_pack_start( GTK_BOX(bbox_vbox), bbox_image_gray, FALSE, FALSE, 30);
  gtk_box_pack_start( GTK_BOX(bbox_vbox), bbox, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(bbox_vbox), lbox, FALSE, FALSE, 15);


  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(bbox_vbox), FALSE, FALSE, 10);
  gtk_box_pack_end( GTK_BOX(vbox), GTK_WIDGET(nodelistnav_widget), TRUE, TRUE, 0);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);
  gtk_widget_show_all( toplevel);

  g_object_set( bbox_image, "visible", FALSE, NULL);

  wow = new CoWowGtk( toplevel);

  timerid = g_timeout_add( 1000, rtmon_scan, this);

  *status = 1;
}

static gboolean rtmon_scan( void *data)
{
  RtMonGtk *rtmon = (RtMonGtk *)data;

  if ( rtmon->nodelistnav->node_list[0].item->data.SystemStatus != rtmon->old_status) {
    GdkColor color;
    char text[20];
    char color_name[20];
    pwr_tStatus sts = rtmon->nodelistnav->node_list[0].item->data.SystemStatus;

    if ( sts == PWR__SRVCONNECTION) {
      strcpy( color_name, "Gray");
      strcpy( text, "Down");
      gtk_widget_set_sensitive( rtmon->bbox_start, TRUE);
      gtk_widget_set_sensitive( rtmon->bbox_restart, FALSE);
      gtk_widget_set_sensitive( rtmon->bbox_stop, FALSE);
      gtk_widget_set_sensitive( rtmon->tools_xtt, FALSE);
      gtk_widget_set_sensitive( rtmon->tools_op, FALSE);
      gtk_widget_set_sensitive( rtmon->file_xtt, FALSE);
      gtk_widget_set_sensitive( rtmon->file_op, FALSE);

      g_object_set( rtmon->bbox_image, "visible", FALSE, NULL);
      g_object_set( rtmon->bbox_image_gray, "visible", TRUE, NULL);
    }
    else {
      if ( ((sts & 7) == 2) || ((sts & 7) == 4)) {
	strcpy( color_name, "Red");
	strcpy( text, "Running");      
      }
      else if ( (sts & 7) == 0) {
	strcpy( color_name, "Yellow");
	strcpy( text, "Running");      
      }
      else {
	strcpy( color_name, "Green");
	strcpy( text, "Running");      
      }
      gtk_widget_set_sensitive( rtmon->bbox_start, FALSE);
      gtk_widget_set_sensitive( rtmon->bbox_restart, TRUE);
      gtk_widget_set_sensitive( rtmon->bbox_stop, TRUE);
      gtk_widget_set_sensitive( rtmon->tools_xtt, TRUE);
      gtk_widget_set_sensitive( rtmon->tools_op, TRUE);
      gtk_widget_set_sensitive( rtmon->file_xtt, TRUE);
      gtk_widget_set_sensitive( rtmon->file_op, TRUE);
      g_object_set( rtmon->bbox_image, "visible", TRUE, NULL);
      g_object_set( rtmon->bbox_image_gray, "visible", FALSE, NULL);
    }

    gdk_color_parse( color_name, &color);
    gtk_widget_modify_bg( rtmon->bbox_label_eb, GTK_STATE_NORMAL, &color);
      
    gtk_label_set_text( GTK_LABEL(rtmon->bbox_label), text);

    rtmon->old_status = sts;
  }
  rtmon->timerid = g_timeout_add( 1000, rtmon_scan, data);
  return FALSE;
}


//
//  Delete rtmon
//
RtMonGtk::~RtMonGtk()
{
  if ( timerid)
    g_source_remove( timerid);
  free_cursor();
  delete nodelistnav;
  gtk_widget_destroy( toplevel);
}

void RtMonGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

void RtMonGtk::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = gdk_cursor_new_for_display( gtk_widget_get_display( toplevel),
					       GDK_WATCH);

  gdk_window_set_cursor( toplevel->window, clock_cursor);
  gdk_display_flush( gtk_widget_get_display( toplevel));
}

void RtMonGtk::reset_cursor()
{
  gdk_window_set_cursor( toplevel->window, NULL);
}

void RtMonGtk::free_cursor()
{
  if (clock_cursor)
    gdk_cursor_unref( clock_cursor);
}

gboolean RtMonGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  RtMonGtk *rtmon = (RtMonGtk *)data;

  if ( rtmon && rtmon->rtmon_displayed)
    rtmon->nodelistnav->set_input_focus();

  return FALSE;
}

void RtMonGtk::activate_exit( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  if ( rtmon->close_cb)
    (rtmon->close_cb)( rtmon->parent_ctx);
  else
    delete rtmon;
}

void RtMonGtk::activate_start( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->activate_start();
}

void RtMonGtk::activate_restart( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->activate_restart();
}

void RtMonGtk::activate_stop( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->activate_stop();
}

void RtMonGtk::activate_reset( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->activate_reset();
}

void RtMonGtk::activate_xtt( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->activate_xtt();
}

void RtMonGtk::activate_op( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->activate_op();
}

void RtMonGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->nodelistnav->zoom( 1.2);
}

void RtMonGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->nodelistnav->zoom( 5.0/6);
}

void RtMonGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->nodelistnav->unzoom();
}

void RtMonGtk::activate_help( GtkWidget *w, gpointer data)
{
  RtMon *rtmon = (RtMon *)data;

  rtmon->activate_help();
}




