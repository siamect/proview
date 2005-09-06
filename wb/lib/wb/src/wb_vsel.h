/* 
 * Proview   $Id: wb_vsel.h,v 1.4 2005-09-06 10:43:32 claes Exp $
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

#ifndef wb_vsel_h
#define wb_vsel_h

/* wb_vsel.h -- select volume */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef wb_nav_macros_h
#include "wb_nav_macros.h"
#endif

#ifndef wb_h
#include "wb.h"
#endif

#define VSEL_MAX_VOLUMES 200

struct vsel_widgets 
{                                             
  Widget	toplevel;
  Widget	vsel_window;
  Widget	label;
  Widget        adb;
  Widget        volumelist;
  Widget        creavolume_dia;
  Widget        volumeid;
  Widget        volumename;
  Widget        volumeclass;
  Widget        creavolume;
};


class WVsel {
 public:
  WVsel (
	 pwr_tStatus   *status,
	 void		*wv_parent_ctx,
	 Widget	wv_parent_wid,
	 char		*wv_name,
	 ldh_tWBContext wv_wbctx,
	 char		*volumename,
	 int		(*bc_success)( void *, pwr_tVolumeId *, int),
	 void		(*bc_cancel)(),
	 int		(*bc_time_to_exit)( void *),
	 int		show_volumes,
	 wb_eType       wv_wb_type);
  void message( char *new_label);
  pwr_tStatus load_volumelist();
  pwr_tStatus check_volumelist( int quiet, int *display_window);

  void                  *parent_ctx;
  Widget                parent_wid;
  char                  name[40];
  struct vsel_widgets	widgets;
  ldh_tWBContext 	wbctx;
  int			(*vsel_bc_success)(void *, pwr_tVolumeId *, int);
  void			(*vsel_bc_cancel)();
  int			(*vsel_bc_time_to_exit)(void *);
  pwr_tVolumeId		volumes[VSEL_MAX_VOLUMES];
  int			volume_count;
  int			all;
  int			write_priv;
  wb_eType              wb_type;

  // Backcall functions from uil
  static void vsel_activate_showall( Widget w, WVsel *vsel, XmToggleButtonCallbackStruct *data);
  static void vsel_activate_ok( Widget w, WVsel *vsel, XmAnyCallbackStruct *data);
  static void vsel_activate_cancel( Widget w, WVsel *vsel, XmAnyCallbackStruct *data);
  static void vsel_activate_close( Widget w, WVsel *vsel, XmAnyCallbackStruct *data);
  static void vsel_create_label( Widget w, WVsel *vsel, XmAnyCallbackStruct *data);
  static void vsel_create_adb( Widget w, WVsel *vsel, XmAnyCallbackStruct *data);
  static void vsel_destroy_adb( Widget w, WVsel *vsel, XmAnyCallbackStruct *data);
  static void vsel_create_volumelist( Widget w, WVsel *vsel, XmAnyCallbackStruct *data);
  static void vsel_action_volumelist( Widget w, WVsel *vsel, XmListCallbackStruct *data);

  ~WVsel();
};

#endif




