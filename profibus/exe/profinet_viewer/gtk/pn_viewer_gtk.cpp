/** 
 * Proview   $Id$
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

/* pn_viewer_gtk.cpp -- Profinet viewer */
#ifdef PWRE_CONF_PNAK

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_lng.h"
#include "cow_xhelp.h"
#include "pn_viewer_gtk.h"
#include "pn_viewernav_gtk.h"
#include "cow_wow_gtk.h"
#include "rt_pb_msg.h"


static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer viewer)
{
  PnViewerGtk::activate_exit(w, viewer);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

PnViewerGtk::PnViewerGtk (
	void *v_parent_ctx,
	GtkWidget *v_parent_wid,
	const char *v_name,
	pwr_tStatus *status) :
  PnViewer( v_parent_ctx, v_name, status), parent_wid(v_parent_wid)
{
  const int	window_width = 800;
  const int    	window_height = 600;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", v_name,
					 NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(PnViewerGtk::action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(PnViewerGtk::activate_exit), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Functions Entry
  GtkWidget *functions_update = gtk_menu_item_new_with_mnemonic("_Update");
  g_signal_connect(functions_update, "activate", G_CALLBACK(activate_update), this);

  GtkWidget *functions_setdevice = gtk_menu_item_new_with_mnemonic("_Set Device Properties");
  g_signal_connect(functions_setdevice, "activate", G_CALLBACK(activate_setdevice), this);

  GtkWidget *functions_changevalue = gtk_menu_item_new_with_mnemonic("_Change Value");
  g_signal_connect(functions_changevalue, "activate", G_CALLBACK(activate_changevalue), this);
  gtk_widget_add_accelerator( functions_changevalue, "activate", accel_g,
			      'q', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_update);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_setdevice);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_changevalue);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("F_unctions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));

  // View menu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(PnViewerGtk::activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(PnViewerGtk::activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(PnViewerGtk::activate_zoom_reset), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Help menu
  GtkWidget *help_help = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, NULL);
  g_signal_connect(help_help, "activate", G_CALLBACK(activate_help), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  form = gtk_vbox_new( FALSE, 0);

  // Statusbar and cmd input
  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);
  cmd_prompt = gtk_label_new( "value > ");
  gtk_widget_set_size_request( cmd_prompt, -1, 25);

  value_recall = new CoWowRecall();
  cmd_entry = new CoWowEntryGtk( value_recall);
  cmd_input = cmd_entry->widget();
  gtk_widget_set_size_request( cmd_input, -1, 25);
  g_signal_connect( cmd_input, "activate", 
  		    G_CALLBACK(valchanged_cmd_input), this);

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_prompt, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_input, TRUE, TRUE, 20);
  gtk_widget_show_all( statusbar);

  // Create viewernav
  viewernav = new PnViewerNavGtk( this, form, &nav_widget);
  viewernav->change_value_cb = &change_value;
  viewernav->message_cb = &message_cb;

  gtk_box_pack_start( GTK_BOX(form), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(form), GTK_WIDGET(nav_widget), TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(form), GTK_WIDGET(statusbar), FALSE, FALSE, 3);

  gtk_container_add( GTK_CONTAINER(toplevel), form);

  gtk_widget_show_all( toplevel);

  g_object_set( cmd_prompt, "visible", FALSE, NULL);
  g_object_set( cmd_input, "visible", FALSE, NULL);

  wow = new CoWowGtk( toplevel);
}


//
//  Delete window
//
PnViewerGtk::~PnViewerGtk()
{
  if ( viewernav)
    delete viewernav;
  if ( toplevel)
    gtk_widget_destroy( toplevel);
}

void PnViewerGtk::message( char severity, const char *msg)
{
  char *messageutf8 = g_convert( msg, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  gtk_label_set_text( GTK_LABEL(msg_label), messageutf8);
  g_free( messageutf8);
}

void PnViewerGtk::open_change_value()
{
  int		sts;

  if ( input_open) {
    g_object_set( cmd_input, "visible", FALSE, NULL);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = viewernav->check_attr_value();
  if ( EVEN(sts)) {
    if ( sts == PB__ATTRNOEDIT)
      message( 'E', "Attribute is no edit");
    else
      message( 'E', "Syntax error");
    return;
  }

  g_object_set( cmd_input, "visible", TRUE, NULL);

  message( ' ', "");
  gtk_widget_grab_focus( cmd_input);

  gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);
  set_prompt( Lng::translate("value >"));
  input_open = 1;
}

void PnViewerGtk::valchanged_cmd_input( GtkWidget *w, gpointer data)
{
  PnViewer *viewer = (PnViewer *)data;
  int 	sts;
  char 	*text, *textutf8;

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(w), 0, -1);
  text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  if ( viewer->input_open) {
    sts = viewer->viewernav->set_attr_value( text);
    g_object_set( w, "visible", FALSE, NULL);
    viewer->set_prompt( "");
    viewer->input_open = 0;
    viewer->viewernav->set_input_focus();
  }
}

void PnViewerGtk::set_prompt( const char *prompt)
{
  if ( strcmp(prompt, "") == 0) {
    g_object_set( cmd_prompt, "visible", FALSE, NULL);
    g_object_set( msg_label, "visible", TRUE, NULL);
  }
  else {
    char *promptutf8 = g_convert( prompt, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);

    g_object_set( msg_label, "visible", FALSE, NULL);
    g_object_set( cmd_prompt, "visible", TRUE, 
		  "label", promptutf8, NULL);
    g_free( promptutf8);
  }
}

gboolean PnViewerGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  PnViewerGtk *viewer = (PnViewerGtk *)data;

  if ( viewer) {
    if ( viewer->focustimer.disabled())
      return FALSE;

    viewer->viewernav->set_input_focus();
    viewer->focustimer.disable( 400);
  }
  return FALSE;
}

void PnViewerGtk::activate_exit( GtkWidget *w, gpointer data)
{
  PnViewerGtk *viewer = (PnViewerGtk *)data;

  viewer->activate_close();
}

void PnViewerGtk::activate_update( GtkWidget *w, gpointer data)
{
  PnViewer *viewer = (PnViewer *)data;

  viewer->activate_update();
}

void PnViewerGtk::activate_setdevice( GtkWidget *w, gpointer data)
{
  PnViewer *viewer = (PnViewer *)data;

  viewer->activate_setdevice();
}

void PnViewerGtk::activate_changevalue( GtkWidget *w, gpointer data)
{
  PnViewer *viewer = (PnViewer *)data;

  viewer->activate_changevalue();
}

void PnViewerGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  PnViewer *viewer = (PnViewer *)data;

  viewer->viewernav->zoom( 1.2);
}

void PnViewerGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  PnViewer *viewer = (PnViewer *)data;

  viewer->viewernav->zoom( 5.0/6);
}

void PnViewerGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  PnViewer *viewer = (PnViewer *)data;

  viewer->viewernav->unzoom();
}

void PnViewerGtk::activate_help( GtkWidget *w, gpointer data)
{
  PnViewer *viewer = (PnViewer *)data;

  viewer->activate_help();
}


#endif







