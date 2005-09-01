/* 
 * Proview   $Id: xtt_clog.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#ifndef xtt_clog_h
#define xtt_clog_h

/* xtt_clog.h -- Alarm and event windows in xtt */

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




