#include "wb_orepdb.h"

wb_orepdb::~wb_orepdb()
{
}

//
//  Operations declared in wb_Orep
//

pwr_tOid wb_orepdb::oid() const
{
    return m_ohead.oid;
}

pwr_tVid wb_orepdb::vid() const
{
    return m_ohead.oid.vid;
}

pwr_tOix wb_orepdb::oix() const
{
    return m_ohead.oid.oix;
}

pwr_tOid wb_orepdb::poid() const
{
    return m_ohead.poid;
}

pwr_tOid wb_orepdb::foid() const
{
    return m_ohead.foid;
}

pwr_tOid wb_orepdb::loid() const
{
    return m_ohead.loid;
}

pwr_tOid wb_orepdb::boid() const
{
    return m_ohead.boid;
}

pwr_tOid wb_orepdb::aoid() const
{
    return m_ohead.aoid;
}

const char * wb_orepdb::name() const
{
    return m_ohead.name;
}


pwr_tTime wb_orepdb::ohTime() const
{
    return m_ohead.time;
}


bool wb_orepdb::isOffspringOf(const wb_orep *o) const
{
    return true;
}

wb_orep *wb_orepdb::ancestor(pwr_tStatus *sts) const
{
    return m_vrep->ancestor(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::parent(pwr_tStatus *sts) const
{
    return m_vrep->parent(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::after(pwr_tStatus *sts) const
{
    return m_vrep->after(sts, (wb_orep*)this);
}


wb_orep *wb_orepdb::before(pwr_tStatus *sts) const
{
    return m_vrep->before(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::first(pwr_tStatus *sts) const
{
    return m_vrep->first(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::child(pwr_tStatus *sts, const char *name) const
{
    return m_vrep->child(sts, (wb_orep*)this, name);
}

wb_orep *wb_orepdb::last(pwr_tStatus *sts) const
{
    return m_vrep->last(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::next(pwr_tStatus *sts) const
{
    return m_vrep->next(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::previous(pwr_tStatus *sts) const
{
    return m_vrep->previous(sts, (wb_orep*)this);
}


wb_position wb_orepdb::position()
{
  wb_position pos;
  return pos;
}

wb_position wb_orepdb::position(ldh_eDest dest)
{
  wb_position pos;
  return pos;
}


wb_adrep *wb_orepdb::attribute(pwr_tStatus*, const char *aname)
{
  return 0;
}

wb_adrep *wb_orepdb::attribute(pwr_tStatus*)
{
  return 0;
}

