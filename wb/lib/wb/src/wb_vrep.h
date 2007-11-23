/* 
 * Proview   $Id: wb_vrep.h,v 1.28 2007-11-23 14:25:09 claes Exp $
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

#ifndef wb_vrep_h
#define wb_vrep_h

#include "pwr.h"
#include "co_cdh.h"
#include "wb_srep.h"
#include "wb_orep.h"
#include "wb_oset.h"
#include "wb_name.h"
#include "wb_cdef.h"
#include "wb_destination.h"
#include "wb_export.h"
#include "wb_treeexport.h"
#include "wb_treeimport.h"
#include <map>
#include <vector>

class wb_erep;
class wb_merep;
class wb_srep;
class wb_cdef;
class wb_destination;
class wb_dbs;

class wb_vrep : public wb_export, public wb_treeexport, public wb_treeimport
{
protected:
  typedef vector<wb_srep*>::iterator srep_iterator;

  char m_name[80];
  pwr_tVid m_vid;
  pwr_tCid m_cid;
  vector<wb_srep*> m_srep;

public:

  virtual ~wb_vrep() {};

  wb_vrep() : m_vid(pwr_cNVid), m_cid(pwr_cNCid) {}
  wb_vrep(pwr_tVid vid) : m_vid(vid), m_cid(pwr_cNCid) {}
  wb_vrep(pwr_tVid vid, pwr_tCid cid) : m_vid(vid), m_cid(cid) {}

  virtual void unref() = 0;
  virtual wb_vrep *ref() = 0;

  virtual ldh_eVolRep type() const = 0;
  virtual wb_erep *erep() = 0;

  virtual wb_vrep *next() = 0;

  virtual pwr_tOid oid(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tVid vid(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tOix oix(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual pwr_tCid cid(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tOid poid(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tOid foid(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tOid loid(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tOid boid(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tOid aoid(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual const char * objectName(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual wb_name longName(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tTime ohTime(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tTime rbTime(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_tTime dbTime(pwr_tStatus *sts, const wb_orep *o) = 0;
  virtual pwr_mClassDef flags(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual bool isOffspringOf(pwr_tStatus *sts, const wb_orep *child, const wb_orep *parent) = 0;

  virtual wb_orep *object(pwr_tStatus *sts) = 0;
  virtual wb_orep *object(pwr_tStatus *sts, pwr_tOid oid) = 0;
  virtual wb_orep *object(pwr_tStatus *sts, wb_name &name) = 0;
  virtual wb_orep *object(pwr_tStatus *sts, const wb_orep *parent, wb_name &name) = 0;
  virtual wb_orep *object(pwr_tStatus *sts, pwr_tCid cid) = 0;

  virtual wb_orep *createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name, pwr_tOix oix = 0) = 0;

  virtual wb_orep *copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name) = 0;
  virtual bool copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination &d) = 0;

  virtual bool moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination &d) = 0;

  virtual bool deleteObject(pwr_tStatus *sts, wb_orep *orep) = 0;
  virtual bool deleteFamily(pwr_tStatus *sts, wb_orep *orep) = 0;
  virtual bool deleteOset(pwr_tStatus *sts, wb_oset *oset) = 0;

  virtual bool renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name &name) = 0;


  virtual bool commit(pwr_tStatus *sts) = 0;
  virtual bool abort(pwr_tStatus *sts) = 0;

  virtual bool writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p) = 0;

  virtual void *readAttribute(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p) = 0;

  virtual void *readBody(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, void *p) = 0;

  virtual bool writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p) = 0;

  virtual wb_orep *ancestor(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual wb_orep *parent(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual wb_orep *after(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual wb_orep *before(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual wb_orep *first(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual wb_orep *child(pwr_tStatus *sts, const wb_orep *o, wb_name &name) = 0;

  virtual wb_orep *last(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual wb_orep *next(pwr_tStatus *sts, const wb_orep *o) = 0;

  //virtual wb_orep *nextClass(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual wb_orep *previous(pwr_tStatus *sts, const wb_orep *o) = 0;

  virtual wb_srep *newSession() = 0;

  virtual bool isLocal(const wb_orep *o) = 0;
  pwr_tVid vid() const { return m_vid;}
  pwr_tCid cid() const { return m_cid;}
  void vid(pwr_tVid vid) { m_vid = vid;}
  void cid(pwr_tCid cid) { m_cid = cid;}

  virtual void name( const char *n) { strcpy( m_name, n);}
  virtual const char *name() { return m_name;}
  virtual wb_merep *merep() const = 0;

  virtual bool createSnapshot(const char *fileName, const pwr_tTime *time) = 0;

  virtual void objectName(const wb_orep *o, char *str) = 0;
  virtual bool isCommonMeta() const { return false;}
  virtual bool isMeta() const { return false;}
  virtual bool accessSupported( ldh_eAccess access) = 0;
  virtual bool duplicateDb() const { return false;}
  virtual void setDuplicateDb( bool duplicate) {}

  void addSrep( wb_srep *srep);
  void removeSrep( wb_srep *srep);
  wb_srep *srep( pwr_tStatus *sts);
  wb_srep *nextSrep( pwr_tStatus *sts, wb_srep* srep);

  virtual const char *fileName() = 0;
  virtual bool time( pwr_tTime *t) { return false;}
};

#endif
