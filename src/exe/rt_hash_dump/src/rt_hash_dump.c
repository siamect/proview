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
