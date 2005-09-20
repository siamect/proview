/* 
 * Proview   $Id: wb_vrepext.h,v 1.5 2005-09-20 13:14:28 claes Exp $
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

#ifndef wb_vrepext_h
#define wb_vrepext_h

#include <string>

#include "wb_vrep.h"
#include "wb_erep.h"
#include "wb_attrname.h"
#include "wb_treeimport.h"
#include "co_dbs.h"
#include "wb_import.h"
#include "wb_vext.h"
#include "wb_provider.h"
#include "wb_cdef.h"

class wb_vrepext;

class ext_object
{
  friend class wb_vrepext;

 public:
  ext_object() : rbody_size(0), dbody_size(0), rbody(0), dbody(0),
    m_cid(0), m_tid(0), fthoid(pwr_cNOid), bwsoid(pwr_cNOid), fwsoid(pwr_cNOid), 
    fchoid(pwr_cNOid), lchoid(pwr_cNOid)
    { 
      strcpy( m_name, "");
      strcpy( m_longname, "");
      strcpy( cname, ""); 
      m_flags.m = 0;
      m_oid.oix = 0;
      m_oid.vid = 0;
    }
  ext_object( vext_sAMsgObject *msg, pwr_tVid vid, wb_cdef &cdef) : 
    rbody_size(0), dbody_size(0), rbody(0), dbody(0)
    {
      strcpy( m_name, msg->name);
      strcpy( m_longname, msg->longname);
      m_oid.oix = msg->oix;
      m_oid.vid = vid;
      fthoid.oix = msg->fthoix;
      fthoid.vid = vid;
      bwsoid.oix = msg->bwsoix;
      bwsoid.vid = vid;
      fwsoid.oix = msg->fwsoix;
      fwsoid.vid = vid;
      fchoid.oix = msg->fchoix;
      fchoid.vid = vid;
      lchoid.oix = msg->lchoix;
      lchoid.vid = vid;      
      m_cid = msg->cid;
      m_flags = cdef.flags();
    }
  ~ext_object() {
    if ( rbody_size) free( rbody);
    if ( dbody_size) free( dbody);
  }    
  ext_object(const ext_object& x) {
    memcpy( this, &x, sizeof(x));
    if ( rbody_size) {
      rbody = calloc( 1, rbody_size);
      memcpy( rbody, x.rbody, rbody_size);
    }    
    if ( dbody_size) {
      dbody = calloc( 1, dbody_size);
      memcpy( dbody, x.dbody, dbody_size);
    }    
  }
  ext_object& operator=(const ext_object& x) {
    memcpy( this, &x, sizeof(x));
    if ( rbody_size) {
      rbody = calloc( 1, rbody_size);
      memcpy( rbody, x.rbody, rbody_size);
    }    
    if ( dbody_size) {
      dbody = calloc( 1, dbody_size);
      memcpy( dbody, x.dbody, dbody_size);
    }    
    return *this;
  }

  const char *name() const { return m_name; }
  const char *longname() const { return m_longname; }
  bool exportHead(wb_import &i) { return false;}
  bool exportDbody(wb_import &i) { return false;}
  bool exportRbody(wb_import &i) { return false;}
  bool exportDocBlock(wb_import &i) { return false;}
  bool exportTree( wb_treeimport &i, bool isRoot) { return false;}
  bool exportPaste( wb_treeimport &i, pwr_tOid destination, bool isRoot, 
			ldh_eDest destcode, bool keepoid, pwr_tOid *rootlist) { return false;}
  
  size_t rbody_size;
  size_t dbody_size;
  void *rbody;
  void *dbody;
  pwr_tCid m_cid;
  pwr_tTid m_tid;
  pwr_tOid m_oid;
  char cname[32];
  char m_name[32];
  char m_longname[120];
  pwr_mClassDef m_flags;
  pwr_tOid fthoid;
  pwr_tOid bwsoid;
  pwr_tOid fwsoid;
  pwr_tOid fchoid;
  pwr_tOid lchoid;
  pwr_tTime time;
};

class wb_orepext;

class wb_vrepext : public wb_vrep
{
  wb_erep *m_erep;
  wb_merep *m_merep;
  unsigned int m_nRef;
  ext_object volume_object;
  char m_providerstr[200];
  int m_connected;
  int m_msgsndid;
  int m_msgrcvid;
  ext_object m_cashe;
  wb_procom *m_procom;
  wb_provider *m_provider;
  procom_eType m_ptype;

public:
  wb_vrepext( wb_erep *erep) : 
    m_erep(erep), m_merep(erep->merep()), m_nRef(0) {}

  wb_vrepext( wb_erep *erep, pwr_tVid vid);
  wb_vrepext( wb_erep *erep, pwr_tVid vid, char *name, char *provider);
  ~wb_vrepext();

  virtual ldh_eVolRep type() const { return ldh_eVolRep_Ext;}
  pwr_tVid vid() const { return m_vid;}
  pwr_tCid cid() const { return m_cid;}

  wb_vrep *next();

  virtual bool createSnapshot(const char *fileName) { return false;}

  char volume_class[32];
  char volume_name[32];

  virtual void unref();
  virtual wb_vrep *ref();

  wb_erep *erep() {return m_erep;}
  wb_merep *merep() const { return m_merep;}

  void info();
  bool createVolumeObject( char *name);

  virtual pwr_tOid oid(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNOid;}
    
  virtual pwr_tVid vid(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNVid;}
    
  virtual pwr_tOix oix(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNOix;}
    

  virtual pwr_tCid cid(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNCid;}
    
  virtual pwr_tOid poid(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNOid;}
  virtual pwr_tOid foid(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNOid;}
  virtual pwr_tOid loid(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNOid;}
  virtual pwr_tOid boid(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNOid;}
  virtual pwr_tOid aoid(pwr_tStatus *sts, const wb_orep *o) { return pwr_cNOid;}
    
  virtual const char * objectName(pwr_tStatus *sts, const wb_orep *o) { return "";}
    
  virtual wb_name longName(pwr_tStatus *sts, const wb_orep *o) { return wb_name();}
    
  virtual pwr_tTime ohTime(pwr_tStatus *sts, const wb_orep *o) { return o->ohTime();}
  virtual pwr_tTime rbTime(pwr_tStatus *sts, const wb_orep *o) { return o->rbTime();}
  virtual pwr_tTime dbTime(pwr_tStatus *sts, const wb_orep *o) { return o->dbTime();}
  virtual pwr_mClassDef flags(pwr_tStatus *sts, const wb_orep *o) { pwr_mClassDef f; f.m = 0; return f;}    
    
  virtual bool isOffspringOf(pwr_tStatus *sts, const wb_orep *child, const wb_orep *parent) { return false;}
    

  wb_orep *object(pwr_tStatus *sts);
  wb_orep *object(pwr_tStatus *sts, pwr_tOid oid);
  wb_orep *object(pwr_tStatus *sts, pwr_tCid cid);
  wb_orep *object(pwr_tStatus *sts, wb_name &name);
  wb_orep *object(pwr_tStatus *sts, const wb_orep *parent, wb_name &name) {return 0;}

  wb_orep *createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name);

  wb_orep *copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name);
  bool copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination &d) {return false;}

  bool moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination &d);

  bool deleteObject(pwr_tStatus *sts, wb_orep *orep);
  bool deleteFamily(pwr_tStatus *sts, wb_orep *orep);
  bool deleteOset(pwr_tStatus *sts, wb_oset *oset) {return false;}

  bool renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name &name);


  bool commit(pwr_tStatus *sts);
  bool abort(pwr_tStatus *sts);

  virtual bool writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p);

  virtual void *readAttribute(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p);

  virtual void *readBody(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, void *p);

  virtual bool writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p);


  wb_orep *ancestor(pwr_tStatus *sts, const wb_orep *o);

  wb_orep *parent(pwr_tStatus *sts, const wb_orep *o);

  wb_orep *after(pwr_tStatus *sts, const wb_orep *o);

  wb_orep *before(pwr_tStatus *sts, const wb_orep *o);

  wb_orep *first(pwr_tStatus *sts, const wb_orep *o);

  wb_orep *child(pwr_tStatus *sts, const wb_orep *o, wb_name &name);

  wb_orep *last(pwr_tStatus *sts, const wb_orep *o);

  wb_orep *next(pwr_tStatus *sts, const wb_orep *o);

  wb_orep *previous(pwr_tStatus *sts, const wb_orep *o);

  wb_srep *newSession() {return 0;}

  bool isLocal(const wb_orep *o) {return o->oid().vid == vid();}

  void objectName(const wb_orep *o, char *str);

  virtual bool exportVolume(wb_import &i) { return false;}
  virtual bool exportHead(wb_import &i) { return false;}
  virtual bool exportRbody(wb_import &i) { return false;}
  virtual bool exportDbody(wb_import &i) { return false;}
  virtual bool exportDocBlock(wb_import &i) { return false;}
  virtual bool exportMeta(wb_import &i) { return false;}
  virtual bool exportTree(wb_treeimport &i, pwr_tOid oid) { return false;}
  bool exportPaste(wb_treeimport &i, pwr_tOid destination, ldh_eDest destcode, bool keepoid,
		   pwr_tOid **rootlist) { return false;}
  virtual bool importTree(bool keepref) { return false;}
  virtual bool importTreeObject(wb_merep *merep, pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
				pwr_tOid boid, const char *name, pwr_mClassDef flags,
				size_t rbSize, size_t dbSize, void *rbody, void *dbody)
    { return false;}
  virtual bool importPaste() { return false;}
  virtual bool importPasteObject(pwr_tOid destination, ldh_eDest destcode, 
				 bool keepoid, pwr_tOid oid, 
				 pwr_tCid cid, pwr_tOid poid,
				 pwr_tOid boid, const char *name, pwr_mClassDef flags,
				 size_t rbSize, size_t dbSize, void *rbody, void *dbody,
				 pwr_tOid *roid)
    { return false;}
  virtual void importIgnoreErrors() {}
  virtual bool accessSupported( ldh_eAccess access) { return true;}
  virtual const char *fileName() { return "";}

  virtual bool importVolume(wb_export &e) { return false;}
  virtual bool importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                          const char *name, const char *normname, pwr_mClassDef flags,
                          pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                          size_t rbSize, size_t dbSize) { return false;}
  virtual bool importRbody(pwr_tOid oid, size_t size, void *body) { return false;}
  virtual bool importDbody(pwr_tOid oid, size_t size, void *body) { return false;}
  virtual bool importDocBlock(pwr_tOid oid, size_t size, char *block) { return false;}
  virtual bool importMeta(dbs_sMenv *mep) { return true;}

 private:
  void put( vext_sQMsg *msg, int size, pwr_tStatus *sts);
  void receive( vext_sAMsg *msg, int size, pwr_tStatus *sts);
};

#endif









