/* 
 * Proview   $Id: xtt_op_gtk.cpp,v 1.11 2008-11-26 15:24:31 claes Exp $
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

/* xtt_op_gtk.cpp -- Alarm and event window in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_login.h"
#include "co_syi.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "co_wow_gtk.h"
#include "co_lng.h"
#include "xtt_op_gtk.h"
#include "rt_xnav_msg.h"

#define OP_HEIGHT_MIN 75
#define OP_HEIGHT_INC 20
#define OP_HEIGHT_MAX (OP_HEIGHT_MIN + 3 * OP_HEIGHT_INC)
#define OP_HEIGHT_STATUSBAR 30


static void cnv_pango_text( char *in, char *out, int size)
{
  char *s = in;
  char *t = out;

  for ( int i = 0; i < size && *s; i++, s++) {
    *(t++) = *s;
    if ( *s == '&') {
      // & -> &amp;
      if ( i+4 >= size)
	break;
      *(t++) = 'a';
      *(t++) = 'm';
      *(t++) = 'p';
      *(t++) = ';';
    }
  }
  *t = 0;
}


OpGtk::OpGtk( void *op_parent_ctx,
	      GtkWidget *op_parent_wid,
	      char *opplace,
	      pwr_tStatus *status) :
  Op( op_parent_ctx, opplace, status), parent_wid(op_parent_wid), title_label(0), a_height(2),
  text_size(12)
{
  memset( a_exist, 0, sizeof(a_exist));
  memset( a_active, 0, sizeof(a_active));
  GdkColor red_color;
  gdk_color_parse( "#FF7575", &red_color);

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
			   "default-height", OP_HEIGHT_MIN,
			   "default-width", 1100,
			   NULL);

  gtk_window_set_decorated( GTK_WINDOW(toplevel), FALSE);
  CoWowGtk::SetWindowIcon( toplevel);

  pwr_tFileName fname;
  dcli_translate_filename( fname, "$pwr_exe/xtt_alarm_active.png");
  aalarm_active[0] = gtk_image_new_from_file( fname);
  aalarm_active[1] = gtk_image_new_from_file( fname);
  aalarm_active[2] = gtk_image_new_from_file( fname);
  aalarm_active[3] = gtk_image_new_from_file( fname);
  aalarm_active[4] = gtk_image_new_from_file( fname);
  aalarm_label[0] = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(aalarm_label[0]), 0.02, 0.5);
  gtk_label_set_use_markup( GTK_LABEL(aalarm_label[0]), TRUE);
  aalarm_label[1] = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(aalarm_label[1]), 0.02, 0.5);
  gtk_label_set_use_markup( GTK_LABEL(aalarm_label[1]), TRUE);
  aalarm_label[2] = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(aalarm_label[2]), 0.02, 0.5);
  gtk_label_set_use_markup( GTK_LABEL(aalarm_label[2]), TRUE);
  aalarm_label[3] = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(aalarm_label[3]), 0.02, 0.5);
  gtk_label_set_use_markup( GTK_LABEL(aalarm_label[3]), TRUE);
  aalarm_label[4] = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(aalarm_label[4]), 0.02, 0.5);
  gtk_label_set_use_markup( GTK_LABEL(aalarm_label[4]), TRUE);
  alarmcnt_label = gtk_label_new("");
  GtkWidget *aalarm_mark = gtk_label_new("A");

  balarm_active = gtk_image_new_from_file( fname);
  balarm_mark = gtk_label_new("");
  balarm_label = gtk_label_new("");
  gtk_misc_set_alignment( GTK_MISC(balarm_label), 0.02, 0.5);
  gtk_label_set_use_markup( GTK_LABEL(balarm_label), TRUE);


  aalarm_box[0] = gtk_hbox_new( FALSE, 0);
  GtkWidget *ebox1 = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(ebox1), aalarm_label[0]);
  gtk_widget_modify_bg( ebox1, GTK_STATE_NORMAL, &red_color);
  GtkWidget *abox1 = gtk_event_box_new();
  gtk_widget_set_size_request( abox1, 20, 20);
  gtk_container_add( GTK_CONTAINER(abox1), aalarm_active[0]);
  gtk_box_pack_start( GTK_BOX(aalarm_box[0]), abox1, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(aalarm_box[0]), ebox1, TRUE, TRUE, 0);
  gtk_widget_set_size_request( aalarm_label[0], -1, 20);
  gtk_widget_set_size_request( aalarm_active[0], -1, 20);

  aalarm_box[1] = gtk_hbox_new( FALSE, 0);
  GtkWidget *ebox2 = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(ebox2), aalarm_label[1]);
  gtk_widget_modify_bg( ebox2, GTK_STATE_NORMAL, &red_color);
  GtkWidget *abox2 = gtk_event_box_new();
  gtk_widget_set_size_request( abox2, 20, 20);
  gtk_container_add( GTK_CONTAINER(abox2), aalarm_active[1]);
  gtk_box_pack_start( GTK_BOX(aalarm_box[1]), abox2, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(aalarm_box[1]), ebox2, TRUE, TRUE, 0);
  gtk_widget_set_size_request( aalarm_label[1], -1, 20);
  gtk_widget_set_size_request( aalarm_active[1], -1, 20);
  
  aalarm_box[2] = gtk_hbox_new( FALSE, 0);
  GtkWidget *ebox3 = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(ebox3), aalarm_label[2]);
  gtk_widget_modify_bg( ebox3, GTK_STATE_NORMAL, &red_color);
  GtkWidget *abox3 = gtk_event_box_new();
  gtk_widget_set_size_request( abox3, 20, 20);
  gtk_container_add( GTK_CONTAINER(abox3), aalarm_active[2]);
  gtk_box_pack_start( GTK_BOX(aalarm_box[2]), abox3, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(aalarm_box[2]), ebox3, TRUE, TRUE, 0);
  gtk_widget_set_size_request( aalarm_label[2], -1, 20);
  gtk_widget_set_size_request( aalarm_active[2], -1, 20);
  
  aalarm_box[3] = gtk_hbox_new( FALSE, 0);
  GtkWidget *ebox4 = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(ebox4), aalarm_label[3]);
  gtk_widget_modify_bg( ebox4, GTK_STATE_NORMAL, &red_color);
  GtkWidget *abox4 = gtk_event_box_new();
  gtk_widget_set_size_request( abox4, 20, 20);
  gtk_container_add( GTK_CONTAINER(abox4), aalarm_active[3]);
  gtk_box_pack_start( GTK_BOX(aalarm_box[3]), abox4, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(aalarm_box[3]), ebox4, TRUE, TRUE, 0);
  gtk_widget_set_size_request( aalarm_label[3], -1, 20);
  gtk_widget_set_size_request( aalarm_active[3], -1, 20);
  
  aalarm_box[4] = gtk_hbox_new( FALSE, 0);
  GtkWidget *ebox5 = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(ebox5), aalarm_label[4]);
  gtk_widget_modify_bg( ebox5, GTK_STATE_NORMAL, &red_color);
  GtkWidget *abox5 = gtk_event_box_new();
  gtk_widget_set_size_request( abox5, 20, 20);
  gtk_container_add( GTK_CONTAINER(abox5), aalarm_active[4]);
  gtk_box_pack_start( GTK_BOX(aalarm_box[4]), abox5, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(aalarm_box[4]), ebox5, TRUE, TRUE, 0);
  gtk_widget_set_size_request( aalarm_label[4], -1, 20);
  gtk_widget_set_size_request( aalarm_active[4], -1, 20);
  
  GtkWidget *vbox_aalarm = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox_aalarm), aalarm_box[0], FALSE, FALSE, 1);
  gtk_box_pack_start( GTK_BOX(vbox_aalarm), aalarm_box[1], FALSE, FALSE, 1);
  gtk_box_pack_start( GTK_BOX(vbox_aalarm), aalarm_box[2], FALSE, FALSE, 1);
  gtk_box_pack_start( GTK_BOX(vbox_aalarm), aalarm_box[3], FALSE, FALSE, 1);
  gtk_box_pack_start( GTK_BOX(vbox_aalarm), aalarm_box[4], FALSE, FALSE, 1);

  balarm_ebox = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(balarm_ebox), balarm_label);
  GtkWidget *bbox = gtk_event_box_new();
  gtk_container_add( GTK_CONTAINER(bbox), balarm_active);
  gtk_widget_set_size_request( bbox, 20, 20);

  balarm_box = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(balarm_box), bbox, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(balarm_box), balarm_ebox, TRUE, TRUE, 0);

  // Acknowledge button for a alarms
  GtkWidget *aalarm_ack = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_acknowledge.png");
  gtk_container_add( GTK_CONTAINER(aalarm_ack), 
		     gtk_image_new_from_file( fname));
  gtk_widget_set_size_request( aalarm_ack, 30, 20);
  gtk_widget_set_size_request( aalarm_mark, -1, 20);
  g_signal_connect(aalarm_ack, "clicked", G_CALLBACK(activate_aalarm_ack), this);
  
  // Increment size button
  GtkWidget *incr_button = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_down.png");
  gtk_container_add( GTK_CONTAINER(incr_button), 
		     gtk_image_new_from_file( fname));
  gtk_widget_set_size_request( incr_button, 30, 20);
  gtk_widget_set_size_request( alarmcnt_label, -1, 20);
  g_signal_connect(incr_button, "clicked", G_CALLBACK(activate_aalarm_incr), this);
  
  // Decrement size button
  decr_button = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_up.png");
  gtk_container_add( GTK_CONTAINER(decr_button), 
		     gtk_image_new_from_file( fname));
  gtk_widget_set_size_request( decr_button, 30, 20);
  g_signal_connect(decr_button, "clicked", G_CALLBACK(activate_aalarm_decr), this);
  
  // Acknowledge button for b alarms
  GtkWidget *balarm_ack = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_acknowledge.png");
  gtk_container_add( GTK_CONTAINER(balarm_ack), 
		     gtk_image_new_from_file( fname));
  gtk_widget_set_size_request( balarm_ack, 30, 20);
  gtk_widget_set_size_request( balarm_mark, -1, 20);
  g_signal_connect(balarm_ack, "clicked", G_CALLBACK(activate_balarm_ack), this);
  
  GtkWidget *hbox_abutton = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_abutton), aalarm_ack, FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(hbox_abutton), aalarm_mark, FALSE, FALSE, 2);

  GtkWidget *hbox_incrbutton = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_incrbutton), incr_button, FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(hbox_incrbutton), alarmcnt_label, FALSE, FALSE, 2);

  GtkWidget *hbox_decrbutton = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_decrbutton), decr_button, FALSE, FALSE, 2);

  GtkWidget *vbox_abutton = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox_abutton), hbox_abutton, FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(vbox_abutton), hbox_incrbutton, FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(vbox_abutton), hbox_decrbutton, FALSE, FALSE, 2);


  GtkWidget *hbox_bbutton = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_bbutton), balarm_ack, FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(hbox_bbutton), balarm_mark, FALSE, FALSE, 2);

  GtkWidget *hbox_a = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_a), vbox_abutton, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_a), vbox_aalarm, TRUE, TRUE, 0);

  GtkWidget *hbox_b = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_b), hbox_bbutton, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_b), balarm_box, TRUE, TRUE, 0);

  
  GtkWidget *vbox_ala = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox_ala), hbox_a, TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(vbox_ala), gtk_hseparator_new(), FALSE, FALSE, 2);
  gtk_box_pack_end( GTK_BOX(vbox_ala), hbox_b, FALSE, FALSE, 0);

  // Menu
  // Accelerators
  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // Functions entry
  // Submenu Alarm

  GtkWidget *functions_alarm_alarmlist = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_AlarmList"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_alarmlist.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_alarm_alarmlist), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_alarm_alarmlist, "activate", 
		    G_CALLBACK(activate_alarmlist), this);

  GtkWidget *functions_alarm_eventlist = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_EventList"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_eventlist.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_alarm_eventlist), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_alarm_eventlist, "activate", 
		    G_CALLBACK(activate_eventlist), this);

  GtkWidget *functions_alarm_eventlog = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("Event_Log"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_eventlog.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_alarm_eventlog), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_alarm_eventlog, "activate", 
		    G_CALLBACK(activate_eventlog), this);

  GtkWidget *functions_alarm_blocklist = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_BlockList"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_blocklist.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_alarm_blocklist), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_alarm_blocklist, "activate", 
		    G_CALLBACK(activate_blocklist), this);

  GtkWidget *functions_alarm = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Alarm"));
  GtkMenu *functions_alarm_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_alarm_menu), functions_alarm_alarmlist);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_alarm_menu), functions_alarm_eventlist);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_alarm_menu), functions_alarm_eventlog);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_alarm_menu), functions_alarm_blocklist);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_alarm),
			    GTK_WIDGET(functions_alarm_menu));
  // End Alarm submenu

  // Submenu Curves
  GtkWidget *functions_curves_trend = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Trends"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_meth_trend.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_curves_trend), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_curves_trend, "activate", 
		    G_CALLBACK(activate_trend), this);

  GtkWidget *functions_curves_fast = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Fast"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_meth_fast.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_curves_fast), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_curves_fast, "activate", 
		    G_CALLBACK(activate_fast), this);

  GtkWidget *functions_curves_history = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Process History"));
  g_signal_connect( functions_curves_history, "activate", 
		    G_CALLBACK(activate_history), this);


  GtkWidget *functions_curves = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Curves"));
  GtkMenu *functions_curves_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_curves_menu), functions_curves_trend);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_curves_menu), functions_curves_fast);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_curves_menu), functions_curves_history);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_curves),
			    GTK_WIDGET(functions_curves_menu));
  // End Curves submenu

  // Submenu View
  GtkWidget *functions_view_incr = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Larger Text"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_view_incr), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_view_incr, "activate", 
		    G_CALLBACK(activate_zoom_in), this);

  GtkWidget *functions_view_decr = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Smaller Text"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_view_decr), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_view_decr, "activate", 
		    G_CALLBACK(activate_zoom_out), this);

  GtkWidget *functions_view = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_View"));
  GtkMenu *functions_view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_view_menu), functions_view_incr);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_view_menu), functions_view_decr);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_view),
			    GTK_WIDGET(functions_view_menu));
  // End View submenu

  // Submenu User
  GtkWidget *functions_user_switch = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("S_witch User"));
  g_signal_connect( functions_user_switch, "activate", 
		    G_CALLBACK(activate_switch_user), this);

  GtkWidget *functions_user_show = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Show User"));
  g_signal_connect( functions_user_show, "activate", 
		    G_CALLBACK(activate_show_user), this);

  GtkWidget *functions_user_logout = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Return"));
  g_signal_connect( functions_user_logout, "activate", 
		    G_CALLBACK(activate_logout), this);

  GtkWidget *functions_user = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_User"));
  GtkMenu *functions_user_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_user_menu), functions_user_switch);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_user_menu), functions_user_show);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_user_menu), functions_user_logout);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_user),
			    GTK_WIDGET(functions_user_menu));
  // End User submenu

  GtkWidget *functions_navigator = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Navigator"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_navigator.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_navigator), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_navigator, "activate", 
		    G_CALLBACK(activate_navigator), this);
  // gtk_widget_add_accelerator( functions_navigator, "activate", accel_g,
  //			      'd', GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

  GtkWidget *functions_graph = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Process Graphics"));
  dcli_translate_filename( fname, "$pwr_exe/wtt_ge.png"); //TODO
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_graph), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_graph, "activate", 
		    G_CALLBACK(activate_graph), this);
  // gtk_widget_add_accelerator( functions_graph, "activate", accel_g,
  //			      'g', GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

  // Submenu Help
  GtkWidget *functions_help_project = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Project"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_help.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_help_project), 
				 gtk_image_new_from_file( fname));
  g_signal_connect( functions_help_project, "activate", 
		    G_CALLBACK(activate_help), this);

  GtkWidget *functions_help_overview = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Overview"));
  g_signal_connect( functions_help_overview, "activate", 
		    G_CALLBACK(activate_help_overview), this);

  GtkWidget *functions_help_opwin = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_OperatorWindow"));
  g_signal_connect( functions_help_opwin, "activate", 
		    G_CALLBACK(activate_help_opwin), this);

  GtkWidget *functions_help_proview = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_About Proview"));
  g_signal_connect( functions_help_proview, "activate", 
		    G_CALLBACK(activate_help_proview), this);


  GtkWidget *functions_help = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Help"));
  GtkMenu *functions_help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_help_menu), functions_help_project);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_help_menu), functions_help_overview);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_help_menu), functions_help_opwin);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_help_menu), functions_help_proview);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions_help),
			    GTK_WIDGET(functions_help_menu));
  // End Help submenu

  functions_close = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Close"));
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions_close), 
				 gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_MENU));

  g_signal_connect(functions_close, "activate", G_CALLBACK(activate_exit), this);
  // gtk_widget_add_accelerator( functions_close, "activate", accel_g,
  //			      'w', GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_alarm);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_curves);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_graph);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_navigator);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_view);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_user);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_help);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_close);

  GtkWidget *functions = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Functions"));
  dcli_translate_filename( fname, "$pwr_exe/xtt_down.png");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(functions), 
				 gtk_image_new_from_file( fname));

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));


  // Toolbar
  GtkWidget *tools = (GtkWidget *) g_object_new(GTK_TYPE_TOOLBAR, NULL);
  GtkWidget *tools2 = (GtkWidget *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  GtkWidget *tools_zoom_in = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(activate_zoom_in), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_zoom_in, CoWowGtk::translate_utf8("Larger Text"), "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(activate_zoom_out), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_zoom_out, CoWowGtk::translate_utf8("Smaller Text"), "");

  GtkWidget *tools_help = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_help.png");
  gtk_container_add( GTK_CONTAINER(tools_help), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_help, "clicked", G_CALLBACK(activate_help), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_help, CoWowGtk::translate_utf8("Help"), "");

  tools_close = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_close.png");
  gtk_container_add( GTK_CONTAINER(tools_close), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_close, "clicked", G_CALLBACK(activate_exit), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_close, CoWowGtk::translate_utf8("Close"), "");

  GtkWidget *tools_alarmlist = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_alarmlist.png");
  gtk_container_add( GTK_CONTAINER(tools_alarmlist), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_alarmlist, "clicked", G_CALLBACK(activate_alarmlist), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools2), tools_alarmlist, CoWowGtk::translate_utf8("AlarmList"), "");

  GtkWidget *tools_eventlist = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_eventlist.png");
  gtk_container_add( GTK_CONTAINER(tools_eventlist), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_eventlist, "clicked", G_CALLBACK(activate_eventlist), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools2), tools_eventlist, CoWowGtk::translate_utf8("EventList"), "");

  GtkWidget *tools_eventlog = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_eventlog.png");
  gtk_container_add( GTK_CONTAINER(tools_eventlog), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_eventlog, "clicked", G_CALLBACK(activate_eventlog), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools2), tools_eventlog, CoWowGtk::translate_utf8("EventLog"), "");

  GtkWidget *tools_blocklist = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_blocklist.png");
  gtk_container_add( GTK_CONTAINER(tools_blocklist), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_blocklist, "clicked", G_CALLBACK(activate_blocklist), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools2), tools_blocklist, CoWowGtk::translate_utf8("Blocked Alarms"), "");

  GtkWidget *tools_navigator = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_navigator.png");
  gtk_container_add( GTK_CONTAINER( tools_navigator), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_navigator, "clicked", G_CALLBACK(activate_navigator), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools2), tools_navigator, CoWowGtk::translate_utf8("Navigator"), "");

  // System pushbuttons

  GtkWidget *sysbutton_box = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sysbutton_box), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sysbutton_box), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sysbutton_box), GTK_WIDGET(tools2), FALSE, FALSE, 0);
  gtk_widget_set_size_request( sysbutton_box, 160, -1);

  configure( opplace);

  // Status bar
  GtkWidget *status_bar;
  if ( !(layout_mask & pwr_mOpWindLayoutMask_HideStatusBar)) {
    char text[80];

    dcli_translate_filename( fname, "$pwr_exe/proview_icon.png");
    GtkWidget *icon_image = gtk_image_new_from_file( fname);

    title_label = gtk_label_new("");

    status_bar = gtk_hbox_new( FALSE, 0);
    gtk_box_pack_start( GTK_BOX(status_bar), GTK_WIDGET(icon_image), FALSE, FALSE, 5);
    gtk_box_pack_start( GTK_BOX(status_bar), GTK_WIDGET(title_label), FALSE, FALSE, 20);

    // Add node supervision buttons
    for ( unsigned int i = 0; i < sup_vect.size(); i++) {
      GtkWidget *node_label = gtk_label_new("");
      snprintf( text, sizeof(text), "<span size=\"%d\">%s</span>", 12 * 1024, sup_vect[i].node_name);
      gtk_label_set_markup( GTK_LABEL(node_label), CoWowGtk::convert_utf8(text));

      GtkWidget *node_button = gtk_button_new();
      GtkWidget *node_hbox = gtk_hbox_new( FALSE, 0);
      dcli_translate_filename( fname, "$pwr_exe/xtt_ind_gray2.png");
      GtkWidget *node_image = gtk_image_new_from_file( fname);
      gtk_box_pack_start( GTK_BOX(node_hbox), GTK_WIDGET(node_image), FALSE, FALSE, 8);
      gtk_box_pack_start( GTK_BOX(node_hbox), GTK_WIDGET(node_label), FALSE, FALSE, 8);
      gtk_container_add( GTK_CONTAINER(node_button), node_hbox);
      sup_vect[i].imagew = (void *)node_image;
      sup_vect[i].buttonw = (void *)node_button;
      g_signal_connect( node_button, "clicked", G_CALLBACK(activate_sup_node), this);
      g_object_set( node_button, "can-focus", FALSE, NULL);
      gtk_box_pack_start( GTK_BOX(status_bar), GTK_WIDGET(node_button), FALSE, FALSE, 5);
    }
  }

  // Main window

  GtkWidget *hbox_conf = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_conf), sysbutton_box, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_conf), gtk_hseparator_new(), FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(hbox_conf), appl_form, TRUE, TRUE, 0);

  GtkWidget *pane = gtk_hpaned_new();
  gtk_paned_pack1( GTK_PANED(pane), vbox_ala, TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(pane), hbox_conf, FALSE, TRUE);

  GtkWidget *vbox_conf = gtk_vbox_new( FALSE, 0);
  if ( !(layout_mask & pwr_mOpWindLayoutMask_HideStatusBar)) {
    gtk_box_pack_start( GTK_BOX(vbox_conf), status_bar, FALSE, FALSE, 0);
    gtk_box_pack_start( GTK_BOX(vbox_conf), gtk_hseparator_new(), FALSE, FALSE, 2);
  }
  gtk_box_pack_start( GTK_BOX(vbox_conf), pane, FALSE, FALSE, 0);

  GtkWidget *frame = gtk_frame_new( NULL);
  GdkColor background;
  gdk_color_parse( "black", &background);
  gtk_widget_modify_bg( frame, GTK_STATE_NORMAL, &background);
  
  gtk_container_add( GTK_CONTAINER(frame), vbox_conf);
  gtk_container_add( GTK_CONTAINER(toplevel), frame);

  gtk_widget_show_all(toplevel);

  if ( start_jop) {
    jop = new Jop( (void *)this);
    jop->command_cb = &jop_command_cb;
  }

  gtk_paned_set_position( GTK_PANED(pane), 500);
  g_object_set( aalarm_box[0], "visible", FALSE, NULL);
  g_object_set( aalarm_box[1], "visible", FALSE, NULL);
  g_object_set( aalarm_box[2], "visible", FALSE, NULL);
  g_object_set( aalarm_box[3], "visible", FALSE, NULL);
  g_object_set( aalarm_box[4], "visible", FALSE, NULL);
  g_object_set( balarm_box, "visible", FALSE, NULL);
  g_object_set( decr_button, "visible", FALSE, NULL);
  if ( layout_mask & pwr_mOpWindLayoutMask_HideCloseButton) {
    g_object_set( tools_close, "visible", FALSE, NULL);
    g_object_set( functions_close, "visible", FALSE, NULL);
  }

  if ( a_height == 5)
    activate_aalarm_decr( 0, this);
  if ( a_height == 4) {
    // g_object_set( decr_button, "visible", TRUE, NULL);
    activate_aalarm_decr( 0, this);
    activate_aalarm_decr( 0, this);
  }

  int width, height;
  GdkWindow *rootwindow = gtk_widget_get_root_window( toplevel);
  gdk_drawable_get_size( rootwindow, &width, &height);
  // gtk_window_get_size( GTK_WINDOW(toplevel), &width, &height);
  if ( width / height >= 2)
    // Assume two screens
    width = width / 2;
  gtk_window_resize( GTK_WINDOW(toplevel), width - 6, OP_HEIGHT_MIN);
  gtk_window_move( GTK_WINDOW(toplevel), 0, 0);

  wow = new CoWowGtk( toplevel);
  sup_timerid = wow->timer_new();
  wow->DisplayWarranty();

  if ( !(layout_mask & pwr_mOpWindLayoutMask_HideStatusBar))
    sup_scan( this);

  *status = 1;
}


//
//  Delete op
//
OpGtk::~OpGtk()
{
  if ( jop)
    delete jop;
  sup_timerid->remove();
  delete sup_timerid;
  delete wow;
  gtk_widget_destroy( toplevel);
}

void OpGtk::map()
{
  if ( poptimer.disabled())
    return;

  gtk_window_present( GTK_WINDOW(toplevel));

  poptimer.disable( 1000);
}

void OpGtk::add_close_button()
{
  g_object_set( tools_close, "visible", TRUE, NULL);
  g_object_set( functions_close, "visible", TRUE, NULL);
}


int OpGtk::create_menu_item( const char *name, int pixmap, int append, const char *cmd)
{
  GtkWidget *w;
  int sts;

  sts = wow->CreateMenuItem( name, (void *) menu_bar, pixmap, append, (void *)&w);
  if ( EVEN(sts)) return sts;

  OpCmd opcmd( w, cmd);
  cmd_vect.push_back(opcmd);
  g_signal_connect(w, "activate", G_CALLBACK(activate_cmd_menu_item), this);

  return 1;
}

int OpGtk::delete_menu_item( const char *name)
{
  return wow->DeleteMenuItem( "Functions-View", (void *) menu_bar);
}

void  OpGtk::update_alarm_info()
{
  evlist_sAlarmInfo info;
  int 		sts;
  int		i;
  int		height, active_height;
  char          str[40];
  char		text[500], ctext[500];
  char		timestr[40];
  int 		fsize = text_size * 1024;
  int		show_time = 0;
  time_eFormat  time_format;
  char 		*s;
  
  if ( get_alarm_info_cb) {
    sts = (get_alarm_info_cb)( parent_ctx, &info);
    if ( EVEN(sts)) return;

    GdkColor red_color, bg_color;
    gdk_color_parse( "Red", &red_color);
    gdk_color_parse( "Gray", &bg_color);

    height = 22;
    active_height = 22;

    snprintf( str, sizeof(str), "%d", info.alarms_total);
    gtk_label_set_text( GTK_LABEL(alarmcnt_label), str);

    if ( layout_mask & pwr_mOpWindLayoutMask_ShowAlarmDateAndTime) {
      time_format = time_eFormat_ComprDateAndTime;
      show_time = 1;
    }
    else if ( layout_mask & pwr_mOpWindLayoutMask_ShowAlarmTime) {
      time_format = time_eFormat_Time;
      show_time = 1;
    }

    for ( i = 0; i < 5; i++) {
      a_exist[i] = info.a_alarm_exist[i];
      a_active[i] = info.a_alarm_active[i];

      if ( info.a_alarm_exist[i]) {

	cnv_pango_text( info.a_alarm_text[i], ctext, sizeof(ctext));

	if ( show_time) {
	  sts = time_AtoAscii( &info.a_alarm_time[i], time_format, timestr, sizeof(timestr));
	  if ( (s = strrchr( timestr, '.')))
	    *s = 0;
	  
	  snprintf( text, sizeof(text), "<span size=\"%d\">%s %s  %s</span>", 
		    fsize, timestr, info.a_alarm_alias[i], ctext);
	}
	else
	  snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
		    fsize, info.a_alarm_alias[i], ctext);
      
	gtk_label_set_markup( GTK_LABEL(aalarm_label[i]), CoWowGtk::convert_utf8(text));
	
	snprintf( text, sizeof(text), "<span size=\"%d\">%s</span>", 
		  fsize, timestr);

	// gtk_widget_modify_bg( aalarm_box[i], GTK_STATE_NORMAL, &red_color);
	if ( i < a_height) {
	  g_object_set( aalarm_box[i], "visible", TRUE, NULL);
	  if ( info.a_alarm_active[i]) {
	    g_object_set( aalarm_active[i], "visible", TRUE, NULL);
	  }
	  else {
	    g_object_set( aalarm_active[i], "visible", FALSE, NULL);
	  }
	}
      }
      else {
	gtk_label_set_text( GTK_LABEL(aalarm_label[i]), "");
	// gtk_widget_modify_bg( aalarm_box[i], GTK_STATE_NORMAL, &bg_color);
	g_object_set( aalarm_active[i], "visible", FALSE, NULL);
	g_object_set( aalarm_box[i], "visible", FALSE, NULL);
      }
    }

    if ( info.b_alarm_exist[0]) {
      GdkColor yellow_color;
      gdk_color_parse( "Yellow", &yellow_color);

      balarm_type = evlist_eEventType_Alarm;
      balarm_prio = mh_eEventPrio_B;

      cnv_pango_text( info.b_alarm_text[0], ctext, sizeof(ctext));

      if ( show_time) {
	sts = time_AtoAscii( &info.b_alarm_time[0], time_format, timestr, sizeof(timestr));
	if ( (s = strrchr( timestr, '.')))
	  *s = 0;

	snprintf( text, sizeof(text), "<span size=\"%d\">%s %s  %s</span>", 
		  fsize, timestr, info.b_alarm_alias[0], ctext);
      }
      else 
	snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
		  fsize, info.b_alarm_alias[0], ctext);

      gtk_label_set_markup( GTK_LABEL(balarm_label), CoWowGtk::convert_utf8(text));
      gtk_widget_modify_bg( balarm_ebox, GTK_STATE_NORMAL, &yellow_color);
      g_object_set( balarm_box, "visible", TRUE, NULL);

      gtk_label_set_text( GTK_LABEL(balarm_mark), "B");

      if ( info.b_alarm_active[0]) {
	g_object_set( balarm_active, "visible", TRUE, NULL);
      }
      else {
	g_object_set( balarm_active, "visible", FALSE, NULL);
      }
    }
    else if ( info.c_alarm_exist[0]) {
      GdkColor blue_color;
      gdk_color_parse( "Lightblue", &blue_color);

      balarm_type = evlist_eEventType_Alarm;
      balarm_prio = mh_eEventPrio_C;
      cnv_pango_text( info.c_alarm_text[0], ctext, sizeof(ctext));
      if ( show_time) {
	sts = time_AtoAscii( &info.c_alarm_time[0], time_format, timestr, sizeof(timestr));
	if ( (s = strrchr( timestr, '.')))
	  *s = 0;

	snprintf( text, sizeof(text), "<span size=\"%d\">%s %s  %s</span>", 
		  fsize, timestr, info.c_alarm_alias[i], ctext);
      }
      else 
	snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
		  fsize, info.c_alarm_alias[0], ctext);
      gtk_label_set_markup( GTK_LABEL(balarm_label), CoWowGtk::convert_utf8(text));
      gtk_widget_modify_bg( balarm_ebox, GTK_STATE_NORMAL, &blue_color);
      g_object_set( balarm_box, "visible", TRUE, NULL);

      gtk_label_set_text( GTK_LABEL(balarm_mark), "C");

      if ( info.b_alarm_active[0]) {
	g_object_set( balarm_active, "visible", TRUE, NULL);
      }
      else {
	g_object_set( balarm_active, "visible", FALSE, NULL);
      }
    }
    else if ( info.d_alarm_exist[0])
    {
      GdkColor violet_color;
      gdk_color_parse( "Violet", &violet_color);

      balarm_type = evlist_eEventType_Alarm;
      balarm_prio = mh_eEventPrio_D;
      cnv_pango_text( info.d_alarm_text[0], ctext, sizeof(ctext));
      if ( show_time) {
	sts = time_AtoAscii( &info.d_alarm_time[0], time_format, timestr, sizeof(timestr));
	if ( (s = strrchr( timestr, '.')))
	  *s = 0;

	snprintf( text, sizeof(text), "<span size=\"%d\">%s %s  %s</span>", 
		  fsize, timestr, info.d_alarm_alias[0], ctext);
      }
      else 
	snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
		  fsize, info.d_alarm_alias[0], ctext);
      gtk_label_set_markup( GTK_LABEL(balarm_label), CoWowGtk::convert_utf8(text));
      gtk_widget_modify_bg( balarm_ebox, GTK_STATE_NORMAL, &violet_color);
      g_object_set( balarm_box, "visible", TRUE, NULL);

      gtk_label_set_text( GTK_LABEL(balarm_mark), "D");

      if ( info.b_alarm_active[0]) {
	g_object_set( balarm_active, "visible", TRUE, NULL);
      }
      else {
	g_object_set( balarm_active, "visible", FALSE, NULL);
      }
    }
    else if ( info.i_alarm_exist[0])
    {
      GdkColor green_color;
      gdk_color_parse( "Green", &green_color);

      balarm_type = evlist_eEventType_Info;
      cnv_pango_text( info.i_alarm_text[0], ctext, sizeof(ctext));
      if ( show_time) {
	sts = time_AtoAscii( &info.i_alarm_time[0], time_format, timestr, sizeof(timestr));
	if ( (s = strrchr( timestr, '.')))
	  *s = 0;

	snprintf( text, sizeof(text), "<span size=\"%d\">%s %s  %s</span>", 
		  fsize, timestr, info.i_alarm_alias[0], ctext);
      }
      else 
      snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
	       fsize, info.i_alarm_alias[0], ctext);
      gtk_label_set_markup( GTK_LABEL(balarm_label), CoWowGtk::convert_utf8(text));
      gtk_widget_modify_bg( balarm_ebox, GTK_STATE_NORMAL, &green_color);
      g_object_set( balarm_box, "visible", TRUE, NULL);

      gtk_label_set_text( GTK_LABEL(balarm_mark), "I");

      if ( info.b_alarm_active[0]) {
	g_object_set( balarm_active, "visible", TRUE, NULL);
      }
      else {
	g_object_set( balarm_active, "visible", FALSE, NULL);
      }
    }
    else {
      gtk_label_set_text( GTK_LABEL(balarm_label), "");
      gtk_widget_modify_bg( balarm_ebox, GTK_STATE_NORMAL, &bg_color);
      gtk_label_set_text( GTK_LABEL(balarm_mark), "");
      g_object_set( balarm_box, "visible", FALSE, NULL);

      g_object_set( balarm_active, "visible", FALSE, NULL);
    }
  }
}

int OpGtk::configure( char *opplace_str)
{
  int 		sts;
  unsigned int 	i;
  pwr_tObjid 	opplace;
  pwr_sClass_OpPlace *opplace_p;
  pwr_sAttrRef	attrref;
  pwr_tTid	tid;

  sts = gdh_NameToObjid( opplace_str, &opplace);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( opplace, (void **) &opplace_p);
  if ( EVEN(sts)) return sts;

  layout_mask = opplace_p->OpWindLayout;
  if ( opplace_p->StartJavaProcess)
    start_jop = 1;


  // Examine Graph objects
  for ( i = 0; i < sizeof(opplace_p->FastAvail)/sizeof(opplace_p->FastAvail[0]); i++) {

    button_aref[i].Objid = pwr_cNOid;
    if ( cdh_ObjidIsNull( opplace_p->FastAvail[i].Objid))
      continue;

    sts = gdh_GetAttrRefTid( &opplace_p->FastAvail[i], &tid);
    if ( EVEN(sts))continue;

    if ( tid != pwr_cClass_XttGraph)
      continue;

     memset( &attrref, 0, sizeof(attrref));
    sts = gdh_ClassAttrToAttrref( pwr_cClass_XttGraph, ".ButtonText", &attrref);
    if ( EVEN(sts)) return sts;

    attrref = cdh_ArefAdd( &opplace_p->FastAvail[i], &attrref);
    sts = gdh_GetObjectInfoAttrref( &attrref, (void *)button_title[i], 
		sizeof(button_title[0]));
    if ( EVEN(sts)) continue;

    if ( strcmp( button_title[i], "") == 0) {
      // Take object name instead
      sts = gdh_AttrrefToName( &opplace_p->FastAvail[i], button_title[i], sizeof(button_title[0]), 
			       cdh_mName_object);
      if ( EVEN(sts)) continue;
    }

    button_aref[i] = opplace_p->FastAvail[i];
  }

  // Create the application buttons
  GtkWidget *b[25];
  for ( i = 0; i < sizeof(opplace_p->FastAvail)/sizeof(opplace_p->FastAvail[0]); i++) {
    if ( cdh_ObjidIsNull( button_aref[i].Objid))
      continue;

    char *textutf8 = g_convert( button_title[i], -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
    b[i] = gtk_button_new_with_label(textutf8);
    g_free( textutf8);
    gtk_widget_set_size_request( b[i], -1, 28);
    switch ( i) {
    case 0:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl1), this);
      break;
    case 1:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl2), this);
      break;
    case 2:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl3), this);
      break;
    case 3:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl4), this);
      break;
    case 4:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl5), this);
      break;
    case 5:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl6), this);
      break;
    case 6:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl7), this);
      break;
    case 7:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl8), this);
      break;
    case 8:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl9), this);
      break;
    case 9:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl10), this);
      break;
    case 10:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl11), this);
      break;
    case 11:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl12), this);
      break;
    case 12:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl13), this);
      break;
    case 13:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl14), this);
      break;
    case 14:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl15), this);
      break;
    case 15:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl16), this);
      break;
    case 16:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl17), this);
      break;
    case 17:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl18), this);
      break;
    case 18:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl19), this);
      break;
    case 19:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl20), this);
      break;
    case 20:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl21), this);
      break;
    case 21:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl22), this);
      break;
    case 22:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl23), this);
      break;
    case 23:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl24), this);
      break;
    case 24:
      g_signal_connect( b[i], "clicked", G_CALLBACK(activate_appl25), this);
      break;
    }
  }

  appl_form = gtk_vbox_new( FALSE, 0);
  for ( i = 0; i < sizeof(opplace_p->FastAvail)/sizeof(opplace_p->FastAvail[0]); i++) {
    if ( i == 0) {
      funcbox[0] = gtk_hbox_new( FALSE, 0);
      gtk_box_pack_start( GTK_BOX(appl_form), funcbox[0], FALSE, FALSE, 0);
    }
    else if ( i == 4) {      
      funcbox[1] = gtk_hbox_new( FALSE, 0);
      gtk_box_pack_start( GTK_BOX(appl_form), funcbox[1], FALSE, FALSE, 0);
    }
    else if ( i == 9) {
      funcbox[2] = gtk_hbox_new( FALSE, 0);
      gtk_box_pack_start( GTK_BOX(appl_form), funcbox[2], FALSE, FALSE, 0);
    }
    else if ( i == 14) {
      funcbox[3] = gtk_hbox_new( FALSE, 0);
      gtk_box_pack_start( GTK_BOX(appl_form), funcbox[3], FALSE, FALSE, 0);
    }
    else if ( i == 19) {
      funcbox[4] = gtk_hbox_new( FALSE, 0);
      gtk_box_pack_start( GTK_BOX(appl_form), funcbox[4], FALSE, FALSE, 0);
    }

    if ( i < 5) {
      if ( cdh_ObjidIsNotNull( button_aref[i].Objid))
	gtk_box_pack_start( GTK_BOX(funcbox[0]), b[i], TRUE, TRUE, 0);
    }
    else if ( i < 10) {
      if ( cdh_ObjidIsNotNull( button_aref[i].Objid))
	gtk_box_pack_start( GTK_BOX(funcbox[1]), b[i], TRUE, TRUE, 0);
    }
    else if ( i < 15) {
      if ( cdh_ObjidIsNotNull( button_aref[i].Objid))
	gtk_box_pack_start( GTK_BOX(funcbox[2]), b[i], TRUE, TRUE, 0);
    }
    else if ( i < 20) {
      if ( cdh_ObjidIsNotNull( button_aref[i].Objid)) {
	gtk_box_pack_start( GTK_BOX(funcbox[3]), b[i], TRUE, TRUE, 0);
	a_height = 4;
      }
    }
    else if ( i < 25) {
      if ( cdh_ObjidIsNotNull( button_aref[i].Objid)) {
	gtk_box_pack_start( GTK_BOX(funcbox[4]), b[i], TRUE, TRUE, 0);
	a_height = 5;
      }
    }
  }

  return XNAV__SUCCESS;
}

void OpGtk::activate_exit( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_exit();
}

void OpGtk::activate_aalarm_ack( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_aalarm_ack();
}

void OpGtk::activate_balarm_ack( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_balarm_ack();
}

void OpGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  OpGtk *op = (OpGtk *)data;

  if ( op->text_size >= 18)
    return;

  op->text_size += 2;
  op->update_alarm_info();
}

void OpGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  OpGtk *op = (OpGtk *)data;

  if ( op->text_size <= 8)
    return;

  op->text_size -= 2;
  op->update_alarm_info();
}

void OpGtk::activate_aalarm_incr( GtkWidget *w, gpointer data)
{
  OpGtk *op = (OpGtk *)data;
  int width, height;

  gtk_window_get_size( GTK_WINDOW(op->toplevel), &width, &height);

  if ( op->a_height == 5)
    return;

  op->a_height++;
  height = OP_HEIGHT_MIN + (op->a_height - 2) * OP_HEIGHT_INC +
    ((op->layout_mask & pwr_mOpWindLayoutMask_HideStatusBar) ? 0 : 1) * OP_HEIGHT_STATUSBAR;

  if ( op->a_height == 3) {
    g_object_set( op->decr_button, "visible", TRUE, NULL);
  }
  else if ( op->a_height == 4) {
    g_object_set( op->funcbox[3], "visible", TRUE, NULL);
  }
  else if ( op->a_height == 5) {
    g_object_set( op->funcbox[4], "visible", TRUE, NULL);
  }
  for ( int i = 2; i < 5; i++) {
    if ( i < op->a_height) {
      if ( op->a_exist[i]) {
	g_object_set( op->aalarm_box[i], "visible", TRUE, NULL);
	if ( op->a_active[i]) {
	  g_object_set( op->aalarm_active[i], "visible", TRUE, NULL);
	}
	else {
	  g_object_set( op->aalarm_active[i], "visible", FALSE, NULL);
	}
      }
    }
  }
  gtk_window_resize( GTK_WINDOW(op->toplevel), width, height);
}

void OpGtk::activate_aalarm_decr( GtkWidget *w, gpointer data)
{
  OpGtk *op = (OpGtk *)data;
  int width, height;

  gtk_window_get_size( GTK_WINDOW(op->toplevel), &width, &height);

  if ( op->a_height == 2)
    return;

  op->a_height--;
  height = OP_HEIGHT_MIN + (op->a_height - 2) * OP_HEIGHT_INC + 
    ((op->layout_mask & pwr_mOpWindLayoutMask_HideStatusBar) ? 0 : 1) * OP_HEIGHT_STATUSBAR;

  if ( op->a_height == 2) {
    g_object_set( op->decr_button, "visible", FALSE, NULL);
  }
  else if ( op->a_height == 3) {
    g_object_set( op->funcbox[3], "visible", FALSE, NULL);
  }
  else if ( op->a_height == 4) {
    g_object_set( op->funcbox[4], "visible", FALSE, NULL);
  }
  for ( int i = 2; i < 5; i++) {
    if ( i >= op->a_height) {
      if ( op->a_exist[i]) {
	g_object_set( op->aalarm_box[i], "visible", FALSE, NULL);
	g_object_set( op->aalarm_active[i], "visible", FALSE, NULL);
      }
    }
  }
  gtk_window_resize( GTK_WINDOW(op->toplevel), width, height);
}

void OpGtk::activate_alarmlist( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_alarmlist();
}

void OpGtk::activate_eventlist( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_eventlist();
}

void OpGtk::activate_eventlog( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_eventlog();
}

void OpGtk::activate_blocklist( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_blocklist();
}

void OpGtk::activate_navigator( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_navigator();
}

void OpGtk::activate_help( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_help();
}

void OpGtk::activate_help_overview( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_help_overview();
}

void OpGtk::activate_help_opwin( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_help_opwin();
}

void OpGtk::activate_help_proview( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_help_proview();
}

void OpGtk::activate_trend( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_trend();
}

void OpGtk::activate_fast( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_fast();
}

void OpGtk::activate_history( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_history();
}

void OpGtk::activate_graph( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_graph();
}

void OpGtk::activate_switch_user( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_switch_user();
}

void OpGtk::activate_show_user( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_show_user();
}

void OpGtk::activate_logout( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_logout();
}

void OpGtk::activate_cmd_menu_item( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;
  pwr_tCmd cmd;
  int sts;

  sts = ((OpGtk *)op)->get_cmd( w, cmd);
  if ( ODD(sts)) {
    op->activate_cmd_menu_item( cmd);
  }
}

void OpGtk::activate_sup_node( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->activate_sup_node( (void *)w);
}

void OpGtk::activate_appl1( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(0);
}

void OpGtk::activate_appl2( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(1);
}

void OpGtk::activate_appl3( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(2);
}

void OpGtk::activate_appl4( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(3);
}

void OpGtk::activate_appl5( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(4);
}

void OpGtk::activate_appl6( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(5);
}

void OpGtk::activate_appl7( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(6);
}

void OpGtk::activate_appl8( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(7);
}

void OpGtk::activate_appl9( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(8);
}

void OpGtk::activate_appl10( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(9);
}

void OpGtk::activate_appl11( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(10);
}

void OpGtk::activate_appl12( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(11);
}

void OpGtk::activate_appl13( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(12);
}

void OpGtk::activate_appl14( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(13);
}

void OpGtk::activate_appl15( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(14);
}

void OpGtk::activate_appl16( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(15);
}

void OpGtk::activate_appl17( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(16);
}

void OpGtk::activate_appl18( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(17);
}

void OpGtk::activate_appl19( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(18);
}

void OpGtk::activate_appl20( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(19);
}

void OpGtk::activate_appl21( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(20);
}

void OpGtk::activate_appl22( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(21);
}

void OpGtk::activate_appl23( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(22);
}

void OpGtk::activate_appl24( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(23);
}

void OpGtk::activate_appl25( GtkWidget *w, gpointer data)
{
  Op *op = (Op*)data;

  op->appl_action(24);
}

int OpGtk::get_cmd( GtkWidget *w, char *cmd)
{
  for ( unsigned int i = 0; i < cmd_vect.size(); i++) {
    if ( w == cmd_vect[i].w) {
      strcpy( cmd, cmd_vect[i].cmd);
      return 1;	
    }
  }
  return 0;
}

void OpGtk::change_sup_color( void *imagew, op_eSupColor color)
{
  GtkWidget *image = (GtkWidget *)imagew;
  pwr_tFileName fname;

  switch ( color) {
  case op_eSupColor_Gray:
    dcli_translate_filename( fname, "$pwr_exe/xtt_ind_gray.png");
    break;
  case op_eSupColor_Green:
    dcli_translate_filename( fname, "$pwr_exe/xtt_ind_green.png");
    break;
  case op_eSupColor_Yellow:
    dcli_translate_filename( fname, "$pwr_exe/xtt_ind_yellow.png");
    break;
  case op_eSupColor_Red:
    dcli_translate_filename( fname, "$pwr_exe/xtt_ind_red.png");
    break;
  case op_eSupColor_Black:
    dcli_translate_filename( fname, "$pwr_exe/xtt_ind_black.png");
    break;
  default:
    dcli_translate_filename( fname, "$pwr_exe/xtt_ind_gray.png");
  }
  gtk_image_set_from_file( GTK_IMAGE(image), fname);
}

void  OpGtk::set_title( char *user) 
{
  char title[80];
  char text[120];
  pwr_tStatus sts;
  char nodename[32];

  if ( !title_label)
    return;

  syi_NodeName( &sts, nodename, sizeof(nodename));

  strcpy( title, CoWowGtk::convert_utf8(user));
  strcat( title, CoWowGtk::translate_utf8(" on "));
  strcat( title, CoWowGtk::convert_utf8(nodename));

  snprintf( text, sizeof(text), "<span size=\"%d\">%s</span>", 14 * 1024, title);
  gtk_label_set_markup( GTK_LABEL(title_label), text);
}
