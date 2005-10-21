/* 
 * Proview   $Id: xtt_ge.cpp,v 1.12 2005-10-21 16:11:22 claes Exp $
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
#include "glow_colpalwidget.h"

extern "C" {
#include "flow_x.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_mrm_util.h"
}

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"
#include "co_lng.h"
#include "xtt_ge.h"
#include "ge_graph.h"
#include "xtt_xnav.h"

//
//  Test program
//
//

// Static variables

static void ge_message( void *ctx, char severity, char *message);

static void ge_enable_set_focus( ge_tCtx gectx)
{
  gectx->set_focus_disabled--;
}

static void ge_disable_set_focus( ge_tCtx gectx, int time)
{
  gectx->set_focus_disabled++;
  gectx->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( gectx->toplevel), time,
	(XtTimerCallbackProc)ge_enable_set_focus, gectx);
}

static void ge_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  ge_tCtx	gectx;

  XtSetArg    (args[0], XmNuserData, &gectx);
  XtGetValues (w, args, 1);

  if ( !gectx)
    return;

  if ( mrm_IsIconicState(w))
    return;

  if ( gectx->set_focus_disabled)
    return;

  if ( gectx->graph)
    ((Graph *)gectx->graph)->set_inputfocus(1);

  ge_disable_set_focus( gectx, 400);

}

static void ge_graph_init_cb( void *client_data)
{
  ge_tCtx gectx = (ge_tCtx) client_data;
  char fname[120];
  int		default_width;
  int		default_height;
  int		sts;
  int		i;
  Arg 		args[7];

  cdh_ToLower( fname, gectx->filename);
  if ( ! strrchr( fname, '.'))
    strcat( fname, ".pwg");
  ((Graph *)gectx->graph)->open( fname);

  ((Graph *)gectx->graph)->init_trace();

  if ( gectx->width == 0 || gectx->height == 0)
  {
    sts = ((Graph *)gectx->graph)->get_default_size( &default_width, &default_height);
    if ( ODD(sts))
    {
      short x1, x2, y1, y2;

      i = 0;
      XtSetArg(args[i],XmNwidth, default_width);i++;
      XtSetArg(args[i],XmNheight, default_height);i++;

      x1 = default_width;
      y1 = default_height;
      x2 = default_width;
      y2 = default_height;
      // This condition is due to a bug in Reflection X 11.0.5...
      if ( !((XNav *)gectx->parent_ctx)->gbl.no_graph_ratio) {
	XtSetArg(args[i], XmNminAspectX, x1); i++;
	XtSetArg(args[i], XmNminAspectY, y1); i++;
	XtSetArg(args[i], XmNmaxAspectX, x2); i++;
	XtSetArg(args[i], XmNmaxAspectY, y2); i++;
      }
      XtSetValues( gectx->toplevel, args,i);
    }
  }

  ((Graph *)gectx->graph)->set_default_layout();

}

static void ge_graph_close_cb( void *client_data)
{
  ge_tCtx gectx = (ge_tCtx) client_data;
  ge_delete( gectx);
}

static void ge_change_value_cb( void *ge_ctx, void *value_object, char *text)
{
  Arg 		args[1];
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->value_input_open) 
  {
    XtUnmanageChild( gectx->value_dialog);
    gectx->value_input_open = 0;
    return;
  }

  XtManageChild( gectx->value_dialog);

  ge_message( gectx, ' ', "");
  XtCallAcceptFocus( gectx->value_input, CurrentTime);
  // XtSetKeyboardFocus( gectx->toplevel, gectx->value_input);

  XtSetArg(args[0],XmNvalue, text);
  XtSetValues( gectx->value_input, args, 1);
  XmTextSetCursorPosition( gectx->value_input, strlen(text));
  XmTextSetSelection( gectx->value_input, 0, strlen(text), CurrentTime);
  gectx->value_input_open = 1;
  gectx->current_value_object = value_object;
}

static int ge_command_cb( void *ge_ctx, char *command)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;
  int		sts;

  if ( gectx->command_cb)
  {
    sts = (gectx->command_cb)( gectx, command);
    return sts;
  }
  return 0;
}

static void ge_display_in_xnav_cb( void *ge_ctx, pwr_sAttrRef *arp)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->display_in_xnav_cb)
    (gectx->display_in_xnav_cb)( gectx->parent_ctx, arp);
}

static void ge_popup_menu_cb( void *ge_ctx, pwr_sAttrRef attrref,
	     unsigned long item_type, unsigned long utility, 
	     char *arg, Widget *popup)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->popup_menu_cb)
    (gectx->popup_menu_cb)( gectx->parent_ctx, attrref, item_type, utility, 
			    arg, popup);
}

static int ge_call_method_cb( void *ge_ctx, char *method, char *filter,
	     pwr_sAttrRef attrref, unsigned long item_type, unsigned long utility, 
	     char *arg)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->call_method_cb)
    return (gectx->call_method_cb)( gectx->parent_ctx, method, filter, attrref, item_type, utility, 
			    arg);
  else return 0;
}

static int ge_is_authorized_cb( void *ge_ctx, unsigned int access)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->is_authorized_cb)
    return (gectx->is_authorized_cb)( gectx->parent_ctx, access);
  return 0;
}

static int ge_get_current_objects_cb( void *ge_ctx, pwr_sAttrRef **alist,
				      int **is_alist)
{
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->get_current_objects_cb)
    return (gectx->get_current_objects_cb)( gectx->parent_ctx, alist, is_alist);
  return 0;
}

static void ge_confirm_cb( void *ge_ctx, void *confirm_object, char *text)
{
  Arg 		args[1];
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  if ( gectx->confirm_open) 
  {
    XtUnmanageChild( gectx->confirm_widget);
    gectx->confirm_open = 0;
    return;
  }

  XtManageChild( gectx->confirm_widget);

  ge_message( gectx, ' ', "");

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, "ISO8859-1"));
  XtSetValues( gectx->confirm_widget, args, 1);
  gectx->confirm_open = 1;
  gectx->current_confirm_object = confirm_object;
}

static void ge_message_dialog_cb( void *ge_ctx, char *text)
{
  Arg 		args[1];
  ge_tCtx	gectx = (ge_tCtx)ge_ctx;

  XtManageChild( gectx->message_dia_widget);

  XtSetArg(args[0],XmNmessageString, XmStringCreateLtoR( text, "ISO8859-1"));
  XtSetValues( gectx->message_dia_widget, args, 1);
}

static void ge_activate_value_input( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char *text;

  text = XmTextGetString( w);
  if ( gectx->value_input_open)
  {
    ((Graph *)gectx->graph)->change_value( gectx->current_value_object, text);
    XtUnmanageChild( gectx->value_dialog);
    gectx->value_input_open = 0;
  }
}

static void ge_activate_confirm_ok( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( gectx->confirm_widget);
  gectx->confirm_open = 0;
  ((Graph *)gectx->graph)->confirm_ok( gectx->current_confirm_object);
}

static void ge_activate_confirm_cancel( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->confirm_open = 0;
  XtUnmanageChild( gectx->confirm_widget);
}

static void ge_message( void *ctx, char severity, char *message)
{
  if ( strcmp( message, "") != 0)
    printf("** XttGe: %s\n", message);
}


static void ge_activate_exit( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  ge_delete( gectx);
}

static void ge_activate_zoom_in( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  ((Graph *)gectx->graph)->zoom( 1.2);
}

static void ge_activate_zoom_out( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  ((Graph *)gectx->graph)->zoom( 5.0/6);
}

static void ge_activate_zoom_reset( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  ((Graph *)gectx->graph)->unzoom();
}

static void ge_activate_help( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  char key[80];

  if ( gectx->help_cb)
  {
    cdh_ToLower( key, gectx->name);
    (gectx->help_cb)( gectx, key);
  }
}

static void ge_create_graph_form( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->graph_form = w;
}

static void ge_create_message_dia( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( XmMessageBoxGetChild( w, XmDIALOG_OK_BUTTON));
  XtUnmanageChild( XmMessageBoxGetChild( w, XmDIALOG_HELP_BUTTON));
}

static void ge_create_menu( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->menu_widget = w;
}

static void ge_create_value_input( Widget w, ge_tCtx gectx, XmAnyCallbackStruct *data)
{
  gectx->value_input = w;
}

static void ge_action_resize( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  ge_tCtx gectx;

  XtSetArg    (args[0], XmNuserData, &gectx);
  XtGetValues (w, args, 1);

  if ( gectx->graph && !gectx->scrollbar && !gectx->navigator)
    ((Graph *)gectx->graph)->set_default_layout();
}

int ge_set_object_focus( ge_tCtx gectx, char *name, int empty)
{
  return ((Graph *)gectx->graph)->set_object_focus( name, empty);
}

int ge_set_folder_index( ge_tCtx gectx, char *name, int idx)
{
  return ((Graph *)gectx->graph)->set_folder_index( name, idx);
}

extern "C" void ge_delete( ge_tCtx gectx)
{
  if ( gectx->close_cb)
    (gectx->close_cb)( gectx);
  if ( gectx->set_focus_disabled)
    XtRemoveTimeOut( gectx->focus_timerid);
  delete (Graph *)((Graph *)gectx->graph);
  XtDestroyWidget( gectx->toplevel);
  free( (char *) gectx);
}

void ge_pop( ge_tCtx gectx)
{
  flow_UnmapWidget( gectx->toplevel);
  flow_MapWidget( gectx->toplevel);
}

void ge_print( ge_tCtx gectx)
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/graph.ps");
  ((Graph *)gectx->graph)->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s 1", filename);
  system(cmd);
}

void ge_swap( ge_tCtx gectx, int mode)
{
  ((Graph *)gectx->graph)->swap( mode);
}

extern "C" ge_tCtx ge_new( Widget parent_wid,
  void			*parent_ctx,
  char 			*name,
  char 			*filename,
  int			scrollbar,
  int			menu,
  int			navigator,
  int			width,
  int			height,
  int			x,
  int			y,
  double		scan_time,
  char                  *object_name,
  int                   use_default_access,
  unsigned int          access,
  void			(*close_cb) (ge_tCtx),
  void			(*help_cb) (ge_tCtx, char *),
  int			(*command_cb) (ge_tCtx, char *),
  int                   (*get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
  int                   (*is_authorized_cb) (void *, unsigned int)
)
{
  char		uid_filename[120] = {"xtt_ge.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  ge_tCtx	gectx;
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
    {"ge_xtt_inputfocus",      (XtActionProc) ge_action_inputfocus},
    {"resize",      (XtActionProc) ge_action_resize},
  };

  static MrmRegisterArg	reglist[] = {
        { "ge_ctx", 0 },
	{"ge_activate_exit",(caddr_t)ge_activate_exit },
	{"ge_activate_zoom_in",(caddr_t)ge_activate_zoom_in },
	{"ge_activate_zoom_out",(caddr_t)ge_activate_zoom_out },
	{"ge_activate_zoom_reset",(caddr_t)ge_activate_zoom_reset },
	{"ge_activate_help",(caddr_t)ge_activate_help },
	{"ge_create_graph_form",(caddr_t)ge_create_graph_form },
	{"ge_create_menu",(caddr_t)ge_create_menu },
	{"ge_create_value_input",(caddr_t)ge_create_value_input },
	{"ge_activate_value_input",(caddr_t)ge_activate_value_input },
	{"ge_activate_confirm_ok",(caddr_t)ge_activate_confirm_ok },
	{"ge_activate_confirm_cancel",(caddr_t)ge_activate_confirm_cancel },
	{"ge_create_message_dia",(caddr_t)ge_create_message_dia }
	};

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  Lng::get_uid( uid_filename, uid_filename);

  // Create object context
  gectx = (ge_tCtx) calloc( 1, sizeof(*gectx));
  strcpy( gectx->filename, filename);
  strcpy( gectx->name, name);
  gectx->parent_ctx = parent_ctx;
  gectx->scrollbar = scrollbar;
  gectx->navigator = navigator;
  gectx->menu = menu;
  gectx->command_cb = command_cb;
  gectx->help_cb = help_cb;
  gectx->close_cb = close_cb;
  gectx->get_current_objects_cb = get_current_objects_cb;
  gectx->is_authorized_cb = is_authorized_cb;
  gectx->width = width;
  gectx->height = height;

  // Motif
  MrmInitialize();

  cdh_StrncpyCutOff( title, name, sizeof(title), 1);

  reglist[0].value = (caddr_t) gectx;


  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  // Save the context structure in the widget
  i = 0;
  XtSetArg(args[i], XmNuserData, (unsigned int) gectx);i++;
  XtSetArg(args[i],XmNdeleteResponse, XmDO_NOTHING);i++;

  gectx->toplevel = XtCreatePopupShell( title, 
		topLevelShellWidgetClass, parent_wid, args, i);

  MrmRegisterNames(reglist, reglist_num);

  sts = MrmFetchWidgetOverride( s_DRMh, "ge_window", gectx->toplevel,
			wname, args, 1, &ge_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", wname);

  sts = MrmFetchWidget(s_DRMh, "input_dialog", gectx->toplevel,
		&gectx->value_dialog, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch input dialog\n");

  sts = MrmFetchWidget(s_DRMh, "confirm_dialog", gectx->toplevel,
		&gectx->confirm_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch confirm dialog\n");

  sts = MrmFetchWidget(s_DRMh, "message_dialog", gectx->toplevel,
		&gectx->message_dia_widget, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch message dialog\n");

  MrmCloseHierarchy(s_DRMh);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext(gectx->toplevel), 
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
  XtSetValues( gectx->toplevel, args,i);
    
  XtManageChild( ge_widget);

  if ( !menu)
    XtUnmanageChild( gectx->menu_widget);

  gectx->graph = (void *) new Graph( gectx, gectx->graph_form, "Plant", 
		&gectx->grow_widget, &sts, "pwrp_exe:", graph_eMode_Runtime, 
		scrollbar, 1, object_name, use_default_access, access);
//  ((Graph *)gectx->graph)->set_scantime( scan_time);
  ((Graph *)gectx->graph)->message_cb = &ge_message;
  ((Graph *)gectx->graph)->close_cb = &ge_graph_close_cb;
  ((Graph *)gectx->graph)->init_cb = &ge_graph_init_cb;
  ((Graph *)gectx->graph)->change_value_cb = &ge_change_value_cb;
  ((Graph *)gectx->graph)->confirm_cb = &ge_confirm_cb;
  ((Graph *)gectx->graph)->message_dialog_cb = &ge_message_dialog_cb;
  ((Graph *)gectx->graph)->command_cb = &ge_command_cb;
  ((Graph *)gectx->graph)->display_in_xnav_cb = &ge_display_in_xnav_cb;
  ((Graph *)gectx->graph)->is_authorized_cb = &ge_is_authorized_cb;
  ((Graph *)gectx->graph)->get_current_objects_cb = &ge_get_current_objects_cb;
  ((Graph *)gectx->graph)->popup_menu_cb = &ge_popup_menu_cb;
  ((Graph *)gectx->graph)->call_method_cb = &ge_call_method_cb;
 
  XtPopup( gectx->toplevel, XtGrabNone);

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

    gectx->nav_shell = XmCreateDialogShell( gectx->grow_widget, "Navigator",
        args, i);
    XtManageChild( gectx->nav_shell);

    ((Graph *)gectx->graph)->create_navigator( gectx->nav_shell);
//    XtManageChild( gectx->nav_widget);
    XtRealizeWidget( gectx->nav_shell);
    ((Graph *)gectx->graph)->set_nav_background_color();
  }

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( gectx->toplevel, 
	(XtCallbackProc)ge_activate_exit, gectx);

  return gectx;
}

