/* 
 * Proview   $Id: wb_wda_gtk.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_wda_gtk_h
#define wb_wda_gtk_h

/* wb_wda_gtk.h -- Spreadsheet editor */

#ifndef wb_wda_h
# include "wb_wda.h"
#endif

#ifndef co_wow_gtk_h
# include "co_wow_gtk.h"
#endif

class WdaGtk : public Wda {
  public:
    WdaGtk( 
	GtkWidget      	*wa_parent_wid, 
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses,
	pwr_tObjid 	wa_objid,
	pwr_tClassId 	wa_classid,
        const char      *wa_attribute,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname);
    ~WdaGtk();
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*msg_label;
    GtkWidget	*cmd_prompt;
    GtkWidget	*cmd_input;
    GtkWidget	*cmd_scrolledinput;
    GtkWidget	*cmd_scrolled_ok;
    GtkWidget	*cmd_scrolled_ca;
    GtkWidget	*cmd_scrolledtextview;
    GtkTextBuffer *cmd_scrolled_buffer;
    GtkWidget	*pane;
    GtkWidget      *wdaclass_dia;
    GtkWidget      *wdaclass_hiervalue;
    GtkWidget      *wdaclass_classvalue;
    GtkWidget      *wdaclass_attrobjects;
    GtkWidget      *wdaattr_dia;
    static CoWowRecall value_recall;
    CoWowEntryGtk  *cmd_entry;
    CoWowFocusTimerGtk focustimer;

    void message( char severity, const char *message);
    void set_prompt( const char *prompt);
    void change_value( int set_focus);
    void change_value_close();
    void pop();
    void open_class_dialog( char *hierstr, char *classstr);
    void create_class_dialog();

    static void activate_change_value( GtkWidget *w, gpointer data);
    static void activate_close_changeval( GtkWidget *w, gpointer data);
    static void activate_exit( GtkWidget *w, gpointer data);
    static void activate_print( GtkWidget *w, gpointer data);
    static void activate_setclass( GtkWidget *w, gpointer data);
    static void activate_setattr( GtkWidget *w, gpointer data);
    static void activate_nextattr( GtkWidget *w, gpointer data);
    static void activate_prevattr( GtkWidget *w, gpointer data);
    static void activate_help( GtkWidget *w, gpointer data);
    static void activate_cmd_input( GtkWidget *w, gpointer data);
    static gboolean action_inputfocus( GtkWidget *w, GdkEvent *event, gpointer data);
    static void valchanged_cmd_input( GtkWidget *w, gpointer data);
    static void activate_cmd_scrolled_ok( GtkWidget *w, gpointer data);
    static void activate_cmd_scrolled_ca( GtkWidget *w, gpointer data);
    static void class_activate_ok( GtkWidget *w, gpointer data);
    static void class_activate_cancel( GtkWidget *w, gpointer data);
};


#endif







