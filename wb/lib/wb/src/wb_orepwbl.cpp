#include "wb_orepwbl.h"


wb_orepwbl::wb_orepwbl( wb_vrepwbl *v, wb_wblnode *n): m_vrep(v), m_wblnode(n)
{}

wb_orepwbl::~wb_orepwbl()
{
}

//
//  Operations declared in wb_Orep
//

pwr_tOid wb_orepwbl::oid() const
{
  return m_wblnode->m_oid;
}

pwr_tVid wb_orepwbl::vid() const
{
  return m_wblnode->m_oid.vid;
}

pwr_tOix wb_orepwbl::oix() const
{
  return m_wblnode->m_oid.oix;
}

pwr_tOix wb_orepwbl::cid() const
{
  return m_wblnode->m_cid;
}

pwr_tOid wb_orepwbl::poid() const
{
  if ( m_wblnode->o_fth)
    return m_wblnode->o_fth->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepwbl::foid() const
{
  if ( m_wblnode->o_fch)
    return m_wblnode->o_fch->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepwbl::loid() const
{
  ref_wblnode n = m_wblnode->get_o_lch();
  if ( n)
    return n->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepwbl::boid() const
{
  if ( m_wblnode->o_bws)
    return m_wblnode->o_bws->m_oid;
  return pwr_cNOid;
}

pwr_tOid wb_orepwbl::aoid() const
{
  if ( m_wblnode->o_fws)
    return m_wblnode->o_fws->m_oid;
  return pwr_cNOid;
}

const char * wb_orepwbl::name() const
{
    return m_wblnode->name;
}

wb_name wb_orepwbl::longName()
{
  char str[200];

  m_vrep->objectName( this, str);
  return wb_name( str);
}


pwr_tTime wb_orepwbl::ohTime() const
{
    return m_wblnode->getFileTime();
}


bool wb_orepwbl::isOffspringOf(const wb_orep *o) const
{
    return true;
}

wb_orep *wb_orepwbl::ancestor(pwr_tStatus *sts) const
{
    return m_vrep->ancestor(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::parent(pwr_tStatus *sts) const
{
    return m_vrep->parent(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::after(pwr_tStatus *sts) const
{
    return m_vrep->after(sts, (wb_orep *)this);
}


wb_orep *wb_orepwbl::before(pwr_tStatus *sts) const
{
    return m_vrep->before(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::first(pwr_tStatus *sts) const
{
    return m_vrep->first(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::child(pwr_tStatus *sts, const char *name) const
{
    return m_vrep->child(sts, (wb_orep *)this, name);
}

wb_orep *wb_orepwbl::last(pwr_tStatus *sts) const
{
    return m_vrep->last(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::next(pwr_tStatus *sts) const
{
    return m_vrep->next(sts, (wb_orep *)this);
}

wb_orep *wb_orepwbl::previous(pwr_tStatus *sts) const
{
    return m_vrep->previous(sts, (wb_orep *)this);
}


wb_position wb_orepwbl::position()
{
  wb_position pos;

  return pos;
}

wb_position wb_orepwbl::position(ldh_eDest dest)
{
  wb_position pos;

  return pos;
}


wb_adrep *wb_orepwbl::attribute(pwr_tStatus*, const char *aname)
{
  return 0;
}

wb_adrep *wb_orepwbl::attribute(pwr_tStatus*)
{
  return 0;
}

