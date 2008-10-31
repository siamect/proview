/* 
 * Proview   $Id: xtt_tblnav_gtk.cpp,v 1.2 2008-10-31 12:51:30 claes Exp $
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

/* sev_tblnav.cpp -- Sev Table Viewer */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "co_cdh.h"
#include "co_time.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "flow_msg.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_gtk.h"


#include "xtt_tbl_gtk.h"
#include "xtt_tblnav_gtk.h"

//
// Create the navigator widget
//
TblNavGtk::TblNavGtk(
	void *xn_parent_ctx,
	GtkWidget	*xn_parent_wid,
	sevcli_sHistItem  *xn_itemlist,
	int xn_item_cnt,
	GtkWidget **w,
	pwr_tStatus *status) :
  TblNav( xn_parent_ctx, xn_itemlist, xn_item_cnt, status),
  parent_wid(xn_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new(
	TblNav::init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}
//
//  Delete a nav context
//

TblNavGtk::~TblNavGtk()
{
  delete brow;
  gtk_widget_destroy( form_widget);
}

void TblNavGtk::set_inputfocus()
{
  gtk_widget_grab_focus( brow_widget);
}
