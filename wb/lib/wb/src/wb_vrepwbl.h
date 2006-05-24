/* 
 * Proview   $Id: wb_vrepwbl.h,v 1.37 2006-05-24 15:00:41 claes Exp $
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

#ifndef wb_vrepwbl_h
#define wb_vrepwbl_h

#include <string>

#include "wb_vrep.h"
#include "antlr/AST.hpp"
#include "wb_wbllexer.hpp"
#include "wb_wblparser.hpp"
#include "wb_wblnode.h"
#include "wb_wblfile.h"
#include "wb_erep.h"
#include "wb_attrname.h"

#define WBL_FILEMAX 500

class wb_orepwbl;

class wb_vrepwbl : public wb_vrep
{
  wb_erep *m_erep;
  wb_merep *m_merep;
  unsigned int m_nRef;
  bool m_ignore_oix;

  map<string, ref_wblnode> m_type_list;
  map<pwr_tTid, ref_wblnode> m_tid_list;
  map<string, ref_wblnode> m_class_list;
  map<pwr_tCid, ref_wblnode> m_cid_list;
  map<pwr_tOix, ref_wblnode> m_oix_list;

  typedef map<string, ref_wblnode>::iterator iterator_type_list;
  typedef map<pwr_tTid, ref_wblnode>::iterator iterator_tid_list;
  typedef map<string, ref_wblnode>::iterator iterator_class_list;
  typedef map<pwr_tCid, ref_wblnode>::iterator iterator_cid_list;
  typedef map<pwr_tOix, ref_wblnode>::iterator iterator_oix_list;

public:
  wb_vrepwbl( wb_erep *erep) : 
    m_erep(erep), m_merep(erep->merep()), m_nRef(0), m_ignore_oix(false), root_object(0), error_cnt(0), file_cnt(0), next_oix(0), volume_node(0) {}

  wb_vrepwbl( wb_erep *erep, pwr_tVid vid) :
    wb_vrep(vid), m_erep(erep), m_merep(erep->merep()), m_nRef(0), root_object(0),error_cnt(0), file_cnt(0), next_oix(0), volume_node(0) {}
  ~wb_vrepwbl();

  virtual ldh_eVolRep type() const { return ldh_eVolRep_Wbl;}
  pwr_tVid vid() const { return m_vid;}
  pwr_tCid cid() const { return m_cid;}

  wb_vrep *next();

  virtual bool createSnapshot(const char *fileName, const pwr_tTime *time);

  int load( const char *fname);
  int load_files( const char *file_spec);
  void info();

  wb_wblfile *file[WBL_FILEMAX];
  wb_wblnode *root_object;

  void error( const char *msg, const char *file, int line_number);
  ref_wblnode find( const char *name);
  void registerClass( const char *name, pwr_tCid cid, ref_wblnode node);
  void registerType( const char *name, pwr_tTid tid, ref_wblnode node);
  void registerVolume( const char *name, pwr_tCid cid, pwr_tVid vid, ref_wblnode node);
  bool registerObject( pwr_tOix oix, ref_wblnode node);
  int nextOix() { return ++next_oix; }
  int classNameToCid( const char *name, pwr_tCid *cid);
  int getTemplateBody( pwr_tCid cid, pwr_eBix bix, size_t *size, void **body);
  int getTypeInfo( pwr_tTid tid, pwr_eType *type, size_t *size,
                   int *elements);
  int getTypeInfo( const char *type, pwr_tTid *tid, pwr_eType *type, size_t *size,
                   int *elements);
  int getClassInfo( pwr_tCid cid, size_t *rsize, size_t *dsize);
  int getAttrInfo( const char *attr, pwr_eBix bix, pwr_tCid cid, size_t *size,
                   size_t *offset, pwr_tTid *tid, int *elements, pwr_eType *type, int *flags);
  int getAttrInfoRec( wb_attrname *attr, pwr_eBix bix, pwr_tCid cid, size_t *size,
                      size_t *offset, pwr_tTid *tid, int *elements, 
                      pwr_eType *type, int *flags, int level);
  void getClassFlags( pwr_tStatus *sts, pwr_tCid cid, pwr_mClassDef *flags);
  ref_wblnode findObject( pwr_tOix oix);
  ref_wblnode findClass( const char *name);
  ref_wblnode findType( const char *name);
  ref_wblnode findClass( pwr_tCid cid);
  ref_wblnode findType( pwr_tTid tid);
  int nameToOid( const char *name, pwr_tOid *oid);
  int nameToAttrRef( const char *name, pwr_sAttrRef *attrref);
  void ignoreOix() { m_ignore_oix = true;}

  int error_cnt;
  int file_cnt;
  int next_oix;
  char volume_class[32];
  char volume_name[32];
  wb_wblnode *volume_node;
    

  virtual void unref();
  virtual wb_vrep *ref();

  wb_erep *erep() {return m_erep;}
  wb_merep *merep() const { return m_merep;}

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
  wb_orep *object(pwr_tStatus *sts, pwr_tCid cid) { *sts = LDH__NYI; return 0;}
  wb_orep *object(pwr_tStatus *sts, wb_name &name);
  wb_orep *object(pwr_tStatus *sts, const wb_orep *parent, wb_name &name) {return 0;}

  wb_orep *createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name) {return 0;}

  wb_orep *copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name) {return 0;}
  bool copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination &d) {return false;}

  bool moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination &d) {return false;}

  bool deleteObject(pwr_tStatus *sts, wb_orep *orep) {return false;}
  bool deleteFamily(pwr_tStatus *sts, wb_orep *orep) {return false;}
  bool deleteOset(pwr_tStatus *sts, wb_oset *oset) {return false;}

  bool renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name &name);


  bool commit(pwr_tStatus *sts) {return false;}
  bool abort(pwr_tStatus *sts) {return false;}

  virtual bool writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p) { return false;}

  virtual void *readAttribute(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p);

  virtual void *readBody(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, void *p);

  virtual bool writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p) { return false;}


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

  virtual bool exportVolume(wb_import &i);
  virtual bool exportHead(wb_import &i);
  virtual bool exportRbody(wb_import &i);
  virtual bool exportDbody(wb_import &i);
  virtual bool exportDocBlock(wb_import &i);
  virtual bool exportMeta(wb_import &i);
  virtual bool exportTree(wb_treeimport &i, pwr_tOid oid);
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
  virtual bool accessSupported( ldh_eAccess access) { return access == ldh_eAccess_ReadOnly;}
  virtual const char *fileName() { return "";}
};

#endif






