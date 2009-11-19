/* 
 * Proview   $Id$
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

#ifndef xtt_trace_motif_h
#define xtt_trace_motif_h

#ifndef xtt_trace_h
#include "xtt_trace.h"
#endif

class RtTraceMotif : public RtTrace {
 public:
  Widget		parent_wid;
  Widget		flow_widget;
  Widget		toplevel;
  Widget		form;
  Widget		menu;
  Widget		nav_shell;
  Widget		nav_widget;

  RtTraceMotif( void *parent_ctx, Widget parent_wid, pwr_tObjid objid,
	    pwr_tStatus *status);
  ~RtTraceMotif();

  void pop();
  void popup_menu_position( int event_x, int event_y, int *x, int *y);
  RtTrace *subwindow_new( void *ctx, pwr_tObjid oid, pwr_tStatus *sts);

 private:
  static void create_form( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void create_menu( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_close( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_print( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_printselect( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_savetrace( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_restoretrace( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_cleartrace( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_display_object( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_collect_insert( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_open_object( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_show_cross( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_open_classgraph( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_trace( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_simulate( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_view( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_zoomin( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_zoomout( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_zoomreset( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_scantime1( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_scantime2( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_scantime3( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_scantime4( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_scantime5( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_help( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);
  static void activate_helpplc( Widget w, RtTrace *tractx, XmAnyCallbackStruct *data);



};

#if 0
			void 		(*close_cb)(tra_tCtx),
			void 		(*help_cb)(tra_tCtx, char *),
			void 		(*subwindow_cb)(void *, pwr_tObjid),
			void 		(*display_object_cb)(void *, pwr_tObjid),
			void 		(*collect_insert_cb)(void *, pwr_tObjid),
			int             (*is_authorized_cb)(void *, unsigned int) 
#endif

#endif






