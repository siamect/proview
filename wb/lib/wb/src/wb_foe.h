/* 
 * Proview   $Id: wb_foe.h,v 1.9 2005-09-06 10:43:31 claes Exp $
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

#ifndef _XtIntrinsic_h 
#include <X11/Intrinsic.h>
#endif

#ifndef wb_foe_macros_h
#include "wb_foe_macros.h"
#endif

#ifndef wb_h
#include "wb.h"
#endif

#ifndef wb_api_h
#include "wb_api.h"
#endif

#ifndef wb_goen_h
#include "wb_goen.h"
#endif

#ifndef wb_gre_h
#include "wb_gre.h"
#endif

typedef struct s_foe_ctx t_foe_ctx;
typedef struct s_foe_ctx *foe_ctx;

#ifndef wb_tra_h
#include "wb_tra.h"
#endif

#define TRACE 0
#define SIMULATE 1
#define VIEW 2
#define EDIT 3

typedef enum
{
	foe_eFuncAccess_View		= 1 << 0,
	foe_eFuncAccess_Edit		= 1 << 1
} foe_eFuncAccess;

#define FOE_ACCESSTYPE_RW 0
#define FOE_ACCESSTYPE_RO 1

enum foe_e_popupmenu
{
	foe_e_popupmenu_attribute	= 1 << 0,
	foe_e_popupmenu_subwindow	= 1 << 1,
	foe_e_popupmenu_connect		= 1 << 2,
	foe_e_popupmenu_delete		= 1 << 3,
	foe_e_popupmenu_paste		= 1 << 4,
	foe_e_popupmenu_copy		= 1 << 5,
	foe_e_popupmenu_cut		= 1 << 6,
	foe_e_popupmenu_printselect	= 1 << 7,
	foe_e_popupmenu_helpclass	= 1 << 8
};

/* Foedit info structure  */

struct s_widgets 
{
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
  Widget	trace_togg;       /* togg under the function
                                     entry of the menu */
  Widget	simulate_togg;    /* togg under the function entry
                                     of the menu */
  Widget	view_togg;        /* togg under the function entry
                                     of the menu */
  Widget	edit_togg;        /* togg under the function entry
                                     of the menu */
  Widget	con_palette;
  Widget	node_palette;
  Widget	nav_palette;
} ;

struct s_foe_ctx
{
  wb_eUtility	ctx_type;
  t_commonpart	cp;

  char		name[80];
  pal_tCtx	node_palctx;
  pal_tCtx	con_palctx;
  nav_tCtx	navctx;
  void		*tractx;
  gre_ctx	grectx;

  voidp         popupmsg_yes_p;
  voidp         popupmsg_no_p; 
  voidp         popupmsg_cancel_p;
  int           msg_label_id;
  int 		function; 		/* TRACE or EDIT or ... */
  int		wanted_function;	
  vldh_t_plc	plcobject;
  int		con_palette_managed;
  int		node_palette_managed;
  int		nav_palette_managed;
  int		con_drawtype;
  int		show_executeorder;
  char		searchstring[80];
  int		searchindex;
  int		(*textinput_func)();
  unsigned long	popupmenu_mask;
  vldh_t_node	popupmenu_node;
  ldh_eAccess	access;
  int		map_window;
  int		advanced_user;
  int		ldh_cb_enabled;
  struct 	s_widgets widgets ;
  int		set_focus_disabled;
  XtIntervalId focus_timerid;
  Atom		objid_atom;
  int		classeditor;
};

/* Methods */
/*----------------------------------------------------------------------
 * Create a new foedit object
 */
int foe_new(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
	pwr_tObjDId	plcprogram,
	ldh_tWBContext 	ldhwbctx,
	ldh_tSesContext	ldhsesctx,
	foe_ctx		*return_foectx,
	int		map_window,
	ldh_eAccess	access
);


int foe_new_local(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
	pwr_tObjDId	plcprogram,
	ldh_tWBContext 	ldhwbctx,
	ldh_tSesContext	ldhsesctx,
	vldh_t_node	nodeobject,
	unsigned long	windowindex,
	unsigned long	new_window,
	foe_ctx		*return_foectx, 
	int		map_window,
	ldh_eAccess	access,
	foe_eFuncAccess	function_access
);


void    foe_delete(
        foe_ctx foectx
);

int	foe_search_string_next ( 
	foe_ctx foectx 
);

int	foe_search_object ( 
	foe_ctx foectx,
	char	*searchstr
);

int	foe_center_object ( 
	foe_ctx foectx,
	pwr_tOid oid
);

int foe_attr_create (
	foe_ctx	foectx,
	vldh_t_node	node
);

int foe_attr_delete (
	foe_ctx	foectx
);

void	foe_quit(
	foe_ctx	foectx
);

void	foe_exit(
	foe_ctx	foectx
);

int foe_subwindow_create (
	foe_ctx		foectx,
	vldh_t_node	object,
	unsigned long	subwindow_nr
);

void foe_message ( 
	foe_ctx	foectx,
	char *new_label 
);

void foe_msgbox ( 
	foe_ctx	foectx,
	char *new_label
);

void foe_popupmsg ( 	
	foe_ctx	foectx,
	char *new_label,
	voidp yes_procedure,
	voidp no_procedure,
	voidp cancel_procedure
);

void foe_display_help   ( 
	foe_ctx foectx,
	char *topic_title
);

int	foe_show_executeorder ( 
	foe_ctx foectx
);

int	foe_get_textinput ( 
	foe_ctx foectx,
	char	*message,
	int	(*function)()
);

int	foe_search_string ( 
	foe_ctx foectx,
	char	*searchstr
);

int foe_redraw_and_save( 
	foe_ctx foectx
);

int foe_modify_popup (
	foe_ctx foectx,
	unsigned long	popupmenu_mask
);

int foe_print_overview (
	foe_ctx foectx
);

int foe_print_document( 
	foe_ctx foectx
);

int foe_print_selected_document( 
	foe_ctx foectx
);

int foe_change_mode(
	foe_ctx foectx,
	int	new_mode
);


void foe_get_hinactx(
	foe_ctx	foectx,
	void **hinactx
);

void error_msg(
	unsigned long 	sts
);

void info_msg (
	unsigned long 	sts
);

void foe_enable_ldh_cb( foe_ctx foectx);
void foe_disable_ldh_cb( foe_ctx foectx);

int	foe_attrlist_get_by_node(
	vldh_t_node	node,
	void	**a_ctx
);


#endif
