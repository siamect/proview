/* co_utl_batch.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <stsdef>
#include <stddef.h>
#include <string.h>
#include <sjcdef.h>
#include <quidef.h>
#include <jbcmsgdef.h>
#include <starlet.h>
#include "pwr.h"
#include "co_utl_batch.h"

typedef union u_Itmlst {
  struct {
    short   buflen;
    short   code;
    char    *bufadr;
    int     retadr;
  } itm;
  int	    end;
} uItmlst;

typedef struct s_Iosb {
  pwr_tStatus sts;
  int zero;
} sIosb;

#define cProgName "cs_utl_batch_dispatcher"

pwr_tStatus
utl_BatchFindQueue (
  char *Queue
) {
  pwr_tStatus	sts;
  uItmlst	slist[3];
  sIosb		iosb;
  int		qstatus = 0;
  int		i = 0;
  const int	qstopped =
    QUI$M_QUEUE_PAUSED    | QUI$M_QUEUE_PAUSING |
    QUI$M_QUEUE_STALLED   | QUI$M_QUEUE_STOPPED |
    QUI$M_QUEUE_STOPPING  | QUI$M_QUEUE_UNAVAILABLE |
    QUI$M_QUEUE_CLOSED;  
  slist[i].itm.buflen = strlen(Queue);
  slist[i].itm.code = QUI$_SEARCH_NAME;
  slist[i].itm.bufadr = Queue;
  slist[i].itm.retadr = 0;
  i++;
  slist[i].itm.buflen = sizeof(qstatus);
  slist[i].itm.code = QUI$_QUEUE_STATUS;
  slist[i].itm.bufadr = (char *) &qstatus;
  slist[i].itm.retadr = 0;
  i++;
  slist[i].end = 0;

  sts = sys$getquiw(0, QUI$_DISPLAY_QUEUE, 0, slist, &iosb, 0, 0);
  if ((qstatus & qstopped) != 0) sts = JBC$_NOSUCHQUE | STS$M_INHIB_MSG;
  if (sts & 1) sts = iosb.sts;
  if (!(sts & 1))
    printf("!! Queue: \"%s\" is not running\n   Check with your system manager.\n", Queue);

  return sts;
}

pwr_tStatus
utl_BatchWaitJob (
  int  *Entry
){
  uItmlst slist[20];
  sIosb iosb;
  pwr_tStatus sts;
  int i = 0;
  
  slist[i].itm.buflen = sizeof(*Entry);
  slist[i].itm.code = SJC$_ENTRY_NUMBER;
  slist[i].itm.bufadr = (char *) Entry;
  slist[i].itm.retadr = 0;
  i++;
  slist[i].end = 0;


  sts = sys$sndjbcw(0, SJC$_SYNCHRONIZE_JOB, 0, slist, &iosb, 0, 0);  
  if (!(sts & 1)) return sts;

  return iosb.sts;
}

pwr_tStatus
utl_BatchEnterJob (
  int  *Entry,
  char *Queue,
  char *File,
  char *Logfile,
  char *Par[]
){
  const int maxPar = 8;
  uItmlst slist[20];
  sIosb iosb;
  int sts;
  int i = 0;
  int j = 0;
  
  slist[i].itm.buflen = strlen(Queue);
  slist[i].itm.code = SJC$_QUEUE;
  slist[i].itm.bufadr = Queue;
  slist[i].itm.retadr = 0;
  i++;
  slist[i].itm.buflen = strlen(File);
  slist[i].itm.code = SJC$_FILE_SPECIFICATION;
  slist[i].itm.bufadr = File;
  slist[i].itm.retadr = 0;
  i++;
#if 0
  slist[i].itm.buflen = 0;
  slist[i].itm.code = SJC$_NOTIFY;
  slist[i].itm.bufadr = 0;
  slist[i].itm.retadr = 0;
  i++;
#endif
  slist[i].itm.buflen = 0;
  slist[i].itm.code = SJC$_NO_LOG_DELETE;
  slist[i].itm.bufadr = 0;
  slist[i].itm.retadr = 0;
  i++;
  slist[i].itm.buflen = sizeof(*Entry);
  slist[i].itm.code = SJC$_ENTRY_NUMBER_OUTPUT;
  slist[i].itm.bufadr = (char *) Entry;
  slist[i].itm.retadr = 0;
  i++;
  slist[i].itm.buflen = 0;
  slist[i].itm.code = SJC$_NO_LOG_SPOOL;
  slist[i].itm.bufadr = 0;
  slist[i].itm.retadr = 0;
  i++;
  slist[i].itm.buflen = strlen(Logfile);
  slist[i].itm.code = SJC$_LOG_SPECIFICATION;
  slist[i].itm.bufadr = Logfile;
  slist[i].itm.retadr = 0;
  i++;

#ifdef __DECC
#define AssignParameter(parnum) {\
  if (Par[j] == NULL || j >= maxPar) break;\
  slist[i].itm.buflen = strlen(Par[j]);\
  slist[i].itm.code = SJC$_PARAMETER_ ## parnum;\
  slist[i].itm.bufadr = Par[j];\
  slist[i].itm.retadr = 0;\
  i++, j++;}
#else
#define AssignParameter(parnum)\
  if (Par[j] == NULL || j >= maxPar) break;\
  slist[i].itm.buflen = strlen(Par[j]);\
  slist[i].itm.code = SJC$_PARAMETER_/**/parnum;\
  slist[i].itm.bufadr = Par[j];\
  slist[i].itm.retadr = 0;\
  i++, j++
#endif

  while (1) {
    AssignParameter(1);
    AssignParameter(2);
    AssignParameter(3);
    AssignParameter(4);
    AssignParameter(5);
    AssignParameter(6);
    AssignParameter(7);
    AssignParameter(8);
  }

  slist[i].end = 0;

  sts = sys$sndjbcw(0, SJC$_ENTER_FILE, 0, slist, &iosb, 0, 0);  
  if (!(sts & 1)) return sts;
  return iosb.sts;
}
