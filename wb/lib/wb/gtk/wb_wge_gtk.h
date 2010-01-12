/* 
 * Proview   $Id: wb_wge_gtk.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_wge_gtk_h
#define wb_wge_gtk_h

#ifndef wb_wge_h
# include "wb_wge.h"
#endif

#ifndef cow_wow_gtk_h
# include "cow_wow_gtk.h"
#endif

class WGeGtk : public WGe{
 public:
  GtkWidget	*parent_wid;
  GtkWidget	*grow_widget;
  GtkWidget	*form_widget;
  GtkWidget	*toplevel;
  GtkWidget	*graph_form;
  GtkWidget	*nav_shell;
  GtkWidget	*nav_widget;
  GtkWidget	*menu_widget;
  GtkWidget	*value_input;
  GtkWidget	*value_dialog;
  GtkWidget	*confirm_widget;
  GtkWidget	*confirm_label;
  GtkWidget	*message_dia_widget;
  GtkWidget	*message_dia_label;
  CoWowFocusTimerGtk focustimer;

  void pop();
  void set_size( int width, int height);
  void set_subwindow_release();
  void create_confirm_dialog();
  
  WGeGtk( GtkWidget *parent_wid, void *parent_ctx, char *name, char *filename,
       int scrollbar, int menu, int navigator, int width, int height, 
       int x, int y, char *object_name, int modal);
  ~WGeGtk();

  static void enable_set_focus( WGeGtk *ge);
  static void disable_set_focus( WGeGtk *ge, int time);
  static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
  static void change_value_cb( void *ge_ctx, void *value_object, char *text);
  static void confirm_cb( void *ge_ctx, void *confirm_object, char *text);
  static void message_dialog_cb( void *ge_ctx, const char *text);
  static void activate_value_input(GtkWidget *w, gpointer data);
  static void activate_confirm_ok(GtkWidget *w, gpointer data);
  static void activate_confirm_cancel(GtkWidget *w, gpointer data);
  static void activate_exit(GtkWidget *w, gpointer data);
  static void activate_zoom_in(GtkWidget *w, gpointer data);
  static void activate_zoom_out(GtkWidget *w, gpointer data);
  static void activate_zoom_reset(GtkWidget *w, gpointer data);
  static void activate_help(GtkWidget *w, gpointer data);
  static void create_graph_form(GtkWidget *w, gpointer data);
  static void create_message_dia(GtkWidget *w, gpointer data);
  static void create_menu(GtkWidget *w, gpointer data);
  static void create_value_input(GtkWidget *w, gpointer data);
  static void action_resize( GtkWidget *w, GtkAllocation *allocation, gpointer data);

  
};

#endif






