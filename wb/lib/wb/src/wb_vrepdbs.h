#ifndef wb_vrepdbs_h
#define wb_vrepdbs_h

#include "co_dbs.h"
#include "wb_vrep.h"
#include "wb_cdef.h"

class wb_merep;
class wb_orepdbs;

class wb_vrepdbs : public wb_vrep
{
public:
  wb_erep *m_erep;
  wb_merep *m_merep;
  unsigned int m_nRef;

  char m_fileName[200];
  bool m_isDbsenvLoaded;
    
  dbs_sEnv m_dbsenv;
    
  wb_vrepdbs(wb_erep *erep, const char *fileName);

  dbs_sEnv *dbsenv();
  bool load();
  void objectName(const wb_orep *o, char *str);
    
    
  virtual void unref();
  virtual wb_vrep *ref();

  virtual wb_erep *erep() ;

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
    

  virtual wb_orep *object(pwr_tStatus *sts, pwr_tOid oid);
  virtual wb_orep *object(pwr_tStatus *sts, wb_name &name);
  virtual wb_orep *object(pwr_tStatus *sts, const wb_orep *parent, wb_name &name);

  virtual wb_orep *createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name);

  virtual wb_orep *copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name);
  virtual bool copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination &d);

  virtual bool moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination &d);

  virtual bool deleteObject(pwr_tStatus *sts, wb_orep *orep);
  virtual bool deleteFamily(pwr_tStatus *sts, wb_orep *orep);
  virtual bool deleteOset(pwr_tStatus *sts, wb_oset *oset);

  virtual bool renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name &name);


  virtual bool commit(pwr_tStatus *sts);
  virtual bool abort(pwr_tStatus *sts);

  virtual bool writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p);

  virtual void *readAttribute(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p);

  virtual void *readBody(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, void *p);

  virtual bool writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p);

  virtual wb_orep *ancestor(pwr_tStatus *sts, const wb_orep *o);

  virtual wb_orep *parent(pwr_tStatus *sts, const wb_orep *o);

  virtual wb_orep *after(pwr_tStatus *sts, const wb_orep *o);

  virtual wb_orep *before(pwr_tStatus *sts, const wb_orep *o);

  virtual wb_orep *first(pwr_tStatus *sts, const wb_orep *o);

  virtual wb_orep *child(pwr_tStatus *sts, const wb_orep *o, wb_name &name);

  virtual wb_orep *last(pwr_tStatus *sts, const wb_orep *o);

  virtual wb_orep *next(pwr_tStatus *sts, const wb_orep *o);

  virtual wb_orep *previous(pwr_tStatus *sts, const wb_orep *o);

  virtual wb_srep *newSession();

  virtual pwr_tVid vid() const;

  wb_orepdbs *new_wb_orepdbs(size_t size);
  void delete_wb_orepdbs(void *p);



  virtual wb_vrep *next ();
  virtual wb_orep *object (pwr_tStatus *);
  virtual bool isLocal (const wb_orep *);
  virtual pwr_tCid cid () const;
  virtual wb_merep *merep () const;
  virtual bool createSnapshot (const char *);

  virtual bool isCommonMeta() const { return true;}
  virtual bool isMeta() const { return (cid() == pwr_eClass_ClassVolume);}


  virtual bool exportVolume(wb_import &e);
  virtual bool exportHead(wb_import &e);
  virtual bool exportRbody(wb_import &e);
  virtual bool exportDbody(wb_import &e);
  virtual bool exportMeta(wb_import &e);
  virtual bool exportTree(wb_treeimport &i, pwr_tOid oid);
  bool wb_vrepdbs::exportTreeObject(wb_treeimport &i, dbs_sObject *op, bool isRoot);
  virtual bool importTree(bool keepref) { return false;}
  virtual bool importTreeObject(wb_merep *merep, pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, const char *name,
                          size_t rbSize, size_t dbSize, void *rbody, void *dbody)
    { return false;}
  virtual bool importPaste() { return false;}
  virtual bool importPasteObject(pwr_tOid destination, ldh_eDest destcode, 
				 bool keepoid, pwr_tOid oid, 
				 pwr_tCid cid, pwr_tOid poid,
				 pwr_tOid boid, const char *name,
				 size_t rbSize, size_t dbSize, void *rbody, void *dbody)
    { return false;}
  virtual bool accessSupported( ldh_eAccess access) { return access == ldh_eAccess_ReadOnly; }

};


#endif
