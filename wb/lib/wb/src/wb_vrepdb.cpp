#include <sys/stat.h>
#include <errno.h>
#include "co_cdh.h"
#include "wb_vrepdb.h"
#include "wb_orepdb.h"
#include "wb_erep.h"
#include "wb_dbs.h"
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

wb_vrepdb::wb_vrepdb(wb_erep *erep, pwr_tVid vid, pwr_tCid cid, const char *volumeName, const char *fileName) :
  m_erep(erep), m_nRef(0), m_ohead()
{  
  strcpy(m_fileName, fileName);

  m_db = new wb_db();
  m_db->create(vid, cid, volumeName, fileName);
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

bool wb_vrepdb::createSnapshot(const char *fileName)
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
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, pwr_tOid oid)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, oid);
  
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
  catch (DbException &e) {
    txn->abort();
    if (p)
      free(p);
    *sts = 2;//LDH__DB_ERROR;
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
      cdef.templateBody(&sts, pwr_eBix_rt, p);
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
      cdef.templateBody(&sts, pwr_eBix_dev, p);
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
  catch (DbException &e) {
    txn->abort();
    if (p)
      free(p);
    *sts = 2;
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
  catch (DbException &e) {
    txn->abort();
    *sts = 2;// LDH__DB_ERROR
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
  //if (rs)
  //printf("wb_vrepdb::deleteFamilyMember, o.get rs %d\n", rs);

  deleteFamilyMember(o.foid(), txn);
  
  deleteFamilyMember(o.aoid(), txn);
  
  wb_db_class c(m_db, o);
  rs = c.del(txn);
  if (rs)
    printf("wb_vrepdb::deleteFamilyMember, c.del rs %d\n", rs);
  
  wb_db_dbody db(m_db, o.oid());
  rs = db.del(txn);
  //if (rs)
    //printf("wb_vrepdb::deleteFamilyMember, db.get rs %d\n", rs);
  wb_db_rbody rb(m_db, o.oid());
  rs = rb.del(txn);
 // if (rs)
    //printf("wb_vrepdb::deleteFamilyMember, rb.get rs %d\n", rs);
  
  rs = o.del(txn);
  if (rs)
    printf("wb_vrepdb::deleteFamilyMember, o.del rs %d\n", rs);
  
  wb_db_name n(m_db, o.oid(), o.poid(), o.normname());
  rs = n.del(txn);
  if (rs)
    printf("wb_vrepdb::deleteFamilyMember, n.del rs %d\n", rs);

  return true;
}

bool wb_vrepdb::deleteFamily(pwr_tStatus *sts, wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  wb_db_ohead o(m_db, orp->oid());
    
  wb_db_txn *txn = m_db->begin(m_db->m_txn);
        
  try {
    o.get(txn);

    unadopt(txn, o);

    deleteFamilyMember(o.oid(), txn);
    
    txn->commit(0);
    return true;
  }
  catch (DbException &e) {
    txn->abort();
    *sts = 2;// LDH__DB_ERROR
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
#if 0
  if (!isLocal(o))
    return ;
  if (!isLocal(d))
    return ;
  wb_location l = o.location();
  //l.trigAnteUnadopt();
  //d.trigAnteAdopt();
#endif
     
  wb_db_txn *txn = m_db->begin(m_db->m_txn);
        
  try {
    wb_db_ohead o(m_db, txn, orp->oid());
    unadopt(txn, o);
    adopt(txn, o, d);
    o.put(txn);
        
    txn->commit(0);
  }
  catch (DbException &e) {
    txn->abort();
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
    *sts = 2;
    return false;
  }
}

bool wb_vrepdb::writeAttribute(pwr_tStatus *sts, wb_orep *orp, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  try {
    int rc = 0;
    m_ohead.get(m_db->m_txn, orp->oid());
    *sts = LDH__SUCCESS;
  
    switch (bix) {
    case pwr_eBix_rt:
    {
    
      wb_db_rbody rb(m_db, m_ohead.oid());
      rc = rb.put(m_db->m_txn, offset, size, p);
      if (rc)
        printf("wb_vrepdb::writeAttribute rb.put rc %d\n", rc);
      break;
    }
    case pwr_eBix_dev:
    {
    
      wb_db_dbody db(m_db, m_ohead.oid());
      rc = db.put(m_db->m_txn, offset, size, p);
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

pwr_mClassDef wb_vrepdb::flags(pwr_tStatus *sts, const wb_orep *orp)
{
  pwr_mClassDef flags; flags.m = 0; // Fix !!
  return flags;
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

wb_orep *wb_vrepdb::next(pwr_tStatus *sts, const wb_orep *orp)
{
  *sts = LDH__SUCCESS;
  try {
    m_ohead.get(m_db->m_txn, orp->oid());
    wb_db_class c(m_db, m_db->m_txn, m_ohead.cid());
    if (c.succ(m_ohead.oid())) {
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
  if (rc)
    printf("wb_vrepdb::adopt, n.put rc %d\n", rc);
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
  n.del(txn);
  if (rc)
    printf("wb_vrepdb::unadopt, n.del rc %d\n", rc);

  o.poid(pwr_cNOid);
  o.aoid(pwr_cNOid);
  o.boid(pwr_cNOid);
    
  p.put(txn);    
  if (rc)
    printf("wb_vrepdb::unadopt, p.put rc %d\n", rc);
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

bool wb_vrepdb::exportMeta(wb_import &i)
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
