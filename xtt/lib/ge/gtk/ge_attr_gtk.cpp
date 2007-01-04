/* 
 * Proview   $Id: ge_attr_gtk.cpp,v 1.1 2007-01-04 08:21:58 claes Exp $
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

/* ge_attr.cpp -- Display object attributes */

#include "glow_std.h"

#include "co_cdh.h"
#include "co_time.h"
#include "co_wow_gtk.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "flow_msg.h"

#include "ge_attr_gtk.h"
#include "ge_attrnav_gtk.h"
#include "ge_dyn.h"
#include "ge_msg.h"

CoWowRecall AttrGtk::value_recall;

void AttrGtk::message( char severity, char *message)
{
  // gtk_statusbar_pop( GTK_STATUSBAR(msg_label), 0);
  // gtk_statusbar_push( GTK_STATUSBAR(msg_label), 0, message);
  gtk_label_set_text( GTK_LABEL(msg_label), message);
}

void AttrGtk::set_prompt( char *prompt) {
  if ( strcmp(prompt, "") == 0) {
    g_object_set( cmd_prompt, "visible", FALSE, NULL);
    g_object_set( msg_label, "visible", TRUE, NULL);
  }
  else {
    g_object_set( msg_label, "visible", FALSE, NULL);
    g_object_set( cmd_prompt, "visible", TRUE, NULL);
  }
}

void AttrGtk::change_value() {
  int		sts;
  GtkWidget	*text_w;
  int		multiline;
  char		*value;

  if ( input_open)  {
    g_object_set( cmd_input, "visible", FALSE, NULL);
    g_object_set( cmd_scrolledinput, "visible", FALSE, NULL);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = attrnav->check_attr_value( &multiline, &value);
  if ( EVEN(sts)) {
    if ( sts == GE__NOATTRSEL)
      message( 'E', "No attribute is selected");
    else
      message( 'E', "Attribute can't be modified");
    return;
  }

  if ( multiline) {
    text_w = cmd_scrolledinput;
    g_object_set( text_w, "visible", TRUE, NULL);

    int w, h;
    gdk_drawable_get_size( pane->window, &w, &h);
    gtk_paned_set_position( GTK_PANED(pane), h - 170);
    gtk_widget_grab_focus( cmd_scrolledtextview);
    // XtManageChild( text_w);
    // XtManageChild( cmd_scrolled_ok);
    // XtManageChild( cmd_scrolled_ca);
  }
  else {
    text_w = cmd_input;
    g_object_set( text_w, "visible", TRUE, NULL);
    gtk_widget_grab_focus( cmd_input);
  }


  message( ' ', "");

  if ( value) {
    if ( multiline) {
      GtkTextIter start_iter, end_iter;
      gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
      gtk_text_buffer_get_end_iter( cmd_scrolled_buffer, &end_iter);
      gtk_text_buffer_delete( cmd_scrolled_buffer, &start_iter, &end_iter);

      gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
      gtk_text_buffer_insert( cmd_scrolled_buffer, &start_iter, value, -1);

      // Select the text
      // gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
      // gtk_text_buffer_get_end_iter( cmd_scrolled_buffer, &end_iter);
      // gtk_text_buffer_select_range( cmd_scrolled_buffer, &start_iter, &end_iter);
    }
    else {
      gint pos = 0;
      gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);
      gtk_editable_insert_text( GTK_EDITABLE(text_w), value, strlen(value), &pos);

      // Select the text
      gtk_editable_set_position( GTK_EDITABLE(cmd_input), -1);
      gtk_editable_select_region( GTK_EDITABLE(cmd_input), 0, -1);
    }
  }
  else {
    gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);
  }
  set_prompt( "value >");
  input_open = 1;
}

int AttrGtk::reconfigure_attr()
{
  attr_sItem 	*itemlist;
  int	   	item_cnt;
  int		sts;

  if ( !reconfigure_attr_cb)
    return 0;
  reconfigure_attr_cb( parent_ctx, object, 
	&itemlist, &item_cnt, &client_data);  

  delete (AttrNav *)attrnav;
  attrnav = new AttrNavGtk( this, pane, "Plant",
		itemlist, item_cnt, &brow_widget, &sts);
  gtk_paned_pack1( GTK_PANED(pane), GTK_WIDGET(brow_widget), TRUE, TRUE);
  gtk_widget_show_all( brow_widget);

  attrnav->message_cb = &Attr::message;
  attrnav->change_value_cb = &Attr::change_value_c;
  attrnav->get_subgraph_info_cb = 
		&Attr::get_subgraph_info_c;
  attrnav->get_dyn_info_cb = 
		&Attr::get_dyn_info_c;
  attrnav->reconfigure_attr_cb = 
		&Attr::reconfigure_attr_c;
  attrnav->set_inputfocus();
  attrnav->get_plant_select_cb = 
		&Attr::get_plant_select_c;
  attrnav->get_current_colors_cb = 
		&Attr::get_current_colors_c;


  return 1;
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  AttrGtk *attr = (AttrGtk *)data;

  if ( attr->close_cb)
    (attr->close_cb)( attr);
  else
    delete attr;
  
  return FALSE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
  // AttrGtk *attr = (AttrGtk *)data;
  // delete attr;
}

static gboolean attr_focus_in_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  AttrGtk *attr = (AttrGtk *)data;
  gboolean scrolledinput_visible;
  gboolean input_visible;
  printf("Focus event\n");

  g_object_get( attr->cmd_scrolledinput, "visible", &scrolledinput_visible, NULL);
  g_object_get( attr->cmd_input, "visible", &input_visible, NULL);
  if ( scrolledinput_visible)
    gtk_widget_grab_focus( attr->cmd_scrolledtextview);
  else if ( input_visible)
    gtk_widget_grab_focus( attr->cmd_input);
  else if ( attr->attrnav)
    attr->attrnav->set_inputfocus();

  return FALSE;
}

static void attr_activate_cmd_input( GtkWidget *w, gpointer data)
{
  char *text;
  AttrGtk *attr = (AttrGtk *)data;
  int sts;

  g_object_set( attr->cmd_prompt, "visible", FALSE, NULL);
  g_object_set( attr->cmd_input, "visible", FALSE, NULL);

  attr->attrnav->set_inputfocus();

  text = gtk_editable_get_chars( GTK_EDITABLE(w), 0, -1);
  if ( attr->input_open) {
    sts = ((AttrNav *)attr->attrnav)->set_attr_value( text);
    g_object_set( w, "visible", FALSE, NULL);
    attr->set_prompt( "");
    attr->input_open = 0;
    if ( attr->redraw_cb)
      (attr->redraw_cb)( attr);

  }
  g_free( text);
}

static void attr_activate_cmd_scrolled_ok( GtkWidget *w, gpointer data)
{
  AttrGtk *attr = (AttrGtk *)data;
  gchar *text;
  int sts;

  if ( attr->input_open) {
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter( attr->cmd_scrolled_buffer, &start_iter);
    gtk_text_buffer_get_end_iter( attr->cmd_scrolled_buffer, &end_iter);

    text = gtk_text_buffer_get_text( attr->cmd_scrolled_buffer, &start_iter, &end_iter,
				     FALSE);

    sts = attr->attrnav->set_attr_value( text);
    g_object_set( attr->cmd_scrolledinput, "visible", FALSE, NULL);
    attr->set_prompt( "");
    attr->input_open = 0;

    int w, h;
    gdk_drawable_get_size( attr->pane->window, &w, &h);
    gtk_paned_set_position( GTK_PANED(attr->pane), h - 50);

    if ( attr->redraw_cb)
      (attr->redraw_cb)( attr);

    attr->attrnav->set_inputfocus();
  }
}

static void attr_activate_cmd_scrolled_ca( GtkWidget *w, gpointer data)
{
  AttrGtk *attr = (AttrGtk *)data;

  if ( attr->input_open) {
    g_object_set( attr->cmd_scrolledinput, "visible", FALSE, NULL);

    int w, h;
    gdk_drawable_get_size( attr->pane->window, &w, &h);
    gtk_paned_set_position( GTK_PANED(attr->pane), h - 50);

    attr->set_prompt( "");
    attr->set_prompt( "");
    attr->input_open = 0;
  }

}


static void attr_activate_changevalue( GtkWidget *w, gpointer attr)
{
  ((Attr *)attr)->change_value();
}

static void attr_activate_close( GtkWidget *w, gpointer data)
{
  Attr *attr = (Attr *)data;

  if ( attr->close_cb)
    (attr->close_cb)( attr);
  else
    delete attr;
}

static void attr_activate_store( GtkWidget *w, gpointer attr)
{
  ((Attr *)attr)->store();
}

static void attr_activate_recall( GtkWidget *w, gpointer attr)
{
  ((Attr *)attr)->recall_next();
}

static void attr_activate_help( GtkWidget *w, gpointer data)
{
  // Not yet implemented
}

static void attr_activate_recall_prev( GtkWidget *w, gpointer attr)
{
  ((Attr *)attr)->recall_prev();
}

#if 0
static void attr_action_inputfocus( GtkWidget w, gpointer data)
{
  Arg args[1];
  AttrMotif *attr;

  XtSetArg    (args[0], XmNuserData, &attr);
  XtGetValues (w, args, 1);

  if ( !attr)
    return;

  if ( flow_IsManaged( attr->cmd_scrolledinput))
    flow_SetInputFocus( attr->cmd_scrolledinput);
  else if ( flow_IsManaged( attr->cmd_input))
    flow_SetInputFocus( attr->cmd_input);
  else if ( attr->attrnav)
    attr->attrnav->set_inputfocus();
}
#endif

AttrGtk::AttrGtk( GtkWidget *a_parent_wid,
  void			*a_parent_ctx,
  void 			*a_object,
  attr_sItem  		*itemlist,
  int			item_cnt ) :
  Attr( a_parent_ctx, a_object, itemlist, item_cnt), 
  parent_wid(a_parent_wid)
{
  int sts;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 400,
			   "default-width", 300,
			   "title", "Object Attributes",
			   NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(attr_focus_in_event), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);

  // Menu
  // Accelerators
  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File entry
  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(attr_activate_close), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Functions entry
  GtkWidget *func_changevalue = gtk_menu_item_new_with_mnemonic( "_Change Value");
  g_signal_connect( func_changevalue, "activate", 
		    G_CALLBACK(attr_activate_changevalue), this);
  gtk_widget_add_accelerator( func_changevalue, "activate", accel_g,
			      'q', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *func_store = gtk_menu_item_new_with_mnemonic( "_Store");
  g_signal_connect( func_store, "activate", 
		    G_CALLBACK(attr_activate_store), this);

  GtkWidget *func_recall = gtk_menu_item_new_with_mnemonic( "_Recall");
  g_signal_connect( func_recall, "activate", 
		    G_CALLBACK(attr_activate_recall), this);
  gtk_widget_add_accelerator( func_recall, "activate", accel_g,
			      'n', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *func_recall_prev = gtk_menu_item_new_with_mnemonic( "Recall _previous");
  g_signal_connect( func_recall_prev, "activate", 
 		    G_CALLBACK(attr_activate_recall_prev), this);
  gtk_widget_add_accelerator( func_recall_prev, "activate", accel_g,
			      'p', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


  GtkMenu *func_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), func_changevalue);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), func_store);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), func_recall);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), func_recall_prev);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("_Functions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(func_menu));

  // Help entry
  GtkWidget *help_help = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_g);
  g_signal_connect(help_help, "activate", G_CALLBACK(attr_activate_help), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  pane = gtk_vpaned_new();

  attrnav = new AttrNavGtk( this, pane, "Plant",
		itemlist, item_cnt, &brow_widget, &sts);
  attrnav->message_cb = &Attr::message;
  attrnav->change_value_cb = &Attr::change_value_c;
  attrnav->get_subgraph_info_cb = 
		&Attr::get_subgraph_info_c;
  attrnav->get_dyn_info_cb = 
		&Attr::get_dyn_info_c;
  attrnav->reconfigure_attr_cb = 
		&Attr::reconfigure_attr_c;
  attrnav->get_plant_select_cb = 
		&Attr::get_plant_select_c;
  attrnav->get_current_colors_cb = 
		&Attr::get_current_colors_c;

  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  // msg_label = gtk_statusbar_new();
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);
  cmd_prompt = gtk_label_new( "value > ");
  gtk_widget_set_size_request( cmd_prompt, -1, 25);

  cmd_entry = new CoWowEntryGtk( &value_recall);
  cmd_input = cmd_entry->widget();
  gtk_widget_set_size_request( cmd_input, -1, 25);
  g_signal_connect( cmd_input, "activate", 
		    G_CALLBACK(attr_activate_cmd_input), this);

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_prompt, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_input, TRUE, TRUE, 0);

  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(vbox), GTK_WIDGET(pane), TRUE, TRUE, 0);

  gtk_paned_pack1( GTK_PANED(pane), GTK_WIDGET(brow_widget), TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(pane), GTK_WIDGET(statusbar), FALSE, TRUE);
  
  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  cmd_scrolled_buffer = gtk_text_buffer_new( NULL);

  cmd_scrolledtextview = gtk_text_view_new_with_buffer( cmd_scrolled_buffer);
  GtkWidget *viewport = gtk_viewport_new( NULL, NULL);
  GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add( GTK_CONTAINER(viewport), cmd_scrolledtextview);
  gtk_container_add( GTK_CONTAINER(scrolledwindow), viewport);

  cmd_scrolled_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( cmd_scrolled_ok, 70, 25);
  g_signal_connect( cmd_scrolled_ok, "clicked", 
 		    G_CALLBACK(attr_activate_cmd_scrolled_ok), this);
  cmd_scrolled_ca = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( cmd_scrolled_ca, 70, 25);
  g_signal_connect( cmd_scrolled_ca, "clicked", 
 		    G_CALLBACK(attr_activate_cmd_scrolled_ca), this);

  GtkWidget *hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(hboxbuttons), cmd_scrolled_ok, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(hboxbuttons), cmd_scrolled_ca, FALSE, FALSE, 0);

  cmd_scrolledinput = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(cmd_scrolledinput), scrolledwindow, TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(cmd_scrolledinput), hboxbuttons, FALSE, FALSE, 5);

  gtk_box_pack_start( GTK_BOX(statusbar), cmd_scrolledinput, TRUE, TRUE, 0);

  gtk_widget_show_all( toplevel);

  g_object_set( cmd_prompt, "visible", FALSE, NULL);
  g_object_set( cmd_input, "visible", FALSE, NULL);
  g_object_set( cmd_scrolledinput, "visible", FALSE, NULL);
  
  int w, h;
  gdk_drawable_get_size( pane->window, &w, &h);
  gtk_paned_set_position( GTK_PANED(pane), h - 50);
  
}

AttrGtk::~AttrGtk()
{
  if ( original_data)
    delete original_data;
  delete (AttrNavGtk *)attrnav;

  gtk_widget_destroy( toplevel);
}
