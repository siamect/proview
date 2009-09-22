/** 
 * Proview   $Id$
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

/* co_logw.cpp -- History log window */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"

#include "co_lng.h"
#include "co_xhelp.h"
#include "co_logw.h"


CoLogW::CoLogW(
	void *msg_parent_ctx,
	const char *logw_name,
	pwr_tStatus *status) :
  parent_ctx(msg_parent_ctx), logwnav(NULL), displayed(0),
  size(0), max_size(500), wow(0)
{
  *status = 1;
  strcpy( name, logw_name);
}

void CoLogW::show( char categories[][20], char *item)
{
  logwnav->show( categories, item);
}








