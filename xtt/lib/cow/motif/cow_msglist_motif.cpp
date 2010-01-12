/** 
 * Proview   $Id: co_msglist_motif.cpp,v 1.2 2008-10-31 12:51:30 claes Exp $
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

/* cow_msglist_motif.cpp -- Message list */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
//#include "pwr_baseclasses.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// Status is defined as int i xlib...
#ifdef Status
# undef Status
#endif

extern "C" {
#include "flow_x.h"
#include "cow_mrm_util.h"
}
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"

#include "cow_msglist_motif.h"

MsgListMotif::MsgListMotif(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	Widget *w) :
  MsgList( ev_parent_ctx), parent_wid(ev_parent_wid)
{
  form_widget = ScrolledBrowCreate( parent_wid, (char *)"MsgList", NULL, 0, 
	MsgList::init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  *w = form_widget;
}


//
//  Delete ev
//
MsgListMotif::~MsgListMotif()
{
  delete brow;
  XtDestroyWidget( form_widget);
}

void MsgListMotif::set_input_focus()
{
  if ( flow_IsViewable( brow_widget))
    XtCallAcceptFocus( brow_widget, CurrentTime);
}



