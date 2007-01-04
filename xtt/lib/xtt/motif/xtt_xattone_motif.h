/* 
 * Proview   $Id: xtt_xattone_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_xattone_motif_h
#define xtt_xattone_motif_h

/* xtt_xattone_motif.h -- Single attribute editor */

#ifndef xtt_xattone_h
# include "xtt_xattone.h"
#endif


class XAttOneMotif : public XAttOne {
 public:
  XAttOneMotif( Widget xa_parent_wid, 
		void *xa_parent_ctx, 
		pwr_sAttrRef *xa_objar,
		char *xa_title,
		unsigned int xa_priv,
		int *xa_sts);
  ~XAttOneMotif();

  Widget	parent_wid;
    Widget	form_widget;
    Widget	toplevel;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_label;
    Widget	cmd_input;
    Widget	cmd_scrolledinput;
    Widget	cmd_scrolled_ok;
    Widget	cmd_scrolled_ap;
    Widget	cmd_scrolled_ca;
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    static char	value_recall[30][160];
    int	value_current_recall;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    int change_value( int set_focus);
    int open_changevalue( char *name);
    void change_value_close();
    void pop();
    void swap( int mode);
    int set_value();

    static void activate_exit( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void create_msg_label( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void create_cmd_prompt( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void create_cmd_label( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void create_cmd_input( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void create_cmd_scrolledinput( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void create_cmd_scrolled_ok( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void create_cmd_scrolled_ap( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void create_cmd_scrolled_ca( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void enable_set_focus( XAttOneMotif *xattone);
    static void disable_set_focus( XAttOneMotif *xattone, int time);
    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
    static void valchanged_cmd_input( Widget w, XEvent *event);
    static void activate_cmd_input( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void activate_cmd_scrolled_ok( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void activate_cmd_scrolled_ap( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);
    static void activate_cmd_scrolled_ca( Widget w, XAttOne *xattone, XmAnyCallbackStruct *data);

};

#endif







