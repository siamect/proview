/* 
 * Proview   $Id: xtt_xattone.cpp,v 1.5 2007-01-04 08:22:47 claes Exp $
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

/* xtt_xattone.cpp -- Display object attributes */

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
#include "xtt_xattone.h"
#include "xtt_xattnav.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"
#include "pwr_privilege.h"



int XAttOne::open_changevalue( char *name)
{
  change_value(0);
  return XATT__SUCCESS;
}

XAttOne::~XAttOne()
{
}

XAttOne::XAttOne( void *xa_parent_ctx, 
		  pwr_sAttrRef *xa_aref,
		  char *xa_title,
		  unsigned int xa_priv,
		  int *xa_sts) :
  parent_ctx(xa_parent_ctx), aref(*xa_aref), priv(xa_priv),
  input_open(0), input_multiline(0), close_cb(0), client_data(0)
{
  *xa_sts = XATT__SUCCESS;
}



