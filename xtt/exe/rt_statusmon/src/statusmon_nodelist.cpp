/* 
 * Proview   $Id: statusmon_nodelist.cpp,v 1.1 2007-05-11 15:04:14 claes Exp $
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

/* xtt_nodelist.cpp -- Console log in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"

#include "co_lng.h"
#include "co_wow.h"
#include "statusmon_nodelist.h"
#include "rt_xnav_msg.h"

Nodelist::Nodelist( void *nodelist_parent_ctx,
	    char *nodelist_name,
	    pwr_tStatus *status) :
  parent_ctx(nodelist_parent_ctx),
  nodelistnav(NULL), nodelist_displayed(0), help_cb(0), close_cb(0)
{
  *status = 1;
}


//
//  Delete nodelist
//
Nodelist::~Nodelist()
{
}

void Nodelist::activate_help()
{
  if ( help_cb)
    (help_cb)( parent_ctx, "consolelog");
}

void Nodelist::find_node_cb( void *ctx, pwr_tOid oid)
{
  Nodelist *nodelist = (Nodelist *)ctx;

  if ( nodelist->nodelistnav->select_node( oid.oix))
    nodelist->pop();  
}

void Nodelist::add_node_ok( Nodelist *nodelist, char *node_name)
{
  nodelist->nodelistnav->add_node( node_name);
}

void Nodelist::activate_add_node()
{
  open_input_dialog( "Enter node name", "Add Node", "",
		     add_node_ok);
}

void remove_node_ok( void *ctx, void *data)
{
  Nodelist *nodelist = (Nodelist *)ctx;

  nodelist->nodelistnav->remove_node( (char *)data);
}

void Nodelist::activate_remove_node()
{
  static char node_name[80];
  char msg[100];
  int sts;

  sts = nodelistnav->get_selected_node( node_name);
  if ( EVEN(sts)) {
    nodelistnav->wow->DisplayError( "Remove Node", "Select a node");
    return;
  }
  sprintf( msg, "Do you want to remove node %s", node_name);

  nodelistnav->wow->DisplayQuestion( this, "Remove Node", msg, remove_node_ok,
				     NULL, node_name);
}

void Nodelist::activate_save()
{
  nodelistnav->save();
}
