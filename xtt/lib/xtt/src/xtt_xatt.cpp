/* 
 * Proview   $Id: xtt_xatt.cpp,v 1.10 2008-10-31 12:51:36 claes Exp $
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

/* xtt_xatt.cpp -- Display object attributes */

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
#include "co_lng.h"
#include "xtt_xatt.h"
#include "xtt_xattnav.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"


void XAtt::message_cb( void *xatt, char severity, const char *message)
{
  ((XAtt *)xatt)->message( severity, message);
}

void XAtt::change_value_cb( void *xatt)
{
  ((XAtt *)xatt)->change_value(1);
}

//
//  Callbackfunctions from menu entries
//
void XAtt::activate_display_object()
{
  if ( call_method_cb) {
    (call_method_cb)( parent_ctx,
		      "$Object-RtNavigator",
		      "$Object-RtNavigatorFilter",
		      objar, 
		      xmenu_eItemType_Object,
		      xmenu_mUtility_AttrEditor, NULL);
  }
}

void XAtt::activate_show_cross()
{
  if ( call_method_cb) {
    (call_method_cb)( parent_ctx,
		      "$Object-OpenCrossref",
		      "$Object-OpenCrossrefFilter",
		      objar,
		      xmenu_eItemType_Object,
		      xmenu_mUtility_AttrEditor, NULL);
  }
}

void XAtt::activate_open_classgraph()
{
  if ( call_method_cb) {
    (call_method_cb)( parent_ctx,
		      "$Object-OpenObjectGraph",
		      "$Object-OpenObjectGraphFilter",
		      objar, 
		      xmenu_eItemType_Object,
		      xmenu_mUtility_AttrEditor, NULL);
  }
}

void XAtt::activate_open_plc()
{
  if ( call_method_cb) {
    (call_method_cb)( parent_ctx,
		      "$Object-OpenTrace",
		      "$Object-OpenTraceFilter",
		      objar, 
		      xmenu_eItemType_Object,
		      xmenu_mUtility_AttrEditor, NULL);
  }
}

void XAtt::activate_help()
{
  // Not yet implemented
}

int XAtt::open_changevalue( char *name)
{
  int sts;

  sts = ((XAttNav*)xattnav)->select_by_name( name);
  if ( EVEN(sts)) return sts;

  change_value(0);
  return XATT__SUCCESS;
}

void XAtt::swap( int mode)
{
  xattnav->swap( mode);
}

XAtt::~XAtt()
{
}

XAtt::XAtt( 
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
        int             *xa_sts) :
 	parent_ctx(xa_parent_ctx), 
	objar(*xa_objar), 
	input_open(0), input_multiline(0), 
	close_cb(0), redraw_cb(0), client_data(0)
{
  *xa_sts = XATT__SUCCESS;
}


void XAtt::xatt_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			   unsigned long item_type, unsigned long utility, 
			   char *arg, int x, int y)
{
  if ( ((XAtt *)ctx)->popup_menu_cb)
    (((XAtt *)ctx)->popup_menu_cb) ( ((XAtt *)ctx)->parent_ctx, attrref,
				   item_type, utility, arg, x, y);
}

int XAtt::xatt_is_authorized_cb( void *ctx, unsigned int access)
{
  XAtt	*xatt = (XAtt *)ctx;

  if ( xatt->is_authorized_cb)
    return (xatt->is_authorized_cb)( xatt->parent_ctx, access);
  return 0;
}



