/* 
 * Proview   $Id: xtt_hist_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_hist_motif_h
#define xtt_hist_motif_h

/* xtt_hist_motif.h -- Historical event window in xtt */

#if defined OS_LINUX

#ifndef xtt_hist_h
# include "xtt_hist.h"
#endif

#ifndef cow_wow_motif_h
# include "cow_wow_motif.h"
#endif

class HistMotif : public Hist {
 public:
  HistMotif( void *hist_parent_ctx,
	     Widget	hist_parent_wid,
	     char *hist_name, pwr_tObjid objid,
	     pwr_tStatus *status);
  ~HistMotif();

  Widget		parent_wid;
  Widget		parent_wid_hist;
  Widget		toplevel_hist;
  Widget		toplevel_search;
  Widget		form_hist;
  Widget		hist_widget;  
  Widget		start_time_help_lbl_w;
  Widget		start_time_entry_w;
  Widget		stop_time_entry_w;    
  Widget		event_text_entry_w;
  Widget		event_name_entry_w;
  Widget		alarm_toggle_w;
  Widget		info_toggle_w;
  Widget		ack_toggle_w;
  Widget		ret_toggle_w;
  Widget		prioA_toggle_w;
  Widget		prioB_toggle_w;
  Widget		prioC_toggle_w;
  Widget		prioD_toggle_w;
  Widget		nrofevents_string_lbl_w;
  Widget		search_string_lbl_w;
  Widget		search_string2_lbl_w;
  Widget		search_string3_lbl_w;
  Widget		search_string4_lbl_w;            
  CoWowFocusTimerMotif 	focustimer;

  void set_num_of_events( int nrOfEvents);
  void set_search_string( const char *s1, const char *s2, 
			  const char *s3, const char *s4);
  void SetListTime( pwr_tTime StartTime, pwr_tTime StopTime, 
		    int Sensitive);

  static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
  static void activate_exit( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void activate_print( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void activate_zoom_in( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void activate_zoom_out( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void activate_zoom_reset( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void activate_open_plc( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void activate_display_in_xnav( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void activate_disp_hundredth( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data);
  static void activate_hide_object( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data);
  static void activate_hide_text( Widget w, Hist *histOP, XmToggleButtonCallbackStruct *data);
  static void activate_help( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void activate_helpevent( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void create_form( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void ok_btn( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  //callbackfunctions from the searchdialog
  static void cancel_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void start_time_help_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void start_time_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void start_time_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void stop_time_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);    static void stop_time_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void today_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void yesterday_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void thisw_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void lastw_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void thism_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void lastm_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void all_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void time_cb( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  
  static void alarm_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void info_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void ack_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void ret_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  
  static void prioA_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void prioB_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void prioC_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void prioD_toggle_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  
  static void event_text_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void event_text_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  
  static void event_name_entry_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void event_name_entry_lf( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void nrofevents_string_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void search_string_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void search_string2_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void search_string3_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  static void search_string4_label_cr( Widget w, Hist *histOP, XmAnyCallbackStruct *data);
  
};

#else
// Dummy for other platforms then OS_LINUX
class Hist {
  public:
    Hist(
	void *hist_parent_ctx,
	Widget	hist_parent_wid,
	char *hist_name, pwr_tObjid objid,
	pwr_tStatus *status) : parent_ctx(hist_parent_ctx) {}
    void 		*parent_ctx;
    void 		(*close_cb)( void *);
    void 		(*start_trace_cb)( void *, pwr_tObjid, char *);
    void 		(*display_in_xnav_cb)( void *, pwr_tObjid);
    void 		(*update_info_cb)( void *);
    void 		(*help_cb)( void *, char *);
    void 		(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
					  unsigned long, char *, Widget * );
};

#endif

#endif
