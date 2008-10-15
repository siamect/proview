/* 
 * Proview   $Id: wb_db.cpp,v 1.43 2008-10-15 06:04:55 claes Exp $
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

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "pwr.h"
#include "pwr_class.h"
#include "co_dcli.h"
#include "db_cxx.h"
#include "wb_ldh.h"
#include "wb_ldh_msg.h"
#include "wb_destination.h"
#include "wb_db.h"
#include "wb_name.h"
#include "wb_export.h"
#include "wb_convert_volume.h"

static void printstat(DbEnv *ep, char *s);
static void get_config( char *name, unsigned int *lk_max_locks, 
			unsigned int *lk_max_objects);

wb_db_info::wb_db_info(wb_db *db) :
  m_db(db), m_data(&m_volume, sizeof(m_volume))
{
}

void wb_db_info::get(wb_db_txn *txn)
{
  int index = 1;
  int ret;
  Dbt key(&index, sizeof(index));
  Dbt data(&m_volume, sizeof(m_volume));
  data.set_ulen(sizeof(m_volume));
  data.set_flags(DB_DBT_USERMEM);

  try {
    ret = m_db->m_t_info->get(txn, &key, &data, 0);
    printf("info get: %d\n", ret);
  } catch (DbException &e) {
    printf("info get Error, %d\n", ret);
    m_db->m_t_info->err(ret, "m_db->m_t_info->get(txn, &key, &data, 0)");
    cout << e.what();
  }
}

void wb_db_info::put(wb_db_txn *txn)
{
  int index = 1;
  int ret;

  m_key.set_data(&index);
  m_key.set_size(sizeof(index));

  ret = m_db->m_t_info->put(txn, &m_key, &m_data, 0);
  printf("info put: %d\n", ret);
}

wb_db_class::wb_db_class(wb_db *db) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(0, 0), m_dbc(0)
{
}

wb_db_class::wb_db_class(wb_db *db, wb_db_txn *txn, pwr_tCid cid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(0, 0), m_dbc(0)
{
  m_k.oid = pwr_cNOid;
  m_k.cid = cid;
}

wb_db_class::wb_db_class(wb_db *db, pwr_tCid cid, pwr_tOid oid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(0, 0), m_dbc(0)
{
  m_k.oid = oid;
  m_k.cid = cid;
}

wb_db_class::wb_db_class(wb_db *db, wb_db_ohead &o) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(0, 0), m_dbc(0)
{
  m_k.oid = o.oid();
  m_k.cid = o.cid();
}

bool wb_db_class::succ(pwr_tOid oid)
{
  m_k.oid = oid;
  m_k.oid.oix += 1;

  if (m_dbc)
    m_dbc->close();

  m_db->m_t_class->cursor(m_db->m_txn, &m_dbc, 0);
  int ret = m_dbc->get(&m_key, &m_data, DB_SET_RANGE);
  m_dbc->close();
  m_dbc = 0;
  if ( ret == 0) {
    void **data = (void **) &m_key;  // A trick to get m_key.data which has the new key
    memcpy( &m_k, *data, sizeof(m_k));
  }
  return ret == 0;
}

bool wb_db_class::succClass(pwr_tCid cid)
{
  m_k.cid = cid + 1;
  m_k.oid = pwr_cNOid;

  if (m_dbc)
    m_dbc->close();

  m_db->m_t_class->cursor(m_db->m_txn, &m_dbc, 0);
  int ret = m_dbc->get(&m_key, &m_data, DB_SET_RANGE);
  m_dbc->close();
  m_dbc = 0;
  if ( ret == 0) {
    void **data = (void **) &m_key;  // A trick to get m_key.data which has the new key
    memcpy( &m_k, *data, sizeof(m_k));
  }
  return ret == 0;

  // return false;
}

bool wb_db_class::pred(pwr_tOid oid)
{
  m_k.oid = oid;

  int ret = m_dbc->get(&m_key, &m_data, DB_SET_RANGE);
  if (ret != 0)
    return false;
  ret = m_dbc->get(&m_key, &m_data, DB_PREV);
  return ret == 0;
}

int wb_db_class::put(wb_db_txn *txn)
{
  return m_db->m_t_class->put(txn, &m_key, &m_data, DB_NOOVERWRITE);
}

int wb_db_class::del(wb_db_txn *txn)
{
  return m_db->m_t_class->del(txn, &m_key, 0);
}

void wb_db_class::iter(void (*func)(pwr_tOid oid, pwr_tCid cid))
{
  int rc = 0;
  
  m_db->m_t_class->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_k, 0, sizeof(m_k));

  m_key.set_data(&m_k);
  m_key.set_ulen(sizeof(m_k));
  m_key.set_flags(DB_DBT_USERMEM);

  /* Walk through the database. */

  while ((rc = m_dbc->get(&m_key, &m_data, DB_NEXT)) == 0) {
    func(m_k.oid, m_k.cid);
  }

  m_dbc->close();
  m_dbc = 0;
}

wb_db_class::~wb_db_class()
{  
  if (m_dbc)
    m_dbc->close();
}

wb_db_class_iterator::wb_db_class_iterator(wb_db *db) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(0, 0), m_dbc(0), m_atEnd(false), m_rc(0)
{
  m_rc = m_db->m_t_class->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_k, 0, sizeof(m_k));

  m_key.set_data(&m_k);
  m_key.set_ulen(sizeof(m_k));
  m_key.set_flags(DB_DBT_USERMEM);

  m_atEnd = (m_rc != 0);  
}

wb_db_class_iterator::wb_db_class_iterator(wb_db *db, pwr_tCid cid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(0, 0), m_dbc(0), m_atEnd(false), m_rc(0)
{
  m_rc = m_db->m_t_class->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_k, 0, sizeof(m_k));

  m_key.set_data(&m_k);
  m_key.set_ulen(sizeof(m_k));
  m_key.set_flags(DB_DBT_USERMEM);

  m_atEnd = (m_rc != 0);  
  m_k.oid = pwr_cNOid;
  m_k.cid = cid;
}

wb_db_class_iterator::wb_db_class_iterator(wb_db *db, pwr_tCid cid, pwr_tOid oid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(0, 0), m_dbc(0), m_atEnd(false), m_rc(0)
{

  m_rc = m_db->m_t_class->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_k, 0, sizeof(m_k));

  m_key.set_data(&m_k);
  m_key.set_ulen(sizeof(m_k));
  m_key.set_flags(DB_DBT_USERMEM);

  m_atEnd = (m_rc != 0);  
  m_k.oid = oid;
  m_k.cid = cid;
}

bool wb_db_class_iterator::first()
{
  m_k.oid = pwr_cNOid;
  m_k.cid = pwr_cNCid;

  //m_rc = m_dbc->get(&m_key, 0, DB_FIRST);
  m_rc = m_dbc->get(&m_key, &m_data, DB_FIRST);
  m_atEnd = (m_rc != 0);

  return !m_atEnd;
}

bool wb_db_class_iterator::succObject()
{
  if (!m_atEnd) {
    m_rc = m_dbc->get(&m_key, &m_data, DB_NEXT);
    m_atEnd = (m_rc != 0);
  }

  return !m_atEnd;
}

bool wb_db_class_iterator::succClass()
{
  m_k.oid = pwr_cNOid;
  m_k.cid++;

  m_rc = m_dbc->get(&m_key, &m_data, DB_SET_RANGE);
  m_atEnd = (m_rc != 0);

  return !m_atEnd;
}

bool wb_db_class_iterator::succClass(pwr_tCid cid)
{
  m_k.oid = pwr_cNOid;
  m_k.cid = cid + 1;

  m_rc = m_dbc->get(&m_key, &m_data, DB_SET_RANGE);
  m_atEnd = (m_rc != 0);

  return !m_atEnd;
}

wb_db_class_iterator::~wb_db_class_iterator()
{
  if (m_dbc)
    m_dbc->close();
  m_dbc = 0;
}

wb_db_name::wb_db_name(wb_db *db, wb_db_txn *txn) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  memset(&m_k, 0, sizeof(m_k));
  m_k.poid = pwr_cNOid;
}

wb_db_name::wb_db_name(wb_db *db, wb_db_ohead &o) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  memset(&m_k, 0, sizeof(m_k));
  m_k.poid = o.poid();
  strcpy(m_k.normname, o.normname());
  m_d.oid = o.oid();
}

wb_db_name::wb_db_name(wb_db *db, pwr_tOid oid, pwr_tOid poid, const char *name) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  wb_name n(name);

  memset(&m_k, 0, sizeof(m_k));

  m_k.poid = poid;
  strcpy(m_k.normname, n.normName(cdh_mName_object));
  m_d.oid = oid;
}

wb_db_name::wb_db_name(wb_db *db, wb_db_txn *txn, pwr_tOid poid, wb_name &name) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  memset(&m_k, 0, sizeof(m_k));
  m_k.poid = poid;
  strcpy(m_k.normname, name.normName(cdh_mName_object));
  m_d.oid = pwr_cNOid;
}

wb_db_name::wb_db_name(wb_db *db, pwr_tOid poid, const char *name) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  memset(&m_k, 0, sizeof(m_k));
  memset(&m_d, 0, sizeof(m_d));

  m_k.poid = poid;
  strcpy(m_k.normname, name);
}

int wb_db_name::get(wb_db_txn *txn)
{
  m_data.set_ulen(sizeof(m_d));
  m_data.set_flags(DB_DBT_USERMEM);
  int rc = m_db->m_t_name->get(txn, &m_key, &m_data, 0);
  return rc;
}

int wb_db_name::put(wb_db_txn *txn)
{
  return m_db->m_t_name->put(txn, &m_key, &m_data, DB_NOOVERWRITE);
}

int wb_db_name::del(wb_db_txn *txn)
{
  return m_db->m_t_name->del(txn, &m_key, 0);
}

void wb_db_name::name(wb_name &name)
{
  memset(m_k.normname, 0, sizeof(m_k.normname));
  strcpy(m_k.normname, name.normName(cdh_mName_object));
}

void wb_db_name::iter(void (*func)(pwr_tOid poid, pwr_tObjName name, pwr_tOid oid))
{
  int rc = 0;

  m_db->m_t_name->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_k, 0, sizeof(m_k));
  memset(&m_d, 0, sizeof(m_d));

  m_key.set_data(&m_k);
  m_key.set_ulen(sizeof(m_k));
  m_key.set_flags(DB_DBT_USERMEM);

  m_data.set_data(&m_d);
  m_data.set_ulen(sizeof(m_d));
  m_data.set_flags(DB_DBT_USERMEM);


  /* Walk through the database and print out the key/data pairs. */

  while ((rc = m_dbc->get(&m_key, &m_data, DB_NEXT)) == 0) {
    func(m_k.poid, m_k.normname, m_d.oid);
  }

  m_dbc->close();
}


wb_db_ohead::wb_db_ohead() :
  m_db(0), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
}

wb_db_ohead::wb_db_ohead(wb_db *db) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
}

wb_db_ohead::wb_db_ohead(wb_db *db, pwr_tOid oid) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
  m_oid = m_o.oid = oid;
}

wb_db_ohead::wb_db_ohead(wb_db *db, wb_db_txn *txn, pwr_tOid oid) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
  m_oid = m_o.oid = oid;
  get(txn);
}

wb_db_ohead::wb_db_ohead(wb_db *db, pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                         pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                         const char *name, const char *normname, pwr_mClassDef flags,
                         pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                         size_t rbSize, size_t dbSize) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{

  memset(&m_o, 0, sizeof(m_o));
  m_oid = m_o.oid = oid;
  m_o.cid = cid;
  m_o.poid = poid;
  strcpy(m_o.name, name);
  strcpy(m_o.normname, normname);
  m_o.time = ohTime;
  m_o.boid = boid;
  m_o.aoid = aoid;
  m_o.foid = foid;
  m_o.loid =loid;

  m_o.flags = flags;

  m_o.body[0].time = rbTime;
  m_o.body[0].size = rbSize;
  m_o.body[1].time = dbTime;
  m_o.body[1].size = dbSize;
}


wb_db_ohead &wb_db_ohead::get(wb_db_txn *txn)
{
  int rc = 0;
  m_data.set_ulen(sizeof(m_o));
  m_data.set_flags(DB_DBT_USERMEM);

  rc = m_db->m_t_ohead->get(txn, &m_key, &m_data, 0);
  if (rc)
    throw DbException( rc);
  return *this;
}

int wb_db_ohead::put(wb_db_txn *txn)
{
  return m_db->m_t_ohead->put(txn, &m_key, &m_data, 0);
}

wb_db_ohead &wb_db_ohead::get(wb_db_txn *txn, pwr_tOid oid)
{
  int rc = 0;
  m_oid = oid;
  m_data.set_ulen(sizeof(m_o));
  m_data.set_flags(DB_DBT_USERMEM);

  rc = m_db->m_t_ohead->get(txn, &m_key, &m_data, 0);
  if ( rc == DB_NOTFOUND)
    throw wb_error(LDH__NOSUCHOBJ);
  if (rc)
    printf("wb_db_ohead::get(txn, oid = %d.%d), get, rc %d\n", oid.vid, oid.oix, rc);
  //pwr_Assert(oid.oix == m_o.oid.oix);
  if (oid.oix != m_o.oid.oix)
    printf("oid.oix (%d.%d) != m_o.oid.oix (%d.%d), %s\n", oid.vid, oid.oix, m_o.oid.vid, m_o.oid.oix, m_o.name);
  return *this;
}

int wb_db_ohead::del(wb_db_txn *txn)
{
  return m_db->m_t_ohead->del(txn, &m_key, 0);
}

void wb_db_ohead::name(wb_name &name)
{
  memset(m_o.name, 0, sizeof(m_o.name));
  memset(m_o.normname, 0, sizeof(m_o.normname));
  strcpy(m_o.name, name.name(cdh_mName_object));
  strcpy(m_o.normname, name.normName(cdh_mName_object));
}

void wb_db_ohead::name(pwr_tOid &oid)
{
  memset(m_o.name, 0, sizeof(m_o.name));
  memset(m_o.normname, 0, sizeof(m_o.normname));
  sprintf(m_o.name, "O%u", oid.oix);
  strcpy(m_o.normname, m_o.name);
}

void wb_db_ohead::clear()
{
  memset(&m_o, 0, sizeof(m_o));
}

void wb_db_ohead::iter(void (*func)(pwr_tOid oid, db_sObject *op))
{
  int rc = 0;

  rc = m_db->m_t_ohead->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_oid, 0, sizeof(m_oid));
  memset(&m_o, 0, sizeof(m_o));

  m_key.set_data(&m_oid);
  m_key.set_ulen(sizeof(m_oid));
  m_key.set_flags(DB_DBT_USERMEM);

  m_data.set_data(&m_o);
  m_data.set_ulen(sizeof(m_o));
  m_data.set_flags(DB_DBT_USERMEM);


  /* Walk through the database and print out the key/data pairs. */

  while ((rc = m_dbc->get(&m_key, &m_data, DB_NEXT)) == 0) {
    func(m_oid, &m_o);
  }

  m_dbc->close();
}

void wb_db_ohead::iter(wb_import &i)
{
  int rc = 0;

  rc = m_db->m_t_ohead->cursor(m_db->m_txn, &m_dbc, 0);

  memset(&m_oid, 0, sizeof(m_oid));
  memset(&m_o, 0, sizeof(m_o));

  m_key.set_data(&m_oid);
  m_key.set_ulen(sizeof(m_oid));
  m_key.set_flags(DB_DBT_USERMEM);

  m_data.set_data(&m_o);
  m_data.set_ulen(sizeof(m_o));
  m_data.set_flags(DB_DBT_USERMEM);

  while ((rc = m_dbc->get(&m_key, &m_data, DB_NEXT)) == 0) {
    i.importHead(m_o.oid, m_o.cid, m_o.poid, m_o.boid, m_o.aoid, m_o.foid, m_o.loid, m_o.name, m_o.normname,
                 m_o.flags, m_o.time, m_o.body[0].time, m_o.body[1].time, m_o.body[0].size, m_o.body[1].size);
  }

  m_dbc->close();
}

wb_db_rbody::wb_db_rbody(wb_db *db, pwr_tOid oid, size_t size, void *p) :
  m_db(db), m_oid(oid), m_size(size), m_p(p), m_key(&m_oid, sizeof(m_oid)), m_data(p, size)
{
}

wb_db_rbody::wb_db_rbody(wb_db *db) :
  m_db(db), m_oid(pwr_cNOid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

wb_db_rbody::wb_db_rbody(wb_db *db, pwr_tOid oid) :
  m_db(db), m_oid(oid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

int wb_db_rbody::put(wb_db_txn *txn)
{
  return m_db->m_t_rbody->put(txn, &m_key, &m_data, 0);
}

int wb_db_rbody::put(wb_db_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.set_doff(offset);
  m_data.set_dlen(size);
  m_data.set_data(p);
  m_data.set_ulen(size);
  m_data.set_size(size);
  m_data.set_flags(DB_DBT_PARTIAL);

  return m_db->m_t_rbody->put(txn, &m_key, &m_data, 0);
}

int wb_db_rbody::get(wb_db_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.set_doff(offset);
  m_data.set_dlen(size);
  m_data.set_data(p);
  m_data.set_ulen(size);
  m_data.set_flags(DB_DBT_USERMEM|DB_DBT_PARTIAL);

  return m_db->m_t_rbody->get(txn, &m_key, &m_data, 0);
}

int wb_db_rbody::del(wb_db_txn *txn)
{
  return m_db->m_t_rbody->del(txn, &m_key, 0);
}

void wb_db_rbody::iter(void (*print)(pwr_tOid oid))
{
  int rc = 0;


  m_db->m_t_rbody->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_oid, 0, sizeof(m_oid));
  m_key.set_data(&m_oid);
  m_key.set_ulen(sizeof(m_oid));
  m_key.set_flags(DB_DBT_USERMEM);
  m_data.set_flags(DB_DBT_USERMEM|DB_DBT_PARTIAL);

  /* Walk through the database and print out the key/data pairs. */

  while (1) {
    try {
      rc = m_dbc->get(&m_key, &m_data, DB_NEXT);
    }
    catch (DbException &e) {
      printf("Exc: %s\n", e.what());
    }

    if (rc == DB_NOTFOUND)
      break;

    print(m_oid);
  }

  m_dbc->close();
}

void wb_db_rbody::iter(wb_import &i)
{
  int rc = 0;


  m_db->m_t_rbody->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_oid, 0, sizeof(m_oid));
  m_key.set_data(&m_oid);
  m_key.set_ulen(sizeof(m_oid));
  m_key.set_flags(DB_DBT_USERMEM);
  m_data.set_flags(DB_DBT_MALLOC);

  while (1) {
    try {
      rc = m_dbc->get(&m_key, &m_data, DB_NEXT);
    }
    catch (DbException &e) {
      printf("Exc: %s\n", e.what());
    }

    if (rc == DB_NOTFOUND)
      break;

    i.importRbody(m_oid, m_data.get_size(), m_data.get_data());
    free(m_data.get_data());
  }

  m_dbc->close();
}

wb_db_dbody::wb_db_dbody(wb_db *db, pwr_tOid oid, size_t size, void *p) :
  m_db(db), m_oid(oid), m_size(size), m_p(p), m_key(&m_oid, sizeof(m_oid)), m_data(p, size)
{
}

wb_db_dbody::wb_db_dbody(wb_db *db) :
  m_db(db), m_oid(pwr_cNOid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

wb_db_dbody::wb_db_dbody(wb_db *db, pwr_tOid oid) :
  m_db(db), m_oid(oid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

int wb_db_dbody::put(wb_db_txn *txn)
{
  return m_db->m_t_dbody->put(txn, &m_key, &m_data, 0);
}

int wb_db_dbody::put(wb_db_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.set_doff(offset);
  m_data.set_dlen(size);
  m_data.set_data(p);
  m_data.set_ulen(size);
  m_data.set_size(size);
  m_data.set_flags(DB_DBT_PARTIAL);

  return m_db->m_t_dbody->put(txn, &m_key, &m_data, 0);
}

int wb_db_dbody::get(wb_db_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.set_doff(offset);
  m_data.set_dlen(size);
  m_data.set_data(p);
  m_data.set_ulen(size);
  m_data.set_flags(DB_DBT_USERMEM|DB_DBT_PARTIAL);

  return m_db->m_t_dbody->get(txn, &m_key, &m_data, 0);
}

int wb_db_dbody::del(wb_db_txn *txn)
{
  return m_db->m_t_dbody->del(txn, &m_key, 0);
}

void wb_db_dbody::iter(void (*print)(pwr_tOid oid))
{
  int rc = 0;

  m_db->m_t_dbody->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_oid, 0, sizeof(m_oid));
  m_key.set_data(&m_oid);
  m_key.set_ulen(sizeof(m_oid));
  m_key.set_flags(DB_DBT_USERMEM);
  m_data.set_flags(DB_DBT_USERMEM|DB_DBT_PARTIAL);


  /* Walk through the database and print out the key/data pairs. */

  while (1) {

    try {
      rc = m_dbc->get(&m_key, &m_data, DB_NEXT);
    }
    catch (DbException &e) {
      printf("Exc: %s\n", e.what());
    }

    if (rc == DB_NOTFOUND)
      break;

    print(m_oid);
 }

  m_dbc->close();
}

void wb_db_dbody::iter(wb_import &i)
{
  int rc = 0;

  m_db->m_t_dbody->cursor(m_db->m_txn, &m_dbc, 0);

  /* Initialize the key/data pair so the flags aren't set. */
  memset(&m_oid, 0, sizeof(m_oid));
  m_key.set_data(&m_oid);
  m_key.set_ulen(sizeof(m_oid));
  m_key.set_flags(DB_DBT_USERMEM);
  m_data.set_flags(DB_DBT_MALLOC);


  /* Walk through the database and print out the key/data pairs. */

  while (1) {

    try {
      rc = m_dbc->get(&m_key, &m_data, DB_NEXT);
    }
    catch (DbException &e) {
      printf("Exc: %s\n", e.what());
    }

    if (rc == DB_NOTFOUND)
      break;

    i.importDbody(m_oid, m_data.get_size(), m_data.get_data());
    free(m_data.get_data());
 }

  m_dbc->close();
}

wb_db::wb_db() :
  m_vid(pwr_cNVid), m_txn(0)
{
}

wb_db::wb_db(pwr_tVid vid) :
  m_vid(vid), m_txn(0)
{
}

void wb_db::close()
{
  m_t_ohead->close(0);
  m_t_rbody->close(0);
  m_t_dbody->close(0);
  m_t_class->close(0);
  m_t_name->close(0);
  m_t_info->close(0);

  if (m_txn) {
    printstat(m_env, "before abort");
    int rc =  m_txn->abort();
    printf("int rc =  m_txn->abort(): %d\n", rc);
  }

  printstat(m_env, "before m_env->close(0)");
  m_env->close(0);
}

void wb_db::copy(wb_export &e, const char *fileName)
{
  pwr_tStatus sts;
  dcli_translate_filename(m_fileName, fileName);

  openDb(false);
  importVolume(e);
  close();
  openDb(true);

  try {
    m_env->txn_begin(0, (DbTxn **)&m_txn, 0);

    wb_db_info i(this);
    i.get(m_txn);
    m_vid = i.vid();
    m_cid = i.cid();
    strcpy(m_volumeName, i.name());
    commit(&sts);
  }
  catch (DbException &e) {
    //txn->abort();
    printf("exeption: %s\n", e.what());
  }

}

void wb_db::create(pwr_tVid vid, pwr_tCid cid, const char *volumeName, const char *fileName)
{
  m_vid = vid;
  m_cid = cid;
  pwr_tStatus sts;
  strcpy(m_volumeName, volumeName);
  dcli_translate_filename(m_fileName, fileName);
  size_t rbSize = 0;
  pwr_uVolume volume;
  pwr_tTime time;
  pwr_tOid oid;
  pwr_mClassDef flags;

  flags.m = pwr_mClassDef_System | pwr_mClassDef_TopObject;


  openDb(true);

  memset(&volume, 0, sizeof(volume));

  switch (cid) {
  case pwr_eClass_RootVolume:
    rbSize = sizeof(pwr_sRootVolume);
    break;
  case pwr_eClass_SubVolume:
    rbSize = sizeof(pwr_sSubVolume);
    break;
  case pwr_eClass_SystemVolume:
    rbSize = sizeof(pwr_sSystemVolume);
    break;
  case pwr_eClass_ClassVolume:
  case pwr_eClass_DetachedClassVolume:
    rbSize = sizeof(pwr_sClassVolume);
    break;
  case pwr_eClass_WorkBenchVolume:
    rbSize = sizeof(pwr_sWorkBenchVolume);
    break;
  case pwr_eClass_DirectoryVolume:
    rbSize = sizeof(pwr_sDirectoryVolume);
    break;
  case pwr_eClass_SharedVolume:
    rbSize = sizeof(pwr_sSharedVolume);
    break;
  case pwr_eClass_CreateVolume:
  case pwr_eClass_MountVolume:
    flags.m = pwr_mClassDef_System | pwr_mClassDef_NoAdopt;
    break;
  case pwr_eClass_MountObject:
    flags.m = pwr_mClassDef_System | pwr_mClassDef_TopObject | pwr_mClassDef_NoAdopt;
    break;
  case pwr_eClass_VolatileVolume:
  case pwr_eClass_ExternVolume:
    flags.m = pwr_mClassDef_System | pwr_mClassDef_TopObject | pwr_mClassDef_DevOnly;
    break;
  case pwr_eClass_DynamicVolume:
    break;
  default: ;
  }

  oid.vid = vid;
  oid.oix = pwr_cNOix;
  wb_name n(volumeName);

  try {
    m_env->txn_begin(0, (DbTxn **)&m_txn, 0);

    importHead(oid, cid, pwr_cNOid, pwr_cNOid, pwr_cNOid, pwr_cNOid, pwr_cNOid, n.name(), n.normName(), flags, time, time, time, rbSize, 0);

    wb_db_info i(this);
    i.get(m_txn);
    m_vid = i.vid();
    m_cid = i.cid();
    strcpy(m_volumeName, i.name());
    commit(&sts);
  }
  catch (DbException &e) {
    //txn->abort();
    printf("exeption: %s\n", e.what());
  }
}

wb_db_txn *wb_db::begin(wb_db_txn *txn)
{
  wb_db_txn *new_txn = 0;

  m_env->txn_begin((DbTxn *)txn, (DbTxn **)&new_txn, 0);

  return new_txn;
}

void  wb_db::open(const char *fileName)
{
  dcli_translate_filename(m_fileName, fileName);

  openDb(true);

  try {
    m_env->txn_begin(0, (DbTxn **)&m_txn, 0);

    wb_db_info i(this);
    i.get(m_txn);
    m_vid = i.vid();
    m_cid = i.cid();
    strcpy(m_volumeName, i.name());
  }
  catch (DbException &e) {
    //txn->abort();
    printf("exeption: %s\n", e.what());
  }
}

static void printstat(DbEnv *ep, char *s)
{
  DB_LOCK_STAT *lp;

  return;

  printf("DbEnv loc statistics, %s:\n", s);

  ep->lock_stat(&lp, 0);
#if DB_VERSION_MAJOR == 4 && DB_VERSION_MINOR > 0    
  printf("  lastid.......: %d\n", lp->st_id);
#else
  printf("  lastid.......: %d\n", lp->st_lastid);
#endif
  printf("  nmodes.......: %d\n", lp->st_nmodes);
  printf("  maxlocks:....: %d\n", lp->st_maxlocks);
  printf("  maxlockers...: %d\n", lp->st_maxlockers);
  printf("  maxobjects...: %d\n", lp->st_maxobjects);
  printf("  nlocks.......: %d\n", lp->st_nlocks);
  printf("  maxnlocks....: %d\n", lp->st_maxnlocks);
  printf("  nlockers.....: %d\n", lp->st_nlockers);
  printf("  maxnlockers..: %d\n", lp->st_maxnlockers);
  printf("  nobjects.....: %d\n", lp->st_nobjects);
  printf("  maxnobjects..: %d\n", lp->st_maxnobjects);
  printf("  nrequests....: %d\n", lp->st_nrequests);
  printf("  nreleases....: %d\n", lp->st_nreleases);
#if DB_VERSION_MAJOR == 4 && DB_VERSION_MINOR == 0    
  printf("  nnowaits.....: %d\n", lp->st_nnowaits);
  printf("  nconflicts...: %d\n", lp->st_nconflicts);
#endif
  printf("  ndeadlocks...: %d\n", lp->st_ndeadlocks);
  printf("  regsize......: %d\n", lp->st_regsize);
  printf("  region_wait..: %d\n", lp->st_region_wait);
  printf("  region_nowait: %d\n", lp->st_region_nowait);
  printf("\n");
}

extern "C" {
  //! Compare wb_db_class keys
  /*!
    - return <0 if ap < tp
    - return 0 if ap = tp.
    - return >0 if ap > tp.
  */
  typedef struct {
    pwr_tCid cid;
    pwr_tOid oid;
  } k_t;

  int wb_db_class_bt_compare(DB *dp, const DBT *ap, const DBT *tp)
  {

    k_t *akp = (k_t *)ap->data;
    k_t *tkp = (k_t *)tp->data;

    if (ap->size == 0 || tp->size == 0 || ap->size != tp->size)
      printf("a.size: %d, t.size: %d\n", ap->size, tp->size);

    if (akp->cid == tkp->cid) {
      if (akp->oid.vid == tkp->oid.vid) {
	if (akp->oid.oix == tkp->oid.oix)
	  return 0;
	else if (akp->oid.oix < tkp->oid.oix)
	  return -1;
	else
	  return 1;
      } else if (akp->oid.vid < tkp->oid.vid)
	return -1;
      else
	return 1;
    } else if (akp->cid < tkp->cid)
      return -1;
    else
      return 1;
  }
};

void wb_db::openDb(bool useTxn)
{
  struct stat sb;
  int rc;
  unsigned int lk_max_locks;
  unsigned int lk_max_objects;
  //DbTxn *txn = 0;

  /* Create the directory, read/write/access owner and group. */
  if (stat(m_fileName, &sb) != 0) {
    if (mkdir(m_fileName, S_IRWXU | S_IRWXG) != 0) {
      fprintf(stderr, "txnapp: mkdir: %s, %s\n", m_fileName, strerror(errno));
      //exit(1);
    }

  }

  get_config( m_fileName, &lk_max_locks, &lk_max_objects);

  m_env = new DbEnv(0/*DB_CXX_NO_EXCEPTIONS*/);
  printf("%s\n", m_env->version(0, 0, 0));
  m_env->set_errpfx("PWR db");
  m_env->set_cachesize(0, 256 * 1024 * 1024, 0);
  rc = m_env->set_lg_bsize(1024*1024*2);
  rc = m_env->set_lg_max(1024*1024*8*2);
  rc = m_env->set_lk_max_locks(lk_max_locks); // Decreased from 500000
  rc = m_env->set_lk_max_objects(lk_max_objects);

#if 0
  try {
    rc = m_env->txn_begin(0, &txn, 0);
  } catch (DbException &e) {
    printf("m_env->txn_begin, %s\n", e.what());
 }
#endif

  try {
    if (useTxn) {
      m_env->open(m_fileName,
      DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN | DB_RECOVER,
      0 /* S_IRUSR | S_IWUSR */);
    } else {
      m_env->open(m_fileName,
                DB_CREATE | DB_INIT_MPOOL | DB_PRIVATE,
      0 /*S_IRUSR | S_IWUSR */);
    }
  } catch (DbException &e) {
    printf("m_env->open, %s\n", e.what());
  }
  printstat(m_env, "after open env");

  m_t_ohead = new Db(m_env, 0);
  m_t_rbody = new Db(m_env, 0);
  m_t_dbody = new Db(m_env, 0);
  m_t_class = new Db(m_env, 0);
  m_t_name  = new Db(m_env, 0);
  m_t_info  = new Db(m_env, 0);

  rc = m_t_class->set_bt_compare(wb_db_class_bt_compare);

#if DB_VERSION_MAJOR == 4 && DB_VERSION_MINOR > 0    
  u_int32_t flags;
  if (useTxn) flags = DB_CREATE | DB_AUTO_COMMIT; 
  else flags = DB_CREATE;
  m_t_ohead->open(NULL, "ohead", NULL, DB_BTREE, flags, 0 /* S_IRUSR | S_IWUSR */);
  m_t_rbody->open(NULL, "rbody", NULL, DB_BTREE, flags, 0 /* S_IRUSR | S_IWUSR */);
  m_t_dbody->open(NULL, "dbody", NULL, DB_BTREE, flags, 0 /* S_IRUSR | S_IWUSR */);
  m_t_class->open(NULL, "class", NULL, DB_BTREE, flags, 0 /* S_IRUSR | S_IWUSR */);
  m_t_name->open(NULL, "name", NULL, DB_BTREE, flags, 0 /* S_IRUSR | S_IWUSR */);
  m_t_info->open(NULL, "info", NULL, DB_BTREE, flags, 0 /* S_IRUSR | S_IWUSR */);
#else
  m_t_ohead->open("ohead", NULL, DB_BTREE, DB_CREATE, 0 /* S_IRUSR | S_IWUSR */);
  m_t_rbody->open("rbody", NULL, DB_BTREE, DB_CREATE, 0 /* S_IRUSR | S_IWUSR */);
  m_t_dbody->open("dbody", NULL, DB_BTREE, DB_CREATE, 0 /* S_IRUSR | S_IWUSR */);
  m_t_class->open("class", NULL, DB_BTREE, DB_CREATE, 0 /* S_IRUSR | S_IWUSR */);
  m_t_name->open("name", NULL, DB_BTREE, DB_CREATE, 0 /* S_IRUSR | S_IWUSR */);
  m_t_info->open("info", NULL, DB_BTREE, DB_CREATE, 0 /* S_IRUSR | S_IWUSR */);
#endif
  printstat(m_env, "after open databases");

}


pwr_tOid wb_db::new_oid(wb_db_txn *txn)
{
  int rc = 0;
  pwr_tOid oid = pwr_cNOid;
  oid.vid = m_vid;
  wb_db_rbody b(this, oid);

  rc = b.get(txn, offsetof(pwr_sRootVolume, NextOix), sizeof(pwr_tOix), &oid.oix);
  if (rc)
    printf("wb_db::new_oid, b.get, rc %d\n", rc);
  oid.oix++;
  rc = b.put(txn, offsetof(pwr_sRootVolume, NextOix), sizeof(pwr_tOix), &oid.oix);
  if (rc)
    printf("wb_db::new_oid, b.put, rc %d\n", rc);

  return oid;
}

pwr_tOid wb_db::new_oid(wb_db_txn *txn, pwr_tOid oid)
{
  pwr_tOid woid;
  woid.vid = m_vid;
  woid.oix = oid.oix;

  try {
    wb_db_ohead o(this, txn, woid);
    return pwr_cNOid;
  } catch (DbException &e) {
    pwr_tOix nextoix;
    int rc = 0;
    pwr_tOid oid = pwr_cNOid;
    oid.vid = m_vid;
    wb_db_rbody b(this, oid);

    rc = b.get(txn, offsetof(pwr_sRootVolume, NextOix), sizeof(pwr_tOix), &nextoix);
    if (rc)
      printf("wb_db::new_oid, b.get, rc %d\n", rc);
    if ( !rc && nextoix < woid.oix + 1) {
      nextoix = woid.oix + 1;
      rc = b.put(txn, offsetof(pwr_sRootVolume, NextOix), sizeof(pwr_tOix), &nextoix);
      if (rc)
	printf("wb_db::new_oid, b.put, rc %d\n", rc);
    }
    return woid;
  }
}

int wb_db::del_family(wb_db_txn *txn, Dbc *cp, pwr_tOid poid)
{
  int ret = 0;
#if 0
  dbName name;
  name.poid = poid;
  name.normname = "";
  pwr_tOid oid = 0;

  FamilyKey  nk(po, );
  FamiltData nd(&oid, sizeof(oid));
  Dbc *ncp;
  cp->dup(*ncp, 0);

  while ((ret = cp->get(&nk, &nd, DB_NEXT)) != DB_NOTFOUND) {
    del_family(txn, ncp, oid);
    cp->del(0);
    oh_k ok(nd);
    oh_d od();
    m_db_ohead->get(txn, &ok, &od, 0);
    wb_DbClistK ck(od);
    m_db_class->del(txn, &ck, 0);
    wb_DbBodyK bk(od);
    m_db_obody->del(txn, &bk, 0);
    m_db_ohead->del(txn, &ok, 0);
  }
  ncp->close();

  ret = m_db_name->del(txn, &key, 0);
#endif
  return ret;
}

//
// Save all changes done in the current transaction.
//
bool wb_db::commit(pwr_tStatus *sts)
{
  int rc = 0;

  printstat(m_env, "before commit");
  rc = m_txn->commit(0);
  if (rc)
    printf("wb_db::commit, rc %d\n", rc);
  rc = m_env->txn_checkpoint(0, 0, 0);
  if (rc)
    printf("wb_db::commit, CHECK, rc %d\n", rc);
  m_env->txn_begin(0, (DbTxn **)&m_txn, 0);
  if ( rc)
    *sts = LDH__DB;
  else
    *sts = LDH__SUCCESS;
  printstat(m_env, "after commit");
  return true;
}

//
// Abort the current transactionm, the database is restored to
// the state it had before the current transaction started.
//
bool wb_db::abort(pwr_tStatus *sts)
{
  printstat(m_env, "before abort");
  int rc;

  rc = m_txn->abort();
  m_env->txn_begin(0, (DbTxn **)&m_txn, 0);
  printstat(m_env, "after abort");
  if ( rc)
    *sts = LDH__DB;
  else
    *sts = LDH__SUCCESS;
  return true;
}

bool wb_db::deleteFamily(pwr_tStatus *sts, wb_db_ohead *o)
{
  DbTxn *txn = 0;

  m_env->txn_begin(m_txn, &txn, 0);

  try {
    //unadopt(txn, wb_Position(o));
    //del_ohead(txn, o);
    //del_clist(txn, o);
    //del_body(txn, o);

    //txn->commit(0);
    //o->mark(is_deleted);

  }
  catch (DbException &e) {
    txn->abort();
  }

  return true;
}

#if 0
bool wb_db::deleteOset(pwr_tStatus *sts, wb_oset *o)
{
  DbTxn *txn = 0;

  m_env->txn_begin(m_txn, &txn, 0);

  try {
    //del_family(txn, o);
    //unadopt(txn, wb_Position(o));
    //del_ohead(txn, o);
    //del_clist(txn, o);
    //del_name(txn, o);
    //del_body(txn, o);

    txn->commit(0);
  }
  catch (DbException &e) {
    txn->abort();
  }

  return true;
}
#endif


bool wb_db::importVolume(wb_export &e)
{
  try {
    //    m_env->txn_begin(0, (DbTxn **)&m_txn, 0);
    //    m_txn = 0;

  printstat(m_env, "importVolume");
    e.exportHead(*this);
  printstat(m_env, "after head");
    e.exportRbody(*this);
  printstat(m_env, "after rbody");
    e.exportDbody(*this);
  printstat(m_env, "after dbody");
    e.exportMeta(*this);
  printstat(m_env, "after meta");
  //  commit(&sts);    
  // txn->commit(0);
  printstat(m_env, "after commit");
  //  m_env->txn_checkpoint(0, 0, 0);
  printstat(m_env, "after checkpoint");
    return true;
  }
  catch (DbException &e) {
  printstat(m_env, "after exception");
  //  m_txn->abort();
    printf("exeption: %s\n", e.what());
  printstat(m_env, "after abort");
    return false;
  }
}


bool wb_db::importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                       pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                       const char *name, const char *normname, pwr_mClassDef flags,
                       pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                       size_t rbSize, size_t dbSize)
{
  wb_db_ohead o(this, oid, cid, poid, boid, aoid, foid, loid, name, normname, flags, ohTime, rbTime, dbTime, rbSize, dbSize);
  o.put(m_txn);
  wb_db_name n(this, oid, poid, normname);
  int rc = n.put(m_txn);
  if (rc) {
    char newName[50];
    sprintf(newName, "O%u_%s", oid.oix, name);
    newName[31] = '\0';
    wb_name nn(newName);
    o.name(nn);
    o.put(m_txn);
    n.name(nn);
    n.put(m_txn);
  }

  wb_db_class c(this, cid, oid);
  c.put(m_txn);
  if (oid.oix == pwr_cNOix) { // This is the volume object
    wb_db_info i(this);
    i.cid(cid);
    i.vid(oid.vid);
    i.name(name);
    i.put(m_txn);
  }

  return true;
}


bool wb_db::importRbody(pwr_tOid oid, size_t size, void *body)
{
  wb_db_rbody b(this, oid, size, body);
  wb_db_ohead oh(this, oid);
  pwr_tTime time;
  clock_gettime(CLOCK_REALTIME, &time);
  oh.get(m_txn);
  oh.rbTime(time);
  oh.put(m_txn);
  b.put(m_txn);
  return true;
}

bool wb_db::importDbody(pwr_tOid oid, size_t size, void *body)
{
  wb_db_dbody b(this, oid, size, body);
  wb_db_ohead oh(this, oid);
  pwr_tTime time;
  clock_gettime(CLOCK_REALTIME, &time);
  oh.get(m_txn);
  oh.dbTime(time);
  oh.put(m_txn);
  b.put(m_txn);
  return true;
}

bool wb_db::importMeta(dbs_sMenv *mep)
{
  pwr_tStatus sts = 1;

  printf("Import meta, my filename: %s\n", m_fileName);

  if (mep != 0)
    dbs_Split(&sts, mep, m_fileName);

  return true;
}

static void get_config( char *name, unsigned int *lk_max_locks, 
			unsigned int *lk_max_objects)
{
  pwr_tFileName fname;
  FILE *fp;
  char line[200];
  char	line_elem[2][100];
  unsigned int max_locks;
  unsigned int max_objects;
  int nr;

  *lk_max_locks = 50000;
  *lk_max_objects = 20000;

  strcpy( fname, name);
  strcat( fname, ".cnf");
  dcli_translate_filename( fname, fname);

  fp = fopen( fname, "r");
  if ( !fp)
    return;

  while ( dcli_read_line( line, sizeof(line), fp)) {
    dcli_trim( line, line);
    if ( line[0] == '#')
      continue;
    if ( strcmp( line, "") == 0)
      continue;
    
    nr = dcli_parse( line, " 	", "",
		(char *) line_elem, sizeof( line_elem)/sizeof( line_elem[0]), 
		sizeof( line_elem[0]), 1);
    if ( nr != 2)
      continue;

    if ( cdh_NoCaseStrcmp( line_elem[0], "lk_max_locks") == 0) {
      nr = sscanf( line_elem[1], "%d", &max_locks);
      if ( nr == 1) {
	*lk_max_locks = max_locks;
	printf( "lk_max_locks.........%d\n", max_locks);
      }
    }
    else if ( cdh_NoCaseStrcmp( line_elem[0], "lk_max_objects") == 0) {
      nr = sscanf( line_elem[1], "%d", &max_objects);
      if ( nr == 1) {
	*lk_max_objects = max_objects;
	printf( "lk_max_objects.......%d\n", max_objects);
      }
    }
  }
  fclose( fp);
}
