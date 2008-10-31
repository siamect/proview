/* 
 * Proview   $Id: ge_attrnav_gtk.cpp,v 1.4 2008-10-31 12:51:33 claes Exp $
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

/* ge_attrnav.cpp -- Display object info */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "co_cdh.h"
#include "co_time.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "flow_msg.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_gtk.h"


#include "ge_attr.h"
#include "ge_attrnav_gtk.h"
#include "ge_dyn.h"
#include "ge_msg.h"

//
// Create the navigator widget
//
AttrNavGtk::AttrNavGtk(
	void *xn_parent_ctx,
	GtkWidget	*xn_parent_wid,
	const char *xn_name,
	attr_sItem  *xn_itemlist,
	int xn_item_cnt,
	GtkWidget **w,
	pwr_tStatus *status) :
  AttrNav( xn_parent_ctx, xn_name, xn_itemlist, xn_item_cnt, status),
  parent_wid(xn_parent_wid), trace_timerid(0)
{
  form_widget = scrolledbrowwidgetgtk_new(
	AttrNav::init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}
//
//  Delete a nav context
//

AttrNavGtk::~AttrNavGtk()
{
  if ( trace_started)
    g_source_remove( trace_timerid);

  delete brow;
  gtk_widget_destroy( form_widget);
}

void AttrNavGtk::trace_start()
{
  AttrNavGtk::trace_scan( this);
}

static gboolean attrnavgtk_trace_scan( void *data)
{
  AttrNavGtk::trace_scan( (AttrNav *)data);
  return FALSE;
}

void AttrNavGtk::trace_scan( AttrNav *attrnav)
{
  int time = 200;

  if ( attrnav->trace_started) {
    brow_TraceScan( attrnav->brow->ctx);

    ((AttrNavGtk *)attrnav)->trace_timerid = g_timeout_add( time,
	attrnavgtk_trace_scan, attrnav);
  }
}

void AttrNavGtk::set_inputfocus()
{
  gtk_widget_grab_focus( brow_widget);
}
