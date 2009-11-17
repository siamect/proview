/* 
 * Proview   $Id: co_statusmon_nodelistnav_gtk.h,v 1.3 2007-10-02 15:53:20 claes Exp $
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

#ifndef cow_statusmon_nodelistnav_gtk_h
#define cow_statusmon_nodelistnav_gtk_h

/* cow_statusmon_nodelistnav_gtk.h -- Console message window. */


// Status is defined as int i xlib...

#ifndef cow_statusmon_nodelistnav_h
# include "cow_statusmon_nodelistnav.h"
#endif

class NodelistNavGtk : public NodelistNav {
  public:
    NodelistNavGtk( void *ev_parent_ctx,
		    GtkWidget *nodelistnav_parent_wid,
		    MsgWindow *nodelistnav_msg_window,
		    char *nodelistnav_msg_nodename,
		    int nodelistnav_mode, int nodelistnav_view_node_descr,
		    int nodelist_msgw_pop,
		    GtkWidget **w);
    ~NodelistNavGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;
    gint		trace_timerid;

    void start_trace( pwr_tObjid Objid, char *object_str);
    void set_input_focus();
    void trace_start();
    void beep();
    static void trace_scan( NodelistNav *nodelistnav);
};

#endif


