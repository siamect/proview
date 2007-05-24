/* 
 * Proview   $Id: co_syi.c,v 1.1 2007-05-24 14:48:38 claes Exp $
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

/* co_syi.c -- System information

   This module gives information about the system.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include "pwr.h"
#include "co_errno.h"
#include "co_syi.h"
#include "co_syi_msg.h"


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
