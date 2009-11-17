/* 
 * Proview   $Id: wb_watt.cpp,v 1.12 2008-10-31 12:51:32 claes Exp $
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

/* wb_watt.cpp -- Display object attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "wb_watt_msg.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_watt.h"
#include "wb_wattnav.h"
#include "wb_wtt.h"
#include "wb_wnav.h"
#include "cow_xhelp.h"


void WAtt::message_cb( void *watt, char severity, const char *message)
{
  ((WAtt *)watt)->message( severity, message);
}

void WAtt::change_value_cb( void *watt)
{
  ((WAtt *)watt)->change_value(1);
}

void WAtt::set_editmode( int editmode, ldh_tSesContext ldhses)
{
  this->ldhses = ldhses;
  this->editmode = editmode;
  ((WAttNav *)wattnav)->set_editmode( editmode, ldhses);
}

int WAtt::open_changevalue( const char *name)
{
  int sts;

  sts = ((WAttNav*)wattnav)->select_by_name( name);
  if ( EVEN(sts)) return sts;

  change_value(0);
  return WATT__SUCCESS;
}

WAtt::~WAtt()
{
}

WAtt::WAtt( 
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses, 
	pwr_sAttrRef 	wa_aref,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname) :
 	parent_ctx(wa_parent_ctx), 
	ldhses(wa_ldhses), aref(wa_aref), editmode(wa_editmode), 
	input_open(0), input_multiline(0), 
	close_cb(0), redraw_cb(0), client_data(0)
{
}











