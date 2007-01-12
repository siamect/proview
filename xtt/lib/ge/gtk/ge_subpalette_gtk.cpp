/* 
 * Proview   $Id: ge_subpalette_gtk.cpp,v 1.2 2007-01-12 07:58:06 claes Exp $
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
#include <ctype.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "co_dcli.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_gtk.h"

#include "ge_attr.h"
#include "ge_subpalette_gtk.h"

//
// Create the navigator widget
//
SubPaletteGtk::SubPaletteGtk(
	void *xn_parent_ctx,
	GtkWidget *xn_parent_wid,
	char *xn_name,
	GtkWidget **w,
	pwr_tStatus *status) :
  SubPalette( xn_parent_ctx, xn_name, status),
	parent_wid(xn_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new(
	SubPalette::init_brow_cb, this, &brow_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}

//
//  Delete a nav context
//
SubPaletteGtk::~SubPaletteGtk()
{
  delete brow;
  // XtDestroyWidget( form_widget);
}

void SubPaletteGtk::set_inputfocus( int focus)
{
  // TODO if ( focus) Set border ...
  
  gtk_widget_grab_focus( brow_widget);
}

