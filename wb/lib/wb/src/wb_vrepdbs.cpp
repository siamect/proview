#include "wb_vrepdbs.h"
#include "wb_orepdbs.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_import.h"
#include "wb_treeimport.h"

void wb_vrepdbs::unref()
{
  if (--m_nRef == 0) 
    delete this;
}

wb_vrep *wb_vrepdbs::ref()
{
  m_nRef++;
  return this;
}

wb_vrepdbs::wb_vrepdbs(wb_erep *erep, const char *fileName) : m_erep(erep), m_nRef(0), m_duplicate(false)
{
  strcpy(m_fileName, fileName);
  m_isDbsenvLoaded = false;
  if ( isCommonMeta())
    m_merep = m_erep->merep();
  else
    m_merep = new wb_merep(m_erep, (wb_mvrep *)this);
}

dbs_sEnv *wb_vrepdbs::dbsenv()
{
  dbs_sEnv *ep;
  pwr_tStatus sts;

  if (!m_isDbsenvLoaded) {
    ep = dbs_Map(&sts, &m_dbsenv, m_fileName);
    if (!ep) {
      throw wb_error(sts);
    }
    m_isDbsenvLoaded = true;
    strcpy(m_name, m_dbsenv.vp->name);
    m_vid = m_dbsenv.vp->vid;
    m_cid = m_dbsenv.vp->cid;
  } else {
    ep = &m_dbsenv;
  }
    
    
  return ep;
}

bool wb_vrepdbs::load()
{
  pwr_tStatus sts;
  bool rsts = ( dbsenv() != 0);

  if ( isMeta())
    m_merep->addDbs(&sts, (wb_mvrep *)this);

  return rsts;
}


wb_orep *wb_vrepdbs::object(pwr_tStatus *sts, pwr_tOid oid)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_OidToObject(sts, dbsenv(), oid);
  if (op == 0)
    return 0;

  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::object(pwr_tStatus *sts, wb_name &name)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_VolumeObject(sts, dbsenv());
    
  for (int i = 0; op && name.hasSegment(i); i++) {
    op = dbs_Child(sts, dbsenv(), op, name.normSegment(i));
  }
    
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }    
    
  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::object(pwr_tStatus *sts, const wb_orep *parent, wb_name &name)
{
  *sts = LDH__SUCCESS;
    
  dbs_sObject *op = dbs_Child(sts, dbsenv(), ((wb_orepdbs *)parent)->o(), name.normName(cdh_mName_object));
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }    
    
  return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name)
{
  *sts = LDH__NYI;
  return 0;
}


wb_orep *wb_vrepdbs::copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name)
{
  *sts = LDH__NYI;
  return 0;
}

bool wb_vrepdbs::copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination &d)
{
  *sts = LDH__NYI;
  return false;
}


bool wb_vrepdbs::moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination &d)
{
  *sts = LDH__NYI;
  return false;
}


bool wb_vrepdbs::deleteObject(pwr_tStatus *sts, wb_orep *orep)
{
  *sts = LDH__NYI;
  return false;
}

bool wb_vrepdbs::deleteFamily(pwr_tStatus *sts, wb_orep *orep)
{
  *sts = LDH__NYI;
  return false;
}

bool wb_vrepdbs::deleteOset(pwr_tStatus *sts, wb_oset *oset)
{
  *sts = LDH__NYI;
  return false;
}

bool wb_vrepdbs::renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name &name)
{
  *sts = LDH__NYI;
  return false;
}

bool wb_vrepdbs::commit(pwr_tStatus *sts)
{
  *sts = LDH__SUCCESS;
  return true;
}

bool wb_vrepdbs::abort(pwr_tStatus *sts)
{
  *sts = LDH__SUCCESS;
  return true;
}

bool wb_vrepdbs::writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  *sts = LDH__NYI;
  return false;
}

void *wb_vrepdbs::readAttribute(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{// note! must be compensated for offset !
  *sts = LDH__SUCCESS;

  dbs_sObject *op = ((wb_orepdbs *)o)->o();
  void *bp = dbs_Body(sts, dbsenv(), op, bix);
    
  if (bp == 0) {
    *sts = LDH__NOSUCHBODY;
    return 0;
  }
    
  if (p) {
    switch (bix) {
    case pwr_eBix_rt:
      memcpy(p, (char *)bp + offset, MIN(op->rbody.size - offset, size));
      break;
    case pwr_eBix_dev:
      memcpy(p, (char *)bp + offset, MIN(op->dbody.size - offset, size));
      break;
    default:
      *sts = LDH__NOSUCHBODY;
      break;
    }
    return p;
  }
        
  return (void *)((char *)bp + offset);
}

void *wb_vrepdbs::readBody(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, void *p)
{
  *sts = LDH__SUCCESS;
    
  dbs_sObject *op = ((wb_orepdbs *)o)->o();
  void *bp = dbs_Body(sts, dbsenv(), op, bix);
    
  if (bp == 0) {
    *sts = LDH__NOSUCHBODY;
    return 0;
  }
    
  if (p) {
    switch (bix) {
    case pwr_eBix_rt:
      memcpy(p, bp, op->rbody.size);
      break;
    case pwr_eBix_dev:
      memcpy(p, bp, op->dbody.size);
      break;
    default:
      *sts = LDH__NOSUCHBODY;
      break;
    }
    return p;
  }
        
  return bp;
}

bool wb_vrepdbs::writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p)
{
  *sts = LDH__NYI;
  return false;
}

wb_orep *wb_vrepdbs::ancestor(pwr_tStatus *sts, const wb_orep *o)
{    
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_Ancestor(sts, dbsenv(), ((wb_orepdbs *)o)->o());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::parent(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_Parent(sts, dbsenv(), ((wb_orepdbs *)o)->o());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::after(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_After(sts, dbsenv(), ((wb_orepdbs *)o)->o());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::before(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_Before(sts, dbsenv(), ((wb_orepdbs *)o)->o());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::first(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_First(sts, dbsenv(), ((wb_orepdbs *)o)->o());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::child(pwr_tStatus *sts, const wb_orep *o, wb_name &name)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_Child(sts, dbsenv(), ((wb_orepdbs *)o)->o(), name.normObject());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::last(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_Last(sts, dbsenv(), ((wb_orepdbs *)o)->o());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::next(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_Next(sts, dbsenv(), ((wb_orepdbs *)o)->o());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::previous(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_Previous(sts, dbsenv(), ((wb_orepdbs *)o)->o());
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;        
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

wb_srep *wb_vrepdbs::newSession()
{
//    *sts = LDH__SUCCESS;

  return (wb_srep*)0;
}

wb_orep *wb_vrepdbs::object(pwr_tStatus *sts)
{
  *sts = LDH__SUCCESS;

  dbs_sObject *op = dbs_Object(sts, dbsenv());    
  if (op == 0) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }

  return new (this) wb_orepdbs(op);
}

bool wb_vrepdbs::isLocal(const wb_orep *)
{
  //*sts = LDH__NYI;
  return false;
}

pwr_tCid wb_vrepdbs::cid() const
{
  return m_dbsenv.vp->cid;;
}

pwr_tVid wb_vrepdbs::vid() const
{
  return m_vid;
}

wb_erep *wb_vrepdbs::erep()
{
  return m_erep;
}

wb_vrep *wb_vrepdbs::next ()
{
  pwr_tStatus sts;

  return m_erep->nextVolume( &sts, vid());
}

wb_merep *wb_vrepdbs::merep() const
{
  return m_merep;
}

bool wb_vrepdbs::createSnapshot(const char *)
{
  return false;
}

wb_orepdbs *wb_vrepdbs::new_wb_orepdbs(size_t size)
{
  wb_orepdbs *o = (wb_orepdbs *) calloc(1, size);
  o->m_vrep = this;
  return o;
}

void wb_vrepdbs::delete_wb_orepdbs(void *p)
{
  free(p);
}

void wb_vrepdbs::objectName(const wb_orep *o, char *str)
{
  pwr_tStatus sts;
    
  *str = 0;

  dbs_ObjectToName(&sts, dbsenv(), ((wb_orepdbs *)o)->o(), str);
}

bool wb_vrepdbs::exportVolume(wb_import &i)
{
  return i.importVolume(*this);
}

bool wb_vrepdbs::exportHead(wb_import &i)
{
  dbs_sObject *op = 0;
  pwr_tStatus sts;
  pwr_mClassDef flags; flags.m = 0; // Fix !!!
  
  while ((op = dbs_NextHead(&sts, dbsenv(), op))) {
    i.importHead(op->oid, op->cid, op->poid, op->boid, op->aoid, op->foid, op->loid, op->name, op->normname,
                 flags, op->time, op->rbody.time, op->dbody.time, op->rbody.size, op->dbody.size);
  }

  return true;
}

bool wb_vrepdbs::exportRbody(wb_import &i)
{
  dbs_sBody *bp = 0;
  pwr_tStatus sts;
  
  while ((bp = dbs_NextRbody(&sts, dbsenv(), bp))) {
    i.importRbody(bp->oid, bp->size, (void*)(bp + 1));
  }

  return true;
}

bool wb_vrepdbs::exportDbody(wb_import &i)
{
  dbs_sBody *bp = 0;
  pwr_tStatus sts;
  
  while ((bp = dbs_NextDbody(&sts, dbsenv(), bp))) {
    i.importDbody(bp->oid, bp->size, (void*)(bp + 1));
  }

  return true;
}

bool wb_vrepdbs::exportMeta(wb_import &i)
{
  return false;
}

bool wb_vrepdbs::exportTree(wb_treeimport &i, pwr_tOid oid)
{
  pwr_tStatus sts;

  dbs_sObject *op = dbs_OidToObject( &sts, dbsenv(), oid);
  if (op == 0)
    throw wb_error(LDH__NOSUCHOBJ);

  exportTreeObject( i, op, true);
  return true;
}

bool wb_vrepdbs::exportTreeObject(wb_treeimport &i, dbs_sObject *op, bool isRoot)
{
  pwr_tStatus sts;
  dbs_sObject *before = dbs_Before(&sts, dbsenv(), op);
  dbs_sObject *parent = dbs_Parent(&sts, dbsenv(), op);
  dbs_sObject *first = dbs_First(&sts, dbsenv(), op);
  dbs_sObject *after = dbs_After(&sts, dbsenv(), op);
  pwr_tOid parentoid = pwr_cNOid;
  pwr_tOid beforeoid = pwr_cNOid;
  void *rbody = 0;
  void *dbody = 0;

  if ( parent && !isRoot)
    parentoid = parent->oid;
  if ( before && !isRoot)
    beforeoid = before->oid;
  if ( op->rbody.size)
    rbody = dbs_Body(&sts, dbsenv(), op, pwr_eBix_rt);
  if ( op->dbody.size)
    dbody = dbs_Body(&sts, dbsenv(), op, pwr_eBix_dev);

  pwr_mClassDef flags; flags.m = 0; // Fix !!!
  i.importTreeObject( m_merep, op->oid, op->cid, parentoid, beforeoid, op->name, flags,
		      op->rbody.size, op->dbody.size, rbody, dbody);

  if ( first)
    exportTreeObject( i, first, false);

  if ( !isRoot && after)
    exportTreeObject( i, after, false);

  return true;
}






