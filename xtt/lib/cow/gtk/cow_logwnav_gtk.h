/** 
 * Proview   $Id$
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

#ifndef cow_logwnav_gtk_h
#define cow_logwnav_gtk_h

/* cow_logwnav_gtk.h -- History log window */

#ifndef co_logwnav_h
# include "cow_logwnav.h"
#endif

class CoLogWNavGtk : public CoLogWNav {
  public:
    CoLogWNavGtk(
	void *ev_parent_ctx,
	GtkWidget *ev_parent_wid,
	int l_show_item,
	GtkWidget **w);
    ~CoLogWNavGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;

    void set_input_focus();
};

#endif



