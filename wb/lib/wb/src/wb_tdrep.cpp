
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
  init();
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
  init();
  m_sts = LDH__SUCCESS;
}

wb_tdrep::wb_tdrep( wb_mvrep *mvrep, const wb_adrep& a) : m_nRef(0)
{
  pwr_tOid oid = cdh_TypeIdToObjid( a.type());
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  init();
  m_sts = LDH__SUCCESS;
}

wb_tdrep::wb_tdrep( const wb_adrep& a) : m_nRef(0)
{
  pwr_tStatus sts;
  
  wb_tdrep *tdrep = a.vrep()->merep()->tdrep( &sts, a);
  m_sts = sts;
  m_orep = tdrep->m_orep;
  m_orep->ref();
  init();
  delete tdrep;
}

pwr_tTid wb_tdrep::tid() 
{
  return cdh_TypeObjidToId( m_orep->oid());
}

void wb_tdrep::init()
{
  pwr_tStatus sts;

  switch ( m_orep->cid()) {
    case pwr_eClass_TypeDef:
    {
      pwr_sTypeDef body;

      m_orep->m_vrep->readBody( &sts, m_orep, cdh_eBix_sys, (void *) &body);
      if ( EVEN(sts)) throw wb_error(sts);

      m_size = body.Size;
      m_type = body.Type;
      m_elements = body.Elements;
      m_typeref = body.TypeRef;
      break;
    }
    case pwr_eClass_Type:
    {
      pwr_sTypeDef body;

      m_orep->m_vrep->readBody( &sts, m_orep, cdh_eBix_sys, (void *) &body);
      if ( EVEN(sts)) throw wb_error(sts);

      m_size = body.Size;
      m_type = body.Type;
      m_elements = 1;
      m_typeref = (pwr_tTid) body.Type;
      break;
    }
    default:
      throw wb_error(LDH__NYI);
  }
}

const char *wb_tdrep::name() const
{
  return m_orep->name();
}

wb_name wb_tdrep::longName()
{
  return m_orep->longName();
}

