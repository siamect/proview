/* 
 * Proview   $Id: ge_graph_motif.h,v 1.1 2007-01-04 08:22:16 claes Exp $
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

#ifndef ge_graph_motif_h
#define ge_graph_motif_h

/* ge_graph_motif.h -- Simple graphic editor */

#ifndef ge_graph_h
# include "ge_graph.h"
#endif

class GraphMotif : public Graph {
 public:
  //! Constructor
  /*!
    \param xn_parent_ctx	Parent context.
    \param xn_parent_wid	Parent widget.
    \param xn_name		Name.
    \param w			Graph widget.
    \param status		Returned status.
    \param xn_default_path	Default path for .pwg files.
    \param graph_mode		Mode, development or runtime.
    \param scrollbar		Use scrollbars.
    \param xn_gdh_init_done	Gdh is alread initialized.
    \param xn_object_name	Object name for a class graph. If zero, this is not a class graph.
    \param xn_use_default_access Use the default access and not the access of the current user.
    \param xn_default_access	Default access. Can be used to override the access of the current user.
  */
  GraphMotif(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	pwr_tStatus *status,
	char *xn_default_path,
	graph_eMode graph_mode = graph_eMode_Development,
	int scrollbar = 1,
	int xn_gdh_init_done = 0,
	char *xn_object_name = 0,
	int xn_use_default_access = 0,
	unsigned int xn_default_access = 0);

  void trace_timer_remove();
  void trace_timer_add( int time);
  int create_navigator( Widget parent);
  void popup_position( int event_x, int event_y, int *x, int *y);
  Attr *attr_new( void *parent_ctx, void *object, attr_sItem *itemlist, int item_cnt);
  void set_inputfocus( int focus);

  Widget		parent_wid;		//! Parent widget.
  Widget		grow_widget;		//! Grow widget.
  Widget		form_widget;		//! Pane widget.
  Widget		toplevel;		//! Toplevel widget.
  Widget		nav_widget;		//! Navigation window widget.
  XtIntervalId		trace_timerid;		//!< Timer id for runtime scan.

  ~GraphMotif();
};
#endif
