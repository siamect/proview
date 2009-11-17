/* 
 * Proview   $Id: xtt_xattone_gtk.cpp,v 1.2 2008-10-31 12:51:36 claes Exp $
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

/* xtt_xattone.cpp -- Display object attributes */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "cow_wow_gtk.h"
#include "rt_xnav_msg.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "co_lng.h"
#include "xtt_xattone_gtk.h"
#include "xtt_xattnav.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"
#include "pwr_privilege.h"

CoWowRecall XAttOneGtk::value_recall;

void XAttOneGtk::message( char severity, const char *message)
{
  gtk_label_set_text( GTK_LABEL(msg_label), message);
}

void XAttOneGtk::set_prompt( char *prompt)
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

int XAttOneGtk::set_value()
{
  char *text;
  int sts;
  char buff[1024];

  if ( input_open) {
    if ( input_multiline) {
      GtkTextIter start_iter, end_iter;
      gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
      gtk_text_buffer_get_end_iter( cmd_scrolled_buffer, &end_iter);

      text = gtk_text_buffer_get_text( cmd_scrolled_buffer, &start_iter, &end_iter,
				       FALSE);
    }
    else {
      text = gtk_editable_get_chars( GTK_EDITABLE(cmd_input), 0, -1);
    }

    sts =  XNav::attr_string_to_value( atype, text, buff, sizeof(buff), 
				       asize);
    g_free( text);
    if ( EVEN(sts)) {
      message( 'E', "Input syntax error");
      return sts;
    }
    sts = gdh_SetObjectInfoAttrref( &aref, buff, asize);
    if ( EVEN(sts)) {
      message( 'E', "Unable to set value");
      return sts;
    }
    message( ' ', "");
  }
  return XATT__SUCCESS;
}

int XAttOneGtk::change_value( int set_focus)
{
  int		sts;
  GtkWidget	*text_w;
  char		*value = 0;
  int		input_size;
  char		aval[1024];
  char 		buf[1024];
  int		len;

  sts = gdh_GetAttributeCharAttrref( &aref, &atype, &asize, &aoffs, &aelem);
  if ( EVEN(sts)) return sts;
  
  switch ( atype) {
  case pwr_eType_String:
  case pwr_eType_Text:
    input_size = asize;
    break;
  default:
    input_size = 80;
  }

  sts = gdh_GetObjectInfoAttrref( &aref, aval, sizeof(aval));
  if ( EVEN(sts)) return sts;

  XNav::attrvalue_to_string( atype, atype, &aval, buf, sizeof(buf), &len, NULL);

  value = buf;

  if ( !access_rw) {
    gtk_label_set_text( GTK_LABEL(cmd_label), buf);
  }
  else {
    if ( atype == pwr_eType_Text) {
      text_w = cmd_scrolledinput;
      g_object_set( cmd_input, "visible", FALSE, NULL);
      g_object_set( cmd_scrolledinput, "visible", TRUE, NULL);

      // int w, h;
      // gdk_drawable_get_size( pane->window, &w, &h);
      // gtk_paned_set_position( GTK_PANED(pane), h - 170);
      if ( set_focus)
	gtk_widget_grab_focus( cmd_scrolledtextview);
      input_multiline = 1;

#if 0
      gtk_entry_set_max_length( text_w, input_size-1);
#endif
      if ( value) {
	GtkTextIter start_iter, end_iter;
	gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
	gtk_text_buffer_get_end_iter( cmd_scrolled_buffer, &end_iter);
	gtk_text_buffer_delete( cmd_scrolled_buffer, &start_iter, &end_iter);
	
	gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
	gtk_text_buffer_insert( cmd_scrolled_buffer, &start_iter, value, -1);
      }
      else {
	GtkTextIter start_iter, end_iter;
	gtk_text_buffer_get_start_iter( cmd_scrolled_buffer, &start_iter);
	gtk_text_buffer_get_end_iter( cmd_scrolled_buffer, &end_iter);
	gtk_text_buffer_delete( cmd_scrolled_buffer, &start_iter, &end_iter);
      }
    }
    else {
      text_w = cmd_input;
      g_object_set( cmd_input, "visible", TRUE, NULL);
      g_object_set( cmd_scrolledinput, "visible", FALSE, NULL);
      if ( set_focus)
	gtk_widget_grab_focus( cmd_input);
      input_multiline = 0;

      gtk_entry_set_max_length( GTK_ENTRY(text_w), input_size-1);

      gint pos = 0;
      gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);

      if ( value) {
	gtk_editable_insert_text( GTK_EDITABLE(text_w), value, strlen(value), &pos);

	// Select the text
	gtk_editable_set_position( GTK_EDITABLE(cmd_input), -1);
	gtk_editable_select_region( GTK_EDITABLE(cmd_input), 0, -1);
      }
    }
    message( ' ', "");
    set_prompt( Lng::translate("value >"));
    input_open = 1;
  }
  return XATT__SUCCESS;
}

//
//  Callbackfunctions from menu entries
//

void XAttOneGtk::activate_exit( GtkWidget *w, gpointer data)
{
  XAttOne *xattone = (XAttOne *)data;

  if ( xattone->close_cb)
    (xattone->close_cb)( xattone->parent_ctx, xattone);
  else
    delete xattone;
}

void XAttOneGtk::activate_help( GtkWidget *w, gpointer data)
{
  // Not yet implemented
}

gboolean XAttOneGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  XAttOneGtk *xattone = (XAttOneGtk *)data;

  if ( xattone->input_open) {
    if ( xattone->input_multiline)
      gtk_widget_grab_focus( xattone->cmd_scrolledtextview);
    else
      gtk_widget_grab_focus( xattone->cmd_input);
  }
  return FALSE;
}

#if 0
void XAttOneGtk::valchanged_cmd_input( Widget w, XEvent *event)
{
  XAttOne 	*xattone;
  int 	sts;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &xattone);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)XAttOne::value_recall, sizeof(XAttOne::value_recall[0]),
	sizeof( XAttOne::value_recall)/sizeof(XAttOne::value_recall[0]),
	&xattone->value_current_recall);
  if ( sts) {
    sts = xattone->set_value();
    if ( ODD(sts)) {
      if ( xattone->close_cb)
	(xattone->close_cb)( xattone->parent_ctx, xattone);
      else
	delete xattone;
    }
  }
}
#endif

void XAttOneGtk::change_value_close()
{
  set_value();
}

void XAttOneGtk::activate_cmd_input( GtkWidget *w, gpointer data)
{
  activate_cmd_scrolled_ok( w, data);
}

void XAttOneGtk::activate_cmd_scrolled_ok( GtkWidget *w, gpointer data)
{
  XAttOne *xattone = (XAttOne *)data;
  int sts;

  sts = xattone->set_value();
  if ( ODD(sts)) {
    if ( xattone->close_cb)
      (xattone->close_cb)( xattone->parent_ctx, xattone);
    else
      delete xattone;
  }
}

void XAttOneGtk::activate_cmd_scrolled_ap( GtkWidget *w, gpointer data)
{
  XAttOne *xattone = (XAttOne *)data;

  xattone->set_value();
}

void XAttOneGtk::activate_cmd_scrolled_ca( GtkWidget *w, gpointer data)
{
  XAttOne *xattone = (XAttOne *)data;

  if ( xattone->close_cb)
    (xattone->close_cb)( xattone->parent_ctx, xattone);
  else
    delete xattone;
}

void XAttOneGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

XAttOneGtk::~XAttOneGtk()
{
  delete cmd_entry;
  gtk_widget_destroy( toplevel);
}

static gboolean delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  XAttOneGtk::activate_exit( w, data);
  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

XAttOneGtk::XAttOneGtk( GtkWidget *xa_parent_wid,
			void *xa_parent_ctx, 
			pwr_sAttrRef *xa_aref,
			char *xa_title,
			unsigned int xa_priv,
			int *xa_sts) :
  XAttOne( xa_parent_ctx, xa_aref, xa_title, xa_priv, xa_sts), 
  parent_wid(xa_parent_wid)
{
  pwr_tAName   	title;

  *xa_sts = gdh_AttrrefToName( &aref, title, sizeof(title), cdh_mNName);
  if ( EVEN(*xa_sts)) return;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 200,
			   "default-width", 500,
			   "title", title,
			   NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

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

  // Help entry
  GtkWidget *help_help = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_g);
  g_signal_connect(help_help, "activate", G_CALLBACK(activate_help), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Prompt, label, input entry
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);
  cmd_prompt = gtk_label_new( "value > ");
  gtk_widget_set_size_request( cmd_prompt, -1, 25);
  cmd_label = gtk_label_new("");
  gtk_widget_set_size_request( cmd_label, -1, 25);
  gtk_misc_set_alignment( GTK_MISC(cmd_label), 0.0, 0.5);
  cmd_entry = new CoWowEntryGtk( &value_recall);
  cmd_input = cmd_entry->widget();
  gtk_widget_set_size_request( cmd_input, -1, 25);
  g_signal_connect( cmd_input, "activate", 
		    G_CALLBACK(activate_cmd_input), this);

  // Scrolled text input
  cmd_scrolled_buffer = gtk_text_buffer_new( NULL);

  cmd_scrolledtextview = gtk_text_view_new_with_buffer( cmd_scrolled_buffer);
  GtkWidget *viewport = gtk_viewport_new( NULL, NULL);
  GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add( GTK_CONTAINER(viewport), cmd_scrolledtextview);
  gtk_container_add( GTK_CONTAINER(scrolledwindow), viewport);

  cmd_scrolledinput = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(cmd_scrolledinput), scrolledwindow, TRUE, TRUE, 0);

  // Buttons
  cmd_scrolled_ok = gtk_button_new_with_label( CoWowGtk::translate_utf8("Ok"));
  gtk_widget_set_size_request( cmd_scrolled_ok, 70, 25);
  g_signal_connect( cmd_scrolled_ok, "clicked", 
 		    G_CALLBACK(activate_cmd_scrolled_ok), this);

  cmd_scrolled_ap = gtk_button_new_with_label( CoWowGtk::translate_utf8("Apply"));
  gtk_widget_set_size_request( cmd_scrolled_ap, 70, 25);
  g_signal_connect( cmd_scrolled_ap, "clicked", 
 		    G_CALLBACK(activate_cmd_scrolled_ap), this);

  cmd_scrolled_ca = gtk_button_new_with_label( CoWowGtk::translate_utf8("Cancel"));
  gtk_widget_set_size_request( cmd_scrolled_ca, 70, 25);
  g_signal_connect( cmd_scrolled_ca, "clicked", 
 		    G_CALLBACK(activate_cmd_scrolled_ca), this);

  GtkWidget *hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(hboxbuttons), cmd_scrolled_ok, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hboxbuttons), cmd_scrolled_ap, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(hboxbuttons), cmd_scrolled_ca, FALSE, FALSE, 0);

  // Horizontal box
  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_prompt, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_label, TRUE, TRUE, 15);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_input, TRUE, TRUE, 15);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_scrolledinput, TRUE, TRUE, 15);


  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);

  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(statusbar), TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(gtk_hseparator_new()), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(vbox), hboxbuttons, FALSE, FALSE, 5);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  gtk_widget_show_all( toplevel);

  if ( priv & pwr_mPrv_RtWrite || priv & pwr_mPrv_System)
    access_rw = 1;
  else
    access_rw = 0;

  if ( access_rw)
    g_object_set( cmd_label, "visible", FALSE, NULL);
  else {
    g_object_set( cmd_input, "visible", FALSE, NULL);
    g_object_set( cmd_scrolledinput, "visible", FALSE, NULL);
    g_object_set( cmd_scrolled_ok, "visible", FALSE, NULL);
    g_object_set( cmd_scrolled_ap, "visible", FALSE, NULL);
  }

  change_value( 1);

  *xa_sts = XATT__SUCCESS;
}



