/* 
 * Proview   $Id: xtt_ev_gtk.cpp,v 1.3 2008-01-08 14:25:50 claes Exp $
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

/* xtt_ev_gtk.cpp -- Alarm and event window in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_wow_gtk.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "rt_mh_util.h"
#include "co_lng.h"
#include "xtt_evlist_gtk.h"
#include "xtt_ev_gtk.h"
#include "rt_xnav_msg.h"

static gint eve_delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  EvGtk::eve_activate_exit( w, data);
  return TRUE;
}

static void eve_destroy_event( GtkWidget *w, gpointer data)
{
}

static gint ala_delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  EvGtk::ala_activate_exit( w, data);
  return TRUE;
}

static void ala_destroy_event( GtkWidget *w, gpointer data)
{
}

static gint blk_delete_event( GtkWidget *w, GdkEvent *event, gpointer data)
{
  EvGtk::blk_activate_exit( w, data);
  return TRUE;
}

static void blk_destroy_event( GtkWidget *w, gpointer data)
{
}

EvGtk::EvGtk( void *ev_parent_ctx,
	      GtkWidget	*ev_parent_wid,
	      char *eve_name,
	      char *ala_name,
	      char *blk_name,
	      pwr_tObjid ev_user,
	      int display_ala,
	      int display_eve,
	      int display_blk,
	      int display_return,
	      int display_ack,
	      int ev_beep,
	      pwr_tMask ev_pop_mask,
	      pwr_tStatus *status) :
  Ev( ev_parent_ctx, eve_name, ala_name, blk_name, ev_user, display_ala, display_eve,
      display_blk, display_return, display_ack, ev_beep, ev_pop_mask, status),
  parent_wid(ev_parent_wid), parent_wid_eve(NULL), parent_wid_ala(NULL)
{
  pwr_tStatus sts;
  pwr_sClass_OpPlace *userobject_ptr;
  const int eve_width = 700;
  const int eve_height = 600;
  const int ala_width = 700;
  const int ala_height = 300;
  const int blk_width = 700;
  const int blk_height = 300;

  *status = 1;

  // Check user object
  if ( cdh_ObjidIsNull( user)) {
    *status = XNAV__NOUSER;
    return;
  }

  sts = gdh_ObjidToPointer( user, (pwr_tAddress *) &userobject_ptr);
  if ( EVEN(sts)) {
    *status = XNAV__NOUSER;
    return;
  }
  ala_size = userobject_ptr->MaxNoOfAlarms;
  eve_size = userobject_ptr->MaxNoOfEvents;
  blk_size = 0;
  create_aliaslist( userobject_ptr);

  // Gtk
  // Eve Window
  {
    char *titleutf8 = g_convert( eve_name, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);

    parent_wid_eve = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
						 "default-height", eve_height,
						 "default-width", eve_width,
						 "title", titleutf8,
						 NULL);
    g_free( titleutf8);

    g_signal_connect( parent_wid_eve, "delete_event", G_CALLBACK(eve_delete_event), this);
    g_signal_connect( parent_wid_eve, "destroy", G_CALLBACK(eve_destroy_event), this);
    g_signal_connect( parent_wid_eve, "focus-in-event", G_CALLBACK(eve_action_inputfocus), this);

    CoWowGtk::SetWindowIcon( parent_wid_eve);

    GtkWidget *eve_vbox = gtk_vbox_new( FALSE, 0);

    // Menu
    // Accelerators
    GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
    gtk_window_add_accel_group(GTK_WINDOW(parent_wid_eve), accel_g);

    GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

    // File entry
    GtkWidget *file_print = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Print"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_print), 
				   gtk_image_new_from_stock( "gtk-print", GTK_ICON_SIZE_MENU));
    g_signal_connect(file_print, "activate", G_CALLBACK(eve_activate_print), this);
    
    GtkWidget *file_close = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Close"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_close), 
				   gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_MENU));
    g_signal_connect(file_close, "activate", G_CALLBACK(eve_activate_exit), this);
    gtk_widget_add_accelerator( file_close, "activate", accel_g,
				'w', GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

    GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

    GtkWidget *file = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_File"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

    // Functions entry
    GtkWidget *functions_ack_last = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Acknowledge"));
    g_signal_connect( functions_ack_last, "activate", 
		      G_CALLBACK(eve_activate_ack_last), this);
    gtk_widget_add_accelerator( functions_ack_last, "activate", accel_g,
				'k', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *functions_open_plc = gtk_menu_item_new_with_mnemonic( 
                        CoWowGtk::translate_utf8("Open _Program"));
    g_signal_connect( functions_open_plc, "activate", 
		      G_CALLBACK(eve_activate_open_plc), this);
    gtk_widget_add_accelerator( functions_open_plc, "activate", accel_g,
				'l', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *functions_display_object = gtk_menu_item_new_with_mnemonic( 
                    CoWowGtk::translate_utf8("_Display object in Navigator"));
    g_signal_connect( functions_display_object, "activate", 
		      G_CALLBACK(eve_activate_display_in_xnav), this);
    gtk_widget_add_accelerator( functions_display_object, "activate", accel_g,
				'd', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkMenu *func_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_ack_last);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_open_plc);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_display_object);

    GtkWidget *functions = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Functions"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(func_menu));

    // View entry
    GtkWidget *view_zoom_in = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _In"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_in), 
				   gtk_image_new_from_stock( "gtk-zoom-in", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_in, "activate", 
		      G_CALLBACK(eve_activate_zoom_in), this);
    gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
				'i', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *view_zoom_out = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Out"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_out), 
				   gtk_image_new_from_stock( "gtk-zoom-out", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_out, "activate", 
		      G_CALLBACK(eve_activate_zoom_out), this);
    gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
				'o', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);
    
    GtkWidget *view_zoom_reset = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Reset"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_reset), 
				   gtk_image_new_from_stock( "gtk-zoom-100", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_reset, "activate", 
		      G_CALLBACK(eve_activate_zoom_reset), this);
    gtk_widget_add_accelerator( view_zoom_reset, "activate", accel_g,
				'b', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *view_disp_hundredth = gtk_check_menu_item_new_with_mnemonic( 
					 CoWowGtk::translate_utf8("_Display hundredth"));
    g_signal_connect( view_disp_hundredth, "activate", 
		      G_CALLBACK(eve_activate_disp_hundredth), this);

    GtkWidget *view_hide_object = gtk_check_menu_item_new_with_mnemonic( 
				    CoWowGtk::translate_utf8("_Hide Event Name"));
    g_signal_connect( view_hide_object, "activate", 
		      G_CALLBACK(eve_activate_hide_object), this);

    GtkWidget *view_hide_text = gtk_check_menu_item_new_with_mnemonic(
                      CoWowGtk::translate_utf8("Hide _Event Text"));
    g_signal_connect( view_hide_text, "activate", 
		      G_CALLBACK(eve_activate_hide_text), this);

    GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_disp_hundredth);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_hide_object);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_hide_text);

    GtkWidget *view = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_View"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

    // Help entry
    GtkWidget *help_help = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Help"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_help), 
				   gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
    g_signal_connect(help_help, "activate", G_CALLBACK(eve_activate_help), this);
    gtk_widget_add_accelerator( help_help, "activate", accel_g,
				'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    GtkWidget *help_helpevent = gtk_menu_item_new_with_mnemonic( 
                CoWowGtk::translate_utf8("Help Selected Event"));
    g_signal_connect( help_helpevent, "activate", 
		      G_CALLBACK(eve_activate_helpevent), this);

    GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_helpevent);

    GtkWidget *help = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Help"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));


    // Create eve evlist
    eve = new EvListGtk( this, eve_vbox, ev_eType_EventList, eve_size, &eve_widget);
    eve->start_trace_cb = &eve_start_trace_cb;
    eve->display_in_xnav_cb = &eve_display_in_xnav_cb;
    eve->name_to_alias_cb = &ev_name_to_alias_cb;
    eve->popup_menu_cb = &ev_popup_menu_cb;

    gtk_box_pack_start( GTK_BOX(eve_vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
    gtk_box_pack_end( GTK_BOX(eve_vbox), GTK_WIDGET(eve_widget), TRUE, TRUE, 0);

    gtk_container_add( GTK_CONTAINER(parent_wid_eve), eve_vbox);
    // gtk_widget_show_all( parent_wid_eve);
  }

  // Ala Window
  {
    char *titleutf8 = g_convert( ala_name, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);

    parent_wid_ala = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
						 "default-height", ala_height,
						 "default-width", ala_width,
						 "title", titleutf8,
						 NULL);
    g_free( titleutf8);

    g_signal_connect( parent_wid_ala, "delete_event", G_CALLBACK(ala_delete_event), this);
    g_signal_connect( parent_wid_ala, "destroy", G_CALLBACK(ala_destroy_event), this);
    g_signal_connect( parent_wid_ala, "focus-in-event", G_CALLBACK(ala_action_inputfocus), this);

    CoWowGtk::SetWindowIcon( parent_wid_ala);

    GtkWidget *ala_vbox = gtk_vbox_new( FALSE, 0);

    // Menu
    // Accelerators
    GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
    gtk_window_add_accel_group(GTK_WINDOW(parent_wid_ala), accel_g);

    GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

    // File entry
    GtkWidget *file_print = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Print"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_print), 
				   gtk_image_new_from_stock( "gtk-print", GTK_ICON_SIZE_MENU));
    g_signal_connect(file_print, "activate", G_CALLBACK(ala_activate_print), this);

    GtkWidget *file_close = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Close"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_close), 
				   gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_MENU));
    g_signal_connect(file_close, "activate", G_CALLBACK(ala_activate_exit), this);
    gtk_widget_add_accelerator( file_close, "activate", accel_g,
				'w', GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

    GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

    GtkWidget *file = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_File"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

    // Functions entry
    GtkWidget *functions_ack_last = gtk_menu_item_new_with_mnemonic( 
                          CoWowGtk::translate_utf8("_Acknowledge"));
    g_signal_connect( functions_ack_last, "activate", 
		      G_CALLBACK(ala_activate_ack_last), this);
    gtk_widget_add_accelerator( functions_ack_last, "activate", accel_g,
				'k', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *functions_open_plc = gtk_menu_item_new_with_mnemonic( 
                      CoWowGtk::translate_utf8("Open _Program"));
    g_signal_connect( functions_open_plc, "activate", 
		      G_CALLBACK(ala_activate_open_plc), this);
    gtk_widget_add_accelerator( functions_open_plc, "activate", accel_g,
				'l', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *functions_display_object = gtk_menu_item_new_with_mnemonic( 
              CoWowGtk::translate_utf8("_Display object in Navigator"));
    g_signal_connect( functions_display_object, "activate", 
		      G_CALLBACK(ala_activate_display_in_xnav), this);
    gtk_widget_add_accelerator( functions_display_object, "activate", accel_g,
				'd', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkMenu *func_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_ack_last);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_open_plc);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_display_object);

    GtkWidget *functions = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Functions"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(func_menu));

    // View entry
    GtkWidget *view_zoom_in = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _In"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_in), 
				   gtk_image_new_from_stock( "gtk-zoom-in", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_in, "activate", 
		      G_CALLBACK(ala_activate_zoom_in), this);
    gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
				'i', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *view_zoom_out = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Out"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_out), 
				   gtk_image_new_from_stock( "gtk-zoom-out", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_out, "activate", 
		      G_CALLBACK(ala_activate_zoom_out), this);
    gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
				'o', GdkModifierType(GDK_CONTROL_MASK),
				GTK_ACCEL_VISIBLE);

    GtkWidget *view_zoom_reset = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Reset"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_reset), 
				   gtk_image_new_from_stock( "gtk-zoom-100", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_reset, "activate", 
		      G_CALLBACK(ala_activate_zoom_reset), this);
    gtk_widget_add_accelerator( view_zoom_reset, "activate", accel_g,
				'b', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *view_disp_hundredth = gtk_check_menu_item_new_with_mnemonic( 
                            CoWowGtk::translate_utf8("_Display hundredth"));
    g_signal_connect( view_disp_hundredth, "activate", 
		      G_CALLBACK(ala_activate_disp_hundredth), this);

    GtkWidget *view_hide_object = gtk_check_menu_item_new_with_mnemonic( 
                             CoWowGtk::translate_utf8("_Hide Event Name"));
    g_signal_connect( view_hide_object, "activate", 
		      G_CALLBACK(ala_activate_hide_object), this);

    GtkWidget *view_hide_text = gtk_check_menu_item_new_with_mnemonic( 
                              CoWowGtk::translate_utf8("Hide _Event Text"));
    g_signal_connect( view_hide_text, "activate", 
		      G_CALLBACK(ala_activate_hide_text), this);

    GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_disp_hundredth);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_hide_object);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_hide_text);

    GtkWidget *view = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_View"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

    // Help entry
    GtkWidget *help_help = gtk_image_menu_item_new_with_mnemonic( 
                              CoWowGtk::translate_utf8("_Help"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_help), 
				   gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
    g_signal_connect(help_help, "activate", G_CALLBACK(ala_activate_help), this);
    gtk_widget_add_accelerator( help_help, "activate", accel_g,
				'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    GtkWidget *help_helpevent = gtk_menu_item_new_with_mnemonic( 
                                         CoWowGtk::translate_utf8("Help Selected Event"));
    g_signal_connect( help_helpevent, "activate", 
		      G_CALLBACK(ala_activate_helpevent), this);

    GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_helpevent);

    GtkWidget *help = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Help"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));


    // Create ala evlist
    ala = new EvListGtk( this, ala_vbox, ev_eType_AlarmList, ala_size, &ala_widget);
    ala->start_trace_cb = &ala_start_trace_cb;
    ala->display_in_xnav_cb = &ala_display_in_xnav_cb;
    ala->name_to_alias_cb = &ev_name_to_alias_cb;
    ala->popup_menu_cb = &ev_popup_menu_cb;
    ala->sound_cb = &ev_sound_cb;

    gtk_box_pack_start( GTK_BOX(ala_vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
    gtk_box_pack_end( GTK_BOX(ala_vbox), GTK_WIDGET(ala_widget), TRUE, TRUE, 0);

    gtk_container_add( GTK_CONTAINER(parent_wid_ala), ala_vbox);
    // gtk_widget_show_all( parent_wid_ala);
  }

  // Blk Window
  {
    char *titleutf8 = g_convert( blk_name, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);

    parent_wid_blk = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
						 "default-height", blk_height,
						 "default-width", blk_width,
						 "title", titleutf8,
						 NULL);
    g_free( titleutf8);

    g_signal_connect( parent_wid_blk, "delete_event", G_CALLBACK(blk_delete_event), this);
    g_signal_connect( parent_wid_blk, "destroy", G_CALLBACK(blk_destroy_event), this);
    g_signal_connect( parent_wid_blk, "focus-in-event", G_CALLBACK(blk_action_inputfocus), this);

    CoWowGtk::SetWindowIcon( parent_wid_blk);

    GtkWidget *blk_vbox = gtk_vbox_new( FALSE, 0);

    // Menu
    // Accelerators
    GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
    gtk_window_add_accel_group(GTK_WINDOW(parent_wid_blk), accel_g);

    GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

    // File entry
    GtkWidget *file_print = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("_Print"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_print), 
				   gtk_image_new_from_stock( "gtk-print", GTK_ICON_SIZE_MENU));
    g_signal_connect(file_print, "activate", G_CALLBACK(blk_activate_print), this);

    GtkWidget *file_close = gtk_image_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Close"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(file_close), 
				   gtk_image_new_from_stock( "gtk-close", GTK_ICON_SIZE_MENU));
    g_signal_connect(file_close, "activate", G_CALLBACK(blk_activate_exit), this);
    gtk_widget_add_accelerator( file_close, "activate", accel_g,
				'w', GdkModifierType(GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);

    GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

    GtkWidget *file = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_File"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

    // Functions entry
    GtkWidget *functions_block_remove = gtk_menu_item_new_with_mnemonic( 
                            CoWowGtk::translate_utf8("_Remove Blocking"));
    g_signal_connect( functions_block_remove, "activate", 
		      G_CALLBACK(blk_activate_block_remove), this);

    GtkWidget *functions_open_plc = gtk_menu_item_new_with_mnemonic( 
                        CoWowGtk::translate_utf8("Open _Program"));
    g_signal_connect( functions_open_plc, "activate", 
		      G_CALLBACK(blk_activate_open_plc), this);
    gtk_widget_add_accelerator( functions_open_plc, "activate", accel_g,
				'l', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *functions_display_object = gtk_menu_item_new_with_mnemonic( 
                           CoWowGtk::translate_utf8("_Display object in Navigator"));
    g_signal_connect( functions_display_object, "activate", 
		      G_CALLBACK(blk_activate_display_in_xnav), this);
    gtk_widget_add_accelerator( functions_display_object, "activate", accel_g,
				'd', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkMenu *func_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_block_remove);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_open_plc);
    gtk_menu_shell_append(GTK_MENU_SHELL(func_menu), functions_display_object);

    GtkWidget *functions = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Functions"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(func_menu));

    // View entry
    GtkWidget *view_zoom_in = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _In"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_in), 
				   gtk_image_new_from_stock( "gtk-zoom-in", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_in, "activate", 
		      G_CALLBACK(blk_activate_zoom_in), this);
    gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
				'i', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *view_zoom_out = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Out"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_out), 
				   gtk_image_new_from_stock( "gtk-zoom-out", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_out, "activate", 
		      G_CALLBACK(blk_activate_zoom_out), this);
    gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
				'o', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkWidget *view_zoom_reset = gtk_image_menu_item_new_with_mnemonic(CoWowGtk::translate_utf8("Zoom _Reset"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(view_zoom_reset), 
				   gtk_image_new_from_stock( "gtk-zoom-100", GTK_ICON_SIZE_MENU));
    g_signal_connect( view_zoom_reset, "activate", 
		      G_CALLBACK(blk_activate_zoom_reset), this);
    gtk_widget_add_accelerator( view_zoom_reset, "activate", accel_g,
				'b', GdkModifierType(GDK_CONTROL_MASK), 
				GTK_ACCEL_VISIBLE);

    GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);

    GtkWidget *view = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_View"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));

    // Help entry
    GtkWidget *help_help = gtk_image_menu_item_new_with_mnemonic( 
                     CoWowGtk::translate_utf8("_Help"));
    gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_help), 
				   gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
    g_signal_connect(help_help, "activate", G_CALLBACK(blk_activate_help), this);
    gtk_widget_add_accelerator( help_help, "activate", accel_g,
				'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_help);

    GtkWidget *help = gtk_menu_item_new_with_mnemonic( CoWowGtk::translate_utf8("_Help"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));


    // Create blk evlist
    blk = new EvListGtk( this, blk_vbox, ev_eType_BlockList, blk_size, &blk_widget);
    blk->start_trace_cb = &blk_start_trace_cb;
    blk->display_in_xnav_cb = &blk_display_in_xnav_cb;
    blk->name_to_alias_cb = &ev_name_to_alias_cb;
    blk->popup_menu_cb = &ev_popup_menu_cb;

    gtk_box_pack_start( GTK_BOX(blk_vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
    gtk_box_pack_end( GTK_BOX(blk_vbox), GTK_WIDGET(blk_widget), TRUE, TRUE, 0);

    gtk_container_add( GTK_CONTAINER(parent_wid_blk), blk_vbox);
    // gtk_widget_show_all( parent_wid_blk);
  }

  // Store this for the mh callbacks
  ev = this;

  if ( display_eve) {
    gtk_widget_show_all( parent_wid_eve);
    eve_displayed = 1;
  }
  else {
    gtk_widget_show_all( parent_wid_eve);
    g_object_set( parent_wid_eve, "visible", FALSE, NULL);
  }

  if ( display_ala) {
    gtk_widget_show_all( parent_wid_ala);
    ala_displayed = 1;
  }
  else {
    gtk_widget_show_all( parent_wid_ala);
    g_object_set( parent_wid_ala, "visible", FALSE, NULL);
  }

  if ( display_blk) {
    gtk_widget_show_all( parent_wid_blk);
    blk_displayed = 1;
  }
  else {
    gtk_widget_show_all( parent_wid_blk);
    g_object_set( parent_wid_blk, "visible", FALSE, NULL);
  }

  wow = new CoWowGtk( parent_wid_ala);

  sts = outunit_connect( user);
  if ( EVEN(sts))
    *status = sts;

}


//
//  Delete ev
//
EvGtk::~EvGtk()
{
  if ( connected)
    mh_OutunitDisconnect();
  if ( parent_wid_eve)
    gtk_widget_destroy( parent_wid_eve);
  if ( parent_wid_ala)
    gtk_widget_destroy( parent_wid_ala);
  if ( parent_wid_blk)
    gtk_widget_destroy( parent_wid_blk);
  if ( eve)
    delete eve;
  if ( ala)
    delete ala;
  ev = NULL;
}

void EvGtk::map_eve()
{
  gtk_window_present( GTK_WINDOW(parent_wid_eve));
  eve_displayed = 1;
}

void EvGtk::map_ala()
{
  gtk_window_present( GTK_WINDOW(parent_wid_ala));
  ala_displayed = 1;
}

void EvGtk::map_blk()
{
  gtk_window_present( GTK_WINDOW(parent_wid_blk));
  blk_displayed = 1;
}

void EvGtk::unmap_eve()
{
  if ( eve_displayed) {
    g_object_set( parent_wid_eve, "visible", FALSE, NULL);
    eve_displayed = 0;
  }
}

void EvGtk::unmap_ala()
{
  if ( ala_displayed) {
    g_object_set( parent_wid_ala, "visible", FALSE, NULL);
    ala_displayed = 0;
  }
}

void EvGtk::unmap_blk()
{
  if ( blk_displayed) {
    g_object_set( parent_wid_blk, "visible", FALSE, NULL);
    blk_displayed = 0;
  }
}

gboolean EvGtk::eve_action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  Ev *ev = (Ev *)data;

  if ( ev && ev->eve_displayed)
    ev->eve->set_input_focus();

  return FALSE;
}

gboolean EvGtk::ala_action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  Ev *ev = (Ev *)data;

  if ( ev && ev->ala_displayed)
    ev->ala->set_input_focus();

  return FALSE;
}

gboolean EvGtk::blk_action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  Ev *ev = (Ev *)data;

  if ( ev && ev->blk_displayed)
    ev->blk->set_input_focus();

  return FALSE;
}

void EvGtk::eve_activate_exit( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->unmap_eve();
  ev->eve_displayed = 0;
}

void EvGtk::ala_activate_exit( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->unmap_ala();
  ev->ala_displayed = 0;
}

void EvGtk::blk_activate_exit( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->unmap_blk();
  ev->blk_displayed = 0;
}

void EvGtk::eve_activate_print( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve_activate_print();
}

void EvGtk::ala_activate_print( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->ala_activate_print();
}

void EvGtk::blk_activate_print( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->blk_activate_print();
}

void EvGtk::eve_activate_ack_last( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve_activate_ack_last();
}

void EvGtk::ala_activate_ack_last( GtkWidget *w, gpointer data)
{
  eve_activate_ack_last( w, data);
}

void EvGtk::eve_activate_zoom_in( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve->zoom( 1.2);
}

void EvGtk::ala_activate_zoom_in( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->ala->zoom( 1.2);
}

void EvGtk::blk_activate_zoom_in( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->blk->zoom( 1.2);
}

void EvGtk::eve_activate_zoom_out( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve->zoom( 5.0/6);
}

void EvGtk::ala_activate_zoom_out( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->ala->zoom( 5.0/6);
}

void EvGtk::blk_activate_zoom_out( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->blk->zoom( 5.0/6);
}

void EvGtk::eve_activate_zoom_reset( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve->unzoom();
}

void EvGtk::ala_activate_zoom_reset( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->ala->unzoom();
}

void EvGtk::blk_activate_zoom_reset( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->blk->unzoom();
}

void EvGtk::blk_activate_block_remove( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->blk->block_remove();
}

void EvGtk::eve_activate_open_plc( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve->start_trace();
}

void EvGtk::ala_activate_open_plc( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->ala->start_trace();
}

void EvGtk::blk_activate_open_plc( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->blk->start_trace();
}

void EvGtk::eve_activate_display_in_xnav( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve->display_in_xnav();
}

void EvGtk::ala_activate_display_in_xnav( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->ala->display_in_xnav();
}

void EvGtk::blk_activate_display_in_xnav( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->blk->display_in_xnav();
}

void EvGtk::eve_activate_disp_hundredth( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  ev->eve->set_display_hundredth( set);
}

void EvGtk::ala_activate_disp_hundredth( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  ev->ala->set_display_hundredth( set);
}

void EvGtk::eve_activate_hide_object( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  ev->eve->set_hide_object( set);
}

void EvGtk::ala_activate_hide_object( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  ev->ala->set_hide_object( set);
}

void EvGtk::eve_activate_hide_text( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  ev->eve->set_hide_text( set);
}

void EvGtk::ala_activate_hide_text( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;
  int set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));

  ev->ala->set_hide_text( set);
}

void EvGtk::eve_activate_help( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve_activate_help();
}

void EvGtk::eve_activate_helpevent( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->eve_activate_helpevent();
}

void EvGtk::ala_activate_help( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->ala_activate_help();
}

void EvGtk::ala_activate_helpevent( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->ala_activate_helpevent();
}

void EvGtk::blk_activate_help( GtkWidget *w, gpointer data)
{
  Ev *ev = (Ev *)data;

  ev->blk_activate_help();
}

