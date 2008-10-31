/* 
 * Proview   $Id: wb_wge_motif.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_wge_motif_h
#define wb_wge_motif_h

#ifndef wb_wge_h
# include "wb_wge.h"
#endif

class WGeMotif : public WGe{
 public:
  Widget	parent_wid;
  Widget	grow_widget;
  Widget	form_widget;
  Widget	toplevel;
  Widget	graph_form;
  Widget	nav_shell;
  Widget	nav_widget;
  Widget	menu_widget;
  Widget	value_input;
  Widget	value_dialog;
  Widget	confirm_widget;
  Widget	message_dia_widget;
  int		set_focus_disabled;
  XtIntervalId 	focus_timerid;

  void pop();
  void set_size( int width, int height);

  WGeMotif( Widget parent_wid, void *parent_ctx, char *name, char *filename,
       int scrollbar, int menu, int navigator, int width, int height, 
       int x, int y, char *object_name, int modal);
  ~WGeMotif();

  static void enable_set_focus( WGeMotif *ge);
  static void disable_set_focus( WGeMotif *ge, int time);
  static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
  static void change_value_cb( void *ge_ctx, void *value_object, char *text);
  static void confirm_cb( void *ge_ctx, void *confirm_object, char *text);
  static void message_dialog_cb( void *ge_ctx, const char *text);
  static void activate_value_input( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void activate_confirm_ok( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void activate_confirm_cancel( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void activate_exit( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void activate_zoom_in( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void activate_zoom_out( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void activate_zoom_reset( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void activate_help( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void create_graph_form( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void create_message_dia( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void create_menu( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void create_value_input( Widget w, WGe *ge, XmAnyCallbackStruct *data);
  static void action_resize( Widget w, XmAnyCallbackStruct *data);

  
};

#endif






