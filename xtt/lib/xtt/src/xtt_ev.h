#ifndef xtt_ev_h
#define xtt_ev_h

/* xtt_ev.h -- Alarm and event windows in xtt

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#if defined __cplusplus
}
#endif

#ifndef xtt_evlist
# include "xtt_evlist.h"
#endif

#ifndef xtt_focustimer_h
# include "xtt_focustimer.h"
#endif

class Ev {
  public:
    Ev(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	char *eve_name,
	char *ala_name,
	pwr_tObjid ev_user,
	int display_ala,
	int display_eve,
	int display_return,
	int display_ack,
	int ev_beep,
	pwr_tStatus *status);
    ~Ev();

    void 		*parent_ctx;
    Widget		parent_wid;
    Widget		parent_wid_eve;
    Widget		parent_wid_ala;
    char 		name[80];
    Widget		toplevel_ala;
    Widget		toplevel_eve;
    Widget		form_ala;
    Widget		form_eve;
    Widget		eve_widget;
    Widget		ala_widget;
    pwr_tObjid		user;
    int			eve_display_ack;
    int			eve_display_return;
    void 		(*start_trace_cb)( void *, pwr_tObjid, char *);
    void 		(*display_in_xnav_cb)( void *, pwr_sAttrRef *);
    void 		(*update_info_cb)( void *);
    void 		(*help_cb)( void *, char *);
    void 		(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
					  unsigned long, char *, Widget * );
    EvList		*eve;
    EvList		*ala;
    int			connected;
    int			ala_displayed;
    int			eve_displayed;
    int			beep;
    int			eve_size;
    int			ala_size;
    FocusTimer		eve_focustimer;
    FocusTimer		ala_focustimer;

    int		outunit_connect( pwr_tObjid	user);
    void	update( double scantime);
    void	map_eve();
    void	map_ala();
    void	unmap_eve();
    void	unmap_ala();
    int         is_mapped_eve() { return eve_displayed;};
    int         is_mapped_ala() { return ala_displayed;};
    int		get_alarm_info( evlist_sAlarmInfo *info);
    void 	ack_last_prio( unsigned long type, unsigned long prio);
    int		get_last_not_acked_prio( mh_sEventId **id, unsigned long type, 
			unsigned long prio);
};

#endif



