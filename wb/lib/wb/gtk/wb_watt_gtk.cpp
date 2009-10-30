/* 
 * Proview   $Id: wb_watt_gtk.cpp,v 1.7 2008-10-31 12:51:31 claes Exp $
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

/* wb_watt_gtk.cpp -- Display object attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_wow_gtk.h"
#include "wb_watt_msg.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_watt_gtk.h"
#include "wb_wattnav_gtk.h"
#include "wb_wtt.h"
#include "wb_wnav.h"
#include "co_xhelp.h"

CoWowRecall WAttGtk::value_recall;

void WAttGtk::message( char severity, const char *message)
{
  gtk_label_set_text( GTK_LABEL(msg_label), message);
}

void WAttGtk::set_prompt( const char *prompt)
{
  if ( strcmp(prompt, "") == 0) {
    g_object_set( cmd_prompt, "visible", FALSE, NULL);
    g_object_set( msg_label, "visible", TRUE, NULL);
  }
  else {
    g_object_set( msg_label, "visible", FALSE, NULL);
    g_object_set( cmd_prompt, "visible", TRUE, NULL);
  }
}

void WAttGtk::change_value( int set_focus)
{
  int		sts;
  GtkWidget	*text_w;
  int		multiline;
  char		*value;
  int		input_size;

  if ( input_open) {
    g_object_set( cmd_input, "visible", FALSE, NULL);
    g_object_set( cmd_scrolledinput, "visible", FALSE, NULL);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = wattnav->check_attr( &multiline, &input_node, input_name,
			     &value, &input_size);
  if ( EVEN(sts)) {
    if ( sts == WATT__NOATTRSEL)
      message( 'E', "No attribute is selected");
    else
      message( 'E', wnav_get_message( sts));
    return;
  }

  if ( multiline) {
    text_w = cmd_scrolledinput;
    g_object_set( text_w, "visible", TRUE, NULL);

    int w, h;
    gdk_drawable_get_size( pane->window, &w, &h);
    gtk_paned_set_position( GTK_PANED(pane), h - 170);
    gtk_widget_grab_focus( cmd_scrolledtextview);
    input_max_length = input_size;
    input_multiline = 1;
  }
  else {
    text_w = cmd_input;
    g_object_set( text_w, 
		  "visible", TRUE, 
		  "max-length", input_size, NULL);
    gtk_widget_grab_focus( cmd_input);
    input_multiline = 0;
  }

  message( ' ', "");

  if ( value) {
    if ( multiline) {
      GtkTextIter start_iter, end_iter;
      gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
      gtk_text_buffer_get_end_iter( cmd_scrolled_buffer, &end_iter);
      gtk_text_buffer_delete( cmd_scrolled_buffer, &start_iter, &end_iter);

      gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);

      char *textutf8 = g_convert( value, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
      gtk_text_buffer_insert( cmd_scrolled_buffer, &start_iter, textutf8, -1);
      g_free( textutf8);

      // Select the text
      // gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
      // gtk_text_buffer_get_end_iter( cmd_scrolled_buffer, &end_iter);
      // gtk_text_buffer_select_range( cmd_scrolled_buffer, &start_iter, &end_iter);
    }
    else {
      char *textutf8 = g_convert( value, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
      gint pos = 0;
      gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);
      gtk_editable_insert_text( GTK_EDITABLE(text_w), textutf8, strlen(textutf8), &pos);
      g_free( textutf8);

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

void WAttGtk::action_text_inserted( GtkTextBuffer *w, GtkTextIter *iter, gchar *str, gint len, gpointer data)
{
  WAttGtk *watt = (WAttGtk *)data;

  int count = gtk_text_buffer_get_char_count( w);  

  if ( count > watt->input_max_length) {
    // Remove inserted chars (note that iter now points at the end of the inserted text)
    GtkTextIter start_iter;

    int offs = gtk_text_iter_get_offset( iter);
    gtk_text_buffer_get_iter_at_offset( w, &start_iter, offs - len);
    gtk_text_buffer_delete( w, &start_iter, iter);

    CoWowGtk wow( watt->toplevel);
    wow.DisplayError( "Error message", "Attribute size exceeded");
  }
  else
    watt->message( ' ', "");
}
//
//  Callbackfunctions from menu entries
//
void WAttGtk::activate_change_value( GtkWidget *w, gpointer data)
{
  WAtt *watt = (WAtt *)data;
  watt->change_value(1);
}

void WAttGtk::activate_close_changeval( GtkWidget *w, gpointer data)
{
  WAtt *watt = (WAtt *)data;
  watt->change_value_close();
}

void WAttGtk::activate_exit( GtkWidget *w, gpointer data)
{
  WAtt *watt = (WAtt *)data;
  if ( watt->close_cb)
    (watt->close_cb)( watt);
  else
    delete watt;
}

void WAttGtk::activate_help( GtkWidget *w, gpointer data)
{
  CoXHelp::dhelp( "objecteditor_refman", 0, navh_eHelpFile_Other, 
		  "$pwr_lang/man_dg.dat", true);
}

gboolean WAttGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  WAttGtk *watt = (WAttGtk *)data;
  gboolean scrolledinput_visible;
  gboolean input_visible;

  if ( watt->focustimer.disabled())
    return FALSE;

  g_object_get( watt->cmd_scrolledinput, "visible", &scrolledinput_visible, NULL);
  g_object_get( watt->cmd_input, "visible", &input_visible, NULL);
  if ( scrolledinput_visible)
    gtk_widget_grab_focus( watt->cmd_scrolledtextview);
  else if ( input_visible)
    gtk_widget_grab_focus( watt->cmd_input);
  else if ( watt->wattnav)
    watt->wattnav->set_inputfocus();

  watt->focustimer.disable( 400);

  return FALSE;
}

#if 0
void WAttGtk::valchanged_cmd_input( Widget w, XEvent *event)
{
  WAtt 	*watt;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &watt);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)WAtt::value_recall, sizeof(WAtt::value_recall[0]),
	sizeof( WAtt::value_recall)/sizeof(WAtt::value_recall[0]),
	&watt->value_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( watt->input_open)
    {
      sts = ((WAttNav *)watt->wattnav)->set_attr_value( watt->input_node, 
		watt->input_name, text);
      XtUnmanageChild( w);
      watt->set_prompt( "");
      watt->input_open = 0;
      if ( watt->redraw_cb)
        (watt->redraw_cb)( watt);

      ((WAttNav *)watt->wattnav)->set_inputfocus();
    }
  }
}
#endif

void WAttGtk::change_value_close()
{
  unsigned char *s;
  int sts;

  if ( input_open) {
    if ( input_multiline) {
      gchar *text, *textutf8;
      GtkTextIter start_iter, end_iter;
      gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
      gtk_text_buffer_get_end_iter( cmd_scrolled_buffer, &end_iter);

      textutf8 = gtk_text_buffer_get_text( cmd_scrolled_buffer, &start_iter, &end_iter,
				       FALSE);
      text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
      g_free( textutf8);

      // Replace ctrl characters with space
      for ( s = (unsigned char *) text; *s; s++) {
	if ( *s < ' ' && *s != 10 && *s != 13)
	  *s = ' ';
      }

      sts = wattnav->set_attr_value( input_node,
				     input_name, text);
      g_free( text);
      g_object_set( cmd_scrolledinput, "visible", FALSE, NULL);
      set_prompt( "");
      input_open = 0;

      int w, h;
      gdk_drawable_get_size( pane->window, &w, &h);
      gtk_paned_set_position( GTK_PANED(pane), h - 50);

      wattnav->redraw();
      wattnav->set_inputfocus();
    }
    else {
      char *text, *textutf8;
      textutf8 = gtk_editable_get_chars( GTK_EDITABLE(cmd_input), 0, -1);
      text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
      g_free( textutf8);

      sts = wattnav->set_attr_value( input_node, 
				     input_name, text);
      g_free( text);
      g_object_set( cmd_input, "visible", FALSE, NULL);
      set_prompt( "");
      input_open = 0;
      if ( redraw_cb)
        (redraw_cb)( this);

      wattnav->set_inputfocus();
    }
  }
}

void WAttGtk::activate_cmd_input( GtkWidget *w, gpointer data)
{
  char *text, *textutf8;
  WAttGtk *watt = (WAttGtk *)data;
  int sts;

  g_object_set( watt->cmd_prompt, "visible", FALSE, NULL);
  g_object_set( watt->cmd_input, "visible", FALSE, NULL);

  watt->wattnav->set_inputfocus();

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(w), 0, -1);
  text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);
  if ( watt->input_open) {
    sts = ((WAttNav *)watt->wattnav)->set_attr_value( watt->input_node, 
						      watt->input_name, text);
    g_object_set( w, "visible", FALSE, NULL);
    watt->set_prompt( "");
    watt->input_open = 0;
    if ( watt->redraw_cb)
      (watt->redraw_cb)( watt);
  }
  g_free( text);
}

void WAttGtk::activate_cmd_scrolled_ok( GtkWidget *w, gpointer data)
{
  WAttGtk *watt = (WAttGtk *)data;
  gchar *text, *textutf8;
  unsigned char *s;
  int sts;

  if ( watt->input_open) {
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter( watt->cmd_scrolled_buffer, &start_iter);
    gtk_text_buffer_get_end_iter( watt->cmd_scrolled_buffer, &end_iter);

    textutf8 = gtk_text_buffer_get_text( watt->cmd_scrolled_buffer, &start_iter, &end_iter,
				     FALSE);
    text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
    g_free( textutf8);

    // Replace ctrl characters with space
    for ( s = (unsigned char *) text; *s; s++) {
      if ( *s < ' ' && *s != 10 && *s != 13)
	*s = ' ';
    }

    sts = ((WAttNav *)watt->wattnav)->set_attr_value( watt->input_node,
						      watt->input_name, text);
    g_object_set( watt->cmd_scrolledinput, "visible", FALSE, NULL);
    watt->set_prompt( "");
    watt->input_open = 0;

    int w, h;
    gdk_drawable_get_size( watt->pane->window, &w, &h);
    gtk_paned_set_position( GTK_PANED(watt->pane), h - 50);

    ((WAttNav *)watt->wattnav)->redraw();
    ((WAttNav *)watt->wattnav)->set_inputfocus();
    g_free( text);
  }
}

void WAttGtk::activate_cmd_scrolled_ca( GtkWidget *w, gpointer data)
{
  WAttGtk *watt = (WAttGtk *)data;

  if ( watt->input_open) {
    g_object_set( watt->cmd_scrolledinput, "visible", FALSE, NULL);

    int w, h;
    gdk_drawable_get_size( watt->pane->window, &w, &h);
    gtk_paned_set_position( GTK_PANED(watt->pane), h - 50);

    watt->set_prompt( "");
    watt->input_open = 0;
    watt->wattnav->set_inputfocus();
  }
}

int WAttGtk::open_changevalue( char *name)
{
  int sts;

  sts = ((WAttNav*)wattnav)->select_by_name( name);
  if ( EVEN(sts)) return sts;

  change_value(0);
  return WATT__SUCCESS;
}

void WAttGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

WAttGtk::~WAttGtk()
{
  delete (WAttNav *)wattnav;
  delete cmd_entry;
  gtk_widget_destroy( toplevel);
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  WAttGtk *watt = (WAttGtk *)data;

  if ( watt->close_cb)
    (watt->close_cb)( watt);
  else
    delete watt;
  
  return FALSE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

WAttGtk::WAttGtk( 
	GtkWidget      	*wa_parent_wid,
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses, 
	pwr_sAttrRef 	wa_aref,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname) :
  WAtt(wa_parent_ctx,wa_ldhses,wa_aref,wa_editmode,wa_advanced_user,
       wa_display_objectname), parent_wid(wa_parent_wid)
{
  int sts;
  int size;
  char *namep;

  sts = ldh_AttrRefToName( ldhses, &aref, ldh_eName_Hierarchy, 
			   &namep, &size);
  if ( EVEN(sts)) return;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", 570,
					 "default-width", 410,
					 "title", CoWowGtk::convert_utf8(namep),
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
  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(activate_exit), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Functions entry
  GtkWidget *func_changevalue = gtk_menu_item_new_with_mnemonic( "_Change Value");
  g_signal_connect( func_changevalue, "activate", 
		    G_CALLBACK(activate_change_value), this);
  gtk_widget_add_accelerator( func_changevalue, "activate", accel_g,
			      'q', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *func_close_changeval = gtk_menu_item_new_with_mnemonic( "C_lose Change Value");
  g_signal_connect( func_close_changeval, "activate", 
		    G_CALLBACK(activate_close_changeval), this);
  gtk_widget_add_accelerator( func_close_changeval, "activate", accel_g,
			      't', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkMenu *func_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), func_changevalue);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), func_close_changeval);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("_Functions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(func_menu));

  // Help entry
  GtkWidget *help_help = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_g);
  g_signal_connect(help_help, "activate", G_CALLBACK(activate_help), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  pane = gtk_vpaned_new();

  if ( ((WUtility *)parent_ctx)->utype == wb_eUtility_WNav)
    parent_ctx = ((WNav *)parent_ctx)->parent_ctx;
  utility = ((WUtility *)parent_ctx)->utype;
  
  wattnav = new WAttNavGtk( (void *)this, wattnav_eType_Object, pane, "Plant",
			      ldhses, aref, wa_editmode, wa_advanced_user,
			      wa_display_objectname, utility, &brow_widget, &sts);
  ((WAttNav *)wattnav)->message_cb = &WAtt::message_cb;
  ((WAttNav *)wattnav)->change_value_cb = &WAtt::change_value_cb;
  
  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);
  cmd_prompt = gtk_label_new( "value > ");
  gtk_widget_set_size_request( cmd_prompt, -1, 25);
  cmd_entry = new CoWowEntryGtk( &value_recall);
  cmd_input = cmd_entry->widget();
  gtk_widget_set_size_request( cmd_input, -1, 25);
  g_signal_connect( cmd_input, "activate", 
		    G_CALLBACK(activate_cmd_input), this);

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_prompt, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_input, TRUE, TRUE, 0);

  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(vbox), GTK_WIDGET(pane), TRUE, TRUE, 0);

  gtk_paned_pack1( GTK_PANED(pane), GTK_WIDGET(brow_widget), TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(pane), GTK_WIDGET(statusbar), FALSE, TRUE);
  
  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  cmd_scrolled_buffer = gtk_text_buffer_new( NULL);
  g_signal_connect_after( cmd_scrolled_buffer, "insert-text", 
 		    G_CALLBACK(action_text_inserted), this);

  cmd_scrolledtextview = gtk_text_view_new_with_buffer( cmd_scrolled_buffer);
  GtkWidget *viewport = gtk_viewport_new( NULL, NULL);
  GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add( GTK_CONTAINER(viewport), cmd_scrolledtextview);
  gtk_container_add( GTK_CONTAINER(scrolledwindow), viewport);

  cmd_scrolled_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( cmd_scrolled_ok, 70, 25);
  g_signal_connect( cmd_scrolled_ok, "clicked", 
 		    G_CALLBACK(activate_cmd_scrolled_ok), this);
  cmd_scrolled_ca = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( cmd_scrolled_ca, 70, 25);
  g_signal_connect( cmd_scrolled_ca, "clicked", 
 		    G_CALLBACK(activate_cmd_scrolled_ca), this);

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

  if ( utility == wb_eUtility_Wtt) {
    ((Wtt *)parent_ctx)->register_utility( (void *) this,
					   wb_eUtility_AttributeEditor);
  }
}











