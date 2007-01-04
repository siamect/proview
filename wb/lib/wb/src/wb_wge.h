/* 
 * Proview   $Id: wb_wge.h,v 1.2 2007-01-04 07:29:04 claes Exp $
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

#ifndef pwr_h
# include "pwr.h"
#endif

class Graph;

class WGe {
 public:
  void 		*parent_ctx;
  pwr_tAName   	name;
  Graph   	*graph;
  pwr_tFileName	filename;
  int		scrollbar;
  int		navigator;
  int		menu;
  void		*current_value_object;
  void		*current_confirm_object;
  int		value_input_open;
  int		confirm_open;
  int		(*command_cb)(void *, char *);
  void		(*close_cb)(void *);
  void		(*help_cb)(void *, char *key);
  int		(*is_authorized_cb)(void *, unsigned int);
  int		width;
  int		height;
  int		modal;
  int 		terminated;
  int		subwindow_release;

  WGe( void *parent_ctx, char *name, char *filename,
       int scrollbar, int menu, int navigator, int width, int height, 
       int x, int y, char *object_name, int modal);
  virtual ~WGe();

  virtual void pop() {}
  virtual void set_size( int width, int height) {}
  virtual void set_subwindow_release();

  int set_object_focus( char *name, int empty);
  int set_folder_index( char *name, int idx);
  int set_subwindow_source( char *name, char *source, int modal);
  void print();

  static void graph_init_cb( void *client_data);
  static void graph_close_cb( void *client_data);
  static int wge_command_cb( void *ge_ctx, char *command);
  static int wge_is_authorized_cb( void *ge_ctx, unsigned int access);
  static void message( void *ctx, char severity, char *message);

  
};

#endif






