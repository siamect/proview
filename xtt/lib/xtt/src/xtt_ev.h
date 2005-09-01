/* 
 * Proview   $Id: xtt_ev.h,v 1.6 2005-09-01 14:57:48 claes Exp $
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

#ifndef xtt_ev_h
#define xtt_ev_h

/* xtt_ev.h -- Alarm and event windows in xtt */

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

typedef struct {
  char Object[120];
  char Alias[8];
} ev_sAlias;

class Ev {
  public:
    Ev(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	char *eve_name,
	char *ala_name,
	char *blk_name,
	pwr_tObjid ev_user,
	int display_ala,
	int display_eve,
	int display_blk,
	int display_return,
	int display_ack,
	int ev_beep,
	pwr_tStatus *status);
    ~Ev();

    void 		*parent_ctx;
    Widget		parent_wid;
    Widget		parent_wid_eve;
    Widget		parent_wid_ala;
    Widget		parent_wid_blk;
    char 		name[80];
    Widget		toplevel_ala;
    Widget		toplevel_eve;
    Widget		toplevel_blk;
    Widget		form_ala;
    Widget		form_eve;
    Widget		form_blk;
    Widget		eve_widget;
    Widget		ala_widget;
    Widget		blk_widget;
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
    EvList		*blk;
    int			connected;
    int			ala_displayed;
    int			eve_displayed;
    int			blk_displayed;
    int			beep;
    int			eve_size;
    int			ala_size;
    int			blk_size;
    FocusTimer		eve_focustimer;
    FocusTimer		ala_focustimer;
    FocusTimer		blk_focustimer;
    ev_sAlias		alias_list[20];

    int		outunit_connect( pwr_tObjid	user);
    void	update( double scantime);
    void	map_eve();
    void	map_ala();
    void	map_blk();
    void	unmap_eve();
    void	unmap_ala();
    void	unmap_blk();
    int         is_mapped_eve() { return eve_displayed;};
    int         is_mapped_ala() { return ala_displayed;};
    int         is_mapped_blk() { return blk_displayed;};
    int		get_alarm_info( evlist_sAlarmInfo *info);
    void 	ack_last_prio( unsigned long type, unsigned long prio);
    int		get_last_not_acked_prio( mh_sEventId **id, unsigned long type, 
			unsigned long prio);
    void	create_aliaslist( void *up);
    char	*name_to_alias( char *name);
};

#endif



