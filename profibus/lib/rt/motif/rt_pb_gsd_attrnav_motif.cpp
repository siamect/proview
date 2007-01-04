/* 
 * Proview   $Id: rt_pb_gsd_attrnav_motif.cpp,v 1.1 2007-01-04 08:43:47 claes Exp $
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

/* rt_pb_gsd_attrnav.cpp -- Display gsd attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_wow_motif.h"

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


#include "rt_pb_gsd.h"
#include "rt_pb_gsd_attr_motif.h"
#include "rt_pb_gsd_attrnav_motif.h"
#include "rt_pb_msg.h"


//
// Create the navigator widget
//
GsdAttrNavMotif::GsdAttrNavMotif( void *xn_parent_ctx,
				  Widget xn_parent_wid,
				  char *xn_name,
				  pb_gsd *xn_gsd,
				  int xn_edit_mode,
				  Widget *w,
				  pwr_tStatus *status) :
  GsdAttrNav( xn_parent_ctx, xn_name, xn_gsd, xn_edit_mode, status),
  parent_wid(xn_parent_wid)
{

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  wow = new CoWowMotif( brow_widget);
  trace_timerid = wow->timer_new();
  *status = 1;
}

//
//  Delete a nav context
//
GsdAttrNavMotif::~GsdAttrNavMotif()
{
  delete trace_timerid;
  delete wow;
  delete brow;
  XtDestroyWidget( form_widget);
}

void GsdAttrNavMotif::set_inputfocus()
{
  XtCallAcceptFocus( brow_widget, CurrentTime);
}
