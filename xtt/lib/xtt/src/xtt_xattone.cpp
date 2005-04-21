/* xtt_xattone.cpp -- Display object attributes

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
#include "co_lng.h"
#include "xtt_xattone.h"
#include "xtt_xattnav.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"
#include "pwr_privilege.h"


// Static member elements
char XAttOne::value_recall[30][160];

// Prototype declarations
static void xao_valchanged_cmd_input( Widget w, XEvent *event);

void XAttOne::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}

void XAttOne::set_prompt( char *prompt)
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

int XAttOne::set_value()
{
  char *text;
  int sts;
  char buff[1024];

  if ( input_open) {
    if ( input_multiline)
      text = XmTextGetString( cmd_scrolledinput);
    else
      text = XmTextGetString( cmd_input);

    sts =  xnav_attr_string_to_value( atype, text, buff, sizeof(buff), 
				      asize);
    if ( EVEN(sts)) {
      message( 'E', "Input syntax error");
      return sts;
    }
    sts = gdh_SetObjectInfoAttrref( &aref, buff, asize);
    if ( EVEN(sts)) {
      message( 'E', "Unable to set value");
      return sts;
    }
    message( ' ', "");
  }
  return XATT__SUCCESS;
}

int XAttOne::change_value( int set_focus)
{
  int		sts;
  Widget	text_w;
  char		*value = 0;
  Arg 		args[5];
  int		input_size;
  char		aval[1024];
  char 		buf[1024];
  int		len;

  sts = gdh_GetAttributeCharAttrref( &aref, &atype, &asize, &aoffs, &aelem);
  if ( EVEN(sts)) return sts;
  
  sts = gdh_GetObjectInfoAttrref( &aref, aval, sizeof(aval));
  if ( EVEN(sts)) return sts;

  xnav_attrvalue_to_string( atype, atype, &aval, buf, sizeof(buf), &len, NULL);

  value = buf;

  if ( !access_rw) {
    XmString	cstr;

    cstr=XmStringCreateLtoR( aval, "ISO8859-1");
    XtSetArg(args[0],XmNlabelString, cstr);
    XtSetValues( cmd_label, args, 1);
    XmStringFree( cstr);
  }
  else {
    if ( atype == pwr_eType_Text) {
      input_multiline = 1;
      text_w = cmd_scrolledinput;
      XtUnmanageChild( cmd_input);
      XtManageChild( text_w);
      XtManageChild( cmd_scrolled_ok);
      XtManageChild( cmd_scrolled_ca);

      // XtSetArg(args[0], XmNpaneMaximum, 300);
      // XtSetValues( xattnav_form, args, 1);

      XtSetArg(args[0], XmNmaxLength, input_size-1);
      XtSetValues( text_w, args, 1);
      if ( value) {
	XmTextSetString( text_w, value);
	//    XmTextSetInsertionPosition( text_w, strlen(value));
      }
      else
	XmTextSetString( text_w, "");

      input_multiline = 1;
    }
    else {
      input_multiline = 0;
      text_w = cmd_input;
      XtSetArg(args[0],XmNmaxLength, input_size-1);
      XtSetValues( text_w, args, 1);
      if ( value) {
	XmTextSetString( text_w, value);
	XmTextSetInsertionPosition( text_w, strlen(value));
      }
      else
	XmTextSetString( text_w, "");

      input_multiline = 0;
    }
    message( ' ', "");
    if ( set_focus)
      flow_SetInputFocus( text_w);
    set_prompt( Lng::translate("value >"));
    input_open = 1;
  }
  return XATT__SUCCESS;
}

//
//  Callbackfunctions from menu entries
//

static void xao_activate_exit( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  if ( xattone->close_cb)
    (xattone->close_cb)( xattone->parent_ctx, xattone);
  else
    delete xattone;
}

static void xao_activate_help( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void xao_create_msg_label( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  xattone->msg_label = w;
}
static void xao_create_cmd_prompt( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  xattone->cmd_prompt = w;
}
static void xao_create_cmd_label( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  xattone->cmd_label = w;
}
static void xao_create_cmd_input( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, xattone);
  XtSetValues (w, args, 1);

  xattone->cmd_input = w;
#if 0
  if ( !(xattone->is_authorized_cb) ||
       (!(xattone->is_authorized_cb) && 
	!(xattone->is_authorized_cb( xattone->parent_ctx, 
				     pwr_mAccess_RtWrite | pwr_mAccess_System))))
#endif
  mrm_TextInit( w, (XtActionProc) xao_valchanged_cmd_input, mrm_eUtility_XAttOne);
}
static void xao_create_cmd_scrolledinput( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  xattone->cmd_scrolledinput = w;
}
static void xao_create_cmd_scrolled_ok( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  xattone->cmd_scrolled_ok = w;
}
static void xao_create_cmd_scrolled_ap( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  xattone->cmd_scrolled_ap = w;
}
static void xao_create_cmd_scrolled_ca( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  xattone->cmd_scrolled_ca = w;
}

static void xao_enable_set_focus( XAttOne *xattone)
{
  xattone->set_focus_disabled--;
}

static void xao_disable_set_focus( XAttOne *xattone, int time)
{
  xattone->set_focus_disabled++;
  xattone->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( xattone->toplevel), time,
	(XtTimerCallbackProc)xao_enable_set_focus, xattone);
}

static void xao_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  XAttOne *xattone;

  XtSetArg    (args[0], XmNuserData, &xattone);
  XtGetValues (w, args, 1);

  if ( !xattone)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( xattone->set_focus_disabled)
    return;

  if ( flow_IsManaged( xattone->cmd_scrolledinput))
    flow_SetInputFocus( xattone->cmd_scrolledinput);
  else if ( flow_IsManaged( xattone->cmd_input))
    flow_SetInputFocus( xattone->cmd_input);

  xao_disable_set_focus( xattone, 400);

}

static void xao_valchanged_cmd_input( Widget w, XEvent *event)
{
  XAttOne 	*xattone;
  int 	sts;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &xattone);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)XAttOne::value_recall, sizeof(XAttOne::value_recall[0]),
	sizeof( XAttOne::value_recall)/sizeof(XAttOne::value_recall[0]),
	&xattone->value_current_recall);
  if ( sts) {
    sts = xattone->set_value();
    if ( ODD(sts)) {
      if ( xattone->close_cb)
	(xattone->close_cb)( xattone->parent_ctx, xattone);
      else
	delete xattone;
    }
  }
}

void XAttOne::change_value_close()
{
  set_value();
}

static void xao_activate_cmd_input( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
}

static void xao_activate_cmd_scrolled_ok( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  int sts;

  sts = xattone->set_value();
  if ( ODD(sts)) {
    if ( xattone->close_cb)
      (xattone->close_cb)( xattone->parent_ctx, xattone);
    else
      delete xattone;
  }
}

static void xao_activate_cmd_scrolled_ap( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  xattone->set_value();
}

static void xao_activate_cmd_scrolled_ca( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data)
{
  if ( xattone->close_cb)
    (xattone->close_cb)( xattone->parent_ctx, xattone);
  else
    delete xattone;
}

int XAttOne::open_changevalue( char *name)
{
  change_value(0);
  return XATT__SUCCESS;
}

void XAttOne::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

XAttOne::~XAttOne()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);
  XtDestroyWidget( parent_wid);
}

XAttOne::XAttOne( 
	Widget 		xa_parent_wid,
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_aref,
	char 		*xa_title,
	unsigned int	xa_priv,
        int             *xa_sts) :
 	parent_wid(xa_parent_wid), parent_ctx(xa_parent_ctx), 
	aref(*xa_aref), priv(xa_priv),
	input_open(0), input_multiline(0), 
	close_cb(0), client_data(0),
	set_focus_disabled(0), value_current_recall(0)
{
  char		uid_filename[120] = {"xtt_xattone.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: xao_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"xao_inputfocus",      (XtActionProc) xao_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "xao_ctx", 0 },
	{"xao_activate_exit",(caddr_t)xao_activate_exit },
	{"xao_activate_help",(caddr_t)xao_activate_help },
	{"xao_create_msg_label",(caddr_t)xao_create_msg_label },
	{"xao_create_cmd_prompt",(caddr_t)xao_create_cmd_prompt },
	{"xao_create_cmd_input",(caddr_t)xao_create_cmd_input },
	{"xao_create_cmd_label",(caddr_t)xao_create_cmd_label },
	{"xao_create_cmd_scrolledinput",(caddr_t)xao_create_cmd_scrolledinput },
	{"xao_create_cmd_scrolled_ok",(caddr_t)xao_create_cmd_scrolled_ok },
	{"xao_create_cmd_scrolled_ap",(caddr_t)xao_create_cmd_scrolled_ap },
	{"xao_create_cmd_scrolled_ca",(caddr_t)xao_create_cmd_scrolled_ca },
	{"xao_activate_cmd_scrolledinput",(caddr_t)xao_activate_cmd_input },
	{"xao_activate_cmd_scrolled_ok",(caddr_t)xao_activate_cmd_scrolled_ok },
	{"xao_activate_cmd_scrolled_ap",(caddr_t)xao_activate_cmd_scrolled_ap },
	{"xao_activate_cmd_scrolled_ca",(caddr_t)xao_activate_cmd_scrolled_ca }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  // for ( i = 0; i < int(sizeof(value_recall)/sizeof(value_recall[0])); i++)
  //  value_recall[i][0] = 0;

  Lng::get_uid( uid_filename, uid_filename);

  // Create object context
//  attrctx->close_cb = close_cb;
//  attrctx->redraw_cb = redraw_cb;

  // Motif
  MrmInitialize();

  *xa_sts = gdh_AttrrefToName( &aref, title, sizeof(title), cdh_mNName);
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

  sts = MrmFetchWidgetOverride( s_DRMh, "xao_window", parent_wid,
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
  XtSetArg(args[i],XmNheight,120);i++;
  XtSetValues( toplevel ,args,i);
    
  if ( priv & pwr_mPrv_RtWrite || priv & pwr_mPrv_System)
    access_rw = 1;
  else
    access_rw = 0;

  XtManageChild( toplevel);
  if ( access_rw) 
    XtUnmanageChild( cmd_label);
  else {
    XtUnmanageChild( cmd_input);
    XtUnmanageChild( cmd_scrolled_ok);
    XtUnmanageChild( cmd_scrolled_ap);
  }
  XtUnmanageChild( cmd_scrolledinput);


  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid,
	(XtCallbackProc)xao_activate_exit, this);

  change_value( 1);

  *xa_sts = XATT__SUCCESS;
}



