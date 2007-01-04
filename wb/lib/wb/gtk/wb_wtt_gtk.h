/* 
 * Proview   $Id: wb_wtt_gtk.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_wtt_gtk_h
#define wb_wtt_gtk_h

/* wb_wtt_gtk.h -- Simple navigator */

#ifndef wb_wtt_h
# include "wb_wtt.h"
#endif

#include <gtk/gtk.h>
#include "co_wow_gtk.h"

class WttGtk : public Wtt {
  public:
    WttGtk( 
	void	*wt_parent_ctx,
	GtkWidget *wt_parent_wid,
	char 	*wt_name,
	char	*iconname,
	ldh_tWBContext wt_wbctx,
	pwr_tVolumeId wt_volid,
	ldh_tVolume wt_volctx,
	wnav_sStartMenu *root_menu,
	pwr_tStatus *status);
    WttGtk() {};

    GtkWidget	*parent_wid;
    GtkWidget	*wnav_brow_widget;
    GtkWidget	*wnavnode_brow_widget;
    GtkWidget	*wnav_paned;
    GtkWidget	*palette_widget;
    GtkWidget	*toplevel;
    GtkWidget	*msg_label;
    GtkWidget	*cmd_prompt;
    GtkWidget	*cmd_input;
    GtkWidget	*wnav_form;
    GtkWidget	*palette_paned;
    GtkWidget	*india_widget;
    GtkWidget	*india_label;
    GtkWidget	*india_text;
    GtkWidget	*confirm_widget;
    GtkWidget	*confirm_label;
    GtkWidget	*boot_dia;
    GtkWidget	*boot_list;
    GtkWidget	*cm_normal_syntax;
    GtkWidget	*cm_gms_syntax;
    GtkWidget	*cm_extern_syntax;
    GtkWidget	*cm_add_attribute;
    GtkWidget	*cm_add_type;
    GtkWidget	*cm_add_volume;
    GtkWidget	*options_form;
    GtkWidget	*show_plant_w;
    GtkWidget	*show_node_w;
    GtkWidget	*show_class_w;
    GtkWidget	*show_alias_w;
    GtkWidget	*show_descrip_w;
    GtkWidget	*show_objref_w;
    GtkWidget	*show_objxref_w;
    GtkWidget	*show_attrref_w;
    GtkWidget	*show_attrxref_w;
    GtkWidget	*build_force_w;
    GtkWidget	*build_debug_w;
    GtkWidget	*build_crossref_w;
    GtkWidget	*build_manual_w;
    GtkWidget	*menu_save_w;
    GtkWidget	*menu_revert_w;
    GtkWidget	*menu_cut_w;
    GtkWidget	*menu_copy_w;
    GtkWidget	*menu_copykeep_w;
    GtkWidget	*menu_paste_w;
    GtkWidget	*menu_pasteinto_w;
    GtkWidget	*menu_rename_w;
    GtkWidget	*menu_utilities_w;
    GtkWidget	*menu_openplc_w;
    GtkWidget	*menu_buildobject_w;
    GtkWidget	*menu_buildvolume_w;
    GtkWidget	*menu_buildnode_w;
    GtkWidget	*menu_distribute_w;
    GtkWidget	*menu_change_value_w;
    GtkWidget	*menu_edit_w;
    GtkWidget	*menu_classeditor_w;
    GtkWidget	*menu_updateclasses_w;
    int		set_focus_disabled;
    gint 	disfocus_timerid;
    gint 	selection_timerid;
    int		avoid_deadlock;
    gint 	deadlock_timerid;
    GdkCursor	*clock_cursor;
    int		popupmenu_x;
    int		popupmenu_y;
    static CoWowRecall cmd_recall;
    static CoWowRecall value_recall;
    static CoWowRecall name_recall;
    CoWowEntryGtk *cmd_entry;
    CoWowFocusTimerGtk focustimer;

    GtkWidget *build_menu();
    GtkWidget *build_submenu( GtkWidget *Parent, int MenuType,
			      char *MenuTitle, void *MenuUserData,
			      void (*Callback)( GtkWidget *, gpointer),
			      void *CallbackData, ldh_sMenuItem *Items, int *idx);

    virtual void set_clock_cursor();
    virtual void reset_cursor();
    virtual void free_cursor();
    virtual void set_window_char( int width, int height);
    virtual void get_window_char( int *width, int *height);
    virtual void menu_setup();
    virtual void set_selection_owner();
    virtual void set_palette_selection_owner();
    virtual int create_popup_menu( pwr_sAttrRef attrref, int x, int y);
    virtual int create_pal_popup_menu( pwr_tCid cid, int x, int y);
    virtual void set_noedit_show();
    virtual void set_edit_show();
    virtual void set_twowindows( int two, int display_wnav, int display_wnavnode);
    virtual void message( char severity, char *message);
    virtual void set_prompt( char *prompt);
    virtual void open_change_value();
    virtual void close_change_value();
    virtual void open_change_name();
    virtual void watt_new( pwr_tAttrRef aref);
    virtual void wda_new( pwr_tOid oid);
    virtual void ge_new( char *graphname);
    virtual void wcast_new( pwr_tAttrRef aref, pwr_tStatus *sts);
    virtual wb_build *build_new();
    virtual void wpkg_new();
    virtual int ute_new( char *title);
    virtual void open_input_dialog( char *text, char *title,
				    char *init_text,
				    void (*ok_cb)( Wtt *, char *));
    virtual void open_confirm( char *text, char *title, 
			       void (*ok_cb)( Wtt *), void (*no_cb)( Wtt *));
    virtual void open_boot_window();
    virtual void update_options_form();
    virtual void set_options();
    virtual void pop();

    void create_options_dialog();
    void create_confirm_dialog();
    void create_input_dialog();

    static gboolean set_selection_owner_proc( void *data);
    static gboolean set_palette_selection_owner_proc( void *data);

    static void activate_change_value( GtkWidget *w, gpointer data);
    static void activate_command( GtkWidget *w, gpointer data);
    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_print( GtkWidget *w, gpointer data);
    static void activate_collapse( GtkWidget *w, gpointer data);
    static void activate_save( GtkWidget *w, gpointer data);
    static void activate_revert( GtkWidget *w, gpointer data);
    static void activate_syntax( GtkWidget *w, gpointer data);
    static void activate_find( GtkWidget *w, gpointer data);
    static void activate_findregex( GtkWidget *w, gpointer data);
    static void activate_findnext( GtkWidget *w, gpointer data);
    static void activate_copy( GtkWidget *w, gpointer data);
    static void activate_cut( GtkWidget *w, gpointer data);
    static void activate_paste( GtkWidget *w, gpointer data);
    static void activate_pasteinto( GtkWidget *w, gpointer data);
    static void activate_copykeep( GtkWidget *w, gpointer data);
    static void activate_rename( GtkWidget *w, gpointer data);
    static void activate_configure( GtkWidget *w, gpointer data);
    static void activate_utilities( GtkWidget *w, gpointer data);
    static void activate_openobject( GtkWidget *w, gpointer data);
    static void activate_openvolobject( GtkWidget *w, gpointer data);
    static void activate_openplc( GtkWidget *w, gpointer data);
    static void activate_buildobject( GtkWidget *w, gpointer data);
    static void activate_openvolume( GtkWidget *w, gpointer data);
    static void activate_openbuffer( GtkWidget *w, gpointer data);
    static void activate_confproject( GtkWidget *w, gpointer data);
    static void activate_openfile_dbs( GtkWidget *w, gpointer data);
    static void activate_openfile_wbl( GtkWidget *w, gpointer data);
    static void activate_openpl( GtkWidget *w, gpointer data);
    static void activate_opengvl( GtkWidget *w, gpointer data);
    static void activate_openudb( GtkWidget *w, gpointer data);
    static void activate_spreadsheet( GtkWidget *w, gpointer data);
    static void activate_openge( GtkWidget *w, gpointer data);
    static void activate_openclasseditor( GtkWidget *w, gpointer data);
    static void activate_buildvolume( GtkWidget *w, gpointer data);
    static void activate_buildnode( GtkWidget *w, gpointer data);
    static void activate_distribute( GtkWidget *w, gpointer data);
    static void activate_showcrossref( GtkWidget *w, gpointer data);
    static void activate_updateclasses( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget *w, gpointer data);
    static void activate_zoom_out( GtkWidget *w, gpointer data);
    static void activate_zoom_reset( GtkWidget *w, gpointer data);
    static void activate_twowindows( GtkWidget *w, gpointer data);
    static void activate_messages( GtkWidget *w, gpointer data);
    static void activate_view( GtkWidget *w, gpointer data);
    static void activate_savesettings( GtkWidget *w, gpointer data);
    static void activate_restoresettings( GtkWidget *w, gpointer data);
    static void activate_scriptproj( GtkWidget *w, gpointer data);
    static void activate_scriptbase( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static void activate_help_project( GtkWidget *w, gpointer data);
    static void activate_help_proview( GtkWidget *w, gpointer data);
    static void valchanged_cmd_input( GtkWidget *w, gpointer data);
    static void activate_india_ok( GtkWidget *w, gpointer data);
    static void activate_india_cancel( GtkWidget *w, gpointer data);
    static void activate_confirm_ok( GtkWidget *w, gpointer data);
    static void activate_confirm_no( GtkWidget *w, gpointer data);
    static void activate_confirm_cancel( GtkWidget *w, gpointer data);
    static void activate_selmode( GtkWidget *w, gpointer data);
    static void boot_list_cr(GtkWidget *w, gpointer data);
    static void boot_cancel_cb(GtkWidget *w, gpointer data);
    static void boot_destroy_cb(GtkWidget *w, gpointer data);
    static gint boot_delete_cb(GtkWidget *w, GdkEvent *event, gpointer data);
    static void boot_ok_cb(GtkWidget *w, gpointer data);
    static void activate_options_ok( GtkWidget *w, gpointer data);
    static void activate_options_apply( GtkWidget *w, gpointer data);
    static void activate_options_cancel( GtkWidget *w, gpointer data);

    static void enable_set_focus( Wtt *wtt);
    static void disable_set_focus( Wtt *wtt, int time);
    static void reset_avoid_deadlock( Wtt *wtt);
    static void set_avoid_deadlock( Wtt *wtt, int time);
    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void menu_position_func( GtkMenu *menu, gint *x, gint *y, gboolean *push_in,
				    gpointer data);
    static void popup_button_cb( GtkWidget *w, gpointer data);

    ~WttGtk();
};

GtkWidget *wtt_create_popup_menu( Wtt *wtt, pwr_sAttrRef aref, pwr_tCid cid,
				  void (*message_cb)(void *, char, char *));
#endif
