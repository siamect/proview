/* 
 * Proview   $Id: xtt_ev.h,v 1.9 2008-01-24 09:36:17 claes Exp $
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

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef xtt_evlist
# include "xtt_evlist.h"
#endif

typedef struct {
  char Object[120];
  char Alias[8];
} ev_sAlias;

class Ev {
  public:
    Ev(
	void *ev_parent_ctx,
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
    virtual ~Ev();

    void 		*parent_ctx;
    char 		name[80];
    pwr_tObjid		user;
    int			eve_display_ack;
    int			eve_display_return;
    void 		(*start_trace_cb)( void *, pwr_tObjid, char *);
    void 		(*display_in_xnav_cb)( void *, pwr_tAttrRef *);
    void 		(*update_info_cb)( void *);
    void 		(*help_cb)( void *, char *);
    void 		(*popup_menu_cb)( void *, pwr_tAttrRef, unsigned long,
					  unsigned long, char *, int x, int y);
    int			(*sound_cb)( void *, pwr_tAttrRef *);
    void       		(*pop_cb)( void *);
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
    ev_sAlias		alias_list[20];
    static Ev		*ev;

    virtual void map_eve() {}
    virtual void map_ala() {}
    virtual void map_blk() {}
    virtual void unmap_eve() {}
    virtual void unmap_ala() {}
    virtual void unmap_blk() {}

    int		outunit_connect( pwr_tObjid	user);
    void	update( double scantime);
    int         is_mapped_eve() { return eve_displayed;};
    int         is_mapped_ala() { return ala_displayed;};
    int         is_mapped_blk() { return blk_displayed;};
    int		get_alarm_info( evlist_sAlarmInfo *info);
    void 	ack_last_prio( unsigned long type, unsigned long prio);
    int		get_last_not_acked_prio( mh_sEventId **id, unsigned long type, 
			unsigned long prio);
    void	create_aliaslist( void *up);
    char	*name_to_alias( char *name);

    void eve_activate_print();
    void ala_activate_print();
    void blk_activate_print();
    void eve_activate_ack_last();
    void eve_activate_help();
    void eve_activate_helpevent();
    void ala_activate_help();
    void ala_activate_helpevent();
    void blk_activate_help();

    static void eve_display_in_xnav_cb( void *ctx, pwr_tAttrRef *arp);
    static void ala_display_in_xnav_cb( void *ctx, pwr_tAttrRef *arp);
    static void blk_display_in_xnav_cb( void *ctx, pwr_tAttrRef *arp);
    static char *ev_name_to_alias_cb( void *ctx, char *name);
    static void eve_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);
    static void ala_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);
    static void blk_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);
    static void ev_popup_menu_cb( void *ctx, pwr_tAttrRef attrref,
				  unsigned long item_type, unsigned long utility,
				  char *arg, int x, int y);
    static int ev_sound_cb( void *ctx, pwr_tAttrRef *attrref);
    static pwr_tStatus mh_ack_bc( mh_sAck *MsgP);
    static pwr_tStatus mh_return_bc( mh_sReturn *MsgP);
    static pwr_tStatus mh_alarm_bc( mh_sMessage *MsgP);
    static pwr_tStatus mh_block_bc( mh_sBlock *MsgP);
    static pwr_tStatus mh_cancel_bc( mh_sReturn *MsgP);
    static pwr_tStatus mh_info_bc( mh_sMessage *MsgP);
    static pwr_tStatus mh_clear_alarmlist_bc( pwr_tNodeIndex nix);
    static pwr_tStatus mh_clear_blocklist_bc( pwr_tNodeIndex nix);

};

#endif



