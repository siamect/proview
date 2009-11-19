/* 
 * Proview   $Id: rt_xtt_motif.cpp,v 1.6 2008-10-31 12:51:32 claes Exp $
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

/* rt_xtt_motif.cpp -- Display plant and node hiererachy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_ini_event.h"
#include "rt_qcom_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "pwr_nmpsclasses.h"
#include "flow_x.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>

extern "C" {
#include "xtt_hotkey_motif.h"
#include "co_dcli.h"
#include "cow_mrm_util.h"
}

#include "co_lng.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
//#include "flow_browwidget.h"
#include "rt_xtt_motif.h"
#include "xtt_trace.h"
#include "xtt_xnav_motif.h"
#include "xtt_item.h"
#include "xtt_url.h"
#include "co_lng.h"
#include "cow_xhelp_motif.h"
#include "cow_wow_motif.h"
#include "rt_xnav_msg.h"
#include "co_syi.h"

// Static variables
static String   fbr[] = {
    NULL};

static XtActionsRec HotkeyActions[] = {
	{ (char*) "Command",	(XtActionProc) XttMotif::hotkey_Command},
	{ (char*) "ToggleDig",	(XtActionProc) XttMotif::hotkey_ToggleDig},
	{ (char*) "SetDig",	(XtActionProc) XttMotif::hotkey_SetDig},
	{ (char*) "ResetDig",	(XtActionProc) XttMotif::hotkey_ResetDig}
    };

void XttMotif::hotkey_Command( Widget w, XKeyEvent* ev, String* av, Cardinal* ac)
{
  if (*ac == 0)
    return;

  hot_xtt->hotkey_activate_command( av[0]);
}

void XttMotif::hotkey_ToggleDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac)
{
  if (ev->type != KeyPress)
    return;

  if (*ac == 0)
    return;

  hotkey_activate_toggledig( av[0]);
}

void XttMotif::hotkey_SetDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac)
{
  if (ev->type != KeyPress)
    return;

  if (*ac == 0)
    return;

  hotkey_activate_setdig( av[0]);
}

void XttMotif::hotkey_ResetDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac)
{
  if (ev->type != KeyPress)
    return;

  if (*ac == 0)
    return;

  hotkey_activate_resetdig( av[0]);
}

void XttMotif::xtt_mainloop (XtAppContext AppCtx)
{
  XEvent Event;

  for (;;) {
    XtAppNextEvent( AppCtx, &Event);
    if (Event.type != KeyPress || 
        TkSUCCESS != hotkey_Process( HotkeyHandle, &Event)) 
      XtDispatchEvent( &Event);
  }
}

int XttMotif::init_hotkey( XtAppContext AppCtx, Widget Top)
{
  hotkeySTATUS    HotkeySts;

  XtAppAddActions( AppCtx, HotkeyActions, XtNumber(HotkeyActions));

  HotkeySts = hotkey_Initialize (Top, &HotkeyHandle);
  switch ( HotkeySts ) {
  case TkERROR : 
    puts ("Xtt: Failed to bind hotkey(s)");
    break;
  case TkNO_BINDINGS :
    puts ("OpWin: No hotkey resource found in resource db");
    break;
  case TkPARSE_ERROR :
    puts ("OpWin: Syntax error in hotkey bindings");
  }
  return 1;
}

void XttMotif::open_input_dialog( const char *text, const char *title,
				  const char *init_text,
				  void (*ok_cb)( Xtt *, char *))
{
  Arg		args[10];
  int 		i;
  XmString	cstr;

  i = 0;
  XtSetArg(args[0], XmNlabelString,
		cstr=XmStringCreateLtoR( (char*) text, (char*) "ISO8859-1") ); i++;
  XtSetValues( india_label, args, i);
  XmStringFree( cstr);
  i = 0;
  XtSetArg(args[0], XmNdialogTitle,
		cstr=XmStringCreateLtoR( (char*) title, (char*) "ISO8859-1") ); i++;
  XtSetValues( india_widget, args, i);
  XmStringFree( cstr);

  XmTextSetString( india_text, (char*) init_text);

  XtManageChild( india_widget);

  XmProcessTraversal( india_text, XmTRAVERSE_CURRENT);

  india_ok_cb = ok_cb;
}

void XttMotif::message( char severity, const char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( (char*) message, (char*) "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetValues( msg_label, args, 1);
  XmStringFree( cstr);
}

void XttMotif::close( void *ctx, int terminate)
{
  Xtt	*xtt = (Xtt *) ctx;
  if ( terminate) {
    xtt->wow->DisplayQuestion( xtt, "Confirm", 
			 "Do you want to close", close_ok, 0, 0);
    // delete xtt->xnav;
    // exit(0);
  }
  else {
    xtt->xnav->displayed = 0;
    flow_UnmapWidget( ((XttMotif *)xtt)->toplevel);
  }
}

void XttMotif::set_dimension( void *ctx, int width, int height)
{
  Xtt	*xtt = (Xtt *) ctx;
  Arg 		args[2];

  int i = 0;
  if ( width) {
    XtSetArg( args[i], XmNwidth, width);i++;
  }
  if ( height) {
    XtSetArg( args[i], XmNheight, height);i++;
  }
  XtSetValues( ((XttMotif *)xtt)->toplevel, args,i);
}

void XttMotif::map( void *ctx)
{
  Xtt	*xtt = (Xtt *) ctx;
  if ( !xtt->xnav->displayed) {
    flow_MapWidget( ((XttMotif *)xtt)->toplevel);
    xtt->xnav->displayed = 1;
  }
  else
    xtt->xnav->pop();
}

void XttMotif::set_prompt( const char *prompt)
{
  Arg 		args[1];
  XmString	cstr;

  cstr=XmStringCreateLtoR( (char*) prompt, (char*) "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetValues( cmd_prompt, args, 1);
  XmStringFree( cstr);
}

void XttMotif::open_change_value()
{
  Arg 		args[1];
  int		sts;

  if ( input_open) {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = xnav->check_attr_value();
  if ( EVEN(sts)) {
    message( 'E', XNav::get_message(sts));
    return;
  }

  if ( command_open)
    command_open = 0;
  else
    XtManageChild( cmd_input);

  message( ' ', "");
  XtSetKeyboardFocus( toplevel, cmd_input);

  XtSetArg(args[0],XmNvalue, "");
  XtSetValues( cmd_input, args, 1);
  set_prompt( Lng::translate("value >"));
  input_open = 1;
}

//
//  Callbackfunctions from menu entries
//
void XttMotif::activate_change_value( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->open_change_value();
}

void XttMotif::activate_command( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  Arg 		args[1];

  if ( xtt->command_open) {
    XtUnmanageChild( ((XttMotif *)xtt)->cmd_input);
    xtt->set_prompt( "");
    xtt->command_open = 0;
    return;
  }

  if ( xtt->input_open)
    xtt->input_open = 0;
  else
    XtManageChild( ((XttMotif *)xtt)->cmd_input);
  xtt->message( ' ', "");
  XtSetKeyboardFocus( ((XttMotif *)xtt)->toplevel, ((XttMotif *)xtt)->cmd_input);

  XtSetArg(args[0],XmNvalue, "");
  XtSetValues( ((XttMotif *)xtt)->cmd_input, args, 1);
  xtt->set_prompt( "xtt >");
  xtt->command_open = 1;
}

void XttMotif::activate_exit( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
    close( xtt, xtt->xnav->op ? 0 : 1);
}

void XttMotif::activate_print( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_print();
}

void XttMotif::activate_find( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_find();
}

void XttMotif::activate_findregex( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_findregex();
}

void XttMotif::activate_findnext( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_findnext();
}

void XttMotif::activate_collapse( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_collapse();
}

void XttMotif::activate_openobject( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_openobject();
}

void XttMotif::activate_openplc( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_openplc();
}

void XttMotif::activate_showcrossref( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->show_crossref();
}

void XttMotif::activate_opengraph( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_opengraph();
}

void XttMotif::activate_collect_insert( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_collect_insert();
}

void XttMotif::activate_collect_show( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->collect_show();
}

void XttMotif::activate_collect_remove( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->collect_remove();
}

void XttMotif::activate_collect_clear( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->collect_clear();
}

void XttMotif::activate_advanceduser( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_advanceduser();
}

void XttMotif::activate_zoom_in( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_zoom_in();
}

void XttMotif::activate_zoom_out( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_zoom_out();
}

void XttMotif::activate_zoom_reset( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->unzoom();
}

void XttMotif::activate_help( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_help();
}

void XttMotif::activate_help_project( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_help_project();
}

void XttMotif::activate_help_proview( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->activate_help_proview();
}

void XttMotif::create_msg_label( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  ((XttMotif *)xtt)->msg_label = w;
}
void XttMotif::create_cmd_prompt( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  ((XttMotif *)xtt)->cmd_prompt = w;
}
void XttMotif::create_cmd_input( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, xtt);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) valchanged_cmd_input, mrm_eUtility_Xtt);

  ((XttMotif *)xtt)->cmd_input = w;
}
void XttMotif::create_xnav_form( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  ((XttMotif *)xtt)->xnav_form = w;
}
void XttMotif::create_india_label( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  ((XttMotif *)xtt)->india_label = w;
}
void XttMotif::create_india_text( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  ((XttMotif *)xtt)->india_text = w;
}
void XttMotif::activate_india_ok( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  char *value;

  value = XmTextGetString( ((XttMotif *)xtt)->india_text);
  XtUnmanageChild( ((XttMotif *)xtt)->india_widget);

  (xtt->india_ok_cb)( xtt, value);
}
void XttMotif::activate_india_cancel( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((XttMotif *)xtt)->india_widget);
}
void XttMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Xtt *xtt;

  XtSetArg    (args[0], XmNuserData, &xtt);
  XtGetValues (w, args, 1);

  if ( mrm_IsIconicState(w))
    return;

  if ( xtt->xnav) {
    if ( ((XttMotif *)xtt)->focustimer.disabled()) {
      // printf( "Inputfocus disabled\n");
      return;
    }
    else {
      // printf( "Set Inputfocus\n");
    }

    xtt->xnav->set_inputfocus();

    ((XttMotif *)xtt)->focustimer.disable( ((XttMotif *)xtt)->toplevel, 400);
  }
}

void XttMotif::valchanged_cmd_input( Widget w, XEvent *event, XmAnyCallbackStruct *data)
{
  Xtt 	*xtt;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &xtt);
  XtGetValues(w, args, 1);

  if ( xtt->input_open)
    sts = mrm_TextInput( w, event, (char *)((XttMotif *)xtt)->value_recall, sizeof(((XttMotif *)xtt)->value_recall[0]),
	sizeof( ((XttMotif *)xtt)->value_recall)/sizeof(((XttMotif *)xtt)->value_recall[0]),
	&((XttMotif *)xtt)->value_current_recall);
  else
    sts = mrm_TextInput( w, event, (char *)((XttMotif *)xtt)->cmd_recall, sizeof(((XttMotif *)xtt)->cmd_recall[0]),
	sizeof( ((XttMotif *)xtt)->cmd_recall)/sizeof(((XttMotif *)xtt)->cmd_recall[0]),
	&((XttMotif *)xtt)->cmd_current_recall);
  if ( sts) {
    text = XmTextGetString( w);
    if ( xtt->input_open) {
      sts = xtt->xnav->set_attr_value( text);
      XtUnmanageChild( w);
      xtt->set_prompt( "");
      xtt->input_open = 0;
    }
    else if ( xtt->command_open) {
      sts = xtt->xnav->command( text);
      XtUnmanageChild( w);
      xtt->set_prompt( "");
      xtt->command_open = 0;
    }
  }
}


int main(  int argc, char *argv[])
{
  int sts;

  new XttMotif( argc, argv, &sts);
  exit(sts);
}

XttMotif::~XttMotif()
{
}

XttMotif::XttMotif( int argc, char *argv[], int *return_sts) :
  Xtt( &argc, &argv, return_sts),
  cmd_current_recall(0), value_current_recall(0) 
{
  char		uid_filename[120] = {"xtt.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  int		i;
  XtAppContext  app_ctx;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	xtt_widget;
  char		title[120] = "Xtt ";
  char		nodename[80];

  static char translations[] =
    "<FocusIn>: xtt_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {(char*) "xtt_inputfocus",      (XtActionProc) action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        {(char*) "xtt_ctx", 0 },
	{(char*) "xtt_activate_exit",(caddr_t)activate_exit },
	{(char*) "xtt_activate_print",(caddr_t)activate_print },
	{(char*) "xtt_activate_find",(caddr_t)activate_find },
	{(char*) "xtt_activate_findregex",(caddr_t)activate_findregex },
	{(char*) "xtt_activate_findnext",(caddr_t)activate_findnext },
	{(char*) "xtt_activate_collapse",(caddr_t)activate_collapse },
	{(char*) "xtt_activate_openobject",(caddr_t)activate_openobject },
	{(char*) "xtt_activate_openplc",(caddr_t)activate_openplc },
	{(char*) "xtt_activate_opengraph",(caddr_t)activate_opengraph },
	{(char*) "xtt_activate_showcrossref",(caddr_t)activate_showcrossref },
	{(char*) "xtt_activate_change_value",(caddr_t)activate_change_value },
	{(char*) "xtt_activate_command",(caddr_t)activate_command },
	{(char*) "xtt_activate_collect_insert",(caddr_t)activate_collect_insert },
	{(char*) "xtt_activate_collect_show",(caddr_t)activate_collect_show },
	{(char*) "xtt_activate_collect_remove",(caddr_t)activate_collect_remove },
	{(char*) "xtt_activate_collect_clear",(caddr_t)activate_collect_clear },
	{(char*) "xtt_activate_advanceduser",(caddr_t)activate_advanceduser },
	{(char*) "xtt_activate_zoom_in",(caddr_t)activate_zoom_in },
	{(char*) "xtt_activate_zoom_out",(caddr_t)activate_zoom_out },
	{(char*) "xtt_activate_zoom_reset",(caddr_t)activate_zoom_reset },
	{(char*) "xtt_activate_help",(caddr_t)activate_help },
	{(char*) "xtt_activate_help_project",(caddr_t)activate_help_project },
	{(char*) "xtt_activate_help_proview",(caddr_t)activate_help_proview },
	{(char*) "xtt_create_msg_label",(caddr_t)create_msg_label },
	{(char*) "xtt_create_cmd_prompt",(caddr_t)create_cmd_prompt },
	{(char*) "xtt_create_cmd_input",(caddr_t)create_cmd_input },
	{(char*) "xtt_create_xnav_form",(caddr_t)create_xnav_form },
	{(char*) "xtt_activate_india_ok",(caddr_t)activate_india_ok },
	{(char*) "xtt_activate_india_cancel",(caddr_t)activate_india_cancel },
	{(char*) "xtt_create_india_label",(caddr_t)create_india_label },
	{(char*) "xtt_create_india_text",(caddr_t)create_india_text }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  syi_NodeName( &sts, nodename, sizeof(nodename));
  if ( ODD(sts))
    strcat( title, nodename);

  // Motif
  Lng::get_uid( uid_filename, uid_filename);
  MrmInitialize();

  toplevel = XtVaAppInitialize (
		      &app_ctx, 
		      "Rt_xtt",
		      NULL, 0, 
		      &argc, argv, 
		      fbr, 
		      XtNallowShellResize,  True,
		      XtNtitle, title,
		      XmNmappedWhenManaged, True,
		      NULL);

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

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "xtt_window", toplevel,
			title, args, i, &xtt_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch xtt_window\n");

  sts = MrmFetchWidget(s_DRMh, (char*) "input_dialog", toplevel,
  		&india_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  MrmCloseHierarchy(s_DRMh);


  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( xtt_widget, compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,350);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( xtt_widget);
  XtUnmanageChild( cmd_input);

  xnav = new XNavMotif( this, xnav_form, "Plant",
		&brow_widget, (xnav_sStartMenu *)root_menu, 
		opplace_str, op_close_button, &sts);
  xnav->message_cb = &xtt_message_cb;
  xnav->close_cb = &close;
  xnav->map_cb = &map;
  xnav->change_value_cb = &change_value;
  xnav->set_dimension_cb = &set_dimension;
  xnav->attach_audio = attach_audio;

  // Create help window
  CoXHelp *xhelp = new CoXHelpMotif( toplevel, this, xhelp_eUtility_Xtt, &sts);
  xhelp->open_URL_cb = open_URL_cb;
  CoXHelp::set_default( xhelp);

  XtRealizeWidget( toplevel);

  if ( !xnav->is_authorized( pwr_mAccess_AllRt, 0))
    xnav->open_login();

  wow = new CoWowMotif( toplevel);
  if ( !quiet)
    wow->DisplayWarranty();

//  XmProcessTraversal( xnav->brow_widget, XmTRAVERSE_CURRENT);
//  xnav->set_inputfocus();

  init_hotkey( app_ctx, toplevel);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)activate_exit, this);

  if ( xnav->op)
    close( this, 0);

  timerid = wow->timer_new();

  // Start timer to check for qcom events
  timerid->add( 1000, qcom_events, this);

  xtt_mainloop( app_ctx);  
}








