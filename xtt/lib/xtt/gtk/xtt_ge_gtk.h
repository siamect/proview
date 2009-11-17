/* 
 * Proview   $Id: xtt_ge_gtk.h,v 1.3 2008-10-31 12:51:36 claes Exp $
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

#ifndef xtt_ge_gtk_h
#define xtt_ge_gtk_h

#ifndef xtt_ge_h
# include "xtt_ge.h"
#endif

#ifndef cow_wow_gtk_h
# include "cow_wow_gtk.h"
#endif

class XttGeGtk : public XttGe {
 public:
  GtkWidget	*parent_wid;
  GtkWidget	*grow_widget;
  GtkWidget	*form_widget;
  GtkWidget	*toplevel;
  GtkWidget	*nav_shell;
  GtkWidget	*nav_widget;
  GtkWidget	*menu_widget;
  GtkWidget	*graph_form;
  GtkWidget	*value_input;
  GtkWidget	*value_dialog;
  GtkWidget	*confirm_widget;
  GtkWidget	*confirm_label;
  GtkWidget	*message_dia_widget;
  GtkWidget	*message_dia_label;
  CoWowFocusTimerGtk focustimer;

  XttGeGtk( GtkWidget *parent_wid, void *parent_ctx, const char *name, const char *filename,
	    int scrollbar, int menu, int navigator, int width, int height,
	    int x, int y, double scan_time, const char *object_name, int use_default_access,
	    unsigned int access,
	    int (*xg_command_cb) (XttGe *, char *),
	    int (*xg_get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
	    int (*xg_is_authorized_cb) (void *, unsigned int));
  ~XttGeGtk();

  void pop();
  void set_size( int width, int height);
  void create_confirm_dialog();

  static void ge_change_value_cb( void *ge_ctx, void *value_object, char *text);
  static void confirm_cb( void *ge_ctx, void *confirm_object, char *text);
  static void message_dialog_cb( void *ge_ctx, const char *text);

  static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
  static void activate_value_input( GtkWidget *w, gpointer data);
  static void activate_confirm_ok( GtkWidget *w, gpointer data);
  static void activate_confirm_cancel( GtkWidget *w, gpointer data);
  static void activate_exit( GtkWidget *w, gpointer data);
  static void activate_zoom_in( GtkWidget *w, gpointer data);
  static void activate_zoom_out( GtkWidget *w, gpointer data);
  static void activate_zoom_reset( GtkWidget *w, gpointer data);
  static void activate_help( GtkWidget *w, gpointer data);
  static void create_graph_form( GtkWidget *w, gpointer data);
  static void create_message_dia( GtkWidget *w, gpointer data);
  static void create_menu( GtkWidget *w, gpointer data);
  static void create_value_input( GtkWidget *w, gpointer data);
  static void action_resize( GtkWidget *w, GtkAllocation *allocation, gpointer data);

};

#endif






