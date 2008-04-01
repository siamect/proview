/* 
 * Proview   $Id: xtt_op.cpp,v 1.9 2008-04-01 14:19:30 claes Exp $
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

/* xtt_op.cpp -- Alarm and event window in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "co_wow.h"
#include "co_lng.h"
#include "xtt_op.h"
#include "rt_xnav_msg.h"

#define OP_HEIGHT_MIN 80
#define OP_HEIGHT_INC 20
#define OP_HEIGHT_MAX (OP_HEIGHT_MIN + 3 * OP_HEIGHT_INC)


Op::Op( void *op_parent_ctx,
	char *opplace,
	pwr_tStatus *status) :
  parent_ctx(op_parent_ctx), start_jop(0), 
  jop(NULL), command_cb(NULL), map_cb(NULL), help_cb(NULL), 
  close_cb(NULL), get_alarm_info_cb(NULL), ack_last_cb(NULL)
{
}


//
//  Delete op
//
Op::~Op()
{
  if ( jop)
    jop->close();
}

int Op::jop_command( char *command) 
{ 
  if ( jop) 
    return jop->command( command);
  return 0;
}

void Op::scan()
{
  if ( jop)
    jop->scan();
}

int Op::appl_action( int idx)
{
  char cmd[80];
  char name[80];
  int sts;

  if ( command_cb) {
    sts = gdh_ObjidToName( button_objid[idx], name, sizeof(name), cdh_mNName);
    strcpy( cmd, "ope gra/obj=");
    strcat( cmd, name);

    command_cb( parent_ctx, cmd);
  }
  return XNAV__SUCCESS;
}


void Op::activate_exit()
{
  if ( close_cb)
    (close_cb)(parent_ctx);
  else
    delete this;
}

void Op::activate_aalarm_ack()
{

  if ( ack_last_cb)
    (ack_last_cb)( parent_ctx, evlist_eEventType_Alarm, mh_eEventPrio_A);
}

void Op::activate_balarm_ack()
{
  if ( ack_last_cb)
    (ack_last_cb)( parent_ctx, balarm_type, balarm_prio);
}

void Op::activate_alarmlist()
{
  char cmd[20] = "show alarm";
  if ( command_cb)
    command_cb( parent_ctx, cmd);
}

void Op::activate_eventlist()
{
  char cmd[20] = "show event";
  if ( command_cb)
    command_cb( parent_ctx, cmd);
}

void Op::activate_eventlog()
{
  char cmd[20] = "show histlist";
  if ( command_cb)
    command_cb( parent_ctx, cmd);
}

void Op::activate_navigator()
{
  if ( map_cb)
    map_cb( parent_ctx);
}

void Op::activate_help()
{
  if ( help_cb)
    help_cb( parent_ctx, "index");
}

void Op::jop_command_cb( void *op, char *command)
{
  if ( ((Op *)op)->command_cb)
    ((Op *)op)->command_cb( ((Op *)op)->parent_ctx, command);
}
