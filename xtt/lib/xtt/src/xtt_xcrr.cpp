/* 
 * Proview   $Id: xtt_xcrr.cpp,v 1.5 2007-01-04 08:22:47 claes Exp $
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

/* xtt_xcrr.cpp -- Display object crossreferences */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "rt_xnav_msg.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "xtt_xcrr.h"
#include "xtt_xattnav.h"
#include "co_lng.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"


XCrr::~XCrr()
{
}

XCrr::XCrr( 
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
        int             *xa_sts) :
 	parent_ctx(xa_parent_ctx), 
	objar(*xa_objar), 
	input_open(0), input_multiline(0), 
	close_cb(0), redraw_cb(0), popup_menu_cb(0), start_trace_cb(0),
	client_data(0)
{
  *xa_sts = XATT__SUCCESS;
}


void XCrr::xcrr_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			       unsigned long item_type, unsigned long utility,
			       char *arg, int x, int y)
{
  if ( ((XCrr *)ctx)->popup_menu_cb)
    (((XCrr *)ctx)->popup_menu_cb) ( ((XCrr *)ctx)->parent_ctx, attrref,
				   item_type, utility, arg, x, y);
}

void XCrr::xcrr_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((XCrr *)ctx)->start_trace_cb)
    ((XCrr *)ctx)->start_trace_cb( ((XCrr *)ctx)->parent_ctx, objid, name);
}

void XCrr::xcrr_close_cb( void *ctx)
{
  if ( ((XCrr *)ctx)->close_cb)
    ((XCrr *)ctx)->close_cb( ((XCrr *)ctx)->parent_ctx, ctx);
}




