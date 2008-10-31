/* 
 * Proview   $Id: xtt_clog.h,v 1.4 2008-10-31 12:51:36 claes Exp $
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

#ifndef xtt_clog_h
#define xtt_clog_h

/* xtt_clog.h -- Alarm and event windows in xtt */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef xtt_clognav
# include "xtt_clognav.h"
#endif

class CoWow;

class CLog {
  public:
    CLog( void *clog_parent_ctx,
	  const char *clog_name,
	  pwr_tStatus *status);
    virtual ~CLog();

    void 		*parent_ctx;
    char 		name[80];
    CLogNav		*clognav;
    int			clog_displayed;
    void 		(*help_cb)( void *, const char *);
    void 		(*close_cb)( void *);
    bool		filesel_loaded;
    CoWow		*wow;

    virtual void pop() {}
    virtual void set_clock_cursor() {}
    virtual void reset_cursor() {}
    virtual void free_cursor() {}

    void activate_next_file();
    void activate_prev_file();
    void activate_help();
};

#endif




