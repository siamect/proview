#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "db_cxx.h"
#include "wb_ldh.h"
#include "wb_destination.h"
#include "wb_db.h"
//#include "wb_import.h"
#include "wb_export.h"


wb_db_class::wb_db_class(wb_db *db, wb_db_txn *txn, pwr_tCid cid) :
  m_db(db), m_key(&m_k, sizeof(m_k))
{
  m_k.oid = pwr_cNOid;
  m_k.cid = cid;
    
  m_db->m_class->cursor(txn, &m_dbc, 0);
}

bool wb_db_class::succ(pwr_tOid oid)
{
  m_k.oid = oid;
  m_k.oid.oix += 1;
    
  int ret = m_dbc->get(&m_key, 0, DB_SET_RANGE);
  return ret == 0;
}

bool wb_db_class::pred(pwr_tOid oid)
{
  m_k.oid = oid;
    
  int ret = m_dbc->get(&m_key, 0, DB_SET_RANGE);
  if (ret != 0)
    return false;
  ret = m_dbc->get(&m_key, 0, DB_PREV);
  return ret == 0;
}

void wb_db_class::put(wb_db_txn *txn)
{
  m_db->m_class->put(txn, &m_key, 0, 0);
}

void wb_db_class::del(wb_db_txn *txn)
{
  m_db->m_class->del(txn, &m_key, 0);
}

wb_db_class::~wb_db_class()
{
  m_dbc->close();
}

wb_db_name::wb_db_name(wb_db *db, wb_db_txn *txn) :
  m_db(db), m_key(&m_k, sizeof(m_k)), m_data(&m_d, sizeof(m_d))
{
  m_k.poid = pwr_cNOid;
    
}

void wb_db_name::put(wb_db_txn *txn)
{
  m_db->m_name->put(txn, &m_key, 0, 0);
}

wb_db_ohead::wb_db_ohead(wb_db *db) :
  m_db(db), m_key(&m_o.oid, sizeof(m_o.oid)), m_data(&m_o, sizeof(m_o))
{
}

wb_db_ohead::wb_db_ohead(wb_db *db, pwr_tOid oid) :
  m_db(db), m_key(&m_o.oid, sizeof(m_o.oid)), m_data(&m_o, sizeof(m_o))
{
  m_o.oid = oid;
}

wb_db_ohead::wb_db_ohead(wb_db *db, wb_db_txn *txn, pwr_tOid oid) :
  m_db(db), m_key(&m_o.oid, sizeof(m_o.oid)), m_data(&m_o, sizeof(m_o))
{
  m_o.oid = oid;
  get(txn);
}

wb_db_ohead &wb_db_ohead::get(wb_db_txn *txn)
{
  m_db->m_ohead->get(txn, &m_key, &m_data, 0);
  return *this;
}

void wb_db_ohead::put(wb_db_txn *txn)
{
  m_db->m_ohead->put(txn, &m_key, &m_data, 0);
}

wb_db_ohead &wb_db_ohead::get(wb_db_txn *txn, pwr_tOid oid)
{
  m_o.oid = oid;
  m_db->m_ohead->get(txn, &m_key, &m_data, 0);
  return *this;
}

wb_db_ohead &wb_db_ohead::get(pwr_tOid oid)
{
  m_o.oid = oid;
  m_db->m_ohead->get(m_db->m_txn, &m_key, &m_data, 0);
  return *this;
}

void wb_db_ohead::del(wb_db_txn *txn)
{
  m_db->m_ohead->del(txn, &m_key, 0);
}

void wb_db::close()
{
  m_ohead->close(0);
  m_obody->close(0);
  m_class->close(0);
  m_name->close(0);
  m_info->close(0);
  
  m_env->close(0);
}

void wb_db::open(char *name)
{
  struct stat sb;

  /* Create the directory, read/write/access owner only. */
  if (stat(name, &sb) != 0) {
    if (mkdir(name, S_IRWXU) != 0) {
      fprintf(stderr, "txnapp: mkdir: %s, %s\n", name, strerror(errno));
      //exit(1);
    }
        
  }

  m_env = new DbEnv(0/*DB_CXX_NO_EXCEPTIONS*/);
  m_env->set_errpfx("PWR db");
    
  m_env->open(name,
              DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN | DB_RECOVER,
              S_IRUSR | S_IWUSR);

  m_ohead = new Db(m_env, 0);
  m_obody = new Db(m_env, 0);
  m_class = new Db(m_env, 0);
  m_name  = new Db(m_env, 0);
  m_info  = new Db(m_env, 0);
    
  m_ohead->open("ohead", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_obody->open("obody", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_class->open("class", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_name->open("name", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
  m_info->open("info", NULL, DB_BTREE, DB_CREATE, S_IRUSR | S_IWUSR);
    
}


pwr_tOid wb_db::new_oid(wb_db_txn *txn)
{
  int index = 1;
  int ret;

  Dbt key(&index, sizeof(index));
  pwr_tOid oid;
  Dbt data(&oid, sizeof(oid));
    
  ret = m_info->get(txn, &key, &data, 0);
  oid.oix++;
  ret = m_info->put(txn, &key, &data, 0);

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


bool wb_db::importVolume(wb_export *e)
{
  m_env->txn_begin(0, (DbTxn **)&m_txn, 0);

  e->exportHead(*this);
  e->exportRbody(*this);
  e->exportDbody(*this);
  e->exportMeta(*this);
    
  m_txn->commit(0);
  m_env->txn_checkpoint(0, 0, 0);
  return true;
}

bool wb_db::importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                       pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                       pwr_tObjName name, pwr_tObjName normname,
                       pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                       size_t rbSize, size_t dbSize)
{
  wb_db_ohead o(this, oid, cid, poid, boid, aoid, foid, loid, name, normname, ohTime, rbTime, dbTime, rbSize, dbSize);
  o.put(m_txn);
  wb_db_name n(this, poid, normname);
  n.put(m_txn);
  wb_db_class c(this, cid, oid);
  c.put(m_txn);
  return true;
}


bool wb_db::importRbody(pwr_tOid oid, size_t size, void *body)
{
  wb_db_body rb(this, oid, size, body);
  rb.put(m_txn);
  return true;
}

bool wb_db::importDbody(pwr_tOid oid, size_t size, void *body)
{
  wb_db_body db(this, oid, size, body);
  db.put(m_txn);
  return true;
}

bool  wb_db::importMeta(const char *fileName)
{
  return true;
}
