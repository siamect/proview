/* 
 * Proview   $Id: wb_wda_motif.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_wda_motif_h
#define wb_wda_motif_h

/* wb_wda_motif.h -- Spreadsheet editor */

#ifndef wb_wda_h
# include "wb_wda.h"
#endif

class WdaMotif : public Wda {
  public:
    WdaMotif( 
	Widget 		wa_parent_wid, 
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses,
	pwr_tObjid 	wa_objid,
	pwr_tClassId 	wa_classid,
        char            *wa_attribute,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname);
    ~WdaMotif();
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
    Widget	wdanav_form;
    Widget      wdaclass_dia;
    Widget      wdaclass_hiervalue;
    Widget      wdaclass_classvalue;
    Widget      wdaclass_attrobjects;
    Widget      wdaattr_dia;
    XtIntervalId focus_timerid;
    int		set_focus_disabled;
    static char	value_recall[30][160];
    int	value_current_recall;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    void change_value( int set_focus);
    void change_value_close();
    void pop();
    void open_class_dialog( char *hierstr, char *classstr);

    static void enable_set_focus( WdaMotif *wda);
    static void disable_set_focus( WdaMotif *wda, int time);
    static void activate_change_value( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_close_changeval( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_exit( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_print( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_setclass( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_setattr( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_nextattr( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_prevattr( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_cmd_input( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void create_msg_label( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void create_cmd_prompt( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void create_cmd_input( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void create_cmd_scrolledinput( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void create_cmd_scrolled_ok( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void create_cmd_scrolled_ca( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void create_wdanav_form( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void class_create_hiervalue( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void class_create_classvalue( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void class_create_attrobjects( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void class_activate_ok( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void class_activate_cancel( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
    static void valchanged_cmd_input( Widget w, XEvent *event);
    static void activate_cmd_scrolled_ok( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
    static void activate_cmd_scrolled_ca( Widget w, WdaMotif *wda, XmAnyCallbackStruct *data);
};


#endif







