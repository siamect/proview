/* ge_attr.cpp -- Display object attributes

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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

#include "ge_attr.h"
#include "ge_attrnav.h"
#include "ge_msg.h"


// Static memeber variables
char	Attr::value_recall[30][160];

static int attr_get_subgraph_info_cb( void *attr_ctx, char *name, 
	attr_sItem **itemlist, int *item_cnt);
static void attr_valchanged_cmd_input( Widget w, XEvent *event);

static void attr_message( void *attr, char severity, char *message)
{
  ((Attr *)attr)->message( severity, message);
}

void Attr::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetValues( msg_label, args, 1);
  XmStringFree( cstr);
}

void Attr::set_prompt( char *prompt)
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

void Attr::change_value()
{
  int		sts;
  Widget	text_w;
  int		multiline;
  char		*value;

  if ( input_open) 
  {
    XtUnmanageChild( cmd_input);
    set_prompt( "");
    input_open = 0;
    return;
  }

  sts = ((AttrNav *)attrnav)->check_attr_value( &multiline, &value);
  if ( EVEN(sts))
  {
    if ( sts == GE__NOATTRSEL)
      message( 'E', "No attribute is selected");
    else
      message( 'E', "Attribute can't be modified");
    return;
  }

  if ( multiline)
  {
    text_w = cmd_scrolledinput;
    XtManageChild( text_w);
    XtManageChild( cmd_scrolled_ok);
    XtManageChild( cmd_scrolled_ca);
  }
  else
  {
    text_w = cmd_input;
    XtManageChild( text_w);
  }


  message( ' ', "");
  flow_SetInputFocus( text_w);
//  XtSetKeyboardFocus( toplevel, text_w);
//  XtCallAcceptFocus( text_w, CurrentTime);

  if ( value)
  {
    XmTextSetString( text_w, value);
    if ( !multiline) {
      XmTextSetInsertionPosition( text_w, strlen(value));
      XmTextSetSelection( text_w, 0, strlen(value), CurrentTime);
    }
  }
  else
  {
    XmTextSetString( cmd_input, "");
  }
//  i = 0;
//  XtSetArg(args[i],XmNeditMode, XmMULTI_LINE_EDIT); i++;
//  XtSetValues( text_w, args, i);
  set_prompt( "value >");
  input_open = 1;
}

static void attr_change_value_cb( void *attr_ctx)
{
  Attr *attr = (Attr *) attr_ctx;
  attr->change_value();
}

static int attr_reconfigure_attr_cb( void *attr_ctx)
{
  attr_sItem 	*itemlist;
  int	   	item_cnt;
  int		sts;

  Attr *attr = (Attr *) attr_ctx;
  if ( !attr->reconfigure_attr_cb)
    return 0;
  attr->reconfigure_attr_cb( attr->parent_ctx, attr->object, 
	&itemlist, &item_cnt, &attr->client_data);  

  delete (AttrNav *)attr->attrnav;
  attr->attrnav = new AttrNav( attr, attr->attrnav_form, "Plant",
		itemlist, item_cnt, &attr->brow_widget, &sts);
  ((AttrNav *)attr->attrnav)->message_cb = &attr_message;
  ((AttrNav *)attr->attrnav)->change_value_cb = &attr_change_value_cb;
  ((AttrNav *)attr->attrnav)->get_subgraph_info_cb = 
		&attr_get_subgraph_info_cb;
  ((AttrNav *)attr->attrnav)->reconfigure_attr_cb = 
		&attr_reconfigure_attr_cb;
  ((AttrNav *)attr->attrnav)->set_inputfocus();

  return 1;
}

static int attr_get_subgraph_info_cb( void *attr_ctx, char *name, 
	attr_sItem **itemlist, int *item_cnt)
{
  Attr *attr = (Attr *) attr_ctx;
  if ( attr->get_subgraph_info_cb)
    return attr->get_subgraph_info_cb( attr->parent_ctx, name, itemlist, 
		item_cnt);  
  return 0;
}

//
//  Callbackfunctions from menu entries
//
static void attr_activate_change_value( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  attr->change_value();
}

static void attr_activate_store( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  if ( attr->store_cb)
    (attr->store_cb)( attr->parent_ctx, attr->object);
}

static void attr_activate_recall_next( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  int sts;
  glow_sTraceData *old_data;
  int idx;

  if ( attr->recall_idx == -1)
    idx = 0;
  else
    idx = attr->recall_idx + 1;

  if ( attr->recall_cb)
    sts = (attr->recall_cb)( attr->parent_ctx, attr->object, 
				     idx, &old_data);
  if ( ODD(sts))
  {
    if ( attr->recall_idx == -1) {
      attr->original_data = *old_data;
      attr->recall_idx = 0;
    }
    attr->recall_idx = idx;
    attr_reconfigure_attr_cb((void *)attr);
  }
}

static void attr_activate_recall_prev( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  int sts;
  glow_sTraceData *old_p;

  if ( attr->recall_idx < 0)
    return;
  attr->recall_idx--;
  if ( attr->recall_idx == -1) {
    // Get original data
    if ( attr->set_data_cb) {
      (attr->set_data_cb)( attr->parent_ctx, attr->object, 
			       &attr->original_data);
      attr_reconfigure_attr_cb((void *)attr);
    }
  }
  else {
    if ( attr->recall_cb) {
      sts = (attr->recall_cb)( attr->parent_ctx, attr->object, 
				  attr->recall_idx, &old_p);
      if ( ODD(sts))
        attr_reconfigure_attr_cb((void *)attr);
      else
        attr->recall_idx++;
    }
  }
}

static void attr_activate_exit( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  if ( attr->close_cb)
    (attr->close_cb)( attr);
  else
    delete attr;
}

static void attr_activate_help( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void attr_create_msg_label( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  attr->msg_label = w;
}
static void attr_create_cmd_prompt( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_prompt = w;
}
static void attr_create_cmd_input( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, attr);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) attr_valchanged_cmd_input, mrm_eUtility_Attr);
  attr->cmd_input = w;
}
static void attr_create_cmd_scrolledinput( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_scrolledinput = w;
}
static void attr_create_cmd_scrolled_ok( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_scrolled_ok = w;
}
static void attr_create_cmd_scrolled_ca( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  attr->cmd_scrolled_ca = w;
}
static void attr_create_attrnav_form( Widget w, Attr *attr, XmAnyCallbackStruct *data)
{
  attr->attrnav_form = w;
}
static void attr_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Attr *attr;

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
  Attr *attr;
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

static void attr_activate_cmd_input( Widget w, Attr *attr, XmAnyCallbackStruct *data)
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

static void attr_activate_cmd_scrolled_ok( Widget w, Attr *attr, XmAnyCallbackStruct *data)
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

static void attr_activate_cmd_scrolled_ca( Widget w, Attr *attr, XmAnyCallbackStruct *data)
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


Attr::~Attr()
{
  delete (AttrNav *)attrnav;
  XtDestroyWidget( parent_wid);
}

Attr::Attr( Widget a_parent_wid,
  void			*a_parent_ctx,
  void 			*a_object,
  attr_sItem  		*itemlist,
  int			item_cnt ) :
  parent_ctx(a_parent_ctx), input_open(0), object(a_object), 
  close_cb(0), redraw_cb(0), get_subgraph_info_cb(0), reconfigure_attr_cb(0),
  store_cb(0), recall_cb(0), set_data_cb(0), client_data(0), recall_idx(-1),
  value_current_recall(0)
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
    {"attr_inputfocus",      (XtActionProc) attr_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "attr_ctx", 0 },
	{"attr_activate_exit",(caddr_t)attr_activate_exit },
	{"attr_activate_store",(caddr_t)attr_activate_store },
	{"attr_activate_recall_prev",(caddr_t)attr_activate_recall_prev },
	{"attr_activate_recall_next",(caddr_t)attr_activate_recall_next },
	{"attr_activate_change_value",(caddr_t)attr_activate_change_value },
	{"attr_activate_help",(caddr_t)attr_activate_help },
	{"attr_create_msg_label",(caddr_t)attr_create_msg_label },
	{"attr_create_cmd_prompt",(caddr_t)attr_create_cmd_prompt },
	{"attr_create_cmd_input",(caddr_t)attr_create_cmd_input },
	{"attr_create_cmd_scrolledinput",(caddr_t)attr_create_cmd_scrolledinput },
	{"attr_create_cmd_scrolled_ok",(caddr_t)attr_create_cmd_scrolled_ok },
	{"attr_create_cmd_scrolled_ca",(caddr_t)attr_create_cmd_scrolled_ca },
	{"attr_create_attrnav_form",(caddr_t)attr_create_attrnav_form },
	{"attr_activate_cmd_scrolledinput",(caddr_t)attr_activate_cmd_input },
	{"attr_activate_cmd_scrolled_ok",(caddr_t)attr_activate_cmd_scrolled_ok },
	{"attr_activate_cmd_scrolled_ca",(caddr_t)attr_activate_cmd_scrolled_ca }
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

  sts = MrmFetchWidgetOverride( s_DRMh, "attr_window", parent_wid,
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

  attrnav = new AttrNav( this, attrnav_form, "Plant",
		itemlist, item_cnt, &brow_widget, &sts);
  ((AttrNav *)attrnav)->message_cb = &attr_message;
  ((AttrNav *)attrnav)->change_value_cb = &attr_change_value_cb;
  ((AttrNav *)attrnav)->get_subgraph_info_cb = 
		&attr_get_subgraph_info_cb;
  ((AttrNav *)attrnav)->reconfigure_attr_cb = 
		&attr_reconfigure_attr_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)attr_activate_exit, this);
}

