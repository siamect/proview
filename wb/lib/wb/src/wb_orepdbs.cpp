#include "wb_orepdbs.h"
#include "wb_vrepdbs.h"
#include "wb_ldh_msg.h"
#include "wb_error.h"

void *
wb_orepdbs::operator new(size_t size, wb_vrepdbs *v)
{
    return (void *)v->new_wb_repdbs(size);
}

void
wb_orepdbs::operator delete(void *p, size_t size, wb_vrepdbs *v)
{
    v->delete_wb_orepdbs(size, p);
}


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
    return m_vrep->after(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::before(pwr_tStatus *sts) const
{
    return m_vrep->before(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::parent(pwr_tStatus *sts) const
{
    return m_vrep->parent(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::first(pwr_tStatus *sts) const
{
    return m_vrep->first(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::child(pwr_tStatus *sts, char *name) const
{
    return m_vrep->child(sts, (wb_orep*)this, name);
}

wb_orep *wb_orepdbs::last(pwr_tStatus *sts) const
{
    return m_vrep->last(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::next(pwr_tStatus *sts) const
{
    return m_vrep->next(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::previous(pwr_tStatus *sts) const
{
    return m_vrep->previous(sts, (wb_orep*)this);
}

wb_adrep *wb_orepdbs::attribute(pwr_tStatus *sts)
{
    return 0;;
}

wb_adrep *wb_orepdbs::attribute(pwr_tStatus *sts, const char *name)
{
    return 0;//m_vrep->attribute(sts, cid(), name);
}
