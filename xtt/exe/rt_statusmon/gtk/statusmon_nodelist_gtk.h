/* 
 * Proview   $Id: statusmon_nodelist_gtk.h,v 1.1 2007-05-11 15:04:14 claes Exp $
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

#ifndef statusmon_nodelist_gtk_h
#define statusmon_nodelist_gtk_h

/* statusmon_nodelist_gtk.h -- Alarm and event windows in statusmon */

#ifndef statusmon_nodelist_h
# include "statusmon_nodelist.h"
#endif

class NodelistGtk : public Nodelist {
  public:
    NodelistGtk( void *nodelist_parent_ctx,
	     GtkWidget *nodelist_parent_wid,
	     char *nodelist_name,
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
    GtkWidget		*india_text;

    void pop();
    void set_clock_cursor();
    void reset_cursor();
    void free_cursor();
    void create_input_dialog();
    void open_input_dialog( char *text, char *title,
			    char *init_text,
			    void (*ok_cb)( Nodelist *, char *));

    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void activate_exit( GtkWidget*w, gpointer data);
    static void activate_add_node( GtkWidget*w, gpointer data);
    static void activate_remove_node( GtkWidget*w, gpointer data);
    static void activate_save( GtkWidget*w, gpointer data);
    static void activate_show_events( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget*w, gpointer data);
    static void activate_zoom_out( GtkWidget*w, gpointer data);
    static void activate_zoom_reset( GtkWidget*w, gpointer data);
    static void activate_help( GtkWidget*w, gpointer data);
    static void activate_india_ok( GtkWidget *w, gpointer data);
    static void activate_india_cancel( GtkWidget *w, gpointer data);
    
};

#endif




