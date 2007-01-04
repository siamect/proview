/* 
 * Proview   $Id: xtt_clog_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_clog_motif_h
#define xtt_clog_motif_h

/* xtt_clog_motif.h -- Alarm and event windows in xtt */

#ifndef xtt_clog_h
# include "xtt_clog.h"
#endif

class CLogMotif : public CLog {
  public:
    CLogMotif( void *clog_parent_ctx,
	       Widget	clog_parent_wid,
	       char *clog_name,
	       pwr_tStatus *status);
    ~CLogMotif();

    Widget		parent_wid;
    Widget		parent_wid_clog;
    Widget		toplevel;
    Widget		form_clog;
    Widget		clognav_widget;
    Widget		filter_form;
    Widget		show_success_w;
    Widget		show_info_w;
    Widget		show_warning_w;
    Widget		show_error_w;
    Widget		show_fatal_w;
    Widget		show_text_w;
    Widget		filter_string_w;
    Widget		filesel_form;
    Widget		filesel_list_w;
    int			clock_cursor;

    void pop();
    void set_clock_cursor();
    void reset_cursor();
    void free_cursor();

    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
    static void activate_exit( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_select_file( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_next_file( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_prev_file( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_update( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_zoom_in( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_zoom_out( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_zoom_reset( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_filter( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void activate_helpmsg( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void create_form( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void filter_tog_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void filter_string_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void filter_act_but_cb( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void filesel_list_cr( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void filesel_ok_cb( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    static void filesel_cancel_cb( Widget w, CLog *clog, XmAnyCallbackStruct *data);
    
};

#endif




