#include "wb_vrepdbs.h"
#include "wb_orepdbs.h"
#include "wb_erep.h"
#include "wb_merep.h"

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

wb_vrepdbs::wb_vrepdbs(wb_erep *erep, const char *fileName) : m_erep(erep)
{
    m_merep = new wb_merep(m_erep, (wb_mvrep *)this);
    strcpy(m_fileName, fileName);
    m_isDbsenvLoaded = false;
}

dbs_sEnv *
wb_vrepdbs::dbsenv()
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
    } else {
        ep = &m_dbsenv;
    }
    
    
    return ep;
}

bool
wb_vrepdbs::load()
{
    pwr_tStatus sts;
    bool rsts = ( dbsenv() != 0);
    m_merep->addDbs(&sts, (wb_mvrep *)this); // only if i am a class volume
    return rsts;
}


wb_orep *
wb_vrepdbs::object(pwr_tStatus *sts, pwr_tOid oid)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_OidToObject(sts, dbsenv(), oid);
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}

wb_orep *
wb_vrepdbs::object(pwr_tStatus *sts, wb_name name)
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

wb_orep *
wb_vrepdbs::object(pwr_tStatus *sts, wb_orep *parent, wb_name name)
{
    *sts = LDH__SUCCESS;
    
    dbs_sObject *op = dbs_Child(sts, dbsenv(), ((wb_orepdbs *)parent)->o(), name.normName(cdh_mName_object));
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;
        return 0;
    }    
    
    return new (this) wb_orepdbs(op);
}


wb_orep *
wb_vrepdbs::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination d, wb_name name)
{
    *sts = LDH__NYI;
    return 0;
}


wb_orep *
wb_vrepdbs::copyObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d, wb_name name)
{
    *sts = LDH__NYI;
    return 0;
}

bool
wb_vrepdbs::copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination d)
{
    *sts = LDH__NYI;
    return false;
}


bool
wb_vrepdbs::moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d)
{
    *sts = LDH__NYI;
    return false;
}


bool
wb_vrepdbs::deleteObject(pwr_tStatus *sts, wb_orep *orep)
{
    *sts = LDH__NYI;
    return false;
}

bool
wb_vrepdbs::deleteFamily(pwr_tStatus *sts, wb_orep *orep)
{
    *sts = LDH__NYI;
    return false;
}

bool
wb_vrepdbs::deleteOset(pwr_tStatus *sts, wb_oset *oset)
{
    *sts = LDH__NYI;
    return false;
}


bool
wb_vrepdbs::renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name name)
{
    *sts = LDH__NYI;
    return false;
}

bool
wb_vrepdbs::commit(pwr_tStatus *sts)
{
    *sts = LDH__SUCCESS;
    return true;
}

bool
wb_vrepdbs::abort(pwr_tStatus *sts)
{
    *sts = LDH__SUCCESS;
    return true;
}


bool
wb_vrepdbs::writeAttribute(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, unsigned int offset, unsigned int size, void *p)
{
    //*sts = LDH__NYI;
    return false;
}


void *
wb_vrepdbs::readAttribute(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, unsigned int offset, unsigned int size, void *p)
{
    //*sts = LDH__NYI;
    return 0;
}


void *
wb_vrepdbs::readBody(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, void *p)
{
    dbs_sObject *op = ((wb_orepdbs *)o)->o();
    void *bp = dbs_Body(sts, dbsenv(), op, bix);
    
    if (bp == 0)
        return 0;
    
    if (p) {
        memcpy(p, bp, op->rbody.size);
        return p;
    }
        
    return bp;
}


bool
wb_vrepdbs::writeBody(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, void *p)
{
    *sts = LDH__NYI;
    return false;
}


wb_orep *
wb_vrepdbs::ancestor(pwr_tStatus *sts, wb_orep *o)
{    
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_Ancestor(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}


wb_orep *
wb_vrepdbs::parent(pwr_tStatus *sts, wb_orep *o)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_Parent(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}

wb_orep *
wb_vrepdbs::after(pwr_tStatus *sts, wb_orep *o)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_After(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}

wb_orep *
wb_vrepdbs::before(pwr_tStatus *sts, wb_orep *o)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_Before(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}


wb_orep *
wb_vrepdbs::first(pwr_tStatus *sts, wb_orep *o)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_First(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}


wb_orep *
wb_vrepdbs::child(pwr_tStatus *sts, wb_orep *o, wb_name name)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_Child(sts, dbsenv(), ((wb_orepdbs *)o)->o(), name.normObject());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}


wb_orep *
wb_vrepdbs::last(pwr_tStatus *sts, wb_orep *o)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_Last(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}


wb_orep *
wb_vrepdbs::next(pwr_tStatus *sts, wb_orep *o)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_Next(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}

wb_orep *
wb_vrepdbs::previous(pwr_tStatus *sts, wb_orep *o)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_Previous(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;        
        return 0;
    }

    return new (this) wb_orepdbs(op);
}


wb_srep *
wb_vrepdbs::newSession()
{
//    *sts = LDH__SUCCESS;

    return (wb_srep*)0;
}


wb_orep *
wb_vrepdbs::object(pwr_tStatus *sts)
{
    *sts = LDH__SUCCESS;

    dbs_sObject *op = dbs_Object(sts, dbsenv());    
    if (op == 0) {
        *sts = LDH__NOSUCHOBJ;
        return 0;
    }

    return new (this) wb_orepdbs(op);
}

bool
wb_vrepdbs::isLocal(wb_orep *) const
{
    //*sts = LDH__NYI;
    return false;
}

pwr_tCid
wb_vrepdbs::cid() const
{
    return m_dbsenv.vp->cid;;
}

pwr_tVid
wb_vrepdbs::vid() const
{
    return m_vid;
}

wb_erep *
wb_vrepdbs::erep() const
{
    return m_erep;
}

wb_vrep *
wb_vrepdbs::next () const
{
  pwr_tStatus sts;

  return m_erep->nextVolume( &sts, vid());
}

wb_merep *
wb_vrepdbs::merep() const
{
    return m_merep;
}

bool
wb_vrepdbs::createSnapshot(char *)
{
    return false;
}

void
wb_vrepdbs::iterObject(wb_dbs *)
{
}

void
wb_vrepdbs::iterRbody(wb_dbs *)
{
}

void
wb_vrepdbs::iterDbody(wb_dbs *)
{
}

wb_orepdbs *
wb_vrepdbs::new_wb_orepdbs(size_t size)
{
    wb_orepdbs *o = (wb_orepdbs *) calloc(1, size);
    o->m_vrep = this;
    return o;
}

void
wb_vrepdbs::delete_wb_orepdbs(void *p)
{
    free(p);
}

void
wb_vrepdbs::objectName(wb_orep *o, char *str)
{
    pwr_tStatus sts;
    
    *str = 0;

    dbs_ObjectToName(&sts, dbsenv(), ((wb_orepdbs *)o)->o(), str);
}
