/* 
 * Proview   $Id: wb_wtt.cpp,v 1.44 2008-10-31 12:51:32 claes Exp $
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

/* wb_wtt.cpp -- Display plant and node hiererachy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "co_wow.h"
#include "wb_utl_api.h"
#include "wb_lfu.h"
#include "co_login.h"
#include "rt_load.h"
#include "wb_foe_msg.h"
#include "wb_pwrb_msg.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_wtt.h"
#include "wb_wnav.h"
#include "wb_wnav_item.h"
#include "wb_pal.h"
#include "wb_watt.h"
#include "wb_wda.h"
#include "wb_wtt.h"
#include "wb_wnav_msg.h"
#include "wb_volume.h"
#include "wb_env.h"
#include "wb_wpkg.h"
#include "co_msgwindow.h"
#include "wb_wnav_selformat.h"
#include "wb_pwrs.h"
#include "wb_build.h"
#include "ge.h"


int Wtt::format_selection( void *ctx, pwr_sAttrRef attrref, 
			   char **value_return, int is_class, int is_attr,
			   wnav_eSelectionFormat format)
{
  static char buff[256];
  Wtt	*wtt = (Wtt *) ctx;
  int   select_syntax;
  int	select_volume, select_attr, select_type;

  switch ( format) {
  case wnav_eSelectionFormat_User:
    select_syntax = wtt->select_syntax;
    select_volume = wtt->select_volume;
    select_attr = wtt->select_attr;
    select_type = wtt->select_type;
    break;
  case wnav_eSelectionFormat_Graph:
    select_syntax = wnav_eSelectionMode_Normal;
    select_volume = 0;
    select_attr = 1;
    select_type = 1;
    break;
  case wnav_eSelectionFormat_Objid:
    select_syntax = wnav_eSelectionMode_Normal;
    select_volume = 1;
    select_attr = 0;
    select_type = 0;
    break;
  case wnav_eSelectionFormat_Attrref:
    select_syntax = wnav_eSelectionMode_Normal;
    select_volume = 1;
    select_attr = 1;
    select_type = 0;
    break;
  }
  if ( wnav_format_selection( wtt->ldhses, attrref, is_class, is_attr,  select_syntax, 
			     select_volume, select_attr, select_type, buff)) {
    *value_return = buff;      
    
    return 1;
  }
  return 0;
}

int Wtt::start_wizard( Wtt *wtt, pwr_tCid vcid)
{
  pwr_tCmd cmd;

  switch ( vcid) {
  case pwr_eClass_DirectoryVolume:
    strcpy( cmd, "@$pwr_exe/wb_wiz_directoryvolume");
    break;
  case pwr_eClass_RootVolume:
    strcpy( cmd, "@$pwr_exe/wb_wiz_rootvolume");
    break;
  default:
    return 1;
  }
  wtt->wnav->command( cmd);
  return 1;
}

void Wtt::set_twowindows_cb( void *wtt, int two, int display_w1,
        int display_w2)
{
  ((Wtt *)wtt)->set_twowindows( two, display_w1, display_w2);
}

pwr_tStatus Wtt::ldh_this_session_cb( void *ctx, ldh_sEvent *event)
{
  Wtt *wtt = (Wtt *) ctx;

  if ( wtt->input_open) {
    if ( wtt->keep_input_open)
      wtt->keep_input_open = 0;
    else
      wtt->close_change_value();
  }
  wtt->wnav->ldh_event( event);
  wtt->wnavnode->ldh_event( event);
  return 1;
}

pwr_tStatus Wtt::ldh_other_session_cb( void *ctx, ldh_sEvent *event)
{
  Wtt *wtt = (Wtt *) ctx;
 
  switch (event->Event) {
  case ldh_eEvent_SessionSaved:
    if ( wtt->input_open)
      wtt->close_change_value();
    wtt->wnav->ldh_refresh( pwr_cNObjid);
    wtt->wnavnode->ldh_refresh( pwr_cNObjid);
    break;
  default:
    ;
  }
  return 1;
}

void Wtt::uted_quit_cb( void *ctx) 
{
  ((Wtt *)ctx)->utedctx = NULL;
}

void Wtt::wpkg_quit_cb( void *ctx) 
{
  delete ((Wtt *)ctx)->wpkg;
  ((Wtt *)ctx)->wpkg = 0;
}

void Wtt::open_vsel_cb( void *ctx, wb_eType type, char *filename, wow_eFileSelType file_type)
{
  Wtt *wtt = (Wtt *)ctx;

  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, type, filename, file_type);

}

void Wtt::set_focus_default()
{
  if ( wnav_mapped)
    set_focus( wnav);
  else if ( wnavnode_mapped)
    set_focus( wnavnode);
}

void Wtt::set_window_char_cb( void *wtt, int width, int height)
{
  ((Wtt *)wtt)->set_window_char( width, height);
}

int Wtt::restore_settings()
{
  char cmd[80];

  strcpy( cmd, "@");
  dcli_translate_filename( &cmd[1], script_filename());
  wnav->command( cmd);
  wnavnode->command( cmd);
  return 1;
}

char *Wtt::script_filename_cb( void *ctx)
{
 return ((Wtt *)ctx)->script_filename();
}

char *Wtt::script_filename()
{
  static pwr_tFileName fname;

  if ( wb_type == wb_eType_Volume)
    sprintf( fname, "%s.pwr_com", wnav_cInitFile);
  else
    sprintf( fname, "%s%d.pwr_com", wnav_cInitFile, (int)wb_type);
  dcli_translate_filename( fname, fname);
  return fname;
}

int Wtt::save_settings()
{
  ofstream fp;
  int width, height;

  fp.open( script_filename());

  get_window_char( &width, &height);

  fp <<
wnav_cScriptDescKey ": Wtt initialization file" << endl <<
"!   This file is generated at 'Set settings'" << endl <<
"!   Dont make any changes, use the symbolfile for application setup" << endl <<
endl <<
"if ( IsW1())" << endl;
  if ( editmode)
    fp <<
"  edit" << endl;
  fp <<
"  set window /width=" << width << " /height=" << height << endl;
  if ( wnavnode_mapped && wnav_mapped)
    fp <<
"  two" << endl <<
"  set inputfocus w1" << endl;
  else if ( wnav_mapped)
    fp <<
"  display w1" << endl;
  else
    fp <<
"  display w2" << endl;

  fp <<
"endif" << endl;  

  wnav->save_settnings( fp);
  wnavnode->save_settnings( fp);
  fp.close();

  return 1;
}

int Wtt::traverse_focus( void *ctx, void *component)
{
  Wtt *wtt = (Wtt *) ctx;

  if ( component == (void *) wtt->wnav) {
    if ( wtt->wnavnode_mapped)
      wtt->set_focus( wtt->wnavnode);
    else if ( wtt->editmode)
      wtt->set_focus( wtt->palette);
  }
  else if ( component == (void *) wtt->wnavnode) {
    if ( wtt->editmode)
      wtt->set_focus( wtt->palette);
    else if ( wtt->wnav_mapped)
      wtt->set_focus( wtt->wnav);
  }
  else if ( component == (void *) wtt->palette) {
    if ( wtt->wnav_mapped)
      wtt->set_focus( wtt->wnav);
    else if ( wtt->wnavnode_mapped)
      wtt->set_focus( wtt->wnavnode);
  }
  else
    return 0;
  return 1;
}

int Wtt::get_global_select_cb( void *ctx, pwr_sAttrRef **sel_list,
			       int **sel_is_attr, int *sel_cnt)
{
  Wtt 	*wtt = (Wtt *) ctx;
  int	sts = 0;

  if ( wtt->wnav_mapped)
    sts = wtt->wnav->get_select( sel_list, sel_is_attr, sel_cnt);
  if ( EVEN(sts) && wtt->wnavnode_mapped)
    sts = wtt->wnavnode->get_select( sel_list, sel_is_attr, sel_cnt);
  return sts;
}

int Wtt::global_unselect_objid_cb( void *ctx, pwr_tObjid objid)
{
  Wtt 	*wtt = (Wtt *) ctx;

  wtt->wnav->unselect_objid( objid);
  wtt->wnavnode->unselect_objid( objid);
  return 1;
}

int Wtt::set_focus_cb( void *ctx, void *component)
{
  return ((Wtt *)ctx)->set_focus( component);
}

int Wtt::set_focus( void *component)
{
  if ( component == (void *)wnav)
  {
    wnav->set_inputfocus( 1);
    wnavnode->set_inputfocus( 0);
    palette->set_inputfocus( 0);
    focused_wnav = (WNav *)component;
    set_selection_owner();
  }
  else if ( component == (void *)wnavnode)
  {
    wnav->set_inputfocus( 0);
    wnavnode->set_inputfocus( 1);
    palette->set_inputfocus( 0);
    focused_wnav = (WNav *)component;
    set_selection_owner();
  }
  else if ( component == (void *)palette)
  {
    wnav->set_inputfocus( 0);
    wnavnode->set_inputfocus( 0);
    palette->set_inputfocus( 1);
    focused_wnav = 0;
    set_palette_selection_owner();
  }
  return 1;
}

void Wtt::create_popup_menu_cb( void *wtt, pwr_tAttrRef aref,
		int x, int y)
{
  ((Wtt *)wtt)->create_popup_menu( aref, x, y);
}

void Wtt::create_pal_popup_menu_cb( void *wtt, pwr_tCid cid,
				    int x, int y)
{
  ((Wtt *)wtt)->create_pal_popup_menu( cid, x, y);
}

void Wtt::gbl_command_cb( void *ctx, const char *cmd)
{
  Wtt *wtt = (Wtt *) ctx;
  char command[200];

  strcpy( command, cmd);
  strcat( command, " /LOCAL");
  wtt->wnav->command( command);
  wtt->wnavnode->command( command);
}

void Wtt::configure_cb( void *ctx, int edit)
{
  Wtt 	*wtt = (Wtt *) ctx;
  int	sts;

  if ( edit)
    wtt->set_edit();
  else {
    if ( wtt->is_saved())
      sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_KeepVolume);
    else
      wtt->open_confirm( "Do you want to save changes", "Confirm revert",
	&Wtt::save_ok, &Wtt::save_no);
  }
}

void Wtt::findregex_ok( Wtt *wtt, char *search_str)
{
  int sts;

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  sts = wtt->focused_wnav->search( search_str, 1);
  if ( EVEN(sts))
    wtt->message( 'E', wnav_get_message( sts));
}

void Wtt::find_ok( Wtt *wtt, char *search_str)
{
  int sts;

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  sts = wtt->focused_wnav->search( search_str, 0);
  if ( EVEN(sts))
    wtt->message( 'E', wnav_get_message( sts));
}

void Wtt::file_selected_cb( void *ctx, char *filename, wow_eFileSelType file_type)
{
  Wtt *wtt = (Wtt *)ctx;

  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, wb_eType_Volume, filename, file_type);
}

void Wtt::save_cb( void *ctx, int quiet)
{
  Wtt *wtt = (Wtt *) ctx;
  int sts;

  if ( !wtt->ldhses) {
    wtt->message( 'E', "Wtt is not attached to a volume");
    return;
  }
  sts = ldh_SaveSession( wtt->ldhses);
  if ( EVEN(sts)) {
    wtt->message( 'E', wnav_get_message( sts));

    if ( !wtt->focused_wnav)
      wtt->set_focus_default();

    wtt->focused_wnav->wow->DisplayError( "Save Error", wnav_get_message(sts));
    return;
  }

  if ( wtt->wb_type == wb_eType_Directory) {
    sts = lfu_SaveDirectoryVolume( wtt->ldhses, wtt->wnav->wow, quiet);
    if ( EVEN(sts)) {
      wtt->message( 'E', "Syntax error");
      return;
    }
  }
  if ( sts != LDH__CONFIRM)
    wtt->message( 'I', "Session saved");
}

void Wtt::revert_ok( Wtt *wtt)
{
  int sts;

  sts = ldh_RevertSession ( wtt->ldhses);
  if ( EVEN(sts))
    wtt->message( 'E', wnav_get_message( sts));
  else
  {
    if ( wtt->input_open)
      wtt->close_change_value();
    wtt->wnav->ldh_refresh( pwr_cNObjid);
    wtt->wnavnode->ldh_refresh( pwr_cNObjid);
    wtt->message( 'E', "Session reverted");
  }
}

void Wtt::revert_cb( void *ctx, int confirm)
{
  Wtt *wtt = (Wtt *) ctx;

  if ( !wtt->ldhses)
  {
    wtt->message( 'E', "Wtt is not attached to a volume");
    return;
  }
  if ( !confirm)
    revert_ok( wtt);
  else
    wtt->open_confirm( "Do you really want to revert", "Confirm revert",
	&Wtt::revert_ok, NULL);
}

int Wtt::attach_volume_cb( void *ctx, pwr_tVolumeId volid, int pop)
{
  Wtt *wtt = (Wtt *) ctx;
  int	sts;
  pwr_tVolumeId	vid;
  pwr_tClassId	classid;

  if ( wtt->ldhses) {
    wtt->message( 'E', "Other volume is already attached");
    return WNAV__VOLATTACHED;
  }

  if ( !wtt->wbctx) {
    sts = get_wbctx( (void *)wtt, &wtt->wbctx);
    if ( EVEN(sts)) return sts;
  }

  if ( volid == 0) {
    // Attach first rootvolume, or if no rootvolume exist some other volume
    sts = ldh_GetVolumeList( wtt->wbctx, &vid);
    while ( ODD(sts)) {
      volid = vid;
      sts = ldh_GetVolumeClass( wtt->wbctx, vid, &classid);
      if ( EVEN(sts)) return sts;

      if ( classid == pwr_eClass_RootVolume)
        break;
      sts = ldh_GetNextVolume( wtt->wbctx, vid, &vid);
    }
    if ( volid == 0) return sts;
  }

  wtt->volid = volid;

  // Open ldh session
  sts = ldh_AttachVolume( wtt->wbctx, volid, &wtt->volctx);
  if ( EVEN(sts)) return sts;

  sts = ldh_OpenSession( &wtt->ldhses,
    wtt->volctx,
    ldh_eAccess_ReadOnly,
    ldh_eUtility_Navigator);
  if ( EVEN(sts)) return sts;
  ldh_AddOtherSessionCallback( wtt->ldhses,
    (void *)wtt,
    Wtt::ldh_other_session_cb);

  wtt->wnav->volume_attached( wtt->wbctx, wtt->ldhses, pop);
  wtt->wnavnode->volume_attached( wtt->wbctx, wtt->ldhses, pop);
  wtt->palette->session_opened( wtt->ldhses, NULL);

  return 1;
}

void Wtt::detach_save_ok( Wtt *wtt)
{
  int sts;

  // Save and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_DetachVolume);
}

void Wtt::detach_save_no( Wtt *wtt)
{
  int sts;

  // Revert and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Revert, wtt_eNoEdit_DetachVolume);
}

void Wtt::save_ok( Wtt *wtt)
{
  int sts;

  // Save and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_KeepVolume);
}

void Wtt::save_no( Wtt *wtt)
{
  int sts;

  // Revert and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Revert, wtt_eNoEdit_KeepVolume);
}

void Wtt::close_ok( Wtt *wtt)
{
  int sts;

  // Save and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_DetachVolume);
  if ( EVEN(sts)) {
    wtt->message( 'E', wnav_get_message( sts));

    if ( !wtt->focused_wnav)
      wtt->set_focus_default();

    wtt->focused_wnav->wow->DisplayError( "Save Error", wnav_get_message(sts));
    return;
  }
  delete wtt;
}

void Wtt::close_no( Wtt *wtt)
{
  int sts;

  // Revert and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Revert, wtt_eNoEdit_DetachVolume);
  delete wtt;
}

void Wtt::close_now_ok( Wtt *wtt)
{
  delete wtt;
}

int Wtt::detach_volume_cb( void *ctx)
{
  Wtt *wtt = (Wtt *) ctx;
  int	sts;

  if ( wtt->editmode == 0) {
    wtt->detach_volume();
  }
  else {
    if ( wtt->is_saved()) {
      // Set noedit and detach
      sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_DetachVolume);
    }
    else {
      wtt->open_confirm( "Do you want to save changes", "Save",
		&Wtt::detach_save_ok, &Wtt::detach_save_no);
      return WNAV__WAITCONFIRM;
    }
  }
  return 1;
}

int Wtt::get_palette_select_cb( void *ctx, pwr_tCid *classid)
{
  Wtt *wtt = (Wtt *) ctx;
  int	sts;

  sts = wtt->palette->get_select( classid);
  return sts;
}

int Wtt::is_saved()
{
  ldh_sSessInfo	info;
  int		sts;

  if ( !ldhses)
    return 1;

  sts = ldh_GetSessionInfo( ldhses, &info);
  return info.Empty;
}

int Wtt::detach_volume()
{
  int sts;

  if ( !ldhses)
    return WNAV__NOVOLATTACHED;

  sts = ldh_CloseSession( ldhses);
  if ( EVEN(sts))
  {
    message( 'E', wnav_get_message( sts));
    return 0;
  }

  wb_volume *v = (wb_volume *) volctx;
  wb_erep *erep = (wb_erep *)(*(wb_env *)wbctx);
  if ( v->vid() != ldh_cWBVolLocal)
    erep->removeExtern( &sts, (wb_vrep *) *v);  

  sts = ldh_DetachVolume( wbctx, volctx);
  if ( EVEN(sts))
  {
    message( 'E', wnav_get_message( sts));
    return 0;
  }

  volid = 0;
  volctx = 0;
  ldhses = 0;

  wnav->volume_detached();
  wnavnode->volume_detached();
  palette->session_closed();
  return 1;
}

int Wtt::set_noedit( wtt_eNoEditMode save, wtt_eNoEditVolMode detach)
{
  int sts;

  if ( editmode == 0)
    return 1;

  set_noedit_show();

  if ( ! is_saved()) {
    if ( save == wtt_eNoEdit_Save) {
      sts = ldh_SaveSession( ldhses);
      // TODO
      if ( ODD(sts) && wb_type == wb_eType_Directory)
        sts = lfu_SaveDirectoryVolume( ldhses, wnav->wow, 0);
    }
    else
      sts = ldh_RevertSession( ldhses);
    if ( EVEN(sts)) {
      message( 'E', wnav_get_message( sts));
      return sts;
    }
  }
  if ( detach == wtt_eNoEdit_KeepVolume) {
    sts = ldh_CloseSession( ldhses);
    if ( EVEN(sts)) {
      message( 'E', wnav_get_message( sts));
      return sts;
    }

    sts = ldh_OpenSession (
    	&ldhses, 
    	volctx,
    	ldh_eAccess_ReadOnly,
    	ldh_eUtility_Navigator);
    ldh_AddOtherSessionCallback( ldhses,
    	(void *) this,
    	Wtt::ldh_other_session_cb);
    wnav->ldhses = ldhses;
    wnavnode->ldhses = ldhses;
    palette->session_opened( ldhses, NULL);

    editmode = 0;
    wnav->set_editmode(0);
    wnavnode->set_editmode(0);
    menu_setup();
    appl.set_editmode( editmode, ldhses);
    if ( utedctx)
      utedctx->set_editmode( editmode, ldhses);
  }
  else {
    sts = detach_volume();
    if ( EVEN(sts)) return sts;
    editmode = 0;
    menu_setup();
    appl.set_editmode( editmode, ldhses);
    if ( utedctx)
      utedctx->set_editmode( editmode, ldhses);
  }
  return 1;
}

int Wtt::set_edit()
{
  int sts;

  if ( editmode == 1)
    return 1;

  if ( !wbctx || !ldhses)
  {
    message( 'E', "Wtt is not attached to a volume");
    return 1;
  }

  switch ( volid) {
  case ldh_cProjectListVolume:
  case ldh_cGlobalVolumeListVolume:
  case ldh_cUserDatabaseVolume:
    // Privilege Administrator required
    if ( !(CoLogin::privilege() & pwr_mPrv_Administrator)) {
      message( 'E', "User is not authorized to administrate");
      return 1;
    }
    break;
  default:
    // Privilege DevConfig required
    if ( !(CoLogin::privilege() & pwr_mPrv_DevConfig)) {
      message( 'E', "User is not authorized to configure");
      return 1;
    }
  }

  sts = ldh_CloseSession( ldhses);
  sts = ldh_OpenSession( &ldhses, volctx, ldh_eAccess_ReadWrite,
    	ldh_eUtility_Configurator);
  if ( EVEN(sts))
  {
    // Access denied, open readsession again
    ldh_OpenSession( &ldhses, volctx, ldh_eAccess_ReadOnly,
    	ldh_eUtility_Navigator);
    ldh_AddOtherSessionCallback( ldhses, (void *)this,
		Wtt::ldh_other_session_cb);
    message( 'E', wnav_get_message( sts));
    wnav->ldhses = ldhses;
    wnavnode->ldhses = ldhses;
    return sts;
  }
  ldh_AddThisSessionCallback( ldhses, (void *)this,
		Wtt::ldh_this_session_cb);

  set_edit_show();

  wnav->ldhses = ldhses;
  wnavnode->ldhses = ldhses;
  palette->session_opened( ldhses, NULL);

  editmode = 1;
  wnav->set_editmode(1);
  wnavnode->set_editmode(1);
  menu_setup();
  appl.set_editmode( editmode, ldhses);
  if ( utedctx)
    utedctx->set_editmode( editmode, ldhses);
  return 1;
}

void Wtt::message_cb( void *ctx, char severity, const char *msg)
{
  if ( ((WUtility *)ctx)->utype == wb_eUtility_Wtt)
    ((Wtt *)ctx)->message( severity, msg);
  else if ( ((WUtility *)ctx)->utype == wb_eUtility_WNav)
    ((WNav *)ctx)->message( severity, msg);
}

void Wtt::close( void *ctx)
{
  Wtt		*wtt = (Wtt *) ctx;

  if ( !wtt->editmode) {
    if ((wtt->time_to_exit_cb) && (wtt->time_to_exit_cb)( wtt))
      wtt->open_confirm( "Do you really want to exit", "Close",
	&Wtt::close_now_ok, NULL);
    else {
      wtt->detach_volume();
      delete wtt;
    }
  }
  else {
    if ( wtt->is_saved()) {
      if ((wtt->time_to_exit_cb) && (wtt->time_to_exit_cb)( wtt))
        wtt->open_confirm( "Do you really want to exit", "Close",
		&Wtt::close_now_ok, NULL);
      else
      {
        wtt->detach_volume();
        delete wtt;
      }
    }
    else
      wtt->open_confirm( "Do you want to save changes", "Save",
	&Wtt::close_ok, &Wtt::close_no);
  }
}

void Wtt::change_value( void *ctx)
{
  ((Wtt *)ctx)->open_change_value();
}

int Wtt::get_wbctx( void *ctx, ldh_tWBContext *wbctx)
{
  Wtt	*wtt = (Wtt *) ctx;
  int	sts;

  if ( wtt->wbctx) {
    *wbctx = wtt->wbctx;
    sts = 1;
  }
  else {
    wtt->set_clock_cursor();
    sts = ldh_OpenWB( &wtt->wbctx, 0, 0);
    if ( ODD(sts))
      *wbctx = wtt->wbctx;

    wtt->reset_cursor();
  }
  return sts;
}


//
//  Callbackfunctions from menu entries
//

void Wtt::activate_print()
{
  char filename[80] = "pwrp_tmp:wnav.ps";
  char cmd[200];
  int sts;


  if ( !focused_wnav)
    set_focus_default();

  set_clock_cursor();
  dcli_translate_filename( filename, filename);
  focused_wnav->print( filename);

#if defined OS_LINUX
  sprintf( cmd, "wb_gre_print.sh %s", filename); 
  sts = system( cmd);
#endif
  reset_cursor();
}

void Wtt::activate_collapse()
{
  wnav->collapse();
  wnavnode->collapse();
}

void Wtt::activate_save()
{  
  set_clock_cursor();
  save_cb( this, 0);
  reset_cursor();
}

void Wtt::activate_revert()
{
  open_confirm( "Do you really want to revert", "Confirm revert",
	&Wtt::revert_ok, NULL);
}

void Wtt::activate_syntax()
{
  int sts;
  int errorcount, warningcount;
  char msg[80];

  message( ' ',"");

  // Call syntax check
  errorcount = 0;
  warningcount = 0;
  set_clock_cursor();

  sts = ldh_SyntaxCheck( ldhses, &errorcount, &warningcount);
  reset_cursor();
  if (EVEN(sts)) {
    message( 'E', wnav_get_message( sts));
    return;
  }

  if ( errorcount) {
    sprintf( msg, "Syntax errors: %d errors, %d warnings", 
		errorcount, warningcount);
    message( 'E', msg);
  }
  else if ( warningcount) {
    sprintf( msg, "Syntax warnings: %d warnings", warningcount);
    message( 'W', msg);
  }
  else
    message( 'I', "Syntax check successfull");
}

void Wtt::activate_find()
{
  open_input_dialog( "Search string", "Find object",
	"", &Wtt::find_ok);
}

void Wtt::activate_findregex()
{
  open_input_dialog( "Search string", "Find regular expression",
	"", &Wtt::findregex_ok);
}

void Wtt::activate_findnext()
{
  int sts;

  if ( !focused_wnav)
    set_focus_default();

  sts = focused_wnav->search_next();
  if ( EVEN(sts))
    message( 'E', wnav_get_message( sts));
}

void Wtt::activate_copy()
{
  pwr_sAttrRef	*sel_list;
  int           *is_attr;
  int 		sel_cnt;
  int           sts = 0;

  message( ' ', "");

  // Get selections in w1 or w2
  if ( wnav_mapped)
    sts = wnav->get_select( &sel_list, &is_attr, &sel_cnt);
  if ( EVEN(sts) && wnavnode_mapped)
    sts = wnavnode->get_select( &sel_list, &is_attr, &sel_cnt);
  if (EVEN(sts)) {
    message( 'I', "No object is selected");
    return;
  }

  sts = ldh_Copy( ldhses, sel_list, 0, 0);
  if (EVEN(sts)) {
    message( 'E', wnav_get_message( sts));
    return;
  }

  if ( sel_cnt == 1)
    message( 'I', "Object copied");
  else
    message( 'I', "Objects copied");

  if ( sel_cnt > 0) {
    free( (char *)sel_list);
    free( (char *)is_attr);
  }
}

void Wtt::activate_cut()
{
  pwr_sAttrRef	*sel_list;
  int           *is_attr;
  int 		sel_cnt;
  int           sts = 0;

  message( ' ', "");

  // Get selections in w1 or w2
  if ( wnav_mapped)
    sts = wnav->get_select( &sel_list, &is_attr, &sel_cnt);
  if ( EVEN(sts) && wnavnode_mapped)
    sts = wnavnode->get_select( &sel_list, &is_attr, &sel_cnt);
  if ( EVEN(sts)) {
    message( 'I', "No object is selected");
    return;
  }

  sts = ldh_Cut( ldhses, sel_list, false);
  if (EVEN(sts)) {
    message( 'E', wnav_get_message( sts));
    return;
  }

  if ( sel_cnt > 0) {
    free( (char *)sel_list);
    free( (char *)is_attr);
  }
}

void Wtt::activate_paste()
{
  pwr_sAttrRef	*sel_list1, *sel_list2;
  int           *sel_is_attr1, *sel_is_attr2;
  int 		sel_cnt1, sel_cnt2;
  int           sts;

  message( ' ', "");

  // I window is empty, paste on root level
  if ( wnav->is_empty() && wnavnode->is_empty()) {
    sts = ldh_Paste( ldhses, pwr_cNObjid, ldh_eDest_After, 0, 0, 0);
    if (EVEN(sts)) {
      message( 'E', wnav_get_message( sts));
      return;
    }
    return;
  }    

  // Get selections in w1 or w2
  sts = wnav->get_select( &sel_list1, &sel_is_attr1, &sel_cnt1);
  sts = wnavnode->get_select( &sel_list2, &sel_is_attr2, &sel_cnt2);
  if ( !sel_cnt1 && !sel_cnt2) {
    message( 'I', "Select destination object");
    return;
  }
  if ( (sel_cnt1 + sel_cnt2) != 1) {
    message( 'I', "Select one destination object");
    if ( sel_cnt1 > 0) {
      free( (char *)sel_list1);
      free( (char *)sel_is_attr1);
    }
    if ( sel_cnt2 > 0) {
      free( (char *)sel_list2);
      free( (char *)sel_is_attr2);
    }
    return;
  }

  set_clock_cursor();
  if ( sel_cnt1)
    sts = ldh_Paste( ldhses, sel_list1->Objid, ldh_eDest_After, 0, 0, 0);
  else
    sts = ldh_Paste( ldhses, sel_list2->Objid, ldh_eDest_After, 0, 0, 0);
  reset_cursor();
  if ( sel_cnt1 > 0)
    free( (char *)sel_list1);
  if ( sel_cnt2 > 0)
    free( (char *)sel_list2);
  if (EVEN(sts))
    message( 'E', wnav_get_message( sts));
}

void Wtt::activate_pasteinto()
{
  pwr_sAttrRef	*sel_list1, *sel_list2;
  int           *sel_is_attr1, *sel_is_attr2;
  int 		sel_cnt1, sel_cnt2;
  int           sts;

  message( ' ', "");

  // I window is empty, paste on root level
  if ( wnav->is_empty() && wnavnode->is_empty()) {
    sts = ldh_Paste( ldhses, pwr_cNObjid, ldh_eDest_After, 0, 0, 0);
    if (EVEN(sts))
      message( 'E', wnav_get_message( sts));
    return;
  }    

  // Get selections in w1 or w2
  sts = wnav->get_select( &sel_list1, &sel_is_attr1, &sel_cnt1);
  sts = wnavnode->get_select( &sel_list2, &sel_is_attr2, &sel_cnt2);
  if ( !sel_cnt1 && !sel_cnt2) {
    message( 'I', "Select destination object");
    return;
  }
  if ( (sel_cnt1 + sel_cnt2) != 1) {
    message( 'I', "Select one destination object");
    if ( sel_cnt1 > 0) {
      free( (char *)sel_list1);
      free( (char *)sel_is_attr1);
    }
    if ( sel_cnt2 > 0) {
      free( (char *)sel_list2);
      free( (char *)sel_is_attr2);
    }
    return;
  }

  set_clock_cursor();
  if ( sel_cnt1)
    sts = ldh_Paste( ldhses, sel_list1->Objid, ldh_eDest_IntoFirst, 0, 0, 0);
  else
    sts = ldh_Paste( ldhses, sel_list2->Objid, ldh_eDest_IntoFirst, 0, 0, 0);
  reset_cursor();
  if ( sel_cnt1 > 0)
    free( (char *)sel_list1);
  if ( sel_cnt2 > 0)
    free( (char *)sel_list2);
  if (EVEN(sts))
    message( 'E', wnav_get_message( sts));
}

void Wtt::activate_copykeep()
{
  pwr_sAttrRef	*sel_list;
  int           *is_attr;
  int 		sel_cnt;
  int           sts = 0;

  message( ' ', "");

  // Get selections in w1 or w2
  if ( wnav_mapped)
    sts = wnav->get_select( &sel_list, &is_attr, &sel_cnt);
  if ( EVEN(sts) && wnavnode_mapped)
    sts = wnavnode->get_select( &sel_list, &is_attr, &sel_cnt);
  if (EVEN(sts)) {
    message( 'I', "No object is selected");
    return;
  }

  sts = ldh_Copy( ldhses, sel_list, 1, 0);
  if (EVEN(sts)) {
    message( 'E', wnav_get_message( sts));
    return;
  }
  if ( sel_cnt == 1)
    message( 'I', "Object copied");
  else
    message( 'I', "Objects copied");

  if ( sel_cnt > 0) {
    free( (char *)sel_list);
    free( (char *)is_attr);
  }
}

void Wtt::activate_configure()
{
  int sts;

  if ( ! editmode)
    set_edit();
  else {
    if ( is_saved())
      sts = set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_KeepVolume);
    else
      open_confirm( "Do you want to save changes", 
	"Save", &Wtt::save_ok, &Wtt::save_no);
  }
}

void Wtt::activate_utilities()
{
  char	   	title[80];
  int		sts;
  ldh_sVolumeInfo 	info;
  char		volname[80];
  int		size;
  char		systemname[80];
  char		systemgroup[80];

  if ( !ldhses) {
    message( 'E', "No volume is attached");
    return;
  }
  message( ' ', "");

  if ( utedctx == NULL ) {
    sts = ldh_GetVolumeInfo( ldh_SessionToVol( ldhses), &info);
    if (EVEN(sts)) {
      message( 'E', wnav_get_message( sts));
      return;
    }

    sts = ldh_VolumeIdToName( ldh_SessionToWB( ldhses), info.Volume,
		volname, sizeof(volname), &size);
    if (EVEN(sts)) {
      message( 'E', wnav_get_message( sts));
      return;
    }

    utl_get_systemname( systemname, systemgroup);
    sprintf( title, "PwR Utilites Volume %s, %s on %s", volname, 
		CoLogin::username(), systemname);
    set_clock_cursor();
    ute_new( title);
    reset_cursor();
    if ( EVEN(sts)) {
      utedctx = NULL;
      message( 'E', wnav_get_message( sts));
      return;
    }
  }
  else
    utedctx->raise_window();
}

void Wtt::activate_openobject()
{
  int 		sts;
  pwr_sAttrRef	*sel_list;
  int           *sel_is_attr;
  int 		sel_cnt1, sel_cnt2;
  int		i;

  message( ' ', "");

  // Get selections in w1
  sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt1);
  set_clock_cursor();
  if (ODD(sts)) {
    for ( i = 0; i < sel_cnt1; i++)
      watt_new( sel_list[i]);
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
  }

  sts = wnavnode->get_select( &sel_list, &sel_is_attr, &sel_cnt2);
  if (ODD(sts)) {
    for ( i = 0; i < sel_cnt2; i++)
      watt_new( sel_list[i]);
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
  }
  reset_cursor();

  // No selection given, return with error
  if ( sel_cnt1 == 0 && sel_cnt2 == 0){
    message( 'E', "No object is selected");
    return;
  }
}

void Wtt::activate_creaobj( ldh_eDest dest)
{
  int 		sts;
  pwr_sAttrRef	*sel_list;
  int           *sel_is_attr;
  int 		sel_cnt;
  pwr_tOid	oid;
  pwr_tCid	cid;
  int		navnode = 0;
  
  message( ' ', "");

  sts = palette->get_select( &cid);
  if ( EVEN(sts)) {
    message( 'E', "Select a class in the palette");
    return;
  }

  // Get selections in w1
  sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts)) {
    sts = wnavnode->get_select( &sel_list, &sel_is_attr, &sel_cnt);
    navnode = 1;
  }
      
  if ( EVEN(sts)) {
    message( 'E', "No object is selected");
    return;
  }

  if ( sel_cnt > 1) {
    message( 'E', "Select one object");
    return;
  }

  sts = ldh_CreateObject( ldhses, &oid, 0, cid, sel_list[0].Objid, dest);
  if ( EVEN(sts)) {
    message( 'E', wnav_get_message( sts));
    return;
  }
  if ( navnode)
    wnavnode->select_object( oid);
  else
    wnav->select_object( oid);

  free( (char *)sel_list);
  free( (char *)sel_is_attr);
}

void Wtt::activate_moveobj( wnav_eDestCode dest)
{
  int 		sts;
  pwr_sAttrRef	*sel_list;
  int           *sel_is_attr;
  int 		sel_cnt;
  pwr_tOid	next_oid;
  int		navnode = 0;
  wnav_eDestCode d;
  pwr_tOid	parent;
  pwr_tCid	cid;
  
  message( ' ', "");

  // Get selections in w1
  sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts)) {
    sts = wnavnode->get_select( &sel_list, &sel_is_attr, &sel_cnt);
    navnode = 1;
  }
      
  if ( EVEN(sts)) {
    message( 'E', "No object is selected");
    return;
  }

  if ( sel_cnt > 1) {
    message( 'E', "Select one object");
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
    return;
  }

  if ( sel_is_attr[0]) {
    message( 'E', "Unable to move attributes");
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
    return;
  }

  if ( navnode)
    sts = wnavnode->get_next( sel_list[0].Objid, dest, &next_oid, &d);
  else
    sts = wnav->get_next( sel_list[0].Objid, dest, &next_oid, &d);
  if ( EVEN(sts)) {
    message( 'E', wnav_get_message( sts));
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
    return;
  }

  // No plc objects can be moved
  sts = ldh_GetParent( ldhses, sel_list[0].Objid, &parent);
  while (ODD(sts)) {
    sts = ldh_GetObjectClass( ldhses, parent, &cid);
    if ( cid == pwr_cClass_plc || cid == pwr_cClass_PlcTemplate) {
      message( 'E', "Unable to move plc objects");
      free( (char *)sel_list);
      free( (char *)sel_is_attr);
      return;
    }
    sts = ldh_GetParent( ldhses, parent, &parent);
  }

  sts = ldh_GetParent( ldhses, next_oid, &parent);
  while (ODD(sts)) {
    sts = ldh_GetObjectClass( ldhses, parent, &cid);
    if ( cid == pwr_cClass_plc || cid == pwr_cClass_PlcTemplate) {
      message( 'E', "Unable to move plc objects");
      free( (char *)sel_list);
      free( (char *)sel_is_attr);
      return;
    }
    sts = ldh_GetParent( ldhses, parent, &parent);
  }

  if ( d == wnav_eDestCode_FirstChild) {
    sts = ldh_GetObjectClass( ldhses, next_oid, &cid);
    if ( EVEN(sts)) return;

    if ( cid == pwr_cClass_plc || cid == pwr_cClass_PlcTemplate) {
      message( 'E', "Unable to move into plcpgm");
      free( (char *)sel_list);
      free( (char *)sel_is_attr);
      return;
    }    
  }

  switch ( d) {
  case wnav_eDestCode_Before:
    sts = ldh_MoveObject( ldhses, sel_list[0].Objid, next_oid, ldh_eDest_Before);
    break;
  case wnav_eDestCode_After:
    sts = ldh_MoveObject( ldhses, sel_list[0].Objid, next_oid, ldh_eDest_After);
    break;
  case wnav_eDestCode_FirstChild:
    sts = ldh_MoveObject( ldhses, sel_list[0].Objid, next_oid, ldh_eDest_IntoFirst);
    break;
  case wnav_eDestCode_LastChild:
    sts = ldh_MoveObject( ldhses, sel_list[0].Objid, next_oid, ldh_eDest_IntoLast);
    break;
  }
  if ( EVEN(sts)) {
    message( 'E', wnav_get_message( sts));
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
    return;
  }
  
  if ( navnode)
    wnavnode->set_select_visible();
  else
    wnav->set_select_visible();

  free( (char *)sel_list);
  free( (char *)sel_is_attr);
}

void Wtt::activate_deleteobj()
{
  int 		sts;
  pwr_sAttrRef	*sel_list;
  int           *sel_is_attr;
  int 		sel_cnt;
  int		has_child = 0;
  char		msg[200];
  pwr_tOid	child;
  
  message( ' ', "");

  // Get selections in w1
  sts = wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts))
    sts = wnavnode->get_select( &sel_list, &sel_is_attr, &sel_cnt);
      
  if ( EVEN(sts)) {
    message( 'E', "No object is selected");
    return;
  }

  
  for ( int i = 0; i < sel_cnt; i++) {
    if ( sel_is_attr[i]) {
      message( 'E', "Selected item is not an object");      
      return;
    }
  }

  for ( int i = 0; i < sel_cnt; i++) {
    sts = ldh_GetChild( ldhses, sel_list[i].Objid, &child);
    if ( ODD(sts)) {
      has_child = 1;
      break;
    }
  }

  if ( sel_cnt == 1) {
    if ( has_child)
      strcpy( msg, "Selected object has children !\nDo you want to delete the object tree");
    else
      strcpy( msg, "Do you want to delete the selected object");
  }
  else {
    if ( has_child)
      strcpy( msg, "Selected objects have children !\nDo you want to delete the object trees");
    else
      strcpy( msg, "Do you want to delete the selected objects");
  }

  open_confirm( msg, "Delete Object", delete_ok, 0);

  free( (char *)sel_list);
  free( (char *)sel_is_attr);
}

void Wtt::delete_ok( Wtt *wtt)
{
  int 		sts;
  pwr_sAttrRef	*sel_list;
  int           *sel_is_attr;
  int 		sel_cnt;
  pwr_tOid	child;
  
  // Get selections in w1
  sts = wtt->wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts)) 
    sts = wtt->wnavnode->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts))
    return;
      
  if ( EVEN(sts)) {
    wtt->message( 'E', "No object is selected");
    return;
  }

  
  for ( int i = 0; i < sel_cnt; i++) {
    if ( sel_is_attr[i]) {
      wtt->message( 'E', "Selected item is not an object");      
      return;
    }
  }

  for ( int i = 0; i < sel_cnt; i++) {
    sts = ldh_GetChild( wtt->ldhses, sel_list[i].Objid, &child);
    if ( ODD(sts)) {
      sts = ldh_DeleteObjectTree( wtt->ldhses, sel_list[i].Objid, 0);
    }
    else {
      sts = ldh_DeleteObject( wtt->ldhses, sel_list[i].Objid);
    }
  }

  free( (char *)sel_list);
  free( (char *)sel_is_attr);
}

void Wtt::activate_openvolobject()
{
  pwr_tObjid objid;
  pwr_sAttrRef aref;
  int sts;
  ldh_sVolumeInfo info;

  message( ' ', "");

  // Open volume object
  sts = ldh_GetVolumeInfo( ldh_SessionToVol( ldhses), &info);
  if ( EVEN(sts)) {
    message( 'E', wnav_get_message( sts));
    return;
  }

  objid = pwr_cNObjid;
  objid.vid = info.Volume;
  aref = cdh_ObjidToAref( objid);

  // Wake object editor
  set_clock_cursor();
  watt_new( aref);
  reset_cursor();
}

void Wtt::activate_openplc()
{
  if ( !focused_wnav)
    set_focus_default();

  set_clock_cursor();
  focused_wnav->open_plc();
  reset_cursor();
}

void Wtt::activate_buildobject()
{
  pwr_tCmd      cmd;
  pwr_tStatus 	sts;
  pwr_sAttrRef	*sel_list;
  int           *sel_is_attr;
  int		sel_cnt;

  message( ' ', "");

  strcpy( cmd, "build object");
  if ( focused_wnav->gbl.build.force)
    strcat( cmd, " /force");
  if ( focused_wnav->gbl.build.debug)
    strcat( cmd, " /debug");
  if ( focused_wnav->gbl.build.crossref)
    strcat( cmd, " /crossref");
  if ( focused_wnav->gbl.build.manual)
    strcat( cmd, " /manual");

  if ( !focused_wnav)
    set_focus_default();

  // Check that something is selected
  sts = focused_wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( ODD(sts)) {
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
  }
  else {
    message( 'E', "No object is selected");
    return;
  }

  set_clock_cursor();
  sts = focused_wnav->command( cmd);
  reset_cursor();
}

void Wtt::activate_openvolume()
{
  set_clock_cursor();
  if ( open_volume_cb)
    (open_volume_cb) ( this, wb_eType_Volume, NULL, wow_eFileSelType_All);
  reset_cursor();
}
 
void Wtt::activate_openbuffer()
{
  set_clock_cursor();
  if ( open_volume_cb)
    (open_volume_cb) ( this, wb_eType_Buffer, NULL, wow_eFileSelType_All);
  reset_cursor();
}
 
void Wtt::activate_confproject()
{
  set_clock_cursor();
  if ( open_project_volume_cb)
    (open_project_volume_cb) ( this);
  reset_cursor();
}
 
void Wtt::activate_openpl()
{
  set_clock_cursor();
  if ( open_volume_cb)
    (open_volume_cb) ( this, wb_eType_ExternVolume, "ProjectList", wow_eFileSelType_);
  reset_cursor();
}

void Wtt::activate_opengvl()
{
  set_clock_cursor();
  if ( open_volume_cb)
    (open_volume_cb) ( this, wb_eType_ExternVolume, "GlobalVolumeList", wow_eFileSelType_);
  reset_cursor();
}

void Wtt::activate_openudb()
{
  set_clock_cursor();
  if ( open_volume_cb)
    (open_volume_cb) ( this, wb_eType_ExternVolume, "UserDatabase", wow_eFileSelType_);
  reset_cursor();
}

void Wtt::activate_spreadsheet()
{
  wda_new( pwr_cNObjid);
}

void Wtt::activate_openge()
{
  int 		sts;
  pwr_sAttrRef 	attrref;
  int           is_attr;
  pwr_tClassId 	classid;
  char		*action;
  int		size;
  int		graph_found = 0;
  char		graph_name[80];

  if ( !ldhses) {
    message( 'E', "No volume is attached");
    return;
  }
  message( ' ', "");

  while( 1) {
    sts = get_select_first( &attrref, &is_attr);
    if ( EVEN(sts)) break;

    sts = ldh_GetObjectClass( ldhses, attrref.Objid, &classid);
    if ( EVEN(sts)) break;

    if ( classid == pwr_cClass_XttGraph) {
      sts = ldh_GetObjectPar( ldhses, attrref.Objid, "RtBody",
			"Action", &action, &size); 
      if ( EVEN(sts)) break;

      cdh_ToLower( graph_name, action);
      free( (char *)action);
    }
    else if ( classid == pwr_cClass_WebGraph) {
      sts = ldh_GetObjectPar( ldhses, attrref.Objid, "RtBody",
			"Name", &action, &size); 
      if ( EVEN(sts)) break;

      cdh_ToLower( graph_name, action);
      if ( strcmp( graph_name, "") != 0 && strstr( graph_name, ".pwg") == 0)
        strcat( graph_name, ".pwg");
      free( (char *)action);
    }
    else
      break;
    if ( strstr( graph_name, ".pwg"))
      graph_found = 1;
    else {
      message('E', "No graph defined in selected object");
      return;
    }
    break;
  }
  set_clock_cursor();
  if ( graph_found)
    ge_new( graph_name);
  else
    ge_new( NULL);
  reset_cursor();
}

void Wtt::activate_openclasseditor()
{
  int 		sts;
  pwr_sAttrRef 	attrref;
  int           is_attr;
  pwr_tClassId 	classid;
  char		name[80];
  char		fname[200];
  int		size;
  pwr_eClassVolumeDatabaseEnum  *dbenum;
  pwr_tCmd	cmd;

  if ( !ldhses) {
    message( 'E', "No volume is attached");
    return;
  }
  message( ' ', "");

  sts = get_select_first( &attrref, &is_attr);
  if ( EVEN(sts)) {
    message('E', "Select a ClassVolumeConfig object");
    return;
  }

  sts = ldh_GetObjectClass( ldhses, attrref.Objid, &classid);
  if ( EVEN(sts)) return;
  
  if ( classid == pwr_cClass_ClassVolumeConfig ||
       classid == pwr_cClass_ClassVolumeLoad ||
       classid == pwr_cClass_DetachedClassVolumeConfig ||
       classid == pwr_cClass_DetachedClassVolumeLoad) {
    sts = ldh_ObjidToName( ldhses, attrref.Objid, ldh_eName_Object, 
					   name, sizeof(name), &size); 
    if ( EVEN(sts)) return;

    sts = ldh_GetObjectPar( ldhses, attrref.Objid, "RtBody",
			    "Database", (char **) &dbenum, &size);
    if ( EVEN(sts)) return;


    switch ( *dbenum) {
    case pwr_eClassVolumeDatabaseEnum_WbLoad:
      cdh_ToLower( name, name);
      sprintf( fname, "$pwrp_db/%s.wb_load", name);
      dcli_translate_filename( fname, fname);

      sprintf( cmd, "open classeditor /file=\"%s\"/database=wbload", fname);

      if ( !focused_wnav)
	set_focus_default();
      set_clock_cursor();
      focused_wnav->command( cmd); 
      reset_cursor();
      break;
    case pwr_eClassVolumeDatabaseEnum_BerkeleyDb:
    case pwr_eClassVolumeDatabaseEnum_MySql: {
      pwr_tFileName filename;

      cdh_ToLower( name, name);
      dcli_translate_filename( filename, "$pwr_exe/wb_open_db.sh");
      sprintf( cmd,
	       "%s \"%s\" \"%s\" \"%s\" \"%s\" &",
	       filename, CoLogin::username(), CoLogin::ucpassword(), name, name);
      
      set_clock_cursor();
      sts = system( cmd);
      reset_cursor();
      if ( sts == -1 || sts == 127) {
	printf("-- Error when creating process.\n");
	return;
      }
      break;
    }
    default:
      return;
    }

    free( dbenum);
  }
  else {
    message('E', "Select a ClassVolumeConfig object");
    return;
  }
}

void Wtt::activate_buildvolume()
{
  message( ' ', "");

  if ( !focused_wnav)
    set_focus_default();

  set_clock_cursor();
  wb_build *build = build_new();
  build->opt = focused_wnav->gbl.build;
  build->volume();
  
  reset_cursor();
  if ( build->evenSts()) {
    message( 'E', wnav_get_message( build->sts()));
    return;
  }
  if ( build->sts() == PWRB__NOBUILT)
    message('I', "Nothing to build");
  delete build;
}

void Wtt::activate_buildnode()
{
  pwr_tStatus sts;
  lfu_t_volumelist *vp, *volp;

  message(' ',"");

  // Check if there is only one node configured for the current volume
  sts = lfu_volumelist_load( load_cNameBootList, 
		(lfu_t_volumelist **) &boot_volumelist,
		&boot_volumecount);
  if (sts == FOE__NOFILE){
    message( 'E', "Project is not configured");
    return;
  }
  int found = 0;
  vp = (lfu_t_volumelist *)boot_volumelist;
  for ( int i = 0; i < boot_volumecount; i++) {
    if ( vp->volume_id == volid) {
      if ( found) {
	found = 0;
	break;
      }
      found = 1;
      volp = vp;
    }
    vp++;
  }
  if ( found) {
    wb_build *build = build_new();
    build->opt = focused_wnav->gbl.build;

    set_clock_cursor();
    build->node( volp->p1, boot_volumelist, boot_volumecount);
    reset_cursor();
    if ( build->evenSts()) {
      char msg[80];
      sprintf( msg, "Error building node %s", volp->p1);
      delete build;
      message( 'E', msg);
      return;
    }
    else if ( build->sts() == PWRB__NOBUILT) {
      message('I', "Nothing to build");
      delete build;
      return;
    }
    else {
      char msg[80];
      sprintf( msg, "Node %s built", volp->p1);
      delete build;
      message( 'E', msg);
      return;
    }
  }

  set_clock_cursor();
  open_boot_window();
  reset_cursor();
}

void Wtt::activate_distribute()
{

  message( ' ', "");

  if ( wpkg == 0) {
    set_clock_cursor();
    wpkg_new();
    reset_cursor();
  }
  else
    wpkg->pop();
}

void Wtt::activate_showcrossref()
{
  char cmd[80] = "crossref";
  if ( !focused_wnav)
    set_focus_default();
  focused_wnav->command( cmd);
}
 
void Wtt::activate_updateclasses()
{
  char cmd[80] = "update classes";
  if ( !focused_wnav)
    set_focus_default();
  set_clock_cursor();
  focused_wnav->command( cmd);
  reset_cursor();
}

void Wtt::activate_zoom_in()
{
  double zoom_factor;

  wnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  wnav->zoom( 1.18);
  wnavnode->zoom( 1.18);
  palette->zoom( 1.18);
}

void Wtt::activate_zoom_out()
{
  double zoom_factor;

  wnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  wnav->zoom( 1.0 / 1.18);
  wnavnode->zoom( 1.0 / 1.18);
  palette->zoom( 1.0 / 1.18);
}

void Wtt::activate_zoom_reset()
{
  wnav->unzoom();
  wnavnode->unzoom();
  palette->unzoom();
}

void Wtt::activate_twowindows()
{
  set_twowindows( !twowindows, 0, 0);
}

void Wtt::activate_messages()
{
  MsgWindow::map_default();
}

void Wtt::activate_scriptproj()
{
  char cmd[80] = "show script /option=project";
  if ( !focused_wnav)
    set_focus_default();
  focused_wnav->command( cmd);
}

void Wtt::activate_scriptbase()
{
  char cmd[80] = "show script /option=base";

  if ( !focused_wnav)
    set_focus_default();
  focused_wnav->command( cmd);
}

void Wtt::activate_help()
{
  char cmd[80] = "help overview /base";

  if ( !focused_wnav)
    set_focus_default();
  focused_wnav->command( cmd);
}

void Wtt::activate_help_project()
{
  char cmd[80] = "help index /project";

  if ( !focused_wnav)
    set_focus_default();
  focused_wnav->command( cmd);
}

void Wtt::activate_help_proview()
{
  char cmd[80] = "help /version";

  if ( !focused_wnav)
    set_focus_default();
  focused_wnav->command( cmd);
}

int Wtt::get_select_first( pwr_sAttrRef *attrref, int *is_attr)
{
  int		sts;
  pwr_sAttrRef	*sellist;
  int           *sel_is_attr;
  int		sel_cnt = 0;

  sts = wnav->get_select( &sellist, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts))
    sts = wnavnode->get_select( &sellist, &sel_is_attr, &sel_cnt);
  if ( ODD(sts)) {
    *attrref = sellist[0];
    *is_attr = sel_is_attr[0];
  }
  else 
    return sts;

  if ( sel_cnt > 0) {
    free( (char *)sellist);
    free( (char *)sel_is_attr);
  }
  return 1;
}


void wtt_watt_close_cb( void *watt)
{
  Wtt *wtt = (Wtt *) ((WAtt *)watt)->parent_ctx;

  delete (WAtt *)watt;
  wtt->appl.remove( watt);
}

void wtt_wda_close_cb( void *wda)
{
  Wtt *wtt = (Wtt *) ((Wda *)wda)->parent_ctx;

  delete (Wda *)wda;
  wtt->appl.remove( wda);
}

void Wtt::register_utility( void *ctx, wb_eUtility utility)
{

  switch( utility)
  {
    case wb_eUtility_AttributeEditor:
      appl.insert( utility, ctx, pwr_cNObjid, "");
      ((WAtt *)ctx)->close_cb = wtt_watt_close_cb;
      break;
    case wb_eUtility_SpreadsheetEditor:
      appl.insert( utility, ctx, pwr_cNObjid, "");
      ((Wda *)ctx)->close_cb = wtt_wda_close_cb;
      break;
    default:
     ;
  }
}

Wtt::Wtt( 
	void	*wt_parent_ctx,
	const char 	*wt_name,
	const char	*iconname,
	void	*wt_wbctx,
	pwr_tVolumeId wt_volid,
	ldh_tVolume wt_volctx,
	wnav_sStartMenu *root_menu,
	pwr_tStatus *status
	) :
	WUtility(wb_eUtility_Wtt), parent_ctx(wt_parent_ctx),
	wb_type(wb_eType_Volume), root_item(0), input_open(0), command_open(0),
	wbctx(wt_wbctx), volctx(wt_volctx), volid(wt_volid), ldhses(0),
	editmode(0), twowindows(0), confirm_open(0), select_volume(0), 
        select_attr(0), select_type(0),
        wnav_mapped(0), wnavnode_mapped(0), utedctx(0), wpkg(0),
	close_cb(0), open_volume_cb(0), open_project_volume_cb(0), time_to_exit_cb(0),
	mcp(0), disable_w2(0), keep_input_open(0)
{
  strcpy( name, wt_name);
  *status = 1;
}


Wtt::~Wtt()
{
}

int Wtt::find( pwr_tOid oid)
{
  pwr_tStatus sts;

  sts = wnav->display_object( oid);
  if ( ODD(sts)) {
    if ( !wnav_mapped)
      set_twowindows( 1, 0, 0);
    return WNAV__SUCCESS;
  }
  else {
    sts = wnavnode->display_object( oid);
    if ( ODD(sts)) {
      if ( !wnavnode_mapped)
	set_twowindows( 1, 0, 0);
      return WNAV__SUCCESS;
    }
  }
  return sts;
}

int Wtt::find_plc( pwr_tOid oid)
{
  if ( !focused_wnav)
    set_focus_default();
  pwr_tStatus sts = focused_wnav->open_plc( oid);
  return sts;
}

int Wtt::get_popup_menu_items( pwr_sAttrRef aref, pwr_tCid cid)
{
  pwr_tStatus 	sts;
  pwr_sAttrRef	*sel1_list;
  int           *sel1_is_attr;
  int		sel1_cnt;
  pwr_sAttrRef	*sel2_list;
  int           *sel2_is_attr;
  int		sel2_cnt;

  if (mcp == NULL)
    mcp = (ldh_sMenuCall *)calloc(1, sizeof(ldh_sMenuCall));

  mcp->message_cb = message_cb;

  if ( cid != pwr_cNCid) {
    // Popup in palette

    mcp->wnav = wnav;
    mcp->wtt = this;
    mcp->PointedSet = ldh_eMenuSet_Class;

    mcp->Pointed.Objid = cdh_ClassIdToObjid( cid);

    mcp->PointedSession = ldhses;
    mcp->Pointed.Size = 0;
    mcp->Pointed.Offset = 0;
    mcp->Pointed.Flags.m = 0;
    mcp->SelectedSet = ldh_eMenuSet_None;
    mcp->SelectedSession = ldhses;
  }
  else {
    // Popup in wnav
    if ( !focused_wnav)
      set_focus_default();

    mcp->wnav = focused_wnav;
    mcp->wtt = this;

    mcp->Pointed = aref;

    if ( mcp->Pointed.Flags.b.Object)
      mcp->PointedSet = ldh_eMenuSet_Object;
    else if ( mcp->Pointed.Flags.b.ObjectAttr)
      mcp->PointedSet = ldh_eMenuSet_ObjectAttr;
    else if ( mcp->Pointed.Flags.b.Array)
      mcp->PointedSet = ldh_eMenuSet_Array;
    else
      mcp->PointedSet = ldh_eMenuSet_Attribute;

    mcp->PointedSession = ldhses;
    mcp->SelectedSet = ldh_eMenuSet_None;
    mcp->SelectedSession = ldhses;


    // Fetch selections
    if ( wnav_mapped)
      wnav->get_select( &sel1_list, &sel1_is_attr, &sel1_cnt);
    else
      sel1_cnt = 0;
    if ( wnavnode_mapped)
      wnavnode->get_select( &sel2_list, &sel2_is_attr, &sel2_cnt);
    else
      sel2_cnt = 0;

    mcp->Selected = (pwr_sAttrRef *) calloc( sel1_cnt + sel2_cnt + 1, sizeof (pwr_sAttrRef));
    if ( sel1_cnt)
      memcpy( mcp->Selected, sel1_list, sel1_cnt * sizeof(pwr_sAttrRef));
    if ( sel2_cnt)
      memcpy( &mcp->Selected[sel1_cnt], sel2_list, sel2_cnt * sizeof(pwr_sAttrRef));
    if ( sel1_cnt) {
      free( sel1_list);
      free( sel1_is_attr);
    }
    if ( sel2_cnt) {
      free( sel2_list);
      free( sel2_is_attr);
    }

    mcp->Selected[sel1_cnt + sel2_cnt].Objid = pwr_cNObjid;
    mcp->SelectCount = sel1_cnt + sel2_cnt;

    if ( mcp->SelectCount == 0)      
      mcp->SelectedSet = ldh_eMenuSet_None;
    else if ( mcp->SelectCount > 1)      
      mcp->SelectedSet = ldh_eMenuSet_Many;
    else {
      if ( mcp->Selected[0].Flags.b.Object)
	mcp->SelectedSet = ldh_eMenuSet_Object;
      else if ( mcp->Selected[0].Flags.b.ObjectAttr)
	mcp->SelectedSet = ldh_eMenuSet_ObjectAttr;
      else if ( mcp->Selected[0].Flags.b.Array)
	mcp->SelectedSet = ldh_eMenuSet_Array;
      else
	mcp->SelectedSet = ldh_eMenuSet_Attribute;
    }	

    if ( sel1_cnt + sel2_cnt == 0) {
      pwr_tCid cid;

      sts = palette->get_select( &cid);
      if ( ODD(sts)) {
	free( (char *) mcp->Selected);
	mcp->Selected = (pwr_sAttrRef *) calloc( 2, sizeof (pwr_sAttrRef));
	mcp->SelectedSet = ldh_eMenuSet_Class;
	mcp->Selected[0].Objid = cdh_ClassIdToObjid( cid);
	mcp->Selected[1].Objid = pwr_cNObjid;
	mcp->SelectCount = 1;
      }
      else {
	// Get primary selection
	char str[200];

	
	sts = 0; // wnav->get_selection( str, sizeof(str));
	if ( ODD(sts)) {
	  sts = ldh_NameToAttrRef( mcp->PointedSession, str, &aref);
	  if ( ODD(sts)) {
	    free( (char *) mcp->Selected);
	    mcp->Selected = (pwr_sAttrRef *) calloc( 2, sizeof (pwr_sAttrRef));
	    mcp->Selected[0] = aref;
	    mcp->Selected[1].Objid = pwr_cNObjid;
	    mcp->SelectCount = 1;
	    if ( mcp->Selected[0].Flags.b.Object)
	      mcp->SelectedSet = ldh_eMenuSet_Object;
	    else if ( mcp->Selected[0].Flags.b.ObjectAttr)
	      mcp->SelectedSet = ldh_eMenuSet_ObjectAttr;
	    else if ( mcp->Selected[0].Flags.b.Array)
	      mcp->SelectedSet = ldh_eMenuSet_Array;
	    else
	      mcp->SelectedSet = ldh_eMenuSet_Attribute;
	  }
	}
      }
    }
  }
  
  sts = ldh_GetMenu( ldhses, mcp);  
  if (EVEN(sts) || mcp->ItemList[0].Level == 0) {
    return 0;
  }
  return 1;
}

WttApplListElem::WttApplListElem( wb_eUtility al_type, void *al_ctx, 
	pwr_tObjid al_objid, const char *al_name):
	type(al_type), ctx(al_ctx), objid(al_objid), next(NULL)
{
  strcpy( name, al_name);
}

WttApplList::~WttApplList()
{
  WttApplListElem *elem;

  for ( elem = root; elem; elem = elem->next)
  {
    switch( elem->type)
    {
      case wb_eUtility_AttributeEditor:
        delete (WAtt *)elem->ctx;
        break;
      case wb_eUtility_SpreadsheetEditor:
        delete (Wda *)elem->ctx;
        break;
      default:
        ;
    }
  }
}

void WttApplList::insert( wb_eUtility type, void *ctx, 
	pwr_tObjid objid, const char *name)
{
  WttApplListElem *elem = new WttApplListElem( type, ctx, objid, name);
  elem->next = root;
  root = elem;
}

void WttApplList::remove( void *ctx)
{
  WttApplListElem *elem;
  WttApplListElem *prev;

  for ( elem = root; elem; elem = elem->next)
  {
    if ( elem->ctx == ctx)
    {
      if ( elem == root)
        root = elem->next;
      else
        prev->next = elem->next;    
      delete elem;
      return;
    }
    prev = elem;
  }
}

int WttApplList::find( wb_eUtility type, pwr_tObjid objid, void **ctx)
{
  WttApplListElem *elem;

  for ( elem = root; elem; elem = elem->next)
  {
    if ( elem->type == type && cdh_ObjidIsEqual( elem->objid, objid))
    {
      *ctx = elem->ctx;
      return 1;
    }
  }
  return 0;
}

void WttApplList::set_editmode( int editmode, ldh_tSesContext ldhses)
{
  WttApplListElem *elem, *next_elem;

  for ( elem = root; elem;)
  {
    switch( elem->type)
    {
      case wb_eUtility_AttributeEditor:
        // Delete the attribute editor
        delete (WAtt *)elem->ctx;
        // Remove element
        next_elem = elem->next;
        remove( elem->ctx);
        elem = next_elem;
        continue;
      case wb_eUtility_SpreadsheetEditor:
        ((Wda *)elem->ctx)->set_editmode( editmode, ldhses);
	break;
      default:
        ;
    }
    elem = elem->next;
  }
}

int WttApplList::find( wb_eUtility type, const char *name, void **ctx)
{
  WttApplListElem *elem;

  for ( elem = root; elem; elem = elem->next)
  {
    if ( elem->type == type && strcmp( name, elem->name) == 0)
    {
      *ctx = elem->ctx;
      return 1;
    }
  }
  return 0;
}

