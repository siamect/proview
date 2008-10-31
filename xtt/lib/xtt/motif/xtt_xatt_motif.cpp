/* 
 * Proview   $Id: xtt_xatt_motif.cpp,v 1.2 2008-10-31 12:51:36 claes Exp $
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

/* xtt_xatt.cpp -- Display object attributes */

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

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "flow_x.h"
#include "rt_xnav_msg.h"
#include "co_mrm_util.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "co_lng.h"
#include "xtt_xatt_motif.h"
#include "xtt_xattnav_motif.h"
#include "xtt_xnav.h"
#include "rt_xatt_msg.h"

// Static member elements
char XAttMotif::value_recall[30][160];

void XAttMotif::message( char severity, const char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( (char*) message, (char*) "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}

void XAttMotif::set_prompt( const char *prompt)
{
  Arg 		args[3];
  XmString	cstr;

  cstr=XmStringCreateLtoR( (char*) prompt, (char*) "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNwidth, 50);
  XtSetArg(args[2],XmNheight, 30);
  XtSetValues( cmd_prompt, args, 3);
  XmStringFree( cstr);
}

void XAttMotif::change_value( int set_focus)
{
  int		sts;
  Widget	text_w;
  int		multiline;
  char		*value;
  Arg 		args[5];
  int		input_size;

  if ( input_open) {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = xattnav->check_attr( &multiline, &input_node, input_name,
		&value, &input_size);
  if ( EVEN(sts)) {
    if ( sts == XATT__NOATTRSEL)
      message( 'E', "No attribute is selected");
    else
      message( 'E', XNav::get_message( sts));
    return;
  }

  if ( multiline) {
    text_w = cmd_scrolledinput;
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
      XmTextSetString( text_w, (char*) "");

    input_multiline = 1;
  }
  else {
    text_w = cmd_input;
    XtManageChild( text_w);
    XtSetArg(args[0],XmNmaxLength, input_size-1);
    XtSetValues( text_w, args, 1);
    if ( value) {
      XmTextSetString( text_w, value);
      XmTextSetInsertionPosition( text_w, strlen(value));
    }
    else
      XmTextSetString( text_w, (char*) "");

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

//
//  Callbackfunctions from menu entries
//
void XAttMotif::activate_change_value( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->change_value(1);
}

void XAttMotif::activate_close_changeval( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->change_value_close();
}

void XAttMotif::activate_exit( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  if ( xatt->close_cb)
    (xatt->close_cb)( xatt->parent_ctx, (void *)xatt);
  else
    delete xatt;
}

void XAttMotif::activate_display_object( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->activate_display_object();
}

void XAttMotif::activate_show_cross( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->activate_show_cross();
}

void XAttMotif::activate_open_classgraph( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->activate_open_classgraph();
}

void XAttMotif::activate_open_plc( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->activate_open_plc();
}

void XAttMotif::activate_help( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  xatt->activate_help();
}

void XAttMotif::create_msg_label( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  ((XAttMotif *)xatt)->msg_label = w;
}
void XAttMotif::create_cmd_prompt( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  ((XAttMotif *)xatt)->cmd_prompt = w;
}
void XAttMotif::create_cmd_input( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, xatt);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) valchanged_cmd_input, mrm_eUtility_XAtt);
  ((XAttMotif *)xatt)->cmd_input = w;
}
void XAttMotif::create_cmd_scrolledinput( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  ((XAttMotif *)xatt)->cmd_scrolledinput = w;
}
void XAttMotif::create_cmd_scrolled_ok( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  ((XAttMotif *)xatt)->cmd_scrolled_ok = w;
}
void XAttMotif::create_cmd_scrolled_ca( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  ((XAttMotif *)xatt)->cmd_scrolled_ca = w;
}
void XAttMotif::create_xattnav_form( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  ((XAttMotif *)xatt)->xattnav_form = w;
}

void XAttMotif::enable_set_focus( XAttMotif *xatt)
{
  xatt->set_focus_disabled--;
}

void XAttMotif::disable_set_focus( XAttMotif *xatt, int time)
{
  xatt->set_focus_disabled++;
  xatt->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( xatt->toplevel), time,
	(XtTimerCallbackProc)enable_set_focus, xatt);
}

void XAttMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  XAttMotif *xatt;

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
    xatt->xattnav->set_inputfocus();

  disable_set_focus( xatt, 400);

}

void XAttMotif::valchanged_cmd_input( Widget w, XEvent *event)
{
  XAttMotif 	*xatt;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &xatt);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)value_recall, sizeof(value_recall[0]),
	sizeof(value_recall)/sizeof(value_recall[0]),
	&xatt->value_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( xatt->input_open)
    {
      sts = xatt->xattnav->set_attr_value( xatt->input_node, 
		xatt->input_name, text);
      XtUnmanageChild( w);
      xatt->set_prompt( "");
      xatt->input_open = 0;
      if ( xatt->redraw_cb)
        (xatt->redraw_cb)( xatt);

      xatt->xattnav->set_inputfocus();
    }
  }
}

void XAttMotif::change_value_close()
{
  char *text;
  int sts;

  text = XmTextGetString( cmd_scrolledinput);
  if ( input_open) {
    if ( input_multiline) {
      sts = xattnav->set_attr_value( input_node,
		input_name, text);
      XtUnmanageChild( cmd_scrolledinput);
      XtUnmanageChild( cmd_scrolled_ok);
      XtUnmanageChild( cmd_scrolled_ca);
      set_prompt( "");
      input_open = 0;

      xattnav->redraw();
      xattnav->set_inputfocus();
    }
    else {
      text = XmTextGetString( cmd_input);

      sts = xattnav->set_attr_value( input_node, 
		input_name, text);
      XtUnmanageChild( cmd_input);
      set_prompt( "");
      input_open = 0;
      if ( redraw_cb)
        (redraw_cb)( this);

      xattnav->set_inputfocus();
    }
  }
}

void XAttMotif::activate_cmd_input( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( w);
  if ( xatt->input_open)
  {
    sts = xatt->xattnav->set_attr_value( xatt->input_node, 
		xatt->input_name, text);
    XtUnmanageChild( w);
    xatt->set_prompt( "");
    xatt->input_open = 0;
    if ( xatt->redraw_cb)
      (xatt->redraw_cb)( xatt);

    xatt->xattnav->set_inputfocus();
  }
}

void XAttMotif::activate_cmd_scrolled_ok( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( ((XAttMotif *)xatt)->cmd_scrolledinput);

  if ( xatt->input_open) {
    sts = xatt->xattnav->set_attr_value( xatt->input_node,
		xatt->input_name, text);
    XtUnmanageChild( ((XAttMotif *)xatt)->cmd_scrolledinput);
    XtUnmanageChild( ((XAttMotif *)xatt)->cmd_scrolled_ok);
    XtUnmanageChild( ((XAttMotif *)xatt)->cmd_scrolled_ca);
    xatt->set_prompt( "");
    xatt->input_open = 0;

    xatt->xattnav->redraw();
    xatt->xattnav->set_inputfocus();
  }
}

void XAttMotif::activate_cmd_scrolled_ca( Widget w, XAtt *xatt, XmAnyCallbackStruct *data)
{

  if ( xatt->input_open) {
    XtUnmanageChild( ((XAttMotif *)xatt)->cmd_scrolledinput);
    XtUnmanageChild( ((XAttMotif *)xatt)->cmd_scrolled_ok);
    XtUnmanageChild( ((XAttMotif *)xatt)->cmd_scrolled_ca);
    xatt->set_prompt( "");
    xatt->input_open = 0;
    xatt->xattnav->set_inputfocus();
  }
}

void XAttMotif::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

XAttMotif::~XAttMotif()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  delete (XAttNav *)xattnav;
  XtDestroyWidget( parent_wid);
}

XAttMotif::XAttMotif( Widget 		xa_parent_wid,
		      void 		*xa_parent_ctx, 
		      pwr_sAttrRef 	*xa_objar,
		      int 		xa_advanced_user,
		      int             *xa_sts) :
  XAtt( xa_parent_ctx, xa_objar, xa_advanced_user, xa_sts),
  parent_wid(xa_parent_wid), set_focus_disabled(0),
  value_current_recall(0)
{
  char		uid_filename[120] = {"xtt_xatt.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  pwr_tAName   	title;
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: xatt_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {(char*) "xatt_inputfocus",      (XtActionProc) action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        {(char*) "xatt_ctx", 0 },
	{(char*) "xatt_activate_exit",(caddr_t)activate_exit },
	{(char*) "xatt_activate_display_object",(caddr_t)activate_display_object },
	{(char*) "xatt_activate_show_cross",(caddr_t)activate_show_cross },
	{(char*) "xatt_activate_open_classgraph",(caddr_t)activate_open_classgraph },
	{(char*) "xatt_activate_open_plc",(caddr_t)activate_open_plc },
	{(char*) "xatt_activate_change_value",(caddr_t)activate_change_value },
	{(char*) "xatt_activate_close_changeval",(caddr_t)activate_close_changeval },
	{(char*) "xatt_activate_help",(caddr_t)activate_help },
	{(char*) "xatt_create_msg_label",(caddr_t)create_msg_label },
	{(char*) "xatt_create_cmd_prompt",(caddr_t)create_cmd_prompt },
	{(char*) "xatt_create_cmd_input",(caddr_t)create_cmd_input },
	{(char*) "xatt_create_cmd_scrolledinput",(caddr_t)create_cmd_scrolledinput },
	{(char*) "xatt_create_cmd_scrolled_ok",(caddr_t)create_cmd_scrolled_ok },
	{(char*) "xatt_create_cmd_scrolled_ca",(caddr_t)create_cmd_scrolled_ca },
	{(char*) "xatt_create_xattnav_form",(caddr_t)create_xattnav_form },
	{(char*) "xatt_activate_cmd_scrolledinput",(caddr_t)activate_cmd_input },
	{(char*) "xatt_activate_cmd_scrolled_ok",(caddr_t)activate_cmd_scrolled_ok },
	{(char*) "xatt_activate_cmd_scrolled_ca",(caddr_t)activate_cmd_scrolled_ca }
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

  cdh_StrncpyCutOff( title, title, 100, 1);

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

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "xatt_window", parent_wid,
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

  xattnav = new XAttNavMotif( (void *)this, xattnav_form, xattnav_eType_Object,
		"Plant", &objar, xa_advanced_user, &brow_widget, &sts);
  xattnav->message_cb = &message_cb;
  xattnav->change_value_cb = &change_value_cb;
  xattnav->popup_menu_cb = &xatt_popup_menu_cb;
  xattnav->is_authorized_cb = &xatt_is_authorized_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid,
	(XtCallbackProc)activate_exit, this);

  *xa_sts = XATT__SUCCESS;
}



