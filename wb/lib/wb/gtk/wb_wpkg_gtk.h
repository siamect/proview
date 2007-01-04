/* 
 * Proview   $Id: wb_wpkg_gtk.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_wpkg_gtk_h
#define wb_wpkg_gtk_h

/* wb_wpkg_gtk.h -- Package window */

#ifndef wb_wpkg_h
# include "wb_wpkg.h"
#endif

#ifndef co_wow_gtk_h
# include "co_wow_gtk.h"
#endif

class WPkgGtk : public WPkg {
  public:
    WPkgGtk( 
	GtkWidget      	*wa_parent_wid, 
	void 		*wa_parent_ctx);
    ~WPkgGtk();
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*msg_label;
    GtkWidget	*wpkgnav_form;
    GdkCursor  	*clock_cursor;
    CoWowFocusTimerGtk focustimer;

    void message( char severity, char *message);
    void pop();
    void set_clock_cursor();
    void reset_cursor();
    void flush();

    static void activate_distribute( GtkWidget *w, gpointer data);
    static void activate_createpkg( GtkWidget *w, gpointer data);
    static void activate_deletepkg( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget *w, gpointer data);
    static void activate_zoom_out( GtkWidget *w, gpointer data);
    static void activate_zoom_reset( GtkWidget *w, gpointer data);
    static void activate_dmode_filediff( GtkWidget *w, gpointer data);
    static void activate_dmode_filetime( GtkWidget *w, gpointer data);
    static void activate_dmode_filepath( GtkWidget *w, gpointer data);
    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);

};


#endif







