/* 
 * Proview   $Id: rt_pb_gsd_attr.cpp,v 1.3 2006-04-12 12:17:45 claes Exp $
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

/* rt_pb_gsd_attr.cpp -- Display gsd attributes */

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
#include "co_mrm_util.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "flow_x.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"
#include "flow_msg.h"

#include "rt_pb_msg.h"
#include "rt_pb_gsd_attr.h"
#include "rt_pb_gsd_attrnav.h"


// Static memeber variables
char	GsdAttr::value_recall[30][160];

static void gsdattr_valchanged_cmd_input( Widget w, XEvent *event);
static void gsdattr_cmd_close_apply_cb( void *ctx, void *data);
static void gsdattr_cmd_close_no_cb( void *ctx, void *data);

static void gsdattr_message( void *attr, char severity, char *message)
{
  ((GsdAttr *)attr)->message( severity, message);
}

void GsdAttr::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetValues( msg_label, args, 1);
  XmStringFree( cstr);
}

void GsdAttr::set_prompt( char *prompt)
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

void GsdAttr::change_value()
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
    XmTextSetString( cmd_input, "");
  }
  set_prompt( "value >");
  input_open = 1;
}

static void gsdattr_change_value_cb( void *attr_ctx)
{
  GsdAttr *attr = (GsdAttr *) attr_ctx;
  attr->change_value();
}

//
//  Callbackfunctions from menu entries
//
static void gsdattr_activate_change_value( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->change_value();
}

static void gsdattr_activate_exit( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  if ( attr->close_cb) {
    if ( attr->gsd->is_modified()) {
      wow_DisplayQuestion( (void *)attr, attr->toplevel, "Apply",
			   "Do you want to apply changes",
			   gsdattr_cmd_close_apply_cb, gsdattr_cmd_close_no_cb, 0);

    }
    else
      (attr->close_cb)( attr->parent_ctx);
  }
  else
    delete attr;
}

static void gsdattr_activate_help( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  int sts;

  if ( attr->help_cb)
    sts = (attr->help_cb)( attr->parent_ctx, "pb_slave_editor /helpfile=\"$pwr_exe/profibus_xtthelp.dat\"");
  
}

static void gsdattr_activate_copy( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  ItemPbModule *item;
  int sts;

  sts = attr->attrnav->get_select( (ItemPb **) &item);
  if ( EVEN(sts)) {
    attr->message('E', "Select a module");
    return;
  }

  if ( item->type != attrnav_eItemType_PbModule) {
    attr->message('E', "Only modules can be copied");
    return;
  }
  
  attr->gsd->copy_module_conf( item->mconf);
  attr->message('I', "Module copied");
}

static void gsdattr_activate_cut( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  ItemPbModule *item;
  int sts;

  sts = attr->attrnav->get_select( (ItemPb **) &item);
  if ( EVEN(sts)) {
    attr->message('E', "Select a module");
    return;
  }

  if ( item->type != attrnav_eItemType_PbModule) {
    attr->message('E', "Only modules can be copied");
    return;
  }
  
  attr->gsd->cut_module_conf( item->mconf);
}

static void gsdattr_activate_paste( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  ItemPbModule *item;
  int sts;

  sts = attr->attrnav->get_select( (ItemPb **) &item);
  if ( EVEN(sts)) {
    attr->message('E', "Select a module");
    return;
  }

  if ( item->type != attrnav_eItemType_PbModule) {
    attr->message('E', "Only modules can be copied");
    return;
  }
  
  attr->gsd->paste_module_conf( item->mconf);
}

static void gsdattr_activate_zoom_in( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  attr->attrnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

  attr->attrnav->zoom( 1.18);
}

static void gsdattr_activate_zoom_out( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  attr->attrnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  attr->attrnav->zoom( 1.0 / 1.18);
}

static void gsdattr_activate_zoom_reset( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->attrnav->unzoom();
}

static void gsdattr_activate_print( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  char filename[80] = "pwrp_tmp:wnav.ps";
  char cmd[200];
  int sts;

  dcli_translate_filename( filename, filename);
  attr->attrnav->print( filename);

  sprintf( cmd, "wb_gre_print.sh %s", filename); 
  sts = system( cmd);
}

static void gsdattr_activate_cmd_ok( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  int sts;
  int idx;
  char msg[80];

  if ( attr->save_cb) {    

    // Check syntax
    sts = attr->gsd->syntax_check( &idx);
    if ( EVEN(sts)) {
      switch ( sts) {
      case PB__NOMODULENAME:
	sprintf( msg, "Syntax error in module %d, No module name", idx + 1);
	break;
      case PB__DUPLMODULENAME:
	sprintf( msg, "Syntax error in module %s, Duplicate module name", 
		 attr->gsd->module_conf[idx].name);
	break;
      case PB__NOMODULECLASS:
	sprintf( msg, "Syntax error in module %s, Module class is missing", 
		 attr->gsd->module_conf[idx].name);
	break;
      default:
	sprintf( msg, "Syntax error in module %d");
      }
      attr->message( 'E', msg);
      return;
    }

    sts = (attr->save_cb)( attr->parent_ctx);
    if ( EVEN(sts))
      attr->message( 'E', "Error saving profibus data");
    else
      attr->gsd->set_modified(0);
  }
}

static void gsdattr_cmd_close_apply_cb( void *ctx, void *data)
{
  GsdAttr *attr = (GsdAttr *)ctx;
  int sts;

  sts = (attr->save_cb)( attr->parent_ctx);
  if ( EVEN(sts))
    attr->message( 'E', "Error saving profibus data");
  else {
    attr->gsd->set_modified(0);
    (attr->close_cb)( attr->parent_ctx);
  }
}

static void gsdattr_cmd_close_no_cb( void *ctx, void *data)
{
  GsdAttr *attr = (GsdAttr *)ctx;

  (attr->close_cb)( attr->parent_ctx);
}

static void gsdattr_activate_cmd_ca( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  if ( attr->close_cb) {
    if ( attr->gsd->is_modified()) {
      wow_DisplayQuestion( (void *)attr, attr->toplevel, "Apply",
			   "Do you want to apply changes",
			   gsdattr_cmd_close_apply_cb, gsdattr_cmd_close_no_cb, 0);

    }
    else
      (attr->close_cb)( attr->parent_ctx);
  }
}

void gsdattr_create_menubutton( Widget w, GsdAttr *attr) 
{
  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) 
  {
    case 1: attr->menubutton_copy = w; break;
    case 2: attr->menubutton_cut = w; break;
    case 3: attr->menubutton_paste = w; break;
    case 4: attr->menubutton_changevalue = w; break;
    default:
      ;
  }
}
static void gsdattr_create_msg_label( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->msg_label = w;
}
static void gsdattr_create_cmd_prompt( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_prompt = w;
}
static void gsdattr_create_cmd_ok( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_ok = w;
}
static void gsdattr_create_cmd_ca( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_cancel = w;
}
static void gsdattr_create_cmd_input( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, attr);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) gsdattr_valchanged_cmd_input, mrm_eUtility_GsdAttr);
  attr->cmd_input = w;
}
static void gsdattr_create_attrnav_form( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data)
{
  attr->attrnav_form = w;
}
static void gsdattr_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  GsdAttr *attr;

  XtSetArg    (args[0], XmNuserData, &attr);
  XtGetValues (w, args, 1);

  if ( !attr)
    return;

  if ( flow_IsManaged( attr->cmd_input))
    flow_SetInputFocus( attr->cmd_input);
  else if ( attr->attrnav)
    ((GsdAttrNav *)attr->attrnav)->set_inputfocus();
}

static void gsdattr_valchanged_cmd_input( Widget w, XEvent *event)
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

GsdAttr::~GsdAttr()
{
  delete (GsdAttrNav *)attrnav;
  XtDestroyWidget( parent_wid);
}

GsdAttr::GsdAttr( Widget a_parent_wid,
  void			*a_parent_ctx,
  void 			*a_object,
  pb_gsd  		*a_gsd,
  int 			a_edit_mode) :
  parent_ctx(a_parent_ctx), gsd(a_gsd), edit_mode(a_edit_mode), input_open(0), object(a_object), 
  close_cb(0), save_cb(0), help_cb(0), client_data(0), recall_idx(-1),
  value_current_recall(0)
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
    {"gsdattr_inputfocus",      (XtActionProc) gsdattr_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "gsdattr_ctx", 0 },
	{"gsdattr_activate_exit",(caddr_t)gsdattr_activate_exit },
	{"gsdattr_activate_print",(caddr_t)gsdattr_activate_print },
	{"gsdattr_activate_copy",(caddr_t)gsdattr_activate_copy },
	{"gsdattr_activate_cut",(caddr_t)gsdattr_activate_cut },
	{"gsdattr_activate_paste",(caddr_t)gsdattr_activate_paste },
	{"gsdattr_activate_zoom_in",(caddr_t)gsdattr_activate_zoom_in },
	{"gsdattr_activate_zoom_out",(caddr_t)gsdattr_activate_zoom_out },
	{"gsdattr_activate_zoom_reset",(caddr_t)gsdattr_activate_zoom_reset },
	{"gsdattr_activate_change_value",(caddr_t)gsdattr_activate_change_value },
	{"gsdattr_activate_help",(caddr_t)gsdattr_activate_help },
	{"gsdattr_create_msg_label",(caddr_t)gsdattr_create_msg_label },
	{"gsdattr_create_menubutton",(caddr_t)gsdattr_create_menubutton },
	{"gsdattr_create_cmd_prompt",(caddr_t)gsdattr_create_cmd_prompt },
	{"gsdattr_create_cmd_input",(caddr_t)gsdattr_create_cmd_input },
	{"gsdattr_create_attrnav_form",(caddr_t)gsdattr_create_attrnav_form },
	{"gsdattr_create_cmd_ok",(caddr_t)gsdattr_create_cmd_ok },
	{"gsdattr_activate_cmd_ok",(caddr_t)gsdattr_activate_cmd_ok },
	{"gsdattr_create_cmd_ca",(caddr_t)gsdattr_create_cmd_ca },
	{"gsdattr_activate_cmd_ca",(caddr_t)gsdattr_activate_cmd_ca }
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

  sts = MrmFetchWidgetOverride( s_DRMh, "gsd_attr_window", parent_wid,
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

  attrnav = new GsdAttrNav( this, attrnav_form, "Plant",
		gsd, edit_mode, &brow_widget, &sts);
  ((GsdAttrNav *)attrnav)->message_cb = &gsdattr_message;
  ((GsdAttrNav *)attrnav)->change_value_cb = &gsdattr_change_value_cb;

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

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)gsdattr_activate_exit, this);
}

