/* 
 * Proview   $Id: ge_subgraphs_motif.cpp,v 1.2 2008-03-19 12:59:38 claes Exp $
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

/* ge_subgraphs_motif.cpp -- Display object info */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "co_dcli.h"
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
#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
extern "C" {
#include "flow_x.h"
}

#include "ge_attr_motif.h"
#include "ge_subgraphs_motif.h"
#include "ge_graph.h"

static void subgraphs_create_sg_form( Widget w, SubGraphsMotif *subgraphs, XmAnyCallbackStruct *data)
{
  subgraphs->subgraphs_form = w;
}

static void subgraphs_activate_exit( Widget w, SubGraphsMotif *subgraphs, XmAnyCallbackStruct *data)
{
  if ( subgraphs->close_cb)
    (subgraphs->close_cb)( subgraphs);
  else
    delete subgraphs;
}

static void subgraphs_activate_attr( Widget w, SubGraphsMotif *subgraphs, XmAnyCallbackStruct *data)
{
  ItemSubGraph 		*item;
  int			sts;

  sts = subgraphs->get_select( (void **) &item);
  if ( ODD(sts))
    subgraphs->edit_attributes( item->nodeclass);
}

static void subgraphs_activate_set_extern( Widget w, SubGraphsMotif *subgraphs, XmAnyCallbackStruct *data)
{
  ItemSubGraph 		*item;
  int			sts;

  sts = subgraphs->get_select( (void **) &item);
  if ( ODD(sts))
    item->set_extern( 1);
}

static void subgraphs_activate_set_intern( Widget w, SubGraphsMotif *subgraphs, XmAnyCallbackStruct *data)
{
  ItemSubGraph 		*item;
  int			sts;

  sts = subgraphs->get_select( (void **) &item);
  if ( ODD(sts))
    item->set_extern( 0);
}

static void subgraphs_activate_help( Widget w, SubGraphsMotif *subgraphs, XmAnyCallbackStruct *data)
{
}

//
// Create the navigator widget
//
SubGraphsMotif::SubGraphsMotif(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	void *xn_growctx,
	Widget *w,
	pwr_tStatus *status) :
  SubGraphs( xn_parent_ctx, xn_name, xn_growctx, status),
  parent_wid(xn_parent_wid), trace_timerid(0)	
{
  char		uid_filename[120] = "pwr_exe:ge_subgraphs.uid";
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static MrmRegisterArg	reglist[] = {
        { "subgraphs_ctx", 0 },
	{"subgraphs_activate_exit",(caddr_t)subgraphs_activate_exit },
	{"subgraphs_activate_attr",(caddr_t)subgraphs_activate_attr },
	{"subgraphs_activate_set_extern",(caddr_t)subgraphs_activate_set_extern },
	{"subgraphs_activate_set_intern",(caddr_t)subgraphs_activate_set_intern },
	{"subgraphs_activate_help",(caddr_t)subgraphs_activate_help },
	{"subgraphs_create_sg_form",(caddr_t)subgraphs_create_sg_form }
	};
  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  dcli_translate_filename( uid_filename, uid_filename);

  // Create object context
  reglist[0].value = (caddr_t) this;

  // Motif
  MrmInitialize();

  strcpy( title, "Loaded SubGraphs");

  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell(title, 
		topLevelShellWidgetClass, parent_wid, args, 0);

  sts = MrmFetchWidgetOverride( s_DRMh, "subgraphs_window", parent_wid,
			name, args, 1, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);

  i = 0;
  XtSetArg(args[i],XmNwidth,500);i++;
  XtSetArg(args[i],XmNheight,1200);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);

  strcpy( name, xn_name);

  form_widget = ScrolledBrowCreate( subgraphs_form,
	name, NULL, 0, 
	SubGraphs::init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);


  XtPopup( parent_wid, XtGrabNone);

  // Create the root item
  *w = toplevel;

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)subgraphs_activate_exit, this);

  *status = 1;
}

//
//  Delete a nav context
//
SubGraphsMotif::~SubGraphsMotif()
{
  subgraphs_tAttr attrlist_p, next_p;

  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  // Delete all attr-widgets in attrlist
  attrlist_p = attrlist;
  next_p = NULL;
  while( attrlist_p)
  {
    next_p = attrlist_p->next;
    delete attrlist_p->attrctx;
    free( (char *) attrlist_p);
    attrlist_p = next_p;
  }

  delete brow;
  XtDestroyWidget( parent_wid);
}

void SubGraphsMotif::trace_start()
{
  SubGraphsMotif::trace_scan( this);
}

void SubGraphsMotif::trace_scan( SubGraphsMotif *subgraphs)
{
  int time = 200;

  if ( subgraphs->trace_started) {
    brow_TraceScan( subgraphs->brow->ctx);

    ((SubGraphsMotif *)subgraphs)->trace_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( subgraphs->brow_widget), time,
	(XtTimerCallbackProc)SubGraphsMotif::trace_scan, subgraphs);
  }
}

Attr *SubGraphsMotif::new_attr( void *object, attr_sItem *items, int num) 
{
  return new AttrMotif( parent_wid, this, object, items, num);
}
