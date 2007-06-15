/* 
 * Proview   $Id: wb_nav_gtk.cpp,v 1.2 2007-06-15 10:53:06 claes Exp $
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

/* wb_nav_gtk.cpp -- Display plant and node hiererachy or plc-editor */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "wb_ldh.h"
#include "wb_ldh_msg.h"
#include "co_cdh.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "wb_nav_gtk.h"

static void nav_sel_lose_cb( GtkWidget *w, GdkEventSelection *event,
			     gpointer data);
static void nav_sel_convert_cb( GtkWidget *w, GtkSelectionData *selection_data,
				guint info, guint time_stamp, gpointer data);

//
// Create the navigator widget
//
NavGtk::NavGtk(
	void *nav_parent_ctx,
	GtkWidget *nav_parent_wid,
	char *nav_name,
	ldh_tSesContext nav_ldhses,
	char *nav_root_name,
	GtkWidget **w,
	pwr_tStatus *status
	) :
  Nav(nav_parent_ctx,nav_name,nav_ldhses,nav_root_name,status), 
  parent_wid(nav_parent_wid)
{
  GtkWidget *scrolledbrow = scrolledbrowwidgetgtk_new(
		Nav::init_brow_cb, this, &brow_widget);

  form_widget = gtk_frame_new( NULL);
  gtk_container_add( GTK_CONTAINER(form_widget), scrolledbrow);
  gtk_container_set_border_width( GTK_CONTAINER(scrolledbrow), 3);

  selection_widget = gtk_invisible_new();
  gtk_selection_add_target( selection_widget, GDK_SELECTION_PRIMARY,
			    GDK_SELECTION_TYPE_STRING, 1);
  g_signal_connect( selection_widget, "selection-get", 
		    G_CALLBACK( nav_sel_convert_cb), this);
  sel_lose_id = g_signal_connect( selection_widget, "selection-clear-event", 
				  G_CALLBACK( nav_sel_lose_cb), this);
  gtk_widget_show_all( brow_widget);

  set_inputfocus(0);

  *w = form_widget;
  *status = 1;
}

//
//  Delete a nav context
//
NavGtk::~NavGtk()
{
  g_signal_handler_disconnect( selection_widget, sel_lose_id);

  PalFile::config_tree_free( menu);
  free_pixmaps();
  gtk_widget_destroy( form_widget);
}

static void nav_sel_convert_cb( GtkWidget *w, GtkSelectionData *selection_data,
				guint info, guint time_stamp, gpointer data)
{
  NavGtk     	*nav = (NavGtk *)data;
  char		name[200];
  pwr_tStatus	sts;
  
  if ( !nav->selection_owner)
    return;

  if ( !nav->get_plant_select_cb) {
    strcpy( name, "");
  }
  else {
    sts = nav->get_plant_select_cb( nav->parent_ctx, name);
    if ( EVEN(sts)) {
      strcpy( name, "");
    }
  }
  gtk_selection_data_set( selection_data, GDK_SELECTION_TYPE_STRING,
			  8, (const guchar *)name, strlen(name));
}

static void nav_sel_lose_cb( GtkWidget *w, GdkEventSelection *event,
			     gpointer data)
{
  Nav *nav = (Nav *)data;

  brow_SelectClear( nav->brow_ctx);
  nav->selection_owner = 0;
}

void NavGtk::set_selection_owner()
{
  gboolean sts;

  sts = gtk_selection_owner_set( selection_widget, GDK_SELECTION_PRIMARY,
			   gtk_get_current_event_time());
  if ( !sts) {
     brow_SelectClear( brow_ctx);
     return;
  }	
  selection_owner = 1;
}

void NavGtk::set_inputfocus( int focus)
{
  if ( !displayed)
    return;

  if ( focus) {
    GdkColor color;

    gdk_color_parse( "Black", &color);
    gtk_widget_modify_bg( form_widget, GTK_STATE_NORMAL, &color);
    gtk_widget_grab_focus( brow_widget);
  }
  else {
    GdkColor color;

    gdk_color_parse( "White", &color);
    gtk_widget_modify_bg( form_widget, GTK_STATE_NORMAL, &color);
  }
}

