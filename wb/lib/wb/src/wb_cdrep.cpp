#include "co_cdh.h"
#include "wb_cdrep.h"
#include "wb_merep.h"
#include "wb_ldh_msg.h"
#include "co_dbs.h"
#include "wb_orepdbs.h"
#include "wb_bdrep.h"
#include "wb_adrep.h"

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
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, wb_name name) : m_nRef(0)
{
  char str[80];
  strcpy( str, "Class-");
  strcat( str, name.object());
  wb_name n = wb_name( str);
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, n);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
  m_sts = LDH__SUCCESS;
}

wb_cdrep::wb_cdrep( wb_mvrep *mvrep, const wb_orep& o) : m_nRef(0)
{
  pwr_tOid oid = cdh_ClassIdToObjid( o.cid());
  m_orep = (wb_orepdbs *) mvrep->object( &m_sts, oid);
  if ( EVEN(m_sts)) throw wb_error( m_sts);

  m_orep->ref();
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

wb_bdrep *wb_cdrep::bdrep( pwr_tStatus *sts, const char *bname)
{
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->child( sts, m_orep, bname);
  if ( EVEN(*sts))
    return 0;
  wb_bdrep *bdrep = new wb_bdrep( *orep);
  return bdrep;
}

wb_bdrep *wb_cdrep::bdrep( pwr_tStatus *sts, pwr_eBix bix)
{
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->first( sts, m_orep);
  wb_orepdbs *old;
  while ( ODD(*sts)) {
    if ( orep->cid() == pwr_eClass_ObjBodyDef &&
         cdh_oixToBix( orep->oid().oix) ==  bix) {
      wb_bdrep *bdrep = new wb_bdrep( *orep);
      return bdrep;
    }
    old = orep;
    orep = (wb_orepdbs *)m_orep->m_vrep->after( sts, orep);

    // Delete
    old->ref();
    old->unref();
  }
  return 0;
}

wb_adrep *wb_cdrep::adrep( pwr_tStatus *sts, const char *aname)
{
  wb_name n = wb_name( aname);
  if ( n.evenSts()) {
    *sts = n.sts();
    return 0;
  }
  wb_orepdbs *orep_attr;
  wb_orepdbs *old;
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->first( sts);
  while ( ODD(*sts)) {
    orep->ref();
    if ( orep->cid() == pwr_eClass_ObjBodyDef) {
      orep_attr = (wb_orepdbs *)orep->m_vrep->child( sts, orep, n);
      if ( ODD(*sts)) {
        wb_adrep *adrep = new wb_adrep( *orep_attr);
        orep->unref();
        return adrep;
      }
    }
    old = orep;
    orep = (wb_orepdbs *)orep->after( sts);
    old->unref();
  }
  *sts = LDH__NOSUCHATTR;
  return 0;
}

wb_orep *wb_cdrep::classBody( pwr_tStatus *sts, const char *bname)
{
  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->child( sts, m_orep, bname);
  if ( EVEN(*sts))
    return 0;
  return orep;
}

pwr_tCid wb_cdrep::cid()
{
  return cdh_ClassObjidToId( m_orep->oid());
}

void wb_cdrep::templateBody( pwr_tStatus *sts, pwr_eBix bix, void *p)
{
  // Get objid for template object
  pwr_tOid oid;
  int cix = cdh_oixToCix( m_orep->oid().oix);  
  oid.vid = m_orep->oid().vid;
  oid.oix = cdh_cixToOix( cix, pwr_eBix_template, 0);

  wb_orepdbs *orep = (wb_orepdbs *)m_orep->m_vrep->object( sts, oid);
  if ( EVEN(*sts)) return;

  m_orep->m_vrep->readBody( sts, orep, bix, p);
  // Delete
  orep->ref();
  orep->unref();
}

const char *wb_cdrep::name() const
{
  return m_orep->name();
}

wb_name wb_cdrep::longName()
{
  return m_orep->longName();
}
