/* 
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

#ifndef cow_pn_gsdml_attrnav_gtk_h
#define cow_pn_gsdml_attrnav_gtk_h

/* cow_pn_gsdml_attrnav_gtk.h -- Profibus gsd configurator navigator */

#ifndef cow_pn_gsdml_attrnav_h
# include "cow_pn_gsdml_attrnav.h"
#endif

//! The navigation area of the attribute editor.
class GsdmlAttrNavGtk : public GsdmlAttrNav {
  public:
    GsdmlAttrNavGtk(
	void *xn_parent_ctx,
	GtkWidget *xn_parent_wid,
	const char *xn_name,
	pn_gsdml *xn_gsdml,
	int xn_edit_mode,
	GtkWidget **w,
	pwr_tStatus *status);
    ~GsdmlAttrNavGtk();
    void set_inputfocus();

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;
};


#endif
