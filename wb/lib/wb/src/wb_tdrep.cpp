
extern "C" {
#include "co_cdh.h"
}
#include "wb_tdrep.h"
#include "wb_merep.h"
#include "wb_ldh_msg.h"
#include "co_dbs.h"
#include "wb_orepdbs.h"

void wb_tdrep::unref()
{
  if ( --m_nRef == 0)
    delete this;
}

wb_tdrep *wb_tdrep::ref()
{
  m_nRef++;
  return this;
}

wb_tdrep::wb_tdrep() : m_nRef(0), m_orep(0), m_sts(LDH__NOCLASS)
{
}

wb_tdrep::wb_tdrep( wb_mvrep *mvrep, pwr_tTid tid) : m_nRef(0)
{
  pwr_tOid oid = cdh_TypeIdToObjid( tid);
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_tdrep::wb_tdrep( wb_mvrep *mvrep, wb_name name) : m_nRef(0)
{
  char str[80];
  strcpy( str, "Type-");
  strcat( str, name.object());
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, str);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_tdrep::wb_tdrep( wb_mvrep *mvrep, const wb_adrep& a) : m_nRef(0)
{
  pwr_tOid oid = cdh_TypeIdToObjid( a.type());
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_tdrep::wb_tdrep( const wb_adrep& a) : m_nRef(0)
{
  pwr_tStatus sts;
  
  wb_tdrep *tdrep = a.vrep()->merep()->tdrep( &sts, a);
  m_sts = sts;
  m_orep = tdrep->m_orep;
  m_orep->ref();
  delete tdrep;
}

pwr_tTid wb_tdrep::tid()
{
  return cdh_TypeObjidToId( m_orep->oid());
}
