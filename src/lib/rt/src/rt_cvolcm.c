/* rt_cvolcm.c -- Cached volumes, client monitor API.

   PROVIEW/R
   Copyright (C) 1997-98 by Mandator AB.

   .  */

#if defined(OS_ELN)
# include $vaxelnc
#else
# include <stdio.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_gdh_msg.h"
#include "rt_pool.h"
#include "rt_vol.h"
#include "rt_net.h"
#include "rt_cvol.h"
#include "rt_cvolcm.h"



/**
 * Adds a remote class volume.
 */
void 
cvolcm_AddClassVolume(
  pwr_tStatus		*sts,
  gdb_sNode		*np,
  const net_sGvolume	*vp
)
{
  gdb_sCclassVolume	*ccvp;
  gdb_sVolume		*cvp;
  

  gdb_AssumeLocked;
  pwr_Assert(vp != NULL);



  ccvp = pool_Alloc(sts, gdbroot->pool, sizeof(*ccvp));
  if (ccvp == NULL) return;

  ccvp->key.nid = vp->nid;
  ccvp->key.vid = vp->vid;
  ccvp->time = vp->time;
  
  cvp = hash_Search(sts, gdbroot->vid_ht, &vp->vid);
  if (cvp == NULL) /* This volume doesn't exist locally, but we may create it later on */
    ccvp->equalClasses = 0;
  else
    ccvp->equalClasses = time_Acomp(&ccvp->time, &cvp->g.time) == 0 ? 1 : 0;

  
  ccvp = hash_Insert(sts, gdbroot->ccvol_ht, ccvp);
  if (ccvp == NULL) errh_Bugcheck(GDH__WEIRD, "adding cached class volume");
  
  pool_QinsertPred(NULL, gdbroot->pool, &ccvp->ccvol_ll, &np->ccvol_lh);

}


/* .  */

gdb_sVolume *
cvolcm_ConnectVolume (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  net_sGvolume		*gvp,
  gdb_sNode		*np
)
{
  gdb_sTouchQ		*tqp;
  gdb_sVolume		*ovp;

  gdb_AssumeLocked;

  if (vp->g.nid != pwr_cNNodeId && vp->g.nid != np->nid) {
    errh_Error("Volume %s (%s) is loaded from another node.\nCurrent: %s, new: %s",
      vp->g.name.orig, cdh_VolumeIdToString(NULL, vp->g.vid, 1, 0),
      cdh_NodeIdToString(NULL, vp->g.nid, 1, 0),
      cdh_NodeIdToString(NULL, np->nid, 1, 0));
    return NULL;
  }

  vp->g = *gvp;

  pwr_Assert(vp->g.nid != pwr_cNNodeId);

  vp->l.nr = pool_Reference(NULL, gdbroot->pool, np);

  /* Add volume name to hash table.  */

  ovp = hash_Search(NULL, gdbroot->vn_ht, &vp->g.name);
  if (ovp != NULL) {
    if (ovp != vp) {
      errh_Warning("Volume name allready exist: %s, vid: %x\n", vp->g.name.orig, vp->g.vid);
      hash_Remove(NULL, gdbroot->vn_ht, ovp);
      ovp = hash_Insert(sts, gdbroot->vn_ht, vp);
      pwr_Assert(ovp == vp);  
    }
  } else {
    ovp = hash_Insert(sts, gdbroot->vn_ht, vp);
    pwr_Assert(ovp == vp);  
  }
  vp->l.flags.b.inVnTable = 1;

  pwr_Assert(!vp->l.flags.b.inOwnList);  
  pool_QinsertPred(sts, gdbroot->pool, &vp->l.own_ll, &np->own_lh);
  vp->l.flags.b.inOwnList = 1;

  /* Initiate touch queues.  */

  tqp = &vp->u.c.cacheLock;
  pool_Qinit(NULL, gdbroot->pool, &tqp->lh);
  tqp->lc_max = 0;
  tqp->flags.b.cacheLock = 1;
  tqp->next = pool_cNRef;

  tqp = &vp->u.c.cacheVol;
  pool_Qinit(NULL, gdbroot->pool, &tqp->lh);
#if defined OS_ELN
  tqp->lc_max = 100;
#else
  tqp->lc_max = 200;
#endif
  tqp->flags.b.cacheVol = 1;
  tqp->next = pool_Reference(NULL, gdbroot->pool, &np->cacheNode);

  vp->l.flags.b.isConnected = 1;
  vp->l.flags.b.isCached = 1;
  vp->l.flags.b.netCached = 1;
  vp->l.flags.b.remote = 1;
  
  return vp;
}

/* .  */

void
cvolcm_CreateObject (
  qcom_sGet		*get
)
{
  pwr_tStatus		sts;
  gdb_sObject		*op;
  gdb_sObject		*pop;
  gdb_sObject		*sop;
  pwr_tObjid		soid;
  net_sCreateObject	*cop = (net_sCreateObject *) get->data;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    op = hash_Search(&sts, gdbroot->oid_ht, &cop->notify.oid);
    if (op != NULL) errh_Bugcheck(GDH__WEIRD, "cvolcm_CreateObject: object already exists");

    pop = hash_Search(&sts, gdbroot->oid_ht, &cop->par.oid);
    if (pop == NULL) break;

    pop->g.soid = cop->par.soid;
    pop->g.flags.b.isParent = 1;

    soid = cop->par.oid;
    soid.oix = cop->sib.blink;
    sop = hash_Search(&sts, gdbroot->oid_ht, &soid);
    if (sop != NULL)
      sop->g.sib.flink = cop->sib.newflink;

    soid.oix = cop->sib.flink;
    sop = hash_Search(&sts, gdbroot->oid_ht, &soid);
    if (sop != NULL)
      sop->g.sib.blink = cop->sib.newblink;
  } gdb_ScopeUnlock;
}

/* .  */

void
cvolcm_DeleteObject (
  qcom_sGet		*get
)
{
  pwr_tStatus		sts;
  gdb_sObject		*op;
  gdb_sObject		*pop;
  gdb_sObject		*sop;
  pwr_tObjid		soid;
  gdb_sVolume		*vp;
  net_sDeleteObject	*dop = (net_sDeleteObject *) get->data;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    pop = hash_Search(&sts, gdbroot->oid_ht, &dop->par.oid);
    op = hash_Search(&sts, gdbroot->oid_ht, &dop->notify.oid);
    if (op != NULL) {
      pwr_Assert(cdh_ObjidIsEqual(op->g.f.poid, dop->par.oid)); 
      vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
      cvol_FreeObject(&sts, vp, op, vol_mLink_cacheDelete);
    }

    if (pop == NULL) break;

    pop->g.soid = dop->par.soid;
    if (cdh_ObjidIsNull(pop->g.soid)) {
      pop->g.flags.b.isParent = 0;
    }

    soid = dop->par.oid;
    soid.oix = dop->sib.blink;
    sop = hash_Search(&sts, gdbroot->oid_ht, &soid);
    if (sop != NULL)
      sop->g.sib.flink = dop->sib.newflink;

    soid.oix = dop->sib.flink;
    sop = hash_Search(&sts, gdbroot->oid_ht, &soid);
    if (sop != NULL)
      sop->g.sib.blink = dop->sib.newblink;
  } gdb_ScopeUnlock;
}

/* Flush all cached information about a node.  */

void
cvolcm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np 
)
{
  pool_sQlink		*vl;
  gdb_sVolume		*vp;
  gdb_sCclassVolume	*cvp;
  pwr_tStatus		lsts;

  gdb_AssumeLocked;
  pwr_Assert(np != gdbroot->my_node && np != gdbroot->no_node);

  for (
    vl = pool_Qsucc(NULL, gdbroot->pool, &np->own_lh);
    vl != &np->own_lh;
    vl = pool_Qsucc(NULL, gdbroot->pool, &np->own_lh)
  ) {
    vp = pool_Qitem(vl, gdb_sVolume, l.own_ll);
    pwr_Assert(vp->l.flags.b.isCached);
    if (vp->l.flags.b.isCached)
      cvolcm_FlushVolume(NULL, vp);
  }


  for (
    vl = pool_Qsucc(NULL, gdbroot->pool, &np->ccvol_lh);
    vl != &np->ccvol_lh;
    vl = pool_Qsucc(NULL, gdbroot->pool, &np->ccvol_lh)
  ) {
    cvp = pool_Qitem(vl, gdb_sCclassVolume, ccvol_ll);

    hash_Remove(&lsts, gdbroot->ccvol_ht, cvp);
    if (EVEN(lsts)) errh_Bugcheck(lsts, "cached class volume inconsistency");
    

    pool_Qremove(NULL, gdbroot->pool, &cvp->ccvol_ll);
    pool_Free(NULL, gdbroot->pool, cvp);

  }
}

/* Remove all objects in a cached volume.
   
   A volume flush is done only on order
   from the host node. Thus it is safe to remove
   all subscribed alarm notifi
   !!! To do !!! Take care about alarm update.  */

void
cvolcm_FlushVolume (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp 
)
{
  pool_sQlink		*ol;
  gdb_sObject		*op;
  gdb_sMountServer	*msp;

  gdb_AssumeLocked;

  for (
    ol = pool_Qsucc(NULL, gdbroot->pool, &vp->l.obj_lh);
    ol != &vp->l.obj_lh;
    ol = pool_Qsucc(NULL, gdbroot->pool, &vp->l.obj_lh)
  ) {
    op = pool_Qitem(ol, gdb_sObject, l.obj_ll);
    if (op->l.flags.b.isMountServer) {
      msp = hash_Search(sts, gdbroot->ms_ht, &op->g.oid);
      if (msp == NULL) errh_Bugcheck(*sts, "mount server inconsitency");
      msp->msor = pool_cNRef;
      pool_Qremove(NULL, gdbroot->pool, &msp->nodms_ll);
      /* Todo !!! Clear alarm and blocklevel in all mount clients. */
    }
    cvol_FlushObject(op);
  }
  
  pwr_Assert(vp->l.flags.b.inOwnList);
  pool_Qremove(NULL, gdbroot->pool, &vp->l.own_ll);
  vp->l.flags.b.inOwnList = 0;
  vp->g.nid = pwr_cNNodeId;

  pwr_Assert(pool_QisEmpty(NULL, gdbroot->pool, &vp->u.c.cacheLock.lh));
  pwr_Assert(vp->u.c.cacheLock.lc == 0);
  pwr_Assert(pool_QisEmpty(NULL, gdbroot->pool, &vp->u.c.cacheVol.lh));
  pwr_Assert(vp->u.c.cacheVol.lc == 0);

  vp->l.flags.b.isConnected = 0;
  vp->l.flags.b.isCached = 0;
  vp->l.flags.b.netCached = 0;
  vp->l.flags.b.remote = 0;
}

/* .  */

void
cvolcm_MoveObject (
  qcom_sGet		*get
)
{
  pwr_tStatus		sts;
  gdb_sObject		*op;
  gdb_sObject		*pop;
  gdb_sObject		*sop;
  pwr_tObjid		soid;
  gdb_sVolume		*vp;
  net_sMoveObject	*mop = (net_sMoveObject *) get->data;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    op = hash_Search(&sts, gdbroot->oid_ht, &mop->notify.oid);
    if (op != NULL) {
      pop = hash_Search(&sts, gdbroot->oid_ht, &mop->npar.oid);
      vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
      if (pop == NULL) {
	/* New father is not in the cache => delete object from cache.  */
	cvol_FreeObject(NULL, vp, op, vol_mLink_cacheDelete);
	op = NULL;
      } else {
	cvol_UnlinkObject(&sts, vp, op, vol_mLink_cacheMove);
	vol_UnlinkObject(&sts, vp, op, vol_mLink_cacheMove);
      }
    }

    pop = hash_Search(&sts, gdbroot->oid_ht, &mop->opar.oid);
    if (pop != NULL) {
      pop->g.soid = mop->opar.soid;

      soid = mop->opar.oid;

      soid.oix = mop->osib.blink;
      sop = hash_Search(&sts, gdbroot->oid_ht, &soid);
      if (sop != NULL)
	sop->g.sib.flink = mop->osib.newflink;

      soid.oix = mop->osib.flink;
      sop = hash_Search(&sts, gdbroot->oid_ht, &soid);
      if (sop != NULL)
	sop->g.sib.blink = mop->osib.newblink;
    }

    pop = hash_Search(&sts, gdbroot->oid_ht, &mop->npar.oid);
    if (pop != NULL) {
      pop->g.soid = mop->npar.soid;

      soid = mop->npar.oid;
      soid.oix = mop->nsib.blink;
      sop = hash_Search(&sts, gdbroot->oid_ht, &soid);
      if (sop != NULL)
	sop->g.sib.flink = mop->nsib.newflink;

      soid.oix = mop->nsib.flink;
      sop = hash_Search(&sts, gdbroot->oid_ht, &soid);
      if (sop != NULL)
	sop->g.sib.blink = mop->nsib.newblink;
    }

    if (op != NULL) {
      vol_LinkObject(&sts, vp, op, vol_mLink_cacheMove);
      cvol_LinkObject(&sts, vp, op, vol_mLink_cacheMove);
    }
  } gdb_ScopeUnlock;
}

/* .  */

void
cvolcm_RenameObject (
  qcom_sGet		*get
)
{
  gdb_sObject		*op;
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sVolume		*vp;
  net_sRenameObject	*rop = (net_sRenameObject *) get->data;

  gdb_AssumeUnlocked;

  gdb_ScopeLock {
    op = hash_Search(&sts, gdbroot->oid_ht, &rop->notify.oid);
    if (op == NULL) break;

    vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
    vol_UnlinkObject(&sts, vp, op, vol_mLink_rename);

    op->g.f = rop->f;

    vol_LinkObject(&sts, vp, op, vol_mLink_rename);
  } gdb_ScopeUnlock;
}

/* Trim the cache to get more free headers.  */

void
cvolcm_TrimOld (
)
{
  gdb_sTouchQ		*oqp = &gdbroot->db->cacheOld;
  gdb_sTouchQ		*fqp = &gdbroot->db->cacheFree;
  pool_sQlink		*ol;
  gdb_sObject		*op;
  gdb_sVolume		*vp;

  gdb_AssumeLocked;

  while (fqp->lc < fqp->lc_min) {
    ol = pool_Qpred(NULL, gdbroot->pool, &oqp->lh);
    if (ol == &oqp->lh) break;
    op = pool_Qitem(ol, gdb_sObject, u.c.cache_ll);
    vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
    cvol_FreeObject(NULL, vp, op, vol_mLink_cacheTrim);
  }
}
