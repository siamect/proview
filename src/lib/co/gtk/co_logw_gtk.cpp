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

/* co_logw_gtk.cpp -- History log window */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_lng.h"
#include "co_xhelp.h"
#include "co_logw_gtk.h"
#include "co_logwnav_gtk.h"
#include "co_wow_gtk.h"


static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer logw)
{
  CoLogWGtk::activate_exit(w, logw);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

CoLogWGtk::CoLogWGtk (
	void *logw_parent_ctx,
	GtkWidget *logw_parent_wid,
	const char *logw_name,
	int l_show_item,
	pwr_tStatus *status) :
  CoLogW( logw_parent_ctx, logw_name, l_show_item, status), parent_wid(logw_parent_wid)
{
  const int	window_width = 800;
  const int    	window_height = 600;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", logw_name,
					 NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(CoLogWGtk::action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(CoLogWGtk::activate_exit), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // View menu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(CoLogWGtk::activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(CoLogWGtk::activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(CoLogWGtk::activate_zoom_reset), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  form = gtk_vbox_new( FALSE, 0);

  // Create logwnav
  logwnav = new CoLogWNavGtk( this, form, l_show_item, &nav_widget);

  gtk_box_pack_start( GTK_BOX(form), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(form), GTK_WIDGET(nav_widget), TRUE, TRUE, 0);

  gtk_container_add( GTK_CONTAINER(toplevel), form);

  gtk_widget_show_all( toplevel);

  wow = new CoWowGtk( toplevel);
}


//
//  Delete window
//
CoLogWGtk::~CoLogWGtk()
{
  if ( logwnav)
    delete logwnav;
  if ( toplevel)
    gtk_widget_destroy( toplevel);
}

gboolean CoLogWGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  CoLogWGtk *logw = (CoLogWGtk *)data;

  if ( logw) {
    if ( logw->focustimer.disabled())
      return FALSE;

    logw->logwnav->set_input_focus();
    logw->focustimer.disable( 400);
  }
  return FALSE;
}

void CoLogWGtk::activate_exit( GtkWidget *w, gpointer data)
{
  CoLogWGtk *logw = (CoLogWGtk *)data;

  delete logw;
}

void CoLogWGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  CoLogW *logw = (CoLogW *)data;

  logw->logwnav->zoom( 1.2);
}

void CoLogWGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  CoLogW *logw = (CoLogW *)data;

  logw->logwnav->zoom( 5.0/6);
}

void CoLogWGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  CoLogW *logw = (CoLogW *)data;

  logw->logwnav->unzoom();
}

void CoLogWGtk::activate_help( GtkWidget *w, gpointer data)
{
  CoXHelp::dhelp( "messagewindow_refman", 0, navh_eHelpFile_Other, "$pwr_lang/man_dg.dat", 
		  true);
}










