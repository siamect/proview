#ifndef wb_vrepdb_h
#define wb_vrepdb_h

#include "wb_vrep.h"
#include "wb_orepdb.h"
#include "wb_db.h"
#include "db_cxx.h"

class wb_vrepdb : public wb_vrep
{
protected:
  pwr_tVid m_vid;
  map<int, wb_srep*> m_srep;
  //wb_session m_wsession;

  wb_erep *m_erep;
  unsigned int m_nSession;
  unsigned int m_nRef;

#if 0
  DbEnv *m_dbenv;
  Db *m_t_ohead;
  Db *m_t_obody;
  Db *m_t_class;
  Db *m_t_name;
  Db *m_t_info;

  DbTxn *m_txn;    
    
  class db_ohead
  {
  public:
    db_sObject m_o;
    Dbt m_key;
    Dbt m_data;
    Db *m_t_ohead;

    db_ohead(Db *t_ohead);
    db_ohead(Db *t_ohead, pwr_tOid oid);
    db_ohead(Db *t_ohead, DbTxn *txn, pwr_tOid oid);
    db_ohead(Db *t_ohead, DbTxn *txn, wb_orep *orp);

    db_ohead &get(DbTxn *txn);
    db_ohead &get(DbTxn *txn, wb_orep *o);
    db_ohead &get(DbTxn *txn, pwr_tOid oid);

    void put(DbTxn *txn);
    void del(DbTxn *txn);
        
    pwr_tOid oid() { return m_o.oid;}
    pwr_tCid cid() { return m_o.cid;}
    pwr_tOid poid() { return m_o.poid;}
    pwr_tOid foid() { return m_o.foid;}
    pwr_tOid loid() { return m_o.loid;}
    pwr_tOid boid() { return m_o.boid;}
    pwr_tOid aoid() { return m_o.aoid;}

    char *name();
    void name(wb_name &name);
        
    void poid(pwr_tOid oid) { m_o.poid = oid;}
    void foid(pwr_tOid oid) { m_o.foid = oid;}
    void loid(pwr_tOid oid) { m_o.loid = oid;}
    void boid(pwr_tOid oid) { m_o.boid = oid;}
    void aoid(pwr_tOid oid) { m_o.aoid = oid;}

    void clear();
  };
    
  class db_name
  {
  public:
    struct
    {
      pwr_tOid     poid;
      pwr_tObjName normname;
    } m_k;
    struct
    {
      pwr_tOid      oid;
      //pwr_tCid      cid;   // saved here to optimize tree traversal
      //pwr_mClassDef flags; // saved here to optimize tree traversal
    } m_d;
        
    Dbt m_key;
    Dbt m_data;

    Db *m_t_name;
        
        
    db_name(Db *t_name, DbTxn *txn);
    db_name(Db *t_name, db_ohead &o);
    db_name(Db *t_name, pwr_tOid, char *name);
    db_name(Db *t_name, DbTxn *txn, pwr_tOid poid, const char *name);
    db_name(Db *t_name, DbTxn *txn, pwr_tOid poid, wb_name name);
        
    void get(DbTxn *txn);
    void put(DbTxn *txn);
    void del(DbTxn *txn);
        
    void name(wb_name &name);
        
    pwr_tOid oid() { return m_d.oid;}
  };

  class db_class
  {
  public:
    struct 
    {
      pwr_tCid cid;
      pwr_tOid oid;
    } m_k;

    Dbt m_key;
    Db *m_t_class;
    Dbc *m_dbc;
        
    db_class(Db *t_class, pwr_tCid cid);
    db_class(Db *t_class, db_ohead &o);
    db_class(Db *t_class, DbTxn *txn, pwr_tCid cid);
    ~db_class();

    bool succ(DbTxn *txn, pwr_tOid oid);
    bool pred(DbTxn *txn, pwr_tOid oid);
    void put(DbTxn *txn);
    void del(DbTxn *txn);
        
    pwr_tCid cid() { return m_k.cid;}
    pwr_tOid oid() { return m_k.oid;}        
                
  };    
    
  wb_db::db_ohead m_ohead;
    
  pwr_tOix wb_vrepdb::new_oix(DbTxn *txn);
  int wb_vrepdb::del_family(DbTxn *txn, Dbc *cp, pwr_tOix poix);
#endif

public:

  wb_db *m_db;
  wb_db_txn *m_txn;    
  wb_db_ohead m_ohead;
  
  wb_vrepdb();
  ~wb_vrepdb();  

  virtual void unref();
  virtual wb_vrep *ref();

  virtual wb_erep *erep();


 
  pwr_tTime ohTime(pwr_tStatus *sts, const wb_orep * const o);
  pwr_tOid oid(pwr_tStatus *sts, const wb_orep * const o);
  pwr_tVid vid(pwr_tStatus *sts, const wb_orep * const o);
  pwr_tOix oix(pwr_tStatus *sts, const wb_orep * const o);
    
  pwr_tCid cid(pwr_tStatus *sts, const wb_orep * const o);
  pwr_tOid poid(pwr_tStatus *sts, const wb_orep * const o);
  pwr_tOid foid(pwr_tStatus *sts, const wb_orep * const o);
  pwr_tOid loid(pwr_tStatus *sts, const wb_orep * const o);
  pwr_tOid boid(pwr_tStatus *sts, const wb_orep * const o);
  pwr_tOid aoid(pwr_tStatus *sts, const wb_orep * const o);
    
  virtual const char * objectName(pwr_tStatus *sts, wb_orep *o) const;
  virtual wb_name longName(pwr_tStatus *sts, wb_orep *o);
  virtual pwr_tTime ohTime(pwr_tStatus *sts, wb_orep *o) const;
    
  virtual bool isOffspringOf(pwr_tStatus *sts, const wb_orep *child, const wb_orep *parent) const;

  virtual wb_orep *object(pwr_tStatus *sts);
  virtual wb_orep *object(pwr_tStatus *sts, pwr_tOid oid);
  virtual wb_orep *object(pwr_tStatus *sts, const char *name);
  virtual wb_orep *object(pwr_tStatus *sts, wb_orep *parent, wb_name name);

  virtual wb_orep *createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination d, wb_name name);

  virtual wb_orep *copyObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d, wb_name name);
  virtual bool copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination d);

  virtual bool moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d);

  virtual bool deleteObject(pwr_tStatus *sts, wb_orep *orep);
  virtual bool deleteFamily(pwr_tStatus *sts, wb_orep *orep);
  virtual bool deleteOset(pwr_tStatus *sts, wb_oset *oset);

  virtual bool renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name name);


  virtual bool commit(pwr_tStatus *sts);
  virtual bool abort(pwr_tStatus *sts);

  virtual bool writeAttribute(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, unsigned int offset, unsigned int size, void *p);

  virtual void *readAttribute(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, unsigned int offset, unsigned int size, void *p);

  virtual void *readBody(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, void *p);

  virtual bool writeBody(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, void *p);

  virtual wb_orep *ancestor(pwr_tStatus *sts, wb_orep *o);

  virtual wb_orep *parent(pwr_tStatus *sts, wb_orep *o);

  virtual wb_orep *after(pwr_tStatus *sts, wb_orep *o);

  virtual wb_orep *before(pwr_tStatus *sts, wb_orep *o);

  virtual wb_orep *first(pwr_tStatus *sts, wb_orep *o);

  virtual wb_orep *child(pwr_tStatus *sts, wb_orep *o, const char *name);

  virtual wb_orep *last(pwr_tStatus *sts, wb_orep *o);

  virtual wb_orep *next(pwr_tStatus *sts, wb_orep *o);

  virtual wb_orep *previous(pwr_tStatus *sts, wb_orep *o);

  virtual wb_srep *newSession();

  virtual bool isLocal(wb_orep *o) const;
  virtual pwr_tVid vid() const;
  virtual void objectName(wb_orep *o, const char *str);

  void load(char *name);

  virtual bool exportVolume(wb_import &e);
    
  virtual bool exportHead(wb_import &e);

  virtual bool exportRbody(wb_import &e);
    
  virtual bool exportDbody(wb_import &e);
    
  virtual bool exportMeta(wb_import &e);

  //pwr_tOid new_oid(DbTxn *txn);
#if 0
  int del_family(DbTxn *txn, Dbc *cp, pwr_tOid poid);
#endif
  void unadopt(wb_db_txn *txn, wb_db_ohead &o);
  void adopt(wb_db_txn *txn, wb_db_ohead &o, wb_destination &dest);
};

#endif
