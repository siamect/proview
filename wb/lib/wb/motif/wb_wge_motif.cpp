/* 
 * Proview   $Id: wb_wge_motif.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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
#include "glow_colpalctx.h"
#include "glow_colpalapi.h"
#include "glow_colpalwidget_motif.h"

#include "flow_x.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_mrm_util.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_motif.h"
#include "co_lng.h"
#include "wb_wge_motif.h"
#include "ge_graph_motif.h"

// Static variables

void WGeMotif::enable_set_focus( WGeMotif *ge)
{
  ge->set_focus_disabled--;
}

void WGeMotif::disable_set_focus( WGeMotif *ge, int time)
{
  ge->set_focus_disabled++;
  ge->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( ge->toplevel), time,
	(XtTimerCallbackProc)enable_set_focus, ge);
}

void WGeMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  WGeMotif *ge;

  XtSetArg    (args[0], XmNuserData, &ge);
  XtGetValues (w, args, 1);

  if ( !ge)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( ge->set_focus_disabled)
    return;

  if ( ge->graph)
    ((Graph *)ge->graph)->set_inputfocus(1);

  disable_set_focus( ge, 400);

}

void WGeMotif::change_value_cb( void *ge_ctx, void *value_object, char *text)
{
  Arg 		args[1];
  WGeMotif   *ge = (WGeMotif *)ge_ctx;

  if ( ge->value_input_open) 
  {
    XtUnmanageChild( ge->value_dialog);
    ge->value_input_open = 0;
    return;
  }

  XtManageChild( ge->value_dialog);

  message( ge, ' ', "");
  XtCallAcceptFocus( ge->value_input, CurrentTime);
  // XtSetKeyboardFocus( ge->toplevel, ge->value_input);

  XtSetArg(args[0],XmNvalue, text);
  XtSetValues( ge->value_input, args, 1);
  XmTextSetCursorPosition( ge->value_input, strlen(text));
  XmTextSetSelection( ge->value_input, 0, strlen(text), CurrentTime);
  ge->value_input_open = 1;
  ge->current_value_object = value_object;
}

void WGeMotif::confirm_cb( void *ge_ctx, void *confirm_object, char *text)
{
  Arg 		args[1];
  WGe	*ge = (WGe *)ge_ctx;

  if ( ge->confirm_open) {
    XtUnmanageChild( ((WGeMotif *)ge)->confirm_widget);
    ge->confirm_open = 0;
    return;
  }

  XtManageChild( ((WGeMotif *)ge)->confirm_widget);

  message( ge, ' ', "");

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, "ISO8859-1"));
  XtSetValues( ((WGeMotif *)ge)->confirm_widget, args, 1);
  ge->confirm_open = 1;
  ge->current_confirm_object = confirm_object;
}

void WGeMotif::message_dialog_cb( void *ge_ctx, char *text)
{
  Arg 		args[1];
  WGe 	*ge = (WGe *)ge_ctx;

  XtManageChild( ((WGeMotif *)ge)->message_dia_widget);

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, "ISO8859-1"));
  XtSetValues( ((WGeMotif *)ge)->message_dia_widget, args, 1);
}

void WGeMotif::activate_value_input( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  char *text;

  text = XmTextGetString( w);
  if ( ge->value_input_open)
  {
    ((Graph *)ge->graph)->change_value( ge->current_value_object, text);
    XtUnmanageChild( ((WGeMotif *)ge)->value_dialog);
    ge->value_input_open = 0;
  }
}

void WGeMotif::activate_confirm_ok( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((WGeMotif *)ge)->confirm_widget);
  ge->confirm_open = 0;
  ((Graph *)ge->graph)->confirm_ok( ge->current_confirm_object);
}

void WGeMotif::activate_confirm_cancel( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  ge->confirm_open = 0;
  XtUnmanageChild( ((WGeMotif *)ge)->confirm_widget);
}

void WGeMotif::activate_exit( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  if ( ge->modal)
    ge->terminated = 1;
  else
    delete ge;
}

void WGeMotif::activate_zoom_in( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  ((Graph *)ge->graph)->zoom( 1.2);
}

void WGeMotif::activate_zoom_out( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  ((Graph *)ge->graph)->zoom( 5.0/6);
}

void WGeMotif::activate_zoom_reset( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  ((Graph *)ge->graph)->unzoom();
}

void WGeMotif::activate_help( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  char key[80];

  if ( ge->help_cb)
  {
    cdh_ToLower( key, ge->name);
    (ge->help_cb)( ge, key);
  }
}

void WGeMotif::create_graph_form( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  ((WGeMotif *)ge)->graph_form = w;
}

void WGeMotif::create_message_dia( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( XmMessageBoxGetChild( w, XmDIALOG_OK_BUTTON));
  XtUnmanageChild( XmMessageBoxGetChild( w, XmDIALOG_HELP_BUTTON));
}

void WGeMotif::create_menu( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  ((WGeMotif *)ge)->menu_widget = w;
}

void WGeMotif::create_value_input( Widget w, WGe *ge, XmAnyCallbackStruct *data)
{
  ((WGeMotif *)ge)->value_input = w;
}

void WGeMotif::action_resize( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  WGe *ge;

  XtSetArg    (args[0], XmNuserData, &ge);
  XtGetValues (w, args, 1);

  if ( ge->graph && !ge->scrollbar && !ge->navigator)
    ((Graph *)ge->graph)->set_default_layout();
}

WGeMotif::~WGeMotif()
{
  if ( close_cb)
    (close_cb)( (void *)this);
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);
  delete graph;
  XtDestroyWidget( toplevel);
}

void WGeMotif::set_size( int width, int height)
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
  // if ( !((XNav *)ge->parent_ctx)->gbl.no_graph_ratio) {
  XtSetArg(args[i], XmNminAspectX, x1); i++;
  XtSetArg(args[i], XmNminAspectY, y1); i++;
  XtSetArg(args[i], XmNmaxAspectX, x2); i++;
  XtSetArg(args[i], XmNmaxAspectY, y2); i++;
  
  XtSetValues( toplevel, args,i);
}

void WGeMotif::pop()
{
  flow_UnmapWidget( toplevel);
  flow_MapWidget( toplevel);
}

WGeMotif::WGeMotif( Widget wge_parent_wid, void *wge_parent_ctx, char *wge_name, char *wge_filename,
	  int wge_scrollbar, int wge_menu, int wge_navigator, int wge_width, int wge_height, 
	  int x, int y, char *object_name, int wge_modal = 0) :
  WGe(wge_parent_ctx, wge_name, wge_filename, wge_scrollbar, wge_menu, wge_navigator, wge_width,
      wge_height, x, y, object_name, wge_modal),
  parent_wid(wge_parent_wid), value_dialog(0), set_focus_disabled(0), focus_timerid(0)
{
  pwr_tFileName	uid_filename = {"$pwr_exe/wb_wge.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  char 		title[300];
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  Widget	wge_widget;
  char		wname[] = "Proview/R Ge";

  static char translations[] =
    "<ConfigureNotify>: resize()\n\
<FocusIn>: wge_inputfocus()";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
  {
    {"wge_inputfocus",      (XtActionProc) action_inputfocus},
    {"resize",      (XtActionProc) action_resize},
  };

  static MrmRegisterArg	reglist[] = {
        { "wge_ctx", 0 },
	{"wge_activate_exit",(caddr_t)activate_exit },
	{"wge_activate_zoom_in",(caddr_t)activate_zoom_in },
	{"wge_activate_zoom_out",(caddr_t)activate_zoom_out },
	{"wge_activate_zoom_reset",(caddr_t)activate_zoom_reset },
	{"wge_activate_help",(caddr_t)activate_help },
	{"wge_create_graph_form",(caddr_t)create_graph_form },
	{"wge_create_menu",(caddr_t)create_menu },
	{"wge_create_value_input",(caddr_t)create_value_input },
	{"wge_activate_value_input",(caddr_t)activate_value_input },
	{"wge_activate_confirm_ok",(caddr_t)activate_confirm_ok },
	{"wge_activate_confirm_cancel",(caddr_t)activate_confirm_cancel },
	{"wge_create_message_dia",(caddr_t)create_message_dia }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  dcli_translate_filename( uid_filename, uid_filename);

  // Create object context
  strcpy( filename, wge_filename);
  strcpy( name, wge_name);
  // command_cb = command_cb;
  // help_cb = help_cb;
  // close_cb = close_cb;
  // is_authorized_cb = is_authorized_cb;

  // Motif
  MrmInitialize();

  cdh_StrncpyCutOff( title, name, sizeof(title), 1);

  reglist[0].value = (caddr_t) this;


  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  // Save the context structure in the widget
  i = 0;
  XtSetArg(args[i], XmNuserData, (unsigned int) this);i++;
  XtSetArg(args[i],XmNdeleteResponse, XmDO_NOTHING);i++;

  toplevel = XtCreatePopupShell( title, 
		topLevelShellWidgetClass, parent_wid, args, i);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, "wge_window", toplevel,
			wname, args, 1, &wge_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", wname);

  sts = MrmFetchWidget(s_DRMh, "input_dialog", toplevel,
		&value_dialog, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, "confirm_dialog", toplevel,
		&confirm_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch confirm dialog\n");

  sts = MrmFetchWidget(s_DRMh, "message_dialog", toplevel,
		&message_dia_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch message dialog\n");

  MrmCloseHierarchy(s_DRMh);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(toplevel), 
						actions, XtNumber(actions));
 
  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( wge_widget, compiled_translations);

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
    
  XtManageChild( wge_widget);

  if ( !menu)
    XtUnmanageChild( menu_widget);

  graph = new GraphMotif( this, graph_form, "Plant", 
		&grow_widget, &sts, "pwrp_exe:", graph_eMode_Runtime, 
		scrollbar, 1, object_name, 0, 0);
  ((Graph *)graph)->message_cb = &message;
  ((Graph *)graph)->close_cb = &graph_close_cb;
  ((Graph *)graph)->init_cb = &graph_init_cb;
  ((Graph *)graph)->change_value_cb = &change_value_cb;
  ((Graph *)graph)->confirm_cb = &confirm_cb;
  ((Graph *)graph)->message_dialog_cb = &message_dialog_cb;
  ((Graph *)graph)->command_cb = &wge_command_cb;
  ((Graph *)graph)->is_authorized_cb = &wge_is_authorized_cb;
 
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

    nav_shell = XmCreateDialogShell( grow_widget, "Navigator",
        args, i);
    XtManageChild( nav_shell);

    ((GraphMotif *)graph)->create_navigator( nav_shell);
//    XtManageChild( nav_widget);
    XtRealizeWidget( nav_shell);
    ((Graph *)graph)->set_nav_background_color();
  }

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( toplevel, 
	(XtCallbackProc)activate_exit, this);

}

