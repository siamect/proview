/* 
 * Proview   $Id: wb_vsel_motif.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_vsel_h_motif
#define wb_vsel_h_motif

/* wb_vsel_motif.h -- select volume */

#ifndef wb_vsel_h
#include "wb_vsel.h"
#endif

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


class WVselMotif : public WVsel {
 public:
  WVselMotif (
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
  ~WVselMotif();

  Widget                parent_wid;
  struct vsel_widgets	widgets;

  void message( char *new_label);
  void list_add_item( char *str);
  void list_clear();



  // Backcall functions from uil
  static void activate_showall( Widget w, WVselMotif *vsel, XmToggleButtonCallbackStruct *data);
  static void activate_ok( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data);
  static void activate_cancel( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data);
  static void activate_close( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data);
  static void create_label( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data);
  static void create_adb( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data);
  static void destroy_adb( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data);
  static void create_volumelist( Widget w, WVselMotif *vsel, XmAnyCallbackStruct *data);
  static void action_volumelist( Widget w, WVselMotif *vsel, XmListCallbackStruct *data);

};

#endif




