#include "wb_orepdb.h"
#include "wb_vrepdb.h"

wb_orepdb::~wb_orepdb()
{
}

//
//  Operations declared in wb_orep
//

pwr_tOid wb_orepdb::oid() const
{
  pwr_tStatus sts;
    
  return m_vrep->oid(&sts, (wb_orep*)this);
}

pwr_tVid wb_orepdb::vid() const
{
  pwr_tStatus sts;
  return m_vrep->vid(&sts, (wb_orep*)this);
}

pwr_tOix wb_orepdb::oix() const
{
  pwr_tStatus sts;
  return m_vrep->oix(&sts, (wb_orep*)this);
}

pwr_tOid wb_orepdb::poid() const
{
  pwr_tStatus sts;
  return m_vrep->poid(&sts, (wb_orep*)this);
}

pwr_tOid wb_orepdb::foid() const
{
  pwr_tStatus sts;
  return m_vrep->foid(&sts, (wb_orep*)this);
}

pwr_tOid wb_orepdb::loid() const
{
  pwr_tStatus sts;
  return m_vrep->loid(&sts, (wb_orep*)this);
}

pwr_tOid wb_orepdb::boid() const
{
  pwr_tStatus sts;
  return m_vrep->boid(&sts, (wb_orep*)this);
}

pwr_tOid wb_orepdb::aoid() const
{
  pwr_tStatus sts;
  return m_vrep->aoid(&sts, (wb_orep*)this);
}

const char * wb_orepdb::name() const
{
  pwr_tStatus sts;
  return m_vrep->objectName(&sts, (wb_orep*)this);
}

pwr_tTime wb_orepdb::ohTime() const
{
  pwr_tStatus sts;
    
  return m_vrep->ohTime(&sts, (wb_orep*)this);
}

bool wb_orepdb::isOffspringOf(const wb_orep *o) const
{
  pwr_tStatus sts;
  return m_vrep->isOffspringOf(&sts, (wb_orep*)this, o);
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

wb_orep* wb_orepdb::first(pwr_tStatus *sts) const
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
