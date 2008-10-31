/* 
 * Proview   $Id: wb_watt_motif.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_watt_motif_h
#define wb_watt_motif_h

/* wb_watt_motif.h -- Object attribute editor */

#ifndef wb_watt_h
# include "wb_watt.h"
#endif


class WAttMotif : public WAtt {
 public:
  WAttMotif( 
	Widget 		wa_parent_wid, 
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses,
	pwr_sAttrRef 	wa_aref,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname);
  ~WAttMotif();
  Widget	parent_wid;
  Widget	brow_widget;
  Widget	form_widget;
  Widget	toplevel;
  Widget	msg_label;
  Widget	cmd_prompt;
  Widget	cmd_input;
  Widget	cmd_scrolledinput;
  Widget	cmd_scrolled_ok;
  Widget	cmd_scrolled_ca;
  Widget	wattnav_form;
  int		set_focus_disabled;
  XtIntervalId focus_timerid;
  static char	value_recall[30][160];
  int		value_current_recall;

  void message( char severity, const char *message);
  void set_prompt( const char *prompt);
  void change_value( int set_focus);
  int open_changevalue( char *name);
  void change_value_close();
  void pop();

  static void activate_change_value( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void activate_close_changeval( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void activate_exit( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void activate_help( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void create_msg_label( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void create_cmd_prompt( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void create_cmd_input( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void create_cmd_scrolledinput( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void create_cmd_scrolled_ok( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void create_cmd_scrolled_ca( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void create_wattnav_form( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
  static void valchanged_cmd_input( Widget w, XEvent *event);
  static void activate_cmd_input( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void activate_cmd_scrolled_ok( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
  static void activate_cmd_scrolled_ca( Widget w, WAttMotif *watt, XmAnyCallbackStruct *data);
};

#endif







