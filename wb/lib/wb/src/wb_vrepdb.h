#ifndef wb_vrepdb_h
#define wb_vrepdb_h

#include "wb_vrep.h"
#include "wb_orepdb.h"
#include "wb_db.h"
#include "db_cxx.h"

class wb_vrepdb : public wb_vrep
{
private:
  bool deleteFamilyMember(pwr_tOid oid, wb_db_txn *txn);

protected:
  wb_erep *m_erep;
  wb_merep *m_merep;

  unsigned int m_nRef;

  char m_fileName[512];

public:

  wb_db *m_db;
  wb_db_ohead m_ohead;
  
  wb_vrepdb(wb_erep *erep, const char *fileName);
  wb_vrepdb(wb_erep *erep, pwr_tVid, pwr_tCid, const char *volumeName, const char *fileName);

  virtual void unref();
  virtual wb_vrep *ref();

  virtual ldh_eVolRep type() const { return ldh_eVolRep_Db;}
  virtual wb_erep *erep();

  virtual wb_merep *merep() const;
 
  virtual wb_vrep *next();

  virtual pwr_tTime ohTime(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_mClassDef flags(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_tOid oid(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_tVid vid(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_tOix oix(pwr_tStatus *sts, const wb_orep *o);
    
  virtual pwr_tCid cid(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_tOid poid(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_tOid foid(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_tOid loid(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_tOid boid(pwr_tStatus *sts, const wb_orep *o);
  virtual pwr_tOid aoid(pwr_tStatus *sts, const wb_orep *o);
    
  virtual const char * objectName(pwr_tStatus *sts, const wb_orep *o);
  virtual wb_name longName(pwr_tStatus *sts, const wb_orep *o);
    
  virtual bool isOffspringOf(pwr_tStatus *sts, const wb_orep *child, const wb_orep *parent);

  virtual wb_orep *object(pwr_tStatus *sts);
  virtual wb_orep *object(pwr_tStatus *sts, pwr_tOid oid);
  virtual wb_orep *object(pwr_tStatus *sts, pwr_tCid cid) { *sts = LDH__NYI; return 0;}
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


  virtual bool commit(pwr_tStatus *sts) {return m_db->commit(sts);}
  
  virtual bool abort(pwr_tStatus *sts) {return m_db->abort(sts);}

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

  virtual bool isLocal(const wb_orep *o);

  virtual bool createSnapshot(const char *fileName);

  virtual void objectName(const wb_orep *o, char *str);

  void objectName(pwr_tOid oid, char *name, int level);

  void load();

  virtual bool exportVolume(wb_import &e);
    
  virtual bool exportHead(wb_import &e);

  virtual bool exportRbody(wb_import &e);
    
  virtual bool exportDbody(wb_import &e);
    
  virtual bool exportDocBlock(wb_import &e);
    
  virtual bool exportMeta(wb_import &e);

  virtual bool exportTree(wb_treeimport &i, pwr_tOid oid) { return false;}
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

#if 0
  int del_family(DbTxn *txn, Dbc *cp, pwr_tOid poid);
#endif
  void unadopt(wb_db_txn *txn, wb_db_ohead &o);
  void adopt(wb_db_txn *txn, wb_db_ohead &o, wb_destination &dest);

  wb_orepdb *new_wb_orepdb(size_t size);
  void delete_wb_orepdb(void *p);
  virtual bool accessSupported( ldh_eAccess access) { return true;}

};

#endif
