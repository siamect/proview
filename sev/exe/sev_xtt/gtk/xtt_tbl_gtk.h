/* 
 * Proview   $Id: xtt_tbl_gtk.h,v 1.4 2008-10-31 12:51:30 claes Exp $
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

#ifndef xtt_tbl_gtk_h
#define xtt_tbl_gtk_h

#include "../src/xtt_tbl.h"
#include "xtt_tblnav_gtk.h"
#include "co_wow_gtk.h"

/* ge_tbl.h -- Ge tblibute editor */

class CoWowRecall;
class CoWowEntryGtk;

class XttTblGtk : public XttTbl {
  public:
    XttTblGtk(
      GtkWidget                 *a_parent_wid,
      void			*a_parent_ctx,
      sevcli_tCtx		a_sevcli,
      sevcli_sHistItem         	*itemlist,
      int			item_cnt);
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*msg_label;
    GtkWidget	*cmd_prompt;
    GtkWidget	*cmd_input;
    GtkWidget	*pane;
    CoWowRecall *cmd_recall;
    CoWowEntryGtk *cmd_entry;

    void message( char severity, char *message);
    XttSevHist *sevhist_new( pwr_tOid oid, char *aname);
    CoLogin *login_new( const char     	*wl_name,
			const char     	*wl_groupname,
			void		(* wl_bc_success)( void *),
			void		(* wl_bc_cancel)( void *),
			pwr_tStatus  	*status);
    void set_prompt( const char *prompt);

    static void activate_command( GtkWidget *w, gpointer data);
    static void valchanged_cmd_input( GtkWidget *w, gpointer data);
    static void activate_print( GtkWidget *w, gpointer data);
    static void activate_login( GtkWidget *w, gpointer data);
    static void activate_logout( GtkWidget *w, gpointer data);
    static void activate_close( GtkWidget *w, gpointer data);
    static void activate_opensevhist( GtkWidget *w, gpointer data);
    static void activate_delete_item( GtkWidget *w, gpointer data);
    static void activate_tree_layout( GtkWidget *w, gpointer data);
    static void activate_list_layout( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget *w, gpointer data);
    static void activate_zoom_out( GtkWidget *w, gpointer data);
    static void activate_zoom_reset( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static void activate_help_project( GtkWidget *w, gpointer data);
    static void activate_help_proview( GtkWidget *w, gpointer data);
    
    ~XttTblGtk();
};


#endif
