/* 
 * Proview   $Id: wb_wpkg.h,v 1.3 2005-09-06 10:43:32 claes Exp $
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

#ifndef wb_wpkg_h
#define wb_wpkg_h

/* wb_wpkg.h -- Package window */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_h
# include "wb.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

class WPkgNav;

class WPkg {
  public:
    WPkg( 
	Widget 		wa_parent_wid, 
	void 		*wa_parent_ctx);
    ~WPkg();
    Widget	parent_wid;
    void 	*parent_ctx;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    WPkgNav	*wpkgnav;
    Widget	msg_label;
    Widget	wpkgnav_form;
    void	(*close_cb) ( void *);
    wb_eUtility	utility;
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    int		display_mode;
    int		clock_cursor;

    void message( char severity, char *message);
    void pop();
    void set_clock_cursor();
    void reset_cursor();
};


#if defined __cplusplus
}
#endif
#endif







