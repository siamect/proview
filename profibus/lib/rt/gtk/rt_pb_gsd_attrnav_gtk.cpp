/* 
 * Proview   $Id: rt_pb_gsd_attrnav_gtk.cpp,v 1.3 2008-10-31 12:51:29 claes Exp $
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

/* rt_pb_gsd_attrnav_gtk.cpp -- Display gsd attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "cow_wow_gtk.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "flow_msg.h"
#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_gtk.h"
#include "rt_pb_gsd.h"
#include "rt_pb_gsd_attr_gtk.h"
#include "rt_pb_gsd_attrnav_gtk.h"
#include "rt_pb_msg.h"


//
// Create the navigator widget
//
GsdAttrNavGtk::GsdAttrNavGtk( void *xn_parent_ctx,
			      GtkWidget *xn_parent_wid,
			      const char *xn_name,
			      pb_gsd *xn_gsd,
			      int xn_edit_mode,
			      GtkWidget **w,
			      pwr_tStatus *status) :
  GsdAttrNav( xn_parent_ctx, xn_name, xn_gsd, xn_edit_mode, status),
  parent_wid(xn_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new( init_brow_cb, 
					   this, &brow_widget);
  gtk_widget_show_all( brow_widget);

  *w = form_widget;

  wow = new CoWowGtk( brow_widget);
  trace_timerid = wow->timer_new();
  *status = 1;
}

//
//  Delete a nav context
//
GsdAttrNavGtk::~GsdAttrNavGtk()
{
  delete trace_timerid;
  delete wow;
  delete brow;
  gtk_widget_destroy( form_widget);
}

void GsdAttrNavGtk::set_inputfocus()
{
  gtk_widget_grab_focus( brow_widget);
}
