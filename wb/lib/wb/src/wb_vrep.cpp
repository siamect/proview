#include "wb_vrep.h"


void wb_vrep::addSrep( wb_srep *srep)
{
  m_srep.push_back( srep);
}

void wb_vrep::removeSrep( wb_srep *srep)
{
  for ( srep_iterator it = m_srep.begin(); it != m_srep.end(); it++) {
    if ( *it == srep) {
      m_srep.erase( it);
      return;
    }
  }
}

//
//  Get first srep
//
wb_srep *wb_vrep::srep( pwr_tStatus *sts)
{
  srep_iterator it = m_srep.begin();
  if ( it == m_srep.end()) {
    *sts = LDH__NOSUCHSESS;
    return 0;
  }
  *sts = LDH__SUCCESS;
  return *it;
}

wb_srep *wb_vrep::nextSrep( pwr_tStatus *sts, wb_srep* srep)
{
  for ( srep_iterator it = m_srep.begin(); it != m_srep.end(); it++) {
    if ( *it == srep) {
      if ( ++it == m_srep.end()) {
        *sts =  LDH__NOSUCHSESS;
        return 0;
      }
      *sts = LDH__SUCCESS;
      return *it;
    }
  }
  *sts = LDH__NOSUCHSESS;
  return 0;
}
