#ifndef xtt_logging_h
#define xtt_logging_h

/* xtt_logging.h -- Logging in xtt

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
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
    char	parameterstr[RTT_LOGG_MAXPAR][120];
    char	shortname[RTT_LOGG_MAXPAR][120];
    char	*parameter_ptr[RTT_LOGG_MAXPAR];
    gdh_tDlid	parameter_subid[RTT_LOGG_MAXPAR];
    unsigned long parameter_type[RTT_LOGG_MAXPAR];
    unsigned long parameter_size[RTT_LOGG_MAXPAR];
    char	conditionstr[120];
    char	*condition_ptr;
    gdh_tDlid	condition_subid;
    int		logg_time;
    char	logg_filename[80];
    FILE	*logg_file;	
    char	old_value[RTT_LOGG_MAXPAR][8];
    pwr_tTime	starttime;
#ifdef OS_VMS
    pthread_t 	thread;
    unsigned int    event_flag;
#endif
#if defined  OS_LYNX || defined OS_LINUX
    pthread_t 	thread;
#endif
    int		line_size;
    int		parameter_count;
    int		print_shortname;
    int		buffer_size;
    int		buffer_count;
    char	*buffer_ptr;
    
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
    int remove( char		*parameterstr);
    int get_parinfo(
			char		*parameter_name,
			pwr_sParInfo	*parinfo);
    int log_print( 
		char		*format,
		... );
    int print_buffer();
    int close_files();
    int set_prio( int prio) { return 1;};
    int set_default_prio() { return 1;};
    void message( char severity, char *msg);
};

#if defined __cplusplus
}
#endif
#endif



