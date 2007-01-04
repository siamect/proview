/* 
 * Proview   $Id: ge_subpalette_gtk.h,v 1.1 2007-01-04 08:21:58 claes Exp $
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

#ifndef ge_subpalette_gtk_h
#define ge_subpalette_gtk_h

#include "ge_subpalette.h"

/*! \file ge_subpalette_gtk.h
    \brief Contains the SubPaletteGtk class. */
/*! \addtogroup Ge */
/*@{*/

class SubPaletteGtk : public SubPalette {
  public:
    SubPaletteGtk(
	void *xn_parent_ctx,
	GtkWidget *xn_parent_wid,
	char *xn_name,
	GtkWidget **w,
	pwr_tStatus *status);
    ~SubPaletteGtk();

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;

    void set_inputfocus( int focus);
};

/*@}*/
#endif






