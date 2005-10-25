/* 
 * Proview   $Id: xtt_xcrr.cpp,v 1.4 2005-10-25 15:28:10 claes Exp $
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

/* xtt_xcrr.cpp -- Display object crossreferences */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "flow_x.h"
#include "rt_xnav_msg.h"
#include "co_mrm_util.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "xtt_xcrr.h"
#include "xtt_xattnav.h"
#include "co_lng.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"


// Prototype declarations
static void xcrr_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			      unsigned long item_type, unsigned long utility, 
			      char *arg, Widget *popup);
static void xcrr_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);
static void xcrr_close_cb( void *ctx);

static void xcrr_activate_exit( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data)
{
  if ( xcrr->close_cb)
    (xcrr->close_cb)( xcrr->parent_ctx, (void *)xcrr);
  else
    delete xcrr;
}

static void xcrr_activate_openplc( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data)
{
  ((XAttNav *)xcrr->xcrrnav)->start_trace();
}

static void xcrr_activate_help( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void xcrr_create_xcrrnav_form( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data)
{
  xcrr->xcrrnav_form = w;
}

static void xcrr_enable_set_focus( XCrr *xcrr)
{
  xcrr->set_focus_disabled--;
}

static void xcrr_disable_set_focus( XCrr *xcrr, int time)
{
  xcrr->set_focus_disabled++;
  xcrr->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( xcrr->toplevel), time,
	(XtTimerCallbackProc)xcrr_enable_set_focus, xcrr);
}

static void xcrr_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  XCrr *xcrr;

  XtSetArg    (args[0], XmNuserData, &xcrr);
  XtGetValues (w, args, 1);

  if ( !xcrr)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( xcrr->set_focus_disabled)
    return;

  if ( xcrr->xcrrnav)
    ((XAttNav *)xcrr->xcrrnav)->set_inputfocus();

  xcrr_disable_set_focus( xcrr, 400);

}

void XCrr::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

XCrr::~XCrr()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  delete (XAttNav *)xcrrnav;
  XtDestroyWidget( parent_wid);
}

XCrr::XCrr( 
	Widget 		xa_parent_wid,
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
        int             *xa_sts) :
 	parent_wid(xa_parent_wid), parent_ctx(xa_parent_ctx), 
	objar(*xa_objar), 
	input_open(0), input_multiline(0), 
	close_cb(0), redraw_cb(0), popup_menu_cb(0), start_trace_cb(0),
	client_data(0), set_focus_disabled(0)
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
    {"xcrr_inputfocus",      (XtActionProc) xcrr_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "xcrr_ctx", 0 },
	{"xcrr_activate_exit",(caddr_t)xcrr_activate_exit },
	{"xcrr_activate_openplc",(caddr_t)xcrr_activate_openplc },
	{"xcrr_activate_help",(caddr_t)xcrr_activate_help },
	{"xcrr_create_xcrrnav_form",(caddr_t)xcrr_create_xcrrnav_form }
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
  XtSetArg (args[i], XmNuserData, (unsigned int) this);i++;
  XtSetArg( args[i], XmNdeleteResponse, XmDO_NOTHING);i++;

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell( title, 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "xcrr_window", parent_wid,
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

  xcrrnav = new XAttNav( (void *)this, xcrrnav_form, xattnav_eType_CrossRef,
	        "Plant", &objar, xa_advanced_user, &brow_widget, &sts);
  ((XAttNav *)xcrrnav)->popup_menu_cb = &xcrr_popup_menu_cb;
  ((XAttNav *)xcrrnav)->start_trace_cb = &xcrr_start_trace_cb;
  ((XAttNav *)xcrrnav)->close_cb = &xcrr_close_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid,
	(XtCallbackProc)xcrr_activate_exit, this);

  *xa_sts = XATT__SUCCESS;
}


static void xcrr_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			      unsigned long item_type, unsigned long utility,
			      char *arg, Widget *popup)
{
  if ( ((XCrr *)ctx)->popup_menu_cb)
    (((XCrr *)ctx)->popup_menu_cb) ( ((XCrr *)ctx)->parent_ctx, attrref,
				   item_type, utility, arg, popup);
}

static void xcrr_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((XCrr *)ctx)->start_trace_cb)
    ((XCrr *)ctx)->start_trace_cb( ((XCrr *)ctx)->parent_ctx, objid, name);
}

static void xcrr_close_cb( void *ctx)
{
  if ( ((XCrr *)ctx)->close_cb)
    ((XCrr *)ctx)->close_cb( ((XCrr *)ctx)->parent_ctx, ctx);
}




