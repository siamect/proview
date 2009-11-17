/* 
 * Proview   $Id: xtt_evlist_motif.cpp,v 1.2 2008-10-31 12:51:36 claes Exp $
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
 */

/* xtt_evlist_motif.cpp -- Alarm or event list in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"

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

#include "flow_x.h"
#include "cow_wow_motif.h"
#include "co_lng.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"
#include "xtt_evlist_motif.h"
#include "xtt_menu.h"

EvListMotif::EvListMotif( void *ev_parent_ctx,
			  Widget	ev_parent_wid,
			  ev_eType ev_type, 
			  int ev_size,
			  Widget *w) :
  EvList( ev_parent_ctx, ev_type, ev_size), parent_wid(ev_parent_wid)
{
  form_widget = ScrolledBrowCreate( parent_wid, (char*) "EvList", NULL, 0, 
				    init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  *w = form_widget;
}


//
//  Delete ev
//
EvListMotif::~EvListMotif()
{
  delete brow;
  XtDestroyWidget( form_widget);
}

void EvListMotif::set_input_focus()
{
  if ( flow_IsViewable( brow_widget))
    XtCallAcceptFocus( brow_widget, CurrentTime);
}

void EvListMotif::bell()
{
  flow_Bell( form_widget);
}

void EvListMotif::popup_position( int x_event, int y_event, int *x, int *y)
{
  CoWowMotif::PopupPosition( brow_widget, x_event, y_event, x, y);
}
