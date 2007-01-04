/* 
 * Proview   $Id: wb_gre_gtk.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_gre_gtk_h
#define wb_gre_gtk_h

#ifndef wb_gre_h
#include "wb_gre.h"
#endif


class WGreGtk : public WGre {
 public:
  GtkWidget	*parent_wid;
  GtkWidget	*gre_window;
  GtkWidget	*flow_widget;
  GtkWidget	*form_widget;
  GtkWidget	*nav_shell;
  GtkWidget	*nav_widget;
  gint		trace_timerid;

  WGreGtk( void *parent_ctx,
	GtkWidget *parent_wid,
	char *name);
  ~WGreGtk();

  void trace_start();
  void trace_stop();
  void get_popup_position( int *x, int *y);

  int new_navigator( GtkWidget *parent);
  int new_navigator_popup();
  static void trace_scan( WGreGtk *gre);
};

#endif
