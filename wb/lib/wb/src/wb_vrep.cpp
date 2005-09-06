/* 
 * Proview   $Id: wb_vrep.cpp,v 1.4 2005-09-06 10:43:32 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

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
