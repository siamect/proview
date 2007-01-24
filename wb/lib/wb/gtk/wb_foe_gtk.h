/* 
 * Proview   $Id: wb_foe_gtk.h,v 1.2 2007-01-24 12:42:44 claes Exp $
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

#ifndef wb_foe_gtk_h
#define wb_foe_gtk_h

/* wb_foe_gtk.h -- function block object editor */

#ifndef wb_foe_h
#include "wb_foe.h"
#endif

#ifndef co_wow_gtk_h
#include "co_wow_gtk.h"
#endif


struct s_widgets {
  GtkWidget	*label;
  GtkWidget	*textinput;
  GtkWidget	*pane;
  GtkWidget	*subpane;
  GtkWidget     *caution;
  GtkWidget     *caution_label;
  GtkWidget     *adb;
  GtkWidget     *msgbox;
  GtkWidget	*foe_window;
  GtkWidget	*popupmenu;
  GtkWidget	*file_entry;
  GtkWidget	*save;
  GtkWidget	*exit;
  GtkWidget	*savetrace;
  GtkWidget	*restoretrace;
  GtkWidget	*print;
  GtkWidget	*syntax;
  GtkWidget	*compile;
  GtkWidget	*redraw;
  GtkWidget	*plcattribute;
  GtkWidget	*winddelete;
  GtkWidget	*quit;
  GtkWidget	*cut;
  GtkWidget	*copy;
  GtkWidget	*paste;
  GtkWidget	*undelete;
  GtkWidget	*unselect;
  GtkWidget	*connect;
  GtkWidget	*del;
  GtkWidget	*changetext;
  GtkWidget	*expand;
  GtkWidget	*compress;
  GtkWidget	*edit_entry;       /* entry of the menu */
  GtkWidget	*search_entry;     /* entry of the menu */
  GtkWidget	*customize_entry;  /* entry of the menu */
  GtkWidget	*trace_togg;       /* togg under the function entry of the menu */
  GtkWidget	*simulate_togg;    /* togg under the function entry of the menu */
  GtkWidget	*view_togg;        /* togg under the function entry of the menu */
  GtkWidget	*edit_togg;        /* togg under the function entry of the menu */
  GtkWidget	*con_palette;
  GtkWidget	*node_palette;
  GtkWidget	*nav_palette;
  GtkWidget	*view_objectpal;
  GtkWidget	*view_conpal;
  GtkWidget	*view_plantpal;
  GtkWidget	*tools_save;
  GtkWidget	*tools_build;
};

class WFoeGtk : public WFoe {
 public:
  GtkWidget	*parent_wid;
  GtkWidget	*toplevel;
  struct 	s_widgets widgets ;
  GdkAtom      	objid_atom;
  int		popupmenu_x;
  int		popupmenu_y;
  CoWowFocusTimerGtk focustimer;

  
  WFoeGtk( void *f_parent_ctx,
	      GtkWidget *f_parent_wid,
	      char *f_name,
	      pwr_tObjid plcprogram,
	      ldh_tWBContext ldhwbctx,
	      ldh_tSesContext ldhsesctx,
	      int f_map_window,
	      ldh_eAccess f_access,
	      pwr_tStatus *sts);
  WFoeGtk( void      	*f_parent_ctx,
	GtkWidget      	*f_parent_wid,
	char		*f_name,
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
  virtual void message( char *new_label); 
  virtual void msgbox( char *new_label); 
  virtual int create_window( int x_top,
			     int y_top,
			     int width_adb,
			     int height_adb, 
			     ldh_tSesContext ldhsession,
			     foe_eFuncAccess function_access);
  virtual int get_textinput( char *message, int (*function) (WFoe *, char *));
  virtual int edit_set_entries();
  virtual int view_set_entries();
  virtual void set_mode_button_state( int mode, int state);
  virtual int modify_popup( unsigned long popupmenu_mask, int x, int y);
  virtual int get_selection( char *str, int len);
  virtual void popupmsg( char *new_label, void (* yes_procedure)(WFoe *), 
			 void (* no_procedure)(WFoe *),
			 void (* cancel_procedure)(WFoe *));

  int register_callbacks();
  void create_confirm_dialog();

  static void reset_current_node( WFoe *foe);
  static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
      
  static void activate_save (GtkWidget *w, gpointer data);
  static void activate_exit( GtkWidget *w, gpointer data);
  static void activate_winddelete( GtkWidget *w, gpointer data);
  static void activate_quit( GtkWidget *w, gpointer data);
  static void activate_savetrace( GtkWidget *w, gpointer data);
  static void activate_restoretrace( GtkWidget *w, gpointer data);
  static void activate_print( GtkWidget *w, gpointer data);
  static void activate_printoverv( GtkWidget *w, gpointer data);
  static void activate_printselect( GtkWidget *w, gpointer data);
  static void activate_syntax( GtkWidget *w, gpointer data);
  static void activate_compile( GtkWidget *w, gpointer data);
  static void delete_subwindow_ok_cb( void *ctx, void *data);
  static void activate_delete( GtkWidget *w, gpointer data);
  static void activate_createobject( GtkWidget *w, gpointer data);
  static void activate_changetext( GtkWidget *w, gpointer data);
  static void activate_cut( GtkWidget *w, gpointer data);
  static void activate_copy( GtkWidget *w, gpointer data);
  static void activate_paste( GtkWidget *w, gpointer data);
  static void activate_attribute( GtkWidget *w, gpointer data);
  static void activate_subwindow( GtkWidget *w, gpointer data);
  static void activate_undelete( GtkWidget *w, gpointer data);
  static void activate_unselect( GtkWidget *w, gpointer data);
  static void activate_gridsize1( GtkWidget *w, gpointer data);
  static void activate_gridsize2( GtkWidget *w, gpointer data);
  static void activate_gridsize3( GtkWidget *w, gpointer data);
  static void activate_gridsize4( GtkWidget *w, gpointer data);
  static void activate_gridsize5( GtkWidget *w, gpointer data);
  static void activate_showgrid( GtkWidget *w, gpointer data);
  static void activate_redraw( GtkWidget *w, gpointer data);
  static void activate_zoomin( GtkWidget *w, gpointer data);
  static void activate_zoomout( GtkWidget *w, gpointer data);
  static void activate_unzoom( GtkWidget *w, gpointer data);
  static void activate_palette_con( GtkWidget *w, gpointer data);
  static void activate_palette_object( GtkWidget *w, gpointer data);
  static void activate_palette_plant( GtkWidget *w, gpointer data);
  static void activate_refcon( GtkWidget *w, gpointer data);
  static void activate_showexeord( GtkWidget *w, gpointer data);
  static void activate_plcattribute( GtkWidget *w, gpointer data);
  static void activate_search( GtkWidget *w, gpointer data);
  static void activate_searchobject( GtkWidget *w, gpointer data);
  static void activate_searchstring( GtkWidget *w, gpointer data);
  static void activate_searchnext( GtkWidget *w, gpointer data);
  static void activate_getobj( GtkWidget *w, gpointer data);
  static void activate_expand( GtkWidget *w, gpointer data);
  static void activate_compress( GtkWidget *w, gpointer data);
  static void activate_help( GtkWidget *w, gpointer data);
  static void activate_helpclass( GtkWidget *w, gpointer data);
  static void activate_trace_togg( GtkWidget *w, gpointer data);
  static void activate_simulate_togg( GtkWidget *w, gpointer data);
  static void activate_view_togg( GtkWidget *w, gpointer data);
  static void activate_edit_togg( GtkWidget *w, gpointer data);
  static void create_label( GtkWidget *w, gpointer data); 
  static void create_textinput( GtkWidget *w, gpointer data); 
  static void create_widget_cb( GtkWidget *w, gpointer data);
  static void create_adb( GtkWidget *w, gpointer data);
  static void create_edit_form( GtkWidget *w, gpointer data);
  static void create_subpane( GtkWidget *w, gpointer data);
  static void create_palette_object( GtkWidget *w, gpointer data); 
  static void popdown_pop( GtkWidget *w, gpointer data);
  static void valchanged_textinput( GtkWidget *w, gpointer data);
  static void yes_popupmsg( GtkWidget *w, gpointer data);
  static void no_popupmsg( GtkWidget *w, gpointer data);
  static void cancel_popupmsg( GtkWidget *w, gpointer data);
  static void popup_unmap_cb(GtkWidget *w, gpointer data);

};


#endif
