#include "wb_merep.h"
#include "wb_erep.h"
#include "wb_ldh_msg.h"


// Get first volume
wb_mvrep *wb_merep::volume( pwr_tStatus *sts)
{
  map<pwr_tVid, wb_mvrep*>::iterator it = m_mvrepdbs.begin(); 
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

wb_mvrep *wb_merep::volume(pwr_tStatus *sts, pwr_tVid vid)
{
  map<pwr_tVid, wb_mvrep*>::iterator it = m_mvrepdbs.find( vid);
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second;
}

void wb_merep::addDbs( pwr_tStatus *sts, wb_mvrep *mvrep)
{
  map<pwr_tVid, wb_mvrep*>::iterator it = m_mvrepdbs.find( mvrep->vid());
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
  map<pwr_tVid, wb_mvrep*>::iterator it = m_mvrepdbs.find( mvrep->vid()); 
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

  map<pwr_tVid, wb_mvrep*>::iterator it =  m_mvrepdbs.find( cdh_CidToVid(o.cid()));
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return it->second->cdrep( o);
}

wb_cdrep *wb_merep::cdrep( pwr_tStatus *sts, pwr_tCid cid)
{
  map<pwr_tVid, wb_mvrep*>::iterator it =  m_mvrepdbs.find( cdh_CidToVid( cid));
  if ( it == m_mvrepdbs.end()) {
    *sts = LDH__NOSUCHVOL;
    return 0;
  }
  return it->second->cdrep( cid);
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
