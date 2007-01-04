/* 
 * Proview   $Id: xtt_ge.h,v 1.11 2007-01-04 08:22:47 claes Exp $
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

#ifndef xtt_ge_h
#define xtt_ge_h

#ifndef pwr_h
# include "pwr.h"
#endif
#include "glow.h"

class Graph;

class XttGe {
 public:
  void 		*parent_ctx;
  pwr_tAName   	name;
  Graph		*graph;
  pwr_tFileName	filename;
  int		scrollbar;
  int		navigator;
  int		menu;
  void		*current_value_object;
  void		*current_confirm_object;
  int		value_input_open;
  int		confirm_open;
  int		(*command_cb)(XttGe *, char *);
  void		(*close_cb)(XttGe *);
  void		(*help_cb)(XttGe *, char *key);
  void		(*display_in_xnav_cb)(void *, pwr_sAttrRef *);
  int		(*is_authorized_cb)(void *, unsigned int);
  void          (*popup_menu_cb)(void *, pwr_sAttrRef, unsigned long,
					 unsigned long, char *, int x, int y); 
  int         	(*call_method_cb)(void *, char *, char *, pwr_sAttrRef,
				  unsigned long, unsigned long, char *);
  int           (*get_current_objects_cb)(void *, pwr_sAttrRef **, int **);
  int           (*sound_cb)(void *, pwr_tAttrRef *);
  int		width;
  int		height;

  XttGe( void *parent_ctx, char *name, char *filename,
	 int scrollbar, int menu, int navigator, int width, int height,
	 int x, int y, double scan_time, char *object_name, int use_default_access,
	 unsigned int access,
	 int (*xg_command_cb) (XttGe *, char *),
	 int (*xg_get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
	 int (*xg_is_authorized_cb) (void *, unsigned int));
  virtual ~XttGe();

  virtual void pop() {}
  virtual void set_size( int width, int height) {}

  void message( char severity, char *msg);
  void print();
  int set_object_focus( char *name, int empty);
  int set_folder_index( char *name, int idx);
  int set_subwindow_source( char *name, char *source);
  void swap( int mode);

  static void graph_init_cb( void *client_data);
  static void graph_close_cb( void *client_data);
  static int ge_command_cb( void *ge_ctx, char *command);
  static int ge_sound_cb( void *ge_ctx, pwr_tAttrRef *aref);
  static void ge_display_in_xnav_cb( void *ge_ctx, pwr_sAttrRef *arp);
  static void ge_popup_menu_cb( void *ge_ctx, pwr_sAttrRef attrref,
			     unsigned long item_type, unsigned long utility, 
			     char *arg, int x, int y);
  static int ge_call_method_cb( void *ge_ctx, char *method, char *filter,
			     pwr_sAttrRef attrref, unsigned long item_type, 
			     unsigned long utility, char *arg);
  static int ge_is_authorized_cb( void *ge_ctx, unsigned int access);
  static int ge_get_current_objects_cb( void *ge_ctx, pwr_sAttrRef **alist,
				     int **is_alist);
  static void message_cb( void *ctx, char severity, char *msg);
};

#endif






