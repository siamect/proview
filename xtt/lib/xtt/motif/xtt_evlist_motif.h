/* 
 * Proview   $Id: xtt_evlist_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_evlist_motif_h
#define xtt_evlist_motif_h

/* xtt_evlist_motif.h -- Alarm and event windows in xtt */

// Status is defined as int i xlib...
#ifdef Status
# undef Status
#endif

#ifndef xtt_evlist_h
# include "xtt_evlist.h"
#endif

class EvListMotif : public EvList {
  public:
    EvListMotif(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	ev_eType ev_type,
	int ev_size,
	Widget *w);
    ~EvListMotif();

    Widget		parent_wid;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;

    void set_input_focus();
    void bell();
    void popup_position( int x_event, int y_event, int *x, int *y);
};


#endif
