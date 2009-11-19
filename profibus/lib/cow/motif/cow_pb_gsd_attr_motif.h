/* 
 * Proview   $Id$
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

#ifndef cow_pb_gsd_attr_motif_h
#define cow_pb_gsd_attr_motif_h

/* cow_pb_gsd_attr_motif.h -- Profibus gsd attribute editor */

#include "cow_pb_gsd_attr.h"

class GsdAttrMotif : public GsdAttr {
 public:
  Widget	parent_wid;
  Widget	brow_widget;
  Widget	form_widget;
  Widget	toplevel;
  Widget	msg_label;
  Widget	cmd_prompt;
  Widget	cmd_input;
  Widget	attrnav_form;
  Widget	cmd_ok;
  Widget	cmd_cancel;
  Widget	menubutton_copy;
  Widget	menubutton_cut;
  Widget	menubutton_paste;
  Widget	menubutton_changevalue;

  GsdAttrMotif( Widget a_parent_wid,
		void *a_parent_ctx,
		void *a_object,
		pb_gsd *a_gsd,
		int a_edit_mode);
  ~GsdAttrMotif();

  void message( char severity, const char *message);
  void set_prompt( const char *prompt);
  void change_value();
    
  static void gsdattr_message( void *attr, char severity, char *message);
  static void gsdattr_change_value_cb( void *attr_ctx);
  static void activate_change_value( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_exit( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_help( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_copy( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_cut( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_paste( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_zoom_in( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_zoom_out( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_zoom_reset( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_print( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void activate_cmd_ok( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void cmd_close_apply_cb( void *ctx, void *data);
  static void cmd_close_no_cb( void *ctx, void *data);
  static void activate_cmd_ca( Widget w, GsdAttr *attr, XmAnyCallbackStruct *dataxo);
  static void create_menubutton( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void create_msg_label( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void create_cmd_prompt( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void create_cmd_ok( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void create_cmd_ca( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void create_cmd_input( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void create_attrnav_form( Widget w, GsdAttr *attr, XmAnyCallbackStruct *data);
  static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
  static void valchanged_cmd_input( Widget w, XEvent *event);
};

#endif








