/* 
 * Proview   $Id: wb_gre_gtk.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb_gre_gtk.cpp
   This module creates the flow window and handles
   the flow callbacks.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pwr.h"
#include "pwr_baseclasses.h"

#include <gtk/gtk.h>

#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_widget_gtk.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_vldh.h"
#include "wb_foe.h"
#include "wb_goec.h"
#include "wb_goen.h"
#include "wb_utl_api.h"
#include "wb_gre_gtk.h"
#include "wb_tra.h"

//
// Constructor
//
WGreGtk::WGreGtk( void *wg_parent_ctx,
		  GtkWidget *wg_parent_wid,
		  char *name) :
  WGre( wg_parent_ctx, 0), parent_wid(wg_parent_wid), trace_timerid(0)
{
  ctx_init();

  form_widget = scrolledflowwidgetgtk_new( init_flow, 
					   this, &flow_widget);
  gtk_widget_show_all( flow_widget);
  popupmenu_mode = GRE_POPUPMENUMODE_OBJECT;
}

WGreGtk::~WGreGtk()
{
  gtk_widget_destroy( form_widget);
}

int WGreGtk::new_navigator( GtkWidget *parent)
{
  nav_widget = flownavwidgetgtk_new( flow_widget);
  return 1;
}

int WGreGtk::new_navigator_popup()
{
#if 0
  Arg 		args[20];
  int		i;

  /* Create navigator popup */
  i = 0;
  XtSetArg(args[i],XmNallowShellResize, TRUE); i++;
  XtSetArg(args[i],XmNallowResize, TRUE); i++;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  XtSetArg(args[i],XmNx,500);i++;
  XtSetArg(args[i],XmNy,500);i++;

  nav_shell = XmCreateDialogShell( flow_widget, "Navigator",
				   args, i);
  XtManageChild( nav_shell);

  i = 0;
  XtSetArg(args[i],XmNwidth,200);i++;
  XtSetArg(args[i],XmNheight,200);i++;
  nav_widget = FlowCreateNav( nav_shell, "navigator",
			      args, i, flow_widget);
  XtManageChild( nav_widget);
  XtRealizeWidget( nav_shell);

#endif
  return 1;
}

static gboolean wgregtk_trace_scan( void *data)
{
  WGreGtk::trace_scan( (WGreGtk *)data);
  return FALSE;
}

void WGreGtk::trace_scan( WGreGtk *gre)
{
  if ( gre->trace_started)
  {
    flow_TraceScan( gre->flow_ctx);

    gre->trace_timerid = g_timeout_add( 500,
			 wgregtk_trace_scan, gre);
  }
}

void WGreGtk::trace_start()
{
  trace_started = 1;
  trace_scan( this);
}

void WGreGtk::trace_stop()
{
  if ( trace_started) {
    trace_started = 0;
    if ( trace_timerid)
      g_source_remove( trace_timerid);
  }
}

void WGreGtk::get_popup_position( int *x, int *y)
{
  gint wind_x, wind_y;

  GtkWidget *parent = gtk_widget_get_parent( form_widget);
  while( !GTK_IS_WINDOW(parent))
    parent = gtk_widget_get_parent( parent);
  if ( parent) {
    gtk_window_get_position( GTK_WINDOW(parent), &wind_x, &wind_y);
    *x += wind_x;
    *y += wind_y + 40;
  }    
}
