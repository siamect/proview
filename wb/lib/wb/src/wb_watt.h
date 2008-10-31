/* 
 * Proview   $Id: wb_watt.h,v 1.6 2008-10-31 12:51:32 claes Exp $
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

#ifndef wb_watt_h
#define wb_watt_h

/* wb_watt.h -- Object attribute editor */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_utility_h
# include "wb_utility.h"
#endif

#ifndef wb_h
# include "wb.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

class WAttNav;

class WAtt {
  public:
    WAtt( 
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses,
	pwr_sAttrRef 	wa_aref,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname);
    virtual ~WAtt();
    void 	*parent_ctx;
    ldh_tSesContext ldhses;
    pwr_sAttrRef aref;
    int		editmode;
    char 	name[80];
    WAttNav	*wattnav;
    void	*root_item;
    int		input_open;
    int         input_multiline;
    void 	*object;
    void	(*close_cb) ( void *);
    void	(*redraw_cb) (void *);
    void	*client_data;
    brow_tObject input_node;
    char	input_name[80];
    wb_eUtility	utility;

    virtual void message( char severity, const char *message) {}
    virtual void set_prompt( const char *prompt) {}
    virtual void change_value( int set_focus) {}
    virtual void change_value_close() {}
    virtual void pop() {}

    void set_editmode( int editmode, ldh_tSesContext ldhses);
    int open_changevalue( const char *name);
    static void message_cb( void *watt, char severity, const char *message);
    static void change_value_cb( void *watt);

};

#endif







