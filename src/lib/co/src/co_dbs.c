/* co_dbs.c -- <short description>

   PROVIEW/R
   Copyright (C) 2002 by Cell Network AB.

   . */


#include <string.h>

#include "pwr.h"
#include "co_dbs.h"
#include "co_dbs_msg.h"
#include "co_errno.h"
#include "co_platform.h"



void *
dbs_Address(pwr_tStatus *sts, dbs_sEnv *ep, dbs_tRef r)
{
#if 0
  if (r == dbs_cNRef)
    return NULL;

  if (!ep->flags.b.isMapped)
      pwr_ReturnOrBugcheck(NULL, sts, DBS__NOTMAPPED, "");

  if (r < ep->size) {
      return (void *)(ep->base + r);
  }
  pwr_ReturnOrBugcheck(NULL, sts, DBS__BADOFFS, "");
#endif
  return NULL;
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

void *dbs_Bfind(pwr_tStatus *sts, dbs_sEnv *ep, dbs_sBintab *tp, void *key, int (comp)(void *key, void *record))
{
    void *p;
    int   c;
    void *start = dbs_Address(sts, ep, tp->start);
    void *end   = dbs_Address(sts, ep, tp->end);
    int   rsize = tp->rsize;
    
    
    while (start < end) {
        p = start + ((end - start) / (2*rsize)) * rsize;
        c = comp(key, p);
        if (c < 0) {
            start = p + rsize;
        } else if (c > 0) {
            end = p - rsize;
        } else {
            return p;
        }
    }
    
    return 0;
}


dbs_sQlink *
dbs_Qalloc(pwr_tStatus*, dbs_sEnv*)
{
}

pwr_tBoolean
dbs_QhasOne(pwr_tStatus *sts, dbs_sEnv *ep, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(ep, item));

    pred = dbs_Address(sts, ep, item->blink);
    succ = dbs_Address(sts, ep, item->flink);
  
    pwr_Assert(checkQ(ep, succ));
    pwr_Assert(checkQ(ep, pred));
    pwr_Assert(item->blink == pred->self);
    pwr_Assert(item->flink == succ->self);
    pwr_Assert(pred->flink == item->self);
    pwr_Assert(succ->blink == item->self);

    return item->flink == item->blink;
}
#endif

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

pwr_tBoolean
dbs_QisEmpty(pwr_tStatus *sts, dbs_sEnv *cp, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(php, item));

    pred = dbs_Address(sts, cp, item->blink);
    succ = dbs_Address(sts, cp, item->flink);
  
    pwr_Assert(checkQ(cp, succ));
    pwr_Assert(checkQ(cp, pred));
    pwr_Assert(item->blink == pred->self);
    pwr_Assert(item->flink == succ->self);
    pwr_Assert(pred->flink == item->self);
    pwr_Assert(succ->blink == item->self);

    return item->flink == item->self;
}

pwr_tBoolean
dbs_QisLinked(pwr_tStatus *sts, dbs_sEnv *cp, dbs_sQlink *item)
{
    dbs_sQlink *pred;
    dbs_sQlink *succ;

    pwr_Assert(checkQ(cp, item));

    pred = dbs_Address(sts, cp, item->blink);
    succ = dbs_Address(sts, cp, item->flink);
  
    pwr_Assert(checkQ(cp, succ));
    pwr_Assert(checkQ(cp, pred));
    pwr_Assert(item->blink == pred->self);
    pwr_Assert(item->flink == succ->self);
    pwr_Assert(pred->flink == item->self);
    pwr_Assert(succ->blink == item->self);

    return item->flink != item->self;
}

pwr_tBoolean
dbs_QisNull(pwr_tStatus *sts, dbs_sEnv *cp, dbs_sQlink *item)
{
    pwr_tBoolean nullQ;

    nullQ = (item->self == dbs_cNRef)
        || (item->blink == dbs_cNRef)
        || (item->flink == dbs_cNRef);
  
    pwr_Assert(item->blink == dbs_cNRef);
    pwr_Assert(item->flink == dbs_cNRef);
    pwr_Assert(item->self  == dbs_cNRef);

    return nullQ;
}

dbs_sQlink *
dbs_Qpred(pwr_tStatus *sts, dbs_sEnv *cp, dbs_sQlink *item)
{
    dbs_sQlink *pred;

    pwr_Assert(checkQ(cp, item));

    pred = dbs_Address(NULL, cp, item->blink);
  
    if (pred != NULL) {
        pwr_Assert(checkQ(cp, pred));
        pwr_Assert(pred->flink == item->self);
        pwr_Assert(pred->self == item->blink);
    }

    return pred;  
}

dbs_sQlink *
dbs_Qsucc(pwr_tStatus *sts, dbs_sEnv *cp, dbs_sQlink *item);
{
    pool_sQlink *succ;

    pwr_Assert(checkQ(cp, item));

    succ = dbs_Address(NULL, cp, item->flink);
  
    if (succ != NULL) {
        pwr_Assert(checkQ(cp, succ));
        pwr_Assert(succ->blink == item->self);
        pwr_Assert(item->flink == succ->self);
    }

    return succ;  
}

dbs_tRef        dbs_RefAlloc(pwr_tStatus*, dbs_sEnv*, pwr_tUInt32);

/* Translate a virtual address to a pool reference.
   Return a pool_tRef, and signals errors.  */

dbs_tRef
dbs_Reference(pwr_tStatus sts*, dbs_sEnv *ep, void *adrs)
{
    long iadrs = (long)adrs;
    long ofs;
    long sect = 
    long base = cp->sect[]

    if (iadrs < (long)cp->base)
        return dbs_cNRef;
    
    ofs = iadrs - (long)ep->base;
    if (ofs >= ep->size)
        return dbs_cNRef;

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

pwr_tBoolean 
dbs_AlignedRead(pwr_tStatus *sts, void *buf, pwr_tUInt32 size, dbs_sEnv *ep)
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


#if 0
dbs_sFile *
dbsdr_sFile(void *s, dbs_sFile *t)
{
    encode_oid();
    encode_int();
    encode_time();
    
}

dbs_sSect *
dbsdr_sSect(void *s, dbs_sFile *t)
{
}

dbs_sVolume *
dbsdr_sVolume(void *s, dbs_sVolume *t)
{
}

dbs_sVolRef *
dbsdr_sVolRef(void *s, dbs_sVolRef *t)
{
}


dbs_sObject *
dbsdr_sObject(void *s, dbs_sObject *t)
{
}

dbs_sBody *
dbsdr_sBody(void *s, dbs_sBody *t)
{
}

#endif
