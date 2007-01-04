/* 
 * Proview   $Id: wb_wpkg_motif.h,v 1.1 2007-01-04 07:29:02 claes Exp $
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

#ifndef wb_wpkg_motif_h
#define wb_wpkg_motif_h

/* wb_wpkg_motif.h -- Package window */

#ifndef wb_wpkg_h
# include "wb_wpkg.h"
#endif

class WPkgMotif : public WPkg {
  public:
    WPkgMotif( 
	Widget 		wa_parent_wid, 
	void 		*wa_parent_ctx);
    ~WPkgMotif();
    Widget	parent_wid;
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    Widget	msg_label;
    Widget	wpkgnav_form;
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    int		clock_cursor;

    void message( char severity, char *message);
    void pop();
    void set_clock_cursor();
    void reset_cursor();
    void flush();

    static void activate_distribute( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data);
    static void activate_createpkg( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data);
    static void activate_deletepkg( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data);
    static void activate_dmode_filediff( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data);
    static void activate_dmode_filetime( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data);
    static void activate_dmode_filepath( Widget w, WPkg *wpkg, XmToggleButtonCallbackStruct *data);
    static void activate_exit( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data);
    static void create_msg_label( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data);
    static void create_wpkgnav_form( Widget w, WPkg *wpkg, XmAnyCallbackStruct *data);
    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);

};


#endif







