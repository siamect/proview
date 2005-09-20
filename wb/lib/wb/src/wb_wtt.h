/* 
 * Proview   $Id: wb_wtt.h,v 1.13 2005-09-20 13:14:28 claes Exp $
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

#ifndef wb_wtt_h
#define wb_wtt_h

/* wb_wtt.h -- Simple navigator */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif


#ifndef flow_std_h
#include "flow_std.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef wb_ldh
# include "wb_ldh.h"
#endif

#ifndef wb_pal
# include "wb_pal.h"
#endif

#ifndef wb_wnav_h
# include "wb_wnav.h"
#endif

#ifndef wb_h
# include "wb.h"
#endif

#ifndef wb_uted_h
#include "wb_uted.h"
#endif

#ifndef wb_wda_h
#include "wb_wda.h"
#endif

#ifndef co_wow_h
#include "co_wow.h"
#endif

class WPkg;

typedef enum {
  	wtt_eNoEdit_Save,
  	wtt_eNoEdit_Revert,
	} wtt_eNoEditMode;

typedef enum {
  	wtt_eNoEdit_DetachVolume,
  	wtt_eNoEdit_KeepVolume,
	} wtt_eNoEditVolMode;

typedef enum {
  	wtt_eInputMode_Attribute,
  	wtt_eInputMode_ObjectName
	} wtt_eInputMode;


class WttApplListElem {
  public:
    WttApplListElem( wb_eUtility al_type, void *al_ctx, pwr_tObjid al_objid,
	char *al_name);
    wb_eUtility		type;
    void		*ctx;
    pwr_tObjid		objid;
    char		name[80];
    WttApplListElem 	*next;
};

class WttApplList {
  public:
    WttApplList() :
       root(NULL) {};
    ~WttApplList();

    WttApplListElem *root;
    void insert( wb_eUtility type, void *ctx, 
	pwr_tObjid objid, char *name);
    void remove( void *ctx);
    int find( wb_eUtility type, char *name, void **ctx);
    int find( wb_eUtility type, pwr_tObjid objid, void **ctx);
    void set_editmode( int editmode, ldh_tSesContext ldhses);
};

class Wtt {
  public:
    Wtt( 
	void	*wt_parent_ctx,
	Widget 	wt_parent_wid,
	char 	*wt_name,
	char	*iconname,
	ldh_tWBContext wt_wbctx,
	pwr_tVolumeId wt_volid,
	ldh_tVolume wt_volctx,
	wnav_sStartMenu *root_menu,
	pwr_tStatus *status);

    wb_eUtility	ctx_type;
    void 	*parent_ctx;
    Widget	parent_wid;
    char 	name[80];
    wb_eType	wb_type;
    WttApplList	appl;
    Widget	wnav_brow_widget;
    Widget	wnavnode_brow_widget;
    Widget	form_widget;
    Widget	palette_widget;
    Widget	toplevel;
    WNav	*wnav;
    WNav	*wnavnode;
    WNav	*focused_wnav;
    Pal		*palette;
    void	*root_item;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	wnav_form;
    Widget	palette_form;
    int		input_open;
    int		command_open;
    ldh_tWBContext wbctx;
    ldh_tVolContext volctx;
    pwr_tVolumeId	volid;
    ldh_tSesContext ldhses;
    int		editmode;
    int		twowindows;
    Widget	india_widget;
    Widget	india_label;
    Widget	india_text;
    Widget	confirm_widget;
    int		confirm_open;
    void		(*india_ok_cb)( Wtt *, char *);
    void		(*confirm_ok_cb)( Wtt *);
    void		(*confirm_no_cb)( Wtt *);
    struct {
      Widget	dia;
      Widget	deb;
      Widget	list;
      void	*volumelist;
      int	volumecount;
    } boot; 
    struct {
      Widget	normal_syntax;
      Widget	gms_syntax;
      Widget	extern_syntax;
      Widget	add_attribute;
      Widget	add_type;
      Widget	add_volume;
    } copy_mode;
    int		select_syntax;
    int		select_volume;
    int		select_attr;
    int		select_type;
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
    int		show_class;
    int		show_alias;
    int		show_descrip;
    int		show_objref;
    int		show_objxref;
    int		show_attrref;
    int		show_attrxref;
    int		wnav_mapped;
    int		wnavnode_mapped;
    Widget	menu_save_w;
    Widget	menu_revert_w;
    Widget	menu_cut_w;
    Widget	menu_copy_w;
    Widget	menu_paste_w;
    Widget	menu_pasteinto_w;
    Widget	menu_rename_w;
    Widget	menu_utilities_w;
    Widget	menu_openplc_w;
    Widget	menu_compile_w;
    Widget	menu_createload_w;
    Widget	menu_createboot_w;
    Widget	menu_distribute_w;
    Widget	menu_change_value_w;
    Widget	menu_edit_w;
    Widget	menu_classeditor_w;
    Widget	menu_createstruct_w;
    uted_ctx	utedctx;
    WPkg	*wpkg;
    WNav	*input_wnav;
    brow_tObject input_node;
    pwr_tObjid	input_objid;
    wtt_eInputMode input_mode;
    void	(*close_cb)(void *ctx);
    void	(*open_volume_cb)(void *ctx, wb_eType, char *, wow_eFileSelType);
    void	(*open_project_volume_cb)(void *ctx);
    int		(*time_to_exit_cb)(void *ctx);
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

    void menu_setup();
    int restore_settings();
    int save_settings();
    void set_editmode( int edit);
    int set_edit();
    int set_noedit( wtt_eNoEditMode save, wtt_eNoEditVolMode detach);
    void set_twowindows( int two, int display_wnav, int display_wnavnode);
    void set_prompt( char *prompt);
    void open_change_value();
    void close_change_value();
    void open_change_name();
    void update_options_form();
    void set_options();
    void message( char severity, char *message);
    int set_focus( void *component);
    int is_saved();
    int detach_volume();
    void set_clock_cursor();
    void reset_cursor();
    void free_cursor();
    int get_select_first( pwr_sAttrRef *attrref, int *is_attr);
    void register_utility( void *ctx, wb_eUtility utility);
    void set_focus_default();
    void pop();
    int find( pwr_tOid oid);
    int find_plc( pwr_tOid oid);

    ~Wtt();
};

Widget wtt_create_popup_menu( Wtt *wtt, pwr_sAttrRef aref, pwr_tCid cid,
			      void (*message_cb)(void *, char, char *));

#if defined __cplusplus
}
#endif
#endif
