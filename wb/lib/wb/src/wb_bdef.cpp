#include "wb_bdef.h"
#include "wb_adef.h"
#include "wb_bdrep.h"

wb_bdef::wb_bdef() : wb_status(LDH__NOSUCHBODY), m_bdrep(0)
{
}

wb_bdef::wb_bdef(wb_bdrep *bdrep) : wb_status(LDH__SUCCESS), m_bdrep(bdrep)
{
  if ( bdrep == 0)
    m_sts = LDH__NOSUCHBODY;
  else
    m_bdrep->ref();
}

wb_bdef::wb_bdef(const wb_bdef& x) : wb_status(x.m_sts), m_bdrep(x.m_bdrep)
{
  if ( m_bdrep)
    m_bdrep->ref();
}

wb_bdef& wb_bdef::operator=(const wb_bdef &x)
{
  if ( x.m_bdrep)
    x.m_bdrep->ref();
  if ( m_bdrep)
    m_bdrep->unref();
  m_bdrep = x.m_bdrep;
  m_sts = x.m_sts;
  return *this;
}


wb_bdef::wb_bdef(const wb_adef *a)
{
}

wb_bdef::wb_bdef(const wb_orep *o, pwr_eBix bix)
{
}

pwr_tOid wb_bdef::boid()
{
  return m_bdrep->boid();
}

pwr_eBix wb_bdef::bix()
{
  return m_bdrep->bix();
}

size_t wb_bdef::size()
{
  return m_bdrep->size();
}

int wb_bdef::nAttribute()
{
  return m_bdrep->nAttribute();
}

const char* wb_bdef::name() const
{
  return m_bdrep->name();
}

wb_name wb_bdef::longName() const
{
  return m_bdrep->longName();
}

pwr_sAttrRef wb_bdef::aref()
{
  return m_bdrep->aref();
}

wb_adef wb_bdef::adef()
{
  pwr_tStatus sts;

  wb_adrep *adrep = m_bdrep->adrep( &sts);
  wb_adef a = wb_adef( adrep);
  return a;
}

wb_adef wb_bdef::adef( const char *aname)
{
  pwr_tStatus sts;

  wb_adrep *adrep = m_bdrep->adrep( &sts, aname);
  wb_adef a = wb_adef( adrep);
  return a;
]
