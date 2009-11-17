/* 
 * Proview   $Id: xtt_hist_gtk.cpp,v 1.1 2007-01-04 08:29:32 claes Exp $
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

/* xtt_hist_gtk.cpp -- Historical event window in xtt

   Author: Jonas Nylund.
   Last modification: 030217
*/

#if defined OS_LINUX

using namespace std;

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "cow_wow_gtk.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "rt_mh_util.h"
#include "rt_elog.h"
#include "co_dcli.h"
extern "C" {
#include <db.h>
}

#include <deque>
#include <algorithm>

#include "co_lng.h"
#include "xtt_hist_gtk.h"
#include "rt_xnav_msg.h"
#include "xtt_evlist_gtk.h"

#define SENS 	1
#define INSENS  0
#define DONT_SET_SENS -1
/* 24 hours in seconds */
#define ONEDAY 86400


static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  HistGtk::activate_exit( w, data);
  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

HistGtk::HistGtk( void *hist_parent_ctx,
		  GtkWidget *hist_parent_wid,
		  char *hist_name, pwr_tObjid objid,
		  pwr_tStatus *status) :
  Hist( hist_parent_ctx, hist_name, objid, status),
  parent_wid(hist_parent_wid), parent_wid_hist(NULL)
{
  const int hist_width = 800;
  const int hist_height = 700;

  //shall be MessageHandler.EventLogSize
  hist_size = 100000;

  // Gtk
  parent_wid_hist = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
						"default-height", hist_height,
						"default-width", hist_width,
						"title", CoWowGtk::translate_utf8(hist_name),
						NULL);

  g_signal_connect( parent_wid_hist, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( parent_wid_hist, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( parent_wid_hist, "focus-in-event", G_CALLBACK(action_inputfocus), this);

  CoWowGtk::SetWindowIcon( parent_wid_hist);

  GtkWidget *hist_vbox = gtk_vbox_new( FALSE, 0);

  // Menu
  // Accelerators
  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(parent_wid_hist), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File entry
  GtkWidget *file_print = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Print"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_print), 
				 gtk_image_new_from_stock( "gtk-print", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_print, "activate", G_CALLBACK(activate_print), this);
  
  GtkWidget *file_close = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Close"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_close), 
				 gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_MENU));
  g_signal_connect(file_close, "activate", G_CALLBACK(activate_exit), this);
  gtk_widget_add_accelerator( file_close, "activate", accel_g,
			      'w', GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_File"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Functions entry
  GtkWidget *functions_open_plc = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("Open _Program"));
  g_signal_connect( functions_open_plc, "activate", 
		    G_CALLBACK(activate_open_plc), this);
  gtk_widget_add_accelerator( functions_open_plc, "activate", accel_g,
			      'l', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_display_object = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Display object in Navigator"));
  g_signal_connect( functions_display_object, "activate", 
		    G_CALLBACK(activate_display_in_xnav), this);
  gtk_widget_add_accelerator( functions_display_object, "activate", accel_g,
			      'd', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_search = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Search"));
  g_signal_connect( functions_search, "activate", 
		    G_CALLBACK(ok_btn), this);
  gtk_widget_add_accelerator( functions_search, "activate", accel_g,
			      'f', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkMenu *func_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_open_plc);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_display_object);
  gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_search);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Functions"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(func_menu));

  // View entry
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _In"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_in), 
				 gtk_image_new_from_stock( "gtk-zoom-in", GTK_ICON_SIZE_MENU));
  g_signal_connect( view_zoom_in, "activate", 
		    G_CALLBACK(activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);
  
  GtkWidget *view_zoom_out = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Out"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_out), 
				 gtk_image_new_from_stock( "gtk-zoom-out", GTK_ICON_SIZE_MENU));
  g_signal_connect( view_zoom_out, "activate", 
		    G_CALLBACK(activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);
  
  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Reset"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_reset), 
				 gtk_image_new_from_stock( "gtk-zoom-100", GTK_ICON_SIZE_MENU));
  g_signal_connect( view_zoom_reset, "activate", 
		    G_CALLBACK(activate_zoom_reset), this);
  gtk_widget_add_accelerator( view_zoom_reset, "activate", accel_g,
			      'b', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

  GtkWidget *view_disp_hundredth = gtk_check_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Display hundredth"));
  g_signal_connect( view_disp_hundredth, "activate", 
		    G_CALLBACK(activate_disp_hundredth), this);

  GtkWidget *view_hide_object = gtk_check_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Hide Event Name"));
  g_signal_connect( view_hide_object, "activate", 
		    G_CALLBACK(activate_hide_object), this);

  GtkWidget *view_hide_text = gtk_check_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("Hide _Event Text"));
  g_signal_connect( view_hide_text, "activate", 
		    G_CALLBACK(activate_hide_text), this);

  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_disp_hundredth);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_hide_object);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_hide_text);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_View"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

  // Help entry
  GtkWidget *help_help = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Help"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_help), 
				 gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
  g_signal_connect(help_help, "activate", G_CALLBACK(activate_help), this);
  gtk_widget_add_accelerator( help_help, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *help_helpevent = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("Help Selected Event"));
  g_signal_connect( help_helpevent, "activate", 
		    G_CALLBACK(activate_helpevent), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_helpevent);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Help"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Search dialog
  // Time box
  GtkWidget *sea_time_start_label = gtk_label_new( CoWowGtk::translate_utf8("Start time"));
  gtk_widget_set_size_request( sea_time_start_label, 120, -1);
  gtk_misc_set_alignment( GTK_MISC(sea_time_start_label), 0.0, 0.5);
  start_time_entry_w = gtk_entry_new();
  gtk_widget_set_size_request( start_time_entry_w, 140, -1);

  GtkWidget *sea_time_stop_label = gtk_label_new( CoWowGtk::translate_utf8("Stop time"));
  gtk_widget_set_size_request( sea_time_stop_label, 120, -1);
  stop_time_entry_w = gtk_entry_new();
  gtk_widget_set_size_request( start_time_entry_w, 140, -1);

  // Time option menu
  GtkWidget *sea_time_all = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8("All    "));
  g_signal_connect( sea_time_all, "activate", G_CALLBACK( all_cb), this);

  GtkWidget *sea_time_today = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8("Today"));
  g_signal_connect( sea_time_today, "activate", G_CALLBACK( today_cb), this);

  GtkWidget *sea_time_yesterday = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8("Yesterday"));
  g_signal_connect( sea_time_yesterday, "activate", G_CALLBACK( yesterday_cb), this);

  GtkWidget *sea_time_thisw = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8("This Week"));
  g_signal_connect( sea_time_thisw, "activate", G_CALLBACK( thisw_cb), this);

  GtkWidget *sea_time_lastw = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8("Last Week"));
  g_signal_connect( sea_time_lastw, "activate", G_CALLBACK( lastw_cb), this);

  GtkWidget *sea_time_thism = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8("This Month"));
  g_signal_connect( sea_time_thism, "activate", G_CALLBACK( thisw_cb), this);

  GtkWidget *sea_time_lastm = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8("Last Month"));
  g_signal_connect( sea_time_lastm, "activate", G_CALLBACK( lastw_cb), this);

  GtkWidget *sea_time_time = gtk_menu_item_new_with_label( CoWowGtk::translate_utf8("Time"));
  g_signal_connect( sea_time_time, "activate", G_CALLBACK( time_cb), this);

  GtkMenu *sea_time_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append( GTK_MENU_SHELL(sea_time_menu), sea_time_all);
  gtk_menu_shell_append( GTK_MENU_SHELL(sea_time_menu), sea_time_today);
  gtk_menu_shell_append( GTK_MENU_SHELL(sea_time_menu), sea_time_yesterday);
  gtk_menu_shell_append( GTK_MENU_SHELL(sea_time_menu), sea_time_thisw);
  gtk_menu_shell_append( GTK_MENU_SHELL(sea_time_menu), sea_time_lastw);
  gtk_menu_shell_append( GTK_MENU_SHELL(sea_time_menu), sea_time_thism);
  gtk_menu_shell_append( GTK_MENU_SHELL(sea_time_menu), sea_time_lastm);
  gtk_menu_shell_append( GTK_MENU_SHELL(sea_time_menu), sea_time_time);
  GtkWidget *sea_time_omenu = (GtkWidget *)g_object_new( GTK_TYPE_OPTION_MENU, 
							 "menu", sea_time_menu, NULL);

  gtk_option_menu_set_history( GTK_OPTION_MENU(sea_time_omenu), 0);

  GtkWidget *sea_timebox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_timebox), sea_time_start_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_timebox), start_time_entry_w, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_timebox), sea_time_stop_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_timebox), stop_time_entry_w, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_timebox), sea_time_omenu, FALSE, FALSE, 10);

  // Event type box
  GtkWidget *sea_type_label = gtk_label_new( CoWowGtk::translate_utf8("Event type"));
  gtk_widget_set_size_request( sea_type_label, 120, -1);
  gtk_misc_set_alignment( GTK_MISC(sea_type_label), 0.0, 0.5);

  alarm_toggle_w = gtk_check_button_new_with_label( CoWowGtk::translate_utf8("Active"));
  gtk_widget_set_size_request( alarm_toggle_w, 100, -1);
  info_toggle_w = gtk_check_button_new_with_label( CoWowGtk::translate_utf8("Message"));
  gtk_widget_set_size_request( info_toggle_w, 100, -1);
  ret_toggle_w = gtk_check_button_new_with_label( CoWowGtk::translate_utf8("Return"));
  gtk_widget_set_size_request( ret_toggle_w, 100, -1);
  ack_toggle_w = gtk_check_button_new_with_label( CoWowGtk::translate_utf8("Ack"));
  gtk_widget_set_size_request( ack_toggle_w, 100, -1);

  GtkWidget *sea_typebox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_typebox), sea_type_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_typebox), alarm_toggle_w, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_typebox), info_toggle_w, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_typebox), ret_toggle_w, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_typebox), ack_toggle_w, FALSE, FALSE, 0);

  // Event priority box
  GtkWidget *sea_prio_label = gtk_label_new( CoWowGtk::translate_utf8("Priority"));
  gtk_widget_set_size_request( sea_prio_label, 120, -1);
  gtk_misc_set_alignment( GTK_MISC(sea_prio_label), 0.0, 0.5);

  prioA_toggle_w = gtk_check_button_new_with_label( CoWowGtk::translate_utf8("A-Alarm"));
  gtk_widget_set_size_request( prioA_toggle_w, 100, -1);
  prioB_toggle_w = gtk_check_button_new_with_label( CoWowGtk::translate_utf8("B-Alarm"));
  gtk_widget_set_size_request( prioB_toggle_w, 100, -1);
  prioC_toggle_w = gtk_check_button_new_with_label( CoWowGtk::translate_utf8("C-Alarm"));
  gtk_widget_set_size_request( prioC_toggle_w, 100, -1);
  prioD_toggle_w = gtk_check_button_new_with_label( CoWowGtk::translate_utf8("D-Alarm"));
  gtk_widget_set_size_request( prioD_toggle_w, 100, -1);

  GtkWidget *sea_priobox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_priobox), sea_prio_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_priobox), prioA_toggle_w, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_priobox), prioB_toggle_w, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_priobox), prioC_toggle_w, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_priobox), prioD_toggle_w, FALSE, FALSE, 0);

  // Event name box
  GtkWidget *sea_eventname_label = gtk_label_new( CoWowGtk::translate_utf8("Event name"));
  gtk_widget_set_size_request( sea_eventname_label, 120, -1);
  gtk_misc_set_alignment( GTK_MISC(sea_eventname_label), 0.0, 0.5);
  event_name_entry_w = gtk_entry_new();

  GtkWidget *sea_eventnamebox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_eventnamebox), sea_eventname_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_eventnamebox), event_name_entry_w, TRUE, TRUE, 0);

  // Event text box
  GtkWidget *sea_eventtext_label = gtk_label_new( CoWowGtk::translate_utf8("Event text"));
  gtk_widget_set_size_request( sea_eventtext_label, 120, -1);
  gtk_misc_set_alignment( GTK_MISC(sea_eventtext_label), 0.0, 0.5);
  event_text_entry_w = gtk_entry_new();

  GtkWidget *sea_eventtextbox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_eventtextbox), sea_eventtext_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_eventtextbox), event_text_entry_w, TRUE, TRUE, 0);

  // Number of events box
  GtkWidget *sea_numevents_label = gtk_label_new( CoWowGtk::translate_utf8("Number of Events:"));
  gtk_widget_set_size_request( sea_numevents_label, 140, -1);
  gtk_misc_set_alignment( GTK_MISC(sea_numevents_label), 0.0, 0.5);
  nrofevents_string_lbl_w = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(nrofevents_string_lbl_w), 0.0, 0.5);

  GtkWidget *sea_search_button = gtk_button_new_with_label( CoWowGtk::translate_utf8("Search"));
  gtk_widget_set_size_request( sea_search_button, 80, 25);  
  g_signal_connect( sea_search_button, "clicked", G_CALLBACK( ok_btn), this);

  GtkWidget *sea_numeventsbox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_numeventsbox), sea_numevents_label, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_numeventsbox), nrofevents_string_lbl_w, TRUE, TRUE, 0);
  gtk_box_pack_end( GTK_BOX(sea_numeventsbox), sea_search_button, FALSE, FALSE, 50);

  // Searchcondition box 1
  search_string_lbl_w = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(search_string_lbl_w), 0.0, 0.5);
  search_string2_lbl_w = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(search_string2_lbl_w), 0.0, 0.5);
  search_string3_lbl_w = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(search_string3_lbl_w), 0.0, 0.5);
  search_string4_lbl_w = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(search_string4_lbl_w), 0.0, 0.5);

  GtkWidget *sea_stringbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_stringbox), search_string_lbl_w, FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(sea_stringbox), search_string2_lbl_w, FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(sea_stringbox), search_string3_lbl_w, FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(sea_stringbox), search_string4_lbl_w, FALSE, FALSE, 2);

  GtkWidget *sea_stringframe = gtk_frame_new( CoWowGtk::translate_utf8("Search Condition"));
  // gtk_frame_set_shadow_type( GTK_FRAME(sea_stringframe), GTK_SHADOW_ETCHED_IN);
  gtk_container_add( GTK_CONTAINER(sea_stringframe), sea_stringbox);

  GtkWidget *sea_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sea_vbox), sea_timebox, FALSE, FALSE, 5);
  gtk_box_pack_start( GTK_BOX(sea_vbox), sea_typebox, FALSE, FALSE, 5);
  gtk_box_pack_start( GTK_BOX(sea_vbox), sea_priobox, FALSE, FALSE, 5);
  gtk_box_pack_start( GTK_BOX(sea_vbox), sea_eventnamebox, FALSE, FALSE, 5);
  gtk_box_pack_start( GTK_BOX(sea_vbox), sea_eventtextbox, FALSE, FALSE, 5);
  gtk_box_pack_start( GTK_BOX(sea_vbox), gtk_hseparator_new(), FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(sea_vbox), sea_numeventsbox, FALSE, FALSE, 5);
  gtk_box_pack_start( GTK_BOX(sea_vbox), sea_stringframe, FALSE, FALSE, 3);

  
  GtkWidget *hist_pane = gtk_vpaned_new();

  // Create hist
  hist = new EvListGtk( this, hist_pane, ev_eType_HistList, hist_size, &hist_widget);
  hist->start_trace_cb = &hist_start_trace_cb;
  hist->display_in_xnav_cb = &hist_display_in_xnav_cb;
  hist->popup_menu_cb = &hist_popup_menu_cb;

  gtk_paned_pack1( GTK_PANED(hist_pane), GTK_WIDGET(sea_vbox), TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(hist_pane), GTK_WIDGET(hist_widget), TRUE, TRUE);

  gtk_box_pack_start( GTK_BOX(hist_vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(hist_vbox), GTK_WIDGET(hist_pane), TRUE, TRUE, 0);

  gtk_container_add( GTK_CONTAINER(parent_wid_hist), hist_vbox);
  gtk_widget_show_all( parent_wid_hist);

  gtk_paned_set_position( GTK_PANED(hist_pane), 300);

  // Init start and stop time
  pwr_tStatus sts;
  pwr_tOName name_str;
  gint pos = 0;
  char buf[80];
  pwr_tTime StopTime;
  pwr_tTime StartTime;

  strcpy( buf, "1970-05-05 00:00:00");
  gtk_editable_insert_text( GTK_EDITABLE(start_time_entry_w), buf, 
			    strlen(buf), &pos);
  gtk_widget_set_sensitive( start_time_entry_w, FALSE);

  sts = time_GetTime( &StopTime);
  sts = AdjustForDayBreak( this, &StopTime, &StartTime);

  StopTime = StartTime;
  StopTime.tv_sec += ONEDAY;
  
  time_AtoFormAscii(&StopTime, SWE, SECOND, buf, sizeof(buf));
  gtk_editable_insert_text( GTK_EDITABLE(stop_time_entry_w), buf, 
			    strlen(buf), &pos);
  gtk_widget_set_sensitive( stop_time_entry_w, FALSE);

  // If objid is applied, search for this object
  pos = 0;
  sts = gdh_ObjidToName( objid, name_str, sizeof(name_str), cdh_mName_pathStrict);
  if (ODD(sts)) {
    gtk_editable_insert_text( GTK_EDITABLE(event_name_entry_w), name_str, 
			      strlen(name_str), &pos);
    this->eventName_str = name_str;
    get_hist_list();
  }

  wow = new CoWowGtk( parent_wid_hist);

  *status = 1;
}


//
//  Delete hist
//
HistGtk::~HistGtk()
{
  if ( hist)
    delete hist;
  gtk_widget_destroy( parent_wid_hist);
}

gboolean HistGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  HistGtk *hist = (HistGtk *)data;

  hist->hist->set_input_focus();

  return FALSE;
}

void HistGtk::ok_btn( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  brow_DeleteAll(histOP->hist->brow->ctx);
  histOP->eventType_Alarm = (bool)gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(((HistGtk *)histOP)->alarm_toggle_w));

  histOP->eventType_Ack = (bool)gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ((HistGtk *)histOP)->ack_toggle_w));

  histOP->eventType_Info = (bool)gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ((HistGtk *)histOP)->info_toggle_w));

  histOP->eventType_Return = (bool)gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ((HistGtk *)histOP)->ret_toggle_w));

  histOP->eventPrio_A = (bool)gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ((HistGtk *)histOP)->prioA_toggle_w));

  histOP->eventPrio_B = (bool)gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ((HistGtk *)histOP)->prioB_toggle_w));

  histOP->eventPrio_C = (bool)gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ((HistGtk *)histOP)->prioC_toggle_w));

  histOP->eventPrio_D = (bool)gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( ((HistGtk *)histOP)->prioD_toggle_w));

  histOP->minTime_str = gtk_editable_get_chars( GTK_EDITABLE(((HistGtk *)histOP)->start_time_entry_w), 0, -1);

  histOP->maxTime_str = gtk_editable_get_chars( GTK_EDITABLE(((HistGtk *)histOP)->stop_time_entry_w), 0, -1);

  histOP->eventText_str = gtk_editable_get_chars( GTK_EDITABLE(((HistGtk *)histOP)->event_text_entry_w), 0, -1);

  histOP->eventName_str = gtk_editable_get_chars( GTK_EDITABLE(((HistGtk *)histOP)->event_name_entry_w), 0, -1);


  histOP->get_hist_list();
  
  g_free(histOP->minTime_str);
  g_free(histOP->maxTime_str);
  g_free(histOP->eventText_str);
  g_free(histOP->eventName_str);

}

void HistGtk::activate_exit( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->close_cb(histOP);
}

void HistGtk::activate_print( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->activate_print();
}

void HistGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->hist->zoom( 1.2);
}

void HistGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->hist->zoom( 5.0/6);
}

void HistGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->hist->unzoom();
}

void HistGtk::activate_open_plc( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->hist->start_trace();
}

void HistGtk::activate_display_in_xnav( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->hist->display_in_xnav();
}

void HistGtk::activate_disp_hundredth( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  histOP->hist->set_display_hundredth( set);
}

void HistGtk::activate_hide_object( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  histOP->hist->set_hide_object( set);
}

void HistGtk::activate_hide_text( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  histOP->hist->set_hide_text( set);
}

void HistGtk::activate_help( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->activate_help();
}

void HistGtk::activate_helpevent( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->activate_helpevent();
}

//callbackfunctions from the searchdialog
void HistGtk::cancel_cb( GtkWidget *w, gpointer data)
{
//  printf("hist_cancel_cb\n");
}

void HistGtk::today_cb( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->today_cb();
}

void HistGtk::yesterday_cb( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->yesterday_cb();
}

void HistGtk::thisw_cb( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->thisw_cb();
}

void HistGtk::lastw_cb( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->lastw_cb();
}

void HistGtk::thism_cb( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->thism_cb();
}

void HistGtk::lastm_cb( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->lastm_cb();
}

void HistGtk::all_cb( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->all_cb();
}

void HistGtk::time_cb( GtkWidget *w, gpointer data)
{
  Hist *histOP = (Hist *)data;

  histOP->time_cb();
}

void HistGtk::set_num_of_events( int nrOfEvents)
{
  char buf[20];

  sprintf(buf, "    %u", nrOfEvents);
  gtk_label_set_text( GTK_LABEL(this->nrofevents_string_lbl_w), buf);
}

void HistGtk::set_search_string( const char *s1, const char *s2, 
			      const char *s3, const char *s4)
{

  gtk_label_set_text( GTK_LABEL(this->search_string_lbl_w), s1);
  gtk_label_set_text( GTK_LABEL(this->search_string2_lbl_w), s2);
  gtk_label_set_text( GTK_LABEL(this->search_string3_lbl_w), s3);
  gtk_label_set_text( GTK_LABEL(this->search_string4_lbl_w), s4);
}


/************************************************************************
*
* Name:		SetListTime
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description:	Sets the Time field for start and stop
* 
*************************************************************************/
void HistGtk::SetListTime( pwr_tTime StartTime, pwr_tTime StopTime, 
			   int Sensitive)
{
  char timestr[32];

  /* Show the resulting times */
  time_AtoFormAscii(&StopTime,SWE, SECOND,timestr,sizeof(timestr));

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(stop_time_entry_w), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(stop_time_entry_w), timestr, 
			    strlen(timestr), &pos);
     
  time_AtoFormAscii(&StartTime,SWE, SECOND,timestr,sizeof(timestr));
  pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(start_time_entry_w), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(start_time_entry_w), timestr, 
			    strlen(timestr), &pos);
    
  if (Sensitive != DONT_SET_SENS) {
    gtk_widget_set_sensitive( start_time_entry_w, Sensitive ? TRUE : FALSE);
    gtk_widget_set_sensitive( stop_time_entry_w, Sensitive ? TRUE : FALSE);
  }

}/* SetListTime */

#endif

















