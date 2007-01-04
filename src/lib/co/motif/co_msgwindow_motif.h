/** 
 * Proview   $Id: co_msgwindow_motif.h,v 1.1 2007-01-04 07:51:41 claes Exp $
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

#ifndef co_msgwindow_motif_h
#define co_msgwindow_motif_h

/* co_msgwindow_motif.h -- Message window */

#ifndef co_msgwindow_h
# include "co_msgwindow.h"
#endif

#include <Xm/Xm.h>

class MsgWindowMotif : public MsgWindow {
  public:
    MsgWindowMotif(
	void *msg_parent_ctx,
	Widget	msg_parent_wid,
	char *msg_name,
	pwr_tStatus *status);
    ~MsgWindowMotif();

    Widget		parent_wid;
    Widget		toplevel;
    Widget		form;
    Widget		nav_widget;

    void	map();
    void	unmap();

    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);
    static void activate_exit( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data);
    static void activate_clear( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data);
    static void activate_zoom_in( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data);
    static void activate_zoom_out( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data);
    static void activate_zoom_reset( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data);
    static void activate_help_message( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data);
    static void create_form( Widget w, MsgWindow *msgw, XmAnyCallbackStruct *data);

};

#endif









