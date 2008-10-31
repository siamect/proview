/* 
 * Proview   $Id: ge_graph_motif.cpp,v 1.3 2008-10-31 12:51:33 claes Exp $
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

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "co_dcli.h"
#include "ge_msg.h"
#include "flow_x.h"
}
 
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_motif.h"

#include "ge_graph_motif.h"
#include "ge_attr_motif.h"
#include "co_wow_motif.h"

GraphMotif::GraphMotif(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	const char *xn_name,
	Widget *w,
	pwr_tStatus *status,
	const char *xn_default_path,
	graph_eMode graph_mode,
	int scrollbar,
	int xn_gdh_init_done,
	const char *xn_object_name,
	int xn_use_default_access,
	unsigned int xn_default_access) :
  Graph( xn_parent_ctx, xn_name, xn_default_path, graph_mode, xn_gdh_init_done, xn_object_name,
	 xn_use_default_access), parent_wid(xn_parent_wid)
{
  default_access = xn_default_access;
  if ( scrollbar)
    form_widget = ScrolledGrowCreate( parent_wid, name, NULL, 0, 
	graph_init_grow_base_cb, this, (Widget *)&grow_widget);
  else
    grow_widget = form_widget = GrowCreate( parent_wid, name, NULL, 0, 
	graph_init_grow_base_cb, this);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  gbl.load_config( this);

  *status = 1;
}

GraphMotif::~GraphMotif()
{
  Attr		*attrctx;
  int		sts;

  closing_down = 1;

  if ( trace_started)
    close_trace( 0);

  sts = attr_list.get_first( (void **) &attrctx);
  while ( sts) {
    delete attrctx;
    attr_list.remove( (void *) attrctx);
    sts = attr_list.get_first( (void **) &attrctx);
  }

  localdb_free();

  for ( int i = 0; i < grow_cnt; i++) {
    grow_SetCtxUserData( grow_stack[i]->ctx, 0);
    delete grow_stack[i];
  }
  grow_SetCtxUserData( grow->ctx, 0);
  delete grow;
  XtDestroyWidget( form_widget);
}

void GraphMotif::trace_timer_remove()
{
  XtRemoveTimeOut( trace_timerid);
}

void GraphMotif::trace_timer_add( int time)
{
  trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(grow_widget), time,
	(XtTimerCallbackProc)Graph::trace_scan, this);
}

int GraphMotif::create_navigator( Widget parent)
{
  Arg 		args[20];
  int		i;

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNpaneMinimum,75);i++;
  XtSetArg(args[i],XmNpaneMaximum,800);i++;
  nav_widget = GrowCreateNav( parent, (char*) "navigator",
        args, i, grow_widget);
  XtManageChild( nav_widget);

  return 1;
}

Attr *GraphMotif::attr_new( void *parent_ctx, void *object, attr_sItem *itemlist, int item_cnt)
{
  Attr *attr =  new AttrMotif( parent_wid, parent_ctx, object, itemlist, item_cnt);
  return attr;
}

void GraphMotif::set_inputfocus( int focus)
{
  Pixel 	bg, fg;

  XtVaGetValues( form_widget, XmNbackground, &bg, XmNforeground, &fg, NULL);
  if ( !focus) {
    // XtSetArg(args[0], XmNborderColor, bg);
    // XtSetValues( form_widget, args, 1);
  }
  else {
    if ( flow_IsViewable( grow_widget)) {
      XtCallAcceptFocus( grow_widget, CurrentTime);
      // XtSetArg(args[0], XmNborderColor, fg);
      // XtSetValues( form_widget, args, 1);
    }
  }
}

void GraphMotif::popup_position( int event_x, int event_y, int *x, int *y) 
{
  CoWowMotif::PopupPosition( grow_widget, event_x, event_y, x, y);
}

