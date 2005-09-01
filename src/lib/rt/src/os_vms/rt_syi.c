/* 
 * Proview   $Id: rt_syi.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_syi.c -- System information
   This module gives information about the system.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syidef.h>
#include <starlet.h>
#include <lib$routines.h>
#include <lnmdef.h>
#include <descrip.h>

#include "pwr.h"
#include "pwr_vms.h"
#include "rt_errh.h"
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
  short rlen = 0;
  int iosb[2];
  pwr_tStatus lsts = 1;
  vms_sItemList il[2];
  static char buffer[32];
  int size;
  int len;

  pwr_dStatus(sts, status, SYI__SUCCESS);

  VMS_ITEM(il[0], sizeof(buffer) - 1, SYI$_NODENAME, buffer, &rlen);
  VMS_ITEM_NULL(il[1]);

  *sts = sys$getsyiw(0, NULL, NULL, il, iosb, NULL, 0);
  if (EVEN(*sts) || EVEN(iosb[0])) {
    *sts = iosb[0];
    return NULL;
  }

  buffer[rlen] = '\0';

  if (ibuffer != NULL && isize > 0) {
    len = MIN(isize - 1, rlen);
    memcpy(ibuffer, buffer, len);
    if (len < rlen)
      *sts = SYI__TRUNCATED;
    return ibuffer;
  } else {
    return buffer;
  }

}

char *
syi_Ethernet (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{

  pwr_dStatus(sts, status, SYI__NYI);

  return ibuffer;
}

char *
syi_NodeSpec (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
#if 0
  static char buffer[] = "63.1023";
  int len;
#endif

  pwr_dStatus(sts, status, SYI__NYI);

  return ibuffer;
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

  pwr_dStatus(sts, status, SYI__SUCCESS);

  return ibuffer;
}
