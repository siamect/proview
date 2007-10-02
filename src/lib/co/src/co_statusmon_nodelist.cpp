/* 
 * Proview   $Id: co_statusmon_nodelist.cpp,v 1.8 2007-10-02 15:53:20 claes Exp $
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
#include "co_syi.h"

#include "co_lng.h"
#include "co_wow.h"
#include "co_statusmon_nodelist.h"
#include "statussrv_utl.h"
#include "rt_xnav_msg.h"

Nodelist::Nodelist( void *nodelist_parent_ctx,
		    char *nodelist_name, int nodelist_mode, 
		    int nodelist_view_node_descr, pwr_tStatus *status) :
  parent_ctx(nodelist_parent_ctx),
  nodelistnav(NULL), nodelist_displayed(0), help_cb(0), close_cb(0), mode(nodelist_mode),
  view_node_descr(nodelist_view_node_descr)
{
  strcpy( remote_gui, "");
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

void Nodelist::add_node_ok( Nodelist *nodelist, char *node_name, 
			    char *description, char *opplace)
{
  nodelist->nodelistnav->add_node( node_name, description, opplace);
}

void Nodelist::activate_add_node()
{
  open_input_dialog( "Node name", "Description", "Operatorplace", "Add Node", "",
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

static void get_display( char *disp)
{
  char display[80] = "";
  char name[80];
  pwr_tStatus sts;

  char *val = getenv("DISPLAY");
  if ( val)
    strcpy( display, val);

  if ( strcmp( display, "") == 0 ||
       strcmp( display, ":0") == 0) {
    syi_NodeName( &sts, name, sizeof(name));
    strcpy( display, name);
    strcat( display, ":0");
  }
  strcpy( disp, display);
}

void Nodelist::activate_open_xtt()
{
  char node_name[80];
  int sts;
  char display[80];

  sts = nodelistnav->get_selected_node( node_name);
  if ( EVEN(sts)) {
    nodelistnav->wow->DisplayError( "Open Xtt", "Select a node");
    return;
  }

  get_display( display);
  statussrv_XttStart( node_name, "", "", display, remote_gui);
}

void Nodelist::activate_open_opplace()
{
  int sts;
  char node_name[80];
  pwr_tOName opplace;
  char display[80];

  sts = nodelistnav->get_selected_node( node_name);
  if ( EVEN(sts)) {
    nodelistnav->wow->DisplayError( "Open Xtt", "Select a node");
    return;
  }
  sts = nodelistnav->get_selected_opplace( opplace);

  get_display( display);
  statussrv_XttStart( node_name, opplace, "", display, remote_gui);
}

void Nodelist::activate_open_rtmon()
{
  char node_name[80];
  int sts;
  char display[80];

  sts = nodelistnav->get_selected_node( node_name);
  if ( EVEN(sts)) {
    nodelistnav->wow->DisplayError( "Open Xtt", "Select a node");
    return;
  }

  get_display( display);
  statussrv_RtMonStart( node_name, "", display, remote_gui);
}

void Nodelist::activate_save()
{
  nodelistnav->save();
}
