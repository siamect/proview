/* 
 * Proview   $Id: wb_wpkg.h,v 1.4 2007-01-04 07:29:04 claes Exp $
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

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_utility_h
# include "wb_utility.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

class WPkgNav;
class CoWow;

class WPkg {
  public:
    WPkg( 
	void 		*wa_parent_ctx);
    virtual ~WPkg();
    void 	*parent_ctx;
    char 	name[80];
    WPkgNav	*wpkgnav;
    void	(*close_cb) ( void *);
    wb_eUtility	utility;
    int		display_mode;
    CoWow	*wow;

    void activate_distribute();
    void activate_createpkg();
    void activate_deletepkg();
    void activate_dmode_filediff( int set);
    void activate_dmode_filetime( int set);
    void activate_dmode_filepath( int set);
    void activate_zoom_in();
    void activate_zoom_out();
    void activate_zoom_reset();

    virtual void message( char severity, char *message) {}
    virtual void pop() {}
    virtual void set_clock_cursor() {}
    virtual void reset_cursor() {}
    virtual void flush() {}

    static void message_cb( void *wpkg, char severity, char *message);
    static void set_clock_cursor_cb( void *wpkg);
    static void reset_cursor_cb( void *wpkg);
    static void deletepkg_ok( void *ctx, void *data);
};

#endif







