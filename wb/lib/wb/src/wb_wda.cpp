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
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "flow_x.h"
#include "wb_wda_msg.h"
#include "co_mrm_util.h"
#include "co_wow.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_wda.h"
#include "wb_wdanav.h"
#include "wb_wtt.h"
#include "wb_wnav.h"


// Static member elements
char Wda::value_recall[30][160];

// Prototype declarations
static void wda_valchanged_cmd_input( Widget w, XEvent *event);
static void wda_set_attr_cb( void *ctx, char *text);

void Wda::message( char severity, char *message)
{
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetArg(args[1],XmNheight, 20);
  XtSetValues( msg_label, args, 2);
  XmStringFree( cstr);
}

void Wda::set_prompt( char *prompt)
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

void Wda::change_value( int set_focus)
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

  sts = ((WdaNav *)wdanav)->check_attr( &multiline, &input_node, input_name,
		&value, &input_size);
  if ( EVEN(sts))
  {
    if ( sts == WDA__NOATTRSEL)
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
    // XtSetValues( wdanav_form, args, 1);

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
    free( value);

  message( ' ', "");
  if ( set_focus)
    flow_SetInputFocus( text_w);
  set_prompt( "value >");
  input_open = 1;
}

static void wda_message_cb( void *wda, char severity, char *message)
{
  ((Wda *)wda)->message( severity, message);
}

static void wda_change_value_cb( void *wda)
{
  ((Wda *)wda)->change_value(1);
}

//
//  Callbackfunctions from menu entries
//
static void wda_activate_change_value( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->change_value(1);
}

static void wda_activate_close_changeval( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->change_value_close();
}

static void wda_activate_exit( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  if ( wda->close_cb)
    (wda->close_cb)( wda);
  else
    delete wda;
}
static void wda_activate_print( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  char filename[80] = "pwrp_tmp:wda.ps";
  char cmd[200];
  int sts;

  dcli_translate_filename( filename, filename);
  ((WdaNav *)wda->wdanav)->print(filename);

#if defined OS_LINUX
  sprintf( cmd, "wb_gre_print.sh %s", filename); 
  sts = system( cmd);
#endif
}
static void wda_activate_setclass( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->open_class_dialog();
}

static void wda_activate_setattr( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->open_attr_dialog();
}

static void wda_activate_nextattr( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  int sts;

  sts = wda->next_attr();
  if ( EVEN(sts))
    wow_DisplayError( wda->parent_wid, "Spreadsheet error",
		      wnav_get_message( sts));
}

static void wda_activate_prevattr( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  int sts;

  sts = wda->prev_attr();
  if ( EVEN(sts))
    wow_DisplayError( wda->parent_wid, "Spreadsheet error",
		      wnav_get_message( sts));
}

static void wda_activate_help( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  // Not yet implemented
}

static void wda_create_msg_label( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->msg_label = w;
}
static void wda_create_cmd_prompt( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->cmd_prompt = w;
}
static void wda_create_cmd_input( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, wda);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) wda_valchanged_cmd_input, mrm_eUtility_Wda);
  wda->cmd_input = w;
}
static void wda_create_cmd_scrolledinput( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->cmd_scrolledinput = w;
}
static void wda_create_cmd_scrolled_ok( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->cmd_scrolled_ok = w;
}
static void wda_create_cmd_scrolled_ca( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->cmd_scrolled_ca = w;
}

static void wda_create_wdanav_form( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->wdanav_form = w;
}
static void wdaclass_create_hiervalue( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->wdaclass_hiervalue = w;
}

static void wdaclass_create_classvalue( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  wda->wdaclass_classvalue = w;
}

static void wdaclass_activate_ok( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  char *hiername;
  char *classname;
  int sts;
  pwr_tClassId new_classid;

  hiername = XmTextGetString( wda->wdaclass_hiervalue);
  classname = XmTextGetString( wda->wdaclass_classvalue);

  if ( strcmp( hiername, "") == 0)
    wda->objid = pwr_cNObjid;
  else {
    sts = ldh_NameToObjid( wda->ldhses, &wda->objid, hiername);
    if ( EVEN(sts)) {
      wow_DisplayError( wda->wdaclass_dia, "Hierarchy object error",
			wnav_get_message( sts));
      return;
    }
  }

  sts = ldh_ClassNameToId( wda->ldhses, &new_classid, classname);
  if ( EVEN(sts)) {
    wow_DisplayError( wda->wdaclass_dia, "Class error",
		      wnav_get_message( sts));
    return;
  }

  XtUnmanageChild( wda->wdaclass_dia);

  if ( wda->classid != new_classid) {
    // Enter attribute
    wda->classid = new_classid;
    wda->open_attr_dialog();
  }
  else {
    // Find new attributes
    sts = ((WdaNav *)wda->wdanav)->update( wda->objid, wda->classid,
		wda->attribute);
    if ( EVEN(sts))
      wow_DisplayError( wda->parent_wid, "Spreadsheet error",
		      wnav_get_message( sts));
  }

}

static void wdaclass_activate_cancel( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( wda->wdaclass_dia);
}

static void wda_enable_set_focus( Wda *wda)
{
  wda->set_focus_disabled--;
}

static void wda_disable_set_focus( Wda *wda, int time)
{
  wda->set_focus_disabled++;
  wda->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( wda->toplevel), time,
	(XtTimerCallbackProc)wda_enable_set_focus, wda);
}

static void wda_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Wda *wda;

  XtSetArg    (args[0], XmNuserData, &wda);
  XtGetValues (w, args, 1);

  if ( !wda)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( wda->set_focus_disabled)
    return;

  if ( flow_IsManaged( wda->cmd_scrolledinput))
    flow_SetInputFocus( wda->cmd_scrolledinput);
  else if ( flow_IsManaged( wda->cmd_input))
    flow_SetInputFocus( wda->cmd_input);
  else if ( wda->wdanav)
    ((WdaNav *)wda->wdanav)->set_inputfocus();

  wda_disable_set_focus( wda, 400);

}

static void wda_valchanged_cmd_input( Widget w, XEvent *event)
{
  Wda 	*wda;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &wda);
  XtGetValues(w, args, 1);

  sts = mrm_TextInput( w, event, (char *)Wda::value_recall, sizeof(Wda::value_recall[0]),
	sizeof( Wda::value_recall)/sizeof(Wda::value_recall[0]),
	&wda->value_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( wda->input_open)
    {
      sts = ((WdaNav *)wda->wdanav)->set_attr_value( wda->input_node, 
		wda->input_name, text);
      XtUnmanageChild( w);
      wda->set_prompt( "");
      wda->input_open = 0;
      if ( wda->redraw_cb)
        (wda->redraw_cb)( wda);

      ((WdaNav *)wda->wdanav)->set_inputfocus();
    }
  }
}

void Wda::change_value_close()
{
  char *text;
  int sts;

  text = XmTextGetString( cmd_scrolledinput);
  if ( input_open) {
    if ( input_multiline) {
      sts = ((WdaNav *)wdanav)->set_attr_value( input_node,
		input_name, text);
      XtUnmanageChild( cmd_scrolledinput);
      XtUnmanageChild( cmd_scrolled_ok);
      XtUnmanageChild( cmd_scrolled_ca);
      set_prompt( "");
      input_open = 0;

      ((WdaNav *)wdanav)->redraw();
      ((WdaNav *)wdanav)->set_inputfocus();
    }
    else {
      text = XmTextGetString( cmd_input);

      sts = ((WdaNav *)wdanav)->set_attr_value( input_node, 
		input_name, text);
      XtUnmanageChild( cmd_input);
      set_prompt( "");
      input_open = 0;
      if ( redraw_cb)
        (redraw_cb)( this);

      ((WdaNav *)wdanav)->set_inputfocus();
    }
  }
}

static void wda_activate_cmd_input( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( w);
  if ( wda->input_open)
  {
    sts = ((WdaNav *)wda->wdanav)->set_attr_value( wda->input_node, 
		wda->input_name, text);
    XtUnmanageChild( w);
    wda->set_prompt( "");
    wda->input_open = 0;
    if ( wda->redraw_cb)
      (wda->redraw_cb)( wda);

    ((WdaNav *)wda->wdanav)->set_inputfocus();
  }
}

static void wda_activate_cmd_scrolled_ok( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{
  char *text;
  int sts;

  text = XmTextGetString( wda->cmd_scrolledinput);
  if ( wda->input_open)
  {
    sts = ((WdaNav *)wda->wdanav)->set_attr_value( wda->input_node,
		wda->input_name, text);
    XtUnmanageChild( wda->cmd_scrolledinput);
    XtUnmanageChild( wda->cmd_scrolled_ok);
    XtUnmanageChild( wda->cmd_scrolled_ca);
    wda->set_prompt( "");
    wda->input_open = 0;

    ((WdaNav *)wda->wdanav)->redraw();
    ((WdaNav *)wda->wdanav)->set_inputfocus();
  }
}

static void wda_activate_cmd_scrolled_ca( Widget w, Wda *wda, XmAnyCallbackStruct *data)
{

  if ( wda->input_open)
  {
    XtUnmanageChild( wda->cmd_scrolledinput);
    XtUnmanageChild( wda->cmd_scrolled_ok);
    XtUnmanageChild( wda->cmd_scrolled_ca);
    wda->set_prompt( "");
    wda->input_open = 0;
    ((WdaNav *)wda->wdanav)->set_inputfocus();
  }
}

void Wda::set_editmode( int editmode, ldh_tSesContext ldhses)
{
  this->ldhses = ldhses;
  this->editmode = editmode;
  ((WdaNav *)wdanav)->set_editmode( editmode, ldhses);
}

int Wda::open_changevalue( char *name)
{
  int sts;

  sts = ((WdaNav*)wdanav)->select_by_name( name);
  if ( EVEN(sts)) return sts;

  change_value(0);
  return WDA__SUCCESS;
}

void Wda::pop()
{
  flow_UnmapWidget( parent_wid);
  flow_MapWidget( parent_wid);
}

void Wda::open_class_dialog()
{
  char hierstr[120];
  char classstr[80];
  int size;
  int sts;
  char *s;

  if ( cdh_ObjidIsNotNull( objid)) {
    sts = ldh_ObjidToName( ldhses, objid, ldh_eName_Hierarchy,
		hierstr, sizeof(hierstr), &size);
    if ( EVEN(sts))
      strcpy( hierstr, "");
  }
  else
    strcpy( hierstr, "");
  
  if ( classid != 0) {
    sts = ldh_ClassIdToName( ldhses, classid, classstr, sizeof(classstr),
			     &size);
    if ( EVEN(sts))
      strcpy( classstr, "");
    else if ( (s = strchr( classstr, '-')))
      strcpy( classstr, s + 1);
  }
  else
    strcpy( classstr, "");

  XmTextSetString( wdaclass_hiervalue, hierstr);
  XmTextSetString( wdaclass_classvalue, classstr);

  XtManageChild( wdaclass_dia);
}

void Wda::open_attr_dialog()
{
  // Get all attributes for this class
  int	i, j;
  char	body[20];
  ldh_sParDef 	*bodydef;
  int sts;
  int rows;
  int attr_cnt;
  char (*attr_vect)[80];

  // Count the attributes
  attr_cnt = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++)
      attr_cnt++;

    free((char *) bodydef);
  }

  attr_vect = (char (*)[80]) calloc( attr_cnt + 1, 80);

  attr_cnt = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++)
      strcpy( attr_vect[attr_cnt++], bodydef[j].ParName);

    free((char *) bodydef);
  }
  strcpy( attr_vect[attr_cnt], "");


  wow_CreateList( parent_wid, "Attributes", (char *)attr_vect,
		  wda_set_attr_cb, (void *)this);

}

int Wda::next_attr()
{
  // Get all attributes for this class
  int	i, j;
  char	body[20];
  ldh_sParDef 	*bodydef;
  int sts;
  int rows;
  int get_next;

  if ( strcmp( attribute, "") == 0)
    // Get first attribute
    get_next = 1;
  else
    get_next = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++) {
      if ( cdh_NoCaseStrcmp( attribute, bodydef[j].ParName) == 0)
        get_next = 1;
      else if ( get_next) {
        strcpy( attribute, bodydef[j].ParName);
        sts = ((WdaNav *)wdanav)->update( objid, classid, attribute);
        free((char *) bodydef);
        return WDA__SUCCESS;
      }
    }
    free((char *) bodydef);
  }
  return WDA__NONEXTATTR;
}

int Wda::prev_attr()
{
  // Get all attributes for this class
  int	i, j;
  char	body[20];
  ldh_sParDef 	*bodydef;
  int sts;
  int rows;
  int get_last;
  char prev_attr[40];

  strcpy( prev_attr, "");
  if ( strcmp( attribute, "") == 0)
    // Get last attribute
    get_last = 1;
  else
    get_last = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++) {
      if ( cdh_NoCaseStrcmp( attribute, bodydef[j].ParName) == 0) {
        if ( strcmp( prev_attr, "") == 0) {
          // get_last = 1;
          free((char *) bodydef);
          return WDA__NOPREVATTR;
        }
        else {
          strcpy( attribute, prev_attr);
          sts = ((WdaNav *)wdanav)->update( objid, classid, attribute);
          free((char *) bodydef);
          return WDA__SUCCESS;
        }
      }
      strcpy( prev_attr, bodydef[j].ParName);
    }
    free((char *) bodydef);
  }

  if ( get_last && strcmp( prev_attr, "") != 0) {
    strcpy( attribute, prev_attr);
    sts = ((WdaNav *)wdanav)->update( objid, classid, attribute);
    return WDA__SUCCESS;
  }
  return WDA__NOPREVATTR;
}

static void wda_set_attr_cb( void *ctx, char *text)
{
  Wda *wda = (Wda *)ctx;
  int sts;

  strcpy( wda->attribute, text);
  sts = ((WdaNav *)wda->wdanav)->update( wda->objid, wda->classid,
		wda->attribute);
  if ( EVEN(sts))
    wow_DisplayError( wda->parent_wid, "Spreadsheet error",
		      wnav_get_message( sts));
}

Wda::~Wda()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  delete (WdaNav *)wdanav;
  XtDestroyWidget( parent_wid);
}

Wda::Wda( 
	Widget 		wa_parent_wid,
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses, 
	pwr_tObjid 	wa_objid,
	pwr_tClassId 	wa_classid,
	char            *wa_attribute,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname) :
 	parent_wid(wa_parent_wid), parent_ctx(wa_parent_ctx), 
	ldhses(wa_ldhses), objid(wa_objid), classid(wa_classid),
	editmode(wa_editmode), 
	input_open(0), input_multiline(0), 
	close_cb(0), redraw_cb(0), client_data(0),
	set_focus_disabled(0)
{
  char		uid_filename[120] = {"pwr_exe:wb_wda.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[] = "Proview/R Navigator";

  static char translations[] =
    "<FocusIn>: wda_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"wda_inputfocus",      (XtActionProc) wda_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "wda_ctx", 0 },
	{"wda_activate_setclass",(caddr_t)wda_activate_setclass },
	{"wda_activate_setattr",(caddr_t)wda_activate_setattr },
	{"wda_activate_nextattr",(caddr_t)wda_activate_nextattr },
	{"wda_activate_prevattr",(caddr_t)wda_activate_prevattr },
	{"wda_activate_exit",(caddr_t)wda_activate_exit },
	{"wda_activate_print",(caddr_t)wda_activate_print },
	{"wda_activate_change_value",(caddr_t)wda_activate_change_value },
	{"wda_activate_close_changeval",(caddr_t)wda_activate_close_changeval },
	{"wda_activate_help",(caddr_t)wda_activate_help },
	{"wda_create_msg_label",(caddr_t)wda_create_msg_label },
	{"wda_create_cmd_prompt",(caddr_t)wda_create_cmd_prompt },
	{"wda_create_cmd_input",(caddr_t)wda_create_cmd_input },
	{"wda_create_cmd_scrolledinput",(caddr_t)wda_create_cmd_scrolledinput },
	{"wda_create_cmd_scrolled_ok",(caddr_t)wda_create_cmd_scrolled_ok },
	{"wda_create_cmd_scrolled_ca",(caddr_t)wda_create_cmd_scrolled_ca },
	{"wda_create_wdanav_form",(caddr_t)wda_create_wdanav_form },
	{"wda_activate_cmd_scrolledinput",(caddr_t)wda_activate_cmd_input },
	{"wda_activate_cmd_scrolled_ok",(caddr_t)wda_activate_cmd_scrolled_ok },
	{"wda_activate_cmd_scrolled_ca",(caddr_t)wda_activate_cmd_scrolled_ca },
	{"wdaclass_activate_ok",(caddr_t)wdaclass_activate_ok },
	{"wdaclass_activate_cancel",(caddr_t)wdaclass_activate_cancel },
	{"wdaclass_create_hiervalue",(caddr_t)wdaclass_create_hiervalue },
	{"wdaclass_create_classvalue",(caddr_t)wdaclass_create_classvalue }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  // for ( i = 0; i < int(sizeof(value_recall)/sizeof(value_recall[0])); i++)
  //  value_recall[i][0] = 0;

  strcpy( attribute, wa_attribute);
  dcli_translate_filename( uid_filename, uid_filename);

  // Create object context
//  attrctx->close_cb = close_cb;
//  attrctx->redraw_cb = redraw_cb;

  // Motif
  MrmInitialize();

  strcpy( title, "PwR Object attributes");

  reglist[0].value = (caddr_t) this;

  // Save the context structure in the widget
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid = XtCreatePopupShell("spreadSheetEditor", 
		topLevelShellWidgetClass, parent_wid, args, 0);

  sts = MrmFetchWidgetOverride( s_DRMh, "wda_window", parent_wid,
			name, args, 1, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  sts = MrmFetchWidget(s_DRMh, "wdaclass_dia", parent_wid,
		&wdaclass_dia, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch class input dialog\n");

  MrmCloseHierarchy(s_DRMh);


  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(toplevel), 
						actions, XtNumber(actions));
 
  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( toplevel, compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,800);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);
  XtUnmanageChild( cmd_input);
  XtUnmanageChild( cmd_scrolledinput);
  XtUnmanageChild( cmd_scrolled_ok);
  XtUnmanageChild( cmd_scrolled_ca);

  utility = *(wb_eUtility *)parent_ctx;
  wdanav = new WdaNav( (void *)this, wdanav_form, "Plant",
		ldhses, objid, classid, attribute, wa_editmode,
		wa_advanced_user,
		wa_display_objectname, utility, &brow_widget, &sts);
  ((WdaNav *)wdanav)->message_cb = &wda_message_cb;
  ((WdaNav *)wdanav)->change_value_cb = &wda_change_value_cb;

  XtPopup( parent_wid, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid, 
	(XtCallbackProc)wda_activate_exit, this);

  if ( utility == wb_eUtility_Wtt)
  {
    ((Wtt *)parent_ctx)->register_utility( (void *) this,
	wb_eUtility_AttributeEditor);
  }
}

