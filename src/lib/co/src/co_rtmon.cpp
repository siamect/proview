/* 
 * Proview   $Id: co_rtmon.cpp,v 1.5 2008-10-31 12:51:30 claes Exp $
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

/* co_statusmon_nodelist.cpp -- Status Monitor */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh.h"

#include "co_lng.h"
#include "co_wow.h"
#include "co_rtmon.h"
#include "co_syi.h"
#include "rt_xnav_msg.h"

RtMon::RtMon( void *rtmon_parent_ctx,
	      const char *rtmon_name,
	      const char *rtmon_display,
	      pwr_tStatus *status) :
  parent_ctx(rtmon_parent_ctx),
  nodelistnav(NULL), rtmon_displayed(0), help_cb(0), close_cb(0)
{
  pwr_tStatus sts;

  if ( rtmon_display)
    strcpy( display, rtmon_display);
  else
    strcpy( display, "");
  syi_NodeName( &sts, nodename, sizeof(nodename));  

  *status = 1;
}


//
//  Delete nodelist
//
RtMon::~RtMon()
{
}

void RtMon::activate_help()
{
  if ( help_cb)
    (help_cb)( parent_ctx, "runtime monitor");
}

void RtMon::activate_start()
{
  char cmd[] = "rt_ini -i &";

  system( cmd);
}

void RtMon::activate_restart()
{
  char cmd[] = "rt_ini -r";

  system( cmd);
}

void RtMon::activate_stop()
{
  char nodename[40];
  char text[80] = "Do you want to stop Runtime Environment on ";
  pwr_tStatus sts;

  syi_NodeName( &sts, nodename, sizeof(nodename));
  strcat( text, nodename);

  wow->DisplayQuestion( this, "Stop Runtime", text, stop_ok_cb, 0, 0);
}

void RtMon::stop_ok_cb( void *ctx, void *data)
{
  char cmd[] = "rt_ini -s";

  system( cmd);
}

void RtMon::activate_reset()
{
  char cmd[] = ". pwr_stop.sh";

  system( cmd);
}

void RtMon::activate_xtt()
{
  pwr_tCmd cmd;
  char displaystr[120] = "";

  if ( strcmp( display, "") != 0)
    sprintf( displaystr, "--display %s", display);

  sprintf( cmd, "rt_xtt -q %s &", displaystr);
  system( cmd);
}

void RtMon::activate_op()
{
  pwr_tCmd cmd;
  char displaystr[120] = "";

  if ( strcmp( display, "") != 0)
    sprintf( displaystr, "--display %s", display);

  sprintf( cmd, "rt_xtt -q -s -c %s &", displaystr);

  system( cmd);
}
