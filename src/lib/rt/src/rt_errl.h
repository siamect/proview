#ifndef rt_errl_h
#define rt_errl_h

/* rt_errl.h -- <short description>

   PROVIEW/R
   Copyright (C) 1998 by Mandator AB.

   <Description>.  
*/

#ifndef pwr_h
# include "pwr.h"
#endif

#if defined OS_LYNX
# define LOG_QUEUE_NAME "/pwrlogqueue"
#elif defined OS_LINUX
# define LOG_QUEUE_NAME "/tmp/pwrlogqueue"
#elif defined OS_ELN
#endif

#define LOG_MAX_MSG_SIZE  256           /* length of logstring */

pwr_tStatus	errl_Exit	();
void		errl_Init	(const char *termname);
void		errl_SetTerm	(const char *termname);
void		errl_SetFile	(const char *filename);

#endif
