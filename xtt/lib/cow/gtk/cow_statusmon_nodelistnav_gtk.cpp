/* 
 * Proview   $Id: co_statusmon_nodelistnav_gtk.cpp,v 1.5 2007-10-02 15:53:20 claes Exp $
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

/* co_statusmon_nodelistnav_gtk.cpp -- Console message window. */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "cow_wow_gtk.h"
#include "rt_gdh.h"
#include "co_syi.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "cow_statusmon_nodelistnav_gtk.h"

NodelistNavGtk::NodelistNavGtk( void *nodelist_parent_ctx,
				GtkWidget *nodelist_parent_wid,
				MsgWindow *nodelist_msg_window,
				char *nodelist_nodename,
				int nodelist_mode,
				int nodelist_view_node_descr,
				int nodelist_msgw_pop,
				GtkWidget **w) :
  NodelistNav( nodelist_parent_ctx, nodelist_msg_window, nodelist_nodename,
	       nodelist_mode, nodelist_view_node_descr, nodelist_msgw_pop), 
  parent_wid(nodelist_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new( init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  *w = form_widget;

  wow = new CoWowGtk( parent_wid);
}


//
//  Delete ev
//
NodelistNavGtk::~NodelistNavGtk()
{
  if ( trace_started)
    g_source_remove( trace_timerid);

  delete brow;
  gtk_widget_destroy( form_widget);
}

void NodelistNavGtk::beep()
{
  gdk_display_beep( gtk_widget_get_display( form_widget));
}

void NodelistNavGtk::set_input_focus()
{
  gtk_widget_grab_focus( brow_widget);
}

void NodelistNavGtk::trace_start()
{
  NodelistNavGtk::trace_scan( this);
}

static gboolean nodelistnavgtk_trace_scan( void *data)
{
  NodelistNavGtk::trace_scan( (NodelistNav *)data);
  return FALSE;
}

void NodelistNavGtk::trace_scan( NodelistNav *nodelistnav)
{
  if ( nodelistnav->trace_started) {
    nodelistnav->update_nodes();

    brow_TraceScan( nodelistnav->brow->ctx);

    ((NodelistNavGtk *)nodelistnav)->trace_timerid = g_timeout_add( nodelistnav->scantime,
	nodelistnavgtk_trace_scan, nodelistnav);
  }
}








