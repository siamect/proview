/* 
 * Proview   $Id: wb_wpkgnav_gtk.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_wpkgnav_gtk_h
#define wb_wpkgnav_gtk_h

#ifndef wb_wpkgnav_h
# include "wb_wpkgnav.h"
#endif

class WPkgNavGtk : public WPkgNav {
  public:
    WPkgNavGtk(
	void 		*wa_parent_ctx,
	GtkWidget      	*wa_parent_wid,
	char 		*wa_name,
	wb_eUtility	wa_utility,
	GtkWidget      	**w,
	pwr_tStatus 	*status);
    ~WPkgNavGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;

    void set_inputfocus();
};

#endif




