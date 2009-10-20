/* 
 * Proview   $Id$
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

#ifndef wb_xcrr_h
#define wb_xcrr_h

class WAttNav;

/* wb_xcrr.h -- Object crossreferences */

#ifndef pwr_h
# include "pwr.h"
#endif

class WCrr {
  public:
    WCrr( 
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	int             *xa_sts);
    virtual ~WCrr();
    void 	*parent_ctx;
    pwr_sAttrRef objar;
    char 	name[80];
    WAttNav	*xcrrnav;
    void	*root_item;
    int		input_open;
    int         input_multiline;
    void 	*object;
    void	(*close_cb) ( void *, void *);
    void	(*redraw_cb) (void *);
    void 	(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
				  unsigned long, char *, int x, int y);
    void 	(*start_trace_cb)( void *, pwr_tObjid, char *);
    void	*client_data;
    brow_tObject input_node;
    char	input_name[80];

    virtual void pop() {}

    static void xcrr_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
				    unsigned long item_type, unsigned long utility, 
				    char *arg, int x, int y);
    static void xcrr_start_trace_cb( void *ctx, pwr_tObjid objid, char *name);
    static void xcrr_close_cb( void *ctx);
};

#endif







