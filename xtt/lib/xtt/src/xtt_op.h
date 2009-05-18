/* 
 * Proview   $Id: xtt_op.h,v 1.5 2008-10-31 12:51:36 claes Exp $
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

#ifndef xtt_op_h
#define xtt_op_h

/* xtt_op.h -- Operator window in xtt */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#ifndef xtt_jop_h
# include "xtt_jop.h"
#endif

#ifndef xtt_evlist_h
# include "xtt_evlist.h"
#endif

class CoWow;

class Op {
 public:
  void 		*parent_ctx;
  unsigned long	balarm_prio;
  unsigned long	balarm_type;
  char		button_title[20][80];
  pwr_tAttrRef  button_aref[20];
  int	       	button_cnt;
  int	       	start_jop;
  Jop	       	*jop;
  void 		(*command_cb)( void *, char *);
  void 		(*map_cb)( void *);
  void 		(*help_cb)( void *, const char *);
  void 		(*close_cb)( void *);
  int 		(*get_alarm_info_cb)( void *, evlist_sAlarmInfo *);
  void 		(*ack_last_cb)( void *, unsigned long, unsigned long);
  CoWow		*wow;

  Op( void *op_parent_ctx,
      char *opplace,
      pwr_tStatus *status);
  virtual ~Op();

  virtual void	map() {}
  virtual int 	configure( char *opplace_str) {return 0;}
  virtual void 	update_alarm_info() {}
  virtual void  add_close_button() {}

  void	set_jop_qid( int qix) { if ( jop) jop->set_jop_qid( qix);};
  void	scan();
  int 	appl_action( int idx);
  int 	jop_command( char *command);
  void activate_exit();
  void activate_aalarm_ack();
  void activate_balarm_ack();
  void activate_alarmlist();
  void activate_eventlist();
  void activate_eventlog();
  void activate_navigator();
  void activate_help();

  static void jop_command_cb( void *op, char *command);
};

#endif
