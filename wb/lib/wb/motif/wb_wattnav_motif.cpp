/* 
 * Proview   $Id: wb_wattnav_motif.cpp,v 1.2 2008-10-31 12:51:31 claes Exp $
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

/* wb_wattnav_motif.cpp -- Display object info */

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "wb_watt_msg.h"
#include "rt_mh_net.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"

#include "wb_watt_motif.h"
#include "wb_wattnav_motif.h"
#include "wb_wnav.h"
#include "wb_wnav_brow.h"
#include "wb_wnav_item.h"
#include "cow_wow_motif.h"

//
// Create the navigator widget
//
WAttNavMotif::WAttNavMotif(
	void 		*wa_parent_ctx,
	Widget		wa_parent_wid,
	const char     	*wa_name,
	ldh_tSesContext wa_ldhses,
	pwr_sAttrRef 	wa_aref,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname,
	wb_eUtility	wa_utility,
	Widget 		*w,
	pwr_tStatus 	*status) :
  WAttNav(wa_parent_ctx,wa_name,wa_ldhses,wa_aref,wa_editmode,wa_advanced_user,
	  wa_display_objectname,wa_utility,status), parent_wid(wa_parent_wid)
{
  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	WAttNav::init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  CoWowMotif::GetAtoms( form_widget, 0, &objid_atom, 0);

  // Create the root item
  *w = form_widget;

  *status = 1;
}

//
//  Delete a nav context
//
WAttNavMotif::~WAttNavMotif()
{
  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  delete brow;
  XtDestroyWidget( form_widget);
}

pwr_tStatus WAttNavMotif::get_selection( char *str, int size)
{
  pwr_tStatus sts;

  sts = CoWowMotif::GetSelection( form_widget, str, size, objid_atom);
  if ( EVEN(sts))
    sts = CoWowMotif::GetSelection( form_widget, str, size, XA_STRING);
  return sts;
}

void WAttNavMotif::set_inputfocus()
{
//  brow_SetInputFocus( brow->ctx);

  if ( !displayed)
    return;

  XtCallAcceptFocus( brow_widget, CurrentTime);
}

static void wattnav_trace_scan( WAttNavMotif *wattnav)
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







