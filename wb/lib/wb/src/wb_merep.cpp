#include <iostream>

#include "wb_merep.h"
#include "wb_cdrep.h"
#include "wb_bdrep.h"
#include "wb_erep.h"
#include "wb_tdrep.h"
#include "wb_attrname.h"
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

wb_mvrep *wb_merep::volume(pwr_tStatus *sts, const char *name)
{
  mvrep_iterator it;
  for ( it = m_mvrepdbs.begin(); it != m_mvrepdbs.end(); it++) {
    if ( cdh_NoCaseStrcmp( it->second->name(), name) == 0) {
      *sts = LDH__SUCCESS;
      return it->second;
    }
  }
  *sts = LDH__NOSUCHVOL;
  return 0;
}

void wb_merep::addDbs( pwr_tStatus *sts, wb_mvrep *mvrep)
{
  mvrep_iterator it = m_mvrepdbs.find( mvrep->vid());
  if ( it == m_mvrepdbs.end()) {
    // Look for vrep in erep list... TODO
    
    m_mvrepdbs[mvrep->vid()] = mvrep;
    mvrep->ref();
    cout << "Metavolume " << mvrep->vid() << " inserted\n";
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
  if ( m_erep && o.vrep() != m_vrep)
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

  if ( name.hasVolume()) {
    wb_mvrep *mvrep = volume( sts, name.volume());
    if ( EVEN( *sts)) return 0;
    try {
      cdrep = new wb_cdrep( mvrep, name);
      *sts = LDH__SUCCESS;
      return cdrep;
    }
    catch ( wb_error& e) {
      *sts = e.sts();
      return 0;
    }
  }
  else {
    for ( mvrep_iterator it = m_mvrepdbs.begin(); it != m_mvrepdbs.end(); it++) {
      try {
	cdrep = new wb_cdrep( it->second, name);
	*sts = LDH__SUCCESS;
	return cdrep;
      }
      catch ( wb_error& e) {
	// Not found in this volume, try next
	continue;
      }
    }
  }
  // Not found
  *sts = LDH__NOCLASS;
  return 0;
}

wb_tdrep *wb_merep::tdrep( pwr_tStatus *sts, const wb_adrep& a)
{
  if ( m_erep && a.vrep() != m_vrep)
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
  *sts = LDH__SUCCESS;
  return it->second->tdrep( tid);
}

wb_tdrep *wb_merep::tdrep( pwr_tStatus *sts, wb_name name)
{
  wb_tdrep *tdrep;

  if ( name.hasVolume()) {
    wb_mvrep *mvrep = volume( sts, name.volume());
    if ( EVEN( *sts)) return 0;

    try {
      tdrep = new wb_tdrep( mvrep, name);
      *sts = LDH__SUCCESS;
      return tdrep;
    }
    catch ( wb_error& e) {
      *sts = e.sts();
      return 0;
    }
  }
  else {
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
  }

  // Not found
  *sts = LDH__NOTYPE;
  return 0;
}

int wb_merep::getAttrInfoRec( wb_attrname *attr, pwr_eBix bix, pwr_tCid cid, size_t *size,
		     size_t *offset, pwr_tTid *tid, int *elements, 
		     pwr_eType *type, int *flags, int level)
{
  pwr_tStatus sts;

  if ( level > 0)
    bix = pwr_eBix_rt;

  wb_cdrep *cd = cdrep( &sts, cid);
  if ( EVEN(sts)) return 0;

  wb_adrep *adrep;
  wb_bdrep *bd = 0;
  if ( bix == 0) {
    // Search in all bodies
    adrep = cd->adrep( &sts, attr->attribute(level));
    if ( EVEN(sts)) {
      delete cd;
      return 0;
    }
  }
  else {
    // Search in specified body
    bd = cd->bdrep( &sts, bix);
    if ( EVEN(sts)) {
      delete cd;
      return 0;
    }
    adrep = bd->adrep( &sts, attr->attribute(level));
    if ( EVEN(sts)) {
      delete cd;
      delete bd;
      return 0;
    }
  }

  if ( attr->hasAttrIndex( level)) {
    int index = attr->attrIndex( level);
    if ( index >= adrep->nElement())
      return 0;

    *offset += adrep->offset() + index * adrep->size() / adrep->nElement();
    *size = adrep->size() / adrep->nElement();
  }
  else {
    *offset += adrep->offset();
    *size = adrep->size();
  }
  if ( attr->hasAttribute( level + 1)) {
    // Fix , Subclass: get cid from type of attr
    if ( !getAttrInfoRec( attr, bix, cid, size, offset, tid, elements, type, flags,
			  level + 1)) {
      delete cd;
      if ( bd) delete bd;
      delete adrep;
      return 0;
    }
  }
  else {
    *tid = adrep->tid();
    *type = adrep->type();
    *elements = adrep->nElement();
    *flags = adrep->flags();
  }
  delete cd;
  if ( bd) delete bd;
  delete adrep;
  return 1;
}
