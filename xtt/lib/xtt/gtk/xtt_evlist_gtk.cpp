/* 
 * Proview   $Id: xtt_evlist_gtk.cpp,v 1.1 2007-01-04 08:29:32 claes Exp $
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

/* xtt_evlist_gtk.cpp -- Alarm or event list in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "co_wow_gtk.h"
#include "co_lng.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "xtt_evlist_gtk.h"

EvListGtk::EvListGtk( void *ev_parent_ctx,
		      GtkWidget *ev_parent_wid,
		      ev_eType ev_type, 
		      int ev_size,
		      GtkWidget **w) :
  EvList( ev_parent_ctx, ev_type, ev_size), parent_wid(ev_parent_wid)
{
  form_widget = scrolledbrowwidgetgtk_new( init_brow_cb, this, &brow_widget);

  gtk_widget_show_all( brow_widget);

  *w = form_widget;
}


//
//  Delete ev
//
EvListGtk::~EvListGtk()
{
  delete brow;
  gtk_widget_destroy( form_widget);
}

void EvListGtk::set_input_focus()
{
  gtk_widget_grab_focus( brow_widget);
}

void EvListGtk::bell()
{
  gdk_display_beep( gtk_widget_get_display( brow_widget));
}

void EvListGtk::popup_position( int x_event, int y_event, int *x, int *y)
{
  CoWowGtk::PopupPosition( brow_widget, x_event, y_event, x, y);
}
