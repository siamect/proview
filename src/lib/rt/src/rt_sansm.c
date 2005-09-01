/* 
 * Proview   $Id: rt_sansm.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* rt_sansm.c -- Subscribed alarm (and block) notification, server monitor calls. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pwr.h"
#include "rt_net.h"
#include "rt_gdb.h"
#include "rt_san.h"
#include "rt_sansm.h"
#include "rt_errh.h"

static san_sServer *
addServer (
  pwr_tStatus		*sts,
  gdb_sNode		*np,
  gdb_sObject		*op,
  net_sSanEntry		*ep
);

static san_sServer *
checkServer (
  gdb_sNode		*np,
  san_sServer		*sp
);

static void
removeServer (
  gdb_sNode		*np,
  san_sServer		*sp
);


static san_sServer *
addServer (
  pwr_tStatus		*sts,
  gdb_sNode		*np,
  gdb_sObject		*op,
  net_sSanEntry		*ep
)
{
  san_sServer		*sp;

  sp = hash_Search(NULL, gdbroot->sans_ht, &ep->sid);
  if (sp != NULL) return sp;


  sp = pool_Alloc(sts, gdbroot->pool, sizeof(san_sServer));
  if (sp == NULL) return NULL;

  sp->sane = *ep;
  sp = hash_Insert(sts, gdbroot->sans_ht, sp);
  if (sp == NULL) return NULL;
  /* To do !!!! This is a pool leak !!!!!! */
#if 0
  {
    pool_Free(NULL, gdbroot->pool, sp);
    return NULL;
  }
#endif
  if (gdbroot->is_tmon)
    sp->op = op;
  pool_QinsertPred(NULL, gdbroot->pool, &sp->sansAct_ll, &np->sansAct_lh);
  sp->flags.b.sansAct = 1;
  np->sansAct_lc++;
  np->sans_gen++;

  return checkServer(np, sp);
}


/* This routine is called both from neth and tmon.
   Do NOT use sp->op */   

static san_sServer *
checkServer (
  gdb_sNode		*np,
  san_sServer		*sp
)
{
  gdb_sObject		*op;

  if (!gdbroot->is_tmon || sp->op == NULL) {
    op = hash_Search(NULL, gdbroot->oid_ht, &sp->sane.oid);
    if (op == NULL || !op->l.flags.b.isOwned) {
      removeServer(np, sp);
      return NULL;
    }
    if (gdbroot->is_tmon)
      sp->op = op;

  } else 
    op = sp->op;

  if (op->l.al.idx <= sp->al.idx) return sp;

  if (!sp->flags.b.sansUpd) {
    pool_QinsertPred(NULL, gdbroot->pool, &sp->sansUpd_ll, &np->sansUpd_lh);    
    sp->flags.b.sansUpd = 1;
  }

  if (!op->g.flags.b.isMountClient) {
    sp->al.a = op->l.al.a;
    sp->al.b = op->l.al.b;
    sp->al.maxa = op->u.n.ral.maxa;
    sp->al.maxb = op->u.n.ral.maxb;
  } else
    memset(&sp->al, 0, sizeof(sp->al));

  sp->al.idx = op->l.al.idx;

  return sp;
}

static void
removeServer (
  gdb_sNode		*np,
  san_sServer		*sp
)
{

  hash_Remove(NULL, gdbroot->sans_ht, sp);

#if 0 /* In next version we will maintain an sancount in the object header
	 ala dlcount.  */
  if (--sp->op.u.n.sansCount == 0 && sp->op->u.n.flags.pendingDelete)
    gdb_UnlockObject();
#endif

  if (sp->flags.b.sansAct) {
    pool_Qremove(NULL, gdbroot->pool, &sp->sansAct_ll);
    np->sansAct_lc--;
    np->sans_gen++;
  }
  if (sp->flags.b.sansUpd)
    pool_Qremove(NULL, gdbroot->pool, &sp->sansUpd_ll);

  pool_Free(NULL, gdbroot->pool, sp);
}

void
sansm_Add (
  qcom_sGet		*get
)
{
  gdb_sObject		*op;
  gdb_sNode		*np;
  san_sServer		*sp;
  int			i;
  net_sSanAdd		*ap = get->data;
  
  gdb_AssumeUnlocked;

  gdb_ScopeLock {

    np = hash_Search(NULL, gdbroot->nid_ht, &ap->hdr.nid);
    if (np == NULL) break;

    for (i = 0; i < ap->count; i++) {
      op = hash_Search(NULL, gdbroot->oid_ht, &ap->sane[i].oid);
      if (op == NULL || !op->l.flags.b.isOwned) continue;

      sp = addServer(NULL, np, op, &ap->sane[i]);
      if (sp == NULL) continue;
    }

  } gdb_ScopeUnlock;
#if 0
  errh_Info("sansm_Add, received %d add from %s, sansAct_lc = %d",
    ap->count, np->name, np->sansAct_lc);
#endif
}

void
sansm_Check (
)
{
  static unsigned int	idx = 0;
  gdb_sNode		*np;
  pool_sQlink		*nl;
  san_sServer		*sp;
  pool_sQlink		*sl;

  gdb_AssumeLocked;

  if (idx == gdbroot->db->al_idx) return;

  for (
    nl = pool_Qsucc(NULL, gdbroot->pool, &gdbroot->db->nod_lh);
    nl != &gdbroot->db->nod_lh;
  ) {
    np = pool_Qitem(nl, gdb_sNode, nod_ll);
    nl = pool_Qsucc(NULL, gdbroot->pool, nl);

    if (!np->flags.b.connected) continue;

    for (
      sl = pool_Qsucc(NULL, gdbroot->pool, &np->sansAct_lh);
      sl != &np->sansAct_lh;
    ) {
      sp = pool_Qitem(sl, san_sServer, sansAct_ll);
      sl = pool_Qsucc(NULL, gdbroot->pool, sl);

      checkServer(np, sp);
    }
  }

  idx = gdbroot->db->al_idx;
}

void
sansm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
)
{
  san_sServer		*sp;
  pool_sQlink		*sl;

  gdb_AssumeLocked;

  for (
    sl = pool_Qsucc(NULL, gdbroot->pool, &np->sansAct_lh);
    sl != &np->sansAct_lh;
  ) {
    sp = pool_Qitem(sl, san_sServer, sansAct_ll);
    sl = pool_Qsucc(NULL, gdbroot->pool, sl);

    removeServer(np, sp);
  }

  if (sts != NULL) *sts = 1;

  return;
}

void
sansm_Remove (
  qcom_sGet		*get
)
{
  gdb_sNode		*np;
  san_sServer		*sp;
  int			i;
  net_sSanRemove	*rp = get->data;

  int old_cnt;  

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    np = hash_Search(NULL, gdbroot->nid_ht, &rp->hdr.nid);
    if (np == NULL) break;

    old_cnt = np->sansAct_lc;

    for (i = 0; i < rp->count; i++) {
      sp = hash_Search(NULL, gdbroot->sans_ht, &rp->sid[i]);
      if (sp == NULL) continue;

      removeServer(np, sp);
    }

  } gdb_ScopeUnlock;
#if 0
  errh_Info("sansm_Remove, %d remove from %s. %d -> %d", 
    rp->count, np->name, old_cnt, np->sansAct_lc);
#endif

}

/* Send alarm notification for server in update list.
   Send at most net_cSanMaxUpdate 'net_sSanData' in update message.  */

pwr_tUInt32
sansm_Update (
  gdb_sNode		*np
)
{
  static net_sSanUpdate	*up = NULL;
  static san_sServer	**spl = NULL;
  int			i;
  pwr_tStatus		sts;
  pwr_tUInt32		gen;
  pool_sQlink		*sl;
  san_sServer		*sp;
  qcom_sQid		tgt;
  

  gdb_AssumeLocked;

  if (pool_QisEmpty(NULL, gdbroot->pool, &np->sansUpd_lh)) return 0;

  if (up == NULL) {
    up  = malloc(sizeof(*up) + (net_cSanMaxUpdate - 1) * sizeof(up->data[0]));
    spl = malloc(sizeof(san_sServer *) * net_cSanMaxUpdate);
  }

  for (
    i = 0, sl = pool_Qsucc(NULL, gdbroot->pool, &np->sansUpd_lh);
    sl != &np->sansUpd_lh && i < net_cSanMaxUpdate;
    i++
  ) {
    sp = pool_Qitem(sl, san_sServer, sansUpd_ll);
    sl = pool_Qsucc(NULL, gdbroot->pool, sl);

    pwr_Assert(sp->flags.b.sansUpd);

#if 0
    pool_Qremove(NULL, gdbroot->pool, &sp->sansUpd_ll);
    sp->flags.b.sansUpd = 0;
#endif

    up->data[i].al = sp->al;
    up->data[i].sane = sp->sane;
    spl[i] = sp;
  }

  up->count = i;

  tgt.nid = np->nid;
  tgt.qix = net_cProcHandler;

  gen = np->sans_gen;

  gdb_Unlock;
#if 0
    errh_Info("sansm_Update, put %d to %s", up->count, np->name);
#endif
    net_Put(&sts, &tgt, up, net_eMsg_sanUpdate, pwr_Offset(up, data[i]));
  gdb_Lock;

  if (EVEN(sts)) return 0;


  if (gen == np->sans_gen) {

    for (i = 0; i < up->count; i++) {
      sp = spl[i];
      pwr_Assert(sp->flags.b.sansUpd);
      pool_Qremove(NULL, gdbroot->pool, &sp->sansUpd_ll);
      sp->flags.b.sansUpd = 0;
    }

  } else {             

    for (i = 0; i < up->count; i++) {
      sp = hash_Search(NULL, gdbroot->sans_ht, &up->data[i].sane.sid);
      if (sp != NULL) {
        pwr_Assert(sp->flags.b.sansUpd);
      	pool_Qremove(NULL, gdbroot->pool, &sp->sansUpd_ll);
      	sp->flags.b.sansUpd = 0;
      }
    }
  }

  return up->count;
}
