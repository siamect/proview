/* 
 * Proview   $Id: rt_xtt_motif.h,v 1.2 2008-10-31 12:51:32 claes Exp $
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

#ifndef rt_xtt_motif_h
#define rt_xtt_motif_h

/* rt_xtt_motif.h -- Simple navigator */


#ifndef rt_xtt_main_h
# include "rt_xtt_main.h"
#endif

#ifndef cow_wow_motif_h
#include "cow_wow_motif.h"
#endif

#ifndef xtt_hotkey_motif_h
#include "xtt_hotkey_motif.h"
#endif

class XttMotif : public Xtt {
  public:
    Widget	parent_wid;
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	xnav_form;
    Widget	india_widget;
    Widget	india_label;
    Widget	india_text;
    CoWowFocusTimerMotif focustimer;
    hotkeyHandle   	HotkeyHandle;
    char	cmd_recall[30][160];
    int		cmd_current_recall;
    char        value_recall[30][160];
    int		value_current_recall;

    XttMotif( int argc, char *argv[], int *sts);
    ~XttMotif();

    void message( char severity, const char *msg);
    void xtt_mainloop (XtAppContext AppCtx);
    int init_hotkey( XtAppContext AppCtx, Widget Top);
    void open_input_dialog( const char *text, const char *title,
			    const char *init_text,
			    void (*ok_cb)( Xtt *, char *));
    void set_prompt( const char *prompt);
    void open_change_value();

    static void close( void *ctx, int terminate);
    static void set_dimension( void *ctx, int width, int height);
    static void map( void *ctx);
 
    static void activate_change_value( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_command( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_exit( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_print( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_find( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_findregex( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_findnext( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_collapse( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_openobject( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_openplc( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_showcrossref( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_opengraph( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_collect_insert( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_collect_show( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_collect_remove( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_collect_clear( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_advanceduser( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_zoom_in( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_zoom_out( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_zoom_reset( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_help_project( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_help_proview( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void create_msg_label( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void create_cmd_prompt( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void create_cmd_input( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void create_xnav_form( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void create_india_label( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void create_india_text( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_india_ok( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void activate_india_cancel( Widget w, Xtt *xtt, XmAnyCallbackStruct *data);
    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
    static void valchanged_cmd_input( Widget w, XEvent *event, XmAnyCallbackStruct *data);
  
    static void hotkey_Command( Widget w, XKeyEvent* ev, String* av, Cardinal* ac);
    static void hotkey_ToggleDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac);
    static void hotkey_SetDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac);
    static void hotkey_ResetDig( Widget w, XKeyEvent* ev, String* av, Cardinal* ac);
};

#endif




















