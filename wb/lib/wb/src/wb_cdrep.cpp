
extern "C" {
#include "co_cdh.h"
}
#include "wb_cdrep.h"
#include "wb_merep.h"
#include "wb_ldh_msg.h"
#include "co_dbs.h"
#include "wb_orepdbs.h"
#include "wb_bdrep.h"

void wb_cdrep::unref()
{
  if ( --m_nRef == 0)
    delete this;
}

wb_cdrep *wb_cdrep::ref()
{
  m_nRef++;
  return this;
}

wb_cdrep::wb_cdrep() : m_nRef(0), m_orep(0), m_sts(LDH__NOCLASS)
{
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, pwr_tCid cid) : m_nRef(0)
{
  pwr_tOid oid = cdh_ClassIdToObjid( cid);
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, oid);
  m_orep->ref();
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, const wb_orep& o) : m_nRef(0)
{
  pwr_tOid oid = cdh_ClassIdToObjid( o.vid());
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( const wb_orep& o) : m_nRef(0)
{
  pwr_tStatus sts;
  
  wb_cdrep *cdrep = o.vrep()->merep()->cdrep( &sts, o);
  m_sts = sts;
  m_orep = cdrep->m_orep;
  m_orep->ref();
  delete cdrep;
}

wb_cdrep::wb_cdrep( wb_adrep *adrep) : m_nRef(0)
{
  pwr_tStatus sts;
  wb_orepdbs *orep = (wb_orepdbs *) adrep->m_orep->parent( &sts);
  if ( EVEN(sts)) throw wb_error(sts);
  orep->ref();
  m_orep = (wb_orepdbs *)orep->parent( &sts);
  if ( EVEN(sts)) throw wb_error(sts);
  m_sts = LDH__SUCCESS;
  orep->unref();
}

wb_bdrep *wb_cdrep::bdrep( pwr_tStatus *sts, char *bname)
{
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->child( sts, m_orep, bname);
  if ( EVEN(*sts))
    return 0;
  wb_bdrep *bdrep = new wb_bdrep( *orep);
  return bdrep;
}
