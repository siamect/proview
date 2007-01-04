/** 
 * Proview   $Id: co_msgwindow_gtk.h,v 1.1 2007-01-04 07:51:41 claes Exp $
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

#ifndef co_msgwindow_gtk_h
#define co_msgwindow_gtk_h

/* co_msgwindow_gtk.h -- Message window */

#ifndef co_msgwindow_h
# include "co_msgwindow.h"
#endif

#ifndef co_wow_gtk_h
# include "co_wow_gtk.h"
#endif

#include <gtk/gtk.h>

class MsgWindowGtk : public MsgWindow {
  public:
    MsgWindowGtk(
	void *msg_parent_ctx,
	GtkWidget *msg_parent_wid,
	char *msg_name,
	pwr_tStatus *status);
    ~MsgWindowGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*toplevel;
    GtkWidget		*form;
    GtkWidget		*nav_widget;
    CoWowFocusTimerGtk  focustimer;
    CoWowFocusTimerGtk	maptimer;

    void	map();
    void	unmap();

    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_clear( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget *w, gpointer data);
    static void activate_zoom_out( GtkWidget *w, gpointer data);
    static void activate_zoom_reset( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static void activate_help_message( GtkWidget *w, gpointer data);

};

#endif









