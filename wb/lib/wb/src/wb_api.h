#ifndef wb_api_h
#define wb_api_h

/* wb_api.h -- c api to c++ classes

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif
#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif
#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef co_wow_h
#include "co_wow.h"
#endif


/* Pal */
typedef void *pal_tCtx;

pal_tCtx pal_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status
	);
void pal_del( pal_tCtx palctx);
void pal_zoom( pal_tCtx palctx, double zoom_factor);
void pal_unzoom( pal_tCtx palctx);
int pal_get_select( pal_tCtx palctx, pwr_tClassId *classid);
void pal_set_inputfocus( pal_tCtx palctx, int focus);

/* Nav */
typedef void *nav_tCtx;

nav_tCtx nav_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status
	);
void nav_del( nav_tCtx navctx);
void nav_zoom( nav_tCtx navctx, double zoom_factor);
void nav_unzoom( nav_tCtx navctx);
int nav_get_select( nav_tCtx navctx, pwr_sAttrRef *attrref, int *is_attr);
void nav_set_inputfocus( nav_tCtx navctx, int focus);

/* Wtt */

typedef void *wtt_tCtx;

wtt_tCtx wtt_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	char *iconname,
	void *wbctx,
	pwr_tVolumeId volid,
	void (*wtt_close_cb)(void *),
	void (*wtt_open_volume_cb)(void *, char *, wow_eFileSelType),
	int (*wtt_time_to_exit_cb)(void *),
	pwr_tStatus *sts
	);
int wtt_get_select_first( wtt_tCtx palctx, pwr_sAttrRef *attrref, 
			  int *is_attr);
int wtt_command( wtt_tCtx wttctx, char *cmd);

typedef void *watt_tCtx;
watt_tCtx watt_new(
	Widget	parent_wid,
	void *parent_ctx,
	ldh_tSesContext ldhses,
	pwr_tObjid objid,
	int editmode,
	int advanced_user,
	int display_objectname,
	void (*watt_close_cb)(void *)
	);
void watt_delete( watt_tCtx watt);
int watt_open_changevalue( watt_tCtx watt, char *name);
void watt_pop( watt_tCtx watt);

#if defined __cplusplus
}
#endif
#endif




