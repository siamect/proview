/* rt_errl.c -- Logging module

   PROVIEW/R
   Copyright (C) 1996,98 by Mandator AB.

   <Description> Handles logging for Linux.  
*/

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "rt_mq.h"
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#include "rt_errl.h"
#include "rt_errh.h"

#define MAX_NO_MSG 100;

static pthread_mutex_t fileMutex;
static pthread_mutex_t termMutex;
static mqd_t mqid = (mqd_t)-1;
static int logFile = -1;
static int newLogFile = 1;
static int term = -1;
static pthread_t tid = 0;
static int yday = -1;
static pwr_tBoolean logToStdout = FALSE;
static void (*errl_log_cb)( void *, char *, char, pwr_tStatus, int, int) = 0;
static void *errl_log_userdata = 0;

static void CheckTimeStamp(int force);
static void *log_thread(void *arg);


void
errl_Init (
  const char	*termName,
  void (*log_cb)( void *, char *, char, pwr_tStatus, int, int),
  void *userdata
)
{
  pthread_mutexattr_t mutexattr;
  pthread_attr_t pthreadattr;
  struct mq_attr mqattr;
  mode_t mode;
  int oflags;
  char name[64];
  char *busid = getenv(pwr_dEnvBusId);
  static int initDone = 0;
  int policy;
  struct sched_param param;

  errl_log_cb = log_cb;
  errl_log_userdata = userdata;
  
  if (initDone)
    return;

  
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
  pthread_mutexattr_destroy(&mutexattr);

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

  pthread_attr_init(&pthreadattr);
  if (pthread_create(&tid, &pthreadattr, log_thread, NULL) == -1) {
    perror("rt_logmod: pthread_create ");
    pthread_attr_destroy(&pthreadattr);  
    return;
  }
  pthread_attr_destroy(&pthreadattr); 
  	  
  param.sched_priority -= 1;
  pthread_setschedparam(tid, policy, &param);

  if (termName && *termName)
    errl_SetTerm(termName);

  logToStdout = getenv("PWR_LOG_TO_STDOUT") != NULL ? TRUE : FALSE; 

  initDone = 1;
  
  return;
}
void
errl_Unlink ()
{
  char name[64];
  char *busid = getenv(pwr_dEnvBusId);

  pthread_cancel(tid);

  sprintf(name, "%s_%s", LOG_QUEUE_NAME, busid ? busid : "");  

  /* We don't care about return status */
  mq_unlink(name);
}
void
errl_SetFile (
  const char *logFileName)
{
  struct timeval time;
  struct timezone zone;
  int retval;
  struct tm *tmv;
  char actFileName[256];

  pwr_tStatus sts = 1;
  int oflags = O_CREAT | O_APPEND | O_WRONLY;
  int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

  pthread_mutex_lock(&fileMutex);
  if (logFile != -1) {
    close(logFile);
    logFile = -1;
  }

  // Add adte & time to logfile name

  retval = gettimeofday(&time, &zone);
  tmv = localtime(&time.tv_sec);

  sprintf(actFileName, "%s.%02d%02d%02d%02d%02d%02d",
	logFileName, tmv->tm_year % 100, tmv->tm_mon + 1, 
	tmv->tm_mday, tmv->tm_hour, tmv->tm_min, tmv->tm_sec);

  if ((logFile = open(actFileName, oflags, mode)) == -1) {
    errh_Error("Cannot open log file: %s", actFileName);
    sts = 2;
  } else {
    errh_Info("Logging to %s", actFileName);
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
  localtime_r(&t, &tmpTm);

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
  errh_sMsg buf;

  while (1) {
    len = mq_receive(mqid, (char *)&buf, LOG_MAX_MSG_SIZE, NULL);
    if (len == -1) {
      if (errno != EINTR)
        perror("rt_logmod.c: mq_receive ");
    } else {
      switch ( buf.message_type) {
      case errh_eMsgType_Log:
	len -= (sizeof(buf) - sizeof(buf.str) - sizeof(buf.message_type) + 1);
	buf.str[len] = 0;
	pthread_mutex_lock(&fileMutex);
	if (logFile != -1) {			    			       
	  /* Set up a timer if you want better performance, ML */
	  CheckTimeStamp(newLogFile); 
	  newLogFile = 0; 	   
	  write(logFile, buf.str, len);
	  write(logFile, "\n", 1);
	}
	pthread_mutex_unlock(&fileMutex);
	
	pthread_mutex_lock(&termMutex);
	if (term != -1) {			    	
	  write(term, buf.str, len);
	  write(term, "\n", 1);
	}
	pthread_mutex_unlock(&termMutex);

	if (logToStdout)
	  printf("%.*s\n", len, buf.str);

	if ( errl_log_cb)
	  (errl_log_cb)( errl_log_userdata, buf.str, buf.severity, buf.sts, buf.anix, buf.message_type);
	break;
      case errh_eMsgType_Status:
	if ( errl_log_cb)
	  (errl_log_cb)( errl_log_userdata, 0, 0, buf.sts, buf.anix, buf.message_type);
      }
    }
  }
}




