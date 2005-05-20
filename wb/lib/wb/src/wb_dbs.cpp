/* wb_ldhdbs.c -- <short description>

PROVIEW/R
Copyright (C) 1996 by Comator Process AB.

This module contains functions to create database snapshot files.  */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pwr.h"
#include "pwr_class.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_tree.h"
#include "co_pdr.h"
#include "co_dbs.h"
#include "co_errno.h"
#include "wb_dbs.h"
#include "wb_vrep.h"
#include "wb_mvrep.h"
#include "wb_merep.h"

static int comp_dbs_name(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);

wb_dbs::wb_dbs(wb_vrep *v) :
  m_oid(pwr_cNOid), m_warnings(0), m_errors(0),
  m_nObjects(0), m_nTreeObjects(0), m_nClassObjects(0),
  m_nNameObjects(0), m_nRbodyObjects(0), m_nDbodyObjects(0), m_oep(0)
{
  pwr_tStatus sts;

  memset(m_fileName, 0, sizeof(m_fileName));
  memset(&m_volume, 0, sizeof(m_volume));
  memset(m_sect, 0, sizeof(m_sect));
    

  clock_gettime(CLOCK_REALTIME, &m_volume.time);

  m_v = v;
    
  m_volume.vid = m_v->vid();
  strcpy(m_volume.name, m_v->name());
  m_volume.cid = m_v->cid();

  //@todo strcpy(m_volume.className, m_ohp->chp->db.name.data);
 
  sprintf(m_fileName, dbs_cNameVolume, dbs_cDirectory, m_v->name());
  dcli_translate_filename(m_fileName, m_fileName);

  m_oid_th = tree_CreateTable(&sts, sizeof(pwr_tOid), offsetof(sOentry, o.oid),
                              sizeof(sOentry), 1000, tree_Comp_oid);

  m_name_th = tree_CreateTable(&sts, sizeof(dbs_sName), offsetof(sNentry, n),
                               sizeof(sNentry), 1000, comp_dbs_name);

  m_class_th = tree_CreateTable(&sts, sizeof(pwr_tCid), offsetof(sCentry, c),
                                sizeof(sCentry), 1000, tree_Comp_cid);

  m_vol_th = tree_CreateTable(&sts, sizeof(pwr_tVid), offsetof(sVentry, v.vid),
                                sizeof(sVentry), 10, tree_Comp_vid);
}

wb_dbs::~wb_dbs()
{
  pwr_tStatus sts;    
    
  printf("-- m_nObjects: %d\n", m_nObjects);
  printf("-- m_nTreeObjects: %d\n", m_nTreeObjects);
  printf("-- m_nClassObjects: %d\n", m_nClassObjects);
  printf("-- m_nNameObjects: %d\n", m_nNameObjects);
  printf("-- m_nRbodyObjects: %d\n", m_nRbodyObjects);
  printf("-- m_nDbodyObjects: %d\n", m_nDbodyObjects);

  tree_DeleteTable(&sts, m_oid_th);
  tree_DeleteTable(&sts, m_name_th);
  tree_DeleteTable(&sts, m_class_th);
}

void
wb_dbs::setFileName(const char *name)
{
  dcli_translate_filename(m_fileName, name);
}

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

  //printf("%d:%s %c %d:%s\n", xKey->poix, xKey->normname, (comp == 1 ? '>' : (comp == -1 ? '<' : '=')), yKey->poix, yKey->normname);

  return comp;
}

bool
wb_dbs::importVolume(wb_export &e)
{

  e.exportHead(*this);
  buildSectName();
  checkObject(m_oep);
  buildSectOid();
  buildSectClass();
  createFile();

  return true;
}

void
wb_dbs::checkObject(sOentry *oep)
{
  int nChild = 0;
  pwr_tStatus sts;
  dbs_sQlink *sib_lh;
  dbs_sQlink *sib_ll;
  sOentry *sep;
  dbs_sName n;
  sNentry *nep;
    

  // Check object
  if (!oep->flags.b.exist) {
    printf("** Object does not exist!\n");
  }

  m_nTreeObjects++;

  
  //printf("tree: %s\n", oep->o.name);
    
  oep->ref = dbs_dMakeRef(dbs_eSect_object, m_sect[dbs_eSect_object].size);
  m_sect[dbs_eSect_object].size += dbs_dAlign(sizeof(dbs_sObject));
  dbs_Qinit(&sts, &oep->o.sib_lh, oep->ref + offsetof(dbs_sObject, sib_lh));
  dbs_Qinit(&sts, &oep->o.sib_ll, oep->ref + offsetof(dbs_sObject, sib_ll));
  dbs_Qinit(&sts, &oep->o.o_ll, oep->ref + offsetof(dbs_sObject, o_ll));

  switch (oep->o.cid) {
  case pwr_eClass_LibHier:
    if (m_volume.cid != pwr_eClass_ClassVolume)
      oep->o.flags.b.devOnly = 1;
    break;
  case pwr_eClass_Alias:
    if (m_volume.cid != pwr_eClass_ClassVolume)
      oep->o.flags.b.isAliasClient = 1;
    break;
  case pwr_eClass_MountVolume:
  case pwr_eClass_CreateVolume:
  case pwr_eClass_MountObject:
    if (m_volume.cid == pwr_eClass_RootVolume) {
      oep->o.flags.b.isMountClient = 1;
    }
    break;
  default:
    break;    
  }

  oep->o.flags.b.isMountClean = 1;
  // Check all children

  sib_lh = sib_ll = &oep->o.sib_lh;
  for (sep = oep->foep; sep != 0; sep = sep->aoep) {
    if (sep->poep != oep) {
      printf("** Object not linked to right parent!\n");
    }

    checkObject(sep);

    sep->o.pref = oep->ref;
    dbs_Qinsert(&sts, sib_ll, &sep->o.sib_ll, sib_lh);
    sib_ll = &sep->o.sib_ll;
    oep->o.flags.b.isMountClean |= sep->o.flags.b.isMountClean && !sep->o.flags.b.isMountClient;
    nChild++;
  }

  /* Check name table, get reference to first and last child in name order.  */
  memset(&n, 0, sizeof(n));
  n.poix = oep->o.oid.oix;
  nep = (sNentry*)tree_FindSuccessor(&sts, m_name_th, &n);
  if (nep != NULL && nep->n.poix == oep->o.oid.oix) {
    oep->o.name_bt.start = nep->ref;
  }

  n.poix += 1;
  nep = (sNentry*)tree_FindPredecessor(&sts, m_name_th, &n);
  if (nep != NULL && nep->n.poix == oep->o.oid.oix) {
    oep->o.name_bt.end = nep->ref;
  }

  oep->o.name_bt.rsize = dbs_dAlign(sizeof(dbs_sName));
}

bool wb_dbs::importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                        pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                        const char *name, const char *normname, pwr_mClassDef flags,
                        pwr_tTime time, pwr_tTime rbTime, pwr_tTime dbTime,
                        size_t rbSize, size_t dbSize)
{
  sNentry     *nep;
  sOentry     *oep;
  pwr_tStatus  sts;
    
  if (cdh_ObjidIsNull(oid))
    printf("** Error: object is null!\n");

  //printf("install: %s o:%d.%d p:%d.%d b:%d.%d a:%d.%d f:%d.%d l:%d.%d \n", name, oid.vid, oid.oix, poid.vid, poid.oix, boid.vid, boid.oix, aoid.vid, aoid.oix, foid.vid, foid.oix, loid.vid, loid.oix);
    
  oep = (sOentry *)tree_Insert(&sts, m_oid_th, &oid);
  if (sts == TREE__INSERTED) {
  } else if (oep->flags.b.exist) {
    printf("** Error: object is already inserted!\n");
  }
    
  if (oid.oix == pwr_cNOix) {
    // this is the volume object
    m_oep = oep;
  }
    
  oep->flags.b.exist = 1;

  // Link objects to its relatives
  if (cdh_ObjidIsNotNull(poid))
    oep->poep = (sOentry *)tree_Insert(&sts, m_oid_th, &poid);
  if (cdh_ObjidIsNotNull(boid))
    oep->boep = (sOentry *)tree_Insert(&sts, m_oid_th, &boid);
  if (cdh_ObjidIsNotNull(aoid))
    oep->aoep = (sOentry *)tree_Insert(&sts, m_oid_th, &aoid);
  if (cdh_ObjidIsNotNull(foid))
    oep->foep = (sOentry *)tree_Insert(&sts, m_oid_th, &foid);
  if (cdh_ObjidIsNotNull(loid))
    oep->loep = (sOentry *)tree_Insert(&sts, m_oid_th, &loid);

  m_nObjects++;
  m_volume.cardinality++;    

  if (flags.b.DevOnly)
    oep->o.flags.b.devOnly = 1;

  oep->o.oid  = oid;
  oep->o.cid  = cid;
  oep->o.poid = poid;
  oep->o.boid = boid;
  oep->o.aoid = aoid;
  oep->o.foid = foid;
  oep->o.loid = loid;
  strcpy(oep->o.name, name);
  strcpy(oep->o.normname, normname);

  oep->o.time = time;
    
  if (cdh_ObjidIsNotNull(poid)) {
    dbs_sName n;
        
    /* insert into name table */
    memset(&n, 0, sizeof(n));
    n.poix = poid.oix;
    strcpy(n.normname, normname);
    nep = (sNentry*)tree_Insert(&sts, m_name_th, &n);
    nep->oep = oep;
  }
    
  oep->o.ohFlags.m = flags.m;
  
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

  return true;
}


pwr_tStatus
wb_dbs::closeFile(pwr_tBoolean doDelete)
{

  if (m_fp != NULL)
    fclose(m_fp);
  if (doDelete && *m_fileName != '\0') {
    if (remove(m_fileName) == 0)
      printf("-- Deleted file: %s\n", m_fileName);
  }

  return LDH__SUCCESS;
}

pwr_tBoolean
wb_dbs::createFile()
{
  pwr_tStatus sts;
  int size;
    
  printf("\n-- Working with load file volume '%s'...\n", m_v->name());

  /*{
    unsigned int i;
        
    for (i = 0; i < (sizeof(m_sect)/sizeof(m_sect[0])); i++) {
    printf("sect[%d]: version: %d, type: %d, size: %d, offset: %d\n", i, m_sect[i].version,
    m_sect[i].type, m_sect[i].size, m_sect[i].offset);
    }
    }*/

  printf("-- Open file...\n");
  sts = openFile();
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

  printf("-- Preparing volref section...\n");
  sts = prepareSectVolref();
  if (EVEN(sts)) goto error_handler;

  size += m_sect[dbs_eSect_volref].size;
  m_sect[dbs_eSect_oid].offset = size;

  printf("-- Writing oid section...\n");
  sts = writeSectOid();
  if (EVEN(sts)) goto error_handler;

  size += m_sect[dbs_eSect_oid].size;
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

  size += m_sect[dbs_eSect_class].size;

  printf("-- Writing directory section...\n");
  sts = writeSectDirectory();
  if (EVEN(sts)) goto error_handler;

  printf("-- Writing file section...\n");
  sts = writeSectFile(size);
  if (EVEN(sts)) goto error_handler;

  printf("-- Writing volref section...\n");
  sts = writeSectVolref(size);
  if (EVEN(sts)) goto error_handler;

  printf("-- Writing volref volumes...\n");
  sts = writeReferencedVolumes();
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
  struct stat sb;
  pwr_tStatus sts;
  int ret;
  

  cdh_ToLower(m_fileName, m_fileName);
    
  m_fp = fopen(m_fileName, "w+b");
  if (m_fp == NULL) {
    printf("** Cannot open file: %s\n", m_fileName);
    perror("   Reason");
    return LDH__FILEOPEN;
  }
  fn = dcli_fgetname(m_fp, m_fileName, m_fileName);
  if (fn != NULL)
    printf("-- Opened load file: %s\n", m_fileName);

  if ((ret = stat(m_fileName, &sb)) != 0) {
    sts = errno_GetStatus();
    perror("stat");
    return sts;
  }

  printf("st_atime...: %ld\n", sb.st_atime);
  printf("st_mtime...: %ld\n", sb.st_mtime);
  printf("st_ctime...: %ld\n", sb.st_ctime);

  return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectFile(size_t size)
{
  char f[dbs_dAlign(sizeof(dbs_sFile))];
  dbs_sFile *fp = (dbs_sFile*)f;
  PDR pdrs;

  co_GetOwnFormat(&fp->format);
  fp->cookie = dbs_cMagicCookie;
  fp->size = size;
  fp->offset = dbs_dAlign(sizeof(*fp));
  fp->formatVersion = dbs_cVersionFormat;
  fp->version = dbs_cVersionFile;
  fp->sectVersion = dbs_cVersionDirectory;
  fp->pwrVersion = 1;
  fp->time = m_volume.time;
  fp->fileType = dbs_eFile_volume;

  strcpy(fp->userName, "");
  strcpy(fp->comment, "");
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

  if (fseek(m_fp, 0, SEEK_SET) != 0)
    return LDH__FILEPOS;

  if (fwrite(f, sizeof(f) , 1, m_fp) < 1) return LDH__FILEWRITE;

  return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectDirectory()
{
    
  m_sect[dbs_eSect_dir].version    = dbs_cVersionDirectory;
  m_sect[dbs_eSect_volume].version = dbs_cVersionVolume;
  m_sect[dbs_eSect_volref].version = dbs_cVersionVolRef;
  m_sect[dbs_eSect_oid].version    = dbs_cVersionOid;
  m_sect[dbs_eSect_object].version = dbs_cVersionObject;
  m_sect[dbs_eSect_scobject].version = dbs_cVersionScObject;
  m_sect[dbs_eSect_rbody].version  = dbs_cVersionRbody;
  m_sect[dbs_eSect_name].version   = dbs_cVersionName;
  m_sect[dbs_eSect_class].version  = dbs_cVersionClass;
  m_sect[dbs_eSect_dbody].version  = dbs_cVersionDbody;

  m_sect[dbs_eSect_dir].type    = dbs_eSect_dir;
  m_sect[dbs_eSect_volume].type = dbs_eSect_volume;
  m_sect[dbs_eSect_volref].type = dbs_eSect_volref;
  m_sect[dbs_eSect_oid].type    = dbs_eSect_oid;
  m_sect[dbs_eSect_object].type = dbs_eSect_object;
  m_sect[dbs_eSect_scobject].type = dbs_eSect_scobject;
  m_sect[dbs_eSect_rbody].type  = dbs_eSect_rbody;
  m_sect[dbs_eSect_name].type   = dbs_eSect_name;
  m_sect[dbs_eSect_class].type  = dbs_eSect_class;
  m_sect[dbs_eSect_dbody].type  = dbs_eSect_dbody;

  if (m_sect[dbs_eSect_dir].size == 0)
    return LDH__SUCCESS;
    
  if (fseek(m_fp, m_sect[dbs_eSect_dir].offset, SEEK_SET) != 0)
    return LDH__FILEPOS;
    
  /*{
    unsigned int i;
        
    for (i = 0; i < (sizeof(m_sect)/sizeof(m_sect[0])); i++) {
    printf("sect[%d]: version: %d, type: %d, size: %d, offset: %d\n", i, m_sect[i].version,
    m_sect[i].type, m_sect[i].size, m_sect[i].offset);
    }
    }*/
    
  if (fwrite(&m_sect, sizeof(m_sect), 1, m_fp) < 1)
    return LDH__FILEWRITE;

  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_dir].offset + m_sect[dbs_eSect_dir].size));
    
  return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectVolume()
{
  char v[dbs_dAlign(sizeof(dbs_sVolume))];
  dbs_sVolume *vp = (dbs_sVolume *)v;


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

  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_volume].offset + m_sect[dbs_eSect_volume].size));

  return LDH__SUCCESS;
}    


pwr_tStatus
wb_dbs::prepareSectVolref()
{
  char v[dbs_dAlign(sizeof(dbs_sVolRef))];
  dbs_sVolRef *vp = (dbs_sVolRef*)v;
  cdh_uTid    cid;
  sCentry     *cep;
  pwr_tStatus sts;
  
    
  if (fseek(m_fp, m_sect[dbs_eSect_volref].offset, SEEK_SET) != 0)
    return LDH__FILEPOS;

  memset(v, 0, sizeof(v));

  cid.pwr = pwr_cNCid;
  cep = (sCentry*)tree_FindSuccessor(&sts, m_class_th, &cid.pwr);
  while (cep) {
    cdh_uVid vid;

    vid.pwr = pwr_cNVid;
    cid.pwr = cep->c.cid;
    vid.v.vid_0 = cid.c.vid_0;
    vid.v.vid_1 = cid.c.vid_1;
        
    if (vid.pwr != m_volume.vid) {
      // printf("volref: %d.%d.%d.%d\n", vid.v.vid_3, vid.v.vid_2, vid.v.vid_1, vid.v.vid_0);

      wb_mvrep *mvrep = m_v->merep()->volume(&sts, vid.pwr);
      if ( EVEN(sts)) throw wb_error_str("Metavolume not found");
      
      sVentry *vep;

      vep = (sVentry*)tree_Insert(&sts, m_vol_th, &vid.pwr);
      if (sts == TREE__INSERTED) {
        /* was inserted now */
        dbs_Open(&sts, &vep->env, mvrep->fileName());
        // printf("  sts...: %d\n", sts);
        // printf("  cookie: %d\n", vep->env.file.cookie);
        // printf("  size..: %d\n", vep->env.file.size);
        // printf("  time..: %ld\n", vep->env.file.time.tv_sec);

        // printf("VolRef File: %s\n", mvrep->fileName());      
      
        strcpy(vep->v.name, mvrep->name());
        vep->v.cid  = mvrep->cid();
        vep->v.time = vep->env.file.time;
        vep->v.size = vep->env.file.size;
        vep->v.offset = 0;

        int i = 0;
        while ((vp = dbs_VolRef(&sts, i, (dbs_sVolRef *)v, &vep->env)) != NULL) {
          sVentry *nvep;
          // printf("  vid.....: %d\n", vp->vid);
          // printf("    sts...: %d\n", sts);
          // printf("    name..: %s\n", vp->name);      
          // printf("    size..: %d\n", vp->size);
          // printf("    time..: %ld\n", vp->time.tv_sec);
          i++;
          nvep = (sVentry*)tree_Insert(&sts, m_vol_th, &vp->vid);
          if (sts == TREE__INSERTED) {
            // printf("  Inserted, sts...: %d\n", sts);
	    wb_mvrep *nmvrep = m_v->merep()->volume(&sts, vp->vid);
	    if ( EVEN(sts)) throw wb_error_str("Metavolume not found");
      
	    dbs_Open(&sts, &nvep->env, nmvrep->fileName());
	    // printf("  sts...: %d\n", sts);
	    // printf("  cookie: %d\n", nvep->env.file.cookie);
	    // printf("  size..: %d\n", nvep->env.file.size);
	    // printf("  time..: %ld\n", nvep->env.file.time.tv_sec);

            strcpy(nvep->v.name, vp->name);
            nvep->v.cid  = vp->cid;
            nvep->v.time = vp->time;
            nvep->v.size = vp->size;
            nvep->v.offset = 0;
          } else {
            // printf("  Not inserted, sts...: %d\n", sts);
          }          
        }
      }


      //if (fwrite(v, sizeof(v), 1, m_fp) < 1)
      //return LDH__FILEWRITE;
      // m_sect[dbs_eSect_volref].size += sizeof(v);
    }
    vid.pwr++;
    cid.pwr = pwr_cNCid;
    cid.c.vid_0 = vid.v.vid_0;
    cid.c.vid_1 = vid.v.vid_1;
    cep = (sCentry*)tree_FindSuccessor(&sts, m_class_th, &cid.pwr);        
  }

  // Search trhough all found volumes and get their volrefs
  int nVolume = 0;
  sVentry *vep;
  vep = (sVentry*)tree_Minimum(&sts, m_vol_th);
  while (vep) {
    // printf("vid: %d\n", vep->v.vid);
    // printf("  cookie: %d\n", vep->env.file.cookie);
    // printf("  name..: %s\n", vep->v.name);
    if (vep->env.file.cookie == 0) {
      printf("  volume not found: %d\n", vep->v.vid);
    } else {
      nVolume++;
      m_sect[dbs_eSect_volref].size += sizeof(v);
    }
    
    vep = (sVentry*)tree_Successor(&sts, m_vol_th, vep);        
  }

  // printf("Found %d volumes.\n", nVolume);

  return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectVolref(size_t size)
{
  pwr_tStatus sts;  
    
  if (fseek(m_fp, m_sect[dbs_eSect_volref].offset, SEEK_SET) != 0)
    return LDH__FILEPOS;

  // Search trhough all found volumes and get their volrefs
  sVentry *vep;
  vep = (sVentry*)tree_Minimum(&sts, m_vol_th);
  while (vep) {
    // printf("vid.......: %d\n", vep->v.vid);
    // printf("  cookie..: %d\n", vep->env.file.cookie);
    // printf("  name....: %s\n", vep->v.name);
    // printf("  size....: %d\n", vep->v.size);
    // printf("  offset..: %d\n", vep->v.offset);
    if (vep->env.file.cookie == 0) {
      printf("  volume not found: %d\n", vep->v.vid);
    } else {
      vep->v.offset = dbs_dAlign(size);
      size += vep->v.size = dbs_dAlign(vep->v.size);
      char v[dbs_dAlign(sizeof(dbs_sVolRef))];
      dbs_sVolRef *vp = (dbs_sVolRef*)v;
      // printf("  size....: %d\n", vep->v.size);
      // printf("  offset..: %d\n", vep->v.offset);
      *vp = vep->v;
      if (fwrite(v, sizeof(v), 1, m_fp) < 1)
        return LDH__FILEWRITE;
    }
    
    vep = (sVentry*)tree_Successor(&sts, m_vol_th, vep);        
  }


  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_volref].offset + m_sect[dbs_eSect_volref].size));

  // printf("  Total file size....: %d\n", size);
  return LDH__SUCCESS;
}

static pwr_tStatus
copyFile(FILE *sfp, FILE *tfp, size_t size)
{
  char buf[512];
  size_t bytes;
  
  while (size > 0) {
    if (size > sizeof(buf)) {
      bytes = sizeof(buf);
    } else {
      bytes = size;
    }
    size -= bytes;  
    
    if (fread(buf, bytes, 1, sfp) < 1)
      return LDH__FILEWRITE;

    if (fwrite(buf, bytes, 1, tfp) < 1)
      return LDH__FILEWRITE;
  }

  return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeReferencedVolumes()
{
  pwr_tStatus sts;  
    

  // Search trhough all found volumes and get their volrefs
  sVentry *vep;
  vep = (sVentry*)tree_Minimum(&sts, m_vol_th);
  while (vep) {
    // printf("vid.......: %d\n", vep->v.vid);
    // printf("  cookie..: %d\n", vep->env.file.cookie);
    // printf("  name....: %s\n", vep->v.name);
    // printf("  size....: %d\n", vep->v.size);
    // printf("  offset..: %d\n", vep->v.offset);
    if (vep->env.file.cookie == 0) {
      printf("  volume not found: %d\n", vep->v.vid);
    } else {
      if (fseek(m_fp, vep->v.offset, SEEK_SET) != 0)
        return LDH__FILEPOS;
      if (fseek(vep->env.f, 0, SEEK_SET) != 0)
        return LDH__FILEPOS;

      sts = copyFile(vep->env.f, m_fp, vep->env.file.size);
      if (EVEN(sts))
        return LDH__FILEWRITE;

      // printf("ftell %ld, offset + size %d, offset %d, size %d\n", ftell(m_fp), vep->v.offset + vep->env.file.size, vep->v.offset, vep->env.file.size);
      assert(ftell(m_fp) == (long)(vep->v.offset + vep->env.file.size));
    }

    vep = (sVentry*)tree_Successor(&sts, m_vol_th, vep);        
  }

  return LDH__SUCCESS;
}

pwr_tStatus
wb_dbs::writeSectOid()
{
  sOentry  *oep;
  char     o[dbs_dAlign(sizeof(dbs_sOid))];
  dbs_sOid *op = (dbs_sOid*)o;
    
  if (m_sect[dbs_eSect_oid].size == 0)
    return LDH__SUCCESS;
    
  if (fseek(m_fp, m_sect[dbs_eSect_oid].offset, SEEK_SET) != 0)
    return LDH__FILEPOS;

  memset(o, 0, sizeof(o));
    
  oep = (sOentry*)tree_Minimum(NULL, m_oid_th);
  while (oep != NULL) {
    op->oid = oep->o.oid;
    op->ref = oep->ref;
        
    if (fwrite(o, sizeof(o), 1, m_fp) < 1)
      return LDH__FILEWRITE;

    oep = (sOentry*)tree_Successor(NULL, m_oid_th, (tree_sNode*)oep);
  }
    
  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_oid].offset + m_sect[dbs_eSect_oid].size));

  return LDH__SUCCESS;
}

static pwr_tStatus
writeTree(wb_dbs::sOentry *oep, FILE *fp)
{
  pwr_tStatus sts;
    
  if (!oep)
    return 1;
    
  if (oep->poep)
    oep->o.flags.b.devOnly |= oep->poep->o.flags.b.devOnly;
  
  if (fwrite(&oep->o, dbs_dAlign(sizeof(oep->o)), 1, fp) < 1)
    return LDH__FILEWRITE;

  sts = writeTree(oep->foep, fp);
  if (EVEN(sts))
    return sts;
    
  sts = writeTree(oep->aoep, fp);
  if (EVEN(sts))
    return sts;

  return 1;
}

pwr_tStatus
wb_dbs::writeSectObject()
{
  //sOentry *oep;
    
  if (m_sect[dbs_eSect_object].size == 0)
    return LDH__SUCCESS;
    
  if (fseek(m_fp, m_sect[dbs_eSect_object].offset, SEEK_SET) != 0)
    return LDH__FILEPOS;

  writeTree(m_oep, m_fp);

  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_object].offset + m_sect[dbs_eSect_object].size));

  return LDH__SUCCESS;
}


bool
wb_dbs::importDbody(pwr_tOid oid, size_t size, void *body)
{
  pwr_tStatus sts;
  sOentry *oep;
  char b[dbs_dAlign(sizeof(dbs_sBody))];
    
  m_nDbodyObjects++;
    
  memset(b, 0, sizeof(b));
    
  oep = (sOentry *)tree_Find(&sts, m_oid_th, &oid);
  if (EVEN(sts)) {
  }

  if (oep->dbody.size == 0) {
    if (size != 0) printf("error dbody size %d %s\n", size, cdh_ObjidToString(0,oid,0));
    return true;
  }

  oep->o.dbody.ref = dbs_dMakeRef(dbs_eSect_dbody, m_sect[dbs_eSect_dbody].size + dbs_dAlign(sizeof(dbs_sBody)));
  m_sect[dbs_eSect_dbody].size += oep->dbody.size + sizeof(b);

  memcpy(b, &oep->dbody, sizeof(oep->dbody));
    
  if (fwrite(b, sizeof(b), 1, m_fp) < 1)
    return false;

  /* @todo!!! objdid_self */

  if (fwrite(body, oep->dbody.size, 1, m_fp) < 1)
    return false;

  return true;
}


pwr_tStatus
wb_dbs::writeSectDbody()
{

  if (fseek(m_fp, m_sect[dbs_eSect_dbody].offset, SEEK_SET) != 0)
    return LDH__FILEPOS;

  m_v->exportDbody(*this);
    
  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_dbody].offset + m_sect[dbs_eSect_dbody].size));

  return LDH__SUCCESS;
}


bool
wb_dbs::importRbody(pwr_tOid oid, size_t size, void *body)
{
  pwr_tStatus sts;
  sOentry *oep;
  char b[dbs_dAlign(sizeof(dbs_sBody))];
    
  m_nRbodyObjects++;
    
  memset(b, 0, sizeof(b));
    
  oep = (sOentry *)tree_Find(&sts, m_oid_th, &oid);
  if (EVEN(sts)) {
  }
  //printf("install body: %u\n", (unsigned int)body);
  //printf(" 1 install Rbody %s, bsize: %d, ssize: %d\n", oep->o.name, oep->rbody.size, m_sect[dbs_eSect_rbody].size);

  if (oep->o.flags.b.isMountClient)
    getMountServer(oep, body);
  
  if (oep->o.flags.b.isAliasClient)
    getAliasServer(oep, body);
  
  if (oep->rbody.size == 0) {
    if (size != 0) printf("error rbody size %d %s\n", size, cdh_ObjidToString(0,oid,0));
    return true;
  }
    
  oep->o.rbody.ref = dbs_dMakeRef(dbs_eSect_rbody, m_sect[dbs_eSect_rbody].size + sizeof(b));
  m_sect[dbs_eSect_rbody].size += oep->rbody.size + sizeof(b);

  memcpy(b, &oep->rbody, sizeof(oep->rbody));
    
  if (fwrite(b, sizeof(b), 1, m_fp) < 1)
    return false;

  //printf("  2 install Rbody %s, bsize: %d, ssize: %d\n", oep->o.name, oep->rbody.size, m_sect[dbs_eSect_rbody].size);
  /* @todo!!! objdid_self */

  if (fwrite(body, oep->rbody.size, 1, m_fp) < 1)
    return false;

  //printf("   3 install Rbody %s, bsize: %d, ssize: %d\n", oep->o.name, oep->rbody.size, m_sect[dbs_eSect_rbody].size);
  return true;
}


pwr_tStatus
wb_dbs::writeSectRbody()
{

  if (fseek(m_fp, m_sect[dbs_eSect_rbody].offset, SEEK_SET) != 0)
    return LDH__FILEPOS;

  m_v->exportRbody(*this);
    
  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_rbody].offset + m_sect[dbs_eSect_rbody].size));

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
    //printf("name: %s, poix: %d\n", nep->n.normname, nep->n.poix);
    nep->n.ref = nep->oep->ref;
        
    if (fwrite(&nep->n,  dbs_dAlign(sizeof(nep->n)), 1, m_fp) < 1)
      return LDH__FILEWRITE;
        
    nep = (sNentry*)tree_Successor(NULL, m_name_th, (tree_sNode*)nep);
  }

  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_name].offset + m_sect[dbs_eSect_name].size));

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
    //printf("class: %d, nObjects: %d\n", cep->c.cid, cep->c.nObjects);
    if (fwrite(&cep->c,  dbs_dAlign(sizeof(cep->c)), 1, m_fp) < 1)
      return LDH__FILEWRITE;
        
    cep = (sCentry*)tree_Successor(NULL, m_class_th, (tree_sNode*)cep);
  }

  assert(ftell(m_fp) == (long)(m_sect[dbs_eSect_class].offset + m_sect[dbs_eSect_class].size));

  return LDH__SUCCESS;
}

void
wb_dbs::buildSectOid()
{
  sOentry *oep;

  oep = (sOentry*)tree_Minimum(NULL, m_oid_th);
  while (oep != NULL) {
    oep->oidref = dbs_dMakeRef(dbs_eSect_oid, m_sect[dbs_eSect_oid].size);
    m_sect[dbs_eSect_oid].size += dbs_dAlign(sizeof(dbs_sOid));
        
    oep = (sOentry*)tree_Successor(NULL, m_oid_th, (tree_sNode*)oep);
  }

  oep = (sOentry*)tree_Minimum(NULL, m_oid_th);
  if (oep != NULL) {
    m_volume.oid_bt.start = oep->oidref;
    oep = (sOentry*)tree_Maximum(NULL, m_oid_th);
    m_volume.oid_bt.end = oep->oidref;
    m_volume.oid_bt.rsize = dbs_dAlign(sizeof(dbs_sOid));
  }
}

void
wb_dbs::buildSectName()
{
  sNentry *nep;
    
  /* allocate space for all name entries in name section*/

  nep = (sNentry*)tree_Minimum(NULL, m_name_th);
  while (nep != NULL) {
    nep->ref = dbs_dMakeRef(dbs_eSect_name, m_sect[dbs_eSect_name].size);
    m_sect[dbs_eSect_name].size += dbs_dAlign(sizeof(dbs_sName));
    m_nNameObjects++;
        
    nep = (sNentry*)tree_Successor(NULL, m_name_th, (tree_sNode*)nep);
  }

  nep = (sNentry*)tree_Minimum(NULL, m_name_th);
  if (nep != NULL) {
    m_volume.name_bt.start = nep->ref;
    nep = (sNentry*)tree_Maximum(NULL, m_name_th);
    m_volume.name_bt.end = nep->ref;
    m_volume.name_bt.rsize = dbs_dAlign(sizeof(dbs_sName));
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
        
    ref = cep->ref = dbs_dMakeRef(dbs_eSect_class, m_sect[dbs_eSect_class].size);
    m_sect[dbs_eSect_class].size += dbs_dAlign(sizeof(dbs_sClass));
    dbs_Qinit(&sts, &cep->c.o_lh, ref + offsetof(dbs_sClass, o_lh));

    // Link all object instances to this class
    o_lh = o_ll = &cep->c.o_lh;
    for (oep = cep->o_lh; oep != 0; oep = oep->o_ll) {
      cep->c.nObjects++;
      m_nClassObjects++;
      dbs_Qinsert(&sts, o_ll, &oep->o.o_ll, o_lh);
      o_ll = &oep->o.o_ll;
    }

    cep = (sCentry*)tree_Successor(NULL, m_class_th, (tree_sNode*)cep);        
  }

  cep = (sCentry*)tree_Minimum(NULL, m_class_th);
  if (cep != NULL) {
    m_volume.class_bt.start = cep->ref;
    cep = (sCentry*)tree_Maximum(NULL, m_class_th);
    m_volume.class_bt.end = cep->ref;
    m_volume.class_bt.rsize = dbs_dAlign(sizeof(dbs_sClass));
  }
}

void
wb_dbs::cidInsert(pwr_tStatus *sts, pwr_tCid cid, sCentry **cep)
{
  *cep = (sCentry*)tree_Insert(sts, m_class_th, &cid);
  // if ( cdh_CidToVid(cid) == 1)
  //   return;
  if (*sts == TREE__INSERTED) {
    // Insert depending classes
    pwr_tCid *lst;
    sCentry *entry;
    int cnt;
    pwr_tStatus lsts;

    m_v->merep()->classDependency( &lsts, cid, &lst, &cnt);
    for ( int i = 0; i < cnt; i++)
      cidInsert(&lsts, lst[i], &entry);
    free(lst);
  }
}

void
wb_dbs::classInsert(sOentry *oep)
{
  pwr_tStatus  sts;
  sCentry     *cep;

  cidInsert(&sts, oep->o.cid, &cep);
  if (sts == TREE__INSERTED) {
    /* was inserted now */

    cep->o_lh = cep->o_lt = oep;
  } else {
    /* was allready present */
    if ( !cep->o_lt)
      cep->o_lh = cep->o_lt = oep;
    else {
      cep->o_lt->o_ll = oep;
      cep->o_lt = oep;
    }
  }    
}

bool
wb_dbs::importMeta(dbs_sMenv *mep)
{
  return true;
}

void wb_dbs::getAliasServer(sOentry *oep, void *p)
{
  pwr_tStatus sts;
  pwr_sAlias *alias = (pwr_sAlias *)p;
  sOentry    *aep;

  oep->o.flags.b.isAliasClient = 0;
  
  if (cdh_ObjidIsNull(alias->Object)) {
    printf("!! Alias does not refer to any object!\n");
    printf("   Alias:  %s\n", pathName(oep));
    printf("   Alias will not be loaded.\n");
    m_warnings++;
    return; 
  }

  if (alias->Object.vid != m_volume.vid) {
    printf("!! Alias refers to object outside own volume!\n");
    printf("   Alias:  %s\n", pathName(oep));
    printf("   Object: %s\n", cdh_ObjidToString(NULL, alias->Object, 1));
    printf("   Alias will not be loaded.\n");
    m_warnings++;
    return; 
  }
  
  aep = (sOentry *)tree_Find(&sts, m_oid_th, &alias->Object);
  if (!aep) {
    printf("!! Alias refers to a non existing object!\n");
    printf("   Alias:  %s\n", pathName(oep));
    printf("   Alias will not be loaded.\n");
    m_warnings++;
    return; 
  }
  
  if (aep->o.flags.b.devOnly) {
    printf("!! An alias may not refer to a non runtime object!\n");
    printf("   Alias:  %s\n", pathName(oep));
    printf("   Object: %s\n", pathName(aep));
    printf("   Alias will not be loaded.\n");
    m_warnings++;
    return;
  }

  switch (aep->o.cid) {
  case pwr_eClass_Alias:
    printf("!! An alias may not refer to another alias!\n");
    printf("   Alias:  %s\n", pathName(oep));
    printf("   Object: %s\n", pathName(aep));
    printf("   Alias will not be loaded.\n");
    m_warnings++;
    return;
    break;
  case pwr_eClass_MountVolume:
  case pwr_eClass_CreateVolume:
  case pwr_eClass_MountObject:
    printf("!! An alias may not refer to a mount object!\n");
    printf("   Alias:  %s\n", pathName(oep));
    printf("   Object: %s\n", pathName(aep));
    printf("   Alias will not be loaded.\n");
    m_warnings++;
    return;
    break;
  }

  oep->o.flags.b.isAliasClient = 1;
  oep->o.soid = aep->o.oid;
}


char *wb_dbs::pathName(sOentry *oep)
{
  static char buff[512];
  static int  level = 0;

  if (level == 0)
    buff[0] = '\0';

  if (oep != m_oep) {
    level++;
    pathName(oep->poep);
    level--;
    strcat(buff, oep->o.name);  
    if (level > 0)
      strcat(buff, "-");
  } else {
    strcat(buff, oep->o.name);
    strcat(buff, ":");
  }
  return buff;
}

void wb_dbs::getMountServer(sOentry *oep, void *p)
{

  switch (oep->o.cid) {
  case pwr_eClass_MountObject:
  {  
    pwr_sMountObject  *mountObject;
    mountObject = (pwr_sMountObject *) p;
    oep->o.soid = mountObject->Object;
    break;
  }
  case pwr_eClass_MountVolume:
  {
    pwr_sMountVolume  *mountVolume;
    mountVolume = (pwr_sMountVolume *) p;
    oep->o.soid = pwr_cNOid;
    oep->o.soid.vid = mountVolume->Volume;
    break;
  }
  case pwr_eClass_CreateVolume:
  {
    pwr_sCreateVolume *createVolume;
    createVolume = (pwr_sCreateVolume *) p;
    oep->o.soid = pwr_cNOid;
    oep->o.soid.vid = createVolume->Volume;
    break;
  }
  default:
    break;
  }
}
