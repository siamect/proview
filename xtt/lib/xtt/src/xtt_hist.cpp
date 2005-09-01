/* 
 * Proview   $Id: xtt_hist.cpp,v 1.9 2005-09-01 14:57:48 claes Exp $
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

/* xtt_hist.cpp -- Historical event window in xtt

   Author: Jonas Nylund.
   Last modification: 030217
*/

#if defined OS_LINUX

using namespace std;

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
#include "rt_mh_util.h"
#include "rt_elog.h"
#include "co_dcli.h"
#include <db.h>
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <Xm/ToggleB.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <deque>
#include <algorithm>

extern "C" {
#include "flow_x.h"
#include "co_mrm_util.h"
}
#include "co_lng.h"
#include "xtt_hist.h"
#include "rt_xnav_msg.h"

#define SENS 	1
#define INSENS  0
#define DONT_SET_SENS -1
/* 24 hours in seconds */
#define ONEDAY 86400

static void hist_display_in_xnav_cb( void *ctx, pwr_sAttrRef *arp);

static void hist_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);

static void hist_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			      unsigned long item_type, unsigned long utility,
			      char *arg, Widget *popup);

static void hist_action_inputfocus( Widget w, XmAnyCallbackStruct *data);

static void hist_activate_exit( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_activate_print( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_activate_zoom_in( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_activate_zoom_out( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_activate_zoom_reset( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_activate_open_plc( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_activate_display_in_xnav( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_activate_disp_hundredth( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data);

static void hist_activate_hide_object( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data);

static void hist_activate_hide_text( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data);

static void hist_activate_help( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_activate_helpevent( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_create_form( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_ok_btn( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

//callbackfunctions from the searchdialog
static void hist_cancel_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_start_time_help_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_start_time_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_start_time_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_stop_time_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_stop_time_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_today_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_yesterday_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_thisw_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_lastw_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_thism_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_lastm_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_all_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_time_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);


static void hist_alarm_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_info_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_ack_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_ret_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_prioA_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_prioB_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_prioC_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_prioD_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_event_text_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_event_text_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static void hist_event_name_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_event_name_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_nrofevents_string_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_search_string_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_search_string2_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_search_string3_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
static void hist_search_string4_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);

static int  GoBackMonth (pwr_tTime TimeIn, pwr_tTime *FromTime,
				pwr_tTime *ToTime);

static int  GoBackWeek (pwr_tTime TimeIn, pwr_tTime *FromTime,
				pwr_tTime *ToTime);

static void SetListTime(Hist *histOP, pwr_tTime StartTime, pwr_tTime StopTime, int Sensitive);

static int  GoBackWeek (pwr_tTime TimeIn, pwr_tTime *FromTime,
				pwr_tTime *ToTime);
static int  GoBackMonth (pwr_tTime TimeIn, pwr_tTime *FromTime,
				pwr_tTime *ToTime);

static pwr_tStatus AdjustForDayBreak(
    Hist *histOP, 
    pwr_tTime *Time, 
    pwr_tTime *NewTime
);


pwr_tStatus mh_clear_alarmlist_bc( pwr_tNodeIndex nix);
bool compDate(sEvent ev1, sEvent ev2);
Hist::Hist(
	void *hist_parent_ctx,
	Widget	hist_parent_wid,
	char *hist_name, pwr_tObjid objid,
	pwr_tStatus *status) :
	parent_ctx(hist_parent_ctx), parent_wid(hist_parent_wid),
	parent_wid_hist(NULL),
	start_trace_cb(NULL), display_in_xnav_cb(NULL), update_info_cb(NULL),
	help_cb(NULL), hist(NULL), minTime_str(NULL), maxTime_str(NULL),
	eventName_str(NULL), eventText_str(NULL), eventPrio_A(false),
	eventPrio_B(false), eventPrio_C(false), eventPrio_D(false),
	eventType_Ack(false), eventType_Alarm(false), eventType_Info(false),
	eventType_Return(false), eventType_Cancel(false), eventType_Block(false),
	eventType_Unblock(false), eventType_Reblock(false),
	eventType_CancelBlock(false)
{
  char		uid_filename[120] = {"xtt_hist.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  MrmType searchclass;

  static char hist_translations[] =
    "<FocusIn>: hist_inputfocus()\n";

  static XtTranslations hist_compiled_translations = NULL;

  static XtActionsRec hist_actions[] =
  {
    {"hist_inputfocus",      (XtActionProc) hist_action_inputfocus}
  };


  static MrmRegisterArg	reglist[] = {
        { "hist_ctx", 0 },
	{"hist_activate_exit",(caddr_t)hist_activate_exit },
	{"hist_activate_print",(caddr_t)hist_activate_print },
	{"hist_activate_zoom_in",(caddr_t)hist_activate_zoom_in },
	{"hist_activate_zoom_out",(caddr_t)hist_activate_zoom_out },
	{"hist_activate_zoom_reset",(caddr_t)hist_activate_zoom_reset },
	{"hist_activate_open_plc",(caddr_t)hist_activate_open_plc },
	{"hist_activate_display_in_xnav",(caddr_t)hist_activate_display_in_xnav },
	{"hist_activate_disp_hundredth",(caddr_t)hist_activate_disp_hundredth },
	{"hist_activate_hide_object",(caddr_t)hist_activate_hide_object },
	{"hist_activate_hide_text",(caddr_t)hist_activate_hide_text },
	{"hist_activate_help",(caddr_t)hist_activate_help },
	{"hist_activate_helpevent",(caddr_t)hist_activate_helpevent },
	{"hist_create_form",(caddr_t)hist_create_form },
	{"hist_ok_btn",(caddr_t)hist_ok_btn },
	{"hist_cancel_cb",(caddr_t)hist_cancel_cb },

	{"hist_start_time_entry_cr",(caddr_t)hist_start_time_entry_cr },
	{"hist_start_time_entry_lf",(caddr_t)hist_start_time_entry_lf },

	{"hist_stop_time_entry_cr",(caddr_t)hist_stop_time_entry_cr },
	{"hist_stop_time_entry_lf",(caddr_t)hist_stop_time_entry_lf },

	{"hist_today_cb",(caddr_t)hist_today_cb },
	{"hist_yesterday_cb",(caddr_t)hist_yesterday_cb },
	{"hist_thisw_cb",(caddr_t)hist_thisw_cb },
	{"hist_lastw_cb",(caddr_t)hist_lastw_cb },
	{"hist_thism_cb",(caddr_t)hist_thism_cb },
	{"hist_lastm_cb",(caddr_t)hist_lastm_cb },
	{"hist_all_cb",(caddr_t)hist_all_cb },
	{"hist_time_cb",(caddr_t)hist_time_cb },


	{"hist_alarm_toggle_cr",(caddr_t)hist_alarm_toggle_cr },
	{"hist_info_toggle_cr",(caddr_t)hist_info_toggle_cr },
	{"hist_ack_toggle_cr",(caddr_t)hist_ack_toggle_cr },
	{"hist_ret_toggle_cr",(caddr_t)hist_ret_toggle_cr },

	{"hist_prioA_toggle_cr",(caddr_t)hist_prioA_toggle_cr },
	{"hist_prioB_toggle_cr",(caddr_t)hist_prioB_toggle_cr },
	{"hist_prioC_toggle_cr",(caddr_t)hist_prioC_toggle_cr },
	{"hist_prioD_toggle_cr",(caddr_t)hist_prioD_toggle_cr },

	{"hist_event_text_entry_cr",(caddr_t)hist_event_text_entry_cr },
	{"hist_event_text_entry_lf",(caddr_t)hist_event_text_entry_lf },

	{"hist_event_name_entry_cr",(caddr_t)hist_event_name_entry_cr },
	{"hist_event_name_entry_lf",(caddr_t)hist_event_name_entry_lf },
	{"hist_nrofevents_string_label_cr",(caddr_t)hist_nrofevents_string_label_cr },
	{"hist_search_string_label_cr",(caddr_t)hist_search_string_label_cr },
	{"hist_search_string2_label_cr",(caddr_t)hist_search_string2_label_cr },
	{"hist_search_string3_label_cr",(caddr_t)hist_search_string3_label_cr },
	{"hist_search_string4_label_cr",(caddr_t)hist_search_string4_label_cr },
	{"hist_start_time_help_label_cr",(caddr_t)hist_start_time_help_label_cr }

	};
  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);


  
  *status = 1;

  Lng::get_uid( uid_filename, uid_filename);

  //shall be MessageHandler.EventLogSize
  hist_size = 100000;
  
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

  parent_wid_hist = XtCreatePopupShell( hist_name, 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "hist_window", parent_wid_hist,
			hist_name, args, 1, &toplevel_hist, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", hist_name);


  sts = MrmFetchWidgetOverride( s_DRMh, "histSearchDialog", form_hist,
			"searchdialog", args, 1, &toplevel_search, &searchclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", "histSearchDialog");


  MrmCloseHierarchy(s_DRMh);

  if ( hist_compiled_translations == NULL) 
  {
    XtAppAddActions( XtWidgetToApplicationContext( toplevel_hist), 
		hist_actions, XtNumber(hist_actions));
    hist_compiled_translations = XtParseTranslationTable( hist_translations);
  }
  XtOverrideTranslations( toplevel_hist, hist_compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,700);i++;
  XtSetArg(args[i],XmNheight,300);i++;
  XtSetValues( toplevel_search ,args,i);

  XtManageChild( toplevel_search);


  i = 0;
  XtSetArg(args[i],XmNwidth,700);i++;
  XtSetArg(args[i],XmNheight,600);i++;
  XtSetValues( toplevel_hist ,args,i);

  XtManageChild( toplevel_hist);
  
  // Create hist...
  hist = new EvList( this, form_hist, ev_eType_HistList, hist_size, &hist_widget);
  hist->start_trace_cb = &hist_start_trace_cb;
  hist->display_in_xnav_cb = &hist_display_in_xnav_cb;
  hist->popup_menu_cb = &hist_popup_menu_cb;


  XtPopup( parent_wid_hist, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid_hist, 
	(XtCallbackProc)hist_activate_exit, this);

  char name_str[80];
  sts = gdh_ObjidToName ( objid, name_str, sizeof(name_str), cdh_mName_pathStrict);
  if (ODD(sts))
  {
    if(this->event_name_entry_w != NULL)
    {
      XmTextSetString(this->event_name_entry_w, name_str);
      this->eventName_str = name_str;
      this->get_hist_list();
    }
  }
   

}


//
//  Delete hist
//
Hist::~Hist()
{
  if ( parent_wid_hist)
    XtDestroyWidget( parent_wid_hist);
  if ( hist)
    delete hist;
}

static void hist_ok_btn( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  brow_DeleteAll(histOP->hist->brow->ctx);
  histOP->eventType_Alarm = (bool)XmToggleButtonGetState(histOP->alarm_toggle_w);

  histOP->eventType_Ack = (bool)XmToggleButtonGetState(histOP->ack_toggle_w);

  histOP->eventType_Info = (bool)XmToggleButtonGetState(histOP->info_toggle_w);

  histOP->eventType_Return = (bool)XmToggleButtonGetState(histOP->ret_toggle_w);

  histOP->eventPrio_A = (bool)XmToggleButtonGetState(histOP->prioA_toggle_w);

  histOP->eventPrio_B = (bool)XmToggleButtonGetState(histOP->prioB_toggle_w);

  histOP->eventPrio_C = (bool)XmToggleButtonGetState(histOP->prioC_toggle_w);

  histOP->eventPrio_D = (bool)XmToggleButtonGetState(histOP->prioD_toggle_w);

  histOP->minTime_str = XmTextGetString(histOP->start_time_entry_w);

  histOP->maxTime_str = XmTextGetString(histOP->stop_time_entry_w);

  histOP->eventText_str = XmTextGetString(histOP->event_text_entry_w);

  histOP->eventName_str = XmTextGetString(histOP->event_name_entry_w);


  histOP->get_hist_list();
  
  XtFree(histOP->minTime_str);
  XtFree(histOP->maxTime_str);
  XtFree(histOP->eventText_str);
  XtFree(histOP->eventName_str);

}


static void hist_start_trace_cb( void *ctx, pwr_tObjid objid, char *name)
{
  if ( ((Hist *)ctx)->start_trace_cb)
    ((Hist *)ctx)->start_trace_cb( ((Hist *)ctx)->parent_ctx, objid, name);
}


static void hist_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			      unsigned long item_type, unsigned long utility,
			      char *arg, Widget *popup)
{
  if ( ((Hist *)ctx)->popup_menu_cb)
    (((Hist *)ctx)->popup_menu_cb) ( ((Hist *)ctx)->parent_ctx, attrref, item_type,
				   utility, arg, popup);
}

static void hist_display_in_xnav_cb( void *ctx, pwr_sAttrRef *arp)
{
  if ( ((Hist *)ctx)->display_in_xnav_cb)
    ((Hist *)ctx)->display_in_xnav_cb( ((Hist *)ctx)->parent_ctx, arp);
}



static void hist_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  Hist *hist;

  XtSetArg    (args[0], XmNuserData, &hist);
  XtGetValues (w, args, 1);


  if ( mrm_IsIconicState(w))
    return;

  if ( hist->hist) {
    if ( hist->focustimer.disabled())
      return;

    hist->hist->set_input_focus();
    hist->focustimer.disable( hist->toplevel_hist, 4000);
  }
  //printf("focus\n");
  //histOP->hist->set_input_focus();
//?????????????????????
  //if ( ev && ev->hist_displayed)
  //  ev->hist->set_input_focus();
  // hist->hist->set_input_focus();

}


static void hist_activate_exit( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->close_cb(histOP);
  //flow_UnmapWidget( histOP->parent_wid_hist);
}

static void hist_activate_print( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  char filename[200];
  dcli_translate_filename( filename, "$pwrp_tmp/xnav.ps");

  histOP->hist->print( filename);
}

static void hist_activate_zoom_in( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->hist->zoom( 1.2);
}

static void hist_activate_zoom_out( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->hist->zoom( 5.0/6);
}

static void hist_activate_zoom_reset( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->hist->unzoom();
}

static void hist_activate_open_plc( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->hist->start_trace();
}

static void hist_activate_display_in_xnav( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->hist->display_in_xnav();
}

static void hist_activate_disp_hundredth( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data)
{
  histOP->hist->set_display_hundredth( data->set);
}

static void hist_activate_hide_object( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data)
{
  histOP->hist->set_hide_object( data->set);
}

static void hist_activate_hide_text( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data)
{
  histOP->hist->set_hide_text( data->set);
}

static void hist_activate_help( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  if ( histOP->help_cb)
    (histOP->help_cb)( histOP->parent_ctx, "histlist");
}

static void hist_activate_helpevent( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  char	eventname[80];
  int 	sts;

  if ( histOP->help_cb)
  {
    sts = histOP->hist->get_selected_event( eventname);
    if( ODD(sts))
      (histOP->help_cb)( histOP->parent_ctx, eventname);
  }
}

static void hist_create_form( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->form_hist = w;
}


//callbackfunctions from the searchdialog
static void hist_cancel_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_cancel_cb\n");
}
static void hist_start_time_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->start_time_entry_w = w;
  XmTextSetString(histOP->start_time_entry_w, "1970-05-05 00:00:00");
  XtSetSensitive(histOP->start_time_entry_w, INSENS);
}

static void hist_start_time_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_start_time_entry_lf\n");
}

static void hist_stop_time_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->stop_time_entry_w = w;
  char buf[80];
  int	    Sts;
  pwr_tTime StopTime;
  pwr_tTime StartTime;

  Sts = clock_gettime(CLOCK_REALTIME, &StopTime);
  Sts = AdjustForDayBreak(histOP, &StopTime, &StartTime);

  StopTime = StartTime;
  StopTime.tv_sec += ONEDAY;
  
  time_AtoFormAscii(&StopTime, SWE, SECOND, buf, sizeof(buf));
  XmTextSetString(histOP->stop_time_entry_w, buf);
  XtSetSensitive(histOP->stop_time_entry_w, INSENS);
}

static void hist_stop_time_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_stop_time_entry_lf\n");
}

static void hist_alarm_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->alarm_toggle_w = w;
  //printf("hist_alarm_toggle_cr\n");
}

static void hist_info_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->info_toggle_w = w;
  //printf("hist_info_toggle_cr\n");
}

static void hist_ack_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->ack_toggle_w = w;
  //printf("hist_ack_toggle_cr\n");
}

static void hist_ret_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->ret_toggle_w = w;
  //printf("hist_ret_toggle_cr\n");
}


static void hist_prioA_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->prioA_toggle_w = w;
  //printf("hist_prioA_toggle_cr\n");
}

static void hist_prioB_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->prioB_toggle_w = w;
  //printf("hist_prioC_toggle_cr\n");
}

static void hist_prioC_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->prioC_toggle_w = w;
  //printf("hist_prioC_toggle_cr\n");
}

static void hist_prioD_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->prioD_toggle_w = w;
  //printf("hist_prioD_toggle_cr\n");
}

static void hist_event_text_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->event_text_entry_w = w;
  //printf("hist_event_text_entry_cr\n");
}

static void hist_event_text_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_event_text_entry_lf\n");
}

static void hist_event_name_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  histOP->event_name_entry_w = w;
  //printf("hist_event_name_entry_cr\n");
}

static void hist_event_name_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_event_name_entry_lf\n");
}

static void hist_nrofevents_string_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_search_string_label_cr\n");
  histOP->nrofevents_string_lbl_w = w;
}

static void hist_search_string_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_search_string_label_cr\n");
  histOP->search_string_lbl_w = w;
}
static void hist_search_string2_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_search_string_label_cr\n");
  histOP->search_string2_lbl_w = w;
}
static void hist_search_string3_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_search_string_label_cr\n");
  histOP->search_string3_lbl_w = w;
}
static void hist_search_string4_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_search_string_label_cr\n");
  histOP->search_string4_lbl_w = w;
}

static void hist_start_time_help_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
//  printf("hist_start_time_help_label_cr\n");
  histOP->start_time_help_lbl_w = w;
  char buf[40] = "Timeformat: 1970-05-05 23:30:00";
  XmString str = XmStringCreateLtoR(buf, "tag1");
  XtVaSetValues(histOP->start_time_help_lbl_w, XmNlabelString,str,NULL);
  XmStringFree(str);
}

static void hist_today_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
    int	    Sts;
    pwr_tTime StartTime;
    pwr_tTime StopTime;

    Sts = clock_gettime(CLOCK_REALTIME, &StopTime);
    Sts = AdjustForDayBreak(histOP, &StopTime, &StartTime);

    StopTime = StartTime;
    StopTime.tv_sec += ONEDAY;

    if (time_Acomp(&StartTime, &StopTime) > 0)
	StartTime.tv_sec -= ONEDAY;

    SetListTime(histOP, StartTime, StopTime, INSENS);
}

static void hist_yesterday_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
    int	    Sts;
    pwr_tTime StartTime;
    pwr_tTime StopTime;

    Sts = clock_gettime(CLOCK_REALTIME, &StopTime);
    Sts = AdjustForDayBreak(histOP, &StopTime, &StartTime);

    if (time_Acomp(&StartTime, &StopTime) > 0)
	StartTime.tv_sec -= ONEDAY;

    StopTime = StartTime;
    StartTime.tv_sec -= ONEDAY;
    
    SetListTime(histOP, StartTime, StopTime, INSENS);
}

static void hist_thisw_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  int	    Sts;
  pwr_tTime CurrTime;
  pwr_tTime StartTime;
  pwr_tTime StopTime;

  Sts = clock_gettime(CLOCK_REALTIME, &CurrTime);

  Sts = GoBackWeek (CurrTime, &StartTime, &StopTime);

  StopTime.tv_sec += ONEDAY;
  CurrTime.tv_sec += ONEDAY;
  
  SetListTime(histOP, StopTime, CurrTime, INSENS);
}

static void hist_lastw_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  int	    Sts;
  pwr_tTime CurrTime;
  pwr_tTime StartTime;
  pwr_tTime StopTime;

  Sts = clock_gettime(CLOCK_REALTIME, &CurrTime);

  Sts = GoBackWeek (CurrTime, &StartTime, &StopTime);

  SetListTime(histOP, StartTime, StopTime, INSENS);

}

static void hist_thism_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  int	    Sts;
  pwr_tTime CurrTime;
  pwr_tTime StartTime;
  pwr_tTime StopTime;

  Sts = clock_gettime(CLOCK_REALTIME, &CurrTime);

  Sts = GoBackMonth (CurrTime, &StartTime, &StopTime);

  StopTime.tv_sec += ONEDAY;
  CurrTime.tv_sec += ONEDAY;
  
  SetListTime(histOP, StopTime, CurrTime, INSENS);

}

static void hist_lastm_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
  int	    Sts;
  pwr_tTime CurrTime;
  pwr_tTime StartTime;
  pwr_tTime StopTime;

  Sts = clock_gettime(CLOCK_REALTIME, &CurrTime);

  Sts = GoBackMonth (CurrTime, &StartTime, &StopTime);

  SetListTime(histOP, StartTime, StopTime, INSENS);
}

static void hist_all_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
    int	    Sts;
    pwr_tTime StartTime;
    pwr_tTime StopTime;
    char timestr[32] = "01-JAN-1970 00:00:00";

    time_AsciiToA(timestr, &StartTime);
    
    Sts = clock_gettime(CLOCK_REALTIME, &StopTime);

    StopTime.tv_sec += ONEDAY;

    SetListTime(histOP, StartTime, StopTime, INSENS);
}

static void hist_time_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data)
{
    int	    Sts;
    pwr_tTime StartTime;
    pwr_tTime StopTime;
    char timestr[32] = "01-JAN-1970 00:00:00";

    time_AsciiToA(timestr, &StartTime);
    
    Sts = clock_gettime(CLOCK_REALTIME, &StopTime);

    SetListTime(histOP, StartTime, StopTime, SENS);

}



pwr_tStatus Hist::hist_add_ack_mess( mh_sAck *MsgP)
{
  // Insert in hist
  hist->event_ack( MsgP);
  return 1;
}

pwr_tStatus Hist::hist_add_return_mess( mh_sReturn *MsgP)
{
  // Insert in hist
  hist->event_return( MsgP);
  return 1;
}

pwr_tStatus Hist::hist_add_alarm_mess( mh_sMessage *MsgP)
{
  hist->event_alarm( MsgP);
  return 1;
}

pwr_tStatus Hist::hist_add_info_mess( mh_sMessage *MsgP)
{
  hist->event_info( MsgP);
  return 1;
}

pwr_tStatus Hist::hist_clear_histlist()
{
//  hist->event_clear_list();
  return 1;
}

void Hist::get_hist_list()
{
  DB *dataBaseP = NULL;
  pwr_tInt32 ret, sts;
  char dbName[200];
  
  //printf("I get_hist_list\n");
  
  dcli_translate_filename( dbName, DATABASE);
  
  switch(Lng::lang)
  {
    case lng_eLanguage_sv_se:
      printSearchStr_sv_se();
      break;
    default:
      printSearchStr_en_us();
      break;
  }  

  /*create the database if it's not already created*/
  if((ret = db_create(&dataBaseP, NULL, 0)) != 0)
  {
    /*error creating db-handle send the mess to errh, then exit*/
    printf("error db_create: %s\n", db_strerror(ret));
    printf(" Fel vid skapande av databashandtag avslutar\n");
    return;
  }
  if((ret = dataBaseP->open(dataBaseP, dbName, NULL, DATABASETYPE, DB_RDONLY, 0)) != 0)
  {
    /*error opening/creating db send the mess to errh, then exit*/
    printf("error db_open: %s\n", db_strerror(ret));
    //goto err;
  }

  pwr_tUInt32 nrOfEvents = 0;
  sEvent *eventp;
  DBT data, key;
  DBC *dbcp;
  deque<sEvent> evDeq;
    
  /* Acquire a cursor for the database. */ 
  if ((ret = dataBaseP->cursor(dataBaseP, NULL, &dbcp, 0)) != 0) 
  {
    printf("error dataBaseP->cursor: %s\n", db_strerror(ret)); 
    return;
  }

  /* Initialize the key/data return pair. */
  memset(&key, 0, sizeof(key));
  memset(&data, 0, sizeof(data));
  
  if((ret = dbcp->c_get(dbcp, &key, &data, DB_FIRST)) == 0)
  {
    eventp = (sEvent *)data.data;
    sts = check_conditions(eventp);
    if(sts == ERROR_TIME_CONVERT)
    {
      printf("Error trying to convert userinput in time-field\n");
    }
    if(ODD(sts))
    {
      nrOfEvents++;
      evDeq.push_front((sEvent)*eventp);
    }
    
      
  }
  
  while((ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT)) == 0)
  {
    eventp = (sEvent *)data.data;
    sts = check_conditions(eventp);
    if(ODD(sts))
    {
      nrOfEvents++;
      evDeq.push_front(*eventp);
    }
    else if(sts == ERROR_TIME_CONVERT)
    {
      printf("Error trying to convert userinput in time-field\n");
    }
  }
  if(ret != DB_NOTFOUND)
  {
    printf("error dbcp->c_get: %s\n", db_strerror(ret));
    printf("Fel vid försök att läsa post nr %u, avslutar\n", nrOfEvents);
  }
  char buf[20];
  sprintf(buf, "    %u", nrOfEvents);
  XmString str = XmStringCreateLtoR(buf, "tag1");
  XtVaSetValues(this->nrofevents_string_lbl_w, XmNlabelString,str,NULL);
  XmStringFree(str);
  /*Close the cursor*/
  if((ret = dbcp->c_close(dbcp)) != 0)
  {
    printf("Error dbcp->c_close(): %s\n", db_strerror(ret));
  }
  /*close the database*/  

  if((ret = dataBaseP->close(dataBaseP,0) != 0))
  {
    printf("error db_close: %s\n", db_strerror(ret));
  }
    
  deque<sEvent>::iterator it;
  it = evDeq.begin();
  //sort the deque
  sort(it, evDeq.end(), compDate);
  hist->set_nodraw();
  while(it != evDeq.end())
  {
    switch (it->EventType) 
    {
      case mh_eEvent_Alarm:
        hist_add_alarm_mess( &(it->Mess.message) );
        break;
      case mh_eEvent_Info:
        hist_add_info_mess( &(it->Mess.message) );
        break;
      case mh_eEvent_Ack:
        hist_add_ack_mess( &(it->Mess.ack) );
        break;
      case mh_eEvent_Cancel:
        //strcpy(ret, "Cancel");
        break;
      case mh_eEvent_Return:
        hist_add_return_mess( &(it->Mess.ret) );
        break;
      case mh_eEvent_Block:
        //strcpy(ret, "Block");
        break;
      case mh_eEvent_Unblock:
        //strcpy(ret, "Unblock");
        break;
      case mh_eEvent_Reblock:
        //strcpy(ret, "Reblock");
        break;
      case mh_eEvent_CancelBlock:
        //strcpy(ret, "CancelBlock");
        break;
      default:
        //strcpy(ret, " ");
        break;
    }
    
    it++;
  }
  hist->reset_nodraw();   
}

//sorting function that might be more complicated in the future
//we might want to sort the display of the events with more conditions than 
//just the time
bool compDate(sEvent ev1, sEvent ev2)
{
  return(time_Acomp(&(ev1.EventTime), &(ev2.EventTime)) > 0);
}

int Hist::check_conditions(sEvent *evp)
{
  pwr_tTime minTime;
  pwr_tTime maxTime;
  int sts;

  //first we compare the time
  if( (minTime_str != NULL) && (strlen(minTime_str) != 0) )
  {
    sts = time_FormAsciiToA(minTime_str, SWE, SECOND, &minTime);
    if(EVEN(sts)) 
      return ERROR_TIME_CONVERT;
    if(time_Acomp(&minTime, &(evp->EventTime)) > 0)
      return 2; //evensts
  }
  if(maxTime_str != NULL && (strlen(maxTime_str) != 0))
  {
    sts = time_FormAsciiToA(maxTime_str, SWE, SECOND, &maxTime);
    if(EVEN(sts)) 
      return ERROR_TIME_CONVERT;
    if(time_Acomp(&maxTime, &(evp->EventTime)) < 0)
      return 2; //evensts
  }
  bool ret = false;
  //then we compare the EventType if nothing is selected everything is selected
  if(eventType_Ack || eventType_Alarm || eventType_Info || eventType_Return || eventType_Cancel || eventType_Block ||
     eventType_Unblock || eventType_Reblock || eventType_CancelBlock)
  {
    switch(evp->EventType) 
    {
      case mh_eEvent_Alarm:
        if(!eventType_Alarm)
	  ret = true;
        break;
      case mh_eEvent_Info:
        if(!eventType_Info)
	  ret = true;
        break;
      case mh_eEvent_Ack:
        if(!eventType_Ack)
	  ret = true;
        break;
      case mh_eEvent_Cancel:
        if(!eventType_Cancel)
	  ret = true;
        break;
      case mh_eEvent_Return:
        if(!eventType_Return)
	  ret = true;
        break;
      case mh_eEvent_Block:
        if(!eventType_Block)
	  ret = true;
        break;
      case mh_eEvent_Unblock:
        if(!eventType_Unblock)
	  ret = true;
        break;
      case mh_eEvent_Reblock:
        if(!eventType_Reblock)
	  ret = true;
        break;
      case mh_eEvent_CancelBlock:
        if(!eventType_CancelBlock)
	  ret = true;
        break;
      default:
        ret = true;
    }
  }
  if(ret)
    return 2;
  
  
  mh_sMsgInfo *msgInfop = NULL;
  mh_sMessage *mp = NULL;
  mh_sReturn  *rp = NULL;
  
  switch (evp->EventType) 
  {
    case mh_eEvent_Alarm:
    case mh_eEvent_Info:
      msgInfop = &(evp->Mess.message.Info);
      mp = &(evp->Mess.message);
      break;
    case mh_eEvent_Ack:
      msgInfop = &(evp->Mess.ack.Info);
      break;
    case mh_eEvent_Cancel:
    case mh_eEvent_Return:
      msgInfop = &(evp->Mess.ret.Info);
      rp = &(evp->Mess.ret);
      break;
    case mh_eEvent_Block:
    case mh_eEvent_Unblock:
    case mh_eEvent_Reblock:
    case mh_eEvent_CancelBlock:
      msgInfop = &(evp->Mess.block.Info);
      break;
    default:
      return 2;
  }
  
  //compare the prio, if nothing is selected everything is selected
  if(this->eventPrio_A || this->eventPrio_B || this->eventPrio_C || this->eventPrio_D)
  {
    //compare the EventPrio
    switch (msgInfop->EventPrio) {
      case mh_eEventPrio_A:
        if(!eventPrio_A)
          return 2;
        break;
      case mh_eEventPrio_B:
        if(!eventPrio_B)
          return 2;
        break;
      case mh_eEventPrio_C:
        if(!eventPrio_C)
          return 2;
        break;
      case mh_eEventPrio_D:
        if(!eventPrio_D)
          return 2;
        break;
      default:
        return 2;
    }
  }

  //compare the EventName
  if(eventName_str != NULL && (strlen(eventName_str) != 0) )
  {
    if( EVEN( compareStr(msgInfop->EventName, eventName_str) ) )
      return 2;
  }
  if(eventText_str != NULL && (strlen(eventText_str) != 0) )
  {
    if(mp != NULL)
    {
      if( EVEN( compareStr(mp->EventText, eventText_str) ) )
        return 2;
    }
    else if(rp != NULL)
    {
      if( EVEN( compareStr(rp->EventText, eventText_str) ) )
        return 2;
    }
    else
      return 2;
  }
  
  //every condition was true so the mess is alright
  return 1;
  
}

int Hist::compareStr(char *ev, char *usr)
{
  int sts;
  int startPos = 0;
  unsigned int endPos = 0;
  char *str1;
  sts = dcli_toupper(usr,usr);
  if(ODD(sts))
  {
    sts = dcli_wildcard(usr,ev);
    sts += 1;
    if(ODD(sts))
      return sts;

    string s = usr;	
    endPos = s.find_first_of(';', startPos);
    while(endPos != string::npos)
    {
      str1 = (char *)(s.substr(startPos, endPos-startPos)).c_str();
      sts = dcli_wildcard(str1,ev);
      sts += 1;
      startPos = endPos+1;
      if(ODD(sts))
        return sts;
      endPos = s.find_first_of(';', startPos);
    }
    str1 = (char *)(s.substr(startPos)).c_str();
    sts = dcli_wildcard(str1,ev);
    sts += 1;
  }
  return sts;
}

void Hist::printSearchStr_en_us()
{
  //return;
  bool addAnd = false;
  bool prioPrinted = false;
  int i = 0;
  string searchStr[4];
  searchStr[0] = " All events";
  searchStr[1] = "";
  searchStr[2] = "";
  searchStr[3] = "";

  if((minTime_str != NULL) && (strlen(minTime_str) > 0) )  
  {
    searchStr[i] += " From ";
    searchStr[i] += minTime_str;
    addAnd = true;
  }
  if((maxTime_str != NULL) && (strlen(maxTime_str) > 0) )  
  {
    searchStr[i] += " To ";
    searchStr[i] += maxTime_str;
    addAnd = true;
  }
  if(eventPrio_A || eventPrio_B || eventPrio_C || eventPrio_D)
  {
    if(addAnd) searchStr[i] += " and";
    i++;
    prioPrinted = true;
    searchStr[i] += " with prio ";
    if(eventPrio_A)
      searchStr[i] += "A";
    if(eventPrio_B)
      searchStr[i] += "B";
    if(eventPrio_C)
      searchStr[i] += "C";
    if(eventPrio_D)
      searchStr[i] += "D";
    addAnd = true;
   }
  if(eventType_Ack || eventType_Alarm || eventType_Info || eventType_Return)
  {
    if(addAnd) searchStr[i] += " and";
    if(!prioPrinted)
      i++;
    searchStr[i] += " with type ";
    if(eventType_Ack)
      searchStr[i] += "Ack ";
    if(eventType_Alarm)
      searchStr[i] += "Active ";
    if(eventType_Info)
      searchStr[i] += "Info ";
    if(eventType_Return)
      searchStr[i] += "Return";
    addAnd = true;
  }
  if((eventName_str != NULL) && (strlen(eventName_str) > 0) )  
  {
    if(addAnd) searchStr[i] += " and";
    i++;
    searchStr[i] += " with EventName ";
    searchStr[i] += eventName_str;
    addAnd = true;
  }
  if((eventText_str != NULL) && (strlen(eventText_str) > 0) )  
  {
    if(addAnd) searchStr[i] += " and";
    i++;
    searchStr[i] += " with EventText ";
    searchStr[i] += eventText_str;
  }
  char buf[searchStr[0].size() + 1];
  strcpy(buf, searchStr[0].c_str());
  XmString str = XmStringCreateLtoR(buf, "tag1");
  XtVaSetValues(this->search_string_lbl_w, XmNlabelString,str,NULL);
  XmStringFree(str);

  char buf2[searchStr[1].size() + 1];
  strcpy(buf2, searchStr[1].c_str());
  XmString str2 = XmStringCreateLtoR(buf2, "tag1");
  XtVaSetValues(this->search_string2_lbl_w, XmNlabelString,str2,NULL);
  XmStringFree(str2);

  char buf3[searchStr[2].size() + 1];
  strcpy(buf3, searchStr[2].c_str());
  XmString str3 = XmStringCreateLtoR(buf3, "tag1");
  XtVaSetValues(this->search_string3_lbl_w, XmNlabelString,str3,NULL);
  XmStringFree(str3);

  char buf4[searchStr[3].size() + 1];
  strcpy(buf4, searchStr[3].c_str());
  XmString str4 = XmStringCreateLtoR(buf4, "tag1");
  XtVaSetValues(this->search_string4_lbl_w, XmNlabelString,str4,NULL);
  XmStringFree(str4);

}

void Hist::printSearchStr_sv_se()
{
  //return;
  bool addAnd = false;
  bool prioPrinted = false;
  int i = 0;
  string searchStr[4];
  searchStr[0] = " Alla händelser";
  searchStr[1] = "";
  searchStr[2] = "";
  searchStr[3] = "";
  if((minTime_str != NULL) && (strlen(minTime_str) > 0) )  
  {
    searchStr[i] += " Från ";
    searchStr[i] += minTime_str;
    addAnd = true;
  }
  if((maxTime_str != NULL) && (strlen(maxTime_str) > 0) )  
  {
    searchStr[i] += " till ";
    searchStr[i] += maxTime_str;
    addAnd = true;
  }
  if(eventPrio_A || eventPrio_B || eventPrio_C || eventPrio_D)
  {
    if(addAnd) searchStr[i] += " och";
    prioPrinted = true;
    i++;
    searchStr[i] += " med prio ";
    if(eventPrio_A)
      searchStr[i] += "A";
    if(eventPrio_B)
      searchStr[i] += "B";
    if(eventPrio_C)
      searchStr[i] += "C";
    if(eventPrio_D)
      searchStr[i] += "D";
    addAnd = true;
   }
  if(eventType_Ack || eventType_Alarm || eventType_Info || eventType_Return)
  {
    if(addAnd) searchStr[i] += " och";
    if(!prioPrinted)
      i++;
    searchStr[i] += " med typ ";
    if(eventType_Ack)
      searchStr[i] += "Kvittens ";
    if(eventType_Alarm)
      searchStr[i] += "Aktiv ";
    if(eventType_Info)
      searchStr[i] += "Info ";
    if(eventType_Return)
      searchStr[i] += "Retur";
    addAnd = true;
  }
  if((eventName_str != NULL) && (strlen(eventName_str) > 0) )  
  {
    if(addAnd) searchStr[i] += " och";
    i++;
    searchStr[i] += " med Händelsenamn ";
    searchStr[i] += eventName_str;
    addAnd = true;
  }
  if((eventText_str != NULL) && (strlen(eventText_str) > 0) )  
  {
    if(addAnd) searchStr[i] += " och";
    i++;
    searchStr[i] += " med händelsetext ";
    searchStr[i] += eventText_str;
  }
  char buf[searchStr[0].size() + 1];
  strcpy(buf, searchStr[0].c_str());
  XmString str = XmStringCreateLtoR(buf, "tag1");
  XtVaSetValues(this->search_string_lbl_w, XmNlabelString,str,NULL);
  XmStringFree(str);

  char buf2[searchStr[1].size() + 1];
  strcpy(buf2, searchStr[1].c_str());
  XmString str2 = XmStringCreateLtoR(buf2, "tag1");
  XtVaSetValues(this->search_string2_lbl_w, XmNlabelString,str2,NULL);
  XmStringFree(str2);

  char buf3[searchStr[2].size() + 1];
  strcpy(buf3, searchStr[2].c_str());
  XmString str3 = XmStringCreateLtoR(buf3, "tag1");
  XtVaSetValues(this->search_string3_lbl_w, XmNlabelString,str3,NULL);
  XmStringFree(str3);

  char buf4[searchStr[3].size() + 1];
  strcpy(buf4, searchStr[3].c_str());
  XmString str4 = XmStringCreateLtoR(buf4, "tag1");
  XtVaSetValues(this->search_string4_lbl_w, XmNlabelString,str4,NULL);
  XmStringFree(str4);
}





/************************************************************************
*
* Name:	GoBackMonth (TimeIn, FromTime, ToTime)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tTime	TimeIn		I	Start time	
* pwr_tTime	*FromTime	O	The first day of the month
* pwr_tTime     *ToTime		O	The last day of the month
*
* Description:	This function computes dates for the first to the last day
*		of the previous month, from the time TimeIn.
*		Output times are only date, e g 1-MAY-1992 00:00:00.00.
*************************************************************************/
static int  GoBackMonth (pwr_tTime TimeIn, pwr_tTime *FromTime,
				pwr_tTime *ToTime)
{
    struct tm	*Tm;
    int		DaysOfMonth, Month, TmYear, Year;

    /* Get the time in and blank time values. */

    Tm = localtime(&TimeIn.tv_sec);

    /* Get number of days in the previous month. */
    if (Tm->tm_mon == 0)
    {
	Month = 11;
	TmYear = Tm->tm_year - 1;
    }
    else
    {
	Month = Tm->tm_mon - 1;
	TmYear = Tm->tm_year;
    }


    switch (Month)
    {
	case 0:
	case 2:
	case 4:
	case 6:
	case 7:
	case 9:
	case 11:
	    DaysOfMonth = 31;
	    break;

	case 3:
	case 5:
	case 8:
	case 10:
	    DaysOfMonth = 30;
	    break;

	case 1: 
    	    Year = TmYear + 1900;
	    if ((Year % 4 == 0 && Year % 100 != 0) || (Year + 1900) % 400 == 0)
		DaysOfMonth = 29;
	    else
		DaysOfMonth = 28;
	    break;
    }

    memset(Tm, 0, sizeof(*Tm));
    Tm->tm_mday = 1; 
    Tm->tm_mon = Month; 
    Tm->tm_year = TmYear; 

    FromTime->tv_sec = mktime(Tm);
    FromTime->tv_nsec = 0;

    Tm->tm_mday = DaysOfMonth; 
    ToTime->tv_sec = mktime(Tm);
    ToTime->tv_nsec = 0;

    return(1);
}   /* END GoBackMonth */


/************************************************************************
*
* Name:	GoBackWeek (TimeIn, FromTime, ToTime)
*
* Type:	int
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tTime	TimeIn		I	Start time	
* pwr_tTime	*FromTime	O	The first day of the month
* pwr_tTime     *ToTime		O	The last day of the month
*
* Description:	This function computes dates for monday to sunday in the
*		previous week from the time now.
*		Output times are only date, e g 1-MAY-1992 00:00:00.00.
*************************************************************************/
static int  GoBackWeek (pwr_tTime TimeIn, pwr_tTime *FromTime,
				pwr_tTime *ToTime)
{
    struct tm	*Tm;
    int		Days;
    pwr_tTime   Time;

    Tm = localtime(&TimeIn.tv_sec);
    if (Tm->tm_wday == 0) /* Sunday */
	Days = 13;
    else
    	Days = Tm->tm_wday + 6;

    Tm->tm_sec = 0;
    Tm->tm_min = 0;
    Tm->tm_hour = 0;
    Time.tv_sec = mktime(Tm);

    FromTime->tv_sec = Time.tv_sec - Days * ONEDAY;
    FromTime->tv_nsec = 0;

    ToTime->tv_sec = Time.tv_sec - (Days - 6) * ONEDAY;
    ToTime->tv_nsec = 0;

    return(1);
}   /* END GoBackWeek */


/************************************************************************
*
* Name:		SetListTime
*
* Type:		
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* 
*
* Description:	Sets the Time field for start and stop
* 
*************************************************************************/
static void SetListTime(Hist *histOP, pwr_tTime StartTime, pwr_tTime StopTime, int Sensitive)
{
    char timestr[32];

    /* Show the resulting times */
    time_AtoFormAscii(&StopTime,SWE, SECOND,timestr,sizeof(timestr));
    XmTextSetString(histOP->stop_time_entry_w, timestr);
    
 
    time_AtoFormAscii(&StartTime,SWE, SECOND,timestr,sizeof(timestr));
    XmTextSetString(histOP->start_time_entry_w, timestr);   
    
    if (Sensitive != DONT_SET_SENS)
    {
	XtSetSensitive(histOP->start_time_entry_w, Sensitive);
	XtSetSensitive(histOP->stop_time_entry_w, Sensitive);
    }

}/* SetListTime */

/************************************************************************
*
* Name: AdjustForDayBreak
*
* Type: void
*
* Type          Parameter       IOGF    Description
* pwr_tTime     *Time           I       The time
* pwr_tTime	*NewTime  	 O	The adjusted time
*
* Description:
*       
*
*************************************************************************/
static pwr_tStatus AdjustForDayBreak(
    Hist *histOP, 
    pwr_tTime *Time, 
    pwr_tTime *NewTime
)
{
    pwr_tStatus Sts;
    char timestr[32]; 
    //char *temp;
    //XmString cs;

    Sts = time_AtoAscii(Time, time_eFormat_DateAndTime, timestr, sizeof(timestr));
    //XtVaGetValues(histOP->ListDaybreakValueW, XmNlabelString, &cs, NULL);
    //CompStrToStr (cs, &temp);
    //sprintf(&timestr[12],"%s:00.00", temp);
    //XtFree(temp);
    
    sprintf(&timestr[12],"00:00:00");
//    printf("timestr %s", timestr);
    return time_AsciiToA(timestr, NewTime);

} /* AdjustForDayBreak */

#endif

















