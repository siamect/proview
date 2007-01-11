/* 
 * Proview   $Id: wb_wpkg_gtk.cpp,v 1.2 2007-01-11 11:40:30 claes Exp $
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

/* wb_wpkg_gtk.cpp -- package window */

#include "glow_std.h"

#include <typeinfo>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_wow_gtk.h"
#include "wb_pkg_msg.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_wpkg_gtk.h"
#include "wb_wpkgnav_gtk.h"
#include "wb_wtt.h"
#include "wb_wnav.h"
#include "wb_pkg.h"
#include "wb_error.h"
#include "co_xhelp.h"

void WPkgGtk::message( char severity, char *message)
{
  gtk_label_set_text( GTK_LABEL(msg_label), message);
}

//
//  Callbackfunctions from menu entries
//
void WPkgGtk::activate_distribute( GtkWidget *w, gpointer data)
{
  WPkg *wpkg = (WPkg *)data;

  wpkg->activate_distribute();
}

void WPkgGtk::activate_createpkg( GtkWidget *w, gpointer data)
{
  WPkg *wpkg = (WPkg *)data;

  wpkg->activate_createpkg();
}

void WPkgGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  WPkg *wpkg = (WPkg *)data;
  wpkg->activate_zoom_in();
}

void WPkgGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  WPkg *wpkg = (WPkg *)data;
  wpkg->activate_zoom_out();
}

void WPkgGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  WPkg *wpkg = (WPkg *)data;
  wpkg->activate_zoom_reset();
}

void WPkgGtk::activate_deletepkg( GtkWidget *w, gpointer data)
{
  WPkg *wpkg = (WPkg *)data;

  wpkg->activate_deletepkg();
}

void WPkgGtk::activate_dmode_filediff( GtkWidget *w, gpointer data)
{  
  WPkg *wpkg = (WPkg *)data;

  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
  wpkg->activate_dmode_filediff( set);
}

void WPkgGtk::activate_dmode_filetime( GtkWidget *w, gpointer data)
{  
  WPkg *wpkg = (WPkg *)data;

  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
  wpkg->activate_dmode_filetime( set);
}

void WPkgGtk::activate_dmode_filepath( GtkWidget *w, gpointer data)
{  
  WPkg *wpkg = (WPkg *)data;

  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
  wpkg->activate_dmode_filepath( set);
}

void WPkgGtk::activate_exit( GtkWidget *w, gpointer data)
{
  WPkg *wpkg = (WPkg *)data;

  if ( wpkg->close_cb)
    (wpkg->close_cb)( wpkg->parent_ctx);
  else
    delete wpkg;
}

void WPkgGtk::activate_help( GtkWidget *w, gpointer data)
{
  CoXHelp::dhelp( "package window", 0, navh_eHelpFile_Base, 0, true);
}

gboolean WPkgGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  WPkgGtk *wpkg = (WPkgGtk *)data;

  if ( wpkg->focustimer.disabled())
    return FALSE;

  ((WPkgNav *)wpkg->wpkgnav)->set_inputfocus();

  wpkg->focustimer.disable( 400);
  return FALSE;
}

void WPkgGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

void WPkgGtk::flush()
{
  gdk_display_flush( gtk_widget_get_display(toplevel));
}

void WPkgGtk::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = gdk_cursor_new_for_display( gtk_widget_get_display(toplevel), GDK_WATCH);

  gdk_window_set_cursor( toplevel->window, clock_cursor);
  gdk_display_flush( gtk_widget_get_display(toplevel));
}

void WPkgGtk::reset_cursor()
{
  gdk_window_set_cursor( toplevel->window, NULL);
  gdk_display_flush( gtk_widget_get_display(toplevel));
}

WPkgGtk::~WPkgGtk()
{

  delete wow;
  delete (WPkgNav *)wpkgnav;
  gtk_widget_destroy( toplevel);
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer wpkg)
{
  WPkgGtk::activate_exit(w, wpkg);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

WPkgGtk::WPkgGtk( 
	GtkWidget      	*wa_parent_wid,
	void 		*wa_parent_ctx)
  : WPkg(wa_parent_ctx), parent_wid(wa_parent_wid),
    clock_cursor(0)
{
  const int	window_width = 600;
  const int    	window_height = 600;
  int 		sts;
  pwr_tFileName fname;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", window_height,
			   "default-width", window_width,
			   NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(WPkgGtk::activate_exit), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Functions Entry
  GtkWidget *functions_distribute = gtk_menu_item_new_with_mnemonic( "_Distribute");
  g_signal_connect( functions_distribute, "activate", 
		    G_CALLBACK(WPkgGtk::activate_distribute), this);
  gtk_widget_add_accelerator( functions_distribute, "activate", accel_g,
			      'd', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_createpkg = gtk_menu_item_new_with_mnemonic( "_Create Package");
  g_signal_connect( functions_createpkg, "activate", 
		    G_CALLBACK(WPkgGtk::activate_createpkg), this);

  GtkWidget *functions_deletepkg = gtk_menu_item_new_with_mnemonic( "D_elete Package");
  g_signal_connect( functions_deletepkg, "activate", 
		    G_CALLBACK(WPkgGtk::activate_deletepkg), this);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_distribute);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_createpkg);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_deletepkg);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("F_unctions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));

  // View menu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(WPkgGtk::activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(WPkgGtk::activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(WPkgGtk::activate_zoom_reset), this);

  GtkWidget *view_dmode_filediff = gtk_menu_item_new_with_mnemonic( "_Display File Differences");
  g_signal_connect( view_dmode_filediff, "activate", 
		    G_CALLBACK(WPkgGtk::activate_dmode_filediff), this);

  GtkWidget *view_dmode_filetime = gtk_menu_item_new_with_mnemonic( "Display Files In _Time Order");
  g_signal_connect( view_dmode_filetime, "activate", 
		    G_CALLBACK(WPkgGtk::activate_dmode_filetime), this);

  GtkWidget *view_dmode_filepath = gtk_menu_item_new_with_mnemonic( "Display File _Path");
  g_signal_connect( view_dmode_filepath, "activate", 
		    G_CALLBACK(WPkgGtk::activate_dmode_filepath), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_dmode_filediff);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_dmode_filetime);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_dmode_filepath);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Menu Help
  GtkWidget *help_help = gtk_image_menu_item_new_with_mnemonic("_Help");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_help), 
				 gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
  g_signal_connect(help_help, "activate", G_CALLBACK(WPkgGtk::activate_help), this);
  gtk_widget_add_accelerator( help_help, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  GtkWidget *tools_distribute = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/wpkg_distribute.png");
  gtk_container_add( GTK_CONTAINER(tools_distribute), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_distribute, "clicked", G_CALLBACK(WPkgGtk::activate_distribute), this);
  gtk_toolbar_append_widget( tools, tools_distribute, "Distribute", "");

  GtkWidget *tools_zoom_in = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(WPkgGtk::activate_zoom_in), this);
  gtk_toolbar_append_widget( tools, tools_zoom_in, "Zoom in", "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(WPkgGtk::activate_zoom_out), this);
  gtk_toolbar_append_widget( tools, tools_zoom_out, "Zoom out", "");

  GtkWidget *tools_zoom_reset = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_reset.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_reset), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_reset, "clicked", G_CALLBACK(WPkgGtk::activate_zoom_reset), this);
  gtk_toolbar_append_widget( tools, tools_zoom_reset, "Zoom reset", "");

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);

  utility = ((WUtility *)parent_ctx)->utype;
  wpkgnav = new WPkgNavGtk( (void *)this, vbox, "Packages",
		utility, &brow_widget, &sts);
  ((WPkgNav *)wpkgnav)->message_cb = message_cb;
  ((WPkgNav *)wpkgnav)->set_clock_cursor_cb = set_clock_cursor_cb;
  ((WPkgNav *)wpkgnav)->reset_cursor_cb = reset_cursor_cb;

  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  msg_label = gtk_label_new( "");

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 20);
  gtk_widget_show_all( statusbar);

  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(brow_widget), TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(statusbar), FALSE, FALSE, 3);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  gtk_widget_show_all( toplevel);

  wow = new CoWowGtk( toplevel);

  if ( utility == wb_eUtility_Wtt) {
    ((Wtt *)parent_ctx)->register_utility( (void *) this,
	wb_eUtility_Distributor);
  }
}


