/** 
 * Proview   $Id: co_xhelp_gtk.cpp,v 1.1 2007-01-04 07:51:41 claes Exp $
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

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_wow_gtk.h"
#include "co_lng.h"
#include "co_xhelpnav_gtk.h"
#include "co_xhelp_gtk.h"

void CoXHelpGtk::open_input_dialog( char *text, char *title,
				    char *init_text,
				    void (*ok_cb)( CoXHelp *, char *))
{
  g_object_set( india_widget, 
		"visible", TRUE, 
		"title", title,
		NULL);

  gtk_label_set_text( GTK_LABEL(india_label), text);

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(india_text), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(india_text), init_text, 
			    strlen(init_text), &pos);
  gtk_widget_grab_focus( india_text);
  india_ok_cb = ok_cb;
}

void CoXHelpGtk::activate_close( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;

  if ( xhelp->close_cb)
    (xhelp->close_cb)( xhelp->parent_ctx, (void *)xhelp);
  else {
    g_object_set( xhelp->toplevel, "visible", FALSE, NULL);
    xhelp->displayed = 0;
  }
  //  delete xhelp;
}

void CoXHelpGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;
  double zoom_factor;

  xhelp->xhelpnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  xhelp->xhelpnav->zoom( 1.18);
}

void CoXHelpGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;
  double zoom_factor;

  xhelp->xhelpnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  xhelp->xhelpnav->zoom( 1.0 / 1.18);
}

void CoXHelpGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;

  xhelp->xhelpnav->unzoom();
}

void CoXHelpGtk::activate_search( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;

  xhelp->open_input_dialog( "Search string", "Search string",
			    "", &CoXHelp::find_ok);
}

void CoXHelpGtk::activate_searchnext( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;

  xhelp->xhelpnav->search_next();
}

void CoXHelpGtk::activate_searchprevious( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;

  xhelp->xhelpnav->search_next_reverse();
}

void CoXHelpGtk::activate_india_ok( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;
  char *text;

  text = gtk_editable_get_chars( GTK_EDITABLE( xhelp->india_text), 0, -1);
  g_object_set( xhelp->india_widget, "visible", FALSE, NULL);

  (xhelp->india_ok_cb)( xhelp, text);

  g_free( text);
}
void CoXHelpGtk::activate_india_cancel( GtkWidget *w, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;

  g_object_set( xhelp->india_widget, "visible", FALSE, NULL);
}

void CoXHelpGtk::activate_help( GtkWidget *w, gpointer data)
{
  CoXHelp::dhelp( "helpwindow_refman", 0, navh_eHelpFile_Other, "$pwr_lang/man_dg.dat", 
		  true);
}

gboolean CoXHelpGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;

  if ( !xhelp)
    return FALSE;

  if ( xhelp->focustimer.disabled())
    return FALSE;

  if ( xhelp->xhelpnav && xhelp->displayed) {
    xhelp->xhelpnav->set_inputfocus();
  }
  xhelp->focustimer.disable( 400);
  return FALSE;
}

void CoXHelpGtk::set_dimension( int width, int height)
{
  g_object_set( toplevel,
		"default-width", width,
		"default-height", height,
		NULL);
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  CoXHelpGtk *xhelp = (CoXHelpGtk *)data;

  if ( xhelp->close_cb)
    (xhelp->close_cb)( xhelp->parent_ctx, (void *)xhelp);
  else {
    g_object_set( xhelp->toplevel, "visible", FALSE, NULL);
    xhelp->displayed = 0;
  }
  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

static gint india_delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  // Hide
  g_object_set( w, "visible", FALSE, NULL);
  return TRUE;
}

void CoXHelpGtk::pop()
{
  displayed = 1;
  gtk_window_present( GTK_WINDOW(toplevel));
}

CoXHelpGtk::~CoXHelpGtk()
{
  delete xhelpnav;
  gtk_widget_destroy( toplevel);
}

CoXHelpGtk::CoXHelpGtk( 
	GtkWidget      	*xa_parent_wid,
	void 		*xa_parent_ctx,
	xhelp_eUtility	utility,
        int             *xa_sts) :
 	CoXHelp(xa_parent_ctx,utility,xa_sts), parent_wid(xa_parent_wid)
{
  int sts;
  char title[80];

  strcpy( title, Lng::translate("Help"));

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 700,
			   "default-width", 400,
			   "title", title,
			   NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(CoXHelpGtk::action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  // Menu
  // Accelerators
  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File entry
  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(CoXHelpGtk::activate_close), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Edit entry
  GtkWidget *edit_search = gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND, accel_g);
  g_signal_connect(edit_search, "activate", G_CALLBACK(CoXHelpGtk::activate_search), this);

  GtkWidget *edit_searchnext = gtk_menu_item_new_with_mnemonic( "Search _Next");
  g_signal_connect( edit_searchnext, "activate", 
		    G_CALLBACK(CoXHelpGtk::activate_searchnext), this);
  gtk_widget_add_accelerator( edit_searchnext, "activate", accel_g,
			      'g', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *edit_searchprevious = gtk_menu_item_new_with_mnemonic( "Search _Previous");
  g_signal_connect( edit_searchprevious, "activate", 
		    G_CALLBACK(CoXHelpGtk::activate_searchprevious), this);
  gtk_widget_add_accelerator( edit_searchprevious, "activate", accel_g,
			      'g', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *edit_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_search);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_searchnext);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_searchprevious);

  GtkWidget *edit = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), GTK_WIDGET(edit_menu));

  // Menu View
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(CoXHelpGtk::activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(CoXHelpGtk::activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(CoXHelpGtk::activate_zoom_reset), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Menu Help
  GtkWidget *help_help = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_g);
  g_signal_connect(help_help, "activate", G_CALLBACK(CoXHelpGtk::activate_help), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);

  xhelpnav = new CoXHelpNavGtk( (void *)this, vbox, title, utility, &brow_widget, 
                             &sts);
  xhelpnav->open_URL_cb = CoXHelp::open_URL;

  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(brow_widget), TRUE, TRUE, 0);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  gtk_widget_show_all( toplevel);
  g_object_set( toplevel, "visible", FALSE, NULL);
  
  // Create an input dialog
  india_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 150,
			   "default-width", 350,
			   "title", "Input Dialog",
			   NULL);
  g_signal_connect( india_widget, "delete_event", G_CALLBACK(india_delete_event), this);
  india_text = gtk_entry_new();
  india_label = gtk_label_new("Graph Name");
  GtkWidget *india_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *india_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( india_ok, 70, 25);
  g_signal_connect( india_ok, "clicked", 
  		    G_CALLBACK(CoXHelpGtk::activate_india_ok), this);
  GtkWidget *india_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( india_cancel, 70, 25);
  g_signal_connect( india_cancel, "clicked", 
  		    G_CALLBACK(CoXHelpGtk::activate_india_cancel), this);

  GtkWidget *india_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(india_hboxtext), india_image, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(india_hboxtext), india_label, FALSE, FALSE, 15);
  gtk_box_pack_end( GTK_BOX(india_hboxtext), india_text, TRUE, TRUE, 30);

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

  *xa_sts = 1;
}






