/* xtt_xhelp.cpp -- Display object attributes

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
#include <Xm/Protocols.h>
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

#include "co_lng.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "xtt_xnav.h"
#include "xtt_xhelp.h"
#include "rt_xatt_msg.h"


// Prototype declarations

static void xhelp_activate_exit( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  if ( xhelp->close_cb)
    (xhelp->close_cb)( xhelp->parent_ctx, (void *)xhelp);
  else {
    flow_UnmapWidget( xhelp->parent_wid);
    xhelp->displayed = 0;
  }
  //  delete xhelp;
}

static void xhelp_activate_zoom_in( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  ((XNav *)xhelp->xhelpnav)->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

   ((XNav *)xhelp->xhelpnav)->zoom( 1.18);
}

static void xhelp_activate_zoom_out( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  ((XNav *)xhelp->xhelpnav)->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  ((XNav *)xhelp->xhelpnav)->zoom( 1.0 / 1.18);
}

static void xhelp_activate_zoom_reset( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  ((XNav *)xhelp->xhelpnav)->unzoom();
}

static void xhelp_activate_help( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void xhelp_create_xhelpnav_form( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  xhelp->xhelpnav_form = w;
}

static void xhelp_enable_set_focus( XHelp *xhelp)
{
  xhelp->set_focus_disabled--;
}

static void xhelp_disable_set_focus( XHelp *xhelp, int time)
{
  xhelp->set_focus_disabled++;
  xhelp->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( xhelp->toplevel), time,
	(XtTimerCallbackProc)xhelp_enable_set_focus, xhelp);
}

static void xhelp_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  XHelp *xhelp;

  XtSetArg    (args[0], XmNuserData, &xhelp);
  XtGetValues (w, args, 1);

  if ( !xhelp)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( xhelp->set_focus_disabled)
    return;

  if ( xhelp->xhelpnav && xhelp->displayed) {
    ((XNav *)xhelp->xhelpnav)->set_inputfocus();
  }
  xhelp_disable_set_focus( xhelp, 400);

}

void XHelp::pop()
{
  ((XNav*)xhelpnav)->pop();
  displayed = 1;
}

void XHelp::clear()
{
  ((XNav *)xhelpnav)->brow_push_all();
  ((XNav *)xhelpnav)->clear();
}

int XHelp::help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name)
{
  return ((XNav *)xhelpnav)->help( key, help_bookmark, file_type,
				   file_name, 0);
}

int XHelp::help_index( navh_eHelpFile file_type, char *file_name)
{
  return ((XNav *)xhelpnav)->help_index( file_type, file_name, 0);
}

void XHelp::set_dimension( int width, int height)
{
  Arg 		args[3];

  int i = 0;
  if ( width) {
    XtSetArg( args[i], XmNwidth, width);i++;
  }
  if ( height) {
    XtSetArg( args[i], XmNheight, height);i++;
  }
  XtSetValues( toplevel, args,i);
}

XHelp::~XHelp()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  //delete (XNav *)xhelpnav;
  XtDestroyWidget( parent_wid);
}

XHelp::XHelp( 
	Widget 		xa_parent_wid,
	void 		*xa_parent_ctx, 
        int             *xa_sts) :
 	parent_wid(xa_parent_wid), parent_ctx(xa_parent_ctx), 
	client_data(0), close_cb(0), set_focus_disabled(0), 
	displayed(0)
{
  char		uid_filename[120] = {"xtt_xhelp.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: xhelp_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"xhelp_inputfocus",      (XtActionProc) xhelp_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "xhelp_ctx", 0 },
	{"xhelp_activate_exit",(caddr_t)xhelp_activate_exit },
	{"xhelp_activate_zoom_in",(caddr_t)xhelp_activate_zoom_in },
	{"xhelp_activate_zoom_out",(caddr_t)xhelp_activate_zoom_out },
	{"xhelp_activate_zoom_reset",(caddr_t)xhelp_activate_zoom_reset },
	{"xhelp_activate_help",(caddr_t)xhelp_activate_help },
	{"xhelp_create_xhelpnav_form",(caddr_t)xhelp_create_xhelpnav_form }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  Lng::get_uid( uid_filename, uid_filename);

  // Motif
  MrmInitialize();

  strcpy( title, Lng::translate("Help"));
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

  sts = MrmFetchWidgetOverride( s_DRMh, "xhelp_window", parent_wid,
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
  XtSetArg(args[i],XmNwidth,500);i++;
  XtSetArg(args[i],XmNheight,700);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);

  xhelpnav = new XNav( (void *)this, xhelpnav_form, title, &brow_widget, 
		       NULL, "", &sts);

  XtPopup( parent_wid, XtGrabNone);
  displayed = 1;

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid,
			     (XtCallbackProc)xhelp_activate_exit, this);

  *xa_sts = XATT__SUCCESS;
}






