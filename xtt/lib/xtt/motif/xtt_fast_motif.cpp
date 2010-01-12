/* 
 * Proview   $Id: xtt_fast_motif.cpp,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "cow_wow_motif.h"
#include "rt_xnav_msg.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"
#include "glow_growctx.h"
#undef min
#undef max
#include "glow_growapi.h"
#include "glow_growwidget_motif.h"
#include "glow_curvectx.h"
#include "glow_curveapi.h"
#include "glow_curvewidget_motif.h"

#include "xtt_xnav.h"
#include "rt_fast.h"
#include "xtt_fast_motif.h"

#include "flow_x.h"
#include "ge_curve_motif.h"


XttFastMotif::XttFastMotif( void *parent_ctx,
			    Widget	parent_wid,
			    char *name,
			    Widget *w,
			    pwr_sAttrRef *fast_arp,
			    int *sts) :
  XttFast( parent_ctx, name, fast_arp, sts), parent_widget(parent_wid)
{
  char title[250];
    
  *sts = XNAV__SUCCESS;

  curve = new GeCurveMotif( this, parent_widget, title, NULL, gcd, 0);
  curve->close_cb = fast_close_cb;
  curve->help_cb = fast_help_cb;

  wow = new CoWowMotif( parent_widget);
  timerid = wow->timer_new();

  timerid->add( 1000, fast_scan, this);
}

XttFastMotif::~XttFastMotif()
{
  timerid->remove();

  for ( int i = 0; i < fast_cnt; i++) {
    gdh_UnrefObjectInfo( new_subid);
  }
  delete curve;
}








