/* ge_attr.cpp -- Display object attributes

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
#include "wb_watt_msg.h"
#include "co_mrm_util.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_watt.h"
#include "wb_wattnav.h"
#include "wb_wtt.h"
#include "wb_wnav.h"
#include "co_xhelp.h"


// Static member elements
char WAtt::value_recall[30][160];

// Prototype declarations
static void watt_valchanged_cmd_input( Widget w, XEvent *event);

void WAtt::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}

void WAtt::set_prompt( char *prompt)
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

void WAtt::change_value( int set_focus)
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

  sts = ((WAttNav *)wattnav)->check_attr( &multiline, &input_node, input_name,
		&value, &input_size);
  if ( EVEN(sts))
  {
    if ( sts == WATT__NOATTRSEL)
      message( 'E', "No attribute is selected");
    else
      message( 'E', wnav_get_message( sts));
    return;
  }

  if ( multiline)
  {
    text_w = cmd_scrolledinput;
    XtManageChild( text_w);
    XtManageChild( cmd_scrolled_ok);
    XtManageChild( cmd_scrolled_ca);

    // XtSetArg(args[0], XmNpaneMaximum, 300);
    // XtSetValues( wattnav_form, args, 1);

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
      XmTextSetSelection( text_w, 0, strlen(value), CurrentTime);
    }
    else
      XmTextSetString( text_w, "");

    input_multiline = 0;
  }

  message( ' ', "");
  if ( set_focus)
    flow_SetInputFocus( text_w);
  set_prompt( "value >");
  input_open = 1;
}

static void watt_message_cb( void *watt, char severity, char *message)
{
  ((WAtt *)watt)->message( severity, message);
}

static void watt_change_value_cb( void *watt)
{
  ((WAtt *)watt)->change_value(1);
}

//
//  Callbackfunctions from menu entries
//
static void watt_activate_change_value( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  watt->change_value(1);
}

static void watt_activate_close_changeval( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  watt->change_value_close();
}

static void watt_activate_exit( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  if ( watt->close_cb)
    (watt->close_cb)( watt);
  else
    delete watt;
}

static void watt_activate_help( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  CoXHelp::dhelp( "object editor", 0, navh_eHelpFile_Base, 0, true);
}

static void watt_create_msg_label( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  watt->msg_label = w;
}
static void watt_create_cmd_prompt( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  watt->cmd_prompt = w;
}
static void watt_create_cmd_input( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, watt);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) watt_valchanged_cmd_input, mrm_eUtility_WAtt);
  watt->cmd_input = w;
}
static void watt_create_cmd_scrolledinput( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  watt->cmd_scrolledinput = w;
}
static void watt_create_cmd_scrolled_ok( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  watt->cmd_scrolled_ok = w;
}
static void watt_create_cmd_scrolled_ca( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  watt->cmd_scrolled_ca = w;
}
static void watt_create_wattnav_form( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  watt->wattnav_form = w;
}

static void watt_enable_set_focus( WAtt *watt)
{
  watt->set_focus_disabled--;
}

static void watt_disable_set_focus( WAtt *watt, int time)
{
  watt->set_focus_disabled++;
  watt->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( watt->toplevel), time,
	(XtTimerCallbackProc)watt_enable_set_focus, watt);
}

static void watt_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  WAtt *watt;

  XtSetArg    (args[0], XmNuserData, &watt);
  XtGetValues (w, args, 1);

  if ( !watt)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( watt->set_focus_disabled)
    return;

  if ( flow_IsManaged( watt->cmd_scrolledinput))
    flow_SetInputFocus( watt->cmd_scrolledinput);
  else if ( flow_IsManaged( watt->cmd_input))
    flow_SetInputFocus( watt->cmd_input);
  else if ( watt->wattnav)
    ((WAttNav *)watt->wattnav)->set_inputfocus();

  watt_disable_set_focus( watt, 400);

}

static void watt_valchanged_cmd_input( Widget w, XEvent *event)
{
  WAtt 	*watt;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &watt);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)WAtt::value_recall, sizeof(WAtt::value_recall[0]),
	sizeof( WAtt::value_recall)/sizeof(WAtt::value_recall[0]),
	&watt->value_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( watt->input_open)
    {
      sts = ((WAttNav *)watt->wattnav)->set_attr_value( watt->input_node, 
		watt->input_name, text);
      XtUnmanageChild( w);
      watt->set_prompt( "");
      watt->input_open = 0;
      if ( watt->redraw_cb)
        (watt->redraw_cb)( watt);

      ((WAttNav *)watt->wattnav)->set_inputfocus();
    }
  }
}

void WAtt::change_value_close()
{
  char *text;
  int sts;

  text = XmTextGetString( cmd_scrolledinput);
  if ( input_open) {
    if ( input_multiline) {
      sts = ((WAttNav *)wattnav)->set_attr_value( input_node,
		input_name, text);
      XtUnmanageChild( cmd_scrolledinput);
      XtUnmanageChild( cmd_scrolled_ok);
      XtUnmanageChild( cmd_scrolled_ca);
      set_prompt( "");
      input_open = 0;

      ((WAttNav *)wattnav)->redraw();
      ((WAttNav *)wattnav)->set_inputfocus();
    }
    else {
      text = XmTextGetString( cmd_input);

      sts = ((WAttNav *)wattnav)->set_attr_value( input_node, 
		input_name, text);
      XtUnmanageChild( cmd_input);
      set_prompt( "");
      input_open = 0;
      if ( redraw_cb)
        (redraw_cb)( this);

      ((WAttNav *)wattnav)->set_inputfocus();
    }
  }
}

static void watt_activate_cmd_input( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( w);
  if ( watt->input_open)
  {
    sts = ((WAttNav *)watt->wattnav)->set_attr_value( watt->input_node, 
		watt->input_name, text);
    XtUnmanageChild( w);
    watt->set_prompt( "");
    watt->input_open = 0;
    if ( watt->redraw_cb)
      (watt->redraw_cb)( watt);

    ((WAttNav *)watt->wattnav)->set_inputfocus();
  }
}

static void watt_activate_cmd_scrolled_ok( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( watt->cmd_scrolledinput);
  if ( watt->input_open)
  {
    sts = ((WAttNav *)watt->wattnav)->set_attr_value( watt->input_node,
		watt->input_name, text);
    XtUnmanageChild( watt->cmd_scrolledinput);
    XtUnmanageChild( watt->cmd_scrolled_ok);
    XtUnmanageChild( watt->cmd_scrolled_ca);
    watt->set_prompt( "");
    watt->input_open = 0;

    ((WAttNav *)watt->wattnav)->redraw();
    ((WAttNav *)watt->wattnav)->set_inputfocus();
  }
}

static void watt_activate_cmd_scrolled_ca( Widget w, WAtt *watt, XmAnyCallbackStruct *data)
{

  if ( watt->input_open)
  {
    XtUnmanageChild( watt->cmd_scrolledinput);
    XtUnmanageChild( watt->cmd_scrolled_ok);
    XtUnmanageChild( watt->cmd_scrolled_ca);
    watt->set_prompt( "");
    watt->input_open = 0;
    ((WAttNav *)watt->wattnav)->set_inputfocus();
  }
}

void WAtt::set_editmode( int editmode, ldh_tSesContext ldhses)
{
  this->ldhses = ldhses;
  this->editmode = editmode;
  ((WAttNav *)wattnav)->set_editmode( editmode, ldhses);
}

int WAtt::open_changevalue( char *name)
{
  int sts;

  sts = ((WAttNav*)wattnav)->select_by_name( name);
  if ( EVEN(sts)) return sts;

  change_value(0);
  return WATT__SUCCESS;
}

void WAtt::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

WAtt::~WAtt()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  delete (WAttNav *)wattnav;
  XtDestroyWidget( parent_wid);
}

WAtt::WAtt( 
	Widget 		wa_parent_wid,
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses, 
	pwr_sAttrRef 	wa_aref,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname) :
 	parent_wid(wa_parent_wid), parent_ctx(wa_parent_ctx), 
	ldhses(wa_ldhses), aref(wa_aref), editmode(wa_editmode), 
	input_open(0), input_multiline(0), 
	close_cb(0), redraw_cb(0), client_data(0),
	set_focus_disabled(0), value_current_recall(0)
{
  char		uid_filename[120] = {"pwr_exe:wb_watt.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: watt_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"watt_inputfocus",      (XtActionProc) watt_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "watt_ctx", 0 },
	{"watt_activate_exit",(caddr_t)watt_activate_exit },
	{"watt_activate_change_value",(caddr_t)watt_activate_change_value },
	{"watt_activate_close_changeval",(caddr_t)watt_activate_close_changeval },
	{"watt_activate_help",(caddr_t)watt_activate_help },
	{"watt_create_msg_label",(caddr_t)watt_create_msg_label },
	{"watt_create_cmd_prompt",(caddr_t)watt_create_cmd_prompt },
	{"watt_create_cmd_input",(caddr_t)watt_create_cmd_input },
	{"watt_create_cmd_scrolledinput",(caddr_t)watt_create_cmd_scrolledinput },
	{"watt_create_cmd_scrolled_ok",(caddr_t)watt_create_cmd_scrolled_ok },
	{"watt_create_cmd_scrolled_ca",(caddr_t)watt_create_cmd_scrolled_ca },
	{"watt_create_wattnav_form",(caddr_t)watt_create_wattnav_form },
	{"watt_activate_cmd_scrolledinput",(caddr_t)watt_activate_cmd_input },
	{"watt_activate_cmd_scrolled_ok",(caddr_t)watt_activate_cmd_scrolled_ok },
	{"watt_activate_cmd_scrolled_ca",(caddr_t)watt_activate_cmd_scrolled_ca }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  // for ( i = 0; i < int(sizeof(value_recall)/sizeof(value_recall[0])); i++)
  //  value_recall[i][0] = 0;

  dcli_translate_filename( uid_filename, uid_filename);

  // Create object context
//  attrctx->close_cb = close_cb;
//  attrctx->redraw_cb = redraw_cb;

  // Motif
  MrmInitialize();

  strcpy( title, "PwR Object attributes");

  reglist[0].value = (caddr_t) this;

  // Save the context structure in the widget
  i = 0;
  XtSetArg (args[i], XmNuserData, (unsigned int) this);i++;
  XtSetArg( args[i], XmNdeleteResponse, XmDO_NOTHING);i++;

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell("objectEditor", 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "watt_window", parent_wid,
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

  utility = *(wb_eUtility *)parent_ctx;
  wattnav = new WAttNav( (void *)this, wattnav_form, "Plant",
		ldhses, aref, wa_editmode, wa_advanced_user,
		wa_display_objectname, utility, &brow_widget, &sts);
  ((WAttNav *)wattnav)->message_cb = &watt_message_cb;
  ((WAttNav *)wattnav)->change_value_cb = &watt_change_value_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)watt_activate_exit, this);

  if ( utility == wb_eUtility_Wtt)
  {
    ((Wtt *)parent_ctx)->register_utility( (void *) this,
	wb_eUtility_AttributeEditor);
  }
}











