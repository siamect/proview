/* 
 * Proview   $Id: rt_xtt_gtk.h,v 1.2 2008-06-24 08:05:25 claes Exp $
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

#ifndef rt_xtt_gtk_h
#define rt_xtt_gtk_h

/* rt_xtt_gtk.h -- Simple navigator */

typedef void *Widget;

#ifndef rt_xtt_main_h
# include "rt_xtt_main.h"
#endif

#ifndef co_wow_gtk_h
# include "co_wow_gtk.h"
#endif

class CoWowRecall;
class CoWowEntryGtk;
class XttHotkey;

class XttGtk : public Xtt {
  public:
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*msg_label;
    GtkWidget	*cmd_prompt;
    GtkWidget	*cmd_input;
    GtkWidget	*xnav_form;
    GtkWidget	*india_widget;
    GtkWidget	*india_label;
    GtkWidget	*india_text;
    CoWowFocusTimerGtk focustimer;
    CoWowRecall *cmd_recall;
    CoWowRecall *value_recall;
    CoWowEntryGtk *cmd_entry;
    XttHotkey	*hotkey;

    XttGtk( int argc, char *argv[], int *sts);
    ~XttGtk();

    void message( char severity, char *msg);
    void xtt_mainloop();
    // int init_hotkey( XtAppContext AppCtx, Widget Top);
    void open_input_dialog( char *text, char *title,
			    char *init_text,
			    void (*ok_cb)( Xtt *, char *));
    void set_prompt( char *prompt);
    void open_change_value();
    void create_input_dialog();

    static void close( void *ctx, int terminate);
    static void set_dimension( void *ctx, int width, int height);
    static void map( void *ctx);
 
    static void activate_change_value( GtkWidget *w, gpointer data);
    static void activate_command( GtkWidget *w, gpointer data);
    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_print( GtkWidget *w, gpointer data);
    static void activate_login( GtkWidget *w, gpointer data);
    static void activate_logout( GtkWidget *w, gpointer data);
    static void activate_find( GtkWidget *w, gpointer data);
    static void activate_findregex( GtkWidget *w, gpointer data);
    static void activate_findnext( GtkWidget *w, gpointer data);
    static void activate_collapse( GtkWidget *w, gpointer data);
    static void activate_openobject( GtkWidget *w, gpointer data);
    static void activate_openplc( GtkWidget *w, gpointer data);
    static void activate_showcrossref( GtkWidget *w, gpointer data);
    static void activate_opengraph( GtkWidget *w, gpointer data);
    static void activate_collect_insert( GtkWidget *w, gpointer data);
    static void activate_collect_show( GtkWidget *w, gpointer data);
    static void activate_collect_remove( GtkWidget *w, gpointer data);
    static void activate_collect_clear( GtkWidget *w, gpointer data);
    static void activate_advanceduser( GtkWidget *w, gpointer data);
    static void activate_zoom_in( GtkWidget *w, gpointer data);
    static void activate_zoom_out( GtkWidget *w, gpointer data);
    static void activate_zoom_reset( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static void activate_help_project( GtkWidget *w, gpointer data);
    static void activate_help_proview( GtkWidget *w, gpointer data);
    static void activate_back( GtkWidget *w, gpointer data);
    static void create_msg_label( GtkWidget *w, gpointer data);
    static void create_cmd_prompt( GtkWidget *w, gpointer data);
    static void create_cmd_input( GtkWidget *w, gpointer data);
    static void create_xnav_form( GtkWidget *w, gpointer data);
    static void create_india_label( GtkWidget *w, gpointer data);
    static void create_india_text( GtkWidget *w, gpointer data);
    static void activate_india_ok( GtkWidget *w, gpointer data);
    static void activate_india_cancel( GtkWidget *w, gpointer data);
    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void valchanged_cmd_input( GtkWidget *w, gpointer data);
  
    static void hotkey_Command( char *arg, void *userdata);
    static void hotkey_ToggleDig( char *arg, void *userdata);
    static void hotkey_SetDig( char *arg, void *userdata);
    static void hotkey_ResetDig( char *arg, void *userdata);
};

#endif




















