/** 
 * Proview   $Id: co_msgwindow_gtk.cpp,v 1.2 2007-05-11 15:11:17 claes Exp $
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

/* co_msgwindow_gtk.cpp -- Message window */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_lng.h"
#include "co_xhelp.h"
#include "co_msgwindow_gtk.h"
#include "co_msglist_gtk.h"
#include "co_wow_gtk.h"


static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer msgw)
{
  MsgWindowGtk::activate_exit(w, msgw);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

MsgWindowGtk::MsgWindowGtk (
	void *msg_parent_ctx,
	GtkWidget *msg_parent_wid,
	char *msg_name,
	pwr_tStatus *status) :
  MsgWindow( msg_parent_ctx, msg_name, status), parent_wid(msg_parent_wid)
{
  const int	window_width = 800;
  const int    	window_height = 600;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", msg_name,
					 NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(MsgWindowGtk::action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  GtkWidget *file_clear = gtk_menu_item_new_with_mnemonic( "C_lear");
  g_signal_connect( file_clear, "activate", 
		    G_CALLBACK(MsgWindowGtk::activate_clear), this);

  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(MsgWindowGtk::activate_exit), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_clear);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // View menu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(MsgWindowGtk::activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(MsgWindowGtk::activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(MsgWindowGtk::activate_zoom_reset), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  form = gtk_vbox_new( FALSE, 0);

  // Create msgnav
  msgnav = new MsgListGtk( this, form, &nav_widget);
  msgnav->find_wnav_cb = MsgWindow::msgw_find_wnav_cb;
  msgnav->find_plc_cb = MsgWindow::msgw_find_plc_cb;

  gtk_box_pack_start( GTK_BOX(form), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(form), GTK_WIDGET(nav_widget), TRUE, TRUE, 0);

  gtk_container_add( GTK_CONTAINER(toplevel), form);

  gtk_widget_show_all( toplevel);
  g_object_set( toplevel, "visible", FALSE, NULL);

  wow = new CoWowGtk( toplevel);
}


//
//  Delete window
//
MsgWindowGtk::~MsgWindowGtk()
{
  if ( toplevel)
    gtk_widget_destroy( toplevel);
  if ( msgnav)
    delete msgnav;
}

void MsgWindowGtk::map()
{
  if ( nodraw) {
    deferred_map = 1;
    return;
  }
  else
    deferred_map = 0;

  if ( maptimer.disabled())
    return;

  if ( !displayed) {
    g_object_set( toplevel, "visible", TRUE, NULL);
    displayed = 1;
  }
  else {
    gtk_window_present( GTK_WINDOW(toplevel));
  }
  maptimer.disable(400);
}

void MsgWindowGtk::unmap()
{
  if ( displayed) {
    g_object_set( toplevel, "visible", FALSE, NULL);
    displayed = 0;
  }
}

gboolean MsgWindowGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  MsgWindowGtk *msgw = (MsgWindowGtk *)data;

  if ( msgw && msgw->displayed) {
    if ( msgw->focustimer.disabled())
      return FALSE;

    msgw->msgnav->set_input_focus();
    msgw->focustimer.disable( 400);
  }
  return FALSE;
}

void MsgWindowGtk::activate_exit( GtkWidget *w, gpointer data)
{
  MsgWindowGtk *msgw = (MsgWindowGtk *)data;

  g_object_set( msgw->toplevel, "visible", FALSE, NULL);
  msgw->displayed = 0;
}

void MsgWindowGtk::activate_clear( GtkWidget *w, gpointer data)
{
  MsgWindow *msgw = (MsgWindow *)data;

  msgw->msgnav->clear();
  msgw->size = 0;
}

void MsgWindowGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  MsgWindow *msgw = (MsgWindow *)data;

  msgw->msgnav->zoom( 1.2);
}

void MsgWindowGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  MsgWindow *msgw = (MsgWindow *)data;

  msgw->msgnav->zoom( 5.0/6);
}

void MsgWindowGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  MsgWindow *msgw = (MsgWindow *)data;

  msgw->msgnav->unzoom();
}

void MsgWindowGtk::activate_help( GtkWidget *w, gpointer data)
{
  CoXHelp::dhelp( "messagewindow_refman", 0, navh_eHelpFile_Other, "$pwr_lang/man_dg.dat", 
		  true);
}

void MsgWindowGtk::activate_help_message( GtkWidget *w, gpointer data)
{
  CoXHelp::dhelp( "message window", 0, navh_eHelpFile_Base, 0, true);
}










