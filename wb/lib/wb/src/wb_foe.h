/* 
 * Proview   $Id: wb_foe.h,v 1.18 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_foe_h
#define wb_foe_h

/* wb_foe.h -- function block object editor */

#ifndef wb_vldh_h
#include "wb_vldh.h"
#endif

#ifndef wb_utility_h
#include "wb_utility.h"
#endif

#ifndef wb_h
#include "wb.h"
#endif

#ifndef wb_goen_h
#include "wb_goen.h"
#endif

#ifndef wb_gre_h
#include "wb_gre.h"
#endif

#ifndef wb_tra_h
#include "wb_tra.h"
#endif

#define TRACE 0
#define SIMULATE 1
#define VIEW 2
#define EDIT 3

typedef enum {
  foe_mOption_EnableComment = 1
} foe_mOption;

typedef enum {
  foe_eFuncAccess_View		= 1 << 0,
  foe_eFuncAccess_Edit		= 1 << 1
} foe_eFuncAccess;

enum foe_e_popupmenu {
  foe_e_popupmenu_attribute	= 1 << 0,
  foe_e_popupmenu_subwindow	= 1 << 1,
  foe_e_popupmenu_connect      	= 1 << 2,
  foe_e_popupmenu_delete       	= 1 << 3,
  foe_e_popupmenu_paste		= 1 << 4,
  foe_e_popupmenu_copy		= 1 << 5,
  foe_e_popupmenu_cut		= 1 << 6,
  foe_e_popupmenu_printselect	= 1 << 7,
  foe_e_popupmenu_helpclass	= 1 << 8,
  foe_e_popupmenu_createobject	= 1 << 9
};

class WFoe;
class WAtt;
class CoWow;
class Pal;
class Nav;

typedef struct 	{
  void		*a_ctx;
  WFoe	 	*foe;
  vldh_t_node	node;
} foe_sAttr;


class WFoe : public WUtility {
 public:
  void		*parent_ctx;
  pwr_tOName   	name;
  Pal		*node_palctx;
  Pal 		*con_palctx;
  Nav		*navctx;
  void		*tractx;
  WGre		*gre;

  void         (* popupmsg_yes_p)(WFoe *);
  void         (* popupmsg_no_p)(WFoe *); 
  void         (* popupmsg_cancel_p)(WFoe *);
  int           msg_label_id;
  int 		function; 		/* TRACE or EDIT or ... */
  int		wanted_function;	
  vldh_t_plc	plcobject;
  int		con_palette_managed;
  int		node_palette_managed;
  int		nav_palette_managed;
  int		con_drawtype;
  int		show_execorder;
  char		searchstring[80];
  int		searchindex;
  int		(*textinput_func)(WFoe *, char *);
  unsigned long	popupmenu_mask;
  vldh_t_node	popupmenu_node;
  ldh_eAccess	access;
  int		map_window;
  int		advanced_user;
  int		ldh_cb_enabled;
  int		classeditor;
  unsigned int	options;
  int		use_feedback_con;
  static foe_sAttr *attr_pointer;
  static int	attr_count;
  CoWow		*wow;

  
  WFoe( void *f_parent_ctx,
	const char *f_name,
	pwr_tObjid plcprogram,
	ldh_tWBContext ldhwbctx,
	ldh_tSesContext ldhsesctx,
	int f_map_window,
	ldh_eAccess f_access,
	unsigned int f_options,
	pwr_tStatus *sts);
  WFoe( void		*f_parent_ctx,
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
	unsigned int	f_options,
	pwr_tStatus 	*sts);
  virtual ~WFoe() {}

  virtual void pop() {}
  virtual WAtt *watt_new( void *a_parent_ctx,
			  ldh_tSesContext a_ldhses,
			  pwr_sAttrRef a_aref,
			  int a_editmode,
			  int a_advanced_user,
			  int a_display_objectname) 
    {return 0;}
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
			       unsigned int	f_options,
			       pwr_tStatus 	*sts) 
    {return 0;}
  virtual void destroy() {}
  virtual void set_title() {}
  virtual void message( const char *new_label) {}
  virtual void msgbox( const char *new_label) {} 
  virtual int create_window( int x_top,
			     int y_top,
			     int width_adb,
			     int height_adb, 
			     ldh_tSesContext ldhsession,
			     foe_eFuncAccess function_access)
    {return 0;}
  virtual int get_textinput( const char *message, int (*function) (WFoe *, char *))
    {return 0;}
  virtual int edit_set_entries() {return 0;}
  virtual int view_set_entries() {return 0;}
  virtual void set_mode_button_state( int mode, int state) {}
  virtual int modify_popup( unsigned long popupmenu_mask, int x, int y) {return 0;}
  virtual void clock_cursor() {}
  virtual void normal_cursor() {}
  virtual int get_selection( char *str, int len) {return 0;}
  virtual void popupmsg( const char *new_label, void (* yes_procedure)(WFoe *), 
			 void (* no_procedure)(WFoe *),
			 void (* cancel_procedure)(WFoe *)) {}

  void activate_save();
  void activate_exit();
  void activate_winddelete();
  void activate_quit();
  void activate_savetrace();
  void activate_restoretrace();
  void activate_print();
  void activate_printoverv();
  void activate_printselect();
  void activate_printpdf();
  void activate_syntax();
  void activate_compile();
  void activate_delete();
  void activate_delete_confirm();
  void activate_createobject( float x, float y);
  void activate_changetext();
  void activate_cut();
  void activate_copy();
  void activate_paste();
  void activate_select_nextobject( flow_eDirection dir);
  void activate_select_addnextobject( flow_eDirection dir);
  void activate_select_nextconpoint( flow_eDirection dir);
  void activate_scroll( flow_eDirection dir);
  void activate_move_object( flow_eDirection dir);
  void activate_attribute();
  void activate_subwindow();
  void activate_undelete();
  void activate_unselect();
  void activate_createnode();
  void activate_createconnection();
  void activate_connect();
  void activate_redraw();
  void activate_zoomin();
  void activate_zoomout();
  void activate_unzoom();
  void activate_refcon( int set);
  void activate_showexeord( int set);
  void activate_plcattribute();
  void activate_searchobject();
  void activate_searchstring();
  void activate_searchnext();
  void activate_getobj();
  void activate_expand();
  void activate_compress();
  void activate_help();
  void activate_helpclass();
  void activate_trace_togg( int set);
  void activate_simulate_togg( int set);
  void activate_view_togg( int set);
  void activate_edit_togg( int set);
  void activate_conpoint_lock();
  void activate_confeedback( int set) { use_feedback_con = set;}

  void function_setup();
  int register_callbacks();
  void enable_ldh_cb();
  void disable_ldh_cb();
  int attr_create( vldh_t_node node);
  int attr_delete();
  int parent_quit();
  int child_quit( vldh_t_node node, unsigned long windowindex);
  int child_delete( vldh_t_node node, vldh_t_wind subwind);
  void quit();
  void foe_exit();
  void foe_delete();
  int init_window();
  int subwindow_create( vldh_t_node object, unsigned long subwindow_nr);
  int view_setup();
  int edit_setup();
  int show_executeorder();
  int search_string_next();
  int search_object( char *searchstr);
  int center_object( pwr_tOid oid);
  int print_overview();
  int print_document();
  int print_selected_document();
  int print_pdf_overview();
  int change_mode( int new_mode);
  int redraw_and_save();
  int create_flow();

  static WFoe *get( pwr_tOid oid);
  static void reset_current_node( WFoe *foe);
  static int search_object( WFoe *foe, char *searchstr);
  static int search_string( WFoe *foe, char *searchstr);
  int new_local( pwr_tObjid	plcprogram,
		 ldh_tWBContext	ldhwbctx,
		 ldh_tSesContext ldhsesctx,
		 vldh_t_node	nodeobject,
		 unsigned long	windowindex,
		 unsigned long	new_window,
		 foe_eFuncAccess function_access);
  void get_hinactx( void **hinactx);
  
  static pwr_tStatus ldh_this_session_cb( void *ctx, ldh_sEvent *event);
  static void error_msg( unsigned long sts);
  static void info_msg( unsigned long sts);
  static int attrlist_get( void *a_ctx, WFoe **foe, vldh_t_node *node);
  static int attrlist_get_by_node( vldh_t_node node, void **a_ctx);
  static int attrlist_insert( void *a_ctx, WFoe *foe, vldh_t_node node);
  static int attrlist_delete( void *a_ctx);
  static void attr_quit( void *a_ctx);
  static void edit_exit_save( WFoe *foe);
  static void edit_exit_nosave( WFoe *foe);
  static void delete_subwindow_ok_cb( void *ctx, void *data);
  static void delete_ok_cb( void *ctx, void *data);
  
  static void exit_save( WFoe *foe);
  static void exit_nosave( WFoe *foe);
  static void deletewind_ok( WFoe *foe);

  static void gre_setup_window( WGre *gre);
  static void gre_node_created( WGre *gre, unsigned long current_node_type,
				float x, float y);
  static void gre_node_moved( WGre *gre);
  static void gre_node_selected( WGre *gre);
  static void gre_con_selected( WGre *gre);
  static void gre_con_created( WGre *gre, double x, double y,
			       vldh_t_node source_obj, unsigned long source_point,
			       vldh_t_node destination_obj, unsigned long destination_point, int select, int *sts);
  static void gre_region_selected( WGre *gre);
  static void gre_enter_leave( WGre *gre);
  static void gre_delete( WGre *gre, void *object, unsigned long object_type);
  static void gre_cut( WGre *gre, void *object, unsigned long object_type);
  static void gre_copy( WGre *gre, void *object, unsigned long object_type);
  static void gre_paste( WGre *gre, float x, float y);
  static void gre_attribute( WGre *gre, vldh_t_node object);
  static void gre_subwindow( WGre *gre, vldh_t_node object, unsigned long subwindow_nr);
  static void gre_popupmenu( WGre *gre, int x_pix, int y_pix, int popupmenu_type,
			     vldh_t_node node);
  static void gre_getobj( WGre *gre, vldh_t_node node, unsigned long index);
  static void gre_undelete( WGre *gre);
  static void gre_unselect( WGre *gre);
  static void gre_help( WGre *gre, char *help_title);
  static void gre_regionmoved( WGre *gre);
  static void gre_message( WGre *gre, const char *message);

  static void pal_select_cb( void *ctx, pwr_tCid cid);

};


#endif
