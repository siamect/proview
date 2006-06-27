/* 
 * Proview   $Id: wb_vrepdb.cpp,v 1.49 2006-06-27 05:52:54 claes Exp $
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

#include <sys/stat.h>
#include <errno.h>
#include "co_cdh.h"
#include "co_tree.h"
#include "co_time.h"
#include "wb_db.h"
#include "wb_vrepdb.h"
#include "wb_orepdb.h"
#include "wb_cdrep.h"
#include "wb_erep.h"
#include "wb_dbs.h"
#include "db_cxx.h"
#include "wb_ldh.h"
#include "wb_merep.h"
#include "wb_attribute.h"
#include "wb_dblock.h"
#include "co_msgwindow.h"
#include "wb_vrepwbl.h"

void wb_vrepdb::unref()
{
  if (--m_nRef == 0) {
    wb_dblock::dbunlock(m_fileName);
    delete this;
  }
}

wb_vrep *wb_vrepdb::ref()
{
  m_nRef++;
  return this;
}


wb_vrepdb::wb_vrepdb(wb_erep *erep, const char *fileName) :
  m_erep(erep), m_nRef(0), m_ohead(), m_oid_th(0)
{
  strcpy(m_fileName, fileName);
  wb_dblock::dblock( m_fileName);

  m_db = new wb_db();
  m_db->open(fileName);
  m_ohead.setDb(m_db);
  strcpy(m_name, m_db->volumeName());
  m_vid = m_db->vid();
  m_cid = m_db->cid();

  m_merep = new wb_merep(m_fileName, erep, this);
  m_merep->compareMeta(m_name, erep->merep());

#if 0
  checkMeta();
  updateMeta();
#endif
}

wb_vrepdb::wb_vrepdb(wb_erep *erep, pwr_tVid vid, pwr_tCid cid, const char *volumeName, const char *fileName) :
  m_erep(erep), m_nRef(0), m_ohead(), m_oid_th(0)
{
  strcpy(m_fileName, fileName);
  wb_dblock::dblock( m_fileName);

  m_db = new wb_db();
  m_db->create(vid, cid, volumeName, fileName);
  m_ohead.setDb(m_db);
  strcpy(m_name, m_db->volumeName());
  m_vid = m_db->vid();
  m_cid = m_db->cid();

  m_merep = m_erep->merep();
  m_merep->copyFiles(m_fileName);
}

wb_erep *wb_vrepdb::erep()
{
  return m_erep;
}

wb_vrep *wb_vrepdb::next()
{
  pwr_tStatus sts;

  return m_erep->nextVolume( &sts, m_db->vid());
}

wb_merep *wb_vrepdb::merep() const
{
  return m_merep;
}

wb_srep *wb_vrepdb::newSession()
{
  return 0;
}

void wb_vrepdb::objectName(pwr_tOid oid, char *name, int level)
{
  if (cdh_ObjidIsNull(oid))
    return;

  wb_db_ohead o(m_db, m_db->m_txn, oid);

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

wb_name wb_vrepdb::longName(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;
  char name[512];

  try {
    objectName(o->oid(), name, 0);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    name[0] = '\0';
  }
  return wb_name(name);
}

bool wb_vrepdb::isOffspringOf(pwr_tStatus *sts, const wb_orep *child, const wb_orep *parent)
{
  return false;
}

bool wb_vrepdb::isLocal(const wb_orep *o)
{
  if (o)
    return o->oid().vid == wb_vrep::vid();

  return false;
}

bool wb_vrepdb::createSnapshot(const char *fileName, const pwr_tTime *time)
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

void wb_vrepdb::objectName(const wb_orep *o, char *str)
{
  if (str)
    *str = '\0';
}

const char *wb_vrepdb::objectName(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, o->oid());

    return m_ohead.name();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}


void wb_vrepdb::load()
{
}


wb_orep* wb_vrepdb::object(pwr_tStatus *sts)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid oid;

    oid.vid = m_vid;
    oid.oix = pwr_cNOix;

    m_ohead.get(m_db->m_txn, oid);
    m_ohead.get(m_db->m_txn, m_ohead.foid());

    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
  catch (wb_error &e) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, pwr_tOid oid)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, oid);

    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (wb_error &e) {
    *sts = e.sts();
    return 0;
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, wb_name &name)
{
  *sts = LDH__SUCCESS;
  //*sts = LDH__NYI;
  // return 0;
  pwr_tOid poid;
  poid.vid = m_db->m_vid;
  poid.oix = pwr_cNOix;

  try {
    for (int i = 0; name.hasSegment(i); i++) {
      wb_db_name n(m_db, poid, name.normSegment(i));
      int rc = n.get(m_db->m_txn);
      if (rc) {
  *sts = LDH__NOSUCHOBJ;
  return 0;
      }
      poid = n.oid();

    }

    m_ohead.get(m_db->m_txn, poid);
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, const wb_orep *parent, wb_name &name)
{
  *sts = LDH__SUCCESS;
  try {
    wb_db_name n(m_db, m_db->m_txn, parent->oid(), name);
    m_ohead.get(m_db->m_txn, n.oid());
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name)
{
  *sts = LDH__SUCCESS;
  wb_db_txn *txn = m_db->begin(m_db->m_txn);
  void *p = 0;

  try {
    int rs = 0;
    pwr_tTime time;
    pwr_tOid oid = m_db->new_oid(txn);
    wb_db_ohead o(m_db, txn, orep->oid());
    clock_gettime(CLOCK_REALTIME, &time);

    if (o.rbSize()) {
      o.rbTime(time);
      p = calloc(1, o.rbSize());
      wb_db_rbody rb(m_db, orep->oid());
      rs = rb.get(txn, 0, o.rbSize(), p);
      if (rs)
        printf("wb_vrepdb::copyObject, rb.get rs %d\n", rs);
      rb.oid(oid);
      rs = rb.put(txn);
      if (rs)
        printf("wb_vrepdb::copyObject, rb.put rs %d\n", rs);
      free(p);
      p = 0;
    }
    if (o.dbSize()) {
      o.dbTime(time);
      p = calloc(1, o.dbSize());
      wb_db_dbody db(m_db, orep->oid());
      rs = db.get(txn, 0, o.dbSize(), p);
      if (rs)
        printf("wb_vrepdb::copyObject, db.get rs %d\n", rs);
      db.oid(oid);
      rs = db.put(txn);
      if (rs)
        printf("wb_vrepdb::copyObject, db.put rs %d\n", rs);
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

    rs = o.put(txn);
    if (rs)
      printf("wb_vrepdb::copyObject, o.put rs %d\n", rs);

    wb_db_class c(m_db, o);
    rs = c.put(txn);
    if (rs)
      printf("wb_vrepdb::copyObject, c.put rs %d\n", rs);

    txn->commit(0);
    return new (this) wb_orepdb(&o.m_o);
  }
  catch (wb_error &e) {
    txn->abort();
    if (p)
      free(p);
    *sts = e.sts();
    return 0;
  }
  catch (DbException &e) {
    txn->abort();
    if (p)
      free(p);
    *sts = LDH__DBERROR;
    return 0;
  }
}

bool wb_vrepdb::copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination &d)
{
  return false;
}

wb_orep* wb_vrepdb::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name)
{
  *sts = LDH__SUCCESS;
  wb_db_txn *txn = m_db->begin(m_db->m_txn);
  void *p = 0;

  try {
    int rs = 0;
    pwr_tTime time;
    pwr_tOid oid = m_db->new_oid(txn);
    wb_db_ohead o(m_db, oid);

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
      wb_db_rbody b(m_db, o.oid(), o.rbSize(), p);
      rs = b.put(txn);
      if (rs)
        printf("wb_vrepdb::createObject, rb.put rs %d\n", rs);
      free(p);
      p = 0;
    }
    if (o.dbSize()) {
      pwr_tStatus sts = 1;

      o.dbTime(time);
      p = calloc(1, o.dbSize());
      cdef.templateBody(&sts, pwr_eBix_dev, p, o.oid());
      wb_db_dbody b(m_db, o.oid(), o.dbSize(), p);
      rs = b.put(txn);
      if (rs)
        printf("wb_vrepdb::createObject, db.put rs %d\n", rs);
      free(p);
      p = 0;
    }

    rs = o.put(txn);
    if (rs)
      printf("wb_vrepdb::createObject, o.put rs %d\n", rs);

    wb_db_class c(m_db, o);
    rs = c.put(txn);
    if (rs)
      printf("wb_vrepdb::createObject, c.put rs %d\n", rs);

    rs = txn->commit(0);

    return new (this) wb_orepdb(&o.m_o);
  }
  catch (wb_error &e) {
    txn->abort();
    if (p)
      free(p);
    //*sts = e.sts();
    throw e;
  }
  catch (DbException &e) {
    txn->abort();
    if (p)
      free(p);
    *sts = LDH__DBERROR;
    return 0;
  }
}

bool wb_vrepdb::deleteObject(pwr_tStatus *sts, wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  wb_db_ohead o(m_db, orp->oid());

  wb_db_txn *txn = m_db->begin(m_db->m_txn);

  try {
    o.get(txn);

    unadopt(txn, o);

    wb_db_class c(m_db, o);
    c.del(txn);

    wb_db_dbody db(m_db, o.oid());
    db.del(txn);
    wb_db_rbody rb(m_db, o.oid());
    rb.del(txn);

    o.del(txn);

    txn->commit(0);
    return true;
  }
  catch (wb_error &e) {
    txn->abort();
    *sts = e.sts();
    return 0;
  }
  catch (DbException &e) {
    txn->abort();
    *sts = LDH__DBERROR;
    return false;
  }
}

bool wb_vrepdb::deleteFamilyMember(pwr_tOid oid, wb_db_txn *txn)
{
  if (cdh_ObjidIsNull(oid))
    return false;

  int rs = 0;

  wb_db_ohead o(m_db, oid);
  o.get(txn);

  deleteFamilyMember(o.foid(), txn);

  deleteFamilyMember(o.aoid(), txn);

  wb_db_class c(m_db, o);
  rs = c.del(txn);
  if (rs)
    printf("wb_vrepdb::deleteFamilyMember, c.del rs %d\n", rs);

  wb_db_dbody db(m_db, o.oid());
  rs = db.del(txn);
  wb_db_rbody rb(m_db, o.oid());
  rs = rb.del(txn);

  wb_db_name n(m_db, o.oid(), o.poid(), o.normname());
  rs = n.del(txn);
  if (rs)
    printf("wb_vrepdb::deleteFamilyMember, n.del rs %d\n", rs);

  rs = o.del(txn);
  if (rs)
    printf("wb_vrepdb::deleteFamilyMember, o.del rs %d\n", rs);

  return true;
}

bool wb_vrepdb::deleteFamily(pwr_tStatus *sts, wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  wb_db_ohead o(m_db, orp->oid());

  wb_db_txn *txn = m_db->begin(m_db->m_txn);

  try {
    o.get(txn);
    printf("wb_vrepdb::deleteFamily %s\n", o.name());

    unadopt(txn, o);

    deleteFamilyMember(o.oid(), txn);

    txn->commit(0);
    printf("wb_vrepdb::deleteFamily success\n");
    return true;
  }
  catch (DbException &e) {
    printf("wb_vrepdb::deleteFamily failure, %s\n", e.what());
    txn->abort();
    *sts = LDH__DBERROR;
    return false;
  }
}

bool wb_vrepdb::deleteOset(pwr_tStatus *sts, wb_oset *oset)
{
  *sts = LDH__NYI;
  return false;
}

bool wb_vrepdb::moveObject(pwr_tStatus *sts, wb_orep *orp, wb_destination &d)
{
  *sts = LDH__SUCCESS;

  wb_db_txn *txn = m_db->begin(m_db->m_txn);

  try {
    wb_db_ohead o(m_db, txn, orp->oid());
    unadopt(txn, o);
    adopt(txn, o, d);
    o.put(txn);

    txn->commit(0);
  }
  catch (wb_error &e) {
    txn->abort();
    *sts = e.sts();
    return 0;
  }
  catch (DbException &e) {
    txn->abort();
    *sts = LDH__DBERROR;
  }

  return true;
}

bool wb_vrepdb::renameObject(pwr_tStatus *sts, wb_orep *orp, wb_name &name)
{
  *sts = LDH__SUCCESS;
  wb_db_txn *txn = m_db->begin(m_db->m_txn);

  try {
    int rc = 0;
    m_ohead.get(txn, orp->oid());

    wb_db_name n(m_db, m_ohead);
    rc = n.del(txn);
    if (rc)
      printf("wb_vrepdb::renameObject, n.del rc %d\n", rc);
    n.name(name);
    rc = n.put(txn);
    if (rc) {
      printf("wb_vrepdb::renameObject, n.put rc %d\n", rc);
      *sts = LDH__NAMALREXI;
      txn->abort();
      m_ohead.clear();
      return false;
    }

    m_ohead.name(name);
    m_ohead.put(txn);

    txn->commit(0);
    return true;
  }
  catch (DbException &e) {
    txn->abort();
    printf("wb_vrepdb::renameObject, exception %s\n", e.what());
    m_ohead.clear();
    *sts = LDH__DBERROR;
    return false;
  }
}

bool wb_vrepdb::writeAttribute(pwr_tStatus *sts, wb_orep *orp, pwr_eBix bix, size_t offset, size_t size, void *p)
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

      wb_db_rbody rb(m_db, m_ohead.oid());
      rc = rb.put(m_db->m_txn, offset, size, p);
      m_ohead.rbTime(time);
      m_ohead.put(m_db->m_txn);
      if (rc)
        printf("wb_vrepdb::writeAttribute rb.put rc %d\n", rc);
      break;
    }
    case pwr_eBix_dev:
    {

      wb_db_dbody db(m_db, m_ohead.oid());
      rc = db.put(m_db->m_txn, offset, size, p);
      m_ohead.dbTime(time);
      m_ohead.put(m_db->m_txn);
      if (rc)
        printf("wb_vrepdb::writeAttribute db.put rc %d\n", rc);
      break;
    }
    default:
      break;
    }

    return true;
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return false;
  }
}

void *wb_vrepdb::readAttribute(pwr_tStatus *sts, const wb_orep *orp, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    *sts = LDH__SUCCESS;

    switch (bix) {
    case pwr_eBix_rt:
    {

      wb_db_rbody rb(m_db, m_ohead.oid());
      rb.get(m_db->m_txn, offset, size, p);
      break;
    }
    case pwr_eBix_dev:
    {

      wb_db_dbody db(m_db, m_ohead.oid());
      db.get(m_db->m_txn, offset, size, p);
      break;
    }
    default:
      p = 0;
    }

    return p;
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

void *wb_vrepdb::readBody(pwr_tStatus *sts, const wb_orep *orp, pwr_eBix bix, void *p)
{
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    *sts = LDH__SUCCESS;

    switch (bix) {
    case pwr_eBix_rt:
    {
      wb_db_rbody rb(m_db, m_ohead.oid());
      rb.get(m_db->m_txn, 0, m_ohead.rbSize(), p);
      break;
    }
    case pwr_eBix_dev:
    {
      wb_db_dbody db(m_db, m_ohead.oid());
      db.get(m_db->m_txn, 0, m_ohead.dbSize(), p);
      break;
    }
    default:
      p = 0;
    }

    return p;
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

bool wb_vrepdb::writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p)
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

      wb_db_rbody rb(m_db, m_ohead.oid());
      rc = rb.put(m_db->m_txn, 0, m_ohead.rbSize(), p);
      if (rc)
        printf("wb_vrepdb::writeBody rb.put rc %d\n", rc);
      m_ohead.rbTime(time);
      m_ohead.put(m_db->m_txn);
      break;
    }
    case pwr_eBix_dev:
    {

      wb_db_dbody db(m_db, m_ohead.oid());
      rc = db.put(m_db->m_txn, 0, m_ohead.dbSize(), p);
      if (rc)
        printf("wb_vrepdb::writeBody db.put rc %d\n", rc);
      m_ohead.dbTime(time);
      m_ohead.put(m_db->m_txn);
      break;
    }
    default:
      break;
    }

    return true;
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return false;
  }
}

wb_orep *wb_vrepdb::ancestor(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__NYI;
  return 0;
}

pwr_tCid wb_vrepdb::cid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).cid();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

pwr_tTime wb_vrepdb::ohTime(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).ohTime();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    pwr_tTime t = {0, 0};

    printf("vrepdb: %s\n", e.what());
    return t;
  }
}

pwr_tTime wb_vrepdb::rbTime(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).rbTime();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    pwr_tTime t = {0, 0};
    
    printf("vrepdb: %s\n", e.what());
    return t;
  }
}

pwr_tTime wb_vrepdb::dbTime(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).dbTime();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    pwr_tTime t = {0, 0};
    
    printf("vrepdb: %s\n", e.what());
    return t;
  }
}

pwr_mClassDef wb_vrepdb::flags(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).flags();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    pwr_mClassDef flags; flags.m = 0;
    return flags;
  }
}

pwr_tVid wb_vrepdb::vid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).vid();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return pwr_cNVid;
  }
}

pwr_tOid wb_vrepdb::oid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).oid();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return pwr_cNOid;
  }
}

pwr_tOix wb_vrepdb::oix(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).oix();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return pwr_cNOix;
  }
}

pwr_tOid wb_vrepdb::poid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).poid();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return pwr_cNOid;
  }
}

pwr_tOid wb_vrepdb::foid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).foid();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return pwr_cNOid;
  }
}

pwr_tOid wb_vrepdb::loid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).loid();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return pwr_cNOid;
  }
}

pwr_tOid wb_vrepdb::aoid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).aoid();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return pwr_cNOid;
  }
}

pwr_tOid wb_vrepdb::boid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_db->m_txn, orp->oid()).boid();
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return pwr_cNOid;
  }
}

wb_orep *wb_vrepdb::parent(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, m_ohead.get(m_db->m_txn, orp->oid()).poid());
    if ( m_ohead.oid().oix == 0) {
      *sts = LDH__NO_PARENT;
      return 0;
    }
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::after(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid aoid = m_ohead.get(m_db->m_txn, orp->oid()).aoid();
    if (cdh_ObjidIsNull(aoid)) {
      *sts = LDH__NO_SIBLING;
      return 0;
    }

    m_ohead.get(m_db->m_txn, aoid);
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::before(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid boid = m_ohead.get(m_db->m_txn, orp->oid()).boid();
    if (cdh_ObjidIsNull(boid)) {
      *sts = LDH__NO_SIBLING;
      return 0;
    }

    m_ohead.get(m_db->m_txn, boid);
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::first(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;

  try {
    pwr_tOid foid = m_ohead.get(m_db->m_txn, orp->oid()).foid();
    if (cdh_ObjidIsNull(foid)) {
      *sts = LDH__NO_CHILD;
      return 0;
    }

    m_ohead.get(m_db->m_txn, foid);
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::child(pwr_tStatus *sts, const wb_orep *orp, wb_name &name)
{
  *sts = LDH__SUCCESS;

  try {
    wb_db_name n(m_db, m_db->m_txn, orp->oid(), name);
    m_ohead.get(m_db->m_txn, n.oid());
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::last(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid loid = m_ohead.get(m_db->m_txn, orp->oid()).loid();
    if (cdh_ObjidIsNull(loid)) {
      *sts = LDH__NO_CHILD;
      return 0;
    }

    m_ohead.get(m_db->m_txn, loid);
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}


wb_orep *wb_vrepdb::object(pwr_tStatus *sts, pwr_tCid cid)
{
  *sts = LDH__SUCCESS;
  try {
    wb_db_class c(m_db, m_db->m_txn, cid);
    pwr_tOid oid;
    oid.vid = m_vid;
    oid.oix = 0;
    if (c.succ(oid)) {
      if (c.cid() == cid) {
        m_ohead.get(m_db->m_txn, c.oid());
        return new (this) wb_orepdb(&m_ohead.m_o);
      }
    }
    *sts =  LDH__NOSUCHOBJ;
    return 0;
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::next(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    wb_db_class c(m_db, m_db->m_txn, m_ohead.cid());
    if (c.succ(m_ohead.oid())) {
      if (c.cid() == m_ohead.cid()) {
        m_ohead.get(m_db->m_txn, c.oid());
        return new (this) wb_orepdb(&m_ohead.m_o);
      }
    }
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::nextClass(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    wb_db_class c(m_db, m_db->m_txn, m_ohead.cid());
    if (c.succClass(m_ohead.cid())) {
      if ( c.cid() == m_ohead.cid()) {
  m_ohead.get(m_db->m_txn, c.oid());
  return new (this) wb_orepdb(&m_ohead.m_o);
      }
    }
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep *wb_vrepdb::previous(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    wb_db_class c(m_db, m_db->m_txn, m_ohead.cid());
    if (c.pred(m_ohead.oid())) {
      m_ohead.get(m_db->m_txn, c.oid());
      return new (this) wb_orepdb(&m_ohead.m_o);
    } else {
      //*sts = LDH__?;
      return 0;
    }
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

void wb_vrepdb::adopt(wb_db_txn *txn, wb_db_ohead &o, wb_destination &dest)
{
  int rc = 0;
  wb_db_ohead d(m_db, txn, dest.oid());

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


  wb_db_ohead p(m_db, txn, o.poid());

  if (o.boid().vid != pwr_cNVid) {
    wb_db_ohead b(m_db, txn, o.boid());
    b.aoid(o.oid());
    rc = b.put(txn);
    if (rc)
      printf("wb_vrepdb::adopt, b.put rc %d\n", rc);
  } else {
    p.foid(o.oid());
  }

  if (o.aoid().vid != pwr_cNVid) {
    wb_db_ohead a(m_db, txn, o.aoid());
    a.boid(o.oid());
    rc = a.put(txn);
    if (rc)
      printf("wb_vrepdb::adopt, a.put rc %d\n", rc);
  } else {
    p.loid(o.oid());
  }

  rc = p.put(txn);
  if (rc)
    printf("wb_vrepdb::adopt, p.put rc %d\n", rc);

  wb_db_name n(m_db, o.oid(), o.poid(), o.normname());
  rc = n.put(txn);
  if (rc) {
    printf("wb_vrepdb::adopt, n.put rc %d\n", rc);
    throw wb_error(LDH__NAMALREXI);
  }

}

void wb_vrepdb::unadopt(wb_db_txn *txn, wb_db_ohead &o)
{
  int rc = 0;
  wb_db_ohead p(m_db, txn, o.poid());

  if (o.boid().vid != pwr_cNVid) {
    wb_db_ohead b(m_db, txn, o.boid());
    b.aoid(o.aoid());
    rc = b.put(txn);
    if (rc)
      printf("wb_vrepdb::unadopt, b.put rc %d\n", rc);
  } else {
    p.foid(o.aoid());
  }

  if (o.aoid().vid != pwr_cNVid) {
    wb_db_ohead a(m_db, txn, o.aoid());
    a.boid(o.boid());
    rc = a.put(txn);
    if (rc)
      printf("wb_vrepdb::unadopt, a.put rc %d\n", rc);
  } else {
    p.loid(o.boid());
  }

  wb_db_name n(m_db, o.oid(), o.poid(), o.normname());
  rc = n.del(txn);
  if (rc)
    printf("wb_vrepdb::unadopt, n.del rc %d\n", rc);

  o.poid(pwr_cNOid);
  o.aoid(pwr_cNOid);
  o.boid(pwr_cNOid);

  rc = p.put(txn);
  if (rc)
    printf("wb_vrepdb::unadopt, p.put rc %d\n", rc);

  rc = o.put(txn);
  if (rc)
    printf("wb_vrepdb::unadopt, o.put rc %d\n", rc);
}

bool wb_vrepdb::exportVolume(wb_import &i)
{
  return i.importVolume(*this);
}

bool wb_vrepdb::exportHead(wb_import &i)
{
  wb_db_ohead *op = new wb_db_ohead(m_db);

  op->iter(i);

  return true;
}

bool wb_vrepdb::exportRbody(wb_import &i)
{
  wb_db_rbody *rp = new wb_db_rbody(m_db);

  rp->iter(i);

  return true;
}

bool wb_vrepdb::exportDbody(wb_import &i)
{
  wb_db_dbody *dp = new wb_db_dbody(m_db);

  dp->iter(i);

  return true;
}

bool wb_vrepdb::exportDocBlock(wb_import &i)
{
  return false;
}

bool wb_vrepdb::exportMeta(wb_import &i)
{
  return false;
}


bool wb_vrepdb::exportTree(wb_treeimport &i, pwr_tOid oid)
{
  return exportTreeHelper(i, oid, true);
}

bool wb_vrepdb::exportTreeHelper(wb_treeimport &i, pwr_tOid oid, bool isRoot)
{
  m_ohead.get(m_db->m_txn, oid);

  void *rbody = 0;
  void *dbody = 0;
  int rbSize = m_ohead.rbSize();
  int dbSize = m_ohead.dbSize();

  if (rbSize) {
    rbody = (void *)malloc(rbSize);
    wb_db_rbody rb(m_db, oid);
    int rc = rb.get(m_db->m_txn, 0, rbSize, rbody);
    if (rc)
      printf("wb_vrepdbs::exportTreeObject, rb.get: %d\n", rc);
  }

  if (dbSize) {
    dbody = (void *)malloc(dbSize);
    wb_db_dbody db(m_db, oid);
    int rc = db.get(m_db->m_txn, 0, dbSize, dbody);
    if (rc)
      printf("wb_vrepdbs::exportTreeObject, db.get: %d\n", rc);
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

wb_orepdb *wb_vrepdb::new_wb_orepdb(size_t size)
{
  wb_orepdb *o = (wb_orepdb *) calloc(1, size);
  o->m_vrep = this;
  return o;
}

void wb_vrepdb::delete_wb_orepdb(void *p)
{
  free(p);
}

bool wb_vrepdb::importPasteObject(pwr_tOid doid, ldh_eDest destcode,
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
      catch (DbException &e) {
        printf("vrepdb, desination (%d.%d) does not exist: %s\n", doid.vid, doid.oix, e.what());
        throw wb_error(LDH__PASTEINCON);
      }
    }

    switch (destcode) {
    case ldh_eDest_After:
      m_destination.poid = m_ohead.poid();
      m_destination.foid = m_ohead.oid();
      m_destination.loid = m_ohead.aoid();
      break;
    case ldh_eDest_IntoFirst:
      m_destination.poid = m_ohead.oid();
      m_destination.loid = m_ohead.foid();
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

bool wb_vrepdb::importPaste()
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
      m_ohead.put(m_db->m_txn);
    }

    oep = (sOentry*)tree_Successor(&sts, m_oid_th, oep);
  }

  if (cdh_ObjidIsNull(m_destination.foid)) {
    m_ohead.get(m_db->m_txn, m_destination.poid);
    m_ohead.foid(m_poep->first->n_oid);
    m_ohead.ohTime(oTime);
    m_ohead.put(m_db->m_txn);

    if (cdh_ObjidIsNotNull(m_destination.loid)) {
      m_ohead.get(m_db->m_txn, m_destination.loid);
      m_ohead.boid(m_poep->last->n_oid);
      m_ohead.ohTime(oTime);
      m_ohead.put(m_db->m_txn);

      m_ohead.get(m_db->m_txn, m_poep->last->n_oid);
      m_ohead.aoid(m_destination.loid);
      m_ohead.ohTime(oTime);
      m_ohead.put(m_db->m_txn);
    }
  }
  if (cdh_ObjidIsNull(m_destination.loid)) {
    m_ohead.get(m_db->m_txn, m_destination.poid);
    m_ohead.loid(m_poep->last->n_oid);
    m_ohead.ohTime(oTime);
    m_ohead.put(m_db->m_txn);

    if (cdh_ObjidIsNotNull(m_destination.foid)) {
      m_ohead.get(m_db->m_txn, m_destination.foid);
      m_ohead.aoid(m_poep->first->n_oid);
      m_ohead.ohTime(oTime);
      m_ohead.put(m_db->m_txn);

      m_ohead.get(m_db->m_txn, m_poep->first->n_oid);
      m_ohead.boid(m_destination.foid);
      m_ohead.ohTime(oTime);
      m_ohead.put(m_db->m_txn);
    }
  }
  if (cdh_ObjidIsNotNull(m_destination.foid) && cdh_ObjidIsNotNull(m_destination.loid)) {
    m_ohead.get(m_db->m_txn, m_destination.foid);
    m_ohead.aoid(m_poep->first->n_oid);
    m_ohead.ohTime(oTime);
    m_ohead.put(m_db->m_txn);

    m_ohead.get(m_db->m_txn, m_poep->first->n_oid);
    m_ohead.boid(m_destination.foid);
    m_ohead.ohTime(oTime);
    m_ohead.put(m_db->m_txn);

    m_ohead.get(m_db->m_txn, m_destination.loid);
    m_ohead.boid(m_poep->last->n_oid);
    m_ohead.ohTime(oTime);
    m_ohead.put(m_db->m_txn);

    m_ohead.get(m_db->m_txn, m_poep->last->n_oid);
    m_ohead.aoid(m_destination.loid);
    m_ohead.ohTime(oTime);
    m_ohead.put(m_db->m_txn);
  }

  importUpdateTree(this);
  importTranslationTableClear();
  tree_DeleteTable(&sts, m_oid_th);
  m_oid_th = 0;

  return true;
}


pwr_tStatus wb_vrepdb::checkMeta()
{
  wb_db_class c(m_db);
  char buff[256];

  m_merepCheck = m_erep->merep();
  m_classCount = 0;
  m_cidChecked = pwr_cNCid;
  m_totalInstanceCount = 0;
  m_needUpdateCount = 0;
  
  setUpdate(false);  

  c.iter(this);

  if (m_classCount == 0) {
    MsgWindow::message( 'W', "Classvolumes need update, execute 'Function->UpdateClasses'");
    return LDH__SUCCESS;
  }

  sprintf(buff, "A total of %d object instances of %d classes can to be updated", m_totalInstanceCount, m_classCount);

  MsgWindow::message('W', buff, msgw_ePop_No);
  MsgWindow::message( 'W', "Classvolumes need update, execute 'Function->UpdateClasses'");
  
  return LDH__SUCCESS;
}

pwr_tStatus wb_vrepdb::updateMeta()
{
  wb_db_class c(m_db);

  m_merepCheck = m_erep->merep();
  m_classCount = 0;
  m_cidChecked = pwr_cNCid;
  m_totalInstanceCount = 0;

  setUpdate(true);

  try {
    c.iter(this);
  } catch (DbException &e) {
    printf("vrepdb: %s\n", e.what());
    return LDH__DBERROR;
  } catch (wb_error &e) {
    printf("vrepdb: %s\n", e.what().c_str());
    return LDH__DBERROR;
  }
  
  if (m_classCount != 0) {

    char buff[256];
    sprintf(buff, "A total of %d object instances of %d classes were updated",
	    m_totalInstanceCount, m_classCount);

    MsgWindow::message('I', buff);

    pwr_tStatus sts = 0;
    commit(&sts);

    if (sts) {
      MsgWindow::message(co_error(sts), "Could not save class updates to database");
      return LDH__DBERROR;
    }
  }
  m_merep->copyFiles(m_fileName, m_erep->merep());
  delete m_merep;
  m_merep = new wb_merep(m_fileName, m_erep, this);
  
  return LDH__SUCCESS;
}

pwr_tStatus wb_vrepdb::checkObject(pwr_tOid oid, pwr_tCid cid)
{
  static wb_cdrep *o_crep = 0;
  static wb_cdrep *n_crep = 0;
  static bool skip = false;
  
  pwr_tTime o_time = {0, 0};
  pwr_tTime n_time = {0, 0};

  if (m_cidChecked != cid) {
    skip = false;
    
    while (cid != 0) {
      pwr_tStatus sts;
      char buff[256];
      char o_timbuf[32];
      char n_timbuf[32];

      if (m_cidChecked != pwr_cNCid) {
        if (n_crep == 0) {
          o_time = o_crep->ohTime();
          time_AtoAscii(&o_time, time_eFormat_DateAndTime, o_timbuf, sizeof(o_timbuf));
          sprintf(buff, "Class \"%s\" [%s], %d instance%s, does not exist in global scope",
		  o_crep->name(), o_timbuf, m_instanceCount, (m_instanceCount == 1 ? "" : "s"));
	  MsgWindow::message('W', buff);
        } else {
          o_time = o_crep->ohTime();
          n_time = n_crep->ohTime();
	  if (time_Acomp(&o_time, &n_time) != 0 || m_instanceCount) {
	    time_AtoAscii(&o_time, time_eFormat_DateAndTime, o_timbuf, sizeof(o_timbuf));
	    time_AtoAscii(&n_time, time_eFormat_DateAndTime, n_timbuf, sizeof(n_timbuf));
	    sprintf(buff, "Class \"%s\" [%s], %d instance%s, can be updated to [%s]",
		  o_crep->name(), o_timbuf, m_instanceCount, (m_instanceCount == 1 ? "" : "s"), n_timbuf);	  
	    MsgWindow::message('W', buff, msgw_ePop_No);
	    m_totalInstanceCount += m_instanceCount;
	    m_needUpdateCount++;
	  }
        }
      }

      o_crep = m_merep->cdrep(&sts, cid);
      n_crep = m_merepCheck->cdrep(&sts, cid);

      if (n_crep == 0) {
      	break;
      }
      	
      o_time = o_crep->ohTime();
      n_time = n_crep->ohTime();

      if (time_Acomp(&o_time, &n_time) == 0) {
      	skip = true;
        break;
      }

      m_classCount++;
      break;
    }
    m_cidChecked = cid;
    m_instanceCount = 0;
  }
  
  if (skip)
    return LDH__SUCCESS;
    
  m_instanceCount++;
  
  return LDH__SUCCESS;
}

pwr_tStatus wb_vrepdb::updateObject(pwr_tOid oid, pwr_tCid cid)
{
  static wb_cdrep *o_crep = 0;
  static wb_cdrep *n_crep = 0;
  static bool skip = false;
  pwr_tTime o_time = {0, 0};
  pwr_tTime n_time = {0, 0};

  if (m_cidChecked != cid) {
    skip = false;
    
    while (cid != 0) {
      pwr_tStatus sts;
      char buff[256];
      char o_timbuf[32];
      char n_timbuf[32];

      if (m_cidChecked != pwr_cNCid) {
        if (n_crep == 0) {
          o_time = o_crep->ohTime();
          time_AtoAscii(&o_time, time_eFormat_DateAndTime, o_timbuf, sizeof(o_timbuf));
          sprintf(buff, "Class \"%s\" [%s], %d instance%s, does not exist in global scope",
		  o_crep->name(), o_timbuf, m_instanceCount, (m_instanceCount == 1 ? "" : "s"));
        } else if ( o_crep != 0) {
          o_time = o_crep->ohTime();
          n_time = n_crep->ohTime();
          time_AtoAscii(&o_time, time_eFormat_DateAndTime, o_timbuf, sizeof(o_timbuf));
          time_AtoAscii(&n_time, time_eFormat_DateAndTime, n_timbuf, sizeof(n_timbuf));
          sprintf(buff, "Class \"%s\" [%s], %d instance%s, %s updated to [%s]",
		  o_crep->name(), o_timbuf, m_instanceCount, (m_instanceCount == 1 ? "" : "s"), (m_instanceCount == 1 ? "was" : "were"), n_timbuf);
          m_totalInstanceCount += m_instanceCount;
        }
        MsgWindow::message('I', buff);
      }

      o_crep = m_merep->cdrep(&sts, cid);
      if (o_crep == 0)
	throw wb_error(sts);
      n_crep = m_merepCheck->cdrep(&sts, cid);

      if (n_crep == 0) {
      	break;
      }
      	
      o_time = o_crep->ohTime();
      n_time = n_crep->ohTime();

      if (time_Acomp(&o_time, &n_time) == 0) {
	  skip = true;
        break;
      }

      m_classCount++;
      break;
    }
    m_cidChecked = cid;
    m_instanceCount = 0;
  }

  if (skip) {
    return LDH__SUCCESS;
  }
  
  m_instanceCount++;

  m_ohead.get(m_db->m_txn, oid);

  o_time = o_crep->ohTime();
  n_time = n_crep->ohTime();

  if (time_Acomp(&o_time, &n_time) == 0)
    return LDH__SUCCESS;

  wb_db_rbody rb(m_db, m_ohead.oid());
  void *rp = calloc(1, m_ohead.rbSize());

  if (m_ohead.rbSize())
    rb.get(m_db->m_txn, 0, m_ohead.rbSize(), rp);

  wb_db_dbody db(m_db, m_ohead.oid());
  void *dp = calloc(1, m_ohead.dbSize());

  if (m_ohead.dbSize())
    db.get(m_db->m_txn, 0, m_ohead.dbSize(), dp);

  void *rbody = 0;
  void *dbody = 0;  
  pwr_tUInt32 rsize;
  pwr_tUInt32 dsize;
  int rc = 0;
  
  n_crep->convertObject(m_merep, rp, dp, &rsize, &dsize, &rbody, &dbody);
  
  free(rp);
  free(dp);
  
  pwr_tTime time;
  clock_gettime(CLOCK_REALTIME, &time);

  if (rbody) {
    rc = rb.put(m_db->m_txn, 0, rsize, rbody);
    if (rc)
      printf("wb_vrepdb::writeBody rb.put rc %d\n", rc);
    rc = 0;
    free(rbody);
    m_ohead.rbSize(rsize);
    m_ohead.rbTime(time);

  }
  if (dbody) {
    rc = db.put(m_db->m_txn, 0, dsize, dbody);
    if (rc)
      printf("wb_vrepdb::writeBody db.put rc %d\n", rc);
    free(dbody);
    m_ohead.dbSize(dsize);
    m_ohead.dbTime(time);
  }

  m_ohead.ohTime(time);
  m_ohead.put(m_db->m_txn);
  
  return LDH__SUCCESS;
}
