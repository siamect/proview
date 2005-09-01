/** 
 * Proview   $Id: wb_orep.h,v 1.12 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_orep_h
#define wb_orep_h

#include "pwr.h"
#include "wb_name.h"
#include "wb_orep.h"
#include "wb_vrep.h"
#include "wb_ldh.h"
//#include "wb_position.h"
#include "wb_adrep.h"

class wb_vrep;
class wb_erep;

class wb_orep
{

protected:
  wb_vrep *m_vrep;
public:
  int m_nRef;

public:

  //virtual wb_orep(int oid) = 0;
  wb_orep();
  virtual ~wb_orep() {}
    
  void unref();
  wb_orep *ref();
    
  virtual pwr_tOid oid() const = 0;
  virtual pwr_tVid vid() const = 0;
  virtual pwr_tOix oix() const = 0;

  virtual pwr_tCid cid() const = 0;
  virtual pwr_tOid poid() const = 0;
  virtual pwr_tOid foid() const = 0;
  virtual pwr_tOid loid() const = 0;
  virtual pwr_tOid boid() const = 0;
  virtual pwr_tOid aoid() const = 0;
    
  virtual const char * name() const = 0;
  virtual wb_name longName() = 0;
  virtual pwr_tTime ohTime() const = 0;
  virtual pwr_mClassDef flags() const = 0;
    
  virtual bool isOffspringOf(const wb_orep *o) const = 0;
    
  // Navigational operations

  virtual wb_orep *ancestor(pwr_tStatus *sts) = 0;  //< get object at top of hierarchy
  virtual wb_orep *parent(pwr_tStatus *sts) = 0;
  virtual wb_orep *after(pwr_tStatus *sts) = 0;     //< get next sibling
  virtual wb_orep *before(pwr_tStatus *sts) = 0;    //< get previous sibling
  virtual wb_orep *first(pwr_tStatus *sts) = 0;     //< get first child
  virtual wb_orep *child(pwr_tStatus *sts, wb_name &name) = 0;    //< get named child
  virtual wb_orep *last(pwr_tStatus *sts) = 0;      //< get last child
  virtual wb_orep *next(pwr_tStatus *sts) = 0;      //< get next in list of objects of same class in one volume
  virtual wb_orep *previous(pwr_tStatus *sts) = 0;  //< get previous in list of objects of same class in one volume

  virtual wb_adrep *attribute(pwr_tStatus*, const char *name) = 0;
  virtual wb_adrep *attribute(pwr_tStatus*) = 0;
  virtual bool docBlock( char **block, int *size) const { return false;}
  virtual bool docBlock( char *block) { return false;}

  virtual wb_erep *erep() const = 0;
  virtual wb_vrep *vrep() const = 0;
};

#endif
