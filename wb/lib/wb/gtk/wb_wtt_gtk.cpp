/* 
 * Proview   $Id: wb_wtt_gtk.cpp,v 1.28 2008-10-31 12:51:31 claes Exp $
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

/* wb_wtt_gtk.cpp -- Display plant and node hiererachy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "cow_wow.h"
#include "wb_utl_api.h"
#include "wb_lfu.h"
#include "rt_load.h"
#include "wb_foe_msg.h"
#include "wb_pwrb_msg.h"
#include "wb_log.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "cow_wow_gtk.h"
#include "wb_wtt.h"
#include "wb_wnav_gtk.h"
#include "wb_wnav_item.h"
#include "wb_pal_gtk.h"
#include "wb_watt_gtk.h"
#include "wb_wda_gtk.h"
#include "wb_uted_gtk.h"
#include "wb_wtt_gtk.h"
#include "wb_wnav_msg.h"
#include "wb_volume.h"
#include "wb_env.h"
#include "wb_wpkg_gtk.h"
#include "cow_rtmon_gtk.h"
#include "cow_statusmon_nodelist_gtk.h"
#include "cow_msgwindow.h"
#include "cow_logw_gtk.h"
#include "wb_wnav_selformat.h"
#include "wb_pwrs.h"
#include "wb_build.h"
#include "wb_wcast_gtk.h"
#include "ge_gtk.h"


#define WTT_PALETTE_WIDTH 160
#define MENU_BAR      1
#define MENU_PULLDOWN 2
#define MENU_POPUP    3
#define MENU_OPTION   4

CoWowRecall WttGtk::cmd_recall;
CoWowRecall WttGtk::value_recall;
CoWowRecall WttGtk::name_recall;

void WttGtk::set_window_char( int width, int height)
{
  gtk_window_resize( GTK_WINDOW(toplevel), width, height);
}

void WttGtk::get_window_char( int *width, int *height)
{
  gtk_window_get_size( GTK_WINDOW(toplevel), width, height);
}

void WttGtk::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = gdk_cursor_new_for_display( gtk_widget_get_display( toplevel),
					       GDK_WATCH);

  gdk_window_set_cursor( toplevel->window, clock_cursor);
  gdk_display_flush( gtk_widget_get_display( toplevel));
}

void WttGtk::reset_cursor()
{
  gdk_window_set_cursor( toplevel->window, NULL);
}

void WttGtk::free_cursor()
{
  if (clock_cursor)
    gdk_cursor_unref( clock_cursor);
}

void WttGtk::menu_setup()
{
  switch( wb_type) {
  case wb_eType_Directory:
    if ( editmode) {
      gtk_widget_set_sensitive( menu_save_w, TRUE);
      gtk_widget_set_sensitive( tools_save_w, TRUE);
      gtk_widget_set_sensitive( menu_revert_w, TRUE);
      gtk_widget_set_sensitive( menu_cut_w, TRUE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, TRUE);
      gtk_widget_set_sensitive( menu_pasteinto_w, TRUE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, TRUE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, FALSE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, TRUE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, TRUE);
    }
    else {
      gtk_widget_set_sensitive( menu_save_w, FALSE);
      gtk_widget_set_sensitive( tools_save_w, FALSE);
      gtk_widget_set_sensitive( menu_revert_w, FALSE);
      gtk_widget_set_sensitive( menu_cut_w, FALSE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, FALSE);
      gtk_widget_set_sensitive( menu_pasteinto_w, FALSE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, FALSE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, FALSE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, TRUE);
      gtk_widget_set_sensitive( menu_distribute_w, TRUE);
      gtk_widget_set_sensitive( menu_change_value_w, FALSE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, TRUE);
      gtk_widget_set_sensitive( menu_classeditor_w, TRUE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    break;
  case wb_eType_Volume:
    if ( editmode) {
      gtk_widget_set_sensitive( menu_save_w, TRUE);
      gtk_widget_set_sensitive( tools_save_w, TRUE);
      gtk_widget_set_sensitive( menu_revert_w, TRUE);
      gtk_widget_set_sensitive( menu_cut_w, TRUE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, TRUE);
      gtk_widget_set_sensitive( menu_pasteinto_w, TRUE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, TRUE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, FALSE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, TRUE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      if ( ldhses && ldh_VolRepType( ldhses) == ldh_eVolRep_Dbs)
	gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
      else
	gtk_widget_set_sensitive( menu_updateclasses_w, TRUE);
    }
    else {
      gtk_widget_set_sensitive( menu_save_w, FALSE);
      gtk_widget_set_sensitive( tools_save_w, FALSE);
      gtk_widget_set_sensitive( menu_revert_w, FALSE);
      gtk_widget_set_sensitive( menu_cut_w, FALSE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, FALSE);
      gtk_widget_set_sensitive( menu_pasteinto_w, FALSE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, FALSE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, TRUE);
      gtk_widget_set_sensitive( menu_buildobject_w, TRUE);
      if ( ldhses && ldh_VolRepType( ldhses) == ldh_eVolRep_Dbs)
	gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      else
	gtk_widget_set_sensitive( menu_buildvolume_w, TRUE);
      gtk_widget_set_sensitive( menu_buildnode_w, TRUE);
      gtk_widget_set_sensitive( menu_distribute_w, TRUE);
      gtk_widget_set_sensitive( menu_change_value_w, FALSE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, TRUE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    break; 
  case wb_eType_Class:
    if ( editmode) {
      gtk_widget_set_sensitive( menu_save_w, TRUE);
      gtk_widget_set_sensitive( tools_save_w, TRUE);
      gtk_widget_set_sensitive( menu_revert_w, TRUE);
      gtk_widget_set_sensitive( menu_cut_w, TRUE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, TRUE);
      gtk_widget_set_sensitive( menu_pasteinto_w, TRUE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, TRUE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, TRUE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, TRUE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    else {
      gtk_widget_set_sensitive( menu_save_w, FALSE);
      gtk_widget_set_sensitive( tools_save_w, FALSE);
      gtk_widget_set_sensitive( menu_revert_w, FALSE);
      gtk_widget_set_sensitive( menu_cut_w, FALSE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, FALSE);
      gtk_widget_set_sensitive( menu_pasteinto_w, FALSE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, FALSE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, FALSE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, TRUE);
      gtk_widget_set_sensitive( menu_buildnode_w, TRUE);
      gtk_widget_set_sensitive( menu_distribute_w, TRUE);
      gtk_widget_set_sensitive( menu_change_value_w, FALSE);
      gtk_widget_set_sensitive( menu_edit_w, FALSE);
      gtk_widget_set_sensitive( tools_edit_w, FALSE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    break;
  case wb_eType_ClassEditor:
    g_object_set( menu_buildobject_w, "visible", FALSE, NULL);
    g_object_set( menu_utilities_w, "visible", FALSE, NULL);
    g_object_set( menu_buildnode_w, "visible", FALSE, NULL);
    g_object_set( menu_classeditor_w, "visible", FALSE, NULL);
    if ( editmode) {
      gtk_widget_set_sensitive( menu_save_w, TRUE);
      gtk_widget_set_sensitive( tools_save_w, TRUE);
      gtk_widget_set_sensitive( menu_revert_w, TRUE);
      gtk_widget_set_sensitive( menu_cut_w, TRUE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, TRUE);
      gtk_widget_set_sensitive( menu_pasteinto_w, TRUE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, TRUE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, FALSE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, TRUE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      if ( ldh_VolRepType( ldhses) == ldh_eVolRep_Ced)
	gtk_widget_set_sensitive( menu_updateclasses_w, TRUE);
      else
	gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    else {
      gtk_widget_set_sensitive( menu_save_w, FALSE);
      gtk_widget_set_sensitive( tools_save_w, FALSE);
      gtk_widget_set_sensitive( menu_revert_w, FALSE);
      gtk_widget_set_sensitive( menu_cut_w, FALSE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, FALSE);
      gtk_widget_set_sensitive( menu_pasteinto_w, FALSE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, FALSE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, TRUE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, TRUE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, FALSE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, TRUE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    break;
  case wb_eType_Buffer:
    if ( editmode) {
      gtk_widget_set_sensitive( menu_save_w, FALSE);
      gtk_widget_set_sensitive( tools_save_w, FALSE);
      gtk_widget_set_sensitive( menu_revert_w, FALSE);
      gtk_widget_set_sensitive( menu_cut_w, TRUE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, TRUE);
      gtk_widget_set_sensitive( menu_pasteinto_w, TRUE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, TRUE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, FALSE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, TRUE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    else {
      gtk_widget_set_sensitive( menu_save_w, FALSE);
      gtk_widget_set_sensitive( tools_save_w, FALSE);
      gtk_widget_set_sensitive( menu_revert_w, FALSE);
      gtk_widget_set_sensitive( menu_cut_w, FALSE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, FALSE);
      gtk_widget_set_sensitive( menu_pasteinto_w, FALSE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, FALSE);
      gtk_widget_set_sensitive( menu_utilities_w, TRUE);
      gtk_widget_set_sensitive( menu_openplc_w, TRUE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, FALSE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    break; 
  case wb_eType_ExternVolume:
    if ( editmode) {
      gtk_widget_set_sensitive( menu_save_w, TRUE);
      gtk_widget_set_sensitive( tools_save_w, TRUE);
      gtk_widget_set_sensitive( menu_revert_w, TRUE);
      gtk_widget_set_sensitive( menu_cut_w, TRUE);
      gtk_widget_set_sensitive( menu_copy_w, TRUE);
      gtk_widget_set_sensitive( menu_paste_w, TRUE);
      gtk_widget_set_sensitive( menu_pasteinto_w, TRUE);
      gtk_widget_set_sensitive( menu_copykeep_w, TRUE);
      gtk_widget_set_sensitive( menu_rename_w, TRUE);
      gtk_widget_set_sensitive( menu_utilities_w, FALSE);
      gtk_widget_set_sensitive( menu_openplc_w, FALSE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, TRUE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    else {
      gtk_widget_set_sensitive( menu_save_w, FALSE);
      gtk_widget_set_sensitive( tools_save_w, TRUE);
      gtk_widget_set_sensitive( menu_revert_w, FALSE);
      gtk_widget_set_sensitive( menu_cut_w, FALSE);
      gtk_widget_set_sensitive( menu_copy_w, FALSE);
      gtk_widget_set_sensitive( menu_paste_w, FALSE);
      gtk_widget_set_sensitive( menu_pasteinto_w, FALSE);
      gtk_widget_set_sensitive( menu_copykeep_w, FALSE);
      gtk_widget_set_sensitive( menu_rename_w, FALSE);
      gtk_widget_set_sensitive( menu_utilities_w, FALSE);
      gtk_widget_set_sensitive( menu_openplc_w, FALSE);
      gtk_widget_set_sensitive( menu_buildobject_w, FALSE);
      gtk_widget_set_sensitive( menu_buildvolume_w, FALSE);
      gtk_widget_set_sensitive( menu_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_distribute_w, FALSE);
      gtk_widget_set_sensitive( menu_change_value_w, FALSE);
      gtk_widget_set_sensitive( menu_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_edit_w, TRUE);
      gtk_widget_set_sensitive( tools_buildnode_w, FALSE);
      gtk_widget_set_sensitive( menu_classeditor_w, FALSE);
      gtk_widget_set_sensitive( menu_updateclasses_w, FALSE);
    }
    break;
  default:
    ;
  }
}

void WttGtk::set_selection_owner()
{
  selection_timerid = g_timeout_add( 200, WttGtk::set_selection_owner_proc, this);  
}

void WttGtk::set_palette_selection_owner() 
{
  selection_timerid = g_timeout_add( 200, 
				     WttGtk::set_palette_selection_owner_proc, this);
}

gboolean WttGtk::set_selection_owner_proc( void *data)
{
  // Delay call to own selection, to make it possible to paste previous selection 
  // to value inputwith MB2
  WttGtk *wtt = (WttGtk *)data;

  wtt->selection_timerid = 0;
  if ( wtt->focused_wnav)
    wtt->focused_wnav->set_selection_owner();
  return FALSE;
}

gboolean WttGtk::set_palette_selection_owner_proc( void *data)
{
  // Delay call to own selection, to make it possible to paste previous selection 
  // to value inputwith MB2
  WttGtk *wtt = (WttGtk *)data;

  wtt->selection_timerid = 0;
  wtt->palette->set_selection_owner();
  return FALSE;
}

int WttGtk::create_popup_menu( pwr_sAttrRef aref,
			       int x, int y)
{
  GtkWidget *popup;
  int sts;

  // Calculate position
  GdkEvent *e = gtk_get_current_event();
  if ( e->any.window == ((WNavGtk *)wnavnode)->brow_widget->window) {
    CoWowGtk::PopupPosition( ((WNavGtk *)wnavnode)->brow_widget, x, y, &popupmenu_x, &popupmenu_y);
  }
  else if ( e->any.window == ((WNavGtk *)wnav)->brow_widget->window) {
    CoWowGtk::PopupPosition( ((WNavGtk *)wnav)->brow_widget, x, y, &popupmenu_x, &popupmenu_y);
  }
  else
    return 0;
  gdk_event_free( e);
  popupmenu_x += 10;

  // Create the menu
  sts = get_popup_menu_items( aref, pwr_cNCid);
  if ( EVEN(sts)) return sts;

  popup = build_menu(); 
  if ( !popup)
    return 0;

  gtk_menu_popup( GTK_MENU(popup), NULL, NULL, menu_position_func, 
  		  this, 0, gtk_get_current_event_time());

  return 1;
}

int WttGtk::create_pal_popup_menu( pwr_tCid cid,
				   int x, int y)
{
  GtkWidget *popup;
  int sts;

  // Calculate position
  gint wind_x, wind_y;

  gtk_window_get_position( GTK_WINDOW(toplevel), &wind_x, &wind_y);
  popupmenu_x = x + wind_x + 5;
  popupmenu_y = y + wind_y + 70;

  // Create the menu
  sts = get_popup_menu_items( pwr_cNAttrRef, cid);
  if ( EVEN(sts)) return sts;

  popup = build_menu(); 
  if ( !popup)
    return 0;

  gtk_menu_popup( GTK_MENU(popup), NULL, NULL, menu_position_func, 
  		  this, 0, gtk_get_current_event_time());

  return 1;
}

void WttGtk::set_noedit_show()
{
  int width, height;

  if ( editmode == 0)
    return;

  g_object_set( palette_widget, "visible", FALSE, NULL);
  get_window_char( &width, &height);
  set_window_char( width - WTT_PALETTE_WIDTH, height);
}

void WttGtk::set_edit_show()
{
  int width, height;

  g_object_set( palette_widget, "visible", TRUE, NULL);
  gtk_paned_set_position( GTK_PANED(palette_paned), WTT_PALETTE_WIDTH);
  get_window_char( &width, &height);
  set_window_char( width + WTT_PALETTE_WIDTH, height);
}

void WttGtk::set_twowindows( int two, int display_wnav, int display_wnavnode)
{
  if ( disable_w2) {
    display_wnav = 1;
    display_wnavnode = 0;
    two = 0;
  }
  if ( display_wnav || display_wnavnode) {
    if ( display_wnav && ! wnav_mapped) {
      g_object_set( wnav_brow_widget, "visible", TRUE, NULL);
      set_focus( wnav);
      wnav_mapped = 1;
    }
    if ( display_wnavnode && ! wnavnode_mapped) {
      g_object_set( wnavnode_brow_widget, "visible", TRUE, NULL);
      set_focus( wnavnode);
      wnavnode_mapped = 1;
    }
    if ( !display_wnav && wnav_mapped) {
      if ( realized)
	g_object_set( wnav_brow_widget, "visible", FALSE, NULL);
      set_focus( wnavnode);
      wnav_mapped = 0;
    }
    if ( !display_wnavnode && wnavnode_mapped) {
      if ( realized)
	g_object_set( wnavnode_brow_widget, "visible", FALSE, NULL);
      set_focus( wnav);
      wnavnode_mapped = 0;
    }
    twowindows = display_wnav && display_wnavnode;
  }
  else {
    if ( two == twowindows)
      return;

    if ( !two) {
      // Keep the focused window
      if ( focused_wnav == wnavnode) {
	if ( realized)
	  g_object_set( wnav_brow_widget, "visible", FALSE, NULL);
        set_focus( wnavnode);
        wnav_mapped = 0;
        wnavnode_mapped = 1;
      }
      else {
	if ( realized)
	  g_object_set( wnavnode_brow_widget, "visible", FALSE, NULL);
        set_focus( wnav);
        wnav_mapped = 1;
        wnavnode_mapped = 0;
      }
    }
    else {
      if ( !wnav_mapped) {
	g_object_set( wnav_brow_widget, "visible", TRUE, NULL);
        set_focus( wnav);
        wnav_mapped = 1;
      }
      if ( !wnavnode_mapped) {
	g_object_set( wnavnode_brow_widget, "visible", TRUE, NULL);
        set_focus( wnavnode);
        wnavnode_mapped = 1;
      }
    }
    twowindows = two;
  }
}

void WttGtk::message( char severity, const char *message)
{
  char *messageutf8 = g_convert( message, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  gtk_label_set_text( GTK_LABEL(msg_label), messageutf8);
  g_free( messageutf8);
}


void WttGtk::set_prompt( const char *prompt)
{
  if ( strcmp(prompt, "") == 0) {
    g_object_set( cmd_prompt, "visible", FALSE, NULL);
    g_object_set( msg_label, "visible", TRUE, NULL);
  }
  else {
    g_object_set( msg_label, "visible", FALSE, NULL);
    g_object_set( cmd_prompt, "visible", TRUE, 
		  "label", prompt, NULL);
  }
}

void WttGtk::watt_new( pwr_tAttrRef aref)
{
  new WAttGtk( toplevel, this, ldhses,
	    aref, editmode,
	    wnavnode->gbl.advanced_user, 1);
}

void WttGtk::wda_new( pwr_tOid oid)
{
  new WdaGtk( toplevel, this, ldhses,
	      oid, 0, "", editmode, wnav->gbl.advanced_user, 1);
}

void WttGtk::ge_new( char *graph_name)
{
  unsigned int opt = wnavnode->gbl.enable_comment ? ge_mOption_EnableComment : 0;
  new GeGtk( NULL, toplevel, ldhses, 0, opt, graph_name);
}

void WttGtk::wcast_new( pwr_tAttrRef aref, pwr_tStatus *sts) 
{
  new WCastGtk( this, toplevel, "Casting Window", ldhses, aref, sts);
}

wb_build *WttGtk::build_new()
{
  return new wb_build( *(wb_session *)ldhses, focused_wnav);
}

void WttGtk::wpkg_new()
{
  wpkg = new WPkgGtk( toplevel, this);
  wpkg->close_cb = Wtt::wpkg_quit_cb;
}

int WttGtk::ute_new( char *title)
{
  pwr_tStatus sts;
  utedctx = new WUtedGtk( this, toplevel, title, "Utilites",
		   ldh_SessionToWB(ldhses), ldhses, editmode, 
		   uted_quit_cb, &sts);
  return sts;
}

void WttGtk::close_change_value()
{
  if ( input_open) {
    g_object_set( cmd_input, "visible", FALSE, NULL);
    set_prompt( "");
    input_open = 0;
  }
}

void WttGtk::open_change_value()
{
  int		sts;
  brow_tObject	*sellist;
  int		sel_cnt1, sel_cnt2;
  char 		*value = 0;
  int		multiline;
  int		input_size;

  if ( input_open) {
    g_object_set( cmd_input, "visible", FALSE, NULL);
    set_prompt( "");
    input_open = 0;
    focused_wnav->set_inputfocus(1);
    return;
  }

  sts = wnav->get_selected_nodes( &sellist, &sel_cnt1);
  if ( ODD(sts)) {
    if ( sel_cnt1 != 1) {
      message('E', "Select one attribute");
      return;
    }

    sts = wnav->check_attr_value( sellist[0], &multiline, &value, &input_size);
    if ( EVEN(sts)) {
      message( 'E', wnav_get_message( sts));
      return;
    }
    if ( multiline) {
      message( 'E', "Edit multiline attributes in the Object Editor");
      return;
    }

    input_node = sellist[0];
    input_wnav = wnav;
    wnav->node_to_objid( input_node, &input_objid);
    free( sellist);
  }
  else {
    sts = wnavnode->get_selected_nodes( &sellist, &sel_cnt2);
    if ( ODD(sts)) {
      if ( sel_cnt2 != 1) {
        message('E', "Select one attribute");
        return;
      }

      sts = wnavnode->check_attr_value( sellist[0], &multiline, &value, &input_size);
      if ( EVEN(sts)) {
        message( 'E', wnav_get_message( sts));
        return;
      }
      if ( multiline) {
        message( 'E', "Edit multiline attributes in the attribute editor");
        return;
      }

      input_node = sellist[0];
      input_wnav = wnavnode;
      wnavnode->node_to_objid( input_node, &input_objid);
      free( sellist);
    }
  }
  if ( sel_cnt1 == 0 && sel_cnt2 == 0) {
    message('E', "Select an attribute");
    return;
  }

  if ( command_open)
    command_open = 0;
  else
    g_object_set( cmd_input, "visible", TRUE, NULL);

  g_object_set( cmd_input, "max-length", input_size, NULL);
  message( ' ', "");
  cmd_entry->set_recall_buffer( &value_recall);
  gtk_widget_grab_focus( cmd_input);

  if ( value) {
    char *textutf8 = g_convert( value, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
    gint pos = 0;
    gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);
    gtk_editable_insert_text( GTK_EDITABLE(cmd_input), textutf8, strlen(textutf8), &pos);
    g_free( textutf8);

    // Select the text
    gtk_editable_set_position( GTK_EDITABLE(cmd_input), -1);
    gtk_editable_select_region( GTK_EDITABLE(cmd_input), 0, -1);
  }
  else
    gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);

  set_prompt( "value >");
  input_open = 1;
  input_mode = wtt_eInputMode_Attribute;
}

void WttGtk::open_change_name()
{
  int		sts;
  brow_tObject	*sellist;
  int		sel_cnt1, sel_cnt2;
  int           size;
  char          name[80];

  if ( input_open) {
    g_object_set( cmd_input, "visible", FALSE, NULL);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = wnav->get_selected_nodes( &sellist, &sel_cnt1);
  if ( ODD(sts)) {
    if ( sel_cnt1 != 1) {
      message('E', "Select one object");
      return;
    }

    sts = wnav->check_object_name( sellist[0]);
    if ( EVEN(sts)) {
      message( 'E', wnav_get_message( sts));
      return;
    }

    input_node = sellist[0];
    input_wnav = wnav;
    wnav->node_to_objid( input_node, &input_objid);
    free( sellist);
  }
  else {
    sts = wnavnode->get_selected_nodes( &sellist, &sel_cnt2);
    if ( ODD(sts)) {
      if ( sel_cnt2 != 1) {
        message('E', "Select one object");
        return;
      }

      sts = wnavnode->check_object_name( sellist[0]);
      if ( EVEN(sts)) {
        message( 'E', wnav_get_message( sts));
        return;
      }

      input_node = sellist[0];
      input_wnav = wnavnode;
      wnavnode->node_to_objid( input_node, &input_objid);
      free( sellist);
    }
  }
  if ( sel_cnt1 == 0 && sel_cnt2 == 0) {
    message('E', "Select an object");
    return;
  }

  if ( command_open)
    command_open = 0;
  else
    g_object_set( cmd_input, "visible", TRUE, NULL);

  g_object_set( cmd_input, "max-length", sizeof(pwr_tObjName) - 1, NULL);
  message( ' ', "");
  cmd_entry->set_recall_buffer( &name_recall);
  gtk_widget_grab_focus( cmd_input);

  sts = ldh_ObjidToName( ldhses, input_objid, ldh_eName_Object, 
					   name, sizeof(name), &size); 

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(cmd_input), 0, -1);
  char *nameutf8 = g_convert( name, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);
  gtk_editable_insert_text( GTK_EDITABLE(cmd_input), nameutf8, strlen(nameutf8), &pos);
  g_free( nameutf8);

  // Select the text
  gtk_editable_set_position( GTK_EDITABLE(cmd_input), -1);
  gtk_editable_select_region( GTK_EDITABLE(cmd_input), 0, -1);

  set_prompt( "name >");
  input_open = 1;
  input_mode = wtt_eInputMode_ObjectName;
}

//
//  Callbackfunctions from menu entries
//
void WttGtk::activate_change_value( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->open_change_value();
}

void WttGtk::activate_command( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  
  if ( wtt->command_open) {
    g_object_set( ((WttGtk *)wtt)->cmd_input, "visible", FALSE, NULL);
    wtt->set_prompt( "");
    wtt->command_open = 0;
    return;
  }

  gtk_editable_delete_text( GTK_EDITABLE(((WttGtk *)wtt)->cmd_input), 0, -1);

  if ( wtt->input_open)
    wtt->input_open = 0;
  else
    g_object_set( ((WttGtk *)wtt)->cmd_input, "visible", TRUE, NULL);

  g_object_set( ((WttGtk *)wtt)->cmd_input, "max-length", sizeof(pwr_tCmd) - 1, NULL);
  wtt->set_prompt( "wtt >");
  wtt->message( ' ', "");
  ((WttGtk *)wtt)->cmd_entry->set_recall_buffer( &cmd_recall);
  gtk_widget_grab_focus( ((WttGtk *)wtt)->cmd_input);

  wtt->command_open = 1;
}

static void wtt_help_cb( void *ctx, const char *topic)
{
  pwr_tCmd cmd;
  sprintf( cmd, "help %s", topic);
  ((Wtt *)ctx)->wnav->command( cmd);
}

void WttGtk::activate_rtmon( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  pwr_tStatus sts;
  
  RtMon *rtmon = new RtMonGtk( wtt, ((WttGtk *)wtt)->toplevel, "Runtime Monitor", 0, &sts);
  rtmon->help_cb = wtt_help_cb;
}

void WttGtk::activate_statusmon( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  pwr_tStatus sts;
  
  Nodelist *nl = new NodelistGtk( wtt, ((WttGtk *)wtt)->toplevel, "Supervision Center", 
				  nodelist_eMode_SystemStatus, 0, 0, &sts);
  nl->set_scantime(3);
  nl->help_cb = wtt_help_cb;
}

void WttGtk::activate_exit( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  Wtt::close( wtt);
}

void WttGtk::activate_print( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_print();
}

void WttGtk::activate_collapse( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_collapse();
}

void WttGtk::activate_save( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_save();
}

void WttGtk::activate_revert( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_revert();
}

void WttGtk::activate_syntax( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_syntax();
}

void WttGtk::activate_history( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  pwr_tStatus sts;
  char categories[3][20];
  pwr_tObjName vname;
  pwr_tOid oid;
  int size;
  char title[80];

  wb_log::category_to_string( wlog_eCategory_ConfiguratorSave, categories[0]);
  wb_log::category_to_string( wlog_eCategory_VolumeBuild, categories[1]);
  strcpy( categories[2], "");

  oid.oix = 0;
  oid.vid = wtt->volid;
  sts = ldh_ObjidToName( wtt->ldhses, oid, ldh_eName_Object, vname, sizeof(vname), &size);
  if ( EVEN(sts)) return;
  
  strcpy( title, "History Configurator ");
  strcat( title, vname);
  CoLogWGtk *logw = new CoLogWGtk( wtt, ((WttGtk *)wtt)->parent_wid, title, 0, &sts);
  logw->show( categories, vname);
}

void WttGtk::activate_find( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_find();
}

void WttGtk::activate_findregex( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_findregex();
}

void WttGtk::activate_findnext( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_findnext();
}

void WttGtk::activate_copy( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_copy();
}

void WttGtk::activate_cut( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_cut();
}

void WttGtk::activate_paste( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_paste();
}

void WttGtk::activate_pasteinto( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_pasteinto();
}

void WttGtk::activate_copykeep( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_copykeep();
}

void WttGtk::activate_rename( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->open_change_name();
}

void WttGtk::activate_creaobjafter( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_creaobj( ldh_eDest_After);
}

void WttGtk::activate_creaobjfirst( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_creaobj( ldh_eDest_IntoFirst);
}

void WttGtk::activate_moveobjup( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_moveobj( wnav_eDestCode_Before);
}

void WttGtk::activate_moveobjdown( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_moveobj( wnav_eDestCode_After);
}

void WttGtk::activate_moveobjinto( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_moveobj( wnav_eDestCode_FirstChild);
}

void WttGtk::activate_moveobjontop( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_moveobj( wnav_eDestCode_LastChild);
}

void WttGtk::activate_deleteobj( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_deleteobj();
}

void WttGtk::activate_configure( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;

  wtt->activate_configure();
}

void WttGtk::activate_utilities( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  // disable_set_focus( wtt, 2000);
  wtt->activate_utilities();
}

void WttGtk::activate_openobject( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openobject();
}

void WttGtk::activate_openvolobject( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openvolobject();
}

void WttGtk::activate_openplc( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openplc();
}

void WttGtk::activate_buildobject( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_buildobject();
}

void WttGtk::activate_openvolume( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openvolume();
}
 
void WttGtk::activate_openbuffer( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openbuffer();
}
 
void WttGtk::activate_confproject( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_confproject();
}
 
void WttGtk::activate_openfile_dbs( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;

  wtt->set_clock_cursor();
  wtt->wnav->wow->CreateFileSelDia( "Loadfile Selection", (void *)wtt,
			Wtt::file_selected_cb, wow_eFileSelType_Dbs);
  wtt->reset_cursor();
}

void WttGtk::activate_openfile_wbl( GtkWidget *w, gpointer data)
{ 
  Wtt *wtt = (Wtt *)data;

  wtt->set_clock_cursor();
  wtt->wnav->wow->CreateFileSelDia( "Loadfile Selection", (void *)wtt,
			Wtt::file_selected_cb, wow_eFileSelType_Wbl);
  wtt->reset_cursor();
}

void WttGtk::activate_openpl( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openpl();
}

void WttGtk::activate_opengvl( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_opengvl();
}

void WttGtk::activate_openudb( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openudb();
}

void WttGtk::activate_spreadsheet( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_spreadsheet();
}

void WttGtk::activate_openge( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openge();
}

void WttGtk::activate_openclasseditor( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_openclasseditor();
}

void WttGtk::activate_buildvolume( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_buildvolume();
}

void WttGtk::activate_buildnode( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_buildnode();
}

void WttGtk::activate_distribute( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_distribute();
}

void WttGtk::activate_showcrossref( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_showcrossref();
}

void WttGtk::activate_updateclasses( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_updateclasses();
}

void WttGtk::activate_zoom_in( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_zoom_in();
}

void WttGtk::activate_zoom_out( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_zoom_out();
}

void WttGtk::activate_zoom_reset( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_zoom_reset();
}

void WttGtk::activate_twowindows( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_twowindows();
}

void WttGtk::activate_messages( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_messages();
}

void WttGtk::activate_view( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  ((WttGtk *)wtt)->create_options_dialog();
  wtt->update_options_form();
}

void WttGtk::activate_savesettings( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->save_settings();
}

void WttGtk::activate_restoresettings( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->restore_settings();
}

void WttGtk::activate_scriptproj( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_scriptproj();
}

void WttGtk::activate_scriptbase( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_scriptbase();
}

void WttGtk::activate_set_advuser( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  pwr_tCmd cmd = "set advanceduser";
  wtt->wnav->command( cmd);
}

void WttGtk::activate_help( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_help();
}

void WttGtk::activate_help_project( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_help_project();
}

void WttGtk::activate_help_proview( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  wtt->activate_help_proview();
}

void WttGtk::valchanged_cmd_input( GtkWidget *w, gpointer data)
{
  Wtt 	*wtt = (Wtt *)data;
  int 	sts;
  char 	*text, *textutf8;

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(w), 0, -1);
  text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  if ( wtt->input_open) {
    switch( wtt->input_mode)  {
    case wtt_eInputMode_Attribute:
      wtt->input_wnav->select_object( wtt->input_node);
      sts = wtt->input_wnav->set_attr_value( wtt->input_node, 
					     wtt->input_objid, text);
      if ( EVEN(sts))
	wtt->message( 'E', wnav_get_message( sts));
      break;
    case wtt_eInputMode_ObjectName:
      wtt->input_wnav->select_object( wtt->input_node);
      sts = wtt->input_wnav->set_object_name( wtt->input_node, 
					      wtt->input_objid, text);
      if ( EVEN(sts))
	wtt->message( 'E', wnav_get_message( sts));
      break;
    default:
      ;
    }
    if ( wtt->input_open) {
      g_object_set( w, "visible", FALSE, NULL);
      wtt->set_prompt( "");
      wtt->input_open = 0;
    }
  }
  else if ( wtt->command_open) {
    if ( !wtt->focused_wnav)
      wtt->set_focus_default();
    wtt->set_clock_cursor();
    sts = wtt->focused_wnav->command( text);
    wtt->reset_cursor();
    g_object_set( w, "visible", FALSE, NULL);
    wtt->set_prompt( "");
    wtt->command_open = 0;
    wtt->focused_wnav->set_inputfocus(1);
  }
  g_free( text);
}

void WttGtk::activate_india_ok( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  char *text, *textutf8;

  textutf8 = gtk_editable_get_chars( GTK_EDITABLE(((WttGtk *)wtt)->india_text), 
				 0, -1);
  text = g_convert( textutf8, -1, "ISO8859-1", "UTF-8", NULL, NULL, NULL);
  g_free( textutf8);

  g_object_set( ((WttGtk *)wtt)->india_widget, "visible", FALSE, NULL);

  (wtt->india_ok_cb)( wtt, text);
  g_free( text);
}

void WttGtk::activate_india_cancel( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;

  g_object_set( ((WttGtk *)wtt)->india_widget, "visible", FALSE, NULL);
}

void WttGtk::activate_confirm_ok( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;

  g_object_set( ((WttGtk *)wtt)->confirm_widget, "visible", FALSE, NULL);
  wtt->confirm_open = 0;

  if ( wtt->confirm_ok_cb)
    (wtt->confirm_ok_cb)( wtt);
}
void WttGtk::activate_confirm_no( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;

  g_object_set( ((WttGtk *)wtt)->confirm_widget, "visible", FALSE, NULL);
  wtt->confirm_open = 0;
  if ( wtt->confirm_no_cb)
    (wtt->confirm_no_cb)( wtt);
}
void WttGtk::activate_confirm_cancel( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;

  g_object_set( ((WttGtk *)wtt)->confirm_widget, "visible", FALSE, NULL);
  wtt->confirm_open = 0;
}

gboolean WttGtk::action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data)
{
  WttGtk *wtt = (WttGtk *)data;

  if ( wtt->focustimer.disabled()) {
    return FALSE;
  }

  if ( wtt->focused_wnav) {
    wtt->set_focus( wtt->focused_wnav);
  }
  wtt->focustimer.disable( 400);

  return FALSE;
}

void WttGtk::open_input_dialog( const char *text, const char *title,
			     const char *init_text,
			     void (*ok_cb)( Wtt *, char *))
{
  create_input_dialog();

  g_object_set( india_widget, 
		"visible", TRUE, 
		"title", title,
		NULL);

  gtk_label_set_text( GTK_LABEL(india_label), text);

  gint pos = 0;
  gtk_editable_delete_text( GTK_EDITABLE(india_text), 0, -1);
  gtk_editable_insert_text( GTK_EDITABLE(india_text), init_text, 
			    strlen(init_text), &pos);

  india_ok_cb = ok_cb;
}

void WttGtk::open_confirm( const char *text, const char *title, 
	void (*ok_cb)( Wtt *), void (*no_cb)( Wtt *))
{
  if ( confirm_open)  {
    g_object_set( confirm_widget, "visible", FALSE, NULL);
    confirm_open = 0;
    return;
  }

  create_confirm_dialog();

  message( ' ', "");

  g_object_set( confirm_widget, "title", title, NULL);
  g_object_set( confirm_label, "label", text, NULL);
  confirm_open = 1;
  confirm_ok_cb = ok_cb;
  confirm_no_cb = no_cb;
}

void WttGtk::activate_selmode( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  int set;

  if ( w == ((WttGtk *)wtt)->cm_normal_syntax) {
    set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
    if (!set) {
      return;
    }
    gtk_widget_set_sensitive( ((WttGtk *)wtt)->cm_add_type, TRUE);
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(((WttGtk *)wtt)->cm_add_type), FALSE);
    wtt->select_type = 0;
    gtk_widget_set_sensitive( ((WttGtk *)wtt)->cm_add_volume, TRUE);
    wtt->select_syntax = wnav_eSelectionMode_Normal;
  }
  if ( w == ((WttGtk *)wtt)->cm_gms_syntax) {
    set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
    if (!set) {
      return;
    }
    gtk_widget_set_sensitive( ((WttGtk *)wtt)->cm_add_type, TRUE);
    gtk_widget_set_sensitive( ((WttGtk *)wtt)->cm_add_volume, TRUE);
    wtt->select_syntax = wnav_eSelectionMode_GMS;
  }
  if ( w == ((WttGtk *)wtt)->cm_extern_syntax) {
    set = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
    if (!set) {
      return;
    }
    gtk_widget_set_sensitive( ((WttGtk *)wtt)->cm_add_type, FALSE);
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(((WttGtk *)wtt)->cm_add_type), FALSE);
    wtt->select_type = 0;
    gtk_widget_set_sensitive( ((WttGtk *)wtt)->cm_add_volume, FALSE);
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(((WttGtk *)wtt)->cm_add_volume), FALSE);
    wtt->select_syntax = wnav_eSelectionMode_Extern;
    wtt->select_volume = 0;
  }
  if ( w == ((WttGtk *)wtt)->cm_add_volume)
    wtt->select_volume = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
  if ( w == ((WttGtk *)wtt)->cm_add_attribute)
    wtt->select_attr = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
  if ( w == ((WttGtk *)wtt)->cm_add_type) {
    wtt->select_type = (int) gtk_check_menu_item_get_active( GTK_CHECK_MENU_ITEM(w));
    if ( wtt->select_type && !wtt->select_attr) {
      gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(((WttGtk *)wtt)->cm_add_attribute), TRUE);
      wtt->select_attr = 1;
    }
  }
}


// Open a Create Boot File window
void WttGtk::open_boot_window()
{
  int i;
  GtkListStore *store;
  GtkTreeIter iter;
  GtkCellRenderer *text_renderer;
  GtkTreeViewColumn *name_column;
  lfu_t_volumelist *volumelist_ptr;
  pwr_tString40 nodename;
  pwr_tStatus 	sts;

  // Load the bootlist
  sts = lfu_volumelist_load( load_cNameBootList, 
		(lfu_t_volumelist **) &boot_volumelist,
		&boot_volumecount);
  if (sts == FOE__NOFILE) {
    message( 'E', "Project is not configured");
    return;
  }
  else if (EVEN(sts)) {
    message( 'E', "Syntax error in bootlist file");
    return;
  }

  // Create the dialog
  boot_dia = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW,
					 "default-height", 200,
					 "default-width", 300,
					 "title", "Build Node",
					 "window-position", GTK_WIN_POS_CENTER,
					 NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(boot_delete_cb), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(boot_destroy_cb), this);
  store = gtk_list_store_new( 1, G_TYPE_STRING);

  strcpy( nodename, "");
  volumelist_ptr = (lfu_t_volumelist *) boot_volumelist;
  for ( i = 0; i < boot_volumecount; i++)
  {
    if ( strcmp(volumelist_ptr->p1, nodename))
    {
      strcpy( nodename, volumelist_ptr->p1);
      gtk_list_store_append( store, &iter);
      gtk_list_store_set( store, &iter, 0, nodename, -1);
    }
    volumelist_ptr++;
  }

  boot_list = (GtkWidget *) g_object_new(GTK_TYPE_TREE_VIEW,
					 "model", store,
					 "rules-hint", TRUE,
					 "headers-visible", FALSE,
					 "reorderable", TRUE,
					 "enable-search", TRUE,
					 "search-column", 0,
					 NULL);


  text_renderer = gtk_cell_renderer_text_new();
  name_column = gtk_tree_view_column_new_with_attributes( "",
							  text_renderer,
							  "text", 0,
							  NULL);
  g_object_set( name_column,
		"resizable", TRUE,
		"clickable", TRUE,
		NULL);

  gtk_tree_view_append_column( GTK_TREE_VIEW(boot_list), name_column);
  g_signal_connect( boot_list, "row-activated", 
 		    G_CALLBACK(WttGtk::boot_row_activated_cb), this);

  GtkWidget *ok_button = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( ok_button, 70, 25);
  g_signal_connect( ok_button, "clicked", 
 		    G_CALLBACK(WttGtk::boot_ok_cb), this);

  GtkWidget *cancel_button = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( cancel_button, 70, 25);
  g_signal_connect( cancel_button, "clicked", 
 		    G_CALLBACK(WttGtk::boot_cancel_cb), this);

  GtkWidget *hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(hboxbuttons), ok_button, FALSE, FALSE, 20);
  gtk_box_pack_end( GTK_BOX(hboxbuttons), cancel_button, FALSE, FALSE, 20);

  GtkWidget *scrolled_window = gtk_scrolled_window_new( NULL, NULL);
  gtk_container_add( GTK_CONTAINER( scrolled_window), boot_list);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
  gtk_box_pack_end( GTK_BOX(vbox), hboxbuttons, FALSE, FALSE, 10);

  gtk_container_add( GTK_CONTAINER(boot_dia), vbox);

  gtk_widget_show_all( boot_dia);

  // GtkTreeSelection *selection = gtk_tree_view_get_selection( GTK_TREE_VIEW(ctx->list));
  // gtk_tree_selection_set_mode( selection, GTK_SELECTION_SINGLE);

  // Set input focus to the scrolled list widget
  gtk_widget_grab_focus( boot_list);


}

// Callbacks for the Create Boot Files window

// Widget callbacks for Cancel button
void WttGtk::boot_cancel_cb(GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  gtk_widget_destroy( ((WttGtk *)wtt)->boot_dia);
}

// Delete callback
gint WttGtk::boot_delete_cb( GtkWidget *w, GdkEvent *event, gpointer data)
{
  return FALSE;
}

// Destroy callback
void WttGtk::boot_destroy_cb(GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;

  if ( wtt->boot_volumelist && wtt->boot_volumecount) {
    free( (char *) wtt->boot_volumelist);
    wtt->boot_volumelist = 0;
  }
  ((WttGtk *)wtt)->boot_dia = 0;
}

void WttGtk::boot_row_activated_cb( GtkTreeView *tree_view, 
				    GtkTreePath *path,
				    GtkTreeViewColumn *column,
				    gpointer data)
{
  boot_ok_cb( 0, data);
}

// Widget callbacks for OK button
void WttGtk::boot_ok_cb(GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  int j, nodecount;
  lfu_t_volumelist *volumelist_ptr;
  pwr_tString40 nodeconfigname;
  char msg[200];
  int bootfile_count;
  GtkTreeIter   iter;
  GtkTreeModel  *store;
  char *text;

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();

  wtt->message( ' ', "");

  nodecount = 0;
  bootfile_count = 0;
  volumelist_ptr = (lfu_t_volumelist *)wtt->boot_volumelist;

  g_object_get( ((WttGtk *)wtt)->boot_list, "model", &store, NULL);

  GtkTreeSelection *selection = gtk_tree_view_get_selection( 
					 GTK_TREE_VIEW(((WttGtk *)wtt)->boot_list));
  if ( gtk_tree_selection_get_selected( selection, NULL, &iter)) {
    gtk_tree_model_get( GTK_TREE_MODEL( store), &iter, 0, 
			&text, -1);
    strcpy( nodeconfigname, "");
    for ( j = 0; j < wtt->boot_volumecount; j++) {
      if ( strcmp(volumelist_ptr->p1, nodeconfigname) != 0) {
	nodecount++;

	strcpy( nodeconfigname, volumelist_ptr->p1);
	if ( strcmp( nodeconfigname, text) == 0) {
	  wb_build build( *(wb_session *)wtt->ldhses, wtt->focused_wnav);
	  build.opt = wtt->focused_wnav->gbl.build;
	  build.node( nodeconfigname, 
		      wtt->boot_volumelist, wtt->boot_volumecount);
	  if ( build.evenSts()) {
	    gtk_widget_destroy( ((WttGtk *)wtt)->boot_dia);
	    sprintf( msg, 
		     "Error creating bootfile for NodeConfig-object %s",
		     nodeconfigname);
	    wtt->message( 'E', msg);
	    return;
	  }
	  else if ( build.sts() != PWRB__NOBUILT)
	    bootfile_count++;
	}
      }
      volumelist_ptr++;
    }
  }

  gtk_widget_destroy(((WttGtk *)wtt)->boot_dia);
  if ( !bootfile_count) {
    wtt->message( 'E', "Nothing to build");
    return;
  }
  sprintf( msg, "Bootfile%s created", (bootfile_count == 1) ? "":"s");
  wtt->message( 'I', msg);
}

/************************************************************************
*
* Name: update_options_form
*
* Description: This routine must be called when the configurator/navigator is 
*	       managed.
* 
*************************************************************************/
void WttGtk::update_options_form()
{
  // Hierarchies
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(enable_comment_w), enable_comment ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_plant_w), wnav_mapped ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_node_w), wnavnode_mapped ? TRUE : FALSE);

  // entry components
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_class_w), show_class ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_alias_w), show_alias ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_descrip_w), show_descrip ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_objref_w), show_objref ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_objxref_w), show_objxref ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_attrref_w), show_attrref ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(show_attrxref_w), show_attrxref ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(build_force_w), build_force ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(build_debug_w), build_debug ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(build_crossref_w), build_crossref ? TRUE : FALSE);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(build_manual_w), build_manual ? TRUE : FALSE);
} 


/************************************************************************
*
* Name: set_options
*
*
*************************************************************************/
void WttGtk::set_options()
{
  enable_comment = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(enable_comment_w));
  show_class = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(show_class_w));
  show_alias = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(show_alias_w));
  show_descrip = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(show_descrip_w));
  show_objref = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(show_objref_w));
  show_objxref = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(show_objxref_w));
  show_attrref = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(show_attrref_w));
  show_attrxref = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(show_attrxref_w));
  build_force = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(build_force_w));
  build_debug = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(build_debug_w));
  build_crossref = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(build_crossref_w));
  build_manual = (int) gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(build_manual_w));

  wnav->set_options( enable_comment, show_class, show_alias, 
	show_descrip, show_objref, show_objxref, 
	show_attrref, show_attrxref, build_force, build_debug,
	build_crossref, build_manual);
  wnavnode->set_options( enable_comment, show_class, show_alias, 
	show_descrip, show_objref, show_objxref, 
        show_attrref, show_attrxref, build_force, build_debug,
	build_crossref, build_manual);
}

// Callbacks from the options form

void WttGtk::activate_options_ok( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  int plant, node;
  
  wtt->message( ' ', "");

  plant = (int) gtk_toggle_button_get_active( 
			  GTK_TOGGLE_BUTTON(((WttGtk *)wtt)->show_plant_w));
  node = (int) gtk_toggle_button_get_active( 
                          GTK_TOGGLE_BUTTON(((WttGtk *)wtt)->show_node_w));

  g_object_set( ((WttGtk *)wtt)->options_form, "visible", FALSE, NULL);
  wtt->set_twowindows( 0, plant, node);
  wtt->set_options();
}

void WttGtk::activate_options_apply( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  int plant, node;
  
  wtt->message( ' ', "");

  plant = (int) gtk_toggle_button_get_active( 
			  GTK_TOGGLE_BUTTON(((WttGtk *)wtt)->show_plant_w));
  node = (int) gtk_toggle_button_get_active( 
                          GTK_TOGGLE_BUTTON(((WttGtk *)wtt)->show_node_w));

  wtt->set_twowindows( 0, plant, node);
  wtt->set_options();
}

void WttGtk::activate_options_cancel( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;

  wtt->message( ' ', "");
  wtt->update_options_form();
  g_object_set( ((WttGtk *)wtt)->options_form, "visible", FALSE, NULL);
}

static gint delete_event( GtkWidget *w, GdkEvent *event, gpointer wtt)
{
  WttGtk::activate_exit(w, wtt);

  return TRUE;
}

static void destroy_event( GtkWidget *w, gpointer data)
{
}

WttGtk::WttGtk( 
	void	*wt_parent_ctx,
	GtkWidget *wt_parent_wid,
	const char 	*wt_name,
	const char	*iconname,
	void	*wt_wbctx,
	pwr_tVolumeId wt_volid,
	ldh_tVolume wt_volctx,
	wnav_sStartMenu *root_menu,
	pwr_tStatus *status
	) :
  Wtt(wt_parent_ctx, wt_name, iconname, wt_wbctx, wt_volid, wt_volctx, root_menu, status),
  parent_wid( wt_parent_wid), india_widget(0), confirm_widget(0), boot_dia(0), options_form(0),
  set_focus_disabled(0), disfocus_timerid(0), selection_timerid(0), avoid_deadlock(0),
  clock_cursor(0), realized(0)
{
  int		window_width = 900;
  int    	window_height = 800;
  // int    	palette_width = 220;
  pwr_tStatus	sts;
  char 		title[80];
  char		volname[80];
  pwr_tClassId	volclass;
  int		size;
  char		layout_w1[80];
  char		layout_w2[80];
  char		layout_palette[80];
  char		title_w1[40];
  char		title_w2[40];
  pwr_tFileName fname;

  if ( wbctx && volid) {
    // Get the volume class and decide what type of navigator */
    sts = ldh_GetVolumeClass( wbctx, volid, &volclass);
    if (EVEN(sts)) {
      *status = sts;
      return;
    }
    if ( volid == ldh_cDirectoryVolume) volclass = pwr_eClass_DirectoryVolume; // Fix

    sts = ldh_VolumeIdToName( wbctx, volid, volname,
		sizeof(volname), &size);
    if (EVEN(sts)) {
      *status = sts;
      return;
    }
    if ( !volctx) {
      sts = ldh_AttachVolume( wbctx, volid, &volctx);
      if (EVEN(sts)) {
        printf("-- Volume '%s' is already attached\n", volname);
        putchar( '\7' );
        *status = sts;
        return;
      }
    }

    sts = ldh_OpenSession( &ldhses, volctx, ldh_eAccess_ReadOnly,
    	ldh_eUtility_Navigator);
    if (EVEN(sts)) {
      printf("Navigator: Can't open session\n");
      *status = sts;
      return;
    }
    ldh_AddOtherSessionCallback( ldhses,  (void *)this, 
		Wtt::ldh_other_session_cb);

    switch( volclass) {
    case pwr_eClass_DirectoryVolume:
      wb_type = wb_eType_Directory;
      sprintf( title, "PwR Directory,   %s", name);
      strcpy( layout_w1, "ProjectNavigatorW1");
      strcpy( layout_w2, "ProjectNavigatorW2");
      strcpy( layout_palette, "ProjectNavigatorPalette");
      strcpy( title_w1, "Volume Configuration");
      strcpy( title_w2, "Node Configuration");
      window_width = 900;
      window_height = 400;
      break;
    case pwr_eClass_ClassVolume:
    case pwr_eClass_DetachedClassVolume:
      if ( ldh_VolRepType( ldhses) == ldh_eVolRep_Mem ||
	   ldh_VolRepType( ldhses) == ldh_eVolRep_Ced) {
	wb_type = wb_eType_ClassEditor;
	sprintf( title, "PwR ClassEditor %s,   %s", volname, name);
      }
      else {
	wb_type = wb_eType_Class;
	sprintf( title, "PwR %s,   %s", volname, name);
      }
      strcpy( layout_w1, "ClassNavigatorW1");
      strcpy( layout_w2, "ClassNavigatorW2");
      strcpy( layout_palette, "ClassNavigatorPalette");
      strcpy( title_w1, "Class Configuration");
      strcpy( title_w2, "Node Configuration");
      break;
    case pwr_eClass_VolatileVolume:
      wb_type = wb_eType_Buffer;
      strcpy( layout_w1, "");
      strcpy( layout_w2, "");
      strcpy( layout_palette, "NavigatorPalette");
      strcpy( title_w1, "Plant Configuration");
      strcpy( title_w2, "Node Configuration");
      sprintf( title, "PwR Buffer %s,   %s", volname, name);
      window_width = 500;
      window_height = 400;
      disable_w2 = 1;
      break;
    case pwr_eClass_ExternVolume: {
      switch ( volid) {
      case ldh_cProjectListVolume:
	wb_type = wb_eType_ExternVolume;
	strcpy( layout_w1, "PrListNavigatorW1");
	strcpy( layout_w2, "PrListNavigatorW1");
	strcpy( layout_palette, "PrListNavigatorPalette");
	strcpy( title_w1, "Project List");
	strcpy( title_w2, "");
	sprintf( title, "PwR Project List");
	window_width = 500;
	window_height = 400;
	disable_w2 = 1;
	break;
      case ldh_cGlobalVolumeListVolume:
	wb_type = wb_eType_ExternVolume;
	strcpy( layout_w1, "GvListNavigatorW1");
	strcpy( layout_w2, "GvListNavigatorW1");
	strcpy( layout_palette, "GvListNavigatorPalette");
	strcpy( title_w1, "Global Volume List");
	strcpy( title_w2, "");
	sprintf( title, "PwR Global Volume List");
	window_width = 500;
	window_height = 400;
	disable_w2 = 1;
	break;
      case ldh_cUserDatabaseVolume:
	wb_type = wb_eType_ExternVolume;
	strcpy( layout_w1, "UserDbNavigatorW1");
	strcpy( layout_w2, "UserDbNavigatorW1");
	strcpy( layout_palette, "UserDbNavigatorPalette");
	strcpy( title_w1, "User Database");
	strcpy( title_w2, "");
	sprintf( title, "PwR User Database");
	window_width = 500;
	window_height = 400;
	disable_w2 = 1;
	break;
      default:
	wb_type = wb_eType_ExternVolume;
	strcpy( layout_w1, "NavigatorW1");
	strcpy( layout_w2, "NavigatorW2");
	strcpy( layout_palette, "NavigatorPalette");
	strcpy( title_w1, "Plant Configuration");
	strcpy( title_w2, "Node Configuration");
	sprintf( title, "PwR %s,   %s", volname, name);
      }
      break;
    }
    default:
      wb_type = wb_eType_Volume;
      strcpy( layout_w1, "NavigatorW1");
      strcpy( layout_w2, "NavigatorW2");
      strcpy( layout_palette, "NavigatorPalette");
      strcpy( title_w1, "Plant Configuration");
      strcpy( title_w2, "Node Configuration");
      sprintf( title, "PwR %s,   %s", volname, name);
    }
  }
  else {
    strcpy( layout_w1, "NavigatorW1");
    strcpy( layout_w2, "NavigatorW2");
    strcpy( title_w1, "Plant Configuration");
    strcpy( title_w2, "Node Configuration");
    strcpy( title, "PwR Wtt");
  }

  // Gtk
  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					 "default-height", window_height,
					 "default-width", window_width,
					 "title", title,
					 NULL);

  g_signal_connect( toplevel, "delete_event", G_CALLBACK(delete_event), this);
  g_signal_connect( toplevel, "destroy", G_CALLBACK(destroy_event), this);
  g_signal_connect( toplevel, "focus-in-event", G_CALLBACK(WttGtk::action_inputfocus), this);

  CoWowGtk::SetWindowIcon( toplevel);

  GtkAccelGroup *accel_g = (GtkAccelGroup *) g_object_new(GTK_TYPE_ACCEL_GROUP, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(toplevel), accel_g);

  GtkMenuBar *menu_bar = (GtkMenuBar *) g_object_new(GTK_TYPE_MENU_BAR, NULL);

  // File Entry
  GtkWidget *file_open_volume = gtk_menu_item_new_with_mnemonic( "_Open Volume");
  g_signal_connect( file_open_volume, "activate", 
		    G_CALLBACK(WttGtk::activate_openvolume), this);

  GtkWidget *file_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, accel_g);
  g_signal_connect(file_close, "activate", G_CALLBACK(WttGtk::activate_exit), this);

  GtkWidget *file_open_dbsfile = gtk_menu_item_new_with_mnemonic( "_Dbs File...");
  g_signal_connect( file_open_dbsfile, "activate", 
		    G_CALLBACK(WttGtk::activate_openfile_dbs), this);

  GtkWidget *file_open_wblfile = gtk_menu_item_new_with_mnemonic( "_Wbl File...");
  g_signal_connect( file_open_wblfile, "activate", 
		    G_CALLBACK(WttGtk::activate_openfile_wbl), this);

  GtkWidget *file_open_buffer = gtk_menu_item_new_with_mnemonic( "_Buffer...");
  g_signal_connect( file_open_buffer, "activate", 
		    G_CALLBACK(WttGtk::activate_openbuffer), this);

  GtkWidget *file_open_pl = gtk_menu_item_new_with_mnemonic( "_ProjectList");
  g_signal_connect( file_open_pl, "activate", 
		    G_CALLBACK(WttGtk::activate_openpl), this);

  GtkWidget *file_open_gvl = gtk_menu_item_new_with_mnemonic( "_GlobalVolumeList");
  g_signal_connect( file_open_gvl, "activate", 
		    G_CALLBACK(WttGtk::activate_opengvl), this);

  GtkWidget *file_open_udb = gtk_menu_item_new_with_mnemonic( "_UserDataBase");
  g_signal_connect( file_open_udb, "activate", 
		    G_CALLBACK(WttGtk::activate_openudb), this);

  // Submenu Open
  GtkWidget *file_open = gtk_menu_item_new_with_mnemonic( "_Open");
  GtkMenu *file_open_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_open_menu), file_open_dbsfile);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_open_menu), file_open_wblfile);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_open_menu), file_open_buffer);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_open_menu), file_open_pl);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_open_menu), file_open_gvl);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_open_menu), file_open_udb);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_open),
			    GTK_WIDGET(file_open_menu));

  GtkWidget *file_volume_attr = gtk_menu_item_new_with_mnemonic( "Volume _Attributes...");
  g_signal_connect( file_volume_attr, "activate", 
		    G_CALLBACK(WttGtk::activate_openvolobject), this);

  GtkWidget *file_print = gtk_image_menu_item_new_from_stock(GTK_STOCK_PRINT, accel_g);
  g_signal_connect(file_print, "activate", G_CALLBACK(WttGtk::activate_print), this);

  menu_save_w = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, accel_g);
  g_signal_connect(menu_save_w, "activate", G_CALLBACK(WttGtk::activate_save), this);

  menu_revert_w = gtk_image_menu_item_new_from_stock(GTK_STOCK_REVERT_TO_SAVED,
							      accel_g);
  g_signal_connect(menu_revert_w, "activate", G_CALLBACK(WttGtk::activate_revert), this);

  GtkWidget *file_history = gtk_menu_item_new_with_mnemonic( "_History");
  g_signal_connect(file_history, "activate", G_CALLBACK(WttGtk::activate_history), this);

  GtkMenu *file_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_open_volume);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_open);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_volume_attr);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_print);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_save_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_revert_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_history);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_close);

  GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));

  // Edit Entry
  menu_edit_w = gtk_menu_item_new_with_mnemonic( "_Edit Mode");
  g_signal_connect( menu_edit_w, "activate", 
		    G_CALLBACK(WttGtk::activate_configure), this);
  gtk_widget_add_accelerator( menu_edit_w, "activate", accel_g,
			      'e', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  // Submenu Search
  GtkWidget *edit_search_findobject = gtk_menu_item_new_with_mnemonic( "_Find Object");
  g_signal_connect( edit_search_findobject, "activate", 
		    G_CALLBACK(WttGtk::activate_find), this);
  gtk_widget_add_accelerator( edit_search_findobject, "activate", accel_g,
			      'f', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *edit_search_findregex = gtk_menu_item_new_with_mnemonic( "Find _Regular expression");
  g_signal_connect( edit_search_findregex, "activate", 
		    G_CALLBACK(WttGtk::activate_findregex), this);

  GtkWidget *edit_search_findnext = gtk_menu_item_new_with_mnemonic( "Find _Next");
  g_signal_connect( edit_search_findnext, "activate", 
		    G_CALLBACK(WttGtk::activate_findnext), this);
  gtk_widget_add_accelerator( edit_search_findnext, "activate", accel_g,
			      'f', GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *edit_search = gtk_menu_item_new_with_mnemonic( "_Search");
  GtkMenu *edit_search_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_search_menu), edit_search_findobject);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_search_menu), edit_search_findregex);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_search_menu), edit_search_findnext);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_search),
			    GTK_WIDGET(edit_search_menu));

  // Submenu CopyMode
  GSList *cm_group = NULL;
  cm_normal_syntax = gtk_radio_menu_item_new_with_mnemonic( cm_group, "_Normal Syntax");
  cm_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(cm_normal_syntax));
  g_signal_connect( cm_normal_syntax, "activate", 
		    G_CALLBACK(WttGtk::activate_selmode), this);

  cm_gms_syntax = gtk_radio_menu_item_new_with_mnemonic( cm_group, "_GMS Syntax");
  cm_group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(cm_gms_syntax));
  g_signal_connect( cm_gms_syntax, "activate", 
		    G_CALLBACK(WttGtk::activate_selmode), this);

  cm_extern_syntax = gtk_radio_menu_item_new_with_mnemonic( cm_group, "_Extern Syntax");
  g_signal_connect( cm_extern_syntax, "activate", 
		    G_CALLBACK(WttGtk::activate_selmode), this);

  cm_add_volume = gtk_check_menu_item_new_with_mnemonic( "Add _Volume");
  g_signal_connect( cm_add_volume, "activate", 
		    G_CALLBACK(WttGtk::activate_selmode), this);
  cm_add_attribute = gtk_check_menu_item_new_with_mnemonic( "_Add Attribute");
  g_signal_connect( cm_add_attribute, "activate", 
		    G_CALLBACK(WttGtk::activate_selmode), this);

  cm_add_type = gtk_check_menu_item_new_with_mnemonic( "Add _Type");
  g_signal_connect( cm_add_type, "activate", 
		    G_CALLBACK(WttGtk::activate_selmode), this);

  GtkWidget *edit_cm = gtk_menu_item_new_with_mnemonic( "_CopyMode");
  GtkMenu *edit_cm_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_cm_menu), cm_normal_syntax);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_cm_menu), cm_gms_syntax);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_cm_menu), cm_extern_syntax);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_cm_menu), gtk_separator_menu_item_new());
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_cm_menu), cm_add_volume);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_cm_menu), cm_add_attribute);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_cm_menu), cm_add_type);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_cm),
			    GTK_WIDGET(edit_cm_menu));

  // Submenu Create Object
  menu_creaobjafter_w = gtk_menu_item_new_with_mnemonic( "_After");
  g_signal_connect( menu_creaobjafter_w, "activate", 
		    G_CALLBACK(WttGtk::activate_creaobjafter), this);
  gtk_widget_add_accelerator( menu_creaobjafter_w, "activate", accel_g,
			      'd', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  menu_creaobjfirst_w = gtk_menu_item_new_with_mnemonic( "_First Child");
  g_signal_connect( menu_creaobjfirst_w, "activate", 
		    G_CALLBACK(WttGtk::activate_creaobjfirst), this);
  gtk_widget_add_accelerator( menu_creaobjfirst_w, "activate", accel_g,
			      'd', GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *edit_creaobj = gtk_menu_item_new_with_mnemonic( "C_reate Object");
  GtkMenu *edit_creaobj_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_creaobj_menu), menu_creaobjafter_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_creaobj_menu), menu_creaobjfirst_w);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_creaobj),
			    GTK_WIDGET(edit_creaobj_menu));

  // Submenu Move Object
  menu_moveobjup_w = gtk_menu_item_new_with_mnemonic( "_Up");
  g_signal_connect( menu_moveobjup_w, "activate", 
		    G_CALLBACK(WttGtk::activate_moveobjup), this);
  gtk_widget_add_accelerator( menu_moveobjup_w, "activate", accel_g,
			      GDK_Up, GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  menu_moveobjdown_w = gtk_menu_item_new_with_mnemonic( "_Down");
  g_signal_connect( menu_moveobjdown_w, "activate", 
		    G_CALLBACK(WttGtk::activate_moveobjdown), this);
  gtk_widget_add_accelerator( menu_moveobjdown_w, "activate", accel_g,
			      GDK_Down, GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  menu_moveobjinto_w = gtk_menu_item_new_with_mnemonic( "_Into");
  g_signal_connect( menu_moveobjinto_w, "activate", 
		    G_CALLBACK(WttGtk::activate_moveobjinto), this);
  gtk_widget_add_accelerator( menu_moveobjinto_w, "activate", accel_g,
			      GDK_Right, GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  menu_moveobjontop_w = gtk_menu_item_new_with_mnemonic( "_OnTop");
  g_signal_connect( menu_moveobjontop_w, "activate",
		    G_CALLBACK(WttGtk::activate_moveobjontop), this);
  gtk_widget_add_accelerator( menu_moveobjontop_w, "activate", accel_g,
			      GDK_Left, GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *edit_moveobj = gtk_menu_item_new_with_mnemonic( "_Move Object");
  GtkMenu *edit_moveobj_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_moveobj_menu), menu_moveobjup_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_moveobj_menu), menu_moveobjdown_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_moveobj_menu), menu_moveobjinto_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_moveobj_menu), menu_moveobjontop_w);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_moveobj),
			    GTK_WIDGET(edit_moveobj_menu));

  menu_deleteobj_w = gtk_menu_item_new_with_mnemonic( "_Delete Object");
  g_signal_connect( menu_deleteobj_w, "activate", 
		    G_CALLBACK(WttGtk::activate_deleteobj), this);
  gtk_widget_add_accelerator( menu_deleteobj_w, "activate", accel_g,
			      GDK_Delete, GdkModifierType(0), GTK_ACCEL_VISIBLE);

  GtkWidget *edit_collapse = gtk_menu_item_new_with_mnemonic( "Co_llapse");
  g_signal_connect( edit_collapse, "activate", 
		    G_CALLBACK(WttGtk::activate_collapse), this);

  menu_cut_w = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, accel_g);
  g_signal_connect(menu_cut_w, "activate", G_CALLBACK(WttGtk::activate_cut), this);

  menu_copy_w = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, accel_g);
  g_signal_connect(menu_copy_w, "activate", G_CALLBACK(WttGtk::activate_copy), this);

  menu_copykeep_w = gtk_menu_item_new_with_mnemonic( "Copy Keep Reference");
  g_signal_connect( menu_copykeep_w, "activate", 
		    G_CALLBACK(WttGtk::activate_copykeep), this);

  menu_paste_w = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, accel_g);
  g_signal_connect(menu_paste_w, "activate", G_CALLBACK(WttGtk::activate_paste), this);

  menu_pasteinto_w = gtk_menu_item_new_with_mnemonic( "Paste Into");
  g_signal_connect( menu_pasteinto_w, "activate", 
		    G_CALLBACK(WttGtk::activate_pasteinto), this);
  gtk_widget_add_accelerator( menu_pasteinto_w, "activate", accel_g,
			      'v', GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  menu_rename_w = gtk_menu_item_new_with_mnemonic( "Rename");
  g_signal_connect( menu_rename_w, "activate", 
		    G_CALLBACK(WttGtk::activate_rename), this);
  gtk_widget_add_accelerator( menu_rename_w, "activate", accel_g,
			      'n', GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkMenu *edit_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), menu_edit_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_search);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_cm);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_creaobj);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_moveobj);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), menu_deleteobj_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit_collapse);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), menu_cut_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), menu_copy_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), menu_copykeep_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), menu_paste_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), menu_pasteinto_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), menu_rename_w);

  GtkWidget *edit = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), GTK_WIDGET(edit_menu));

  // Functions menu
  menu_buildnode_w = gtk_image_menu_item_new_with_mnemonic( "_Build Node");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(menu_buildnode_w), 
				 gtk_image_new_from_stock( "gtk-execute", GTK_ICON_SIZE_MENU));
  g_signal_connect( menu_buildnode_w, "activate", 
		    G_CALLBACK(WttGtk::activate_buildnode), this);
  gtk_widget_add_accelerator( menu_buildnode_w, "activate", accel_g,
			      'b', GdkModifierType(GDK_CONTROL_MASK | GDK_SHIFT_MASK), 
			      GTK_ACCEL_VISIBLE);

  menu_buildvolume_w = gtk_menu_item_new_with_mnemonic( "B_uild Volume");
  g_signal_connect( menu_buildvolume_w, "activate", 
		    G_CALLBACK(WttGtk::activate_buildvolume), this);

  menu_buildobject_w = gtk_menu_item_new_with_mnemonic( "Bu_ild Object");
  g_signal_connect( menu_buildobject_w, "activate", 
		    G_CALLBACK(WttGtk::activate_buildobject), this);

  menu_utilities_w = gtk_menu_item_new_with_mnemonic( "_Utilities...");
  g_signal_connect( menu_utilities_w, "activate", 
		    G_CALLBACK(WttGtk::activate_utilities), this);
  gtk_widget_add_accelerator( menu_utilities_w, "activate", accel_g,
			      'u', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_spreadsheet = gtk_menu_item_new_with_mnemonic( "_Spreadsheet...");
  g_signal_connect( functions_spreadsheet, "activate", 
		    G_CALLBACK(WttGtk::activate_spreadsheet), this);

  menu_distribute_w = gtk_menu_item_new_with_mnemonic( "_Distribute...");
  g_signal_connect( menu_distribute_w, "activate", 
		    G_CALLBACK(WttGtk::activate_distribute), this);

  menu_utilities_w = gtk_menu_item_new_with_mnemonic( "_Utilities...");
  g_signal_connect( menu_utilities_w, "activate", 
		    G_CALLBACK(WttGtk::activate_utilities), this);
  gtk_widget_add_accelerator( menu_utilities_w, "activate", accel_g,
			      'u', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_openobject = gtk_menu_item_new_with_mnemonic( "_Open Object...");
  g_signal_connect( functions_openobject, "activate", 
		    G_CALLBACK(WttGtk::activate_openobject), this);
  gtk_widget_add_accelerator( functions_openobject, "activate", accel_g,
			      'a', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  menu_openplc_w = gtk_menu_item_new_with_mnemonic( "Open _Program");
  g_signal_connect( menu_openplc_w, "activate", 
		    G_CALLBACK(WttGtk::activate_openplc), this);
  gtk_widget_add_accelerator(  menu_openplc_w, "activate", accel_g,
			      'l', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_openge = gtk_menu_item_new_with_mnemonic( "Open _Ge...");
  g_signal_connect( functions_openge, "activate", 
		    G_CALLBACK(WttGtk::activate_openge), this);
  gtk_widget_add_accelerator( functions_openge, "activate", accel_g,
			      'k', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  menu_classeditor_w = gtk_menu_item_new_with_mnemonic( "Open _ClassEditor");
  g_signal_connect( menu_classeditor_w, "activate", 
		    G_CALLBACK(WttGtk::activate_openclasseditor), this);

  menu_updateclasses_w = gtk_menu_item_new_with_mnemonic( "U_pdate Classes");
  g_signal_connect( menu_updateclasses_w, "activate", 
		    G_CALLBACK(WttGtk::activate_updateclasses), this);

  GtkWidget *functions_showcrossref = gtk_menu_item_new_with_mnemonic( "Show C_rossreferences");
  g_signal_connect( functions_showcrossref, "activate", 
		    G_CALLBACK(WttGtk::activate_showcrossref), this);
  gtk_widget_add_accelerator( functions_showcrossref, "activate", accel_g,
			      'r', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_syntax = gtk_menu_item_new_with_mnemonic( "S_yntax Check");
  g_signal_connect(functions_syntax, "activate", G_CALLBACK(WttGtk::activate_syntax), this);

  menu_change_value_w = gtk_menu_item_new_with_mnemonic( "Change _Value");
  g_signal_connect( menu_change_value_w, "activate", 
		    G_CALLBACK(WttGtk::activate_change_value), this);
  gtk_widget_add_accelerator(  menu_change_value_w, "activate", accel_g,
			      'q', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *functions_command = gtk_menu_item_new_with_mnemonic( "Co_mmand");
  g_signal_connect( functions_command, "activate", 
		    G_CALLBACK(WttGtk::activate_command), this);
  gtk_widget_add_accelerator( functions_command, "activate", accel_g,
			      'b', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkMenu *functions_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_buildnode_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_buildvolume_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_buildobject_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_utilities_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_spreadsheet);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_distribute_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_openobject);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_openplc_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_openge);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_classeditor_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_updateclasses_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_showcrossref);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_syntax);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), menu_change_value_w);
  gtk_menu_shell_append(GTK_MENU_SHELL(functions_menu), functions_command);

  GtkWidget *functions = gtk_menu_item_new_with_mnemonic("F_unctions");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), functions);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(functions), GTK_WIDGET(functions_menu));

  // View menu
  GtkWidget *view_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
  g_signal_connect(view_zoom_in, "activate", G_CALLBACK(WttGtk::activate_zoom_in), this);
  gtk_widget_add_accelerator( view_zoom_in, "activate", accel_g,
			      'i', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
  g_signal_connect(view_zoom_out, "activate", G_CALLBACK(WttGtk::activate_zoom_out), this);
  gtk_widget_add_accelerator( view_zoom_out, "activate", accel_g,
			      'o', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *view_zoom_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
  g_signal_connect(view_zoom_reset, "activate", G_CALLBACK(WttGtk::activate_zoom_reset), this);

  GtkWidget *view_twowindows = gtk_menu_item_new_with_mnemonic( "T_wo Windows");
  g_signal_connect( view_twowindows, "activate", 
		    G_CALLBACK(WttGtk::activate_twowindows), this);
  gtk_widget_add_accelerator( view_twowindows, "activate", accel_g,
			      't', GdkModifierType(GDK_CONTROL_MASK), 
			      GTK_ACCEL_VISIBLE);

  GtkWidget *view_messages = gtk_menu_item_new_with_mnemonic( "_Messages");
  g_signal_connect( view_messages, "activate", 
		    G_CALLBACK(WttGtk::activate_messages), this);


  GtkMenu *view_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_in);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_out);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_zoom_reset);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_twowindows);
  gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_messages);

  GtkWidget *view = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));


  // Options menu
  GtkWidget *options_settings = gtk_menu_item_new_with_mnemonic( "_Settings...");
  g_signal_connect( options_settings, "activate", 
		    G_CALLBACK(WttGtk::activate_view), this);

  GtkWidget *options_savesettings = gtk_menu_item_new_with_mnemonic( "S_ave Settings");
  g_signal_connect( options_savesettings, "activate", 
		    G_CALLBACK(WttGtk::activate_savesettings), this);

  GtkWidget *options_restoresettings = gtk_menu_item_new_with_mnemonic( "_Restore Settings");
  g_signal_connect( options_restoresettings, "activate", 
		    G_CALLBACK(WttGtk::activate_restoresettings), this);

  // Submenu Script
  GtkWidget *options_script_proj = gtk_menu_item_new_with_mnemonic( "_Project...");
  g_signal_connect( options_script_proj, "activate", 
		    G_CALLBACK(WttGtk::activate_scriptproj), this);

  GtkWidget *options_script_base = gtk_menu_item_new_with_mnemonic( "_Base...");
  g_signal_connect( options_script_base, "activate", 
		    G_CALLBACK(WttGtk::activate_scriptbase), this);

  GtkWidget *options_script = gtk_menu_item_new_with_mnemonic( "_Script");
  GtkMenu *options_script_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(options_script_menu), options_script_proj);
  gtk_menu_shell_append(GTK_MENU_SHELL(options_script_menu), options_script_base);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(options_script),
			    GTK_WIDGET(options_script_menu));

  GtkMenu *options_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(options_menu), options_settings);
  gtk_menu_shell_append(GTK_MENU_SHELL(options_menu), options_savesettings);
  gtk_menu_shell_append(GTK_MENU_SHELL(options_menu), options_restoresettings);
  gtk_menu_shell_append(GTK_MENU_SHELL(options_menu), options_script);

  GtkWidget *options = gtk_menu_item_new_with_mnemonic("_Options");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), options);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(options), GTK_WIDGET(options_menu));

  // Tools menu
  GtkWidget *mtools_rtmon = gtk_menu_item_new_with_mnemonic( "_Runtime Monitor");
  g_signal_connect( mtools_rtmon, "activate", 
		    G_CALLBACK(WttGtk::activate_rtmon), this);

  GtkWidget *mtools_statusmon = gtk_menu_item_new_with_mnemonic( "_Supervision Center");
  g_signal_connect( mtools_statusmon, "activate", 
		    G_CALLBACK(WttGtk::activate_statusmon), this);

  GtkMenu *mtools_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(mtools_menu), mtools_rtmon);
  gtk_menu_shell_append(GTK_MENU_SHELL(mtools_menu), mtools_statusmon);

  GtkWidget *mtools = gtk_menu_item_new_with_mnemonic("_Tools");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), mtools);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(mtools), GTK_WIDGET(mtools_menu));

  // Menu Help
  GtkWidget *help_overview = gtk_image_menu_item_new_with_mnemonic("_Overview");
  gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(help_overview), 
				 gtk_image_new_from_stock( "gtk-help", GTK_ICON_SIZE_MENU));
  g_signal_connect(help_overview, "activate", G_CALLBACK(WttGtk::activate_help), this);
  gtk_widget_add_accelerator( help_overview, "activate", accel_g,
			      'h', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

  GtkWidget *help_project = gtk_menu_item_new_with_mnemonic( "_Project");
  g_signal_connect( help_project, "activate", 
		    G_CALLBACK(WttGtk::activate_help_project), this);

  GtkWidget *help_proview = gtk_menu_item_new_with_mnemonic( "_About Proview");
  g_signal_connect( help_proview, "activate", 
		    G_CALLBACK(WttGtk::activate_help_proview), this);

  GtkMenu *help_menu = (GtkMenu *) g_object_new( GTK_TYPE_MENU, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_overview);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_project);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), help_proview);

  GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));

  // Toolbar
  GtkToolbar *tools = (GtkToolbar *) g_object_new(GTK_TYPE_TOOLBAR, NULL);

  tools_save_w = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_save_w), 
	  gtk_image_new_from_stock( "gtk-save", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_save_w, "clicked", G_CALLBACK(WttGtk::activate_save), this);
  g_object_set( tools_save_w, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_save_w, "Save", "");

  tools_edit_w = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/foe_edit.png");
  gtk_container_add( GTK_CONTAINER(tools_edit_w), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_edit_w, "clicked", G_CALLBACK(WttGtk::activate_configure), this);
  g_object_set( tools_edit_w, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_edit_w, "Edit", "");

  tools_buildnode_w = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_buildnode_w), 
	  gtk_image_new_from_stock( "gtk-execute", GTK_ICON_SIZE_SMALL_TOOLBAR));
  if ( wb_type != wb_eType_ClassEditor) {
    g_signal_connect(tools_buildnode_w, "clicked", G_CALLBACK(WttGtk::activate_buildnode), this);
    gtk_toolbar_append_widget( tools, tools_buildnode_w, "Build Node", "");
  }
  else {
    g_signal_connect(tools_buildnode_w, "clicked", G_CALLBACK(WttGtk::activate_buildvolume), this);
    gtk_toolbar_append_widget( tools, tools_buildnode_w, "Build Volume", "");
  }
  g_object_set( tools_buildnode_w, "can-focus", FALSE, NULL);

  GtkWidget *tools_distribute = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/wpkg_distribute.png");
  gtk_container_add( GTK_CONTAINER(tools_distribute), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_distribute, "clicked", G_CALLBACK(WttGtk::activate_distribute), this);
  g_object_set( tools_distribute, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_distribute, "Distribute", "");

  GtkWidget *tools_openplc = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/wtt_program.png");
  gtk_container_add( GTK_CONTAINER(tools_openplc), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_openplc, "clicked", G_CALLBACK(WttGtk::activate_openplc), this);
  g_object_set( tools_openplc, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_openplc, "Open Program", "");

  GtkWidget *tools_openge = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/wtt_ge.png");
  gtk_container_add( GTK_CONTAINER(tools_openge), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_openge, "clicked", G_CALLBACK(WttGtk::activate_openge), this);
  g_object_set( tools_openge, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_openge, "Open Ge", "");

  GtkWidget *tools_utilities = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/wtt_utilities.png");
  gtk_container_add( GTK_CONTAINER(tools_utilities), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_utilities, "clicked", G_CALLBACK(WttGtk::activate_utilities), this);
  g_object_set( tools_utilities, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_utilities, "Utilities", "");

  GtkWidget *tools_twowindows = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/wtt_twowindows.png");
  gtk_container_add( GTK_CONTAINER(tools_twowindows), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_twowindows, "clicked", G_CALLBACK(WttGtk::activate_twowindows), this);
  g_object_set( tools_twowindows, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_twowindows, "Two windows/One window", "");

  GtkWidget *tools_messages = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/wtt_messages.png");
  gtk_container_add( GTK_CONTAINER(tools_messages), 
	  gtk_image_new_from_file( fname));
  g_signal_connect(tools_messages, "clicked", G_CALLBACK(WttGtk::activate_messages), this);
  g_object_set( tools_messages, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_messages, "Show Messages", "");

  GtkWidget *tools_options = gtk_button_new();
  gtk_container_add( GTK_CONTAINER(tools_options), 
	  gtk_image_new_from_stock( "gtk-preferences", GTK_ICON_SIZE_SMALL_TOOLBAR));
  g_signal_connect(tools_options, "clicked", G_CALLBACK(WttGtk::activate_view), this);
  g_object_set( tools_options, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_options, "Options", "");

  GtkWidget *tools_zoom_in = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_in.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_in), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_in, "clicked", G_CALLBACK(WttGtk::activate_zoom_in), this);
  g_object_set( tools_zoom_in, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_in, "Zoom in", "");

  GtkWidget *tools_zoom_out = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_out.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_out), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_out, "clicked", G_CALLBACK(WttGtk::activate_zoom_out), this);
  g_object_set( tools_zoom_out, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_out, "Zoom out", "");

  GtkWidget *tools_zoom_reset = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_zoom_reset.png");
  gtk_container_add( GTK_CONTAINER(tools_zoom_reset), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_zoom_reset, "clicked", G_CALLBACK(WttGtk::activate_zoom_reset), this);
  g_object_set( tools_zoom_reset, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_zoom_reset, "Zoom reset", "");

  GtkWidget *tools_set_advuser = gtk_button_new();
  dcli_translate_filename( fname, "$pwr_exe/xtt_advuser.png");
  gtk_container_add( GTK_CONTAINER(tools_set_advuser), 
		     gtk_image_new_from_file( fname));
  g_signal_connect(tools_set_advuser, "clicked", G_CALLBACK(WttGtk::activate_set_advuser), this);
  g_object_set( tools_set_advuser, "can-focus", FALSE, NULL);
  gtk_toolbar_append_widget( tools, tools_set_advuser, "Advanced user", "");

  // Vertical palette pane
  palette_paned = gtk_hpaned_new();
  wnav_paned = gtk_hpaned_new();

  gtk_paned_pack2( GTK_PANED(palette_paned), wnav_paned, TRUE, TRUE);

  palette = new PalGtk( this, palette_paned, "Objects",
		ldhses, layout_palette,
		&palette_widget, &sts);
  palette->set_focus_cb = &Wtt::set_focus_cb;
  palette->traverse_focus_cb = &Wtt::traverse_focus;
  palette->create_popup_menu_cb = &Wtt::create_pal_popup_menu_cb;
  gtk_paned_pack1( GTK_PANED(palette_paned), palette_widget, FALSE, TRUE);

  wnav = new WNavGtk( this, wnav_paned, title_w2, layout_w1,
		&wnav_brow_widget, ldhses, root_menu, 
		wnav_eWindowType_W1, &sts);
  wnav->message_cb = &Wtt::message_cb;
  wnav->close_cb = &Wtt::close;
  wnav->change_value_cb = &Wtt::change_value;
  wnav->get_wbctx_cb = &Wtt::get_wbctx;
  wnav->attach_volume_cb = &Wtt::attach_volume_cb;
  wnav->detach_volume_cb = &Wtt::detach_volume_cb;
  wnav->get_palette_select_cb = &Wtt::get_palette_select_cb;
  wnav->set_focus_cb = &Wtt::set_focus_cb;
  wnav->traverse_focus_cb = &Wtt::traverse_focus;
  wnav->set_twowindows_cb = &Wtt::set_twowindows_cb;
  wnav->set_configure_cb = &Wtt::configure_cb;
  wnav->gbl_command_cb = &Wtt::gbl_command_cb;
  wnav->create_popup_menu_cb = &Wtt::create_popup_menu_cb;
  wnav->save_cb = &Wtt::save_cb;
  wnav->revert_cb = &Wtt::revert_cb;
  wnav->script_filename_cb = &Wtt::script_filename_cb;
  wnav->format_selection_cb = WttGtk::format_selection;
  wnav->get_global_select_cb = Wtt::get_global_select_cb;
  wnav->global_unselect_objid_cb = Wtt::global_unselect_objid_cb;
  wnav->set_window_char_cb = Wtt::set_window_char_cb;
  wnav->open_vsel_cb = Wtt::open_vsel_cb;
  focused_wnav = wnav;
  wnav_mapped = 1;

  wnavnode = new WNavGtk( this, wnav_paned, title_w2, layout_w2,
		&wnavnode_brow_widget, ldhses, root_menu, 
		wnav_eWindowType_W2, &sts);
  wnavnode->message_cb = &Wtt::message_cb;
  wnavnode->close_cb = &Wtt::close;
  wnavnode->change_value_cb = &Wtt::change_value;
  wnavnode->get_wbctx_cb = &Wtt::get_wbctx;
  wnavnode->attach_volume_cb = &Wtt::attach_volume_cb;
  wnavnode->detach_volume_cb = &Wtt::detach_volume_cb;
  wnavnode->get_palette_select_cb = &Wtt::get_palette_select_cb;
  wnavnode->set_focus_cb = &Wtt::set_focus_cb;
  wnavnode->traverse_focus_cb = &Wtt::traverse_focus;
  wnavnode->set_twowindows_cb = &Wtt::set_twowindows_cb;
  wnavnode->set_configure_cb = &Wtt::configure_cb;
  wnavnode->gbl_command_cb = &Wtt::gbl_command_cb;
  wnavnode->create_popup_menu_cb = &Wtt::create_popup_menu_cb;
  wnavnode->save_cb = &Wtt::save_cb;
  wnavnode->revert_cb = &Wtt::revert_cb;
  wnavnode->script_filename_cb = &Wtt::script_filename_cb;
  wnavnode->format_selection_cb = WttGtk::format_selection;
  wnavnode->get_global_select_cb = Wtt::get_global_select_cb;
  wnavnode->global_unselect_objid_cb = Wtt::global_unselect_objid_cb;
  wnavnode->set_window_char_cb = Wtt::set_window_char_cb;
  wnavnode->open_vsel_cb = Wtt::open_vsel_cb;
  wnavnode_mapped = 1;

  gtk_paned_pack1( GTK_PANED(wnav_paned), wnav_brow_widget, TRUE, TRUE);
  gtk_paned_pack2( GTK_PANED(wnav_paned), wnavnode_brow_widget, FALSE, TRUE);
  gtk_widget_show( wnav_paned);

  // Statusbar and cmd input
  GtkWidget *statusbar = gtk_hbox_new( FALSE, 0);
  msg_label = gtk_label_new( "");
  gtk_widget_set_size_request( msg_label, -1, 25);
  cmd_prompt = gtk_label_new( "value > ");
  gtk_widget_set_size_request( cmd_prompt, -1, 25);
  cmd_entry = new CoWowEntryGtk( &cmd_recall);
  cmd_input = cmd_entry->widget();
  gtk_widget_set_size_request( cmd_input, -1, 25);
  g_signal_connect( cmd_input, "activate", 
  		    G_CALLBACK(WttGtk::valchanged_cmd_input), this);

  gtk_box_pack_start( GTK_BOX(statusbar), msg_label, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_prompt, FALSE, FALSE, 20);
  gtk_box_pack_start( GTK_BOX(statusbar), cmd_input, TRUE, TRUE, 20);
  gtk_widget_show_all( statusbar);

  GtkWidget *vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(menu_bar), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(tools), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(palette_paned), TRUE, TRUE, 0);
  gtk_box_pack_start( GTK_BOX(vbox), GTK_WIDGET(statusbar), FALSE, FALSE, 3);

  gtk_container_add( GTK_CONTAINER(toplevel), vbox);

  gtk_widget_show_all( toplevel);
  realized = 1;

  if ( disable_w2)
    wnavnode_mapped = 0;

  if ( !wnav_mapped)
    g_object_set( wnav_brow_widget, "visible", FALSE, NULL);
  if ( !wnavnode_mapped)
    g_object_set( wnavnode_brow_widget, "visible", FALSE, NULL);
  twowindows = wnav_mapped && wnavnode_mapped ? 1 : 0;

  if ( !editmode)
    g_object_set( palette_widget, "visible", FALSE, NULL);
  g_object_set( cmd_prompt, "visible", FALSE, NULL);
  g_object_set( cmd_input, "visible", FALSE, NULL);
  gtk_paned_set_position( GTK_PANED(wnav_paned), window_width / 2);

  wnav->get_options( &enable_comment, &show_class, &show_alias, 
	&show_descrip, &show_objref, &show_objxref, 
	&show_attrref, &show_attrxref, &build_force, &build_debug,
	&build_crossref, &build_manual);

  if ( wbctx && volid) {
    wnav->volume_attached( wbctx, ldhses, 0);
    wnavnode->volume_attached( wbctx, ldhses, 0);
  }

  if ( (wb_type == wb_eType_Directory || wb_type == wb_eType_Volume) && ldhses) {
    // Start configuration wizard if volume is empty
    pwr_tOid oid;

    sts = ldh_GetRootList( ldhses, &oid);
    if ( EVEN( sts)) {
      pwr_tCid vcid;

      if ( wbctx && volid) {
	sts = ldh_GetVolumeClass( wbctx, volid, &vcid);
	if ( vcid == pwr_eClass_RootVolume || vcid == pwr_eClass_DirectoryVolume) {
	  wnav->wow->HideWarranty();  // Warranty window is hidden behind the wizard
	  set_edit();
	  start_wizard( this, vcid);
	}
      }
    }
  }
  if ( wnav->gbl.advanced_user)
    g_object_set( tools_set_advuser, "visible", FALSE, NULL);

  menu_setup();
  *status = 1;
}


WttGtk::~WttGtk()
{
  if ( close_cb)
    (close_cb)( this);
  else
    exit(0);

  free_cursor();

  if ( utedctx)
    delete utedctx;

  if ( selection_timerid)
    g_source_remove( selection_timerid);

  wnav->closing_down = 1;
  wnavnode->closing_down = 1;
  if ( mcp)
    free( mcp);
  delete cmd_entry;
  delete wnav;
  delete wnavnode;
  delete palette;

  if ( options_form)
    gtk_widget_destroy( options_form);
  if ( confirm_widget)
    gtk_widget_destroy( confirm_widget);
  gtk_widget_destroy( toplevel);
}

void WttGtk::pop()
{
  gtk_window_present( GTK_WINDOW(toplevel));
}

static gboolean options_delete_event( GtkWidget *w, GdkEvent *event, gpointer wtt)
{
  ((Wtt *)wtt)->update_options_form();
  g_object_set( ((WttGtk *)wtt)->options_form, "visible", FALSE, NULL);
  return TRUE;
}

void WttGtk::create_options_dialog()
{
  if ( options_form) {
    g_object_set( options_form, "visible", TRUE, NULL);
    return;
  }

  // Create the options dialog
  options_form = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					     "default-height", 450,
					     "default-width", 650,
					     "title", "Options",
					     "window-position", GTK_WIN_POS_CENTER,
					     NULL);
  g_signal_connect( options_form, "delete_event", G_CALLBACK(options_delete_event), this);

  GtkWidget *hier_label = gtk_label_new( "Hierarchy");
  enable_comment_w = gtk_check_button_new_with_label( "Enable Comment");
  show_plant_w = gtk_check_button_new_with_label( "Plant Configuration");
  show_node_w = gtk_check_button_new_with_label( "Node Configuration");

  GtkWidget *hier_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hier_vbox), hier_label, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(hier_vbox), enable_comment_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(hier_vbox), show_plant_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(hier_vbox), show_node_w, FALSE, FALSE, 7);

  GtkWidget *entry_label = gtk_label_new( "Entry");
  show_class_w = gtk_check_button_new_with_label( "Class");
  show_alias_w = gtk_check_button_new_with_label( "Alias");
  show_descrip_w = gtk_check_button_new_with_label( "Description");
  show_objref_w = gtk_check_button_new_with_label( "Object Reference");
  show_objxref_w = gtk_check_button_new_with_label( "Object Cross Reference");
  show_attrref_w = gtk_check_button_new_with_label( "Attribute Reference");
  show_attrxref_w = gtk_check_button_new_with_label( "Attribute Cross Reference");

  GtkWidget *entry_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(entry_vbox), entry_label, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(entry_vbox), show_class_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(entry_vbox), show_alias_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(entry_vbox), show_descrip_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(entry_vbox), show_objref_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(entry_vbox), show_objxref_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(entry_vbox), show_attrref_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(entry_vbox), show_attrxref_w, FALSE, FALSE, 7);

  GtkWidget *build_label = gtk_label_new( "Build");
  build_force_w = gtk_check_button_new_with_label( "Force");
  build_debug_w = gtk_check_button_new_with_label( "Debug");
  build_crossref_w = gtk_check_button_new_with_label( "Crossreference");
  build_manual_w = gtk_check_button_new_with_label( "Manual");

  GtkWidget *build_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(build_vbox), build_label, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(build_vbox), build_force_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(build_vbox), build_debug_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(build_vbox), build_crossref_w, FALSE, FALSE, 7);
  gtk_box_pack_start( GTK_BOX(build_vbox), build_manual_w, FALSE, FALSE, 7);

  GtkWidget *hbox = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox), hier_vbox, FALSE, FALSE, 50);
  gtk_box_pack_start( GTK_BOX(hbox), gtk_vseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox), entry_vbox, FALSE, FALSE, 50);
  gtk_box_pack_start( GTK_BOX(hbox), gtk_vseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(hbox), build_vbox, FALSE, FALSE, 50);

  GtkWidget *options_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( options_ok, 70, 25);
  g_signal_connect( options_ok, "clicked", 
  		    G_CALLBACK(WttGtk::activate_options_ok), this);
  GtkWidget *options_apply = gtk_button_new_with_label( "Apply");
  gtk_widget_set_size_request( options_apply, 70, 25);
  g_signal_connect( options_apply, "clicked", 
  		    G_CALLBACK(WttGtk::activate_options_apply), this);
  GtkWidget *options_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( options_cancel, 70, 25);
  g_signal_connect( options_cancel, "clicked", 
  		    G_CALLBACK(WttGtk::activate_options_cancel), this);

  GtkWidget *options_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(options_hboxbuttons), options_ok, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(options_hboxbuttons), options_apply, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(options_hboxbuttons), options_cancel, FALSE, FALSE, 0);

  GtkWidget *options_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(options_vbox), hbox, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(options_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(options_vbox), options_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(options_form), options_vbox);
  gtk_widget_show_all( options_form);

}

static gint confirm_delete_event( GtkWidget *w, GdkEvent *event, gpointer wtt)
{
  g_object_set( ((WttGtk *)wtt)->confirm_widget, "visible", FALSE, NULL);
  return TRUE;
}

void WttGtk::create_confirm_dialog()
{
  if ( confirm_widget) {
    g_object_set( confirm_widget, "visible", TRUE, NULL);
    return;
  }

  // Create a confirm window
  confirm_widget = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW, 
					       "default-height", 150,
					       "default-width", 400,
					       "title", "Confirm",
					       "window-position", GTK_WIN_POS_CENTER,
					       NULL);
  g_signal_connect( confirm_widget, "delete_event", G_CALLBACK(confirm_delete_event), this);
  confirm_label = gtk_label_new("");
  GtkWidget *confirm_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *confirm_ok = gtk_button_new_with_label( "Yes");
  gtk_widget_set_size_request( confirm_ok, 70, 25);
  g_signal_connect( confirm_ok, "clicked", 
  		    G_CALLBACK(WttGtk::activate_confirm_ok), this);

  GtkWidget *confirm_no = gtk_button_new_with_label( "No");
  gtk_widget_set_size_request( confirm_no, 70, 25);
  g_signal_connect( confirm_no, "clicked", 
  		    G_CALLBACK(WttGtk::activate_confirm_no), this);

  GtkWidget *confirm_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( confirm_cancel, 70, 25);
  g_signal_connect( confirm_cancel, "clicked", 
  		    G_CALLBACK(WttGtk::activate_confirm_cancel), this);

  GtkWidget *confirm_hboxtext = gtk_hbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(confirm_hboxtext), confirm_image, FALSE, FALSE, 15);
  gtk_box_pack_start( GTK_BOX(confirm_hboxtext), confirm_label, TRUE, TRUE, 15);

  GtkWidget *confirm_hboxbuttons = gtk_hbox_new( TRUE, 40);
  gtk_box_pack_start( GTK_BOX(confirm_hboxbuttons), confirm_ok, FALSE, FALSE, 0);
  gtk_box_pack_start( GTK_BOX(confirm_hboxbuttons), confirm_no, FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(confirm_hboxbuttons), confirm_cancel, FALSE, FALSE, 0);

  GtkWidget *confirm_vbox = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(confirm_vbox), confirm_hboxtext, TRUE, TRUE, 30);
  gtk_box_pack_start( GTK_BOX(confirm_vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
  gtk_box_pack_end( GTK_BOX(confirm_vbox), confirm_hboxbuttons, FALSE, FALSE, 15);
  gtk_container_add( GTK_CONTAINER(confirm_widget), confirm_vbox);
  gtk_window_set_transient_for( GTK_WINDOW(gtk_widget_get_toplevel(confirm_widget)), 
  				GTK_WINDOW(gtk_widget_get_toplevel(toplevel)));
  gtk_widget_show_all( confirm_widget);
}
							  
static gint india_delete_event( GtkWidget *w, GdkEvent *event, gpointer wtt)
{
  g_object_set( ((WttGtk *)wtt)->india_widget, "visible", FALSE, NULL);
  return TRUE;
}

void WttGtk::create_input_dialog()
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
  india_text = gtk_entry_new();
  g_signal_connect( india_text, "activate", 
  		    G_CALLBACK(WttGtk::activate_india_ok), this);
  india_label = gtk_label_new("");
  GtkWidget *india_image = (GtkWidget *)g_object_new( GTK_TYPE_IMAGE, 
				"stock", GTK_STOCK_DIALOG_QUESTION,
				"icon-size", GTK_ICON_SIZE_DIALOG,
				"xalign", 0.5,
				"yalign", 1.0,
				NULL);

  GtkWidget *india_ok = gtk_button_new_with_label( "Ok");
  gtk_widget_set_size_request( india_ok, 70, 25);
  g_signal_connect( india_ok, "clicked", 
  		    G_CALLBACK(WttGtk::activate_india_ok), this);
  GtkWidget *india_cancel = gtk_button_new_with_label( "Cancel");
  gtk_widget_set_size_request( india_cancel, 70, 25);
  g_signal_connect( india_cancel, "clicked", 
  		    G_CALLBACK(WttGtk::activate_india_cancel), this);

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
  gtk_window_set_transient_for( GTK_WINDOW(gtk_widget_get_toplevel(india_widget)), 
				GTK_WINDOW(gtk_widget_get_toplevel(toplevel)));
}

GtkWidget *WttGtk::build_menu()
{
  GtkWidget *popup;
  int i = 0;

  popup = build_submenu( wnav_form, MENU_POPUP, "", mcp, 
			 popup_button_cb, (void *) this, 
			 (ldh_sMenuItem *) mcp->ItemList, &i);
  //  if (popup != NULL) 
  //    XtAddCallback( popup, XmNunmapCallback, 
  //		   (XtCallbackProc)popup_unmap_cb, this);

  return popup;
}

GtkWidget *WttGtk::build_submenu( GtkWidget *Parent, int MenuType,
				  const char *MenuTitle, void *MenuUserData,
				  void (*Callback)( GtkWidget *, gpointer),
				  void *CallbackData, ldh_sMenuItem *Items, int *idx)
{
  GtkWidget *Menu, *W;
  int i;
  unsigned int Level;

  Menu = (GtkWidget *) g_object_new( GTK_TYPE_MENU, NULL);

  Level = Items[*idx].Level;


  for (; Items[*idx].Level != 0 && Items[*idx].Level >= Level; (*idx)++) {
    if (Items[*idx].Item == ldh_eMenuItem_Cascade) {
      if (MenuType == MENU_OPTION)  {
        printf("You can't have submenus from option menu items.");
        return NULL;
      } 
      else {
        i = *idx;
        (*idx)++;	
	GtkWidget *Item = gtk_menu_item_new_with_label( Items[i].Name);
        W = build_submenu(Menu, MENU_PULLDOWN, Items[i].Name, MenuUserData, 
			  Callback, CallbackData, Items, idx);
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(Item),
				  GTK_WIDGET(W));
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), Item);
	gtk_widget_show(Item);
        (*idx)--;
      }
    }
    else {
      if (Items[*idx].Item == ldh_eMenuItem_Separator) {
	// Separator
	W = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), W);
	gtk_widget_show(W);
      }
      else {
	// Pushbutton
	W = gtk_menu_item_new_with_label( Items[*idx].Name);
	gtk_widget_set_sensitive( W, Items[*idx].Flags.f.Sensitive ? TRUE : FALSE);
	g_object_set_data( (GObject *)W, "userdata", (gpointer)*idx);
	if ( Callback)
	  g_signal_connect( W, "activate", 
			    G_CALLBACK(Callback), CallbackData);
	gtk_menu_shell_append(GTK_MENU_SHELL(Menu), W);
	gtk_widget_show(W);
      } 
    }
  }

  return Menu;
}

void WttGtk::menu_position_func( GtkMenu *menu, gint *x, gint *y, gboolean *push_in,
				 gpointer data)
{
  WttGtk *wtt = (WttGtk *)data;

  *x = wtt->popupmenu_x;
  *y = wtt->popupmenu_y;
  *push_in = FALSE;
}

#if 0
void WttMotif::popup_unmap_cb(Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  XtDestroyWidget(w);
}
#endif 

void WttGtk::popup_button_cb( GtkWidget *w, gpointer data)
{
  Wtt *wtt = (Wtt *)data;
  int idx;
  pwr_tStatus sts;

#if 0
  Widget menu;
  // Find the menu widget
  menu = XtParent(w);
  while (1) {
    if (strcmp(XtName(menu), "_popup") == 0 || 
	  strcmp(XtName(menu), "_pulldown") == 0)
      break;
    menu = XtParent(menu);
  }

  XtVaGetValues (w, XmNuserData, &idx, NULL);
#endif

  idx = (int) (unsigned long)g_object_get_data( (GObject *)w, "userdata");

  wtt->mcp->ChosenItem = idx;
  wtt->set_clock_cursor();
  sts = ldh_CallMenuMethod( wtt->mcp, wtt->mcp->ChosenItem);
  if (EVEN(sts))
    wtt->message( 'E', wnav_get_message(sts));
  wtt->reset_cursor();
}

void WttGtk::disable_focus()
{
  focustimer.disable( 400);
  focustimer.disabled();
}
