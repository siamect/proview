/* 
 * Proview   $Id: xtt_xcrr_motif.cpp,v 1.2 2008-10-31 12:51:36 claes Exp $
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

/* xtt_xcrr_motif.cpp -- Display object crossreferences */

#include "glow_std.h"
#include "pwr.h"
#include "pwr_baseclasses.h"

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "flow_x.h"
#include "rt_xnav_msg.h"
#include "cow_mrm_util.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "xtt_xcrr_motif.h"
#include "xtt_xattnav_motif.h"
#include "co_lng.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"


void XCrrMotif::activate_exit( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data)
{
  if ( xcrr->close_cb)
    (xcrr->close_cb)( xcrr->parent_ctx, (void *)xcrr);
  else
    delete xcrr;
}

void XCrrMotif::activate_openplc( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data)
{
  xcrr->xcrrnav->start_trace();
}

void XCrrMotif::activate_help( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

void XCrrMotif::create_xcrrnav_form( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data)
{
  ((XCrrMotif *)xcrr)->xcrrnav_form = w;
}

void XCrrMotif::enable_set_focus( XCrrMotif *xcrr)
{
  xcrr->set_focus_disabled--;
}

void XCrrMotif::disable_set_focus( XCrrMotif *xcrr, int time)
{
  xcrr->set_focus_disabled++;
  xcrr->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( xcrr->toplevel), time,
	(XtTimerCallbackProc)enable_set_focus, xcrr);
}

void XCrrMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  XCrrMotif *xcrr;

  XtSetArg    (args[0], XmNuserData, &xcrr);
  XtGetValues (w, args, 1);

  if ( !xcrr)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( xcrr->set_focus_disabled)
    return;

  if ( xcrr->xcrrnav)
    xcrr->xcrrnav->set_inputfocus();

  disable_set_focus( xcrr, 400);

}

void XCrrMotif::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

XCrrMotif::~XCrrMotif()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  delete xcrrnav;
  XtDestroyWidget( parent_wid);
}

XCrrMotif::XCrrMotif( 
	Widget 		xa_parent_wid,
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
        int             *xa_sts) :
  XCrr( xa_parent_ctx, xa_objar, xa_advanced_user, xa_sts),
  parent_wid(xa_parent_wid), set_focus_disabled(0)
{
  char		uid_filename[120] = {"xtt_xcrr.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  pwr_tAName   	title;
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: xcrr_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {(char*) "xcrr_inputfocus",      (XtActionProc) action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        {(char*) "xcrr_ctx", 0 },
	{(char*) "xcrr_activate_exit",(caddr_t)activate_exit },
	{(char*) "xcrr_activate_openplc",(caddr_t)activate_openplc },
	{(char*) "xcrr_activate_help",(caddr_t)activate_help },
	{(char*) "xcrr_create_xcrrnav_form",(caddr_t)create_xcrrnav_form }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  Lng::get_uid( uid_filename, uid_filename);

  // Motif
  MrmInitialize();

  *xa_sts = gdh_AttrrefToName( &objar, title, sizeof(title), cdh_mNName);
  if ( EVEN(*xa_sts)) return;

  reglist[0].value = (caddr_t) this;

  // Save the context structure in the widget
  i = 0;
  XtSetArg (args[i], XmNuserData, (XtPointer) this);i++;
  XtSetArg( args[i], XmNdeleteResponse, XmDO_NOTHING);i++;

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell( title, 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "xcrr_window", parent_wid,
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
  XtSetArg(args[i],XmNwidth,420);i++;
  XtSetArg(args[i],XmNheight,300);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);

  xcrrnav = new XAttNavMotif( (void *)this, xcrrnav_form, xattnav_eType_CrossRef,
	        "Plant", &objar, xa_advanced_user, &brow_widget, &sts);
  xcrrnav->popup_menu_cb = &xcrr_popup_menu_cb;
  xcrrnav->start_trace_cb = &xcrr_start_trace_cb;
  xcrrnav->close_cb = &xcrr_close_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid,
	(XtCallbackProc)activate_exit, this);

  *xa_sts = XATT__SUCCESS;
}


