/* co_dbs.c -- <short description>

   PROVIEW/R
   Copyright (C) 2002 by Cell Network AB.

   . */


#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include "pwr.h"
#include "co_dbs.h"
#include "co_dbs_msg.h"
#include "co_errno.h"
#include "co_platform.h"


static pwr_tBoolean
checkQ(const dbs_sEnv *ep, dbs_sQlink *item)
{
  dbs_sQlink *link;

  if (item == NULL) {
    printf("checkQ in volume: %s, item == NULL", "not known");
    return NO;
  }
  if (item->self == dbs_cNref) {
    printf("checkQ in volume: %s, item->self == dbs_cNref, item: %u", "not known", (unsigned int)item);
    return NO;
  }

  link = dbs_Address(NULL, ep, item->self);
  if (item != link) {
    printf("checkQ in volume: %s, item != dbs_Address(NULL, ep, item->self),\n item: %u != %u",
      "not known", (unsigned int)item, (unsigned int)link);
    return NO;
  }

  return YES;
}

void *
dbs_Address(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_tRef r)
{
    dbs_uRefBits bits;
    
    if (r == dbs_cNref)
        return NULL;

    if (!ep->flags.b.isMapped)
        pwr_Return(NULL, sts, DBS__NOTMAPPED);

    bits.m = r;
  
    if (bits.b.sect > dbs_eSect_)
        pwr_Return(NULL, sts, DBS__BADSECT);
  
    if (bits.b.offs >= ep->sect[bits.b.sect].size)
        pwr_Return(NULL, sts, DBS__BADOFFS);
      

    return (void *)(ep->base + ep->sect[bits.b.sect].offset + bits.b.offs);
}

void *dbs_Bfind(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sBintab *tp, void *key, int (comp)(void *key, void *record))
{
    char *p;
    int   c;
    char *start = dbs_Address(sts, ep, tp->start);
    char *end   = dbs_Address(sts, ep, tp->end);
    int   rsize = tp->rsize;

    
    
    while (start <= end) {
        p = start + ((end - start) / (2*rsize)) * rsize;
        c = comp(key, p);
        if (c > 0) {
            start = p + rsize;
        } else if (c < 0) {
            end = p - rsize;
        } else {
            return p;
        }
    }
    
    return 0;
}

#if 0
dbs_sEnv *
dbs_Create(pwr_tStatus*, dbs_sEnv*, char*, pwr_tUInt32, pwr_tUInt32)
{
}

void
dbs_Dump(pwr_tStatus*, dbs_sEnv*)
{
}

pwr_tBoolean
dbs_Free(pwr_tStatus*, dbs_sEnv*, void*)
{
}

pwr_tBoolean
dbs_FreeReference(pwr_tStatus*, dbs_sEnv*, dbs_tRef)
{
}

dbs_tRef
dbs_InDbs(pwr_tStatus*, dbs_sEnv*, void*, pwr_tUInt32)
{
}

dbs_tRef
dbs_ItemReference(pwr_tStatus*, dbs_sEnv*, void*)
{
}



dbs_sQlink *
dbs_Qalloc(pwr_tStatus*, dbs_sEnv*)
{
}

#endif
pwr_tBoolean
dbs_QhasOne(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(ep, item));

    pred = dbs_Address(sts, ep, item->pred);
    succ = dbs_Address(sts, ep, item->succ);
  
    pwr_Assert(checkQ(ep, succ));
    pwr_Assert(checkQ(ep, pred));
    pwr_Assert(item->pred == pred->self);
    pwr_Assert(item->succ == succ->self);
    pwr_Assert(pred->succ == item->self);
    pwr_Assert(succ->pred == item->self);

    return item->succ == item->pred;
}

/* Initialize a queue item.  */

dbs_tRef
dbs_Qinit(pwr_tStatus *sts, dbs_sQlink *item, dbs_tRef r)
{

    if (r != dbs_cNref)
        item->self = item->succ = item->pred = r;
    return r;
}

dbs_sQlink *
dbs_Qinsert(pwr_tStatus *sts, dbs_sQlink *pred, dbs_sQlink *item, dbs_sQlink *succ)
{

    pwr_Assert(pred->succ == succ->self);
    pwr_Assert(succ->pred == pred->self);

    pred->succ = succ->pred = item->self;
    item->pred = pred->self;
    item->succ = succ->self;

    return item;
}


#if 0
dbs_sQlink     *dbs_QinsertPred(pwr_tStatus*, dbs_sEnv*, dbs_sQlink*, dbs_sQlink*);
dbs_sQlink     *dbs_QinsertSucc(pwr_tStatus*, dbs_sEnv*, dbs_sQlink*, dbs_sQlink*);
#endif
#if 1
pwr_tBoolean
dbs_QisEmpty(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(ep, item));

    pred = dbs_Address(sts, ep, item->pred);
    succ = dbs_Address(sts, ep, item->succ);
  
    pwr_Assert(checkQ(ep, succ));
    pwr_Assert(checkQ(ep, pred));
    pwr_Assert(item->pred == pred->self);
    pwr_Assert(item->succ == succ->self);
    pwr_Assert(pred->succ == item->self);
    pwr_Assert(succ->pred == item->self);

    return item->succ == item->self;
}

pwr_tBoolean
dbs_QisLinked(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(ep, item));

    pred = dbs_Address(sts, ep, item->pred);
    succ = dbs_Address(sts, ep, item->succ);
  
    pwr_Assert(checkQ(ep, succ));
    pwr_Assert(checkQ(ep, pred));
    pwr_Assert(item->pred == pred->self);
    pwr_Assert(item->succ == succ->self);
    pwr_Assert(pred->succ == item->self);
    pwr_Assert(succ->pred == item->self);

    return item->succ != item->self;
}

pwr_tBoolean
dbs_QisNull(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sQlink *item)
{
    pwr_tBoolean nullQ;

    nullQ = (item->self == dbs_cNref)
        || (item->pred == dbs_cNref)
        || (item->succ == dbs_cNref);
  
    pwr_Assert(item->pred == dbs_cNref);
    pwr_Assert(item->succ == dbs_cNref);
    pwr_Assert(item->self  == dbs_cNref);

    return nullQ;
}

dbs_sQlink *
dbs_Qpred(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sQlink *item)
{
    dbs_sQlink *pred;

    pwr_Assert(checkQ(ep, item));

    pred = dbs_Address(NULL, ep, item->pred);
  
    if (pred != NULL) {
        pwr_Assert(checkQ(ep, pred));
        pwr_Assert(pred->succ == item->self);
        pwr_Assert(pred->self == item->pred);
    }

    return pred;  
}

dbs_sQlink *
dbs_Qsucc(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sQlink *item)
{
    dbs_sQlink *succ;

    pwr_Assert(checkQ(ep, item));

    succ = dbs_Address(NULL, ep, item->succ);
  
    if (succ != NULL) {
        pwr_Assert(checkQ(ep, succ));
        pwr_Assert(succ->pred == item->self);
        pwr_Assert(item->succ == succ->self);
    }

    return succ;  
}

/* Translate a virtual address to a  reference.
   Return a dbs_tRef, and signals errors.  */

dbs_tRef
dbs_Reference(pwr_tStatus *sts, const dbs_sEnv *ep, void *adrs)
{
    long iadrs = (long)adrs;
    long ofs;
    //long base = ep->sect[]

    if (iadrs < (long)ep->base)
        return dbs_cNref;
    
    ofs = iadrs - (long)ep->base;
    if (ofs >= ep->size)
        return dbs_cNref;

    return (dbs_tRef)ofs;
}

#endif

pwr_tBoolean
dbs_Close(pwr_tStatus *sts, dbs_sEnv *ep) 
{
     *sts = DBS__SUCCESS;
     fclose(ep->f);
     ep->f = NULL;
 
     return TRUE;
}

dbs_sEnv *
dbs_Open(pwr_tStatus *sts, dbs_sEnv *ep, const char *filename)
{
    FILE *f;
    co_mFormat srcFormat, ownFormat;
    PDR pdrs;

    *sts = DBS__SUCCESS;
    memset(ep, 0, sizeof(*ep));
    
    f = fopen(filename, "r");
    if (f == NULL) {
        *sts = errno_GetStatus();
        return NULL;
    } 

    if (fread(&ep->file, sizeof(ep->file), 1, f) == 0) {
        *sts = errno_GetStatus();
        fclose(f);
        return NULL;
    }
    
#if 0
    srcFormat.m = ntohl(ep->file.format.m);    
#else
    srcFormat.m = ep->file.format.m;    
#endif
    co_GetOwnFormat(&ownFormat);

    if (srcFormat.m != ownFormat.m) {
        pdrmem_create(&pdrs, &ep->file, sizeof(ep->file), PDR_DECODE, srcFormat, ownFormat);
        if (!pdr_dbs_sFile(&pdrs, &ep->file)) {
            *sts = DBS__PDRFILE;
            fclose(f);
            return NULL;
        }
    }
    
    fseek(f, 0, SEEK_SET);
    ep->f = f;

    return ep;
}

dbs_sEnv *
dbs_Map(pwr_tStatus *sts, dbs_sEnv *ep, const char *filename)
{
    struct stat sb;
    int ret, i;
    int fd;
    //char *buf;
    //int c;
    dbs_sFile *fp;
    dbs_sSect *sp;
    dbs_sVolume *vp;
    dbs_sObject *op, *eop;
    dbs_sName *np, *enp;
    dbs_sOid *oidp, *eoidp;
    dbs_uRefBits nf, nl, cf, cl, of, ol;
    dbs_sClass *cp, *ecp;
    
    
    if ((ret = stat(filename, &sb)) != 0) {
        perror("stat");
        return NULL;
    }

    *sts = DBS__SUCCESS;
    memset(ep, 0, sizeof(*ep));
    *sts = errno_GetStatus();

    printf("st_dev....: %d\t\t%s\n",   (int)sb.st_dev,     "device");
    printf("st_ino....: %d\t%s\n",   sb.st_ino,     "inode");
    printf("st_mode...: %d\t%s\n",   sb.st_mode,    "protection");
    printf("st_nlink..: %d\t\t%s\n", sb.st_nlink,   "number of hard links");
    printf("st_uid....: %d\t%s\n",   sb.st_uid,     "user ID of owner");
    printf("st_gid....: %d\t\t%s\n", sb.st_gid,     "group ID of owner");
    printf("st_redv...: %d\t\t%s\n",   (int)sb.st_rdev,    "device type (if inode device)");
    printf("st_size...: %d\t\t%s\n", sb.st_size,    "total size, in bytes");
    printf("st_blksize: %d\t%s\n",   sb.st_blksize, "blocksize for filesystem I/O");
    printf("st_blocks.: %d\t\t%s\n", sb.st_blocks,  "number of blocks allocated");
    printf("st_atime..: %d\t%s\n",   sb.st_atime,   "time of last access");
    printf("st_mtime..: %d\t%s\n",   sb.st_mtime,   "time of last modification");
    printf("st_ctime..: %d\t%s\n",   sb.st_ctime,   "time of last change");
    printf("st_mode...: %d\t%s\n",   sb.st_mode,    "mode");
    

    fd = open(filename, O_RDWR);
    printf("open fd: %d\n", fd);
    
    errno = 0;
    ep->base = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    perror("mmap");
    printf("mmap buf: %d\n", ret);
    
    ret = close(fd);
    printf("close ret: %d\n", ret);

    ep->flags.b.isMapped = 1;
    
    fp = (dbs_sFile*)ep->base;

    printf("format.......: %d\n", fp->format.m);
    printf("cookie.......: %d\n", fp->cookie);
    printf("size.........: %d\n", fp->size);
    printf("offset.......: %d\n", fp->offset);
    printf("formatVersion: %d\n", fp->formatVersion);
    printf("version......: %d\n", fp->version);
    printf("sectVersion..: %d\n", fp->sectVersion);
    printf("pwrVersion...: %d\n", fp->pwrVersion);
    printf("time.........: %ld\n", fp->time.tv_sec);
    printf("fileType.....: %d\n", fp->fileType);
    printf("userName.....: %s\n", fp->userName);

    sp = (dbs_sSect*)((char *)fp + dbs_dAlign(sizeof(dbs_sFile)));
    vp = (dbs_sVolume*)((char *)fp + sp[dbs_eSect_volume].offset);

    printf("vid........: %d\n", vp->vid);
    printf("name.......: %s\n", vp->name);
    printf("cid........: %d\n", vp->cid);
    printf("className..: %s\n", vp->className);
    printf("time.......: %ld\n", vp->time.tv_sec);
    printf("cardinality: %d\n", vp->cardinality);
    printf("rbodysize..: %d\n", vp->rbodySize);

    nf.m = vp->name_bt.start;
    nl.m = vp->name_bt.end;
    cf.m = vp->class_bt.start;
    cl.m = vp->class_bt.end;
    of.m = vp->oid_bt.start;
    ol.m = vp->oid_bt.end;

    printf("name_bt....: %d.%d -> %d.%d %d\n", nf.b.sect, nf.b.offs, nl.b.sect, nl.b.offs, vp->name_bt.rsize);
    printf("class_bt...: %d.%d -> %d.%d %d\n", cf.b.sect, cf.b.offs, cl.b.sect, cl.b.offs, vp->class_bt.rsize);
    printf("oid_bt.....: %d.%d -> %d.%d %d\n", of.b.sect, of.b.offs, ol.b.sect, ol.b.offs, vp->oid_bt.rsize);
    

    ep->sect = (dbs_sSect*)((char *)fp + dbs_dAlign(sizeof(dbs_sFile)));
    sp = (dbs_sSect*)((char *)fp + dbs_dAlign(sizeof(dbs_sFile)));

    for (i = 0; i < dbs_eSect_; i++, sp++) {
        printf("sect: i:%d, version:%d, type:%d, size:%d, offset:%d\n", i, sp->version,
               sp->type, sp->size, sp->offset);
    }
    
    sp = (dbs_sSect*)((char *)fp + dbs_dAlign(sizeof(dbs_sFile)));
    op = (dbs_sObject*)((char *)fp + sp[dbs_eSect_object].offset);
    eop = (dbs_sObject*)((char *)op + sp[dbs_eSect_object].size);
    for (; op < eop; op = (dbs_sObject*)((char *)op + dbs_dAlign(sizeof(dbs_sObject)))) {
        dbs_uRefBits r, p, s, bf, bl;
        r.m = op->rbody.ref;
        p.m = op->pref;
        s.m = op->sib_ll.self;
        bf.m = op->name_bt.start;
        bl.m = op->name_bt.end;
        
        
        printf("O %s, %d.%d, %d r[%d.%d] p[%d.%d] s[%d.%d] bs[%d.%d] be[%d.%d] %d\n",
               op->name, op->oid.vid, op->oid.oix, op->rbody.size, r.b.sect, r.b.offs,
               p.b.sect, p.b.offs, s.b.sect, s.b.offs, bf.b.sect, bf.b.offs, bl.b.sect, bl.b.offs, op->name_bt.rsize);
    }
    
    oidp = (dbs_sOid*)((char *)fp + sp[dbs_eSect_oid].offset);
    eoidp = (dbs_sOid*)((char *)oidp + sp[dbs_eSect_oid].size);
    for (; oidp < eoidp; oidp = (dbs_sOid*)((char *)oidp + dbs_dAlign(sizeof(dbs_sOid)))) {
        dbs_uRefBits s;
        dbs_sObject *op, *oop, *aop;
        pwr_tStatus sts;
        s.m = oidp->ref;
        
        op = dbs_Address(&sts, ep, oidp->ref);
        oop = dbs_OidToObject(&sts, ep, op->oid);
        aop = dbs_After(&sts, ep, op);

        printf("Oid %d.%d, [%d.%d] -> %s %d.%d ", oidp->oid.vid, oidp->oid.oix, s.b.sect, s.b.offs,
               op->name, op->oid.vid, op->oid.oix);
        if (oop == NULL)
            printf(" NULL");
        else
            printf("(%d.%d)", oop->oid.vid, oop->oid.oix);

        if (aop == NULL)
            printf(" NULL\n");
        else
            printf(" (%s)\n", aop->name);
    }
    
    np = (dbs_sName*)((char *)fp + sp[dbs_eSect_name].offset);
    enp = (dbs_sName*)((char *)np + sp[dbs_eSect_name].size);
    for (; np < enp; np = (dbs_sName*)((char *)np + dbs_dAlign(sizeof(dbs_sName)))) {
        dbs_uRefBits r;
        dbs_sObject *op, *oop, *nop;
        pwr_tStatus sts;
        
        
        r.m = np->ref;
        op = dbs_Address(&sts, ep, np->ref);
        oop = dbs_OidToObject(&sts, ep, op->oid);
        nop = dbs_NameToObject(&sts, ep, op->poid, op->normname);

        
        printf("N %d %s, [%d.%d] -> %s %d.%d ", np->poix, np->normname, r.b.sect, r.b.offs,
            op->name, op->oid.vid, op->oid.oix);
        if (oop == NULL)
            printf("NULL");
        else
            printf("(%d.%d)", oop->oid.vid, oop->oid.oix);
        if (oop == NULL)
            printf(" NULL\n");
        else
            printf(" (%d.%d)\n", nop->oid.vid, nop->oid.oix);
    }

    cp = (dbs_sClass*)((char *)fp + sp[dbs_eSect_class].offset);
    ecp = (dbs_sClass*)((char *)np + sp[dbs_eSect_class].size);
    for (; cp < ecp; cp = (dbs_sClass*)((char *)cp + dbs_dAlign(sizeof(dbs_sClass)))) {
        dbs_sObject *op;
        pwr_tStatus sts;        
        
        printf("C %d\n", cp->cid);
        op = dbs_ClassToObject(&sts, ep, cp->cid);
        while (op != NULL) {
            printf("   %d %d.%d %s\n", op->cid, op->oid.vid, op->oid.oix, op->name);

            op = dbs_Next(&sts, ep, op);
        }
    }

    return ep;
}

static int
comp_oid(void *key, void *record)
{
    pwr_tOid *k = (pwr_tOid *) key;
    dbs_sOid *o = (dbs_sOid *) record;

    if (k->vid == o->oid.vid) {
        if (k->oix == o->oid.oix)
            return 0;
        else if (k->oix < o->oid.oix)
            return -1;
        else
            return 1;
    } else if (k->vid < o->oid.vid)
        return -1;
    else
        return 1;

}

static int
comp_cid(void *key, void *record)
{
    pwr_tCid *k = (pwr_tCid *) key;
    dbs_sClass *c = (dbs_sClass *) record;

    if (*k == c->cid)
        return 0;
    else if (*k < c->cid)
        return -1;
    else
        return 1;
}

static int
comp_name(void *key, void *record)
{
    dbs_sName *k = (dbs_sName *) key;
    dbs_sName *n = (dbs_sName *) record;

    if (k->poix < n->poix)
        return -1;
    else if (k->poix == n->poix)
        return strcmp(k->normname, n->normname);
    else
        return 1;
}


dbs_sObject *
dbs_OidToObject(pwr_tStatus *sts, const dbs_sEnv *ep, pwr_tOid oid)
{
    dbs_sOid *oidp;
    dbs_sObject *op;
    dbs_sVolume *vp = (dbs_sVolume *)(ep->base + ep->sect[dbs_eSect_volume].offset);

    oidp = (dbs_sOid *)dbs_Bfind(sts, ep, &vp->oid_bt, &oid, comp_oid);
    if (oidp == NULL)
        return NULL;
    
    op = dbs_Address(sts, ep, oidp->ref);
    
    return op;
}

dbs_sObject *
dbs_After(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op)
{
    dbs_sOid *oidp;
    dbs_sVolume *vp = (dbs_sVolume *)(ep->base + ep->sect[dbs_eSect_volume].offset);
    dbs_sQlink *ol;

    dbs_sObject *pop;
    
    oidp = (dbs_sOid *)dbs_Bfind(sts, ep, &vp->oid_bt, &op->poid, comp_oid);
    if (oidp == NULL)
        return NULL;
    
    pop = dbs_Address(sts, ep, oidp->ref);
    
    ol = dbs_Qsucc(sts, ep, &op->sib_ll);
    
    if (ol == &pop->sib_lh)
        return NULL;

    return dbs_Qitem(ol, dbs_sObject, sib_ll);
}


dbs_sObject *
dbs_Before(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op)
{
    dbs_sOid *oidp;
    dbs_sVolume *vp = (dbs_sVolume *)(ep->base + ep->sect[dbs_eSect_volume].offset);
    dbs_sQlink *ol;

    dbs_sObject *pop;
    
    oidp = (dbs_sOid *)dbs_Bfind(sts, ep, &vp->oid_bt, &op->poid, comp_oid);
    if (oidp == NULL)
        return NULL;
    
    pop = dbs_Address(sts, ep, oidp->ref);
    
    ol = dbs_Qpred(sts, ep, &op->sib_ll);
    
    if (ol == &pop->sib_lh)
        return NULL;

    return dbs_Qitem(ol, dbs_sObject, sib_ll);
}


dbs_sObject *
dbs_Parent(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op)
{
    return dbs_Address(sts, ep, op->pref);
}

dbs_sObject *
dbs_First(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op)
{
    dbs_sQlink *ol;
    
    if (dbs_QisEmpty(sts, ep, &op->sib_lh))
        return NULL;

    ol = dbs_Qsucc(sts, ep, &op->sib_lh);

    return dbs_Qitem(ol, dbs_sObject, sib_ll);
}

dbs_sObject *
dbs_Last(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op)
{
    dbs_sQlink *ol;
    
    if (dbs_QisEmpty(sts, ep, &op->sib_lh))
        return NULL;

    ol = dbs_Qpred(sts, ep, &op->sib_lh);

    return dbs_Qitem(ol, dbs_sObject, sib_ll);
}


dbs_sObject *
dbs_NameToObject(pwr_tStatus *sts, const dbs_sEnv *ep, pwr_tOid oid, char *name)
{
    dbs_sOid *oidp;
    dbs_sObject *op;
    dbs_sVolume *vp = (dbs_sVolume *)(ep->base + ep->sect[dbs_eSect_volume].offset);
    dbs_sName n;
    dbs_sName *np;
    
    oidp = (dbs_sOid *)dbs_Bfind(sts, ep, &vp->oid_bt, &oid, comp_oid);
    if (oidp == NULL)
        return NULL;
    
    op = dbs_Address(sts, ep, oidp->ref);
    if (op == NULL)
        return NULL;
    
    n.poix = oid.oix;
    strcpy(n.normname, name);
    
    np = (dbs_sName *)dbs_Bfind(sts, ep, &vp->name_bt, &n, comp_name);
    if (np == NULL)
        return NULL;
    
    op = dbs_Address(sts, ep, np->ref);
    
    return op;
}

dbs_sObject *
dbs_Child(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op, char *name)
{
    dbs_sName n;
    dbs_sName *np;
    
    n.poix = op->oid.oix;
    strcpy(n.normname, name);
    
    np = (dbs_sName *)dbs_Bfind(sts, ep, &op->name_bt, &n, comp_name);
    if (np == NULL)
        return NULL;
    
    op = dbs_Address(sts, ep, np->ref);
    
    return op;
}

dbs_sObject *
dbs_ClassToObject(pwr_tStatus *sts, const dbs_sEnv *ep, pwr_tCid cid)
{
    dbs_sClass *cp;
    dbs_sVolume *vp = (dbs_sVolume *)(ep->base + ep->sect[dbs_eSect_volume].offset);
    dbs_sQlink *ol;
    
    cp = (dbs_sClass *)dbs_Bfind(sts, ep, &vp->class_bt, &cid, comp_cid);
    if (cp == NULL)
        return NULL;
    
    if (dbs_QisEmpty(sts, ep, &cp->o_lh))
        return NULL;
    
    ol = dbs_Qsucc(sts, ep, &cp->o_lh);

    return dbs_Qitem(ol, dbs_sObject, o_ll);
}

dbs_sObject *
dbs_Next(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op)
{
    dbs_sClass *cp;
    dbs_sVolume *vp = (dbs_sVolume *)(ep->base + ep->sect[dbs_eSect_volume].offset);
    dbs_sQlink *ol;
    
    cp = (dbs_sClass *)dbs_Bfind(sts, ep, &vp->class_bt, &op->cid, comp_cid);
    if (cp == NULL)
        return NULL;
    
    ol = dbs_Qsucc(sts, ep, &op->o_ll);
    
    if (ol == &cp->o_lh)
        return NULL;

    return dbs_Qitem(ol, dbs_sObject, o_ll);
}

dbs_sObject *
dbs_Previous(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op)
{
    dbs_sClass *cp;
    dbs_sVolume *vp = (dbs_sVolume *)(ep->base + ep->sect[dbs_eSect_volume].offset);
    dbs_sQlink *ol;
    
    cp = (dbs_sClass *)dbs_Bfind(sts, ep, &vp->class_bt, &op->cid, comp_cid);
    if (cp == NULL)
        return NULL;
    
    ol = dbs_Qpred(sts, ep, &op->o_ll);
    
    if (ol == &cp->o_lh)
        return NULL;

    return dbs_Qitem(ol, dbs_sObject, o_ll);
}

dbs_sObject *
dbs_Ancestor(pwr_tStatus *sts, const dbs_sEnv *ep, dbs_sObject *op)
{
    while (op != NULL) {
        if (op->oid.oix == 1)
            return NULL;
        if (op->poid.oix == 1)
            return op;
        op = dbs_Address(sts, ep, op->pref);
    }

    return op;
}

pwr_tBoolean 
dbs_AlignedRead(pwr_tStatus *sts, void *buf, pwr_tUInt32 size, const dbs_sEnv *ep)
{
    int offset;
    *sts = DBS__SUCCESS;   
    

    if (fread(buf, size, 1, ep->f) == 0)
      pwr_Return(NO, sts, errno_GetStatus());

    if ((offset = dbs_dPadding(size)) > 0)
        if(fseek(ep->f, offset, SEEK_CUR))
            pwr_Return(NO, sts, errno_GetStatus());

    return YES;
}

void
dbs_GetVolumeName(pwr_tStatus *sts, dbs_sEnv *ep, char *name)
{
    dbs_sVolume *vp = (dbs_sVolume *)(ep->base + ep->sect[dbs_eSect_volume].offset);
    strcpy(name, vp->name);
}

