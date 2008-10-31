/* 
 * Proview   $Id: wb_wcast_gtk.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_wcast_gtk_h
#define wb_wcast_gtk_h

/* wb_wcast_gtk.h -- Attribute object casting window. */

#ifndef wb_wcast_h
# include "wb_wcast.h"
#endif

class WCastGtk : public WCast {
  public:
    GtkWidget	*parent_wid;

    WCastGtk( 
	void	*wc_parent_ctx,
	GtkWidget *wc_parent_wid,
	const char *wc_name,
	ldh_tSesContext wc_ldhses,
	pwr_sAttrRef wc_aref,
	pwr_tStatus *status);
    ~WCastGtk();

};

#endif



