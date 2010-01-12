/* 
 * Proview   $Id: co_rtmon.h,v 1.3 2008-10-31 12:51:30 claes Exp $
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

#ifndef cow_rtmon_h
#define cow_rtmon_h

/* cow_rtmon.h -- Status Monitor node list */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef cow_statusmon_nodelistnav
# include "cow_statusmon_nodelistnav.h"
#endif

class CoWow;
class MsgWindow;

class RtMon {
  public:
    RtMon( void *rtmon_parent_ctx,
	   const char *rtmon_name,
	   const char *rtmon_display,
	   pwr_tStatus *status);
    virtual ~RtMon();

    void 		*parent_ctx;
    char 		name[80];
    NodelistNav		*nodelistnav;
    int			rtmon_displayed;
    void 		(*help_cb)( void *, const char *);
    void 		(*close_cb)( void *);
    void 		(*india_ok_cb)( RtMon *, char *);
    CoWow		*wow;
    char		nodename[40];
    char		display[80];

    virtual void pop() {}
    virtual void set_clock_cursor() {}
    virtual void reset_cursor() {}
    virtual void free_cursor() {}

    void set_scantime( float scantime) { nodelistnav->scantime = int(scantime * 1000);}
    void activate_help();
    void activate_start();
    void activate_restart();
    void activate_stop();
    void activate_reset();
    void activate_xtt();
    void activate_op();
    static void stop_ok_cb( void *ctx, void *data);
    static void reset_ok_cb( void *ctx, void *data);
};

#endif




