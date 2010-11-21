/** 
 * Proview   $Id$
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

/* pn_viewernav_gtk.cpp -- Profinet viewer */
#ifdef PWRE_CONF_PNAK

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "pn_viewernav_gtk.h"

PnViewerNavGtk::PnViewerNavGtk (
	void *l_parent_ctx,
	GtkWidget *l_parent_wid,
	GtkWidget **w) :
  PnViewerNav( l_parent_ctx), parent_wid(l_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new(
	PnViewerNav::init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  // Create the root item
  *w = form_widget;
}


//
//  Delete ev
//
PnViewerNavGtk::~PnViewerNavGtk()
{
  delete brow;
  gtk_widget_destroy( form_widget);
}

void PnViewerNavGtk::set_input_focus()
{
  gtk_widget_grab_focus( brow_widget);
}


#endif
