/* 
 * Proview   $Id: ge_attrnav_gtk.h,v 1.2 2008-10-31 12:51:33 claes Exp $
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

#ifndef ge_attrnav_gtk_h
#define ge_attrnav_gtk_h

/* xtt_attrnav_gtk.h -- Simple navigator */

#include <gtk/gtk.h>

#ifndef ge_attrnav_h
# include "ge_attrnav.h"
#endif

//! The navigation area of the attribute editor.
class AttrNavGtk : public AttrNav {
  public:
    AttrNavGtk(
	void *xn_parent_ctx,
	GtkWidget	*xn_parent_wid,
	const char *xn_name,
	attr_sItem  *xn_itemlist,
	int xn_item_cnt,
	GtkWidget **w,
	pwr_tStatus *status);
    ~AttrNavGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;
    gint		trace_timerid;

    void start_trace( pwr_tObjid Objid, char *object_str);
    void set_inputfocus();
    void trace_start();
    static void trace_scan( AttrNav *attrnav);
};

#endif
