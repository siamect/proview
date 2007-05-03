/* 
 * Proview   $Id: ge_graph_gtk.cpp,v 1.3 2007-05-03 14:22:13 claes Exp $
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

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "co_dcli.h"
#include "ge_msg.h"
 
#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"

#include "ge_graph_gtk.h"
#include "ge_attr_gtk.h"
#include "glow_growwidget_gtk.h"

GraphGtk::GraphGtk(
	void *xn_parent_ctx,
	GtkWidget	*xn_parent_wid,
	char *xn_name,
	GtkWidget **w,
	pwr_tStatus *status,
	char *xn_default_path,
	graph_eMode graph_mode,
	int scrollbar,
	int xn_gdh_init_done,
	char *xn_object_name,
	int xn_use_default_access,
	unsigned int xn_default_access) :
  Graph( xn_parent_ctx, xn_name, xn_default_path, graph_mode, xn_gdh_init_done, xn_object_name,
	 xn_use_default_access), parent_wid(xn_parent_wid)
{
  default_access = xn_default_access;
  if ( scrollbar) {
    form_widget = scrolledgrowwidgetgtk_new( graph_init_grow_base_cb, this, &grow_widget);
  }
  else {
    grow_widget = form_widget = growwidgetgtk_new( graph_init_grow_base_cb, this);
    // gtk_menu_shell_append( GTK_MENU_SHELL(parent_wid), grow_widget);
  }

  // Create the root item
  *w = form_widget;

  gbl.load_config( this);

  *status = 1;
}

GraphGtk::~GraphGtk()
{
  Attr		*attrctx;
  int		sts;

  closing_down = 1;

  if ( trace_started)
    close_trace( 0);

  sts = attr_list.get_first( (void **) &attrctx);
  while ( sts) {
    delete attrctx;
    attr_list.remove( (void *) attrctx);
    sts = attr_list.get_first( (void **) &attrctx);
  }

  localdb_free();

  for ( int i = 0; i < grow_cnt; i++) {
    grow_SetCtxUserData( grow_stack[i]->ctx, 0);
    delete grow_stack[i];
  }
  grow_SetCtxUserData( grow->ctx, 0);
  delete grow;
  gtk_widget_destroy( form_widget);
}

void GraphGtk::trace_timer_remove()
{
  g_source_remove( trace_timerid);
  trace_timerid = 0;
}

static gboolean graphgdk_trace_scan( void *data)
{
  Graph::trace_scan( (Graph *)data);
  return FALSE;
}

void GraphGtk::trace_timer_add( int time)
{
  trace_timerid = g_timeout_add( time,
				 graphgdk_trace_scan, this);
}

int GraphGtk::create_navigator( GtkWidget *parent)
{
  nav_widget = grownavwidgetgtk_new( grow_widget);
  return 1;
}

Attr *GraphGtk::attr_new( void *parent_ctx, void *object, attr_sItem *itemlist, int item_cnt)
{
  AttrGtk *attr =  new AttrGtk( parent_wid, parent_ctx, object, itemlist, item_cnt);
  return attr;
}

void GraphGtk::set_inputfocus( int focus)
{
  gtk_widget_grab_focus( grow_widget);
}

void GraphGtk::popup_position( int event_x, int event_y, int *x, int *y) 
{
  CoWowGtk::PopupPosition( grow_widget, event_x, event_y, x, y);
}
