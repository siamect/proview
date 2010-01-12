/* 
 * Proview   $Id: rt_trace_gtk.cpp,v 1.5 2007-12-03 14:51:39 claes Exp $
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

/* rt_trace_gtk.cpp -- trace in runtime environment */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#if defined OS_LINUX
#include <sys/stat.h>
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_privilege.h"

#include "flow_ctx.h"
#include "flow_api.h"
#include "co_cdh.h"
#include "co_api.h"
#include "co_dcli.h"
#include "rt_gdh.h"
#include "flow_widget_gtk.h"
#include "xtt_trace_gtk.h"
#include "cow_wow_gtk.h"

void RtTraceGtk::activate_close(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTraceGtk *)data;

  tractx->activate_close();
}

void RtTraceGtk::activate_print(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_print();
}

void RtTraceGtk::activate_printselect(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_printselect();
}

void RtTraceGtk::activate_savetrace(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_savetrace();
}

void RtTraceGtk::activate_restoretrace(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_restoretrace();
}

void RtTraceGtk::activate_cleartrace(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_cleartrace();
}

void RtTraceGtk::activate_display_object(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_display_object();
}

void RtTraceGtk::activate_collect_insert(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_collect_insert();
}

void RtTraceGtk::activate_open_object(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_open_object();
}

void RtTraceGtk::activate_open_subwindow(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_open_subwindow();
}

void RtTraceGtk::activate_show_cross(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_show_cross();
}

void RtTraceGtk::activate_open_classgraph(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_open_classgraph();
}

void RtTraceGtk::activate_trace(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_trace();
}

void RtTraceGtk::activate_simulate(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_simulate();
}

void RtTraceGtk::activate_view(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_view();
}

void RtTraceGtk::activate_zoomin(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  flow_Zoom( tractx->flow_ctx, 1.0/0.7);
}

void RtTraceGtk::activate_zoomout(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  flow_Zoom( tractx->flow_ctx, 0.7);
}

void RtTraceGtk::activate_zoomreset(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  flow_UnZoom( tractx->flow_ctx);
}

void RtTraceGtk::activate_scantime1(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->scan_time = 0.5;
}

void RtTraceGtk::activate_scantime2(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->scan_time = 0.2;
}

void RtTraceGtk::activate_scantime3(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->scan_time = 0.1;
}

void RtTraceGtk::activate_scantime4(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->scan_time = 0.05;
}

void RtTraceGtk::activate_scantime5(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->scan_time = 0.02;
}

void RtTraceGtk::activate_help(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_help();
}

void RtTraceGtk::activate_helpplc(GtkWidget *w, gpointer data)
{
  RtTrace *tractx = (RtTrace *)data;

  tractx->activate_helpplc();
}

RtTraceGtk::~RtTraceGtk()
{
  trace_tNode *node, *fnode;

  trace_stop();

  /* Delete all trace children windows */
  for (node = trace_list; node;) {
    delete node->tractx;
    fnode = node->Next;
    free((char *)node);
    node = fnode;
  }
  if ( nav_shell)
    gtk_widget_destroy( nav_shell);

  /* Destroy the widgets */
  if ( toplevel)
    gtk_widget_destroy( toplevel);
}

void RtTraceGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

void RtTraceGtk::popup_menu_position( int event_x, int event_y, int *x, int *y) 
{
  CoWowGtk::PopupPosition( flow_widget, event_x, event_y, x, y);
}

RtTrace *RtTraceGtk::subwindow_new( void *ctx, pwr_tObjid oid, pwr_tStatus *sts)
{
  return new RtTraceGtk( ctx, flow_widget, oid, sts);
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer tra)
{
  RtTraceGtk::activate_close(w, tra);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

#if 0
static gint nav_delete_event( GtkWidget *w, GdkEvent *event, gpointer tra)
{
  gtk_widget_destroy( ((RtTraceGtk *)tra)->nav_shell);
  ((RtTraceGtk *)tra)->nav_shell = 0;
  return TRUE;
}
#endif

RtTraceGtk::RtTraceGtk( void *tr_parent_ctx, GtkWidget *tr_parent_wid, pwr_tObjid tr_objid,
			pwr_tStatus *status) :
  RtTrace( tr_parent_ctx, tr_objid, status), parent_wid(tr_parent_wid), toplevel(0), nav_shell(0)
{

  const int	window_width = 900;
  const int    	window_height = 800;
  const int    	nav_width = 180;
  pwr_tStatus 	sts;
  pwr_tObjid	window_objid;
  pwr_tClassId	cid;
  char   	title[220];
  pwr_tOName   	hostname;
  pwr_tOName   	plcconnect;
  pwr_tFileName fname;

  sts = gdh_ObjidToName( tr_objid, name, sizeof(name), cdh_mNName); 
  if (EVEN(sts)) {
    *status = sts;
    return;
  }
  strcpy( title, "Trace ");
  strcat( title, name);

  /* Find plcwindow */
  sts = gdh_GetObjectClass( tr_objid, &cid);
  if ( EVEN(sts)) {
    *status = sts;
    return;
  }

  if ( !(cid == pwr_cClass_windowplc ||
         cid == pwr_cClass_windowcond ||
         cid == pwr_cClass_windoworderact ||
         cid == pwr_cClass_windowsubstep )) {
    sts = gdh_GetChild( tr_objid, &window_objid);
    if ( EVEN(sts)) {
      *status = sts;
      return;
    }
  }
  else
    window_objid = tr_objid; 

  sts = gdh_GetObjectClass( window_objid, &cid);
  if ( EVEN(sts)) {
    *status = sts;
    return;
  }

  if ( !(cid == pwr_cClass_windowplc ||
         cid == pwr_cClass_windowcond ||
         cid == pwr_cClass_windoworderact ||
         cid == pwr_cClass_windowsubstep )) {
    *status = 0;
    return;
  }

  sts = get_filename( window_objid, filename, &m_has_host, hostname, 
		      plcconnect);
  if ( EVEN(sts)) {
    *status = sts;
    return;
  }

  /* Create object context */
  objid = window_objid;
  if ( m_has_host) {
    strcpy( m_hostname, hostname);
    strcpy( m_plcconnect, plcconnect);
  }

  char *titleutf8 = g_convert( title, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);

  // Gtk
  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", titleutf8,
					 NULL);
  g_free( titleutf8);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  // Submenu Print
  GtkWidget *file_print = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Print"));
  g_signal_connect( file_print, "activate", 
		    G_CALLBACK(activate_print), this);

  GtkWidget *file_print_select = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("Print _Selected Documents"));
  g_signal_connect( file_print_select, "activate", 
		    G_CALLBACK(activate_printselect), this);

  GtkWidget *file_savetrace = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("Save _Trace"));
  g_signal_connect( file_savetrace, "activate", 
		    G_CALLBACK(activate_savetrace), this);

  GtkWidget *file_restoretrace = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Restore Trace"));
  g_signal_connect( file_restoretrace, "activate", 
		    G_CALLBACK(activate_restoretrace), this);

  GtkWidget *file_cleartrace = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("C_lear Trace"));
  g_signal_connect( file_cleartrace, "activate", 
		    G_CALLBACK(activate_cleartrace), this);

  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(activate_close), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print_select);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_savetrace);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_restoretrace);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_cleartrace);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_File"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Functions Entry
  GtkWidget *functions_open_object = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Open Object"));
  g_signal_connect( functions_open_object, "activate", 
		    G_CALLBACK(activate_open_object), this);
  gtk_widget_add_accelerator( functions_open_object, "activate", accel_g,
  			      'a', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_open_subwindow = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("Open S_ubwindow"));
  g_signal_connect( functions_open_subwindow, "activate", 
		    G_CALLBACK(activate_open_subwindow), this);
  gtk_widget_add_accelerator( functions_open_subwindow, "activate", accel_g,
  			      'l', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_display_object = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Display object in Navigator"));
  g_signal_connect( functions_display_object, "activate", 
		    G_CALLBACK(activate_display_object), this);
  gtk_widget_add_accelerator( functions_display_object, "activate", accel_g,
  			      'd', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_show_cross = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Show Crossreferences"));
  g_signal_connect( functions_show_cross, "activate", 
		    G_CALLBACK(activate_show_cross), this);
  gtk_widget_add_accelerator( functions_show_cross, "activate", accel_g,
  			      'r', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_open_classgraph = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Open ObjectGraph"));
  g_signal_connect( functions_open_classgraph, "activate", 
		    G_CALLBACK(activate_open_classgraph), this);
  gtk_widget_add_accelerator( functions_open_classgraph, "activate", accel_g,
  			      'g', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_collect_insert = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Collect Insert"));
  g_signal_connect( functions_collect_insert, "activate", 
		    G_CALLBACK(activate_collect_insert), this);
  gtk_widget_add_accelerator( functions_collect_insert, "activate", accel_g,
  			      'v', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_open_object);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_open_subwindow);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_display_object);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_show_cross);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_open_classgraph);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_collect_insert);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Functions"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));


  // View Entry

  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect( view_zoom_in, "activate", 
		    G_CALLBACK(activate_zoomin), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
  			      'i', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect( view_zoom_out, "activate", 
		    G_CALLBACK(activate_zoomout), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
  			      'o', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect( view_zoom_reset, "activate", 
		    G_CALLBACK(activate_zoomreset), this);
  gtk_widget_add_accelerator( view_zoom_reset, "activate", accel_g,
  			      'b', GdkModifierType(GDK_CONTROL_MASK), 
  			      GTK_ACCEL_VISIBLE);

  // Submenu ScanTime
  GSList *view_sc_group = NULL;
  GtkWidget *view_scantime1 = gtk_radio_menu_item_new_with_mnemonic( view_sc_group, 
									"0.50 s");
  view_sc_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_scantime1));
  g_signal_connect( view_scantime1, "activate", 
		    G_CALLBACK(activate_scantime1), this);

  GtkWidget *view_scantime2 = gtk_radio_menu_item_new_with_mnemonic( view_sc_group, 
									"0.20 s");
  view_sc_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_scantime1));
  g_signal_connect( view_scantime2, "activate", 
		    G_CALLBACK(activate_scantime2), this);

  GtkWidget *view_scantime3 = gtk_radio_menu_item_new_with_mnemonic( view_sc_group, 
									"0.10 s");
  view_sc_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_scantime1));
  g_signal_connect( view_scantime3, "activate", 
		    G_CALLBACK(activate_scantime3), this);

  GtkWidget *view_scantime4 = gtk_radio_menu_item_new_with_mnemonic( view_sc_group, 
									"0.05 s");
  view_sc_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(view_scantime1));
  g_signal_connect( view_scantime4, "activate", 
		    G_CALLBACK(activate_scantime4), this);

  GtkWidget *view_scantime5 = gtk_radio_menu_item_new_with_mnemonic( view_sc_group, 
									"0.02 s");
  g_signal_connect( view_scantime5, "activate", 
		    G_CALLBACK(activate_scantime5), this);

  GtkWidget *view_sc = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_ScanTime"));
  GtkMenu *view_sc_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_sc_menu), view_scantime1);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_sc_menu), view_scantime2);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_sc_menu), view_scantime3);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_sc_menu), view_scantime4);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_sc_menu), view_scantime5);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_sc),
			    GTK_WIDGET(view_sc_menu));

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_sc);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_View"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Mode entry
  GSList *mode_group = NULL;
  GtkWidget *mode_view = gtk_radio_menu_item_new_with_mnemonic( mode_group, CoWowGtk::translate_utf8("V_iew"));
  mode_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(mode_view));
  g_signal_connect( mode_view, "activate", 
		    G_CALLBACK(activate_view), this);

  GtkWidget *mode_trace = gtk_radio_menu_item_new_with_mnemonic( mode_group, CoWowGtk::translate_utf8("_Trace"));
  mode_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(mode_trace));
  g_signal_connect( mode_trace, "activate", 
		    G_CALLBACK(activate_trace), this);

  GtkWidget *mode_simulate = gtk_radio_menu_item_new_with_mnemonic( mode_group, CoWowGtk::translate_utf8("_Simulate"));
  gtk_widget_add_accelerator( mode_simulate, "activate", accel_g,
  			      's', GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
  			      GTK_ACCEL_VISIBLE);
  g_signal_connect( mode_simulate, "activate", 
		    G_CALLBACK(activate_simulate), this);

  GtkMenu *mode_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), mode_view);
  gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), mode_trace);
  gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), mode_simulate);

  GtkWidget *mode = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Mode"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), mode);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(mode), GTK_WIDGET(mode_menu));

  // Menu Help
  GtkWidget *help_help = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("On _Trace"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_help), 
				 gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
  g_signal_connect(help_help, "activate", G_CALLBACK(activate_help), this);
  gtk_widget_add_accelerator( help_help, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *help_plcpgm = gtk_check_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("On _PlcPgm"));
  g_signal_connect( help_plcpgm, "activate", 
		    G_CALLBACK(activate_helpplc), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_plcpgm);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Help"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  GtkWidget *tools_display_object = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_navigator.png");
  gtk_container_add( GTK_CONTAINER(tools_display_object), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_display_object, "clicked", G_CALLBACK(activate_display_object), this);
  gtk_toolbar_append_widget( tools, tools_display_object, "Display object in Navigator", "");
  
  GtkWidget *tools_show_cross = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_crossref.png");
  gtk_container_add( GTK_CONTAINER(tools_show_cross), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_show_cross, "clicked", G_CALLBACK(activate_show_cross), this);
  gtk_toolbar_append_widget( tools, tools_show_cross, "Show Crossreferences", "");
  
  GtkWidget *tools_zoom_in = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(activate_zoomin), this);
  gtk_toolbar_append_widget( tools, tools_zoom_in, "Zoom in", "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(activate_zoomout), this);
  gtk_toolbar_append_widget( tools, tools_zoom_out, "Zoom out", "");

  GtkWidget *tools_zoom_reset = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_reset.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_reset), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_reset, "clicked", G_CALLBACK(activate_zoomreset), this);
  gtk_toolbar_append_widget( tools, tools_zoom_reset, "Zoom reset", "");

  // Flow widget
  GtkWidget *flow_scrolled = scrolledflowwidgetgtk_new( init_flow, 
							this, &flow_widget);
  gtk_widget_show_all(flow_widget);

  nav_widget = flownavwidgetgtk_new( flow_widget);

  GtkWidget *paned = gtk_hpaned_new();
  gtk_paned_pack1( GTK_PANED(paned), flow_scrolled, TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(paned), nav_widget, FALSE, TRUE);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(paned), TRUE, TRUE, 0);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);
  gtk_widget_show_all( toplevel);

  gtk_paned_set_position( GTK_PANED(paned), window_width - nav_width);

#if 0
  // Navigator window
  nav_shell = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					  "default-height", 200,
					  "default-width", 200,
					  "title", "Navigator",
					  NULL);
  g_signal_connect( nav_shell, "delete_event", G_CALLBACK(nav_delete_event), this);

  nav_widget = flownavwidgetgtk_new( flow_widget);
  gtk_container_add( GTK_CONTAINER(nav_shell), nav_widget);
  gtk_widget_show_all( nav_shell);
#endif

  wow = new CoWowGtk( toplevel);
  trace_timerid = wow->timer_new();

  viewsetup();
  flow_Open( flow_ctx, filename);

  gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM( mode_trace), TRUE);
  gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM( view_scantime1), TRUE);

  //trasetup();
  //trace_start();

#if defined OS_LINUX
  {
    struct stat info;

    if ( stat( filename, &info) != -1)
      version = info.st_ctime;    
  }
#endif
#if 0
  FlowWidget	fwidget;
  char		uid_filename[120] = {"xtt_trace.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  unsigned long sts;
  pwr_tOName   	name;
  int		i;
  pwr_tObjid	window_objid;
  pwr_tClassId	cid;
  char   	title[220];
  pwr_tOName   	hostname;
  pwr_tOName   	plcconnect;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	trace_widget;
  static MrmRegisterArg	reglist[] = {
        { "tra_ctx", 0 },
	{"tra_activate_close",(caddr_t)activate_close },
	{"tra_activate_print",(caddr_t)activate_print },
	{"tra_activate_printselect",(caddr_t)activate_printselect },
	{"tra_activate_savetrace",(caddr_t)activate_savetrace },
	{"tra_activate_restoretrace",(caddr_t)activate_restoretrace },
	{"tra_activate_cleartrace",(caddr_t)activate_cleartrace },
	{"tra_activate_trace",(caddr_t)activate_trace },
	{"tra_activate_display_object",(caddr_t)activate_display_object },
	{"tra_activate_open_object",(caddr_t)activate_open_object },
	{"tra_activate_show_cross",(caddr_t)activate_show_cross },
	{"tra_activate_open_classgraph",(caddr_t)activate_open_classgraph },
	{"tra_activate_collect_insert",(caddr_t)activate_collect_insert },
	{"tra_activate_view",(caddr_t)activate_view },
	{"tra_activate_simulate",(caddr_t)activate_simulate },
	{"tra_activate_zoomin",(caddr_t)activate_zoomin },
	{"tra_activate_zoomout",(caddr_t)activate_zoomout },
	{"tra_activate_zoomreset",(caddr_t)activate_zoomreset },
	{"tra_activate_scantime1",(caddr_t)activate_scantime1 },
	{"tra_activate_scantime2",(caddr_t)activate_scantime2 },
	{"tra_activate_scantime3",(caddr_t)activate_scantime3 },
	{"tra_activate_scantime4",(caddr_t)activate_scantime4 },
	{"tra_activate_scantime5",(caddr_t)activate_scantime5 },
	{"tra_activate_help",(caddr_t)activate_help },
	{"tra_activate_helpplc",(caddr_t)activate_helpplc },
	{"tra_create_form",(caddr_t)create_form },
	{"tra_create_menu",(caddr_t)create_menu }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  lng_get_uid( uid_filename, uid_filename);

  sts = gdh_ObjidToName( tr_objid, name, sizeof(name), cdh_mNName); 
  if (EVEN(sts)) {
    *status = sts;
    return;
  }
  strcpy( title, "Trace ");
  strcat( title, name);

  /* Find plcwindow */
  sts = gdh_GetObjectClass( tr_objid, &cid);
  if ( EVEN(sts)) {
    *status = sts;
    return;
  }

  if ( !(cid == pwr_cClass_windowplc ||
         cid == pwr_cClass_windowcond ||
         cid == pwr_cClass_windoworderact ||
         cid == pwr_cClass_windowsubstep ))
  {

    sts = gdh_GetChild( tr_objid, &window_objid);
    if ( EVEN(sts)) {
      *status = sts;
      return;
    }
  }
  else
    window_objid = tr_objid; 

  sts = gdh_GetObjectClass( window_objid, &cid);
  if ( EVEN(sts)) {
    *status = sts;
    return;
  }

  if ( !(cid == pwr_cClass_windowplc ||
         cid == pwr_cClass_windowcond ||
         cid == pwr_cClass_windoworderact ||
         cid == pwr_cClass_windowsubstep )) {
    *status = 0;
    return;
  }

  sts = get_filename( window_objid, filename, &m_has_host, hostname, 
		      plcconnect);
  if ( EVEN(sts)) {
    *status = sts;
    return;
  }

  /* Create object context */
  objid = window_objid;
  if ( m_has_host) {
    strcpy( m_hostname, hostname);
    strcpy( m_plcconnect, plcconnect);
  }
  reglist[0].value = (caddr_t) this;
 
  toplevel = XtCreatePopupShell( name, 
		topLevelShellWidgetClass, parent_wid, args, 0);

  /* Save the context structure in the widget */
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, "trace_window", toplevel,
			title, args, 1, &trace_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);


  i = 0;
  XtSetArg(args[i],XmNwidth,800);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( trace_widget);

  i = 0;
/*
  XtSetArg(args[i],XmNwidth,790);i++;
  XtSetArg(args[i],XmNheight,560);i++;
*/
  XtSetArg( args[i], XmNtopAttachment, XmATTACH_WIDGET);i++;
  XtSetArg( args[i], XmNtopWidget, menu);i++;
  XtSetArg( args[i], XmNrightAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNleftAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNbottomAttachment, XmATTACH_FORM);i++;
  flow_widget = FlowCreate( form, "Flow window", args, i, 
			    init_flow, (void *)this);

  XtManageChild( (Widget) flow_widget);
/*
  XtRealizeWidget(toplevel);
*/
  XtPopup( toplevel, XtGrabNone);

  fwidget = (FlowWidget) flow_widget;
  flow_ctx = (flow_tCtx)fwidget->flow.flow_ctx;
  flow_SetCtxUserData( flow_ctx, this);

  /* Create navigator popup */

  i = 0;
  XtSetArg(args[i],XmNallowShellResize, TRUE); i++;
  XtSetArg(args[i],XmNallowResize, TRUE); i++;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNx,500);i++;
  XtSetArg(args[i],XmNy,500);i++;

  nav_shell = XmCreateDialogShell( flow_widget, "Navigator",
        args, i);
  XtManageChild( nav_shell);

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  nav_widget = FlowCreateNav( nav_shell, "navigator",
        args, i, flow_widget);
  XtManageChild( nav_widget);
  XtRealizeWidget( nav_shell);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)activate_close, this);

  wow = new CoWowGtk( toplevel);
  trace_timerid = wow->timer_new();

  viewsetup();
  flow_Open( flow_ctx, filename);
  trasetup();
  trace_start();

#if defined OS_LINUX
  {
    struct stat info;

    if ( stat( filename, &info) != -1)
      version = info.st_ctime;    
  }
#endif
#endif
}
