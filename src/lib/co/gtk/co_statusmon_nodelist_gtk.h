/* 
 * Proview   $Id: co_statusmon_nodelist_gtk.h,v 1.3 2007-05-25 13:39:28 claes Exp $
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

#ifndef co_statusmon_nodelist_gtk_h
#define co_statusmon_nodelist_gtk_h

/* co_statusmon_nodelist_gtk.h -- Status Monitor */

#ifndef co_statusmon_nodelist_h
# include "co_statusmon_nodelist.h"
#endif

#ifndef co_wow_gtk_h
# include "co_wow_gtk.h"
#endif

class NodelistGtk : public Nodelist {
  public:
    NodelistGtk( void *nodelist_parent_ctx,
		 GtkWidget *nodelist_parent_wid,
		 char *nodelist_name, int msgw_pop,
		 pwr_tStatus *status);
    ~NodelistGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*parent_wid_nodelist;
    GtkWidget		*toplevel;
    GtkWidget		*form_nodelist;
    GtkWidget		*nodelistnav_widget;
    GdkCursor  		*clock_cursor;
    GtkWidget		*india_widget;
    GtkWidget		*india_label;
    GtkWidget		*india_label2;
    GtkWidget		*india_text;
    GtkWidget		*india_text2;
    CoWowFocusTimerGtk  focustimer;

    void pop();
    void set_clock_cursor();
    void reset_cursor();
    void free_cursor();
    void create_input_dialog();
    void open_input_dialog( char *text, char *text2, char *title,
			    char *init_text,
			    void (*ok_cb)( Nodelist *, char *, char *));

    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void activate_exit( GtkWidget*w, gpointer data);
    static void activate_add_node( GtkWidget*w, gpointer data);
    static void activate_remove_node( GtkWidget*w, gpointer data);
    static void activate_open_xtt( GtkWidget*w, gpointer data);
    static void activate_open_opplace( GtkWidget*w, gpointer data);
    static void activate_open_rtmon( GtkWidget*w, gpointer data);
    static void activate_save( GtkWidget*w, gpointer data);
    static void activate_show_events( GtkWidget *w, gpointer data);
    static void activate_pop_events( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget*w, gpointer data);
    static void activate_zoom_out( GtkWidget*w, gpointer data);
    static void activate_zoom_reset( GtkWidget*w, gpointer data);
    static void activate_help( GtkWidget*w, gpointer data);
    static void activate_india_ok( GtkWidget *w, gpointer data);
    static void activate_india_cancel( GtkWidget *w, gpointer data);
    
};

#endif




