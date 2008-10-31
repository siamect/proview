/* 
 * Proview   $Id: wb_gre_motif.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_gre_motif_h
#define wb_gre_motif_h

#ifndef wb_gre_h
#include "wb_gre.h"
#endif


class WGreMotif : public WGre {
 public:
  Widget	parent_wid;
  Widget	gre_window;
  Widget	flow_widget;
  Widget	form_widget;
  Widget	nav_shell;
  Widget	nav_widget;
  XtIntervalId	trace_timerid;

  WGreMotif( void *parent_ctx,
	Widget parent_wid,
	const char *name);
  ~WGreMotif();

  void trace_start();
  void trace_stop();
  void get_popup_position( int *x, int *y);

  int new_navigator( Widget parent);
  int new_navigator_popup();
  static void trace_scan( WGreMotif *gre);
};

#endif
