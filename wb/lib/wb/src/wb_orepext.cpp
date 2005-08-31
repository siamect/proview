#include "wb_orepext.h"


wb_orepext::wb_orepext( wb_vrepext *v, ext_object& n): m_vrep(v), m_extobject(n)
{}

wb_orepext::~wb_orepext()
{
}

//
//  Operations declared in wb_Orep
//

pwr_tOid wb_orepext::oid() const
{
  return m_extobject.m_oid;
}

pwr_tVid wb_orepext::vid() const
{
  return m_extobject.m_oid.vid;
}

pwr_tOix wb_orepext::oix() const
{
  return m_extobject.m_oid.oix;
}

pwr_tOix wb_orepext::cid() const
{
  return m_extobject.m_cid;
}

pwr_tOid wb_orepext::poid() const
{
  return m_extobject.fthoid;
}

pwr_tOid wb_orepext::foid() const
{
  return m_extobject.fchoid;
}

pwr_tOid wb_orepext::loid() const
{
  return m_extobject.lchoid;
}

pwr_tOid wb_orepext::boid() const
{
  return m_extobject.bwsoid;
}

pwr_tOid wb_orepext::aoid() const
{
  return m_extobject.fwsoid;
}

const char *wb_orepext::name() const
{
  return m_extobject.name();
}

wb_name wb_orepext::longName()
{
  return wb_name( m_extobject.longname());
}


pwr_tTime wb_orepext::ohTime() const
{
  pwr_tTime t = {0,0};
  return t;
}

pwr_mClassDef wb_orepext::flags() const
{
  return m_extobject.m_flags;
}


bool wb_orepext::isOffspringOf(const wb_orep *o) const
{
  return true;
}

wb_orep *wb_orepext::ancestor(pwr_tStatus *sts)
{
  return m_vrep->ancestor(sts, (wb_orep *)this);
}

wb_orep *wb_orepext::parent(pwr_tStatus *sts)
{
  return m_vrep->parent(sts, (wb_orep *)this);
}

wb_orep *wb_orepext::after(pwr_tStatus *sts)
{
  return m_vrep->after(sts, (wb_orep *)this);
}


wb_orep *wb_orepext::before(pwr_tStatus *sts)
{
  return m_vrep->before(sts, (wb_orep *)this);
}

wb_orep *wb_orepext::first(pwr_tStatus *sts)
{
  return m_vrep->first(sts, (wb_orep *)this);
}

wb_orep *wb_orepext::child(pwr_tStatus *sts, wb_name &name)
{
  return m_vrep->child(sts, (wb_orep *)this, name);
}

wb_orep *wb_orepext::last(pwr_tStatus *sts)
{
  return m_vrep->last(sts, (wb_orep *)this);
}

wb_orep *wb_orepext::next(pwr_tStatus *sts)
{
  return m_vrep->next(sts, (wb_orep *)this);
}

wb_orep *wb_orepext::previous(pwr_tStatus *sts)
{
  return m_vrep->previous(sts, (wb_orep *)this);
}

wb_adrep *wb_orepext::attribute(pwr_tStatus*, const char *aname)
{
  return 0;
}

wb_adrep *wb_orepext::attribute(pwr_tStatus*)
{
  return 0;
}








