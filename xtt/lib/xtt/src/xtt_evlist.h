#ifndef xtt_evlist_h
#define xtt_evlist_h

/* xtt_evlist.h -- Alarm and event windows in xtt

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


// Status is defined as int i xlib...
#ifdef Status
# undef Status
#endif

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif
#ifndef rt_mh_h
# include "rt_mh.h"
#endif
#ifndef rt_mh_outunit_h
# include "rt_mh_outunit.h"
#endif

#if defined __cplusplus
}
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browctx.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

#define ALARM_INFO_A_SIZE  5
#define ALARM_INFO_B_SIZE  2
#define ALARM_INFO_C_SIZE  2
#define ALARM_INFO_D_SIZE  2
#define ALARM_INFO_I_SIZE  2
typedef struct {
	char	a_alarm_text[ALARM_INFO_A_SIZE][80];
	int	a_alarm_active[ALARM_INFO_A_SIZE];
	int	a_alarm_exist[ALARM_INFO_A_SIZE];
	char	b_alarm_text[ALARM_INFO_B_SIZE][80];
	int	b_alarm_active[ALARM_INFO_B_SIZE];
	int	b_alarm_exist[ALARM_INFO_B_SIZE];
	char	c_alarm_text[ALARM_INFO_C_SIZE][80];
	int	c_alarm_active[ALARM_INFO_C_SIZE];
	int	c_alarm_exist[ALARM_INFO_C_SIZE];
	char	d_alarm_text[ALARM_INFO_D_SIZE][80];
	int	d_alarm_active[ALARM_INFO_D_SIZE];
	int	d_alarm_exist[ALARM_INFO_D_SIZE];
	char	i_alarm_text[ALARM_INFO_I_SIZE][80];
	int	i_alarm_active[ALARM_INFO_I_SIZE];
	int	i_alarm_exist[ALARM_INFO_I_SIZE];
	int	alarms_total;
	int	alarms_notacked;
	} evlist_sAlarmInfo;

typedef enum {
	ev_eType_AlarmList,
	ev_eType_EventList,
	ev_eType_HistList,
	ev_eType_BlockList
	} ev_eType;

typedef enum {
	evlist_eEventType_Info,
	evlist_eEventType_Return,
	evlist_eEventType_Ack,
	evlist_eEventType_Alarm,
	evlist_eEventType_Block,
	evlist_eEventType_Cancel
	} evlist_eEventType;

typedef enum {
	evlist_eItemType_Alarm
	} evlist_eItemType;

class EvListBrow {
  public:
    EvListBrow( BrowCtx *brow_ctx, void *evl) : ctx(brow_ctx), evlist(evl) {};
    ~EvListBrow();

    BrowCtx		*ctx;
    void		*evlist;
    brow_tNodeClass 	nc_event;
    brow_tNodeClass 	nc_a_alarm;
    brow_tNodeClass 	nc_b_alarm;
    brow_tNodeClass 	nc_info;
    flow_sAnnotPixmap 	*pixmap_leaf;
    flow_sAnnotPixmap 	*pixmap_map;
    flow_sAnnotPixmap 	*pixmap_openmap;
    flow_sAnnotPixmap 	*pixmap_attr;
    flow_sAnnotPixmap 	*pixmap_attrarray;
    flow_sAnnotPixmap 	*pixmap_alarm;
    flow_sAnnotPixmap 	*pixmap_ack;
    flow_sAnnotPixmap 	*pixmap_eventalarm;
    flow_sAnnotPixmap 	*pixmap_eventacked;
    flow_sAnnotPixmap 	*pixmap_eventreturn;
    flow_sAnnotPixmap 	*pixmap_blockr;
    flow_sAnnotPixmap 	*pixmap_blockl;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};

class EvList {
  public:
    EvList(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	ev_eType ev_type,
	int ev_size,
	Widget *w);
    ~EvList();

    void 		*parent_ctx;
    Widget		parent_wid;
    ev_eType		type;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    EvListBrow		*brow;
    int			size;
    int			max_size;
    int			display_hundredth;
    int			hide_object;
    int			hide_text;
    void 		(*start_trace_cb)( void *, pwr_tObjid, char *);
    void 		(*display_in_xnav_cb)( void *, pwr_sAttrRef *);
    void 		(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
					  unsigned long, char *, Widget *);
    double		acc_beep_time;
    double		beep_interval;

    void event_info( mh_sMessage *msg);
    void event_alarm( mh_sMessage *msg);
    void event_block( mh_sBlock *msg);
    void event_cancel( mh_sReturn *msg);
    void event_ack( mh_sAck *msg);
    void event_return( mh_sReturn *msg);
    void event_clear_alarmlist( pwr_tNodeIndex nix);
    int get_last_not_acked( mh_sEventId **id);
    int get_last_not_acked_beep( mh_sEventId **id);
    int id_to_item( mh_sEventId *id, void **item);
    int oid_to_item( pwr_tOid oid, void **item);
    void ack( mh_sEventId *id);
    void set_input_focus();
    void zoom( double zoom_factor);
    void unzoom();
    void beep( double scantime);
    void start_trace();
    void display_in_xnav();
    void set_nodraw();
    void reset_nodraw();
    void update_text();
    void set_display_hundredth( int value);
    void set_hide_object( int value);
    void set_hide_text( int value);
    int get_alarm_info( evlist_sAlarmInfo *info);
    int get_last_not_acked_prio( mh_sEventId **id, unsigned long type, unsigned long prio);
    int get_selected_event( char *eventname);
    int get_destination( pwr_tTime time, void **dest);
    void block_remove();
};

class ItemAlarm {
  public:
    ItemAlarm( EvList *evlist, char *item_name, pwr_tTime item_time,
	char *item_eventtext, char *item_eventname, int item_eventflags,
	unsigned long item_eventprio, mh_sEventId item_eventid,
	pwr_tObjid item_object, unsigned long item_status,
	evlist_eEventType item_event_type,
	brow_tNode dest, flow_eDest dest_code);
    evlist_eItemType	type;
    evlist_eEventType	event_type;
    EvList		*evlist;
    brow_tNode		node;
    char	 	name[40];
    pwr_tTime		time;
    char		eventtext[80];
    char		eventname[80];
    int			eventflags;
    unsigned long	eventprio;
    mh_sEventId		eventid;
    pwr_tObjid		object;
    unsigned long	status;

    void	update_text();
};

#endif
