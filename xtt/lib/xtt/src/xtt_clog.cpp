/* xtt_clog.cpp -- Alarm and event window in xtt

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <Xm/ToggleB.h>
#include <Xm/List.h>

extern "C" {
#include "flow_x.h"
}
#include "co_lng.h"
#include "co_wow.h"
#include "xtt_clog.h"
#include "rt_xnav_msg.h"

static void clog_action_inputfocus( Widget w, XmAnyCallbackStruct *data);
static void clog_activate_exit( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_select_file( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_next_file( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_prev_file( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_zoom_in( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_zoom_out( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_zoom_reset( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_filter( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_help( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_activate_helpmsg( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_create_form( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_filter_tog_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_filter_string_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_filter_act_but_cb( Widget w, CLog *clog);
static void clog_filesel_list_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data);
static void clog_filesel_ok_cb( Widget w, CLog *clog);
static void clog_filesel_cancel_cb( Widget w, CLog *clog);

CLog::CLog(
	void *clog_parent_ctx,
	Widget	clog_parent_wid,
	char *clog_name,
	pwr_tStatus *status) :
	parent_ctx(clog_parent_ctx), parent_wid(clog_parent_wid),
	clognav(NULL), clog_displayed(0), help_cb(0), close_cb(0), filesel_loaded(false),
	clock_cursor(0)
{
  char		uid_filename[120] = {"xtt_clog.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;

  static char clog_translations[] =
    "<FocusIn>: clog_inputfocus()\n";
  static XtTranslations clog_compiled_translations = NULL;
  static XtActionsRec clog_actions[] =
  {
    {"clog_inputfocus",      (XtActionProc) clog_action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "clog_ctx", 0 },
	{"clog_activate_exit",(caddr_t)clog_activate_exit },
	{"clog_activate_select_file",(caddr_t)clog_activate_select_file },
	{"clog_activate_next_file",(caddr_t)clog_activate_next_file },
	{"clog_activate_prev_file",(caddr_t)clog_activate_prev_file },
	{"clog_activate_zoom_in",(caddr_t)clog_activate_zoom_in },
	{"clog_activate_zoom_out",(caddr_t)clog_activate_zoom_out },
	{"clog_activate_zoom_reset",(caddr_t)clog_activate_zoom_reset },
	{"clog_activate_filter",(caddr_t)clog_activate_filter },
	{"clog_activate_help",(caddr_t)clog_activate_help },
	{"clog_activate_helpmsg",(caddr_t)clog_activate_helpmsg },
	{"clog_create_form",(caddr_t)clog_create_form },
	{"clog_filter_tog_cr",(caddr_t)clog_filter_tog_cr },
	{"clog_filter_string_cr",(caddr_t)clog_filter_string_cr },
	{"clog_filter_act_but_cb",(caddr_t)clog_filter_act_but_cb },
	{"clog_filesel_ok_cb",(caddr_t)clog_filesel_ok_cb },
	{"clog_filesel_cancel_cb",(caddr_t)clog_filesel_cancel_cb },
	{"clog_filesel_list_cr",(caddr_t)clog_filesel_list_cr }
	};
  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  *status = 1;

  Lng::get_uid( uid_filename, uid_filename);

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

  parent_wid_clog = XtCreatePopupShell( clog_name, 
		topLevelShellWidgetClass, parent_wid, args, i);

  sts = MrmFetchWidgetOverride( s_DRMh, "clog_window", parent_wid_clog,
			clog_name, args, 1, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", clog_name);

  sts = MrmFetchWidget(s_DRMh, "filterForm", toplevel,
		&filter_form, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch filter dialog\n");

  sts = MrmFetchWidget(s_DRMh, "fileselForm", toplevel,
		&filesel_form, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch filesel dialog\n");

  MrmCloseHierarchy(s_DRMh);

  if ( clog_compiled_translations == NULL) 
  {
    XtAppAddActions( XtWidgetToApplicationContext( toplevel), 
		clog_actions, XtNumber(clog_actions));
    clog_compiled_translations = XtParseTranslationTable( clog_translations);
  }
  XtOverrideTranslations( toplevel, clog_compiled_translations);

  i = 0;
  XtSetArg(args[i],XmNwidth,1000);i++;
  XtSetArg(args[i],XmNheight,800);i++;
  XtSetValues( toplevel ,args,i);
    
  XtManageChild( toplevel);

  // Create clognav
  clognav = new CLogNav( this, form_clog, &clognav_widget);

  XtPopup( parent_wid_clog, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid_clog, 
	(XtCallbackProc)clog_activate_exit, this);

  *status = 1;
}


//
//  Delete clog
//
CLog::~CLog()
{
  free_cursor();
  XtDestroyWidget( parent_wid_clog);
  delete clognav;
}

void CLog::pop()
{
  flow_UnmapWidget( parent_wid_clog);
  flow_MapWidget( parent_wid_clog);
}

void CLog::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = XCreateFontCursor( flow_Display(toplevel), XC_watch);

  XDefineCursor( flow_Display(toplevel), flow_Window(toplevel), clock_cursor);
  XFlush( flow_Display(toplevel));
}

void CLog::reset_cursor()
{
  XUndefineCursor( flow_Display(toplevel), flow_Window(toplevel));
}

void CLog::free_cursor()
{
  if (clock_cursor)
    XFreeCursor( flow_Display(toplevel), clock_cursor);
}

static void clog_action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  CLog *clog;

  XtSetArg    (args[0], XmNuserData, &clog);
  XtGetValues (w, args, 1);

  if ( clog && clog->clog_displayed)
    clog->clognav->set_input_focus();
}

static void clog_activate_exit( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  if ( clog->close_cb)
    (clog->close_cb)( clog->parent_ctx);
  else
    delete clog;
}

static void clog_activate_zoom_in( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->clognav->zoom( 1.2);
}

static void clog_activate_zoom_out( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->clognav->zoom( 5.0/6);
}

static void clog_activate_zoom_reset( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->clognav->unzoom();
}

static void clog_activate_filter( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  bool success, info, warning, error, fatal, text;

  clog->clognav->get_filter( &success, &info, &warning, &error, &fatal, &text);
  XmToggleButtonSetState(clog->show_success_w, success, 0);
  XmToggleButtonSetState(clog->show_info_w, info, 0);
  XmToggleButtonSetState(clog->show_warning_w, warning, 0);
  XmToggleButtonSetState(clog->show_error_w, error, 0);
  XmToggleButtonSetState(clog->show_fatal_w, fatal, 0);
  XmToggleButtonSetState(clog->show_text_w, text, 0);

  XtManageChild( clog->filter_form);
}

static void clog_activate_select_file( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  XmString cstr;  
  char str[200];
  char *s;

  if ( !clog->filesel_loaded) {
    for ( int i = 0; i < (int) clog->clognav->file_list.size(); i++) {
      time_AtoAscii( &clog->clognav->file_list[i].time, time_eFormat_ComprDateAndTime, 
		     str, sizeof(str));
      str[17] = 0;
      strcat( str, "    ");
      s = strrchr( clog->clognav->file_list[i].name, '/');
      if ( s)
	strcat( str, s+1);
      else
	strcat( str, clog->clognav->file_list[i].name);

      cstr = XmStringCreateSimple( str);
      XmListAddItemUnselected( clog->filesel_list_w, cstr, 0);
      XmStringFree(cstr);
    }
    clog->filesel_loaded = true;
  }
  XtManageChild( clog->filesel_form);
}

static void clog_activate_next_file( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->set_clock_cursor();
  if ( !clog->clognav->next_file())
    wow_DisplayError( clog->toplevel, "File error", "No next file found");
  clog->reset_cursor();
}

static void clog_activate_prev_file( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->set_clock_cursor();
  if ( !clog->clognav->prev_file())
    wow_DisplayError( clog->toplevel, "File error", "No previous file found");
  clog->reset_cursor();
}

static void clog_activate_help( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  if ( clog->help_cb)
    (clog->help_cb)( clog->parent_ctx, "consolelog");
}

static void clog_activate_helpmsg( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
}

static void clog_create_form( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->form_clog = w;
}

static void clog_filter_string_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data) 
{
  clog->filter_string_w = w;
}
static void clog_filter_tog_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data) 
{

  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) {
  case 1:
    // Success
    clog->show_success_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 2:
    // Info
    clog->show_info_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 3:
    // Warning
    clog->show_warning_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 4:
    // Error
    clog->show_error_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 5:
    // Fatal
    clog->show_fatal_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 6:
    // Text
    clog->show_text_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  default:
    break;
  }
}

static void clog_filter_act_but_cb( Widget w, CLog *clog)
{
  int key;
  char *str;

  bool success = XmToggleButtonGetState( clog->show_success_w);
  bool info = XmToggleButtonGetState( clog->show_info_w);
  bool warning = XmToggleButtonGetState( clog->show_warning_w);
  bool error = XmToggleButtonGetState( clog->show_error_w);
  bool fatal = XmToggleButtonGetState( clog->show_fatal_w);
  bool text = XmToggleButtonGetState( clog->show_text_w);
  str = XmTextGetString( clog->filter_string_w);

  XtVaGetValues(w, XmNuserData, &key, NULL);
  switch (key) {
  case 1 :
    // Ok
    XtUnmanageChild( clog->filter_form);
  case 2 :
    // Apply
    clog->set_clock_cursor();
    clog->clognav->set_filter( success, info, warning, error, fatal, text, str);
    clog->reset_cursor();
    break;

  case 3 :
    // Cancel
    XtUnmanageChild( clog->filter_form);
    break;
  }
}

static void clog_filesel_list_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->filesel_list_w = w;
}

static void clog_filesel_ok_cb( Widget w, CLog *clog)
{
  int *pos_list, pos_cnt;
  int key;

  if (XmListGetSelectedPos( clog->filesel_list_w, &pos_list, &pos_cnt)) {
    clog->set_clock_cursor();
    clog->clognav->read( pos_list, pos_cnt);
    clog->reset_cursor();
  }

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) {
  case 1:
    XtUnmanageChild( clog->filesel_form);
  }
}

static void clog_filesel_cancel_cb( Widget w, CLog *clog)
{
  XtUnmanageChild( clog->filesel_form);
}



