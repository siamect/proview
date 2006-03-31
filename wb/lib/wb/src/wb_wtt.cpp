/* 
 * Proview   $Id: wb_wtt.cpp,v 1.30 2006-03-31 14:29:39 claes Exp $
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

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "flow_x.h"
#include "co_wow.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/ToggleB.h>
#include <Xm/List.h>
#include <X11/cursorfont.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>

extern "C" {
#include "co_mrm_util.h"
#include "wb_utl.h"
#include "wb_lfu.h"
#include "wb_login.h"
#if defined OS_VMS
#include "wb_ate.h"
#include "wb_sped.h"
#endif

#include "ge.h"
#include "rt_load.h"
#include "wb_foe_msg.h"
#include "wb_pwrb_msg.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
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


#define MESSAGE_RETURN_STS(sts)\
{\
  wtt->message( 'E', wnav_get_message( sts));\
  return;\
};

#define MESSAGE_RETURN(str)\
{\
  wtt->message( 'E', str);\
  return;\
};

#define WTT_PALETTE_WIDTH 160

//
//

static int wtt_get_wbctx( void *ctx, ldh_tWBContext *wbctx);
static int wtt_set_focus_cb( void *ctx, void *comp);
static void wtt_open_input_dialog( Wtt *wtt, char *text, char *title,
	char *init_text,
	void (*ok_cb)( Wtt *, char *));
static void wtt_open_confirm( Wtt *wtt, char *text, char *title, 
	void (*ok_cb)( Wtt *), void (*no_cb)( Wtt *));
static void wtt_save_ok( Wtt *wtt);
static void wtt_save_no( Wtt *wtt);
static void wtt_open_boot_window( Wtt *wtt);
static void wtt_boot_list_cr(Widget w, Wtt *wtt);
static void wtt_boot_cancel_cb(Widget w, Wtt *wtt);
static void wtt_boot_destroy_cb(Widget w, Wtt *wtt);
static void wtt_boot_ok_cb(Widget w, Wtt *wtt);
static void wtt_disable_set_focus( Wtt *wtt, int time);
static void wtt_reset_avoid_deadlock( Wtt *wtt);
static void wtt_set_avoid_deadlock( Wtt *wtt, int time);
static void wtt_set_twowindows_cb( void *wtt, int two, int display_w1,
        int display_w2);
static void wtt_message_cb( void *ctx, char severity, char *msg);

static int wtt_start_wizard( Wtt *wtt)
{
  pwr_tCmd cmd;

  strcpy( cmd, "@$pwr_exe/wb_wiz_directoryvolume");
  wtt->wnav->command( cmd);
  return 1;
}

static void wtt_set_twowindows_cb( void *wtt, int two, int display_w1,
        int display_w2)
{
  ((Wtt *)wtt)->set_twowindows( two, display_w1, display_w2);
}

static pwr_tStatus wtt_ldh_this_session_cb (
  void *ctx,
  ldh_sEvent *event 
)
{
  Wtt *wtt = (Wtt *) ctx;

  if ( wtt->input_open)
    wtt->close_change_value();
  wtt->wnav->ldh_event( event);
  wtt->wnavnode->ldh_event( event);
  return 1;
}

static pwr_tStatus wtt_ldh_other_session_cb(
  void *ctx,
  ldh_sEvent *event)
{
  Wtt *wtt = (Wtt *) ctx;
 
  switch (event->Event) 
  {
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

extern "C" void wtt_uted_quit_cb( void *ctx) 
{
  ((Wtt *)ctx)->utedctx = NULL;
}

extern "C" void wtt_wpkg_quit_cb( void *ctx) 
{
  delete ((Wtt *)ctx)->wpkg;
  ((Wtt *)ctx)->wpkg = 0;
}

static void wtt_open_vsel_cb( void *ctx, wb_eType type, char *filename, wow_eFileSelType file_type)
{
  Wtt *wtt = (Wtt *)ctx;

  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, type, filename, file_type);

}

static void wtt_set_window_char_cb( void *ctx, short width, short height)
{
  Wtt *wtt = (Wtt *)ctx;
  Arg args[5];
  int i;

  i = 0;
  if ( width)
  {
    XtSetArg(args[i], XmNwidth, width);i++;
  }
  if ( height)
  {
    XtSetArg(args[i], XmNheight, height);i++;
  }
  XtSetValues( wtt->toplevel, args,i);
}

void Wtt::set_focus_default()
{
  if ( wnav_mapped)
    set_focus( wnav);
  else if ( wnavnode_mapped)
    set_focus( wnavnode);
}

void Wtt::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = XCreateFontCursor( flow_Display(toplevel), XC_watch);

  XDefineCursor( flow_Display(toplevel), flow_Window(toplevel), clock_cursor);
  XFlush( flow_Display(toplevel));
}

void Wtt::reset_cursor()
{
  XUndefineCursor( flow_Display(toplevel), flow_Window(toplevel));
}

void Wtt::free_cursor()
{
  if (clock_cursor)
    XFreeCursor( flow_Display(toplevel), clock_cursor);
}

void Wtt::menu_setup()
{
  Arg	sensitive[1];
  Arg	nosensitive[1];

  XtSetArg( sensitive[0],XmNsensitive, 1);
  XtSetArg( nosensitive[0],XmNsensitive, 0);

  switch( wb_type) {
    case wb_eType_Directory:
      if ( editmode) {
        XtSetValues( menu_save_w, sensitive, 1);
        XtSetValues( menu_revert_w, sensitive, 1);
        XtSetValues( menu_cut_w, sensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, sensitive, 1);
        XtSetValues( menu_pasteinto_w, sensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, sensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, sensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      else {
        XtSetValues( menu_save_w, nosensitive, 1);
        XtSetValues( menu_revert_w, nosensitive, 1);
        XtSetValues( menu_cut_w, nosensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, nosensitive, 1);
        XtSetValues( menu_pasteinto_w, nosensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, nosensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, sensitive, 1);
        XtSetValues( menu_distribute_w, sensitive, 1);
        XtSetValues( menu_change_value_w, nosensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, sensitive, 1);
      }
      break;
    case wb_eType_Volume:
      if ( editmode) {
        XtSetValues( menu_save_w, sensitive, 1);
        XtSetValues( menu_revert_w, sensitive, 1);
        XtSetValues( menu_cut_w, sensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, sensitive, 1);
        XtSetValues( menu_pasteinto_w, sensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, sensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, sensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      else {
        XtSetValues( menu_save_w, nosensitive, 1);
        XtSetValues( menu_revert_w, nosensitive, 1);
        XtSetValues( menu_cut_w, nosensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, nosensitive, 1);
        XtSetValues( menu_pasteinto_w, nosensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, nosensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, sensitive, 1);
        XtSetValues( menu_buildobject_w, sensitive, 1);
	if ( ldhses && ldh_VolRepType( ldhses) == ldh_eVolRep_Dbs)
          XtSetValues( menu_buildvolume_w, nosensitive, 1);
	else
          XtSetValues( menu_buildvolume_w, sensitive, 1);
        XtSetValues( menu_buildnode_w, sensitive, 1);
        XtSetValues( menu_distribute_w, sensitive, 1);
        XtSetValues( menu_change_value_w, nosensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      break; 
   case wb_eType_Class:
     if ( editmode) {
        XtSetValues( menu_save_w, sensitive, 1);
        XtSetValues( menu_revert_w, sensitive, 1);
        XtSetValues( menu_cut_w, sensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, sensitive, 1);
        XtSetValues( menu_pasteinto_w, sensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, sensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, sensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      else {
        XtSetValues( menu_save_w, nosensitive, 1);
        XtSetValues( menu_revert_w, nosensitive, 1);
        XtSetValues( menu_cut_w, nosensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, nosensitive, 1);
        XtSetValues( menu_pasteinto_w, nosensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, nosensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
	XtSetValues( menu_buildvolume_w, sensitive, 1);
        XtSetValues( menu_buildnode_w, sensitive, 1);
        XtSetValues( menu_distribute_w, sensitive, 1);
        XtSetValues( menu_change_value_w, nosensitive, 1);
	XtSetValues( menu_edit_w, nosensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      break;
   case wb_eType_ClassEditor:
     XtUnmanageChild( menu_buildobject_w);
     XtUnmanageChild( menu_utilities_w);
     XtUnmanageChild( menu_buildnode_w);
     XtUnmanageChild( menu_classeditor_w);
     if ( editmode) {
        XtSetValues( menu_save_w, sensitive, 1);
        XtSetValues( menu_revert_w, sensitive, 1);
        XtSetValues( menu_cut_w, sensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, sensitive, 1);
        XtSetValues( menu_pasteinto_w, sensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, sensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, sensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      else {
        XtSetValues( menu_save_w, nosensitive, 1);
        XtSetValues( menu_revert_w, nosensitive, 1);
        XtSetValues( menu_cut_w, nosensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, nosensitive, 1);
        XtSetValues( menu_pasteinto_w, nosensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, nosensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, sensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
	XtSetValues( menu_buildvolume_w, sensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, nosensitive, 1);
	XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      break;
    case wb_eType_Buffer:
      if ( editmode) {
        XtSetValues( menu_save_w, nosensitive, 1);
        XtSetValues( menu_revert_w, nosensitive, 1);
        XtSetValues( menu_cut_w, sensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, sensitive, 1);
        XtSetValues( menu_pasteinto_w, sensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, sensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, sensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      else {
        XtSetValues( menu_save_w, nosensitive, 1);
        XtSetValues( menu_revert_w, nosensitive, 1);
        XtSetValues( menu_cut_w, nosensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, nosensitive, 1);
        XtSetValues( menu_pasteinto_w, nosensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, nosensitive, 1);
        XtSetValues( menu_utilities_w, sensitive, 1);
        XtSetValues( menu_openplc_w, sensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, nosensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      break; 
    case wb_eType_ExternVolume:
      if ( editmode) {
        XtSetValues( menu_save_w, sensitive, 1);
        XtSetValues( menu_revert_w, sensitive, 1);
        XtSetValues( menu_cut_w, sensitive, 1);
        XtSetValues( menu_copy_w, sensitive, 1);
        XtSetValues( menu_paste_w, sensitive, 1);
        XtSetValues( menu_pasteinto_w, sensitive, 1);
        XtSetValues( menu_copykeep_w, sensitive, 1);
        XtSetValues( menu_rename_w, sensitive, 1);
        XtSetValues( menu_utilities_w, nosensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, sensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      else {
        XtSetValues( menu_save_w, nosensitive, 1);
        XtSetValues( menu_revert_w, nosensitive, 1);
        XtSetValues( menu_cut_w, nosensitive, 1);
        XtSetValues( menu_copy_w, nosensitive, 1);
        XtSetValues( menu_paste_w, nosensitive, 1);
        XtSetValues( menu_pasteinto_w, nosensitive, 1);
        XtSetValues( menu_copykeep_w, nosensitive, 1);
        XtSetValues( menu_rename_w, nosensitive, 1);
        XtSetValues( menu_utilities_w, nosensitive, 1);
        XtSetValues( menu_openplc_w, nosensitive, 1);
        XtSetValues( menu_buildobject_w, nosensitive, 1);
        XtSetValues( menu_buildvolume_w, nosensitive, 1);
        XtSetValues( menu_buildnode_w, nosensitive, 1);
        XtSetValues( menu_distribute_w, nosensitive, 1);
        XtSetValues( menu_change_value_w, nosensitive, 1);
        XtSetValues( menu_edit_w, sensitive, 1);
        XtSetValues( menu_classeditor_w, nosensitive, 1);
      }
      break;
    default:
      ;
  }
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

static char *wtt_script_filename_cb( void *ctx)
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
  Arg args[5]; 
  int i;
  short width, height;

  fp.open( script_filename());

  i = 0;
  XtSetArg(args[i], XmNwidth, &width);i++;
  XtSetArg(args[i], XmNheight, &height);i++;
  XtGetValues( toplevel, args,i);

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

static int wtt_traverse_focus( void *ctx, void *component)
{
  Wtt *wtt = (Wtt *) ctx;

  if ( component == (void *) wtt->wnav)
  {
    if ( wtt->wnavnode_mapped)
      wtt->set_focus( wtt->wnavnode);
    else if ( wtt->editmode)
      wtt->set_focus( wtt->palette);
  }
  else if ( component == (void *) wtt->wnavnode)
  {
    if ( wtt->editmode)
      wtt->set_focus( wtt->palette);
    else if ( wtt->wnav_mapped)
      wtt->set_focus( wtt->wnav);
  }
  else if ( component == (void *) wtt->palette)
  {
    if ( wtt->wnav_mapped)
      wtt->set_focus( wtt->wnav);
    else if ( wtt->wnavnode_mapped)
      wtt->set_focus( wtt->wnavnode);
  }
  else
    return 0;
  return 1;
}

static int wtt_get_global_select_cb( void *ctx, pwr_sAttrRef **sel_list,
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

static int wtt_global_unselect_objid_cb( void *ctx, pwr_tObjid objid)
{
  Wtt 	*wtt = (Wtt *) ctx;

  wtt->wnav->unselect_objid( objid);
  wtt->wnavnode->unselect_objid( objid);
  return 1;
}

static int wtt_set_focus_cb( void *ctx, void *component)
{
  return ((Wtt *)ctx)->set_focus( component);
}

void wtt_set_selection_owner_proc( Wtt *wtt)
{
  // Delay call to own selection, to make it possible to paste previous selection to value inputwith MB2
  wtt->selection_timerid = 0;
  if ( wtt->focused_wnav)
    wtt->focused_wnav->set_selection_owner();
}

void wtt_set_palette_selection_owner_proc( Wtt *wtt)
{
  // Delay call to own selection, to make it possible to paste previous selection to value inputwith MB2
  wtt->selection_timerid = 0;
  wtt->palette->set_selection_owner();
}

int Wtt::set_focus( void *component)
{
  if ( component == (void *)wnav)
  {
    wnav->set_inputfocus( 1);
    wnavnode->set_inputfocus( 0);
    palette->set_inputfocus( 0);
    focused_wnav = (WNav *)component;
    selection_timerid = XtAppAddTimeOut( XtWidgetToApplicationContext( toplevel), 200,
    	(XtTimerCallbackProc)wtt_set_selection_owner_proc, this);
  }
  else if ( component == (void *)wnavnode)
  {
    wnav->set_inputfocus( 0);
    wnavnode->set_inputfocus( 1);
    palette->set_inputfocus( 0);
    focused_wnav = (WNav *)component;
    selection_timerid = XtAppAddTimeOut( XtWidgetToApplicationContext( toplevel), 200,
    	(XtTimerCallbackProc)wtt_set_selection_owner_proc, this);
  }
  else if ( component == (void *)palette)
  {
    wnav->set_inputfocus( 0);
    wnavnode->set_inputfocus( 0);
    palette->set_inputfocus( 1);
    focused_wnav = 0;
    selection_timerid = XtAppAddTimeOut( XtWidgetToApplicationContext( toplevel), 200,
    	(XtTimerCallbackProc)wtt_set_palette_selection_owner_proc, this);
  }
  return 1;
}

static void wtt_create_popup_menu_cb( void *ctx, pwr_sAttrRef aref,
		int x, int y)
{
  Wtt *wtt = (Wtt *) ctx;
  Widget popup;
  Arg args[5]; 
  int i;
  short x1, y1, x2, y2, x3, y3;
  short menu_x, menu_y;

  if ( !wtt->ldhses)
    return;

  XtSetArg( args[0], XmNx, &x1);
  XtSetArg( args[1], XmNy, &y1);
  XtGetValues( wtt->wnav_form, args, 2);

  XtSetArg( args[0], XmNx, &x2);
  XtSetArg( args[1], XmNy, &y2);
  XtGetValues( XtParent(wtt->wnav_form), args, 2);

  XtSetArg( args[0], XmNx, &x3);
  XtSetArg( args[1], XmNy, &y3);
  XtGetValues( wtt->toplevel, args, 2);

  menu_x = x + x1 + x2 + x3 + 8;
  menu_y = y + y1 + y2 + y3;
  popup = wtt_create_popup_menu( wtt, aref, pwr_cNCid, wtt_message_cb);
  if ( !popup)
    return;

  i = 0;
  XtSetArg(args[i], XmNx, menu_x);i++;
  XtSetArg(args[i], XmNy, menu_y);i++;
  XtSetValues( popup ,args,i);

//    XmMenuPosition(popup, (XButtonPressedEvent *)data->event);
  XtManageChild(popup);
}

static void wtt_create_pal_popup_menu_cb( void *ctx, pwr_tCid cid,
		int x, int y)
{
  Wtt *wtt = (Wtt *) ctx;
  Widget popup;
  Arg args[5]; 
  int i;
  short x1, y1, x2, y2, x3, y3;
  short menu_x, menu_y;

  if ( !wtt->ldhses)
    return;

  XtSetArg( args[0], XmNx, &x1);
  XtSetArg( args[1], XmNy, &y1);
  XtGetValues( wtt->palette_form, args, 2);

  XtSetArg( args[0], XmNx, &x2);
  XtSetArg( args[1], XmNy, &y2);
  XtGetValues( XtParent(wtt->palette_form), args, 2);

  XtSetArg( args[0], XmNx, &x3);
  XtSetArg( args[1], XmNy, &y3);
  XtGetValues( wtt->toplevel, args, 2);

  menu_x = x + x1 + x2 + x3 + 8;
  menu_y = y + y1 + y2 + y3;
  popup = wtt_create_popup_menu( wtt, pwr_cNAttrRef, cid, wtt_message_cb);
  if ( !popup)
    return;

  i = 0;
  XtSetArg(args[i], XmNx, menu_x);i++;
  XtSetArg(args[i], XmNy, menu_y);i++;
  XtSetValues( popup ,args,i);

//    XmMenuPosition(popup, (XButtonPressedEvent *)data->event);
  XtManageChild(popup);
}

static void wtt_gbl_command_cb( void *ctx, char *cmd)
{
  Wtt *wtt = (Wtt *) ctx;
  char command[200];

  strcpy( command, cmd);
  strcat( command, " /LOCAL");
  wtt->wnav->command( command);
  wtt->wnavnode->command( command);
}

static void wtt_configure_cb( void *ctx, int edit)
{
  Wtt 	*wtt = (Wtt *) ctx;
  int	sts;

  if ( edit)
    wtt->set_edit();
  else
  {
    if ( wtt->is_saved())
      sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_KeepVolume);
    else
      wtt_open_confirm( wtt, "Do you want to save changes", "Confirm revert",
	&wtt_save_ok, &wtt_save_no);
  }
}

static void wtt_findregex_ok( Wtt *wtt, char *search_str)
{
  int sts;

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  sts = wtt->focused_wnav->search( search_str, 1);
  if ( EVEN(sts))
    wtt->message( 'E', wnav_get_message( sts));
}

static void wtt_find_ok( Wtt *wtt, char *search_str)
{
  int sts;

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  sts = wtt->focused_wnav->search( search_str, 0);
  if ( EVEN(sts))
    wtt->message( 'E', wnav_get_message( sts));
}

static void wtt_file_selected_cb( void *ctx, char *filename, wow_eFileSelType file_type)
{
  Wtt *wtt = (Wtt *)ctx;

  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, wb_eType_Volume, filename, file_type);
}

static void wtt_save_cb( void *ctx)
{
  Wtt *wtt = (Wtt *) ctx;
  int sts;

  if ( !wtt->ldhses)
  {
    wtt->message( 'E', "Wtt is not attached to a volume");
    return;
  }
  sts = ldh_SaveSession( wtt->ldhses);
  if ( EVEN(sts))
  {
    wtt->message( 'E', wnav_get_message( sts));
    return;
  }

  if ( wtt->wb_type == wb_eType_Directory)
  {
    sts = lfu_SaveDirectoryVolume( wtt->ldhses, wtt->toplevel);
    if ( EVEN(sts))
    {
      wtt->message( 'E', "Syntax error");
      return;
    }
  }
  if ( sts != LDH__CONFIRM)
    wtt->message( 'I', "Session saved");
}

static void wtt_revert_ok( Wtt *wtt)
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

static void wtt_revert_cb( void *ctx, int confirm)
{
  Wtt *wtt = (Wtt *) ctx;

  if ( !wtt->ldhses)
  {
    wtt->message( 'E', "Wtt is not attached to a volume");
    return;
  }
  if ( !confirm)
    wtt_revert_ok( wtt);
  else
    wtt_open_confirm( wtt, "Do you really want to revert", "Confirm revert",
	&wtt_revert_ok, NULL);
}

static int wtt_attach_volume_cb(
		void *ctx,
		pwr_tVolumeId	volid,
		int pop)
{
  Wtt *wtt = (Wtt *) ctx;
  int	sts;
  pwr_tVolumeId	vid;
  pwr_tClassId	classid;

  if ( wtt->ldhses)
  {
    wtt->message( 'E', "Other volume is already attached");
    return WNAV__VOLATTACHED;
  }

  if ( !wtt->wbctx)
  {
    sts = wtt_get_wbctx( (void *)wtt, &wtt->wbctx);
    if ( EVEN(sts)) return sts;
  }

  if ( volid == 0)
  {
    // Attach first rootvolume, or if no rootvolume exist some other volume
    sts = ldh_GetVolumeList( wtt->wbctx, &vid);
    while ( ODD(sts))
    {
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
    wtt_ldh_other_session_cb);

  wtt->wnav->volume_attached( wtt->wbctx, wtt->ldhses, pop);
  wtt->wnavnode->volume_attached( wtt->wbctx, wtt->ldhses, pop);
  wtt->palette->session_opened( wtt->ldhses, NULL);

  return 1;
}

static void wtt_detach_save_ok( Wtt *wtt)
{
  int sts;

  // Save and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_DetachVolume);
}

static void wtt_detach_save_no( Wtt *wtt)
{
  int sts;

  // Revert and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Revert, wtt_eNoEdit_DetachVolume);
}

static void wtt_save_ok( Wtt *wtt)
{
  int sts;

  // Save and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_KeepVolume);
}

static void wtt_save_no( Wtt *wtt)
{
  int sts;

  // Revert and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Revert, wtt_eNoEdit_KeepVolume);
}

static void wtt_close_ok( Wtt *wtt)
{
  int sts;

  // Save and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_DetachVolume);
  delete wtt;
}

static void wtt_close_no( Wtt *wtt)
{
  int sts;

  // Revert and close ldh session
  sts = wtt->set_noedit( wtt_eNoEdit_Revert, wtt_eNoEdit_DetachVolume);
  delete wtt;
}

static void wtt_close_now_ok( Wtt *wtt)
{
  delete wtt;
}

static int wtt_detach_volume_cb( void *ctx)
{
  Wtt *wtt = (Wtt *) ctx;
  int	sts;

  if ( wtt->editmode == 0)
  {
    wtt->detach_volume();
  }
  else
  {
    if ( wtt->is_saved())
    {
      // Set noedit and detach
      sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_DetachVolume);
    }
    else
    {
      wtt_open_confirm( wtt, "Do you want to save changes", "Save",
		&wtt_detach_save_ok, &wtt_detach_save_no);
      return WNAV__WAITCONFIRM;
    }
  }
  return 1;
}

static int wtt_get_palette_select_cb( void *ctx,
		pwr_tClassId *classid)
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
  Arg args[5];
  int i;
  int sts;
  short width;

  if ( editmode == 0)
    return 1;

  i = 0;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM);i++;
  XtSetValues( wnav_form ,args,i);

  XtUnmanageChild( palette_form);

  i = 0;
  XtSetArg(args[i], XmNwidth, &width);i++;
  XtGetValues( toplevel, args,i);

  i = 0;
  XtSetArg(args[i], XmNwidth, width - WTT_PALETTE_WIDTH);i++;
  XtSetValues( toplevel, args,i);

  if ( ! is_saved())
  {
    if ( save == wtt_eNoEdit_Save)
    {
      sts = ldh_SaveSession( ldhses);
      if ( ODD(sts) && wb_type == wb_eType_Directory)
        sts = lfu_SaveDirectoryVolume( ldhses, toplevel);
    }
    else
      sts = ldh_RevertSession( ldhses);
    if ( EVEN(sts))
    {
      message( 'E', wnav_get_message( sts));
      return 0;
    }
  }
  if ( detach == wtt_eNoEdit_KeepVolume)
  {
    sts = ldh_CloseSession( ldhses);
    if ( EVEN(sts))
    {
      message( 'E', wnav_get_message( sts));
      return 0;
    }

    sts = ldh_OpenSession (
    	&ldhses, 
    	volctx,
    	ldh_eAccess_ReadOnly,
    	ldh_eUtility_Navigator);
    ldh_AddOtherSessionCallback( ldhses,
    	(void *) this,
    	wtt_ldh_other_session_cb);
    wnav->ldhses = ldhses;
    wnavnode->ldhses = ldhses;
    palette->session_opened( ldhses, NULL);

    editmode = 0;
    wnav->set_editmode(0);
    wnavnode->set_editmode(0);
    menu_setup();
    appl.set_editmode( editmode, ldhses);
    if ( utedctx)
      uted_set_editmode( utedctx, editmode, ldhses);
  }
  else
  {
    sts = detach_volume();
    if ( EVEN(sts)) return sts;
    editmode = 0;
    menu_setup();
    appl.set_editmode( editmode, ldhses);
    if ( utedctx)
      uted_set_editmode( utedctx, editmode, ldhses);
  }
  return 1;
}

int Wtt::set_edit()
{
  Arg args[5];
  int i;
  int sts;
  short width;

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
    if ( !(login_prv.priv & pwr_mPrv_Administrator)) {
      message( 'E', "User is not authorized to administrate");
      return 1;
    }
    break;
  default:
    // Privilege DevConfig required
    if ( !(login_prv.priv & pwr_mPrv_DevConfig)) {
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
		wtt_ldh_other_session_cb);
    message( 'E', wnav_get_message( sts));
    wnav->ldhses = ldhses;
    wnavnode->ldhses = ldhses;
    return sts;
  }
  ldh_AddThisSessionCallback( ldhses, (void *)this,
		wtt_ldh_this_session_cb);

  XtManageChild( palette_form);

  i = 0;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET);i++;
  XtSetArg(args[i], XmNleftWidget, palette_form);i++;
  XtSetValues( wnav_form ,args,i);

  i = 0;
  XtSetArg(args[i], XmNwidth, &width);i++;
  XtGetValues( toplevel, args,i);

  i = 0;
  XtSetArg(args[i], XmNwidth, width + WTT_PALETTE_WIDTH);i++;
  XtSetValues( toplevel, args,i);

  wnav->ldhses = ldhses;
  wnavnode->ldhses = ldhses;
  palette->session_opened( ldhses, NULL);

  editmode = 1;
  wnav->set_editmode(1);
  wnavnode->set_editmode(1);
  menu_setup();
  appl.set_editmode( editmode, ldhses);
  if ( utedctx)
    uted_set_editmode( utedctx, editmode, ldhses);
  return 1;
}

void Wtt::set_twowindows( int two, int display_wnav, int display_wnavnode)
{

  if ( display_wnav || display_wnavnode)
  {
    if ( display_wnav && ! wnav_mapped)
    {
      XtManageChild( wnav_brow_widget);
      set_focus( wnav);
      wnav_mapped = 1;
    }
    if ( display_wnavnode && ! wnavnode_mapped)
    {
      XtManageChild( wnavnode_brow_widget);
      set_focus( wnavnode);
      wnavnode_mapped = 1;
    }
    if ( !display_wnav && wnav_mapped)
    {
      XtUnmanageChild( wnav_brow_widget);
      set_focus( wnavnode);
      wnav_mapped = 0;
    }
    if ( !display_wnavnode && wnavnode_mapped)
    {
      XtUnmanageChild( wnavnode_brow_widget);
      set_focus( wnav);
      wnavnode_mapped = 0;
    }
    twowindows = display_wnav && display_wnavnode;
  }
  else
  {
    if ( two == twowindows)
      return;

    if ( !two)
    {
      // Keep the focused window
      if ( focused_wnav == wnavnode)
      {
        XtUnmanageChild( wnav_brow_widget);
        set_focus( wnavnode);
        wnav_mapped = 0;
        wnavnode_mapped = 1;
      }
      else
      {
        XtUnmanageChild( wnavnode_brow_widget);
        set_focus( wnav);
        wnav_mapped = 1;
        wnavnode_mapped = 0;
      }
    }
    else
    {
      if ( !wnav_mapped)
      {
        XtManageChild( wnav_brow_widget);
        set_focus( wnav);
        wnav_mapped = 1;
      }
      if ( !wnavnode_mapped)
      {
        XtManageChild( wnavnode_brow_widget);
        set_focus( wnavnode);
        wnavnode_mapped = 1;
      }
    }
    twowindows = two;
  }
}

static void wtt_message_cb( void *ctx, char severity, char *msg)
{
 ((Wtt *)ctx)->message( severity, msg);
}

void Wtt::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}


static void wtt_close( void *ctx)
{
  Wtt		*wtt = (Wtt *) ctx;

  if ( !wtt->editmode)
  {
    if ((wtt->time_to_exit_cb) && (wtt->time_to_exit_cb)( wtt))
      wtt_open_confirm( wtt, "Do you really want to exit", "Close",
	&wtt_close_now_ok, NULL);
    else
    {
      wtt->detach_volume();
      delete wtt;
    }
  }
  else
  {
    if ( wtt->is_saved())
    {
      if ((wtt->time_to_exit_cb) && (wtt->time_to_exit_cb)( wtt))
        wtt_open_confirm( wtt, "Do you really want to exit", "Close",
		&wtt_close_now_ok, NULL);
      else
      {
        wtt->detach_volume();
        delete wtt;
      }
    }
    else
      wtt_open_confirm( wtt, "Do you want to save changes", "Save",
	&wtt_close_ok, &wtt_close_no);
  }
}

static void wtt_change_value( void *ctx)
{
  ((Wtt *)ctx)->open_change_value();
}

static int wtt_get_wbctx( void *ctx, ldh_tWBContext *wbctx)
{
  Wtt	*wtt = (Wtt *) ctx;
  int	sts;

  if ( wtt->wbctx)
  {
    *wbctx = wtt->wbctx;
    sts = 1;
  }
  else
  {
    wtt->set_clock_cursor();
    sts = ldh_OpenWB( &wtt->wbctx, 0, 0);
    if ( ODD(sts))
      *wbctx = wtt->wbctx;

    wtt->reset_cursor();
  }
  return sts;
}

void Wtt::set_prompt( char *prompt)
{
  Arg 		args[3];
  XmString	cstr;

  cstr=XmStringCreateLtoR( prompt, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNwidth, 50);
  XtSetArg(args[2],XmNheight, 30);
  XtSetValues( cmd_prompt, args, 3);
  XmStringFree( cstr);
}

void Wtt::close_change_value()
{
  if ( input_open) {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
  }
}

void Wtt::open_change_value()
{
  int		sts;
  brow_tObject	*sellist;
  int		sel_cnt1, sel_cnt2;
  char 		*value = 0;
  int		multiline;
  int		input_size;

  if ( input_open) {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = wnav->get_selected_nodes( &sellist, &sel_cnt1);
  if ( ODD(sts))
  {
    if ( sel_cnt1 != 1)
    {
      message('E', "Select one attribute");
      return;
    }

    sts = wnav->check_attr_value( sellist[0], &multiline, &value, &input_size);
    if ( EVEN(sts))
    {
      message( 'E', wnav_get_message( sts));
      return;
    }
    if ( multiline)
    {
      message( 'E', "Edit multiline attributes in the attribute editor");
      return;
    }

    input_node = sellist[0];
    input_wnav = wnav;
    wnav->node_to_objid( input_node, &input_objid);
    free( sellist);
  }
  else
  {
    sts = wnavnode->get_selected_nodes( &sellist, &sel_cnt2);
    if ( ODD(sts))
    {
      if ( sel_cnt2 != 1)
      {
        message('E', "Select one attribute");
        return;
      }

      sts = wnavnode->check_attr_value( sellist[0], &multiline, &value, &input_size);
      if ( EVEN(sts))
      {
        message( 'E', wnav_get_message( sts));
        return;
      }
      if ( multiline)
      {
        message( 'E', "Edit multiline attributes in the attribute editor");
        return;
      }

      input_node = sellist[0];
      input_wnav = wnavnode;
      wnavnode->node_to_objid( input_node, &input_objid);
      free( sellist);
    }
  }
  if ( sel_cnt1 == 0 && sel_cnt2 == 0)
  {
    message('E', "Select an attribute");
    return;
  }

  if ( command_open)
    command_open = 0;
  else
    XtManageChild( cmd_input);

  message( ' ', "");
  XtSetKeyboardFocus( toplevel, cmd_input);

  if ( value)
  {
    XmTextSetString( cmd_input, value);
    XmTextSetInsertionPosition( cmd_input, strlen(value));
    XmTextSetSelection( cmd_input, 0, strlen(value), CurrentTime);
  }
  else
    XmTextSetString( cmd_input, "");

  set_prompt( "value >");
  input_open = 1;
  input_mode = wtt_eInputMode_Attribute;
}

void Wtt::open_change_name()
{
  int		sts;
  brow_tObject	*sellist;
  int		sel_cnt1, sel_cnt2;
  int           size;
  char          name[80];

  if ( input_open) 
  {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = wnav->get_selected_nodes( &sellist, &sel_cnt1);
  if ( ODD(sts))
  {
    if ( sel_cnt1 != 1)
    {
      message('E', "Select one object");
      return;
    }

    sts = wnav->check_object_name( sellist[0]);
    if ( EVEN(sts))
    {
      message( 'E', wnav_get_message( sts));
      return;
    }

    input_node = sellist[0];
    input_wnav = wnav;
    wnav->node_to_objid( input_node, &input_objid);
    free( sellist);
  }
  else
  {
    sts = wnavnode->get_selected_nodes( &sellist, &sel_cnt2);
    if ( ODD(sts))
    {
      if ( sel_cnt2 != 1)
      {
        message('E', "Select one object");
        return;
      }

      sts = wnavnode->check_object_name( sellist[0]);
      if ( EVEN(sts))
      {
        message( 'E', wnav_get_message( sts));
        return;
      }

      input_node = sellist[0];
      input_wnav = wnavnode;
      wnavnode->node_to_objid( input_node, &input_objid);
      free( sellist);
    }
  }
  if ( sel_cnt1 == 0 && sel_cnt2 == 0)
  {
    message('E', "Select an object");
    return;
  }

  if ( command_open)
    command_open = 0;
  else
    XtManageChild( cmd_input);

  message( ' ', "");
  XtSetKeyboardFocus( toplevel, cmd_input);

  sts = ldh_ObjidToName( ldhses, input_objid, ldh_eName_Object, 
					   name, sizeof(name), &size); 

  XmTextSetString( cmd_input, name);
  XmTextSetInsertionPosition( cmd_input, strlen(name));
  XmTextSetSelection( cmd_input, 0, strlen(name), CurrentTime);
  set_prompt( "name >");
  input_open = 1;
  input_mode = wtt_eInputMode_ObjectName;
}

//
//  Callbackfunctions from menu entries
//
static void wtt_activate_change_value( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->open_change_value();
}

static void wtt_activate_command( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  if ( wtt->command_open) 
  {
    XtUnmanageChild( wtt->cmd_input);
    wtt->set_prompt( "");
    wtt->command_open = 0;
    return;
  }

  if ( wtt->input_open)
    wtt->input_open = 0;
  else
    XtManageChild( wtt->cmd_input);
  wtt->message( ' ', "");
  XtSetKeyboardFocus( wtt->toplevel, wtt->cmd_input);

  XmTextSetString( wtt->cmd_input, "");
  wtt->set_prompt( "wtt >");
  wtt->command_open = 1;
}

static void wtt_activate_exit( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt_close( wtt);
}

static void wtt_activate_print( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char filename[80] = "pwrp_tmp:wnav.ps";
  char cmd[200];
  int sts;


  if ( !wtt->focused_wnav)
    wtt->set_focus_default();

  wtt->set_clock_cursor();
  dcli_translate_filename( filename, filename);
  wtt->focused_wnav->print( filename);

#if defined OS_LINUX
  sprintf( cmd, "wb_gre_print.sh %s", filename); 
  sts = system( cmd);
#endif
  wtt->reset_cursor();
}

static void wtt_activate_collapse( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->wnav->collapse();
  wtt->wnavnode->collapse();
}

static void wtt_activate_save( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  wtt_save_cb( wtt);
  wtt->reset_cursor();
}

static void wtt_activate_revert( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt_open_confirm( wtt, "Do you really want to revert", "Confirm revert",
	&wtt_revert_ok, NULL);
}

static void wtt_activate_syntax( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  int sts;
  int errorcount, warningcount;
  char message[80];

  wtt->message( ' ',"");

  // Call syntax check
  errorcount = 0;
  warningcount = 0;
  wtt->set_clock_cursor();

  sts = ldh_SyntaxCheck( wtt->ldhses, &errorcount, &warningcount);
  wtt->reset_cursor();
  if (EVEN(sts))
    MESSAGE_RETURN_STS( sts);

  if ( errorcount)
  {
    sprintf( message, "Syntax errors: %d errors, %d warnings", 
		errorcount, warningcount);
    wtt->message( 'E', message);
  }
  else if ( warningcount)
  {
    sprintf( message, "Syntax warnings: %d warnings", warningcount);
    wtt->message( 'W', message);
  }
  else
    wtt->message( 'I', "Syntax check successfull");
}

static void wtt_activate_find( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt_open_input_dialog( wtt, "Search string", "Find object",
	"", &wtt_find_ok);
}

static void wtt_activate_findregex( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt_open_input_dialog( wtt, "Search string", "Find regular expression",
	"", &wtt_findregex_ok);
}

static void wtt_activate_findnext( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  int sts;

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();

  sts = wtt->focused_wnav->search_next();
  if ( EVEN(sts))
    wtt->message( 'E', wnav_get_message( sts));
}

static void wtt_activate_copy( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  pwr_sAttrRef	*sel_list;
  int           *is_attr;
  int 		sel_cnt;
  int           sts = 0;

  wtt->message( ' ', "");

  // Get selections in w1 or w2
  if ( wtt->wnav_mapped)
    sts = wtt->wnav->get_select( &sel_list, &is_attr, &sel_cnt);
  if ( EVEN(sts) && wtt->wnavnode_mapped)
    sts = wtt->wnavnode->get_select( &sel_list, &is_attr, &sel_cnt);
  if (EVEN(sts)) {
    wtt->message( 'I', "No object is selected");
    return;
  }

  sts = ldh_Copy( wtt->ldhses, sel_list, 0, 0);
  if (EVEN(sts))
    MESSAGE_RETURN_STS( sts);
  if ( sel_cnt == 1)
    wtt->message( 'I', "Object copied");
  else
    wtt->message( 'I', "Objects copied");

  if ( sel_cnt > 0) {
    free( (char *)sel_list);
    free( (char *)is_attr);
  }
}

static void wtt_activate_cut( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  pwr_sAttrRef	*sel_list;
  int           *is_attr;
  int 		sel_cnt;
  int           sts = 0;

  wtt->message( ' ', "");

  // Get selections in w1 or w2
  if ( wtt->wnav_mapped)
    sts = wtt->wnav->get_select( &sel_list, &is_attr, &sel_cnt);
  if ( EVEN(sts) && wtt->wnavnode_mapped)
    sts = wtt->wnavnode->get_select( &sel_list, &is_attr, &sel_cnt);
  if ( EVEN(sts)) {
    wtt->message( 'I', "No object is selected");
    return;
  }

  sts = ldh_Cut( wtt->ldhses, sel_list, false);
  if (EVEN(sts))
    MESSAGE_RETURN_STS( sts);

  if ( sel_cnt > 0) {
    free( (char *)sel_list);
    free( (char *)is_attr);
  }
}

static void wtt_activate_paste( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  pwr_sAttrRef	*sel_list1, *sel_list2;
  int           *sel_is_attr1, *sel_is_attr2;
  int 		sel_cnt1, sel_cnt2;
  int           sts;

  wtt->message( ' ', "");

  // I window is empty, paste on root level
  if ( wtt->wnav->is_empty() && wtt->wnavnode->is_empty()) {
    sts = ldh_Paste( wtt->ldhses, pwr_cNObjid, ldh_eDest_After, 0, 0);
    if (EVEN(sts))
      MESSAGE_RETURN_STS( sts);
    return;
  }    

  // Get selections in w1 or w2
  sts = wtt->wnav->get_select( &sel_list1, &sel_is_attr1, &sel_cnt1);
  sts = wtt->wnavnode->get_select( &sel_list2, &sel_is_attr2, &sel_cnt2);
  if ( !sel_cnt1 && !sel_cnt2) {
    wtt->message( 'I', "Select destination object");
    return;
  }
  if ( (sel_cnt1 + sel_cnt2) != 1) {
    wtt->message( 'I', "Select one destination object");
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

  wtt->set_clock_cursor();
  if ( sel_cnt1)
    sts = ldh_Paste( wtt->ldhses, sel_list1->Objid, ldh_eDest_After, 0, 0);
  else
    sts = ldh_Paste( wtt->ldhses, sel_list2->Objid, ldh_eDest_After, 0, 0);
  wtt->reset_cursor();
  if ( sel_cnt1 > 0)
    free( (char *)sel_list1);
  if ( sel_cnt2 > 0)
    free( (char *)sel_list2);
  if (EVEN(sts))
    MESSAGE_RETURN_STS( sts);
}
static void wtt_activate_pasteinto( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  pwr_sAttrRef	*sel_list1, *sel_list2;
  int           *sel_is_attr1, *sel_is_attr2;
  int 		sel_cnt1, sel_cnt2;
  int           sts;

  wtt->message( ' ', "");

  // I window is empty, paste on root level
  if ( wtt->wnav->is_empty() && wtt->wnavnode->is_empty()) {
    sts = ldh_Paste( wtt->ldhses, pwr_cNObjid, ldh_eDest_After, 0, 0);
    if (EVEN(sts))
      MESSAGE_RETURN_STS( sts);
    return;
  }    

  // Get selections in w1 or w2
  sts = wtt->wnav->get_select( &sel_list1, &sel_is_attr1, &sel_cnt1);
  sts = wtt->wnavnode->get_select( &sel_list2, &sel_is_attr2, &sel_cnt2);
  if ( !sel_cnt1 && !sel_cnt2) {
    wtt->message( 'I', "Select destination object");
    return;
  }
  if ( (sel_cnt1 + sel_cnt2) != 1) {
    wtt->message( 'I', "Select one destination object");
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

  wtt->set_clock_cursor();
  if ( sel_cnt1)
    sts = ldh_Paste( wtt->ldhses, sel_list1->Objid, ldh_eDest_IntoFirst, 0, 0);
  else
    sts = ldh_Paste( wtt->ldhses, sel_list2->Objid, ldh_eDest_IntoFirst, 0, 0);
  wtt->reset_cursor();
  if ( sel_cnt1 > 0)
    free( (char *)sel_list1);
  if ( sel_cnt2 > 0)
    free( (char *)sel_list2);
  if (EVEN(sts))
    MESSAGE_RETURN_STS( sts);
}

static void wtt_activate_copykeep( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  pwr_sAttrRef	*sel_list;
  int           *is_attr;
  int 		sel_cnt;
  int           sts = 0;

  wtt->message( ' ', "");

  // Get selections in w1 or w2
  if ( wtt->wnav_mapped)
    sts = wtt->wnav->get_select( &sel_list, &is_attr, &sel_cnt);
  if ( EVEN(sts) && wtt->wnavnode_mapped)
    sts = wtt->wnavnode->get_select( &sel_list, &is_attr, &sel_cnt);
  if (EVEN(sts)) {
    wtt->message( 'I', "No object is selected");
    return;
  }

  sts = ldh_Copy( wtt->ldhses, sel_list, 1, 0);
  if (EVEN(sts))
    MESSAGE_RETURN_STS( sts);
  if ( sel_cnt == 1)
    wtt->message( 'I', "Object copied");
  else
    wtt->message( 'I', "Objects copied");

  if ( sel_cnt > 0) {
    free( (char *)sel_list);
    free( (char *)is_attr);
  }
}

static void wtt_activate_rename( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->open_change_name();
}

static void wtt_activate_configure( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  int sts;

  if ( ! wtt->editmode)
    wtt->set_edit();
  else
  {
    if ( wtt->is_saved())
      sts = wtt->set_noedit( wtt_eNoEdit_Save, wtt_eNoEdit_KeepVolume);
    else
      wtt_open_confirm( wtt, "Do you want to save changes", 
	"Save", &wtt_save_ok, &wtt_save_no);
  }
}

static void wtt_activate_utilities( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char	   	title[80];
  int		sts;
  ldh_sVolumeInfo 	info;
  char		volname[80];
  int		size;
  char		systemname[80];
  char		systemgroup[80];

  if ( !wtt->ldhses) 
    MESSAGE_RETURN( "No volume is attached");
  wtt->message( ' ', "");

  wtt_disable_set_focus( wtt, 2000);

  if ( wtt->utedctx == NULL )
  {
    sts = ldh_GetVolumeInfo( ldh_SessionToVol( wtt->ldhses), &info);
    if (EVEN(sts)) MESSAGE_RETURN_STS( sts);

    sts = ldh_VolumeIdToName( ldh_SessionToWB( wtt->ldhses), info.Volume,
		volname, sizeof(volname), &size);
    if (EVEN(sts)) MESSAGE_RETURN_STS( sts);

    utl_get_systemname( systemname, systemgroup);
    sprintf( title, "PwR Utilites Volume %s, %s on %s", volname, 
		login_prv.username, systemname);
    wtt->set_clock_cursor();
    sts = uted_new( wtt, wtt->toplevel, title, "Utilites",
		ldh_SessionToWB(wtt->ldhses), wtt->ldhses, wtt->editmode, 
		&wtt->utedctx, wtt_uted_quit_cb);
    wtt->reset_cursor();
    if ( EVEN(sts))
    {
      wtt->utedctx = NULL;
      MESSAGE_RETURN_STS( sts);
    }
  }
  else
    uted_raise_window( wtt->utedctx);
}

static void wtt_activate_openobject( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
#if 0
  pwr_tObjid 	objid;
  int 		sts;
  pwr_sAttrRef	*sel_list;
  int 		sel_cnt1, sel_cnt2;
  int		i;


  wtt->message( ' ', "");

  // Get selections in w1
  sts = wtt->wnav->get_select( &sel_list, &sel_cnt1);
  wtt->set_clock_cursor();
  if ODD(sts)
  {
    for ( i = 0; i < sel_cnt1; i++)
      ate_edit( wtt, wtt->toplevel, "Attribute Editor", 0,
		sel_list[i].Objid, wtt->ldhses, NULL, NULL, NULL, NULL, 
		wtt->editmode, TRUE);
    free( (char *)sel_list);
  }

  sts = wtt->wnavnode->get_select( &sel_list, &sel_cnt2);
  if ODD(sts)
  {
    for ( i = 0; i < sel_cnt2; i++)
      ate_edit( wtt, wtt->toplevel, "Attribute Editor", 0,
		sel_list[i].Objid, wtt->ldhses, NULL, NULL, NULL, NULL, 
		wtt->editmode, TRUE);
    free( (char *)sel_list);
  }
  wtt->reset_cursor();

  // No selection given, return with error
  if ( sel_cnt1 == 0 && sel_cnt2 == 0)
    MESSAGE_RETURN( "No object selected");
#else
  int 		sts;
  pwr_sAttrRef	*sel_list;
  int           *sel_is_attr;
  int 		sel_cnt1, sel_cnt2;
  int		i;

  wtt->message( ' ', "");

  // Get selections in w1
  sts = wtt->wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt1);
  wtt->set_clock_cursor();
  if ODD(sts)
  {
    for ( i = 0; i < sel_cnt1; i++)
    {
      new WAtt( wtt->toplevel, wtt, wtt->ldhses,
		sel_list[i], wtt->editmode, wtt->wnav->gbl.advanced_user, 1);
    }
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
  }

  sts = wtt->wnavnode->get_select( &sel_list, &sel_is_attr, &sel_cnt2);
  if ODD(sts)
  {
    for ( i = 0; i < sel_cnt2; i++)
      new WAtt( wtt->toplevel, wtt, wtt->ldhses,
		sel_list[i], wtt->editmode,
		wtt->wnavnode->gbl.advanced_user, 1);
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
  }
  wtt->reset_cursor();

  // No selection given, return with error
  if ( sel_cnt1 == 0 && sel_cnt2 == 0)
    MESSAGE_RETURN( "No object selected");
#endif
}

static void wtt_activate_openvolobject( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  pwr_tObjid objid;
  pwr_sAttrRef aref;
  int sts;
  ldh_sVolumeInfo info;

  wtt->message( ' ', "");

  // Open volume object
  sts = ldh_GetVolumeInfo( ldh_SessionToVol( wtt->ldhses), &info);
  if ( EVEN(sts)) MESSAGE_RETURN_STS( sts);

  objid = pwr_cNObjid;
  objid.vid = info.Volume;
  aref = cdh_ObjidToAref( objid);

  // Wake object editor
  wtt->set_clock_cursor();
  new WAtt( wtt->toplevel, wtt, wtt->ldhses,
		aref, wtt->editmode, wtt->wnav->gbl.advanced_user, 0);
  wtt->reset_cursor();
}

static void wtt_activate_openplc( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  if ( wtt->avoid_deadlock)
    return;

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();

  wtt_disable_set_focus( wtt, 2000);
  wtt->set_clock_cursor();
  wtt->focused_wnav->open_plc();
  wtt_set_avoid_deadlock( wtt, 2000);
  wtt->reset_cursor();
}

static void wtt_activate_buildobject( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  pwr_tCmd      cmd;
  pwr_tStatus 	sts;
  pwr_sAttrRef	*sel_list;
  int           *sel_is_attr;
  int		sel_cnt;

  wtt->message( ' ', "");

  strcpy( cmd, "build object");
  if ( wtt->focused_wnav->gbl.build.force)
    strcat( cmd, " /force");
  if ( wtt->focused_wnav->gbl.build.debug)
    strcat( cmd, " /debug");
  if ( wtt->focused_wnav->gbl.build.crossref)
    strcat( cmd, " /crossref");
  if ( wtt->focused_wnav->gbl.build.manual)
    strcat( cmd, " /manual");

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();

  // Check that something is selected
  sts = wtt->focused_wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( ODD(sts)) {
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
  }
  else {
    wtt->message( 'E', "No object is selected");
    return;
  }

  wtt->set_clock_cursor();
  sts = wtt->focused_wnav->command( cmd);
  wtt->reset_cursor();

#if 0
  // Get selections in w1 or w2
  if ( wtt->wnav_mapped)
    sts = wtt->wnav->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts) && wtt->wnavnode_mapped)
    sts = wtt->wnavnode->get_select( &sel_list, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts)) 
    // Compile all modified plcpgm's
    strcpy( cmd, "compile /modified");
  else {
    // Compile all selected plcpgm's
    strcpy( cmd, "compile");
    free( (char *)sel_list);
    free( (char *)sel_is_attr);
  }

  wtt->set_clock_cursor();
  if ( !wtt->editmode) {
    // Open a temporary write session
    sts = ldh_OpenSession( &ldhses, wtt->volctx, ldh_eAccess_ReadWrite,
    	ldh_eUtility_Configurator);
    if ( EVEN(sts)) {
      wtt->reset_cursor();
      wow_DisplayError( wtt->toplevel, "Compile error", "Other session is open");
      return;
    }

    ldh_AddThisSessionCallback( ldhses, (void *)wtt,
		wtt_ldh_this_session_cb);
    wtt->wnav->editmode = 1;
    wtt->wnav->ldhses = ldhses;

    sts = wtt->wnav->command( cmd);

    sts = ldh_CloseSession( ldhses);
    wtt->wnav->editmode = 0;
    wtt->wnav->ldhses = wtt->ldhses;
  }
  else
    sts = wtt->wnav->command( cmd);
  wtt->reset_cursor();
#endif
}

static void wtt_activate_openvolume( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, wb_eType_Volume, NULL, wow_eFileSelType_All);
  wtt->reset_cursor();
}
 
static void wtt_activate_openbuffer( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, wb_eType_Buffer, NULL, wow_eFileSelType_All);
  wtt->reset_cursor();
}
 
static void wtt_activate_confproject( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  if ( wtt->open_project_volume_cb)
    (wtt->open_project_volume_cb) ( wtt);
  wtt->reset_cursor();
}
 
static void wtt_activate_openfile_dbs( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  wow_CreateFileSelDia( wtt->toplevel, "Loadfile Selection", (void *)wtt,
			wtt_file_selected_cb, wow_eFileSelType_Dbs);
  wtt->reset_cursor();
}

static void wtt_activate_openfile_wbl( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  wow_CreateFileSelDia( wtt->toplevel, "Loadfile Selection", (void *)wtt,
			wtt_file_selected_cb, wow_eFileSelType_Wbl);
  wtt->reset_cursor();
}

static void wtt_activate_openpl( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, wb_eType_ExternVolume, "ProjectList", wow_eFileSelType_);
  wtt->reset_cursor();
}

static void wtt_activate_opengvl( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, wb_eType_ExternVolume, "GlobalVolumeList", wow_eFileSelType_);
  wtt->reset_cursor();
}

static void wtt_activate_openudb( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_clock_cursor();
  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, wb_eType_ExternVolume, "UserDatabase", wow_eFileSelType_);
  wtt->reset_cursor();
}

static void wtt_activate_spreadsheet( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
#if defined OS_VMS
  pwr_tObjid 	objid;
  pwr_tClassId 	classid;
  int		sts;
  pwr_sAttrRef	*sellist;
  int           *sel_is_attr;
  int		sel_cnt;

  wtt->message( ' ', "");

  objid = pwr_cNObjid;
  classid = pwr_cNClassId;
  sel_cnt = 0;

  // Get selections i w1 and w2
  sts = wtt->wnav->get_select( &sellist, &sel_is_attr, &sel_cnt);
  if ( EVEN(sts))
    sts = wtt->wnavnode->get_select( &sellist, &sel_is_attr, &sel_cnt);
  if ( ODD(sts))
  {
    objid = sellist[0].Objid;
    sts = ldh_GetObjectClass( wtt->ldhses, objid, &classid);
  }

  // Wake spred sheet editor
  wtt->set_clock_cursor();
  sped_new( wtt, wtt->toplevel, NULL, objid, classid, wtt->ldhses, 
	wtt->editmode);
  wtt->reset_cursor();
  if ( sel_cnt > 0) {
    free( (char *)sellist);
    free( (char *)sel_is_attr);
  }
  return;
#else
  new Wda( wtt->toplevel, wtt, wtt->ldhses,
		pwr_cNObjid, 0, "", wtt->editmode, wtt->wnav->gbl.advanced_user, 1);
#endif
}

static void wtt_activate_openge( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  int 		sts;
  pwr_sAttrRef 	attrref;
  int           is_attr;
  pwr_tClassId 	classid;
  char		*action;
  int		size;
  int		graph_found = 0;
  char		graph_name[80];

  if ( !wtt->ldhses) 
    MESSAGE_RETURN( "No volume is attached");
  wtt->message( ' ', "");

  while( 1)
  {
    sts = wtt->get_select_first( &attrref, &is_attr);
    if ( EVEN(sts)) break;

    sts = ldh_GetObjectClass( wtt->ldhses, attrref.Objid, &classid);
    if ( EVEN(sts)) break;

    if ( classid == pwr_cClass_XttGraph) {
      sts = ldh_GetObjectPar( wtt->ldhses, attrref.Objid, "RtBody",
			"Action", &action, &size); 
      if ( EVEN(sts)) break;

      cdh_ToLower( graph_name, action);
      free( (char *)action);
    }
    else if ( classid == pwr_cClass_WebGraph) {
      sts = ldh_GetObjectPar( wtt->ldhses, attrref.Objid, "RtBody",
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
    else
    {
      wtt->message('E', "No graph defined in selected object");
      return;
    }
    break;
  }
  wtt->set_clock_cursor();
  if ( graph_found)
    ge_new( NULL, wtt->toplevel, wtt->ldhses, 0, graph_name);
  else
    ge_new( NULL, wtt->toplevel, wtt->ldhses, 0, NULL);
  wtt->reset_cursor();
}

static void wtt_activate_openclasseditor( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  int 		sts;
  pwr_sAttrRef 	attrref;
  int           is_attr;
  pwr_tClassId 	classid;
  char		name[80];
  char		fname[200];
  int		size;

  if ( !wtt->ldhses) 
    MESSAGE_RETURN( "No volume is attached");
  wtt->message( ' ', "");

  sts = wtt->get_select_first( &attrref, &is_attr);
  if ( EVEN(sts)) {
    wtt->message('E', "Select a ClassVolumeConfig object");
    return;
  }

  sts = ldh_GetObjectClass( wtt->ldhses, attrref.Objid, &classid);
  if ( EVEN(sts)) return;
  
  if ( classid == pwr_cClass_ClassVolumeConfig ||
       classid == pwr_cClass_ClassVolumeLoad) {
    sts = ldh_ObjidToName( wtt->ldhses, attrref.Objid, ldh_eName_Object, 
					   name, sizeof(name), &size); 
    if ( EVEN(sts)) return;

    cdh_ToLower( name, name);
    sprintf( fname, "$pwrp_db/%s.wb_load", name);
    dcli_translate_filename( fname, fname);
  }
  else {
    wtt->message('E', "Select a ClassVolumeConfig object");
    return;
  }

  wtt->set_clock_cursor();
  if ( wtt->open_volume_cb)
    (wtt->open_volume_cb) ( wtt, wb_eType_ClassEditor, fname, wow_eFileSelType_WblClass);
  wtt->reset_cursor();
}

static void wtt_activate_buildvolume( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->message( ' ', "");

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();

  wtt->set_clock_cursor();
  wb_build build( *(wb_session *)wtt->ldhses, wtt->focused_wnav, wtt->toplevel);
  build.opt = wtt->focused_wnav->gbl.build;
  build.volume();
  
  wtt->reset_cursor();
  if ( build.evenSts()) MESSAGE_RETURN_STS(build.sts());
  if ( build.sts() == PWRB__NOBUILT)
    wtt->message('I', "Nothing to build");
}

static void wtt_activate_buildnode( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->message(' ',"");

  if ( wtt->boot.dia == 0) 
  {
    wtt->set_clock_cursor();
    wtt_open_boot_window( wtt);
    wtt->reset_cursor();
  }
  else
    XRaiseWindow( flow_Display(wtt->boot.dia), flow_Window(XtParent(wtt->boot.dia)));
}

static void wtt_activate_distribute( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{

  wtt->message( ' ', "");

  if ( wtt->wpkg == 0) {
    wtt->set_clock_cursor();
    wtt->wpkg = new WPkg( wtt->toplevel, wtt);
    wtt->wpkg->close_cb = wtt_wpkg_quit_cb;
    wtt->reset_cursor();
  }
  else
    wtt->wpkg->pop();
}

static void wtt_activate_showcrossref( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char cmd[80] = "crossref";
  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  wtt->focused_wnav->command( cmd);
}

static void wtt_activate_zoom_in( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  wtt->wnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  wtt->wnav->zoom( 1.18);
  wtt->wnavnode->zoom( 1.18);
  wtt->palette->zoom( 1.18);
}

static void wtt_activate_zoom_out( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  wtt->wnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  wtt->wnav->zoom( 1.0 / 1.18);
  wtt->wnavnode->zoom( 1.0 / 1.18);
  wtt->palette->zoom( 1.0 / 1.18);
}

static void wtt_activate_zoom_reset( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->wnav->unzoom();
  wtt->wnavnode->unzoom();
  wtt->palette->unzoom();
}

static void wtt_activate_twowindows( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->set_twowindows( !wtt->twowindows, 0, 0);
}

static void wtt_activate_messages( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  MsgWindow::map_default();
}

static void wtt_activate_view( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->update_options_form();
  XtManageChild( wtt->options_form);
}

static void wtt_activate_savesettings( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->save_settings();
}

static void wtt_activate_restoresettings( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->restore_settings();
}

static void wtt_activate_scriptproj( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char cmd[80] = "show script /option=project";
  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  wtt->focused_wnav->command( cmd);
}

static void wtt_activate_scriptbase( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char cmd[80] = "show script /option=base";

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  wtt->focused_wnav->command( cmd);
}

static void wtt_activate_help( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char cmd[80] = "help overview /base";

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  wtt->focused_wnav->command( cmd);
}

static void wtt_activate_help_project( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char cmd[80] = "help index /project";

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  wtt->focused_wnav->command( cmd);
}

static void wtt_activate_help_proview( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char cmd[80] = "help /version";

  if ( !wtt->focused_wnav)
    wtt->set_focus_default();
  wtt->focused_wnav->command( cmd);
}

void wtt_create_menubutton( Widget w, Wtt *wtt) 
{
  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) 
  {
    case 1: wtt->menu_save_w = w; break;
    case 2: wtt->menu_revert_w = w; break;
    case 3: wtt->menu_cut_w = w; break;
    case 4: wtt->menu_rename_w = w; break;
    case 5: wtt->menu_utilities_w = w; break;
    case 6: wtt->menu_openplc_w = w; break;
    case 7: wtt->menu_buildvolume_w = w; break;
    case 8: wtt->menu_buildnode_w = w; break;
    case 9: wtt->menu_distribute_w = w; break;
    case 10: wtt->menu_change_value_w = w; break;
    case 11: wtt->menu_copy_w = w; break;
    case 12: wtt->menu_paste_w = w; break;
    case 13: wtt->menu_buildobject_w = w; break;
    case 14: wtt->menu_pasteinto_w = w; break;
    case 15: wtt->menu_edit_w = w; break;
    case 16: wtt->menu_classeditor_w = w; break;
    case 17: wtt->menu_copykeep_w = w; break;
    default:
      ;
  }
}

static void wtt_create_msg_label( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->msg_label = w;
}
static void wtt_create_cmd_prompt( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->cmd_prompt = w;
}

static void wtt_valchanged_cmd_input( Widget w, XEvent *event)
{
  Wtt 	*wtt;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &wtt);
  XtGetValues(w, args, 1);

  if ( wtt->input_open)
    sts = mrm_TextInput( w, event, (char *)wtt->value_recall, sizeof(wtt->value_recall[0]),
	sizeof( wtt->value_recall)/sizeof(wtt->value_recall[0]),
	&wtt->value_current_recall);
  else
    sts = mrm_TextInput( w, event, (char *)wtt->cmd_recall, sizeof(wtt->cmd_recall[0]),
	sizeof( wtt->cmd_recall)/sizeof(wtt->cmd_recall[0]),
	&wtt->cmd_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( wtt->input_open)
    {
      switch( wtt->input_mode) 
      {
        case wtt_eInputMode_Attribute:
          wtt->input_wnav->select_object( wtt->input_node);
          sts = wtt->input_wnav->set_attr_value( wtt->input_node, 
		wtt->input_objid, text);
          if ( EVEN(sts))
            wtt->message( 'E', wnav_get_message( sts));
          break;
        case wtt_eInputMode_ObjectName:
          wtt->input_wnav->select_object( wtt->input_node);
          sts = wtt->input_wnav->set_object_name( wtt->input_node, 
		wtt->input_objid, text);
          if ( EVEN(sts))
            wtt->message( 'E', wnav_get_message( sts));
          break;
        default:
          ;
      }
      if ( wtt->input_open) {
	XtUnmanageChild( w);
	wtt->set_prompt( "");
	wtt->input_open = 0;
      }
    }
    else if ( wtt->command_open)
    {
      if ( !wtt->focused_wnav)
        wtt->set_focus_default();
      wtt->set_clock_cursor();
      sts = wtt->focused_wnav->command( text);
      wtt->reset_cursor();
      XtUnmanageChild( w);
      wtt->set_prompt( "");
      wtt->command_open = 0;
    }
  }
}

static void wtt_create_cmd_input( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, wtt);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) wtt_valchanged_cmd_input, mrm_eUtility_Wtt);

  wtt->cmd_input = w;
}
static void wtt_create_wnav_form( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->wnav_form = w;
}
static void wtt_create_palette_form( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->palette_form = w;
}
static void wtt_create_india_label( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->india_label = w;
}
static void wtt_create_india_text( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->india_text = w;
}
static void wtt_activate_india_ok( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  char *value;

  value = XmTextGetString( wtt->india_text);
  XtUnmanageChild( wtt->india_widget);

  (wtt->india_ok_cb)( wtt, value);
}
static void wtt_activate_india_cancel( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( wtt->india_widget);
}
static void wtt_activate_confirm_ok( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( wtt->confirm_widget);
  wtt->confirm_open = 0;
  if ( wtt->confirm_ok_cb)
    (wtt->confirm_ok_cb)( wtt);
}
static void wtt_activate_confirm_no( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( wtt->confirm_widget);
  wtt->confirm_open = 0;
  if ( wtt->confirm_no_cb)
    (wtt->confirm_no_cb)( wtt);
}
static void wtt_activate_confirm_cancel( Widget w, Wtt *wtt, XmAnyCallbackStruct *data)
{
  wtt->confirm_open = 0;
  XtUnmanageChild( wtt->confirm_widget);
}

static void wtt_enable_set_focus( Wtt *wtt)
{
  wtt->set_focus_disabled--;
}

static void wtt_disable_set_focus( Wtt *wtt, int time)
{
  wtt->set_focus_disabled++;
  wtt->disfocus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( wtt->toplevel), time,
	(XtTimerCallbackProc)wtt_enable_set_focus, wtt);
}

static void  wtt_reset_avoid_deadlock( Wtt *wtt)
{
  wtt->avoid_deadlock = 0;
}

static void  wtt_set_avoid_deadlock( Wtt *wtt, int time)
{
  wtt->avoid_deadlock = 1;
  wtt->deadlock_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( wtt->toplevel), time,
	(XtTimerCallbackProc)wtt_reset_avoid_deadlock, wtt);
}

static void wtt_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Wtt *wtt;

  XtSetArg    (args[0], XmNuserData, &wtt);
  XtGetValues (w, args, 1);

  if ( mrm_IsIconicState(w))
    return;

  if ( wtt->set_focus_disabled)
    return;

  if ( wtt->focused_wnav) {
    wtt->set_focus( wtt->focused_wnav);
    // wtt->focus_timerid = XtAppAddTimeOut( XtWidgetToApplicationContext( wtt->toplevel), 200,
    //	(XtTimerCallbackProc)wtt_set_focus_proc, wtt);
  }
  wtt_disable_set_focus( wtt, 400);
}

static void wtt_open_input_dialog( Wtt *wtt, char *text, char *title,
	char *init_text,
	void (*ok_cb)( Wtt *, char *))
{
  Arg		args[10];
  int 		i;
  XmString	cstr;

  i = 0;
  XtSetArg(args[0], XmNlabelString,
		cstr=XmStringCreateLtoR( text, "ISO8859-1") ); i++;
  XtSetValues( wtt->india_label, args, i);
  XmStringFree( cstr);
  i = 0;
  XtSetArg(args[0], XmNdialogTitle,
		cstr=XmStringCreateLtoR( title, "ISO8859-1") ); i++;
  XtSetValues( wtt->india_widget, args, i);
  XmStringFree( cstr);

  XmTextSetString( wtt->india_text, init_text);

  XtManageChild( wtt->india_widget);

  XmProcessTraversal( wtt->india_text, XmTRAVERSE_CURRENT);

  wtt->india_ok_cb = ok_cb;
}

static void wtt_open_confirm( Wtt *wtt, char *text, char *title, 
	void (*ok_cb)( Wtt *), void (*no_cb)( Wtt *))
{
  Arg 		args[3];
  XmString	cstr;

  if ( wtt->confirm_open) 
  {
    XtUnmanageChild( wtt->confirm_widget);
    wtt->confirm_open = 0;
    return;
  }

  XtManageChild( wtt->confirm_widget);

  wtt->message( ' ', "");

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, "ISO8859-1"));
  XtSetArg(args[1], XmNdialogTitle,
		cstr=XmStringCreateLtoR( title, "ISO8859-1") );
  XtSetValues( wtt->confirm_widget, args, 2);
  XmStringFree( cstr);
  wtt->confirm_open = 1;
  wtt->confirm_ok_cb = ok_cb;
  wtt->confirm_no_cb = no_cb;
}

static void wtt_activate_selmode( Widget w, Wtt *wtt, 
	XmToggleButtonCallbackStruct *data)
{
  String name;
    
  name = XtName(w);
    
  if (!strcmp(name,"normalSyntax"))
  {
    if (!data->set)
    {
      XmToggleButtonSetState( w, 1, 0);
      return;
    }
    XmToggleButtonSetState( wtt->copy_mode.gms_syntax, 0, 0);
    XmToggleButtonSetState( wtt->copy_mode.extern_syntax, 0, 0);
    XmToggleButtonSetState( wtt->copy_mode.extern_syntax, 0, 0);
    XtSetSensitive( wtt->copy_mode.add_type, 1);
    XmToggleButtonSetState( wtt->copy_mode.add_type, 0, 0);
    wtt->select_type = 0;
    XtSetSensitive( wtt->copy_mode.add_volume, 1);
    wtt->select_syntax = wnav_eSelectionMode_Normal;
  }
  else if (!strcmp(name,"gmsSyntax"))
  {
    if (!data->set)
    {
      XmToggleButtonSetState( w, 1, 0);
      return;
    }
    XmToggleButtonSetState( wtt->copy_mode.normal_syntax, 0, 0);
    XmToggleButtonSetState( wtt->copy_mode.extern_syntax, 0, 0);
    XtSetSensitive( wtt->copy_mode.add_type, 1);
    XtSetSensitive( wtt->copy_mode.add_volume, 1);
    wtt->select_syntax = wnav_eSelectionMode_GMS;
  }
  else if (!strcmp(name,"externSyntax"))
  {
    if (!data->set)
    {
      XmToggleButtonSetState( w, 1, 0);
      return;
    }
    XmToggleButtonSetState( wtt->copy_mode.normal_syntax, 0, 0);
    XmToggleButtonSetState( wtt->copy_mode.gms_syntax, 0, 0);
    XtSetSensitive( wtt->copy_mode.add_type, 0);
    XmToggleButtonSetState( wtt->copy_mode.add_type, 0, 0);
    XtSetSensitive( wtt->copy_mode.add_volume, 0);
    wtt->select_type = 0;
    XmToggleButtonSetState( wtt->copy_mode.add_volume, 0, 0);
    wtt->select_syntax = wnav_eSelectionMode_Extern;
    wtt->select_volume = 0;
  }
  else if (!strcmp(name,"addVolume"))
    wtt->select_volume = data->set;
  else if (!strcmp(name,"addAttribute"))
    wtt->select_attr = data->set;
  else if (!strcmp(name,"addType"))
  {
    wtt->select_type = data->set;
    if ( data->set && !wtt->select_attr)
    {
      XmToggleButtonSetState( wtt->copy_mode.add_attribute, 1, 0);
      wtt->select_attr = 1;
    }
  }
}

static void wtt_create_selmode( Widget w, Wtt *wtt, 
    XmToggleButtonCallbackStruct *data)
{
  XmToggleButtonCallbackStruct cbs;
  String name;

  name = XtName(w);
    
  if (!strcmp(name,"normalSyntax"))
  {
      wtt->copy_mode.normal_syntax = w;
      XmToggleButtonSetState( w, 1, 0);
  }
  else if (!strcmp(name,"gmsSyntax"))
    wtt->copy_mode.gms_syntax = w;
  else if (!strcmp(name,"externSyntax"))
    wtt->copy_mode.extern_syntax = w;
  else if (!strcmp(name,"addVolume"))
    wtt->copy_mode.add_volume = w;
  else if (!strcmp(name,"addAttribute"))
    wtt->copy_mode.add_attribute = w;
  else if (!strcmp(name,"addType"))
  {
    wtt->copy_mode.add_type = w;

    // Register button which is set
    cbs.reason = XmCR_ACTIVATE;
    cbs.event = NULL;
    cbs.set = True;
    wtt_activate_selmode( wtt->copy_mode.normal_syntax, wtt, &cbs);
  }
}

// Open a Create Boot File window
static void wtt_open_boot_window( Wtt *wtt)
{
  XmString cstr;  
  int i;
  lfu_t_volumelist *volumelist_ptr;
  pwr_tString40 nodename;
  pwr_tStatus 	sts;
  
/* DRM database hierarchy related variables */
  char		uid_filename[200] = {"pwr_exe:wb_wtt.uid"};
  char		*uid_filename_p = uid_filename;

  static MrmRegisterArg reglist[] =
  {
  /* First the context variable */
    	{ "wtt_ctx", NULL },
	{"wtt_boot_list_cr",(caddr_t)wtt_boot_list_cr },
	{"wtt_boot_ok_cb",(caddr_t)wtt_boot_ok_cb },
	{"wtt_boot_cancel_cb",(caddr_t)wtt_boot_cancel_cb },
	{"wtt_boot_destroy_cb",(caddr_t)wtt_boot_destroy_cb },
	};
  static int reglist_num = XtNumber(reglist);

  // Load the bootlist
  sts = lfu_volumelist_load( load_cNameBootList, 
		(lfu_t_volumelist **) &wtt->boot.volumelist,
		&wtt->boot.volumecount);
  if (sts == FOE__NOFILE) {
    MESSAGE_RETURN( "Project is not configured");
  }
  else if (EVEN(sts)) {
    MESSAGE_RETURN( "Syntax error in bootlist file");
  }

  if (wtt->boot.dia == NULL) {
    sts = dcli_translate_filename( uid_filename, uid_filename);
    if ( EVEN(sts)) return;

    reglist[0].value = (caddr_t) wtt;
    uilutil_fetch( 
      	&uid_filename_p,
      	1,
      	reglist, 
      	reglist_num,
      	wtt->toplevel,
      	"bootFilesWindow", 
     	"bootFilesWindow", 
     	 NULL, 
     	 0,
     	 &wtt->boot.dia, 
     	 NULL 
     	 );

    XtVaSetValues(
      XtParent(wtt->boot.dia), 
      XmNdeleteResponse, XmDESTROY, 
      NULL
      ); 
  }

  strcpy( nodename, "");
  volumelist_ptr = (lfu_t_volumelist *) wtt->boot.volumelist;
  for ( i = 0; i < wtt->boot.volumecount; i++)
  {
    if ( strcmp(volumelist_ptr->p1, nodename))
    {
      strcpy( nodename, volumelist_ptr->p1);
      cstr = XmStringCreateSimple( nodename);
      XmListAddItemUnselected(wtt->boot.list, cstr, 0);
      XmStringFree(cstr);
    }
    volumelist_ptr++;
  }

  XtManageChild( wtt->boot.dia);
  return;
}

// Callbacks for the Create Boot Files window

// Create callback for the list widget
static void wtt_boot_list_cr(Widget w, Wtt *wtt)
{
    wtt->boot.list = w;
}

// Widget callbacks for Cancel button
static void wtt_boot_cancel_cb(Widget w, Wtt *wtt)
{
    XtDestroyWidget( wtt->boot.dia);
}

// Destroy callback
static void wtt_boot_destroy_cb(Widget w, Wtt *wtt)
{
    if ( wtt->boot.volumelist && wtt->boot.volumecount)
    {
      XtFree( (char *) wtt->boot.volumelist);
      wtt->boot.volumelist = 0;
    }
    wtt->boot.dia = 0;
}

// Widget callbacks for OK button
static void wtt_boot_ok_cb(Widget w, Wtt *wtt)
{
    int *pos_list, pos_cnt;
    int i, j, nodecount;
    lfu_t_volumelist *volumelist_ptr;
    pwr_tString40 nodeconfigname;
    char message[200];
    int bootfile_count;

    if ( !wtt->focused_wnav)
      wtt->set_focus_default();

    wtt->message( ' ', "");

    nodecount = 0;
    bootfile_count = 0;
    volumelist_ptr = (lfu_t_volumelist *)wtt->boot.volumelist;
    if (XmListGetSelectedPos(wtt->boot.list, &pos_list, &pos_cnt)) {
      for (i = 0; i < pos_cnt; i++) {
        strcpy( nodeconfigname, "");
        for ( j = 0; j < wtt->boot.volumecount; j++) {
          if ( strcmp(volumelist_ptr->p1, nodeconfigname)) {
            nodecount++;

            strcpy( nodeconfigname, volumelist_ptr->p1);
	    if ( nodecount == pos_list[i]) {
	      wb_build build( *(wb_session *)wtt->ldhses, wtt->focused_wnav, wtt->toplevel);
	      build.opt = wtt->focused_wnav->gbl.build;
	      build.node( nodeconfigname, 
			  wtt->boot.volumelist, wtt->boot.volumecount);
	      if ( build.evenSts()) {
                XtDestroyWidget(wtt->boot.dia);
	        sprintf( message, 
			"Error creating bootfile for NodeConfig-object %s",
			nodeconfigname);
	        MESSAGE_RETURN( message);
	      }
	      else if ( build.sts() != PWRB__NOBUILT)
		bootfile_count++;
              volumelist_ptr++;
	      break;
	    }
          }
          volumelist_ptr++;
        }
      }
    }

    XtDestroyWidget(wtt->boot.dia);
    if ( !bootfile_count)
      MESSAGE_RETURN("Nothing to build");
    sprintf( message, "Bootfile%s created", (bootfile_count == 1) ? "":"s");
    MESSAGE_RETURN( message);
	  
}

/************************************************************************
*
* Name: update_options_form
*
* Description: This routine must be called when the configurator/navigator is 
*	       managed.
* 
*************************************************************************/
void Wtt::update_options_form()
{

/*
 * Hierarchies
 */
 XmToggleButtonSetState(show_plant_w, 
			  (Boolean) wnav_mapped, False);
 XmToggleButtonSetState(show_node_w, 
			  (Boolean) wnavnode_mapped, False);

/*
 * entry components
 */
 XmToggleButtonSetState( show_class_w, (Boolean) show_class, False);
 XmToggleButtonSetState( show_alias_w, (Boolean) show_alias, False);
 XmToggleButtonSetState( show_descrip_w, (Boolean) show_descrip, False);
 XmToggleButtonSetState( show_objref_w, (Boolean) show_objref, False);
 XmToggleButtonSetState( show_objxref_w, (Boolean) show_objxref, False);
 XmToggleButtonSetState( show_attrref_w, (Boolean) show_attrref, False);
 XmToggleButtonSetState( show_attrxref_w, (Boolean) show_attrxref, False);
 XmToggleButtonSetState( build_force_w, (Boolean) build_force, False);
 XmToggleButtonSetState( build_debug_w, (Boolean) build_debug, False);
 XmToggleButtonSetState( build_crossref_w, (Boolean) build_crossref, False);
 XmToggleButtonSetState( build_manual_w, (Boolean) build_manual, False);


} /* update_options_form */


/************************************************************************
*
* Name: set_options
*
*
*************************************************************************/
void Wtt::set_options()
{
  show_class = XmToggleButtonGetState(show_class_w);
  show_alias = XmToggleButtonGetState(show_alias_w);
  show_descrip = XmToggleButtonGetState(show_descrip_w);
  show_objref = XmToggleButtonGetState(show_objref_w);
  show_objxref = XmToggleButtonGetState(show_objxref_w);
  show_attrref = XmToggleButtonGetState(show_attrref_w);
  show_attrxref = XmToggleButtonGetState(show_attrxref_w);
  build_force = XmToggleButtonGetState(build_force_w);
  build_debug = XmToggleButtonGetState(build_debug_w);
  build_crossref = XmToggleButtonGetState(build_crossref_w);
  build_manual = XmToggleButtonGetState(build_manual_w);

  wnav->set_options( show_class, show_alias, 
	show_descrip, show_objref, show_objxref, 
	show_attrref, show_attrxref, build_force, build_debug,
	build_crossref, build_manual);
  wnavnode->set_options( show_class, show_alias, 
	show_descrip, show_objref, show_objxref, 
        show_attrref, show_attrxref, build_force, build_debug,
	build_crossref, build_manual);

}/* set_options */


/* Callbacks from the options form */
/************************************************************************
*
* Name: wtt_options_act_but_cb
*
*	Callback from the OK, Apply or Cancel button in the Options form
* 
*************************************************************************/
void wtt_options_act_but_cb( Widget w, Wtt *wtt)
{
  int key;
  int plant, node;
  
  wtt->message( ' ', "");

  plant = XmToggleButtonGetState( wtt->show_plant_w);
  node = XmToggleButtonGetState( wtt->show_node_w);

  XtVaGetValues(w, XmNuserData, &key, NULL);
  switch (key) {
  case 1 : /* OK */
    XtUnmanageChild( wtt->options_form);
  case 2 : /* Apply */
    wtt->set_twowindows( 0, plant, node);
    wtt->set_options();
    break;

  case 3 : /* Cancel */
    wtt->update_options_form();
    XtUnmanageChild( wtt->options_form);
    break;
  }


}/* wtt_options_act_but_cb */

/************************************************************************
*
* Name: wtt_options_form_cr
*
* Description: Create callback for the Options form
*
*************************************************************************/

void wtt_options_form_cr( Widget w, Wtt *wtt) 
{
  wtt->options_form = w;
}

/************************************************************************
*
* Name: wtt_options_entry_tog_cr
*
* Description: Create callback for entry toggle buttons in the Options form
*
*************************************************************************/

void wtt_options_entry_tog_cr( Widget w, Wtt *wtt) 
{

  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) {
  case 1: /* Class */
    wtt->show_class_w = w;
    wtt->show_class = XmToggleButtonGetState(w);
    break;
  case 2: /* Alias */
    wtt->show_alias_w = w;
    wtt->show_alias = XmToggleButtonGetState(w);
    break;
  case 3: /* Description */
    wtt->show_descrip_w = w;
    wtt->show_descrip = XmToggleButtonGetState(w);
    break;
  case 4: /* Objref */
    wtt->show_objref_w = w;
    wtt->show_objref = XmToggleButtonGetState(w);
    break;
  case 5: /* Objxref */
    wtt->show_objxref_w = w;
    wtt->show_objxref = XmToggleButtonGetState(w);
    break;
  case 6: /* Attrref */
    wtt->show_attrref_w = w;
    wtt->show_attrref = XmToggleButtonGetState(w);
    break;
  case 7: /* Attrxref */
    wtt->show_attrxref_w = w;
    wtt->show_attrxref = XmToggleButtonGetState(w);
    break;
  case 8:
    wtt->build_force_w = w;
    wtt->build_force = XmToggleButtonGetState(w);
    break;
  case 9:
    wtt->build_debug_w = w;
    wtt->build_debug = XmToggleButtonGetState(w);
    break;
  case 10:
    wtt->build_crossref_w = w;
    wtt->build_crossref = XmToggleButtonGetState(w);
    break;
  case 11:
    wtt->build_manual_w = w;
    wtt->build_manual = XmToggleButtonGetState(w);
    break;
  default:
    break;
  }

} /* wtt_options_entry_cr */

/************************************************************************
*
* Name: wtt_options_hier_tog_cr
*
* Description: Create callback for hier toggle buttons in the Options form
*
*************************************************************************/

void wtt_options_hier_tog_cr( Widget w, Wtt *wtt) 
{
  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) {
  case 1: /* Plant Hier */
    wtt->show_plant_w = w;
    break;
  case 2: /* Node Hier */
    wtt->show_node_w = w;
    break;
  default:
    ;
  }

} /* wtt_options_hier_tog_cr */


static pwr_tBoolean wtt_format_selection(
    void *ctx,
    pwr_sAttrRef attrref,
    XtPointer   *value_return,
    unsigned long   *length_return,
    pwr_tBoolean is_class,
    pwr_tBoolean is_attr,
    wnav_eSelectionFormat format
    )
{
  char buff[256];
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
    *value_return = XtNewString(buff);      
    *length_return = strlen(buff) + 1;
    
    return TRUE;
  }
  return FALSE;
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
	Widget 	wt_parent_wid,
	char 	*wt_name,
	char	*iconname,
	void	*wt_wbctx,
	pwr_tVolumeId wt_volid,
	ldh_tVolume wt_volctx,
	wnav_sStartMenu *root_menu,
	pwr_tStatus *status
	) :
	ctx_type(wb_eUtility_Wtt), parent_ctx(wt_parent_ctx), parent_wid( wt_parent_wid),
	wb_type(wb_eType_Volume), root_item(0), input_open(0), command_open(0),
	wbctx(wt_wbctx), volctx(wt_volctx), volid(wt_volid), ldhses(0),
	editmode(0), twowindows(0), confirm_open(0), select_volume(0), 
        select_attr(0), select_type(0),
        wnav_mapped(0), wnavnode_mapped(0), utedctx(0), wpkg(0),
	close_cb(0), open_volume_cb(0), open_project_volume_cb(0), time_to_exit_cb(0),
	set_focus_disabled(0), disfocus_timerid(0), selection_timerid(0), avoid_deadlock(0), 
	clock_cursor(0), cmd_current_recall(0), value_current_recall(0)
{
  char		uid_filename[200] = {"pwr_exe:wb_wtt.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy 	s_DRMh;
  MrmType 	dclass;
  Widget	wtt_widget;
  char		volname[80];
  pwr_tClassId	volclass;
  int		size;
  char		layout_w1[80];
  char		layout_w2[80];
  char		layout_palette[80];
  char		title_w1[40];
  char		title_w2[40];
  int		hide_wnavnode = 0;

  static char translations[] = "\
<FocusIn>: wtt_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"wtt_inputfocus",      (XtActionProc) wtt_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "wtt_ctx", 0 },
	{"wtt_activate_exit",(caddr_t)wtt_activate_exit },
	{"wtt_activate_collapse",(caddr_t)wtt_activate_collapse },
	{"wtt_activate_save",(caddr_t)wtt_activate_save },
	{"wtt_activate_revert",(caddr_t)wtt_activate_revert },
	{"wtt_activate_syntax",(caddr_t)wtt_activate_syntax },
	{"wtt_activate_print",(caddr_t)wtt_activate_print },
	{"wtt_activate_find",(caddr_t)wtt_activate_find },
	{"wtt_activate_findregex",(caddr_t)wtt_activate_findregex },
	{"wtt_activate_findnext",(caddr_t)wtt_activate_findnext },
	{"wtt_activate_selmode",(caddr_t)wtt_activate_selmode },
	{"wtt_activate_cut",(caddr_t)wtt_activate_cut },
	{"wtt_activate_copy",(caddr_t)wtt_activate_copy },
	{"wtt_activate_paste",(caddr_t)wtt_activate_paste },
	{"wtt_activate_pasteinto",(caddr_t)wtt_activate_pasteinto },
	{"wtt_activate_copykeep",(caddr_t)wtt_activate_copykeep },
	{"wtt_activate_rename",(caddr_t)wtt_activate_rename },
	{"wtt_activate_configure",(caddr_t)wtt_activate_configure },
	{"wtt_activate_utilities",(caddr_t)wtt_activate_utilities },
	{"wtt_activate_openplc",(caddr_t)wtt_activate_openplc },
	{"wtt_activate_openobject",(caddr_t)wtt_activate_openobject },
	{"wtt_activate_openvolobject",(caddr_t)wtt_activate_openvolobject },
	{"wtt_activate_confproject",(caddr_t)wtt_activate_confproject },
	{"wtt_activate_openvolume",(caddr_t)wtt_activate_openvolume },
	{"wtt_activate_openbuffer",(caddr_t)wtt_activate_openbuffer },
	{"wtt_activate_openfile_dbs",(caddr_t)wtt_activate_openfile_dbs },
	{"wtt_activate_openfile_wbl",(caddr_t)wtt_activate_openfile_wbl },
	{"wtt_activate_openpl",(caddr_t)wtt_activate_openpl },
	{"wtt_activate_opengvl",(caddr_t)wtt_activate_opengvl },
	{"wtt_activate_openudb",(caddr_t)wtt_activate_openudb },
	{"wtt_activate_spreadsheet",(caddr_t)wtt_activate_spreadsheet },
	{"wtt_activate_openge",(caddr_t)wtt_activate_openge },
	{"wtt_activate_openclasseditor",(caddr_t)wtt_activate_openclasseditor },
	{"wtt_activate_buildobject",(caddr_t)wtt_activate_buildobject },
	{"wtt_activate_buildvolume",(caddr_t)wtt_activate_buildvolume },
	{"wtt_activate_buildnode",(caddr_t)wtt_activate_buildnode },
	{"wtt_activate_distribute",(caddr_t)wtt_activate_distribute },
	{"wtt_activate_showcrossref",(caddr_t)wtt_activate_showcrossref },
	{"wtt_activate_change_value",(caddr_t)wtt_activate_change_value },
	{"wtt_activate_command",(caddr_t)wtt_activate_command },
	{"wtt_activate_zoom_in",(caddr_t)wtt_activate_zoom_in },
	{"wtt_activate_zoom_out",(caddr_t)wtt_activate_zoom_out },
	{"wtt_activate_zoom_reset",(caddr_t)wtt_activate_zoom_reset },
	{"wtt_activate_twowindows",(caddr_t)wtt_activate_twowindows },
	{"wtt_activate_messages",(caddr_t)wtt_activate_messages },
	{"wtt_activate_view",(caddr_t)wtt_activate_view },
	{"wtt_activate_savesettings",(caddr_t)wtt_activate_savesettings },
	{"wtt_activate_restoresettings",(caddr_t)wtt_activate_restoresettings },
	{"wtt_activate_scriptproj",(caddr_t)wtt_activate_scriptproj },
	{"wtt_activate_scriptbase",(caddr_t)wtt_activate_scriptbase },
	{"wtt_activate_help",(caddr_t)wtt_activate_help },
	{"wtt_activate_help_project",(caddr_t)wtt_activate_help_project },
	{"wtt_activate_help_proview",(caddr_t)wtt_activate_help_proview },
	{"wtt_create_menubutton",(caddr_t)wtt_create_menubutton },
	{"wtt_create_msg_label",(caddr_t)wtt_create_msg_label },
	{"wtt_create_cmd_prompt",(caddr_t)wtt_create_cmd_prompt },
	{"wtt_create_cmd_input",(caddr_t)wtt_create_cmd_input },
	{"wtt_create_palette_form",(caddr_t)wtt_create_palette_form },
	{"wtt_create_wnav_form",(caddr_t)wtt_create_wnav_form },
	{"wtt_create_selmode",(caddr_t)wtt_create_selmode },
	{"wtt_activate_india_ok",(caddr_t)wtt_activate_india_ok },
	{"wtt_activate_india_cancel",(caddr_t)wtt_activate_india_cancel },
	{"wtt_create_india_label",(caddr_t)wtt_create_india_label },
	{"wtt_create_india_text",(caddr_t)wtt_create_india_text },
	{"wtt_activate_confirm_ok",(caddr_t)wtt_activate_confirm_ok },
	{"wtt_activate_confirm_no",(caddr_t)wtt_activate_confirm_no },
	{"wtt_activate_confirm_cancel",(caddr_t)wtt_activate_confirm_cancel },
	{"wtt_options_form_cr",(caddr_t)wtt_options_form_cr },
	{"wtt_options_entry_tog_cr",(caddr_t)wtt_options_entry_tog_cr },
	{"wtt_options_hier_tog_cr",(caddr_t)wtt_options_hier_tog_cr },
	{"wtt_options_act_but_cb",(caddr_t)wtt_options_act_but_cb }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  strcpy( name, wt_name);
  boot.dia = 0;
  for ( i = 0; i < int(sizeof(cmd_recall)/sizeof(cmd_recall[0])); i++)
    cmd_recall[i][0] = 0;
  for ( i = 0; i < int(sizeof(value_recall)/sizeof(value_recall[0])); i++)
    value_recall[i][0] = 0;

  sts = dcli_translate_filename( uid_filename, uid_filename);
  if ( EVEN(sts)) {
    printf( "** pwr_exe is not defined\n");
    exit(0);
  }

  if ( wbctx && volid) {
    // Get the volume class and decide what type of navigator */
    sts = ldh_GetVolumeClass( wbctx, volid, &volclass);
    if (EVEN(sts)) {
      *status = sts;
      return;
    }
    if ( volid == ldh_cDirectoryVolume) volclass = pwr_eClass_DirectoryVolume; // Fix

    sts = ldh_VolumeIdToName( wbctx, volid, volname,
		sizeof(volname), &size);
    if (EVEN(sts)) {
      *status = sts;
      return;
    }
    if ( !volctx) {
      sts = ldh_AttachVolume( wbctx, volid, &volctx);
      if (EVEN(sts)) {
        printf("-- Volume '%s' is already attached\n", volname);
        putchar( '\7' );
        *status = sts;
        return;
      }
    }

    sts = ldh_OpenSession( &ldhses, volctx, ldh_eAccess_ReadOnly,
    	ldh_eUtility_Navigator);
    if (EVEN(sts)) {
      printf("Navigator: Can't open session\n");
      *status = sts;
      return;
    }
    ldh_AddOtherSessionCallback( ldhses,  (void *)this, 
		wtt_ldh_other_session_cb);

    switch( volclass) {
    case pwr_eClass_DirectoryVolume:
      wb_type = wb_eType_Directory;
      sprintf( title, "PwR Navigator Directory %s", name);
      strcpy( layout_w1, "ProjectNavigatorW1");
      strcpy( layout_w2, "ProjectNavigatorW2");
      strcpy( layout_palette, "ProjectNavigatorPalette");
      strcpy( title_w1, "Volume Configuration");
      strcpy( title_w2, "Node Configuration");
      break;
    case pwr_eClass_ClassVolume:
      if ( ldh_VolRepType( ldhses) == ldh_eVolRep_Mem) {
	wb_type = wb_eType_ClassEditor;
	sprintf( title, "PwR ClassEditor Volume %s, %s", volname, name);
      }
      else {
	wb_type = wb_eType_Class;
	sprintf( title, "PwR Navigator Volume %s, %s", volname, name);
      }
      strcpy( layout_w1, "ClassNavigatorW1");
      strcpy( layout_w2, "ClassNavigatorW2");
      strcpy( layout_palette, "ClassNavigatorPalette");
      strcpy( title_w1, "Class Configuration");
      strcpy( title_w2, "Node Configuration");
      break;
    case pwr_eClass_VolatileVolume:
      wb_type = wb_eType_Buffer;
      strcpy( layout_w1, "");
      strcpy( layout_w2, "");
      strcpy( layout_palette, "NavigatorPalette");
      strcpy( title_w1, "Plant Configuration");
      strcpy( title_w2, "Node Configuration");
      sprintf( title, "PwR Navigator Buffer %s, %s", volname, name);
      hide_wnavnode = 1;
      break;
    case pwr_eClass_ExternVolume: {
      switch ( volid) {
      case ldh_cProjectListVolume:
	wb_type = wb_eType_ExternVolume;
	strcpy( layout_w1, "PrListNavigatorW1");
	strcpy( layout_w2, "PrListNavigatorW1");
	strcpy( layout_palette, "PrListNavigatorPalette");
	strcpy( title_w1, "Project List");
	strcpy( title_w2, "");
	sprintf( title, "PwR Project List");
	hide_wnavnode = 1;
	break;
      case ldh_cGlobalVolumeListVolume:
	wb_type = wb_eType_ExternVolume;
	strcpy( layout_w1, "GvListNavigatorW1");
	strcpy( layout_w2, "GvListNavigatorW1");
	strcpy( layout_palette, "GvListNavigatorPalette");
	strcpy( title_w1, "Global Volume List");
	strcpy( title_w2, "");
	sprintf( title, "PwR Global Volume List");
	hide_wnavnode = 1;
	break;
      case ldh_cUserDatabaseVolume:
	wb_type = wb_eType_ExternVolume;
	strcpy( layout_w1, "UserDbNavigatorW1");
	strcpy( layout_w2, "UserDbNavigatorW1");
	strcpy( layout_palette, "UserDbNavigatorPalette");
	strcpy( title_w1, "User Database");
	strcpy( title_w2, "");
	sprintf( title, "PwR User Database");
	hide_wnavnode = 1;
	break;
      default:
	wb_type = wb_eType_ExternVolume;
	strcpy( layout_w1, "NavigatorW1");
	strcpy( layout_w2, "NavigatorW2");
	strcpy( layout_palette, "NavigatorPalette");
	strcpy( title_w1, "Plant Configuration");
	strcpy( title_w2, "Node Configuration");
	sprintf( title, "PwR Navigator Volume %s, %s", volname, name);
      }
      break;
    }
    default:
      wb_type = wb_eType_Volume;
      strcpy( layout_w1, "NavigatorW1");
      strcpy( layout_w2, "NavigatorW2");
      strcpy( layout_palette, "NavigatorPalette");
      strcpy( title_w1, "Plant Configuration");
      strcpy( title_w2, "Node Configuration");
      sprintf( title, "PwR Navigator Volume %s, %s", volname, name);
    }
  }
  else {
    strcpy( layout_w1, "NavigatorW1");
    strcpy( layout_w2, "NavigatorW2");
    strcpy( title_w1, "Plant Configuration");
    strcpy( title_w2, "Node Configuration");
    strcpy( title, "PwR Wtt");
  }

  // Motif
  MrmInitialize();


  toplevel = XtCreatePopupShell( title, 
		topLevelShellWidgetClass, parent_wid, args, 0);

  reglist[0].value = (caddr_t) this;

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(toplevel), 
						actions, XtNumber(actions));
 
  // Save the context structure in the widget
  i = 0;
  XtSetArg( args[i], XmNuserData, (unsigned int) this);i++;

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, "wtt_window", toplevel,
			name, args, i, &wtt_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  sts = MrmFetchWidget(s_DRMh, "input_dialog", toplevel,
		&india_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, "confirm_dialog", toplevel,
		&confirm_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch confirm dialog\n");

  sts = MrmFetchWidget(s_DRMh, "optionsForm", toplevel,
		&options_form, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch options dialog\n");

  MrmCloseHierarchy(s_DRMh);


  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( wtt_widget, compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,600);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( wtt_widget);
  XtUnmanageChild( cmd_input);

  wnav = new WNav( this, wnav_form, title_w2, layout_w1,
		&wnav_brow_widget, ldhses, root_menu, 
		wnav_eWindowType_W1, &sts);
  wnav->message_cb = &wtt_message_cb;
  wnav->close_cb = &wtt_close;
  wnav->change_value_cb = &wtt_change_value;
  wnav->get_wbctx_cb = &wtt_get_wbctx;
  wnav->attach_volume_cb = &wtt_attach_volume_cb;
  wnav->detach_volume_cb = &wtt_detach_volume_cb;
  wnav->get_palette_select_cb = &wtt_get_palette_select_cb;
  wnav->set_focus_cb = &wtt_set_focus_cb;
  wnav->traverse_focus_cb = &wtt_traverse_focus;
  wnav->set_twowindows_cb = &wtt_set_twowindows_cb;
  wnav->set_configure_cb = &wtt_configure_cb;
  wnav->gbl_command_cb = &wtt_gbl_command_cb;
  wnav->create_popup_menu_cb = &wtt_create_popup_menu_cb;
  wnav->save_cb = &wtt_save_cb;
  wnav->revert_cb = &wtt_revert_cb;
  wnav->script_filename_cb = &wtt_script_filename_cb;
  wnav->format_selection_cb = wtt_format_selection;
  wnav->get_global_select_cb = wtt_get_global_select_cb;
  wnav->global_unselect_objid_cb = wtt_global_unselect_objid_cb;
  wnav->set_window_char_cb = wtt_set_window_char_cb;
  wnav->open_vsel_cb = wtt_open_vsel_cb;
  focused_wnav = wnav;
  wnav_mapped = 1;

  i = 0;
  XtSetArg(args[i], XmNtraversalOn, True);i++;
  XtSetArg(args[i], XmNnavigationType, XmTAB_GROUP);i++;
  XtSetValues( wnav_brow_widget, args,i);

  wnavnode = new WNav( this, wnav_form, title_w2, layout_w2,
		&wnavnode_brow_widget, ldhses, root_menu, 
		wnav_eWindowType_W2, &sts);
  wnavnode->message_cb = &wtt_message_cb;
  wnavnode->close_cb = &wtt_close;
  wnavnode->change_value_cb = &wtt_change_value;
  wnavnode->get_wbctx_cb = &wtt_get_wbctx;
  wnavnode->attach_volume_cb = &wtt_attach_volume_cb;
  wnavnode->detach_volume_cb = &wtt_detach_volume_cb;
  wnavnode->get_palette_select_cb = &wtt_get_palette_select_cb;
  wnavnode->set_focus_cb = &wtt_set_focus_cb;
  wnavnode->traverse_focus_cb = &wtt_traverse_focus;
  wnavnode->set_twowindows_cb = &wtt_set_twowindows_cb;
  wnavnode->set_configure_cb = &wtt_configure_cb;
  wnavnode->gbl_command_cb = &wtt_gbl_command_cb;
  wnavnode->create_popup_menu_cb = &wtt_create_popup_menu_cb;
  wnavnode->save_cb = &wtt_save_cb;
  wnavnode->revert_cb = &wtt_revert_cb;
  wnavnode->script_filename_cb = &wtt_script_filename_cb;
  wnavnode->format_selection_cb = wtt_format_selection;
  wnavnode->get_global_select_cb = wtt_get_global_select_cb;
  wnavnode->global_unselect_objid_cb = wtt_global_unselect_objid_cb;
  wnavnode->set_window_char_cb = wtt_set_window_char_cb;
  wnavnode->open_vsel_cb = wtt_open_vsel_cb;

  i = 0;
  XtSetArg(args[i], XmNheight, 300);i++;
  XtSetArg(args[i], XmNpaneMinimum, 100);i++;
  XtSetArg(args[i], XmNnavigationType, XmTAB_GROUP);i++;
  XtSetArg(args[i], XmNtraversalOn, True);i++;
  XtSetValues( wnavnode_brow_widget, args,i);

  palette = new Pal( this, palette_form, "Objects",
		ldhses, layout_palette,
		&palette_widget, &sts);
  palette->set_focus_cb = &wtt_set_focus_cb;
  palette->traverse_focus_cb = &wtt_traverse_focus;
  palette->create_popup_menu_cb = &wtt_create_pal_popup_menu_cb;

  i = 0;
  XtSetArg(args[i],XmNwidth, WTT_PALETTE_WIDTH);i++;
  XtSetValues( palette_widget,args,i);
    

  XtUnmanageChild( palette_form);
  XtUnmanageChild( wnavnode_brow_widget);
  XtPopup( toplevel, XtGrabNone);
//  XtRealizeWidget( toplevel);

//  XmProcessTraversal( wnav->wnav_brow_widget, XmTRAVERSE_CURRENT);
//  wnav->set_inputfocus();

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)wtt_activate_exit, this);

  wnav->get_options( &show_class, &show_alias, 
	&show_descrip, &show_objref, &show_objxref, 
	&show_attrref, &show_attrxref, &build_force, &build_debug,
	&build_crossref, &build_manual);

  if ( wbctx && volid) {
    wnav->volume_attached( wbctx, ldhses, 0);
    wnavnode->volume_attached( wbctx, ldhses, 0);
  }

  if ( wb_type == wb_eType_Directory) {
    // Start configuration wizard if volume is empty
    pwr_tOid oid;

    sts = ldh_GetRootList( ldhses, &oid);
    if ( EVEN( sts)) {
      wow_HideWarranty();  // Warranty window is hidden behind the wizard
      set_edit();
      XtAppAddWorkProc( XtWidgetToApplicationContext(toplevel),
			(XtWorkProc)wtt_start_wizard, this) ;
    }
  }

  menu_setup();
  *status = 1;
}


Wtt::~Wtt()
{
  if ( close_cb)
    (close_cb)( this);
  else
    exit(0);

  free_cursor();

  if ( utedctx)
    uted_delete( utedctx);

  if ( set_focus_disabled)
    XtRemoveTimeOut( disfocus_timerid);
  if ( selection_timerid)
    XtRemoveTimeOut( selection_timerid);

  wnav->closing_down = 1;
  wnavnode->closing_down = 1;
  delete wnav;
  delete wnavnode;
  delete palette;

  XtDestroyWidget( toplevel);
}

void Wtt::pop()
{
  flow_UnmapWidget( toplevel);
  flow_MapWidget( toplevel);
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

WttApplListElem::WttApplListElem( wb_eUtility al_type, void *al_ctx, 
	pwr_tObjid al_objid, char *al_name):
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
	pwr_tObjid objid, char *name)
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

int WttApplList::find( wb_eUtility type, char *name, void **ctx)
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

