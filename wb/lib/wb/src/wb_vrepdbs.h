#ifndef wb_vrepdbs_h
#define wb_vrepdbs_h

#include "co_dbs.h"
#include "wb_vrep.h"
#include "wb_cdef.h"

class wb_vrepdbs : public wb_vrep
{
public:
    pwr_tVid m_vid;
    map<int, wb_srep*> m_srep;
    //wb_session m_wsession;

    wb_erep *m_erep;
    unsigned int m_nSession;
    unsigned int m_nRef;

    char m_fileName[200];
    bool m_isDbsenvLoaded;
    
    dbs_sEnv m_dbsenv;
    
    wb_vrepdbs(const char *fileName);
    dbs_sEnv *dbsenv();
    bool load();
    
    virtual void unref();
    virtual wb_vrep *ref();

    virtual wb_erep *erep() const;

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

    virtual wb_orep *ancestor(pwr_tStatus *sts, wb_orep *o);

    virtual wb_orep *parent(pwr_tStatus *sts, wb_orep *o);

    virtual wb_orep *after(pwr_tStatus *sts, wb_orep *o);

    virtual wb_orep *before(pwr_tStatus *sts, wb_orep *o);

    virtual wb_orep *first(pwr_tStatus *sts, wb_orep *o);

    virtual wb_orep *child(pwr_tStatus *sts, wb_orep *o, char *name);

    virtual wb_orep *last(pwr_tStatus *sts, wb_orep *o);

    virtual wb_orep *next(pwr_tStatus *sts, wb_orep *o);

    virtual wb_orep *previous(pwr_tStatus *sts, wb_orep *o);

    virtual wb_srep *newSession();

    virtual pwr_tVid vid() const;

    wb_orepdbs *new_wb_orepdbs(size_t size);
    void delete_wb_orepdbs(void *p);



    virtual wb_vrep *next () const;
    virtual wb_orep *object (pwr_tStatus *);
    virtual bool isLocal (wb_orep *) const;
    virtual pwr_tCid cid () const;
    virtual wb_merep *merep () const;
    virtual bool createSnapshot (char *);
    virtual void iterObject (wb_dbs *);
    virtual void iterRbody (wb_dbs *);
    virtual void iterDbody (wb_dbs *);
};


#endif
