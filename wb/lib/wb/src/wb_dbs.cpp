/* wb_ldhdbs.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

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
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_tree.h"
#include "co_platform.h"
#include "co_pdr.h"
#include "co_dbs.h"
#include "pwr_class.h"
#include "wb_dbs.h"
#include "wb_ldh_msg.h"
#include "wb_ldhld.h"
#include "wb_ldh.h"
#include "rt_dlinux.h"
#include "wb_object.h"
#include "wb_volume.h"
#include "wb_error.h"

static int comp_dbs_name(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);

wb_dbs::wb_dbs(wb_vrep *v)
{
    pwr_tStatus sts;

    clock_gettime(CLOCK_REALTIME, &m_volume.time);

    m_v = v;
    
    m_volume.vid = m_v->vid();
    strcpy(m_volume.name, m_v->name());
    m_volume.cid = m_v->cid();

    //@todo strcpy(m_volume.className, m_ohp->chp->db.name.data);
 
    sprintf(m_name, dbs_cNameVolume, dbs_cDirectory, m_v->name());
    dcli_translate_filename(m_name, m_name);

    m_oix_th = tree_CreateTable(&sts, sizeof(pwr_tOix), offsetof(sOentry, o.oid.oix),
                                sizeof(sOentry), 1000, tree_Comp_oix);

    m_name_th = tree_CreateTable(&sts, sizeof(dbs_sName), offsetof(sNentry, n.poix),
                                 sizeof(sNentry), 1000, comp_dbs_name);

    m_class_th = tree_CreateTable(&sts, sizeof(pwr_tOid), offsetof(sCentry, c),
                                  sizeof(sCentry), 1000, tree_Comp_oid);
}

wb_dbs::~wb_dbs()
{
    pwr_tStatus sts;
    
    tree_DeleteTable(&sts, m_oix_th);
    tree_DeleteTable(&sts, m_name_th);
    tree_DeleteTable(&sts, m_class_th);
}


static int
comp_dbs_name(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y)
{
    sNentry *xKey = (sNentry *) (tp->keyOffset + (char *) x);
    sNentry *yKey = (sNentry *) (tp->keyOffset + (char *) y);
    int comp;
    

    if (xKey->n.poix < yKey->n.poix)
        comp = -1;
    else if (xKey->n.poix == yKey->n.poix)
        comp = strcmp(xKey->n.normname, yKey->n.normname);
    else
        comp = 1;

//    printf("%d:%s %c %d:%s\n", xKey->poix, xKey->normname, (comp == 1 ? '>' : (comp == -1 ? '<' : '=')), yKey->poix, yKey->normname);

    return comp;
}

void
wb_dbs::buildFile()
{
    m_v->iterObject(this);
    buildSectName();
    checkObject(m_oep);
    buildSectClass();
    createFile();
}

void
wb_dbs::checkObject(sOentry *oep)
{
    int nChild = 0;
    pwr_tStatus sts;
    dbs_sQlink *sib_lh;
    dbs_sQlink *sib_ll;
    sOentry *sep;
    sNentry ne;
    sNentry *nep;
    

    // Check object
    if (!oep->flags.b.exist) {
        printf("Object does not exist!\n");
    } else {
        m_volume.cardinality++;
    }

    oep->ref = dbs_dMakeRef(dbs_eSect_object, m_sect[dbs_eSect_object].size);
    m_sect[dbs_eSect_object].size += dbs_dAlign(sizeof(dbs_sObject));
    dbs_Qinit(&sts, &oep->o.sib_lh, oep->ref + offsetof(dbs_sObject, sib_lh));
    dbs_Qinit(&sts, &oep->o.sib_ll, oep->ref + offsetof(dbs_sObject, sib_ll));
    dbs_Qinit(&sts, &oep->o.o_ll, oep->ref + offsetof(dbs_sObject, o_ll));

    // Check all children

    sib_lh = sib_ll = &oep->o.sib_lh;
    for (sep = oep->foep; sep != 0; sep = sep->aoep) {
        if (sep->poep != oep) {
            printf("Object not linked to right parent!\n");
        }

        checkObject(sep);

        sep->o.pref = oep->ref;
        dbs_Qinsert(&sts, sib_ll, &sep->o.sib_ll, sib_lh);
        sib_ll = &sep->o.sib_ll;
        oep->o.flags.b.isMountClean |= sep->o.flags.b.isMountClean && !sep->o.flags.b.isMountClient;
        nChild++;
    }

    /* Check name table, get reference to first and last child in name order.  */
    memset(&ne, 0, sizeof(ne));
    ne.n.poix = oep->o.oid.oix;
    nep = (sNentry*)tree_FindSuccessor(&sts, m_name_th, &ne);
    if (nep != NULL && nep->n.poix == oep->o.oid.oix) {
        oep->o.name_bt.start = nep->n.ref;
    }

    ne.n.poix += 1;
    nep = (sNentry*)tree_FindPredecessor(&sts, m_name_th, &ne);
    if (nep != NULL && nep->n.poix == oep->o.oid.oix) {
        oep->o.name_bt.end = nep->n.ref;
    }
}

pwr_tStatus
wb_dbs::installObject(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid, pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid,
                      pwr_tOid loid, pwr_tObjName name, pwr_tObjName normname, pwr_tTime time,
                      pwr_tTime rbTime, pwr_tTime dbTime, size_t rbSize, size_t dbSize)
{
    sNentry      ne;
    sNentry     *nep;
    sOentry     *oep;
    pwr_tStatus  sts;
    
    oep = (sOentry *)tree_Insert(&sts, m_oix_th, &oid.oix);
    if (sts == TREE__INSERTED) {
    } else {
        printf("Error: object is already inserted!\n");
    }
    
    if (oid.oix == pwr_cNOix) {
        // this is the volume object
        m_oep = oep;
    }
    
    oep->flags.b.exist = 1;

    // Link objects to its relatives
    oep->poep = (sOentry *)tree_Insert(&sts, m_oix_th, &poid.oix);
    oep->boep = (sOentry *)tree_Insert(&sts, m_oix_th, &boid.oix);
    oep->aoep = (sOentry *)tree_Insert(&sts, m_oix_th, &aoid.oix);
    oep->foep = (sOentry *)tree_Insert(&sts, m_oix_th, &foid.oix);
    oep->loep = (sOentry *)tree_Insert(&sts, m_oix_th, &loid.oix);

    m_volume.cardinality++;    

#if 0
    if (fep == NULL)
        oep->flags.b.devOnly = o.isDevOnly();
    else
        oep->flags.b.devOnly = fep->flags.b.devOnly || o.isDevOnly();
#endif

    oep->o.oid  = oid;
    oep->o.cid  = cid;
    oep->o.poid = poid;
    strcpy(oep->o.name, name);
    strcpy(oep->o.normname, normname);

    oep->o.time = time;
    
    /* insert into name table */
    memset(&ne, 0, sizeof(ne));
    ne.n.poix = poid.oix;
    strcpy(ne.n.normname, normname);
    nep = (sNentry*)tree_Insert(&sts, m_name_th, &ne);
    nep->oep = oep;
    
    classInsert(oep);

    oep->o.rbody.time = rbTime;
    oep->o.rbody.size = rbSize;
    if (rbSize > 0) {
        oep->rbody.oid = oid;
        oep->rbody.size = dbs_dAlign(rbSize);
        m_volume.rbodySize += oep->rbody.size;
    }

    oep->o.dbody.time = dbTime;
    oep->o.dbody.size = dbSize;
    if (oep->o.dbody.size > 0) {
        oep->dbody.oid = oid;
        oep->dbody.size = dbs_dAlign(oep->o.dbody.size);
    }

    return LDH__SUCCESS;
}


pwr_tStatus
wb_dbs::closeFile(pwr_tBoolean doDelete)
{

    if (m_fp != NULL)
        fclose(m_fp);
    if (doDelete && *m_name != '\0') {
        if (remove(m_name) == 0)
            printf("-- Deleted file: %s\n", m_name);
    }

    return LDH__SUCCESS;
}

pwr_tBoolean
wb_dbs::createFile()
{
    pwr_tStatus sts;
    int size;
    
    printf("\n-- Working with load file volume '%s'...\n", m_v->name());

    printf("-- Open file...\n");
    sts = openFile();
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing file section...\n");
    sts = writeSectFile();
    if (EVEN(sts)) goto error_handler;

    size = dbs_dAlign(sizeof(dbs_sFile));

    m_sect[dbs_eSect_dir].size = sizeof(m_sect);
    m_sect[dbs_eSect_dir].offset = size;
    size += m_sect[dbs_eSect_dir].size;
    
    m_sect[dbs_eSect_volume].size = dbs_dAlign(sizeof(dbs_sVolume));
    m_sect[dbs_eSect_volume].offset = size;

    printf("-- Writing volume section...\n");
    sts = writeSectVolume();
    if (EVEN(sts)) goto error_handler;

    size += m_sect[dbs_eSect_volume].size;
    m_sect[dbs_eSect_volref].offset = size;

    printf("-- Writing volref section...\n");
    sts = writeSectVolref();
    if (EVEN(sts)) goto error_handler;

    size += m_sect[dbs_eSect_volref].size;
    m_sect[dbs_eSect_oix].offset = size;

    printf("-- Writing oix section...\n");
    sts = writeSectOix();
    if (EVEN(sts)) goto error_handler;

    size += m_sect[dbs_eSect_oix].size;
    m_sect[dbs_eSect_rbody].offset = size;

    printf("-- Writing rbody section...\n");
    sts = writeSectRbody();
    if (EVEN(sts)) goto error_handler;

    size += m_sect[dbs_eSect_rbody].size;
    m_sect[dbs_eSect_dbody].offset = size;

    printf("-- Writing dbody section...\n");
    sts = writeSectDbody();
    if (EVEN(sts)) goto error_handler;

    size += m_sect[dbs_eSect_dbody].size;
    m_sect[dbs_eSect_object].offset = size;

    printf("-- Writing object section...\n");
    sts = writeSectObject();
    if (EVEN(sts)) goto error_handler;

    size += m_sect[dbs_eSect_object].size;
    m_sect[dbs_eSect_name].offset = size;

    printf("-- Writing name section...\n");
    sts = writeSectName();
    if (EVEN(sts)) goto error_handler;

    size += m_sect[dbs_eSect_name].size;
    m_sect[dbs_eSect_class].offset = size;

    printf("-- Writing class section...\n");
    sts = writeSectClass();
    if (EVEN(sts)) goto error_handler;

    printf("-- Writing directory section...\n");
    sts = writeSectDirectory();
    if (EVEN(sts)) goto error_handler;

    printf("-- Close file...\n");
    sts = closeFile(0);
    if (EVEN(sts)) goto error_handler;

    printf("-- Successfully created load file for volume '%s'\n", m_v->name());
    printf("-- %d objects with a total body size of %d bytes were written to new file.\n",
           m_volume.cardinality, m_volume.rbodySize);

    return 1;

  error_handler:
    /* Clean up the mess.  */
    printf("-- Load file was not created for volume '%s'\n", m_v->name());
    closeFile(1);
    return 0;
}

pwr_tStatus
wb_dbs::openFile()
{
    char *fn;

    cdh_ToLower(m_name, m_name);
    
    m_fp = fopen(m_name, "w+b");
    if (m_fp == NULL) {
        printf("** Cannot open file: %s\n", m_name);
        perror("   Reason");
        return LDH__FILEOPEN;
    }
    fn = dcli_fgetname(m_fp, m_name, m_name);
    if (fn != NULL)
        printf("-- Opened load file: %s\n", m_name);

    return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectFile()
{
    char f[dbs_dAlign(sizeof(dbs_sFile))];
    dbs_sFile *fp = (dbs_sFile*)f;
    PDR pdrs;

    co_GetOwnFormat(&fp->format);
    fp->cookie = 550715339;
    //file.size;
    fp->offset = dbs_dAlign(sizeof(*fp));
    fp->formatVersion = dbs_cVersionFormat;
    fp->version = dbs_cVersionFile;
    fp->sectVersion = dbs_cVersionDirectory;
    fp->pwrVersion = 1;
    fp->time = m_volume.time;
    fp->fileType = dbs_eFile_volume;

    //file.userName;
    //file.comment;
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

    pdrmem_create(&pdrs, (char *) fp, sizeof(*fp), PDR_DECODE, fp->format, fp->format);
    if (!pdr_dbs_sFile(&pdrs, fp))
        return LDH__XDR;

    if (fwrite(f, sizeof(f) , 1, m_fp) < 1) return LDH__FILEWRITE;

    return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectDirectory()
{
    
    m_sect[dbs_eSect_dir].version    = dbs_cVersionDirectory;
    m_sect[dbs_eSect_volume].version = dbs_cVersionVolume;
    m_sect[dbs_eSect_volref].version = dbs_cVersionVolRef;
    m_sect[dbs_eSect_oix].version    = dbs_cVersionOix;
    m_sect[dbs_eSect_object].version = dbs_cVersionObject;
    m_sect[dbs_eSect_rbody].version  = dbs_cVersionRbody;
    m_sect[dbs_eSect_name].version   = dbs_cVersionName;
    m_sect[dbs_eSect_class].version  = dbs_cVersionClass;
    m_sect[dbs_eSect_dbody].version  = dbs_cVersionDbody;

    m_sect[dbs_eSect_dir].type    = dbs_eSect_dir;
    m_sect[dbs_eSect_volume].type = dbs_eSect_volume;
    m_sect[dbs_eSect_volref].type = dbs_eSect_volref;
    m_sect[dbs_eSect_oix].type    = dbs_eSect_oix;
    m_sect[dbs_eSect_object].type = dbs_eSect_object;
    m_sect[dbs_eSect_rbody].type  = dbs_eSect_rbody;
    m_sect[dbs_eSect_name].type   = dbs_eSect_name;
    m_sect[dbs_eSect_class].type  = dbs_eSect_class;
    m_sect[dbs_eSect_dbody].type  = dbs_eSect_dbody;

    if (m_sect[dbs_eSect_dir].size == 0)
        return LDH__SUCCESS;
    
    if (fseek(m_fp, m_sect[dbs_eSect_dir].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

    if (fwrite(&m_sect, sizeof(m_sect), 1, m_fp) < 1)
        return LDH__FILEWRITE;

    return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectVolume()
{
    char v[dbs_dAlign(sizeof(dbs_sVolume))];
    dbs_sVolume *vp = (dbs_sVolume *)v;


    if (m_sect[dbs_eSect_volume].size == 0)
        return LDH__SUCCESS;

    if (fseek(m_fp, m_sect[dbs_eSect_volume].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

    memset(v, 0, sizeof(v));
    memcpy(v, &m_volume, sizeof(m_volume));
    
    printf("vid........: %d\n", vp->vid);
    printf("name.......: %s\n", vp->name);
    printf("cid........: %d\n", vp->cid);
    printf("className..: %s\n", vp->className);
    printf("time.......: %ld\n", vp->time.tv_sec);
    printf("cardinality: %d\n", vp->cardinality);
    printf("rbodysize..: %d\n", vp->rbodySize);

    if (fwrite(v, sizeof(v), 1, m_fp) < 1)
        return LDH__FILEWRITE;

    return LDH__SUCCESS;
}    


pwr_tStatus
wb_dbs::writeSectVolref()
{
    //dbs_sVolRef		    volref;
    
    if (m_sect[dbs_eSect_volref].size == 0)
        return LDH__SUCCESS;
    
    if (fseek(m_fp, m_sect[dbs_eSect_volref].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

#if 0
    memset(&volref, 0, sizeof(volref));



    memset(&ce, 0, sizeof(ce));
    ce.c.cid = 0;
    cep = (sCentry*)tree_FindSuccessor(&sts, m_class_th, &ce);
    while (cep) {
        cdh_uTypeId cid;
        cdh_uVolumeId vid;
        
        cid.pwr = cep->c.cid;
        vid.pwr = pwr_cNvid;
        vid.v.vid_0 = cid.c.vid_0;
        vid.v.vid_1 = cid.c.vid_1;
        
        vp->vid  = vid.pwr;
        vp->cid  = pwr_eClass_ClassVolume;
        //vp->time = ?;

        
    }
    
    if (nep != NULL && nep->n.poix == oep->o.oid.oix) {
        oep->o.name_bt.start = nep->n.ref;
    }

    ce.c.cid = 0;
    nep = (sNentry*)tree_FindPredecessor(&sts, m_name_th, &ne);
    if (nep != NULL && nep->n.poix == oep->o.oid.oix) {
        oep->o.name_bt.end = nep->n.ref;
    }
#endif
#if 0
    sts = OpenSect(dbs_eSect_volref);
    if (EVEN(sts)) return sts;
    for (
        vtp = (ldhi_sVidEntry *) tree_Minimum(&sts, m_sp->wb->vidtab);
        vtp != NULL;
        vtp = (ldhi_sVidEntry *) tree_Successor(&sts, m_sp->wb->vidtab, vtp)
        ) {
/* +++++++++ A workaround for Claes Sjöfors 1998-03-02 */
        /* if this is the pwrs-volume then
           we are not depending on any volume */

        if (m_ohp->db.oid.vid == 1) continue;

        /* if this is a class volume then
           we depend only on pwrs */

        if (m_ohp->db.cid == pwr_eClass_ClassVolume) {
            if (vtp->vhp->ohp->db.oid.vid != 1) continue;
        }
/* --------- End of workaround */

        if (vtp->vhp->ohp->db.cid == pwr_eClass_ClassVolume) {
            sts = ldhi_GetObjectBody(m_sp, vtp->vhp->ohp, ldhi_eBid_SysBody, &obp);
            if (EVEN(sts)) return sts;
            classVolume = (pwr_sClassVolume *) obp->body;

            volref.vid  = vtp->vhp->ohp->db.oid.vid;
            volref.cid  = pwr_eClass_ClassVolume;
            volref.time = classVolume->RtVersion;

//            xdrmem_create(&xdrs, (char *) &VolRef, sizeof(VolRef), XDR_ENCODE);
//            if(!xdr_dbs_sVolRef(&xdrs, &VolRef)) return LDH__XDR;
            if (fwrite(&VolRef, sizeof(volRef), 1, m_fp) < 1) return LDH__FILEWRITE;
//            m_head.SectSize += sizeof(volRef);
        }
    }
#endif

    return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectOix()
{
    sOentry  *oep;
    char     o[dbs_dAlign(sizeof(dbs_sOix))];
    dbs_sOix *op = (dbs_sOix*)o;
    
    if (m_sect[dbs_eSect_oix].size == 0)
        return LDH__SUCCESS;
    
    if (fseek(m_fp, m_sect[dbs_eSect_oix].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

    memset(o, 0, sizeof(o));
    
    oep = (sOentry*)tree_Minimum(NULL, m_oix_th);
    while (oep != NULL) {
        op->oix = oep->o.oid.oix;
        op->ref = oep->ref;
        
        if (fwrite(o, sizeof(o), 1, m_fp) < 1)
            return LDH__FILEWRITE;

        oep = (sOentry*)tree_Successor(NULL, m_oix_th, (tree_sNode*)oep);
    }
    
    return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectObject()
{
    sOentry *oep;
    
    if (m_sect[dbs_eSect_object].size == 0)
        return LDH__SUCCESS;
    
    if (fseek(m_fp, m_sect[dbs_eSect_object].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

    // @todo change to tree-order
    oep = (sOentry*)tree_Minimum(NULL, m_oix_th);
    while (oep != NULL) {
        //printf("o.rsize[%s]: %d\n", oep->o.name, oep->rbody.size);
        if (fwrite(&oep->o, dbs_dAlign(sizeof(oep->o)), 1, m_fp) < 1)
            return LDH__FILEWRITE;

        oep = (sOentry*)tree_Successor(NULL, m_oix_th, (tree_sNode*)oep);
    }

    return LDH__SUCCESS;
}


pwr_tStatus
wb_dbs::installDbody(pwr_tOid oid, void *body)
{
    pwr_tStatus sts;
    sOentry *oep;
    char b[dbs_dAlign(sizeof(dbs_sBody))];
    
    memset(b, 0, sizeof(b));
    
    oep = (sOentry *)tree_Find(&sts, m_oix_th, &oid.oix);
    if (EVEN(sts)) {
    }

    oep->o.dbody.ref = dbs_dMakeRef(dbs_eSect_dbody, m_sect[dbs_eSect_dbody].size + dbs_dAlign(sizeof(dbs_sBody)));
    m_sect[dbs_eSect_dbody].size += oep->dbody.size + sizeof(b);

    memcpy(b, &oep->dbody, sizeof(oep->dbody));
    
    if (fwrite(b, sizeof(b), 1, m_fp) < 1)
        return LDH__FILEWRITE;

    /* @todo!!! objdid_self */

    if (fwrite(body, oep->dbody.size, 1, m_fp) < 1)
        return LDH__FILEWRITE;

    return sts;
}


pwr_tStatus
wb_dbs::writeSectDbody()
{

    if (m_sect[dbs_eSect_dbody].size == 0)
        return LDH__SUCCESS;    

    if (fseek(m_fp, m_sect[dbs_eSect_dbody].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

    m_v->iterDbody(this);
    
    return LDH__SUCCESS;
}


pwr_tStatus
wb_dbs::installRbody(pwr_tOid oid, void *body)
{
    pwr_tStatus sts;
    sOentry *oep;
    char b[dbs_dAlign(sizeof(dbs_sBody))];
    
    memset(b, 0, sizeof(b));
    
    oep = (sOentry *)tree_Find(&sts, m_oix_th, &oid.oix);
    if (EVEN(sts)) {
    }

    oep->o.rbody.ref = dbs_dMakeRef(dbs_eSect_rbody, m_sect[dbs_eSect_rbody].size + dbs_dAlign(sizeof(dbs_sBody)));
    m_sect[dbs_eSect_rbody].size += oep->rbody.size + sizeof(b);

    memcpy(b, &oep->rbody, sizeof(oep->rbody));
    
    if (fwrite(b, sizeof(b), 1, m_fp) < 1)
        return LDH__FILEWRITE;

    /* @todo!!! objdid_self */

    if (fwrite(body, oep->rbody.size, 1, m_fp) < 1)
        return LDH__FILEWRITE;

    return sts;
}


pwr_tStatus
wb_dbs::writeSectRbody()
{

    if (m_sect[dbs_eSect_rbody].size == 0)
        return LDH__SUCCESS;    

    if (fseek(m_fp, m_sect[dbs_eSect_rbody].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

    m_v->iterRbody(this);
    
    return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectName()
{
    sNentry *nep;
    
    if (m_sect[dbs_eSect_name].size == 0)
        return LDH__SUCCESS;    

    if (fseek(m_fp, m_sect[dbs_eSect_name].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

    nep = (sNentry*)tree_Minimum(NULL, m_name_th);
    while (nep != NULL) {
        if (fwrite(&nep->n,  dbs_dAlign(sizeof(nep->n)), 1, m_fp) < 1)
            return LDH__FILEWRITE;
        
        nep = (sNentry*)tree_Successor(NULL, m_name_th, (tree_sNode*)nep);
    }

    return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectClass()
{
    sCentry *cep;

    
    if (m_sect[dbs_eSect_class].size == 0)
        return LDH__SUCCESS;
    
    if (fseek(m_fp, m_sect[dbs_eSect_class].offset, SEEK_SET) != 0)
        return LDH__FILEPOS;

    cep = (sCentry*)tree_Minimum(NULL, m_class_th);
    while (cep != NULL) {
        if (fwrite(&cep->c,  dbs_dAlign(sizeof(cep->c)), 1, m_fp) < 1)
            return LDH__FILEWRITE;
        
        cep = (sCentry*)tree_Successor(NULL, m_class_th, (tree_sNode*)cep);
    }

    return LDH__SUCCESS;
}

void
wb_dbs::buildSectName()
{
    sNentry *nep;
    
    /* allocate space for all name entries in name section*/

    nep = (sNentry*)tree_Minimum(NULL, m_name_th);
    while (nep != NULL) {
        nep->n.ref = dbs_dMakeRef(dbs_eSect_name, m_sect[dbs_eSect_name].size);
        m_sect[dbs_eSect_name].size += dbs_dAlign(sizeof(dbs_sName));

        nep = (sNentry*)tree_Successor(NULL, m_name_th, (tree_sNode*)nep);
    }
}

void
wb_dbs::buildSectClass()
{
    pwr_tStatus sts;
    sCentry *cep;
    dbs_tRef ref;
    dbs_sQlink *o_lh;
    dbs_sQlink *o_ll;
    sOentry *oep;
    
    /* allocate space for all name entries in name section*/

    cep = (sCentry*)tree_Minimum(NULL, m_class_th);
    while (cep != NULL) {
        ref = dbs_dMakeRef(dbs_eSect_class, m_sect[dbs_eSect_class].size);
        m_sect[dbs_eSect_class].size += dbs_dAlign(sizeof(dbs_sClass));
        dbs_Qinit(&sts, &cep->c.o_lh, ref + offsetof(dbs_sClass, o_lh));

        // Link all object instances to this class
        o_lh = o_ll = &cep->c.o_lh;
        for (oep = cep->o_lh; oep != 0; oep = oep->o_ll) {
            dbs_Qinsert(&sts, o_ll, &oep->o.o_ll, o_lh);
            o_ll = &oep->o.o_ll;
        }

        cep = (sCentry*)tree_Successor(NULL, m_class_th, (tree_sNode*)cep);        
    }
}

void
wb_dbs::classInsert(sOentry *oep)
{
    pwr_tStatus  sts;
    sCentry     *cep;

    cep = (sCentry*)tree_Insert(&sts, m_class_th, &oep->o.cid);
    if (sts == TREE__INSERTED) {
        /* was inserted now */
        cep->o_lh = cep->o_lt = oep;
    } else {
        /* was allready present */
        cep->o_lt->o_ll = oep;
        cep->o_lt = oep;
    }    
}
