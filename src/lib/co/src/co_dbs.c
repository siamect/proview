/* co_dbs.c -- <short description>

   PROVIEW/R
   Copyright (C) 2002 by Cell Network AB.

   . */


#if defined OS_ELN
#include string
#include unistd
#include types
#include errno
#else
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#endif

#include "pwr.h"
#include "co_dbs.h"
#include "co_dbs_msg.h"
#include "co_errno.h"
#include "co_platform.h"


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

dbs_sVolRef *
dbs_VolRef(pwr_tStatus *sts, pwr_tUInt32 index, dbs_sVolRef *vp, const dbs_sEnv *ep)
{
  dbs_sSect sect;
  
  int nVolref = 0;
  
  fseek(ep->f, ep->file.offset + (dbs_eSect_volref * dbs_dAlign(sizeof(sect))), SEEK_SET);

  if (fread(&sect, sizeof(sect), 1, ep->f) == 0) {
    *sts = errno_GetStatus();
    return NULL;
  }

  nVolref = sect.size / dbs_dAlign(sizeof(*vp));
  if (index >= nVolref)
    return NULL;
    
    
  fseek(ep->f, sect.offset + (index * dbs_dAlign(sizeof(*vp))), SEEK_SET);
    
  if (fread(vp, sizeof(*vp), 1, ep->f) == 0) {
    *sts = errno_GetStatus();
    return NULL;
  }
  
  return vp;
}

dbs_sVolume *
dbs_Volume(pwr_tStatus *sts, dbs_sVolume *vp, const dbs_sEnv *ep)
{
  dbs_sSect sect;
  
  fseek(ep->f, ep->file.offset + (dbs_eSect_volume * dbs_dAlign(sizeof(sect))), SEEK_SET);

  if (fread(&sect, sizeof(sect), 1, ep->f) == 0) {
    *sts = errno_GetStatus();
    return NULL;
  }

  fseek(ep->f, sect.offset, SEEK_SET);
    
  if (fread(vp, sizeof(*vp), 1, ep->f) == 0) {
    *sts = errno_GetStatus();
    return NULL;
  }
  
  return vp;
}

pwr_tBoolean
dbs_Close(pwr_tStatus *sts, dbs_sEnv *ep) 
{
  *sts = DBS__SUCCESS;
  if (ep->f != NULL) {
    fclose(ep->f);
    ep->f = NULL;
  } else {
    printf("ERROR, dbs_Close, trying to close a non opened file\n");
  }
  
  return TRUE;
}

dbs_sEnv *
dbs_Open(pwr_tStatus *sts, dbs_sEnv *ep, const char *filename)
{
    FILE *f;
    co_mFormat srcFormat, ownFormat;
    PDR pdrs;
    dbs_sSect sect;

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
    
    fseek(f, ep->file.offset, SEEK_SET);

    if (fread(&sect, sizeof(sect), 1, f) == 0) {
        *sts = errno_GetStatus();
        fclose(f);
        return NULL;
    }

    ep->nSect = sect.size / dbs_dAlign(sizeof(sect));
    

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

#if defined(OS_LINUX) || defined(OS_LYNX)

static pwr_tBoolean
checkQ(const dbs_sVenv *vep, dbs_sQlink *item)
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

  link = dbs_Address(NULL, vep, item->self);
  if (item != link) {
    printf("checkQ in volume: %s, item != dbs_Address(NULL, ep, item->self),\n item: %u != %u",
      "not known", (unsigned int)item, (unsigned int)link);
    return NO;
  }

  return YES;
}

void *
dbs_Address(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_tRef r)
{
    dbs_uRefBits bits;
    
    if (r == dbs_cNref)
        return NULL;

    if (!vep->mp->flags.b.isMapped)
        pwr_Return(NULL, sts, DBS__NOTMAPPED);

    if (vep->index != 0) {
      /*printf("index: %d, name %s\n", vep->index, vep->vp->name)*/;
    }
    
    bits.m = r;
  
    if (bits.b.sect > dbs_eSect_)
        pwr_Return(NULL, sts, DBS__BADSECT);
  
    if (bits.b.offs >= vep->sect[bits.b.sect].size)
        pwr_Return(NULL, sts, DBS__BADOFFS);
      

    return (void *)(vep->base + vep->sect[bits.b.sect].offset + bits.b.offs);
}

void *dbs_Bfind(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sBintab *tp, void *key, int (comp)(void *key, void *record))
{
    char *p;
    int   c;
    char *start = dbs_Address(sts, vep, tp->start);
    char *end   = dbs_Address(sts, vep, tp->end);
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

pwr_tBoolean
dbs_QhasOne(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(vep, item));

    pred = dbs_Address(sts, vep, item->pred);
    succ = dbs_Address(sts, vep, item->succ);
  
    pwr_Assert(checkQ(vep, succ));
    pwr_Assert(checkQ(vep, pred));
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


pwr_tBoolean
dbs_QisEmpty(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(vep, item));

    pred = dbs_Address(sts, vep, item->pred);
    succ = dbs_Address(sts, vep, item->succ);
  
    pwr_Assert(checkQ(vep, succ));
    pwr_Assert(checkQ(vep, pred));
    pwr_Assert(item->pred == pred->self);
    pwr_Assert(item->succ == succ->self);
    pwr_Assert(pred->succ == item->self);
    pwr_Assert(succ->pred == item->self);

    return item->succ == item->self;
}

pwr_tBoolean
dbs_QisLinked(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(vep, item));

    pred = dbs_Address(sts, vep, item->pred);
    succ = dbs_Address(sts, vep, item->succ);
  
    pwr_Assert(checkQ(vep, succ));
    pwr_Assert(checkQ(vep, pred));
    pwr_Assert(item->pred == pred->self);
    pwr_Assert(item->succ == succ->self);
    pwr_Assert(pred->succ == item->self);
    pwr_Assert(succ->pred == item->self);

    return item->succ != item->self;
}

pwr_tBoolean
dbs_QisNull(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sQlink *item)
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
dbs_Qpred(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sQlink *item)
{
    dbs_sQlink *pred;

    pwr_Assert(checkQ(vep, item));

    pred = dbs_Address(NULL, vep, item->pred);
  
    if (pred != NULL) {
        pwr_Assert(checkQ(vep, pred));
        pwr_Assert(pred->succ == item->self);
        pwr_Assert(pred->self == item->pred);
    }

    return pred;  
}

dbs_sQlink *
dbs_Qsucc(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sQlink *item)
{
    dbs_sQlink *succ;

    pwr_Assert(checkQ(vep, item));

    succ = dbs_Address(NULL, vep, item->succ);
  
    if (succ != NULL) {
        pwr_Assert(checkQ(vep, succ));
        pwr_Assert(succ->pred == item->self);
        pwr_Assert(item->succ == succ->self);
    }

    return succ;  
}

/* Translate a virtual address to a  reference.
   Return a dbs_tRef, and signals errors.  */

dbs_tRef
dbs_Reference(pwr_tStatus *sts, const dbs_sVenv *vep, void *adrs)
{
    long iadrs = (long)adrs;
    long ofs;
    //long base = ep->sect[]

    if (iadrs < (long)vep->base)
        return dbs_cNref;
    
    ofs = iadrs - (long)vep->base;
    if (ofs >= vep->size)
        return dbs_cNref;

    return (dbs_tRef)ofs;
}

dbs_sMenv *
dbs_Map(pwr_tStatus *sts, const char *filename)
{
    struct stat sb;
    int ret;
    int fd;
    int i;
    dbs_sSect *sect;
    dbs_sFile file;
    void *base = 0;
    int nVolRef;
    dbs_sVolRef *vrp;
    dbs_sMenv *mep = 0;
    dbs_sVenv *vep = 0;

    *sts = DBS__SUCCESS;

    if ((ret = stat(filename, &sb)) != 0) {
        *sts = errno_GetStatus();
        perror("stat");
        return NULL;
    }

    fd = open(filename, O_RDWR);
    
    errno = 0;

    ret = read(fd, &file, sizeof(file));
    // printf("st_size...: %ld\n", sb.st_size);
    // printf("size......: %d\n", file.size);
    
    base = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (base == NULL) {
        *sts = errno_GetStatus();
        perror("mmap");
        ret = close(fd);
        return NULL;
    }
    ret = close(fd);
    
    sect = (dbs_sSect*)(base + dbs_dAlign(sizeof(dbs_sFile)));
    vrp = (dbs_sVolRef*)(base + sect[dbs_eSect_volref].offset);
    nVolRef = sect[dbs_eSect_volref].size / dbs_dAlign(sizeof(dbs_sVolRef));

    mep = (dbs_sMenv *)calloc(1, sizeof(dbs_sMenv) + (nVolRef * sizeof(dbs_sVenv)));
    
    mep->size = sb.st_size;
    mep->flags.b.isMapped = 1;
    mep->vrp     = vrp;
    mep->nVolRef = nVolRef;
    mep->base = base;
    vep = mep->venv;

    vep->mp       = mep;
    vep->index    = 0;
    vep->base     = base;
    vep->sect     = (dbs_sSect*)(vep->base + dbs_dAlign(sizeof(dbs_sFile)));
    vep->vp       = (dbs_sVolume*)(vep->base + vep->sect[dbs_eSect_volume].offset);
    vep->name_bt  = &vep->vp->name_bt;
    vep->oid_bt   = &vep->vp->oid_bt;
    vep->class_bt = &vep->vp->class_bt;
      
    for (i = 0; i < nVolRef; i++) {
      vep = &mep->venv[i + 1];
      
      vep->mp       = mep;
      vep->index    = i + 1;
      vep->base     = base + vrp[i].offset;
      vep->sect     = (dbs_sSect*)(vep->base + dbs_dAlign(sizeof(dbs_sFile)));
      vep->vp       = (dbs_sVolume*)(vep->base + vep->sect[dbs_eSect_volume].offset);
      vep->name_bt  = &vep->vp->name_bt;
      vep->oid_bt   = &vep->vp->oid_bt;
      vep->class_bt = &vep->vp->class_bt;
      vep->size     = ((dbs_sFile*)vep->base)->size;
    }
    
    return mep;
}

dbs_sVenv*
dbs_Vmap(pwr_tStatus *sts, int index, dbs_sMenv *mep)
{
  *sts = DBS__SUCCESS;

  if (index > mep->nVolRef)
    return 0;
  
  return &mep->venv[index];
}

int
dbs_nVolRef(pwr_tStatus *sts, const dbs_sMenv *mep)
{
  return mep->nVolRef;
}


void
dbs_Split(pwr_tStatus *sts, dbs_sMenv *mep, char *dirName)
{
  int i;
  
  for (i = 0; i < mep->nVolRef; i++) {
    dbs_sVenv *vep;
    dbs_sVolRef *vrp;
    int n;
    int j;
    int offset;
    size_t bytes;
    size_t size;
    FILE *fp;
    char fileName[512];
    char *p;
    
    vep = &mep->venv[i + 1];
      
    vrp = (dbs_sVolRef*)(vep->base + vep->sect[dbs_eSect_volref].offset);
    n = vep->sect[dbs_eSect_volref].size / sizeof(*vrp);
    printf("Write meta file %s, %d volrefs\n", vep->vp->name, n);
    printf("  size %d, index %d, nSect %d\n", vep->size, vep->index, vep->nSect);
    size = offset = vep->size;

    sprintf(fileName, "%s/%s.dbs", dirName, vep->vp->name);
    cdh_ToLower(fileName, fileName);
    
    fp = fopen(fileName, "w+b");
    if (fp == NULL) {
      printf("** Cannot open file: %s\n", fileName);
      perror("   Reason");
      return;
    } else {
      printf("!! Opened file: %s\n", fileName);
    }


    p = vep->base;
  
    while (size > 0) {
      if (size > 512) {
        bytes = 512;
      } else {
        bytes = size;
      }
      size -= bytes;  
    
      if (fwrite(p, bytes, 1, fp) < 1)
        return;
      p += bytes;
    }

    fclose(fp);
    
    for (j = 0; j < n; j++, vrp++) {
      //vrp->offset = offset;
      printf("  %s, size %d, offset %d\n", vrp->name, vrp->size, offset);
      offset += vrp->size;
    }
  }
}

pwr_tBoolean
dbs_Unmap(pwr_tStatus *sts, dbs_sMenv *mep) 
{
  *sts = DBS__SUCCESS;
  if (mep->flags.b.isMapped) {
    return TRUE;
  } else if (mep->f != NULL) {
    printf("ERROR, dbs_Unmap, trying to unmap a non mapped file\n");
  }
  
  return TRUE;
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
dbs_OidToObject(pwr_tStatus *sts, const dbs_sVenv *vep, pwr_tOid oid)
{
  dbs_sOid *oidp;
  dbs_sObject *op;

  oidp = (dbs_sOid *)dbs_Bfind(sts, vep, vep->oid_bt, &oid, comp_oid);
  if (oidp == NULL)
    return NULL;
    
  op = dbs_Address(sts, vep, oidp->ref);
    
  return op;
}

dbs_sObject *
dbs_After(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
  dbs_sOid *oidp;
  dbs_sQlink *ol;

  dbs_sObject *pop;
    
  oidp = (dbs_sOid *)dbs_Bfind(sts, vep, vep->oid_bt, &op->poid, comp_oid);
  if (oidp == NULL)
    return NULL;
    
  pop = dbs_Address(sts, vep, oidp->ref);
    
  ol = dbs_Qsucc(sts, vep, &op->sib_ll);
    
  if (ol == &pop->sib_lh)
    return NULL;

  return dbs_Qitem(ol, dbs_sObject, sib_ll);
}


dbs_sObject *
dbs_Before(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
  dbs_sOid *oidp;
  dbs_sQlink *ol;

  dbs_sObject *pop;
    
  oidp = (dbs_sOid *)dbs_Bfind(sts, vep, vep->oid_bt, &op->poid, comp_oid);
  if (oidp == NULL)
    return NULL;
    
  pop = dbs_Address(sts, vep, oidp->ref);
    
  ol = dbs_Qpred(sts, vep, &op->sib_ll);
    
  if (ol == &pop->sib_lh)
    return NULL;

  return dbs_Qitem(ol, dbs_sObject, sib_ll);
}


dbs_sObject *
dbs_Parent(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
    return dbs_Address(sts, vep, op->pref);
}

dbs_sObject *
dbs_First(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
    dbs_sQlink *ol;
    
    if (dbs_QisEmpty(sts, vep, &op->sib_lh))
        return NULL;

    ol = dbs_Qsucc(sts, vep, &op->sib_lh);

    return dbs_Qitem(ol, dbs_sObject, sib_ll);
}

dbs_sObject *
dbs_Last(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
    dbs_sQlink *ol;
    
    if (dbs_QisEmpty(sts, vep, &op->sib_lh))
        return NULL;

    ol = dbs_Qpred(sts, vep, &op->sib_lh);

    return dbs_Qitem(ol, dbs_sObject, sib_ll);
}


dbs_sObject *
dbs_NameToObject(pwr_tStatus *sts, const dbs_sVenv *vep, pwr_tOid oid, char *name)
{
    dbs_sOid *oidp;
    dbs_sObject *op;
    dbs_sName n;
    dbs_sName *np;
    
    oidp = (dbs_sOid *)dbs_Bfind(sts, vep, vep->oid_bt, &oid, comp_oid);
    if (oidp == NULL)
        return NULL;
    
    op = dbs_Address(sts, vep, oidp->ref);
    if (op == NULL)
        return NULL;
    
    n.poix = oid.oix;
    strcpy(n.normname, name);
    
    np = (dbs_sName *)dbs_Bfind(sts, vep, vep->name_bt, &n, comp_name);
    if (np == NULL)
        return NULL;
    
    op = dbs_Address(sts, vep, np->ref);
    
    return op;
}

static void
objectName(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op, char *name, int level)
{
    if (op == NULL)
        return;

    if (op->oid.oix == pwr_cNOix) {
        strcpy(name, op->name);
        strcat(name, ":");
    } else {
        objectName(sts, vep, dbs_Address(sts, vep, op->pref), name, level+1);
        strcat(name, op->name);
        if (level > 0)
            strcat(name, "-");
    }
}

void
dbs_ObjectToName(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op, char *name)
{
    objectName(sts, vep, op, name, 0);
}

dbs_sObject *
dbs_Child(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op, char *name)
{
    dbs_sName n;
    dbs_sName *np;

    if ( !op->name_bt.start)
      return NULL;
    
    n.poix = op->oid.oix;
    strcpy(n.normname, name);
    
    np = (dbs_sName *)dbs_Bfind(sts, vep, &op->name_bt, &n, comp_name);
    if (np == NULL)
        return NULL;
    
    op = dbs_Address(sts, vep, np->ref);
    
    return op;
}

dbs_sObject *
dbs_ClassToObject(pwr_tStatus *sts, const dbs_sVenv *vep, pwr_tCid cid)
{
    dbs_sClass *cp;
    dbs_sQlink *ol;
    
    cp = (dbs_sClass *)dbs_Bfind(sts, vep, vep->class_bt, &cid, comp_cid);
    if (cp == NULL)
        return NULL;
    
    if (dbs_QisEmpty(sts, vep, &cp->o_lh))
        return NULL;
    
    ol = dbs_Qsucc(sts, vep, &cp->o_lh);

    return dbs_Qitem(ol, dbs_sObject, o_ll);
}

dbs_sObject *
dbs_Next(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
    dbs_sClass *cp;
    dbs_sQlink *ol;
    
    cp = (dbs_sClass *)dbs_Bfind(sts, vep, vep->class_bt, &op->cid, comp_cid);
    if (cp == NULL)
        return NULL;
    
    ol = dbs_Qsucc(sts, vep, &op->o_ll);
    
    if (ol == &cp->o_lh)
        return NULL;

    return dbs_Qitem(ol, dbs_sObject, o_ll);
}

dbs_sObject *
dbs_Previous(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
    dbs_sClass *cp;
    dbs_sQlink *ol;
    
    cp = (dbs_sClass *)dbs_Bfind(sts, vep, vep->class_bt, &op->cid, comp_cid);
    if (cp == NULL)
        return NULL;
    
    ol = dbs_Qpred(sts, vep, &op->o_ll);
    
    if (ol == &cp->o_lh)
        return NULL;

    return dbs_Qitem(ol, dbs_sObject, o_ll);
}

dbs_sObject *
dbs_Ancestor(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
    while (op != NULL) {
        if (op->oid.oix == 1)
            return NULL;
        if (op->poid.oix == 1)
            return op;
        op = dbs_Address(sts, vep, op->pref);
    }

    return op;
}

void
dbs_GetVolumeName(pwr_tStatus *sts, dbs_sVenv *vep, char *name)
{
    dbs_sVolume *vp = (dbs_sVolume *)(vep->base + vep->sect[dbs_eSect_volume].offset);
    strcpy(name, vp->name);
}

dbs_sObject *
dbs_VolumeObject(pwr_tStatus *sts, const dbs_sVenv *vep)
{
    return (dbs_sObject *)(vep->base + vep->sect[dbs_eSect_object].offset);
}

dbs_sObject *
dbs_Object(pwr_tStatus *sts, const dbs_sVenv *vep)
{
    dbs_sObject *op = dbs_VolumeObject(sts, vep);
    if (op == NULL)
        return NULL;
    return dbs_First(sts, vep, op);
}

void *
dbs_Body(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op, pwr_eBix bix)
{
    char *p = NULL;
    
    switch (bix) {
    case pwr_eBix_rt:
        p = dbs_Address(sts, vep, op->rbody.ref);
        break;
    case pwr_eBix_dev:
        p = dbs_Address(sts, vep, op->dbody.ref);
        break;
    default:
        *sts = DBS__NOSUCHBODY;
        break;
    }
    
    return p;
}

dbs_sObject *
dbs_NextHead(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sObject *op)
{
    dbs_sObject *eop = (dbs_sObject*)(vep->base + vep->sect[dbs_eSect_object].offset + vep->sect[dbs_eSect_object].size);

    if (op == NULL)
      op = (dbs_sObject*)(vep->base + vep->sect[dbs_eSect_object].offset);
    else
      op = (dbs_sObject*)((char *)op + dbs_dAlign(sizeof(dbs_sObject)));

    if (op >= eop)
      return NULL;
    
    return op;
}

dbs_sBody *
dbs_NextRbody(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sBody *bp)
{
    dbs_sBody *ebp = (dbs_sBody*)(vep->base + vep->sect[dbs_eSect_rbody].offset + vep->sect[dbs_eSect_rbody].size);

    if (bp == NULL)
      bp = (dbs_sBody*)(vep->base + vep->sect[dbs_eSect_rbody].offset);
    else
      bp = (dbs_sBody*)((char *)bp + dbs_dAlign(sizeof(dbs_sBody)) + dbs_dAlign(bp->size));

    if (bp >= ebp)
      return NULL;
    
    return bp;
}


dbs_sBody *
dbs_NextDbody(pwr_tStatus *sts, const dbs_sVenv *vep, dbs_sBody *bp)
{
    dbs_sBody *ebp = (dbs_sBody*)(vep->base + vep->sect[dbs_eSect_dbody].offset + vep->sect[dbs_eSect_dbody].size);

    if (bp == NULL)
      bp = (dbs_sBody*)(vep->base + vep->sect[dbs_eSect_dbody].offset);
    else
      bp = (dbs_sBody*)((char *)bp + dbs_dAlign(sizeof(dbs_sBody)) + dbs_dAlign(bp->size));

    if (bp >= ebp)
      return NULL;
    
    return bp;
}

#endif
