#ifndef wb_vrepmem_h
#define wb_vrepmem_h

#include <string>

#include "wb_vrep.h"
#include "wb_erep.h"
#include "wb_attrname.h"
#include "wb_treeimport.h"

class wb_vrepmem;

class mem_object
{
  friend class wb_vrepmem;

 public:
  mem_object() : rbody_size(0), dbody_size(0), rbody(0), dbody(0),
    m_cid(0), m_tid(0), fth(0), bws(0), fws(0), fch(0),
    is_built(0) 
    { 
      strcpy( m_name, "");
      strcpy( cname, ""); 
      m_flags.m = 0;
      m_oid.oix = 0;
      m_oid.vid = 0;
    }
  ~mem_object() {
    if ( rbody_size) free( rbody);
    if ( dbody_size) free( dbody);
  }    
  char *name() { return m_name; }
  mem_object *get_lch() { 
    mem_object *c = fch;
    if ( c)
      while ( c->fws) 
	c = c->fws;
    return c;
  }

  bool exportHead(wb_import &i) { return false;}
  bool exportDbody(wb_import &i) { return false;}
  bool exportRbody(wb_import &i) { return false;}
  bool exportTree( wb_treeimport &i, bool isRoot) {
    pwr_tOid fthoid = (fth && !isRoot) ? fth->m_oid : pwr_cNOid;
    pwr_tOid bwsoid = (bws && !isRoot) ? bws->m_oid : pwr_cNOid;

    i.importTreeObject( 0, m_oid, m_cid, fthoid, bwsoid, name(), 
			rbody_size, dbody_size, rbody, dbody);
  
    if ( fch)
      fch->exportTree( i, false);

    if ( !isRoot && fws)
      fws->exportTree( i, false);

    return true;
  }
  bool exportPaste( wb_treeimport &i, pwr_tOid destination, bool isRoot, 
			ldh_eDest destcode, bool keepoid, pwr_tOid *rootlist) {
    pwr_tOid fthoid = (fth && !isRoot) ? fth->m_oid : pwr_cNOid;
    pwr_tOid bwsoid = (bws && !isRoot) ? bws->m_oid : pwr_cNOid;
    pwr_tOid oid;

    i.importPasteObject( destination, destcode, keepoid, m_oid, m_cid, fthoid, bwsoid, 
			 name(), rbody_size, dbody_size, rbody, dbody, &oid);
    if ( rootlist)
      *rootlist++ = oid;
  
    if ( fch)
      fch->exportPaste( i, destination, false, destcode, keepoid, 0);

    if ( fws)
      fws->exportPaste( i, destination, false, destcode, keepoid, rootlist);

    return true;
  }

  mem_object *next( pwr_tCid cid, pwr_tOix *oix) {
    // search is turned on when oix = 0
    if ( *oix == m_oid.oix)
      *oix = 0;
    mem_object *n;
    if ( fch) {
      if ( !*oix && fch->m_cid == cid)
	return fch;
      else {
	n = fch->next( cid, oix);
	if ( n)
	  return n;
      }
    }
    if ( fws) {
      if ( !*oix && fws->m_cid == cid)
	return fws;
      else {
	n = fws->next( cid, oix);
	if ( n)
	  return n;
      }
    }
    return 0;
  }

  mem_object *find( wb_name *oname, int level);
  
  size_t rbody_size;
  size_t dbody_size;
  void *rbody;
  void *dbody;
  pwr_tCid m_cid;
  pwr_tTid m_tid;
  pwr_tOid m_oid;
  char cname[32];
  char m_name[32];
  pwr_mClassDef m_flags;
  mem_object *fth;
  mem_object *bws;
  mem_object *fws;
  mem_object *fch;
  int is_built;
};

class wb_orepmem;

class wb_vrepmem : public wb_vrep
{
  wb_erep *m_erep;
  wb_merep *m_merep;
  unsigned int m_nRef;
  mem_object *root_object;
  int m_nextOix;
  pwr_tVid m_source_vid;

  map<pwr_tOix, mem_object *> m_oix_list;

  typedef map<pwr_tOix, mem_object *>::iterator iterator_oix_list;

public:
  wb_vrepmem( wb_erep *erep) : 
    m_erep(erep), m_merep(erep->merep()), m_nRef(0), root_object(0), m_nextOix(0) {}

  wb_vrepmem( wb_erep *erep, pwr_tVid vid);
  ~wb_vrepmem();

  virtual ldh_eVolRep type() const { return ldh_eVolRep_Mem;}
  pwr_tVid vid() const { return m_vid;}
  pwr_tCid cid() const { return m_cid;}

  wb_vrep *next();

  virtual bool createSnapshot(const char *fileName);

  char volume_class[32];
  char volume_name[32];

  virtual void unref();
  virtual wb_vrep *ref();

  wb_erep *erep() {return m_erep;}
  wb_merep *merep() const { return m_merep;}

  int nextOix();
  mem_object *findObject( pwr_tOix oix);
  mem_object *find( const char *name);
  int nameToOid( const char *name, pwr_tOid *oid);
  bool registerObject( pwr_tOix oix, mem_object *node);
  bool unregisterObject( pwr_tOix oix);
  void registerVolume( const char *name, pwr_tCid cid, pwr_tVid vid, mem_object *node);
  void info();


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
    
  virtual pwr_tTime ohTime(pwr_tStatus *sts, const wb_orep *o) { pwr_tTime t = {0, 0}; return t;}
    
    
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


  bool commit(pwr_tStatus *sts) {return true;}
  bool abort(pwr_tStatus *sts) {return true;}

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

  virtual bool exportVolume(wb_import &i);
  virtual bool exportHead(wb_import &i);
  virtual bool exportRbody(wb_import &i);
  virtual bool exportDbody(wb_import &i);
  virtual bool exportMeta(wb_import &i);
  virtual bool exportTree(wb_treeimport &i, pwr_tOid oid);
  bool exportPaste(wb_treeimport &i, pwr_tOid destination, ldh_eDest destcode, bool keepoid,
		   pwr_tOid **rootlist);
  virtual bool importTreeObject(wb_merep *merep, pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, const char *name,
                          size_t rbSize, size_t dbSize, void *rbody, void *dbody);
  virtual bool importTree( bool keepref);
  virtual bool importPasteObject(pwr_tOid destination, ldh_eDest destcode,
				 bool keepoid, pwr_tOid oid, 
				 pwr_tCid cid, pwr_tOid poid,
				 pwr_tOid boid, const char *name,
				 size_t rbSize, size_t dbSize, void *rbody, void *dbody,
				 pwr_tOid *roid);
  virtual bool importPaste();
  bool updateObject( wb_orep *o, bool keepref);
  bool updateSubClass( wb_adrep *subattr, char *body, bool keepref);
  virtual bool accessSupported( ldh_eAccess access) { return true;}

 private:
  bool nameCheck( mem_object *memo);
  bool nameCheck( mem_object *parent, char *name, ldh_eDest code);
  void deleteChildren( mem_object *memo);

};

#endif









