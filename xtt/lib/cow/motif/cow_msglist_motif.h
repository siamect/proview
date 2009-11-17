/** 
 * Proview   $Id: co_msglist_motif.h,v 1.1 2007-01-04 07:51:41 claes Exp $
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

#ifndef cow_msglist_motif_h
#define cow_msglist_motif_h

/* cow_msglist_motif.h -- Message list */

#ifndef cow_msglist_h
# include "cow_msglist.h"
#endif

class MsgListMotif : public MsgList {
  public:
    MsgListMotif(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	Widget *w);
    ~MsgListMotif();

    Widget		parent_wid;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;

    void set_input_focus();
};

#endif



