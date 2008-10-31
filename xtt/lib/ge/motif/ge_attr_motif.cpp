/* 
 * Proview   $Id: ge_attr_motif.cpp,v 1.3 2008-10-31 12:51:33 claes Exp $
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

/* ge_attr.cpp -- Display object attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_mrm_util.h"
#include "flow_x.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_motif.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_motif.h"
#include "flow_msg.h"

#include "ge_attr_motif.h"
#include "ge_attrnav_motif.h"
#include "ge_dyn.h"
#include "ge_msg.h"


// Static memeber variables
char	AttrMotif::value_recall[30][160];

static void attr_valchanged_cmd_input( Widget w, XEvent *event);

void AttrMotif::message( char severity, const char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( (char*) message, (char*) "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetValues( msg_label, args, 1);
  XmStringFree( cstr);
}

void AttrMotif::set_prompt( const char *prompt)
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

void AttrMotif::change_value()
{
  int		sts;
  Widget	text_w;
  int		multiline;
  char		*value;

  if ( input_open) {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = ((AttrNav *)attrnav)->check_attr_value( &multiline, &value);
  if ( EVEN(sts)) {
    if ( sts == GE__NOATTRSEL)
      message( 'E', "No attribute is selected");
    else
      message( 'E', "Attribute can't be modified");
    return;
  }

  if ( multiline) {
    text_w = cmd_scrolledinput;
    XtManageChild( text_w);
    XtManageChild( cmd_scrolled_ok);
    XtManageChild( cmd_scrolled_ca);
  }
  else {
    text_w = cmd_input;
    XtManageChild( text_w);
  }


  message( ' ', "");
  flow_SetInputFocus( text_w);
//  XtSetKeyboardFocus( toplevel, text_w);
//  XtCallAcceptFocus( text_w, CurrentTime);

  if ( value) {
    XmTextSetString( text_w, value);
    if ( !multiline) {
      XmTextSetInsertionPosition( text_w, strlen(value));
      XmTextSetSelection( text_w, 0, strlen(value), CurrentTime);
    }
  }
  else {
    XmTextSetString( cmd_input, (char*) "");
  }
//  i = 0;
//  XtSetArg(args[i],XmNeditMode, XmMULTI_LINE_EDIT); i++;
//  XtSetValues( text_w, args, i);
  set_prompt( "value >");
  input_open = 1;
}

int AttrMotif::reconfigure_attr()
{
  attr_sItem 	*itemlist;
  int	   	item_cnt;
  int		sts;

  if ( !reconfigure_attr_cb)
    return 0;
  reconfigure_attr_cb( parent_ctx, object, 
	&itemlist, &item_cnt, &client_data);  

  delete (AttrNav *)attrnav;
  attrnav = new AttrNavMotif( this, attrnav_form, "Plant",
		itemlist, item_cnt, &brow_widget, &sts);
  ((AttrNav *)attrnav)->message_cb = &Attr::message;
  ((AttrNav *)attrnav)->change_value_cb = &Attr::change_value_c;
  ((AttrNav *)attrnav)->get_subgraph_info_cb = 
		&Attr::get_subgraph_info_c;
  ((AttrNav *)attrnav)->get_dyn_info_cb = 
		&Attr::get_dyn_info_c;
  ((AttrNav *)attrnav)->reconfigure_attr_cb = 
		&Attr::reconfigure_attr_c;
  ((AttrNav *)attrnav)->set_inputfocus();
  ((AttrNav *)attrnav)->get_plant_select_cb = 
		&Attr::get_plant_select_c;
  ((AttrNav *)attrnav)->get_current_colors_cb = 
		&Attr::get_current_colors_c;


  return 1;
}

//
//  Callbackfunctions from menu entries
//
static void attr_activate_change_value( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->change_value();
}

static void attr_activate_store( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->store();
}

static void attr_activate_recall_next( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->recall_next();
}

static void attr_activate_recall_prev( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->recall_prev();
}

static void attr_activate_exit( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  if ( attr->close_cb)
    (attr->close_cb)( attr);
  else
    delete attr;
}

static void attr_activate_help( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void attr_create_msg_label( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->msg_label = w;
}
static void attr_create_cmd_prompt( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_prompt = w;
}
static void attr_create_cmd_input( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, attr);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) attr_valchanged_cmd_input, mrm_eUtility_Attr);
  attr->cmd_input = w;
}
static void attr_create_cmd_scrolledinput( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_scrolledinput = w;
}
static void attr_create_cmd_scrolled_ok( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_scrolled_ok = w;
}
static void attr_create_cmd_scrolled_ca( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_scrolled_ca = w;
}
static void attr_create_attrnav_form( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  attr->attrnav_form = w;
}
static void attr_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  AttrMotif *attr;

  XtSetArg    (args[0], XmNuserData, &attr);
  XtGetValues (w, args, 1);

  if ( !attr)
    return;

  if ( flow_IsManaged( attr->cmd_scrolledinput))
    flow_SetInputFocus( attr->cmd_scrolledinput);
  else if ( flow_IsManaged( attr->cmd_input))
    flow_SetInputFocus( attr->cmd_input);
  else if ( attr->attrnav)
    ((AttrNav *)attr->attrnav)->set_inputfocus();
}

static void attr_valchanged_cmd_input( Widget w, XEvent *event)
{
  AttrMotif *attr;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &attr);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)attr->value_recall, sizeof(attr->value_recall[0]),
	sizeof( attr->value_recall)/sizeof(attr->value_recall[0]),
	&attr->value_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( attr->input_open)
    {
      sts = ((AttrNav *)attr->attrnav)->set_attr_value( text);
      XtUnmanageChild( w);
      attr->set_prompt( "");
      attr->input_open = 0;
      if ( attr->redraw_cb)
        (attr->redraw_cb)( attr);

      ((AttrNav *)attr->attrnav)->set_inputfocus();
    }
  }
}

static void attr_activate_cmd_input( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( w);
  if ( attr->input_open)
  {
    sts = ((AttrNav *)attr->attrnav)->set_attr_value( text);
    XtUnmanageChild( w);
    attr->set_prompt( "");
    attr->input_open = 0;
    if ( attr->redraw_cb)
      (attr->redraw_cb)( attr);

    ((AttrNav *)attr->attrnav)->set_inputfocus();
  }
}

static void attr_activate_cmd_scrolled_ok( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( attr->cmd_scrolledinput);
  if ( attr->input_open)
  {
    sts = ((AttrNav *)attr->attrnav)->set_attr_value( text);
    XtUnmanageChild( attr->cmd_scrolledinput);
    XtUnmanageChild( attr->cmd_scrolled_ok);
    XtUnmanageChild( attr->cmd_scrolled_ca);
    attr->set_prompt( "");
    attr->input_open = 0;
    if ( attr->redraw_cb)
      (attr->redraw_cb)( attr);
  }
}

static void attr_activate_cmd_scrolled_ca( Widget w, AttrMotif *attr, XmAnyCallbackStruct *data)
{

  if ( attr->input_open)
  {
    XtUnmanageChild( attr->cmd_scrolledinput);
    XtUnmanageChild( attr->cmd_scrolled_ok);
    XtUnmanageChild( attr->cmd_scrolled_ca);
    attr->set_prompt( "");
    attr->input_open = 0;
  }
}


AttrMotif::~AttrMotif()
{
  if ( original_data)
    delete original_data;
  delete (AttrNav *)attrnav;
  XtDestroyWidget( parent_wid);
}

AttrMotif::AttrMotif( Widget a_parent_wid,
  void			*a_parent_ctx,
  void 			*a_object,
  attr_sItem  		*itemlist,
  int			item_cnt ) :
  Attr( a_parent_ctx, a_object, itemlist, item_cnt), 
  parent_wid(a_parent_wid), value_current_recall(0)
{
  char		uid_filename[120] = {"pwr_exe:ge_attr.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: attr_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {(char*) "attr_inputfocus",      (XtActionProc) attr_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        {(char*) "attr_ctx", 0 },
	{(char*) "attr_activate_exit",(caddr_t)attr_activate_exit },
	{(char*) "attr_activate_store",(caddr_t)attr_activate_store },
	{(char*) "attr_activate_recall_prev",(caddr_t)attr_activate_recall_prev },
	{(char*) "attr_activate_recall_next",(caddr_t)attr_activate_recall_next },
	{(char*) "attr_activate_change_value",(caddr_t)attr_activate_change_value },
	{(char*) "attr_activate_help",(caddr_t)attr_activate_help },
	{(char*) "attr_create_msg_label",(caddr_t)attr_create_msg_label },
	{(char*) "attr_create_cmd_prompt",(caddr_t)attr_create_cmd_prompt },
	{(char*) "attr_create_cmd_input",(caddr_t)attr_create_cmd_input },
	{(char*) "attr_create_cmd_scrolledinput",(caddr_t)attr_create_cmd_scrolledinput },
	{(char*) "attr_create_cmd_scrolled_ok",(caddr_t)attr_create_cmd_scrolled_ok },
	{(char*) "attr_create_cmd_scrolled_ca",(caddr_t)attr_create_cmd_scrolled_ca },
	{(char*) "attr_create_attrnav_form",(caddr_t)attr_create_attrnav_form },
	{(char*) "attr_activate_cmd_scrolledinput",(caddr_t)attr_activate_cmd_input },
	{(char*) "attr_activate_cmd_scrolled_ok",(caddr_t)attr_activate_cmd_scrolled_ok },
	{(char*) "attr_activate_cmd_scrolled_ca",(caddr_t)attr_activate_cmd_scrolled_ca }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

#if !defined OS_VMS
  {
    char *s;

    if ( (s = getenv( "pwr_exe")) == NULL)
    {
      printf( "** pwr_exe is not defined\n");
      exit(0);
    }
    sprintf( uid_filename, "%s/ge_attr.uid", s);
  }
#endif

  // Motif
  MrmInitialize();

  strcpy( title, "PROVIEW/R Attr");

  reglist[0].value = (caddr_t) this;

  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell("objectEditor", 
		topLevelShellWidgetClass, a_parent_wid, args, 0);

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "attr_window", parent_wid,
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
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);
  XtUnmanageChild( cmd_input);
  XtUnmanageChild( cmd_scrolledinput);
  XtUnmanageChild( cmd_scrolled_ok);
  XtUnmanageChild( cmd_scrolled_ca);

  attrnav = new AttrNavMotif( this, attrnav_form, "Plant",
		itemlist, item_cnt, &brow_widget, &sts);
  ((AttrNav *)attrnav)->message_cb = &Attr::message;
  ((AttrNav *)attrnav)->change_value_cb = &Attr::change_value_c;
  ((AttrNav *)attrnav)->get_subgraph_info_cb = 
		&Attr::get_subgraph_info_c;
  ((AttrNav *)attrnav)->get_dyn_info_cb = 
		&Attr::get_dyn_info_c;
  ((AttrNav *)attrnav)->reconfigure_attr_cb = 
		&Attr::reconfigure_attr_c;
  ((AttrNav *)attrnav)->get_plant_select_cb = 
		&Attr::get_plant_select_c;
  ((AttrNav *)attrnav)->get_current_colors_cb = 
		&Attr::get_current_colors_c;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)attr_activate_exit, this);
}

