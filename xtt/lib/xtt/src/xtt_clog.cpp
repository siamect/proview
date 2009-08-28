/* 
 * Proview   $Id: xtt_clog.cpp,v 1.5 2008-10-31 12:51:36 claes Exp $
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

/* xtt_clog.cpp -- Console log in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"

#include "co_lng.h"
#include "co_wow.h"
#include "xtt_clog.h"
#include "rt_xnav_msg.h"

CLog::CLog( void *clog_parent_ctx,
	    const char *clog_name,
	    pwr_tStatus *status) :
  parent_ctx(clog_parent_ctx),
  clognav(NULL), clog_displayed(0), help_cb(0), close_cb(0), filesel_loaded(false)
{
  *status = 1;
}


//
//  Delete clog
//
CLog::~CLog()
{
}

void CLog::activate_next_file()
{
  set_clock_cursor();
  if ( !clognav->next_file())
    wow->DisplayError( "File error", "No next file found");
  reset_cursor();
}

void CLog::activate_prev_file()
{
  set_clock_cursor();
  if ( !clognav->prev_file())
    wow->DisplayError( "File error", "No previous file found");
  reset_cursor();
}

void CLog::activate_help()
{
  if ( help_cb)
    (help_cb)( parent_ctx, "opg_nav_systemmsg");
}



