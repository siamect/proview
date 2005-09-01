/** 
 * Proview   $Id: wb_orepdbs.h,v 1.14 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_orepdbs_h
#define wb_orepdbs_h

#include "wb_orep.h"
#include "wb_vrepdbs.h"
#include "pwr.h"

class wb_orepdbs : public wb_orep
{
  dbs_sObject *m_o;
  // wb_vrepdbs *m_vrep;
  unsigned int m_refCount;
    
  friend class wb_cdrep;
  friend class wb_bdrep;
  friend class wb_adrep;
  friend class wb_tdrep;
  friend class wb_vrepdbs;
    
public:

  wb_orepdbs(dbs_sObject *o);
  //~wb_orepdbs();

  void* operator new(size_t size, wb_vrepdbs *v);
  //void operator delete(void *p, wb_vrepdbs *v);
    
  virtual pwr_tOid oid() const;
  virtual pwr_tVid vid() const;
  virtual pwr_tOix oix() const;

  virtual pwr_tCid cid() const;
  virtual pwr_tOid poid() const;
  virtual pwr_tOid foid() const;
  virtual pwr_tOid loid() const;
  virtual pwr_tOid boid() const;
  virtual pwr_tOid aoid() const;
    
  virtual const char * name() const;
  virtual wb_name longName();
  virtual pwr_tTime ohTime() const;
  virtual pwr_mClassDef flags() const;
    
  virtual bool isOffspringOf(const wb_orep *o) const;
    
  // Navigational operations

  virtual wb_orep *ancestor(pwr_tStatus *sts);  //< get object at top of hierarchy
  virtual wb_orep *parent(pwr_tStatus *sts);
  virtual wb_orep *after(pwr_tStatus *sts);     //< get next sibling
  virtual wb_orep *before(pwr_tStatus *sts);    //< get previous sibling
  virtual wb_orep *first(pwr_tStatus *sts);     //< get first child
  virtual wb_orep *child(pwr_tStatus *sts, wb_name &name);    //< get named child
  virtual wb_orep *last(pwr_tStatus *sts);      //< get last child
  virtual wb_orep *next(pwr_tStatus *sts);      //< get next in list of objects of same class in one volume
  virtual wb_orep *previous(pwr_tStatus *sts);  //< get previous in list of objects of same class in one volume

  virtual wb_adrep *attribute(pwr_tStatus*, const char *name);
  virtual wb_adrep *attribute(pwr_tStatus*);
    
  wb_erep *erep() const { return m_vrep->erep();}
  wb_vrep *vrep() const { return m_vrep;}

  dbs_sObject *o() const { return m_o;}
    
};

#endif
