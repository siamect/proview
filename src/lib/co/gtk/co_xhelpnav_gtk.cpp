/** 
 * Proview   $Id: co_xhelpnav_gtk.cpp,v 1.1 2007-01-04 07:51:41 claes Exp $
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

/* co_xhelpnav_gtk.cpp -- helptext navigator */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_msg.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "flow_utils.h"

#include "co_xhelpnav_gtk.h"

extern "C" {
#include "co_api.h"
}

void CoXHelpNavGtk::pop()
{
}

//
// Create the navigator widget
//
CoXHelpNavGtk::CoXHelpNavGtk(
	void *xn_parent_ctx,
	GtkWidget *xn_parent_wid,
	char *xn_name,
	xhelp_eUtility xn_utility,
	GtkWidget **w,
	pwr_tStatus *status) :
	CoXHelpNav(xn_parent_ctx,xn_name,xn_utility,status),
	parent_wid(xn_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new(
	CoXHelpNav::init_brow_base_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  displayed = 1;

  *w = form_widget;
  *status = 1;
}

//
//  Delete a nav context
//
CoXHelpNavGtk::~CoXHelpNavGtk()
{
  closing_down = 1;

  for ( int i = 0; i < brow_cnt; i++) {
    brow_DeleteSecondaryCtx( brow_stack[i]->ctx);
    brow_stack[i]->free_pixmaps();
    delete brow_stack[i];
  }
  delete brow;
  gtk_widget_destroy( form_widget);
}

void CoXHelpNavGtk::set_inputfocus()
{
  gtk_widget_grab_focus( brow_widget);
}






