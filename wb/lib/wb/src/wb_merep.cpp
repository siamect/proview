#include "wb_merep.h"
#include "wb_erep.h"
#include "wb_ldh_msg.h"


// Get first volume
wb_mvrep *wb_merep::volume( pwr_tStatus *sts)
{
  mvrep_iterator it = m_mvrepdbs.begin(); 
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

wb_mvrep *wb_merep::volume(pwr_tStatus *sts, pwr_tVid vid)
{
  mvrep_iterator it = m_mvrepdbs.find( vid);
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

void wb_merep::addDbs( pwr_tStatus *sts, wb_mvrep *mvrep)
{
  mvrep_iterator it = m_mvrepdbs.find( mvrep->vid());
  if ( it == m_mvrepdbs.end()) {
    // Look for vrep in erep list... TODO
    
    m_mvrepdbs[mvrep->vid()] = mvrep;
    mvrep->ref();
    *sts = LDH__SUCCESS;
  }
  else
    *sts = LDH__VOLIDALREXI;
}

void wb_merep::removeDbs(pwr_tStatus *sts, wb_mvrep *mvrep)
{
  mvrep_iterator it = m_mvrepdbs.find( mvrep->vid()); 
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return;
  }
  it->second->unref();
  m_mvrepdbs.erase( it);
  *sts = LDH__SUCCESS;
}

wb_cdrep *wb_merep::cdrep( pwr_tStatus *sts, const wb_orep& o)
{
  if ( o.vrep() != m_vrep)
    // Fetch from other meta environment
    return m_erep->cdrep( sts, o);

  pwr_tVid vid = cdh_CidToVid(o.cid());
  mvrep_iterator it =  m_mvrepdbs.find( vid);
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second->cdrep( o);
}

wb_cdrep *wb_merep::cdrep( pwr_tStatus *sts, pwr_tCid cid)
{
  mvrep_iterator it =  m_mvrepdbs.find( cdh_CidToVid( cid));
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  try {
    *sts = LDH__SUCCESS;
    return it->second->cdrep( cid);
  }
  catch ( wb_error& e) {
    *sts = e.sts();
    return 0;
  }
}

wb_cdrep *wb_merep::cdrep( pwr_tStatus *sts, wb_name name)
{
  wb_cdrep *cdrep;

  for ( mvrep_iterator it = m_mvrepdbs.begin(); it != m_mvrepdbs.end(); it++) {
    try {
      cdrep = new wb_cdrep( it->second, name);
      *sts = LDH__SUCCESS;
      return cdrep;
    }
    catch ( wb_error& e) {
      // Not found in this volume, try next
    }
  }

  // Not found
  *sts = LDH__NOCLASS;
  return 0;
}

wb_tdrep *wb_merep::tdrep( pwr_tStatus *sts, const wb_adrep& a)
{
  if ( a.vrep() != m_vrep)
    // Fetch from other meta environment
    return m_erep->tdrep( sts, a);

  map<pwr_tVid, wb_mvrep*>::iterator it =  m_mvrepdbs.find( cdh_TidToVid(a.type()));
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second->tdrep( a);
}

wb_tdrep *wb_merep::tdrep( pwr_tStatus *sts, pwr_tTid tid)
{
  map<pwr_tVid, wb_mvrep*>::iterator it =  m_mvrepdbs.find( cdh_TidToVid( tid));
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  return it->second->tdrep( tid);
}

wb_tdrep *wb_merep::tdrep( pwr_tStatus *sts, wb_name name)
{
  wb_tdrep *tdrep;

  for ( mvrep_iterator it = m_mvrepdbs.begin(); it != m_mvrepdbs.end(); it++) {
    try {
      tdrep = new wb_tdrep( it->second, name);
      *sts = LDH__SUCCESS;
      return tdrep;
    }
    catch ( wb_error& e) {
      // Not found in this volume, try next
    }
  }

  // Not found
  *sts = LDH__NOTYPE;
  return 0;
}

