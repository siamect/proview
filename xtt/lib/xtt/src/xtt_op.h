#ifndef xtt_op_h
#define xtt_op_h

/* xtt_op.h -- Operator window in xtt

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
