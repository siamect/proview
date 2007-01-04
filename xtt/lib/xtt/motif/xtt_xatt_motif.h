/* 
 * Proview   $Id: xtt_xatt_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_xatt_motif_h
#define xtt_xatt_motif_h

/* xtt_xatt_motif.h -- Object attribute editor */

#ifndef xtt_xatt_h
# include "xtt_xatt.h"
#endif



class XAttMotif : public XAtt {
 public:
  XAttMotif ( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	int             *xa_sts);
    ~XAttMotif();
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
    Widget	xattnav_form;
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    static char	value_recall[30][160];
    int	value_current_recall;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    void change_value( int set_focus);
    int open_changevalue( char *name);
    void change_value_close();
    void pop();

    static void activate_change_value( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_close_changeval( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_exit( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_display_object( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_show_cross( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_open_classgraph( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_open_plc( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void create_msg_label( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void create_cmd_prompt( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void create_cmd_input( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void create_cmd_scrolledinput( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void create_cmd_scrolled_ok( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void create_cmd_scrolled_ca( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void create_xattnav_form( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void enable_set_focus( XAttMotif *xatt);
    static void disable_set_focus( XAttMotif *xatt, int time);
    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
    static void valchanged_cmd_input( Widget w, XEvent *event);
    static void activate_cmd_input( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_cmd_scrolled_ok( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    static void activate_cmd_scrolled_ca( Widget w, XAtt *xatt, XmAnyCallbackStruct *data);
    
};

#endif







