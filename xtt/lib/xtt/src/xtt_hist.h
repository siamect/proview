#ifndef xtt_hist_h
#define xtt_hist_h

/* xtt_hist.h -- Historical event window in xtt

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined OS_LINUX

#if defined __cplusplus
extern "C" {
#include "rt_elog.h"
}
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef xtt_evlist
# include "xtt_evlist.h"
#endif
#define ERROR_TIME_CONVERT  -99
class Hist {
  public:
    Hist(
	void *hist_parent_ctx,
	Widget	hist_parent_wid,
	char *hist_name, pwr_tObjid objid,
	pwr_tStatus *status);
    ~Hist();

    void 		*parent_ctx;
    Widget		parent_wid;
    Widget		parent_wid_hist;

    char 		name[80];
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
            
    pwr_tObjid		user;
    int			hist_display_ack;
    int			hist_display_return;
    void 		(*close_cb)( void *);
    void 		(*start_trace_cb)( void *, pwr_tObjid, char *);
    void 		(*display_in_xnav_cb)( void *, pwr_sAttrRef *);
    void 		(*update_info_cb)( void *);
    void 		(*help_cb)( void *, char *);
    void 		(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
					  unsigned long, char *, Widget * );
    EvList		*hist;

    int			hist_size;
    char 		*minTime_str;
    char 		*maxTime_str;
    char 		*eventName_str;
    char 		*eventText_str;
    
    bool		eventPrio_A;
    bool		eventPrio_B;
    bool		eventPrio_C;
    bool		eventPrio_D;
    
    bool		eventType_Ack;
    bool		eventType_Alarm;
    bool		eventType_Info;
    bool		eventType_Return;
    bool		eventType_Cancel;
    bool		eventType_Block;
    bool		eventType_Unblock;
    bool		eventType_Reblock;
    bool		eventType_CancelBlock;
    
    void		get_hist_list();
    pwr_tStatus 	hist_add_ack_mess( mh_sAck *MsgP);
    pwr_tStatus 	hist_add_return_mess( mh_sReturn *MsgP);
    pwr_tStatus 	hist_add_alarm_mess( mh_sMessage *MsgP);
    pwr_tStatus 	hist_add_info_mess( mh_sMessage *MsgP);
    pwr_tStatus 	hist_clear_histlist();
    int			check_conditions(sEvent *evp);
    int			compareStr(char *, char *);
    void		printSearchStr_en_us();
    void		printSearchStr_sv_se();
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
