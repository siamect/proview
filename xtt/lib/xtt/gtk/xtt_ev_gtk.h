/* 
 * Proview   $Id: xtt_ev_gtk.h,v 1.1 2007-01-04 08:29:32 claes Exp $
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

#ifndef xtt_ev_gtk_h
#define xtt_ev_gtk_h

/* xtt_ev_gtk.h -- Alarm and event windows in xtt */

#ifndef xtt_ev_h
# include "xtt_ev.h"
#endif

class EvGtk : public Ev {
  public:
    EvGtk(
	void *ev_parent_ctx,
	GtkWidget *ev_parent_wid,
	char *eve_name,
	char *ala_name,
	char *blk_name,
	pwr_tObjid ev_user,
	int display_ala,
	int display_eve,
	int display_blk,
	int display_return,
	int display_ack,
	int ev_beep,
	pwr_tMask ev_pop_mask,
	pwr_tStatus *status);
    ~EvGtk();

    GtkWidget		*parent_wid;
    GtkWidget 		*parent_wid_eve;
    GtkWidget		*parent_wid_ala;
    GtkWidget		*parent_wid_blk;
    GtkWidget		*toplevel_ala;
    GtkWidget		*toplevel_eve;
    GtkWidget		*toplevel_blk;
    GtkWidget		*form_ala;
    GtkWidget		*form_eve;
    GtkWidget		*form_blk;
    GtkWidget		*eve_widget;
    GtkWidget		*ala_widget;
    GtkWidget		*blk_widget;

    void map_eve();
    void map_ala();
    void map_blk();
    void unmap_eve();
    void unmap_ala();
    void unmap_blk();

    static gboolean eve_action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static gboolean ala_action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static gboolean blk_action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void eve_activate_exit( GtkWidget *w, gpointer data);
    static void ala_activate_exit( GtkWidget *w, gpointer data);
    static void blk_activate_exit( GtkWidget *w, gpointer data);
    static void eve_activate_print( GtkWidget *w, gpointer data);
    static void ala_activate_print( GtkWidget *w, gpointer data);
    static void blk_activate_print( GtkWidget *w, gpointer data);
    static void eve_activate_ack_last( GtkWidget *w, gpointer data);
    static void ala_activate_ack_last( GtkWidget *w, gpointer data);
    static void eve_activate_zoom_in( GtkWidget *w, gpointer data);
    static void ala_activate_zoom_in( GtkWidget *w, gpointer data);
    static void blk_activate_zoom_in( GtkWidget *w, gpointer data);
    static void eve_activate_zoom_out( GtkWidget *w, gpointer data);
    static void ala_activate_zoom_out( GtkWidget *w, gpointer data);
    static void blk_activate_zoom_out( GtkWidget *w, gpointer data);
    static void eve_activate_zoom_reset( GtkWidget *w, gpointer data);
    static void ala_activate_zoom_reset( GtkWidget *w, gpointer data);
    static void blk_activate_zoom_reset( GtkWidget *w, gpointer data);
    static void blk_activate_block_remove( GtkWidget *w, gpointer data);
    static void eve_activate_open_plc( GtkWidget *w, gpointer data);
    static void ala_activate_open_plc( GtkWidget *w, gpointer data);
    static void blk_activate_open_plc( GtkWidget *w, gpointer data);
    static void eve_activate_display_in_xnav( GtkWidget *w, gpointer data);
    static void ala_activate_display_in_xnav( GtkWidget *w, gpointer data);
    static void blk_activate_display_in_xnav( GtkWidget *w, gpointer data);
    static void eve_activate_disp_hundredth( GtkWidget *w, gpointer data);
    static void ala_activate_disp_hundredth( GtkWidget *w, gpointer data);
    static void eve_activate_hide_object( GtkWidget *w, gpointer data);
    static void ala_activate_hide_object( GtkWidget *w, gpointer data);
    static void eve_activate_hide_text( GtkWidget *w, gpointer data);
    static void ala_activate_hide_text( GtkWidget *w, gpointer data);
    static void eve_activate_help( GtkWidget *w, gpointer data);
    static void ala_activate_help( GtkWidget *w, gpointer data);
    static void blk_activate_help( GtkWidget *w, gpointer data);
    static void eve_activate_helpevent( GtkWidget *w, gpointer data);
    static void ala_activate_helpevent( GtkWidget *w, gpointer data);
};

#endif



