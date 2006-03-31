/* 
 * Proview   $Id: wb_api.cpp,v 1.9 2006-03-31 14:24:34 claes Exp $
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
 **/

/* wb_api.cpp -- c api for c++ classes. */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>
#if defined OS_VMS
#include <descrip.h>
#include <lib$routines.h>
#endif

extern "C" {
#include "pwr.h"
#include "wb_ldh.h"
#include "co_cdh.h"
#include "pwr_baseclasses.h"
}

#include <Xm/Xm.h>
#if defined OS_VMS
#include <Xm/DECspecific.h>
#endif
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/IntrinsicP.h>
#if defined OS_VMS
#include <decw$include:Xlib.h>
#include <decw$include:Xutil.h>
#include <decw$include:MrmWidget.h>
#endif

#if defined OS_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
// #include <Mrm/MrmWidget.h>
#endif

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_pal.h"
#include "wb_nav.h"
#include "wb_wtt.h"
#undef min
#undef max
#include "wb_watt.h"
#include "wb_wnav.h"
#include "wb_wcast.h"
#include "wb_api.h"



//
//  Interface to Pal
//

pal_tCtx pal_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status
	)
{
  Pal *pal = new Pal( parent_ctx, parent_wid, name, ldhses, root_name,
	w, status);
  return (pal_tCtx) pal;
}

void pal_del( pal_tCtx palctx)
{
  delete (Pal *)palctx;
}

void pal_zoom( pal_tCtx palctx, double zoom_factor)
{
  ((Pal *)palctx)->zoom( zoom_factor);
}

void pal_set_inputfocus( pal_tCtx palctx, int focus)
{
  ((Pal *)palctx)->set_inputfocus( focus);
}
 
void pal_unzoom( pal_tCtx palctx)
{
  ((Pal *)palctx)->unzoom();
}

int pal_get_select( pal_tCtx palctx, pwr_tClassId *classid)
{
  return ((Pal *)palctx)->get_select( classid);
}

//
//  Interface to Nav
//

nav_tCtx nav_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status
	)
{
  Nav *nav = new Nav( parent_ctx, parent_wid, name, ldhses, root_name,
	w, status);
  return (nav_tCtx) nav;
}

void nav_del( nav_tCtx navctx)
{
  delete (Nav *)navctx;
}

void nav_zoom( nav_tCtx navctx, double zoom_factor)
{
  ((Nav *)navctx)->zoom( zoom_factor);
}

void nav_set_inputfocus( nav_tCtx navctx, int focus)
{
  ((Nav *)navctx)->set_inputfocus( focus);
}
 
void nav_unzoom( nav_tCtx navctx)
{
  ((Nav *)navctx)->unzoom();
}

int nav_get_select( nav_tCtx navctx, pwr_sAttrRef *attrref, int *is_attr)
{
  return ((Nav *)navctx)->get_select( attrref, is_attr);
}



//
//  Interface to Wtt
//

extern "C" wtt_tCtx wtt_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	char *iconname,
	void *wbctx,
	pwr_tVolumeId volid,
	void (*wtt_close_cb)(void *),
	void (*wtt_open_volume_cb)(void *, wb_eType, char *, wow_eFileSelType),
	int (*wtt_time_to_exit_cb)(void *),
	pwr_tStatus *sts
	)
{
  Wtt *wtt = new Wtt( parent_ctx, parent_wid, name, 
	iconname, wbctx, volid, 0, 0, sts);
  wtt->close_cb = wtt_close_cb;
  wtt->open_volume_cb = wtt_open_volume_cb;
  wtt->time_to_exit_cb = wtt_time_to_exit_cb;

  return (wtt_tCtx) wtt;
}

int wtt_get_select_first( wtt_tCtx wttctx, pwr_sAttrRef *attrref, int *is_attr)
{
  return ((Wtt *)wttctx)->get_select_first( attrref, is_attr);
}

int wtt_command( wtt_tCtx wttctx, char *cmd)
{
  return ((Wtt *)wttctx)->wnav->command( cmd);
}

int wnav_command( wnav_tCtx wnavctx, char *cmd)
{
  return ((WNav *)wnavctx)->command( cmd);
}

//
//  Interface to Watt
//

extern "C" watt_tCtx watt_new(
	Widget	parent_wid,
	void *parent_ctx,
	ldh_tSesContext ldhses,
	pwr_sAttrRef aref,
	int editmode,
	int advanced_user,
	int display_objectname,
	void (*watt_close_cb)(void *)
	)
{
  WAtt *watt = new WAtt( parent_wid, parent_ctx, ldhses, aref, editmode,
	advanced_user, display_objectname);
  if ( watt_close_cb)
    watt->close_cb = watt_close_cb;

  return (watt_tCtx) watt;
}

extern "C" void watt_delete( watt_tCtx watt )
{
  delete (WAtt *)watt;
}

extern "C" int watt_open_changevalue( watt_tCtx watt, char *name)
{
  return ((WAtt *)watt)->open_changevalue( name);
}

extern "C" void watt_pop( watt_tCtx watt)
{
  ((WAtt *)watt)->pop();
}


extern "C" wcast_tCtx wcast_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	pwr_sAttrRef aref,
	pwr_tStatus *status)
{
  WCast *wcast = new WCast( parent_ctx, parent_wid, name, ldhses, aref, status);
  return (wcast_tCtx) wcast;
}

extern "C" void wcast_delete( wcast_tCtx wcast)
{
  delete (WCast *)wcast;
}
