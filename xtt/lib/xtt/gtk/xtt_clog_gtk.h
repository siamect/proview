/* 
 * Proview   $Id: xtt_clog_gtk.h,v 1.1 2007-01-04 08:29:32 claes Exp $
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

#ifndef xtt_clog_gtk_h
#define xtt_clog_gtk_h

/* xtt_clog_gtk.h -- Alarm and event windows in xtt */

#ifndef xtt_clog_h
# include "xtt_clog.h"
#endif

class CLogGtk : public CLog {
  public:
    CLogGtk( void *clog_parent_ctx,
	     GtkWidget *clog_parent_wid,
	     char *clog_name,
	     pwr_tStatus *status);
    ~CLogGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*parent_wid_clog;
    GtkWidget		*toplevel;
    GtkWidget		*form_clog;
    GtkWidget		*clognav_widget;
    GtkWidget		*filter_form;
    GtkWidget		*show_success_w;
    GtkWidget		*show_info_w;
    GtkWidget		*show_warning_w;
    GtkWidget		*show_error_w;
    GtkWidget		*show_fatal_w;
    GtkWidget		*show_text_w;
    GtkWidget		*filter_string_w;
    GtkWidget		*filesel_form;
    GtkWidget		*filesel_list_w;
    GdkCursor  		*clock_cursor;

    void pop();
    void set_clock_cursor();
    void reset_cursor();
    void free_cursor();
    void create_filter_dialog();

    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void activate_exit( GtkWidget*w, gpointer data);
    static void activate_select_file( GtkWidget*w, gpointer data);
    static void activate_next_file( GtkWidget*w, gpointer data);
    static void activate_prev_file( GtkWidget*w, gpointer data);
    static void activate_update( GtkWidget*w, gpointer data);
    static void activate_zoom_in( GtkWidget*w, gpointer data);
    static void activate_zoom_out( GtkWidget*w, gpointer data);
    static void activate_zoom_reset( GtkWidget*w, gpointer data);
    static void activate_filter( GtkWidget*w, gpointer data);
    static void activate_help( GtkWidget*w, gpointer data);
    static void activate_helpmsg( GtkWidget*w, gpointer data);
    static void filter_ok_cb( GtkWidget*w, gpointer data);
    static void filter_apply_cb( GtkWidget*w, gpointer data);
    static void filter_cancel_cb( GtkWidget*w, gpointer data);
    static void file_selected_cb( void *ctx, char *text);
    
};

#endif




