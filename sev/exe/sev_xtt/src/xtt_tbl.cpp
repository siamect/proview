/* 
 * Proview   $Id: xtt_tbl.cpp,v 1.1 2008-07-17 11:18:31 claes Exp $
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

/* xtt_tbl.cpp -- Display object attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include "co_cdh.h"
#include "co_time.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "flow_msg.h"

#include "xtt_tbl.h"
#include "xtt_tblnav.h"

XttTbl::~XttTbl()
{
}

XttTbl::XttTbl( void *xn_parent_ctx, sevcli_tCtx xn_sevcli) :
  parent_ctx(xn_parent_ctx), sevcli(xn_sevcli)
{
}

void XttTbl::message( void *xtttbl, char severity, char *message)
{
  ((XttTbl *)xtttbl)->message( severity, message);
}

void XttTbl::activate_opendshist()
{
  sevcli_sHistItem *hi;

  tblnav->get_select( &hi);

  dshist_new( hi->oid, hi->aname);
}
