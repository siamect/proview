/* 
 * Proview   $Id: wb_wpkg_motif.cpp,v 1.2 2008-10-31 12:51:31 claes Exp $
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

/* wb_wpkg_motif.cpp -- package window */

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

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "cow_wow_motif.h"
#include "flow_x.h"
#include "wb_pkg_msg.h"
#include "cow_mrm_util.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_wpkg_motif.h"
#include "wb_wpkgnav_motif.h"
#include "wb_wtt.h"
#undef min
#undef max
#include "wb_wnav.h"
#include "wb_pkg.h"
#include "wb_error.h"
#include "cow_xhelp.h"

void WPkgMotif::message( char severity, const char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( (char*) message, (char*) "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}

//
//  Callbackfunctions from menu entries
//
void WPkgMotif::activate_distribute( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  wpkg->activate_distribute();
}

void WPkgMotif::activate_createpkg( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  wpkg->activate_createpkg();
}


void WPkgMotif::activate_deletepkg( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  wpkg->activate_deletepkg();
}

void WPkgMotif::activate_dmode_filediff( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data)
{  
  wpkg->activate_dmode_filediff( data->set);
}

void WPkgMotif::activate_dmode_filetime( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data)
{  
  wpkg->activate_dmode_filetime( data->set);
}

void WPkgMotif::activate_dmode_filepath( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data)
{  
  wpkg->activate_dmode_filepath( data->set);
}

void WPkgMotif::activate_exit( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  if ( wpkg->close_cb)
    (wpkg->close_cb)( wpkg->parent_ctx);
  else
    delete wpkg;
}

void WPkgMotif::activate_help( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  CoXHelp::dhelp( "package window", 0, navh_eHelpFile_Base, 0, true);
}

void WPkgMotif::create_msg_label( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  ((WPkgMotif *)wpkg)->msg_label = w;
}
void WPkgMotif::create_wpkgnav_form( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data)
{
  ((WPkgMotif *)wpkg)->wpkgnav_form = w;
}

static void wpkg_enable_set_focus( WPkgMotif *wpkg)
{
  wpkg->set_focus_disabled--;
}

static void wpkg_disable_set_focus( WPkgMotif *wpkg, int time)
{
  wpkg->set_focus_disabled++;
  wpkg->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( wpkg->toplevel), time,
	(XtTimerCallbackProc)wpkg_enable_set_focus, wpkg);
}

void WPkgMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  WPkgMotif *wpkg;

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

void WPkgMotif::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

void WPkgMotif::flush()
{
  XFlush( flow_Display( toplevel));
}

void WPkgMotif::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = XCreateFontCursor( flow_Display(toplevel), XC_watch);

  XDefineCursor( flow_Display(toplevel), flow_Window(toplevel), clock_cursor);
  XFlush( flow_Display(toplevel));
}

void WPkgMotif::reset_cursor()
{
  XUndefineCursor( flow_Display(toplevel), flow_Window(toplevel));
}

WPkgMotif::~WPkgMotif()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  delete wow;
  delete (WPkgNav *)wpkgnav;
  XtDestroyWidget( parent_wid);
}

WPkgMotif::WPkgMotif( 
	Widget 		wa_parent_wid,
	void 		*wa_parent_ctx)
  : WPkg(wa_parent_ctx), parent_wid(wa_parent_wid),
  set_focus_disabled(0), clock_cursor(0)
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
    {(char*) "wpkg_inputfocus",      (XtActionProc) action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        {(char*) "wpkg_ctx", 0 },
	{(char*) "wpkg_activate_exit",(caddr_t)activate_exit },
	{(char*) "wpkg_activate_distribute",(caddr_t)activate_distribute },
	{(char*) "wpkg_activate_createpkg",(caddr_t)activate_createpkg },
	{(char*) "wpkg_activate_deletepkg",(caddr_t)activate_deletepkg },
	{(char*) "wpkg_activate_dmode_filediff",(caddr_t)activate_dmode_filediff },
	{(char*) "wpkg_activate_dmode_filetime",(caddr_t)activate_dmode_filetime },
	{(char*) "wpkg_activate_dmode_filepath",(caddr_t)activate_dmode_filepath },
	{(char*) "wpkg_activate_help",(caddr_t)activate_help },
	{(char*) "wpkg_create_msg_label",(caddr_t)create_msg_label },
	{(char*) "wpkg_create_wpkgnav_form",(caddr_t)create_wpkgnav_form }
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

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "wpkg_window", parent_wid,
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

  utility = ((WUtility *)parent_ctx)->utype;
  wpkgnav = new WPkgNavMotif( (void *)this, wpkgnav_form, "Packages",
		utility, &brow_widget, &sts);
  ((WPkgNav *)wpkgnav)->message_cb = message_cb;
  ((WPkgNav *)wpkgnav)->set_clock_cursor_cb = set_clock_cursor_cb;
  ((WPkgNav *)wpkgnav)->reset_cursor_cb = reset_cursor_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)activate_exit, this);

  wow = new CoWowMotif( toplevel);

  if ( utility == wb_eUtility_Wtt) {
    ((Wtt *)parent_ctx)->register_utility( (void *) this,
	wb_eUtility_Distributor);
  }
}


