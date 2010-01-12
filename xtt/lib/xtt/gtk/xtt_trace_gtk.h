/* 
 * Proview   $Id: rt_trace_gtk.h,v 1.2 2007-12-03 14:51:39 claes Exp $
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

#ifndef xtt_trace_gtk_h
#define xtt_trace_gtk_h

#ifndef xtt_trace_h
#include "xtt_trace.h"
#endif

class RtTraceGtk : public RtTrace {
 public:
  GtkWidget		*parent_wid;
  GtkWidget		*flow_widget;
  GtkWidget		*toplevel;
  GtkWidget		*form;
  GtkWidget		*menu;
  GtkWidget		*nav_shell;
  GtkWidget		*nav_widget;

  RtTraceGtk( void *parent_ctx, GtkWidget *parent_wid, pwr_tObjid objid,
	      pwr_tStatus *status);
  ~RtTraceGtk();

  void pop();
  void popup_menu_position( int event_x, int event_y, int *x, int *y);
  RtTrace *subwindow_new( void *ctx, pwr_tObjid oid, pwr_tStatus *sts);

  static void activate_close(GtkWidget *w, gpointer data);
 private:
  static void activate_print(GtkWidget *w, gpointer data);
  static void activate_printselect(GtkWidget *w, gpointer data);
  static void activate_savetrace(GtkWidget *w, gpointer data);
  static void activate_restoretrace(GtkWidget *w, gpointer data);
  static void activate_cleartrace(GtkWidget *w, gpointer data);
  static void activate_display_object(GtkWidget *w, gpointer data);
  static void activate_collect_insert(GtkWidget *w, gpointer data);
  static void activate_open_object(GtkWidget *w, gpointer data);
  static void activate_open_subwindow(GtkWidget *w, gpointer data);
  static void activate_show_cross(GtkWidget *w, gpointer data);
  static void activate_open_classgraph(GtkWidget *w, gpointer data);
  static void activate_trace(GtkWidget *w, gpointer data);
  static void activate_simulate(GtkWidget *w, gpointer data);
  static void activate_view(GtkWidget *w, gpointer data);
  static void activate_zoomin(GtkWidget *w, gpointer data);
  static void activate_zoomout(GtkWidget *w, gpointer data);
  static void activate_zoomreset(GtkWidget *w, gpointer data);
  static void activate_scantime1(GtkWidget *w, gpointer data);
  static void activate_scantime2(GtkWidget *w, gpointer data);
  static void activate_scantime3(GtkWidget *w, gpointer data);
  static void activate_scantime4(GtkWidget *w, gpointer data);
  static void activate_scantime5(GtkWidget *w, gpointer data);
  static void activate_help(GtkWidget *w, gpointer data);
  static void activate_helpplc(GtkWidget *w, gpointer data);

};
#endif






