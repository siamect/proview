/* 
 * Proview   $Id: wb_vrepdbms.cpp,v 1.2 2007-10-19 07:00:02 claes Exp $
 * Copyright (C) 2007 SSAB Oxelösund AB.
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

#if defined PWRE_CONF_MYSQL

#include <sys/stat.h>
#include <errno.h>
#include "co_cdh.h"
#include "co_tree.h"
#include "co_time.h"
#include "wb_dbms.h"
#include "wb_vrepdbms.h"
#include "wb_orepdbms.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"
#include "wb_adrep.h"
#include "wb_erep.h"
#include "wb_dbs.h"
#include "wb_ldh.h"
#include "wb_merep.h"
#include "wb_attribute.h"
#include "wb_dblock.h"
#include "co_msgwindow.h"
#include "wb_vrepwbl.h"

typedef struct sArefKey
{
  pwr_tCid   cid;
  pwr_eBix   bix;
  int        offset;
} sArefKey;

typedef struct sAref 
{
  tree_sNode node;
  sArefKey   key;
} sAref;

typedef struct sAttributeInfo {
  pwr_tAttrRef aref;
  pwr_tAix     aix;
  pwr_tUInt32  nElement;
} sAttributeInfo;

typedef struct sAttributeKey {
  pwr_tCid         cid;
  pwr_eBix         bix;
  pwr_tUInt32      oStart;
  pwr_tUInt32      oEnd;
} sAttributeKey;

typedef struct sAttribute {
  tree_sNode     node;
  sAttributeKey  key;
  sAttributeInfo o;
  sAttributeInfo n;
} sAttribute;

typedef struct sClass
{
  tree_sNode   node;
  pwr_tCid     cid;
  pwr_tObjName name;
  pwr_tTime    o_time;
  pwr_tTime    n_time;
  pwr_tUInt32  count;
} sClass;

typedef struct sClassChk
{
  tree_sNode   node;
  pwr_tCid     cid;
} sClassChk;

static int comp_attribute(tree_sTable *tp, tree_sNode *x, tree_sNode *y);
static int comp_aref(tree_sTable *tp, tree_sNode *x, tree_sNode *y);

void wb_vrepdbms::unref()
{
  if (--m_nRef == 0) {
    wb_dblock::dbunlock(m_fileName);
    delete this;
  }
}

wb_vrep *wb_vrepdbms::ref()
{
  m_nRef++;
  return this;
}

wb_vrepdbms::wb_vrepdbms(wb_erep *erep, const char *fileName) :
  m_erep(erep), m_nRef(0), m_ohead(), m_oid_th(0)
{
  strcpy(m_fileName, fileName);
  wb_dblock::dblock( m_fileName);

  wb_dbms_env *env = new wb_dbms_env(fileName);
  env->open();
  m_db = new wb_dbms();
  m_db->open(env);
  m_ohead.setDb(m_db);
  strcpy(m_name, m_db->volumeName());
  m_vid = m_db->vid();
  m_cid = m_db->cid();

  m_merep = new wb_merep(m_fileName, erep, this);
  m_merep->compareMeta(m_name, erep->merep());
}

wb_vrepdbms::wb_vrepdbms(wb_erep *erep, pwr_tVid vid, pwr_tCid cid, const char *volumeName, const char *fileName) :
  m_erep(erep), m_nRef(0), m_ohead(), m_oid_th(0)
{
  strcpy(m_fileName, fileName);
  wb_dblock::dblock( m_fileName);

  m_db = new wb_dbms();
  m_db->create(vid, cid, volumeName, fileName);
  m_ohead.setDb(m_db);
  strcpy(m_name, m_db->volumeName());
  m_vid = m_db->vid();
  m_cid = m_db->cid();

  m_merep = m_erep->merep();
  m_merep->copyFiles(m_fileName);
}

wb_erep *wb_vrepdbms::erep()
{
  return m_erep;
}

wb_vrep *wb_vrepdbms::next()
{
  pwr_tStatus sts;

  return m_erep->nextVolume( &sts, m_db->vid());
}

wb_merep *wb_vrepdbms::merep() const
{
  return m_merep;
}

wb_srep *wb_vrepdbms::newSession()
{
  return 0;
}

void wb_vrepdbms::objectName(pwr_tOid oid, char *name, int level)
{
  if (cdh_ObjidIsNull(oid))
    return;

  wb_dbms_ohead o(m_db, m_db->m_txn, oid);

  if (o.oix() == pwr_cNOix) {
    strcpy(name, o.name());
    strcat(name, ":");
  } else {
    objectName(o.poid(), name, level+1);
    strcat(name, o.name());
    if (level > 0)
      strcat(name, "-");
  }
}

wb_name wb_vrepdbms::longName(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;
  char name[512];

  try {
    objectName(o->oid(), name, 0);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    name[0] = '\0';
  }
  return wb_name(name);
}

bool wb_vrepdbms::isOffspringOf(pwr_tStatus *sts, const wb_orep *child, const wb_orep *parent)
{
  return false;
}

bool wb_vrepdbms::isLocal(const wb_orep *o)
{
  if (o)
    return o->oid().vid == wb_vrep::vid();

  return false;
}

bool wb_vrepdbms::createSnapshot(const char *fileName, const pwr_tTime *time)
{
  try {
    wb_dbs dbs(this);

    if ( fileName)
      dbs.setFileName( fileName);

    dbs.importVolume(*this);

    return true;
  } catch (wb_error& e) {
    return false;
  }
}

void wb_vrepdbms::objectName(const wb_orep *o, char *str)
{
  if (str)
    *str = '\0';
}

const char *wb_vrepdbms::objectName(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, o->oid());

    return m_ohead.name();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}


void wb_vrepdbms::load()
{
}


wb_orep* wb_vrepdbms::object(pwr_tStatus *sts)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid oid;

    oid.vid = m_vid;
    oid.oix = pwr_cNOix;

    m_ohead.get(m_db->m_txn, oid);
    m_ohead.get(m_db->m_txn, m_ohead.foid());

    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
  catch (wb_error &e) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
}

wb_orep* wb_vrepdbms::object(pwr_tStatus *sts, pwr_tOid oid)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, oid);

    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_error &e) {
    *sts = e.sts();
    return 0;
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep* wb_vrepdbms::object(pwr_tStatus *sts, wb_name &name)
{
  pwr_tOid poid;
  poid.vid = m_db->m_vid;
  poid.oix = pwr_cNOix;
  *sts = LDH__SUCCESS;

  try {
    for (int i = 0; name.hasSegment(i); i++) {
      wb_dbms_name n(m_db, poid, name.normSegment(i));
      int rc = n.get(m_db->m_txn);
      if (rc) {
        *sts = LDH__NOSUCHOBJ;
        return 0;
      }
      poid = n.oid();

    }

    m_ohead.get(m_db->m_txn, poid);
    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep* wb_vrepdbms::object(pwr_tStatus *sts, const wb_orep *parent, wb_name &name)
{
  *sts = LDH__SUCCESS;
  try {
    wb_dbms_name n(m_db, m_db->m_txn, parent->oid(), name);
    m_ohead.get(m_db->m_txn, n.oid());
    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name)
{
  *sts = LDH__SUCCESS;
  wb_dbms_txn *txn = m_db->subBegin(m_db->m_txn);
  void *p = 0;

  try {
    int rs = 0;
    pwr_tTime time;
    pwr_tOid oid = m_db->new_oid(txn);
    wb_dbms_ohead o(m_db, txn, orep->oid());
    clock_gettime(CLOCK_REALTIME, &time);

    if (o.rbSize()) {
      o.rbTime(time);
      p = calloc(1, o.rbSize());
      wb_dbms_rbody rb(m_db, orep->oid());
      rs = rb.get(txn, 0, o.rbSize(), p);
      if (rs)
        printf("wb_vrepdbms::copyObject, rb.get rs %d\n", rs);
      rb.oid(oid);
      rs = rb.ins(txn);
      if (rs)
        printf("wb_vrepdbms::copyObject, rb.ins rs %d\n", rs);
      free(p);
      p = 0;
    }
    if (o.dbSize()) {
      o.dbTime(time);
      p = calloc(1, o.dbSize());
      wb_dbms_dbody db(m_db, orep->oid());
      rs = db.get(txn, 0, o.dbSize(), p);
      if (rs)
        printf("wb_vrepdbms::copyObject, db.get rs %d\n", rs);
      db.oid(oid);
      rs = db.ins(txn);
      if (rs)
        printf("wb_vrepdbms::copyObject, db.ins rs %d\n", rs);
      free(p);
      p = 0;
    }

    o.oid(oid);
    if (name)
      o.name(name);
    else
      o.name(oid);

    o.ohTime(time);
    o.poid(pwr_cNOid);
    o.boid(pwr_cNOid);
    o.aoid(pwr_cNOid);
    o.foid(pwr_cNOid);
    o.loid(pwr_cNOid);

    adopt(txn, o, d);

    rs = o.ins(txn);
    if (rs)
      printf("wb_vrepdbms::copyObject, o.put rs %d\n", rs);

    wb_dbms_class c(m_db, o);
    rs = c.ins(txn);
    if (rs)
      printf("wb_vrepdbms::copyObject, c.put rs %d\n", rs);

    txn->subCommit();
    return new (this) wb_orepdbms(&o.m_o);
  }
  catch (wb_error &e) {
    txn->subAbort();
    if (p)
      free(p);
    *sts = e.sts();
    return 0;
  }
  catch (wb_dbms_error &e) {
    txn->subAbort();
    if (p)
      free(p);
    *sts = LDH__DBERROR;
    return 0;
  }
}

bool wb_vrepdbms::copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination &d)
{
  return false;
}

wb_orep* wb_vrepdbms::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name)
{
  *sts = LDH__SUCCESS;
  wb_dbms_txn *txn = m_db->subBegin(m_db->m_txn);
  void *p = 0;

  try {
    int rs = 0;
    pwr_tTime time;
    pwr_tOid oid = m_db->new_oid(txn);
    wb_dbms_ohead o(m_db, oid);

    clock_gettime(CLOCK_REALTIME, &time);

    o.cid(cdef.cid());
    if (name)
      o.name(name);
    else
      o.name(oid);

    o.ohTime(time);
    o.flags(cdef.flags());
    o.rbSize(cdef.size(pwr_eBix_rt));
    o.dbSize(cdef.size(pwr_eBix_dev));

    adopt(txn, o, d);

    if (o.rbSize()) {
      pwr_tStatus sts = 1;

      o.rbTime(time);
      p = calloc(1, o.rbSize());
      cdef.templateBody(&sts, pwr_eBix_rt, p, o.oid());
      wb_dbms_rbody b(m_db, o.oid(), o.rbSize(), p);
      rs = b.ins(txn);
      if (rs)
        printf("wb_vrepdbms::createObject, rb.ins rs %d\n", rs);
      free(p);
      p = 0;
    }
    if (o.dbSize()) {
      pwr_tStatus sts = 1;

      o.dbTime(time);
      p = calloc(1, o.dbSize());
      cdef.templateBody(&sts, pwr_eBix_dev, p, o.oid());
      wb_dbms_dbody b(m_db, o.oid(), o.dbSize(), p);
      rs = b.ins(txn);
      if (rs)
        printf("wb_vrepdbms::createObject, db.ins rs %d\n", rs);
      free(p);
      p = 0;
    }

    rs = o.ins(txn);
    if (rs)
      printf("wb_vrepdbms::createObject, o.ins rs %d\n", rs);

    wb_dbms_class c(m_db, o);
    rs = c.ins(txn);
    if (rs)
      printf("wb_vrepdbms::createObject, c.ins rs %d\n", rs);

    rs = txn->subCommit();

    return new (this) wb_orepdbms(&o.m_o);
  }
  catch (wb_error &e) {
    txn->subAbort();
    if (p)
      free(p);
    //*sts = e.sts();
    throw e;
  }
  catch (wb_dbms_error &e) {
    txn->subAbort();
    if (p)
      free(p);
    *sts = LDH__DBERROR;
    return 0;
  }
}

bool wb_vrepdbms::deleteObject(pwr_tStatus *sts, wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  wb_dbms_ohead o(m_db, orp->oid());

  wb_dbms_txn *txn = m_db->subBegin(m_db->m_txn);

  try {
    o.get(txn);

    unadopt(txn, o);

    wb_dbms_class c(m_db, o);
    c.del(txn);

    wb_dbms_dbody db(m_db, o.oid());
    db.del(txn);
    wb_dbms_rbody rb(m_db, o.oid());
    rb.del(txn);

    o.del(txn);

    txn->subCommit();
    return true;
  }
  catch (wb_error &e) {
    txn->subAbort();
    *sts = e.sts();
    return 0;
  }
  catch (wb_dbms_error &e) {
    txn->subAbort();
    *sts = LDH__DBERROR;
    return false;
  }
}

bool wb_vrepdbms::deleteFamilyMember(pwr_tOid oid, wb_dbms_txn *txn)
{
  if (cdh_ObjidIsNull(oid))
    return false;

  int rs = 0;

  wb_dbms_ohead o(m_db, oid);
  o.get(txn);

  deleteFamilyMember(o.foid(), txn);

  deleteFamilyMember(o.aoid(), txn);

  wb_dbms_class c(m_db, o);
  rs = c.del(txn);
  if (rs)
    printf("wb_vrepdbms::deleteFamilyMember, c.del rs %d\n", rs);

  wb_dbms_dbody db(m_db, o.oid());
  rs = db.del(txn);
  wb_dbms_rbody rb(m_db, o.oid());
  rs = rb.del(txn);

  wb_dbms_name n(m_db, o.oid(), o.poid(), o.normname());
  rs = n.del(txn);
  if (rs)
    printf("wb_vrepdbms::deleteFamilyMember, n.del rs %d\n", rs);

  rs = o.del(txn);
  if (rs)
    printf("wb_vrepdbms::deleteFamilyMember, o.del rs %d\n", rs);

  return true;
}

bool wb_vrepdbms::deleteFamily(pwr_tStatus *sts, wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  wb_dbms_ohead o(m_db, orp->oid());

  wb_dbms_txn *txn = m_db->subBegin(m_db->m_txn);

  try {
    o.get(txn);
    printf("wb_vrepdbms::deleteFamily %s\n", o.name());

    unadopt(txn, o);

    deleteFamilyMember(o.oid(), txn);

    txn->subCommit();
    printf("wb_vrepdbms::deleteFamily success\n");
    return true;
  }
  catch (wb_dbms_error &e) {
    printf("wb_vrepdbms::deleteFamily failure, %s\n", e.what().c_str());
    txn->subAbort();
    *sts = LDH__DBERROR;
    return false;
  }
}

bool wb_vrepdbms::deleteOset(pwr_tStatus *sts, wb_oset *oset)
{
  *sts = LDH__NYI;
  return false;
}

bool wb_vrepdbms::moveObject(pwr_tStatus *sts, wb_orep *orp, wb_destination &d)
{
  *sts = LDH__SUCCESS;

  wb_dbms_txn *txn = m_db->subBegin(m_db->m_txn);

  try {
    wb_dbms_ohead o(m_db, txn, orp->oid());
    unadopt(txn, o);
    adopt(txn, o, d);
    o.upd(txn);

    txn->subCommit();
  }
  catch (wb_error &e) {
    txn->subAbort();
    *sts = e.sts();
    return 0;
  }
  catch (wb_dbms_error &e) {
    txn->subAbort();
    *sts = LDH__DBERROR;
  }

  return true;
}

bool wb_vrepdbms::renameObject(pwr_tStatus *sts, wb_orep *orp, wb_name &name)
{
  *sts = LDH__SUCCESS;
  wb_dbms_txn *txn = m_db->subBegin(m_db->m_txn);

  try {
    int rc = 0;
    m_ohead.get(txn, orp->oid());

    wb_dbms_name n(m_db, m_ohead);
    rc = n.del(txn);
    if (rc)
      printf("wb_vrepdbms::renameObject, n.del rc %d\n", rc);
    n.name(name);
    rc = n.ins(txn);
    if (rc) {
      printf("wb_vrepdbms::renameObject, n.ins rc %d\n", rc);
      *sts = LDH__NAMALREXI;
      txn->abort();
      m_ohead.clear();
      return false;
    }

    m_ohead.name(name);
    m_ohead.upd(txn);

    txn->subCommit();
    return true;
  }
  catch (wb_dbms_error &e) {
    txn->subAbort();
    printf("wb_vrepdbms::renameObject, exception %s\n", e.what().c_str());
    m_ohead.clear();
    *sts = LDH__DBERROR;
    return false;
  }
}

bool wb_vrepdbms::writeAttribute(pwr_tStatus *sts, wb_orep *orp, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  try {
    int rc = 0;
    m_ohead.get(m_db->m_txn, orp->oid());
    *sts = LDH__SUCCESS;
    pwr_tTime time;
    clock_gettime(CLOCK_REALTIME, &time);

    switch (bix) {
    case pwr_eBix_rt:
    {

      wb_dbms_rbody rb(m_db, m_ohead.oid());
      rc = rb.upd(m_db->m_txn, offset, size, p);
      m_ohead.rbTime(time);
      m_ohead.upd(m_db->m_txn);
      if (rc)
        printf("wb_vrepdbms::writeAttribute rb.upd rc %d\n", rc);
      break;
    }
    case pwr_eBix_dev:
    {

      wb_dbms_dbody db(m_db, m_ohead.oid());
      rc = db.upd(m_db->m_txn, offset, size, p);
      m_ohead.dbTime(time);
      m_ohead.upd(m_db->m_txn);
      if (rc)
        printf("wb_vrepdbms::writeAttribute db.upd rc %d\n", rc);
      break;
    }
    default:
      break;
    }

    return true;
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return false;
  }
}

void *wb_vrepdbms::readAttribute(pwr_tStatus *sts, const wb_orep *orp, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  int rc = 0;
  
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    *sts = LDH__SUCCESS;

    switch (bix) {
    case pwr_eBix_rt:
    {

      wb_dbms_rbody rb(m_db, m_ohead.oid());
      rc = rb.get(m_db->m_txn, offset, size, p);
      break;
    }
    case pwr_eBix_dev:
    {

      wb_dbms_dbody db(m_db, m_ohead.oid());
      rc = db.get(m_db->m_txn, offset, size, p);
      break;
    }
    default:
      p = 0;
    }

    if (rc)
      *sts = LDH__NOSUCHATTR;
    return p;
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHATTR;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

void *wb_vrepdbms::readBody(pwr_tStatus *sts, const wb_orep *orp, pwr_eBix bix, void *p)
{
  int rc = 0;
  
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    *sts = LDH__SUCCESS;

    switch (bix) {
    case pwr_eBix_rt:
    {
      wb_dbms_rbody rb(m_db, m_ohead.oid());
      rc = rb.get(m_db->m_txn, 0, m_ohead.rbSize(), p);
      break;
    }
    case pwr_eBix_dev:
    {
      wb_dbms_dbody db(m_db, m_ohead.oid());
      rc = db.get(m_db->m_txn, 0, m_ohead.dbSize(), p);
      break;
    }
    default:
      p = 0;
    }

    if (rc)
      *sts = LDH__NOSUCHBODY;
    return p;
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHBODY;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

bool wb_vrepdbms::writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p)
{
  try {
    int rc = 0;
    m_ohead.get(m_db->m_txn, o->oid());
    *sts = LDH__SUCCESS;
    pwr_tTime time;
    clock_gettime(CLOCK_REALTIME, &time);

    switch (bix) {
    case pwr_eBix_rt:
    {

      wb_dbms_rbody rb(m_db, m_ohead.oid());
      rc = rb.upd(m_db->m_txn, 0, m_ohead.rbSize(), p);
      if (rc)
        printf("wb_vrepdbms::writeBody rb.upd rc %d\n", rc);
      m_ohead.rbTime(time);
      m_ohead.upd(m_db->m_txn);
      break;
    }
    case pwr_eBix_dev:
    {

      wb_dbms_dbody db(m_db, m_ohead.oid());
      rc = db.upd(m_db->m_txn, 0, m_ohead.dbSize(), p);
      if (rc)
        printf("wb_vrepdbms::writeBody db.upd rc %d\n", rc);
      m_ohead.dbTime(time);
      m_ohead.upd(m_db->m_txn);
      break;
    }
    default:
      break;
    }

    return true;
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return false;
  }
}

wb_orep *wb_vrepdbms::ancestor(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__NYI;
  return 0;
}

pwr_tCid wb_vrepdbms::cid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).cid();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

pwr_tTime wb_vrepdbms::ohTime(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).ohTime();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    pwr_tTime t = {0, 0};

    printf("vrepdbms: %s\n", e.what().c_str());
    return t;
  }
}

pwr_tTime wb_vrepdbms::rbTime(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).rbTime();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    pwr_tTime t = {0, 0};
    
    printf("vrepdbms: %s\n", e.what().c_str());
    return t;
  }
}

pwr_tTime wb_vrepdbms::dbTime(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).dbTime();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    pwr_tTime t = {0, 0};
    
    printf("vrepdbms: %s\n", e.what().c_str());
    return t;
  }
}

pwr_mClassDef wb_vrepdbms::flags(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).flags();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    pwr_mClassDef flags; flags.m = 0;
    return flags;
  }
}

pwr_tVid wb_vrepdbms::vid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).vid();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return pwr_cNVid;
  }
}

pwr_tOid wb_vrepdbms::oid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).oid();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return pwr_cNOid;
  }
}

pwr_tOix wb_vrepdbms::oix(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).oix();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return pwr_cNOix;
  }
}

pwr_tOid wb_vrepdbms::poid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).poid();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return pwr_cNOid;
  }
}

pwr_tOid wb_vrepdbms::foid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).foid();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return pwr_cNOid;
  }
}

pwr_tOid wb_vrepdbms::loid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).loid();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return pwr_cNOid;
  }
}

pwr_tOid wb_vrepdbms::aoid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).aoid();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return pwr_cNOid;
  }
}

pwr_tOid wb_vrepdbms::boid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).boid();
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return pwr_cNOid;
  }
}

wb_orep *wb_vrepdbms::parent(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, m_ohead.get(m_db->m_txn, orp->oid()).poid());
    if ( m_ohead.oid().oix == 0) {
      *sts = LDH__NO_PARENT;
      return 0;
    }
    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::after(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid aoid = m_ohead.get(m_db->m_txn, orp->oid()).aoid();
    if (cdh_ObjidIsNull(aoid)) {
      *sts = LDH__NO_SIBLING;
      return 0;
    }

    m_ohead.get(m_db->m_txn, aoid);
    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::before(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid boid = m_ohead.get(m_db->m_txn, orp->oid()).boid();
    if (cdh_ObjidIsNull(boid)) {
      *sts = LDH__NO_SIBLING;
      return 0;
    }

    m_ohead.get(m_db->m_txn, boid);
    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::first(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;

  try {
    pwr_tOid foid = m_ohead.get(m_db->m_txn, orp->oid()).foid();
    if (cdh_ObjidIsNull(foid)) {
      *sts = LDH__NO_CHILD;
      return 0;
    }

    m_ohead.get(m_db->m_txn, foid);
    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::child(pwr_tStatus *sts, const wb_orep *orp, wb_name &name)
{
  *sts = LDH__SUCCESS;

  try {
    wb_dbms_name n(m_db, m_db->m_txn, orp->oid(), name);
    m_ohead.get(m_db->m_txn, n.oid());
    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::last(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid loid = m_ohead.get(m_db->m_txn, orp->oid()).loid();
    if (cdh_ObjidIsNull(loid)) {
      *sts = LDH__NO_CHILD;
      return 0;
    }

    m_ohead.get(m_db->m_txn, loid);
    return new (this) wb_orepdbms(&m_ohead.m_o);
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}


wb_orep *wb_vrepdbms::object(pwr_tStatus *sts, pwr_tCid cid)
{
  *sts = LDH__SUCCESS;
  try {
    wb_dbms_class c(m_db, m_db->m_txn, cid);
    pwr_tOid oid;
    oid.vid = m_vid;
    oid.oix = 0;
    if (c.succ(oid)) {
      if (c.cid() == cid) {
        m_ohead.get(m_db->m_txn, c.oid());
        return new (this) wb_orepdbms(&m_ohead.m_o);
      }
    }
    *sts =  LDH__NOSUCHOBJ;
    return 0;
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::next(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    wb_dbms_class c(m_db, m_db->m_txn, m_ohead.cid());
    if (c.succ(m_ohead.oid())) {
      if (c.cid() == m_ohead.cid()) {
        m_ohead.get(m_db->m_txn, c.oid());
        return new (this) wb_orepdbms(&m_ohead.m_o);
      }
    }
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::nextClass(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    wb_dbms_class c(m_db, m_db->m_txn, m_ohead.cid());
    if (c.succClass(m_ohead.cid())) {
      if ( c.cid() == m_ohead.cid()) {
        m_ohead.get(m_db->m_txn, c.oid());
        return new (this) wb_orepdbms(&m_ohead.m_o);
      }
    }
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms: %s\n", e.what().c_str());
    return 0;
  }
}

wb_orep *wb_vrepdbms::previous(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    wb_dbms_class c(m_db, m_db->m_txn, m_ohead.cid());
    if (c.pred(m_ohead.oid())) {
      m_ohead.get(m_db->m_txn, c.oid());
      return new (this) wb_orepdbms(&m_ohead.m_o);
    } else {
      //*sts = LDH__?;
      return 0;
    }
  }
  catch (wb_dbms_error &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdbms %s\n", e.what().c_str());
    return 0;
  }
}

void wb_vrepdbms::adopt(wb_dbms_txn *txn, wb_dbms_ohead &o, wb_destination &dest)
{
  int rc = 0;
  wb_dbms_ohead d(m_db, txn, dest.oid());

  switch (dest.code()) {
  case ldh_eDest_IntoFirst:
    o.poid(d.oid());
    o.boid(pwr_cNOid);
    o.aoid(d.foid());
    break;
  case ldh_eDest_IntoLast:
    o.poid(d.oid());
    o.aoid(pwr_cNOid);
    o.boid(d.loid());
    break;
  case ldh_eDest_After:
    o.poid(d.poid());
    o.boid(d.oid());
    o.aoid(d.aoid());
    break;
  case ldh_eDest_Before:
    o.poid(d.poid());
    o.boid(d.boid());
    o.aoid(d.oid());
    break;
  default:
    ;// throw
  }


  wb_dbms_ohead p(m_db, txn, o.poid());

  if (o.boid().vid != pwr_cNVid) {
    wb_dbms_ohead b(m_db, txn, o.boid());
    b.aoid(o.oid());
    rc = b.upd(txn);
    if (rc)
      printf("wb_vrepdbms:adopt, b.upd rc %d\n", rc);
  } else {
    p.foid(o.oid());
  }

  if (o.aoid().vid != pwr_cNVid) {
    wb_dbms_ohead a(m_db, txn, o.aoid());
    a.boid(o.oid());
    rc = a.upd(txn);
    if (rc)
      printf("wb_vrepdbms:adopt, a.upd rc %d\n", rc);
  } else {
    p.loid(o.oid());
  }

  rc = p.upd(txn);
  if (rc)
    printf("wb_vrepdbms:adopt, p.upd rc %d\n", rc);

  wb_dbms_name n(m_db, o.oid(), o.poid(), o.normname());
  rc = n.ins(txn);
  if (rc) {
    printf("wb_vrepdbms:adopt, n.ins rc %d\n", rc);
    throw wb_error(LDH__NAMALREXI);
  }

}

void wb_vrepdbms::unadopt(wb_dbms_txn *txn, wb_dbms_ohead &o)
{
  int rc = 0;
  wb_dbms_ohead p(m_db, txn, o.poid());

  if (o.boid().vid != pwr_cNVid) {
    wb_dbms_ohead b(m_db, txn, o.boid());
    b.aoid(o.aoid());
    rc = b.upd(txn);
    if (rc)
      printf("wb_vrepdbms:unadopt, b.upd rc %d\n", rc);
  } else {
    p.foid(o.aoid());
  }

  if (o.aoid().vid != pwr_cNVid) {
    wb_dbms_ohead a(m_db, txn, o.aoid());
    a.boid(o.boid());
    rc = a.upd(txn);
    if (rc)
      printf("wb_vrepdbms:unadopt, a.upd rc %d\n", rc);
  } else {
    p.loid(o.boid());
  }

  wb_dbms_name n(m_db, o.oid(), o.poid(), o.normname());
  rc = n.del(txn);
  if (rc)
    printf("wb_vrepdbms:unadopt, n.del rc %d\n", rc);

  o.poid(pwr_cNOid);
  o.aoid(pwr_cNOid);
  o.boid(pwr_cNOid);

  rc = p.upd(txn);
  if (rc)
    printf("wb_vrepdbms:unadopt, p.upd rc %d\n", rc);

  rc = o.upd(txn);
  if (rc)
    printf("wb_vrepdbms:unadopt, o.upd rc %d\n", rc);
}

bool wb_vrepdbms::exportVolume(wb_import &i)
{
  return i.importVolume(*this);
}

bool wb_vrepdbms::exportHead(wb_import &i)
{
  wb_dbms_ohead *op = new wb_dbms_ohead(m_db);

  op->iter(i);

  delete(op);
  
  return true;
}

bool wb_vrepdbms::exportRbody(wb_import &i)
{
  wb_dbms_rbody *rp = new wb_dbms_rbody(m_db);

  rp->iter(i);

  delete(rp);
  
  return true;
}

bool wb_vrepdbms::exportDbody(wb_import &i)
{
  wb_dbms_dbody *dp = new wb_dbms_dbody(m_db);

  dp->iter(i);

  delete(dp);
  
  return true;
}

bool wb_vrepdbms::exportDocBlock(wb_import &i)
{
  return false;
}

bool wb_vrepdbms::exportMeta(wb_import &i)
{
  return false;
}


bool wb_vrepdbms::exportTree(wb_treeimport &i, pwr_tOid oid)
{
  return exportTreeHelper(i, oid, true);
}

bool wb_vrepdbms::exportTreeHelper(wb_treeimport &i, pwr_tOid oid, bool isRoot)
{
  m_ohead.get(m_db->m_txn, oid);

  void *rbody = 0;
  void *dbody = 0;
  int rbSize = m_ohead.rbSize();
  int dbSize = m_ohead.dbSize();

  if (rbSize) {
    rbody = (void *)malloc(rbSize);
    wb_dbms_rbody rb(m_db, oid);
    int rc = rb.get(m_db->m_txn, 0, rbSize, rbody);
    if (rc)
      printf("wb_vrepdbms::exportTreeObject, rb.get: %d\n", rc);
  }

  if (dbSize) {
    dbody = (void *)malloc(dbSize);
    wb_dbms_dbody db(m_db, oid);
    int rc = db.get(m_db->m_txn, 0, dbSize, dbody);
    if (rc)
      printf("wb_vrepdbms::exportTreeObject, db.get: %d\n", rc);
  }

  pwr_mClassDef flags = m_ohead.flags(); // flags.m = 0; // Fix !!!
  if (isRoot) {
    i.importTreeObject(m_merep, m_ohead.oid(), m_ohead.cid(), pwr_cNOid, pwr_cNOid, m_ohead.name(), flags,
                       m_ohead.rbSize(), m_ohead.dbSize(), rbody, dbody);
  } else {
    i.importTreeObject(m_merep, m_ohead.oid(), m_ohead.cid(), m_ohead.poid(), m_ohead.boid(), m_ohead.name(), flags,
                       m_ohead.rbSize(), m_ohead.dbSize(), rbody, dbody);
  }

  if (rbSize)
    free(rbody);
  if (dbSize)
    free(dbody);

  oid = m_ohead.foid();
  while (cdh_ObjidIsNotNull(oid)) {
    exportTreeHelper(i, oid, false);
    m_ohead.get(m_db->m_txn, oid);
    oid = m_ohead.aoid();
  }

  return true;
}

wb_orepdbms *wb_vrepdbms::new_wb_orepdbms(size_t size)
{
  wb_orepdbms*o = (wb_orepdbms*) calloc(1, size);
  o->m_vrep = this;
  return o;
}

void wb_vrepdbms::delete_wb_orepdbms(void *p)
{
  free(p);
}

bool wb_vrepdbms::importPasteObject(pwr_tOid doid, ldh_eDest destcode,
                                      bool keepoid, pwr_tOid oid,
                                      pwr_tCid cid, pwr_tOid poid,
                                      pwr_tOid boid, const char *name, pwr_mClassDef flags,
                                      size_t rbSize, size_t dbSize, void *rbody, void *dbody,
                                      pwr_tOid *roid)
{
  pwr_tStatus sts;
  static pwr_tTime oTime;

  if (cdh_ObjidIsNull(poid) && cdh_ObjidIsNull(boid)) {
    if (m_oid_th) {
      tree_DeleteTable(&sts, m_oid_th);
    }
    importTranslationTableClear();
    importSetSourceVid(oid.vid);
    m_oid_th = tree_CreateTable(&sts, sizeof(pwr_tOid), offsetof(sOentry, o_oid), sizeof(sOentry), 1000, tree_Comp_oid);
    m_poep = (sOentry *)tree_Insert(&sts, m_oid_th, &poid);

    clock_gettime(CLOCK_REALTIME, &oTime);

    memset(&m_destination, 0, sizeof(m_destination));

    m_destination.oid = doid;

    if (cdh_ObjidIsNotNull(doid)) {
      try {
        m_ohead.get(m_db->m_txn, doid);
      }
      catch (wb_dbms_error &e) {
        printf("vrepdbms desination (%d.%d) does not exist: %s\n", doid.vid, doid.oix, e.what().c_str());
        throw wb_error(LDH__PASTEINCON);
      }
    }

    switch (destcode) {
    case ldh_eDest_After:
      m_destination.poid = m_ohead.poid();
      m_destination.foid = m_ohead.oid();
      m_destination.loid = m_ohead.aoid();
      break;
    case ldh_eDest_Before:
      m_destination.poid = m_ohead.poid();
      m_destination.foid = m_ohead.boid();
      m_destination.loid = m_ohead.oid();
      break;
    case ldh_eDest_IntoFirst:
      m_destination.poid = m_ohead.oid();
      m_destination.loid = m_ohead.foid();
      break;
    case ldh_eDest_IntoLast:
      m_destination.poid = m_ohead.oid();
      m_destination.foid = m_ohead.loid();
      break;
    default:
      throw wb_error(LDH__NYI);
    }

    m_poep->n_oid = m_destination.poid;
  }

  sOentry *oep = (sOentry *)tree_Insert(&sts, m_oid_th, &oid);
  sOentry *poep = oep->parent = (sOentry *)tree_Insert(&sts, m_oid_th, &poid);

  if (cdh_ObjidIsNotNull(boid)) {
    oep->before = (sOentry *)tree_Insert(&sts, m_oid_th, &boid);
    poep->last = oep->before->after = oep;
  } else {
    poep->first = poep->last = oep;
  }

  if (keepoid) {
    oep->n_oid = m_db->new_oid(m_db->m_txn, oid);
  } else {
    oep->n_oid = m_db->new_oid(m_db->m_txn);
  }

  importTranslationTableInsert(oid.oix, oep->n_oid.oix);

  wb_name n(name);

  m_db->importHead(oep->n_oid, cid, oep->parent->n_oid, pwr_cNOid, pwr_cNOid, pwr_cNOid, pwr_cNOid,
                   name, n.normName(), flags, oTime, oTime, oTime, rbSize, dbSize);

  if (rbSize > 0) {
    m_db->importRbody(oep->n_oid, rbSize, rbody);
  }
  if (dbSize > 0) {
    m_db->importDbody(oep->n_oid, dbSize, dbody);
  }

  return true;
}

bool wb_vrepdbms::importPaste()
{
  pwr_tStatus sts;
  pwr_tTime oTime;

  clock_gettime(CLOCK_REALTIME, &oTime);

  sOentry *oep = (sOentry*)tree_Minimum(&sts, m_oid_th);
  while (oep) {
    if (oep != m_poep) {
      m_ohead.get(m_db->m_txn, oep->n_oid);
      m_ohead.poid(oep->parent->n_oid);

      if (oep->before)
        m_ohead.boid(oep->before->n_oid);
      if (oep->after)
        m_ohead.aoid(oep->after->n_oid);
      if (oep->first)
        m_ohead.foid(oep->first->n_oid);
      if (oep->last)
        m_ohead.loid(oep->last->n_oid);

      m_ohead.ohTime(oTime);
      m_ohead.ins(m_db->m_txn);
    }

    oep = (sOentry*)tree_Successor(&sts, m_oid_th, oep);
  }

  if (cdh_ObjidIsNull(m_destination.foid)) {
    m_ohead.get(m_db->m_txn, m_destination.poid);
    m_ohead.foid(m_poep->first->n_oid);
    m_ohead.ohTime(oTime);
    m_ohead.upd(m_db->m_txn);

    if (cdh_ObjidIsNotNull(m_destination.loid)) {
      m_ohead.get(m_db->m_txn, m_destination.loid);
      m_ohead.boid(m_poep->last->n_oid);
      m_ohead.ohTime(oTime);
      m_ohead.upd(m_db->m_txn);

      m_ohead.get(m_db->m_txn, m_poep->last->n_oid);
      m_ohead.aoid(m_destination.loid);
      m_ohead.ohTime(oTime);
      m_ohead.upd(m_db->m_txn);
    }
  }
  if (cdh_ObjidIsNull(m_destination.loid)) {
    m_ohead.get(m_db->m_txn, m_destination.poid);
    m_ohead.loid(m_poep->last->n_oid);
    m_ohead.ohTime(oTime);
    m_ohead.upd(m_db->m_txn);

    if (cdh_ObjidIsNotNull(m_destination.foid)) {
      m_ohead.get(m_db->m_txn, m_destination.foid);
      m_ohead.aoid(m_poep->first->n_oid);
      m_ohead.ohTime(oTime);
      m_ohead.upd(m_db->m_txn);

      m_ohead.get(m_db->m_txn, m_poep->first->n_oid);
      m_ohead.boid(m_destination.foid);
      m_ohead.ohTime(oTime);
      m_ohead.upd(m_db->m_txn);
    }
  }
  if (cdh_ObjidIsNotNull(m_destination.foid) && cdh_ObjidIsNotNull(m_destination.loid)) {
    m_ohead.get(m_db->m_txn, m_destination.foid);
    m_ohead.aoid(m_poep->first->n_oid);
    m_ohead.ohTime(oTime);
    m_ohead.upd(m_db->m_txn);

    m_ohead.get(m_db->m_txn, m_poep->first->n_oid);
    m_ohead.boid(m_destination.foid);
    m_ohead.ohTime(oTime);
    m_ohead.upd(m_db->m_txn);

    m_ohead.get(m_db->m_txn, m_destination.loid);
    m_ohead.boid(m_poep->last->n_oid);
    m_ohead.ohTime(oTime);
    m_ohead.upd(m_db->m_txn);

    m_ohead.get(m_db->m_txn, m_poep->last->n_oid);
    m_ohead.aoid(m_destination.loid);
    m_ohead.ohTime(oTime);
    m_ohead.upd(m_db->m_txn);
  }

  importUpdateTree(this);
  importTranslationTableClear();
  tree_DeleteTable(&sts, m_oid_th);
  m_oid_th = 0;

  return true;
}

pwr_tStatus wb_vrepdbms::checkMeta()
{
  pwr_tStatus sts = LDH__SUCCESS;
  pwr_tStatus db_sts = LDH__SUCCESS;
  int totalObjectCount = 0;
  int nAref = 0;
  int nClass = 0;
  
  m_class_th = tree_CreateTable(&sts, sizeof(pwr_tCid), offsetof(sClass, cid), sizeof(sClass), 1000, tree_Comp_cid);
  m_classchk_th = tree_CreateTable(&sts, sizeof(pwr_tCid), offsetof(sClassChk, cid), sizeof(sClassChk), 1000, tree_Comp_cid);
  m_aref_th = tree_CreateTable(&sts, sizeof(sArefKey), offsetof(sAref, key), sizeof(sAref), 1000, comp_aref);
  m_attribute_th = tree_CreateTable(&sts, sizeof(sAttributeKey), offsetof(sAttribute, key), sizeof(sAttribute), 1000, comp_attribute);
  
  try {
    wb_dbms_class_iterator ip(m_db);
  
    for (ip.first(); !ip.atEnd(); ip.succClass()) {
      nClass += checkClass(ip.cid());
      checkAttributes(ip.cid());
    }
    for (ip.first(); !ip.atEnd(); ip.succObject()) {
      pwr_tCid cid = ip.cid();
      sClass *cp = (sClass *)tree_Find(&sts, m_class_th, &cid);

      if (!cp)
        continue;
      
      cp->count++;      
      
      if (time_IsNull(&cp->n_time))
        continue;
    }

  } catch (wb_dbms_error &e) {
    printf("***wb_dbms_error vrepdbms: %s\n", e.what().c_str());
    db_sts = LDH__DBERROR;
  } catch (wb_error &e) {
    printf("***wb_error vrepdbms: %s\n", e.what().c_str());
    db_sts = LDH__DBERROR;
  }


  for (
    sClass *cp = (sClass *)tree_Minimum(&sts, m_class_th);
    cp != NULL;
    cp = (sClass *)tree_Successor(&sts, m_class_th, cp)
    ) {
    char o_timbuf[32];
    char n_timbuf[32];
    
    if (cp->count > 0) {
      char buff[256];
      
      if (time_IsNull(&cp->n_time)) {

        time_AtoAscii(&cp->o_time, time_eFormat_DateAndTime, o_timbuf, sizeof(o_timbuf));
        sprintf(buff, "Class \"%s\" [%s], %d object%s, does not exist in global scope",
                cp->name, o_timbuf, cp->count, (cp->count == 1 ? "" : "s"));
        MsgWindow::message('W', buff);

      } else {
        
        time_AtoAscii(&cp->o_time, time_eFormat_NumDateAndTime, o_timbuf, sizeof(o_timbuf));
        time_AtoAscii(&cp->n_time, time_eFormat_NumDateAndTime, n_timbuf, sizeof(n_timbuf));

        sprintf(buff, "Class \"%s\" [%s], %d object%s, can be updated to [%s]",
                cp->name, o_timbuf, cp->count, (cp->count == 1 ? "" : "s"), n_timbuf);
        MsgWindow::message('W', buff, msgw_ePop_No);
        totalObjectCount += cp->count;
      }
    }
  }
  if (ODD(db_sts) && tree_Cardinality(&sts, m_class_th) != 0) {
    char buff[256];

    sprintf(buff, "A total of %d object%s of %d class%s, and  %d attribute reference%s can be updated",
            totalObjectCount, (totalObjectCount == 1 ? "" : "s"),
            tree_Cardinality(&sts, m_class_th), (tree_Cardinality(&sts, m_class_th) == 1 ? "" : "es"),
            nAref, (nAref == 1 ? "" : "s"));

    MsgWindow::message('W', buff, msgw_ePop_No);
    MsgWindow::message( 'W', "Classvolumes need update, execute 'Function->UpdateClasses'");
  }  

  tree_DeleteTable(&sts, m_attribute_th);
  tree_DeleteTable(&sts, m_aref_th);
  tree_DeleteTable(&sts, m_classchk_th);
  tree_DeleteTable(&sts, m_class_th);

  return sts;
}



static int
comp_attribute(tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
  sAttribute *xp = (sAttribute *) x; 
  sAttribute *yp = (sAttribute *) y;
  
  if (xp->key.cid > yp->key.cid)
    return 1;

  if (xp->key.cid < yp->key.cid)
    return -1;

  if (xp->key.bix > yp->key.bix)
    return 1;

  if (xp->key.bix < yp->key.bix)
    return -1;

  if (xp->key.oStart > yp->key.oEnd)
    return 1;

  if (xp->key.oEnd < yp->key.oStart)
    return -1;

  return 0;
}

static int
comp_aref(tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
  sAref *xp = (sAref *) x; 
  sAref *yp = (sAref *) y;

  if (xp->key.cid > yp->key.cid)
    return 1;

  if (xp->key.cid < yp->key.cid)
    return -1;

  if (xp->key.bix > yp->key.bix)
    return 1;

  if (xp->key.bix < yp->key.bix)
    return -1;

  if (xp->key.offset > yp->key.offset)
    return 1;

  if (xp->key.offset < yp->key.offset)
    return -1;

  return 0;
}

int wb_vrepdbms::updateArefs(pwr_tOid oid, pwr_tCid cid)
{
  pwr_tStatus sts;
  sArefKey ak;
  int nAref[2] = {0, 0};
  int rbSize = 0;
  int dbSize = 0;
  int rrc = 0;
  int drc = 0;
  
  ak.cid = cid;
  ak.bix = pwr_eBix__;
  ak.offset = 0;
  char *rp = 0;
  char *dp = 0;
  
  sAref *ap = (sAref *)tree_FindSuccessor(&sts, m_aref_th, &ak);

  if (ap == 0 || ap->key.cid != cid)
    return 0;

  try {
    wb_dbms_ohead ohead(m_db, m_db->m_txn, oid);
    rbSize = ohead.rbSize();
    dbSize = ohead.dbSize();
    wb_dbms_rbody rb(m_db, ohead.oid());
    wb_dbms_dbody db(m_db, ohead.oid());
    
    if (rbSize) {      
      rp = (char *)calloc(1, rbSize);
      rrc = rb.get(m_db->m_txn, 0, rbSize, rp);
    }
    if (dbSize) {
      dp = (char *)calloc(1, dbSize);
      drc = db.get(m_db->m_txn, 0, dbSize, dp);
    }
  } catch (wb_dbms_error &e) {
    ap = 0;
  } catch (wb_error &e) {
    ap = 0;
  }


  while (ap != 0 && ap->key.cid == cid) {
    char *p = 0;
    
    if (ap->key.bix == pwr_eBix_rt)
      p = rp;
    else if (ap->key.bix == pwr_eBix_dev)
      p = dp;
    else
      p = 0;
    
    if (p != 0) {
        
      try {
          
        pwr_sAttrRef *arp = (pwr_sAttrRef *)(p + ap->key.offset);        
        wb_db_ohead aohead(m_db, m_db->m_txn, arp->Objid);
        wb_cdrep *n_cdrep = m_erep->merep()->cdrep(&sts, aohead.cid());
	wb_bdrep *n_bdrep;
        if (EVEN(sts))
	  printf("cdrep sts %d", sts);
	else
	  n_bdrep = n_cdrep->bdrep(&sts, ap->key.bix);
        if (EVEN(sts)) {
	  ap = (sAref *)tree_FindSuccessor(&sts, m_aref_th, &ap->key);
	  continue;
	}

	if ( arp->Flags.b.Object) {
	  // Check if rbody size of changed
	  pwr_tCid cid = aohead.cid();
	  sClass *cp = (sClass *)tree_Find(&sts, m_class_th, &cid);

	  if ( cp && cp->n_rbsize != cp->o_rbsize) {
	    arp->Size = cp->n_rbsize;
	    nAref[ap->key.bix - 1]++;
	  }
	}
	else {
	  sAttributeKey k;
	  k.cid = aohead.cid();
	  k.bix = ap->key.bix;
	  k.oStart = arp->Offset;
	  k.oEnd = arp->Offset + arp->Size - 1;
        
	  sAttribute *cap = (sAttribute *)tree_Find(&sts, m_attribute_th, &k);
	  if (cap != 0) {
	    nAref[ap->key.bix - 1]++;
	    if (arp->Size > cap->o.aref.Size) {
	      arp->Offset = 0;
	      arp->Size = n_bdrep->size();
	    } else if (arp->Offset == cap->o.aref.Offset && arp->Size == cap->o.aref.Size) {  
	      arp->Offset = cap->n.aref.Offset;
	      arp->Size = cap->n.aref.Size;
	    } else if (cap->o.aref.Flags.b.Array) {
	      pwr_tUInt32 oElementSize = cap->o.aref.Size / cap->o.nElement;
	      pwr_tUInt32 oOffset = arp->Offset - cap->o.aref.Offset;
	      pwr_tUInt32 index = oOffset / oElementSize;
	      pwr_tUInt32 nElementSize = cap->n.aref.Size / cap->n.nElement;
	      
	      if (index >= cap->n.nElement) {
		index = cap->n.nElement - 1;
	      }
	      arp->Offset = cap->n.aref.Offset + (index * nElementSize);
	      arp->Size = nElementSize;
	    }          
	  }
	}        
      } catch (wb_dbms_error &e) {
        //printf("wb_dbms_error vrepdbmsupdateArefs 2: %s, oid: %d.%d, cid: %d \n", e.what().c_str(), oid.vid, oid.oix, cid);
      } catch (wb_error &e) {
        //printf("wb_error vrepdbmsupdateArefs 2: oid: %d.%d, cid: %d %s\n", oid.vid, oid.oix, cid, e.what().c_str().c_str());
      }
    }
      
      
    ap = (sAref *)tree_FindSuccessor(&sts, m_aref_th, &ap->key);
  }

  try {
    wb_dbms_ohead ohead(m_db, m_db->m_txn, oid);
    wb_dbms_rbody rb(m_db, ohead.oid());
    wb_dbms_dbody db(m_db, ohead.oid());

    if (rbSize && nAref[0]) {
      rb.upd(m_db->m_txn, 0, rbSize, rp);
    }
    if (dbSize && nAref[1]) {
      db.upd(m_db->m_txn, 0, dbSize, dp);
    }
  } catch (wb_dbms_error &e) {
    //printf("wb_dbms_error vrepdbmsupdateArefs body.put: oid: %d.%d, cid: %d %s\n", oid.vid, oid.oix, cid, e.what().c_str());
  } catch (wb_error &e) {
    //printf("wb_error vrepdbmsupdateArefs body.put: oid: %d.%d, cid: %d %s\n", oid.vid, oid.oix, cid, e.what().c_str().c_str());
  }
    
  if (rp)
    free(rp);
  if (dp)
    free(dp);

  return nAref[0] + nAref[1];
}

pwr_tStatus wb_vrepdbms::updateMeta()
{
  int rc = 0;
  pwr_tStatus sts = LDH__SUCCESS;
  pwr_tStatus db_sts = LDH__SUCCESS;
  int nAref = 0;
  int nObject = 0;
  int nClass = 0;
  int totalObjectCount = 0;
  
  m_aref_th = tree_CreateTable(&sts, sizeof(sArefKey), offsetof(sAref, key), sizeof(sAref), 1000, comp_aref);
  m_class_th = tree_CreateTable(&sts, sizeof(pwr_tCid), offsetof(sClass, cid), sizeof(sClass), 1000, tree_Comp_cid);
  m_classchk_th = tree_CreateTable(&sts, sizeof(pwr_tCid), offsetof(sClassChk, cid), sizeof(sClassChk), 1000, tree_Comp_cid);
  m_attribute_th = tree_CreateTable(&sts, sizeof(sAttributeKey), offsetof(sAttribute, key), sizeof(sAttribute), 1000, comp_attribute);
  

  try {
    wb_dbms_class_iterator ip(m_db);
  
    for (ip.first(); !ip.atEnd(); ip.succClass()) {
      try {
	nClass += checkClass(ip.cid());
	checkAttributes(ip.cid());
      } catch (wb_error &e) {
	printf("vrepdbms::updateMeta: %s\n", e.what().c_str());	
      }
    }
    for (ip.first(); !ip.atEnd(); ip.succObject()) {
      pwr_tCid cid = ip.cid();
      sClass *cp = (sClass *)tree_Find(&sts, m_class_th, &cid);

      if (!cp)
        continue;
      
      cp->count++;      
      
      if (time_IsNull(&cp->n_time))
        continue;

      nAref   += updateArefs(ip.oid(), ip.cid());
      nObject += updateObject(ip.oid(), ip.cid());
    }
  } catch (wb_dbms_error &e) {
    printf("vrepdbms:updateMeta: %s\n", e.what().c_str());
    db_sts = LDH__DBERROR;
  } catch (wb_error &e) {
    printf("vrepdbms:updateMeta: %s\n", e.what().c_str());
    db_sts = LDH__DBERROR;
  }
  
  for (
    sClass *cp = (sClass *)tree_Minimum(&sts, m_class_th);
    cp != NULL;
    cp = (sClass *)tree_Successor(&sts, m_class_th, cp)
    ) {
    char o_timbuf[32];
    char n_timbuf[32];
    
    if (cp->count > 0) {
      char buff[256];
      
      if (time_IsNull(&cp->n_time)) {

        time_AtoAscii(&cp->o_time, time_eFormat_DateAndTime, o_timbuf, sizeof(o_timbuf));
        sprintf(buff, "Class \"%s\" [%s], %d object%s, does not exist in global scope",
                cp->name, o_timbuf, cp->count, (cp->count == 1 ? "" : "s"));
        MsgWindow::message('W', buff);

      } else {
        
        time_AtoAscii(&cp->o_time, time_eFormat_NumDateAndTime, o_timbuf, sizeof(o_timbuf));
        time_AtoAscii(&cp->n_time, time_eFormat_NumDateAndTime, n_timbuf, sizeof(n_timbuf));

        sprintf(buff, "Class \"%s\" [%s], %d object%s, %s updated to [%s]",
                cp->name, o_timbuf, cp->count, (cp->count == 1 ? "" : "s"),
                (cp->count == 1 ? "was" : "were"), n_timbuf);	  
        MsgWindow::message('I', buff, msgw_ePop_No);
        totalObjectCount += cp->count;
      }
    }
  }
  if (ODD(db_sts) && tree_Cardinality(&sts, m_class_th) != 0 && totalObjectCount > 0) {
    char buff[256];

    commit(&rc);

    if (rc) {
      sprintf(buff, "A total of %d object%s of %d classe%s %s updated, but could not be saved to database.",
              nObject, (nObject == 1 ? "" : "s"), tree_Cardinality(&sts, m_class_th), 
              (tree_Cardinality(&sts, m_class_th) == 1 ? "" : "s"), 
              (nObject == 1 ? "was" : "were"));
      MsgWindow::message(co_error(rc), buff);
      db_sts = LDH__DBERROR;
    } else {
      m_merep->copyFiles(m_fileName, m_erep->merep());
      delete m_merep;
      m_merep = new wb_merep(m_fileName, m_erep, this);

      sprintf(buff, "A total of %d object%s of %d class%s, and  %d attribute reference%s, %s updated",
              totalObjectCount, (totalObjectCount == 1 ? "" : "s"),
              tree_Cardinality(&sts, m_class_th), (tree_Cardinality(&sts, m_class_th) == 1 ? "" : "es"),
              nAref, (nAref == 1 ? "" : "s"), (nAref == 1 ? "was" : "were"));

      MsgWindow::message('I', buff);
    }
  }
  else if ( ODD(db_sts)) {
    // No changes, update file only
    m_merep->copyFiles(m_fileName, m_erep->merep());
    delete m_merep;
    m_merep = new wb_merep(m_fileName, m_erep, this);
  }
  
  tree_DeleteTable(&sts, m_attribute_th);
  tree_DeleteTable(&sts, m_aref_th);
  tree_DeleteTable(&sts, m_classchk_th);
  tree_DeleteTable(&sts, m_class_th);

  return sts;
}

/* Check if class is changed, return 1 if it is.  */
int
wb_vrepdbms::checkClass(pwr_tCid cid)
{
  wb_cdrep *o_crep = 0;
  wb_cdrep *n_crep = 0;
  pwr_tTime o_time = {0, 0};
  pwr_tTime n_time = {0, 0};
  pwr_tStatus sts;
  int n = 0;
  
  // Insert in check class table
  tree_Insert(&sts, m_classchk_th, &cid);

  o_crep = m_merep->cdrep(&sts, cid);
  if (o_crep == 0) {
    // Class does not exist
    return 0;    
  }
  
  o_time = o_crep->ohTime();

  n_crep = m_erep->merep()->cdrep(&sts, cid);      	
  if (n_crep == 0)
    throw wb_error( LDH__NOSUCHCLASS);

  n_time = n_crep->ohTime();

  if (time_Acomp(&o_time, &n_time) != 0) {
    n = 1;
  }
  else {
    pwr_tCid *cidlist;
    pwr_tAttrRef *arlist;
    int cidcnt;

    // Check all classes this class is built of
    m_erep->merep()->classDependency( &sts, cid, &cidlist, &arlist, &cidcnt);
    for ( int i = 0; i < cidcnt; i++) {
      sClassChk *cpchk = (sClassChk *)tree_Find(&sts, m_classchk_th, &cidlist[i]);
      if ( !cpchk) {
	// Class is not yet checked
	int nc = checkClass( cidlist[i]);
	if ( nc) {
	  n = nc + 1;
	  break;
	}
      }      
      else {
	// Class is checked previously, see if class is modified
	sClassChk *cp = (sClassChk *)tree_Find(&sts, m_class_th, &cidlist[i]);
	if ( cp) {
	  n = 1;
	  break;
	}
      }
    }
    free( cidlist);
    free( arlist);
  }
  if ( n) {
    // Class is modified, insert in modified class table
    sClass *ccp = (sClass *)tree_Insert(&sts, m_class_th, &cid);
    ccp->o_time = o_time;
    ccp->n_time = n_time;
    ccp->o_rbsize = o_crep->size( pwr_eBix_rt);
    ccp->n_rbsize = n_crep->size( pwr_eBix_rt);
    strcpy(ccp->name, o_crep->name());
    return n;
  }
  return 0;
}

int
wb_vrepdbms::updateObject(pwr_tOid oid, pwr_tCid cid)
{
  pwr_tStatus sts;
  wb_cdrep *n_crep = m_erep->merep()->cdrep(&sts, cid);

  if (n_crep == 0) {
    return 0;
  }
  
  m_ohead.get(m_db->m_txn, oid);
  wb_dbms_rbody rb(m_db, m_ohead.oid());
  void *rp = calloc(1, m_ohead.rbSize());

  if (m_ohead.rbSize())
    rb.get(m_db->m_txn, 0, m_ohead.rbSize(), rp);

  wb_dbms_dbody db(m_db, m_ohead.oid());
  void *dp = calloc(1, m_ohead.dbSize());

  if (m_ohead.dbSize())
    db.get(m_db->m_txn, 0, m_ohead.dbSize(), dp);

  void *rbody = 0;
  void *dbody = 0;
  pwr_tUInt32 rsize;
  pwr_tUInt32 dsize;
  int rc = 0;
  
  n_crep->convertObject(m_merep, rp, dp, &rsize, &dsize, &rbody, &dbody);
  
  if (rp)
    free(rp);
  if (dp)
    free(dp);
  
  pwr_tTime time;
  clock_gettime(CLOCK_REALTIME, &time);

  if (rbody) {
    rc = rb.upd(m_db->m_txn, 0, rsize, rbody);
    if (rc)
      printf("wb_vrepdbms:writeBody rb.upd rc %d\n", rc);
    rc = 0;
    free(rbody);
    m_ohead.rbSize(rsize);
    m_ohead.rbTime(time);

  }
  if (dbody) {
    rc = db.upd(m_db->m_txn, 0, dsize, dbody);
    if (rc)
      printf("wb_vrepdbms:writeBody db.upd rc %d\n", rc);
    free(dbody);
    m_ohead.dbSize(dsize);
    m_ohead.dbTime(time);
  }

  m_ohead.ohTime(time);
  m_ohead.upd(m_db->m_txn);
  
  return 1;
}

void wb_vrepdbms::checkAttributes(pwr_tCid cid)
{
  pwr_tStatus sts;
  wb_cdrep *o_cdrep = m_merep->cdrep(&sts, cid);
  if (EVEN(sts)) {
    // This is really weird, should not happen.
    printf("This is weird, class does not exist in old meta data, cid: %d, sts: %d\n", cid, sts);
    return;
  }
  
  wb_cdrep *n_cdrep = m_erep->merep()->cdrep(&sts, cid);
  if (EVEN(sts)) {
    // The class does not exist in the new version of meta data
    return;
  }

  for (int i = 0; i < 2; i++) {
    pwr_eBix bix = i ? pwr_eBix_dev: pwr_eBix_rt;

    wb_bdrep *n_bdrep = n_cdrep->bdrep(&sts, bix);
    if (ODD(sts)) {
      wb_bdrep *o_bdrep = o_cdrep->bdrep(&sts, bix);
      if (ODD(sts)) {
        wb_adrep *o_adrep = o_bdrep->adrep(&sts);

        while (ODD(sts)) {

          if (o_adrep->type() == pwr_eType_AttrRef) {
            sArefKey a;

            a.cid = cid;
            a.bix = bix;
            a.offset = o_adrep->offset();
            tree_Insert(&sts, m_aref_th, &a); 
          }

          // Indentify attribute with the same aix
          bool found = false;
          wb_adrep *n_adrep = n_bdrep->adrep( &sts);
          while (ODD(sts)) {
            if (o_adrep->aix() == n_adrep->aix()) {
              found = true;
              break;
            }
            wb_adrep *prev = n_adrep;
            n_adrep = n_adrep->next(&sts);
            delete prev;
          }
          if (found) {
            if (
              (o_adrep->offset()   != n_adrep->offset())   ||
              (o_adrep->size()     != n_adrep->size())     ||
              (o_adrep->type()     != n_adrep->type())     ||
              //(o_adrep->tid()      != n_adrep->tid())      ||
              (o_adrep->nElement() != n_adrep->nElement()) ||
              (o_adrep->index()    != n_adrep->index())
              )
              {
                sAttributeKey ak;
                sAttribute *ap;
                  
                ak.cid = cid;
                ak.bix = bix;
                ak.oStart = o_adrep->offset();
                ak.oEnd = o_adrep->offset() + o_adrep->size() - 1;                
                ap = (sAttribute *) tree_Insert(&sts, m_attribute_th, &ak);
                ap->o.aref = o_adrep->aref();
                ap->n.aref = n_adrep->aref();
                ap->o.aix = o_adrep->aix();
                ap->n.aix = n_adrep->aix();
                ap->o.nElement = o_adrep->nElement();
                ap->n.nElement = n_adrep->nElement();
              }
            if (o_adrep->isClass() && n_adrep->subClass() == o_adrep->subClass()) {
              checkSubClass( cid, o_adrep->subClass(), o_adrep->offset(), n_adrep->offset());
            }
            delete n_adrep;
          }
          wb_adrep *prev = o_adrep;
          o_adrep = o_adrep->next(&sts);
          delete prev;
        }
        if (o_adrep) delete o_adrep;
      }
      if (o_bdrep) delete o_bdrep;
    }
    if (n_bdrep) delete n_bdrep;
  }
}

void wb_vrepdbms::checkSubClass(pwr_tCid cid, pwr_tCid subcid, unsigned int o_offset, unsigned int n_offset)
{
  pwr_tStatus sts;
  wb_cdrep *o_cdrep = m_merep->cdrep(&sts, subcid);
  if (EVEN(sts)) throw wb_error(sts);
  wb_cdrep *n_cdrep = m_erep->merep()->cdrep(&sts, subcid);
  if (EVEN(sts)) return;

  pwr_eBix bix = pwr_eBix_rt;

  wb_bdrep *n_bdrep = n_cdrep->bdrep(&sts, bix);
  if (EVEN(sts)) return;
  wb_bdrep *o_bdrep = o_cdrep->bdrep(&sts, bix);
  if (EVEN(sts)) return;

  wb_adrep *o_adrep = o_bdrep->adrep(&sts);

  while (ODD(sts)) {
    bool found = false;
    wb_adrep *n_adrep = n_bdrep->adrep(&sts);
    while (ODD(sts)) {
      if (o_adrep->aix() == n_adrep->aix()) {
        found = true;
        break;
      }
      wb_adrep *prev = n_adrep;
      n_adrep = n_adrep->next(&sts);
      delete prev;
    }
    if (found) {
      if (
        (o_adrep->offset()   != n_adrep->offset())   ||
        (o_adrep->size()     != n_adrep->size())     ||
        (o_adrep->type()     != n_adrep->type())     ||
        //(o_adrep->tid()      != n_adrep->tid())      ||
        (o_adrep->nElement() != n_adrep->nElement()) ||
        (o_adrep->index()    != n_adrep->index())) {

        sAttributeKey ak;
        sAttribute *ap;
                  
        ak.cid = cid;
        ak.bix = bix;
        ak.oStart = o_adrep->offset() + o_offset;
        ak.oEnd = ak.oStart + o_adrep->size() - 1;                 
        ap = (sAttribute *) tree_Insert(&sts, m_attribute_th, &ak);
        
        ap->o.aref = o_adrep->aref();
        ap->n.aref = n_adrep->aref();
        ap->o.aix = o_adrep->aix();
        ap->n.aix = n_adrep->aix();
        ap->o.nElement = o_adrep->nElement();
        ap->n.nElement = n_adrep->nElement();
      }
      if (o_adrep->isClass() && o_adrep->subClass() == n_adrep->subClass()) {
        checkSubClass( cid, n_adrep->subClass(), o_adrep->offset(), n_adrep->offset());
      } else if (o_adrep->type() == pwr_eType_AttrRef) {
        sArefKey a;

        a.cid = subcid;
        a.bix = bix;
        a.offset = o_adrep->offset();
        tree_Insert(&sts, m_aref_th, &a);        
      }
      
      if (n_adrep) delete n_adrep;
    }
    wb_adrep *prev = o_adrep;
    o_adrep = o_adrep->next(&sts);
    delete prev;
  }
  if (o_cdrep) delete o_cdrep;
  if (n_cdrep) delete n_cdrep;
}

#endif
