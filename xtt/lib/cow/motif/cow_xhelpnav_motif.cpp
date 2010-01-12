/** 
 * Proview   $Id: co_xhelpnav_motif.cpp,v 1.1 2007-01-04 07:51:41 claes Exp $
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

/* cow_xhelpnav_motif.cpp -- helptext navigator */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_msg.h"

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
#include "flow_utils.h"

#include "cow_xhelpnav_motif.h"

extern "C" {
#include "flow_x.h"
#include "cow_mrm_util.h"
#include "co_api.h"
}

static Boolean set_displayed( void *xhelpnav)
{
  ((CoXHelpNav *)xhelpnav)->displayed = 1;
  return True;
}

void CoXHelpNavMotif::pop()
{
  Widget parent, top;

  parent = XtParent( parent_wid);
  while( parent)
  {
    top = parent;
    if ( flow_IsShell( top))
      break;
    parent = XtParent( parent);
  }
  displayed = 0;
  flow_UnmapWidget( top);
  flow_MapWidget( top);

  // A fix to avoid a krash in setinputfocus
  XtAppAddWorkProc( XtWidgetToApplicationContext(top),
			(XtWorkProc)set_displayed, (XtPointer)this);
}

//
// Create the navigator widget
//
CoXHelpNavMotif::CoXHelpNavMotif(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	xhelp_eUtility xn_utility,
	Widget *w,
	pwr_tStatus *status) :
	CoXHelpNav(xn_parent_ctx,xn_name,xn_utility,status),
	parent_wid(xn_parent_wid)
{
  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	CoXHelpNav::init_brow_base_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);
  displayed = 1;

  *w = form_widget;
  *status = 1;
}

//
//  Delete a nav context
//
CoXHelpNavMotif::~CoXHelpNavMotif()
{
  closing_down = 1;

  for ( int i = 0; i < brow_cnt; i++) {
    brow_DeleteSecondaryCtx( brow_stack[i]->ctx);
    brow_stack[i]->free_pixmaps();
    delete brow_stack[i];
  }
  delete brow;
  XtDestroyWidget( form_widget);
}

void CoXHelpNavMotif::set_inputfocus()
{
  if ( displayed && flow_IsViewable( brow_widget)) {
    XtCallAcceptFocus( brow_widget, CurrentTime);
  }
}






