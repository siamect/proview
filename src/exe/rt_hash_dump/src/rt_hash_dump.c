/* 
 * Proview   $Id: rt_hash_dump.c,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#if defined OS_ELN
# include $vaxelnc
#else
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
#endif

#include "rt_gdh.h"
#include "rt_gdb.h"
#include "rt_hash.h"

int main ()
{
  pwr_tStatus sts;

  sts = gdh_Init("pwr_hash_dump");
  if (EVEN(sts)) {
    exit (sts);
  }

  printf("subc_ht, Subscription Clients\n");
  hash_Print(&sts, gdbroot->subc_ht);
  printf("subs_ht, Subscription Servers\n");
  hash_Print(&sts, gdbroot->subs_ht);
  printf("sans_ht, Subscribed Alarm Notification Servers\n");
  hash_Print(&sts, gdbroot->sans_ht);
  printf("oid_ht,  Objects\n");
  hash_Print(&sts, gdbroot->oid_ht);
  printf("vid_ht,  Volumes\n");
  hash_Print(&sts, gdbroot->vid_ht);
  printf("vn_ht,   Volumne Names\n");
  hash_Print(&sts, gdbroot->vn_ht);
  printf("cid_ht,  Classes\n");
  hash_Print(&sts, gdbroot->cid_ht);
  printf("tid_ht,  Types\n");
  hash_Print(&sts, gdbroot->tid_ht);
  printf("nid_ht,  Nodes\n");
  hash_Print(&sts, gdbroot->nid_ht);
  printf("family,  Object Names\n");
  hash_Print(&sts, gdbroot->family_ht);
  printf("ms_ht,   Mount Servers\n");
  hash_Print(&sts, gdbroot->ms_ht);
  printf("as_ht,   Alias Servers\n");
  hash_Print(&sts, gdbroot->as_ht);

  exit(0);
}
