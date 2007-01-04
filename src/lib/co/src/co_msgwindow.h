/** 
 * Proview   $Id: co_msgwindow.h,v 1.7 2007-01-04 07:51:42 claes Exp $
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

#ifndef co_msgwindow_h
#define co_msgwindow_h

/* co_msgwindow.h -- Message window */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_msglist_h
# include "co_msglist.h"
#endif

#ifndef co_error_h
# include "co_error.h"
#endif

typedef enum {
  msgw_ePop_No,
  msgw_ePop_Yes,
  msgw_ePop_Default
} msgw_ePop;

class CoWow;
class CoWowFocusTimer;

class MsgWindow {
  public:
    MsgWindow(
	void *msg_parent_ctx,
	char *msg_name,
	pwr_tStatus *status);
    virtual ~MsgWindow() {}

    void 		*parent_ctx;
    char 		name[80];
    MsgList		*msgnav;
    int		        displayed;
    int                 deferred_map;
    int			nodraw;
    int			size;
    int			max_size;
    void 		(*find_wnav_cb)( void *, pwr_tObjid);
    void 		(*find_plc_cb)( void *, pwr_tObjid);
    CoWow		*wow;

    static MsgWindow 	*default_window;
    static int	       	hide_info;

    virtual void	map() {}
    virtual void	unmap() {}

    int         is_mapped() { return displayed;};
    void 	insert( int severity, const char *text, pwr_tOid oid = pwr_cNOid, bool is_plc = false);
    void	set_nodraw() { msgnav->set_nodraw(); nodraw++;}
    void	reset_nodraw();
    static void	set_default( MsgWindow *msgw) { default_window = msgw;}
    static void message( int severity, const char *text, msgw_ePop pop = msgw_ePop_Default, 
			 pwr_tOid oid = pwr_cNOid, bool is_plc = false);
    static void message( const co_error& e, const char *text1 = 0, const char *text2 = 0,
			 pwr_tOid oid = pwr_cNOid, bool is_plc = false);
    static void message( int severity, const char *text1, const char *text2, const char *text3 = 0,
			 pwr_tOid oid = pwr_cNOid, bool is_plc = false);
    static bool	has_window() { return default_window != 0;}
    static CoWow *get_wow() { return default_window ? default_window->wow : 0;}
    static void map_default() { if ( default_window) default_window->map();}
    static void dset_nodraw() { if ( default_window) default_window->set_nodraw();}
    static void dreset_nodraw() { if ( default_window) default_window->reset_nodraw();}
    static void hide_info_messages( int hide) { hide_info = hide;}
    static void msgw_find_wnav_cb( void *ctx, pwr_tOid oid);
    static void msgw_find_plc_cb( void *ctx, pwr_tOid oid);
};

#endif









