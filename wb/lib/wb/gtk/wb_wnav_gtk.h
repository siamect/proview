/* 
 * Proview   $Id: wb_wnav_gtk.h,v 1.2 2008-06-24 07:52:21 claes Exp $
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

#ifndef wb_wnav_gtk_h
#define wb_wnav_gtk_h

#include "wb_wnav.h"

/* wtt_wnav_gtk.h -- Simple navigator */

class CoLogin;

class WNavGtk : public WNav {
  public:
    WNavGtk(
	void *xn_parent_ctx,
	GtkWidget *xn_parent_wid,
	char *xn_name,
	char *xn_layout,
	GtkWidget **w,
        ldh_tSesContext	xn_ldhses,
	wnav_sStartMenu *root_menu,
	wnav_eWindowType xn_type,
	pwr_tStatus *status);
    ~WNavGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;
    GtkWidget		*selection_widget;
    GtkWidget		*message_dialog_entry;
    guint        	trace_timerid;
    GdkAtom		graph_atom;
    GdkAtom		objid_atom;
    GdkAtom		attrref_atom;
    gulong		sel_lose_id;

    void pop();
    void set_inputfocus( int focus);
    void trace_start();
    void set_selection_owner();
    Ge *ge_new( char *graph_name);
    WGe *wge_new( char *name, char *filename, char *object_name,
		  int modal);
    void create_popup_menu( pwr_tAttrRef aref, int x, int y);
    int get_selection( char *str, int len);
    int open_foe( char *name, pwr_tOid plcpgm, void **foectx, int map_window,
		  ldh_eAccess access, pwr_tOid oid);
    void wda_new( pwr_tOid oid, pwr_tCid cid, char *attribute,
		  int edit_mode, int advuser, int display_objectname);
    void message_dialog( char *title, char *text);
    int confirm_dialog( char *title, char *text, int display_cancel,
				 int *cancel);
    int continue_dialog( char *title, char *text);
    int prompt_dialog( char *title, char *text, char **value);
    void wge_subwindow_loop( WGe *wge);
    void wge_modal_loop( WGe *wge);
    bool has_window() {return parent_wid ? true : false;}
    wb_utl *utl_new();
    CoLogin *login_new( char *name, char *groupname,
			void (* bc_success)( void *), void (* bc_cancel)( void *), pwr_tStatus *status);

    static void trace_scan( WNavGtk *wnav);
    static void sel_lose_cb( GtkWidget *w, GdkEventSelection *event,
			     gpointer data);
    static void sel_convert_cb( GtkWidget  *w, GtkSelectionData *selection_data,
				guint info, guint time_stamp, gpointer data);
};

#endif
