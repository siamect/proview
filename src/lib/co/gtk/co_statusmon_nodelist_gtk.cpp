/* 
 * Proview   $Id: co_statusmon_nodelist_gtk.cpp,v 1.8 2008-10-31 12:51:30 claes Exp $
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

/* co_statusmon_nodelist_gtk.cpp -- Console log in statusmon */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh.h"

#include "co_lng.h"
#include "co_dcli.h"
#include "co_wow_gtk.h"
#include "co_msgwindow_gtk.h"
#include "co_statusmon_nodelist.h"
#include "rt_xnav_msg.h"
#include "co_statusmon_nodelist_gtk.h"
#include "co_statusmon_nodelistnav_gtk.h"

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  NodelistGtk::activate_exit( w, data);
  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

NodelistGtk::NodelistGtk( void *nodelist_parent_ctx,
			  GtkWidget *nodelist_parent_wid,
			  const char *nodelist_name, int nodelist_mode, 
			  int nodelist_view_node_descr, int msgw_pop, 
			  pwr_tStatus *status) :
  Nodelist( nodelist_parent_ctx, nodelist_name, nodelist_mode, 
	    nodelist_view_node_descr, status), 
  parent_wid(nodelist_parent_wid), clock_cursor(0), india_widget(0)
{
  pwr_tStatus sts;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 600,
			   "default-width", 700,
			   "title", nodelist_name,
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

  GtkWidget *file_add_node = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Add Node"));
  g_signal_connect(file_add_node, "activate", G_CALLBACK(activate_add_node), this);

  GtkWidget *file_remove_node = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Remove Node"));
  g_signal_connect(file_remove_node, "activate", G_CALLBACK(activate_remove_node), this);

  GtkWidget *file_open_xtt = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Open _Runtime Navigator"));
  g_signal_connect(file_open_xtt, "activate", G_CALLBACK(activate_open_xtt), this);

  GtkWidget *file_open_opplace = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Open Operatorplace"));
  g_signal_connect(file_open_opplace, "activate", G_CALLBACK(activate_open_opplace), this);

  GtkWidget *file_open_rtmon = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Open Runtime Monitor"));
  g_signal_connect(file_open_rtmon, "activate", G_CALLBACK(activate_open_rtmon), this);

  GtkWidget *file_save = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Save Configuration"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_save), 
				 gtk_image_new_from_stock( "gtk-save", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_save, "activate", G_CALLBACK(activate_save), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_save);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_add_node);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_remove_node);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_open_xtt);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_open_opplace);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_open_rtmon);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_File"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // View menu
  GtkWidget *view_show_events = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Show _Events"));
  g_signal_connect(view_show_events, "activate", G_CALLBACK(activate_show_events), this);

  GtkWidget *view_pop_events = gtk_check_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Pop Events"));
  g_signal_connect(view_pop_events, "activate", G_CALLBACK(activate_pop_events), this);

  // Submenu Remote GUI
  GSList *rg_group = NULL;
  GtkWidget *view_gui_gtk = gtk_radio_menu_item_new_with_mnemonic( rg_group, "_GTK");
  rg_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_gui_gtk));
  g_signal_connect( view_gui_gtk, "activate", 
		    G_CALLBACK(activate_gui_gtk), this);

  GtkWidget *view_gui_motif = gtk_radio_menu_item_new_with_mnemonic( rg_group, "_Motif");
  rg_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_gui_motif));
  g_signal_connect( view_gui_motif, "activate", 
		    G_CALLBACK(activate_gui_motif), this);


  GtkWidget *view_gui = gtk_menu_item_new_with_mnemonic( "_Remote GUI");
  GtkMenu *view_gui_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_gui_menu), view_gui_gtk);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_gui_menu), view_gui_motif);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_gui),
			    GTK_WIDGET(view_gui_menu));


  // Submenu Mode
  GSList *mode_group = NULL;
  GtkWidget *view_mode_systemstatus = gtk_radio_menu_item_new_with_mnemonic( mode_group, "_SystemStatus");
  mode_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_mode_systemstatus));
  g_signal_connect( view_mode_systemstatus, "activate", 
		    G_CALLBACK(activate_mode_systemstatus), this);

  GtkWidget *view_mode_userstatus1 = gtk_radio_menu_item_new_with_mnemonic( mode_group, "UserStatus_1");
  mode_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_mode_userstatus1));
  g_signal_connect( view_mode_userstatus1, "activate", 
		    G_CALLBACK(activate_mode_userstatus1), this);

  GtkWidget *view_mode_userstatus2 = gtk_radio_menu_item_new_with_mnemonic( mode_group, "UserStatus_2");
  mode_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_mode_userstatus2));
  g_signal_connect( view_mode_userstatus2, "activate", 
		    G_CALLBACK(activate_mode_userstatus2), this);

  GtkWidget *view_mode_userstatus3 = gtk_radio_menu_item_new_with_mnemonic( mode_group, "UserStatus_3");
  mode_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_mode_userstatus3));
  g_signal_connect( view_mode_userstatus3, "activate", 
		    G_CALLBACK(activate_mode_userstatus3), this);

  GtkWidget *view_mode_userstatus4 = gtk_radio_menu_item_new_with_mnemonic( mode_group, "UserStatus_4");
  mode_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_mode_userstatus4));
  g_signal_connect( view_mode_userstatus4, "activate", 
		    G_CALLBACK(activate_mode_userstatus4), this);

  GtkWidget *view_mode_userstatus5 = gtk_radio_menu_item_new_with_mnemonic( mode_group, "UserStatus_5");
  mode_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_mode_userstatus5));
  g_signal_connect( view_mode_userstatus5, "activate", 
		    G_CALLBACK(activate_mode_userstatus5), this);


  GtkWidget *view_mode = gtk_menu_item_new_with_mnemonic( "_Mode");
  GtkMenu *view_mode_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_mode_menu), view_mode_systemstatus);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_mode_menu), view_mode_userstatus1);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_mode_menu), view_mode_userstatus2);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_mode_menu), view_mode_userstatus3);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_mode_menu), view_mode_userstatus4);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_mode_menu), view_mode_userstatus5);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_mode),
			    GTK_WIDGET(view_mode_menu));


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
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_show_events);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_pop_events);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_gui);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_mode);
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

  msg_window = new MsgWindowGtk( this, toplevel, "Status Events", &sts);
  msg_window->find_wnav_cb = find_node_cb;
  msg_window->msg( 'I', "Status Montitor started");
  
  nodelistnav = new NodelistNavGtk( this, vbox, msg_window, 0, mode, view_node_descr,
				    msgw_pop, &nodelistnav_widget);

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  GtkWidget *tools_save = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_save), 
	  gtk_image_new_from_stock( "gtk-save", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_save, "clicked", G_CALLBACK(activate_save), this);
  g_object_set( tools_save, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_save, CoWowGtk::translate_utf8("Save configuration"), "");

  GtkWidget *tools_add_node = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_add_node), 
	  gtk_image_new_from_stock( "gtk-add", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_add_node, "clicked", G_CALLBACK(activate_add_node), this);
  g_object_set( tools_add_node, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_add_node,CoWowGtk::translate_utf8("Add node"), "");

  GtkWidget *tools_remove_node = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_remove_node), 
	  gtk_image_new_from_stock( "gtk-remove", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_remove_node, "clicked", G_CALLBACK(activate_remove_node), this);
  g_object_set( tools_remove_node, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_remove_node,CoWowGtk::translate_utf8("Remove node"), "");

  pwr_tFileName fname;
  GtkWidget *tools_xtt = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_navigator.png");
  gtk_container_add( GTK_CONTAINER(tools_xtt), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_xtt, "clicked", G_CALLBACK(activate_open_xtt), this);
  g_object_set( tools_xtt, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_xtt,CoWowGtk::translate_utf8("Start Runtime Navigator on selected node"), "");

  GtkWidget *tools_op = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_op.png");
  gtk_container_add( GTK_CONTAINER(tools_op), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_op, "clicked", G_CALLBACK(activate_open_opplace), this);
  g_object_set( tools_op, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_op,CoWowGtk::translate_utf8("Start Operatorplace on selected node"), "");

  GtkWidget *tools_rtmon = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_rtmon.png");
  gtk_container_add( GTK_CONTAINER(tools_rtmon), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_rtmon, "clicked", G_CALLBACK(activate_open_rtmon), this);
  g_object_set( tools_rtmon, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_rtmon,CoWowGtk::translate_utf8("Start Runtime Monitor on selected node"), "");

  GtkWidget *tools_zoom_in = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
	  gtk_image_new_from_stock( "gtk-zoom-in", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(activate_zoom_in), this);
  g_object_set( tools_zoom_in, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_in,CoWowGtk::translate_utf8("Zoom in"), "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
	  gtk_image_new_from_stock( "gtk-zoom-out", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(activate_zoom_out), this);
  g_object_set( tools_zoom_out, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_out,CoWowGtk::translate_utf8("Zoom out"), "");

  GtkWidget *tools_zoom_reset = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_zoom_reset), 
	  gtk_image_new_from_stock( "gtk-zoom-100", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_zoom_reset, "clicked", G_CALLBACK(activate_zoom_reset), this);
  g_object_set( tools_zoom_reset, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_reset,CoWowGtk::translate_utf8("Zoom reset"), "");

  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(vbox), GTK_WIDGET(nodelistnav_widget), TRUE, TRUE, 0);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);
  gtk_widget_show_all( toplevel);

  wow = new CoWowGtk( toplevel);

  *status = 1;
}


//
//  Delete nodelist
//
NodelistGtk::~NodelistGtk()
{
  free_cursor();
  delete nodelistnav;
  gtk_widget_destroy( toplevel);
}

void NodelistGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

void NodelistGtk::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = gdk_cursor_new_for_display( gtk_widget_get_display( toplevel),
					       GDK_WATCH);

  gdk_window_set_cursor( toplevel->window, clock_cursor);
  gdk_display_flush( gtk_widget_get_display( toplevel));
}

void NodelistGtk::reset_cursor()
{
  gdk_window_set_cursor( toplevel->window, NULL);
}

void NodelistGtk::free_cursor()
{
  if (clock_cursor)
    gdk_cursor_unref( clock_cursor);
}

gboolean NodelistGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  NodelistGtk *nodelist = (NodelistGtk *)data;

  if ( nodelist->focustimer.disabled())
    return FALSE;

  if ( nodelist->nodelist_displayed)
    nodelist->nodelistnav->set_input_focus();

  nodelist->focustimer.disable( 400);
  return FALSE;
}

void NodelistGtk::activate_exit( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  if ( nodelist->close_cb)
    (nodelist->close_cb)( nodelist->parent_ctx);
  else
    delete nodelist;
}

void NodelistGtk::activate_add_node( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->activate_add_node();
}

void NodelistGtk::activate_remove_node( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->activate_remove_node();
}

void NodelistGtk::activate_open_xtt( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->activate_open_xtt();
}

void NodelistGtk::activate_open_opplace( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->activate_open_opplace();
}

void NodelistGtk::activate_open_rtmon( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->activate_open_rtmon();
}

void NodelistGtk::activate_save( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->activate_save();
}

void NodelistGtk::activate_show_events( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->msg_window->map();
}

void NodelistGtk::activate_pop_events( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  nodelist->nodelistnav->set_msgw_pop( set);
}

void NodelistGtk::activate_gui_motif( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  if ( set)
    strcpy( nodelist->remote_gui, "motif");
  else
    strcpy( nodelist->remote_gui, "");
}

void NodelistGtk::activate_gui_gtk( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  if ( set)
    strcpy( nodelist->remote_gui, "");
}

void NodelistGtk::activate_mode_systemstatus( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  if ( set)
    nodelist->nodelistnav->set_mode( nodelist_eMode_SystemStatus);
}

void NodelistGtk::activate_mode_userstatus1( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  if ( set)
    nodelist->nodelistnav->set_mode( nodelist_eMode_Status1);
}

void NodelistGtk::activate_mode_userstatus2( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  if ( set)
    nodelist->nodelistnav->set_mode( nodelist_eMode_Status2);
}

void NodelistGtk::activate_mode_userstatus3( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  if ( set)
    nodelist->nodelistnav->set_mode( nodelist_eMode_Status3);
}

void NodelistGtk::activate_mode_userstatus4( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  if ( set)
    nodelist->nodelistnav->set_mode( nodelist_eMode_Status4);
}

void NodelistGtk::activate_mode_userstatus5( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  int set = gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w)) ? 1 : 0;

  if ( set)
    nodelist->nodelistnav->set_mode( nodelist_eMode_Status5);
}

void NodelistGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->nodelistnav->zoom( 1.2);
}

void NodelistGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->nodelistnav->zoom( 5.0/6);
}

void NodelistGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->nodelistnav->unzoom();
}

void NodelistGtk::activate_help( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  nodelist->activate_help();
}

void NodelistGtk::open_input_dialog( const char *text, const char *text2, const char *text3, 
				     const char *title, const char *init_text,
				     void (*ok_cb)( Nodelist *, char *, char *, char *))
{
  create_input_dialog();

  g_object_set( india_widget, 
		"visible", TRUE, 
		"title", title,
		NULL);

  gtk_label_set_text( GTK_LABEL(india_label), text);
  gtk_label_set_text( GTK_LABEL(india_label2), text2);
  gtk_label_set_text( GTK_LABEL(india_label3), text3);

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(india_text), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(india_text), init_text, 
			    strlen(init_text), &pos);

  india_ok_cb = ok_cb;
}

void NodelistGtk::activate_india_ok( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;
  char *text, *text2, *text3, *textutf8;

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(((NodelistGtk *)nodelist)->india_text), 
				 0, -1);
  text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(((NodelistGtk *)nodelist)->india_text2), 
				 0, -1);
  text2 = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(((NodelistGtk *)nodelist)->india_text3), 
				 0, -1);
  text3 = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  g_object_set( ((NodelistGtk *)nodelist)->india_widget, "visible", FALSE, NULL);

  (nodelist->india_ok_cb)( nodelist, text, text2, text3);
  g_free( text);
}

void NodelistGtk::activate_india_cancel( GtkWidget *w, gpointer data)
{
  Nodelist *nodelist = (Nodelist *)data;

  g_object_set( ((NodelistGtk *)nodelist)->india_widget, "visible", FALSE, NULL);
}

static gint india_delete_event( GtkWidget *w, GdkEvent *event, gpointer nodelist)
{
  g_object_set( ((NodelistGtk *)nodelist)->india_widget, "visible", FALSE, NULL);
  return TRUE;
}

void NodelistGtk::create_input_dialog()
{
  if ( india_widget) {
    g_object_set( india_widget, "visible", TRUE, NULL);
    return;
  }

  // Create an input dialog
  india_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					     "default-height", 150,
					     "default-width", 350,
					     "title", "Input Dialog",
					     "window-position", GTK_WIN_POS_CENTER,
					     NULL);
  g_signal_connect( india_widget, "delete_event", G_CALLBACK(india_delete_event), this);
  india_text = gtk_entry_new();
  g_signal_connect( india_text, "activate", 
  		    G_CALLBACK(NodelistGtk::activate_india_ok), this);
  india_text2 = gtk_entry_new();
  g_signal_connect( india_text2, "activate", 
  		    G_CALLBACK(NodelistGtk::activate_india_ok), this);
  india_text3 = gtk_entry_new();
  g_signal_connect( india_text3, "activate", 
  		    G_CALLBACK(NodelistGtk::activate_india_ok), this);
  india_label = gtk_label_new("");
  india_label2 = gtk_label_new("");
  india_label3 = gtk_label_new("");
  GtkWidget *india_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 0.4,
				NULL);

  GtkWidget *india_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( india_ok, 70, 25);
  g_signal_connect( india_ok, "clicked", 
  		    G_CALLBACK(NodelistGtk::activate_india_ok), this);
  GtkWidget *india_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( india_cancel, 70, 25);
  g_signal_connect( india_cancel, "clicked", 
  		    G_CALLBACK(NodelistGtk::activate_india_cancel), this);

  GtkWidget *india_vboxtext = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_vboxtext), india_text, FALSE, FALSE, 12);
  gtk_box_pack_start( GTK_BOX(india_vboxtext), india_text2, FALSE, FALSE, 12);
  gtk_box_pack_start( GTK_BOX(india_vboxtext), india_text3, FALSE, FALSE, 12);

  GtkWidget *india_vboxlabel = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_vboxlabel), india_label, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(india_vboxlabel), india_label2, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(india_vboxlabel), india_label3, FALSE, FALSE, 15);

  GtkWidget *india_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_hboxtext), india_image, FALSE, FALSE, 40);
  gtk_box_pack_start( GTK_BOX(india_hboxtext), india_vboxlabel, FALSE, FALSE, 15);
  gtk_box_pack_end( GTK_BOX(india_hboxtext), india_vboxtext, TRUE, TRUE, 30);

  GtkWidget *india_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(india_hboxbuttons), india_ok, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(india_hboxbuttons), india_cancel, FALSE, FALSE, 0);

  GtkWidget *india_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_vbox), india_hboxtext, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(india_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(india_vbox), india_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(india_widget), india_vbox);
  gtk_widget_show_all( india_widget);
  g_object_set( india_widget, "visible", FALSE, NULL);
}




