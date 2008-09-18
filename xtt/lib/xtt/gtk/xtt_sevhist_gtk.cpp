/* 
 * Proview   $Id: xtt_sevhist_gtk.cpp,v 1.1 2008-09-18 14:52:58 claes Exp $
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

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include <gtk/gtk.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_wow_gtk.h"
#include "rt_xnav_msg.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_curvectx.h"
#include "glow_curveapi.h"
#include "xtt_xnav.h"
#include "xtt_sevhist_gtk.h"
#include "ge_curve_gtk.h"


XttSevHistGtk::XttSevHistGtk( void *parent_ctx,
			    GtkWidget *parent_wid,
			    char *name,
			    GtkWidget **w,
			    pwr_tOid *xn_oid,
			    pwr_tOName *xn_aname,
			    sevcli_tCtx xn_scctx,
			    int *sts) :
  XttSevHist( parent_ctx, name, xn_oid, xn_aname, xn_scctx, sts), parent_widget(parent_wid)
{
  char title[250];
    
  if ( EVEN(*sts))
    // Error from XttSevHist
    return;

  *sts = XNAV__SUCCESS;

  curve = new GeCurveGtk( this, parent_widget, title, NULL, gcd, 0);
  curve->close_cb = sevhist_close_cb;
  curve->help_cb = sevhist_help_cb;
  curve->higher_res_cb = sevhist_higher_res_cb;
  curve->lower_res_cb = sevhist_lower_res_cb;

  wow = new CoWowGtk( parent_widget);
  timerid = wow->timer_new();

  timerid->add( 1000, sevhist_scan, this);
}

XttSevHistGtk::~XttSevHistGtk()
{
  timerid->remove();

  delete curve;
}








