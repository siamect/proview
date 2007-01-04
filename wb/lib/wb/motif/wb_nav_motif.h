/* 
 * Proview   $Id: wb_nav_motif.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_nav_motif_h
#define wb_nav_motif_h

/* wb_nav_motif.h -- Simple navigator */

#ifndef wb_nav_h
# include "wb_nav.h"
#endif

class NavMotif : public Nav {
 public:
  NavMotif(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status
	);
  ~NavMotif();

  Widget	parent_wid;
  Widget	brow_widget;
  Widget	form_widget;
  Widget	toplevel;

  void set_inputfocus( int focus);
  void set_selection_owner();
};

#endif





