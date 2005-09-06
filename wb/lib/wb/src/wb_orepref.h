/* 
 * Proview   $Id: wb_orepref.h,v 1.4 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_orepref_h
#define wb_orepref_h

#include "wb_orep.h"
#include "wb_vrepref.h"
#include "pwr.h"

class wb_orepref : public wb_orep
{
  wb_vrepref *m_vrep;
  unsigned int m_refCount;
  pwr_tCid m_cid;

  friend class wb_vrepref;
public:

  wb_orepref( wb_vrepref *vrep, int oix) : m_vrep(vrep), m_cid(oix) {}
  ~wb_orepref() {}
    
  virtual pwr_tOid oid() const { pwr_tOid oid = {m_cid, m_vrep->vid()}; return oid;}
  virtual pwr_tVid vid() const { return m_vrep->vid();}
  virtual pwr_tOix oix() const { return m_cid;}

  virtual pwr_tCid cid() const { return m_cid;}
  virtual pwr_tOid poid() const { return pwr_cNObjid;}
  virtual pwr_tOid foid() const { return pwr_cNObjid;}
  virtual pwr_tOid loid() const { return pwr_cNObjid;}
  virtual pwr_tOid boid() const { return pwr_cNObjid;}
  virtual pwr_tOid aoid() const { return pwr_cNObjid;}
    
  virtual const char * name() const { return m_vrep->name();}
  virtual wb_name longName() {
    char str[200];

    m_vrep->objectName( this, str);
    return wb_name( str);
  }
  virtual pwr_tTime ohTime() const { pwr_tTime t = {0,0}; return t;}
  virtual pwr_tTime rbTime() const { pwr_tTime t = {0,0}; return t;}
  virtual pwr_tTime dbTime() const { pwr_tTime t = {0,0}; return t;}
  virtual pwr_mClassDef flags() const { pwr_mClassDef m; m.m = 0; return m;}
    
  virtual bool isOffspringOf(const wb_orep *o) const {return false;}
  wb_orep *ancestor(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *parent(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *after(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *before(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *first(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *child(pwr_tStatus *sts, wb_name &name) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *last(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep *next(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_orep  *previous(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  virtual wb_adrep *attribute(pwr_tStatus *sts, const char *name) {*sts = LDH__NOSUCHOBJ; return 0;}
  virtual wb_adrep *attribute(pwr_tStatus *sts) {*sts = LDH__NOSUCHOBJ; return 0;}
  wb_erep *erep() const { return m_vrep->erep();}
  wb_vrep *vrep() const { return m_vrep;}
  virtual ldh_eVolRep vtype() const { return ldh_eVolRep_Ref;}

  virtual bool docBlock( char **block, int *size) const {return false;}
  virtual bool docBlock( char *block) {return false;}
};

#endif

