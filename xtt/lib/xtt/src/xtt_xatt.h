/* 
 * Proview   $Id: xtt_xatt.h,v 1.5 2007-01-04 08:22:47 claes Exp $
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

#ifndef xtt_xatt_h
#define xtt_xatt_h

/* xtt_xatt.h -- Object attribute editor */

#ifndef pwr_h
# include "pwr.h"
#endif

class XAttNav;

class XAtt {
  public:
    XAtt( 
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	int             *xa_sts);
    virtual ~XAtt();
    void 	*parent_ctx;
    pwr_sAttrRef objar;
    char 	name[80];
    XAttNav	*xattnav;
    void	*root_item;
    int		input_open;
    int         input_multiline;
    void 	*object;
    void	(*close_cb) ( void *, void *);
    void	(*redraw_cb) (void *);
    void 	(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
				  unsigned long, char *, int x, int y);
    int         (*call_method_cb)(void *, char *, char *, pwr_sAttrRef,
				  unsigned long, unsigned long, char *);
    int		(*is_authorized_cb)(void *, unsigned int);
    void	*client_data;
    brow_tObject input_node;
    char	input_name[80];

    virtual void message( char severity, char *message) {}
    virtual void set_prompt( char *prompt) {}
    virtual void change_value( int set_focus) {}
    virtual void change_value_close() {}
    virtual void pop() {}

    int open_changevalue( char *name);
    void swap( int mode);
    void activate_display_object();
    void activate_show_cross();
    void activate_open_classgraph();
    void activate_open_plc();
    void activate_help();

    static void xatt_popup_menu_cb( void *ctx, pwr_sAttrRef attrref,
			       unsigned long item_type, unsigned long utility,
			       char *arg, int x, int y);
    static int xatt_is_authorized_cb( void *ctx, unsigned int access);
    static void message_cb( void *xatt, char severity, char *message);
    static void change_value_cb( void *xatt);
};

#endif







