#include "wb_orepdbs.h"
#include "wb_vrepdbs.h"
#include "wb_ldh_msg.h"
#include "wb_error.h"

void *
wb_orepdbs::operator new(size_t size, wb_vrepdbs *v)
{
    return (void *)v->new_wb_orepdbs(size);
}

#if 0
void
wb_orepdbs::operator delete(void *p, wb_vrepdbs *v)
{
    v->delete_wb_orepdbs(p);
}
#endif

wb_orepdbs::wb_orepdbs(dbs_sObject *o)
{
    m_o = o;
    // m_v = ?; is fixed by operato new hopefully!!!
    m_refCount = 0;
}

#if 0
wb_orepdbs::~wb_orepdbs()
{
}
#endif

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

pwr_tVid wb_orepdbs::vid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->oid.vid;
}

pwr_tOix wb_orepdbs::oix() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->oid.oix;
}

pwr_tOid wb_orepdbs::poid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->poid;
}

pwr_tOid wb_orepdbs::foid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return pwr_cNOid;
}

pwr_tOid wb_orepdbs::loid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return pwr_cNOid;
}

pwr_tOid wb_orepdbs::boid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return pwr_cNOid;
}

pwr_tOid wb_orepdbs::aoid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return pwr_cNOid;
}

pwr_tCid wb_orepdbs::cid() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->cid;
}

const char * wb_orepdbs::name() const
{
    if (m_o == 0)
        throw wb_error_str("No current object");
    
    return m_o->name;
}

wb_name wb_orepdbs::longName()
{
  char str[200];

  m_vrep->objectName(this, str);
  return wb_name(str);
}

pwr_tTime wb_orepdbs::ohTime() const
{
    return m_o->time;
}

bool wb_orepdbs::isOffspringOf(const wb_orep *o) const
{
    return false;
}


wb_orep *wb_orepdbs::after(pwr_tStatus *sts) const
{
    return m_vrep->after(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::before(pwr_tStatus *sts) const
{
    return m_vrep->before(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::ancestor(pwr_tStatus *sts) const
{
    return m_vrep->ancestor(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::parent(pwr_tStatus *sts) const
{
    return m_vrep->parent(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::first(pwr_tStatus *sts) const
{
    return m_vrep->first(sts, (wb_orep*)this);
}

wb_orep *wb_orepdbs::child(pwr_tStatus *sts, const char *name) const
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

wb_position
wb_orepdbs::position()
{
  wb_position pos;

  return pos;
}

wb_position
wb_orepdbs::position(ldh_eDest dest)
{
  wb_position pos;

  return pos;
}
