#include "wb_orepdb.h"
#include "wb_vrepdb.h"


wb_orepdb::wb_orepdb() :
  m_o(0), m_oid(pwr_cNOid)
{
}

wb_orepdb::wb_orepdb(pwr_tOid oid) :
  m_o(0), m_oid(oid)
{
}

wb_orepdb::wb_orepdb(db_sObject *o) :
  m_o(o), m_oid(o->oid)
{
}

wb_orepdb::~wb_orepdb()
{
}

void *
wb_orepdb::operator new(size_t size, wb_vrepdb *v)
{
  return (void *)v->new_wb_orepdb(size);
}

#if 1
void
wb_orepdb::operator delete(void *p)
{
  wb_orepdb *o = (wb_orepdb *)p;
  
  ((wb_vrepdb *)o->m_vrep)->delete_wb_orepdb(p);
}
#endif

//
//  Operations declared in wb_orep
//

pwr_tOid wb_orepdb::oid() const
{
  pwr_tStatus sts;
    
  return m_oid;
}

pwr_tVid wb_orepdb::cid() const
{
  pwr_tStatus sts;
  return m_vrep->cid(&sts, (wb_orep*)this);
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

wb_name wb_orepdb::longName()
{
  return wb_name();
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

wb_orep *wb_orepdb::ancestor(pwr_tStatus *sts)
{
  return m_vrep->ancestor(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::parent(pwr_tStatus *sts)
{
  return m_vrep->parent(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::after(pwr_tStatus *sts)
{
  return m_vrep->after(sts, (wb_orep*)this);
}


wb_orep *wb_orepdb::before(pwr_tStatus *sts)
{
  return m_vrep->before(sts, (wb_orep*)this);
}

wb_orep* wb_orepdb::first(pwr_tStatus *sts)
{
  return m_vrep->first(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::child(pwr_tStatus *sts, wb_name &name)
{
  return m_vrep->child(sts, (wb_orep*)this, name);
}

wb_orep *wb_orepdb::last(pwr_tStatus *sts)
{
  return m_vrep->last(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::next(pwr_tStatus *sts)
{
  return m_vrep->next(sts, (wb_orep*)this);
}

wb_orep *wb_orepdb::previous(pwr_tStatus *sts)
{
  return m_vrep->previous(sts, (wb_orep*)this);
}

wb_adrep *wb_orepdb::attribute(pwr_tStatus*, const char *aname)
{
  return 0;
}

wb_adrep *wb_orepdb::attribute(pwr_tStatus*)
{
  return 0;
}
