/* 
 * Proview   $Id: wb_adef.h,v 1.12 2005-09-06 10:43:30 claes Exp $
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

#ifndef wb_adef_h
#define wb_adef_h

#include "pwr.h"
#include "wb_status.h"
#include "wb_ldh.h"

class wb_orep;
class wb_adrep;
class wb_bdef;
class wb_cdef;
class wb_name;


class wb_adef : public wb_status
{
  wb_adrep *m_adrep;
    
public:
  wb_adef();
  wb_adef(const wb_adef&);  // x = other_object
  wb_adef(wb_adrep*);
  wb_adef(const wb_orep&);  // x = other orep
  wb_adef& operator=(const wb_adef&);

  ~wb_adef();

  operator bool() const { return (m_adrep != 0);}
  operator wb_adrep*() const { return m_adrep;}
  bool operator==(wb_adef&);
    
  wb_bdef bdef();
  wb_cdef cdef();

  pwr_sAttrRef aref();
  size_t size();   // get objects runtime body size
  int offset();
  pwr_eType type();
  int nElement();
  int index();
  int flags();
  pwr_tOid aoid();
  pwr_tAix aix();
  pwr_tCid cid();
  pwr_eBix bix();
  pwr_tOid boid();
    
  wb_adef next();
  wb_adef prev();

  const char *name();
  const char *subName();
  wb_name longName();
  void *body( void *p = 0);
  bool isClass();
  bool isSuperClass();
  pwr_tCid subClass();
    
private:
  void check();
};

#endif


