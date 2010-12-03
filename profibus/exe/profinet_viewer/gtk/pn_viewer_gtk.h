/** 
 * Proview   $Id$
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
 **/

#ifndef pn_viewer_gtk_h
#define pn_viewer_gtk_h

/* pn_viewer_gtk.h -- Profinet viewer */

#ifndef pn_viewer_h
# include "../src/pn_viewer.h"
#endif

#ifndef cow_wow_gtk_h
# include "cow_wow_gtk.h"
#endif

#include <gtk/gtk.h>

class PnViewerGtk : public PnViewer {
  public:
    PnViewerGtk(
	void *v_parent_ctx,
	GtkWidget *v_parent_wid,
	const char *v_name,
	const char *v_device,
	pwr_tStatus *status);
    ~PnViewerGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*toplevel;
    GtkWidget		*form;
    GtkWidget		*nav_widget;
    GtkWidget		*msg_label;
    GtkWidget		*cmd_prompt;
    GtkWidget		*cmd_input;
    CoWowFocusTimerGtk  focustimer;
    CoWowFocusTimerGtk	maptimer;
    CoWowRecall *value_recall;
    CoWowEntryGtk *cmd_entry;

    void message( char severity, const char *msg);
    void set_prompt( const char *prompt);
    void open_change_value();

    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void valchanged_cmd_input( GtkWidget *w, gpointer data);
    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_update( GtkWidget *w, gpointer data);
    static void activate_setdevice( GtkWidget *w, gpointer data);
    static void activate_changevalue( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget *w, gpointer data);
    static void activate_zoom_out( GtkWidget *w, gpointer data);
    static void activate_zoom_reset( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);

};

#endif









