#ifndef xtt_clog_h
#define xtt_clog_h

/* xtt_clog.h -- Alarm and event windows in xtt

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#if defined __cplusplus
}
#endif

#ifndef xtt_clognav
# include "xtt_clognav.h"
#endif

class CLog {
  public:
    CLog(
	void *clog_parent_ctx,
	Widget	clog_parent_wid,
	char *clog_name,
	pwr_tStatus *status);
    ~CLog();

    void 		*parent_ctx;
    Widget		parent_wid;
    Widget		parent_wid_clog;
    char 		name[80];
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
    CLogNav		*clognav;
    int			clog_displayed;
    void 		(*help_cb)( void *, char *);
    void 		(*close_cb)( void *);
    bool		filesel_loaded;
    int			clock_cursor;

    void	pop();
    void	set_clock_cursor();
    void	reset_cursor();
    void	free_cursor();
};

#endif




