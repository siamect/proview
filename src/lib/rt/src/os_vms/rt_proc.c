/* rt_proc.c -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */

#if !defined(OS_VMS)
# error "This file is valid only for OS_VMS"
#endif

#include <descrip.h>
#include <string.h>
#include "pwr.h"
#include "co_cdh.h"
#include "co_errno.h"
#include "rt_proc.h"
#include "rt_proc_msg.h"
#include "rt_errh.h"


pwr_tStatus
proc_Load (
  proc_sProcess *p
)
{
  pwr_tStatus sts = PROC__SUCCESS;

  return sts;
}

pwr_tStatus
proc_Start (
  proc_sProcess *p
)
{
  pwr_tStatus sts;
  char *fmt;
  char nameb[40];
  char buf[512];
  int flags;
  $DESCRIPTOR(com,"");
  $DESCRIPTOR(name,"");

  cdh_ToUpper(nameb, p->name);
  name.dsc$w_length = strlen(nameb);
  name.dsc$a_pointer = nameb;

  if (p->flags.b.user)
    fmt = "@pwr_exe:pwrp_run %s \"%s\" %d %d \"%s\"";
  else
    fmt = "@pwr_exe:pwr_run %s \"%s\" %d %d \"%s\"";

  com.dsc$w_length = sprintf(buf, fmt, p->file, p->name, p->flags.b.debug, p->p_prio, p->arg);
  com.dsc$a_pointer = buf;
  flags = 1; /* Nowait */
  sts = lib$spawn(&com, NULL, NULL, &flags, &name, &p->pid);

  return sts;
}

pwr_tStatus
proc_SetPriority (
  int prio
)
{
  pwr_tStatus sts = PROC__SUCCESS;

  return sts;
}

pwr_tStatus
proc_UnloadProgram (
  proc_sProcess *p
)
{
  pwr_tStatus sts = PROC__SUCCESS;

  return sts;
}
