/** 
 * Proview   $Id: co_wow_motif.h,v 1.4 2008-12-01 16:42:35 claes Exp $
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

#ifndef co_wow_motif_h
#define co_wow_motif_h

/* co_wow_motif.h -- useful windows */

#include "co_wow.h"

#include <Xm/Xm.h>
#include <X11/Xatom.h>

//#ifndef co_wow_api_h
typedef struct {
  Widget    toplevel;
  Widget    list;
  char      *texts;
  int	    textsize;
  void      (* action_cb) ( void *, char *);
  void      (* cancel_cb) ( void *);
  void      *parent_ctx;
} *wow_tListCtx;

typedef struct {
  void			*ctx;
  void			*data;
  void	         	(* questionbox_ok) ( void *, void *);
  void	         	(* questionbox_cancel) ( void *, void *);
  void	         	(* questionbox_help) ( void *, void *);
} wow_t_question_cb;

//#endif

class CoWowWidgetMotif : public CoWowWidget {
  CoWowWidgetMotif( Widget w) : widget(w) {}
  Widget widget;
};

class CoWowFocusTimerMotif {
 public:
  CoWowFocusTimerMotif() : 
    set_focus_disabled(0), request_cnt(0), focus_timerid(0) {}
  ~CoWowFocusTimerMotif();
  void disable( Widget w, int time);
  int disabled();    

 private:
  int		set_focus_disabled;
  int		request_cnt;
  XtIntervalId 	focus_timerid;

  static void enable_set_focus( void *ft, XtIntervalId *id);
};

class CoWowTimerMotif : public CoWowTimer {
 public:
  XtIntervalId m_timerid;
  Widget m_w;

  CoWowTimerMotif( Widget w) : m_timerid(0), m_w(w) {}
  ~CoWowTimerMotif();
  void add( int time, void (* callback)(void *data), void *data);
  void remove();
  static void timer_cb( void *data);
};

class CoWowMotif : public CoWow {
 public:
  Widget m_parent;

  CoWowMotif( Widget parent) : m_parent(parent) {}
  ~CoWowMotif() {}
  void DisplayQuestion( void *ctx, const char *title, const char *text,
				void (* questionbox_ok) ( void *, void *),
				void (* questionbox_cancel) ( void *, void *),
				void *data);
  void DisplayError( const char *title, const char *text);
  void DisplayText( const char *title, const char *text) {}
  void *CreateList( const char *title, const char *texts,
		    void (action_cb)( void *, char *),
		    void (cancel_cb)( void *),
		    void *ctx,
		    int show_apply_button = 0);
  void CreateFileSelDia( const char *title, void *parent_ctx,
	       void (*file_selected_cb)(void *, char *, wow_eFileSelType),
	       wow_eFileSelType file_type);
  int DisplayWarranty();
  void DisplayLicense();  
  CoWowTimer *timer_new();

  static void GetAtoms( Widget w, Atom *graph_atom, Atom *objid_atom, Atom *attrref_atom);
  static int GetSelection( Widget w, char *str, int size, Atom atom);
  static void PopupPosition( Widget parent, int x_event, int y_event, int *x, int *y);

  static void error_ok_cb(Widget w);
  static void question_ok_cb( Widget dialog, XtPointer data, 
			      XmAnyCallbackStruct *cbs);
  static void question_cancel_cb ( Widget dialog, XtPointer data, 
				   XmAnyCallbackStruct *cbs);
  static void question_help_cb( Widget dialog,XtPointer data, 
				XmAnyCallbackStruct *cbs);
  static void list_ok_cb( Widget w, XtPointer data, 
			  XmAnyCallbackStruct *cbs);
  static void list_cancel_cb( Widget w, XtPointer data, 
			      XmAnyCallbackStruct *cbs);
  static void list_action_cb( Widget w, XtPointer data,
			      XmListCallbackStruct *cbs);
  static void file_ok_cb( Widget widget, XtPointer udata, XtPointer data);
  static void file_cancel_cb( Widget widget, XtPointer udata, XtPointer data);
  static void file_search_cb( Widget widget, XtPointer data);
  static void warranty_ok_cb( void *ctx, void *data);
  static void warranty_cancel_cb( void *ctx, void *data);
  static void warranty_help_cb( void *ctx, void *data);

};

#endif

