#include <sys/stat.h>
#include <errno.h>
#include "wb_vrepdb.h"
#include "wb_orepdb.h"
#include "db_cxx.h"
#include "wb_ldh.h"


void wb_vrepdb::unref()
{
  if (--m_nRef == 0)
    delete this;
}

wb_vrep *wb_vrepdb::ref()
{
  m_nRef++;
  return this;
}


//
// Save all changes done in the current transaction.
//
bool wb_vrepdb::commit(pwr_tStatus *sts)
{
  return m_db->commit(m_txn);
}

//
// Abort the current transactionm, the database is restored to
// the state it had before the current transaction started.
//
bool wb_vrepdb::abort(pwr_tStatus *sts)
{    
  return m_db->abort(m_txn);
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts)
{
//    m_ohead.get(m_txn, wb_oid(m_vid, 0));
  m_ohead.get(m_ohead.foid());
  
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, pwr_tOid oid)
{
  m_ohead.get(oid);    
  
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, wb_orep *parent, wb_name name)
{
  wb_db_name n(m_db, m_txn, parent->oid(), name);
  m_ohead.get(n.oid());
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep* wb_vrepdb::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination d, wb_name name)
{
  wb_db_txn *txn = m_db->begin(0);
        
  try {
    wb_db_ohead o(m_db, m_db->new_oid(txn));

    o.put(txn);
        
    m_db->adopt(txn, o, d);

/*
  pwr_tOid        oid;
  pwr_tCid        cid;
  pwr_tOid        poid;
  pwr_tObjName    name;
  pwr_tObjName    normname;
  pwr_tTime       time;
  pwr_tOid        boid;
  pwr_tOid        aoid;
  pwr_tOid        foid;
  pwr_tOid        loid;
    
  pwr_mClassDef   flags;
  struct {
  pwr_tTime      time;
  pwr_tUInt32    size;
  } body[2];

*/
    wb_db_class c(m_db, o);
    c.put(txn);

//        db_body b(m_t_body, o);
//        b.put(txn);
        
    m_db->commit(txn);
    return new (this) wb_orepdb(&o.m_o);
  }
  catch (DbException &e) {
    m_db->abort(txn);
    return 0;
  }    
}

bool wb_vrepdb::deleteObject(pwr_tStatus *sts, wb_orep *orp)
{    
  wb_db_ohead o(m_db, orp->oid());
    
  wb_db_txn *txn = m_db->begin(0);
        
  try {
    m_db->unadopt(txn, o);

    wb_db_class c(m_db, o);
    c.del(txn);

//        db_body b(m_t_body, o);
//        b.del(txn);

    o.del(txn);
        
    m_db->commit(txn);
  }
  catch (DbException &e) {
    m_db->abort(txn);
  }
    
  return true;
}

bool wb_vrepdb::moveObject(pwr_tStatus *sts, wb_orep *orp, wb_destination d)
{
#if 0
  if (!isLocal(o))
    return ;
  if (!isLocal(d))
    return ;
  wb_location l = o.location();
  //l.trigAnteUnadopt();
  //d.trigAnteAdopt();
#endif
     
  wb_db_txn *txn = m_db->begin(0);
        
  try {
    wb_db_ohead o(m_db, txn, orp->oid());
    unadopt(txn, o);
    adopt(txn, o, d);
    o.put(txn);
        
    m_db->commit(txn);
  }
  catch (DbException &e) {
    m_db->abort(txn);
  }
    
  return true;
}

bool wb_vrepdb::renameObject(pwr_tStatus *sts, wb_orep *orp, wb_name name)
{ 
  wb_db_txn *txn = m_db->begin(0);
        
  try {
    m_ohead.get(txn, orp->oid());

    wb_db_name n(m_db, m_ohead);
    n.del(txn);
    n.name(name);
    n.put(txn);

    m_ohead.name(name);
    m_ohead.put(txn);

    m_db->commit(txn);
    *sts = LDH__SUCCESS;
    return true;
  }
  catch (DbException &e) {
    m_db->abort(txn);
    m_ohead.clear();
    //*sts = LDH__?
    return false;
  }
}

bool wb_vrepdb::writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, unsigned int offset, unsigned int size, void *p)
{
  //body.oid = ?;
  //body.bix = ?;
  //key.data = &body;
  //key.len = sizeof(body);
    
  //data.data = ?;
  //data.offset = ?;
  //data.size = ?;
    
  //update(body-tab)

  return true;
}

void *wb_vrepdb::readAttribute(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, unsigned int offset, unsigned int size, void *p)
{
//    ob_k obk(o->oid(), bix);
  //ob_d obd;
    
  //obd.set_doff(offset);
  //obd.set_dlen(size);
    
    
  wb_db_txn *txn = m_db->begin(0);
        
  try {
    //m_db.obody.get(txn, &bk, &bd, 0);

    m_db->commit(txn);
  }
  catch (DbException &e) {
    m_db->abort(txn);
    // ?? How do we reset the Orep ???, the name was changed
  }
  return 0;
}

void *wb_vrepdb::readBody(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, void *p)
{
  return 0;
}

bool wb_vrepdb::writeBody(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, void *p)
{
  return true;
}

wb_orep *wb_vrepdb::ancestor(pwr_tStatus *sts, wb_orep *o)
{
  return 0;
}

pwr_tCid wb_vrepdb::cid(pwr_tStatus *sts, const wb_orep * const orp)
{
  return m_ohead.get(orp->oid()).cid();
}

pwr_tOid wb_vrepdb::poid(pwr_tStatus *sts, const wb_orep * const orp)
{
  return m_ohead.get(orp->oid()).poid();
}

pwr_tOid wb_vrepdb::foid(pwr_tStatus *sts, const wb_orep * const orp)
{
  return m_ohead.get(orp->oid()).foid();
}

pwr_tOid wb_vrepdb::loid(pwr_tStatus *sts, const wb_orep * const orp)
{
  return m_ohead.get(orp->oid()).loid();
}

pwr_tOid wb_vrepdb::aoid(pwr_tStatus *sts, const wb_orep * const orp)
{
  return m_ohead.get(orp->oid()).aoid();
}

pwr_tOid wb_vrepdb::boid(pwr_tStatus *sts, const wb_orep * const orp)
{
  return m_ohead.get(orp->oid()).boid();
}

wb_orep *wb_vrepdb::parent(pwr_tStatus *sts, wb_orep *orp)
{
  m_ohead.get(m_ohead.get(orp->oid()).poid());
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep *wb_vrepdb::after(pwr_tStatus *sts, wb_orep *orp)
{
  m_ohead.get(m_ohead.get(orp->oid()).aoid());
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep *wb_vrepdb::before(pwr_tStatus *sts, wb_orep *orp)
{
  m_ohead.get(m_ohead.get(orp->oid()).boid());
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep *wb_vrepdb::first(pwr_tStatus *sts, wb_orep *orp)
{
  m_ohead.get(m_ohead.get(orp->oid()).foid());
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep *wb_vrepdb::child(pwr_tStatus *sts, wb_orep *orp, const char *name)
{
  wb_db_name n(m_db, orp->oid(), name);
  m_ohead.get(n.oid());
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep *wb_vrepdb::last(pwr_tStatus *sts, wb_orep *orp)
{
  m_ohead.get(m_ohead.get(orp->oid()).loid());
  return new (this) wb_orepdb(&m_ohead.m_o);
}

wb_orep *wb_vrepdb::next(pwr_tStatus *sts, wb_orep *orp)
{
  m_ohead.get(orp->oid());
  wb_db_class c(m_db, m_ohead.cid());
  if (c.succ(m_ohead.oid())) {
    m_ohead.get(c.oid());
    return new (this) wb_orepdb(&m_ohead.m_o);
  } else {
    //*sts = LDH__?;
    return 0;
  }
}

wb_orep *wb_vrepdb::previous(pwr_tStatus *sts, wb_orep *orp)
{
  m_ohead.get(orp->oid());
  wb_db_class c(m_db, m_ohead.cid());
  if (c.pred(m_ohead.oid())) {
    m_ohead.get(c.oid());
    return new (this) wb_orepdb(&m_ohead.m_o);
  } else {
    //*sts = LDH__?;
    return 0;
  }
}

void wb_vrepdb::adopt(wb_db_txn *txn, wb_db_ohead &o, wb_destination &dest)
{
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
  }
    
  if (o.boid().vid != pwr_cNVid) {
    wb_db_ohead b(m_db, txn, o.boid());
    b.aoid(o.oid());
    b.put(txn);
  } else {
    d.foid(o.oid());
  }
    
  if (o.aoid().vid != pwr_cNVid) {
    wb_db_ohead a(m_db, txn, o.aoid());
    a.aoid(o.oid());
    a.put(txn);
  } else {
    d.loid(o.oid());
  }
    
  d.put(txn);

  wb_db_name n(m_db, o.poid(), o.name());
  n.put(txn);
}

void wb_vrepdb::unadopt(wb_db_txn *txn, wb_db_ohead &o)
{
  wb_db_ohead p(m_db, txn, o.poid());
    
  if (o.boid().vid != pwr_cNVid) {
    wb_db_ohead b(m_db, txn, o.boid());
    b.aoid(o.aoid());
    b.put(txn);
  } else {
    p.foid(o.aoid());
  }

  if (o.aoid().vid != pwr_cNVid) {
    wb_db_ohead a(m_db, txn, o.aoid());
    a.boid(o.boid());
    a.put(txn);
  } else {
    p.loid(o.boid());
  }

  o.poid(pwr_cNOid);
  o.aoid(pwr_cNOid);
  o.boid(pwr_cNOid);
    
  p.put(txn);
    
  wb_db_name n(m_db, o.poid(), o.name());
  n.del(txn);
}

