/* 
 * Proview   $Id: rt_xtt_gtk.cpp,v 1.12 2008-02-27 06:59:27 claes Exp $
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

/* rt_xtt_gtk.cpp -- Display plant and node hiererachy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_ini_event.h"
#include "rt_qcom_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_wow_gtk.h"
#include "pwr_baseclasses.h"
#include "pwr_nmpsclasses.h"

#include <gtk/gtk.h>

#include "xtt_hotkey_gtk.h"
#include "co_dcli.h"

#include "co_lng.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "rt_xtt_gtk.h"
//#include "rt_trace.h"
#include "xtt_xnav_gtk.h"
#include "xtt_item.h"
#include "xtt_url.h"
#include "co_lng.h"
#include "co_xhelp_gtk.h"
#include "co_wow_gtk.h"
#include "rt_xnav_msg.h"
#include "co_syi.h"
#include "xtt_methodtoolbar_gtk.h"

void XttGtk::hotkey_Command( char *arg, void *userdata)
{
  Xtt *xtt = (Xtt *)userdata;

  xtt->hotkey_activate_command( arg);
}

void XttGtk::hotkey_ToggleDig( char *arg, void *userdata)
{
  hotkey_activate_toggledig( arg);
}

void XttGtk::hotkey_SetDig( char *arg, void *userdata)
{
  hotkey_activate_setdig( arg);
}

void XttGtk::hotkey_ResetDig( char *arg, void *userdata)
{
  hotkey_activate_resetdig( arg);
}

static GdkFilterReturn xtt_hotkey_filter( GdkXEvent *xevent, GdkEvent *event, gpointer data)
{
  return (GdkFilterReturn) XttHotkey::event_handler( xevent, data);
}

void XttGtk::xtt_mainloop()
{
  gtk_main();
}

#if 0
int XttGtk::init_hotkey( XtAppContext AppCtx, Widget Top)
{
  hotkeySTATUS    HotkeySts;

  XtAppAddActions( AppCtx, HotkeyActions, XtNumber(HotkeyActions));

  HotkeySts = hotkey_Initialize (Top, &HotkeyHandle);
  switch ( HotkeySts ) {
  case TkERROR : 
    puts ("Xtt: Failed to bind hotkey(s)");
    break;
  case TkNO_BINDINGS :
    puts ("OpWin: No hotkey resource found in resource db");
    break;
  case TkPARSE_ERROR :
    puts ("OpWin: Syntax error in hotkey bindings");
  }
  return 1;
}
#endif

void XttGtk::open_input_dialog( char *text, char *title,
			     char *init_text,
			     void (*ok_cb)( Xtt *, char *))
{
  create_input_dialog();

  g_object_set( india_widget, 
		"visible", TRUE, 
		"title", CoWowGtk::translate_utf8(title),
		NULL);

  gtk_label_set_text( GTK_LABEL(india_label), CoWowGtk::translate_utf8(text));

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(india_text), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(india_text), init_text, 
			    strlen(init_text), &pos);

  india_ok_cb = ok_cb;
  gtk_widget_grab_focus( india_text);
}

void XttGtk::message( char severity, char *msg)
{
  char *messageutf8 = g_convert( msg, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  gtk_label_set_text( GTK_LABEL(msg_label), messageutf8);
  g_free( messageutf8);
}

void XttGtk::close( void *ctx, int terminate)
{
  Xtt	*xtt = (Xtt *) ctx;
  char  title[80];

  if ( terminate) {
    strcpy( title, CoWowGtk::translate_utf8("Confirm"));
    xtt->wow->DisplayQuestion( xtt, title, 
			 Lng::translate( "Do you want to close"), close_ok, 0, 0);
  }
  else {
    xtt->xnav->displayed = 0;
    g_object_set( ((XttGtk *)xtt)->toplevel, "visible", FALSE, NULL);
  }
}

void XttGtk::set_dimension( void *ctx, int width, int height)
{
  Xtt	*xtt = (Xtt *) ctx;

  gtk_window_resize( GTK_WINDOW(((XttGtk *)xtt)->toplevel), width, height);
}

void XttGtk::map( void *ctx)
{
  Xtt	*xtt = (Xtt *) ctx;

  gtk_window_present( GTK_WINDOW(((XttGtk *)xtt)->toplevel));
  xtt->xnav->displayed = 1;
}

void XttGtk::set_prompt( char *prompt)
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

void XttGtk::open_change_value()
{
  int		sts;

  if ( input_open) {
    g_object_set( cmd_input, "visible", FALSE, NULL);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = xnav->check_attr_value();
  if ( EVEN(sts)) {
    message( 'E', XNav::get_message(sts));
    return;
  }

  if ( command_open)
    command_open = 0;
  else
    g_object_set( cmd_input, "visible", TRUE, NULL);

  message( ' ', "");
  gtk_widget_grab_focus( cmd_input);

  gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);
  set_prompt( Lng::translate("value >"));
  input_open = 1;
}

//
//  Callbackfunctions from menu entries
//
void XttGtk::activate_change_value( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->open_change_value();
}

void XttGtk::activate_command( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  if ( xtt->command_open) {
    g_object_set( ((XttGtk *)xtt)->cmd_input, "visible", FALSE, NULL);
    xtt->set_prompt( "");
    xtt->command_open = 0;
    return;
  }

  if ( xtt->input_open)
    xtt->input_open = 0;
  else
    g_object_set( ((XttGtk *)xtt)->cmd_input, "visible", TRUE, NULL);
  xtt->message( ' ', "");
  gtk_widget_grab_focus( ((XttGtk *)xtt)->cmd_input);

  gtk_editable_delete_text( GTK_EDITABLE(((XttGtk *)xtt)->cmd_input), 0, -1);
  xtt->set_prompt( "xtt >");
  xtt->command_open = 1;
}

void XttGtk::activate_exit( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  close( xtt, xtt->xnav->op ? 0 : 1);
}

void XttGtk::activate_print( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_print();
}

void XttGtk::activate_find( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_find();
}

void XttGtk::activate_findregex( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_findregex();
}

void XttGtk::activate_findnext( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_findnext();
}

void XttGtk::activate_collapse( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_collapse();
}

void XttGtk::activate_openobject( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_openobject();
}

void XttGtk::activate_openplc( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_openplc();
}

void XttGtk::activate_showcrossref( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->xnav->show_crossref();
}

void XttGtk::activate_opengraph( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_opengraph();
}

void XttGtk::activate_collect_insert( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_collect_insert();
}

void XttGtk::activate_collect_show( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->xnav->collect_show();
}

void XttGtk::activate_collect_remove( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->xnav->collect_remove();
}

void XttGtk::activate_collect_clear( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->xnav->collect_clear();
}

void XttGtk::activate_advanceduser( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_advanceduser();
}

void XttGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_zoom_in();
}

void XttGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_zoom_out();
}

void XttGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->xnav->unzoom();
}

void XttGtk::activate_help( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_help();
}

void XttGtk::activate_help_project( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_help_project();
}

void XttGtk::activate_help_proview( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->activate_help_proview();
}

void XttGtk::activate_back( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  xtt->xnav->brow_push();
}

void XttGtk::activate_india_ok( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;
  char *text, *textutf8;

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(((XttGtk *)xtt)->india_text), 
				 0, -1);
  text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  g_object_set( ((XttGtk *)xtt)->india_widget, "visible", FALSE, NULL);

  (xtt->india_ok_cb)( xtt, text);
}
void XttGtk::activate_india_cancel( GtkWidget *w, gpointer data)
{
  Xtt *xtt = (Xtt *)data;

  g_object_set( ((XttGtk *)xtt)->india_widget, "visible", FALSE, NULL);
}

gboolean XttGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  XttGtk *xtt = (XttGtk *)data;

  if ( xtt->focustimer.disabled()) {
    return FALSE;
  }

  if ( xtt->xnav) {
    xtt->xnav->set_inputfocus();
  }
  xtt->focustimer.disable( 400);

  return FALSE;
}

void XttGtk::valchanged_cmd_input( GtkWidget *w, gpointer data)
{
  Xtt 	*xtt = (Xtt *)data;
  int 	sts;
  char 	*text, *textutf8;

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(w), 0, -1);
  text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  if ( xtt->input_open) {
    sts = xtt->xnav->set_attr_value( text);
    g_object_set( w, "visible", FALSE, NULL);
    xtt->set_prompt( "");
    xtt->input_open = 0;
    xtt->xnav->set_inputfocus();
  }
  else if ( xtt->command_open) {
    sts = xtt->xnav->command( text);
    g_object_set( w, "visible", FALSE, NULL);
    xtt->set_prompt( "");
    xtt->command_open = 0;
    xtt->xnav->set_inputfocus();
  }
}

int main(  int argc, char *argv[])
{
  int sts;

  gtk_init( &argc, &argv);

  new XttGtk( argc, argv, &sts);
  exit(sts);
}

XttGtk::~XttGtk()
{
  delete cmd_entry;
  delete cmd_recall;
  delete value_recall;
  delete hotkey;
  delete methodtoolbar;
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer xtt)
{
  XttGtk::activate_exit(w, xtt);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

XttGtk::XttGtk( int argc, char *argv[], int *return_sts) :
  Xtt( argc, argv, return_sts)
{
  const int	window_width = 400;
  const int    	window_height = 700;
  pwr_tStatus   sts;
  pwr_tFileName fname;
  char		title[120] = "Xtt ";
  char		nodename[80];

  syi_NodeName( &sts, nodename, sizeof(nodename));
  if ( ODD(sts))
    strcat( title, nodename);

  // Gtk
  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", title,
					 NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(XttGtk::action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);
 
  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  GtkWidget *file_print = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Print"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_print), 
				 gtk_image_new_from_stock( "gtk-print", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_print, "activate", G_CALLBACK(XttGtk::activate_print), this);

  GtkWidget *file_close = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Close"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_close), 
				 gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_close, "activate", G_CALLBACK(XttGtk::activate_exit), this);
  gtk_widget_add_accelerator( file_close, "activate", accel_g,
			      'w', GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_File"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Edit Entry
  // Submenu Search
  GtkWidget *edit_search_findobject = gtk_menu_item_new_with_mnemonic( 
					      CoWowGtk::translate_utf8("_Find Object"));
  g_signal_connect( edit_search_findobject, "activate", 
		    G_CALLBACK(XttGtk::activate_find), this);
  gtk_widget_add_accelerator( edit_search_findobject, "activate", accel_g,
			      'f', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *edit_search_findregex = gtk_menu_item_new_with_mnemonic( 
                                    CoWowGtk::translate_utf8("Find _Regular expression"));
  g_signal_connect( edit_search_findregex, "activate", 
		    G_CALLBACK(XttGtk::activate_findregex), this);

  GtkWidget *edit_search_findnext = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Find _Next"));
  g_signal_connect( edit_search_findnext, "activate", 
		    G_CALLBACK(XttGtk::activate_findnext), this);
  gtk_widget_add_accelerator( edit_search_findnext, "activate", accel_g,
			      'f', GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *edit_search = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Search"));
  GtkMenu *edit_search_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_search_menu), edit_search_findobject);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_search_menu), edit_search_findregex);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_search_menu), edit_search_findnext);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_search),
			    GTK_WIDGET(edit_search_menu));

  GtkWidget *edit_collapse = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Co_llapse"));
  g_signal_connect( edit_collapse, "activate", 
		    G_CALLBACK(XttGtk::activate_collapse), this);

  GtkMenu *edit_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_search);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_collapse);

  GtkWidget *edit = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Edit"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), GTK_WIDGET(edit_menu));

  // Functions menu
  GtkWidget *functions_openobject = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Open Object..."));
  g_signal_connect( functions_openobject, "activate", 
		    G_CALLBACK(XttGtk::activate_openobject), this);
  gtk_widget_add_accelerator( functions_openobject, "activate", accel_g,
			      'a', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_openplc = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Open _Program"));
  g_signal_connect( functions_openplc, "activate", 
		    G_CALLBACK(XttGtk::activate_openplc), this);
  gtk_widget_add_accelerator(  functions_openplc, "activate", accel_g,
			      'l', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_opengraph = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Open _ClassGraph"));
  g_signal_connect( functions_opengraph, "activate", 
		    G_CALLBACK(XttGtk::activate_opengraph), this);
  gtk_widget_add_accelerator( functions_opengraph, "activate", accel_g,
			      'g', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_showcrossref = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Show C_rossreferences"));
  g_signal_connect( functions_showcrossref, "activate", 
		    G_CALLBACK(XttGtk::activate_showcrossref), this);
  gtk_widget_add_accelerator( functions_showcrossref, "activate", accel_g,
			      'r', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_change_value = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Change _Value"));
  g_signal_connect( functions_change_value, "activate", 
		    G_CALLBACK(XttGtk::activate_change_value), this);
  gtk_widget_add_accelerator(  functions_change_value, "activate", accel_g,
			      'q', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_command = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Co_mmand"));
  g_signal_connect( functions_command, "activate", 
		    G_CALLBACK(XttGtk::activate_command), this);
  gtk_widget_add_accelerator( functions_command, "activate", accel_g,
			      'b', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  // Submenu Search
  GtkWidget *functions_collect_insert = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Insert"));
  g_signal_connect( functions_collect_insert, "activate", 
		    G_CALLBACK(XttGtk::activate_collect_insert), this);
  gtk_widget_add_accelerator( functions_collect_insert, "activate", accel_g,
			      'v', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_collect_show = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Show"));
  g_signal_connect( functions_collect_show, "activate", 
		    G_CALLBACK(XttGtk::activate_collect_show), this);
  gtk_widget_add_accelerator( functions_collect_show, "activate", accel_g,
			      'n', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_collect_remove = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Remove"));
  g_signal_connect( functions_collect_remove, "activate", 
		    G_CALLBACK(XttGtk::activate_collect_remove), this);

  GtkWidget *functions_collect_clear = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("C_lear"));
  g_signal_connect( functions_collect_clear, "activate", 
		    G_CALLBACK(XttGtk::activate_collect_clear), this);


  GtkWidget *functions_collect = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Collect"));
  GtkMenu *functions_collect_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_collect_menu), functions_collect_insert);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_collect_menu), functions_collect_show);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_collect_menu), functions_collect_remove);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_collect_menu), functions_collect_clear);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_collect),
			    GTK_WIDGET(functions_collect_menu));
  // End submenu

  GtkWidget *functions_advuser = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Advanced user"));
  g_signal_connect( functions_advuser, "activate", 
		    G_CALLBACK(XttGtk::activate_advanceduser), this);
  gtk_widget_add_accelerator( functions_advuser, "activate", accel_g,
			      'u', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_openobject);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_openplc);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_opengraph);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_showcrossref);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_change_value);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_command);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_collect);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_advuser);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Functions"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));

  // View menu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _In"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_in), 
				 gtk_image_new_from_stock( "gtk-zoom-in", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(XttGtk::activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Out"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_out), 
				 gtk_image_new_from_stock( "gtk-zoom-out", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(XttGtk::activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Reset"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_reset), 
				 gtk_image_new_from_stock( "gtk-zoom-100", GTK_ICON_SIZE_MENU));
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(XttGtk::activate_zoom_reset), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_View"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));


  // Menu Help
  GtkWidget *help_overview = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Overview"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_overview), 
				 gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
  g_signal_connect(help_overview, "activate", G_CALLBACK(XttGtk::activate_help), this);
  gtk_widget_add_accelerator( help_overview, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *help_project = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Project"));
  g_signal_connect( help_project, "activate", 
		    G_CALLBACK(XttGtk::activate_help_project), this);

  GtkWidget *help_proview = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_About Proview"));
  g_signal_connect( help_proview, "activate", 
		    G_CALLBACK(XttGtk::activate_help_proview), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_overview);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_project);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_proview);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Help"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  GtkWidget *tools_back = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_back), 
	  gtk_image_new_from_stock( "gtk-go-back", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_back, "clicked", G_CALLBACK(activate_back), this);
  g_object_set( tools_back, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_back,CoWowGtk::translate_utf8("Go back"), "");

  GtkWidget *tools_advuser = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_advuser.png");
  gtk_container_add( GTK_CONTAINER(tools_advuser), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_advuser, "clicked", G_CALLBACK(XttGtk::activate_advanceduser), this);
  g_object_set( tools_advuser, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_advuser,CoWowGtk::translate_utf8("Advanced user"), "");
  
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

  // Toolbar
  methodtoolbar = new XttMethodToolbarGtk(0);
  GtkToolbar *tools2 = (GtkToolbar *) ((XttMethodToolbarGtk *)methodtoolbar)->build();

  // Statusbar and cmd input
  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);
  cmd_prompt = gtk_label_new( "value > ");
  gtk_widget_set_size_request( cmd_prompt, -1, 25);

  cmd_recall = new CoWowRecall();
  value_recall = new CoWowRecall();
  cmd_entry = new CoWowEntryGtk( cmd_recall);
  cmd_input = cmd_entry->widget();
  gtk_widget_set_size_request( cmd_input, -1, 25);
  g_signal_connect( cmd_input, "activate", 
  		    G_CALLBACK(XttGtk::valchanged_cmd_input), this);

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_prompt, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_input, TRUE, TRUE, 20);
  gtk_widget_show_all( statusbar);

  xnav = new XNavGtk( this, xnav_form, "Plant",
		&brow_widget, (xnav_sStartMenu *)root_menu, 
		opplace_str, op_close_button, &sts);
  xnav->message_cb = &xtt_message_cb;
  xnav->close_cb = &close;
  xnav->map_cb = &map;
  xnav->change_value_cb = &change_value;
  xnav->set_dimension_cb = &set_dimension;
  xnav->selection_changed_cb = &selection_changed;
  xnav->attach_audio = attach_audio;

  methodtoolbar->m_xnav = xnav;

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools2), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(brow_widget), TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(statusbar), FALSE, FALSE, 3);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  gtk_widget_show_all( toplevel);

  g_object_set( cmd_prompt, "visible", FALSE, NULL);
  g_object_set( cmd_input, "visible", FALSE, NULL);

  // Create help window
  CoXHelp *xhelp = new CoXHelpGtk( toplevel, this, xhelp_eUtility_Xtt, &sts);
  xhelp->open_URL_cb = open_URL_cb;
  CoXHelp::set_default( xhelp);

  wow = new CoWowGtk( toplevel);
  if ( !quiet)
    wow->DisplayWarranty();

  // TODO if ( xnav->op)
  //  close( this, 0);

  timerid = wow->timer_new();

  // Start timer to check for qcom events
  timerid->add( 1000, qcom_events, this);

  hotkey = new XttHotkey("$HOME/Rt_xtt");

  hotkey->register_action( "SetDig", hotkey_SetDig, this);
  hotkey->register_action( "ResetDig", hotkey_ResetDig, this);
  hotkey->register_action( "ToggleDig", hotkey_ToggleDig, this);
  hotkey->register_action( "Command", hotkey_Command, this);

  int n_screens = gdk_display_get_n_screens( gdk_display_get_default());
  for ( int i = 0; i < n_screens; i++) {
    GdkWindow *root = gdk_screen_get_root_window( gdk_display_get_screen( 
                 gdk_display_get_default(), i));
    gdk_window_add_filter( root, xtt_hotkey_filter, hotkey);
  }
  methodtoolbar->set_sensitive();

  if ( select_opplace)
    list_opplace();

  xtt_mainloop();  

}

static gint india_delete_event( GtkWidget *w, GdkEvent *event, gpointer xtt)
{
  g_object_set( ((XttGtk *)xtt)->india_widget, "visible", FALSE, NULL);
  return TRUE;
}

void XttGtk::create_input_dialog()
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
  CoWowGtk::SetWindowIcon( india_widget);

  india_text = gtk_entry_new();
  g_signal_connect( india_text, "activate", 
  		    G_CALLBACK(activate_india_ok), this);
  india_label = gtk_label_new("");
  GtkWidget *india_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *india_ok = gtk_button_new_with_label( CoWowGtk::translate_utf8("Ok"));
  gtk_widget_set_size_request( india_ok, 70, 25);
  g_signal_connect( india_ok, "clicked", 
  		    G_CALLBACK(activate_india_ok), this);
  GtkWidget *india_cancel = gtk_button_new_with_label( CoWowGtk::translate_utf8("Cancel"));
  gtk_widget_set_size_request( india_cancel, 70, 25);
  g_signal_connect( india_cancel, "clicked", 
  		    G_CALLBACK(activate_india_cancel), this);

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
}






