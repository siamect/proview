/* 
 * Proview   $Id: wb_wdanav_motif.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb_wdanav_motif.cpp -- Display object info */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "wb_wda_msg.h"
#include "rt_mh_net.h"
#include "wb_trv.h"
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

#include "wb_wda_motif.h"
#include "wb_wdanav_motif.h"
#include "wb_wnav.h"
#include "wb_wnav_brow.h"
#include "wb_wnav_item.h"

//
// Create the navigator widget
//
WdaNavMotif::WdaNavMotif(
	void 		*wa_parent_ctx,
	Widget		wa_parent_wid,
	char 		*wa_name,
	ldh_tSesContext wa_ldhses,
	pwr_tObjid 	wa_objid,
	pwr_tClassId 	wa_classid,
        char            *wa_attribute,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname,
	wb_eUtility	wa_utility,
	Widget 		*w,
	pwr_tStatus 	*status) :
  WdaNav(wa_parent_ctx,wa_name,wa_ldhses,wa_objid,wa_classid,wa_attribute,wa_editmode,
      wa_advanced_user,wa_display_objectname,wa_utility,status), 
  parent_wid(wa_parent_wid)
{
  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	WdaNav::init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}

//
//  Delete a nav context
//
WdaNavMotif::~WdaNavMotif()
{
  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  delete brow;
  XtDestroyWidget( form_widget);
}

void WdaNavMotif::set_inputfocus()
{
//  brow_SetInputFocus( brow->ctx);

  if ( !displayed)
    return;

  XtCallAcceptFocus( brow_widget, CurrentTime);
}

static void wdanav_trace_scan( WdaNavMotif *wdanav)
{
  int time = 200;

  if ( wdanav->trace_started)
  {
    brow_TraceScan( wdanav->brow->ctx);

    wdanav->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(wdanav->brow_widget) , time,
	(XtTimerCallbackProc)wdanav_trace_scan, wdanav);
  }
}









