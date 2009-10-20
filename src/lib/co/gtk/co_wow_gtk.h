/** 
 * Proview   $Id: co_wow_gtk.h,v 1.8 2008-12-01 16:42:35 claes Exp $
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

#ifndef co_wow_gtk_h
#define co_wow_gtk_h

/* co_wow_gtk.h -- useful windows */

#include "co_wow.h"

#include <gtk/gtk.h>


typedef struct {
  void			*ctx;
  void			*data;
  void	         	(* questionbox_ok) ( void *, void *);
  void	         	(* questionbox_cancel) ( void *, void *);
  void	         	(* questionbox_help) ( void *, void *);
  GtkWidget 		*question_widget;
} wow_t_question_cb;

class CoWowEntryGtk {
 public:
  CoWowRecall	*m_re;
  GtkWidget	*w;
  bool		m_re_alloc;
  
  CoWowEntryGtk( CoWowRecall *re = 0);
  ~CoWowEntryGtk();
  GtkWidget *widget() { return w;}
  void set_recall_buffer( CoWowRecall *re) { 
    if ( !m_re_alloc) 
      m_re = re;
  }
  static gboolean event_cb( GtkWidget *w, GdkEvent *event, gpointer data);
};

class CoWowWidgetGtk : public CoWowWidget {
  CoWowWidgetGtk( GtkWidget *w) : widget(w) {}
  GtkWidget *widget;  
};

class CoWowFocusTimerGtk {
 public:
  CoWowFocusTimerGtk() : 
    set_focus_disabled(0), request_cnt(0), focus_timerid(0) {}
  ~CoWowFocusTimerGtk();
  void disable( int time);
  int disabled();    

 private:
  int		set_focus_disabled;
  int		request_cnt;
  gint 		focus_timerid;

  static gboolean enable_set_focus( void *ft);
};

class CoWowTimerGtk : public CoWowTimer {
 public:
  gint m_timerid;

  CoWowTimerGtk() : m_timerid(0) {}
  ~CoWowTimerGtk();
  void add( int time, void (* callback)(void *data), void *data);
  void remove();
  static gboolean timer_cb( void *data);
};

class CoWowGtk : public CoWow {
 public:
  GtkWidget *m_parent;

  CoWowGtk( GtkWidget *parent) : m_parent(parent) {}
  ~CoWowGtk() {}
  void DisplayQuestion( void *ctx, const char *title, const char *text,
			void (* questionbox_ok) ( void *, void *),
			void (* questionbox_cancel) ( void *, void *),
			void *data);
  void DisplayError( const char *title, const char *text);
  void DisplayText( const char *title, const char *text);
  void *CreateList( const char *title, const char *texts, int textsize,
		    void (action_cb)( void *, char *),
		    void (cancel_cb)( void *),
		    void *ctx,
		    int show_apply_button = 0);
  void CreateFileSelDia( const char *title, void *parent_ctx,
			 void (*file_selected_cb)(void *, char *, wow_eFileSelType),
			 wow_eFileSelType file_type);
  int CreateModalDialog( const char *title, const char *text, const char *button1, const char *button2,
			 const char *button3, const char *image);
  wow_sModalInputDialog *CreateModalInputDialog( const char *title, const char *text, 
						 const char *button1, const char *button2, 
						 const char *button3, const char *image,
						 int input_length);
  int DisplayWarranty();
  void DisplayLicense();
  pwr_tStatus CreateMenuItem( const char *name, void *menu, int pixmap, int append, void *w);
  pwr_tStatus DeleteMenuItem( const char *name, void *menu);
  CoWowTimer *timer_new();
  void SetParent( GtkWidget *parent) { m_parent = parent;}

  static void PopupPosition( GtkWidget *parent, int x_event, int y_event, int *x, int *y);
  static void GetAtoms( GdkAtom *graph_atom, GdkAtom *objid_atom, GdkAtom *attrref_atom);
  static int GetSelection( GtkWidget *w, char *str, int size, GdkAtom atom);
  static void SetWindowIcon( GtkWidget *w);
  static void question_ok_cb( GtkWidget *w, gpointer data);
  static void question_cancel_cb( GtkWidget *w, gpointer data);
  static void warranty_cb( GtkWidget *w, gint response, gpointer data);
  static void license_cb( GtkWidget *w, gint response, gpointer data);
  static char *translate_utf8( const char *str);
  static char *convert_utf8( const char *str);

#if 0
  static void error_ok_cb(Widget w);
  static void question_help_cb( Widget dialog,XtPointer data, 
				XmAnyCallbackStruct *cbs);
#endif
  static void list_row_activated_cb( GtkTreeView *tree_view, 
				     GtkTreePath *path,
				     GtkTreeViewColumn *column,
				     gpointer data);
  static void list_apply_cb( GtkWidget *w, gpointer data);
  static void list_ok_cb( GtkWidget *w, gpointer data);
  static void list_cancel_cb( GtkWidget *w, gpointer data);
};

#endif

