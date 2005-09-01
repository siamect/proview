/* 
 * Proview   $Id: xtt_ev.cpp,v 1.7 2005-09-01 14:57:48 claes Exp $
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

/* xtt_ev.cpp -- Alarm and event window in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "rt_mh_util.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern "C" {
#include "flow_x.h"
}
#include "co_lng.h"
#include "co_mrm_util.h"
#include "xtt_ev.h"
#include "rt_xnav_msg.h"

static Ev *ev = NULL;

static void ev_eve_display_in_xnav_cb( void *ctx, pwr_sAttrRef *arp);
static void ev_ala_display_in_xnav_cb( void *ctx, pwr_sAttrRef *arp);
static void ev_blk_display_in_xnav_cb( void *ctx, pwr_sAttrRef *arp);
static char *ev_name_to_alias_cb( void *ctx, char *name);
static void ev_eve_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);
static void ev_ala_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);
static void ev_blk_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);
static void ev_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			      unsigned long item_type, unsigned long utility,
			      char *arg, Widget *popup);
static void ev_eve_action_inputfocus( Widget w, XmAnyCallbackStruct *data);
static void ev_ala_action_inputfocus( Widget w, XmAnyCallbackStruct *data);
static void ev_blk_action_inputfocus( Widget w, XmAnyCallbackStruct *data);
static void ev_eve_activate_exit( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_exit( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_exit( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_print( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_print( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_print( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_ack_last( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_ack_last( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_zoom_in( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_zoom_in( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_zoom_in( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_zoom_out( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_zoom_out( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_zoom_out( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_zoom_reset( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_zoom_reset( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_zoom_reset( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_block_remove( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_open_plc( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_open_plc( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_open_plc( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_display_in_xnav( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_display_in_xnav( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_display_in_xnav( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_disp_hundredth( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data);
static void ev_ala_activate_disp_hundredth( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data);
static void ev_eve_activate_hide_object( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data);
static void ev_ala_activate_hide_object( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data);
static void ev_eve_activate_hide_text( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data);
static void ev_ala_activate_hide_text( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data);
static void ev_eve_activate_help( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_help( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_activate_help( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_activate_helpevent( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_activate_helpevent( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_eve_create_form( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_ala_create_form( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static void ev_blk_create_form( Widget w, Ev *ev, XmAnyCallbackStruct *data);
static pwr_tStatus ev_mh_ack_bc( mh_sAck *MsgP);
static pwr_tStatus ev_mh_return_bc( mh_sReturn *MsgP);
static pwr_tStatus ev_mh_alarm_bc( mh_sMessage *MsgP);
static pwr_tStatus ev_mh_block_bc( mh_sBlock *MsgP);
static pwr_tStatus ev_mh_cancel_bc( mh_sReturn *MsgP);
static pwr_tStatus ev_mh_info_bc( mh_sMessage *MsgP);
static pwr_tStatus ev_mh_clear_alarmlist_bc( pwr_tNodeIndex nix);
static pwr_tStatus ev_mh_clear_blocklist_bc( pwr_tNodeIndex nix);

Ev::Ev(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	char *eve_name,
	char *ala_name,
	char *blk_name,
	pwr_tObjid ev_user,
	int display_ala,
	int display_eve,
	int display_blk,
	int display_return,
	int display_ack,
	int ev_beep,
	pwr_tStatus *status) :
	parent_ctx(ev_parent_ctx), parent_wid(ev_parent_wid),
	parent_wid_eve(NULL), parent_wid_ala(NULL),
	user(ev_user), eve_display_ack(display_ack), 
	eve_display_return(display_return),
	start_trace_cb(NULL), display_in_xnav_cb(NULL), update_info_cb(NULL),
	help_cb(NULL), eve(NULL), ala(NULL),
	connected(0), ala_displayed(0), eve_displayed(0), beep(ev_beep)
{
  char		uid_filename[120] = {"xtt_eve.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  int		i;
  pwr_sClass_User	*userobject_ptr;
  MrmHierarchy s_DRMh;
  MrmType dclass;

  static char eve_translations[] =
    "<FocusIn>: eve_inputfocus()\n";
  static char ala_translations[] =
    "<FocusIn>: ala_inputfocus()\n";
  static char blk_translations[] =
    "<FocusIn>: blk_inputfocus()\n";
  static XtTranslations eve_compiled_translations = NULL;
  static XtTranslations ala_compiled_translations = NULL;
  static XtTranslations blk_compiled_translations = NULL;

  static XtActionsRec eve_actions[] =
  {
    {"eve_inputfocus",      (XtActionProc) ev_eve_action_inputfocus}
  };
  static XtActionsRec ala_actions[] =
  {
    {"ala_inputfocus",      (XtActionProc) ev_ala_action_inputfocus}
  };
  static XtActionsRec blk_actions[] =
  {
    {"blk_inputfocus",      (XtActionProc) ev_blk_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "ev_ctx", 0 },
	{"ev_eve_activate_exit",(caddr_t)ev_eve_activate_exit },
	{"ev_eve_activate_print",(caddr_t)ev_eve_activate_print },
	{"ev_eve_activate_ack_last",(caddr_t)ev_eve_activate_ack_last },
	{"ev_eve_activate_zoom_in",(caddr_t)ev_eve_activate_zoom_in },
	{"ev_eve_activate_zoom_out",(caddr_t)ev_eve_activate_zoom_out },
	{"ev_eve_activate_zoom_reset",(caddr_t)ev_eve_activate_zoom_reset },
	{"ev_eve_activate_open_plc",(caddr_t)ev_eve_activate_open_plc },
	{"ev_eve_activate_display_in_xnav",(caddr_t)ev_eve_activate_display_in_xnav },
	{"ev_eve_activate_disp_hundredth",(caddr_t)ev_eve_activate_disp_hundredth },
	{"ev_eve_activate_hide_object",(caddr_t)ev_eve_activate_hide_object },
	{"ev_eve_activate_hide_text",(caddr_t)ev_eve_activate_hide_text },
	{"ev_eve_activate_help",(caddr_t)ev_eve_activate_help },
	{"ev_eve_activate_helpevent",(caddr_t)ev_eve_activate_helpevent },
	{"ev_eve_create_form",(caddr_t)ev_eve_create_form },
	{"ev_ala_activate_exit",(caddr_t)ev_ala_activate_exit },
	{"ev_ala_activate_print",(caddr_t)ev_ala_activate_print },
	{"ev_ala_activate_ack_last",(caddr_t)ev_ala_activate_ack_last },
	{"ev_ala_activate_zoom_in",(caddr_t)ev_ala_activate_zoom_in },
	{"ev_ala_activate_zoom_out",(caddr_t)ev_ala_activate_zoom_out },
	{"ev_ala_activate_zoom_reset",(caddr_t)ev_ala_activate_zoom_reset },
	{"ev_ala_activate_open_plc",(caddr_t)ev_ala_activate_open_plc },
	{"ev_ala_activate_display_in_xnav",(caddr_t)ev_ala_activate_display_in_xnav },
	{"ev_ala_activate_disp_hundredth",(caddr_t)ev_ala_activate_disp_hundredth },
	{"ev_ala_activate_hide_object",(caddr_t)ev_ala_activate_hide_object },
	{"ev_ala_activate_hide_text",(caddr_t)ev_ala_activate_hide_text },
	{"ev_ala_activate_help",(caddr_t)ev_ala_activate_help },
	{"ev_ala_activate_helpevent",(caddr_t)ev_ala_activate_helpevent },
	{"ev_ala_create_form",(caddr_t)ev_ala_create_form },
	{"ev_blk_activate_exit",(caddr_t)ev_blk_activate_exit },
	{"ev_blk_activate_print",(caddr_t)ev_blk_activate_print },
	{"ev_blk_activate_zoom_in",(caddr_t)ev_blk_activate_zoom_in },
	{"ev_blk_activate_zoom_out",(caddr_t)ev_blk_activate_zoom_out },
	{"ev_blk_activate_zoom_reset",(caddr_t)ev_blk_activate_zoom_reset },
	{"ev_blk_activate_block_remove",(caddr_t)ev_blk_activate_block_remove },
	{"ev_blk_activate_open_plc",(caddr_t)ev_blk_activate_open_plc },
	{"ev_blk_activate_display_in_xnav",(caddr_t)ev_blk_activate_display_in_xnav },
	{"ev_blk_activate_help",(caddr_t)ev_blk_activate_help },
	{"ev_blk_create_form",(caddr_t)ev_blk_create_form }
	};
  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  *status = 1;

  Lng::get_uid( uid_filename, uid_filename);

  // Check user object
  if ( cdh_ObjidIsNull( user))
  {
    *status = XNAV__NOUSER;
    return;
  }

  sts = gdh_ObjidToPointer ( user, (pwr_tAddress *) &userobject_ptr);
  if ( EVEN(sts)) 
  {
    *status = XNAV__NOUSER;
    return;
  }
  ala_size = userobject_ptr->MaxNoOfAlarms;
  eve_size = userobject_ptr->MaxNoOfEvents;
  blk_size = 0;
  create_aliaslist( userobject_ptr);

  reglist[0].value = (caddr_t) this;

  // Motif
  MrmInitialize();


  // Save the context structure in the widget
  i = 0;
  XtSetArg(args[i], XmNuserData, (unsigned int) this);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid_eve = XtCreatePopupShell( eve_name, 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "eve_window", parent_wid_eve,
			eve_name, args, 1, &toplevel_eve, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", eve_name);

  parent_wid_ala = XtCreatePopupShell( ala_name, 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "ala_window", parent_wid_ala,
			ala_name, args, 1, &toplevel_ala, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", ala_name);

  parent_wid_blk = XtCreatePopupShell( blk_name, 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "blk_window", parent_wid_blk,
			blk_name, args, 1, &toplevel_blk, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", blk_name);

  MrmCloseHierarchy(s_DRMh);

  if ( eve_compiled_translations == NULL) 
  {
    XtAppAddActions( XtWidgetToApplicationContext( toplevel_eve), 
		eve_actions, XtNumber(eve_actions));
    eve_compiled_translations = XtParseTranslationTable( eve_translations);
  }
  XtOverrideTranslations( toplevel_eve, eve_compiled_translations);

  if ( ala_compiled_translations == NULL) 
  {
    XtAppAddActions( XtWidgetToApplicationContext( toplevel_ala), 
		ala_actions, XtNumber(ala_actions));
    ala_compiled_translations = XtParseTranslationTable( ala_translations);
  }
  XtOverrideTranslations( toplevel_ala, ala_compiled_translations);

  if ( blk_compiled_translations == NULL) 
  {
    XtAppAddActions( XtWidgetToApplicationContext( toplevel_blk), 
		blk_actions, XtNumber(blk_actions));
    blk_compiled_translations = XtParseTranslationTable( blk_translations);
  }
  XtOverrideTranslations( toplevel_blk, blk_compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,700);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetValues( toplevel_eve ,args,i);
    
  i = 0;
  XtSetArg(args[i],XmNwidth,700);i++;
  XtSetArg(args[i],XmNheight,300);i++;
  XtSetValues( toplevel_ala ,args,i);
    
  i = 0;
  XtSetArg(args[i],XmNwidth,700);i++;
  XtSetArg(args[i],XmNheight,300);i++;
  XtSetValues( toplevel_blk ,args,i);
    
  XtManageChild( toplevel_eve);
  XtManageChild( toplevel_ala);
  XtManageChild( toplevel_blk);

  // Create ala and eve...
  eve = new EvList( this, form_eve, ev_eType_EventList, eve_size, &eve_widget);
  eve->start_trace_cb = &ev_eve_start_trace_cb;
  eve->display_in_xnav_cb = &ev_eve_display_in_xnav_cb;
  eve->name_to_alias_cb = &ev_name_to_alias_cb;
  eve->popup_menu_cb = &ev_popup_menu_cb;
  ala = new EvList( this, form_ala, ev_eType_AlarmList, ala_size, &ala_widget);
  ala->start_trace_cb = &ev_ala_start_trace_cb;
  ala->display_in_xnav_cb = &ev_ala_display_in_xnav_cb;
  ala->name_to_alias_cb = &ev_name_to_alias_cb;
  ala->popup_menu_cb = &ev_popup_menu_cb;
  blk = new EvList( this, form_blk, ev_eType_BlockList, blk_size, &blk_widget);
  blk->start_trace_cb = &ev_blk_start_trace_cb;
  blk->display_in_xnav_cb = &ev_blk_display_in_xnav_cb;
  blk->popup_menu_cb = &ev_popup_menu_cb;
  // blk->hide_text = 1;

//  XtManageChild( form_widget);

  if ( display_eve) {
    XtPopup( parent_wid_eve, XtGrabNone);
    eve_displayed = 1;
  }
  else
    XtRealizeWidget( parent_wid_eve);

  if ( display_ala) {
    XtPopup( parent_wid_ala, XtGrabNone);
    ala_displayed = 1;
  }
  else
    XtRealizeWidget( parent_wid_ala);

  if ( display_blk) {
    XtPopup( parent_wid_blk, XtGrabNone);
    blk_displayed = 1;
  }
  else
    XtRealizeWidget( parent_wid_blk);


  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid_eve, 
	(XtCallbackProc)ev_eve_activate_exit, this);
  flow_AddCloseVMProtocolCb( parent_wid_ala, 
	(XtCallbackProc)ev_ala_activate_exit, this);
  flow_AddCloseVMProtocolCb( parent_wid_blk, 
	(XtCallbackProc)ev_blk_activate_exit, this);

  // Store this for the mh callbacks
  ev = this;

  sts = outunit_connect( user);
  if ( EVEN(sts))
    *status = sts;
}


//
//  Delete ev
//
Ev::~Ev()
{
  if ( connected)
    mh_OutunitDisconnect();
  if ( parent_wid_eve)
    XtDestroyWidget( parent_wid_eve);
  if ( parent_wid_ala)
    XtDestroyWidget( parent_wid_ala);
  if ( eve)
    delete eve;
  if ( ala)
    delete ala;
  ev = NULL;
}

void Ev::map_eve()
{
  if ( !eve_displayed)
  {
    flow_MapWidget( parent_wid_eve);
    eve_displayed = 1;
  }
  else
  {
    flow_UnmapWidget( parent_wid_eve);
    flow_MapWidget( parent_wid_eve);
  }
}

void Ev::map_ala()
{
  if ( !ala_displayed)
  {
    flow_MapWidget( parent_wid_ala);
    ala_displayed = 1;
  }
  else
  {
    flow_UnmapWidget( parent_wid_ala);
    flow_MapWidget( parent_wid_ala);
  }
}

void Ev::map_blk()
{
  if ( !blk_displayed)
  {
    flow_MapWidget( parent_wid_blk);
    blk_displayed = 1;
  }
  else
  {
    flow_UnmapWidget( parent_wid_blk);
    flow_MapWidget( parent_wid_blk);
  }
}

void Ev::unmap_eve()
{
  if ( eve_displayed)
  {
    flow_UnmapWidget( parent_wid_eve);
    eve_displayed = 0;
  }
}

void Ev::unmap_ala()
{
  if ( ala_displayed)
  {
    flow_UnmapWidget( parent_wid_ala);
    ala_displayed = 0;
  }
}

void Ev::unmap_blk()
{
  if ( blk_displayed)
  {
    flow_UnmapWidget( parent_wid_blk);
    blk_displayed = 0;
  }
}

static void ev_eve_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((Ev *)ctx)->start_trace_cb)
    ((Ev *)ctx)->start_trace_cb( ((Ev *)ctx)->parent_ctx, objid, name);
}

static void ev_ala_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((Ev *)ctx)->start_trace_cb)
    ((Ev *)ctx)->start_trace_cb( ((Ev *)ctx)->parent_ctx, objid, name);
}

static void ev_blk_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((Ev *)ctx)->start_trace_cb)
    ((Ev *)ctx)->start_trace_cb( ((Ev *)ctx)->parent_ctx, objid, name);
}

static void ev_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			      unsigned long item_type, unsigned long utility,
			      char *arg, Widget *popup)
{
  if ( ((Ev *)ctx)->popup_menu_cb)
    (((Ev *)ctx)->popup_menu_cb) ( ((Ev *)ctx)->parent_ctx, attrref, item_type,
				   utility, arg, popup);
}

static char *ev_name_to_alias_cb( void *ctx, char *name)
{
  return ((Ev *)ctx)->name_to_alias( name);
}

static void ev_eve_display_in_xnav_cb( void *ctx, pwr_sAttrRef *arp)
{
  if ( ((Ev *)ctx)->display_in_xnav_cb)
    ((Ev *)ctx)->display_in_xnav_cb( ((Ev *)ctx)->parent_ctx, arp);
}

static void ev_ala_display_in_xnav_cb( void *ctx, pwr_sAttrRef *arp)
{
  if ( ((Ev *)ctx)->display_in_xnav_cb)
    ((Ev *)ctx)->display_in_xnav_cb( ((Ev *)ctx)->parent_ctx, arp);
}

static void ev_blk_display_in_xnav_cb( void *ctx, pwr_sAttrRef *arp)
{
  if ( ((Ev *)ctx)->display_in_xnav_cb)
    ((Ev *)ctx)->display_in_xnav_cb( ((Ev *)ctx)->parent_ctx, arp);
}

static void ev_eve_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Ev *ev;

  XtSetArg    (args[0], XmNuserData, &ev);
  XtGetValues (w, args, 1);

  if ( mrm_IsIconicState(w))
    return;

  if ( ev && ev->eve_displayed) {
    if ( ev->eve_focustimer.disabled())
      return;

    ev->eve->set_input_focus();
    ev->eve_focustimer.disable( ev->toplevel_eve, 400);
  }
}

static void ev_ala_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Ev *ev;

  XtSetArg    (args[0], XmNuserData, &ev);
  XtGetValues (w, args, 1);

  if ( mrm_IsIconicState(w))
    return;

  if ( ev && ev->ala_displayed) {
    if ( ev->ala_focustimer.disabled())
      return;

    ev->ala->set_input_focus();
    ev->ala_focustimer.disable( ev->toplevel_ala, 400);
  }
}

static void ev_blk_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Ev *ev;

  XtSetArg    (args[0], XmNuserData, &ev);
  XtGetValues (w, args, 1);

  if ( mrm_IsIconicState(w))
    return;

  if ( ev && ev->blk_displayed) {
    if ( ev->blk_focustimer.disabled())
      return;

    ev->blk->set_input_focus();
    ev->blk_focustimer.disable( ev->toplevel_blk, 400);
  }
}

static void ev_eve_activate_exit( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  flow_UnmapWidget( ev->parent_wid_eve);
  ev->eve_displayed = 0;
}

static void ev_ala_activate_exit( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  flow_UnmapWidget( ev->parent_wid_ala);
  ev->ala_displayed = 0;
}

static void ev_blk_activate_exit( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  flow_UnmapWidget( ev->parent_wid_blk);
  ev->blk_displayed = 0;
}

static void ev_eve_activate_print( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/xnav.ps");
  ev->eve->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s", filename);
  system(cmd);
}

static void ev_ala_activate_print( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/xnav.ps");
  ev->ala->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s", filename);
  system(cmd);
}

static void ev_blk_activate_print( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/xnav.ps");
  ev->blk->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s", filename);
  system(cmd);
}

static void ev_eve_activate_ack_last( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  mh_sEventId *id;
  int sts;

  sts = ev->ala->get_last_not_acked( &id);
  if ( EVEN(sts)) return;
  ev->ala->ack( id);
  ev->eve->ack( id);
  mh_OutunitAck( id);
}

static void ev_ala_activate_ack_last( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev_eve_activate_ack_last( w, ev, data);
}

static void ev_eve_activate_zoom_in( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->eve->zoom( 1.2);
}

static void ev_ala_activate_zoom_in( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->ala->zoom( 1.2);
}

static void ev_blk_activate_zoom_in( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->blk->zoom( 1.2);
}

static void ev_eve_activate_zoom_out( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->eve->zoom( 5.0/6);
}

static void ev_ala_activate_zoom_out( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->ala->zoom( 5.0/6);
}

static void ev_blk_activate_zoom_out( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->blk->zoom( 5.0/6);
}

static void ev_eve_activate_zoom_reset( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->eve->unzoom();
}

static void ev_ala_activate_zoom_reset( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->ala->unzoom();
}

static void ev_blk_activate_zoom_reset( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->blk->unzoom();
}

static void ev_blk_activate_block_remove( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->blk->block_remove();
}

static void ev_eve_activate_open_plc( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->eve->start_trace();
}

static void ev_ala_activate_open_plc( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->ala->start_trace();
}

static void ev_blk_activate_open_plc( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->blk->start_trace();
}

static void ev_eve_activate_display_in_xnav( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->eve->display_in_xnav();
}

static void ev_ala_activate_display_in_xnav( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->ala->display_in_xnav();
}

static void ev_blk_activate_display_in_xnav( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->blk->display_in_xnav();
}

static void ev_eve_activate_disp_hundredth( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data)
{
  ev->eve->set_display_hundredth( data->set);
}

static void ev_ala_activate_disp_hundredth( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data)
{
  ev->ala->set_display_hundredth( data->set);
}

static void ev_eve_activate_hide_object( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data)
{
  ev->eve->set_hide_object( data->set);
}

static void ev_ala_activate_hide_object( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data)
{
  ev->ala->set_hide_object( data->set);
}

static void ev_eve_activate_hide_text( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data)
{
  ev->eve->set_hide_text( data->set);
}

static void ev_ala_activate_hide_text( Widget w, Ev *ev, XmToggleButtonCallbackStruct *data)
{
  ev->ala->set_hide_text( data->set);
}

static void ev_eve_activate_help( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  if ( ev->help_cb)
    (ev->help_cb)( ev->parent_ctx, "eventlist");
}

static void ev_eve_activate_helpevent( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  char	eventname[80];
  int 	sts;

  if ( ev->help_cb)
  {
    sts = ev->eve->get_selected_event( eventname);
    if( ODD(sts))
      (ev->help_cb)( ev->parent_ctx, eventname);
  }
}

static void ev_ala_activate_help( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  if ( ev->help_cb)
    (ev->help_cb)( ev->parent_ctx, "alarmlist");
}

static void ev_ala_activate_helpevent( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  char	eventname[80];
  int 	sts;

  if ( ev->help_cb)
  {
    sts = ev->ala->get_selected_event( eventname);
    if( ODD(sts))
      (ev->help_cb)( ev->parent_ctx, eventname);
  }
}

static void ev_blk_activate_help( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  if ( ev->help_cb)
    (ev->help_cb)( ev->parent_ctx, "blocklist");
}

static void ev_eve_create_form( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->form_eve = w;
}

static void ev_ala_create_form( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->form_ala = w;
}

static void ev_blk_create_form( Widget w, Ev *ev, XmAnyCallbackStruct *data)
{
  ev->form_blk = w;
}

int	Ev::get_alarm_info( evlist_sAlarmInfo *info)
{
  return ala->get_alarm_info( info);
}

int	Ev::outunit_connect( pwr_tObjid	user)
{
  int		sts;

  // Wait for mh has flagged initizated
  mh_UtilWaitForMh();

  sts = mh_OutunitConnect(
		user,
		mh_eOutunitType_Operator,
		0,
		ev_mh_ack_bc,
		ev_mh_alarm_bc,
		ev_mh_block_bc,
		ev_mh_cancel_bc,
		ev_mh_clear_alarmlist_bc,
		ev_mh_clear_blocklist_bc,
		ev_mh_info_bc,
		ev_mh_return_bc
		);
  if (EVEN(sts)) return sts;

  connected = 1;
  return XNAV__SUCCESS;
}

void Ev::update( double scantime)
{
  int sts;
  int nodraw_set = 0;

  sts = mh_OutunitReceive();     
  while (ODD(sts))
  {
    if ( !nodraw_set)
    {
      eve->set_nodraw();
      ala->set_nodraw();
      nodraw_set = 1;
    }
    sts = mh_OutunitReceive();     
  }
  if ( nodraw_set)
  {
    eve->reset_nodraw();
    ala->reset_nodraw();
  }

  ala->beep( scantime);
}      

void Ev::ack_last_prio( unsigned long type, unsigned long prio)
{
  mh_sEventId 	*id;
  int		sts;

  sts = ala->get_last_not_acked_prio( &id, type, prio);
  if ( ODD(sts))
  {
    ala->ack( id);
    eve->ack( id);
    mh_OutunitAck( id);
  }
}

int Ev::get_last_not_acked_prio( mh_sEventId **id, unsigned long type, 
	unsigned long prio)
{
  return ala->get_last_not_acked_prio( id, type, prio);
}

void Ev::create_aliaslist( void *up)
{
  char alias_array[2][80];
  int nr;
  int i, j;
  int alias_size;
  ev_sAlias dum;
  pwr_sClass_User *userp = (pwr_sClass_User *)up;

  int listsize = MIN( sizeof(userp->SelectList)/sizeof(userp->SelectList[0]),
		      sizeof(alias_list)/sizeof(alias_list[0]));
						    

  for ( i = 0, j = 0; i < listsize; i++) {
    nr = dcli_parse( userp->SelectList[i], " 	", "",
	     (char *) alias_array, sizeof( alias_array)/sizeof( alias_array[0]), 
	     sizeof( alias_array[0]), 0);
    if ( nr < 2)
      continue;
    cdh_ToUpper( alias_list[j].Object, alias_array[0]);
    strncpy( alias_list[j].Alias, alias_array[1], sizeof(alias_list[j].Alias));
    alias_list[j].Alias[sizeof(alias_list[j].Alias)-1] = 0;
    j++;
  }
  alias_size = j;

  // Order
  for ( i = alias_size - 1; i > 0; i--) {
    for ( j = 0; j < i; j++) {
      if ( strcmp( alias_list[j].Object, alias_list[j+1].Object) <= 0) {
	memcpy( &dum, &alias_list[j+1], sizeof(dum));
	memcpy( &alias_list[j+1], &alias_list[j], sizeof(dum));
	memcpy( &alias_list[j], &dum, sizeof(dum));
      }
    }
  }
}

char *Ev::name_to_alias( char *name)
{
  char oname[120];
  static char alias[40];

  cdh_ToUpper( oname, name);

  for ( int i = 0; i < (int)(sizeof(alias_list)/sizeof(alias_list[0])); i++) {
    if ( alias_list[i].Alias[0] == 0)
      break;
    if ( strncmp( oname, alias_list[i].Object, strlen(alias_list[i].Object)) == 0) {
      strcpy( alias, alias_list[i].Alias);
      return alias;
    }	 
  }
  strcpy( alias, "");
  return alias;
}

static pwr_tStatus ev_mh_ack_bc( mh_sAck *MsgP)
{
  if ( ev->eve_display_ack)
  {
    // Insert in eve
    ev->eve->event_ack( MsgP);
  }
  ev->ala->event_ack( MsgP);

  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

static pwr_tStatus ev_mh_return_bc( mh_sReturn *MsgP)
{
  if ( ev->eve_display_return)
  {
    // Insert in eve
    ev->eve->event_return( MsgP);
  }
  ev->ala->event_return( MsgP);

  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

static pwr_tStatus ev_mh_alarm_bc( mh_sMessage *MsgP)
{
  ev->eve->event_alarm( MsgP);
  ev->ala->event_alarm( MsgP);
  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

static pwr_tStatus ev_mh_block_bc( mh_sBlock *MsgP)
{
  ev->blk->event_block( MsgP);
  ev->eve->event_block( MsgP);
  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

static pwr_tStatus ev_mh_cancel_bc( mh_sReturn *MsgP)
{
  ev->ala->event_cancel( MsgP);
  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

static pwr_tStatus ev_mh_info_bc( mh_sMessage *MsgP)
{
  ev->eve->event_info( MsgP);
  ev->ala->event_info( MsgP);
  if ( ev->update_info_cb)
    ev->update_info_cb( ev->parent_ctx);
  return 1;
}

static pwr_tStatus ev_mh_clear_alarmlist_bc( pwr_tNodeIndex nix)
{
  ev->ala->event_clear_alarmlist( nix);
  return 1;
}

static pwr_tStatus ev_mh_clear_blocklist_bc( pwr_tNodeIndex nix)
{
  ev->blk->event_clear_alarmlist( nix);
  return 1;
}

