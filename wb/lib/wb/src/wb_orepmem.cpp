#include "wb_orepmem.h"


wb_orepmem::wb_orepmem( wb_vrepmem *v, mem_object *n): m_vrep(v), m_memobject(n)
{}

wb_orepmem::~wb_orepmem()
{
}

//
//  Operations declared in wb_Orep
//

pwr_tOid wb_orepmem::oid() const
{
  return m_memobject->m_oid;
}

pwr_tVid wb_orepmem::vid() const
{
  return m_memobject->m_oid.vid;
}

pwr_tOix wb_orepmem::oix() const
{
  return m_memobject->m_oid.oix;
}

pwr_tOix wb_orepmem::cid() const
{
  return m_memobject->m_cid;
}

pwr_tOid wb_orepmem::poid() const
{
  if ( m_memobject->fth)
    return m_memobject->fth->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepmem::foid() const
{
  if ( m_memobject->fch)
    return m_memobject->fch->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepmem::loid() const
{
  mem_object *n = m_memobject->get_lch();
  if ( n)
    return n->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepmem::boid() const
{
  if ( m_memobject->bws)
    return m_memobject->bws->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepmem::aoid() const
{
  if ( m_memobject->fws)
    return m_memobject->fws->m_oid;
  return pwr_cNOid;
}

const char * wb_orepmem::name() const
{
  return m_memobject->name();
}

wb_name wb_orepmem::longName()
{
  char str[200];

  m_vrep->objectName( this, str);
  return wb_name( str);
}


pwr_tTime wb_orepmem::ohTime() const
{
  pwr_tTime t = {0,0};
  return t;
}

pwr_mClassDef wb_orepmem::flags() const
{
  return m_memobject->m_flags;
}


bool wb_orepmem::isOffspringOf(const wb_orep *o) const
{
  return true;
}

wb_orep *wb_orepmem::ancestor(pwr_tStatus *sts)
{
  return m_vrep->ancestor(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::parent(pwr_tStatus *sts)
{
  return m_vrep->parent(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::after(pwr_tStatus *sts)
{
  return m_vrep->after(sts, (wb_orep *)this);
}


wb_orep *wb_orepmem::before(pwr_tStatus *sts)
{
  return m_vrep->before(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::first(pwr_tStatus *sts)
{
  return m_vrep->first(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::child(pwr_tStatus *sts, wb_name &name)
{
  return m_vrep->child(sts, (wb_orep *)this, name);
}

wb_orep *wb_orepmem::last(pwr_tStatus *sts)
{
  return m_vrep->last(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::next(pwr_tStatus *sts)
{
  return m_vrep->next(sts, (wb_orep *)this);
}

wb_orep *wb_orepmem::previous(pwr_tStatus *sts)
{
  return m_vrep->previous(sts, (wb_orep *)this);
}

wb_adrep *wb_orepmem::attribute(pwr_tStatus*, const char *aname)
{
  return 0;
}

wb_adrep *wb_orepmem::attribute(pwr_tStatus*)
{
  return 0;
}

