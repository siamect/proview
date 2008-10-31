/* 
 * Proview   $Id: wb_vsel_gtk.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_vsel_h_gtk
#define wb_vsel_h_gtk

/* wb_vsel_gtk.h -- select volume */

#ifndef wb_vsel_h
#include "wb_vsel.h"
#endif

struct vsel_widgets 
{                                             
  GtkWidget	*toplevel;
  GtkWidget	*vsel_window;
  GtkWidget	*label;
  GtkWidget     *adb;
  GtkWidget     *volumelist;
  GtkWidget     *creavolume_dia;
  GtkWidget     *volumeid;
  GtkWidget     *volumename;
  GtkWidget     *volumeclass;
  GtkWidget     *creavolume;
};


class WVselGtk : public WVsel {
 public:
  WVselGtk (
	 pwr_tStatus   *status,
	 void		*wv_parent_ctx,
	 GtkWidget	*wv_parent_wid,
	 const char    	*wv_name,
	 ldh_tWBContext wv_wbctx,
	 char		*volumename,
	 int		(*bc_success)( void *, pwr_tVolumeId *, int),
	 void		(*bc_cancel)(),
	 int		(*bc_time_to_exit)( void *),
	 int		show_volumes,
	 wb_eType       wv_wb_type);
  ~WVselGtk();

  GtkWidget             *parent_wid;
  struct vsel_widgets	widgets;
  GtkListStore 		*store;

  void message( char *new_label);
  void list_add_item( char *str);
  void list_clear();



  // Backcall functions from uil
  static void activate_showall( GtkWidget *w, gpointer data);
  static void activate_ok( GtkWidget *w, gpointer data);
  static void activate_cancel( GtkWidget *w, gpointer data);
  static void activate_close( GtkWidget *w, gpointer data);

};

#endif




