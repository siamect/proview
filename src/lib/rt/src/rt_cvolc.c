/* rt_cvolc.c -- Cached volumes, client user API.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   .  */

#ifdef	OS_ELN
# include $vaxelnc
#else
# include <stdio.h>
# include <string.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "rt_qcom.h"
#include "rt_gdh_msg.h"
#include "rt_pool.h"
#include "rt_vol.h"
#include "rt_net.h"
#include "rt_ndc.h"
#include "rt_cvol.h"
#include "rt_cvolc.h"
#include "rt_cmvolc.h"

static gdb_sObject *
fetch (
  pwr_tStatus	*sts,
  gdb_sVolume	*vp,
  qcom_sPut	*put
);


/* General cache fetch routine.  */

static gdb_sObject *
fetch (
  pwr_tStatus	*sts,
  gdb_sVolume	*vp,
  qcom_sPut	*put
)
{
  pwr_tStatus	lsts;
  qcom_sQid	tgt;
  qcom_sGet	get;
  net_sObjectR	*rsp;
  net_sGobject	*gop;
  int		i;
  gdb_sNode	*np;
  gdb_sObject	*op;
  gdb_sObject	*rop  = NULL;

  gdb_AssumeLocked;

  np = hash_Search(sts, gdbroot->nid_ht, &vp->g.nid);
  if (np == NULL) return NULL;

  tgt.nid = np->nid;
  tgt.qix = net_cProcHandler;

  gdb_Unlock;

    rsp = net_Request(&lsts, &tgt, put, &get, net_eMsg_objectR);

  gdb_Lock;

  if (EVEN(lsts))
    pwr_Return(NULL, sts, lsts);
  if (EVEN(lsts = rsp->sts)) {
    net_Free(NULL, rsp);
    pwr_Return(NULL, sts, lsts);
  }

  for (i = 0, gop = &rsp->g[0]; i < rsp->count; i++, gop++) {
    op = cvol_LoadObject(&lsts, np, vp, gop);
    if (cdh_ObjidIsEqual(gop->oid, rsp->oid)) {
      /* This is the referenced object.  */
      rop = op;
      if (sts != NULL) *sts = lsts;	/* return this status code */
    }
  }

  pwr_Assert(rop != NULL);

  net_Free(NULL, rsp);

  return rop;
}


#if 1
void *
cvolc_GetObjectInfo (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const pwr_sAttrRef	*arp,
  gdb_sCclass		*ccp,
  const pwr_sAttrRef	*rarp,
  pwr_tUInt32		ridx,
  mvol_sAttribute	*ap,
  void			*p,
  int			size
)
{
  qcom_sQid		tgt;
  qcom_sPut		put;
  net_sGetObjectInfo	*smp;	/* Send message.  */
  net_sGetObjectInfoR	*rmp;	/* Receive message.  */
  pwr_tBoolean		equal;
  ndc_sRemoteToNative	*tbl = NULL;
  
  gdb_AssumeUnlocked;
  pwr_Assert(sts != NULL);

  if (ccp == NULL) {
    equal = 1;
    rarp = arp;
  } else {
    equal = 0;
    cmvolc_AssumeLocked(ccp);
  }

  
  smp = net_Alloc(sts, &put, sizeof(*smp), net_eMsg_getObjectInfo);
  if (smp == NULL) return sts;

  tgt.nid = np->nid;
  tgt.qix = net_cProcHandler;
  smp->aref = *rarp;

  rmp = net_Request(sts, &tgt, &put, NULL, net_eMsg_getObjectInfoR);

  if (rmp == NULL) {
    return NULL;
  } else {
    if (ODD(rmp->sts)) {
      size = MIN(arp->Size, size);
      if (ccp == NULL || equal) {
        ndc_ConvertData(sts, np, arp, p, rmp->info, size, ndc_eOp_decode);
      } else {

        if (!ccp->flags.b.rnConv && ap->aop == NULL) { /* whole object */

          gdb_ScopeLock {

            if (!ccp->flags.b.rnConv) {
         
              if (ap->cp == NULL) errh_Bugcheck(GDH__WEIRD, "can't get class");

              tbl = pool_Alloc(sts, gdbroot->pool, sizeof(*tbl) * ap->cp->acount);
              if (tbl == NULL)
                break;                

              ndc_UpdateRemoteToNativeTable(sts, tbl, ap->cp->acount, ap->cp, ccp);
              if (ODD(*sts)) {
                ccp->rnConv = pool_Reference(NULL, gdbroot->pool, tbl);
                ccp->flags.b.rnConv = 1;
              } else {
                pool_Free(NULL, gdbroot->pool, tbl);
                tbl = NULL;
              }
            }
          
          } gdb_ScopeUnlock;
        }
        
        ndc_ConvertRemoteData(sts, np, ccp, rarp, rmp->info, rmp->info, rarp->Size, ndc_eOp_decode);
        if (ODD(*sts)) {
          if (ccp->flags.b.rnConv) {
            if (tbl == NULL) {
              tbl = pool_Address(sts, gdbroot->pool, ccp->rnConv);
              if (tbl == NULL)
                errh_Bugcheck(*sts, "failed getting address for conversion table");
            }
            ndc_ConvertRemoteToNativeTable(sts, ccp, tbl, rarp,
                                               arp, p, rmp->info, size);

          } else {
            /* The object pointer may be invalid after gdb has been open, reset it */
            ap->op = NULL;
            ndc_ConvertRemoteToNativeData(sts, ccp, ridx, ap, rarp, arp, p, rmp->info, size);
          }
        }
      }      
    } else if (sts != NULL) 
      *sts = rmp->sts;
    net_Free(NULL, rmp);
    return p;
  }
}
#else
void *
cvolc_GetObjectInfo (
  pwr_tStatus		*sts,
  gdb_sNode		*np,
  pwr_sAttrRef		*arp,
  void			*p,
  int			size
)
{
  qcom_sQid		tgt;
  qcom_sPut		put;
  net_sGetObjectInfo	*smp;	/* Send message.  */
  net_sGetObjectInfoR	*rmp;	/* Receive message.  */

  gdb_AssumeUnlocked;

  smp = net_Alloc(sts, &put, sizeof(*smp), net_eMsg_getObjectInfo);
  if (smp == NULL) return sts;

  tgt.nid = np->nid;
  tgt.qix = net_cProcHandler;
  smp->aref = *arp;

  rmp = net_Request(sts, &tgt, &put, NULL, net_eMsg_getObjectInfoR);

  if (rmp == NULL) {
    return NULL;
  } else {
    if (ODD(rmp->sts)) {
      size = MIN(arp->Size, size);
      ndc_ConvertData(sts, np, arp, p, rmp->info, size, ndc_eOp_decode);
    }
    if (sts != NULL) *sts = rmp->sts;
    net_Free(NULL, rmp);
    return p;
  }
}
#endif

/* Lock a cached object, i.e. exclude it
   from cache trim.  */

void
cvolc_LockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
)
{
  gdb_sVolume		*vp;

  gdb_AssumeLocked;
  pwr_Assert(op != NULL);

  if (!op->l.flags.b.isCached || op->u.c.flags.b.cacheLock) return;

  vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
  pwr_Assert(vp != NULL);

  cvol_QmoveSucc(op, cvol_Qget(op), &vp->u.c.cacheLock);
}

/* Fetch an object identified by its name.  */

gdb_sObject *
cvolc_NameToObject (
  pwr_tStatus		*sts,
  gdb_sObject		*p_op,
  cdh_sParseName	*pn,
  int			index,
  pwr_tBitMask		trans
)
{
  net_sNameToObject	*mp;
  qcom_sPut		put;
  pwr_tUInt32		len;
  char			*s;
  int			i;
  gdb_sVolume		*vp;
  
  gdb_AssumeLocked;

  if (!p_op->l.flags.b.isCached) pwr_Return(NULL, sts, GDH__NOSUCHOBJ);

  vp = pool_Address(NULL, gdbroot->pool, p_op->l.vr);
  if (vp == NULL) pwr_Return(NULL, sts, GDH__NOSUCHOBJ);

  for (len = 0, i = index; i < pn->nObject; i++)
    len += 1 + pn->object[i].name.pack.c.len;

  len = (len + 3) & ~3;	/* Size up to nearest multipel of 4.  */

  mp = net_Alloc(sts, &put, len + sizeof(*mp), net_eMsg_nameToObject);
  if (mp == NULL) return NULL;

  mp->poid   = pn->object[index].poid;
  mp->trans  = trans;
  mp->len    = len;
  mp->rcount = mp->lcount = net_cObjectMaxCount / 2;

  for (i = index, s = mp->name; i < pn->nObject; i++) {
    memcpy(s, pn->object[i].name.norm, pn->object[i].name.pack.c.len);
    s += pn->object[i].name.pack.c.len;
    *s++ = '-';
  }
  *--s = '\0';

  return fetch(sts, vp, &put);

}

/* Fetch an object identified by its object identifier.  */

gdb_sObject *
cvolc_OidToObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  pwr_tObjid		oid,
  pwr_tBitMask		trans,
  cvol_eHint		hint
)
{
  net_sOidToObject	*mp;
  qcom_sPut		put;

  gdb_AssumeLocked;

  mp = net_Alloc(sts, &put, sizeof(*mp), net_eMsg_oidToObject);
  if (mp == NULL) return NULL;

  mp->oid   = oid;
  mp->trans = trans;

  switch (hint) {
  case cvol_eHint_prev:
    mp->lcount = net_cObjectMaxCount;
    mp->rcount = 0;
    break;

  case cvol_eHint_next:
    mp->lcount = 0;
    mp->rcount = net_cObjectMaxCount;
    break;

  default:
    mp->lcount = 0;
    mp->rcount = 0;
    break;
  }
    
  return fetch(sts, vp, &put);
}

/* Update the object body of an object residing on
   a remote node.  */

pwr_tBoolean
cvolc_SetObjectInfo (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const pwr_sAttrRef	*arp,
  const gdb_sCclass	*ccp,
  const pwr_sAttrRef	*rarp,
  pwr_tUInt32		ridx,
  mvol_sAttribute	*ap,
  const void		*p,
  int			size
)
{
  qcom_sQid		tgt;
  qcom_sPut		put;
  net_sSetObjectInfo	*smp;
  int			msize;
  net_sSetObjectInfoR	*rmp;
  pwr_tBoolean		equal;

  gdb_AssumeUnlocked;

  if (ccp == NULL) {
    equal = 1;
    rarp = arp;
  } else {
    equal = 0;
    cmvolc_AssumeLocked(ccp);
  }

  tgt.nid = np->nid;
  tgt.qix = net_cProcHandler;

  size = MIN(arp->Size, size);

  msize = sizeof(net_sSetObjectInfo) - sizeof(smp->info) + rarp->Size;
  msize = (msize + 3) & ~3;   /* Size up to nearest multiple of 4.  */

  smp = net_Alloc(sts, &put, msize, net_eMsg_setObjectInfo);
  if (smp == NULL) return NO;

  smp->size = rarp->Size;
  smp->aref = *rarp;

  if (equal)
    ndc_ConvertData(sts, np, arp, smp->info, p, size, ndc_eOp_encode);
  else {
    /* The object pointer may be invalid after gdb has been open, reset it */
    ap->op = NULL;
    ndc_ConvertNativeToRemoteData(sts, ccp, ridx, ap, rarp, arp, smp->info, p, rarp->Size);
    if (ODD(*sts)) {
      ndc_ConvertRemoteData(sts, np, ccp, rarp, smp->info, smp->info, rarp->Size, ndc_eOp_encode);
    }    
    if (EVEN(*sts)) {
      net_Free(NULL, smp);
      return NO;
    }
  }

  rmp = net_Request(sts, &tgt, &put, NULL, net_eMsg_setObjectInfoR);

  if (rmp == NULL) {
    return NO;
  } else {
    if (sts != NULL) *sts = rmp->sts;
    net_Free(NULL, rmp);
    return YES;
  }
}

/* Move an object to the head of its volume touched list. */
void
cvolc_TouchObject (
  gdb_sObject		*op
)
{
  gdb_sVolume		*vp;
  gdb_sTouchQ		*fqp;
  gdb_sTouchQ		*vqp;

  gdb_AssumeLocked;
  pwr_Assert(op != NULL);

  if (!op->l.flags.b.isCached || op->u.c.flags.b.cacheLock) return;

  vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
  pwr_Assert(vp != NULL);
  vqp = &vp->u.c.cacheVol;
  fqp = cvol_Qget(op);

  cvol_QmoveSucc(op, fqp, vqp);
  if (fqp != vqp) cvol_Qtrim(vqp);
}

/* Unlock a cached object, i.e. include it
   in cache trim.  */

void
cvolc_UnlockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
)
{
  gdb_sVolume		*vp;
  gdb_sTouchQ		*fqp;
  gdb_sTouchQ		*vqp;

  gdb_AssumeLocked;
  pwr_Assert(op != NULL);

  if (!op->l.flags.b.isCached || !op->u.c.flags.b.cacheLock) return;

  vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
  pwr_Assert(vp != NULL);
  vqp = &vp->u.c.cacheVol;
  fqp = cvol_Qget(op);
  pwr_Assert(fqp == &vp->u.c.cacheLock);
  cvol_QmoveSucc(op, fqp, vqp);
  cvol_Qtrim(vqp);
}

