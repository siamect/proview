/** 
 * Proview   $Id: wb_bdrep.h,v 1.12 2005-09-01 14:57:57 claes Exp $
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

#ifndef wb_bdrep_h
#define wb_bdrep_h

#include "pwr.h"
#include "wb_ldh.h"
#include "wb_name.h"

class wb_adrep;
class wb_orep;

class wb_bdrep
{
  int m_nRef;
  wb_orep *m_orep;
  pwr_tStatus m_sts;
    
public:
  wb_bdrep();
  wb_bdrep(wb_orep& o); // own orep
  wb_bdrep( wb_adrep *adrep);
  ~wb_bdrep();

  void unref();
  wb_bdrep *ref();

  pwr_sAttrRef aref() { pwr_sAttrRef a; return a;} // Fix
  size_t size();
  int nAttribute();
  pwr_eBix bix();
  pwr_tOid boid();
  pwr_tCid bcid();    

  const char* name() const;   // get body  name
  wb_name longName() const;
  
    
  wb_adrep *adrep( pwr_tStatus *sts); // Get first attribute
  wb_adrep *adrep( pwr_tStatus *sts, const char *aname);
  wb_adrep *super( pwr_tStatus *sts);

  pwr_tStatus sts() { return m_sts;}
    
            
};

#endif

