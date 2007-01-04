/* 
 * Proview   $Id: ge_subgraphs_motif.h,v 1.1 2007-01-04 08:22:16 claes Exp $
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

#ifndef ge_subgraphs_motif_h
#define ge_subgraphs_motif_h

#ifndef ge_subgraphs_h
# include "ge_subgraphs.h"
#endif

/*! \file ge_subgraphs_motif.h
    \brief Contains the SubGraphs class and related classes. */
/*! \addtogroup Ge */
/*@{*/

//! Display loaded subgraphs.
class SubGraphsMotif : public SubGraphs {
  public:
    SubGraphsMotif(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	void *grow_ctx,
	Widget *w,
	pwr_tStatus *status);

    Widget		parent_wid;
    Widget		subgraphs_form;
    Widget		toplevel;
    Widget		brow_widget;
    Widget		form_widget;
    XtIntervalId	trace_timerid;

    void trace_start();
    Attr *new_attr( void *object, attr_sItem *items, int num);
    static void trace_scan( SubGraphsMotif *subgraphs);
    ~SubGraphsMotif();
};


/*@}*/
#endif
