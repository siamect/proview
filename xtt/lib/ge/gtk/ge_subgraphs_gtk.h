/* 
 * Proview   $Id: ge_subgraphs_gtk.h,v 1.2 2008-10-31 12:51:33 claes Exp $
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

#ifndef ge_subgraphs_gtk_h
#define ge_subgraphs_gtk_h

#include <gtk/gtk.h>

#ifndef ge_subgraphs_h
# include "ge_subgraphs.h"
#endif

/*! \file ge_subgraphs_gtk.h
    \brief Contains the SubGraphs class and related classes. */
/*! \addtogroup Ge */
/*@{*/

//! Display loaded subgraphs.
class SubGraphsGtk : public SubGraphs {
  public:
    SubGraphsGtk(
	void 		*xn_parent_ctx,
	GtkWidget	*xn_parent_wid,
	const char     	*xn_name,
	void 		*grow_ctx,
	GtkWidget 	**w,
	pwr_tStatus 	*status);

    GtkWidget		*parent_wid;
    GtkWidget		*subgraphs_form;
    GtkWidget		*toplevel;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    gint		trace_timerid;

    void trace_start();
    Attr *new_attr( void *object, attr_sItem *items, int num);
    static void trace_scan( SubGraphsGtk *subgraphs);
    ~SubGraphsGtk();
};


/*@}*/
#endif
