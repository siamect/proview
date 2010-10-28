/* 
 * Proview   $Id: xtt_tbl_gtk.cpp,v 1.4 2008-10-31 12:51:30 claes Exp $
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

/* xtt_tbl.cpp -- Sev Table Viewer */

#include <fstream>
#include <vector>

#include "glow_std.h"

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "cow_wow_gtk.h"
#include "cow_xhelp_gtk.h"
#include "cow_login_gtk.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "glow.h"
#include "glow_curvectx.h"

#include "flow_msg.h"

#include "xtt_tbl_gtk.h"
#include "xtt_tblnav_gtk.h"
#include "xtt_sevhist_gtk.h"

void XttTblGtk::message( char severity, char *message)
{
  gtk_label_set_text( GTK_LABEL(msg_label), message);
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  XttTblGtk *xtttbl = (XttTblGtk *)data;

  if  ( xtttbl->close_cb)
    (xtttbl->close_cb)( xtttbl);
  else
    delete xtttbl;
  
  return FALSE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
  // XttTblGtk *xtttbl = (XttTblGtk *)data;
  // delete xtttbl;
}

static gboolean xtttbl_focus_in_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  XttTblGtk *xtttbl = (XttTblGtk *)data;

  if ( xtttbl->tblnav)
    xtttbl->tblnav->set_inputfocus();

  return FALSE;
}

void XttTblGtk::activate_print( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized())
    return;

  xtt->activate_print();
}

void XttTblGtk::activate_login( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  xtt->open_login();
}

void XttTblGtk::activate_logout( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  xtt->logout();
}

void XttTblGtk::activate_close( GtkWidget *w, gpointer data)
{
  XttTbl *xtttbl = (XttTbl *)data;
  if  ( xtttbl->close_cb)
    (xtttbl->close_cb)( xtttbl);
  else
    delete xtttbl;
}

void XttTblGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  xtt->activate_zoom_in();
}

void XttTblGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  xtt->activate_zoom_out();
}

void XttTblGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  xtt->activate_zoom_reset();
}

void XttTblGtk::activate_help( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized())
    return;

  xtt->activate_help();
}

void XttTblGtk::activate_help_project( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized())
    return;

  xtt->activate_help_project();
}

void XttTblGtk::activate_help_proview( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized())
    return;

  xtt->activate_help_proview();
}

void XttTblGtk::activate_opensevhist( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized())
    return;

  xtt->activate_opensevhist();
}

void XttTblGtk::activate_delete_item( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized( pwr_mPrv_SevAdmin))
    return;

  xtt->activate_delete_item();
}

void XttTblGtk::activate_tree_layout( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized())
    return;

  xtt->tblnav->show_tree();
}

void XttTblGtk::activate_list_layout( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized())
    return;

  xtt->tblnav->show_list();
}

XttSevHist *XttTblGtk::sevhist_new( pwr_tOid *oidv, pwr_tOName *anamev, pwr_tOName *onamev, bool *sevhistobjectv, pwr_tStatus *sts)
{
  GtkWidget *w;

  return new XttSevHistGtk( (void *)this, toplevel, "Process History", &w, oidv, anamev, onamev,
			    sevhistobjectv, sevcli, sts);
}

CoLogin *XttTblGtk::login_new( const char      	*name,
			       const char      	*groupname,
			       void		(* bc_success)( void *),
			       void		(* bc_cancel)( void *),
			       pwr_tStatus  	*status)
{
  return new CoLoginGtk( this, toplevel, name, groupname, bc_success, bc_cancel, status);
}

void XttTblGtk::activate_command( GtkWidget *w, gpointer data)
{
  XttTbl *xtt = (XttTbl *)data;

  if ( !xtt->is_authorized())
    return;

  if ( xtt->command_open) {
    g_object_set( ((XttTblGtk *)xtt)->cmd_input, "visible", FALSE, NULL);
    xtt->set_prompt( "");
    xtt->command_open = 0;
    return;
  }

  gtk_editable_delete_text( GTK_EDITABLE(((XttTblGtk *)xtt)->cmd_input), 0, -1);

  g_object_set( ((XttTblGtk *)xtt)->cmd_input, "visible", TRUE, NULL);
  xtt->message( ' ', "");
  gtk_widget_grab_focus( ((XttTblGtk *)xtt)->cmd_input);

  xtt->set_prompt( "xtt >");
  xtt->command_open = 1;
}

void XttTblGtk::set_prompt( const char *prompt)
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

void XttTblGtk::valchanged_cmd_input( GtkWidget *w, gpointer data)
{
  XttTbl 	*xtt = (XttTbl *)data;
  int 	sts;
  char 	*text, *textutf8;

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(w), 0, -1);
  text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  sts = xtt->command( text);
  g_object_set( w, "visible", FALSE, NULL);
  xtt->set_prompt( "");
  xtt->command_open = 0;
  xtt->tblnav->set_inputfocus();
}

XttTblGtk::XttTblGtk( GtkWidget *a_parent_wid,
		      void	*a_parent_ctx,
		      sevcli_tCtx  a_sevcli,
		      sevcli_sHistItem *itemlist,
		      int    	item_cnt) :
  XttTbl( a_parent_ctx, a_sevcli), 
  parent_wid(a_parent_wid)
{
  int sts;
  pwr_tFileName fname;

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 700,
			   "default-width", 500,
			   "title", "SevXtt Navigator",
			   NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(xtttbl_focus_in_event), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);

  // Menu
  // Accelerators
  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File entry
  GtkWidget *file_print = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Print"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_print), 
				 gtk_image_new_from_stock( "gtk-print", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_print, "activate", G_CALLBACK(activate_print), this);

  GtkWidget *file_login = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Login"));
  g_signal_connect( file_login, "activate", 
		    G_CALLBACK(activate_login), this);

  GtkWidget *file_logout = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Log_Out"));
  g_signal_connect( file_logout, "activate", 
		    G_CALLBACK(activate_logout), this);

  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(activate_close), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_login);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_logout);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));


  // Functions entry
  GtkWidget *functions_opensevhist = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Open Process History"));
  g_signal_connect(functions_opensevhist, "activate", G_CALLBACK(activate_opensevhist), this);
  gtk_widget_add_accelerator( functions_opensevhist, "activate", accel_g,
			      'g', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_command = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Co_mmand"));
  g_signal_connect( functions_command, "activate", 
		    G_CALLBACK(activate_command), this);
  gtk_widget_add_accelerator( functions_command, "activate", accel_g,
			      'b', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

GtkWidget *functions_delete_item = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Delete Item"));
  g_signal_connect(functions_delete_item, "activate", G_CALLBACK(activate_delete_item), this);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_opensevhist);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_command);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_delete_item);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Functions"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));


  // View menu
  GtkWidget *view_tree_layout = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Tree Layout"));
  g_signal_connect(view_tree_layout, "activate", G_CALLBACK(activate_tree_layout), this);

  GtkWidget *view_list_layout = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_List Layout"));
  g_signal_connect(view_list_layout, "activate", G_CALLBACK(activate_list_layout), this);

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
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_tree_layout);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_list_layout);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_View"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Help entry
  GtkWidget *help_overview = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Overview"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_overview), 
				 gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
  g_signal_connect(help_overview, "activate", G_CALLBACK(activate_help), this);
  gtk_widget_add_accelerator( help_overview, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *help_project = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Project"));
  g_signal_connect( help_project, "activate", 
		    G_CALLBACK(activate_help_project), this);

  GtkWidget *help_proview = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_About Proview"));
  g_signal_connect( help_proview, "activate", 
		    G_CALLBACK(activate_help_proview), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_overview);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_project);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_proview);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Help"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  GtkWidget *tools_opensevhist = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_meth_trend.png");
  gtk_container_add( GTK_CONTAINER(tools_opensevhist), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_opensevhist, "clicked", G_CALLBACK(activate_opensevhist), this);
  g_object_set( tools_opensevhist, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_opensevhist,CoWowGtk::translate_utf8("Open history item"), "");

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

  // Statusbar and cmd input
  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);
  cmd_prompt = gtk_label_new( "xtt > ");
  gtk_widget_set_size_request( cmd_prompt, -1, 25);

  cmd_recall = new CoWowRecall();
  cmd_entry = new CoWowEntryGtk( cmd_recall);
  cmd_input = cmd_entry->widget();
  gtk_widget_set_size_request( cmd_input, -1, 25);
  g_signal_connect( cmd_input, "activate", 
  		    G_CALLBACK(valchanged_cmd_input), this);

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_prompt, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_input, TRUE, TRUE, 20);
  gtk_widget_show_all( statusbar);

  tblnav = new TblNavGtk( this, toplevel,
		itemlist, item_cnt, &brow_widget, &sts);
  tblnav->message_cb = &XttTbl::message;
  tblnav->is_authorized_cb = &XttTbl::is_authorized;
  tblnav->command_cb = &XttTbl::command_cb;

  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(brow_widget), TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(statusbar), FALSE, FALSE, 3);
  
  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  gtk_widget_show_all( toplevel);

  g_object_set( cmd_prompt, "visible", FALSE, NULL);
  g_object_set( cmd_input, "visible", FALSE, NULL);

  // Create help window
  CoXHelp *xhelp = new CoXHelpGtk( toplevel, this, xhelp_eUtility_Xtt, &sts);
  CoXHelp::set_default( xhelp);

  wow = new CoWowGtk( toplevel);

  if ( !is_authorized( pwr_mAccess_AllSev, 0))
    open_login();

  if ( !quiet) {
    if ( cologin)
      // Set login window as parent to warranty as focus is left to parent.
      ((CoWowGtk *)wow)->SetParent( ((CoLoginGtk *)cologin)->widgets.toplevel);

    wow->DisplayWarranty();

    if ( cologin)
      ((CoWowGtk *)wow)->SetParent( toplevel);
  }


}

XttTblGtk::~XttTblGtk()
{
  delete (TblNavGtk *)tblnav;

  gtk_widget_destroy( toplevel);
}
