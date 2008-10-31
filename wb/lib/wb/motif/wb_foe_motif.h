/* 
 * Proview   $Id: wb_foe_motif.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_foe_motif_h
#define wb_foe_motif_h

/* wb_foe_motif.h -- function block object editor */

#ifndef wb_foe_h
#include "wb_foe.h"
#endif


struct s_widgets {
  Widget	label;
  Widget	textinput;
  Widget	pane;
  Widget	subpane;
  Widget        caution;
  Widget        adb;
  Widget        msgbox;
  Widget	foe_window;
  Widget	popupmenu;
  Widget	file_entry;
  Widget	save;
  Widget	exit;
  Widget	savetrace;
  Widget	restoretrace;
  Widget	print;
  Widget	syntax;
  Widget	compile;
  Widget	redraw;
  Widget	plcattribute;
  Widget	winddelete;
  Widget	quit;
  Widget	cut;
  Widget	copy;
  Widget	paste;
  Widget	undelete;
  Widget	unselect;
  Widget	connect;
  Widget	del;
  Widget	changetext;
  Widget	expand;
  Widget	compress;
  Widget	edit_entry;       /* entry of the menu */
  Widget	search_entry;     /* entry of the menu */
  Widget	customize_entry;  /* entry of the menu */
  Widget	trace_togg;       /* togg under the function entry of the menu */
  Widget	simulate_togg;    /* togg under the function entry of the menu */
  Widget	view_togg;        /* togg under the function entry of the menu */
  Widget	edit_togg;        /* togg under the function entry of the menu */
  Widget	con_palette;
  Widget	node_palette;
  Widget	nav_palette;
} ;

class WFoeMotif : public WFoe {
 public:
  Widget	parent_wid;
  struct 	s_widgets widgets ;
  int		set_focus_disabled;
  XtIntervalId focus_timerid;
  Atom		objid_atom;

  
  WFoeMotif( void *f_parent_ctx,
	      Widget f_parent_wid,
	      const char *f_name,
	      pwr_tObjid plcprogram,
	      ldh_tWBContext ldhwbctx,
	      ldh_tSesContext ldhsesctx,
	      int f_map_window,
	      ldh_eAccess f_access,
	      pwr_tStatus *sts);
  WFoeMotif( void      	*f_parent_ctx,
	Widget		f_parent_wid,
	const char     	*f_name,
	pwr_tObjid     	plcprogram,
	ldh_tWBContext 	ldhwbctx,
	ldh_tSesContext	ldhsesctx,
	vldh_t_node    	nodeobject,
	unsigned long	windowindex,
	unsigned long	new_window,
	int	       	f_map_window,
	ldh_eAccess    	f_access,
	foe_eFuncAccess	function_access,
	pwr_tStatus 	*sts);

  virtual void pop();
  virtual WAtt *watt_new( void *a_parent_ctx,
			  ldh_tSesContext a_ldhses,
			  pwr_sAttrRef a_aref,
			  int a_editmode,
			  int a_advanced_user,
			  int a_display_objectname);
  virtual WFoe *subwindow_new( void	       	*f_parent_ctx,
			       char	       	*f_name,
			       pwr_tObjid     	plcprogram,
			       ldh_tWBContext 	ldhwbctx,
			       ldh_tSesContext	ldhsesctx,
			       vldh_t_node    	nodeobject,
			       unsigned long	windowindex,
			       unsigned long	new_window,
			       int	       		f_map_window,
			       ldh_eAccess    	f_access,
			       foe_eFuncAccess	function_access,
			       pwr_tStatus 	*sts);
  virtual void destroy();
  virtual void set_title();
  virtual void message( const char *new_label); 
  virtual void msgbox( const char *new_label); 
  virtual int create_window( int x_top,
			     int y_top,
			     int width_adb,
			     int height_adb, 
			     ldh_tSesContext ldhsession,
			     foe_eFuncAccess function_access);
  virtual int get_textinput( const char *message, int (*function) (WFoe *, char *));
  virtual int edit_set_entries();
  virtual int view_set_entries();
  virtual void set_mode_button_state( int mode, int state);
  virtual int modify_popup( unsigned long popupmenu_mask, int x, int y);
  virtual int get_selection( char *str, int len);
  virtual void popupmsg( const char *new_label, void (* yes_procedure)(WFoe *), 
			 void (* no_procedure)(WFoe *),
			 void (* cancel_procedure)(WFoe *));

  int register_callbacks();

  static void reset_current_node( WFoe *foe);
  static void popup_unmap_cb(Widget w, WFoe *foe);
  static void enable_set_focus( WFoeMotif *foe);
  static void disable_set_focus( WFoeMotif *foe, int time);
  static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
      
  static void activate_save (Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_exit( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_winddelete( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_quit( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_savetrace( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_restoretrace( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_print( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_printoverv( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_printselect( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_syntax( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_compile( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void delete_subwindow_ok_cb( void *ctx, void *data);
  static void activate_delete( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_changetext( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_cut( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_copy( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_paste( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_attribute( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_subwindow( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_undelete( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_unselect( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_gridsize1( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_gridsize2( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_gridsize3( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_gridsize4( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_gridsize5( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_showgrid( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_redraw( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_zoomin( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_zoomout( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_unzoom( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_palette_con( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_palette_object( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_palette_plant( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_refcon( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_showexeord( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_plcattribute( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_search( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_searchobject( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_searchstring( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_searchnext( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_getobj( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_expand( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_compress( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_help( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_helpclass( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void activate_trace_togg( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_simulate_togg( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_view_togg( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void activate_edit_togg( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data);
  static void create_label( Widget w, WFoe *foe, XmAnyCallbackStruct *data); 
  static void create_textinput( Widget w, WFoe *foe, XmAnyCallbackStruct *data); 
  static void create_widget_cb( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void create_adb( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void create_edit_form( Widget w, WFoe *foe, XmAnyCallbackStruct data);
  static void create_subpane( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void create_palette_object( Widget w, WFoe *foe, XmAnyCallbackStruct *data); 
  static void popdown_pop( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void valchanged_textinput( Widget w, WFoeMotif *foe, XmAnyCallbackStruct *data);
  static void yes_popupmsg( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void no_popupmsg( Widget w, WFoe *foe, XmAnyCallbackStruct *data);
  static void cancel_popupmsg( Widget w, WFoe *foe, XmAnyCallbackStruct *data);

};


#endif
