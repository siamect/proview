/* 
 * Proview   $Id: xtt_logging.h,v 1.6 2008-10-31 12:51:36 claes Exp $
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

#ifndef xtt_logging_h
#define xtt_logging_h

/* xtt_logging.h -- Logging in xtt */

#ifndef co_ccm_h
#include "co_ccm.h"
#endif

#ifndef rt_gdh_h
#include "rt_gdh.h"
#endif

#if defined OS_VMS
# include <pthread.h>
#endif

#define	RTT_LOGG_MAXENTRY	10
#define	RTT_LOGG_MAXPAR		100
#define RTT_BUFFER_DEFSIZE	100
#define RTT_LOGG_LINE_DEFSIZE	512

typedef enum {
  xtt_LoggType_Cont  = 1,
  xtt_LoggType_Mod   = 2
} xtt_LoggType;

class XttLogging {
  public:
    XttLogging();
    ~XttLogging();

    void        *xnav;
    int         index;
    int		active;
    int		intern;
    int		stop_logg;
    int		logg_type;
    int		logg_priority;
    pwr_tAName	parameterstr[RTT_LOGG_MAXPAR];
    pwr_tAName	shortname[RTT_LOGG_MAXPAR];
    char	*parameter_ptr[RTT_LOGG_MAXPAR];
    gdh_tDlid	parameter_subid[RTT_LOGG_MAXPAR];
    unsigned long parameter_type[RTT_LOGG_MAXPAR];
    unsigned long parameter_size[RTT_LOGG_MAXPAR];
    pwr_tAName	conditionstr;
    char	*condition_ptr;
    gdh_tDlid	condition_subid;
    int		logg_time;
    pwr_tFileName logg_filename;
    FILE	*logg_file;	
    char	old_value[RTT_LOGG_MAXPAR][8];
    pwr_tTime	starttime;
#ifdef OS_VMS
    pthread_t 	thread;
    unsigned int    event_flag;
#endif
#if defined  OS_LYNX || defined OS_LINUX || OS_MACOS
    pthread_t 	thread;
#endif
    int		line_size;
    int		parameter_count;
    int		print_shortname;
    int		buffer_size;
    int		wanted_buffer_size;
    int		buffer_count;
    char	*buffer_ptr;
    ccm_tSingleLineCtx cond_ccm_ctx;
    
    void init( int logg_index, void *logg_xnav);
    int logging_set(
			int		logg_time,
			char		*filename,
			char		*parameterstr,
			char		*conditionstr,
			int		logg_type,
			int		insert,
			int		buffer_size,
			int		stop,
			int		priority,
			int		create,
			int		line_size,
			int		shortname);
    int show();
    int show_entry(
			char		*buff,
			int		*buff_cnt);
    int store(          char		*filename);
    int start();
    int stop();
    int entry_stop();
    int remove( char *parameterstr);
    int log_print( const char *format, ... );
    int print_buffer();
    int close_files();
    int set_prio( int prio) { return 1;};
    int set_default_prio() { return 1;};
    void message( char severity, const char *msg);
};

#endif



