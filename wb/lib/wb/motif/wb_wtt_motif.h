/* 
 * Proview   $Id: wb_wtt_motif.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_wtt_motif_h
#define wb_wtt_motif_h

/* wb_wtt_motif.h -- Simple navigator */

#ifndef wb_wtt_h
# include "wb_wtt.h"
#endif


class WttMotif : public Wtt {
  public:
    WttMotif( 
	void	*wt_parent_ctx,
	Widget 	wt_parent_wid,
	char 	*wt_name,
	char	*iconname,
	ldh_tWBContext wt_wbctx,
	pwr_tVolumeId wt_volid,
	ldh_tVolume wt_volctx,
	wnav_sStartMenu *root_menu,
	pwr_tStatus *status);
    WttMotif() {};

    Widget	parent_wid;
    Widget	wnav_brow_widget;
    Widget	wnavnode_brow_widget;
    Widget	form_widget;
    Widget	palette_widget;
    Widget	toplevel;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	wnav_form;
    Widget	palette_form;
    Widget	india_widget;
    Widget	india_label;
    Widget	india_text;
    Widget	confirm_widget;
    Widget	boot_dia;
    Widget	boot_list;
    Widget	cm_normal_syntax;
    Widget	cm_gms_syntax;
    Widget	cm_extern_syntax;
    Widget	cm_add_attribute;
    Widget	cm_add_type;
    Widget	cm_add_volume;
    Widget	options_form;
    Widget	show_plant_w;
    Widget	show_node_w;
    Widget	show_class_w;
    Widget	show_alias_w;
    Widget	show_descrip_w;
    Widget	show_objref_w;
    Widget	show_objxref_w;
    Widget	show_attrref_w;
    Widget	show_attrxref_w;
    Widget	build_force_w;
    Widget	build_debug_w;
    Widget	build_crossref_w;
    Widget	build_manual_w;
    Widget	menu_save_w;
    Widget	menu_revert_w;
    Widget	menu_cut_w;
    Widget	menu_copy_w;
    Widget	menu_copykeep_w;
    Widget	menu_paste_w;
    Widget	menu_pasteinto_w;
    Widget	menu_rename_w;
    Widget	menu_utilities_w;
    Widget	menu_openplc_w;
    Widget	menu_buildobject_w;
    Widget	menu_buildvolume_w;
    Widget	menu_buildnode_w;
    Widget	menu_distribute_w;
    Widget	menu_change_value_w;
    Widget	menu_edit_w;
    Widget	menu_classeditor_w;
    Widget	menu_updateclasses_w;
    int		set_focus_disabled;
    XtIntervalId disfocus_timerid;
    XtIntervalId selection_timerid;
    int		avoid_deadlock;
    XtIntervalId deadlock_timerid;
    int		clock_cursor;
    char	cmd_recall[30][160];
    int		cmd_current_recall;
    char	value_recall[30][160];
    int		value_current_recall;

    virtual void set_clock_cursor();
    virtual void reset_cursor();
    virtual void free_cursor();
    virtual void set_window_char( int width, int height);
    virtual void get_window_char( int *width, int *height);
    virtual void menu_setup();
    virtual void set_selection_owner();
    virtual void set_palette_selection_owner();
    virtual int create_popup_menu( pwr_tAttrRef attrref, int x, int y);
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

    Widget build_menu();
    Widget build_submenu( Widget Parent, int MenuType,
			  char *MenuTitle, void *MenuUserData,
			  void (*Callback)( Widget, Wtt *, XmAnyCallbackStruct *),
			  void *CallbackData, ldh_sMenuItem *Items, int *idx);

    static void set_selection_owner_proc( WttMotif *wtt);
    static void set_palette_selection_owner_proc( WttMotif *wtt);

    static void activate_change_value( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_command( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_exit( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_print( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_collapse( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_save( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_revert( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_syntax( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_find( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_findregex( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_findnext( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_copy( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_cut( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_paste( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_pasteinto( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_copykeep( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_rename( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_configure( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_utilities( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openobject( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openvolobject( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openplc( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_buildobject( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openvolume( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openbuffer( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_confproject( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openfile_dbs( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openfile_wbl( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openpl( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_opengvl( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openudb( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_spreadsheet( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openge( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_openclasseditor( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_buildvolume( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_buildnode( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_distribute( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_showcrossref( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_updateclasses( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_zoom_in( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_zoom_out( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_zoom_reset( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_twowindows( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_messages( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_view( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_savesettings( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_restoresettings( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_scriptproj( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_scriptbase( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_help_project( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_help_proview( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void create_menubutton( Widget w, Wtt *wtt);
    static void create_msg_label( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void create_cmd_prompt( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void valchanged_cmd_input( Widget w, XEvent *event);
    static void create_cmd_input( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void create_wnav_form( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void create_palette_form( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void create_india_label( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void create_india_text( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_india_ok( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_india_cancel( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_confirm_ok( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_confirm_no( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_confirm_cancel( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void activate_selmode( Widget w, Wtt *wtt, XmToggleButtonCallbackStruct *data);
    static void create_selmode( Widget w, Wtt *wtt, XmToggleButtonCallbackStruct *data);
    static void boot_list_cr(Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void boot_cancel_cb(Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void boot_destroy_cb(Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void boot_ok_cb(Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void options_act_but_cb( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void options_form_cr( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void options_entry_tog_cr( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void options_hier_tog_cr( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);

    static void enable_set_focus( Wtt *wtt);
    static void disable_set_focus( Wtt *wtt, int time);
    static void reset_avoid_deadlock( Wtt *wtt);
    static void set_avoid_deadlock( Wtt *wtt, int time);
    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
    static void popup_unmap_cb(Widget w, Wtt *wtt, XmAnyCallbackStruct *data);
    static void popup_button_cb( Widget w, Wtt *wtt, XmAnyCallbackStruct *data);

    ~WttMotif();
};

#endif
