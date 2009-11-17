/* 
 * Proview   $Id: rt_pb_gsd_attr_motif.cpp,v 1.2 2008-10-31 12:51:29 claes Exp $
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

/* rt_pb_gsd_attr_motif.cpp -- Display gsd attributes */

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
#include "co_time.h"
#include "cow_mrm_util.h"
#include "co_dcli.h"
#include "flow_x.h"
}
#include "cow_wow_motif.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "flow_msg.h"

#include "rt_pb_msg.h"
#include "rt_pb_gsd_attr_motif.h"
#include "rt_pb_gsd_attrnav_motif.h"


void GsdAttrMotif::message( char severity, const char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( (char*) message, (char*) "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetValues( msg_label, args, 1);
  XmStringFree( cstr);
}

void GsdAttrMotif::set_prompt( const char *prompt)
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

void GsdAttrMotif::change_value()
{
  int		sts;
  Widget	text_w;
  char		*value;

  if ( input_open) {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return;
  }

  if ( !edit_mode) {
    message( 'E', "Not in edit mode");
    return;
  }

  sts = ((GsdAttrNav *)attrnav)->check_attr_value( &value);
  if ( EVEN(sts)) {
    if ( sts == PB__NOATTRSEL)
      message( 'E', "No attribute is selected");
    else
      message( 'E', "Attribute can't be modified");
    return;
  }

  text_w = cmd_input;
  XtManageChild( text_w);


  message( ' ', "");
  flow_SetInputFocus( text_w);

  if ( value) {
    XmTextSetString( text_w, value);
    XmTextSetInsertionPosition( text_w, strlen(value));
    XmTextSetSelection( text_w, 0, strlen(value), CurrentTime);
  }
  else {
    XmTextSetString( cmd_input, (char*) "");
  }
  set_prompt( "value >");
  input_open = 1;
}

//
//  Callbackfunctions from menu entries
//
void GsdAttrMotif::activate_change_value( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->change_value();
}

void GsdAttrMotif::activate_exit( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_exit();
}

void GsdAttrMotif::activate_help( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_help();
}

void GsdAttrMotif::activate_copy( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_copy();
}

void GsdAttrMotif::activate_cut( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_cut();
}

void GsdAttrMotif::activate_paste( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_paste();
}

void GsdAttrMotif::activate_zoom_in( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_zoom_in();
}

void GsdAttrMotif::activate_zoom_out( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_zoom_out();
}

void GsdAttrMotif::activate_zoom_reset( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_zoom_reset();
}

void GsdAttrMotif::activate_print( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_print();
}

void GsdAttrMotif::activate_cmd_ok( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_cmd_ok();
}

void GsdAttrMotif::activate_cmd_ca( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->activate_cmd_ca();
}

void GsdAttrMotif::create_menubutton( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data) 
{
  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) {
  case 1: ((GsdAttrMotif *)attr)->menubutton_copy = w; break;
  case 2: ((GsdAttrMotif *)attr)->menubutton_cut = w; break;
  case 3: ((GsdAttrMotif *)attr)->menubutton_paste = w; break;
  case 4: ((GsdAttrMotif *)attr)->menubutton_changevalue = w; break;
  default:
    ;
  }
}
void GsdAttrMotif::create_msg_label( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  ((GsdAttrMotif *)attr)->msg_label = w;
}
void GsdAttrMotif::create_cmd_prompt( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  ((GsdAttrMotif *)attr)->cmd_prompt = w;
}
void GsdAttrMotif::create_cmd_ok( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  ((GsdAttrMotif *)attr)->cmd_ok = w;
}
void GsdAttrMotif::create_cmd_ca( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  ((GsdAttrMotif *)attr)->cmd_cancel = w;
}
void GsdAttrMotif::create_cmd_input( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, attr);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) valchanged_cmd_input, mrm_eUtility_GsdAttr);
  ((GsdAttrMotif *)attr)->cmd_input = w;
}
void GsdAttrMotif::create_attrnav_form( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  ((GsdAttrMotif *)attr)->attrnav_form = w;
}
void GsdAttrMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  GsdAttrMotif *attr;

  XtSetArg    (args[0], XmNuserData, &attr);
  XtGetValues (w, args, 1);

  if ( !attr)
    return;

  if ( flow_IsManaged( attr->cmd_input))
    flow_SetInputFocus( attr->cmd_input);
  else if ( attr->attrnav)
    ((GsdAttrNav *)attr->attrnav)->set_inputfocus();
}

void GsdAttrMotif::valchanged_cmd_input( Widget w, XEvent *event)
{
  GsdAttr *attr;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &attr);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)attr->value_recall, sizeof(attr->value_recall[0]),
	sizeof( attr->value_recall)/sizeof(attr->value_recall[0]),
	&attr->value_current_recall);
  if ( sts) {
    text = XmTextGetString( w);
    if ( attr->input_open)
    {
      sts = ((GsdAttrNav *)attr->attrnav)->set_attr_value( text);
      XtUnmanageChild( w);
      attr->set_prompt( "");
      attr->input_open = 0;

      ((GsdAttrNav *)attr->attrnav)->set_inputfocus();
    }
  }
}

GsdAttrMotif::~GsdAttrMotif()
{
  delete (GsdAttrNav *)attrnav;
  XtDestroyWidget( parent_wid);
}

GsdAttrMotif::GsdAttrMotif( Widget a_parent_wid,
			    void *a_parent_ctx,
			    void *a_object,
			    pb_gsd *a_gsd,
			    int a_edit_mode) :
  GsdAttr( a_parent_ctx, a_object, a_gsd, a_edit_mode)
{
  char		uid_filename[120] = {"pwr_exe:pb_gsd_attr.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: gsdattr_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {(char*) "gsdattr_inputfocus",      (XtActionProc) action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        {(char*) "gsdattr_ctx", 0 },
	{(char*) "gsdattr_activate_exit",(caddr_t)activate_exit },
	{(char*) "gsdattr_activate_print",(caddr_t)activate_print },
	{(char*) "gsdattr_activate_copy",(caddr_t)activate_copy },
	{(char*) "gsdattr_activate_cut",(caddr_t)activate_cut },
	{(char*) "gsdattr_activate_paste",(caddr_t)activate_paste },
	{(char*) "gsdattr_activate_zoom_in",(caddr_t)activate_zoom_in },
	{(char*) "gsdattr_activate_zoom_out",(caddr_t)activate_zoom_out },
	{(char*) "gsdattr_activate_zoom_reset",(caddr_t)activate_zoom_reset },
	{(char*) "gsdattr_activate_change_value",(caddr_t)activate_change_value },
	{(char*) "gsdattr_activate_help",(caddr_t)activate_help },
	{(char*) "gsdattr_create_msg_label",(caddr_t)create_msg_label },
	{(char*) "gsdattr_create_menubutton",(caddr_t)create_menubutton },
	{(char*) "gsdattr_create_cmd_prompt",(caddr_t)create_cmd_prompt },
	{(char*) "gsdattr_create_cmd_input",(caddr_t)create_cmd_input },
	{(char*) "gsdattr_create_attrnav_form",(caddr_t)create_attrnav_form },
	{(char*) "gsdattr_create_cmd_ok",(caddr_t)create_cmd_ok },
	{(char*) "gsdattr_activate_cmd_ok",(caddr_t)activate_cmd_ok },
	{(char*) "gsdattr_create_cmd_ca",(caddr_t)create_cmd_ca },
	{(char*) "gsdattr_activate_cmd_ca",(caddr_t)activate_cmd_ca }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  dcli_translate_filename( uid_filename, uid_filename);

  // Motif
  MrmInitialize();

  strcpy( title, "PwR GsdAttr");

  reglist[0].value = (caddr_t) this;

  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell("pbGsdEditor", 
		topLevelShellWidgetClass, a_parent_wid, args, 0);

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "gsd_attr_window", parent_wid,
			name, args, 1, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);


  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext( toplevel), 
						actions, XtNumber(actions));
 
  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( toplevel, compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,500);i++;
  XtSetArg(args[i],XmNheight,700);i++;
  XtSetValues( toplevel,args,i);

  i = 0;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;
  XtSetValues( parent_wid,args,i);
    
  XtManageChild( toplevel);
  XtUnmanageChild( cmd_input);

  attrnav = new GsdAttrNavMotif( this, attrnav_form, (char*) "Plant",
		gsd, edit_mode, &brow_widget, &sts);
  ((GsdAttrNav *)attrnav)->message_cb = &GsdAttr::gsdattr_message;
  ((GsdAttrNav *)attrnav)->change_value_cb = &GsdAttr::gsdattr_change_value_cb;

  XtPopup( parent_wid, XtGrabNone);

  if ( !edit_mode) {
    i = 0;
    XtSetArg( args[i], XmNsensitive, 0);i++;
    XtSetValues( cmd_ok, args, i);
    XtSetValues( menubutton_copy, args, i);
    XtSetValues( menubutton_cut, args, i);
    XtSetValues( menubutton_paste, args, i);
    XtSetValues( menubutton_changevalue, args, i);
  }

  wow = new CoWowMotif( toplevel);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)activate_exit, this);
}

