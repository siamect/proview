/* 
 * Proview   $Id: wb_wcast_gtk.cpp,v 1.2 2008-10-31 12:51:31 claes Exp $
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

/* wb_wcast_gtk.cpp -- Attribute object casting window. */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "cow_wow_gtk.h"
#include "wb_erep.h"
#include "wb_session.h"
#include "wb_attribute.h"
#include "co_msg.h"
#include "wb_wcast_gtk.h"


WCastGtk::WCastGtk( 
	void	*wc_parent_ctx,
	GtkWidget *wc_parent_wid,
	const char 	*wc_name,
	ldh_tSesContext wc_ldhses,
	pwr_sAttrRef wc_aref,
	pwr_tStatus *status
	) :
	WCast(wc_parent_ctx,wc_name,wc_ldhses,wc_aref,status), parent_wid( wc_parent_wid)
{
  wow = new CoWowGtk(wc_parent_wid);

  open_castlist();
  *status = 1;
}

WCastGtk::~WCastGtk()
{
  delete wow;
}


