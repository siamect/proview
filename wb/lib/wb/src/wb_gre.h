/* 
 * Proview   $Id: wb_gre.h,v 1.9 2007-09-25 13:36:32 claes Exp $
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

#ifndef wb_gre_h
#define wb_gre_h

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef wb_vldh_h
#include "wb_vldh.h"
#endif

#define	GRE_PASTE_CENTER	0
#define	GRE_PASTE_CURSOR	1
#define	MAX_NUMBER_OF_OBJECTTYPES	300
#define	MAX_NUMBER_OF_CONTYPES		70
#define	MAX_SEL_NODE	500
#define	MAX_SEL_CON	500
#define	MAX_DEL_NODE	500
#define	MAX_DEL_CON	500

#define GRE_CON_CREATE	1
#define GRE_CON_REDRAW	2
#define GRE_CON_NONROUTE 4
#define GRE_CON_NODEREDRAW 8

#define	GRE_POPUPMENUMODE_OBJECT	0
#define	GRE_POPUPMENUMODE_AREA		1

#ifndef wb_goen_h
#include "wb_goen.h"
#endif

class WGre {
 public:
  void		*parent_ctx;
  flow_tCtx	flow_ctx;
  float		grid_size;
  unsigned long	gridobject;
  unsigned long	grid_on;
  vldh_t_wind	wind;
  unsigned long	conref_nodetypeid;  
  unsigned long	display_nodetypeid;  

  unsigned long	concreate_mask;
  unsigned long	nodemoved_mask;
  unsigned long	regionsel_mask;
  unsigned long	nodecreate_mask;
  unsigned long	nodecreate_click;
  unsigned long	nodesel_mask;
  unsigned long	nodesel_click;
  unsigned long	nodeseladd_mask;
  unsigned long	nodeseladd_click;
  unsigned long	delete_mask;
  unsigned long	delete_click;
  unsigned long	cut_mask;
  unsigned long	cut_click;
  unsigned long	copy_mask;
  unsigned long	copy_click;
  unsigned long	paste_mask;
  unsigned long	paste_click;
  unsigned long	attribute_mask;
  unsigned long	attribute_click;
  unsigned long	subwindow_mask;
  unsigned long	subwindow_click;
  unsigned long	reserv_mask;
  unsigned long	reserv_click;
  unsigned long	popupmenu_mask;
  unsigned long	popupmenu_click;
  unsigned long	getobj_mask;
  unsigned long	getobj_click;
  unsigned long	help_mask;
  unsigned long	help_click;
	
  flow_tNodeClass	nodeclass_list[MAX_NUMBER_OF_OBJECTTYPES][4];
  flow_tConClass	conclass_list[MAX_NUMBER_OF_CONTYPES];
  vldh_t_node		sel_node_list[MAX_SEL_NODE];
  int			sel_node_count;
  vldh_t_con		sel_con_list[MAX_SEL_CON];
  int			sel_con_count;
  vldh_t_node		del_node_list[MAX_DEL_NODE];
  int			del_node_count;
  vldh_t_con		del_con_list[MAX_DEL_CON];
  int			del_con_count;
  flow_tNode		searchrect_node_id;
  int			popupmenu_mode;
  int			trace_started;
  flow_tNodeClass	trace_analyse_nc;
  flow_tConClass	trace_con_cc;
  flow_tNode		trace_changenode;
  int			popup_menu_x;
  int			popup_menu_y;

  /* Callbacks */
  void (*gre_setup_window)(WGre *);
  void (*gre_node_created)(WGre *, unsigned long, float, float);
  void (*gre_con_created)(WGre *, double, double, vldh_t_node, unsigned long, vldh_t_node,  unsigned long);
  void (*gre_node_moved)(WGre *);
  void (*gre_delete)(WGre *, void *, unsigned long);
  void (*gre_cut)(WGre *, void *, unsigned long);
  void (*gre_copy)(WGre *, void *, unsigned long);
  void (*gre_paste)(WGre *, float, float);
  void (*gre_attribute)(WGre *, vldh_t_node);
  void (*gre_subwindow)(WGre *, vldh_t_node, unsigned long);
  void (*gre_reserv)();
  void (*gre_popupmenu)(WGre *, int, int, int, vldh_t_node);
  void (*gre_getobj)(WGre *, vldh_t_node, unsigned long);
  void (*gre_undelete)(WGre *);
  void (*gre_unselect)(WGre *);
  void (*gre_help)(WGre *, char *);
  void (*gre_regionmoved)(WGre *);
  void (*gre_message)(WGre *, char *);

  WGre( void *parent_ctx,
	char *name);
  virtual ~WGre();

  virtual void trace_start() {}
  virtual void trace_stop() {}
  virtual void get_popup_position( int *x, int *y) {}

  int node_sethighlight( vldh_t_node node, unsigned long highlight_flag);
  int node_gethighlight( vldh_t_node node, unsigned long *highlight_flag);
  void message( char *message);
  void ctx_init();
  void unselect();
  void search_rectangle_delete();
  void search_rectangle_create( vldh_t_node node);
  void create_cursors();
  int undelete();
  int undelete_reset();
  void delnode_insert( vldh_t_node node_id);
  void delcon_insert( vldh_t_con con_id);
  int delete_node( vldh_t_node node);
  void delete_con( vldh_t_con con);
  void delete_selected();
  int paste_node_exec( float x, float y);
  int cut();
  int cut_node( vldh_t_node node);
  int copy_node( vldh_t_node node);
  int copy();
  int window_draw();
  int get_selnodes( unsigned long *node_count, vldh_t_node **nodelist);
  int get_nodeclass( pwr_tClassId cid, ldh_tSesContext ldhses, unsigned long node_type,
		     unsigned int *mask, unsigned long subwindowmark,
		     unsigned long node_width, flow_tNodeClass *node_class,
		     vldh_t_node node);
  int init();
  int edit_setup();
  int view_setup();
  int setup_backcalls (
	void (*setup_window_bc)(WGre *),
	void (*node_created_bc)(WGre *, unsigned long, float, float),
	void (*con_created_bc)(WGre *, double, double, vldh_t_node, unsigned long, vldh_t_node,  unsigned long),
	void (*node_moved_bc)(WGre *),
	void (*delete_bc)(WGre *, void *, unsigned long),
	void (*cut_bc)(WGre *, void *, unsigned long),
	void (*copy_bc)(WGre *, void *, unsigned long),
	void (*paste_bc)(WGre *, float, float),
	void (*attribute_bc)(WGre *, vldh_t_node),
	void (*subwindow_bc)(WGre *, vldh_t_node, unsigned long),
	void (*reserv_bc)(),
	void (*popupmenu_bc)(WGre *, int, int, int, vldh_t_node),
	void (*getobj_bc)(WGre *, vldh_t_node, unsigned long),
	void (*undelete_bc)(WGre *),
	void (*unselect_bc)(WGre *),
	void (*help_bc)(WGre *, char *),
	void (*regionmoved_bc)(WGre *),
	void (*message_bc)(WGre *, char *));
  int create_node( pwr_tClassId cid, float x, float y, vldh_t_node *node);
  int create_con( pwr_tClassId  cid,
		  vldh_t_node source_obj, unsigned long source_point,
		  vldh_t_node destination_obj, unsigned long destination_point,
		  unsigned long drawtype);
  int print_docobj( vldh_t_node doc_obj);
  int measure_object( vldh_t_node node, float *ll_x, float *ll_y,
		      float *width, float *height);
  int print_rectangle( float ll_x, float ll_y, float ur_x, float ur_y,
		       char *file_id);
  int print_pdf_rectangle( float ll_x, float ll_y, float ur_x, float ur_y,
			   char *file_id);
  void paste( float cursor_x, float cursor_y, int paste_type);
  int subwindow_mark( vldh_t_node object);
  pwr_tStatus node_update_points( vldh_t_node node,
				  unsigned long old_input_mask, 
				  unsigned long old_output_mask,
				  unsigned long new_input_mask,
				  unsigned long new_output_mask,
				  int *point_array, int *point_count);
  int node_update( vldh_t_node object);
  void zoom( float zoom);
  void unzoom();
  int zoom_absolute( float absolute_zoom, int expand, float *realized_zoom);
  int display();
  int set_display_value( vldh_t_node node, char *string);
  int undisplay();
  int node_select( vldh_t_node node);
  int node_unselect( vldh_t_node node);
  int center_node( vldh_t_node node);
  int init_docobjects();
  int redraw();
  int set_trace_attributes( char *host);
  int save( char *filename);
  void set_grid_size( double size);
  void disable_button_events();
  int get_conclass( pwr_tClassId cid, ldh_tSesContext ldhses, unsigned long con_type,
		    flow_tConClass *con_class);
  void pixel_to_position( int pix_x, int pix_y, double *x, double *y);

  void select_nextobject( flow_eDirection dir);

  static int get_annot_width( flow_tNodeClass nodeclass,
			      float *annot_width, char *annot_str, int annot_count,
			      int annot_size);
  static int get_annotations( vldh_t_node node, char *annot_str,
			      int *annot_nr, int *annot_count, int annot_max,
			      int annot_size);
  static int modif_annot( vldh_t_node node, int annotation, char *str);
  static int node_annotations_draw( vldh_t_node node, int paste);
  static int node_annot_message( vldh_t_node node, char *message, int msg_size,
				 int annot_max_size);
  static int flow_cb( FlowCtx *ctx, flow_tEvent event);
  static int init_flow( FlowCtx *ctx, void *client_data);
};

#endif
