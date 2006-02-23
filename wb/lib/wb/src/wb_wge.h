/* 
 * Proview   $Id: wb_wge.h,v 1.1 2006-02-23 14:47:36 claes Exp $
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

#ifndef wb_wge_h
#define wb_wge_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

class Graph;

class WGe {
 public:
  void 		*parent_ctx;
  Widget	parent_wid;
  pwr_tAName   	name;
  Widget	grow_widget;
  Widget	form_widget;
  Widget	toplevel;
  Widget	nav_shell;
  Widget	nav_widget;
  Widget	menu_widget;
  Graph		*graph;
  Widget	graph_form;
  pwr_tFileName	filename;
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
  int		(*command_cb)(void *, char *);
  void		(*close_cb)(void *);
  void		(*help_cb)(void *, char *key);
  int		(*is_authorized_cb)(void *, unsigned int);
  int		width;
  int		height;
  int		set_focus_disabled;
  XtIntervalId 	focus_timerid;
  int		modal;
  int 		terminated;
  int		subwindow_release;

  void pop();
  void print();
  int set_object_focus( char *name, int empty);
  int set_folder_index( char *name, int idx);
  int set_subwindow_source( char *name, char *source, int modal);
  void set_subwindow_release();

  WGe( Widget parent_wid, void *parent_ctx, char *name, char *filename,
       int scrollbar, int menu, int navigator, int width, int height, 
       int x, int y, char *object_name, int modal);
  ~WGe();
};

#if defined __cplusplus
}
#endif
#endif






