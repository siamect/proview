/* 
 * Proview   $Id: xtt_xattnav_gtk.cpp,v 1.3 2008-10-31 12:51:36 claes Exp $
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

/* wb_xattnav.cpp -- Display object info */

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_xatt_msg.h"
#include "rt_mh_net.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "xtt_xatt.h"
#include "xtt_xattnav_gtk.h"
#include "xtt_xnav.h"
#include "xtt_xnav_brow.h"
#include "xtt_xnav_crr.h"
#include "xtt_item.h"
#include "pwr_privilege.h"
#include "co_wow_gtk.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

//
// Create the navigator widget
//
XAttNavGtk::XAttNavGtk(
	void 		*xa_parent_ctx,
	GtkWidget      	*xa_parent_wid,
	xattnav_eType   xa_type,
	const char     	*xa_name,
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	GtkWidget      	**w,
	pwr_tStatus 	*status) :
  XAttNav( xa_parent_ctx, xa_type, xa_name, xa_objar, xa_advanced_user, status),
  parent_wid(xa_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new( init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  // Create the root item
  *w = form_widget;

  wow = new CoWowGtk( form_widget);
  trace_timerid = wow->timer_new();
  *status = 1;
}

//
//  Delete a nav context
//
XAttNavGtk::~XAttNavGtk()
{
  if ( trace_started)
    trace_timerid->remove();

  delete trace_timerid;
  delete wow;
  delete brow;
  gtk_widget_destroy( form_widget);
}

void XAttNavGtk::set_inputfocus()
{
  if ( !displayed)
    return;

  gtk_widget_grab_focus( brow_widget);
}

void XAttNavGtk::popup_position( int x_event, int y_event, int *x, int *y)
{
  CoWowGtk::PopupPosition( brow_widget, x_event, y_event, x, y);
}





