/* 
 * Proview   $Id: xtt_ev.cpp,v 1.9 2007-01-04 08:22:46 claes Exp $
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

/* xtt_ev.cpp -- Alarm and event window in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "rt_mh_util.h"

#include "co_lng.h"
#include "xtt_ev.h"
#include "rt_xnav_msg.h"

// Static pointer to Ev for mh callbacks
Ev *Ev::ev = 0;


Ev::Ev( void *ev_parent_ctx,
	char *eve_name,
	char *ala_name,
	char *blk_name,
	pwr_tObjid ev_user,
	int display_ala,
	int display_eve,
	int display_blk,
	int display_return,
	int display_ack,
	int ev_beep,
	pwr_tStatus *status) :
  parent_ctx(ev_parent_ctx),
  user(ev_user), eve_display_ack(display_ack), 
  eve_display_return(display_return),
  start_trace_cb(NULL), display_in_xnav_cb(NULL), update_info_cb(NULL),
  help_cb(NULL), popup_menu_cb(0), sound_cb(0), eve(NULL), ala(NULL),
  connected(0), ala_displayed(0), eve_displayed(0), beep(ev_beep)
{
}


//
//  Delete ev
//
Ev::~Ev()
{
}

void Ev::eve_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((Ev *)ctx)->start_trace_cb)
    ((Ev *)ctx)->start_trace_cb( ((Ev *)ctx)->parent_ctx, objid, name);
}

void Ev::ala_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((Ev *)ctx)->start_trace_cb)
    ((Ev *)ctx)->start_trace_cb( ((Ev *)ctx)->parent_ctx, objid, name);
}

void Ev::blk_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((Ev *)ctx)->start_trace_cb)
    ((Ev *)ctx)->start_trace_cb( ((Ev *)ctx)->parent_ctx, objid, name);
}

void Ev::ev_popup_menu_cb( void *ctx, pwr_tAttrRef attrref,
			      unsigned long item_type, unsigned long utility,
			      char *arg, int x, int y)
{
  if ( ((Ev *)ctx)->popup_menu_cb)
    (((Ev *)ctx)->popup_menu_cb) ( ((Ev *)ctx)->parent_ctx, attrref, item_type,
				   utility, arg, x, y);
}

int Ev::ev_sound_cb( void *ctx, pwr_tAttrRef *attrref)
{
  if ( ((Ev *)ctx)->sound_cb)
    return (((Ev *)ctx)->sound_cb) ( ((Ev *)ctx)->parent_ctx, attrref);
  return 0;
}

char *Ev::ev_name_to_alias_cb( void *ctx, char *name)
{
  return ((Ev *)ctx)->name_to_alias( name);
}

void Ev::eve_display_in_xnav_cb( void *ctx, pwr_tAttrRef *arp)
{
  if ( ((Ev *)ctx)->display_in_xnav_cb)
    ((Ev *)ctx)->display_in_xnav_cb( ((Ev *)ctx)->parent_ctx, arp);
}

void Ev::ala_display_in_xnav_cb( void *ctx, pwr_tAttrRef *arp)
{
  if ( ((Ev *)ctx)->display_in_xnav_cb)
    ((Ev *)ctx)->display_in_xnav_cb( ((Ev *)ctx)->parent_ctx, arp);
}

void Ev::blk_display_in_xnav_cb( void *ctx, pwr_tAttrRef *arp)
{
  if ( ((Ev *)ctx)->display_in_xnav_cb)
    ((Ev *)ctx)->display_in_xnav_cb( ((Ev *)ctx)->parent_ctx, arp);
}

void Ev::eve_activate_print()
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/xnav.ps");
  eve->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s", filename);
  system(cmd);
}

void Ev::ala_activate_print()
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/xnav.ps");
  ala->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s", filename);
  system(cmd);
}

void Ev::blk_activate_print()
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/xnav.ps");
  blk->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s", filename);
  system(cmd);
}

void Ev::eve_activate_ack_last()
{
  mh_sEventId *id;
  int sts;

  sts = ala->get_last_not_acked( &id);
  if ( EVEN(sts)) return;
  ala->ack( id);
  eve->ack( id);
  mh_OutunitAck( id);
}

void Ev::eve_activate_help()
{
  if ( help_cb)
    (help_cb)( parent_ctx, "eventlist");
}

void Ev::eve_activate_helpevent()
{
  char	eventname[80];
  int 	sts;

  if ( help_cb) {
    sts = ev->eve->get_selected_event( eventname);
    if( ODD(sts))
      (help_cb)( parent_ctx, eventname);
  }
}

void Ev::ala_activate_help()
{
  if ( help_cb)
    (help_cb)( parent_ctx, "alarmlist");
}

void Ev::ala_activate_helpevent()
{
  char	eventname[80];
  int 	sts;

  if ( help_cb) {
    sts = ala->get_selected_event( eventname);
    if( ODD(sts))
      (help_cb)( parent_ctx, eventname);
  }
}

void Ev::blk_activate_help()
{
  if ( help_cb)
    (help_cb)( parent_ctx, "blocklist");
}

int	Ev::get_alarm_info( evlist_sAlarmInfo *info)
{
  return ala->get_alarm_info( info);
}

int	Ev::outunit_connect( pwr_tObjid	user)
{
  int		sts;

  // Wait for mh has flagged initizated
  mh_UtilWaitForMh();

  sts = mh_OutunitConnect(
		user,
		mh_eOutunitType_Operator,
		0,
		mh_ack_bc,
		mh_alarm_bc,
		mh_block_bc,
		mh_cancel_bc,
		mh_clear_alarmlist_bc,
		mh_clear_blocklist_bc,
		mh_info_bc,
		mh_return_bc
		);
  if (EVEN(sts)) return sts;

  connected = 1;
  return XNAV__SUCCESS;
}

void Ev::update( double scantime)
{
  int sts;
  int nodraw_set = 0;

  sts = mh_OutunitReceive();     
  while (ODD(sts))
  {
    if ( !nodraw_set)
    {
      eve->set_nodraw();
      ala->set_nodraw();
      nodraw_set = 1;
    }
    sts = mh_OutunitReceive();     
  }
  if ( nodraw_set)
  {
    eve->reset_nodraw();
    ala->reset_nodraw();
  }

  ala->beep( scantime);
}      

void Ev::ack_last_prio( unsigned long type, unsigned long prio)
{
  mh_sEventId 	*id;
  int		sts;

  sts = ala->get_last_not_acked_prio( &id, type, prio);
  if ( ODD(sts))
  {
    ala->ack( id);
    eve->ack( id);
    mh_OutunitAck( id);
  }
}

int Ev::get_last_not_acked_prio( mh_sEventId **id, unsigned long type, 
	unsigned long prio)
{
  return ala->get_last_not_acked_prio( id, type, prio);
}

void Ev::create_aliaslist( void *up)
{
  char alias_array[2][80];
  int nr;
  int i, j;
  int alias_size;
  ev_sAlias dum;
  pwr_sClass_User *userp = (pwr_sClass_User *)up;

  int listsize = MIN( sizeof(userp->SelectList)/sizeof(userp->SelectList[0]),
		      sizeof(alias_list)/sizeof(alias_list[0]));
						    
  for ( i = 0, j = 0; i < listsize; i++) {
    strcpy( alias_list[i].Alias, "");
    strcpy( alias_list[i].Object, "");
  }
  for ( i = 0, j = 0; i < listsize; i++) {
    nr = dcli_parse( userp->SelectList[i], " 	", "",
	     (char *) alias_array, sizeof( alias_array)/sizeof( alias_array[0]), 
	     sizeof( alias_array[0]), 0);
    if ( nr < 2)
      continue;

    cdh_ToUpper( alias_list[j].Object, alias_array[0]);
    strncpy( alias_list[j].Alias, alias_array[1], sizeof(alias_list[j].Alias));
    alias_list[j].Alias[sizeof(alias_list[j].Alias)-1] = 0;
    j++;
  }
  alias_size = j;

  // Order
  for ( i = alias_size - 1; i > 0; i--) {
    for ( j = 0; j < i; j++) {
      if ( strcmp( alias_list[j].Object, alias_list[j+1].Object) <= 0) {
	memcpy( &dum, &alias_list[j+1], sizeof(dum));
	memcpy( &alias_list[j+1], &alias_list[j], sizeof(dum));
	memcpy( &alias_list[j], &dum, sizeof(dum));
      }
    }
  }
}

char *Ev::name_to_alias( char *name)
{
  char oname[120];
  static char alias[40];

  cdh_ToUpper( oname, name);

  for ( int i = 0; i < (int)(sizeof(alias_list)/sizeof(alias_list[0])); i++) {
    if ( alias_list[i].Alias[0] == 0)
      break;
    if ( strcmp( alias_list[i].Object, "") != 0 && 
	 strncmp( oname, alias_list[i].Object, strlen(alias_list[i].Object)) == 0) {
      strcpy( alias, alias_list[i].Alias);
      return alias;
    }	 
  }
  strcpy( alias, "");
  return alias;
}

pwr_tStatus Ev::mh_ack_bc( mh_sAck *MsgP)
{
  if ( ev->eve_display_ack)
  {
    // Insert in eve
    ev->eve->event_ack( MsgP);
  }
  ev->ala->event_ack( MsgP);

  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

pwr_tStatus Ev::mh_return_bc( mh_sReturn *MsgP)
{
  if ( ev->eve_display_return)
  {
    // Insert in eve
    ev->eve->event_return( MsgP);
  }
  ev->ala->event_return( MsgP);

  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

pwr_tStatus Ev::mh_alarm_bc( mh_sMessage *MsgP)
{
  ev->eve->event_alarm( MsgP);
  ev->ala->event_alarm( MsgP);
  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

pwr_tStatus Ev::mh_block_bc( mh_sBlock *MsgP)
{
  ev->blk->event_block( MsgP);
  ev->eve->event_block( MsgP);
  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

pwr_tStatus Ev::mh_cancel_bc( mh_sReturn *MsgP)
{
  ev->ala->event_cancel( MsgP);
  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

pwr_tStatus Ev::mh_info_bc( mh_sMessage *MsgP)
{
  ev->eve->event_info( MsgP);
  ev->ala->event_info( MsgP);
  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

pwr_tStatus Ev::mh_clear_alarmlist_bc( pwr_tNodeIndex nix)
{
  ev->ala->event_clear_alarmlist( nix);
  return 1;
}

pwr_tStatus Ev::mh_clear_blocklist_bc( pwr_tNodeIndex nix)
{
  ev->blk->event_clear_alarmlist( nix);
  return 1;
}


