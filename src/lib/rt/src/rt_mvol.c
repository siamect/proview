/* rt_mvol.c -- Meta Volumes

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   .  */

#if defined(OS_ELN)
# include $vaxelnc
#else
# include <stdio.h>
# include <string.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "rt_errh.h"
#include "co_cdh.h"
#include "rt_gdh_msg.h"
#include "rt_mvol_msg.h"
#include "rt_gdb.h"
#include "rt_vol.h"
#include "rt_mvol.h"
#include "rt_pool.h"


static pwr_tBoolean
  decodeAttribute (
    pwr_tStatus         *sts,
    gdb_sObject		*op,
    co_eBO              bo
  );

static pwr_tBoolean
  decodeClassDef (
    pwr_tStatus         *sts,
    gdb_sObject		*op,
    co_eBO              bo
  );

static pwr_tBoolean
  decodeObjBodyDef (
    pwr_tStatus         *sts,
    gdb_sObject		*op,
    co_eBO              bo
  );



  /* Decode an Attribute object.  */

static pwr_tBoolean
  decodeAttribute (
    pwr_tStatus         *sts,
    gdb_sObject		*op,
    co_eBO              bo)
  {
    pwr_sParam		*p;
    co_mFormat          fm;
    
    co_GetOwnFormat(&fm);

    if (bo == fm.b.bo)
      return YES;


    p = pool_Address(sts, gdbroot->rtdb, op->u.n.body);
    if (p == NULL) return NO;

    ENDIAN_SWAP_INTP(&p->Info.Type);
    ENDIAN_SWAP_INTP(&p->Info.Offset);
    ENDIAN_SWAP_INTP(&p->Info.Size);
    ENDIAN_SWAP_INTP(&p->Info.Flags);
    ENDIAN_SWAP_INTP(&p->Info.Elements);
    ENDIAN_SWAP_INTP(&p->Info.ParamIndex);
    ENDIAN_SWAP_INTP(&p->TypeRef);

    op->u.n.flags.b.bodyDecoded = 1;

    return YES;
  }
  
  /* Decode a ClassDef object.  */

  static pwr_tBoolean
  decodeClassDef (
    pwr_tStatus         *sts,
    gdb_sObject		*op,
    co_eBO              bo
  )
  {
    pwr_sClassDef	*p;
    co_mFormat          fm;
    
    co_GetOwnFormat(&fm);

    if (bo == fm.b.bo)
      return YES;

    p = pool_Address(sts, gdbroot->rtdb, op->u.n.body);
    if (p == NULL) return NO;

    ENDIAN_SWAP_INTP(&p->Editor);
    ENDIAN_SWAP_INTP(&p->Method);
    ENDIAN_SWAP_INTP(&p->Flags);
    ENDIAN_SWAP_INTP(&p->PopEditor);

    op->u.n.flags.b.bodyDecoded = 1;

    return YES;
  }

  /* Decode an ObjBodyDef object.  */

  static pwr_tBoolean
  decodeObjBodyDef (
    pwr_tStatus         *sts,
    gdb_sObject		*op,
    co_eBO              bo
  )
  {
    pwr_sObjBodyDef	*p;
    co_mFormat          fm;
    
    co_GetOwnFormat(&fm);

    if (bo == fm.b.bo)
      return YES;

    p = pool_Address(sts, gdbroot->rtdb, op->u.n.body);
    if (p == NULL) return NO;

    ENDIAN_SWAP_INTP(&p->NumOfParams);
    ENDIAN_SWAP_INTP(&p->Size);
    ENDIAN_SWAP_INTP(&p->NextAix);

    op->u.n.flags.b.bodyDecoded = 1;

    return YES;
  }


/* Get the definition of an attribute denoted by
   class identity and attribute name.

   Return the attribute definition, and optionally
   fill in 'arp' if not null.  */

mvol_sAttribute *
mvol_AnameToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_tClassId		cid,
  cdh_sParseName	*pn
)
{
  cdh_uObjid            coid;
    
  ap->cp = hash_Search(sts, gdbroot->cid_ht, &cid);
  if (ap->cp == NULL) pwr_Return(NULL, sts, GDH__NOSUCHCLASS);

  ap->bop = pool_Address(sts, gdbroot->pool, ap->cp->bor);
  if (ap->bop == NULL) pwr_Return(NULL, sts, GDH__ATTRIBUTE);

  if (pn == NULL || !pn->flags.b.attribute) {
    /* Use the whole body.  */
    
    ap->aop = NULL;
    ap->adef = NULL;
    ap->idx = ULONG_MAX;
  } else {
    pn->attribute.poid = ap->bop->g.oid;
    ap->aop = hash_Search(sts, gdbroot->family_ht, &pn->attribute);
    if (ap->aop == NULL) pwr_Return(NULL, sts, GDH__ATTRIBUTE);
    ap->adef = pool_Address(NULL, gdbroot->rtdb, ap->aop->u.n.body);
    if (pn->flags.b.index)
      ap->idx = pn->index;
    else
      ap->idx = ULONG_MAX;
  }

  ap->cop   = pool_Address(NULL, gdbroot->pool, ap->cp->cor);
  ap->cdef  = pool_Address(NULL, gdbroot->rtdb, ap->cp->cbr);
  ap->bop   = pool_Address(NULL, gdbroot->pool, ap->cp->bor);
  ap->bdef  = pool_Address(NULL, gdbroot->rtdb, ap->cp->bbr);
  ap->flags.m = 0;

  if (ap->adef != NULL) {
    coid.pwr = ap->aop->g.oid;
    ap->aix   = coid.t.aix;
    ap->size  = ap->adef->Info.Size;
    ap->offs  = ap->adef->Info.Offset;
    ap->tid   = ap->adef->Info.Type;
    ap->elem  = ap->adef->Info.Elements;
    ap->flags.b.Indirect = ((ap->adef->Info.Flags & PWR_MASK_POINTER) != 0) &&
      ((ap->adef->Info.Flags & PWR_MASK_PRIVATE) == 0);
    if (ap->idx != ULONG_MAX) {
      if (ap->idx > ap->adef->Info.Elements - 1)
	pwr_Return(NULL, sts, GDH__SUBSCRIPT);
      ap->size /= ap->elem;
      ap->offs += ap->size * ap->idx;
      ap->elem = 1;
    }
  } else {
    ap->aix   = ULONG_MAX;
    ap->size  = ap->cp->size;
    ap->offs  = 0;
    ap->tid   = cdh_TypeObjidToId(ap->bop->g.oid);
    ap->elem  = 1;
  }

  pwr_Return(ap, sts, GDH__SUCCESS);
}

/* Translate an attribute reference to the internal
   attribute format.  */

mvol_sAttribute *
mvol_ArefToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_sAttrRef		*arp,
  pwr_tClassId		cid
)
{
  pwr_sParam		*param;
  gdb_sClass		*cp;
  cdh_uObjid            coid;
  int			i;


#if 0
  /* This must wait until next version.  */
  pwr_Assert(cid == arp->Body);
  pwr_tDlid		dlid;		/* pwr_tDlid, must be at same offset as
					   pwr_tSubid in subscription clients and servers */
#endif

  cp = hash_Search(sts, gdbroot->cid_ht, &cid);
  if (cp == NULL) {
    pwr_Return(NULL, sts, GDH__NOSUCHCLASS);
  }

  for (i=0; i < cp->acount; i++) {
    if (arp->Offset <= cp->attr[i].moffset) break;
  }

  if (i >= cp->acount)
    pwr_Return(NULL, sts, GDH__ATTRIBUTE);

  /* Check that attribute reference is on a even boundary */

  param = pool_Address(NULL, gdbroot->rtdb, cp->attr[i].abr);

  if (arp->Offset > param->Info.Offset &&
      ((arp->Offset - param->Info.Offset) % (param->Info.Size / param->Info.Elements)) != 0)
    pwr_Return(NULL, sts, GDH__ATTRIBUTE);

  /* We now have a match! Figure out the details!  */

  ap->aop   = pool_Address(NULL, gdbroot->pool, cp->attr[i].aor);
  ap->adef  = param;
  ap->idx   = ULONG_MAX;		/* Guess, no index.  */

  if (arp->Size > (param->Info.Size / param->Info.Elements) || arp->Size == 0) {

    /* If this is the first attribute, then match whole object
       otherwise say the attribute is ok!  */

    if (param->Info.ParamIndex == 0) {
      ap->aop = NULL;
      ap->adef = NULL;
    }
  } else if (param->Info.Elements > 1) {

    /* Calculate index.  */

    ap->idx = (arp->Offset - param->Info.Offset) / (param->Info.Size / param->Info.Elements);
  }

  ap->cp    = cp;
  ap->cop   = pool_Address(NULL, gdbroot->pool, cp->cor);
  ap->cdef  = pool_Address(NULL, gdbroot->rtdb, cp->cbr);
  ap->bop   = pool_Address(NULL, gdbroot->pool, cp->bor);
  ap->bdef  = pool_Address(NULL, gdbroot->rtdb, cp->bbr);

  if (ap->aop != NULL) {
    coid.pwr  = ap->aop->g.oid;
    ap->aix   = coid.t.aix;
    ap->size  = param->Info.Size;
    ap->offs  = param->Info.Offset;
    ap->tid   = param->Info.Type;
    ap->elem  = param->Info.Elements;

    if (ap->idx != ULONG_MAX) {
      ap->size /= ap->elem;
      ap->offs += ap->size * ap->idx;
      ap->elem = 1;
    }
  } else {
    ap->aix   = ULONG_MAX;
    ap->size  = arp->Size == 0 ? cp->size : MIN(arp->Size, cp->size);
    ap->offs  = 0;
    ap->tid   = cdh_TypeObjidToId(ap->bop->g.oid);
    ap->elem  = 1;
  }


  ap->flags.b.Indirect = arp->Flags.b.Indirect;


  pwr_Return(ap, sts, GDH__SUCCESS);
}

pwr_sAttrRef *
mvol_AttributeToAref (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_sAttrRef		*arp
)	
{
  if (ap->op != NULL)
    arp->Objid = ap->op->g.oid;

  if (ap->aop != NULL)
    arp->Body = cdh_TypeObjidToId(ap->aop->g.oid);
  else
    arp->Body = cdh_TypeObjidToId(ap->bop->g.oid);
    
  arp->Offset  = ap->offs;
  arp->Size    = ap->size;

  arp->Flags.b.Indirect = ap->flags.b.Indirect;

  pwr_Return(arp, sts, GDH__SUCCESS);
}

gdb_sObject *
mvol_ClassList (
  pwr_tStatus		*sts,
  pwr_tClassId		cid,
  pwr_tObjid		oid,
  mvol_eList		list
)
{
  gdb_sClass		*cp;
  gdb_sObject		*op = NULL;
  pool_sQlink		*ol;

  if (cdh_ObjidIsNotNull(oid)) {
    op = vol_OidToObject(sts, oid, gdb_mLo_native, vol_mTrans_none, cvol_eHint_none);
    if (op == NULL)
      return NULL;
    cid = op->g.cid;
  } else if (cid == pwr_cNClassId)
    pwr_Return(NULL, sts, GDH__WEIRD);

  cp = (gdb_sClass *) hash_Search(sts, gdbroot->cid_ht, &cid); 
  if (cp == NULL)
    pwr_Return(NULL, sts, GDH__BADOBJTYPE);

  switch (list) {
  case mvol_eList_first:
    ol = pool_Qsucc(NULL, gdbroot->pool, &cp->cid_lh);
    break;
  case mvol_eList_last:
    ol = pool_Qpred(NULL, gdbroot->pool, &cp->cid_lh);
    break;
  case mvol_eList_next:
    ol = pool_Qsucc(NULL, gdbroot->pool, &op->u.n.cid_ll);
    break;
  case mvol_eList_prev:
    ol = pool_Qpred(NULL, gdbroot->pool, &op->u.n.cid_ll);
    break;
  default:
    pwr_Return(NULL, sts, GDH__WEIRD);
  }

  if (ol == NULL || ol == &cp->cid_lh)
    pwr_Return(NULL, sts, GDH__NO_TYPE);

  return pool_Qitem(ol, gdb_sObject, u.n.cid_ll);
}

gdb_sClass *
mvol_LinkClass (
  pwr_tStatus		*sts,
  gdb_sClass		*cp,
  pwr_tBitMask		ilink
)
{
  cdh_uObjid            coid;
  pwr_sObjBodyDef	*bp;  
  pwr_sParInfo		*abp;  
  gdb_sObject		*aop;
  gdb_sObject		*cop;
  gdb_sObject		*bop;
#if 0
  gdb_sObject		*op;
#endif
  pool_sQlink		*ol;
  int			count;
  int			i;

  
  if (cp->cor == pool_cNRef) {
#if 0
    printf("*** Class has no definition, cid: %s\n", cdh_ClassIdToString(NULL, cp->cid, 1));
    for (
      ol = pool_Qsucc(sts, gdbroot->pool, &cp->cid_lh);
      ol != &cp->cid_lh;
      ol = pool_Qsucc(sts, gdbroot->pool, ol)
    ) {
      op = pool_Qitem(ol, gdb_sObject, u.n.cid_ll);
      printf("---   %s\n", op->g.f.name.orig);
    }
#endif
#if 0
    pwr_Return(NULL, sts, MVOL__NOCLASSDEF);
#else
    pwr_Return(NULL, sts, MVOL__SUCCESS);
#endif
  }

  cop = pool_Address(NULL, gdbroot->pool, cp->cor);

  if (cp->bor == pool_cNRef) {
#if 0
    printf("*** Class has no body definition, cid: %s, %s\n",
      cdh_ClassIdToString(NULL, cp->cid, 1), cop->g.f.name.orig);

    for (
      ol = pool_Qsucc(sts, gdbroot->pool, &cp->cid_lh);
      ol != &cp->cid_lh;
      ol = pool_Qsucc(sts, gdbroot->pool, ol)
    ) {
      op = pool_Qitem(ol, gdb_sObject, u.n.cid_ll);
      if (op->g.size != cp->size)
	printf("---   %s: cp: %d, op: %d\n", op->g.f.name.orig, cp->size, op->g.size);
    }
#endif
#if 0
    pwr_Return(NULL, sts, MVOL__NOBODYDEF);
#else
    pwr_Return(NULL, sts, MVOL__SUCCESS);
#endif
  }

  bp = (pwr_sObjBodyDef *) pool_Address(NULL, gdbroot->rtdb, cp->bbr);
  if (bp == NULL) errh_Bugcheck(MVOL__NOBODYDEF, "body missing");
    
  cp->size = bp->Size;
  cp->acount = bp->NumOfParams;

#if 0
  printf("--- Class: %s\n", cop->g.f.name.orig);
  for (
    count = 0, ol = pool_Qsucc(sts, gdbroot->pool, &cp->cid_lh);
    ol != &cp->cid_lh;
    ol = pool_Qsucc(sts, gdbroot->pool, ol)
  ) {
    op = pool_Qitem(ol, gdb_sObject, u.n.cid_ll);
    count++;
  }
  printf("---   %d\n", count);
#endif

  bop = pool_Address(NULL, gdbroot->pool, cp->bor);

  cp = gdb_ReAddClass(sts, cp, cp->acount);

  for (
    count = 0, ol = pool_Qsucc(sts, gdbroot->pool, &bop->u.n.sib_lh);
    ol != &bop->u.n.sib_lh;
    count++, ol = pool_Qsucc(sts, gdbroot->pool, ol)
  ) {
    aop = pool_Qitem(ol, gdb_sObject, u.n.sib_ll);
    abp = pool_Address(NULL, gdbroot->rtdb, aop->u.n.body);
    i = abp->ParamIndex;

    cp->attr[i].aor     = pool_ItemReference(NULL, gdbroot->pool, aop);
    cp->attr[i].abr     = aop->u.n.body;
    cp->attr[i].flags.m = abp->Flags;
    cp->attr[i].type    = abp->Type;
    cp->attr[i].offs    = abp->Offset;
    cp->attr[i].size    = abp->Size;
    cp->attr[i].elem    = abp->Elements;
    cp->attr[i].moffset = abp->Offset + abp->Size - 1;
    coid.pwr = aop->g.oid;
    cp->attr[i].aix     = coid.t.aix;
#if 0
    printf("---   %s, idx: %d, mof: %d\n", aop->g.f.name.orig, i, cp->attr[i].moffset);
#endif
    pwr_Assert(i == count);
    if (count > 0) pwr_Assert(cp->attr[i-1].moffset < cp->attr[i].moffset);
  }

  pwr_Assert(count == cp->acount);
  return cp;
}

gdb_sClass *
mvol_LinkObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		ilink
)
{
  gdb_sClass		*cp;
  gdb_sClass		*scp;
  pwr_tClassId		cid;

  pwr_Assert(vp->l.flags.b.isNative);

#if 0
  if (op->g.cid == pwr_cNClassId) {
    printf("*** Object is of not known class: %s\n", cdh_ClassIdToString(NULL, op->g.cid, 1));
    printf("     %s, poid: %s\n", op->g.f.name.orig, cdh_ObjidToString(NULL, op->g.f.poid, 1));
  }
#endif

  /* Alias clients should be inserted in Alias class list.  */
  if (op->g.flags.b.isAliasClient) {
    cid = pwr_eClass_Alias;
  } else {
    cid = op->g.cid;
  }

  cp = gdb_AddClass(sts, cid, gdb_mAdd__);
  if (cp == NULL) {
    errh_Bugcheck(*sts, "add class");
  }
  
  if (vp->g.cid == pwr_eClass_ClassVolume && strcmp(op->g.f.name.norm, "TEMPLATE") == 0)
    return cp; /* We don't want the 'Template' object.  */

  pwr_Assert(!op->u.n.flags.b.inCidList);
  pool_QinsertPred(sts, gdbroot->pool, &op->u.n.cid_ll, &cp->cid_lh);
  op->u.n.flags.b.inCidList = 1;

  if (vp->g.cid == pwr_eClass_ClassVolume) {
    switch (cid) {
    case pwr_eClass_ClassDef:
      /* Link the class definition object to the class.  */
      scp = gdb_AddClass(sts, cdh_ClassObjidToId(op->g.oid), gdb_mAdd__);
      if (scp == NULL) errh_Bugcheck(*sts, "add classdef object");
      
      if (scp->cor != pool_cNRef) printf("scp->cor != pool_cNRef, %s\n", op->g.f.name.orig);
      scp->cor = pool_ItemReference(sts, gdbroot->pool, op);
      if (scp->cbr != pool_cNRef) printf("scp->cbr != pool_cNRef, %s\n", op->g.f.name.orig);
      scp->cbr = op->u.n.body;
      if (!decodeClassDef(sts, op, vp->u.n.format.b.bo)) return NULL;
      break;

    case pwr_eClass_ObjBodyDef:
      /* Link the body definition object to the class.  */
      if (strcmp(op->g.f.name.norm, "DEVBODY") == 0) break;

      scp = gdb_AddClass(sts, cdh_ClassObjidToId(op->g.oid), gdb_mAdd__);
      if (scp == NULL) errh_Bugcheck(*sts, "add classdef object");
      
      scp->bor = pool_ItemReference(sts, gdbroot->pool, op);
      scp->bbr = op->u.n.body;
      if (!decodeObjBodyDef(sts, op, vp->u.n.format.b.bo)) return NULL;
      break;

    case pwr_eClass_Input:
    case pwr_eClass_Output:
    case pwr_eClass_Intern:
    case pwr_eClass_ObjXRef:
    case pwr_eClass_AttrXRef:
    case pwr_eClass_Buffer:
    case pwr_eClass_Param:
      if(!decodeAttribute(sts, op, vp->u.n.format.b.bo)) return NULL;
      break;
    }
  }

  return cp;
}

gdb_sClass *
mvol_NameToClass (
  pwr_tStatus		*sts,
  char			*name
)
{
  cdh_sParseName	ParseName;
  cdh_sParseName	*pn;
  pwr_tClassId		cid = pwr_eClass_ClassHier;
  gdb_sClass		*cp;
  pool_sQlink		*ol;
  gdb_sObject		*op;

  pn = cdh_ParseName(sts, &ParseName, pwr_cNObjid, name, 0);
  if (pn == NULL) return NULL;
  if (
    (pn->nObject != 1) || pn->flags.b.parent ||
    pn->flags.b.attribute || pn->flags.b.index
  ) {
    pwr_Return(NULL, sts, GDH__BADOBJTYPE);
  }

  cp = hash_Search(sts, gdbroot->cid_ht, &cid);
  if (cp == NULL)
    pwr_Return(NULL, sts, GDH__BADOBJTYPE);

  for (
    ol = pool_Qsucc(NULL, gdbroot->pool, &cp->cid_lh);
    ol != &cp->cid_lh;
    ol = pool_Qsucc(NULL, gdbroot->pool, ol)
  ) {
    op = pool_Qitem(ol, gdb_sObject, u.n.cid_ll);
    pn->object[pn->nObject-1].poid = op->g.oid;
    op = hash_Search(sts, gdbroot->family_ht, &pn->object[pn->nObject-1]);
    if (op != NULL) break;
  }

  if (op != NULL && op->g.cid == pwr_eClass_ClassDef) {
    cid =  cdh_ClassObjidToId(op->g.oid);
    return hash_Search(sts, gdbroot->cid_ht, &cid);
  } else {
    pwr_Return(NULL, sts, GDH__BADOBJTYPE);
  }
}
