/* 
 * Proview   $Id: co_statusmon_nodelist_gtk.h,v 1.7 2008-10-31 12:51:30 claes Exp $
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

#ifndef cow_statusmon_nodelist_gtk_h
#define cow_statusmon_nodelist_gtk_h

/* cow_statusmon_nodelist_gtk.h -- Status Monitor */

#ifndef cow_statusmon_nodelist_h
# include "cow_statusmon_nodelist.h"
#endif

#ifndef cow_wow_gtk_h
# include "cow_wow_gtk.h"
#endif

class NodelistGtk : public Nodelist {
  public:
    NodelistGtk( void *nodelist_parent_ctx,
		 GtkWidget *nodelist_parent_wid,
		 const char *nodelist_name, int nodelist_mode, 
		 int nodelist_view_node_descr, int msgw_pop,
		 pwr_tStatus *status);
    ~NodelistGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*parent_wid_nodelist;
    GtkWidget		*toplevel;
    GtkWidget		*form_nodelist;
    GtkWidget		*nodelistnav_widget;
    GdkCursor  		*clock_cursor;
    GtkWidget		*add_india_widget;
    GtkWidget		*add_india_label;
    GtkWidget		*add_india_label2;
    GtkWidget		*add_india_label3;
    GtkWidget		*add_india_text;
    GtkWidget		*add_india_text2;
    GtkWidget		*add_india_text3;
    GtkWidget		*mod_india_widget;
    GtkWidget		*mod_india_label;
    GtkWidget		*mod_india_label2;
    GtkWidget		*mod_india_label3;
    GtkWidget		*mod_india_text;
    GtkWidget		*mod_india_text2;
    GtkWidget		*mod_india_text3;
    CoWowFocusTimerGtk  focustimer;

    void pop();
    void set_clock_cursor();
    void reset_cursor();
    void free_cursor();
    void create_add_input_dialog();
    void create_mod_input_dialog();
    void open_add_input_dialog( const char *text, const char *text2, const char *text3, 
				const char *title, const char *init_text,
				void (*ok_cb)( Nodelist *, char *, char *, char *));
    void open_mod_input_dialog( const char *text, const char *text2, const char *text3,
				const char *title, const char *init_text, 
				const char *init_text2, const char *init_text3,
				void (*ok_cb)( Nodelist *, char *, char *, char *));

    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void activate_exit( GtkWidget*w, gpointer data);
    static void activate_add_node( GtkWidget*w, gpointer data);
    static void activate_modify_node( GtkWidget*w, gpointer data);
    static void activate_remove_node( GtkWidget*w, gpointer data);
    static void activate_open_xtt( GtkWidget*w, gpointer data);
    static void activate_open_opplace( GtkWidget*w, gpointer data);
    static void activate_open_rtmon( GtkWidget*w, gpointer data);
    static void activate_save( GtkWidget*w, gpointer data);
    static void activate_show_events( GtkWidget *w, gpointer data);
    static void activate_pop_events( GtkWidget *w, gpointer data);
    static void activate_gui_motif( GtkWidget *w, gpointer data);
    static void activate_gui_gtk( GtkWidget *w, gpointer data);
    static void activate_mode_systemstatus( GtkWidget *w, gpointer data);
    static void activate_mode_userstatus1( GtkWidget *w, gpointer data);
    static void activate_mode_userstatus2( GtkWidget *w, gpointer data);
    static void activate_mode_userstatus3( GtkWidget *w, gpointer data);
    static void activate_mode_userstatus4( GtkWidget *w, gpointer data);
    static void activate_mode_userstatus5( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget*w, gpointer data);
    static void activate_zoom_out( GtkWidget*w, gpointer data);
    static void activate_zoom_reset( GtkWidget*w, gpointer data);
    static void activate_help( GtkWidget*w, gpointer data);
    static void activate_add_india_ok( GtkWidget *w, gpointer data);
    static void activate_add_india_cancel( GtkWidget *w, gpointer data);
    static void activate_mod_india_ok( GtkWidget *w, gpointer data);
    static void activate_mod_india_cancel( GtkWidget *w, gpointer data);
    
};

#endif




