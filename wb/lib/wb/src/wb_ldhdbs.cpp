/** 
 * Proview   $Id: wb_ldhdbs.cpp,v 1.2 2005-09-01 14:57:58 claes Exp $
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
 **/

/* wb_ldhdbs.c
   This module contains functions to create database snapshot files.  */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef OS_VMS
#include <descrip.h>
#include <lib$routines.h>
#endif

#include <X11/Intrinsic.h>

#include "pwr.h"
#include "pwr_class.h"
#include "wb_ldhi.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_tree.h"
#include "co_pdr.h"
#include "co_dbs.h"
#include "pwr_class.h"
#include "wb_ldh_msg.h"
#include "wb_ldhld.h"
#include "wb_ldh.h"
#include "rt_dlinux.h"
#include "wb_object.h"

#if 0
typedef union {
    struct {
        pwr_tBit    load          : 1;
        pwr_tBit    createNewFile : 1;
        pwr_tBit    useOldFile    : 1;
        pwr_tBit    debug         : 1;
    } b;

    pwr_tBitMask m;

#define mLSCB_load		     1
#define mLSCB_createNewFile 2
#define mLSCB_useOldFile	 4
#define mLSCB_debug	     8
} mLSCB;

typedef union {
    struct {
        pwr_tBit    devOnly       : 1;
    } b;

    pwr_tBitMask m;

#define mOentry_devOnly		     1
} mOentry;

/**
 * Load File Control Block
 */
typedef struct sLFCB {
    FILE         *fp;                 /**< file pointer */
    char         name[100];           /**< name of load file */
    //dbs_sSect    sect[dbs_eSect_];    /**< section header */
    //fpos_t       sectPos;             /**< section position in file */
    //fpos_t       identSectPos;        /**< identity Section position in file */
    pwr_tBoolean isNew;               /**< true if new load file, else false */
} sLFCB;

typedef struct sOentry sOentry;
typedef struct sNentry sNentry;
typedef struct sMentry sMentry;

struct sOentry {
    tree_sNode       node;
    dbs_sObject      o;
    dbs_sBody        rbody;
    dbs_sBody        dbody;    
    mOentry          flags;
    dbs_tRef         ref;

    sOentry         *poep;
    sOentry         *boep;
    sOentry         *aoep;
    sOentry         *foep;
    sOentry         *loep;
    
    sOentry         *o_ll;
    sOentry         *cid_ll;
    sNentry         *name_f;
    sNentry         *name_l;
    wb_object       *ohp;
};

/**
 * Load Control Block
 */
typedef struct sLCB {
    ldh_tSession       *sp;		/* Session context.  */
    wb_object         *o;		/* Volume object header.  */
    dbs_sVolume         volume;
    pwr_tOid            oid;
    dbs_sSect           sect[dbs_eSect_];    /**< section header */
    dbs_eFile           file;
    sLFCB               oldFile;
    sLFCB               newFile;
    unsigned int        warnings;
    unsigned int        errors;
    
    sOentry             *oep;   /* object entry of volume object */
    sOentry             *o_lh;
    sOentry             *o_ll;

    sMentry             *vol_lh; /* Header of volref list. */
    sMentry             *vol_ll;

    tree_sTable         *oix_th;
    tree_sTable         *name_th;
    tree_sTable         *meta_th;
    
} sLCB;

struct sNentry {
    tree_sNode node;
    dbs_sName  n;
    dbs_tRef   ref;
    sOentry    *oep;
};

struct sCentry {
    tree_sNode node;
    dbs_sMeta  m;
    dbs_tRef   ref;
    sOentry    *cid_lh;
    sMentry    *vol_ll;
    //sOentry    *o_lh;
};

static sLCB          *alloc_lcb(ldhi_sSession *sp);
static void           buildSnapshot(sLCB *lcbp);
static pwr_tStatus    closeFile(sLFCB *lfcbp, pwr_tBoolean doDelete);
static int            comp_dbs_name(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);
static pwr_tBoolean   createFile(sLCB *lcbp);
static void           free_lcb(sLCB *lcbp);
static ldhi_sObjHead *getAliasServer(sLCB *lcbp, ldhi_sObjHead *o, pwr_tOid *soid);
static pwr_tStatus    getMountServer(sLCB *lcbp, ldhi_sObjHead *o, pwr_tOid *soid);
static void          *getDbody(sLCB *lcbp, sOentry *oep);
static sOentry       *getObject(sLCB *lcbp, ldhi_sObjHead *o, sOentry *fep);
static void          *getRbody(sLCB *lcbp, sOentry *oep);
static void           getVolume(sLCB *lcbp);
static pwr_tStatus    initDirectory(sLCB *lcbp);
static sOentry       *installObject(sLCB *lcbp, ldhi_sObjHead *o, sOentry *fep);
static void           metaInsert(sLCB *lcbp, sOentry *oep);
static pwr_tStatus    openFile(sLFCB *lfcbp);
static pwr_tStatus    writeSectFile(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectDirectory(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectVolume(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectVolref(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectOix(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectObject(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectRbody(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectName(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectMeta(sLCB *lcbp, sLFCB *lfcbp);
static pwr_tStatus    writeSectDbody(sLCB *lcbp, sLFCB *lfcbp);

static int
comp_dbs_name(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y)
{
    dbs_sName *xKey = (dbs_sName *) (tp->keyOffset + (char *) x);
    dbs_sName *yKey = (dbs_sName *) (tp->keyOffset + (char *) y);
    int comp;
    

    if (xKey->poix < yKey->poix)
        comp = -1;
    else if (xKey->poix == yKey->poix)
        comp = strcmp(xKey->normname, yKey->normname);
    else
        comp = 1;

//    printf("%d:%s %c %d:%s\n", xKey->poix, xKey->normname, (comp == 1 ? '>' : (comp == -1 ? '<' : '=')), yKey->poix, yKey->normname);

    return comp;
}


static sOentry *
installObject(sLCB *lcbp, wb_object &o, sOentry *fep)
{
    sNentry      ne;
    sNentry     *nep;
    sOentry     *oep;
    pwr_tStatus  sts;
    
    oep = (sOentry *)tree_Insert(&sts, m_lcbp->oix_th, &o.oix());
    if (sts == TREE__INSERTED) {
        
    } else {
    }
    
    oep->fep = fep = (sOentry *)tree_Insert(&sts, m_lcbp->oix_th, &o.foix());
    if (sts == TREE__INSERTED) {
        fep->flags.b.exist = 0;
    } else {
    }

    oep->aep = aep = (sOentry *)tree_Insert(&sts, m_lcbp->oix_th, &o.aoix());
    if (sts == TREE__INSERTED) {
        
    } else {
    }
    
    




    m_lcbp->volume.cardinality++;
    
#if 0
    /* link in object load list */
    if (m_lcbp->o_lh == NULL) {
        m_lcbp->o_ll = m_lcbp->o_lh = oep;
    } else {
        m_lcbp->o_ll->o_ll = oep;
        m_lcbp->o_ll = oep;
    }
#endif

    if (fep == NULL)
        oep->flags.b.devOnly = o.isDevOnly();
    else
        oep->flags.b.devOnly = fep->flags.b.devOnly || o.isDevOnly();

    oep->o.oid  = o.oid();
    oep->o.cid  = o.cid();
    oep->o.poid = o.poid();
    strcpy(oep->o.name, o.name());
    strcpy(oep->o.normname, o.normname());

/** @todo temporary fix for object time . ML 020703 */
#if 1
    oep->o.time = m_lcbp->volume.time;
#else
    oep->o.time = o.time(); 
#endif

#if 0
    oep->o.flags = ;
    oep->o.sib_ll = ;
    oep->o.cid_ll = ;
    oep->o.name_bt = ;
#endif
    if (fep != NULL)
        oep->o.pref = fep->ref;
    

    oep->ref = dbs_dMakeRef(dbs_eSect_object, m_lcbp->sect[dbs_eSect_object].size);
    m_lcbp->sect[dbs_eSect_object].size += dbs_dAlign(sizeof(dbs_sObject));
    dbs_Qinit(&sts, &oep->o.sib_lh, oep->ref + offsetof(dbs_sObject, sib_lh));
    dbs_Qinit(&sts, &oep->o.sib_ll, oep->ref + offsetof(dbs_sObject, sib_ll));
    dbs_Qinit(&sts, &oep->o.oid_ll, oep->ref + offsetof(dbs_sObject, oid_ll));
    
    /* insert into name table */
    memset(&ne, 0, sizeof(ne));
    ne.n.poix = o.poid().oix;
    strcpy(ne.n.normname, o.normname());
    nep = (sNentry*)tree_Insert(&sts, m_lcbp->name_th, &ne.n);
    nep->oep = oep;
    
    // @todo move to calculate ref part: nep->n.ref = oep->ref;
            
    //metaInsert(lcbp, oep);

    oep->o.rbody.time = o.rbTime();
    oep->o.rbody.size = o.rbSize();
    if (oep->o.rbody.size > 0) {
        oep->rbody.oid = o.oid();
        oep->rbody.size = dbs_dAlign(oep->o.rbody.size);
//        printf("o.rsize[%s]: %d + %d -> %d\n", o->db.name.data, oep->rbody.size, dbs_align(sizeof(dbs_sBody)), m_lcbp->sect[dbs_eSect_rbody].size);
        oep->o.rbody.ref = dbs_dMakeRef(dbs_eSect_rbody, m_lcbp->sect[dbs_eSect_rbody].size + dbs_dAlign(sizeof(dbs_sBody)));
    
        m_lcbp->sect[dbs_eSect_rbody].size += oep->rbody.size + dbs_dAlign(sizeof(dbs_sBody));
        m_lcbp->volume.rbodySize += oep->rbody.size;
    }

    oep->o.dbody.time = o.dbTime();
    oep->o.dbody.size = o.dbSize();
    if (oep->o.dbody.size > 0) {
        oep->dbody.oid = o.oid();
        oep->dbody.size = dbs_dAlign(oep->o.dbody.size);
        oep->o.dbody.ref = dbs_dMakeRef(dbs_eSect_dbody, m_lcbp->sect[dbs_eSect_dbody].size + dbs_dAlign(sizeof(dbs_sBody)));
    
        m_lcbp->sect[dbs_eSect_dbody].size += oep->dbody.size + dbs_dAlign(sizeof(dbs_sBody));
    }

    return oep;
}

static void *
getRbody(sLCB *m_lcbp, sOentry *oep)
{
    pwr_tStatus    sts;
    ldhi_sObjBody *bp;
    
    sts = ldhi_GetObjectBody(m_lcbp->sp, oep->ohp, ldhi_eBid_SysBody, &bp);
    if (EVEN(sts))
        return NULL;
    
    return bp->body;
}

static void *
getDbody(sLCB *m_lcbp, sOentry *oep)
{
    pwr_tStatus    sts;
    ldhi_sObjBody *bp;
    
    sts = ldhi_GetObjectBody(m_lcbp->sp, oep->ohp, ldhi_eBid_DevBody, &bp);
    if (EVEN(sts))
        return NULL;
    
    return bp->body;
}


static void
getVolume(sLCB *m_lcbp)
{

    m_lcbp->oep = getObject(m_lcbp, m_lcbp->o, NULL);
}

static sOentry *
getObject(sLCB *lcbp, wb_object &o, sOentry *fep)
{
    int         isMountClient = 0;
    int         isAliasClient = 0;
    pwr_tOid    soid          = pwr_cNOid;
    dbs_sName   ne;
    sNentry    *nep;
    sOentry    *oep;
    sOentry    *cep;
    pwr_tStatus sts;
    wb_object   a;
    wb_object   c;
    dbs_sQlink *sib_lh;
    dbs_sQlink *sib_ll;
    dbs_sQlink *ll;
    
    switch (o.cid()) {
    case pwr_eClass_LibHier:
        if (m_lcbp->volume.cid == pwr_eClass_ClassVolume)
            break;
        /* do not load libHiers */
        return NULL;
    case pwr_eClass_Alias:
        if (m_lcbp->volume.cid == pwr_eClass_ClassVolume)
            break;
        
        a = getAliasServer(m_lcbp, o, &soid);
        if (!a) {
            /* do not load this alias */
            return NULL;
        }
        isAliasClient = 1;
        break;
    case pwr_eClass_MountVolume:
    case pwr_eClass_CreateVolume:
    case pwr_eClass_MountObject:
        if (m_lcbp->volume.cid == pwr_eClass_ClassVolume)
            break;
        isMountClient = 1;
        if (m_lcbp->volume.cid != pwr_eClass_RootVolume) {
            // @todo!!! report error/warning?
            return NULL;
        }
        sts = getMountServer(m_lcbp, o, &soid);
        break;
    default:
        break;    
    }

    oep = installObject(m_lcbp, o, fep);
    oep->o.soid = soid;
    oep->o.flags.b.isMountClient = isMountClient;
    oep->o.flags.b.isAliasClient = isAliasClient;
    oep->o.flags.b.isMountClean = 1;
    oep->ohp = &o; // @todo is this really necessary?
    
    sib_lh = sib_ll = &oep->o.sib_lh;
    for (c = o.first(); c; c = c.after()) {
        cep = getObject(m_lcbp, c, oep);
        if (cep == NULL)
            continue;
        ll = &cep->o.sib_ll;
        dbs_Qinsert(&sts, sib_ll, ll, sib_lh);
        sib_ll = ll;
        oep->o.flags.b.isMountClean |= cep->o.flags.b.isMountClean && !cep->o.flags.b.isMountClient;
    }
    
    /* get reference to first and last child in name order */
//    printf("%s [%d]", oep->o.name, oep->ref);
    memset(&ne, 0, sizeof(ne));
    ne.poix = oep->o.oid.oix;
    nep = (sNentry*)tree_FindSuccessor(&sts, m_lcbp->name_th, &ne);
    if (nep != NULL && nep->n.poix == oep->o.oid.oix) {
        oep->name_f = nep;
//        printf(" %s", nep->n.normname);
    }
    
    ne.poix += 1;
    nep = (sNentry*)tree_FindPredecessor(&sts, m_lcbp->name_th, &ne);
    if (nep != NULL && nep->n.poix == oep->o.oid.oix) {
        oep->name_l = nep;
//        printf(" %s", nep->n.normname);
    }
//    printf("\n");

    return oep;
}

static void
metaInsert(sLCB *lcbp, sOentry *oep) 
{
    pwr_tStatus  sts;
    pwr_tOid     coid;
    sMentry     *cep;

#if 0    
    /* Tanke! Hur faar jag in volymsobjectet foer refererade volymer? */

    if (oep->o.oid.oix == 0 && oep->o.cid == pwr_eClass_ClassVolume) {
        sMentry *mep = (sMentry*)tree_Insert(&sts, m_lcbp->meta_th, &oep->o.oid);
        if (sts == TREE__INSERTED) {
            mep->m.type = dbs_eMeta_m;
            mep->m.u.m.oid = oep->o.oid;            
            mep->m.u.m.cid = oep->o.cid;
            strcpy(mep->m.u.m.name, oep->o.name);
            mep->m.u.m.time = oep->o.time;
        }
    }
#endif
   
    coid = cdh_ClassIdToObjid(oep->o.cid);

    cep = (sMentry*)tree_Insert(&sts, m_lcbp->meta_th, &coid);
    if (sts == TREE__INSERTED) {
        /* not previously inserted */
        ldhi_sObjHead   *chp;
        ldhi_sObjHead   *bhp;
        ldhi_sObjBody   *bbp;
        pwr_sObjBodyDef *bp;
        pwr_tOid         v_oid;
        sMentry         *vep;
        

        v_oid.oix = 0;
        v_oid.vid = coid.vid;
        
        vep = (sMentry*)tree_Insert(&sts, m_lcbp->meta_th, &v_oid);
        if (sts == TREE__INSERTED) {
            ldhi_sObjHead   *vhp;

            sts = ldhi_ObjidToOh(m_lcbp->sp->wb, v_oid, &vhp);
            if (EVEN(sts)) {
                printf("No such volume, %s\n", cdh_ClassObjidToString(NULL, v_oid, 0));
                return;
            }
                        
            vep->m.type = dbs_eMeta_m;
            vep->m.u.m.oid = oep->o.oid;            
            vep->m.u.m.cid = oep->o.cid;
            strcpy(vep->m.u.m.name, vhp->db.name.data);
            vep->m.u.m.time = vhp->db.time;
        }

        sts = ldhi_ObjidToOh(m_lcbp->sp->wb, coid, &chp);
        if (EVEN(sts)) {
            printf("No such class, %s\n", cdh_ClassObjidToString(NULL, coid, 0));
            return;
        }
//        printf("m_c:     [%s] %s\n", cdh_ClassObjidToString(NULL, coid, 0), chp->db.name.data);
        cep->m.type = dbs_eMeta_c;
        cep->m.u.c.oid = chp->db.oid;
        cep->m.u.c.cid = oep->o.cid;
        strcpy(cep->m.u.c.name, chp->db.name.data);
        cep->m.u.c.time = oep->o.time;
        // cep->m.u.c.cid_lh gets its value in 
        // cep->m.u.c.bid_lh gets its value in
        
    }

    oep->cid_ll = cep->cid_lh;
    cep->cid_lh = oep;
}


static void
buildSnapshot(sLCB *lcbp)
{
    dbs_sQlink *cid_lh;
    dbs_sQlink *cid_ll;
    dbs_sQlink *bid_lh;
    dbs_sQlink *bid_ll;
    dbs_sQlink *aid_lh;
    dbs_sQlink *aid_ll;
    dbs_sQlink *oid_lh;
    dbs_sQlink *oid_ll;
    dbs_sQlink *tid_lh;
    dbs_sQlink *tid_ll;
    dbs_sQlink *ll;
    sNentry    *nep;
    sMentry    *mep;
    sOentry    *oep;
    pwr_tStatus sts;

    m_lcbp->sect[dbs_eSect_oix].size = dbs_dAlign(sizeof(dbs_sOix)) * m_lcbp->volume.cardinality;

    /* allocate space for all name entries in name section*/

    nep = (sNentry*)tree_Minimum(NULL, m_lcbp->name_th);
    while (nep != NULL) {
        nep->ref = dbs_dMakeRef(dbs_eSect_name, m_lcbp->sect[dbs_eSect_name].size);
        m_lcbp->sect[dbs_eSect_name].size += dbs_dAlign(sizeof(dbs_sName));

        nep = (sNentry*)tree_Successor(NULL, m_lcbp->name_th, (tree_sNode*)nep);
    }

    /* allocate space for all meta entries in meta section */

    mep = (sMentry*)tree_Minimum(NULL, m_lcbp->meta_th);
    while (mep != NULL) {
        
        mep->ref = dbs_dMakeRef(dbs_eSect_meta, m_lcbp->sect[dbs_eSect_meta].size);
        m_lcbp->sect[dbs_eSect_meta].size += dbs_dAlign(sizeof(dbs_sMeta));

        switch (mep->m.type) {
        case dbs_eMeta_m:
//            printf("m: [%s] %s\n", cdh_ClassObjidToString(NULL, mep->m.u.m.oid, 0), mep->m.u.m.name);
            cid_lh = cid_ll = &mep->m.u.m.cid_lh;
            dbs_Qinit(&sts, cid_lh, mep->ref + offsetof(dbs_sMeta, u.m.cid_lh));
            tid_lh = tid_ll = &mep->m.u.m.tid_lh;
            dbs_Qinit(&sts, tid_lh, mep->ref + offsetof(dbs_sMeta, u.m.tid_lh));
            break;
        case dbs_eMeta_c:
            /* insert in list of classes in this mvol */
//            printf("c: [%s] %s\n", cdh_ClassObjidToString(NULL, mep->m.u.c.oid, 0), mep->m.u.c.name);
            ll = &mep->m.u.c.cid_ll;
            dbs_Qinit(&sts, ll, mep->ref + offsetof(dbs_sMeta, u.c.cid_ll));
            dbs_Qinsert(&sts, cid_ll, ll, cid_lh);
            cid_ll = ll;

            oid_lh = oid_ll = &mep->m.u.c.oid_lh;
            dbs_Qinit(&sts, oid_lh, mep->ref + offsetof(dbs_sMeta, u.c.oid_lh));
            bid_lh = bid_ll = &mep->m.u.c.bid_lh;
            dbs_Qinit(&sts, bid_lh, mep->ref + offsetof(dbs_sMeta, u.c.bid_lh));

            /* link all objects in class list */
            for (oep = mep->cid_lh; oep != NULL; oep = oep->cid_ll) {
//                printf("   %s\n", oep->o.name);
                oep->o.cref = mep->ref;
                ll = &oep->o.oid_ll;
                dbs_Qinsert(&sts, oid_ll, ll, oid_lh);
                oid_ll = ll;
            }
            
            break;
        default:
            break;
        }
        
        mep = (sMentry*)tree_Successor(NULL, m_lcbp->meta_th, mep);
    }


    oep = (sOentry*)tree_Minimum(NULL, m_lcbp->oix_th);
    while (oep != NULL) {
        if (oep->name_f != NULL) {
            oep->o.name_f = oep->name_f->ref;
            pwr_Assert(oep->name_l != NULL);
            oep->o.name_l = oep->name_l->ref;
        }
 
        oep = (sOentry*)tree_Successor(NULL, m_lcbp->oix_th, (tree_sNode*)oep);
    }
}

static pwr_tStatus
closeFile(sLFCB *lfcbp, pwr_tBoolean doDelete)
{

    if (lfcbp->fp != NULL)
        fclose(lfcbp->fp);
    if (doDelete && *lfcbp->name != '\0') {
        if (remove(lfcbp->name) == 0)
            printf("-- Deleted file: %s\n", lfcbp->name);
    }

    return LDH__SUCCESS;
}

static pwr_tBoolean
createFile(sLCB *lcbp)
{
    pwr_tStatus sts;
    int i;
    int size;
    
    /* Calculate offset of all sections. */
    size = dbs_dAlign(sizeof(dbs_sFile));
    m_lcbp->sect[dbs_eSect_dir].size = sizeof(m_lcbp->sect);
    
    for (i = 0; i < dbs_eSect_; i++) {
        m_lcbp->sect[i].offset = size;
        size += m_lcbp->sect[i].size;
        printf("sect[%d].size: %d, .offset: %d\n", i, m_lcbp->sect[i].size, m_lcbp->sect[i].offset);
    }
    
    printf("\n-- Working with load file volume '%s'...\n", m_lcbp->o->name());

    //readOldLoadFile(m_lcbp, &m_lcbp->oldFile);

    printf("-- Open file...\n");
    sts = openFile(&m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing file section...\n");
    sts = writeSectFile(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing directory section...\n");
    sts = writeSectDirectory(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing volume section...\n");
    sts = writeSectVolume(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing volref section...\n");
    sts = writeSectVolref(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    //printf("-- Writing object headers...\n");

    //sts = writeHeads(m_lcbp, &m_lcbp->newFile, m_lcbp->o);

    printf("-- Writing oix section...\n");
    sts = writeSectOix(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing object section...\n");
    sts = writeSectObject(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    //printf("-- Writing object rt bodies from workbench...\n");

    printf("-- Writing rbody section...\n");
    sts = writeSectRbody(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing name section...\n");
    sts = writeSectName(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing meta section...\n");
    sts = writeSectMeta(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing dbody section...\n");
    sts = writeSectDbody(m_lcbp, &m_lcbp->newFile);
    if (EVEN(sts)) goto error_handler;

    //closeLoadFile(&m_lcbp->oldFile, 0);
    printf("-- Close file...\n");
    sts = closeFile(&m_lcbp->newFile, 0);
    if (EVEN(sts)) goto error_handler;

    printf("-- Successfully created load file for volume '%s'\n", m_lcbp->o->name());
    printf("-- %d objects with a total body size of %d bytes were written to new file.\n",
           m_lcbp->volume.cardinality, m_lcbp->volume.rbodySize);

    return 1;

  error_handler:
    /* Clean up the mess.  */
    printf("-- Load file was not created for volume '%s'\n", m_lcbp->o->name());
    //closeLoadFile(&m_lcbp->oldFile, 0);
    closeFile(&m_lcbp->newFile, 1);
    return 0;
}

static ldhi_sObjHead *
getAliasServer(sLCB *m_lcbp, ldhi_sObjHead *o, pwr_tOid *soid)
{
    pwr_tStatus      sts;
    pwr_sAlias       *alias;
    ldhi_sObjBody    *obp;
    ldhi_sObjHead    *a;

    if (m_lcbp->o->cid() == pwr_eClass_ClassVolume) {
        return NULL;
    }

    alias = (pwr_sAlias *)o.rbody();

    if (cdh_ObjidIsNull(alias->Object)) {
        printf("!! Alias does not refer to any object!\n");
        printf("   Alias:  %s\n", ldhi_PathName(o, 0));
        printf("   Alias will not be loaded.\n");
        m_lcbp->warnings++;
        return NULL; 
    }

    if (alias->Object.vid != m_lcbp->o->vid()) {
        printf("!! Alias refers to object outside own volume!\n");
        printf("   Alias:  %s\n", ldhi_PathName(o, 0));
        printf("   Object: %s\n", cdh_ObjidToString(NULL, alias->Object, 1));
        printf("   Alias will not be loaded.\n");
        m_lcbp->warnings++;
        return NULL; 
    }
  
    sts = ldhi_ObjidToOh(m_lcbp->sp->wb, alias->Object, &a);
    if (EVEN(sts)) {
        printf("!! Alias refers to a non existing object!\n");
        printf("   Alias:  %s\n", ldhi_PathName(o, 0));
        printf("   Alias will not be loaded.\n");
        m_lcbp->warnings++;
        return NULL; 
    }
  
    if (a->db.flags.f.DevOnly) {
        printf("!! An alias may not refer to a non runtime object!\n");
        printf("   Alias:  %s\n", ldhi_PathName(o, 0));
        printf("   Object: %s\n", ldhi_PathName(a, 0));
        printf("   Alias will not be loaded.\n");
        m_lcbp->warnings++;
        return NULL;
    }

    switch (a->db.cid) {
    case pwr_eClass_Alias:
        printf("!! An alias may not refer to another alias!\n");
        printf("   Alias:  %s\n", ldhi_PathName(o, 0));
        printf("   Object: %s\n", ldhi_PathName(a, 0));
        printf("   Alias will not be loaded.\n");
        m_lcbp->warnings++;
        return NULL;
        break;
    case pwr_eClass_MountVolume:
    case pwr_eClass_CreateVolume:
    case pwr_eClass_MountObject:
        printf("!! An alias may not refer to a mount object!\n");
        printf("   Alias:  %s\n", ldhi_PathName(o, 0));
        printf("   Object: %s\n", ldhi_PathName(a, 0));
        printf("   Alias will not be loaded.\n");
        m_lcbp->warnings++;
        return NULL;
        break;
    }

    *soid = a->db.oid;
    
    return a;
}

static pwr_tStatus
getMountServer(sLCB *m_lcbp, wb_object &o, pwr_tOid *soid)
{
    pwr_tStatus        sts;
    pwr_sMountObject  *mountObject;
    pwr_sCreateVolume *createVolume;
    pwr_sMountVolume  *mountVolume;

    switch (o->db.cid) {
    case pwr_eClass_MountObject:
        mountObject = (pwr_sMountObject *) o.rbody();
        *soid = mountObject->Object;
        break;
    case pwr_eClass_MountVolume:
        mountVolume = (pwr_sMountVolume *) o.rbody();
        *soid = pwr_cNOid;
        soid->vid = mountVolume->Volume;
        break;
    case pwr_eClass_CreateVolume:
        createVolume = (pwr_sCreateVolume *) o.rbody();
        *soid = pwr_cNOid;
        soid->vid = createVolume->Volume;
        break;
    }

    return LDH__SUCCESS;
}  

static sLCB*
alloc_lcb(ldhi_sSession *sp)
{
    pwr_tStatus      sts;
    sLCB *m_lcbp;
    
    m_lcbp = (sLCB*)calloc(1, sizeof(*m_lcbp));

    clock_gettime(CLOCK_REALTIME, &m_lcbp->volume.time);

    m_lcbp->o = //sp->volctx->vhp->ohp;
    m_lcbp->sp = sp;

    m_lcbp->oid = m_lcbp->o->oid();
    m_lcbp->volume.vid = m_lcbp->o->vid();
    strcpy(m_lcbp->volume.name, m_lcbp->o->name());
    m_lcbp->volume.cid = m_lcbp->o->cid();
    @todo strcpy(m_lcbp->volume.className, m_lcbp->ohp->chp->db.name.data);
 
    m_lcbp->file = dbs_eFile_volume;
    sprintf(m_lcbp->oldFile.name, dbs_cNameVolume, dbs_cDirectory, m_lcbp->o->name());
    dcli_translate_filename(m_lcbp->oldFile.name, m_lcbp->oldFile.name);
    sprintf(m_lcbp->newFile.name, dbs_cNameVolume, dbs_cDirectory, m_lcbp->o->name());
    dcli_translate_filename(m_lcbp->newFile.name, m_lcbp->newFile.name);
    m_lcbp->oldFile.isNew = 0;
    m_lcbp->newFile.isNew = 1;

    m_lcbp->oix_th = tree_CreateTable(&sts, sizeof(pwr_tOix), offsetof(sOentry, o.oid.oix),
                                    sizeof(sOentry), 1000, tree_Comp_oix);

    m_lcbp->name_th = tree_CreateTable(&sts, sizeof(dbs_sName), offsetof(sNentry, n),
                                     sizeof(sNentry), 1000, comp_dbs_name);

    m_lcbp->meta_th = tree_CreateTable(&sts, sizeof(pwr_tOid), offsetof(sMentry, m.u.o),
                                     sizeof(sMentry), 1000, tree_Comp_oid);

    initDirectory(m_lcbp);
    
    return m_lcbp;
}

static void
free_lcb(sLCB *m_lcbp)
{
    return;
}


static pwr_tStatus
openFile(sLFCB *lfcbp)
{
    char *fn;

    cdh_ToLower(lfcbp->name, lfcbp->name);
    
    lfcbp->fp = fopen(lfcbp->name, (lfcbp->isNew ? "w+b" : "rb"));
    if (lfcbp->fp == NULL) {
        printf("%s Cannot open %s file: %s\n",
               (lfcbp->isNew ? "**" : "--"), (lfcbp->isNew ? "new" : "old"), lfcbp->name);
        perror("   Reason");
        return LDH__FILEOPEN;
    }
    fn = dcli_fgetname(lfcbp->fp, lfcbp->name, lfcbp->name);
    if (fn != NULL)
        printf("-- Opened %s load file: %s\n", (lfcbp->isNew ? "new" : "old"), lfcbp->name);

    return LDH__SUCCESS;
}

static pwr_tStatus
initDirectory(sLCB *m_lcbp)
{
  
    m_lcbp->sect[dbs_eSect_dir].version    = dbs_cVersionDirectory;
    m_lcbp->sect[dbs_eSect_volume].version = dbs_cVersionVolume;
    m_lcbp->sect[dbs_eSect_volref].version = dbs_cVersionVolRef;
    m_lcbp->sect[dbs_eSect_oix].version    = dbs_cVersionOix;
    m_lcbp->sect[dbs_eSect_object].version = dbs_cVersionObject;
    m_lcbp->sect[dbs_eSect_rbody].version  = dbs_cVersionRbody;
    m_lcbp->sect[dbs_eSect_name].version   = dbs_cVersionName;
    m_lcbp->sect[dbs_eSect_meta].version   = dbs_cVersionMeta;
    m_lcbp->sect[dbs_eSect_dbody].version  = dbs_cVersionDbody;
    m_lcbp->sect[dbs_eSect_fixup].version  = dbs_cVersionFixup;

    m_lcbp->sect[dbs_eSect_dir].type    = dbs_eSect_dir;
    m_lcbp->sect[dbs_eSect_volume].type = dbs_eSect_volume;
    m_lcbp->sect[dbs_eSect_volref].type = dbs_eSect_volref;
    m_lcbp->sect[dbs_eSect_oix].type    = dbs_eSect_oix;
    m_lcbp->sect[dbs_eSect_object].type = dbs_eSect_object;
    m_lcbp->sect[dbs_eSect_rbody].type  = dbs_eSect_rbody;
    m_lcbp->sect[dbs_eSect_name].type   = dbs_eSect_name;
    m_lcbp->sect[dbs_eSect_meta].type   = dbs_eSect_meta;
    m_lcbp->sect[dbs_eSect_dbody].type  = dbs_eSect_dbody;
    m_lcbp->sect[dbs_eSect_fixup].type  = dbs_eSect_fixup;

    m_lcbp->sect[dbs_eSect_dir].size = dbs_dAlign(sizeof(dbs_sSect)) * dbs_eSect_;
    m_lcbp->sect[dbs_eSect_volume].size = dbs_dAlign(sizeof(dbs_sVolume));

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectFile(sLCB *m_lcbp, sLFCB *lfcbp)
{
    dbs_sFile *f = (dbs_sFile*)calloc(1, dbs_dAlign(sizeof(*f)));
    PDR         pdrs;

//    if (ftell(lfcbp->fp) != lfcbp->sect[dbs_eSect_file].offset)
//        return LDH__FILEPOS;

    printf("ftell: %ld\n", ftell(lfcbp->fp));

    co_GetOwnFormat(&f->format);
    f->cookie = 550715339;
    //file.size;
    f->offset = dbs_dAlign(sizeof(*f));
    f->formatVersion = dbs_cVersionFormat;
    f->version = dbs_cVersionFile;
    f->sectVersion = dbs_cVersionDirectory;
    f->pwrVersion = 1;
    f->time = m_lcbp->volume.time;
    f->fileType = m_lcbp->file;  
    //file.userName;
    //file.comment;
    printf("format.......: %d\n", f->format.m);
    printf("cookie.......: %d\n", f->cookie);
    printf("size.........: %d\n", f->size);
    printf("offset.......: %d\n", f->offset);
    printf("formatVersion: %d\n", f->formatVersion);
    printf("version......: %d\n", f->version);
    printf("sectVersion..: %d\n", f->sectVersion);
    printf("pwrVersion...: %d\n", f->pwrVersion);
    printf("time.........: %ld\n", f->time.tv_sec);
    printf("fileType.....: %d\n", f->fileType);
    printf("userName.....: %s\n", f->userName);

    pdrmem_create(&pdrs, (char *) f, sizeof(*f), PDR_DECODE, f->format, f->format);
    if (!pdr_dbs_sFile(&pdrs, f))
        return LDH__XDR;

//    xdrmem_create(&xdrs, (char *) &file, sizeof(file), XDR_ENCODE);
//    if(!xdr_dbs_sFile(&xdrs, &file)) return LDH__XDR;
    if (fwrite(f, dbs_dAlign(sizeof(*f)) , 1, lfcbp->fp) < 1) return LDH__FILEWRITE;

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectDirectory(sLCB *m_lcbp, sLFCB *lfcbp)
{
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_dir].offset);
    
    if (pos != m_lcbp->sect[dbs_eSect_dir].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_dir].size == 0)
        return LDH__SUCCESS;
    
    if (fwrite(&m_lcbp->sect, sizeof(m_lcbp->sect), 1, lfcbp->fp) < 1)
        return LDH__FILEWRITE;

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectVolume(sLCB *m_lcbp, sLFCB *lfcbp)
{
    dbs_sVolume *v = (dbs_sVolume*)calloc(1, dbs_dAlign(sizeof(*v)));
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_volume].offset);

    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_volume].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_volume].size == 0)
        return LDH__SUCCESS;

    memcpy(v, &m_lcbp->volume, sizeof(m_lcbp->volume));
    
    printf("vid........: %d\n", v->vid);
    printf("name.......: %s\n", v->name);
    printf("cid........: %d\n", v->cid);
    printf("className..: %s\n", v->className);
    printf("time.......: %ld\n", v->time.tv_sec);
    printf("cardinality: %d\n", v->cardinality);
    printf("rbodysize..: %d\n", v->rbodySize);

    if (fwrite(v, dbs_dAlign(sizeof(m_lcbp->volume)), 1, lfcbp->fp) < 1)
        return LDH__FILEWRITE;

    return LDH__SUCCESS;
}    


static pwr_tStatus
writeSectVolref(sLCB *m_lcbp, sLFCB *lfcbp)
{
//    pwr_tStatus		    sts;
    dbs_sVolRef		    volref;
//    ldhi_sVidEntry	   *vtp;
//    ldhi_sObjBody	   *obp;
//    pwr_sClassVolume   *classVolume;
//    XDR			xdrs;
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_volref].offset);
  
    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_volref].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_volref].size == 0)
        return LDH__SUCCESS;
    
    memset(&volref, 0, sizeof(volref));

#if 0
    sts = OpenSect(lfcbp, dbs_eSect_volref);
    if (EVEN(sts)) return sts;
    for (
        vtp = (ldhi_sVidEntry *) tree_Minimum(&sts, m_lcbp->sp->wb->vidtab);
        vtp != NULL;
        vtp = (ldhi_sVidEntry *) tree_Successor(&sts, m_lcbp->sp->wb->vidtab, vtp)
        ) {
/* +++++++++ A workaround for Claes Sjöfors 1998-03-02 */
        /* if this is the pwrs-volume then
           we are not depending on any volume */

        if (m_lcbp->ohp->db.oid.vid == 1) continue;

        /* if this is a class volume then
           we depend only on pwrs */

        if (m_lcbp->ohp->db.cid == pwr_eClass_ClassVolume) {
            if (vtp->vhp->ohp->db.oid.vid != 1) continue;
        }
/* --------- End of workaround */

        if (vtp->vhp->ohp->db.cid == pwr_eClass_ClassVolume) {
            sts = ldhi_GetObjectBody(m_lcbp->sp, vtp->vhp->ohp, ldhi_eBid_SysBody, &obp);
            if (EVEN(sts)) return sts;
            classVolume = (pwr_sClassVolume *) obp->body;

            volref.vid  = vtp->vhp->ohp->db.oid.vid;
            volref.cid  = pwr_eClass_ClassVolume;
            volref.time = classVolume->RtVersion;

//            xdrmem_create(&xdrs, (char *) &VolRef, sizeof(VolRef), XDR_ENCODE);
//            if(!xdr_dbs_sVolRef(&xdrs, &VolRef)) return LDH__XDR;
            if (fwrite(&VolRef, sizeof(volRef), 1, lfcbp->fp) < 1) return LDH__FILEWRITE;
//            lfcbp->head.SectSize += sizeof(volRef);
        }
    }
#endif

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectOix(sLCB *m_lcbp, sLFCB *lfcbp)
{
    sOentry *oep;
    dbs_sOix *o = (dbs_sOix*)calloc(1, dbs_dAlign(sizeof(*o)));
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_oix].offset);
    
    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_oix].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_oix].size == 0)
        return LDH__SUCCESS;
    
    oep = (sOentry*)tree_Minimum(NULL, m_lcbp->oix_th);
    while (oep != NULL) {
        o->oix = oep->o.oid.oix;
        o->ref = oep->ref;
        
        if (fwrite(o, dbs_dAlign(sizeof(*o)), 1, lfcbp->fp) < 1)
            return LDH__FILEWRITE;

        oep = (sOentry*)tree_Successor(NULL, m_lcbp->oix_th, (tree_sNode*)oep);
    }

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectObject(sLCB *m_lcbp, sLFCB *lfcbp)
{
    sOentry *oep;
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_object].offset);
    
    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_object].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_object].size == 0)
        return LDH__SUCCESS;
    
    for (oep = m_lcbp->o_lh; oep != NULL; oep = oep->o_ll) {
        if (fwrite(&oep->o, dbs_dAlign(sizeof(oep->o)), 1, lfcbp->fp) < 1)
            return LDH__FILEWRITE;
    }

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectRbody(sLCB *m_lcbp, sLFCB *lfcbp)
{
    sOentry *oep;
    void    *p;
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_rbody].offset);

    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_rbody].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_rbody].size == 0)
        return LDH__SUCCESS;
    
    for (oep = m_lcbp->o_lh; oep != NULL; oep = oep->o_ll) {
        if (oep->rbody.size == 0)
            continue;
//        printf("o.rsize[%s]: %d\n", oep->o.name, oep->rbody.size);
        if (fwrite(&oep->rbody, sizeof(oep->rbody), 1, lfcbp->fp) < 1)
            return LDH__FILEWRITE;
        p = getRbody(m_lcbp, oep);
        /* @todo!!! objdid_self */
        if (fwrite(p, oep->rbody.size, 1, lfcbp->fp) < 1)
            return LDH__FILEWRITE;
    }

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectName(sLCB *m_lcbp, sLFCB *lfcbp)
{
    sNentry *nep;
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_name].offset);
    
    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_name].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_name].size == 0)
        return LDH__SUCCESS;
    
    nep = (sNentry*)tree_Minimum(NULL, m_lcbp->name_th);
    while (nep != NULL) {
        if (fwrite(&nep->n,  dbs_dAlign(sizeof(nep->n)), 1, lfcbp->fp) < 1)
            return LDH__FILEWRITE;
        
        nep = (sNentry*)tree_Successor(NULL, m_lcbp->name_th, (tree_sNode*)nep);
    }

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectMeta(sLCB *m_lcbp, sLFCB *lfcbp)
{
    sMentry *mep;
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_meta].offset);
    
    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_meta].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_meta].size == 0)
        return LDH__SUCCESS;
    
    mep = (sMentry*)tree_Minimum(NULL, m_lcbp->meta_th);
    while (mep != NULL) {
        if (fwrite(&mep->m,  dbs_dAlign(sizeof(mep->m)), 1, lfcbp->fp) < 1)
            return LDH__FILEWRITE;
        
        mep = (sMentry*)tree_Successor(NULL, m_lcbp->meta_th, (tree_sNode*)mep);
    }

    return LDH__SUCCESS;
}

static pwr_tStatus
writeSectDbody(sLCB *m_lcbp, sLFCB *lfcbp)
{
    sOentry   *oep;
    void      *p;
    int pos;
    
    pos = ftell(lfcbp->fp);
    
    printf("ftell: %d, offset: %d\n", pos, m_lcbp->sect[dbs_eSect_dbody].offset);

    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_dbody].offset)
        return LDH__FILEPOS;

    if (m_lcbp->sect[dbs_eSect_dbody].size == 0)
        return LDH__SUCCESS;
    
    for (oep = m_lcbp->o_lh; oep != NULL; oep = oep->o_ll) {
        if (oep->dbody.size == 0)
            continue;
        if (fwrite(&oep->dbody, sizeof(oep->dbody), 1, lfcbp->fp) < 1)
            return LDH__FILEWRITE;
        p = getDbody(m_lcbp, oep);
        /* @todo!!! objdid_self */
        if (fwrite(p, oep->dbody.size, 1, lfcbp->fp) < 1)
            return LDH__FILEWRITE;
    }

    return LDH__SUCCESS;
}

#if 0
pwr_tStatus
ldhld_CreateLoadFile(ldhi_sSession *sp) // Fix
{
    //pwr_tStatus   sts;
    sLCB          *m_lcbp;

    m_lcbp = alloc_lcb(sp);

    getVolume(m_lcbp);
    buildSnapshot(m_lcbp);

    if (m_lcbp->errors != 0) {
        if (m_lcbp->errors != 0) printf("** Errors: %d\n", m_lcbp->errors);
        goto error_handler;
    }

    createFile(m_lcbp);

    if (m_lcbp->warnings != 0) printf("!! Warnings: %d\n", m_lcbp->warnings);
    if (m_lcbp->errors != 0) {
        if (m_lcbp->errors != 0) printf("** Errors: %d\n", m_lcbp->errors);
        goto error_handler;
    }

    free_lcb(m_lcbp);
    
    return LDH__SUCCESS;

  error_handler:
    printf("-- Loadfiles cannot be created!\n");
    free_lcb(m_lcbp);
    return LDH__NOTLOAD;
}
#endif

wb_volume::createSnapshot()
{
    try {
        initSnapshot();
        iterObject(installObject);
        iterBody(installBody);
        allocateSections();
        createFile();
    }
    catch (wb_exception e) {
    }    
}


/* @todo!!!
   Calculate sect.offset

*/


#if 0
static pwr_tStatus    closeSect(sLFCB *lfcbp);
static pwr_tStatus    findSect(sLFCB *lfcbp, dbs_eSect SectType, pwr_tVersion SectVersion);
static pwr_sClassDef *getClassDef(sLCB *m_lcbp, ldhi_sObjHead *o);
static pwr_tStatus    getFirstAttribute(sLCB *m_lcbp, ldhi_sObjHead *o, ldhi_sObjHead **ahp);

static pwr_sClassDef *
getClassDef(sLCB *m_lcbp, ldhi_sObjHead *o)
{
    pwr_tStatus    sts;
    pwr_sClassDef *classDef;
    ldhi_sObjBody *obp;

    sts = ldhi_GetObjectBody(m_lcbp->sp, o->chp, ldhi_eBid_SysBody, &obp);
    classDef = (pwr_sClassDef*)obp->body;
    return classDef;
}

static pwr_tStatus
getFirstAttribute(sLCB *m_lcbp, ldhi_sObjHead *o, ldhi_sObjHead **ahp)
{
    pwr_tStatus    sts;
    ldhi_sObjHead *obhp;
    ldhi_sObjName  nn;


    if (!o->db.flags.f.RtBody) {
        /* Object has no body. */
        *ahp = NULL;
    } else if (o->db.flags.f.System) {
        sts = ldhi_SetNormObjName(&nn, "SysBody");
        sts = ldhi_FindSiblingByName(m_lcbp->sp, o->chp->chhp, &nn, &obhp);
        *ahp = obhp->chhp;
    } else {
        sts = ldhi_SetNormObjName(&nn, "RtBody");
        sts = ldhi_FindSiblingByName(m_lcbp->sp, o->chp->chhp, &nn, &obhp);
        if (EVEN(sts)) {
            /* No RtBody => Only DevBody. */
            *ahp = NULL;
        } else {
            *ahp = obhp->chhp;
        }
    }
    return LDH__SUCCESS;
}


static pwr_tStatus
closeSect(sLFCB *lfcbp)
{
    fpos_t    pos;
    dbs_sHead head;
    XDR       xdrs;
  
    if (fgetpos(lfcbp->fp, &pos)) return LDH__FILEPOS;
    if (fsetpos(lfcbp->fp, &lfcbp->sectPos)) return LDH__FILEPOS;
    xdrmem_create(&xdrs, (char *) &head, sizeof(head), XDR_ENCODE);
    if(!xdr_dbs_sHead(&xdrs, &lfcbp->head)) return LDH__XDR;
    if (fwrite(&head, sizeof(head), 1, lfcbp->fp) < 1) return LDH__FILEWRITE;
    if (fsetpos(lfcbp->fp, &pos)) return LDH__FILEPOS;

    return LDH__SUCCESS;
}

static pwr_tStatus
findSect(sLFCB *lfcbp, dbs_eSect SectType, pwr_tVersion SectVersion)
{
    XDR xdrs;
  
    if (fseek(lfcbp->fp, 0, 0) != 0)
        return LDH__FILEPOS;
  
    for (;;) {
        if (fread(&lfcbp->head, sizeof(lfcbp->head), 1, lfcbp->fp) < 1) return LDH__FILEREAD;
        xdrmem_create(&xdrs, (char *) &lfcbp->head, sizeof(lfcbp->head), XDR_DECODE);
        if(!xdr_dbs_sHead(&xdrs, &lfcbp->head)) return LDH__XDR;
        if (lfcbp->head.SectType == SectType) {
            if (lfcbp->head.HeadVersion != dbs_cVersionHead)
                return LDH__BADFILE;
            if (lfcbp->head.SectVersion != SectVersion)
                return LDH__BADFILE;
            return LDH__SUCCESS;
        }
        if (fseek(lfcbp->fp, lfcbp->head.SectSize, 1) != 0) return LDH__FILEPOS;
    }

    return LDH__SUCCESS;
}


#endif

#if 0
static pwr_tStatus
writeSectVolume(sLCB *m_lcbp, sLFCB *lfcbp, pwr_tBoolean new)
{
    pwr_tStatus		sts;
    dbs_sVolume		volume;
    fpos_t		CurPos;
//    XDR			xdrs;
  
    if (ftell(lfcbp->fp) != m_lcbp->sect[dbs_eSect_volume].offset)
        return LDH__FILEPOS;

    memset(&volume, 0, sizeof(volume));

    if (new) {
//        sts = OpenSect(lfcbp, dbs_eSect_Volume);
//        if (EVEN(sts)) return sts;
        if (fgetpos(lfcbp->fp, &lfcbp->identSectPos)) return LDH__FILEPOS;
    } else {
        if (fgetpos(lfcbp->fp, &CurPos)) return LDH__FILEPOS;
        if (fsetpos(lfcbp->fp, &lfcbp->identSectPos)) return LDH__FILEPOS;
    }

    volume.id = m_lcbp->vid;
    strcpy(volume.name, m_lcbp->o->name());
    volume.cid = m_lcbp->cid;
    @todo strcpy(volume.className, m_lcbp->ohp->chp->db.name.data);
    volume.version = m_lcbp->version;
    volume.creTime = m_lcbp->time;
    volume.cardinality = m_lcbp->cardinality;
    volume.bodySize = m_lcbp->bodySize;
#if 0
    volume.Volume = *m_lcbp->Volume;
#endif

//    xdrmem_create(&xdrs, (char *) &Volume, sizeof(Volume), XDR_ENCODE);
//    if(!xdr_dbs_sVolume(&xdrs, &Volume)) return LDH__XDR;
    if (fwrite(&Volume, sizeof(Volume), 1, lfcbp->fp) < 1) return LDH__FILEWRITE;
    lfcbp->head.SectSize = sizeof(Volume);

    if (new) {
        sts = closeSect(lfcbp);
        if (EVEN(sts)) return sts;
    } else {
        if (fsetpos(lfcbp->fp, &CurPos)) return LDH__FILEPOS;
    }

    return LDH__SUCCESS;
}
#endif
#endif
