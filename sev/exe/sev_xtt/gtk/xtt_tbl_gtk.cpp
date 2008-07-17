/* 
 * Proview   $Id: xtt_tbl_gtk.cpp,v 1.1 2008-07-17 11:18:31 claes Exp $
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

#include "glow_std.h"

#include "co_cdh.h"
#include "co_time.h"
#include "co_wow_gtk.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "glow.h"
#include "glow_curvectx.h"

#include "flow_msg.h"

#include "xtt_tbl_gtk.h"
#include "xtt_tblnav_gtk.h"
#include "xtt_dshist_gtk.h"

void XttTblGtk::message( char severity, char *message)
{
  gtk_label_set_text( GTK_LABEL(msg_label), message);
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  XttTblGtk *xtttbl = (XttTblGtk *)data;

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

static void xtttbl_activate_close( GtkWidget *w, gpointer data)
{
  XttTbl *xtttbl = (XttTbl *)data;

  delete xtttbl;
}

static void xtttbl_activate_help( GtkWidget *w, gpointer data)
{
}

static void xtttbl_activate_opendshist( GtkWidget *w, gpointer data)
{
  ((XttTbl *)data)->activate_opendshist();
}

XttDsHist *XttTblGtk::dshist_new( pwr_tOid oid, char *aname)
{
  GtkWidget *w;
  pwr_tStatus sts;
  pwr_tOid oidv[2];
  pwr_tOName anamev[2];

  oidv[0] = oid;
  oidv[1] = pwr_cNOid;
  strncpy( anamev[0], aname, sizeof(anamev[0]));

  return new XttDsHistGtk( (void *)this, toplevel, "DsHist", &w, oidv, anamev, 
			   sevcli, &sts);
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

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", 700,
			   "default-width", 500,
			   "title", "Object Attributes",
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
  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(xtttbl_activate_close), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));


  // Functions entry
  GtkWidget *functions_opendshist = gtk_menu_item_new_with_mnemonic( "_Open DsHist");
  g_signal_connect(functions_opendshist, "activate", G_CALLBACK(xtttbl_activate_opendshist), this);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_opendshist);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("_Functions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));


  // Help entry
  GtkWidget *help_help = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_g);
  g_signal_connect(help_help, "activate", G_CALLBACK(xtttbl_activate_help), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  pane = gtk_vpaned_new();

  tblnav = new TblNavGtk( this, pane,
		itemlist, item_cnt, &brow_widget, &sts);
  tblnav->message_cb = &XttTbl::message;

  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  // msg_label = gtk_statusbar_new();
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 0);

  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(vbox), GTK_WIDGET(pane), TRUE, TRUE, 0);

  gtk_paned_pack1( GTK_PANED(pane), GTK_WIDGET(brow_widget), TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(pane), GTK_WIDGET(statusbar), FALSE, TRUE);
  
  gtk_container_add( GTK_CONTAINER(toplevel), vbox);


  gtk_widget_show_all( toplevel);

  int w, h;
  gdk_drawable_get_size( pane->window, &w, &h);
  gtk_paned_set_position( GTK_PANED(pane), h - 50);
  
}

XttTblGtk::~XttTblGtk()
{
  delete (TblNavGtk *)tblnav;

  gtk_widget_destroy( toplevel);
}
