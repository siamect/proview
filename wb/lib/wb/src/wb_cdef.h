/** 
 * Proview   $Id: wb_cdef.h,v 1.15 2005-09-01 14:57:57 claes Exp $
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

#ifndef wb_cdef_h
#define wb_cdef_h

#include "pwr.h"
#include "pwr_class.h"
#include "wb_name.h"

class wb_bdef;
class wb_adef;
class wb_cdrep;
class wb_mvrep;
class wb_object;
class wb_orep;

class wb_cdef : public wb_status
{


public:
  wb_cdrep *m_cdrep;

  wb_cdef();
  wb_cdef( wb_cdrep *cdrep);
  wb_cdef( wb_adef&);  // x = other_object
  wb_cdef(const wb_orep&);  // x = other orep
  wb_cdef(wb_mvrep *, pwr_tCid);
  wb_cdef(const wb_cdef&);
  ~wb_cdef();

  wb_cdef& operator=(const wb_cdef&);
  operator bool() const { return (m_cdrep != 0);}
  operator wb_cdrep*() const {return m_cdrep;}
  
  bool operator==(wb_cdef&);
    
  size_t size( pwr_eBix bix);
  pwr_tCid cid();
  pwr_tOid oid() { pwr_tOid oid = pwr_cNOid; return oid;} // Fix

  const char *name() const;
  wb_name longName();
  pwr_mClassDef flags();

  wb_bdef bdef(pwr_eBix bix);
  wb_bdef bdef(const char *bname);
  wb_bdef bdef(wb_name bname);

  void templateBody( pwr_tStatus *sts, pwr_eBix bix, void *p, pwr_tOid oid);
  wb_object classBody( const char *bname);
  wb_cdef super();

private:
  void check() const;
    
};

#endif


