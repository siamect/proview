#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "co_dcli.h"
#include "db_cxx.h"
#include "wb_ldh.h"
#include "wb_destination.h"
#include "wb_db.h"
#include "wb_name.h"
#include "wb_export.h"


wb_db_info::wb_db_info(wb_db *db) :
  m_db(db), m_data(&m_volume, sizeof(m_volume))
{
}

void wb_db_info::get(wb_db_txn *txn)
{
  int index = 0;
  int ret;
  
  m_key.set_data(&index);
  m_key.set_size(sizeof(index));
  m_data.set_ulen(sizeof(m_volume));
  m_data.set_flags(DB_DBT_USERMEM);
  
  ret = m_db->m_t_info->get(txn, &m_key, &m_data, 0);
  printf("info get: %d\n", ret);
}

void wb_db_info::put(wb_db_txn *txn)
{
  int index = 0;
  int ret;
  
  m_key.set_data(&index);
  m_key.set_size(sizeof(index));
  
  ret = m_db->m_t_info->put(txn, &m_key, &m_data, 0);
  printf("info put: %d\n", ret);
}

wb_db_class::wb_db_class(wb_db *db, wb_db_txn *txn, pwr_tCid cid) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(0, 0), m_dbc(0)
{
  m_k.oid = pwr_cNOid;
  m_k.cid = cid;
    
  m_db->m_t_class->cursor(txn, &m_dbc, 0);
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
    
  int ret = m_dbc->get(&m_key, &m_data, DB_SET_RANGE);
  return ret == 0;
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

void wb_db_class::put(wb_db_txn *txn)
{
  m_db->m_t_class->put(txn, &m_key, &m_data, 0);
}

void wb_db_class::del(wb_db_txn *txn)
{
  m_db->m_t_class->del(txn, &m_key, 0);
}

wb_db_class::~wb_db_class()
{
  if (m_dbc)
    m_dbc->close();
}

wb_db_name::wb_db_name(wb_db *db, wb_db_txn *txn) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  m_k.poid = pwr_cNOid;
}

wb_db_name::wb_db_name(wb_db *db, wb_db_ohead &o) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  m_k.poid = o.poid();
  strcpy(m_k.normname, o.normname());
  m_d.oid = o.oid();
}

wb_db_name::wb_db_name(wb_db *db, pwr_tOid oid, pwr_tOid poid, const char *name) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  wb_name n(name);
  
  m_k.poid = poid;
  strcpy(m_k.normname, n.normName(cdh_mName_object));
  m_d.oid = oid;
}

wb_db_name::wb_db_name(wb_db *db, wb_db_txn *txn, pwr_tOid poid, wb_name &name) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  m_k.poid = poid;
  strcpy(m_k.normname, name.normName(cdh_mName_object));
  m_d.oid = pwr_cNOid;
}

//wb_db_name::wb_db_name(wb_db *db, pwr_tOid, char *name) :
//  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
//{
//}

//wb_db_name::wb_db_name(wb_db *db, pwr_tOid poid, const char *name) :
//  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
//{
//  m_k.poid = poid();
//  strcpy(m_k.normname, name);
//  m_d.oid = ?;
//}

//wb_db_name::wb_db_name(wb_db *db, wb_db_txn *txn, pwr_tOid poid, wb_name name) :
//  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
//{
//}


void wb_db_name::put(wb_db_txn *txn)
{
  m_db->m_t_name->put(txn, &m_key, &m_data, 0);
}

void wb_db_name::del(wb_db_txn *txn)
{
  m_db->m_t_name->del(txn, &m_key, 0);
}

void wb_db_name::name(wb_name &name)
{
  strcpy(m_k.normname, name.normName(cdh_mName_object));
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
  m_oid = oid;
}

wb_db_ohead::wb_db_ohead(wb_db *db, wb_db_txn *txn, pwr_tOid oid) :
  m_db(db), m_key(&m_oid, sizeof(m_oid)), m_data(&m_o, sizeof(m_o))
{
  memset(&m_o, 0, sizeof(m_o));
  m_oid = oid;
  get(txn);
}

wb_db_ohead::wb_db_ohead(wb_db *db, pwr_tOid oid, pwr_tCid cid,
                         pwr_tOid poid, pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                         const char *name, const char *normname,
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
    
  m_o.flags.m = 0;

  m_o.body[0].time = rbTime;
  m_o.body[0].size = rbSize;
  m_o.body[1].time = dbTime;
  m_o.body[1].size = dbSize;
}


wb_db_ohead &wb_db_ohead::get(wb_db_txn *txn)
{
  m_data.set_ulen(sizeof(m_o));
  m_data.set_flags(DB_DBT_USERMEM);

  int ret = m_db->m_t_ohead->get(txn, &m_key, &m_data, 0);
  return *this;
}

void wb_db_ohead::put(wb_db_txn *txn)
{
  m_db->m_t_ohead->put(txn, &m_key, &m_data, 0);
}

wb_db_ohead &wb_db_ohead::get(wb_db_txn *txn, pwr_tOid oid)
{
  m_oid = oid;
  m_data.set_ulen(sizeof(m_o));
  m_data.set_flags(DB_DBT_USERMEM);

  int ret = m_db->m_t_ohead->get(txn, &m_key, &m_data, 0);
  return *this;
}

void wb_db_ohead::del(wb_db_txn *txn)
{
  m_db->m_t_ohead->del(txn, &m_key, 0);
}

void wb_db_ohead::name(wb_name &name)
{
  strcpy(m_o.name, name.name(cdh_mName_object));
  strcpy(m_o.normname, name.normName(cdh_mName_object));
}

void wb_db_ohead::clear()
{
  memset(&m_o, 0, sizeof(m_o));
}

wb_db_rbody::wb_db_rbody(wb_db *db, pwr_tOid oid, size_t size, void *p) :
  m_db(db), m_oid(oid), m_size(size), m_p(p), m_key(&m_oid, sizeof(m_oid)), m_data(p, size)
{
}

wb_db_rbody::wb_db_rbody(wb_db *db, pwr_tOid oid) :
  m_db(db), m_oid(oid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

void wb_db_rbody::put(wb_db_txn *txn)
{
  m_db->m_t_rbody->put(txn, &m_key, &m_data, 0);
}


void wb_db_rbody::get(wb_db_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.set_doff(offset);
  m_data.set_dlen(size);
  m_data.set_data(p);
  
  m_db->m_t_rbody->get(txn, &m_key, &m_data, DB_DBT_PARTIAL);
}

wb_db_dbody::wb_db_dbody(wb_db *db, pwr_tOid oid, size_t size, void *p) :
  m_db(db), m_oid(oid), m_size(size), m_p(p), m_key(&m_oid, sizeof(m_oid)), m_data(p, size)
{
}

wb_db_dbody::wb_db_dbody(wb_db *db, pwr_tOid oid) :
  m_db(db), m_oid(oid), m_size(0), m_p(0), m_key(&m_oid, sizeof(m_oid)), m_data(0, 0)
{
}

void wb_db_dbody::put(wb_db_txn *txn)
{
  m_db->m_t_dbody->put(txn, &m_key, &m_data, 0);
}

void wb_db_dbody::get(wb_db_txn *txn, size_t offset, size_t size, void *p)
{
  m_data.set_doff(offset);
  m_data.set_dlen(size);
  m_data.set_data(p);
  
  m_db->m_t_dbody->get(txn, &m_key, &m_data, DB_DBT_PARTIAL);
}


wb_db::wb_db()
{
}

wb_db::wb_db(pwr_tVid vid) :
  m_vid(vid)
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
  
  m_env->close(0);
}

void wb_db::create(pwr_tVid vid, pwr_tCid cid, const char *volumeName, const char *fileName)
{
  m_vid = vid;
  m_cid = cid;
  strcpy(m_volumeName, volumeName);
  dcli_translate_filename(m_fileName, fileName);
  
  openDb();
}

wb_db_txn *wb_db::begin(wb_db_txn *txn)
{
  wb_db_txn *new_txn;
  
  m_env->txn_begin((DbTxn *)txn, (DbTxn **)&new_txn, 0);
  
  return new_txn;
}

void  wb_db::open(const char *fileName)
{
  dcli_translate_filename(m_fileName, fileName);
  openDb();
  
  m_env->txn_begin(0, (DbTxn **)&m_txn, 0);

  wb_db_info i(this);
  i.get(m_txn);
  m_vid = i.vid();
  m_cid = i.cid();
  strcpy(m_volumeName, i.name());
}

void wb_db::openDb()
{
  struct stat sb;

  /* Create the directory, read/write/access owner only. */
  if (stat(m_fileName, &sb) != 0) {
    if (mkdir(m_fileName, S_IRWXU) != 0) {
      fprintf(stderr, "txnapp: mkdir: %s, %s\n", m_fileName, strerror(errno));
      //exit(1);
    }
        
  }

  m_env = new DbEnv(0/*DB_CXX_NO_EXCEPTIONS*/);
  m_env->set_errpfx("PWR db");
    
  m_env->open(m_fileName,
              DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN | DB_RECOVER,
              S_IRUSR | S_IWUSR);

  m_t_ohead = new Db(m_env, 0);
  m_t_rbody = new Db(m_env, 0);
  m_t_dbody = new Db(m_env, 0);
  m_t_class = new Db(m_env, 0);
  m_t_name  = new Db(m_env, 0);
  m_t_info  = new Db(m_env, 0);
    
  m_t_ohead->open("ohead", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_t_rbody->open("rbody", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_t_dbody->open("dbody", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_t_class->open("class", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_t_name->open("name", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_t_info->open("info", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
    
}


pwr_tOid wb_db::new_oid(wb_db_txn *txn)
{
  int index = 1;
  int ret;

  Dbt key(&index, sizeof(index));
  pwr_tOid oid;
  Dbt data(&oid, sizeof(oid));
    
  ret = m_t_info->get(txn, &key, &data, 0);
  oid.oix++;
  ret = m_t_info->put(txn, &key, &data, 0);

  return oid;
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
bool wb_db::commit(wb_db_txn *txn)
{
  txn->commit(0);
  m_env->txn_checkpoint(0, 0, 0);
  return true;
}

//
// Abort the current transactionm, the database is restored to
// the state it had before the current transaction started.
//
bool wb_db::abort(wb_db_txn *txn)
{    
  txn->abort();
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

    txn->commit(0);
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
    m_env->txn_begin(0, (DbTxn **)&m_txn, 0);

    e.exportHead(*this);
    e.exportRbody(*this);
    e.exportDbody(*this);
    e.exportMeta(*this);
    
    m_txn->commit(0);
    m_env->txn_checkpoint(0, 0, 0);
    return true;
  }
  catch (DbException &e) {
    m_txn->abort();
    printf("exeption: %s\n", e.what());
    return false;
  }
}


bool wb_db::importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                       pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                       const char *name, const char *normname,
                       pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                       size_t rbSize, size_t dbSize)
{
  wb_db_ohead o(this, oid, cid, poid, boid, aoid, foid, loid, name, normname, ohTime, rbTime, dbTime, rbSize, dbSize);
  o.put(m_txn);
  wb_db_name n(this, oid, poid, normname);
  n.put(m_txn);
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
  wb_db_rbody rb(this, oid, size, body);
  rb.put(m_txn);
  return true;
}

bool wb_db::importDbody(pwr_tOid oid, size_t size, void *body)
{
  wb_db_dbody db(this, oid, size, body);
  db.put(m_txn);
  return true;
}

bool  wb_db::importMeta(dbs_sEnv *ep)
{
  return true;
}
