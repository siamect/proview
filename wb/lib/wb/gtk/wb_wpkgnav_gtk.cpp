/* 
 * Proview   $Id: wb_wpkgnav_gtk.cpp,v 1.3 2008-10-31 12:51:31 claes Exp $
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


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "rt_load.h"
#include "wb_pkg_msg.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "wb_wpkg_gtk.h"
#include "wb_wpkgnav_gtk.h"
#include "wb_wnav.h"
#include "wb_wnav_brow.h"
#include "wb_wnav_item.h"
#include "wb_error.h"

//
// Create the navigator widget
//
WPkgNavGtk::WPkgNavGtk(
	void 		*wa_parent_ctx,
	GtkWidget      	*wa_parent_wid,
	const char     	*wa_name,
	wb_eUtility	wa_utility,
	GtkWidget      	**w,
	pwr_tStatus 	*status) :
	WPkgNav(wa_parent_ctx,wa_name,wa_utility,status), parent_wid(wa_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new(
	init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  *w = form_widget;
  *status = 1;
}

//
//  Delete a nav context
//
WPkgNavGtk::~WPkgNavGtk()
{
  delete brow;
  gtk_widget_destroy( form_widget);
}

void WPkgNavGtk::set_inputfocus()
{
  if ( !displayed)
    return;

  gtk_widget_grab_focus( brow_widget);
}
















