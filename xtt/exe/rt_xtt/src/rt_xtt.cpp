/* rt_xtt.cpp -- Display plant and node hiererachy

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
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
#include "xtt_hotkey.h"
#include "co_dcli.h"
#include "co_mrm_util.h"
}

#include "co_lng.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "rt_xtt.h"
#include "rt_trace.h"
#include "xtt_xnav.h"
#include "xtt_item.h"
#include "co_lng.h"
#include "rt_xnav_msg.h"

//
//
//
static void xtt_hotkey_Command( Widget w, XKeyEvent* ev, String* av, Cardinal* ac);
static void xtt_hotkey_ToggleDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac);
static void xtt_hotkey_SetDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac);
static void xtt_hotkey_ResetDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac);

// Static variables
static String   fbr[] = {
    NULL};

static XtActionsRec HotkeyActions[] = {
	{ "Command",	(XtActionProc) xtt_hotkey_Command},
	{ "ToggleDig",	(XtActionProc) xtt_hotkey_ToggleDig},
	{ "SetDig",	(XtActionProc) xtt_hotkey_SetDig},
	{ "ResetDig",	(XtActionProc) xtt_hotkey_ResetDig}
    };

static xnav_sStartMenu alarm_menu[] = {
	{ "Alarm list", xnav_eItemType_Command,	(void *) "show alarm"},
	{ "Event list", xnav_eItemType_Command,	(void *) "show event"},
	{ "Historical list", xnav_eItemType_Command,	(void *) "show hist"},
	{ "", 0, NULL}};
static xnav_sStartMenu nethandler_menu[] = {
	{ "Link", xnav_eItemType_Command, (void *) "show link"},
	{ "Subscription Client", xnav_eItemType_Command, (void *) "show subcli"},
	{ "Subscription Server", xnav_eItemType_Command, (void *) "show subsrv"},
	{ "", 0, NULL}};
static xnav_sStartMenu communication_menu[] = {
	{ "RemNode", xnav_eItemType_Command, (void *) "show remnode"},
	{ "RemTrans", xnav_eItemType_Command, (void *) "show remtrans"},
	{ "", 0, NULL}};
static xnav_sStartMenu logging_menu[] = {
	{ "Logging entry 1", xnav_eItemType_Command, (void *) "show logging/entry=1"},
	{ "Logging entry 2", xnav_eItemType_Command, (void *) "show logging/entry=2"},
	{ "Logging entry 3", xnav_eItemType_Command, (void *) "show logging/entry=3"},
	{ "Logging entry 4", xnav_eItemType_Command, (void *) "show logging/entry=4"},
	{ "Logging entry 5", xnav_eItemType_Command, (void *) "show logging/entry=5"},
	{ "Logging entry 6", xnav_eItemType_Command, (void *) "show logging/entry=6"},
	{ "Logging entry 7", xnav_eItemType_Command, (void *) "show logging/entry=7"},
	{ "Logging entry 8", xnav_eItemType_Command, (void *) "show logging/entry=8"},
	{ "Logging entry 9", xnav_eItemType_Command, (void *) "show logging/entry=9"},
	{ "Logging entry 10", xnav_eItemType_Command, (void *) "show logging/entry=10"},
	{ "", 0, NULL}};
static xnav_sStartMenu system_menu[] = {
	{ "Nethandler", xnav_eItemType_Menu, 	(void *)&nethandler_menu},
	{ "Communication", xnav_eItemType_Menu, (void *)&communication_menu},
	{ "Device", 	xnav_eItemType_Command, (void *) "show device"},
	{ "PlcThread", 	xnav_eItemType_Command, (void *) "show plcthread"},
	{ "PlcPgm", 	xnav_eItemType_Command,	(void *) "show plcpgm"},
	{ "Logging", 	xnav_eItemType_Menu,	(void *)&logging_menu},
	{ "", 0, NULL}};
static xnav_sStartMenu root_menu[] = {
	{ "Database", 	xnav_eItemType_Command, (void *) "show database"},
	{ "Alarm", 	xnav_eItemType_Menu, 	(void *)&alarm_menu},
	{ "Store",	xnav_eItemType_Command, (void *) "show file"},
	{ "System",	xnav_eItemType_Menu, 	(void *)&system_menu},
	{ "Exit", 	xnav_eItemType_Command,	(void *) "exit"},
	{ "", 0, NULL}};
  


typedef struct xtt_sCtx {
  void 		*parent_ctx;
  Widget	parent_wid;
  char 		name[80];
  Widget	brow_widget;
  Widget	form_widget;
  Widget	toplevel;
  XNav		*xnav;
  void		*root_item;
  Widget	msg_label;
  Widget	cmd_prompt;
  Widget	cmd_input;
  Widget	xnav_form;
  int		input_open;
  int		command_open;
  char		cmd_recall[30][160];
  int		cmd_current_recall;
  char		value_recall[30][160];
  int		value_current_recall;
  Widget	india_widget;
  Widget	india_label;
  Widget	india_text;
  void		(*india_ok_cb)( struct xtt_sCtx *, char *);
} *xtt_tCtx;

static Xtt *hot_xtt;
static hotkeyHandle   HotkeyHandle = NULL;  	    

static void xtt_message( void *ctx, char severity, char *message);
static void xtt_open_change_value( Xtt *xtt);
static void xtt_valchanged_cmd_input( Widget w, XEvent *event);

static void xtt_hotkey_Command( Widget w, XKeyEvent* ev, String* av, Cardinal* ac)
{
  char cmd[200];

  if (*ac == 0)
    return;

  strcpy( cmd, av[0]);
  if ( hot_xtt->xnav)
    hot_xtt->xnav->command( cmd);
}

static void xtt_hotkey_ToggleDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac)
{
  pwr_tBoolean  value;
  char		name[120];
  int		sts;


  if (ev->type != KeyPress)
    return;

  if (*ac == 0)
    return;

  dcli_remove_blank( name, av[0]);
  if ( !strchr(name, '.'))
    strcat( name, ".ActualValue");

  sts = gdh_GetObjectInfo( name, &value, sizeof(value));
  if (EVEN(sts)) 
  {
    printf("rt_xtt hotkey: ToggleDig. Can't get %s\n", name);
    return;
  }

  value = !value;
  sts = gdh_SetObjectInfo( name, &value, sizeof(value));
  if (EVEN(sts))
    printf("rt_xtt hotkey: ToggleDig. Can't get %s\n", name);
}

static void xtt_hotkey_SetDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac)
{
  pwr_tBoolean  value = 1;
  char		name[120];
  int		sts;


  if (ev->type != KeyPress)
    return;

  if (*ac == 0)
    return;

  dcli_remove_blank( name, av[0]);
  if ( !strchr(name, '.'))
    strcat( name, ".ActualValue");

  sts = gdh_SetObjectInfo( name, &value, sizeof(value));
  if (EVEN(sts))
    printf("rt_xtt hotkey: SetDig. Can't get %s\n", name);
}

static void xtt_hotkey_ResetDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac)
{
  pwr_tBoolean  value = 0;
  char		name[120];
  int		sts;


  if (ev->type != KeyPress)
    return;

  if (*ac == 0)
    return;

  dcli_remove_blank( name, av[0]);
  if ( !strchr(name, '.'))
    strcat( name, ".ActualValue");

  sts = gdh_SetObjectInfo( name, &value, sizeof(value));
  if (EVEN(sts))
    printf("rt_xtt hotkey: SetDig. Can't get %s\n", name);
}

static void xtt_mainloop (XtAppContext AppCtx)
{
  XEvent Event;

  for (;;)
  {
    XtAppNextEvent (AppCtx, &Event);

    if (Event.type != KeyPress || 
        TkSUCCESS != hotkey_Process(HotkeyHandle, &Event)) 
      XtDispatchEvent (&Event);
  }
}

static int xtt_init_hotkey( XtAppContext    AppCtx,
			    Widget	    Top)
{
  hotkeySTATUS    HotkeySts;

  XtAppAddActions (AppCtx, HotkeyActions, XtNumber(HotkeyActions));

  HotkeySts = hotkey_Initialize (Top, &HotkeyHandle);
  switch ( HotkeySts )
  {
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

static void xtt_findregex_ok( Xtt *xtt, char *search_str)
{
  int sts;

  sts = xtt->xnav->search( search_str, 1);
  if ( EVEN(sts))
    xtt_message( xtt, 'E', xtt->xnav->get_message( sts));
}

static void xtt_find_ok( Xtt *xtt, char *search_str)
{
  int sts;

  sts = xtt->xnav->search( search_str, 0);
  if ( EVEN(sts))
    xtt_message( xtt, 'E', xtt->xnav->get_message( sts));
}


static void xtt_open_input_dialog( Xtt *xtt, char *text, char *title,
	char *init_text,
	void (*ok_cb)( Xtt *, char *))
{
  Arg		args[10];
  int 		i;
  XmString	cstr;

  i = 0;
  XtSetArg(args[0], XmNlabelString,
		cstr=XmStringCreateLtoR( text, "ISO8859-1") ); i++;
  XtSetValues( xtt->india_label, args, i);
  XmStringFree( cstr);
  i = 0;
  XtSetArg(args[0], XmNdialogTitle,
		cstr=XmStringCreateLtoR( title, "ISO8859-1") ); i++;
  XtSetValues( xtt->india_widget, args, i);
  XmStringFree( cstr);

  XmTextSetString( xtt->india_text, init_text);

  XtManageChild( xtt->india_widget);

  XmProcessTraversal( xtt->india_text, XmTRAVERSE_CURRENT);

  xtt->india_ok_cb = ok_cb;
}

static void xtt_message( void *ctx, char severity, char *message)
{
  Xtt	        *xtt = (Xtt *) ctx;
  Arg 		args[2];
  XmString	cstr;

  cstr=XmStringCreateLtoR( message, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetValues( xtt->msg_label, args, 1);
  XmStringFree( cstr);
}

static void xtt_close( void *ctx)
{
  Xtt	*xtt = (Xtt *) ctx;
  if ( !xtt->xnav->op)
  {
    delete xtt->xnav;
    exit(0);
  }
  else
  {
    xtt->xnav->displayed = 0;
    flow_UnmapWidget( xtt->toplevel);
  }
}

static  void xtt_set_dimension( void *ctx, int width, int height)
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
  XtSetValues( xtt->toplevel, args,i);
}

static void xtt_map( void *ctx)
{
  Xtt	*xtt = (Xtt *) ctx;
  if ( !xtt->xnav->displayed)
  {
    flow_MapWidget( xtt->toplevel);
    xtt->xnav->displayed = 1;
  }
  else
    xtt->xnav->pop();
}

static void xtt_change_value( void *ctx)
{
  Xtt *xtt = (Xtt *) ctx;

  xtt_open_change_value( xtt);
}

static void xtt_set_prompt( Xtt *xtt, char *prompt)
{
  Arg 		args[1];
  XmString	cstr;

  cstr=XmStringCreateLtoR( prompt, "ISO8859-1");
  XtSetArg(args[0],XmNlabelString, cstr);
  XtSetValues( xtt->cmd_prompt, args, 1);
  XmStringFree( cstr);
}

static void xtt_open_change_value( Xtt *xtt)
{
  Arg 		args[1];
  int		sts;

  if ( xtt->input_open) 
  {
    XtUnmanageChild( xtt->cmd_input);
    xtt_set_prompt( xtt, "");
    xtt->input_open = 0;
    return;
  }

  sts = xtt->xnav->check_attr_value();
  if ( EVEN(sts))
  {
    xtt_message( xtt, 'E', XNav::get_message(sts));
    return;
  }

  if ( xtt->command_open)
    xtt->command_open = 0;
  else
    XtManageChild( xtt->cmd_input);

  xtt_message( xtt, ' ', "");
  XtSetKeyboardFocus( xtt->toplevel, xtt->cmd_input);

  XtSetArg(args[0],XmNvalue, "");
  XtSetValues( xtt->cmd_input, args, 1);
  xtt_set_prompt( xtt, Lng::translate("value >"));
  xtt->input_open = 1;
}

//
//  Callbackfunctions from menu entries
//
static void xtt_activate_change_value( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt_open_change_value( xtt);
}

static void xtt_activate_command( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  Arg 		args[1];

  if ( xtt->command_open) 
  {
    XtUnmanageChild( xtt->cmd_input);
    xtt_set_prompt( xtt, "");
    xtt->command_open = 0;
    return;
  }

  if ( xtt->input_open)
    xtt->input_open = 0;
  else
    XtManageChild( xtt->cmd_input);
  xtt_message( xtt, ' ', "");
  XtSetKeyboardFocus( xtt->toplevel, xtt->cmd_input);

  XtSetArg(args[0],XmNvalue, "");
  XtSetValues( xtt->cmd_input, args, 1);
  xtt_set_prompt( xtt, "xtt >");
  xtt->command_open = 1;
}

static void xtt_activate_exit( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt_close( xtt);
}

static void xtt_activate_print( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->print( "pwrp_tmp:xnav.ps");
}

static void xtt_activate_find( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt_open_input_dialog( xtt, "Search string", "Find object",
	"", &xtt_find_ok);
}

static void xtt_activate_findregex( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
   xtt_open_input_dialog( xtt, "Search string", "Find regular expression",
	"", &xtt_findregex_ok);
}

static void xtt_activate_findnext( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  int sts;

  sts = xtt->xnav->search_next();
  if ( EVEN(sts))
    xtt_message( xtt, 'E', xtt->xnav->get_message( sts));
}

static void xtt_activate_collapse( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->brow_push_all();
}

static void xtt_activate_openobject( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  int		sts;
  int		is_attr;
  pwr_sAttrRef	attrref;

  sts = xtt->xnav->get_select( &attrref, &is_attr);
  if ( ODD(sts))
    xtt->xnav->open_object( attrref.Objid);
}

static void xtt_activate_openplc( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
    xtt->xnav->start_trace_selected();
}

static void xtt_activate_showcrossref( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
    xtt->xnav->show_crossref();
}

static void xtt_activate_opengraph( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  int		sts;
  pwr_tClassId	classid;
  char		name[120];
  char		classname[80];
  char		filename[120];
  char		fname[120];
  char		found_file[120];
  int		is_attr;
  pwr_sAttrRef	attrref;
  char		cmd[200];

  sts = xtt->xnav->get_select( &attrref, &is_attr);
  if ( EVEN(sts))
  {
    // If collect brow open collect graph
    if ( xtt->xnav->brow->ctx == xtt->xnav->collect_brow->ctx)
    {   
      sprintf( cmd, "open graph /collect");
      xtt->xnav->command( cmd);
    }
    return;
  }
  if ( !is_attr)
  {
    sts = gdh_ObjidToName( attrref.Objid,
		  name, sizeof(name), cdh_mNName);
    if ( EVEN(sts)) return;

    sts = gdh_GetObjectClass( attrref.Objid, &classid);
    if ( EVEN(sts)) return;

    if ( classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup) {
      // Open trend
      sprintf( cmd, "open trend /name=%s /title=\"%s\"", name, name);
      xtt->xnav->command( cmd);

      return;
    }

    sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
		  classname, sizeof(classname), cdh_mName_object);
    if ( EVEN(sts)) return;
    cdh_ToLower( classname, classname);

    sprintf( filename, "pwr_exe:pwr_c_%s.pwg", classname);
    dcli_translate_filename( fname, filename);
    sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
    dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
    if ( EVEN(sts))
    {
      sprintf( filename, "pwrp_exe:%s.pwg", classname);
      dcli_translate_filename( fname, filename);
      sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
      dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
      if ( EVEN(sts))
      {
        xtt_message( xtt, 'E', "Can't find graph for this object");
        return;
      }
    }
    sprintf( cmd, "open graph %s/inst=%s/name=\"%s\"", 
           filename, name, name);

    // Add scrollbars for some classes
    switch ( classid) {
      case pwr_cClass_NMpsCell:
      case pwr_cClass_NMpsStoreCell:
        strcat( cmd, "/sc");
        break;
      default:
        ;
    }
    xtt->xnav->command( cmd);
  }
  else
  {
    pwr_tTypeId		attr_type;
    unsigned int	attr_size;
    unsigned int	attr_offset;
    unsigned int	attr_dimension;

    sts = gdh_AttrrefToName( &attrref, name, sizeof(name), cdh_mNName);
    if ( EVEN(sts)) return;

    sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
    if ( EVEN(sts)) {
      // Try volume strict name
      sts = gdh_AttrrefToName( &attrref, name, sizeof(name), 
          cdh_mName_volumeStrict);
      if ( EVEN(sts)) return;

      sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
      if ( EVEN(sts)) return;
    }
    
    switch( attr_type)
    {
      case pwr_eType_Float32:
        sprintf( filename, "pwr_exe:pwr_t_float32.pwg");
        break;
      case pwr_eType_Boolean:
        sprintf( filename, "pwr_exe:pwr_t_boolean.pwg");
        break;
      default:
        xtt_message( xtt, 'E', "No graph for this attribute type");
        return;
    }
    sprintf( cmd, "open graph %s/inst=%s/name=\"%s\"", filename, name, name);
    xtt->xnav->command( cmd);
  }
}

static void xtt_activate_collect_insert( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  pwr_sAttrRef	attrref;
  int		is_attr;
  int		sts;

  sts = xtt->xnav->get_select( &attrref, &is_attr);
  if ( ODD(sts))
    xtt->xnav->collect_insert( &attrref);
}

static void xtt_activate_collect_show( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->collect_show();
}

static void xtt_activate_advanceduser( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  char cmd[80];

  strcpy( cmd, "set advanceduser");
  xtt->xnav->command( cmd);
}

static void xtt_activate_zoom_in( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  xtt->xnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

   xtt->xnav->zoom( 1.18);
}

static void xtt_activate_zoom_out( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  double zoom_factor;

  xtt->xnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  xtt->xnav->zoom( 1.0 / 1.18);
}

static void xtt_activate_zoom_reset( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->unzoom();
}

static void xtt_activate_help( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->open_help();
  xtt->xnav->xhelp->help("overview", "", navh_eHelpFile_Base, NULL);
}

static void xtt_activate_help_project( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav->open_help();
  xtt->xnav->xhelp->help("index", "", navh_eHelpFile_Project, NULL);
}

static void xtt_create_msg_label( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->msg_label = w;
}
static void xtt_create_cmd_prompt( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->cmd_prompt = w;
}
static void xtt_create_cmd_input( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  Arg args[2];

  XtSetArg    (args[0], XmNuserData, xtt);
  XtSetValues (w, args, 1);

  mrm_TextInit( w, (XtActionProc) xtt_valchanged_cmd_input, mrm_eUtility_Xtt);

  xtt->cmd_input = w;
}
static void xtt_create_xnav_form( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->xnav_form = w;
}
static void xtt_create_india_label( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->india_label = w;
}
static void xtt_create_india_text( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  xtt->india_text = w;
}
static void xtt_activate_india_ok( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  char *value;

  value = XmTextGetString( xtt->india_text);
  XtUnmanageChild( xtt->india_widget);

  (xtt->india_ok_cb)( xtt, value);
}
static void xtt_activate_india_cancel( Widget w, Xtt *xtt, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( xtt->india_widget);
}
static void xtt_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Xtt *xtt;

  XtSetArg    (args[0], XmNuserData, &xtt);
  XtGetValues (w, args, 1);

  if ( xtt->xnav) {
    xtt->xnav->set_inputfocus();
  }
}

static void xtt_valchanged_cmd_input( Widget w, XEvent *event)
{
  Xtt 	*xtt;
  int 	sts;
  char 	*text;
  Arg 	args[2];

  XtSetArg(args[0], XmNuserData, &xtt);
  XtGetValues(w, args, 1);

  if ( xtt->input_open)
    sts = mrm_TextInput( w, event, (char *)xtt->value_recall, sizeof(xtt->value_recall[0]),
	sizeof( xtt->value_recall)/sizeof(xtt->value_recall[0]),
	&xtt->value_current_recall);
  else
    sts = mrm_TextInput( w, event, (char *)xtt->cmd_recall, sizeof(xtt->cmd_recall[0]),
	sizeof( xtt->cmd_recall)/sizeof(xtt->cmd_recall[0]),
	&xtt->cmd_current_recall);
  if ( sts)
  {
    text = XmTextGetString( w);
    if ( xtt->input_open)
    {
      sts = xtt->xnav->set_attr_value( text);
      XtUnmanageChild( w);
      xtt_set_prompt( xtt, "");
      xtt->input_open = 0;
    }
    else if ( xtt->command_open)
    {
      sts = xtt->xnav->command( text);
      XtUnmanageChild( w);
      xtt_set_prompt( xtt, "");
      xtt->command_open = 0;
    }
  }
}


int main(  int argc, char *argv[])
{
  int sts;

  new Xtt( argc, argv, &sts);
  exit(sts);
}

Xtt::Xtt( int argc, char *argv[], int *return_sts) :
  root_item(0), input_open(0), command_open(0), cmd_current_recall(0), 
  value_current_recall(0), india_ok_cb(0)
{
  char		uid_filename[120] = {"xtt.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[80];
  int		i;
  XtAppContext  app_ctx;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	xtt_widget;
  char		name[] = "PwR Xtt";

  static char translations[] =
    "<FocusIn>: xtt_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"xtt_inputfocus",      (XtActionProc) xtt_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "xtt_ctx", 0 },
	{"xtt_activate_exit",(caddr_t)xtt_activate_exit },
	{"xtt_activate_print",(caddr_t)xtt_activate_print },
	{"xtt_activate_find",(caddr_t)xtt_activate_find },
	{"xtt_activate_findregex",(caddr_t)xtt_activate_findregex },
	{"xtt_activate_findnext",(caddr_t)xtt_activate_findnext },
	{"xtt_activate_collapse",(caddr_t)xtt_activate_collapse },
	{"xtt_activate_openobject",(caddr_t)xtt_activate_openobject },
	{"xtt_activate_openplc",(caddr_t)xtt_activate_openplc },
	{"xtt_activate_opengraph",(caddr_t)xtt_activate_opengraph },
	{"xtt_activate_showcrossref",(caddr_t)xtt_activate_showcrossref },
	{"xtt_activate_change_value",(caddr_t)xtt_activate_change_value },
	{"xtt_activate_command",(caddr_t)xtt_activate_command },
	{"xtt_activate_collect_insert",(caddr_t)xtt_activate_collect_insert },
	{"xtt_activate_collect_show",(caddr_t)xtt_activate_collect_show },
	{"xtt_activate_advanceduser",(caddr_t)xtt_activate_advanceduser },
	{"xtt_activate_zoom_in",(caddr_t)xtt_activate_zoom_in },
	{"xtt_activate_zoom_out",(caddr_t)xtt_activate_zoom_out },
	{"xtt_activate_zoom_reset",(caddr_t)xtt_activate_zoom_reset },
	{"xtt_activate_help",(caddr_t)xtt_activate_help },
	{"xtt_activate_help_project",(caddr_t)xtt_activate_help_project },
	{"xtt_create_msg_label",(caddr_t)xtt_create_msg_label },
	{"xtt_create_cmd_prompt",(caddr_t)xtt_create_cmd_prompt },
	{"xtt_create_cmd_input",(caddr_t)xtt_create_cmd_input },
	{"xtt_create_xnav_form",(caddr_t)xtt_create_xnav_form },
	{"xtt_activate_india_ok",(caddr_t)xtt_activate_india_ok },
	{"xtt_activate_india_cancel",(caddr_t)xtt_activate_india_cancel },
	{"xtt_create_india_label",(caddr_t)xtt_create_india_label },
	{"xtt_create_india_text",(caddr_t)xtt_create_india_text }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);
  char	opplace_str[80] = "";
  int	opplace_found = 0;

  hot_xtt = this;

  sts = gdh_Init("rt_xtt");
  if (EVEN(sts)) {
    *return_sts = sts;
    return;
  }

  // Set language
  for ( i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-l") == 0 && i + 1 < argc)
      Lng::set( argv[i+1]);
  }

  if ( argc >= 2 && strncmp( argv[1], "-", 1) != 0)
  {
    pwr_tObjid op_objid;
    pwr_tClassId op_class;

    // First argument is opplace object
    strcpy( opplace_str, argv[1]);

    sts = gdh_NameToObjid( opplace_str, &op_objid);
    if ( EVEN(sts))
    {
      printf("** Unable to find opplace\n");
      exit(sts);
    }

    sts = gdh_GetObjectClass( op_objid, &op_class);
    if ( EVEN(sts)) exit( sts);

    if ( op_class != pwr_cClass_OpPlace)    
    {
      printf("** Error in opplace object class\n");
      exit(sts);
    }

    opplace_found = 1;
  }

  // Motif
  Lng::get_uid( uid_filename, uid_filename);
  MrmInitialize();

  strcpy( title, "PwR Xtt");

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

  sts = MrmFetchWidgetOverride( s_DRMh, "xtt_window", toplevel,
			name, args, i, &xtt_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  sts = MrmFetchWidget(s_DRMh, "input_dialog", toplevel,
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

  xnav = new XNav( this, xnav_form, "Plant",
		&brow_widget, (xnav_sStartMenu *)root_menu, 
		opplace_str, &sts);
  xnav->message_cb = &xtt_message;
  xnav->close_cb = &xtt_close;
  xnav->map_cb = &xtt_map;
  xnav->change_value_cb = &xtt_change_value;
  xnav->set_dimension_cb = &xtt_set_dimension;

  XtRealizeWidget( toplevel);

//  XmProcessTraversal( xnav->brow_widget, XmTRAVERSE_CURRENT);
//  xnav->set_inputfocus();

  xtt_init_hotkey( app_ctx, toplevel);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)xtt_activate_exit, this);

  if ( xnav->op)
    xtt_close( this);

  xtt_mainloop( app_ctx);  
}

