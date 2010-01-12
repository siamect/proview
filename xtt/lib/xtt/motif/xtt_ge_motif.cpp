/* 
 * Proview   $Id: xtt_ge_motif.cpp,v 1.3 2008-10-31 12:51:36 claes Exp $
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

#include "glow_std.h"
#include "pwr.h"
#include "pwr_baseclasses.h"

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Xm/DialogS.h>
#include <Xm/MessageB.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
//#include "glow_colpalctx.h"
//#include "glow_colpalapi.h"
//#include "glow_colpalwidget_motif.h"

extern "C" {
#include "flow_x.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "cow_mrm_util.h"
}

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_motif.h"
#include "co_lng.h"
#include "xtt_ge_motif.h"
#include "ge_graph_motif.h"
#include "xtt_xnav.h"

//
//  Test program
//
//

// Static variables

void XttGeMotif::enable_set_focus( XttGeMotif *ge)
{
  ge->set_focus_disabled--;
}

void XttGeMotif::disable_set_focus( XttGeMotif *ge, int time)
{
  ge->set_focus_disabled++;
  ge->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( ge->toplevel), time,
	(XtTimerCallbackProc)enable_set_focus, ge);
}

void XttGeMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  XttGeMotif *ge;

  XtSetArg    (args[0], XmNuserData, &ge);
  XtGetValues (w, args, 1);

  if ( !ge)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( ge->set_focus_disabled)
    return;

  if ( ge->graph)
    ge->graph->set_inputfocus(1);

  disable_set_focus( ge, 400);

}

void XttGeMotif::set_size( int width, int height)
{
  short 	x1, x2, y1, y2;
  int		default_width;
  int		default_height;
  int		i;
  Arg 		args[7];

  default_width = width + 20;
  default_height = height + 20;
  i = 0;
  XtSetArg(args[i],XmNwidth, default_width);i++;
  XtSetArg(args[i],XmNheight, default_height);i++;

  x1 = default_width;
  y1 = default_height;
  x2 = default_width;
  y2 = default_height;
  // This condition is due to a bug in Reflection X 11.0.5...
  if ( !((XNav *)parent_ctx)->gbl.no_graph_ratio) {
    XtSetArg(args[i], XmNminAspectX, x1); i++;
    XtSetArg(args[i], XmNminAspectY, y1); i++;
    XtSetArg(args[i], XmNmaxAspectX, x2); i++;
    XtSetArg(args[i], XmNmaxAspectY, y2); i++;
  }  
  XtSetValues( toplevel, args,i);
}

void XttGeMotif::ge_change_value_cb( void *ge_ctx, void *value_object, char *text)
{
  Arg 		args[1];
  XttGe	*ge = (XttGe*)ge_ctx;

  if ( ge->value_input_open) 
  {
    XtUnmanageChild( ((XttGeMotif *)ge)->value_dialog);
    ge->value_input_open = 0;
    return;
  }

  XtManageChild( ((XttGeMotif *)ge)->value_dialog);

  ge->message( ' ', "");
  XtCallAcceptFocus( ((XttGeMotif *)ge)->value_input, CurrentTime);
  // XtSetKeyboardFocus( ((XttGeMotif *)ge)->toplevel, ((XttGeMotif *)ge)->value_input);

  XtSetArg(args[0],XmNvalue, text);
  XtSetValues( ((XttGeMotif *)ge)->value_input, args, 1);
  XmTextSetCursorPosition( ((XttGeMotif *)ge)->value_input, strlen(text));
  XmTextSetSelection( ((XttGeMotif *)ge)->value_input, 0, strlen(text), CurrentTime);
  ge->value_input_open = 1;
  ge->current_value_object = value_object;
}

void XttGeMotif::confirm_cb( void *ge_ctx, void *confirm_object, char *text)
{
  Arg 		args[1];
  XttGe	*ge = (XttGe *)ge_ctx;

  if ( ge->confirm_open) {
    XtUnmanageChild( ((XttGeMotif *)ge)->confirm_widget);
    ge->confirm_open = 0;
    return;
  }

  XtManageChild( ((XttGeMotif *)ge)->confirm_widget);

  ge->message( ' ', "");

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, (char*) "ISO8859-1"));
  XtSetValues( ((XttGeMotif *)ge)->confirm_widget, args, 1);
  ge->confirm_open = 1;
  ge->current_confirm_object = confirm_object;
}

void XttGeMotif::message_dialog_cb( void *ge_ctx, const char *text)
{
  Arg 		args[1];
  XttGe	*ge = (XttGe *)ge_ctx;

  XtManageChild( ((XttGeMotif *)ge)->message_dia_widget);

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( (char*) text, (char*) "ISO8859-1"));
  XtSetValues( ((XttGeMotif *)ge)->message_dia_widget, args, 1);
}

void XttGeMotif::activate_value_input( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  char *text;

  text = XmTextGetString( w);
  if ( ge->value_input_open)
  {
    ge->graph->change_value( ge->current_value_object, text);
    XtUnmanageChild( ((XttGeMotif *)ge)->value_dialog);
    ge->value_input_open = 0;
  }
}

void XttGeMotif::activate_confirm_ok( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((XttGeMotif *)ge)->confirm_widget);
  ge->confirm_open = 0;
  ge->graph->confirm_ok( ge->current_confirm_object);
}

void XttGeMotif::activate_confirm_cancel( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  ge->confirm_open = 0;
  XtUnmanageChild( ((XttGeMotif *)ge)->confirm_widget);
}

void XttGeMotif::activate_exit( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  delete ge;
}

void XttGeMotif::activate_zoom_in( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  ge->graph->zoom( 1.2);
}

void XttGeMotif::activate_zoom_out( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  ge->graph->zoom( 5.0/6);
}

void XttGeMotif::activate_zoom_reset( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  ge->graph->unzoom();
}

void XttGeMotif::activate_help( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  char key[80];

  if ( ge->help_cb)
  {
    cdh_ToLower( key, ge->name);
    (ge->help_cb)( ge, key);
  }
}

void XttGeMotif::create_graph_form( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  ((XttGeMotif *)ge)->graph_form = w;
}

void XttGeMotif::create_message_dia( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( XmMessageBoxGetChild( w, XmDIALOG_OK_BUTTON));
  XtUnmanageChild( XmMessageBoxGetChild( w, XmDIALOG_HELP_BUTTON));
}

void XttGeMotif::create_menu( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  ((XttGeMotif *)ge)->menu_widget = w;
}

void XttGeMotif::create_value_input( Widget w, XttGe *ge, XmAnyCallbackStruct *data)
{
  ((XttGeMotif *)ge)->value_input = w;
}

void XttGeMotif::action_resize( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  XttGe *ge;

  XtSetArg    (args[0], XmNuserData, &ge);
  XtGetValues (w, args, 1);

  if ( ge->graph && !ge->scrollbar && !ge->navigator)
    ge->graph->set_default_layout();
}

XttGeMotif::~XttGeMotif()
{
  if ( close_cb)
    (close_cb)( this);
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);
  delete graph;
  XtDestroyWidget( toplevel);
}

void XttGeMotif::pop()
{
  flow_UnmapWidget( toplevel);
  flow_MapWidget( toplevel);
}

XttGeMotif::XttGeMotif( Widget xg_parent_wid, void *xg_parent_ctx, const char *xg_name, 
			const char *xg_filename, int xg_scrollbar, int xg_menu, 
			int xg_navigator, int xg_width, int xg_height, int x, int y, 
			double scan_time, const char *object_name,
			int use_default_access, unsigned int access,
			int (*xg_command_cb) (XttGe *, char *),
			int (*xg_get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
			int (*xg_is_authorized_cb) (void *, unsigned int)) :
  XttGe( xg_parent_ctx, xg_name, xg_filename, xg_scrollbar, xg_menu, xg_navigator, xg_width,
	 xg_height, x, y, scan_time, object_name, use_default_access, access,
	 xg_command_cb, xg_get_current_objects_cb, xg_is_authorized_cb), 
  parent_wid(xg_parent_wid), set_focus_disabled(0), focus_timerid(0)
{
  char		uid_filename[120] = {"xtt_ge.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[300];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	ge_widget;
  char		wname[] = "Proview/R Ge";

  static char translations[] =
    "<ConfigureNotify>: resize()\n\
<FocusIn>: ge_xtt_inputfocus()";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {(char*) "ge_xtt_inputfocus",      (XtActionProc) action_inputfocus},
    {(char*) "resize",      (XtActionProc) action_resize},
  };

  static MrmRegisterArg	reglist[] = {
        {(char*) "ge_ctx", 0 },
	{(char*) "ge_activate_exit",(caddr_t)activate_exit },
	{(char*) "ge_activate_zoom_in",(caddr_t)activate_zoom_in },
	{(char*) "ge_activate_zoom_out",(caddr_t)activate_zoom_out },
	{(char*) "ge_activate_zoom_reset",(caddr_t)activate_zoom_reset },
	{(char*) "ge_activate_help",(caddr_t)activate_help },
	{(char*) "ge_create_graph_form",(caddr_t)create_graph_form },
	{(char*) "ge_create_menu",(caddr_t)create_menu },
	{(char*) "ge_create_value_input",(caddr_t)create_value_input },
	{(char*) "ge_activate_value_input",(caddr_t)activate_value_input },
	{(char*) "ge_activate_confirm_ok",(caddr_t)activate_confirm_ok },
	{(char*) "ge_activate_confirm_cancel",(caddr_t)activate_confirm_cancel },
	{(char*) "ge_create_message_dia",(caddr_t)create_message_dia }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  Lng::get_uid( uid_filename, uid_filename);

  // Motif
  MrmInitialize();

  cdh_StrncpyCutOff( title, name, sizeof(title), 1);

  reglist[0].value = (caddr_t) this;


  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  // Save the context structure in the widget
  i = 0;
  XtSetArg(args[i], XmNuserData, (XtPointer) this);i++;
  XtSetArg(args[i],XmNdeleteResponse, XmDO_NOTHING);i++;

  toplevel = XtCreatePopupShell( title, 
				 topLevelShellWidgetClass, parent_wid, args, i);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, (char*) "ge_window", toplevel,
			wname, args, 1, &ge_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", wname);

  sts = MrmFetchWidget(s_DRMh, (char*) "input_dialog", toplevel,
		&value_dialog, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, (char*) "confirm_dialog", toplevel,
		&confirm_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch confirm dialog\n");

  sts = MrmFetchWidget(s_DRMh, (char*) "message_dialog", toplevel,
		&message_dia_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch message dialog\n");

  MrmCloseHierarchy(s_DRMh);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(toplevel), 
						actions, XtNumber(actions));
 
  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( ge_widget, compiled_translations);

  i = 0;
  if ( width == 0 || height == 0)
  {
    XtSetArg(args[i],XmNwidth,600);i++;
    XtSetArg(args[i],XmNheight,500);i++;
  }
  else
  {
    XtSetArg(args[i],XmNwidth,width);i++;
    XtSetArg(args[i],XmNheight,height);i++;
    XtSetArg(args[i], XmNminAspectX, width); i++;
    XtSetArg(args[i], XmNminAspectY, height); i++;
    XtSetArg(args[i], XmNmaxAspectX, width); i++;
    XtSetArg(args[i], XmNmaxAspectY, height); i++;
  }
  if ( !(x == 0 && y == 0))
  {
    XtSetArg(args[i],XmNx, x);i++;
    XtSetArg(args[i],XmNy, y);i++;
  }
  XtSetValues( toplevel, args,i);
    
  XtManageChild( ge_widget);

  if ( !menu)
    XtUnmanageChild( menu_widget);

  graph = new GraphMotif( this, graph_form, "Plant", 
		&grow_widget, &sts, "pwrp_exe:", graph_eMode_Runtime, 
		scrollbar, 1, object_name, use_default_access, access);
//  graph->set_scantime( scan_time);
  graph->message_cb = &message_cb;
  graph->close_cb = &graph_close_cb;
  graph->init_cb = &graph_init_cb;
  graph->change_value_cb = &ge_change_value_cb;
  graph->confirm_cb = &confirm_cb;
  graph->message_dialog_cb = &message_dialog_cb;
  graph->command_cb = &ge_command_cb;
  graph->display_in_xnav_cb = &ge_display_in_xnav_cb;
  graph->is_authorized_cb = &ge_is_authorized_cb;
  graph->get_current_objects_cb = &ge_get_current_objects_cb;
  graph->popup_menu_cb = &ge_popup_menu_cb;
  graph->call_method_cb = &ge_call_method_cb;
  graph->sound_cb = &ge_sound_cb;
 
  XtPopup( toplevel, XtGrabNone);

  if ( navigator)
  {
    // Create navigator popup

    i = 0;
    XtSetArg(args[i],XmNallowShellResize, TRUE); i++;
    XtSetArg(args[i],XmNallowResize, TRUE); i++;
    XtSetArg(args[i],XmNwidth,200);i++;
    XtSetArg(args[i],XmNheight,200);i++;
    XtSetArg(args[i],XmNx,500);i++;
    XtSetArg(args[i],XmNy,500);i++;
    XtSetArg(args[i],XmNdeleteResponse, XmDO_NOTHING);i++;

    nav_shell = XmCreateDialogShell( grow_widget, (char*) "Navigator",
        args, i);
    XtManageChild( nav_shell);

    ((GraphMotif *)graph)->create_navigator( nav_shell);
//    XtManageChild( nav_widget);
    XtRealizeWidget( nav_shell);
    graph->set_nav_background_color();
  }

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)activate_exit, this);
}

