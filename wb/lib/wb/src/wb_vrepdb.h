#ifndef wb_vrepdb_h
#define wb_vrepdb_h

#include "wb_vrep.h"
#include "wb_orepdb.h"
#include "db_cxx.h"

class wb_vrepdb : public wb_vrep , public DbEnv
{
protected:
    pwr_tVid m_vid;
    map<int, wb_srep*> m_srep;
    //wb_session m_wsession;

    wb_erep *m_erep;
    unsigned int m_nSession;
    unsigned int m_nRef;

    Db *m_db_ohead;
    Db *m_db_obody;
    Db *m_db_clist;
    Db *m_db_name;
    Db *m_db_info;

    DbTxn *m_txn;
    
    class oh_k : public Dbt
    {
        pwr_tOix m_oix;
    
    public:

        oh_k();
        oh_k(wb_orepdb *o);
        oh_k(pwr_tOix Oix);
    
    };

    class oh_d : public Dbt
    {
        wb_orepdb *m_orep;

    public:
    
        oh_d();
        oh_d(wb_orepdb *o);
        oh_d(pwr_tOix Oix);
    
    };

    class ob_k : public Dbt
    {
    public:

        struct {
            pwr_tOix oix;
            pwr_tOix bix;
        } m_data;
    

        ob_k();
        ob_k(wb_orepdb *o);
        ob_k(wb_orepdb *o, pwr_tOix bix);
        ob_k(pwr_tOix oix);
        ob_k(pwr_tOix oix, pwr_tOix bix);
    
    };

    class na_k : public Dbt
    {
    public:

        struct {
            pwr_tOix poix;
            //pwr_tName name;
            char name[32];
        } m_key;

        na_k();
        na_k(wb_orepdb *o);
        na_k(wb_orepdb *o, char *name);
        na_k(pwr_tOix poix);
        na_k(pwr_tOix poix, char *name);
        na_k(pwr_tOix poix, wb_name name);
    
    };

    class na_d : public Dbt
    {
    public:
        pwr_tOix m_oix;

        na_d();
        na_d(wb_orepdb *o);
        na_d(pwr_tOix oix);
    
        pwr_tOix oix();
    };

#if 0
    int wb_vrepdb::get_ohead(DbTxn *txn, wb_orepdb *o);
    int wb_vrepdb::get_ohead(wb_orep **orep, DbTxn *txn, pwr_tOid oid);
    int wb_vrepdb::get_ohead(wb_orep **orep, DbTxn *txn, wb_orepdb *o, char *name);

    int wb_vrepdb::put_ohead(DbTxn *txn, wb_orepdb *o, u_int32_t flags);

    int wb_vrepdb::del_ohead(DbTxn *txn, wb_orepdb *o);

    int wb_vrepdb::get_clist(DbTxn *txn, wb_orepdb *o);
    int wb_vrepdb::put_clist(DbTxn *txn, wb_orepdb *o, u_int32_t flags);
    int wb_vrepdb::del_clist(DbTxn *txn, wb_orepdb *o);

    int wb_vrepdb::get_name(DbTxn *txn, wb_orepdb *o);
    int wb_vrepdb::put_name(DbTxn *txn, wb_orepdb *o, u_int32_t flags);
    int wb_vrepdb::del_name(DbTxn *txn, wb_orepdb *o);

    pwr_tOix wb_vrepdb::new_oix(DbTxn *txn);
    int wb_vrepdb::del_family(DbTxn *txn, Dbc *cp, pwr_tOix poix);
#endif

public:
    
    wb_vrepdb();
    ~wb_vrepdb();  

    virtual void unref();
    virtual wb_vrep *ref();

    virtual wb_erep *erep();

    virtual wb_orep *object(pwr_tStatus *sts, pwr_tOid oid);
    virtual wb_orep *object(pwr_tStatus *sts, char *name);
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

    virtual bool writeAttribute();

    virtual bool readAttribute(wb_orep *o, pwr_tOix bix, unsigned int offset, unsigned int size);

    virtual bool readBody();

    virtual bool writeBody();

    virtual wb_orep *ancestor(pwr_tStatus *sts, wb_orep *o) const;

    virtual wb_orep *parent(pwr_tStatus *sts, wb_orep *o) const;

    virtual wb_orep *after(pwr_tStatus *sts, wb_orep *o) const;

    virtual wb_orep *before(pwr_tStatus *sts, wb_orep *o) const;

    virtual wb_orep *first(pwr_tStatus *sts, wb_orep *o) const;

    virtual wb_orep *child(pwr_tStatus *sts, wb_orep *o, char *name) const;

    virtual wb_orep *last(pwr_tStatus *sts, wb_orep *o) const;

    virtual wb_orep *next(pwr_tStatus *sts, wb_orep *o) const;

    virtual wb_orep *previous(pwr_tStatus *sts, wb_orep *o) const;

    virtual wb_srep *newSession();

    virtual bool isLocal(wb_orep *o) const;
    virtual pwr_tVid vid() const;

};

#endif
