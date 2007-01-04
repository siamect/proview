/* 
 * Proview   $Id: xtt_op_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_op_motif_h
#define xtt_op_motif_h

/* xtt_op_motif.h -- Operator window in xtt */

#ifndef xtt_op_h
# include "xtt_op.h"
#endif


class OpMotif : public Op {
 public:
  OpMotif( void *op_parent_ctx,
	   Widget op_parent_wid,
	   char *opplace,
	   pwr_tStatus *status);
  ~OpMotif();

  Widget		parent_wid;
  Widget		parent_wid_op;
  Widget		toplevel;
  Widget		alarmcnt_label;
  Widget		aalarm_label[5];
  Widget		aalarm_active[5];
  Widget		balarm_label;
  Widget		balarm_active;
  Widget		balarm_mark;
  Widget		appl_form;

  void	map();
  int 	configure( char *opplace_str);
  void 	update_alarm_info();

  static void activate_exit( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_aalarm_ack( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_balarm_ack( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_aalarm_incr( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_aalarm_decr( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_alarmlist( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_eventlist( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_eventlog( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_navigator( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_help( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl1( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl2( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl3( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl4( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl5( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl6( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl7( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl8( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl9( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl10( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl11( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl12( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl13( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl14( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void activate_appl15( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_alarmcnt_label( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_label1( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_label2( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_label3( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_label4( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_label5( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_active1( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_active2( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_active3( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_active4( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_aalarm_active5( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_balarm_label( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_balarm_mark( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_balarm_active( Widget w, Op *op, XmAnyCallbackStruct *data);
  static void create_appl_form( Widget w, Op *op, XmAnyCallbackStruct *data);
};

#endif
