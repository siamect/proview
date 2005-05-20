/* xtt_xatt.cpp -- Display object attributes

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
#include "xtt_xatt.h"
#include "xtt_xattnav.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"


// Static member elements
char XAtt::value_recall[30][160];

// Prototype declarations
static void xatt_valchanged_cmd_input( Widget w, XEvent *event);
static void xatt_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			      unsigned long item_type, unsigned long utility,
			      char *arg, Widget *popup);
static int xatt_is_authorized_cb( void *ctx, unsigned int access);

void XAtt::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}

void XAtt::set_prompt( char *prompt)
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

void XAtt::change_value( int set_focus)
{
  int		sts;
  Widget	text_w;
  int		multiline;
  char		*value;
  Arg 		args[5];
  int		input_size;

  if ( input_open) 
  {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = ((XAttNav *)xattnav)->check_attr( &multiline, &input_node, input_name,
		&value, &input_size);
  if ( EVEN(sts))
  {
    if ( sts == XATT__NOATTRSEL)
      message( 'E', "No attribute is selected");
    else
      message( 'E', XNav::get_message( sts));
    return;
  }

  if ( multiline)
  {
    text_w = cmd_scrolledinput;
    XtManageChild( text_w);
    XtManageChild( cmd_scrolled_ok);
    XtManageChild( cmd_scrolled_ca);

    // XtSetArg(args[0], XmNpaneMaximum, 300);
    // XtSetValues( xattnav_form, args, 1);

    XtSetArg(args[0], XmNmaxLength, input_size-1);
    XtSetValues( text_w, args, 1);
    if ( value)
    {
      XmTextSetString( text_w, value);
//    XmTextSetInsertionPosition( text_w, strlen(value));
    }
    else
      XmTextSetString( text_w, "");

    input_multiline = 1;
  }
  else
  {
    text_w = cmd_input;
    XtManageChild( text_w);
    XtSetArg(args[0],XmNmaxLength, input_size-1);
    XtSetValues( text_w, args, 1);
    if ( value)
    {
      XmTextSetString( text_w, value);
      XmTextSetInsertionPosition( text_w, strlen(value));
    }
    else
      XmTextSetString( text_w, "");

    input_multiline = 0;
  }

  if ( value)
    XtFree( value);

  message( ' ', "");
  if ( set_focus)
    flow_SetInputFocus( text_w);
  set_prompt( Lng::translate("value >"));
  input_open = 1;
}

static void xatt_message_cb( void *xatt, char severity, char *message)
{
  ((XAtt *)xatt)->message( severity, message);
}

static void xatt_change_value_cb( void *xatt)
{
  ((XAtt *)xatt)->change_value(1);
}

//
//  Callbackfunctions from menu entries
//
static void xatt_activate_change_value( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->change_value(1);
}

static void xatt_activate_close_changeval( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->change_value_close();
}

static void xatt_activate_exit( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  if ( xatt->close_cb)
    (xatt->close_cb)( xatt->parent_ctx, (void *)xatt);
  else
    delete xatt;
}

static void xatt_activate_display_object( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{

  if ( xatt->call_method_cb)
  {
    (xatt->call_method_cb)(xatt->parent_ctx,
			     "$Object-RtNavigator",
			     "$Object-RtNavigatorFilter",
			     xatt->objar, 
			     xmenu_eItemType_Object,
			     xmenu_mUtility_AttrEditor, NULL);
  }
}

static void xatt_activate_show_cross( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{

  if ( xatt->call_method_cb)
  {
    (xatt->call_method_cb)(xatt->parent_ctx,
			     "$Object-OpenCrossref",
			     "$Object-OpenCrossrefFilter",
			     xatt->objar,
			     xmenu_eItemType_Object,
			     xmenu_mUtility_AttrEditor, NULL);
  }
}

static void xatt_activate_open_classgraph( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{

  if ( xatt->call_method_cb)
  {
    (xatt->call_method_cb)(xatt->parent_ctx,
			     "$Object-OpenObjectGraph",
			     "$Object-OpenObjectGraphFilter",
			     xatt->objar, 
			     xmenu_eItemType_Object,
			     xmenu_mUtility_AttrEditor, NULL);
  }
}

static void xatt_activate_open_plc( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{

  if ( xatt->call_method_cb)
  {
    (xatt->call_method_cb)(xatt->parent_ctx,
			     "$Object-OpenTrace",
			     "$Object-OpenTraceFilter",
			     xatt->objar, 
			     xmenu_eItemType_Object,
			     xmenu_mUtility_AttrEditor, NULL);
  }
}

static void xatt_activate_help( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void xatt_create_msg_label( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->msg_label = w;
}
static void xatt_create_cmd_prompt( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->cmd_prompt = w;
}
static void xatt_create_cmd_input( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, xatt);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) xatt_valchanged_cmd_input, mrm_eUtility_XAtt);
  xatt->cmd_input = w;
}
static void xatt_create_cmd_scrolledinput( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->cmd_scrolledinput = w;
}
static void xatt_create_cmd_scrolled_ok( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->cmd_scrolled_ok = w;
}
static void xatt_create_cmd_scrolled_ca( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->cmd_scrolled_ca = w;
}
static void xatt_create_xattnav_form( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->xattnav_form = w;
}

static void xatt_enable_set_focus( XAtt *xatt)
{
  xatt->set_focus_disabled--;
}

static void xatt_disable_set_focus( XAtt *xatt, int time)
{
  xatt->set_focus_disabled++;
  xatt->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( xatt->toplevel), time,
	(XtTimerCallbackProc)xatt_enable_set_focus, xatt);
}

static void xatt_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  XAtt *xatt;

  XtSetArg    (args[0], XmNuserData, &xatt);
  XtGetValues (w, args, 1);

  if ( !xatt)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( xatt->set_focus_disabled)
    return;

  if ( flow_IsManaged( xatt->cmd_scrolledinput))
    flow_SetInputFocus( xatt->cmd_scrolledinput);
  else if ( flow_IsManaged( xatt->cmd_input))
    flow_SetInputFocus( xatt->cmd_input);
  else if ( xatt->xattnav)
    ((XAttNav *)xatt->xattnav)->set_inputfocus();

  xatt_disable_set_focus( xatt, 400);

}

static void xatt_valchanged_cmd_input( Widget w, XEvent *event)
{
  XAtt 	*xatt;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &xatt);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)XAtt::value_recall, sizeof(XAtt::value_recall[0]),
	sizeof( XAtt::value_recall)/sizeof(XAtt::value_recall[0]),
	&xatt->value_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( xatt->input_open)
    {
      sts = ((XAttNav *)xatt->xattnav)->set_attr_value( xatt->input_node, 
		xatt->input_name, text);
      XtUnmanageChild( w);
      xatt->set_prompt( "");
      xatt->input_open = 0;
      if ( xatt->redraw_cb)
        (xatt->redraw_cb)( xatt);

      ((XAttNav *)xatt->xattnav)->set_inputfocus();
    }
  }
}

void XAtt::change_value_close()
{
  char *text;
  int sts;

  text = XmTextGetString( cmd_scrolledinput);
  if ( input_open) {
    if ( input_multiline) {
      sts = ((XAttNav *)xattnav)->set_attr_value( input_node,
		input_name, text);
      XtUnmanageChild( cmd_scrolledinput);
      XtUnmanageChild( cmd_scrolled_ok);
      XtUnmanageChild( cmd_scrolled_ca);
      set_prompt( "");
      input_open = 0;

      ((XAttNav *)xattnav)->redraw();
      ((XAttNav *)xattnav)->set_inputfocus();
    }
    else {
      text = XmTextGetString( cmd_input);

      sts = ((XAttNav *)xattnav)->set_attr_value( input_node, 
		input_name, text);
      XtUnmanageChild( cmd_input);
      set_prompt( "");
      input_open = 0;
      if ( redraw_cb)
        (redraw_cb)( this);

      ((XAttNav *)xattnav)->set_inputfocus();
    }
  }
}

static void xatt_activate_cmd_input( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( w);
  if ( xatt->input_open)
  {
    sts = ((XAttNav *)xatt->xattnav)->set_attr_value( xatt->input_node, 
		xatt->input_name, text);
    XtUnmanageChild( w);
    xatt->set_prompt( "");
    xatt->input_open = 0;
    if ( xatt->redraw_cb)
      (xatt->redraw_cb)( xatt);

    ((XAttNav *)xatt->xattnav)->set_inputfocus();
  }
}

static void xatt_activate_cmd_scrolled_ok( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( xatt->cmd_scrolledinput);
  if ( xatt->input_open)
  {
    sts = ((XAttNav *)xatt->xattnav)->set_attr_value( xatt->input_node,
		xatt->input_name, text);
    XtUnmanageChild( xatt->cmd_scrolledinput);
    XtUnmanageChild( xatt->cmd_scrolled_ok);
    XtUnmanageChild( xatt->cmd_scrolled_ca);
    xatt->set_prompt( "");
    xatt->input_open = 0;

    ((XAttNav *)xatt->xattnav)->redraw();
    ((XAttNav *)xatt->xattnav)->set_inputfocus();
  }
}

static void xatt_activate_cmd_scrolled_ca( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{

  if ( xatt->input_open)
  {
    XtUnmanageChild( xatt->cmd_scrolledinput);
    XtUnmanageChild( xatt->cmd_scrolled_ok);
    XtUnmanageChild( xatt->cmd_scrolled_ca);
    xatt->set_prompt( "");
    xatt->input_open = 0;
    ((XAttNav *)xatt->xattnav)->set_inputfocus();
  }
}

int XAtt::open_changevalue( char *name)
{
  int sts;

  sts = ((XAttNav*)xattnav)->select_by_name( name);
  if ( EVEN(sts)) return sts;

  change_value(0);
  return XATT__SUCCESS;
}

void XAtt::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

void XAtt::swap( int mode)
{
  ((XAttNav *)xattnav)->swap( mode);
}

XAtt::~XAtt()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  delete (XAttNav *)xattnav;
  XtDestroyWidget( parent_wid);
}

XAtt::XAtt( 
	Widget 		xa_parent_wid,
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
        int             *xa_sts) :
 	parent_wid(xa_parent_wid), parent_ctx(xa_parent_ctx), 
	objar(*xa_objar), 
	input_open(0), input_multiline(0), 
	close_cb(0), redraw_cb(0), client_data(0),
	set_focus_disabled(0), value_current_recall(0)
{
  char		uid_filename[120] = {"xtt_xatt.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: xatt_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"xatt_inputfocus",      (XtActionProc) xatt_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "xatt_ctx", 0 },
	{"xatt_activate_exit",(caddr_t)xatt_activate_exit },
	{"xatt_activate_display_object",(caddr_t)xatt_activate_display_object },
	{"xatt_activate_show_cross",(caddr_t)xatt_activate_show_cross },
	{"xatt_activate_open_classgraph",(caddr_t)xatt_activate_open_classgraph },
	{"xatt_activate_open_plc",(caddr_t)xatt_activate_open_plc },
	{"xatt_activate_change_value",(caddr_t)xatt_activate_change_value },
	{"xatt_activate_close_changeval",(caddr_t)xatt_activate_close_changeval },
	{"xatt_activate_help",(caddr_t)xatt_activate_help },
	{"xatt_create_msg_label",(caddr_t)xatt_create_msg_label },
	{"xatt_create_cmd_prompt",(caddr_t)xatt_create_cmd_prompt },
	{"xatt_create_cmd_input",(caddr_t)xatt_create_cmd_input },
	{"xatt_create_cmd_scrolledinput",(caddr_t)xatt_create_cmd_scrolledinput },
	{"xatt_create_cmd_scrolled_ok",(caddr_t)xatt_create_cmd_scrolled_ok },
	{"xatt_create_cmd_scrolled_ca",(caddr_t)xatt_create_cmd_scrolled_ca },
	{"xatt_create_xattnav_form",(caddr_t)xatt_create_xattnav_form },
	{"xatt_activate_cmd_scrolledinput",(caddr_t)xatt_activate_cmd_input },
	{"xatt_activate_cmd_scrolled_ok",(caddr_t)xatt_activate_cmd_scrolled_ok },
	{"xatt_activate_cmd_scrolled_ca",(caddr_t)xatt_activate_cmd_scrolled_ca }
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

  sts = MrmFetchWidgetOverride( s_DRMh, "xatt_window", parent_wid,
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
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);
  XtUnmanageChild( cmd_input);
  XtUnmanageChild( cmd_scrolledinput);
  XtUnmanageChild( cmd_scrolled_ok);
  XtUnmanageChild( cmd_scrolled_ca);

  xattnav = new XAttNav( (void *)this, xattnav_form, xattnav_eType_Object,
		"Plant", &objar, xa_advanced_user, &brow_widget, &sts);
  ((XAttNav *)xattnav)->message_cb = &xatt_message_cb;
  ((XAttNav *)xattnav)->change_value_cb = &xatt_change_value_cb;
  ((XAttNav *)xattnav)->popup_menu_cb = &xatt_popup_menu_cb;
  ((XAttNav *)xattnav)->is_authorized_cb = &xatt_is_authorized_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid,
	(XtCallbackProc)xatt_activate_exit, this);

  *xa_sts = XATT__SUCCESS;
}


static void xatt_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			   unsigned long item_type, unsigned long utility, 
			   char *arg, Widget *popup)
{
  if ( ((XAtt *)ctx)->popup_menu_cb)
    (((XAtt *)ctx)->popup_menu_cb) ( ((XAtt *)ctx)->parent_ctx, attrref,
				   item_type, utility, arg, popup);
}

static int xatt_is_authorized_cb( void *ctx, unsigned int access)
{
  XAtt	*xatt = (XAtt *)ctx;

  if ( xatt->is_authorized_cb)
    return (xatt->is_authorized_cb)( xatt->parent_ctx, access);
  return 0;
}



