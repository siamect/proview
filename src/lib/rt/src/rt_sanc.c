/* rt_sanc.c -- Subscribed alarm (and block) notification, client side.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   <Description>.  */

#include <stdlib.h>
#include <string.h>
#include "pwr.h"
#include "rt_net.h"
#include "rt_pool.h"
#include "rt_gdb.h"
#include "rt_san.h"
#include "rt_san_msg.h"
#include "rt_sanc.h"


pwr_tBoolean
sanc_Subscribe (
  pwr_tStatus		*sts,
  gdb_sObject		*op
)
{
  gdb_sVolume		*vp;
  gdb_sNode		*np;

  if (sts != NULL) *sts = 1;

  gdb_AssumeLocked;

  if (op->l.flags.b.isNative) pwr_Return(NO, sts, SAN__NATIVE);

  if (op->u.c.flags.m & (gdb_mCo_sancAdd | gdb_mCo_sancAct)) {
    op->u.c.sanexp = 0;
    return YES;  
  }

  vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
  np = pool_Address(NULL, gdbroot->pool, vp->l.nr);

  if (op->u.c.flags.b.sancRem) {
    pool_Qremove(NULL, gdbroot->pool, &op->u.c.sanc_ll);
    op->u.c.flags.b.sancRem = 0;
    pwr_Assert(np->sancRem_lc != 0);
    np->sancRem_lc--;
    pool_QinsertPred(NULL, gdbroot->pool, &op->u.c.sanc_ll, &np->sancAct_lh);
    op->u.c.flags.b.sancAct = 1;
    np->sancAct_lc++;
    return YES;
  }

  pwr_Assert(!pool_QisLinked(NULL, gdbroot->pool, &op->u.c.sanc_ll));

  pool_QinsertPred(NULL, gdbroot->pool, &op->u.c.sanc_ll, &np->sancAdd_lh);
  op->u.c.flags.b.sancAdd = 1;
  memset(&op->l.al, 0, sizeof(op->l.al));
  np->sancAdd_lc++;
  ++gdbroot->db->sancid.rix;
  op->u.c.sanid = gdbroot->db->sancid;
  op->u.c.sanexp = 0;

  return YES;
}


void
sanc_SubscribeMountServers (
  pwr_tStatus		*status,
  gdb_sNode		*np
)
{
  gdb_sMountServer 	*msp;
  pool_sQlink		*msl;		
  gdb_sObject		*op;

  pwr_dStatus(sts, status, 1);

  gdb_AssumeLocked;

  for (
    msl = pool_Qsucc(NULL, gdbroot->pool, &np->nodms_lh);
    msl != &np->nodms_lh;
    msl = pool_Qsucc(NULL, gdbroot->pool, msl)
  ) {
    msp = pool_Qitem(msl, gdb_sMountServer, nodms_ll);

    op = pool_Address(sts, gdbroot->pool, msp->msor);
    if (op == NULL) continue;

    sanc_Subscribe(sts, op);
  }

  *sts = 1;
}
