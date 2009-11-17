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

#ifndef cow_logw_gtk_h
#define cow_logw_gtk_h

/* cow_logw_gtk.h -- History log window */

#ifndef cow_logw_h
# include "cow_logw.h"
#endif

#ifndef cow_wow_gtk_h
# include "cow_wow_gtk.h"
#endif

#include <gtk/gtk.h>

class CoLogWGtk : public CoLogW {
  public:
    CoLogWGtk(
	void *msg_parent_ctx,
	GtkWidget *msg_parent_wid,
	const char *msg_name,
	int l_show_item,
	pwr_tStatus *status);
    ~CoLogWGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*toplevel;
    GtkWidget		*form;
    GtkWidget		*nav_widget;
    CoWowFocusTimerGtk  focustimer;
    CoWowFocusTimerGtk	maptimer;

    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget *w, gpointer data);
    static void activate_zoom_out( GtkWidget *w, gpointer data);
    static void activate_zoom_reset( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);

};

#endif









