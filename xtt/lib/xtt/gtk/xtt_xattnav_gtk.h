/* 
 * Proview   $Id: xtt_xattnav_gtk.h,v 1.1 2007-01-04 08:29:32 claes Exp $
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

#ifndef xtt_xattnav_gtk_h
#define xtt_xattnav_gtk_h

/* wb_xattnav_gtk.h -- */

#ifndef xtt_xattnav_h
# include "xtt_xattnav.h"
#endif

class XAttNavGtk : public XAttNav {
  public:
    XAttNavGtk(
	void 		*xa_parent_ctx,
	GtkWidget      	*xa_parent_wid,
        xattnav_eType   xa_type,
	char 		*xa_name,
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	GtkWidget      	**w,
	pwr_tStatus 	*status);
    ~XAttNavGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;

    void popup_position( int x_event, int y_event, int *x, int *y);
    void set_inputfocus();
};

#endif
