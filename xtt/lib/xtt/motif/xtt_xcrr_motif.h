/* 
 * Proview   $Id: xtt_xcrr_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_xcrr_motif_h
#define xtt_xcrr_motif_h

/* xtt_xcrr_motif.h -- Object crossreferences */

#ifndef xtt_xcrr_h
# include "xtt_xcrr.h"
#endif

class XCrrMotif : public XCrr {
  public:
    XCrrMotif( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	int             *xa_sts);
    ~XCrrMotif();
    Widget	parent_wid;
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    Widget	xcrrnav_form;
    int		set_focus_disabled;
    XtIntervalId focus_timerid;

    void pop();

    static void activate_exit( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data);
    static void activate_openplc( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data);
    static void create_xcrrnav_form( Widget w, XCrr *xcrr, XmAnyCallbackStruct *data);
    static void enable_set_focus( XCrrMotif *xcrr);
    static void disable_set_focus( XCrrMotif *xcrr, int time);
    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);

};

#endif







