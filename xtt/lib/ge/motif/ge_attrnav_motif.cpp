/* 
 * Proview   $Id: ge_attrnav_motif.cpp,v 1.3 2008-10-31 12:51:33 claes Exp $
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

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
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
#include "flow_msg.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_motif.h"


#include "ge_attr.h"
#include "ge_attrnav_motif.h"
#include "ge_dyn.h"
#include "ge_msg.h"

//
// Create the navigator widget
//
AttrNavMotif::AttrNavMotif(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	const char *xn_name,
	attr_sItem  *xn_itemlist,
	int xn_item_cnt,
	Widget *w,
	pwr_tStatus *status) :
  AttrNav( xn_parent_ctx, xn_name, xn_itemlist, xn_item_cnt, status),
  parent_wid(xn_parent_wid), trace_timerid(0)
{
  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	AttrNav::init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}
//
//  Delete a nav context
//

AttrNavMotif::~AttrNavMotif()
{
  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  delete brow;
  XtDestroyWidget( form_widget);
}

void AttrNavMotif::trace_start()
{
  AttrNavMotif::trace_scan( this);
}

void AttrNavMotif::trace_scan( AttrNav *attrnav)
{
  int time = 200;

  if ( attrnav->trace_started)
  {
    brow_TraceScan( attrnav->brow->ctx);

    ((AttrNavMotif *)attrnav)->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( ((AttrNavMotif *)attrnav)->brow_widget) , time,
	(XtTimerCallbackProc)AttrNavMotif::trace_scan, attrnav);
  }
}

void AttrNavMotif::set_inputfocus()
{
//  brow_SetInputFocus( brow->ctx);
  XtCallAcceptFocus( brow_widget, CurrentTime);
}
