#include "wb_orepdbs.h"
#include "wb_vrepdbs.h"
#include "wb_ldh_msg.h"
#include "wb_error.h"

/*
void * wb_orepdbs::operator new(size_t size, wb_vrepdbs *v)
{
  return v->newOrepDbs(size);
}

void wb_orepdbs::operator delete(size_t size, void *p, wb_vrepdbs *v)
{
  v->deleteOrepDbs(size, p);
}
*/

wb_orepdbs::wb_orepdbs(dbs_sObject *o)
{
    m_o = o;
    // m_v = ?; is fixed by operato new hopefully!!!
    m_refCount = 0;
}

wb_orepdbs::~wb_orepdbs()
{
}

/*
wb_orepdbs *wb_orepdbs::ref()
{
    m_refCount++;
    return this;
}

void wb_orepdbs::unref()
{
    if (--m_refCount == 0)
        delete this;
}
*/

pwr_tOid wb_orepdbs::oid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->oid;
}

pwr_tOid wb_orepdbs::poid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->poid;
}

pwr_tCid wb_orepdbs::cid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->cid;
}

char * const wb_orepdbs::name()
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->name;
}

wb_orep *wb_orepdbs::after(pwr_tStatus *sts) const
{
    dbs_sQlink *succ = dbs_Qsucc(sts, m_vrep->m_dbsenv, &m_o->sib_ll);
    if (succ == 0)
        return 0;

    return new (m_vrep) wb_orepdbs(dbs_Qitem(succ, dbs_sObject, sib_ll));
}

wb_orep *wb_orepdbs::before(pwr_tStatus *sts) const
{
    dbs_sQlink *pred = dbs_Qpred(sts, m_vrep->m_dbsenv, &m_o->sib_ll);
    if (pred == 0)
        return 0;

    return new (m_vrep) wb_orepdbs(dbs_Qitem(pred, dbs_sObject, sib_ll));
}

wb_orep *wb_orepdbs::parent(pwr_tStatus *sts) const
{
    if (m_o->pref == dbs_cNref) {
        *sts = LDH__NO_PARENT;
        return 0;
    }
    
    dbs_sObject *dbs_o = (dbs_sObject*)dbs_Address(sts, m_vrep->m_dbsenv, m_o->pref);
    if (dbs_o == 0) {
        return 0;
    }
    

    return new (m_vrep) wb_orepdbs(dbs_o);
}

wb_adrep *attribute(pwr_tStatus *sts)
{
    return 0;;
}

wb_adrep *attribute(pwr_tStatus *sts, const char *name)
{
    return 0;//m_vrep->attribute(sts, cid(), name);
}
