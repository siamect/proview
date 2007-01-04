/** 
 * Proview   $Id: co_xhelp_gtk.h,v 1.1 2007-01-04 07:51:41 claes Exp $
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

#include <gtk/gtk.h>

#ifndef co_xhelp_gtk_h
#define co_xhelp_gtk_h

#ifndef co_xhelp_h
# include "co_xhelp.h"
#endif

#ifndef co_wow_gtk_h
# include "co_wow_gtk.h"
#endif

class CoXHelpGtk : public CoXHelp {
  public:
    CoXHelpGtk( 
	GtkWidget 	*xa_parent_wid, 
	void 		*xa_parent_ctx,
	xhelp_eUtility  utility,
	int             *xa_sts);
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*xhelpnav_form;
    GtkWidget	*india_widget;
    GtkWidget	*india_label;
    GtkWidget	*india_text;
    CoWowFocusTimerGtk focustimer;

    void set_dimension( int width, int height);    
    void pop();
    void open_input_dialog( char *text, char *title,
			    char *init_text,
			    void (*ok_cb)( CoXHelp *, char *));
    static void activate_close( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget *w, gpointer data);
    static void activate_zoom_out( GtkWidget *w, gpointer data);
    static void activate_zoom_reset( GtkWidget *w, gpointer data);
    static void activate_search( GtkWidget *w, gpointer data);
    static void activate_searchnext( GtkWidget *w, gpointer data);
    static void activate_searchprevious( GtkWidget *w, gpointer data);
    static void activate_india_ok( GtkWidget *w, gpointer data);
    static void activate_india_cancel( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static void enable_set_focus( CoXHelpGtk *xhelp);
    static void disable_set_focus( CoXHelpGtk *xhelp, int time);
    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);

    static GtkWidget *get_widget() { 
      if ( default_xhelp)
	return ((CoXHelpGtk *)default_xhelp)->parent_wid;
      else
	return 0;
    }

    ~CoXHelpGtk();
};


#endif







