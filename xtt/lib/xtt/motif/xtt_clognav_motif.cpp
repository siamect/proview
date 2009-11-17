/* 
 * Proview   $Id: xtt_clognav_motif.cpp,v 1.3 2008-10-31 12:51:36 claes Exp $
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

/* xtt_clognav_motif.cpp -- Console message window. */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "co_syi.h"

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
#include "cow_mrm_util.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"
#include "xtt_clognav_motif.h"
#include "xtt_menu.h"

CLogNavMotif::CLogNavMotif( void *clog_parent_ctx,
			    Widget clog_parent_wid,
			    Widget *w) :
  CLogNav( clog_parent_ctx), parent_wid(clog_parent_wid)
{
  form_widget = ScrolledBrowCreate( parent_wid, (char*) "CLogNav", NULL, 0, 
	init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  *w = form_widget;
}


//
//  Delete ev
//
CLogNavMotif::~CLogNavMotif()
{
  delete brow;
  XtDestroyWidget( form_widget);
}

void CLogNavMotif::set_input_focus()
{
  if ( flow_IsViewable( brow_widget))
    XtCallAcceptFocus( brow_widget, CurrentTime);
}







