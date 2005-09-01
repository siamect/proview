/* 
 * Proview   $Id: rt_errl.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_errl.c -- Logging module
   Handles logging for LynxOS.  
*/

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>

#include "rt_errl.h"
#include "rt_errh.h"

#define MAX_NO_MSG 100;

static pthread_mutex_t fileMutex;
static pthread_mutex_t termMutex;
static mqd_t mqid = (mqd_t)-1;
static int logFile = -1;
static int newLogFile = 1;
static int term = -1;
static int yday = -1;
static pwr_tBoolean logToStdout = FALSE;

static void CheckTimeStamp(int force);
static void *log_thread(void *arg);


void
errl_Init (
  const char	*termName
)
{
  pthread_mutexattr_t mutexattr;
  pthread_t tid;
  pthread_attr_t pthreadattr;
  struct mq_attr mqattr;
  mode_t mode;
  int oflags;
  char name[64];
  char *busid = getenv(pwr_dEnvBusId);
  static int initDone = 0;
#ifndef PWR_LYNX_30
  int policy;
  struct sched_param param;
#else
  int prio;
#endif
  
  if (initDone)
    return;

#ifndef PWR_LYNX_30
  if ((pthread_getschedparam(pthread_self(), &policy, &param)) == -1) {
    perror("rt_errl: pthread_getprio(pthread_self() ");
    return;
  }

  pthread_mutexattr_init(&mutexattr);
  if (pthread_mutex_init(&fileMutex, &mutexattr) == -1) {
    perror("rt_logmod: pthread_mutex_init(&fileMutex, mutexattr) ");
    return;
  }

  if (pthread_mutex_init(&termMutex, &mutexattr) == -1) {
    perror("rt_logmod: pthread_mutex_init(&termMutex, mutexattr) ");
    return;
  }

#else
  if ((prio = pthread_getprio(pthread_self())) == -1) {
    perror("rt_errl: pthread_getprio(pthread_self() ");
    return;
  }

  pthread_mutexattr_create(&mutexattr);
  if (pthread_mutex_init(&fileMutex, mutexattr) == -1) {
    perror("rt_logmod: pthread_mutex_init(&fileMutex, mutexattr) ");
    return;
  }

  if (pthread_mutex_init(&termMutex, mutexattr) == -1) {
    perror("rt_logmod: pthread_mutex_init(&termMutex, mutexattr) ");
    return;
  }
#endif
  



#ifndef PWR_LYNX_30
  pthread_mutexattr_destroy(&mutexattr);
#else
  pthread_mutexattr_delete(&mutexattr);
#endif

  mqattr.mq_msgsize = LOG_MAX_MSG_SIZE;  /* max mess size */
  mqattr.mq_maxmsg = MAX_NO_MSG;     /* max no of msg in this queue */
  mqattr.mq_flags = 0;               /* not used entry */
  oflags = O_CREAT | O_RDWR;
  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  sprintf(name, "%s_%s", LOG_QUEUE_NAME, busid ? busid : "");  
  mqid = mq_open(name, oflags, mode, &mqattr);
  if (mqid == (mqd_t)-1) {
    perror("rt_logmod: mq_open ");
    return;
  }

#ifndef PWR_LYNX_30
  pthread_attr_init(&pthreadattr);
  if (pthread_create(&tid, &pthreadattr, log_thread, NULL) == -1) {
    perror("rt_logmod: pthread_create ");
    pthread_attr_destroy(&pthreadattr);  
    return;
  }
  pthread_attr_destroy(&pthreadattr); 
  	  
  param.sched_priority -= 1;
  pthread_setschedparam(tid, policy, &param);
#else

  pthread_attr_create(&pthreadattr);
  if (pthread_create(&tid, pthreadattr, log_thread, NULL) == -1) {
    perror("rt_logmod: pthread_create ");
    pthread_attr_delete(&pthreadattr);  
    return;
  }
  pthread_attr_delete(&pthreadattr); 
  
  pthread_setprio(tid, prio - 1);
#endif


  if (termName && *termName)
    errl_SetTerm(termName);

  logToStdout = getenv("PWR_LOG_TO_STDOUT") != NULL ? TRUE : FALSE; 

  initDone = 1;
  
  return;
}
void
errl_SetFile (
  const char *logFileName)
{
  pwr_tStatus sts = 1;
  int oflags = O_CREAT | O_APPEND | O_WRONLY;
  int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

  pthread_mutex_lock(&fileMutex);
  if (logFile != -1) {
    close(logFile);
    logFile = -1;
  }

  if ((logFile = open(logFileName, oflags, mode)) == -1) {
    errh_Error("Cannot open log file: %s", logFileName);
    sts = 2;
  } else {
    errh_Info("Logging to %s", logFileName);
    newLogFile = 1;
  }
  pthread_mutex_unlock(&fileMutex);
}

void
errl_SetTerm (
  const char	*termName
)
{
  int		oflags = O_APPEND | O_WRONLY;

  pthread_mutex_lock(&termMutex);
  if (term != -1) {
    close(term);
    term = -1;
  }

  if (termName && *termName) {
    if ((term = open(termName, oflags)) == -1) {
      errh_Error("Cannot open terminal: %s", termName);
    }
  }
  pthread_mutex_unlock(&termMutex);

}

static void
CheckTimeStamp (
  int		force
)
{
  time_t	t;
  struct tm	tmpTm;

  t = time(NULL);
  localtime_r(&tmpTm, &t);

  if (force || (yday != tmpTm.tm_yday)) {
    char buf[64];
    #define STAMP "DATE STAMP:  "

    write(logFile, STAMP, strlen(STAMP));
    strftime(buf,  sizeof(buf), "%e-%b-%Y\n", &tmpTm);
    write(logFile, buf, strlen(buf));

    pthread_mutex_lock(&termMutex);
    if (term != -1) 
      write(term, buf, strlen(buf));
    pthread_mutex_unlock(&termMutex);

    if (logToStdout)
      printf("%.*s", (int)strlen(buf), buf);

    yday = tmpTm.tm_yday;
  }
}

static void *
log_thread (void *arg)
{
  int len;
  char buf[LOG_MAX_MSG_SIZE];

  while (1) {
    len = mq_receive(mqid, buf, LOG_MAX_MSG_SIZE, NULL);
    if (len == -1) {
      perror("rt_logmod.c: mq_receive ");
    } else {
      pthread_mutex_lock(&fileMutex);
      if (logFile != -1) {			    			       
	/* Set up a timer if you want better performance, ML */
	CheckTimeStamp(newLogFile); 
	newLogFile = 0; 	   
	write(logFile, buf, len);
	write(logFile, "\n", 1);
      }
      pthread_mutex_unlock(&fileMutex);

      pthread_mutex_lock(&termMutex);
      if (term != -1) {			    	
	write(term, buf, len);
	write(term, "\n", 1);
      }
      pthread_mutex_unlock(&termMutex);

      if (logToStdout)
	printf("%.*s\n", len, buf);

    }
  }
}
