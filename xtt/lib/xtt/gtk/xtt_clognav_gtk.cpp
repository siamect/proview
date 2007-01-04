/* 
 * Proview   $Id: xtt_clognav_gtk.cpp,v 1.1 2007-01-04 08:29:32 claes Exp $
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

/* xtt_clognav_gtk.cpp -- Console message window. */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_syi.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "xtt_clognav_gtk.h"
#include "xtt_menu.h"

CLogNavGtk::CLogNavGtk( void *clog_parent_ctx,
			GtkWidget *clog_parent_wid,
			GtkWidget **w) :
  CLogNav( clog_parent_ctx), parent_wid(clog_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new( init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  *w = form_widget;
}


//
//  Delete ev
//
CLogNavGtk::~CLogNavGtk()
{
  delete brow;
  gtk_widget_destroy( form_widget);
}

void CLogNavGtk::set_input_focus()
{
  gtk_widget_grab_focus( brow_widget);
}







