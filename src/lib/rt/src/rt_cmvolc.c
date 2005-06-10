/* rt_cmvolc.c -- Cached class volumes, client side

   PROVIEW/R
   Copyright (C) 1996-2002 by Cell Network AB.

   This module contains the meta cache handling routines.  */

#ifndef OS_ELN
# include <stdio.h>
# include <string.h>
#endif

#if defined(OS_ELN)
# include $vaxelnc
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "co_time.h"
#include "rt_gdh_msg.h"
#include "rt_hash_msg.h"
#include "rt_pool.h"
#include "rt_vol.h"
#include "rt_net.h"
#include "rt_cmvolc.h"
#include "rt_errh.h"



static gdb_sObject *
createObject (
  pwr_tStatus		*status,
  gdb_sVolume		*vp,
  const net_sGobject	*nop,
  const pwr_tTime	*time,
  pwr_tBitMask		dbsFlags,
  const void		*bodyData,
  int			bodySize,
  pwr_tObjid		poid
  );

static gdb_sVolume *
createVolume(
  pwr_tStatus* 		sts,
  const char*		vname,
  pwr_tVolumeId 	cvid
  );


static void
deleteCclass(
  gdb_sCclass 		*ccp
  );

static gdb_sCclass *
linkCclass(
  pwr_tStatus		*sts, /**< Status */
  gdb_sCclass		*ccp  /**< The cached class which shall be linked */
  );


static void
trimQ(
  gdb_sTouchQ 		*tqp
  );



/** Fetches a cached class.
 *  If the class is not in the cache then it's fetched from the remote node 
 *
 *  @return NULL if an error or classes are equal else a pointer to the class
 */
gdb_sCclass *
cmvolc_GetCachedClass (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const gdb_sVolume	*vp,
  mvol_sAttribute	*ap,
  pwr_tBoolean		*equal, /**< set if classes are equal then NULL is returned */
  pwr_tBoolean		*fetched, /**< true if the class has been fected from the remote node */
  gdb_sClass            *cp
)
{
  qcom_sQid		tgt;
  qcom_sPut		put;
  net_sGetCclass	*smp;	     /* Send message.  */
  net_sGetCclassR	*rmp = NULL; /* Receive message.  */
  gdb_sCcVolKey		ccvKey;
  gdb_sCclassKey	ccKey;
  gdb_sCclass		*ccp;
  gdb_sCclassVolume	*ccvp;
  int 			i;
  pwr_tUInt32		size;
  pwr_tUInt32		nextIdx = 0;
  pwr_tUInt32		stopIdx;
  gdb_sObject           *cop;
  pwr_tTime             time;

  gdb_AssumeLocked;
  
  *equal = 0;
  *fetched = 0;
  

  /* Handle nodes that don't support cached classes */
  if (!np->cclassSupport) {
    *equal = 1;
    if (cp == NULL) {
      ap->op->u.c.flags.b.classChecked = 1;
      ap->op->u.c.flags.b.classEqual = 1;
    }
    pwr_Return(NULL, sts, GDH__SUCCESS);
  }
  
  /** @todo Check vp->u.c.equalClasses first (when implemented) */

  ccvKey.nid = np->nid;
  if (cp == NULL)
    ccvKey.vid = ap->op->g.cid >> 16; /* Class Id to Class Volume Id */
  else
    ccvKey.vid = cp->cid >> 16;
  

  ccvp = hash_Search(sts, gdbroot->ccvol_ht, &ccvKey);
  if (ccvp == NULL) {
    errh_Bugcheck(GDH__WEIRD, "Cached Class volume doesn't exist");
    /** @todo Verify that this bugcheck is valid */
  }
  
  /* If cp is not NULL then we always fetch the class */
  
  if (ccvp->equalClasses && cp == NULL) {
    *equal = 1;
    ap->op->u.c.flags.b.classChecked = 1;
    ap->op->u.c.flags.b.classEqual = 1;
    pwr_Return(NULL, sts, GDH__SUCCESS);
  }
  
  if (cp != NULL) {
    ccKey.cid = cp->cid;
    cop = pool_Address(NULL, gdbroot->pool, cp->cor);
    time = cop->u.n.time;
  }
  else {
    ccKey.cid = ap->op->g.cid;
    time = ap->cop->u.n.time;
  }
  ccKey.ccvoltime = ccvp->time;

  ccp = hash_Search(sts, gdbroot->cclass_ht, &ccKey);
  if (ccp != NULL) {
    cmvolc_TouchClass(ccp);

    if (time_Acomp(&time, &ccp->time) == 0 && cp == NULL) {
      *equal = 1;
      ap->op->u.c.flags.b.classChecked = 1;
      ap->op->u.c.flags.b.classEqual = 1;
      pwr_Return(NULL, sts, GDH__SUCCESS);
    } else {
      if (cp == NULL) {
        ap->op->u.c.flags.b.classChecked = 1;
        ap->op->u.c.flags.b.classEqual = 0;
      }
      pwr_Return(ccp, sts, GDH__SUCCESS);
    }
  }

  /* We have to fetch the remote class */

  *fetched = 1;
  
  /* If classes equal, create cached class */
  
  if (ccvp->equalClasses && cp != NULL) {

    *fetched = 0;
    size = sizeof(*ccp) + (cp->acount - 1) * sizeof(ccp->attr[0]);
    ccp = pool_Alloc(sts, gdbroot->pool, size);
    if (ccp == NULL) {
      return NULL;
    }

    *equal = 1;

    ccp->size = cp->size;
    ccp->time = time;


    for (i = 0; i < cp->acount; i++) {
      ccp->attr[i].aix     = cp->attr[i].aix;
      ccp->attr[i].flags   = cp->attr[i].flags;
      ccp->attr[i].type    = cp->attr[i].type;
      ccp->attr[i].offs    = cp->attr[i].offs;
      ccp->attr[i].size    = cp->attr[i].size;
      ccp->attr[i].elem    = cp->attr[i].elem;
      ccp->attr[i].moffset = cp->attr[i].moffset;
    }

    ccp->acount = cp->acount;

  } else {
    do {
      gdb_Unlock;
    
      smp = net_Alloc(sts, &put, sizeof(*smp), net_eMsg_getCclass);
      if (smp == NULL)
        goto netError;
    
      tgt.nid = np->nid;
      tgt.qix = net_cProcHandler;
      smp->ver = net_cVersion;
      smp->cid = ccKey.cid;
      smp->time = time;
      smp->aidx = nextIdx;
    
      rmp = net_Request(sts, &tgt, &put, NULL, net_eMsg_getCclassR);
      if (rmp == NULL || EVEN(rmp->sts))
        goto netError;
    
      gdb_Lock;

      if (ccp == NULL) {      
        if (rmp->equal && cp == NULL)
          size = sizeof(*ccp);
        else if (rmp->equal && cp != NULL)
          size = sizeof(*ccp) + (cp->acount - 1) * sizeof(ccp->attr[0]);
        else
          size = sizeof(*ccp) + (rmp->cclass.acount - 1) * sizeof(ccp->attr[0]);

        ccp = pool_Alloc(sts, gdbroot->pool, size);
        if (ccp == NULL) {
          net_Free(NULL, rmp);
          return NULL;
        }

        if (rmp->equal && cp == NULL) {
          ccp->size = 0;
          ccp->time = time;
        } else if (rmp->equal && cp != NULL) {
          ccp->size = cp->size;
          ccp->time = time;
        } else {
          ccp->size = rmp->cclass.size;
          ccp->time = rmp->cclass.time;
        }
      }
    

      if (rmp->equal && cp == NULL) {
        *equal = 1;
        ccp->acount = 0;
      } else if (rmp->equal && cp != NULL) {
        *equal = 1;

        for (i = 0; i < cp->acount; i++) {
          ccp->attr[i].aix     = cp->attr[i].aix;
          ccp->attr[i].flags   = cp->attr[i].flags;
          ccp->attr[i].type    = cp->attr[i].type;
          ccp->attr[i].offs    = cp->attr[i].offs;
          ccp->attr[i].size    = cp->attr[i].size;
          ccp->attr[i].elem    = cp->attr[i].elem;
          ccp->attr[i].moffset = cp->attr[i].moffset;
        }

        ccp->acount = cp->acount;

      } else {
        nextIdx = rmp->naidx;
        stopIdx = nextIdx != ULONG_MAX ? nextIdx : rmp->cclass.acount;
        
        for (i = rmp->saidx; i < stopIdx; i++)
          ccp->attr[i] = rmp->attr[i];

        ccp->acount = rmp->cclass.acount;  
      }
      
      net_Free(NULL, rmp);
      rmp = NULL;

    } while(!*equal && nextIdx != ULONG_MAX);
  }

  ccp->key = ccKey;
  ccp->flags.b.equal = *equal;
  
  ccp = linkCclass(sts, ccp);
  
  if (*equal && cp == NULL)
    ccp = NULL;
  
  if (cp != NULL)
    *equal = 0;
  
  return ccp;


netError:
  if (rmp != NULL) {
    if (sts != NULL)
      *sts = rmp->sts;
    net_Free(NULL, rmp);
  }


  gdb_Lock;
  
  if (ccp != NULL)
    pool_Free(NULL, gdbroot->pool, ccp);
  
  return NULL;
}

/** 
 * @brief Fetches a non existing class from the remote node.
 *  
 * The fetched class will be inserted as a native class and will not
 * be treated as a cached class. If the class volume doesn't exist, it will
 * also be created.
 *
 */
void
cmvolc_GetNonExistingClass (
  pwr_tStatus		*sts,
  gdb_sObject		*op
  )
{

#define MAX_ATTR (((net_cSizeLarge - sizeof(*rmp)) / sizeof(rmp->attr[0])) + 1)
#define MAX_MSG ((MAX_ATTR + 4094)/4095)


  qcom_sQid		tgt;
  qcom_sPut		put;
  net_sGetGclass	*smp;	     /* Send message.  */
  net_sGetGclassR	*rmp = NULL; /* Receive message.  */
  int 			i, j;
  pwr_tUInt32		nextIdx = 0;
  pwr_tClassId		cid;
  pwr_tVolumeId		cvid;
  gdb_sClass		*cp;
  gdb_sVolume		*vp;
  gdb_sNode		*np;
  net_sGetGclassR	*msgs[MAX_MSG];
  int			msgcnt = 0;
  pwr_tStatus		lsts;
  gdb_sObject		*cop;
  gdb_sObject		*bop;
  gdb_sObject		*aop;
  net_sGattribute	*gap;
  net_sGclass		*gcp;  
  pwr_tObjid		poid;


  gdb_AssumeLocked;

  pwr_Assert(op->l.flags.b.isCached);

  cid = op->g.cid;

  vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
  np = pool_Address(NULL, gdbroot->pool, vp->l.nr);
  if (!np->cclassSupport) {
    *sts = GDH__NOCCLASSSUP;
    return;
  }    
  

  gdb_Unlock;

  do {
    
    smp = net_Alloc(sts, &put, sizeof(*smp), net_eMsg_getGclass);
    if (smp == NULL)
      goto netError;
    
    tgt.nid = np->nid;
    tgt.qix = net_cProcHandler;
    smp->ver = net_cVersion;
    smp->cid = cid;
    smp->aidx = nextIdx;
    
    rmp = net_Request(sts, &tgt, &put, NULL, net_eMsg_getGclassR);
    if (rmp == NULL || EVEN(rmp->sts))
      goto netError;
    
    nextIdx = rmp->naidx;
        
    msgs[msgcnt++] = rmp;
    rmp = NULL;

  } while(nextIdx != ULONG_MAX);
  
  gdb_Lock;
  

  /* Check if someone else has loaded the class while gdb was unlocked */
  
  cp = hash_Search(NULL, gdbroot->cid_ht, &cid);
  if (cp == NULL) {
    cvid = cid >> 16;


    /* Create the volume if it doesn't exist */
    vp = hash_Search(NULL, gdbroot->vid_ht, &cvid);
    if (vp == NULL) {
      vp = createVolume(sts, msgs[0]->vname, cvid);
      if (vp == NULL)
        goto cleanup;
    }
    
    
    
    /* Create the class */
    gcp = &msgs[0]->gclass;
    
    poid.vid = cvid;
    poid.oix = 1; /* ClassHier object is the father */
    
    cop = createObject(sts, vp, &gcp->co, &gcp->time, gcp->dbsFlags, 
                       &gcp->cb, sizeof(gcp->cb), poid);
    if (cop == NULL)
      goto cleanup;
    

    bop = createObject(&lsts, vp, &gcp->bo, &gcp->time, 0,
                       &gcp->bb, sizeof(gcp->bb), cop->g.oid);
    if (bop == NULL)
      errh_Bugcheck(lsts, "cmvolc_GetNonExistingClass, load sys body failed");
    
    
    for (i = j = 0, gap = msgs[0]->attr; i < gcp->acount; i++, gap++) {
      aop = createObject(&lsts, vp, &gap->ao, &gcp->time, 0, 
                         &gap->ab, sizeof(gap->ab), bop->g.oid);
      if (aop == NULL)
        errh_Bugcheck(lsts, "cmvolc_GetNonExistingClass, load attribute failed");
      
      if (i + 1 == msgs[j]->naidx) {
        j++;
        pwr_Assert(j < msgcnt);
        gap = msgs[j]->attr;
      }

    }
    

    cp = hash_Search(NULL, gdbroot->cid_ht, &cid);
    if (cp == NULL) 
      errh_Bugcheck(GDH__WEIRD, "cmvolc_GetNonExistingClass, can't find created class");
    
    cp = mvol_LinkClass(sts, cp, gdb_mAdd__);
    if (cp == NULL) 
      errh_Bugcheck(GDH__WEIRD, "cmvolc_GetNonExistingClass, couldn't link class");


  } else if (sts != NULL)
    *sts = GDH__SUCCESS;
    

cleanup:
  for (i = 0; i < msgcnt; i++)
    net_Free(NULL, msgs[i]);  

  return;


netError:
  for (i = 0; i < msgcnt; i++)
    net_Free(NULL, msgs[i]);
  if (rmp != NULL) {
    if (sts != NULL)
      *sts = rmp->sts;
    net_Free(NULL, rmp);
  }
  gdb_Lock;
  
  return;
}


void
cmvolc_LockClass (
  pwr_tStatus		*sts,
  gdb_sCclass		*ccp
)
{
  gdb_AssumeLocked;
  pwr_Assert(ccp != NULL);

  if (sts != NULL)
    *sts = GDH__SUCCESS;

  if (ccp->flags.b.cacheLock) {
    ccp->lockCnt++;
    return;
  }
  

  pwr_Assert(ccp->lockCnt == 0);

  cmvolc_TouchClass(ccp);
  
  ccp->flags.b.cacheLock = 1;
  ccp->lockCnt = 1;
  
  
}




/** Move a cached class to the head of the touched list.  */

/** @todo Check if this function should be static */
void
cmvolc_TouchClass (
  gdb_sCclass		*ccp
)
{
  gdb_AssumeLocked;
  pwr_Assert(ccp != NULL);

  if (ccp->flags.b.cacheLock)
    return;

  pool_Qremove(NULL, gdbroot->pool, &ccp->cache_ll);  
  pool_QinsertSucc(NULL, gdbroot->pool, &ccp->cache_ll, &gdbroot->db->cacheCclass.lh);

}



void
cmvolc_UnlockClass (
  pwr_tStatus		*sts,
  gdb_sCclass		*ccp
)
{
  gdb_AssumeLocked;

  pwr_Assert(ccp != NULL);
  pwr_Assert(ccp->flags.b.cacheLock);
  pwr_Assert(ccp->lockCnt > 0);

  if (sts != NULL)
    *sts = GDH__SUCCESS;

  if (--ccp->lockCnt == 0) {
    pwr_Assert(pool_QisEmpty(sts, gdbroot->pool, &ccp->subc_lh));

    ccp->flags.b.cacheLock = 0;
    cmvolc_TouchClass(ccp);
  }  
}


static gdb_sObject *
createObject (
  pwr_tStatus		*status,
  gdb_sVolume		*vp,
  const net_sGobject	*nop,
  const pwr_tTime	*time,
  pwr_tBitMask		dbsFlags,
  const void		*bodyData,
  int			bodySize,
  pwr_tObjid		poid
)
{
  gdb_sObject		*op;
  net_mGo		flags;
  pwr_tObjid		soid = pwr_cNObjid;
  dbs_mFlags		lflags;
  void*			body;
  
  

  pwr_dStatus(sts, status, GDH__SUCCESS);
  lflags.m = dbsFlags;
  
  flags.m = 0;

  op = gdb_AddObject(sts, nop->f.name.orig, nop->oid, nop->cid, nop->size, poid, flags.m, soid);
  if (op == NULL) return NULL;
  if (vol_LinkObject(sts, vp, op, vol_mLink_load) == NULL)
    return NULL;
  op->u.n.flags.b.isMountClean = 1;
  op->u.n.time = *time;
  op->u.n.lflags.m = lflags.m;

  body = pool_Address(sts, gdbroot->rtdb, op->u.n.body);
  if (body == NULL && bodySize > 0)
    return NULL;
  
  if (op->g.size < bodySize)
    return NULL;
  
  if (bodySize > 0)
    memcpy(body, bodyData, bodySize);

  /** @todo verify that we can you use all lists in vol_mLink_build */
  if (vol_LinkObject(sts, vp, op, vol_mLink_build) == NULL)
    return NULL;

  return op;
}

static gdb_sVolume *
createVolume(
  pwr_tStatus* 		sts,
  const char*		vname,
  pwr_tVolumeId 	cvid)
{
  gdb_sVolume		*vp;
  gdb_sObject		*op;
  pwr_tStatus		lsts;
  pwr_tTime		time;
  net_sGobject		goh;
  pwr_sClassVolume	cv;
  pwr_tObjid		poid;
  
  vp = gdb_AddVolume(sts, cvid, gdb_mAdd_failIfAdded);
  if (vp == NULL)
    return NULL;

  clock_gettime(CLOCK_REALTIME, &time);
  vp = gdb_LoadVolume(&lsts, cvid, vname, pwr_eClass_ClassVolume, 
                      gdbroot->my_node->nid, time, gdb_mLoad_build, 
                      &gdbroot->my_node->fm);

  if (vp == NULL)
    errh_Bugcheck(lsts, "createVolume, gdb_LoadVolume failed");

  /* Create the Volume object ($ClassVolume) */

  memset(&goh, 0, sizeof(goh));
  goh.oid.vid = cvid;
  goh.oid.oix = 0;
  goh.cid = pwr_eClass_ClassVolume;
  strcpy(goh.f.name.orig, vname);
  goh.size = sizeof(cv);
  
  memset(&cv, 0, sizeof(cv));
  strcpy(cv.Description, "Created Dynamically");
  cv.RtCreTime = time;
  
  op = createObject(&lsts, vp, &goh, &time, 0, &cv, sizeof(cv), pwr_cNObjid);
  if (op == NULL)
    pwr_Return(NULL, sts, lsts);
  

  /* Create the Class object $(ClassHier)  */
  poid = goh.oid;
  memset(&goh, 0, sizeof(goh));
  goh.oid.vid = cvid;
  goh.oid.oix = 1;
  goh.cid = pwr_eClass_ClassHier;
  strcpy(goh.f.name.orig, "Class");
  goh.size = 0;  
  op = createObject(&lsts, vp, &goh, &time, 0, NULL, 0, poid);
  if (op == NULL)
    pwr_Return(NULL, sts, lsts);
  

  return vp;
  
}

static void
deleteCclass(gdb_sCclass *ccp)
{
  pwr_tStatus	sts;

  pool_Qremove(NULL, gdbroot->pool, &ccp->cache_ll);  
  gdbroot->db->cacheCclass.lc--;

  if (hash_Remove(&sts, gdbroot->cclass_ht, ccp) == NULL)
    errh_Bugcheck(sts, "Removing cached class from hash table");


  if (ccp->flags.b.nrConv)
    pool_FreeReference(NULL, gdbroot->pool, ccp->nrConv);

  if (ccp->flags.b.rnConv)
    pool_FreeReference(NULL, gdbroot->pool, ccp->rnConv);

  pool_Free(NULL, gdbroot->pool, ccp);
  
}


/**
 * Insert and links the cached class. If the class is already inserted into 
 * the hash table, then is the passed class freed and the other instance is 
 * returned.
 *
 * @return The newly linked cached class or the existing one if there is one
 */

static gdb_sCclass *
linkCclass	(
  pwr_tStatus		*sts, /**< Status */
  gdb_sCclass		*ccp  /**< The cached class that shall be linked */
)
{
  gdb_sCclass		*ccp2;
  gdb_sTouchQ		*tqp = &gdbroot->db->cacheCclass;
  
  gdb_AssumeLocked;

  errh_Info("linkClass");
  

  ccp2 = hash_Insert(sts, gdbroot->cclass_ht, ccp);
  if (ccp2 == NULL) {
    if (*sts == HASH__DUPLICATE) {
      /* It's OK, someone else has inserted the class while gdb was unlocked */
      ccp2 = hash_Search(sts, gdbroot->cclass_ht, &ccp->key);
      if (ccp2 == NULL) 
        errh_Bugcheck(GDH__WEIRD, "couldn't find cached class");

      pool_Free(NULL, gdbroot->pool, ccp);
      cmvolc_TouchClass(ccp2);      
      *sts = GDH__SUCCESS;
      return ccp2;   
    } else
      errh_Bugcheck(GDH__WEIRD, "inserting cached class");
  }

  ccp->nrConv = pool_cNRef;
  ccp->rnConv = pool_cNRef;
  
  pool_Qinit(NULL, gdbroot->pool, &ccp->subc_lh);

  pool_QinsertSucc(NULL, gdbroot->pool, &ccp->cache_ll, &tqp->lh);
  tqp->lc++;

  if (tqp->lc > tqp->lc_max)
    trimQ(tqp);

  errh_Info("linkClass, success\n");

  return ccp;
}


static void
trimQ(gdb_sTouchQ *tqp)
{
  gdb_sCclass		*ccp;
  gdb_sCclass		*ccp2;
  pool_sQlink		*ccl;

  for (ccl = pool_Qpred(NULL, gdbroot->pool, &tqp->lh);
       tqp->lc > tqp->lc_max;
       ccl = pool_Qpred(NULL, gdbroot->pool, &ccp->cache_ll)
  ) {
      
    if (ccl == &tqp->lh) 
      break;

    ccp = pool_Qitem(ccl, gdb_sCclass, cache_ll);
    if (ccp == NULL) 
      errh_Bugcheck(GDH__WEIRD, "trimQ, pool_Qitem failed");

    if (ccp->flags.b.cacheLock)
      continue;
    

    if (tqp->lc > tqp->lc_max + 1) {      
      ccp2 = ccp;
      ccl = pool_Qsucc(NULL, gdbroot->pool, &ccp->cache_ll);
      ccp = pool_Qitem(ccl, gdb_sCclass, cache_ll);
      if (ccp == NULL) 
        errh_Bugcheck(GDH__WEIRD, "trimQ, pool_Qitem failed");
      deleteCclass(ccp2);
    } else {
      deleteCclass(ccp);
      break;
    }
  }
    
}


