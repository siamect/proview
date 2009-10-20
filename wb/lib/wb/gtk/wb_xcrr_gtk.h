/* 
 * Proview   $Id: xtt_xcrr_gtk.h,v 1.1 2007-01-04 08:29:32 claes Exp $
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

#ifndef wb_xcrr_gtk_h
#define wb_xcrr_gtk_h

/* wb_xcrr_gtk.h -- Object crossreferences */

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

#ifndef wb_xcrr_h
# include "wb_xcrr.h"
#endif

#ifndef co_wow_gtk_h
# include "co_wow_gtk.h"
#endif

class WCrrGtk : public WCrr {
  public:
    WCrrGtk( 
	GtkWidget      	*xa_parent_wid, 
	void 		*xa_parent_ctx, 
	ldh_tSesContext xa_ldhses, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	int             *xa_sts);
    ~WCrrGtk();
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*xcrrnav_form;
    CoWowFocusTimerGtk focustimer;

    void pop();

    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_openplc( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);

};

#endif







