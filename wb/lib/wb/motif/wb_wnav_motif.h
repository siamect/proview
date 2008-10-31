/* 
 * Proview   $Id: wb_wnav_motif.h,v 1.3 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_wnav_motif_h
#define wb_wnav_motif_h

#include "wb_wnav.h"

/* wtt_wnav_motif.h -- Simple navigator */

class CoLogin;

class WNavMotif : public WNav {
  public:
    WNavMotif(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	const char *xn_name,
	const char *xn_layout,
	Widget *w,
        ldh_tSesContext	xn_ldhses,
	wnav_sStartMenu *root_menu,
	wnav_eWindowType xn_type,
	pwr_tStatus *status);
    ~WNavMotif();

    Widget		parent_wid;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    XtIntervalId        deadlock_timerid;
    XtIntervalId        trace_timerid;
    Atom		graph_atom;
    Atom		objid_atom;
    Atom		attrref_atom;
    int		        avoid_deadlock;

    void pop();
    void set_inputfocus( int focus);
    void trace_start();
    void set_selection_owner();
    Ge *ge_new( char *graph_name);
    WGe *wge_new( char *name, char *filename, char *object_name,
		  int modal);
    void create_popup_menu( pwr_tAttrRef aref, int x, int y);
    int get_selection( char *str, int len);
    int open_foe( const char *name, pwr_tOid plcpgm, void **foectx, int map_window,
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
    CoLogin *login_new( const char *name, const char *groupname,
			void (* bc_success)( void *), void (* bc_cancel)( void *), pwr_tStatus *status);

    static void trace_scan( WNavMotif *wnav);
    static void sel_lose_cb( Widget w, Atom *selection);
    static Boolean sel_convert_cb( Widget w, Atom *selection, Atom *target,
				   Atom *type_return, XtPointer *value_return,
				   unsigned long *length_return, int *format_return);
};

#endif
