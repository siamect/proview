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
#include <map>

class wb_erep;
class wb_merep;
class wb_srep;
class wb_cdef;
class wb_destination;
class wb_dbs;

class wb_vrep
{
protected:
    char m_name[80];
public:    

    virtual ~wb_vrep() {};
    
    virtual void unref() = 0;
    virtual wb_vrep *ref() = 0;

    virtual wb_erep *erep() const = 0;
    
    virtual wb_vrep *next() const = 0;

    virtual wb_orep *object(pwr_tStatus *sts) = 0;
    virtual wb_orep *object(pwr_tStatus *sts, pwr_tOid oid) = 0;
    virtual wb_orep *object(pwr_tStatus *sts, wb_name name) = 0;
    virtual wb_orep *object(pwr_tStatus *sts, wb_orep *parent, wb_name name) = 0;

    virtual wb_orep *createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination d, wb_name name) = 0;

    virtual wb_orep *copyObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d, wb_name name) = 0;
    virtual bool copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination d) = 0;

    virtual bool moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d) = 0;

    virtual bool deleteObject(pwr_tStatus *sts, wb_orep *orep) = 0;
    virtual bool deleteFamily(pwr_tStatus *sts, wb_orep *orep) = 0;
    virtual bool deleteOset(pwr_tStatus *sts, wb_oset *oset) = 0;

    virtual bool renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name name) = 0;


    virtual bool commit(pwr_tStatus *sts) = 0;
    virtual bool abort(pwr_tStatus *sts) = 0;

    virtual bool writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, unsigned int offset, unsigned int size, void *p) = 0;

    virtual void *readAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, unsigned int offset, unsigned int size, void *p) = 0;

    virtual void *readBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p) = 0;

    virtual bool writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p) = 0;

    virtual wb_orep *ancestor(pwr_tStatus *sts, wb_orep *o) = 0;

    virtual wb_orep *parent(pwr_tStatus *sts, wb_orep *o) = 0;

    virtual wb_orep *after(pwr_tStatus *sts, wb_orep *o) = 0;

    virtual wb_orep *before(pwr_tStatus *sts, wb_orep *o) = 0;

    virtual wb_orep *first(pwr_tStatus *sts, wb_orep *o) = 0;

    virtual wb_orep *child(pwr_tStatus *sts, wb_orep *o, wb_name name) = 0;

    virtual wb_orep *last(pwr_tStatus *sts, wb_orep *o) = 0;

    virtual wb_orep *next(pwr_tStatus *sts, wb_orep *o) = 0;

    virtual wb_orep *previous(pwr_tStatus *sts, wb_orep *o) = 0;

    virtual wb_srep *newSession() = 0;

    virtual bool isLocal(wb_orep *o) const = 0;
    virtual pwr_tVid vid() const = 0;
    virtual pwr_tCid cid() const = 0;
    virtual void name( const char *n) { strcpy( m_name, n);}
    virtual const char *name() { return m_name;}
    virtual wb_merep *merep() const = 0;

    virtual bool createSnapshot(const char *fileName) = 0;
    
    virtual void iterObject(wb_dbs *) = 0;
    virtual void iterRbody(wb_dbs *) = 0;
    virtual void iterDbody(wb_dbs *) = 0;
    virtual void objectName(wb_orep *o, char *str) = 0;
    virtual bool isCommonMeta() const { return false;}
    virtual bool isMeta() const { return false;}
};

#endif
