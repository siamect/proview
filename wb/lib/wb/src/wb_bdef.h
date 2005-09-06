/* 
 * Proview   $Id: wb_bdef.h,v 1.9 2005-09-06 10:43:30 claes Exp $
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

#ifndef wb_bdef_h
#define wb_bdef_h

#include "pwr.h"
#include "wb_ldh.h"
#include "wb_status.h"

class wb_orep;
class wb_bdrep;
class wb_adef;
class wb_name;

class wb_bdef : public wb_status
{
  int m_nRef;
  wb_bdrep *m_bdrep;

public:
  wb_bdef();
  wb_bdef(wb_bdrep* bdrep);
  wb_bdef(const wb_bdef&);
  wb_bdef& operator=(const wb_bdef&);

  wb_bdef(const wb_adef *a);
  wb_bdef(const wb_orep *o, pwr_eBix bix);

  ~wb_bdef();

  operator bool() const { return (m_bdrep != 0);}
  operator wb_bdrep*() const;
  bool operator==(wb_bdef&);
    
  pwr_tOid boid();
  pwr_eBix bix();
  size_t size();
  int nAttribute();

  const char* name() const; // get body name
  wb_name longName() const;

  pwr_sAttrRef aref();
    
  wb_adef adef(); // Get first attribute
  wb_adef adef(pwr_tAix aix);
  wb_adef adef(const char *aname);
  wb_adef adef(wb_name aname);
            
};

#endif
