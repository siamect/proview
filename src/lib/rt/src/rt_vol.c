/* 
 * Proview   $Id: rt_vol.c,v 1.9 2006-02-09 14:29:56 claes Exp $
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

/* rt_vol.c -- Volumes */

#ifdef OS_ELN
# include $vaxelnc
#else
# include <string.h>
#endif

#include "pwr.h"
#include "rt_gdh_msg.h"
#include "pwr_class.h"
#include "rt_errh.h"
#include "rt_gdb.h"
#include "rt_vol.h"
#include "rt_cvol.h"
#include "rt_cvolc.h"
#include "rt_mvol.h"
#include "rt_pool.h"
#include "rt_sub.h"


gdb_sAliasServer *
vol_AddAliasClient (
  pwr_tStatus		*sts,
  gdb_sObject		*op
)
{
  gdb_sAliasServer	*asp;

  pwr_Assert(!op->u.n.flags.b.inAliasClientList);

  asp = gdb_AddAliasServer(sts, op->g.soid, gdb_mAdd__);
  if (asp == NULL) return NULL;

  pool_QinsertPred(NULL, gdbroot->pool, &op->u.n.cli_ll, &asp->cli_lh);
  op->u.n.flags.b.inAliasClientList = 1;

  return asp;
}    

gdb_sMountServer *
vol_AddMountClient (
  pwr_tStatus           *sts,
  gdb_sObject		*op
)
{
  gdb_sMountServer	*msp;
  
  pwr_Assert(op->l.flags.b.isNative);
  pwr_Assert(op->g.flags.b.isMountClient);
  pwr_Assert(!op->u.n.flags.b.inMountClientList);

  /* Find the mount server.  */

  msp = vol_AddMountServer(sts, op->g.soid);
  if (msp == NULL) return NULL;

  pool_QinsertPred(sts, gdbroot->pool, &op->u.n.cli_ll, &msp->cli_lh);
  op->u.n.flags.b.inMountClientList = 1;

  return msp;
}

gdb_sMountedOn *
vol_AddMountedOn (
  pwr_tStatus           *sts,
  pwr_tVolumeId		vid,
  gdb_sNode		*np
)
{
  gdb_sMountedOn	*mop;
  gdb_sVolume		*vp;

  vp = hash_Search(sts, gdbroot->vid_ht, &vid);
  if (vp == NULL) return NULL;
  mop = pool_Alloc(sts, gdbroot->pool, sizeof(mop));
  if (mop == NULL) return NULL;

  mop->vid = vp->g.vid;
  mop->nid = np->nid;
  mop->nr = pool_Reference(sts, gdbroot->pool, np);
  mop->vr = pool_Reference(sts, gdbroot->pool, vp);
  pool_QinsertPred(sts, gdbroot->pool, &mop->nodmo_ll, &np->nodmo_lh);
  pool_QinsertPred(sts, gdbroot->pool, &mop->volmo_ll, &vp->u.n.volmo_lh);

  return mop;
}

gdb_sMountServer *
vol_AddMountServer (
  pwr_tStatus           *sts,
  pwr_tObjid		soid
)
{
  gdb_sMountServer	*msp;
  gdb_sNode		*np;
  gdb_sVolume		*vp;
  gdb_sObject		*op;

  msp = hash_Search(sts, gdbroot->ms_ht, &soid);
  if (msp != NULL) return msp;

  msp = gdb_AddMountServer(sts, soid, gdb_mAdd__);
  if (msp == NULL) return NULL;

  vp = gdb_AddVolume(sts, soid.vid, gdb_mAdd__);
  if (vp == NULL) return NULL;

  pool_QinsertPred(NULL, gdbroot->pool, &msp->volms_ll, &vp->l.volms_lh);

  if (vp->l.flags.b.isMounted) {
    op = hash_Search(NULL, gdbroot->oid_ht, &soid);
    if (op != NULL) {
      msp->msor = pool_ItemReference(NULL, gdbroot->pool, op);
      op->l.flags.b.isMountServer = 1;
    }
    np = hash_Search(NULL, gdbroot->nid_ht, &vp->g.nid);
    if (np != NULL && np->nid != pwr_cNNodeId)
      pool_QinsertPred(NULL, gdbroot->pool, &msp->nodms_ll, &np->nodms_lh);
  }

  return msp;
}

/* Translate an attribute reference to the internal
   attribute format.  */

mvol_sAttribute *
vol_ArefToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_sAttrRef		*arp,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		trans
)
{

  ap->op = vol_OidToObject(sts, arp->Objid, lo_flags, trans, cvol_eHint_none);
  if (ap->op == NULL) return NULL;

  return mvol_ArefToAttribute(sts, ap, arp, ap->op->g.cid);
}

void *
vol_AttributeToAddress (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap
)
{
  void			*p = NULL;
  pwr_tStatus		lsts;

  if (ap == NULL || ap->op == NULL)
    errh_Bugcheck(GDH__WEIRD, "vol_AttributeToAddress");

  if (ap->op->g.size == 0 || (ap->op->l.flags.m & gdb_mLo_native) == 0)
    pwr_Return(NULL, sts, GDH__BADOBJTYPE);

  if (ap->offs + (ap->flags.b.Indirect ? sizeof(pool_tRef) : ap->size) > ap->op->g.size)
   pwr_Return(NULL, sts, GDH__SIZE);

  do {
    p = pool_Address(sts, gdbroot->rtdb, ap->op->u.n.body); 
    if (p == NULL) break;

    p = (void *)((unsigned int)p + ap->offs);

    if (!ap->flags.b.Indirect) break;

    p = pool_Address(&lsts, gdbroot->rtdb, *(int *)p);

  } while (0);

  if (p == NULL)
    pwr_Return(NULL, sts, GDH__RTDBNULL);
  else
    pwr_Return(p, sts, GDH__SUCCESS);
}

char *
vol_AttributeToName (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_tBitMask		nametype,
  char			*s
)
{
  char			index[20];
  cdh_mName		lnametype;  
  gdb_AssumeLocked;

  lnametype.m = nametype;

  s = vol_ObjectToName(sts, ap->op, lnametype.m, s);
  if (s == NULL) return NULL;

  if (ap->aop != NULL && lnametype.b.attribute) {
    if (*s != '\0' || lnametype.b.separator)
      strcat(s, ".");
    strcat(s, ap->name);
    if (ap->idx != ULONG_MAX && lnametype.b.index) {
      sprintf(index, "[%d]", ap->idx);
      strcat(s, index);
    }
  }  

  return s;
}

/* Insert an object in the list of siblings.  */

void
vol_InsertSiblist (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sObject		*pop
)
{
  pwr_tObjid		soid = pop->g.oid;
  gdb_sObject		*fop;
  gdb_sObject		*lop;

  pwr_Assert(!op->g.flags.b.inSibList);
  pwr_Assert(op->g.sib.flink == pwr_cNObjectIx);
  pwr_Assert(op->g.sib.blink == pwr_cNObjectIx);
  pwr_Assert(op->g.oid.vid == pop->g.oid.vid);
  
  if (cdh_ObjidIsNull(pop->g.soid)) {
    pop->g.soid = op->g.oid;
    op->g.sib.flink = op->g.sib.blink = op->g.oid.oix;
  } else {
    fop = hash_Search(sts, gdbroot->oid_ht, &pop->g.soid);
    if (fop->g.sib.blink == fop->g.oid.oix) {
      lop = fop;
    } else {
      soid.oix = fop->g.sib.blink;
      lop= hash_Search(sts, gdbroot->oid_ht, &soid);
    }
    op->g.sib.flink = lop->g.sib.flink;
    op->g.sib.blink = fop->g.sib.blink;
    fop->g.sib.blink = op->g.oid.oix;
    lop->g.sib.flink = op->g.oid.oix;
  }

  op->g.flags.b.inSibList = 1;
  pop->g.flags.b.isParent = 1;
}

pool_tRef
vol_AttributeToReference (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap
)
{
  pool_tRef		r = pool_cNRef;
  void			*p = NULL;
  pwr_tStatus		lsts;

  if (ap == NULL || ap->op == NULL)
    errh_Bugcheck(GDH__WEIRD, "vol_AttributeToAddress");

  if (ap->op->g.size == 0 || (ap->op->l.flags.m & gdb_mLo_native) == 0)
    pwr_Return(pool_cNRef, sts, GDH__BADOBJTYPE);

  if (ap->offs + (ap->flags.b.Indirect ? sizeof(pool_tRef) : ap->size) > ap->op->g.size)
   pwr_Return(pool_cNRef, sts, GDH__SIZE);

  do {
    if (ap->op->u.n.body == pool_cNRef) break;

    r = ap->op->u.n.body + ap->offs;

    if (!ap->flags.b.Indirect) break;

    p = pool_Address(&lsts, gdbroot->rtdb, r);
    r = *(pool_tRef *)p;
  } while (0);

  if (r == pool_cNRef)
    pwr_Return(pool_cNRef, sts, GDH__RTDBNULL);
  else
    pwr_Return(r, sts, GDH__SUCCESS);
}

/* Link an object.  */

gdb_sObject *
vol_LinkObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		ilink
)
{
  pwr_tStatus		lsts = 1;
  gdb_sObject		*pop;
  char			string[80];
  gdb_sMountServer	*msp;
  vol_mLink		link;

  gdb_AssumeLocked;

  link.m = ilink;

  pwr_Assert(vp->g.vid == op->g.oid.vid);

  if (link.b.init) {
    op->l.flags.m |= (vp->l.flags.m & gdb_mLv_objectFlags);
    pool_Qinit(NULL, gdbroot->pool, &op->l.obj_ll);
    pool_Qinit(NULL, gdbroot->pool, &op->l.oid_htl);
    pool_Qinit(NULL, gdbroot->pool, &op->l.family_htl);
    if (vp->l.flags.b.isNative) {
      pool_Qinit(NULL, gdbroot->pool, &op->u.n.cid_ll);
      pool_Qinit(NULL, gdbroot->pool, &op->u.n.cli_ll);
      pool_Qinit(NULL, gdbroot->pool, &op->u.n.sib_lh);
      pool_Qinit(NULL, gdbroot->pool, &op->u.n.sib_ll);
    } else if (vp->l.flags.b.isCached) {
      pool_Qinit(NULL, gdbroot->pool, &op->u.c.cache_ll);
      pool_Qinit(NULL, gdbroot->pool, &op->u.c.sanc_ll);
    }
  }

  pop = hash_Search(&lsts, gdbroot->oid_ht, &op->g.f.poid);
  if (pop == NULL && cdh_ObjidIsNotNull(op->g.f.poid)) {
    sprintf(string, "Orphan found. Objid %s, Name %s\n",  cdh_ObjidToString(NULL, op->g.oid, 0), op->g.f.name.orig);
    errh_Bugcheck(lsts, string);
  }

  /* Local part.  */

  if (link.b.loObjList && !op->l.flags.b.inObjList) {
    pool_QinsertPred(NULL, gdbroot->pool, &op->l.obj_ll, &vp->l.obj_lh);
    op->l.flags.b.inObjList = 1;
  }

  if (link.b.loOidTab && !op->l.flags.b.inOidTab) {
    op = hash_Insert(&lsts, gdbroot->oid_ht, op);
    if (op == NULL) pwr_Return(NULL, sts, lsts);
    op->l.flags.b.inOidTab = 1;
  }

  if (link.b.loFamilyTab && !op->l.flags.b.inFamilyTab) {
    op = hash_Insert(&lsts, gdbroot->family_ht, op);
    if (op == NULL) 
      errh_Bugcheck(lsts, "hash_Insert(sts, gdbroot->family_ht, op)");
    op->l.flags.b.inFamilyTab = 1;
  }

  if (link.b.loParentRef && op->l.por == pool_cNRef) {
    if (pop != NULL)
      op->l.por = pool_ItemReference(NULL, gdbroot->pool, pop);
    else
      op->l.por = pool_cNRef;
  }

  if (link.b.loVolumeRef && op->l.vr == pool_cNRef) {
    op->l.vr = pool_ItemReference(NULL, gdbroot->pool, vp);
  }

  /* Check if mount server.  */

  if (link.b.loMountServer && !op->l.flags.b.isMountServer) {
    msp = hash_Search(NULL, gdbroot->ms_ht, &op->g.oid);
    if (msp != NULL) {
      msp->msor = pool_ItemReference(NULL, gdbroot->pool, op);
      op->l.flags.b.isMountServer = 1;
    }
  }

  /* Native part.  */

  if (op->l.flags.b.isNative) {
    
    /* Global part. This is only done in native volumes.  */

    if (link.b.goAdopt && vp->l.flags.b.isLoaded && pop != NULL) {
      vol_InsertSiblist(NULL, op, pop);
    }

    if(link.b.noCidList && !op->u.n.flags.b.inCidList)
      mvol_LinkObject(NULL, vp, op, link.m);    

    if(link.b.noAliasClientList && op->g.flags.b.isAliasClient && !op->u.n.flags.b.inAliasClientList)
      vol_AddAliasClient(NULL, op);    

    if (link.b.noMountClientList && op->g.flags.b.isMountClient && !op->u.n.flags.b.inMountClientList)
      vol_AddMountClient(NULL, op);    

    if (pop != NULL && link.b.noSibList && !op->u.n.flags.b.inSibList) {
      pool_QinsertPred(NULL, gdbroot->pool, &op->u.n.sib_ll, &pop->u.n.sib_lh);
      op->u.n.flags.b.inSibList = 1;
      pop->u.n.flags.b.hasChild = 1;
    }

    if (op->u.n.body == pool_cNRef && op->g.size > 0) {
      op->u.n.body = pool_RefAlloc(NULL, gdbroot->rtdb, op->g.size);
    }

  } else if (op->l.flags.b.isCached) {


  }
  return op;
}

/* Link a Sub Class object.  */

gdb_sScObject *
vol_LinkScObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sScObject	*scp,
  pwr_tBitMask		ilink
)
{
  pwr_tStatus		lsts = 1;
  gdb_sObject		*pop = NULL;
  gdb_sScObject		*pscp = NULL;
  gdb_sClass		*cp;
  vol_mLinkSc		link;
  char			string[80];
  

  gdb_AssumeLocked;

  link.m = ilink;

  pwr_Assert(vp->g.vid == (scp->oid.vid & 0xffffff));

  if (link.b.init) {
    //scp->flags.m |= (vp->l.flags.m & gdb_mLv_objectFlags);
    pool_Qinit(NULL, gdbroot->pool, &scp->sc_htl);
    pool_Qinit(NULL, gdbroot->pool, &scp->sc_ll);
    pool_Qinit(NULL, gdbroot->pool, &scp->cid_ll);
    pool_Qinit(NULL, gdbroot->pool, &scp->sib_lh);
    pool_Qinit(NULL, gdbroot->pool, &scp->sib_ll);

    scp->flags.b.isParentSc = ((scp->poid.vid & 0xff000000) != 0);
  }

  
  if (scp->flags.b.isParentSc) {
    pscp = hash_Search(&lsts, gdbroot->sc_ht, &scp->poid);
    if (pscp == NULL) {
      sprintf(string, "Orphan found. Objid %s\n",  
              cdh_ObjidToString(NULL, scp->oid, 0));
      errh_Bugcheck(lsts, string);
    }
  } else {
    pop = hash_Search(&lsts, gdbroot->oid_ht, &scp->poid);
    if (pop == NULL) {
      sprintf(string, "Orphan found. Objid %s\n",  
              cdh_ObjidToString(NULL, scp->oid, 0));
      errh_Bugcheck(lsts, string);
    }
  }
  

  if (link.b.scList && !scp->flags.b.inScList) {
    pool_QinsertPred(NULL, gdbroot->pool, &scp->sc_ll, &vp->u.n.sc_lh);
    scp->flags.b.inScList = 1;
  }

  if (link.b.scTab && !scp->flags.b.inScTab) {
    scp = hash_Insert(&lsts, gdbroot->sc_ht, scp);
    if (scp == NULL) 
      pwr_Return(NULL, sts, lsts);
    scp->flags.b.inScTab = 1;
  }

  if (link.b.parentRef && scp->por == pool_cNRef) {
    if (scp->flags.b.isParentSc)
      scp->por = pool_ItemReference(NULL, gdbroot->pool, pscp);
    else
      scp->por = pool_ItemReference(NULL, gdbroot->pool, pop);
  }

  if (link.b.volumeRef && scp->vr == pool_cNRef) {
    scp->vr = pool_ItemReference(NULL, gdbroot->pool, vp);
  }

  if (link.b.classRef && scp->cr == pool_cNRef) {
    cp = hash_Search(&lsts, gdbroot->cid_ht, &scp->cid);
    if (cp != NULL) {
      sprintf(string, "Class 0x%x not found for object, objid %s\n",  
              scp->cid, cdh_ObjidToString(NULL, scp->oid, 0));

      errh_Bugcheck(lsts, string);
    }
      
    scp->cr = pool_ItemReference(NULL, gdbroot->pool, cp);
  }


  if(link.b.cidList && !scp->flags.b.inCidList)
    mvol_LinkScObject(NULL, vp, scp);


  if (link.b.sibList && !scp->flags.b.inSibList) {
    if (scp->flags.b.isParentSc) {
      pool_QinsertPred(NULL, gdbroot->pool, &scp->sib_ll, &pscp->sib_lh);
      pscp->flags.b.hasSc = 1;

    } else {    
      pool_QinsertPred(NULL, gdbroot->pool, &scp->sib_ll, &pop->u.n.sc_lh);
      pop->u.n.flags.b.hasSc = 1;
    }
    scp->flags.b.inSibList = 1;    
  }

  if (scp->body == pool_cNRef && scp->size > 0) {
    if (scp->flags.b.isParentSc) {
      pwr_Assert(pscp->body != pool_cNRef);
      scp->body = pscp->body + scp->offset;
    } else {
      pwr_Assert(pop->u.n.body != pool_cNRef);
      scp->body = pop->u.n.body + scp->offset;
    }
  }

  return scp;
}

/* Mount a volume  */

gdb_sVolume *
vol_MountVolume (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid
)
{
  gdb_sVolume		*vp;

  gdb_AssumeLocked;

  vp = gdb_AddVolume(sts, vid, gdb_mAdd__);
  if (vp == NULL) return NULL;

  vp->l.flags.b.isMounted = 1;

  return vp;
}

#if 0
/* The routine calculates the offset, size and indirection
   flag for a certain object or parameter name reference.
   The database lock must be held by the caller
   If alias then the referenced object will be used .  */

gdb_sObject *
vol_NameToAref (
  pwr_tStatus		*sts,
  pool_tRef		*oh,
  gdb_sVolume		**vhp,
  pwr_sAttrRef		*arp,
  pwr_sParam		**Param,
  cdh_sNormName		*nn,
  gdb_mLO		lo_flags,
  vol_mTrans		trans,
)
{
  gdb_sObject		*ohp;
  gdb_sVolume		*avhp;
  pwr_sParam		*lParam;

  ohp = vol_NameToOhp(sts, oh, vhp, nn, lo_flags, trans);
  if (ohp == NULL) return NULL;

  arp->Objid = ohp->g.oid;

  if (nn->flags.b.Attr) {	/* Attribute string present.  */
    aohp = vol_AnameToOhp(sts, NULL, &avhp, ohp->g.cid, nn->attribute, &Param);
    if (aohp != NULL) {
      vol_AttributeToAref(sts, arp, aohp, Param, nn->index);
    }
  } else {			/* No attribute.  */
    arp->Offset = 0;
    arp->Size = ohp->g.size;
    arp->Flags.b.Indirect = FALSE;
  }

  if (EVEN(*sts)) return NULL;

  if (parp != NULL) *parp = lparp;
  return ohp;

}
#endif

/* Get the definition of an attribute denoted by
   a full object + attribute name.

   Return the attribute definition, and optionally
   fill in 'arp' if not null.  */

mvol_sAttribute *
vol_NameToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  cdh_sParseName	*pn,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		trans
)
{
  gdb_sObject		*op;

  gdb_AssumeLocked;

  op = vol_NameToObject(sts, pn, lo_flags, trans);
  if (op == NULL) return NULL;

  ap = mvol_AnameToAttribute(sts, ap, op->g.cid, pn, op);
  if (ap == NULL) return NULL; 

  ap->op = op;
  /* !!! To do !!! Kolla att allt aer ifyllt! */

  return ap;
}

/* Get the object corresponding to a given name.  */

gdb_sObject *
vol_NameToParentObject (
  pwr_tStatus		*sts,
  cdh_sParseName	*pn,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		trans
)
{
  gdb_sObject		*op = NULL;
  gdb_sVolume		*vp;

  if (pn->flags.b.idString && pn->eId == cdh_eId_objid) {
    op = vol_OidToObject(sts, pn->uId.oid, lo_flags, trans, cvol_eHint_none);
    if (op == NULL || op->g.oid.oix == pwr_cNObjectIx)
      pwr_Return(NULL, sts, GDH__NOSUCHOBJ);
    op = vol_OidToObject(sts, op->g.f.poid, lo_flags, trans, cvol_eHint_none);
  } else if (pn->nObject > 1) {
    pn->nObject--;
    op = vol_NameToObject(sts, pn, lo_flags, trans);
    pn->nObject++;
  } else if (pn->nObject == 1) {
    if (pn->flags.b.volume) {
      vp = hash_Search(sts, gdbroot->vn_ht, &pn->volume);
      if (vp != NULL)
	op = hash_Search(sts, gdbroot->oid_ht, &vp->g.oid);
    } else {
      op = hash_Search(sts, gdbroot->oid_ht, &gdbroot->my_volume->g.oid);
    }
  }

  if (op == NULL)
    pwr_Return(NULL, sts, GDH__NOSUCHOBJ);
  return op;
}

/* Get the object corresponding to a given name.  */

gdb_sObject *
vol_NameToObject (
  pwr_tStatus		*sts,
  cdh_sParseName	*pn,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		trans
)
{
  gdb_sObject		*op = NULL;
  gdb_sObject		*pop = NULL;
  int			i;
  gdb_sVolume		*vp;

  if (pn->flags.b.idString) {
    if (pn->eId == cdh_eId_objid)
      return vol_OidToObject(sts, pn->uId.oid, lo_flags, trans, cvol_eHint_none);
    else
      pwr_Return(NULL, sts, GDH__NOSUCHOBJ);
  }

  if (pn->flags.b.volume) {
    vp = hash_Search(sts, gdbroot->vn_ht, &pn->volume);
    if (vp == NULL) return NULL; 
    op = vol_OidToObject(sts, vp->g.oid, gdb_mLo_global, vol_mTrans_all, cvol_eHint_name);
  } else if (pn->flags.b.parent) {
    op = vol_OidToObject(sts, pn->poid, gdb_mLo_global, vol_mTrans_all, cvol_eHint_name);
  } else {
    op = vol_OidToObject(sts, gdbroot->my_volume->g.oid, gdb_mLo_owned, vol_mTrans_none, cvol_eHint_none);
  }

  if (op == NULL) return NULL;

  for (i = 0; op != NULL && i < pn->nObject; i++) {
    pop = vol_TranslateObject(sts, op, lo_flags, trans);
    if (pop == NULL)
      return NULL;

    pn->object[i].poid = pop->g.oid;
    op = hash_Search(sts, gdbroot->family_ht, &pn->object[i]);
    if (op == NULL) {
      vp = pool_Address(NULL, gdbroot->pool, pop->l.vr);    
      if (vp->l.flags.b.isMounted && vp->l.flags.b.isCached)
	return cvolc_NameToObject(sts, pop, pn, i, trans);
      else
	return NULL;
    }
  }

  return vol_TranslateObject(sts, op, lo_flags, trans);
}

/* Get the address of an object body.  */

char *
vol_ObjectToAddress (
  pwr_tStatus		*sts,
  gdb_sObject		*op
)
{

  if (op == NULL) pwr_Return(NULL, sts, GDH__NOSUCHOBJ);
  if (!op->l.flags.b.isNative) pwr_Return(NULL, sts, GDH__REMOTE);

  return pool_Address(sts, gdbroot->rtdb, op->u.n.body);
}

/* Get the full name of an object.

   NOTA BENE !!!
    This function is not reentrant.  */

char *
vol_ObjectToName (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tBitMask		nametype,
  char			*buff
)
{
  char			*s = NULL;
  gdb_sObject		*pop;
  static int		level = 0;
  cdh_mName		lnametype;

  gdb_AssumeLocked;

  lnametype.m = nametype;

  if (level == 0)
    buff[0] = '\0';

  if (op->g.oid.oix != pwr_cNObjectIx) {
    if (op->l.por != pool_cNRef) {
      pop = pool_Address(NULL, gdbroot->pool, op->l.por);
      level++;
      s = vol_ObjectToName(sts, pop, nametype, buff);
      level--;
    }
    if (s == NULL)
      return NULL;
    if ((lnametype.b.path && level > 0) || (lnametype.b.object && level == 0)) {
      strcat(buff, op->g.f.name.orig);
      if (level > 0)
	strcat(buff, "-");
    }
  } else if (lnametype.b.volume) {
    strcat(buff, op->g.f.name.orig);
    strcat(buff, ":");
  }
  return buff;
}

/* Get an object identifier.  */

pwr_tObjid
vol_Oid (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  pwr_tClassId		cid
)
{

  vp->u.n.next_oid.oix++;
  return vp->u.n.next_oid;
}

/* Get the object header pointer of an object identified by an
   objid. Mount client-server translation is done only if server object
   is in root volume or in system volumes.  */

gdb_sObject *
vol_OidToObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		itrans,
  cvol_eHint		hint		/* Give a hint to the cache handler.  */
)
{
  gdb_sVolume		*vp = NULL;
  gdb_sObject		*op = NULL;
  gdb_sObject		*nop = NULL;
  vol_mTrans		trans;

  gdb_AssumeLocked;

  trans.m = itrans;

  if (cdh_ObjidIsNull(oid))
    pwr_Return(NULL, sts, GDH__NOSUCHOBJ);

  op = hash_Search(sts, gdbroot->oid_ht, &oid);
  if (op == NULL) {
    if (lo_flags & gdb_mLo_remote) {
      vp = hash_Search(sts, gdbroot->vid_ht, &oid.vid); 
      if (vp == NULL) pwr_Return(NULL, sts, GDH__NOSUCHVOL);
      if (!vp->l.flags.b.isMounted) pwr_Return(NULL, sts, GDH__NOTMOUNTED);

      if (vp->l.flags.m & lo_flags) {
	return cvolc_OidToObject(sts, vp, oid, trans.m, hint);
      }
    }
    pwr_Return(NULL, sts, GDH__NOSUCHOBJ);
  } else {
    vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
  }

  /* The order of the following two translation checks implies that
     it is possible to mount an alias, but it is not possible to
     create an alias of a mount object.  */

  if (op->g.flags.b.isMountClient) {
    if (trans.b.mount && vp->l.flags.b.transMount) {
      nop = vol_OidToObject(sts, op->g.soid, lo_flags, trans.m, hint);
      if (nop == NULL) {
	pwr_Return(op, sts, GDH__SUCCESS); /* ??? Should we do this ?? */
      } else {
	op = nop;
      }
    } else {
      /* ??? Should I return the untranslated oh or NULL ??? */
      return op;
    }
  }

  if (op->g.flags.b.isAliasClient) {
    if (trans.b.alias /* && vp->l.flags.b.transAlias */) {
      nop = vol_OidToObject(sts, op->g.soid, lo_flags, trans.m, hint);
      if (nop == NULL) {
	pwr_Return(op, sts, GDH__SUCCESS); /* ??? Should we do this ?? */
      } else {
	op = nop;
      }
    } else {
      /* ??? Should I return the untranslated oh or NULL ??? */
      return op;
    }
  }

  if (!(op->l.flags.m & lo_flags))
    pwr_Return(NULL, sts, GDH__BADOBJTYPE);

  return op;
}

/* Get the object header pointer of an object identified by an
   object index (oix). Mount client-server translation is done
   only if server object is in root volume or in system volumes.  */

gdb_sObject *
vol_OixToObject (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid,
  pwr_tObjectIx		oix,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		trans,
  cvol_eHint		hint		/* Give a hint to the cache handler.  */
)
{
  pwr_tObjid		oid;

  oid.vid = vid;
  oid.oix = oix;

  return vol_OidToObject(sts, oid, lo_flags, trans, hint);
}

/* Propagate an alarmlevel upwards.  */

void
vol_PropagateAlarmLevel (
  pwr_tStatus		*status,
  gdb_sObject		*op,
  pwr_tBoolean		l_maxa,
  pwr_tUInt32		old_maxa,
  pwr_tUInt32		new_maxa,
  pwr_tBoolean		propagate_alias
)
{
  pwr_tUInt32		pold_maxa;	/* Parents old max alarm level.  */
  pwr_tUInt32		pnew_maxa;	/* Parents new max alarm level.  */
  pwr_tUInt32		max = 0; 
  pwr_tUInt32		max2; 
  gdb_sObject		*pop;		/* Parent object pointer.  */
  pool_sQlink		*col;		/* Child object link.  */
  gdb_sObject		*cop;		/* Child object pointer.  */
  pool_sQlink		*aol;		/* Alias client object link.  */
  gdb_sObject		*aop;		/* Alias client object pointer.  */
  gdb_sAliasServer	*asp;		/* Alias server pointer.  */

  pwr_dStatus(sts, status, 1);

  if (op->g.oid.oix == pwr_cNObjectIx)
    return; /* This is the volume object.  */

  if (old_maxa == new_maxa)
    return; /* There is no change.  */

  pop = pool_Address(NULL, gdbroot->pool, op->l.por);

  if (pop->l.al.idx >= op->l.al.idx)
    return; /* We have already been here.  */

  pold_maxa = l_maxa ? pop->l.al.maxa : MAX(pop->u.n.ral.maxa, pop->l.al.a);

  if (old_maxa < new_maxa) {
    if (l_maxa)
      pop->l.al.maxa = MAX(pop->l.al.maxa, new_maxa);
    else
      pop->u.n.ral.maxa = MAX(pop->u.n.ral.maxa, new_maxa);

  } else {
    for (
      col = pool_Qsucc(NULL, gdbroot->pool, &pop->u.n.sib_lh);
      col != &pop->u.n.sib_lh;
      col = pool_Qsucc(NULL, gdbroot->pool, col)
    ) {
      cop = pool_Qitem(col, gdb_sObject, u.n.sib_ll);
      if (l_maxa)
        max2 = cop->g.flags.b.isMountClient ? MAX(cop->l.al.maxa, cop->l.al.a) : cop->l.al.maxa;
      else
        max2 = cop->g.flags.b.isMountClient ? 0 : MAX(cop->u.n.ral.maxa, cop->l.al.a);
      max = MAX(max, max2);
    }

    if (l_maxa) {
      if (max != pop->l.al.maxa)
        pop->l.al.maxa = max;
    } else {
      if (max != pop->u.n.ral.maxa)
        pop->u.n.ral.maxa = max;
    }
  }

  pop->l.al.idx = op->l.al.idx;
  pnew_maxa = l_maxa ? pop->l.al.maxa : MAX(pop->u.n.ral.maxa, pop->l.al.a);

  if (pop->l.flags.b.isMountServer) {

    /* Walk trough all mount clients.  */
    gdb_sMountServer *sp;
    unsigned int o_maxa, n_maxa;
    
    sp = hash_Search(sts, gdbroot->ms_ht, &pop->g.oid);
    if (sp == NULL) return;

    for (
      col = pool_Qsucc(NULL, gdbroot->pool, &sp->cli_lh);
      col != &sp->cli_lh;
      col = pool_Qsucc(NULL, gdbroot->pool, col)
    ) {
      cop = pool_Qitem(col, gdb_sObject, u.n.cli_ll);
      o_maxa = MAX(cop->l.al.a, cop->l.al.maxa);
      cop->l.al.maxa = pop->u.n.ral.maxa;
      cop->l.al.idx = pop->l.al.idx;
      n_maxa = MAX(cop->l.al.a, cop->l.al.maxa);
      if (o_maxa != n_maxa)
	vol_PropagateAlarmLevel(sts, cop, YES, o_maxa, n_maxa, YES);
    }
 
  }

  if (propagate_alias && pop->g.flags.b.isAliasServer && pop->l.flags.b.isOwned) {

    /* Walk trough all alias clients.  */
 
    /* Left todo */
    asp = (gdb_sAliasServer *) hash_Search(sts, gdbroot->as_ht, &pop->g.oid);
    pwr_Assert(asp != NULL);
    for (
      aol = pool_Qsucc(NULL, gdbroot->pool, &asp->cli_lh);
      aol != &asp->cli_lh;
      aol = pool_Qsucc(NULL, gdbroot->pool, aol)
    ) {
      aop = pool_Qitem(aol, gdb_sObject, u.n.cli_ll);
      pwr_Assert(aop->g.flags.b.isAliasClient && aop->u.n.flags.b.inAliasClientList);
      aop->l.al.a = pop->l.al.a;
      aop->l.al.maxa = pop->l.al.maxa;
      aop->u.n.ral.maxa = pop->u.n.ral.maxa;
      aop->l.al.idx = pop->l.al.idx;
      if (pold_maxa != pnew_maxa)
	vol_PropagateAlarmLevel(sts, aop, l_maxa, pold_maxa, pnew_maxa, YES);
    }
    pop->l.al.idx = ++gdbroot->db->al_idx;
  }

  if (pold_maxa != pnew_maxa)
    vol_PropagateAlarmLevel(sts, pop, l_maxa, pold_maxa, pnew_maxa, propagate_alias);
}

void
vol_PropagateBlockLevel (
  pwr_tStatus		*status,
  gdb_sObject		*op,
  pwr_tBoolean		l_maxb,
  pwr_tUInt32		old_maxb,
  pwr_tUInt32		new_maxb,
  pwr_tBoolean		propagate_alias
)
{
  pwr_tUInt32		pold_maxb;	/* Parents old max alarm level.  */
  pwr_tUInt32		pnew_maxb;	/* Parents new max alarm level.  */
  pwr_tUInt32		max = 0; 
  pwr_tUInt32		max2; 
  gdb_sObject		*pop;		/* Parent object pointer.  */
  pool_sQlink		*col;		/* Child object link.  */
  gdb_sObject		*cop;		/* Child object pointer.  */
  pool_sQlink		*aol;		/* Alias client object link.  */
  gdb_sObject		*aop;		/* Alias client object pointer.  */
  gdb_sAliasServer	*asp;		/* Alias server pointer.  */

  pwr_dStatus(sts, status, 1);


  if (op->g.oid.oix == pwr_cNObjectIx)
    return; /* This is the volume object.  */

  if (old_maxb == new_maxb)
    return; /* There is no change.  */


  pop = pool_Address(NULL, gdbroot->pool, op->l.por);

  if (pop->l.al.idx >= op->l.al.idx)
    return; /* We have already been here.  */

  pold_maxb = l_maxb ? pop->l.al.maxb : MAX(pop->u.n.ral.maxb, pop->l.al.b);

  if (old_maxb < new_maxb) {
    if (l_maxb)
      pop->l.al.maxb = MAX(pop->l.al.maxb, new_maxb);
    else
      pop->u.n.ral.maxb = MAX(pop->u.n.ral.maxb, new_maxb);

  } else {
    for (
      col = pool_Qsucc(NULL, gdbroot->pool, &pop->u.n.sib_lh);
      col != &pop->u.n.sib_lh;
      col = pool_Qsucc(NULL, gdbroot->pool, col)
    ) {
      cop = pool_Qitem(col, gdb_sObject, u.n.sib_ll);
      if (l_maxb)
        max2 = cop->g.flags.b.isMountClient ? MAX(cop->l.al.maxb, cop->l.al.b) : cop->l.al.maxb;
      else
        max2 = cop->g.flags.b.isMountClient ? 0 : MAX(cop->u.n.ral.maxb, cop->l.al.b);
      max = MAX(max, max2);
    }

    if (l_maxb) {
      if (max != pop->l.al.maxb)
        pop->l.al.maxb = max;
    } else {
      if (max != pop->u.n.ral.maxb)
        pop->u.n.ral.maxb = max;
    }
  }

  pop->l.al.idx = op->l.al.idx;
  pnew_maxb = l_maxb ? pop->l.al.maxb : MAX(pop->u.n.ral.maxb, pop->l.al.b);

  if (pop->l.flags.b.isMountServer) {

    /* Walk trough all mount clients.  */
    gdb_sMountServer *sp;
    unsigned int o_maxb, n_maxb;
    
    sp = hash_Search(sts, gdbroot->ms_ht, &pop->g.oid);
    if (sp == NULL) return;

    for (
      col = pool_Qsucc(NULL, gdbroot->pool, &sp->cli_lh);
      col != &sp->cli_lh;
      col = pool_Qsucc(NULL, gdbroot->pool, col)
    ) {
      cop = pool_Qitem(col, gdb_sObject, u.n.cli_ll);
      o_maxb = MAX(cop->l.al.b, cop->l.al.maxb);
      cop->l.al.maxb = pop->u.n.ral.maxb;
      cop->l.al.idx = pop->l.al.idx;
      n_maxb = MAX(cop->l.al.b, cop->l.al.maxb);
      if (o_maxb != n_maxb)
	vol_PropagateBlockLevel(sts, cop, YES, o_maxb, n_maxb, YES);
    }
 
  }



  if (propagate_alias && pop->g.flags.b.isAliasServer && pop->l.flags.b.isOwned) {

    /* Walk trough all alias clients.  */
 
    asp = (gdb_sAliasServer *) hash_Search(sts, gdbroot->as_ht, &pop->g.oid);
    pwr_Assert(asp != NULL);
    for (
      aol = pool_Qsucc(NULL, gdbroot->pool, &asp->cli_lh);
      aol != &asp->cli_lh;
      aol = pool_Qsucc(NULL, gdbroot->pool, aol)
    ) {
      aop = pool_Qitem(aol, gdb_sObject, u.n.cli_ll);
      pwr_Assert(aop->g.flags.b.isAliasClient && aop->u.n.flags.b.inAliasClientList);
      aop->l.al.b = pop->l.al.b;
      aop->l.al.maxb = pop->l.al.maxb;
      aop->u.n.ral.maxb = pop->u.n.ral.maxb;
      aop->l.al.idx = pop->l.al.idx;
      if (pold_maxb != pnew_maxb)
	vol_PropagateBlockLevel(sts, aop, l_maxb, pold_maxb, pnew_maxb, YES);
    }
    pop->l.al.idx = ++gdbroot->db->al_idx;
  }

  if (pold_maxb != pnew_maxb)
    vol_PropagateBlockLevel(sts, pop, l_maxb, pold_maxb, pnew_maxb, propagate_alias);
}

void
vol_RemoveMountClient (
  pwr_tStatus           *sts,
  gdb_sObject		*op
)
{

#if 0
  pool_QueRemove(sts, mcep->msclst, msp->msclst);
#endif
}

void
vol_RemoveMountedOn (
  pwr_tStatus           *sts,
  gdb_sMountedOn	*mop
)
{

  pool_Qremove(sts, gdbroot->pool, &mop->volmo_ll);
  pool_Qremove(sts, gdbroot->pool, &mop->nodmo_ll);
  pool_Free(sts, gdbroot->pool, mop);

}

gdb_sMountServer *
vol_RemoveMountServer (
  pwr_tStatus           *status,
  pwr_tObjid		oid
)
{
  gdb_sMountServer	*msp = NULL;

  pwr_dStatus(sts, status, GDH__SUCCESS);
#if 0
  msp = hash_Search(sts, ms, vhp->mstab, &oid);
  if (msp == NULL) 
  msp = hash_Remove(sts, ms, vhp->mstab, &oid);
  if (msp == NULL) 
  }
  pool_QueRemove(sts, msp->mslst, vhp->gvhp->msclst);
  if (vhp->gvhp->flags.b.IsMounted) {
    ohp = vol_OixToOhp(sts, &oh, vhp, oid);
    if (ohp != NULL) {
      msp->oh = oh;
      ohp->l.flags.b.isMountServer = 0;
    }
  }
  pool_Free(sts, gdbroot->dbpool, );

#endif
  return msp;
}

/* Remove an object from the list of siblings.  */

void
vol_RemoveSiblist (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sObject		*pop
)
{
  pwr_tVolumeId		vid = pop->g.oid.vid;
  gdb_sObject		*f_op;
  gdb_sObject		*b_op;

  pwr_Assert(pop->g.flags.b.isParent);
  pwr_Assert(cdh_ObjidIsNotNull(pop->g.soid));
  pwr_Assert(op->g.flags.b.inSibList);

  f_op = vol_OixToObject(sts, vid, op->g.sib.flink, gdb_mLo_native, vol_mTrans_none, cvol_eHint_none);
  if (f_op == NULL) errh_Bugcheck(GDH__WEIRD, "vol_RemoveSiblist forward");
  b_op = vol_OixToObject(sts, vid, op->g.sib.blink, gdb_mLo_native, vol_mTrans_none, cvol_eHint_none);
  if (b_op == NULL) errh_Bugcheck(GDH__WEIRD, "vol_RemoveSiblist backward");

  if (op == f_op) {
    pwr_Assert(f_op == b_op);
    pop->g.soid = pwr_cNObjid;
  } else {
    f_op->g.sib.blink = op->g.sib.blink;
    b_op->g.sib.flink = op->g.sib.flink;
    if (cdh_ObjidIsEqual(pop->g.soid, op->g.oid)) {
      pop->g.soid = f_op->g.oid;
    }
  }

  op->g.sib.flink = op->g.sib.blink = pwr_cNObjectIx;
  op->g.flags.b.inSibList = 0;
  if (cdh_ObjidIsNull(pop->g.soid)) pop->g.flags.b.isParent = 0;
}

void
vol_SetAlarmLevel (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tUInt32		a
)
{
  pwr_tUInt32		old_rmaxa;
  pwr_tUInt32		new_rmaxa;

  if (!op->l.flags.b.isOwned) {
    *sts = GDH__NOTOWNED;
    return;
  }

  if (a == op->l.al.a)
    return;

  old_rmaxa = MAX(op->l.al.a, op->u.n.ral.maxa);
  op->l.al.a = a;
  op->l.al.idx = ++gdbroot->db->al_idx;
  op->u.n.ral.idx = gdbroot->db->al_idx;
  new_rmaxa = MAX(op->l.al.a, op->u.n.ral.maxa);


  if (op->l.flags.b.isMountServer) {
    gdb_sMountServer *sp;
    pool_sQlink		*col;		
    gdb_sObject		*cop;
    
    sp = hash_Search(sts, gdbroot->ms_ht, &op->g.oid);
    if (sp == NULL) return;

    for (
      col = pool_Qsucc(NULL, gdbroot->pool, &sp->cli_lh);
      col != &sp->cli_lh;
      col = pool_Qsucc(NULL, gdbroot->pool, col)
    ) {
      cop = pool_Qitem(col, gdb_sObject, u.n.cli_ll);
      cop->l.al.a = op->l.al.a;
      cop->u.n.ral.maxa = op->l.al.maxa;
      cop->l.al.idx = op->l.al.idx;
      if (old_rmaxa != new_rmaxa)
	vol_PropagateAlarmLevel(sts, cop, YES, old_rmaxa, new_rmaxa, YES);
    }
  } 



  if (op->g.flags.b.isAliasServer) {
    pool_sQlink		*aol;		/* Alias client object link.  */
    gdb_sObject		*aop;		/* Alias client object pointer.  */
    gdb_sAliasServer	*asp;		/* Alias server pointer.  */

    /* Walk trough all alias clients.  */
 
    asp = (gdb_sAliasServer *) hash_Search(NULL, gdbroot->as_ht, &op->g.oid);
    pwr_Assert(asp != NULL);
    for (
      aol = pool_Qsucc(NULL, gdbroot->pool, &asp->cli_lh);
      aol != &asp->cli_lh;
      aol = pool_Qsucc(NULL, gdbroot->pool, aol)
    ) {
      aop = pool_Qitem(aol, gdb_sObject, u.n.cli_ll);
      pwr_Assert(aop->g.flags.b.isAliasClient && aop->u.n.flags.b.inAliasClientList);
      aop->l.al.a = op->l.al.a;
      aop->l.al.maxa = op->l.al.maxa;
      aop->u.n.ral.maxa = op->u.n.ral.maxa;
      aop->l.al.idx = op->l.al.idx;
      if (old_rmaxa != new_rmaxa)
	vol_PropagateAlarmLevel(sts, aop, NO, old_rmaxa, new_rmaxa, YES);
    }
    op->l.al.idx = ++gdbroot->db->al_idx;
  }

  if (old_rmaxa != new_rmaxa)
    vol_PropagateAlarmLevel(sts, op, NO, old_rmaxa, new_rmaxa, YES);
}

void
vol_SetBlockLevel (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tUInt32		b
)
{
  pwr_tUInt32		old_rmaxb;
  pwr_tUInt32		new_rmaxb;

  if (!op->l.flags.b.isOwned) {
    *sts = GDH__NOTOWNED;
    return;
  }

  if (b == op->l.al.b)
    return;

  old_rmaxb = MAX(op->l.al.b, op->u.n.ral.maxb);
  op->l.al.b = b;
  op->l.al.idx = ++gdbroot->db->al_idx;
  op->u.n.ral.idx = gdbroot->db->al_idx;
  new_rmaxb = MAX(op->l.al.b, op->u.n.ral.maxb);


  if (op->l.flags.b.isMountServer) {
    gdb_sMountServer    *sp;
    pool_sQlink		*col;		
    gdb_sObject		*cop;
    
    sp = hash_Search(sts, gdbroot->ms_ht, &op->g.oid);
    if (sp == NULL) return;

    for (
      col = pool_Qsucc(NULL, gdbroot->pool, &sp->cli_lh);
      col != &sp->cli_lh;
      col = pool_Qsucc(NULL, gdbroot->pool, col)
    ) {
      cop = pool_Qitem(col, gdb_sObject, u.n.cli_ll);
      cop->l.al.b = op->l.al.b;
      cop->u.n.ral.maxb = op->l.al.maxb;
      cop->l.al.idx = op->l.al.idx;
      if (old_rmaxb != new_rmaxb)
	vol_PropagateBlockLevel(sts, cop, YES, old_rmaxb, new_rmaxb, YES);
    }
  } 


  if (op->g.flags.b.isAliasServer) {
    pool_sQlink		*aol;		/* Alias client object link.  */
    gdb_sObject		*aop;		/* Alias client object pointer.  */
    gdb_sAliasServer	*asp;		/* Alias server pointer.  */

    /* Walk trough all alias clients.  */
 
    asp = (gdb_sAliasServer *) hash_Search(NULL, gdbroot->as_ht, &op->g.oid);
    pwr_Assert(asp != NULL);
    for (
      aol = pool_Qsucc(NULL, gdbroot->pool, &asp->cli_lh);
      aol != &asp->cli_lh;
      aol = pool_Qsucc(NULL, gdbroot->pool, aol)
    ) {
      aop = pool_Qitem(aol, gdb_sObject, u.n.cli_ll);
      pwr_Assert(aop->g.flags.b.isAliasClient && aop->u.n.flags.b.inAliasClientList);
      aop->l.al.b = op->l.al.b;
      aop->l.al.maxb = op->l.al.maxb;
      aop->u.n.ral.maxb = op->u.n.ral.maxb;
      aop->l.al.idx = op->l.al.idx;
      if (old_rmaxb != new_rmaxb)
	vol_PropagateBlockLevel(sts, aop, NO, old_rmaxb, new_rmaxb, YES);
    }
    op->l.al.idx = ++gdbroot->db->al_idx;
  }

  if (old_rmaxb != new_rmaxb)
    vol_PropagateBlockLevel(sts, op, NO, old_rmaxb, new_rmaxb, YES);
}

/* Find a sibling identified by its name.  */

gdb_sObject *
vol_FamilyToObject (
  pwr_tStatus		*sts,
  char			*name,
  pwr_tObjid		poid
)
{
  cdh_sFamily		f;

  return (gdb_sObject *) hash_Search(sts, gdbroot->family_ht, cdh_Family(&f, name, poid));
}

gdb_sObject *
vol_TranslateObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		itrans
)
{
  gdb_sVolume		*vp;
  vol_mTrans		trans;

  trans.m = itrans;

  if (op->g.flags.b.isAliasClient && trans.b.alias) {
    op = vol_OidToObject(sts, op->g.soid, lo_flags, trans.m, cvol_eHint_none);
  }

  if (op != NULL && op->g.flags.b.isMountClient && trans.b.mount) {
    vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
    if (vp->l.flags.b.transMount) {
      op = vol_OidToObject(sts, op->g.soid, lo_flags, trans.m, cvol_eHint_none);
    }
  }

  return op;
}

/* Unlink an object.  */

void
vol_UnlinkObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		ilink
)
{
  gdb_sObject		*pop = NULL;
  vol_mLink		link;
  gdb_sMountServer	*msp;

  link.m = ilink;

  gdb_AssumeLocked;

  if (op->l.por != pool_cNRef)
    pop = pool_Address(NULL, gdbroot->pool, op->l.por);

  /* Global part. This is only done in native volumes.  */

  if (link.b.goAdopt && op->l.flags.b.isLoaded && pop != NULL) {
    vol_RemoveSiblist(NULL, op, pop);
  }

  /* Local part.  */

  if (link.b.loObjList && op->l.flags.b.inObjList) {
    pwr_Assert(pool_QisLinked(NULL, gdbroot->pool, &op->l.obj_ll));
    pool_Qremove(NULL, gdbroot->pool, &op->l.obj_ll);
    op->l.flags.b.inObjList = 0;
  }

  if (link.b.loOidTab && op->l.flags.b.inOidTab) {
    pwr_Assert(op == hash_Search(NULL, gdbroot->oid_ht, &op->g.oid));
    hash_Remove(NULL, gdbroot->oid_ht, op);
    op->l.flags.b.inOidTab = 0;
  }

  if (link.b.loFamilyTab && op->l.flags.b.inFamilyTab) {
    pwr_Assert(op == hash_Search(NULL, gdbroot->family_ht, &op->g.f));
    hash_Remove(NULL, gdbroot->family_ht, op);
    op->l.flags.b.inFamilyTab = 0;
  }

  if (link.b.loParentRef)
    op->l.por = pool_cNRef;

  if (link.b.loVolumeRef)
    op->l.vr = pool_cNRef;

  /* Check if mount server.  */

  if (link.b.loMountServer && op->l.flags.b.isMountServer) {
    msp = hash_Search(NULL, gdbroot->ms_ht, &op->g.oid);
    if (msp != NULL) {
      msp->msor = pool_cNRef;
    }
    op->l.flags.b.isMountServer = 0;
  }

  /* Native/Cached part.  */

  if (op->l.flags.b.isNative) {
    
    if (link.b.noSibList && op->u.n.flags.b.inSibList) {
      pool_Qremove(NULL, gdbroot->pool, &op->u.n.sib_ll);
      op->u.n.flags.b.inSibList = 0;
    }
    if (pop != NULL) {
      if (pool_QisLinked(NULL, gdbroot->pool, &pop->u.n.sib_lh))
	pwr_Assert(pop->u.n.flags.b.hasChild);
      else {
	pop->u.n.flags.b.hasChild = 0;
	if (pop->u.n.flags.b.pendingDelete)
	  gdb_RemoveObject(NULL, pop);
      }
    }

    if(link.b.noCidList && op->u.n.flags.b.inCidList) {
#if 1
      pool_Qremove(NULL, gdbroot->pool, &op->u.n.cid_ll);
      op->u.n.flags.b.inCidList = 0;
#else
      mvol_UnlinkObject(NULL, vp, op, link.m);    
#endif
    }

    if(link.b.noAliasClientList && op->g.flags.b.isAliasClient && op->u.n.flags.b.inAliasClientList) {
#if 1
      pool_Qremove(NULL, gdbroot->pool, &op->u.n.cli_ll);
      op->u.n.flags.b.inAliasClientList = 0;
#else
      vol_RemoveAliasClient(NULL, op);    
#endif
    }

    if (link.b.noMountClientList && op->g.flags.b.isMountClient && op->u.n.flags.b.inMountClientList) {
#if 1
      pool_Qremove(NULL, gdbroot->pool, &op->u.n.cli_ll);
      op->u.n.flags.b.inMountClientList = 0;
#else
      vol_RemoveMountClient(NULL, op);    
#endif
    }

    if (link.b.noSub && op->u.n.subcount != 0) {
      sub_UnlinkObject(op);
    }

    if (link.b.init) {
      gdb_RemoveObject(NULL, op);
    }

  } else if (op->l.flags.b.isCached) {


  }

}

void
vol_UpdateAlarm (
  pwr_tStatus		*status,
  gdb_sObject		*op,
  net_sAlarm		al
)
{
  pool_sQlink		*col;		/* Child object link.  */
  gdb_sObject		*cop;		/* Child object pointer.  */

  pwr_dStatus(sts, status, 1);

  gdb_AssumeLocked;
  

  if (op->l.flags.b.isMountServer) {
    gdb_sMountServer 	*sp;
    pwr_tUInt32		old_maxa;
    pwr_tUInt32		new_maxa;
    pwr_tUInt32		old_maxb;
    pwr_tUInt32		new_maxb;
    
    old_maxa = MAX(op->l.al.a, op->l.al.maxa);
    op->l.al.idx = ++gdbroot->db->al_idx;
    op->l.al.a = al.a;
    op->l.al.maxa = al.maxa;
    new_maxa = MAX(op->l.al.a, op->l.al.maxa);

    sp = hash_Search(sts, gdbroot->ms_ht, &op->g.oid);
    if (sp == NULL) return;

    for (
      col = pool_Qsucc(NULL, gdbroot->pool, &sp->cli_lh);
      col != &sp->cli_lh;
      col = pool_Qsucc(NULL, gdbroot->pool, col)
    ) {
      cop = pool_Qitem(col, gdb_sObject, u.n.cli_ll);
      cop->l.al.a = op->l.al.a;
      cop->l.al.maxa = op->l.al.maxa;
      cop->l.al.idx = op->l.al.idx;
      if (old_maxa != new_maxa)
	vol_PropagateAlarmLevel(sts, cop, YES, old_maxa, new_maxa, YES);
    }


    old_maxb = MAX(op->l.al.b, op->l.al.maxb);
    op->l.al.idx = ++gdbroot->db->al_idx;
    op->l.al.b = al.b;
    op->l.al.maxb = al.maxb;
    new_maxb = MAX(op->l.al.b, op->l.al.maxb);

    for (
      col = pool_Qsucc(NULL, gdbroot->pool, &sp->cli_lh);
      col != &sp->cli_lh;
      col = pool_Qsucc(NULL, gdbroot->pool, col)
    ) {
      cop = pool_Qitem(col, gdb_sObject, u.n.cli_ll);
      cop->l.al.b = op->l.al.b;
      cop->l.al.maxb = op->l.al.maxb;
      cop->l.al.idx = op->l.al.idx;
      if (old_maxb != new_maxb)
	vol_PropagateBlockLevel(sts, cop, YES, old_maxb, new_maxb, YES);
    }


  } else  
    op->l.al = al;


}

//
// Get DisableAttr value on local object
//
pwr_tDisableAttr
vol_ArefDisabled (
  pwr_tStatus 		*sts,
  pwr_sAttrRef		*arp
)
{
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;
  void			*p;
  pwr_sAttrRef		daref;

  memset(&attribute, 0, sizeof(attribute));

  gdb_AssumeLocked;

  daref = cdh_ArefToDisableAref( arp);
  ap = vol_ArefToAttribute(sts, &attribute, &daref, gdb_mLo_native, vol_mTrans_none);
  if (ap == NULL) return pwr_cNDisableAttr;

  p = vol_AttributeToAddress(sts, ap);
  if ( p) 
    return *(pwr_tDisableAttr *)p;
  return pwr_cNDisableAttr;
}

#if 0

gdb_sVolume *
vol_VolumeList (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  vol_mClass		volclass,
  vol_mType		voltype,
  pwr_tNodeId		owner,
  vol_eList		list
)
{
  pool_sQlink		*vl;

  do {
    switch (list) {
    case vol_eList_first:
      vl = pool_Qsucc(NULL, gdbroot->pool, &gdbroot->db->vol_lh);
      list = vol_eList_Next;
      break;
    case vol_eList_last:
      vl = pool_Qpred(NULL, gdbroot->pool, &gdbroot->db->vol_lh);
      list = vol_eList_Prev;
      break;
    case vol_eList_next:
      vl = pool_Qsucc(NULL, gdbroot->pool, &vp->vol_ll);
      break;
    case vol_eList_prev:
      vl = pool_Qpred(NULL, gdbroot->pool, &vp->vol_ll);
      break;
    default:
      *sts = GDH__WEIRD;
      return NULL;
    }

    if (vl == NULL || vl == &gdbroot->db->vol_lh) {
      vp = NULL;
      break;
    }

    vp = pool_Qitem(vl, gdb_sVolume, vol_ll);
    if (
      vp != NULL &&
      vp->class & volclass &&
      vp->type & voltype &&
      vp->flags.b.IsMounted &&
      (owner == pwr_cNNodeId || vp->nid == owner)
    ) {
      break;
    }
  } while (vp != NULL);

  if (vp == NULL)
    *sts = GDH__NOSUCHVOL;

  return vp;
}
#endif
