#include <sys/stat.h>
#include <errno.h>
#include "wb_vrepdb.h"
#include "wb_orepdb.h"
#include "wb_erep.h"
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


wb_vrepdb::wb_vrepdb(wb_erep *erep, const char *fileName) :
  m_erep(erep), m_nRef(0), m_ohead()
{  
  strcpy(m_fileName, fileName);

  m_db = new wb_db();
  m_db->open(fileName);
  m_ohead.setDb(m_db);
  strcpy(m_name, m_db->volumeName());
  m_vid = m_db->vid();
  m_cid = m_db->cid();

  m_merep = m_erep->merep();
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

    wb_db_ohead o(m_db, m_txn, oid);
    
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

bool wb_vrepdb::createSnapshot(const char *fileName)
{
  return false;
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
    m_ohead.get(m_txn, o->oid());
  
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


//
// Save all changes done in the current transaction.
//

bool wb_vrepdb::commit(pwr_tStatus *sts)
{
  *sts = LDH__SUCCESS;
  return m_db->commit(m_txn);
}

//
// Abort the current transactionm, the database is restored to
// the state it had before the current transaction started.
//
bool wb_vrepdb::abort(pwr_tStatus *sts)
{
  *sts = LDH__SUCCESS;
  return m_db->abort(m_txn);
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts)
{
  *sts = LDH__SUCCESS;
  try {
    pwr_tOid oid;

    oid.vid = m_vid;
    oid.oix = pwr_cNOix;

    m_ohead.get(m_txn, oid);
    m_ohead.get(m_txn, m_ohead.foid());
  
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, pwr_tOid oid)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_txn, oid);
  
    return new (this) wb_orepdb(&m_ohead.m_o);
  }
  catch (DbException &e) {
    *sts = LDH__NOSUCHOBJ;
    printf("vrepdb: %s\n", e.what());
    return 0;
  }
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, wb_name &name)
{
  *sts = LDH__NYI;
  return 0;
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, const wb_orep *parent, wb_name &name)
{
  *sts = LDH__SUCCESS;
  try {
    wb_db_name n(m_db, m_txn, parent->oid(), name);
    m_ohead.get(m_txn, n.oid());
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
  *sts = LDH__NYI;
  return 0;
}

bool wb_vrepdb::copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination &d)
{
  return false;
}

wb_orep* wb_vrepdb::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name)
{
  *sts = LDH__NYI;
  wb_db_txn *txn = m_db->begin(0);
        
  try {
    wb_db_ohead o(m_db, m_db->new_oid(txn));

    o.put(txn);
        
    adopt(txn, o, d);

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
  *sts = LDH__NYI;
  wb_db_ohead o(m_db, orp->oid());
    
  wb_db_txn *txn = m_db->begin(0);
        
  try {
    unadopt(txn, o);

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

bool wb_vrepdb::deleteFamily(pwr_tStatus *sts, wb_orep *orep)
{
  *sts = LDH__NYI;
  return false;
}

bool wb_vrepdb::deleteOset(pwr_tStatus *sts, wb_oset *oset)
{
  *sts = LDH__NYI;
  return false;
}

bool wb_vrepdb::moveObject(pwr_tStatus *sts, wb_orep *orp, wb_destination &d)
{
  *sts = LDH__NYI;
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

bool wb_vrepdb::renameObject(pwr_tStatus *sts, wb_orep *orp, wb_name &name)
{ 
  *sts = LDH__NYI;
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

bool wb_vrepdb::writeAttribute(pwr_tStatus *sts, wb_orep *orp, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  try {
    m_ohead.get(m_txn, orp->oid());
    *sts = LDH__SUCCESS;
  
    switch (bix) {
    case pwr_eBix_rt:
    {
    
      wb_db_rbody rb(m_db, m_ohead.oid());
      rb.put(m_txn, offset, size, p);
      break;
    }
    case pwr_eBix_dev:
    {
    
      wb_db_dbody db(m_db, m_ohead.oid());
      db.put(m_txn, offset, size, p);
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
    m_ohead.get(m_txn, orp->oid());
    *sts = LDH__SUCCESS;
  
    switch (bix) {
    case pwr_eBix_rt:
    {
    
      wb_db_rbody rb(m_db, m_ohead.oid());
      rb.get(m_txn, offset, size, p);
      break;
    }
    case pwr_eBix_dev:
    {
    
      wb_db_dbody db(m_db, m_ohead.oid());
      db.get(m_txn, offset, size, p);
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
    m_ohead.get(m_txn, orp->oid());
    *sts = LDH__SUCCESS;
  
    switch (bix) {
    case pwr_eBix_rt:
    {
      wb_db_rbody rb(m_db, m_ohead.oid());
      rb.get(m_txn, 0, m_ohead.rbSize(), p);
      break;
    }
    case pwr_eBix_dev:
    {
      wb_db_dbody db(m_db, m_ohead.oid());
      db.get(m_txn, 0, m_ohead.dbSize(), p);
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
  *sts = LDH__NYI;
  return true;
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
    return m_ohead.get(m_txn, orp->oid()).cid();
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
    return m_ohead.get(m_txn, orp->oid()).ohTime();
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
  pwr_mClassDef flags; flags.m = 0; // Fix !!
  return flags;
}

pwr_tVid wb_vrepdb::vid(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    return m_ohead.get(m_txn, orp->oid()).vid();
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
    return m_ohead.get(m_txn, orp->oid()).oid();
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
    return m_ohead.get(m_txn, orp->oid()).oix();
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
    return m_ohead.get(m_txn, orp->oid()).poid();
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
    return m_ohead.get(m_txn, orp->oid()).foid();
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
    return m_ohead.get(m_txn, orp->oid()).loid();
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
    return m_ohead.get(m_txn, orp->oid()).aoid();
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
    return m_ohead.get(m_txn, orp->oid()).boid();
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
    m_ohead.get(m_txn, m_ohead.get(m_txn, orp->oid()).poid());
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
    pwr_tOid aoid = m_ohead.get(m_txn, orp->oid()).aoid();
    if (cdh_ObjidIsNull(aoid)) {
      *sts = LDH__NO_SIBLING;
      return 0;
    }
    
    m_ohead.get(m_txn, aoid);
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
    pwr_tOid boid = m_ohead.get(m_txn, orp->oid()).boid();
    if (cdh_ObjidIsNull(boid)) {
      *sts = LDH__NO_SIBLING;
      return 0;
    }
    
    m_ohead.get(m_txn, boid);
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
    pwr_tOid foid = m_ohead.get(m_txn, orp->oid()).foid();
    if (cdh_ObjidIsNull(foid)) {
      *sts = LDH__NO_CHILD;
      return 0;
    }
    
    m_ohead.get(m_txn, foid);
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
    wb_db_name n(m_db, m_txn, orp->oid(), name);
    m_ohead.get(m_txn, n.oid());
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
    pwr_tOid loid = m_ohead.get(m_txn, orp->oid()).loid();
    if (cdh_ObjidIsNull(loid)) {
      *sts = LDH__NO_CHILD;
      return 0;
    }
    
    m_ohead.get(m_txn, loid);
    return new (this) wb_orepdb(&m_ohead.m_o);
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
    m_ohead.get(m_txn, orp->oid());
    wb_db_class c(m_db, m_txn, m_ohead.cid());
    if (c.succ(m_ohead.oid())) {
      m_ohead.get(m_txn, c.oid());
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

wb_orep *wb_vrepdb::previous(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_txn, orp->oid());
    wb_db_class c(m_db, m_txn, m_ohead.cid());
    if (c.pred(m_ohead.oid())) {
      m_ohead.get(m_txn, c.oid());
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
    ;
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

  wb_db_name n(m_db, o.oid(), o.poid(), o.normname());
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
    
  wb_db_name n(m_db, o.oid(), o.poid(), o.normname());
  n.del(txn);
}

bool wb_vrepdb::exportVolume(wb_import &e)
{
  return false;
}

bool wb_vrepdb::exportHead(wb_import &e)
{
  return false;
}

bool wb_vrepdb::exportRbody(wb_import &e)
{
  return false;
}
    
bool wb_vrepdb::exportDbody(wb_import &e)
{
  return false;
}

bool wb_vrepdb::exportMeta(wb_import &e)
{
  return false;
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
