/* 
 * Proview   $Id: xtt_ge.h,v 1.7 2005-09-01 14:57:48 claes Exp $
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
 */

#ifndef rt_ge_h
#define rt_ge_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

typedef struct ge_sCtx {
  void 		*parent_ctx;
  Widget	parent_wid;
  char 		name[80];
  Widget	grow_widget;
  Widget	form_widget;
  Widget	toplevel;
  Widget	nav_shell;
  Widget	nav_widget;
  Widget	menu_widget;
  void		*graph;
  Widget	graph_form;
  char		filename[120];
  int		scrollbar;
  int		navigator;
  int		menu;
  void		*current_value_object;
  void		*current_confirm_object;
  int		value_input_open;
  int		confirm_open;
  Widget	value_input;
  Widget	value_dialog;
  Widget	confirm_widget;
  Widget	message_dia_widget;
  int		(*command_cb)(struct ge_sCtx *, char *);
  void		(*close_cb)(struct ge_sCtx *);
  void		(*help_cb)(struct ge_sCtx *, char *key);
  void		(*display_in_xnav_cb)(void *, pwr_sAttrRef *);
  int		(*is_authorized_cb)(void *, unsigned int);
  void          (*popup_menu_cb)(void *, pwr_sAttrRef, unsigned long,
					 unsigned long, char *, Widget *); 
  int         	(*call_method_cb)(void *, char *, char *, pwr_sAttrRef,
				  unsigned long, unsigned long, char *);
  int           (*get_current_objects_cb)(void *, pwr_sAttrRef **, int **);
  int		width;
  int		height;
  int		set_focus_disabled;
  XtIntervalId 	focus_timerid;
} *ge_tCtx;

void ge_pop( ge_tCtx gectx);
void ge_print( ge_tCtx gectx);
int ge_set_object_focus( ge_tCtx gectx, char *name, int empty);
int ge_set_folder_index( ge_tCtx gectx, char *name, int idx);
void ge_swap( ge_tCtx gectx, int mode);

extern "C" ge_tCtx ge_new( Widget parent_wid,
  void			*parent_ctx,
  char 			*name,
  char 			*filename,
  int			scrollbar,
  int			menu,
  int			navigator,
  int			width,
  int			height,
  int			x,
  int			y,
  double		scan_time,
  char			*object_name,
  int                   use_default_access,
  unsigned int          access,
  void			(*close_cb) (ge_tCtx),
  void			(*help_cb) (ge_tCtx, char *),
  int                   (*command_cb) (ge_tCtx, char *),
  int                   (*get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
  int                   (*is_authorized_cb) (void *, unsigned int)
);
extern "C" void ge_delete( ge_tCtx gectx);

#if defined __cplusplus
}
#endif
#endif






