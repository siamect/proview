/* 
 * Proview   $Id: wb_gre_motif.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb_gre_motif.cpp
   This module creates the flow window and handles
   the flow callbacks.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pwr.h"
#include "pwr_baseclasses.h"

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/IntrinsicP.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/DialogS.h>


#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_widget_motif.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_vldh.h"
#include "wb_foe.h"
#include "wb_goec.h"
#include "wb_goen.h"
#include "wb_utl_api.h"
#include "wb_gre_motif.h"
#include "wb_tra.h"

//
// Constructor
//
WGreMotif::WGreMotif( void *wg_parent_ctx,
		      Widget wg_parent_wid,
		      char *name) :
  WGre( wg_parent_ctx, 0), parent_wid(wg_parent_wid), trace_timerid(0)
{
  Arg		args[20];
  int		i;
	
  static MrmRegisterArg	reglist[] = {
    { "gre_ctx", 0 }
	};

  /* Create object context */
  reglist[0].value = (caddr_t) this;

  /* Fill in common part */
  parent_wid = wg_parent_wid;

  ctx_init();
 
  i = 0;
  XtSetArg( args[i], XmNwidth, 600);i++;
  XtSetArg( args[i], XmNheight, 600);i++;
  XtSetArg( args[i], XmNtopAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNrightAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNleftAttachment, XmATTACH_FORM);i++;
  XtSetArg( args[i], XmNbottomAttachment, XmATTACH_FORM);i++;

  form_widget = ScrolledFlowCreate( parent_wid, "Flow window", 
	      args, i, init_flow, this, &flow_widget);

  XtManageChild( form_widget);
  popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
}

WGreMotif::~WGreMotif()
{
  XtDestroyWidget( XtParent( form_widget));
}

int WGreMotif::new_navigator( Widget parent)
{
  Arg 		args[20];
  int		i;

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNpaneMinimum,75);i++;
  XtSetArg(args[i],XmNpaneMaximum,800);i++;
  nav_widget = FlowCreateNav( parent, "navigator",
        args, i, flow_widget);
  XtManageChild( nav_widget);

  return 1;
}

int WGreMotif::new_navigator_popup()
{
  Arg 		args[20];
  int		i;

  /* Create navigator popup */
  i = 0;
  XtSetArg(args[i],XmNallowShellResize, TRUE); i++;
  XtSetArg(args[i],XmNallowResize, TRUE); i++;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNx,500);i++;
  XtSetArg(args[i],XmNy,500);i++;

  nav_shell = XmCreateDialogShell( flow_widget, "Navigator",
				   args, i);
  XtManageChild( nav_shell);

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  nav_widget = FlowCreateNav( nav_shell, "navigator",
			      args, i, flow_widget);
  XtManageChild( nav_widget);
  XtRealizeWidget( nav_shell);

  return 1;
}

void WGreMotif::trace_scan( WGreMotif *gre)
{
  if ( gre->trace_started)
  {


    flow_TraceScan( gre->flow_ctx);

    gre->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(gre->flow_widget) ,500,
	(XtTimerCallbackProc)trace_scan, gre);
  }
}

void WGreMotif::trace_start()
{
  trace_started = 1;
  trace_scan( this);
}

void WGreMotif::trace_stop()
{
  if ( trace_started) {
    trace_started = 0;
    if ( trace_timerid)
      XtRemoveTimeOut( trace_timerid);
  }
}

void WGreMotif::get_popup_position( int *x, int *y)
{
    Arg		arg[4];
    Widget     	w;
    short      	x_work_area, y_work_area;

    w = XtParent( parent_wid);
    XtSetArg( arg[0], XmNx, &x_work_area);
    XtSetArg( arg[1], XmNy, &y_work_area);
    XtGetValues( w, arg, 2);
    *x += x_work_area;
    *y += y_work_area;

    w = XtParent(w);
    XtSetArg( arg[0], XmNx, &x_work_area);
    XtSetArg( arg[1], XmNy, &y_work_area);
    XtGetValues( w, arg, 2);
    *x += x_work_area;
    *y += y_work_area;

    /* Add coordinates of the work_area widget */
    // XtSetArg( arg[0], XmNx, &x_work_area);
    // XtSetArg( arg[1], XmNy, &y_work_area);
    // XtGetValues( flow_widget, arg, 2);
    
    /* Add coordinates of the work_area widget */
    XtSetArg( arg[0], XmNx, &x_work_area);
    XtSetArg( arg[1], XmNy, &y_work_area);
    XtGetValues( parent_wid, arg, 2);

    *x += x_work_area;
    *y += y_work_area;
}
