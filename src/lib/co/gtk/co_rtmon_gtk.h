/* 
 * Proview   $Id: co_rtmon_gtk.h,v 1.3 2008-10-31 12:51:30 claes Exp $
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

#ifndef co_rtmon_gtk_h
#define co_rtmon_gtk_h

/* co_rtmon_gtk.h -- Status Monitor */

#ifndef co_rtmon_h
# include "co_rtmon.h"
#endif

class RtMonGtk : public RtMon {
  public:
    RtMonGtk( void *rtmon_parent_ctx,
	      GtkWidget *rtmon_parent_wid,
	      const char *rtmon_name,
	      const char *rtmon_display,
	      pwr_tStatus *status);
    ~RtMonGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*parent_wid_nodelist;
    GtkWidget		*toplevel;
    GtkWidget		*form_nodelist;
    GtkWidget		*nodelistnav_widget;
    GtkWidget 		*bbox_label;
    GtkWidget 		*bbox_label_eb;
    GtkWidget 		*bbox_start;
    GtkWidget 		*bbox_restart;
    GtkWidget 		*bbox_stop;
    GtkWidget 		*lbox_reset;
    GtkWidget 		*bbox_image;
    GtkWidget 		*bbox_image_gray;
    GtkWidget 		*tools_xtt;
    GtkWidget 		*tools_op;
    GtkWidget 		*file_xtt;
    GtkWidget 		*file_op;
    GdkCursor  		*clock_cursor;
    gint		timerid;
    pwr_tStatus		old_status;

    void pop();
    void set_clock_cursor();
    void reset_cursor();
    void free_cursor();
    void create_input_dialog();
    void open_input_dialog( char *text, char *title,
			    char *init_text,
			    void (*ok_cb)( RtMon *, char *));

    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void activate_exit( GtkWidget*w, gpointer data);
    static void activate_start( GtkWidget *w, gpointer data);
    static void activate_restart( GtkWidget *w, gpointer data);
    static void activate_stop( GtkWidget *w, gpointer data);
    static void activate_reset( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget*w, gpointer data);
    static void activate_zoom_out( GtkWidget*w, gpointer data);
    static void activate_zoom_reset( GtkWidget*w, gpointer data);
    static void activate_xtt( GtkWidget*w, gpointer data);
    static void activate_op( GtkWidget*w, gpointer data);
    static void activate_help( GtkWidget*w, gpointer data);
    
};

#endif




