
#include "wb_bdrep.h"
#include "wb_adrep.h"
#include "wb_orepdbs.h"

void wb_bdrep::unref()
{
  if ( --m_nRef == 0)
    delete this;
}

wb_bdrep *wb_bdrep::ref()
{
  m_nRef++;
  return this;
}

wb_bdrep::wb_bdrep(wb_orepdbs& o) : m_nRef(0), m_orep(&o), m_sts(LDH__SUCCESS)
{
  m_orep->ref();
}

wb_bdrep::wb_bdrep( wb_adrep *adrep) : m_nRef(0)
{
  pwr_tStatus sts;
  m_orep = (wb_orepdbs *) adrep->m_orep->parent( &sts);
  if ( EVEN(sts)) throw wb_error(sts);
  m_sts = LDH__SUCCESS;
}

wb_bdrep::~wb_bdrep()
{
  m_orep->unref();
}

wb_adrep *wb_bdrep::adrep( pwr_tStatus *sts)
{
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->first( sts, m_orep);
  if ( EVEN(*sts)) return 0;

  return new wb_adrep(*orep);
}

wb_adrep *wb_bdrep::adrep( pwr_tStatus *sts, char *aname)
{
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->child( sts, m_orep, aname);
  if ( EVEN(*sts))
    return 0;
  wb_adrep *adrep = new wb_adrep( *orep);
  return adrep;
}

cdh_eBix wb_bdrep::bix()
{
  return (cdh_eBix) cdh_oixToBix( m_orep->oid().oix);
}

size_t wb_bdrep::size()
{
  pwr_tStatus sts;
  pwr_sObjBodyDef body;

  m_orep->m_vrep->readBody( &sts, m_orep, bix(), (void *) &body);
  if ( EVEN(sts)) throw wb_error(sts);

  return body.Size;
}
