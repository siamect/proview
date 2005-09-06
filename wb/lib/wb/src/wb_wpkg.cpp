/* 
 * Proview   $Id: wb_wpkg.cpp,v 1.3 2005-09-06 10:43:32 claes Exp $
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

/* wb_wpkg.cpp -- package window */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include "glow_std.h"

#include <typeinfo>
#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "flow_x.h"
#include "wb_pkg_msg.h"
#include "co_mrm_util.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_wpkg.h"
#include "wb_wpkgnav.h"
#include "wb_wtt.h"
#include "wb_wnav.h"
#include "wb_pkg.h"
#include "wb_error.h"
#include "co_xhelp.h"
#include "co_wow.h"

static void wpkg_deletepkg_ok( void *ctx, void *data);

void WPkg::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}

static void wpkg_message_cb( void *wpkg, char severity, char *message)
{
  ((WPkg *)wpkg)->message( severity, message);
}

static void wpkg_set_clock_cursor_cb( void *wpkg)
{
  ((WPkg *)wpkg)->set_clock_cursor();
}

static void wpkg_reset_cursor_cb( void *wpkg)
{
  ((WPkg *)wpkg)->reset_cursor();
}

//
//  Callbackfunctions from menu entries
//
static void wpkg_activate_distribute( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  WItemPkg **itemlist;
  int item_count;
  int sts;

  sts = wpkg->wpkgnav->get_select( &itemlist, &item_count);
  if ( EVEN(sts)) {
    wpkg->message( 'E', "Select a node or package");
    return;
  }

  if ( item_count > 1) {
    for ( int i = 0; i < item_count; i++) {
      if ( typeid( *itemlist[i]) != typeid( WItemPkgNode)) {
	wpkg->message( 'E', "All selected items are not nodes");
	free( itemlist);
	return;
      }
    }
    
    wpkg->set_clock_cursor();
    for ( int i = 0; i < item_count; i++) {
      try {
	wb_pkg *pkg = new wb_pkg( ((WItemPkgNode *)itemlist[i])->nodename);
	delete pkg;
      }
      catch ( wb_error &e) {
	wpkg->message(' ', (char *)e.what().c_str());
	wpkg->reset_cursor();
	free( itemlist);
	return;
      }
      wpkg->wpkgnav->refresh_node( itemlist[i]);

      char msg[80];
      sprintf( msg, "Distribution successfull to %s", ((WItemPkgNode *)itemlist[i])->nodename);
      wpkg->message('I', msg);
      XFlush( flow_Display( wpkg->toplevel));
    }
    wpkg->reset_cursor();
    free( itemlist);
  }
  else {
    // One is selected
    if ( typeid( *itemlist[0]) == typeid( WItemPkgNode)) {
      wpkg->set_clock_cursor();
      try {
	wb_pkg *pkg = new wb_pkg( ((WItemPkgNode *)itemlist[0])->nodename);
	delete pkg;
      }
      catch ( wb_error &e) {
	wpkg->message(' ', (char *)e.what().c_str());
      }
      wpkg->wpkgnav->refresh_node( itemlist[0]);

      char msg[80];
      sprintf( msg, "Distribution successfull to %s", ((WItemPkgNode *)itemlist[0])->nodename);
      wpkg->message('I', msg);
      wpkg->reset_cursor();
    }
    else if ( typeid( *itemlist[0]) == typeid( WItemPkgPackage)) {
      wpkg->set_clock_cursor();
      wb_pkg::copyPackage( ((WItemPkgPackage *)itemlist[0])->packagename);
      char msg[80];
      sprintf( msg, "Distribution successfull of %s", ((WItemPkgPackage *)itemlist[0])->packagename);
      wpkg->message('I', msg);
      wpkg->reset_cursor();
    }
    else {
      wpkg->message( 'E', "Select a node or package");
    }
  }
}

static void wpkg_activate_createpkg( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  WItemPkg **itemlist;
  int item_count;
  int sts;

  sts = wpkg->wpkgnav->get_select( &itemlist, &item_count);
  if ( EVEN(sts)) {
    wpkg->message( 'E', "Select a node");
    return;
  }

  for ( int i = 0; i < item_count; i++) {
    if ( typeid( *itemlist[i]) != typeid( WItemPkgNode)) {
      wpkg->message( 'E', "All selected items are not nodes");
      free( itemlist);
      return;
    }
  }
    
  wpkg->set_clock_cursor();
  for ( int i = 0; i < item_count; i++) {
    try {
      wb_pkg *pkg = new wb_pkg( ((WItemPkgNode *)itemlist[i])->nodename, false);
      delete pkg;
    }
    catch ( wb_error &e) {
      wpkg->message(' ', (char *)e.what().c_str());
      wpkg->reset_cursor();
      free( itemlist);
      return;
    }
    wpkg->wpkgnav->refresh_node( itemlist[i]);

    char msg[80];
    sprintf( msg, "Package created for node %s", ((WItemPkgNode *)itemlist[i])->nodename);
    wpkg->message('I', msg);
    XFlush( flow_Display( wpkg->toplevel));
  }
  wpkg->reset_cursor();
  free( itemlist);
}

static void wpkg_activate_deletepkg( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  WItemPkg **itemlist;
  int item_count;
  int sts;

  sts = wpkg->wpkgnav->get_select( &itemlist, &item_count);
  if ( EVEN(sts)) {
    wpkg->message( 'E', "Select a package");
    return;
  }

  for ( int i = 0; i < item_count; i++) {
    if ( typeid( *itemlist[i]) != typeid( WItemPkgPackage)) {
      wpkg->message( 'E', "All selected items are not packages");
      free( itemlist);
      return;
    }
  }
  free( itemlist);
    
  wow_DisplayQuestion( wpkg, wpkg->toplevel, "Delete package", 
		      "Do you want to delete selected packages",
		      wpkg_deletepkg_ok, 0, 0);
}

static void wpkg_deletepkg_ok( void *ctx, void *data)
{
  WPkg *wpkg = (WPkg *)ctx;

  WItemPkg **itemlist;
  int item_count;
  int sts;

  sts = wpkg->wpkgnav->get_select( &itemlist, &item_count);
  if ( EVEN(sts)) {
    wpkg->message( 'E', "Select a package");
    return;
  }

  for ( int i = 0; i < item_count; i++) {
    if ( typeid( *itemlist[i]) != typeid( WItemPkgPackage)) {
      wpkg->message( 'E', "All selected items are not packages");
      free( itemlist);
      return;
    }
  }
  WItemPkg *node_item = wpkg->wpkgnav->get_parent( itemlist[0]);

  wpkg->set_clock_cursor();
  for ( int i = 0; i < item_count; i++) {
    wpkg->wpkgnav->delete_package( (WItemPkgPackage *)itemlist[i]);
  }
  wpkg->wpkgnav->refresh_node( node_item);
  wpkg->reset_cursor();
  free( itemlist);
}

static void wpkg_activate_dmode_filediff( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data)
{  
  if ( data->set) {
    wpkg->display_mode |= wpkg_mDisplayMode_FileDiff;
    wpkg->wpkgnav->set_display_mode( wpkg->display_mode);
  }
  else {
    wpkg->display_mode &= ~wpkg_mDisplayMode_FileDiff;
    wpkg->wpkgnav->set_display_mode( wpkg->display_mode);
  }
}

static void wpkg_activate_dmode_filetime( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data)
{  
  if ( data->set) {
    wpkg->display_mode |= wpkg_mDisplayMode_FileOrderTime;
    wpkg->wpkgnav->set_display_mode( wpkg->display_mode);
  }
  else {
    wpkg->display_mode &= ~wpkg_mDisplayMode_FileOrderTime;
    wpkg->wpkgnav->set_display_mode( wpkg->display_mode);
  }
}

static void wpkg_activate_dmode_filepath( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data)
{  
  if ( data->set) {
    wpkg->display_mode |= wpkg_mDisplayMode_FilePath;
    wpkg->wpkgnav->set_display_mode( wpkg->display_mode);
  }
  else {
    wpkg->display_mode &= ~wpkg_mDisplayMode_FilePath;
    wpkg->wpkgnav->set_display_mode( wpkg->display_mode);
  }
}

static void wpkg_activate_exit( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  if ( wpkg->close_cb)
    (wpkg->close_cb)( wpkg->parent_ctx);
  else
    delete wpkg;
}

static void wpkg_activate_help( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  CoXHelp::dhelp( "package window", 0, navh_eHelpFile_Base, 0, true);
}

static void wpkg_create_msg_label( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  wpkg->msg_label = w;
}
static void wpkg_create_wpkgnav_form( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  wpkg->wpkgnav_form = w;
}

static void wpkg_enable_set_focus( WPkg *wpkg)
{
  wpkg->set_focus_disabled--;
}

static void wpkg_disable_set_focus( WPkg *wpkg, int time)
{
  wpkg->set_focus_disabled++;
  wpkg->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( wpkg->toplevel), time,
	(XtTimerCallbackProc)wpkg_enable_set_focus, wpkg);
}

static void wpkg_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  WPkg *wpkg;

  XtSetArg    (args[0], XmNuserData, &wpkg);
  XtGetValues (w, args, 1);

  if ( !wpkg)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( wpkg->set_focus_disabled)
    return;

  ((WPkgNav *)wpkg->wpkgnav)->set_inputfocus();

  wpkg_disable_set_focus( wpkg, 400);

}

void WPkg::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

void WPkg::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = XCreateFontCursor( flow_Display(toplevel), XC_watch);

  XDefineCursor( flow_Display(toplevel), flow_Window(toplevel), clock_cursor);
  XFlush( flow_Display(toplevel));
}

void WPkg::reset_cursor()
{
  XUndefineCursor( flow_Display(toplevel), flow_Window(toplevel));
}

WPkg::~WPkg()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  delete (WPkgNav *)wpkgnav;
  XtDestroyWidget( parent_wid);
}

WPkg::WPkg( 
	Widget 		wa_parent_wid,
	void 		*wa_parent_ctx)
  : parent_wid(wa_parent_wid), parent_ctx(wa_parent_ctx), 
  close_cb(0), set_focus_disabled(0), display_mode(0), clock_cursor(0)
{
  char		uid_filename[120] = {"pwr_exe:wb_wpkg.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: wpkg_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"wpkg_inputfocus",      (XtActionProc) wpkg_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "wpkg_ctx", 0 },
	{"wpkg_activate_exit",(caddr_t)wpkg_activate_exit },
	{"wpkg_activate_distribute",(caddr_t)wpkg_activate_distribute },
	{"wpkg_activate_createpkg",(caddr_t)wpkg_activate_createpkg },
	{"wpkg_activate_deletepkg",(caddr_t)wpkg_activate_deletepkg },
	{"wpkg_activate_dmode_filediff",(caddr_t)wpkg_activate_dmode_filediff },
	{"wpkg_activate_dmode_filetime",(caddr_t)wpkg_activate_dmode_filetime },
	{"wpkg_activate_dmode_filepath",(caddr_t)wpkg_activate_dmode_filepath },
	{"wpkg_activate_help",(caddr_t)wpkg_activate_help },
	{"wpkg_create_msg_label",(caddr_t)wpkg_create_msg_label },
	{"wpkg_create_wpkgnav_form",(caddr_t)wpkg_create_wpkgnav_form }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  dcli_translate_filename( uid_filename, uid_filename);

  // Motif
  MrmInitialize();

  strcpy( title, "PwR Package distributor");

  reglist[0].value = (caddr_t) this;

  // Save the context structure in the widget
  i = 0;
  XtSetArg (args[i], XmNuserData, (unsigned int) this);i++;
  XtSetArg( args[i], XmNdeleteResponse, XmDO_NOTHING);i++;

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell("PwR Distributor", 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "wpkg_window", parent_wid,
			name, args, 1, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);


  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(toplevel), 
		     actions, XtNumber(actions));
 
  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( toplevel, compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,600);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);

  utility = *(wb_eUtility *)parent_ctx;
  wpkgnav = new WPkgNav( (void *)this, wpkgnav_form, "Packages",
		utility, &brow_widget, &sts);
  ((WPkgNav *)wpkgnav)->message_cb = &wpkg_message_cb;
  ((WPkgNav *)wpkgnav)->set_clock_cursor_cb = &wpkg_set_clock_cursor_cb;
  ((WPkgNav *)wpkgnav)->reset_cursor_cb = &wpkg_reset_cursor_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)wpkg_activate_exit, this);

  if ( utility == wb_eUtility_Wtt)
  {
    ((Wtt *)parent_ctx)->register_utility( (void *) this,
	wb_eUtility_Distributor);
  }
}
