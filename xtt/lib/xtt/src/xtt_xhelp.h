/* 
 * Proview   $Id: xtt_xhelp.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#ifndef wb_xhelp_h
#define wb_xhelp_h

/* wb_xhelp.h -- Helptext navigator */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif



class XHelp {
  public:
    XHelp( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx, 
	int             *xa_sts);
    ~XHelp();
    Widget	parent_wid;
    void 	*parent_ctx;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    void	*xhelpnav;
    Widget	xhelpnav_form;
    void	*client_data;
    void	(*close_cb) ( void *, void *);
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    int         displayed;

    void pop();
    void clear();
    int help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name);
    int	help_index( navh_eHelpFile file_type, char *file_name);
    void set_dimension( int width, int height);    
};


#if defined __cplusplus
}
#endif
#endif







