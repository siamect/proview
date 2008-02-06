/* 
 * Proview   $Id: xtt_op_gtk.cpp,v 1.9 2008-02-06 09:08:33 claes Exp $
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




OpGtk::OpGtk( void *op_parent_ctx,
	      GtkWidget *op_parent_wid,
	      char *opplace,
	      pwr_tStatus *status) :
  Op( op_parent_ctx, opplace, status), parent_wid(op_parent_wid), a_height(2),
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

  // Toolbar
  GtkWidget *tools = (GtkWidget *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  GtkWidget *tools_zoom_in = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(activate_zoom_in), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_zoom_in, "Zoom in", "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(activate_zoom_out), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_zoom_out, "Zoom out", "");

  GtkWidget *tools_help = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_help), 
	  gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_help, "clicked", G_CALLBACK(activate_help), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_help, "Help", "");

  GtkWidget *tools_navigator = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_navigator.png");
  gtk_container_add( GTK_CONTAINER( tools_navigator), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_navigator, "clicked", G_CALLBACK(activate_navigator), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_navigator, "Open Navigator", "");

  tools_close = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_close), 
	  gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_close, "clicked", G_CALLBACK(activate_exit), this);
  gtk_toolbar_append_widget( GTK_TOOLBAR(tools), tools_close, "Close", "");

  // System pushbuttons
  GtkWidget *alarmlist_button = gtk_button_new_with_label("Alarmlist");
  gtk_widget_set_size_request( alarmlist_button, 90, 25);
  g_signal_connect( alarmlist_button, "clicked", G_CALLBACK(activate_alarmlist), this);

  GtkWidget *eventlist_button = gtk_button_new_with_label("Eventlist");
  gtk_widget_set_size_request( eventlist_button, 90, 25);
  g_signal_connect( eventlist_button, "clicked", G_CALLBACK(activate_eventlist), this);

  eventlog_button = gtk_button_new_with_label("Eventlog");
  gtk_widget_set_size_request( eventlog_button, 90, 25);
  g_signal_connect( eventlog_button, "clicked", G_CALLBACK(activate_eventlog), this);

  help_button = gtk_button_new_with_label("Help");
  gtk_widget_set_size_request( help_button, 90, 25);
  g_signal_connect( help_button, "clicked", G_CALLBACK(activate_help), this);

  GtkWidget *sysbutton_box = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sysbutton_box), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sysbutton_box), alarmlist_button, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sysbutton_box), eventlist_button, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sysbutton_box), eventlog_button, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(sysbutton_box), help_button, FALSE, FALSE, 0);
  gtk_widget_set_size_request( sysbutton_box, 150, -1);

  // Main window
  configure( opplace);

  GtkWidget *hbox_conf = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_conf), sysbutton_box, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox_conf), gtk_hseparator_new(), FALSE, FALSE, 2);
  gtk_box_pack_start( GTK_BOX(hbox_conf), appl_form, TRUE, TRUE, 0);

  GtkWidget *pane = gtk_hpaned_new();
  gtk_paned_pack1( GTK_PANED(pane), vbox_ala, TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(pane), hbox_conf, FALSE, TRUE);

  GtkWidget *frame = gtk_frame_new( NULL);
  GdkColor background;
  gdk_color_parse( "black", &background);
  gtk_widget_modify_bg( frame, GTK_STATE_NORMAL, &background);
  
  gtk_container_add( GTK_CONTAINER(frame), pane);
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
  g_object_set( help_button, "visible", FALSE, NULL);
  g_object_set( eventlog_button, "visible", FALSE, NULL);
  g_object_set( decr_button, "visible", FALSE, NULL);
  g_object_set( tools_close, "visible", FALSE, NULL);

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
  wow->DisplayWarranty();

  *status = 1;
}


//
//  Delete op
//
OpGtk::~OpGtk()
{
  if ( jop)
    delete jop;
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
}

void  OpGtk::update_alarm_info()
{
  evlist_sAlarmInfo info;
  int 		sts;
  int		i;
  int		height, active_height;
  char          str[40];
  char		text[500];
  int 		fsize = text_size * 1024;

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

    for ( i = 0; i < 5; i++) {
      a_exist[i] = info.a_alarm_exist[i];
      a_active[i] = info.a_alarm_active[i];
      if ( info.a_alarm_exist[i]) {
	
	snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
		 fsize, info.a_alarm_alias[i], info.a_alarm_text[i]);
	gtk_label_set_markup( GTK_LABEL(aalarm_label[i]), CoWowGtk::convert_utf8(text));
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
      snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
	       fsize, info.b_alarm_alias[0], info.b_alarm_text[0]);
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
      snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
	       fsize, info.c_alarm_alias[0], info.c_alarm_text[0]);
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
      snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
	       fsize, info.d_alarm_alias[0], info.d_alarm_text[0]);
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
      snprintf( text, sizeof(text), "<span size=\"%d\">%s  %s</span>", 
	       fsize, info.i_alarm_alias[0], info.i_alarm_text[0]);
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
  int		i;
  pwr_tObjid 	opplace;
  pwr_tObjid 	user;
  pwr_sClass_OpPlace *opplace_p;
  pwr_sClass_User *user_p;
  pwr_sAttrRef	attrref;

  sts = gdh_NameToObjid( opplace_str, &opplace);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( opplace, (void **) &opplace_p);
  if ( EVEN(sts)) return sts;

  // Fix 
  if ( strncmp( opplace_p->OpWinProgram, "Jop", 3) == 0)
    start_jop = 1;

  // Find matching user object
  sts = gdh_GetClassList( pwr_cClass_User, &user);
  while ( ODD (sts)) {

    sts = gdh_ObjidToPointer( user, (void **) &user_p);
    if ( EVEN(sts)) return sts;

    if ( user_p->OpNumber == opplace_p->OpNumber)
      break;
    sts = gdh_GetNextObject( user, &user);
  }
  if ( EVEN(sts)) return sts;

  // Examine Graph objects
  button_cnt = user_p->NoFastAvail;
  if ( button_cnt > 15)
    button_cnt = 15;
  for ( i = 0; i < button_cnt; i++) {
    if ( i >= 15)
      break;
    memset( &attrref, 0, sizeof(attrref));
    sts = gdh_ClassAttrToAttrref( pwr_cClass_XttGraph, ".ButtonText", &attrref);
    if ( EVEN(sts)) return sts;

    attrref = cdh_ArefAdd( &user_p->FastAvail[i], &attrref);
    sts = gdh_GetObjectInfoAttrref( &attrref, (void *)button_title[i], 
		sizeof(button_title[0]));
    if ( EVEN(sts)) 
      strcpy( button_title[i], "");

    button_objid[i] = attrref.Objid;
  }

  // Create the application buttons
  GtkWidget *b[15];
  for ( i = 0; i < button_cnt; i++) {
    char *textutf8 = g_convert( button_title[i], -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
    b[i] = gtk_button_new_with_label(textutf8);
    g_free( textutf8);
    gtk_widget_set_size_request( b[i], -1, 25);
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
    }
  }

  appl_form = gtk_vbox_new( FALSE, 0);
  GtkWidget *bbox[3];
  for ( i = 0; i < button_cnt; i++) {
    if ( i == 0) {
      bbox[0] = gtk_hbox_new( FALSE, 0);
      gtk_box_pack_start( GTK_BOX(appl_form), bbox[0], FALSE, FALSE, 0);
    }
    else if ( i == 4) {      
      bbox[1] = gtk_hbox_new( FALSE, 0);
      gtk_box_pack_start( GTK_BOX(appl_form), bbox[1], FALSE, FALSE, 0);
    }
    else if ( i == 9) {
      bbox[2] = gtk_hbox_new( FALSE, 0);
      gtk_box_pack_start( GTK_BOX(appl_form), bbox[2], FALSE, FALSE, 0);
    }

    if ( i < 5)
      gtk_box_pack_start( GTK_BOX(bbox[0]), b[i], TRUE, TRUE, 0);
    else if ( i < 10)
      gtk_box_pack_start( GTK_BOX(bbox[1]), b[i], TRUE, TRUE, 0);
    else if ( i < 15)
      gtk_box_pack_start( GTK_BOX(bbox[2]), b[i], TRUE, TRUE, 0);
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
  height = OP_HEIGHT_MIN + (op->a_height - 2) * OP_HEIGHT_INC;

  if ( op->a_height == 3) {
    g_object_set( op->decr_button, "visible", TRUE, NULL);
  }
  else if ( op->a_height == 4) {
    g_object_set( op->eventlog_button, "visible", TRUE, NULL);
  }
  else if ( op->a_height == 5) {
    g_object_set( op->help_button, "visible", TRUE, NULL);
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
  height = OP_HEIGHT_MIN + (op->a_height - 2) * OP_HEIGHT_INC;

  if ( op->a_height == 2) {
    g_object_set( op->decr_button, "visible", FALSE, NULL);
  }
  else if ( op->a_height == 3) {
    g_object_set( op->eventlog_button, "visible", FALSE, NULL);
  }
  else if ( op->a_height == 4) {
    g_object_set( op->help_button, "visible", FALSE, NULL);
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


