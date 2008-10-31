/* 
 * Proview   $Id: wb_utl_gtk.cpp,v 1.2 2008-10-31 12:51:31 claes Exp $
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

/* wb_utl.c
   Utilitys for getting information about the plcprogram structure.  */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pwr.h"

#include <gtk/gtk.h>

#include "flow_ctx.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_foe_gtk.h"
#include "wb_utl_gtk.h"

static int gtk_initialized = 0;

int wb_utl_gtk::utl_foe_new( const char *name, pwr_tOid plcpgm,
			     ldh_tWBContext ldhwbctx, ldh_tSesContext ldhsesctx,
			     WFoe **foe, int map_window, ldh_eAccess access)
{
  pwr_tStatus sts = 1;

  *foe = WFoe::get( plcpgm);
  if ( !*foe)
    *foe = new WFoeGtk( 0, widget, name, plcpgm, ldhwbctx, ldhsesctx,
			map_window, access, &sts);
  return sts;
}

int wb_utl_gtk::utl_foe_new_local( WFoe *foe, const char *name, pwr_tOid plcpgm, 
				     ldh_tWBContext ldhwbctx, ldh_tSesContext ldhsesctx, 
				     vldh_t_node nodeobject, unsigned long windowindex, 
				     unsigned long new_window, WFoe **return_foe, 
				     int map_window, ldh_eAccess access, 
				     foe_eFuncAccess function_access)
{
  pwr_tStatus sts = 1;

  *return_foe = WFoe::get( plcpgm);
  if ( !*return_foe)
    *return_foe = new WFoeGtk( foe, ((WFoeGtk *)foe)->widgets.foe_window,
			       name, plcpgm, ldhwbctx, ldhsesctx,
			       nodeobject, windowindex, new_window,
			       map_window, access, function_access, &sts);
  return sts;
}
 

/*************************************************************************
*
* Name:		utl_create_mainwindow()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description: 	
*
**************************************************************************/

int wb_utl_gtk::create_mainwindow( int argc, char **argv)
{
  GtkWidget *toplevel;

  if ( !gtk_initialized) {
    gtk_init( &argc, &argv);
    gtk_initialized = 1;
  }

  toplevel = (GtkWidget *) g_object_new( GTK_TYPE_WINDOW,
					 "default-height", 100,
					 "default-width", 100,
					 "title", "Pwr wb",
					 NULL);
  gtk_widget_show_all( toplevel);
  g_object_set( toplevel, "visible", FALSE, NULL);

  widget = toplevel;

  return FOE__SUCCESS;
}

int wb_utl_gtk::destroy_mainwindow()
{
  gtk_widget_destroy( widget);
  return FOE__SUCCESS;
}
