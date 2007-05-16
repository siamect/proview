/* 
 * Proview   $Id: xtt_op_gtk.h,v 1.2 2007-05-16 12:37:39 claes Exp $
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

#ifndef xtt_op_gtk_h
#define xtt_op_gtk_h

/* xtt_op_gtk.h -- Operator window in xtt */

#ifndef xtt_op_h
# include "xtt_op.h"
#endif


class OpGtk : public Op {
 public:
  OpGtk( void *op_parent_ctx,
	 GtkWidget *op_parent_wid,
	 char *opplace,
	 pwr_tStatus *status);
  ~OpGtk();

  GtkWidget		*parent_wid;
  GtkWidget		*parent_wid_op;
  GtkWidget		*toplevel;
  GtkWidget		*alarmcnt_label;
  GtkWidget		*aalarm_label[5];
  GtkWidget		*aalarm_active[5];
  GtkWidget		*aalarm_box[5];
  GtkWidget		*balarm_label;
  GtkWidget		*balarm_active;
  GtkWidget		*balarm_box;
  GtkWidget		*balarm_ebox;
  GtkWidget		*balarm_mark;
  GtkWidget		*appl_form;
  GtkWidget		*eventlog_button;
  GtkWidget		*help_button;
  GtkWidget		*decr_button;
  GtkWidget		*tools_close;
  int			a_height;
  int			a_exist[5];
  int			a_active[5];
  int			text_size;

  void	map();
  int 	configure( char *opplace_str);
  void 	update_alarm_info();
  void  add_close_button();

  static void activate_exit( GtkWidget *w, gpointer data);
  static void activate_aalarm_ack( GtkWidget *w, gpointer data);
  static void activate_balarm_ack( GtkWidget *w, gpointer data);
  static void activate_aalarm_incr( GtkWidget *w, gpointer data);
  static void activate_aalarm_decr( GtkWidget *w, gpointer data);
  static void activate_zoom_in( GtkWidget *w, gpointer data);
  static void activate_zoom_out( GtkWidget *w, gpointer data);
  static void activate_alarmlist( GtkWidget *w, gpointer data);
  static void activate_eventlist( GtkWidget *w, gpointer data);
  static void activate_eventlog( GtkWidget *w, gpointer data);
  static void activate_navigator( GtkWidget *w, gpointer data);
  static void activate_help( GtkWidget *w, gpointer data);
  static void activate_appl1( GtkWidget *w, gpointer data);
  static void activate_appl2( GtkWidget *w, gpointer data);
  static void activate_appl3( GtkWidget *w, gpointer data);
  static void activate_appl4( GtkWidget *w, gpointer data);
  static void activate_appl5( GtkWidget *w, gpointer data);
  static void activate_appl6( GtkWidget *w, gpointer data);
  static void activate_appl7( GtkWidget *w, gpointer data);
  static void activate_appl8( GtkWidget *w, gpointer data);
  static void activate_appl9( GtkWidget *w, gpointer data);
  static void activate_appl10( GtkWidget *w, gpointer data);
  static void activate_appl11( GtkWidget *w, gpointer data);
  static void activate_appl12( GtkWidget *w, gpointer data);
  static void activate_appl13( GtkWidget *w, gpointer data);
  static void activate_appl14( GtkWidget *w, gpointer data);
  static void activate_appl15( GtkWidget *w, gpointer data);
};

#endif
