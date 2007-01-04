/* 
 * Proview   $Id: xtt_clog_motif.cpp,v 1.1 2007-01-04 08:30:03 claes Exp $
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

/* xtt_clog_motif.cpp -- Console log in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"

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

#include "flow_x.h"
#include "co_lng.h"
#include "co_wow_motif.h"
#include "xtt_clog.h"
#include "rt_xnav_msg.h"
#include "xtt_clog_motif.h"
#include "xtt_clognav_motif.h"

CLogMotif::CLogMotif( void *clog_parent_ctx,
		      Widget	clog_parent_wid,
		      char *clog_name,
		      pwr_tStatus *status) :
  CLog( clog_parent_ctx, clog_name, status), parent_wid(clog_parent_wid),
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
    {"clog_inputfocus",      (XtActionProc) action_inputfocus}
  };

  static MrmRegisterArg	reglist[] = {
        { "clog_ctx", 0 },
	{"clog_activate_exit",(caddr_t)activate_exit },
	{"clog_activate_select_file",(caddr_t)activate_select_file },
	{"clog_activate_next_file",(caddr_t)activate_next_file },
	{"clog_activate_prev_file",(caddr_t)activate_prev_file },
	{"clog_activate_update",(caddr_t)activate_update },
	{"clog_activate_zoom_in",(caddr_t)activate_zoom_in },
	{"clog_activate_zoom_out",(caddr_t)activate_zoom_out },
	{"clog_activate_zoom_reset",(caddr_t)activate_zoom_reset },
	{"clog_activate_filter",(caddr_t)activate_filter },
	{"clog_activate_help",(caddr_t)activate_help },
	{"clog_activate_helpmsg",(caddr_t)activate_helpmsg },
	{"clog_create_form",(caddr_t)create_form },
	{"clog_filter_tog_cr",(caddr_t)filter_tog_cr },
	{"clog_filter_string_cr",(caddr_t)filter_string_cr },
	{"clog_filter_act_but_cb",(caddr_t)filter_act_but_cb },
	{"clog_filesel_ok_cb",(caddr_t)filesel_ok_cb },
	{"clog_filesel_cancel_cb",(caddr_t)filesel_cancel_cb },
	{"clog_filesel_list_cr",(caddr_t)filesel_list_cr }
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
  clognav = new CLogNavMotif( this, form_clog, &clognav_widget);

  XtPopup( parent_wid_clog, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid_clog, 
	(XtCallbackProc)activate_exit, this);

  wow = new CoWowMotif( toplevel);

  *status = 1;
}


//
//  Delete clog
//
CLogMotif::~CLogMotif()
{
  free_cursor();
  XtDestroyWidget( parent_wid_clog);
  delete clognav;
}

void CLogMotif::pop()
{
  flow_UnmapWidget( parent_wid_clog);
  flow_MapWidget( parent_wid_clog);
}

void CLogMotif::set_clock_cursor()
{
  if ( !clock_cursor)
    clock_cursor = XCreateFontCursor( flow_Display(toplevel), XC_watch);

  XDefineCursor( flow_Display(toplevel), flow_Window(toplevel), clock_cursor);
  XFlush( flow_Display(toplevel));
}

void CLogMotif::reset_cursor()
{
  XUndefineCursor( flow_Display(toplevel), flow_Window(toplevel));
}

void CLogMotif::free_cursor()
{
  if (clock_cursor)
    XFreeCursor( flow_Display(toplevel), clock_cursor);
}

void CLogMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  CLog *clog;

  XtSetArg    (args[0], XmNuserData, &clog);
  XtGetValues (w, args, 1);

  if ( clog && clog->clog_displayed)
    clog->clognav->set_input_focus();
}

void CLogMotif::activate_exit( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  if ( clog->close_cb)
    (clog->close_cb)( clog->parent_ctx);
  else
    delete clog;
}

void CLogMotif::activate_zoom_in( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->clognav->zoom( 1.2);
}

void CLogMotif::activate_zoom_out( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->clognav->zoom( 5.0/6);
}

void CLogMotif::activate_zoom_reset( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->clognav->unzoom();
}

void CLogMotif::activate_filter( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  bool success, info, warning, error, fatal, text;

  clog->clognav->get_filter( &success, &info, &warning, &error, &fatal, &text);
  XmToggleButtonSetState( ((CLogMotif *)clog)->show_success_w, success, 0);
  XmToggleButtonSetState( ((CLogMotif *)clog)->show_info_w, info, 0);
  XmToggleButtonSetState( ((CLogMotif *)clog)->show_warning_w, warning, 0);
  XmToggleButtonSetState( ((CLogMotif *)clog)->show_error_w, error, 0);
  XmToggleButtonSetState( ((CLogMotif *)clog)->show_fatal_w, fatal, 0);
  XmToggleButtonSetState( ((CLogMotif *)clog)->show_text_w, text, 0);

  XtManageChild( ((CLogMotif *)clog)->filter_form);
}

void CLogMotif::activate_select_file( Widget w, CLog *clog, XmAnyCallbackStruct *data)
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
      XmListAddItemUnselected( ((CLogMotif *)clog)->filesel_list_w, cstr, 0);
      XmStringFree(cstr);
    }
    clog->filesel_loaded = true;
  }
  XtManageChild( ((CLogMotif *)clog)->filesel_form);
}

void CLogMotif::activate_next_file( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->activate_next_file();
}

void CLogMotif::activate_prev_file( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->activate_prev_file();
}

void CLogMotif::activate_update( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->set_clock_cursor();
  clog->clognav->update();
  clog->reset_cursor();
}

void CLogMotif::activate_help( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  clog->activate_help();
}

void CLogMotif::activate_helpmsg( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
}

void CLogMotif::create_form( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  ((CLogMotif *)clog)->form_clog = w;
}

void CLogMotif::filter_string_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data) 
{
  ((CLogMotif *)clog)->filter_string_w = w;
}
void CLogMotif::filter_tog_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data) 
{

  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) {
  case 1:
    // Success
    ((CLogMotif *)clog)->show_success_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 2:
    // Info
    ((CLogMotif *)clog)->show_info_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 3:
    // Warning
    ((CLogMotif *)clog)->show_warning_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 4:
    // Error
    ((CLogMotif *)clog)->show_error_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 5:
    // Fatal
    ((CLogMotif *)clog)->show_fatal_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  case 6:
    // Text
    ((CLogMotif *)clog)->show_text_w = w;
    XmToggleButtonSetState(w, 1, 0);
    break;
  default:
    break;
  }
}

void CLogMotif::filter_act_but_cb( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  int key;
  char *str;

  bool success = XmToggleButtonGetState( ((CLogMotif *)clog)->show_success_w);
  bool info = XmToggleButtonGetState( ((CLogMotif *)clog)->show_info_w);
  bool warning = XmToggleButtonGetState( ((CLogMotif *)clog)->show_warning_w);
  bool error = XmToggleButtonGetState( ((CLogMotif *)clog)->show_error_w);
  bool fatal = XmToggleButtonGetState( ((CLogMotif *)clog)->show_fatal_w);
  bool text = XmToggleButtonGetState( ((CLogMotif *)clog)->show_text_w);
  str = XmTextGetString( ((CLogMotif *)clog)->filter_string_w);

  XtVaGetValues(w, XmNuserData, &key, NULL);
  switch (key) {
  case 1 :
    // Ok
    XtUnmanageChild( ((CLogMotif *)clog)->filter_form);
  case 2 :
    // Apply
    clog->set_clock_cursor();
    clog->clognav->set_filter( success, info, warning, error, fatal, text, str);
    clog->reset_cursor();
    break;

  case 3 :
    // Cancel
    XtUnmanageChild( ((CLogMotif *)clog)->filter_form);
    break;
  }
}

void CLogMotif::filesel_list_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  ((CLogMotif *)clog)->filesel_list_w = w;
}

void CLogMotif::filesel_ok_cb( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  int *pos_list, pos_cnt;
  int key;

  if (XmListGetSelectedPos( ((CLogMotif *)clog)->filesel_list_w, &pos_list, &pos_cnt)) {
    clog->set_clock_cursor();
    clog->clognav->read( pos_list, pos_cnt);
    clog->reset_cursor();
  }

  XtVaGetValues (w, XmNuserData, &key, NULL);
  switch (key) {
  case 1:
    XtUnmanageChild( ((CLogMotif *)clog)->filesel_form);
  }
}

void CLogMotif::filesel_cancel_cb( Widget w, CLog *clog, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((CLogMotif *)clog)->filesel_form);
}



