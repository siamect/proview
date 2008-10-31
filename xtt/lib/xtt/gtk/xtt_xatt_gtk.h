/* 
 * Proview   $Id: xtt_xatt_gtk.h,v 1.3 2008-10-31 12:51:36 claes Exp $
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

#ifndef xtt_xatt_gtk_h
#define xtt_xatt_gtk_h

/* xtt_xatt_gtk.h -- Object attribute editor */

#ifndef xtt_xatt_h
# include "xtt_xatt.h"
#endif

#ifndef co_wow_gtk_h
# include "co_wow_gtk.h"
#endif



class XAttGtk : public XAtt {
 public:
  XAttGtk ( 
	GtkWidget      	*xa_parent_wid, 
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	int             *xa_sts);
    ~XAttGtk();
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*msg_label;
    GtkWidget	*cmd_prompt;
    GtkWidget	*cmd_input;
    GtkWidget	*cmd_scrolledinput;
    GtkWidget	*cmd_scrolledtextview;
    GtkWidget	*cmd_scrolled_ok;
    GtkWidget	*cmd_scrolled_ca;
    GtkTextBuffer *cmd_scrolled_buffer;
    GtkWidget	*pane;
    static CoWowRecall value_recall;
    CoWowEntryGtk *cmd_entry;
    CoWowFocusTimerGtk focustimer;
    int input_max_length;

    void message( char severity, const char *message);
    void set_prompt( const char *prompt);
    void change_value( int set_focus);
    int open_changevalue( char *name);
    void change_value_close();
    void pop();

    static void activate_change_value( GtkWidget *w, gpointer data);
    static void activate_close_changeval( GtkWidget *w, gpointer data);
    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_display_object( GtkWidget *w, gpointer data);
    static void activate_show_cross( GtkWidget *w, gpointer data);
    static void activate_open_classgraph( GtkWidget *w, gpointer data);
    static void activate_open_plc( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void valchanged_cmd_input( GtkWidget *w, gpointer data);
    static void activate_cmd_input( GtkWidget *w, gpointer data);
    static void activate_cmd_scrolled_ok( GtkWidget *w, gpointer data);
    static void activate_cmd_scrolled_ca( GtkWidget *w, gpointer data);
    static void action_text_inserted( GtkTextBuffer *w, GtkTextIter *iter, gchar *str, gint len, gpointer data);
    
};

#endif







