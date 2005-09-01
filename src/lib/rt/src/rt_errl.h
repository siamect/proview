/* 
 * Proview   $Id: rt_errl.h,v 1.4 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_errl_h
#define rt_errl_h

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
void		errl_Init	(const char *termname,
				 void (*log_cb)( void *, char *, char, pwr_tStatus, int, int),
				 void *userdata);
void		errl_SetTerm	(const char *termname);
void		errl_SetFile	(const char *filename);

#if defined OS_LINUX
void		errl_Unlink	();
#endif

#endif
