#ifndef wb_orepdbs_h
#define wb_orepdbs_h

#include "wb_orep.h"
#include "wb_vrepdbs.h"
#include "pwr.h"

class wb_orepdbs : public wb_orep
{
    dbs_sObject *m_o;
    wb_vrepdbs *m_vrep;
    unsigned int m_refCount;
    
public:


    wb_orepdbs(int oid);
    wb_orepdbs(dbs_sObject *o);
    ~wb_orepdbs();

    // void* operator new(size_t size, wb_vrepdbs *v);
    // void operator delete(void *p, size_t size, wb_vrepdbs *v);
    
    virtual pwr_tOid oid() const;
    virtual pwr_tVid vid() const;
    virtual pwr_tOix oix() const;

    virtual pwr_tCid cid() const;
    virtual pwr_tOid poid() const;
    virtual pwr_tOid foid() const;
    virtual pwr_tOid loid() const;
    virtual pwr_tOid boid() const;
    virtual pwr_tOid aoid() const;
    
    virtual char * const name();
    virtual pwr_tTime ohTime() const;
    
    virtual bool isOffspringOf(const wb_orep *o) const;
    
    // Navigational operations

    virtual wb_orep *ancestor(pwr_tStatus *sts) const;  //< get object at top of hierarchy
    virtual wb_orep *parent(pwr_tStatus *sts) const;
    virtual wb_orep *after(pwr_tStatus *sts) const;     //< get next sibling
    virtual wb_orep *before(pwr_tStatus *sts) const;    //< get previous sibling
    virtual wb_orep *first(pwr_tStatus *sts) const;     //< get first child
    virtual wb_orep *child(pwr_tStatus *sts, char *name) const;    //< get named child
    virtual wb_orep *last(pwr_tStatus *sts) const;      //< get last child
    virtual wb_orep *next(pwr_tStatus *sts) const;      //< get next in list of objects of same class in one volume
    virtual wb_orep *previous(pwr_tStatus *sts) const;  //< get previous in list of objects of same class in one volume

    virtual wb_position position();
    virtual wb_position position(ldh_eDest dest);

    virtual wb_adrep *attribute(pwr_tStatus*, const char *aname);
    virtual wb_adrep *attribute(pwr_tStatus*);
    
};

#endif
