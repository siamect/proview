/* co_xhelp.cpp -- Display object attributes

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

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "flow_x.h"
#include "co_mrm_util.h"
}

#include "co_lng.h"
#include "co_xhelpnav.h"
#include "co_xhelp.h"

XHelp *XHelp::default_xhelp = 0;

// Prototype declarations

static void xhelp_open_input_dialog( XHelp *xhelp, char *text, char *title,
	char *init_text,
	void (*ok_cb)( XHelp *, char *))
{
  Arg		args[10];
  int 		i;
  XmString	cstr;

  i = 0;
  XtSetArg(args[0], XmNlabelString,
		cstr=XmStringCreateLtoR( text, "ISO8859-1") ); i++;
  XtSetValues( xhelp->india_label, args, i);
  XmStringFree( cstr);
  i = 0;
  XtSetArg(args[0], XmNdialogTitle,
		cstr=XmStringCreateLtoR( title, "ISO8859-1") ); i++;
  XtSetValues( xhelp->india_widget, args, i);
  XmStringFree( cstr);

  XmTextSetString( xhelp->india_text, init_text);

  XtManageChild( xhelp->india_widget);

  XmProcessTraversal( xhelp->india_text, XmTRAVERSE_CURRENT);

  xhelp->india_ok_cb = ok_cb;
}

static void xhelp_find_ok( XHelp *xhelp, char *search_str)
{
  xhelp->xhelpnav->search( search_str, false);
}

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

  xhelp->xhelpnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  xhelp->xhelpnav->zoom( 1.18);
}

static void xhelp_activate_zoom_out( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  xhelp->xhelpnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  xhelp->xhelpnav->zoom( 1.0 / 1.18);
}

static void xhelp_activate_zoom_reset( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  xhelp->xhelpnav->unzoom();
}

static void xhelp_activate_search( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  xhelp_open_input_dialog( xhelp, "Search string", "Search string",
	"", &xhelp_find_ok);
}

static void xhelp_activate_searchnext( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  xhelp->xhelpnav->search_next();
}

static void xhelp_activate_searchprevious( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  xhelp->xhelpnav->search_next_reverse();
}

static void xhelp_create_india_label( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  xhelp->india_label = w;
}
static void xhelp_create_india_text( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  xhelp->india_text = w;
}
static void xhelp_activate_india_ok( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  char *value;

  value = XmTextGetString( xhelp->india_text);
  XtUnmanageChild( xhelp->india_widget);

  (xhelp->india_ok_cb)( xhelp, value);
}
static void xhelp_activate_india_cancel( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( xhelp->india_widget);
}

static void xhelp_activate_help( Widget w, XHelp *xhelp, XmAnyCallbackStruct *data)
{
  XHelp::dhelp( "helpwindow", 0, navh_eHelpFile_Base, 0, true);
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
    xhelp->xhelpnav->set_inputfocus();
  }
  xhelp_disable_set_focus( xhelp, 400);

}


void XHelp::pop()
{
  ((XHelpNav*)xhelpnav)->pop();
  displayed = 1;
}

void XHelp::clear()
{
  xhelpnav->brow_push_all();
  xhelpnav->clear();
}

int XHelp::help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, bool strict)
{
  return xhelpnav->help( key, help_bookmark, file_type,
				   file_name, 0, strict);
}

//
//  Static function to call help for the default xhelp
//
int XHelp::dhelp( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, bool strict)
{
  if ( !default_xhelp)
    return 0;

  default_xhelp->clear();
  default_xhelp->pop();
  return default_xhelp->help( key, help_bookmark, file_type,
			 file_name, strict);
}


int XHelp::help_index( navh_eHelpFile file_type, char *file_name)
{
  return xhelpnav->help_index( file_type, file_name, 0);
}

int XHelp::dhelp_index( navh_eHelpFile file_type, char *file_name)
{
  if ( !default_xhelp)
    return 0;

  default_xhelp->clear();
  default_xhelp->pop();
  return default_xhelp->help_index( file_type, file_name);
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

  delete xhelpnav;
  XtDestroyWidget( parent_wid);
}

XHelp::XHelp( 
	Widget 		xa_parent_wid,
	void 		*xa_parent_ctx,
	xhelp_eUtility	utility,
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
	{"xhelp_activate_search",(caddr_t)xhelp_activate_search },
	{"xhelp_activate_searchnext",(caddr_t)xhelp_activate_searchnext },
	{"xhelp_activate_searchprevious",(caddr_t)xhelp_activate_searchprevious },
	{"xhelp_activate_india_ok",(caddr_t)xhelp_activate_india_ok },
	{"xhelp_activate_india_cancel",(caddr_t)xhelp_activate_india_cancel },
	{"xhelp_create_india_label",(caddr_t)xhelp_create_india_label },
	{"xhelp_create_india_text",(caddr_t)xhelp_create_india_text },
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

  sts = MrmFetchWidget(s_DRMh, "input_dialog", toplevel,
		&india_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

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

  xhelpnav = new XHelpNav( (void *)this, xhelpnav_form, title, utility, &brow_widget, 
		       &sts);

  // XtPopup( parent_wid, XtGrabNone);
  // displayed = 1;
  XtRealizeWidget( parent_wid);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid,
			     (XtCallbackProc)xhelp_activate_exit, this);

  *xa_sts = 1;
}






