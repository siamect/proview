/* rt_syi.c -- System information

   PROVIEW/R
   Copyright (C) 2000 by Mandator AB.

   This module gives information about the system.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include "pwr.h"
#include "co_errno.h"
#include "rt_syi.h"
#include "rt_syi_msg.h"


char *
syi_HostName (
  pwr_tStatus *status,
  char *ibuffer,
  int isize
)
{

  return syi_NodeName(status, ibuffer, isize);
}

char *
syi_NodeName (
  pwr_tStatus *status,
  char *ibuffer,
  int isize
)
{
  char* cp;
  pwr_dStatus(sts, status, SYI__SUCCESS);

  if (gethostname(ibuffer, isize) != 0) {
    if (errno == EINVAL) {
      *sts = SYI__TRUNCATED;
    } else { 
      *sts = errno_Status(errno);
      ibuffer = NULL;
    }
  }
  
  /* Remove domain */
  if ((cp = strchr(ibuffer, '.')))
    *cp = 0;

  return ibuffer;
}

char *
syi_Ethernet (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  pwr_dStatus(sts, status, SYI__NYI);

  return NULL;
}

char *
syi_NodeSpec (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  pwr_dStatus(sts, status, SYI__NYI);

  return NULL;
}

char *
syi_HostSpec (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{

  return syi_NodeSpec(status, ibuffer, size);
}

/* Return true if node is booted locally or
   false if booted from remote node via network.  */

pwr_tBoolean
syi_LocalBoot (
  pwr_tStatus *status
)
{

  return YES;
}

/* .  */

char*
syi_BootDisk (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  pwr_dStatus(sts, status, SYI__NYI);

  return NULL;
}
