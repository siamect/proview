#ifndef wb_orep_h
#define wb_orep_h

#include "pwr.h"
#include "wb_orep.h"
#include "wb_vrep.h"
#include "wb_ldh.h"
#include "wb_position.h"
#include "wb_adrep.h"

class wb_vrep;
class wb_erep;

class wb_orep
{

protected:
    wb_vrep *m_vrep;
    int m_nRef;

public:

    //virtual wb_orep(int oid) = 0;
    virtual ~wb_orep() {}
    
    void unref();
    wb_orep *ref();
    
    virtual pwr_tOid oid() const = 0;
    virtual pwr_tVid vid() const = 0;
    virtual pwr_tOix oix() const = 0;

    virtual pwr_tCid cid() const = 0;
    virtual pwr_tOid poid() const = 0;
    virtual pwr_tOid foid() const = 0;
    virtual pwr_tOid loid() const = 0;
    virtual pwr_tOid boid() const = 0;
    virtual pwr_tOid aoid() const = 0;
    
    virtual char * const name() = 0;
    virtual pwr_tTime ohTime() const = 0;
    
    virtual bool isOffspringOf(const wb_orep *o) const = 0;
    
    // Navigational operations

    virtual wb_orep *ancestor(pwr_tStatus *sts) const = 0;  //< get object at top of hierarchy
    virtual wb_orep *parent(pwr_tStatus *sts) const = 0;
    virtual wb_orep *after(pwr_tStatus *sts) const = 0;     //< get next sibling
    virtual wb_orep *before(pwr_tStatus *sts) const = 0;    //< get previous sibling
    virtual wb_orep *first(pwr_tStatus *sts) const = 0;     //< get first child
    virtual wb_orep *child(pwr_tStatus *sts, char *name) const = 0;    //< get named child
    virtual wb_orep *last(pwr_tStatus *sts) const = 0;      //< get last child
    virtual wb_orep *next(pwr_tStatus *sts) const = 0;      //< get next in list of objects of same class in one volume
    virtual wb_orep *previous(pwr_tStatus *sts) const = 0;  //< get previous in list of objects of same class in one volume

    virtual wb_position position() = 0;
    virtual wb_position position(ldh_eDest dest) = 0;

    virtual wb_adrep *attribute(pwr_tStatus*, const char *aname) = 0;
    virtual wb_adrep *attribute(pwr_tStatus*) = 0;

    virtual wb_erep *erep() const = 0;
    virtual wb_vrep *vrep() const = 0;
};

#endif
