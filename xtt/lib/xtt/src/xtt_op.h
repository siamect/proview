/* 
 * Proview   $Id: xtt_op.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#if defined __cplusplus
}
#endif

#ifndef xtt_jop_h
# include "xtt_jop.h"
#endif

#ifndef xtt_evlist_h
# include "xtt_evlist.h"
#endif

class Op {
  public:
    Op(
	void *op_parent_ctx,
	Widget	op_parent_wid,
	char *opplace,
	pwr_tStatus *status);
    ~Op();

    void 		*parent_ctx;
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
    unsigned long	balarm_prio;
    unsigned long	balarm_type;
    char		button_title[20][80];
    pwr_tObjid		button_objid[20];
    int			button_cnt;
    int			start_jop;
    Jop			*jop;
    void 		(*command_cb)( void *, char *);
    void 		(*map_cb)( void *);
    void 		(*help_cb)( void *, char *);
    void 		(*close_cb)( void *);
    int 		(*get_alarm_info_cb)( void *, evlist_sAlarmInfo *);
    void 		(*ack_last_cb)( void *, unsigned long, unsigned long);

    void	map();
    void	scan();
    int 	configure( char *opplace_str);
    int 	appl_action( int idx);
    void	set_jop_qid( int qix) { if ( jop) jop->set_jop_qid( qix);};
    int 	jop_command( char *command);
    void 	update_alarm_info();
};

#endif
