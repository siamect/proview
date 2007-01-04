/* 
 * Proview   $Id: wb_wattnav_gtk.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb_wattnav_gtk.cpp -- Display object info */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "wb_watt_msg.h"
#include "rt_mh_net.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "wb_watt_gtk.h"
#include "wb_wattnav_gtk.h"
#include "wb_wnav.h"
#include "wb_wnav_brow.h"
#include "wb_wnav_item.h"
#include "co_wow_gtk.h"

//
// Create the navigator widget
//
WAttNavGtk::WAttNavGtk(
	void 		*wa_parent_ctx,
	GtkWidget      	*wa_parent_wid,
	char 		*wa_name,
	ldh_tSesContext wa_ldhses,
	pwr_sAttrRef 	wa_aref,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname,
	wb_eUtility	wa_utility,
	GtkWidget      	**w,
	pwr_tStatus 	*status) :
  WAttNav(wa_parent_ctx,wa_name,wa_ldhses,wa_aref,wa_editmode,wa_advanced_user,
	  wa_display_objectname,wa_utility,status), parent_wid(wa_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new(
	WAttNav::init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  CoWowGtk::GetAtoms( 0, &objid_atom, 0);

  *w = form_widget;
  *status = 1;
}

//
//  Delete a nav context
//
WAttNavGtk::~WAttNavGtk()
{
  delete brow;
  gtk_widget_destroy( form_widget);
}

pwr_tStatus WAttNavGtk::get_selection( char *str, int size)
{
  pwr_tStatus sts;

  sts = CoWowGtk::GetSelection( form_widget, str, size, objid_atom);
  if ( EVEN(sts))
    sts = CoWowGtk::GetSelection( form_widget, str, size, GDK_SELECTION_TYPE_STRING);
  return sts;
}

void WAttNavGtk::set_inputfocus()
{
  if ( !displayed)
    return;

  gtk_widget_grab_focus( brow_widget);
}

#if 0
static void wattnav_trace_scan( WAttNavGtk *wattnav)
{
  int time = 200;

  if ( wattnav->trace_started)
  {
    brow_TraceScan( wattnav->brow->ctx);

    wattnav->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(wattnav->brow_widget) , time,
	(XtTimerCallbackProc)wattnav_trace_scan, wattnav);
  }
}
#endif






